/* SPDX-License-Identifier: GPL-3.0-only */
#include "PluginProcessor.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include <juce_audio_utils/juce_audio_utils.h>

#include "PluginEditor.h"

namespace {

constexpr double kCoreRate = 48000.0;
constexpr int kControlBlock = 32;
constexpr int kStateVersion = 3;
constexpr int kUiPageLayout = 2;
constexpr float kIntToFloat = 1.0f / 32768.0f;

bool isEngineMessage(const juce::MidiMessage& message)
{
    if (message.getRawDataSize() < 3)
        return false;
    return message.isNoteOnOrOff() || message.isController() ||
           message.isPitchWheel();
}

} // namespace

extern "C" int midi_get(uint8_t* status, uint8_t* data1, uint8_t* data2);

extern "C" int midi_get(uint8_t*, uint8_t*, uint8_t*)
{
    return 0; /* the plugin feeds MidiBridge directly */
}

ScopeRing::ScopeRing() = default;

void ScopeRing::push(const float* data, int count)
{
    count = juce::jlimit(0, kCapacity, count);
    int start1 = 0, size1 = 0, start2 = 0, size2 = 0;
    fifo_.prepareToWrite(count, start1, size1, start2, size2);
    const int total = size1 + size2;
    if (total <= 0)
        return;
    std::copy(data, data + size1, buffer_.begin() + start1);
    std::copy(data + size1, data + total, buffer_.begin() + start2);
    fifo_.finishedWrite(total);
}

void ScopeRing::read(float* destination, int count)
{
    count = juce::jlimit(0, kCapacity, count);
    const int available = fifo_.getNumReady();
    int start1 = 0, size1 = 0, start2 = 0, size2 = 0;
    fifo_.prepareToRead(available, start1, size1, start2, size2);
    const int total = size1 + size2;
    if (total > 0) {
        std::copy(buffer_.begin() + start1, buffer_.begin() + start1 + size1,
                  scratch_.begin());
        std::copy(buffer_.begin() + start2, buffer_.begin() + start2 + size2,
                  scratch_.begin() + size1);
        fifo_.finishedRead(total);
    }
    for (int i = 0; i < count; ++i) {
        const int source = total - count + i;
        destination[i] = source >= 0 ? scratch_[static_cast<size_t>(source)] : 0.0f;
    }
}

juce::String TypePAlphaAudioProcessor::getPartName(int part) const
{
    return partNames_[static_cast<size_t>(juce::jlimit(0, 1, part))];
}

void TypePAlphaAudioProcessor::setPartName(int part, const juce::String& name)
{
    partNames_[static_cast<size_t>(juce::jlimit(0, 1, part))] = name;
}

void TypePAlphaAudioProcessor::readOutputScope(float* destination, int count)
{
    outputScope_.read(destination, count);
}

void TypePAlphaAudioProcessor::readTapScope(int channel, float* destination, int count)
{
    if (channel < 0 || channel >= static_cast<int>(tapScopes_.size())) {
        std::fill(destination, destination + count, 0.0f);
        return;
    }
    tapScopes_[static_cast<size_t>(channel)].read(destination, count);
}

TypePAlphaAudioProcessor::TypePAlphaAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput(
          "Output", juce::AudioChannelSet::stereo(), true)),
      midi_(engine_),
      apvts_(*this, nullptr, P1_STATE_TREE_ID, createLayout())
{
    lastParams_.fill(std::numeric_limits<int32_t>::min());
    for (int i = 0; i < kP1ParamCount; ++i)
        paramValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1ParamSpecs[static_cast<std::size_t>(i)].id);
}

juce::AudioProcessorValueTreeState::ParameterLayout
TypePAlphaAudioProcessor::createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for (const auto& spec : kP1ParamSpecs) {
        const int low = spec.min;
        const int high = spec.max > spec.min ? spec.max : spec.min + 1;
        const int def = juce::jlimit(low, high, spec.def);
        layout.add(std::make_unique<juce::AudioParameterInt>(
            juce::ParameterID{spec.id, 1}, spec.name, low, high, def));
    }
    return layout;
}

void TypePAlphaAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    hostRate_ = sampleRate;
    engine_.reset();
    midi_.reset();
    rateBridge_.reset();
    directBuffer_.assign(static_cast<std::size_t>(kControlBlock) * 2u, 0);
    if (std::abs(sampleRate - kCoreRate) > 0.5) {
        rateBridge_.prepare(sampleRate, samplesPerBlock);
        setLatencySamples(rateBridge_.latencySamples());
    } else {
        setLatencySamples(0);
    }
    lastParams_.fill(std::numeric_limits<int32_t>::min());
    paramsDirty_.store(false, std::memory_order_relaxed);
    syncParams();
}

void TypePAlphaAudioProcessor::releaseResources()
{
    engine_.allNotesOff();
}

bool TypePAlphaAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    if (layouts.getMainInputChannelSet() != juce::AudioChannelSet::disabled())
        return false;
    const auto out = layouts.getMainOutputChannelSet();
    return out == juce::AudioChannelSet::stereo() ||
           out == juce::AudioChannelSet::mono();
}

void TypePAlphaAudioProcessor::syncParams()
{
    for (int i = 0; i < kP1ParamCount; ++i) {
        const auto* value = paramValues_[static_cast<std::size_t>(i)];
        if (value == nullptr)
            continue;
        const auto next = static_cast<int32_t>(std::lround(value->load()));
        const int32_t clamped = juce::jlimit(
            static_cast<int32_t>(kP1ParamSpecs[static_cast<std::size_t>(i)].min),
            static_cast<int32_t>(kP1ParamSpecs[static_cast<std::size_t>(i)].max),
            next);
        if (clamped != lastParams_[static_cast<std::size_t>(i)]) {
            engine_.setParam(
                static_cast<uint32_t>(
                    kP1ParamSpecs[static_cast<std::size_t>(i)].engineId),
                clamped);
            lastParams_[static_cast<std::size_t>(i)] = clamped;
        }
    }
}

void TypePAlphaAudioProcessor::feedMessage(p1::MidiBridge& midi,
                                         const juce::MidiMessage& message)
{
    const auto* data = message.getRawData();
    midi.feed(data[0], data[1], data[2]);
}

void TypePAlphaAudioProcessor::renderCore(int32_t* out, int frames)
{
    engine_.render(out, static_cast<uint32_t>(frames));
}

void TypePAlphaAudioProcessor::pushTapScopes(p1::Engine& engine, int frames)
{
    const int count = std::min(frames, 32);
    if (count <= 0)
        return;
    float tapData[32] = {};
    for (int channel = 0; channel < static_cast<int>(tapScopes_.size());
         ++channel) {
        const int32_t* tap = engine.tap(static_cast<uint32_t>(channel));
        for (int i = 0; i < count; ++i)
            tapData[i] = static_cast<float>(tap[i]) * kIntToFloat;
        tapScopes_[static_cast<std::size_t>(channel)].push(tapData, count);
    }
}

void TypePAlphaAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                          juce::MidiBuffer& midi)
{
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();

    const int frames = buffer.getNumSamples();
    if (frames == 0)
        return;

    if (paramsDirty_.exchange(false, std::memory_order_relaxed))
        lastParams_.fill(std::numeric_limits<int32_t>::min());

    const int channels = buffer.getNumChannels();
    if (channels == 0) {
        midi.clear();
        return;
    }
    if (std::abs(hostRate_ - kCoreRate) < 0.5) {
        float* left = buffer.getWritePointer(0);
        float* right = channels > 1 ? buffer.getWritePointer(1) : nullptr;
        auto event = midi.begin();
        int pos = 0;
        while (pos < frames) {
            const int length = std::min(kControlBlock, frames - pos);
            const int end = pos + length;

            while (event != midi.end() && (*event).samplePosition < end) {
                const auto message = (*event).getMessage();
                if (isEngineMessage(message))
                    feedMessage(midi_, message);
                ++event;
            }

            syncParams();
            renderCore(directBuffer_.data(), length);
            for (int i = 0; i < length; ++i) {
                const auto index = static_cast<std::size_t>(i) * 2u;
                left[pos + i] = static_cast<float>(directBuffer_[index]) * kIntToFloat;
                if (right != nullptr)
                    right[pos + i] =
                        static_cast<float>(directBuffer_[index + 1u]) * kIntToFloat;
            }
            outputScope_.push(left + pos, length);
            pushTapScopes(engine_, length);
            pos = end;
        }

        while (event != midi.end()) {
            const auto message = (*event).getMessage();
            if (isEngineMessage(message))
                feedMessage(midi_, message);
            ++event;
        }
    } else {
        for (const auto metadata : midi) {
            const auto message = metadata.getMessage();
            if (isEngineMessage(message))
                feedMessage(midi_, message);
        }
        syncParams();
        float* left = buffer.getWritePointer(0);
        float* right = channels > 1 ? buffer.getWritePointer(1) : left;
        rateBridge_.process(
            engine_, left, right, frames,
            [](void* context, p1::Engine& engine, int count) {
                static_cast<TypePAlphaAudioProcessor*>(context)->pushTapScopes(
                    engine, count);
            },
            this);
        outputScope_.push(left, frames);
    }

    midi.clear();
}

juce::File TypePAlphaAudioProcessor::libraryFile() const
{
    if (libraryFileOverride_ != juce::File{})
        return libraryFileOverride_;
    return juce::File::getSpecialLocation(
               juce::File::userApplicationDataDirectory)
        .getChildFile("P1Onken")
        .getChildFile("Type P.alpha")
        .getChildFile("library.p1lib");
}

void TypePAlphaAudioProcessor::ensureLibraryLoaded()
{
    if (libraryLoaded_)
        return;
    libraryLoaded_ = true;
    const auto file = libraryFile();
    if (!file.existsAsFile())
        return;
    juce::MemoryBlock data;
    if (!file.loadFileAsData(data))
        return;
    const int currentMulti = presets_.currentMulti();
    const int source0 = presets_.partSource(0);
    const int source1 = presets_.partSource(1);
    if (presets_.loadLibrary(static_cast<const uint8_t*>(data.getData()),
                             data.getSize())) {
        presets_.setCurrentMulti(currentMulti);
        presets_.setPartSource(0, source0);
        presets_.setPartSource(1, source1);
    }
}

void TypePAlphaAudioProcessor::saveLibrary()
{
    const auto file = libraryFile();
    file.getParentDirectory().createDirectory();
    const auto bytes = presets_.packLibrary();
    if (!file.replaceWithData(bytes.data(), bytes.size())) {
        DBG("could not save the plugin library");
    }
}

juce::AudioProcessorEditor* TypePAlphaAudioProcessor::createEditor()
{
    ensureLibraryLoaded();
    return new PluginEditor(*this);
}

void TypePAlphaAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts_.copyState();
    state.setProperty("stateVersion", kStateVersion, nullptr);
    state.setProperty("patchName", patchName_, nullptr);
    state.setProperty("part1Name", partNames_[0], nullptr);
    state.setProperty("part2Name", partNames_[1], nullptr);
    state.setProperty("uiActivePart", uiActivePart_, nullptr);
    state.setProperty("uiPage", uiPage_, nullptr);
    state.setProperty("uiPageLayout", kUiPageLayout, nullptr);
    state.setProperty("currentMulti", presets_.currentMulti(), nullptr);
    for (int part = 0; part < kP1PartCount; ++part) {
        state.setProperty("partSource" + juce::String(part),
                          presets_.partSource(part), nullptr);
    }
    const auto partBank = presets_.packPartBank();
    const auto multiBank = presets_.packMultiBank();
    state.setProperty(
        "partBank",
        juce::MemoryBlock(partBank.data(), partBank.size()).toBase64Encoding(),
        nullptr);
    state.setProperty(
        "multiBank",
        juce::MemoryBlock(multiBank.data(), multiBank.size()).toBase64Encoding(),
        nullptr);
    if (auto xml = state.createXml())
        copyXmlToBinary(*xml, destData);
}

void TypePAlphaAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml == nullptr)
        return;
    auto state = juce::ValueTree::fromXml(*xml);
    if (!state.isValid() || !state.hasType(apvts_.state.getType()))
        return;
    patchName_ = state.getProperty("patchName", "init patch").toString();
    partNames_[0] = state.getProperty("part1Name", "init").toString();
    partNames_[1] = state.getProperty("part2Name", "init").toString();
    uiActivePart_ = juce::jlimit(0, 1, static_cast<int>(state.getProperty("uiActivePart", 0)));
    int page = static_cast<int>(state.getProperty("uiPage", 0));
    const int pageLayout =
        static_cast<int>(state.getProperty("uiPageLayout", 0));
    if (pageLayout < kUiPageLayout) {
        /* The four-tab layout had operators at 1, operator lfos at 2 and
         * fx/mod at 3. The six-tab layout moves fx/mod to 1. */
        if (page == 1)
            page = 2;
        else if (page == 2)
            page = 3;
        else if (page == 3)
            page = 1;
    }
    uiPage_ = juce::jlimit(0, 5, page);
    presets_.setCurrentMulti(static_cast<int>(state.getProperty("currentMulti", -1)));
    for (int part = 0; part < kP1PartCount; ++part) {
        presets_.setPartSource(
            part, static_cast<int>(
                      state.getProperty("partSource" + juce::String(part), -1)));
    }
    juce::MemoryBlock partBank;
    partBank.fromBase64Encoding(state.getProperty("partBank", "").toString());
    if (partBank.getSize() > 0)
        presets_.loadPartBank(static_cast<const uint8_t*>(partBank.getData()),
                              partBank.getSize());
    juce::MemoryBlock multiBank;
    multiBank.fromBase64Encoding(state.getProperty("multiBank", "").toString());
    if (multiBank.getSize() > 0)
        presets_.loadMultiBank(static_cast<const uint8_t*>(multiBank.getData()),
                               multiBank.getSize());
    apvts_.replaceState(state);
    paramsDirty_.store(true, std::memory_order_relaxed);
    updateHostDisplay(juce::AudioProcessor::ChangeDetails().withProgramChanged(true));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TypePAlphaAudioProcessor();
}
