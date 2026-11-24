/* SPDX-License-Identifier: GPL-3.0-only */
#include "PluginProcessor.h"

#include <algorithm>
#include <cmath>
#include <limits>

#include <juce_audio_utils/juce_audio_utils.h>

#include "CutoffLaw.h"
#include "MacroLaw.h"
#include "MacroTargets.h"
#include "PluginEditor.h"
#include "VolumeLaw.h"
#include "Widgets.h"
#include "macro_target_ids.h"

namespace {

constexpr double kCoreRate = 48000.0;
constexpr int kControlBlock = 32;
constexpr int kStateVersion = 14;
/* The state version whose cutoff values already use the current law. A later
 * version bump must not run the cutoff conversion again. */
constexpr int kCutoffLawStateVersion = 5;
/* Layout 5 inserts the macros page at 1, so every later page moves up one. */
constexpr int kUiPageLayout = 5;
/* Layout 4 inserted op phasemap at 4, which moved the three spectral pages. */
constexpr int kPhaseMapPageLayout = 4;
constexpr float kIntToFloat = 1.0f / 32768.0f;

bool isEngineMessage(const juce::MidiMessage& message)
{
    if (message.getRawDataSize() < 3)
        return false;
    return message.isNoteOnOrOff() || message.isController() ||
           message.isPitchWheel();
}

/* The engine applies the velocity source itself for the operator level, feedback,
 * morph, envelope rate, LFO rate and LFO depth groups, on the all-operator and the
 * carrier and modulator rows. The phase envelope and phase LFO and the part-level
 * pitch and filter envelopes stay in the plugin's part-level layer. */
bool engineHandlesVelocityTarget(int target, int memberId)
{
    if (target <= 0 || target > p1::macros::kTargetCount)
        return false;
    const juce::String name(
        p1::macros::kTargets[static_cast<std::size_t>(target - 1)].name);
    if (name.contains(" levels") || name.contains(" feedbacks") ||
        name.contains(" morphs"))
        return true;
    if (name.contains(" env rates"))
        return p1::macros::isEnvRateMember(memberId);
    if (name.contains(" lfo rates"))
        return p1::macros::isLfoRateMember(memberId);
    if (name.contains(" lfo depths"))
        return p1::macros::isLfoDepthMember(memberId);
    return false;
}

/* The part-block indices of the four operator warm-up rates. The migration
 * needs them because the operator envelope grew from seven to eight parameters
 * inside each operator block. */
std::array<int, 4> operatorEnvelopeIndices()
{
    std::array<int, 4> out{{-1, -1, -1, -1}};
    for (int op = 0; op < 4; ++op) {
        const std::string wanted =
            "op" + std::to_string(op + 1) + ".eg.warm.p0";
        for (int i = 0; i < kP1PartParamCount; ++i) {
            if (wanted == kP1ParamSpecs[static_cast<std::size_t>(i)].id) {
                out[static_cast<std::size_t>(op)] = i;
                break;
            }
        }
    }
    return out;
}

/* The part-block index of a `.p0` parameter, or -1. The preset migration needs
 * these for the remaps that cannot be expressed as an append. */
int partParamIndex(const std::string& wanted)
{
    for (int i = 0; i < kP1PartParamCount; ++i) {
        if (wanted == kP1ParamSpecs[static_cast<std::size_t>(i)].id)
            return i;
    }
    return -1;
}

/* The part-block index of the pan envelope, for the version 11 remap. */
int panEnvelopeIndex() { return partParamIndex("pan_env_on.p0"); }

/* The part-block indices of each operator feedback envelope, for the version
 * 12 remap. */
std::array<int, 4> feedbackEnvelopeIndices()
{
    std::array<int, 4> out{{-1, -1, -1, -1}};
    for (int op = 0; op < 4; ++op)
        out[static_cast<std::size_t>(op)] = partParamIndex(
            "op" + std::to_string(op + 1) + "_fbenv_on.p0");
    return out;
}

/* Every PresetStore that reads an old library needs the same block indices for
 * the remaps. The in-memory store used to get these, whilst the temporary disk
 * store in saveLibrary did not, so an old record was migrated with -1 bases and
 * then written back corrupted. Configure both through this one function. */
void configurePresetStore(p1::PresetStore& store)
{
    store.setOperatorEnvelopeIndices(operatorEnvelopeIndices());
    store.setPanEnvelopeIndex(panEnvelopeIndex());
    store.setOffsetEnvelopeIndices(feedbackEnvelopeIndices(),
                                   partParamIndex("penv_on.p0"));
    store.setAbsEnvelopeIndices(
        partParamIndex("fenv_on.p0"),
        {partParamIndex("op1_spec_env_on.p0"),
         partParamIndex("op2_spec_env_on.p0"),
         partParamIndex("op3_spec_env_on.p0"),
         partParamIndex("op4_spec_env_on.p0")});
    std::array<int, 34> fadeIndices{};
    int fadeCount = 0;
    for (int op = 1; op <= 4; ++op) {
        for (const char* family : {"trem", "fblfo", "dlfo", "spec_lfo"}) {
            const auto prefix = "op" + std::to_string(op) + "_" + family;
            fadeIndices[static_cast<std::size_t>(fadeCount++)] =
                partParamIndex(prefix + "_rate_fade.p0");
            fadeIndices[static_cast<std::size_t>(fadeCount++)] =
                partParamIndex(prefix + "_depth_fade.p0");
        }
    }
    fadeIndices[static_cast<std::size_t>(fadeCount++)] =
        partParamIndex("pitch_rate_fade.p0");
    fadeIndices[static_cast<std::size_t>(fadeCount++)] =
        partParamIndex("pitch_depth_fade.p0");
    store.setLfoFadeIndices(fadeIndices);
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

juce::String TypePAlphaAudioProcessor::getMacroName(int index) const
{
    return macroNames_[static_cast<size_t>(juce::jlimit(0, 5, index))];
}

void TypePAlphaAudioProcessor::setMacroName(int index, const juce::String& name)
{
    macroNames_[static_cast<size_t>(juce::jlimit(0, 5, index))] =
        name.substring(0, 32);
}

double TypePAlphaAudioProcessor::macroEffectiveValue(int index) const
{
    if (index < 0 || index >= 6)
        return 0.0;
    const std::size_t i = static_cast<std::size_t>(index);
    const int knob = macroValueValues_[i] != nullptr
        ? static_cast<int>(std::lround(macroValueValues_[i]->load()))
        : 0;
    const int source = macroMidiValues_[i] != nullptr
        ? static_cast<int>(std::lround(macroMidiValues_[i]->load()))
        : 1;
    const int range = macroMidiRangeValues_[i] != nullptr
        ? static_cast<int>(std::lround(macroMidiRangeValues_[i]->load()))
        : 0;
    int sourceValue = 0;
    if (source == 1)
        sourceValue = macroCc_[i].load();
    else if (source == 2)
        sourceValue = modWheel_.load();
    else if (source == 3)
        sourceValue = noteVelocity_.load();
    return p1::macroEffective(knob, sourceValue, range);
}

TypePAlphaAudioProcessor::MacroFeedback
TypePAlphaAudioProcessor::macroFeedback(int index) const
{
    MacroFeedback result;
    if (index < 0 || index >= kP1ParamCount ||
        index >= static_cast<int>(macroMemberIndex_.size()) ||
        macroMemberIndex_[static_cast<std::size_t>(index)] < 0)
        return result;
    const auto& spec = kP1ParamSpecs[static_cast<std::size_t>(index)];
    auto* parameter = apvts_.getParameter(spec.id);
    const auto* stored = apvts_.getRawParameterValue(spec.id);
    if (parameter == nullptr || stored == nullptr)
        return result;
    const int base = static_cast<int>(std::lround(stored->load()));
    double offset = 0.0;
    for (int m = 0; m < 6; ++m) {
        const auto macro = macroEffectiveValue(m);
        for (int d = 0; d < 3; ++d) {
            const std::size_t slot = static_cast<std::size_t>(m * 3 + d);
            const int target = macroTargetValues_[slot] != nullptr
                ? static_cast<int>(
                      std::lround(macroTargetValues_[slot]->load()))
                : 0;
            const int range = macroRangeValues_[slot] != nullptr
                ? static_cast<int>(
                      std::lround(macroRangeValues_[slot]->load()))
                : 0;
            if (target <= 0 || target > p1::macros::kTargetCount || range == 0)
                continue;
            const int row = target - 1;
            const int part = p1::macros::kTargetPart[row];
            const int algorithm =
                algorithmValues_[static_cast<std::size_t>(part)] != nullptr
                    ? static_cast<int>(std::lround(
                          algorithmValues_[static_cast<std::size_t>(part)]
                              ->load()))
                    : 0;
            if (!p1::macroRowIncludes(row, index, algorithm))
                continue;
            result.targeted = true;
            offset += p1::macroOffset(macro, range);
        }
    }
    result.effective = static_cast<double>(base);
    if (offset != 0.0) {
        if (spec.logarithmic) {
            const double baseNorm =
                parameter->convertTo0to1(static_cast<float>(base));
            const double effectiveNorm =
                p1::macroTargetNormalised(baseNorm, offset);
            result.effective =
                static_cast<double>(std::lround(parameter->convertFrom0to1(
                    static_cast<float>(effectiveNorm))));
        } else {
            const double span = static_cast<double>(spec.max - spec.min);
            result.effective = std::round(std::clamp(
                static_cast<double>(base) + offset * span,
                static_cast<double>(spec.min), static_cast<double>(spec.max)));
        }
    }
    result.hasEffective = result.effective != static_cast<double>(base);
    return result;
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
    configurePresetStore(presets_);
    lastParams_.fill(std::numeric_limits<int32_t>::min());
    for (int i = 0; i < kP1ParamCount; ++i)
        paramValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1ParamSpecs[static_cast<std::size_t>(i)].id);
    outputVolume_ = apvts_.getRawParameterValue(p1::kVolumeParamID);

    /* The macro block order is six values, six source selects, six source ranges,
     * eighteen targets and eighteen destination ranges. */
    static_assert(kP1MacroParamCount == 54, "the macro block is 54 parameters");
    for (int i = 0; i < 6; ++i) {
        macroValueValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[i]);
        macroMidiValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[6 + i]);
        macroMidiRangeValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[12 + i]);
        macroTargetValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[18 + i]);
        macroRangeValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[36 + i]);
        macroCc_[static_cast<std::size_t>(i)].store(0);
    }
    modWheel_.store(0);
    noteVelocity_.store(0);
    for (int i = 6; i < 18; ++i) {
        macroTargetValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[18 + i]);
        macroRangeValues_[static_cast<std::size_t>(i)] =
            apvts_.getRawParameterValue(kP1MacroParamIDs[36 + i]);
    }
    algorithmValues_[0] = apvts_.getRawParameterValue("algorithm.p0");
    algorithmValues_[1] = apvts_.getRawParameterValue("algorithm.p1");
    macroMemberIndex_.assign(static_cast<std::size_t>(kP1ParamCount), -1);
    for (const auto& member : p1::macros::kTargetMemberPool) {
        const int id = member.id;
        if (id < 0 || id >= kP1ParamCount ||
            macroMemberIndex_[static_cast<std::size_t>(id)] >= 0)
            continue;
        macroMemberIndex_[static_cast<std::size_t>(id)] =
            static_cast<int>(macroMemberIds_.size());
        macroMemberIds_.push_back(id);
        macroMemberParams_.push_back(
            apvts_.getParameter(kP1ParamSpecs[static_cast<std::size_t>(id)].id));
        macroMemberLog_.push_back(member.log);
    }
    macroOffsets_.assign(macroMemberIds_.size(), 0.0);
}

juce::AudioProcessorValueTreeState::ParameterLayout
TypePAlphaAudioProcessor::createLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    for (const auto& spec : kP1ParamSpecs) {
        const int low = spec.min;
        const int high = spec.max > spec.min ? spec.max : spec.min + 1;
        const int def = juce::jlimit(low, high, spec.def);
        const auto display = juce::String(spec.display);
        /* The LFO rates are log parameters. The raw value is a Q32 phase
         * increment, so a linear range puts 4 Hz in the bottom two percent of the
         * knob. The custom range keeps 0 as the off stop and maps 0.01 Hz to
         * 223 Hz logarithmically above it, which the macro offset also uses. */
        if (spec.logarithmic && display == "hz") {
            constexpr float floorHz = 894.0f;  // 0.01 Hz in Q32 phase increments
            auto range = juce::NormalisableRange<float>(
                static_cast<float>(low), static_cast<float>(high),
                [](float start, float end, float value) {
                    if (value <= 0.0f)
                        return start;
                    return juce::jmin(end,
                                      floorHz * std::pow(end / floorHz, value));
                },
                [](float start, float end, float value) {
                    juce::ignoreUnused(start);
                    if (value <= 0.0f)
                        return 0.0f;
                    return std::log(value / floorHz) /
                           std::log(end / floorHz);
                });
            auto attributes = juce::AudioParameterFloatAttributes()
                                  .withAutomatable(spec.automatable)
                                  .withStringFromValueFunction([spec](float value, int) {
                                      return p1::ui::formatParamValue(spec, value);
                                  })
                                  .withValueFromStringFunction([spec](const juce::String& text) {
                                      return static_cast<float>(
                                          p1::ui::parseParamValue(spec, text));
                                  });
            layout.add(std::make_unique<juce::AudioParameterFloat>(
                juce::ParameterID{spec.id, 1}, spec.name, range,
                static_cast<float>(def), attributes));
            continue;
        }
        auto attributes = juce::AudioParameterIntAttributes()
                              .withAutomatable(spec.automatable);
        if (display == "cutoff" || display == "oct" || display == "pct" ||
            display == "frac3" || display == "lfo_wave") {
            attributes = attributes
                .withStringFromValueFunction([spec](int value, int) {
                    return p1::ui::formatParamValue(spec, value);
                })
                .withValueFromStringFunction([spec](const juce::String& text) {
                    return static_cast<int>(
                        std::lround(p1::ui::parseParamValue(spec, text)));
                });
        }
        layout.add(std::make_unique<juce::AudioParameterInt>(
            juce::ParameterID{spec.id, 1}, spec.name, low, high, def,
            attributes));
    }
    auto volumeAttributes = juce::AudioParameterIntAttributes()
        .withStringFromValueFunction([](int value, int) {
            return p1::ui::volumeText(value);
        })
        .withValueFromStringFunction([](const juce::String& text) {
            return p1::ui::volumePositionFromText(text);
        });
    layout.add(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID{p1::kVolumeParamID, 1}, "volume", 0,
        p1::kVolumePositions, p1::kVolumeUnity, volumeAttributes));
    return layout;
}

void TypePAlphaAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    hostRate_ = sampleRate;
    engine_.reset();
    midi_.reset();
    rateBridge_.reset();
    directBuffer_.assign(static_cast<std::size_t>(kControlBlock) * 2u, 0);
    monoFold_.assign(static_cast<std::size_t>(std::max(1, samplesPerBlock)), 0.0f);
    scheduled_.clear();
    scheduled_.reserve(static_cast<std::size_t>(std::max(1, samplesPerBlock)) * 2u + 16u);
    deferredOffs_.clear();
    deferredOffs_.reserve(64);
    nextScheduled_ = 0;
    coreFrames_ = 0;
    chunkNotes_.reset();
    for (auto& value : liveMorph_)
        value.store(-1, std::memory_order_relaxed);
    if (std::abs(sampleRate - kCoreRate) > 0.5) {
        rateBridge_.prepare(sampleRate, samplesPerBlock);
        setLatencySamples(rateBridge_.latencySamples());
    } else {
        setLatencySamples(0);
    }
    lastParams_.fill(std::numeric_limits<int32_t>::min());
    paramsDirty_.store(false, std::memory_order_relaxed);
    macroApplied_ = false;
    syncParams();
    const int volumePosition = outputVolume_ != nullptr
        ? static_cast<int>(std::lround(outputVolume_->load()))
        : p1::kVolumeUnity;
    volumeGain_ = p1::volumeGainFromPosition(volumePosition);
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

void TypePAlphaAudioProcessor::syncMacros()
{
    double values[6] = {};
    int sources[6] = {};
    bool anyValue = false;
    for (int i = 0; i < 6; ++i) {
        const std::size_t index = static_cast<std::size_t>(i);
        values[i] = macroEffectiveValue(i);
        sources[i] = macroMidiValues_[index] != nullptr
            ? static_cast<int>(std::lround(macroMidiValues_[index]->load()))
            : 1;
        anyValue = anyValue || std::abs(values[i]) > 1.0e-9;
    }
    if (!anyValue && !macroApplied_)
        return;
    std::fill(macroOffsets_.begin(), macroOffsets_.end(), 0.0);
    if (anyValue) {
        for (int m = 0; m < 6; ++m) {
            if (std::abs(values[m]) < 1.0e-9)
                continue;
            for (int d = 0; d < 3; ++d) {
                const int slot = m * 3 + d;
                const int target =
                    macroTargetValues_[static_cast<std::size_t>(slot)] != nullptr
                        ? static_cast<int>(std::lround(
                              macroTargetValues_[static_cast<std::size_t>(slot)]
                                  ->load()))
                        : 0;
                const int range =
                    macroRangeValues_[static_cast<std::size_t>(slot)] != nullptr
                        ? static_cast<int>(std::lround(
                              macroRangeValues_[static_cast<std::size_t>(slot)]
                                  ->load()))
                        : 0;
                if (target <= 0 || target > p1::macros::kTargetCount ||
                    range == 0)
                    continue;
                const int row = target - 1;
                const int part = p1::macros::kTargetPart[row];
                const int algorithm =
                    algorithmValues_[static_cast<std::size_t>(part)] != nullptr
                        ? static_cast<int>(std::lround(
                              algorithmValues_[static_cast<std::size_t>(part)]
                                  ->load()))
                        : 0;
                const auto& mask = p1::macros::kCarrierMasks[static_cast<std::size_t>(
                    juce::jlimit(0, 7, algorithm))];
                const int role = p1::macros::kTargetRole[row];
                const int start = p1::macros::kTargetMemberOffsets[row];
                const int end = p1::macros::kTargetMemberOffsets[row + 1];
                const double offset = p1::macroOffset(values[m], range);
                for (int i = start; i < end; ++i) {
                    const auto& member =
                        p1::macros::kTargetMemberPool[static_cast<std::size_t>(i)];
                    if (role == 1 &&
                        (member.op == 0 || !mask[member.op - 1]))
                        continue;
                    if (role == 2 &&
                        (member.op == 0 || mask[member.op - 1]))
                        continue;
                    const int index =
                        macroMemberIndex_[static_cast<std::size_t>(member.id)];
                    if (index < 0)
                        continue;
                    /* The engine applies the velocity source itself for the
                     * per-voice groups. The phase envelope and phase LFO members
                     * of a mixed row stay in the part-level layer. */
                    if (sources[m] == 3 &&
                        engineHandlesVelocityTarget(target, member.id))
                        continue;
                    macroOffsets_[static_cast<std::size_t>(index)] += offset;
                }
            }
        }
    }
    for (std::size_t k = 0; k < macroMemberIds_.size(); ++k) {
        const int id = macroMemberIds_[k];
        const auto& spec = kP1ParamSpecs[static_cast<std::size_t>(id)];
        const int base = paramValues_[static_cast<std::size_t>(id)] != nullptr
            ? static_cast<int>(
                  std::lround(paramValues_[static_cast<std::size_t>(id)]->load()))
            : 0;
        int effective = base;
        const double offset = macroOffsets_[k];
        if (offset != 0.0) {
            if (macroMemberLog_[k] != 0 && macroMemberParams_[k] != nullptr) {
                const double baseNorm =
                    macroMemberParams_[k]->convertTo0to1(static_cast<float>(base));
                const double effectiveNorm =
                    p1::macroTargetNormalised(baseNorm, offset);
                effective = static_cast<int>(std::lround(
                    macroMemberParams_[k]->convertFrom0to1(
                        static_cast<float>(effectiveNorm))));
            } else {
                const double span = static_cast<double>(spec.max - spec.min);
                const double shifted = static_cast<double>(base) + offset * span;
                effective = static_cast<int>(std::lround(std::clamp(
                    shifted, static_cast<double>(spec.min),
                    static_cast<double>(spec.max))));
            }
        }
        engine_.setParam(static_cast<uint32_t>(spec.engineId), effective);
    }
    macroApplied_ = anyValue;
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

void TypePAlphaAudioProcessor::publishMorph(p1::Engine& engine)
{
    for (int part = 0; part < kP1PartCount; ++part)
        for (int op = 0; op < 4; ++op)
            liveMorph_[static_cast<std::size_t>(part * 4 + op)].store(
                engine.specMorph(op, part), std::memory_order_relaxed);
}

void TypePAlphaAudioProcessor::publishPhaseMaps(p1::Engine& engine)
{
    /* The audio side writes its own buffer, then hands it over by swapping it
     * with the middle slot. The reader swaps its own buffer in the same way, so
     * the writer's back buffer is never the buffer the reader is copying. */
    static constexpr uint32_t kIndexMask = 3u;
    static constexpr uint32_t kNewData = 4u;
    const uint32_t back = phaseMapBack_;
    for (int part = 0; part < kP1PartCount; ++part)
        for (int op = 0; op < 4; ++op)
            engine.phaseMapSnapshot(
                op, part,
                phaseMapSnapshots_[back][static_cast<std::size_t>(part * 4 + op)]);
    phaseMapBack_ =
        phaseMapMiddle_.exchange(back | kNewData, std::memory_order_acq_rel) &
        kIndexMask;
}

void TypePAlphaAudioProcessor::feedScheduled(int64_t limit)
{
    while (nextScheduled_ < scheduled_.size() &&
           scheduled_[nextScheduled_].coreFrame < limit) {
        const auto& event = scheduled_[nextScheduled_];
        const auto& message = event.message;
        const bool noteOff =
            message.isNoteOff() ||
            (message.isNoteOn() && message.getVelocity() == 0);
        if (noteOff && chunkNotes_.held(message.getChannel(),
                                        message.getNoteNumber())) {
            /* The note started in this same tick, so the release waits until the
             * tick has rendered. It sounds for one control block rather than
             * cancelling before any audio. */
            deferredOffs_.push_back(message);
        } else {
            feedMessage(midi_, message);
            if (message.isNoteOn() && message.getVelocity() > 0)
                chunkNotes_.noteOn(message.getChannel(),
                                   message.getNoteNumber());
        }
        ++nextScheduled_;
    }
}

void TypePAlphaAudioProcessor::flushDeferred()
{
    for (const auto& message : deferredOffs_)
        feedMessage(midi_, message);
    deferredOffs_.clear();
}

void TypePAlphaAudioProcessor::coreChunk(void* context, p1::Engine& engine,
                                         int frames)
{
    auto* self = static_cast<TypePAlphaAudioProcessor*>(context);
    self->flushDeferred();
    self->coreFrames_ += static_cast<uint64_t>(frames);
    self->chunkNotes_.reset();
    self->feedScheduled(static_cast<int64_t>(self->coreFrames_) + kControlBlock);
    self->pushTapScopes(engine, frames);
    self->publishMorph(engine);
    self->publishPhaseMaps(engine);
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

            chunkNotes_.reset();
            deferredOffs_.clear();
            while (event != midi.end() && (*event).samplePosition < end) {
                const auto message = (*event).getMessage();
                if (message.isController()) {
                    const int controller = message.getControllerNumber();
                    const int controllerValue = message.getControllerValue();
                    if (controller == 1) {
                        modWheel_.store(controllerValue,
                                        std::memory_order_relaxed);
                    } else if (controller >= 72 && controller <= 77) {
                        macroCc_[static_cast<std::size_t>(controller - 72)]
                            .store(controllerValue, std::memory_order_relaxed);
                    }
                } else if (message.isNoteOn() &&
                           message.getVelocity() > 0) {
                    noteVelocity_.store(message.getVelocity(),
                                        std::memory_order_relaxed);
                }
                if (!isEngineMessage(message)) {
                    ++event;
                    continue;
                }
                const bool noteOff =
                    message.isNoteOff() ||
                    (message.isNoteOn() && message.getVelocity() == 0);
                if (noteOff && chunkNotes_.held(message.getChannel(),
                                                message.getNoteNumber())) {
                    deferredOffs_.push_back(message);
                } else {
                    feedMessage(midi_, message);
                    if (message.isNoteOn() && message.getVelocity() > 0)
                        chunkNotes_.noteOn(message.getChannel(),
                                           message.getNoteNumber());
                }
                ++event;
            }

            syncParams();
            syncMacros();
            renderCore(directBuffer_.data(), length);
            for (int i = 0; i < length; ++i) {
                const auto index = static_cast<std::size_t>(i) * 2u;
                const float l =
                    static_cast<float>(directBuffer_[index]) * kIntToFloat;
                const float r =
                    static_cast<float>(directBuffer_[index + 1u]) * kIntToFloat;
                if (right != nullptr) {
                    left[pos + i] = l;
                    right[pos + i] = r;
                } else {
                    /* Fold the stereo pair down at 0.5, so a centred part keeps
                     * its level and a hard-panned part sits 6 dB down. */
                    left[pos + i] = (l + r) * 0.5f;
                }
            }
            flushDeferred();
            outputScope_.push(left + pos, length);
            pushTapScopes(engine_, length);
            publishMorph(engine_);
            publishPhaseMaps(engine_);
            pos = end;
        }

        while (event != midi.end()) {
            const auto message = (*event).getMessage();
            if (isEngineMessage(message))
                feedMessage(midi_, message);
            ++event;
        }
    } else {
        /* Map each host event into the core timeline, then feed it when the
         * bridge is about to render the tick it belongs to. At 96 and 192 kHz a
         * block generates fewer core frames than host frames, so an event can
         * fall in a tick that the next block renders. Those wait in
         * `scheduled_`; this block applies any that are still there first. */
        chunkNotes_.reset();
        if (nextScheduled_ < scheduled_.size())
            feedScheduled(std::numeric_limits<int64_t>::max());
        scheduled_.clear();
        nextScheduled_ = 0;
        chunkNotes_.reset();
        const double ratio = rateBridge_.ratio();
        for (const auto metadata : midi) {
            const auto message = metadata.getMessage();
            if (message.isController()) {
                const int controller = message.getControllerNumber();
                const int controllerValue = message.getControllerValue();
                if (controller == 1) {
                    modWheel_.store(controllerValue,
                                    std::memory_order_relaxed);
                } else if (controller >= 72 && controller <= 77) {
                    macroCc_[static_cast<std::size_t>(controller - 72)]
                        .store(controllerValue, std::memory_order_relaxed);
                }
            } else if (message.isNoteOn() && message.getVelocity() > 0) {
                noteVelocity_.store(message.getVelocity(),
                                    std::memory_order_relaxed);
            }
            if (!isEngineMessage(message))
                continue;
            const int64_t coreFrame = static_cast<int64_t>(std::floor(
                static_cast<double>(coreFrames_) +
                static_cast<double>(metadata.samplePosition) * ratio));
            scheduled_.push_back({message, coreFrame});
        }
        syncParams();
        syncMacros();
        float* left = buffer.getWritePointer(0);
        const bool mono = channels == 1;
        if (mono && monoFold_.size() < static_cast<std::size_t>(frames))
            monoFold_.resize(static_cast<std::size_t>(frames));
        /* The bridge writes two channels, so a mono host takes the second one in
         * a scratch buffer and folds it down after the pull. */
        float* right = mono ? monoFold_.data() : buffer.getWritePointer(1);
        if (rateBridge_.hasCore(frames))
            feedScheduled(static_cast<int64_t>(coreFrames_) + kControlBlock);
        rateBridge_.process(engine_, left, right, frames, &coreChunk, this);
        if (mono) {
            for (int i = 0; i < frames; ++i)
                left[i] = (left[i] + right[i]) * 0.5f;
        }
        outputScope_.push(left, frames);
        publishMorph(engine_);
        publishPhaseMaps(engine_);
    }

    const int volumePosition = outputVolume_ != nullptr
        ? static_cast<int>(std::lround(outputVolume_->load()))
        : p1::kVolumeUnity;
    const float volumeTarget = p1::volumeGainFromPosition(volumePosition);
    if (volumeTarget != volumeGain_) {
        /* The ramp starts where the last block ended, so a drag steps in the
           same 32 to 512 sample slices as the rest of the panel. */
        buffer.applyGainRamp(0, frames, volumeGain_, volumeTarget);
        volumeGain_ = volumeTarget;
    } else if (volumeGain_ != 1.0f) {
        /* The engine renders at unity every block, so a constant trim has to be
           applied every block. Only the unity setting can skip it. */
        buffer.applyGain(0, frames, volumeGain_);
    }

    midi.clear();
}

bool TypePAlphaAudioProcessor::phaseMap(int op, int part,
                                        p1::PhaseMapSnapshot& out) const
{
    if (op < 0 || op > 3 || part < 0 || part >= kP1PartCount)
        return false;
    static constexpr uint32_t kIndexMask = 3u;
    static constexpr uint32_t kNewData = 4u;
    if ((phaseMapMiddle_.load(std::memory_order_acquire) & kNewData) != 0u)
        phaseMapFront_ =
            phaseMapMiddle_.exchange(phaseMapFront_, std::memory_order_acq_rel) &
            kIndexMask;
    out = phaseMapSnapshots_[phaseMapFront_][static_cast<std::size_t>(part * 4 + op)];
    return out.valid;
}

double TypePAlphaAudioProcessor::specMorph(int op, int part) const
{
    if (op < 0 || op > 3 || part < 0 || part >= kP1PartCount)
        return -1.0;
    const int live =
        liveMorph_[static_cast<std::size_t>(part * 4 + op)].load(
            std::memory_order_relaxed);
    return live < 0 ? -1.0 : static_cast<double>(live) / 99.0;
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

void TypePAlphaAudioProcessor::resetPhaseMaps()
{
    engine_.resetPhaseMaps();
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

LibrarySaveResult TypePAlphaAudioProcessor::saveLibrary()
{
    LibrarySaveResult result;
    const auto file = libraryFile();
    file.getParentDirectory().createDirectory();

    /* The read-merge-write must be one transaction across processes, or a save
     * from the standalone and one from the DAW can each read the same revision
     * and the second replaces the first's slot. The wait is unbounded because the
     * transaction is short and the OS releases the lock if a holder dies; JUCE's
     * POSIX path reports a timed-out attempt as success, so a finite timeout
     * would not give the exclusion this needs. */
    juce::InterProcessLock libraryLock("P1Onken-TypePAlpha-Library");
    if (!libraryLock.enter(-1)) {
        result.busy = true;
        return result;
    }
    struct LockRelease {
        juce::InterProcessLock& lock;
        ~LockRelease() { lock.exit(); }
    } release{libraryLock};

    if (!file.existsAsFile()) {
        /* The first save: write this instance's bank as revision 1. */
        presets_.setRevision(presets_.revision() + 1);
        const auto bytes = presets_.packLibrary();
        if (!file.replaceWithData(bytes.data(), bytes.size())) {
            presets_.setRevision(presets_.revision() - 1);
            DBG("could not save the plugin library");
            return result;
        }
        presets_.markSaved();
        result.written = true;
        return result;
    }

    juce::MemoryBlock data;
    p1::PresetStore disk(kP1PartParamCount, kP1PartCount, p1::partParamDefaults(),
                         kP1PartCutoffIndex);
    configurePresetStore(disk);
    if (!file.loadFileAsData(data) ||
        !disk.loadLibrary(static_cast<const uint8_t*>(data.getData()),
                          data.getSize())) {
        result.unreadable = true;
        return result;
    }

    const auto conflicts = presets_.mergeInto(disk);
    if (!conflicts.empty()) {
        result.conflict = true;
        result.multi = conflicts.front().multi;
        result.slot = conflicts.front().slot;
        result.name = conflicts.front().name;
    }

    const uint32_t current =
        presets_.revision() > disk.revision() ? presets_.revision()
                                              : disk.revision();
    disk.setRevision(current + 1);
    const auto bytes = disk.packLibrary();
    if (!file.replaceWithData(bytes.data(), bytes.size())) {
        DBG("could not save the plugin library");
        return result;
    }

    result.written = true;
    presets_.markSaved();
    if (!result.conflict)
        presets_.adoptLibrary(disk);
    else
        presets_.setRevision(disk.revision());
    return result;
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
    for (int i = 0; i < 6; ++i) {
        state.setProperty("macroName" + juce::String(i), macroNames_[i],
                          nullptr);
    }
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
    const int loadedVersion = static_cast<int>(state.getProperty("stateVersion", 0));
    if (loadedVersion < kCutoffLawStateVersion) {
        for (int i = 0; i < state.getNumChildren(); ++i) {
            auto child = state.getChild(i);
            if (!child.hasType("PARAM"))
                continue;
            const auto id = child.getProperty("id").toString();
            if (id == "cutoff.p0" || id == "cutoff.p1") {
                const int old = static_cast<int>(child.getProperty("value"));
                child.setProperty("value", p1::cutoffOldToNew(old), nullptr);
            }
        }
    }
    patchName_ = state.getProperty("patchName", "init patch").toString();
    partNames_[0] = state.getProperty("part1Name", "init").toString();
    partNames_[1] = state.getProperty("part2Name", "init").toString();
    for (int i = 0; i < 6; ++i) {
        macroNames_[static_cast<std::size_t>(i)] =
            state.getProperty("macroName" + juce::String(i), "").toString();
    }
    uiActivePart_ = juce::jlimit(0, 1, static_cast<int>(state.getProperty("uiActivePart", 0)));
    int page = static_cast<int>(state.getProperty("uiPage", 0));
    const int pageLayout =
        static_cast<int>(state.getProperty("uiPageLayout", 0));
    if (pageLayout < 2) {
        /* The four-tab layout had operators at 1, op lfos at 2 and
         * fx/mod at 3. The six-tab layout moves fx/mod to 1. */
        if (page == 1)
            page = 2;
        else if (page == 2)
            page = 3;
        else if (page == 3)
            page = 1;
    }
    if (pageLayout == 2 && page == 5) {
        /* The first six-tab layout had the mod page at 5. The split inserts
         * op spectra b at 5, so the mod page moves to 6 before the phase tab
         * shifts the spectral pages again. */
        page = 6;
    }
    if (pageLayout < kPhaseMapPageLayout && page >= 4) {
        /* Layout 3 inserted op phasemap at 4, so the three spectral pages
         * move from 4/5/6 to 5/6/7. */
        ++page;
    }
    if (pageLayout < kUiPageLayout && page >= 1) {
        /* Layout 5 inserted the macros page at 1, so every page from part
         * params onward moves up one. */
        ++page;
    }
    uiPage_ = juce::jlimit(0, 8, page);
    presets_.setCurrentMulti(static_cast<int>(state.getProperty("currentMulti", -1)));
    for (int part = 0; part < kP1PartCount; ++part) {
        presets_.setPartSource(
            part, static_cast<int>(
                      state.getProperty("partSource" + juce::String(part), -1)));
    }
    /* The banks stay in the project as a portable snapshot of the library, but
     * loading a project never writes them into the shared store. A project made
     * against an older library would otherwise replace every slot on disk at the
     * next save. Restoring the snapshot is a separate, explicit action. */
    apvts_.replaceState(state);
    resetPhaseMaps();
    paramsDirty_.store(true, std::memory_order_relaxed);
    updateHostDisplay(juce::AudioProcessor::ChangeDetails().withProgramChanged(true));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TypePAlphaAudioProcessor();
}
