/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 2 to 6: the JUCE processor smoke, state and parameter test, which
 * needs no DAW. */
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Engine.hpp"
#include "ParamIDs.h"
#include "Params.h"
#include "PluginProcessor.h"
#include "Widgets.h"

namespace {

constexpr int kFrames = 256;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("plugin: %-64s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

bool bitEqual(float a, float b)
{
    return std::memcmp(&a, &b, sizeof a) == 0;
}

void renderReference(int32_t* out, int frames, int noteOffset, int32_t algorithm)
{
    p1::Engine reference;
    reference.reset();
    for (const auto& spec : kP1ParamSpecs)
        reference.setParam(
            static_cast<uint32_t>(spec.engineId),
            std::strcmp(spec.id, "algorithm.p0") == 0 ? algorithm : spec.def);
    if (noteOffset > 0)
        reference.render(out, static_cast<uint32_t>(noteOffset));
    reference.noteOn(60, 100, 0);
    reference.render(out + noteOffset * 2, static_cast<uint32_t>(frames - noteOffset));
}

bool matches(const juce::AudioBuffer<float>& buffer, const int32_t* reference, int frames)
{
    for (int i = 0; i < frames; ++i) {
        const float left = static_cast<float>(reference[i * 2]) / 32768.0f;
        if (!bitEqual(buffer.getSample(0, i), left))
            return false;
        if (buffer.getNumChannels() > 1) {
            const float right = static_cast<float>(reference[i * 2 + 1]) / 32768.0f;
            if (!bitEqual(buffer.getSample(1, i), right))
                return false;
        }
    }
    return true;
}

} // namespace

int main()
{
    if (const auto* spec = p1::ui::findSpec("cutoff.p0")) {
        check(p1::ui::formatParamValue(*spec, 36958) == "909Hz",
              "the cutoff reads in hertz");
    } else {
        check(false, "the cutoff spec is present");
    }
    if (const auto* comp = p1::ui::findSpec("lp.comp.p0"))
        check(comp->def == 7209, "the internal makeup defaults to 22 percent");
    else
        check(false, "the internal makeup spec is present");
    auto processorOwner = std::make_unique<TypePAlphaAudioProcessor>();
    auto& processor = *processorOwner;

    check(processor.getParameters().size() == kP1ParamCount,
          "the APVTS exposes every manifest parameter");
    {
        int bad = 0;
        for (auto* parameter : processor.getParameters()) {
            for (int step = 0; step <= 20; ++step) {
                const double norm = static_cast<double>(step) / 20.0;
                const auto first = parameter->getText(
                    static_cast<float>(norm), 64);
                const auto second = parameter->getText(
                    parameter->getValueForText(first), 64);
                if (first != second) {
                    std::printf("roundtrip %s %.2f: %s -> %s\n",
                                parameter->getName(64).toRawUTF8(), norm,
                                first.toRawUTF8(), second.toRawUTF8());
                    ++bad;
                }
            }
        }
        check(bad == 0, "every host parameter text round-trips");
    }
    check(processor.acceptsMidi() && !processor.producesMidi(),
          "the processor accepts MIDI and produces audio only");

    std::set<juce::String> ids;
    std::array<bool, kP1ParamCount> engineIds{};
    bool contiguous = true;
    for (const auto& spec : kP1ParamSpecs) {
        ids.insert(juce::String(spec.id));
        if (spec.engineId < 0 || spec.engineId >= kP1ParamCount)
            contiguous = false;
        else
            engineIds[static_cast<std::size_t>(spec.engineId)] = true;
    }
    for (bool seen : engineIds)
        contiguous = contiguous && seen;
    check(static_cast<int>(ids.size()) == kP1ParamCount,
          "the APVTS parameter ids are unique");
    check(contiguous, "the engine ids cover every manifest index");

    processor.setRateAndBufferSizeDetails(48000.0, kFrames);
    processor.prepareToPlay(48000.0, kFrames);
    check(processor.getLatencySamples() == 0,
          "48 kHz bypass reports zero latency");

    juce::AudioBuffer<float> block(2, kFrames);
    juce::MidiBuffer midi;
    midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
    processor.processBlock(block, midi);

    int32_t reference[kFrames * 2] = {};
    renderReference(reference, kFrames, 0, 4);
    check(matches(block, reference, kFrames), "48 kHz output is bit-exact with the engine");

    juce::MemoryBlock state;
    processor.getStateInformation(state);
    check(state.getSize() > 0, "the state saves to a blob");

    auto* algorithm =
        processor.parameters().getRawParameterValue("algorithm.p0");
    check(algorithm != nullptr && std::lround(algorithm->load()) == 4,
          "algorithm.p0 starts at the manifest default");

    auto* parameter = processor.parameters().getParameter("algorithm.p0");
    if (parameter != nullptr) {
        parameter->setValueNotifyingHost(parameter->convertTo0to1(7.0f));
        processor.processBlock(block, midi);
    }
    check(algorithm != nullptr && std::lround(algorithm->load()) == 7,
          "an automation write reaches the parameter state");

    processor.setStateInformation(state.getData(), static_cast<int>(state.getSize()));
    check(algorithm != nullptr && std::lround(algorithm->load()) == 4,
          "the state restores every parameter");

    {
        std::unique_ptr<juce::XmlElement> oldXml(processor.getXmlFromBinary(
            state.getData(), static_cast<int>(state.getSize())));
        check(oldXml != nullptr, "the saved state decodes as XML");
        if (oldXml != nullptr) {
            auto tree = juce::ValueTree::fromXml(*oldXml);
            tree.setProperty("uiPage", 5, nullptr);
            tree.setProperty("uiPageLayout", 2, nullptr);
            juce::MemoryBlock older;
            processor.copyXmlToBinary(*tree.createXml(), older);
            processor.setStateInformation(older.getData(),
                                          static_cast<int>(older.getSize()));
            check(processor.getUiPage() == 7,
                  "the six-tab mod page moves to 7 on restore");

            tree.setProperty("uiPage", 5, nullptr);
            tree.setProperty("uiPageLayout", 3, nullptr);
            juce::MemoryBlock previous;
            processor.copyXmlToBinary(*tree.createXml(), previous);
            processor.setStateInformation(previous.getData(),
                                          static_cast<int>(previous.getSize()));
            check(processor.getUiPage() == 6,
                  "the layout-3 spectra b page moves to 6 on restore");

            tree.setProperty("uiPage", 3, nullptr);
            tree.removeProperty("uiPageLayout", nullptr);
            juce::MemoryBlock oldest;
            processor.copyXmlToBinary(*tree.createXml(), oldest);
            processor.setStateInformation(oldest.getData(),
                                          static_cast<int>(oldest.getSize()));
            check(processor.getUiPage() == 1,
                  "the four-tab fx page moves to 1 on restore");
        }
        processor.setStateInformation(state.getData(),
                                      static_cast<int>(state.getSize()));
    }

    {
        std::unique_ptr<juce::XmlElement> oldXml(processor.getXmlFromBinary(
            state.getData(), static_cast<int>(state.getSize())));
        if (oldXml != nullptr) {
            auto tree = juce::ValueTree::fromXml(*oldXml);
            tree.setProperty("stateVersion", 4, nullptr);
            for (int i = 0; i < tree.getNumChildren(); ++i) {
                auto child = tree.getChild(i);
                if (child.hasType("PARAM") &&
                    child.getProperty("id").toString() == "cutoff.p0")
                    child.setProperty("value", 8000, nullptr);
            }
            juce::MemoryBlock older;
            processor.copyXmlToBinary(*tree.createXml(), older);
            processor.setStateInformation(older.getData(),
                                          static_cast<int>(older.getSize()));
            auto* cutoff =
                processor.parameters().getRawParameterValue("cutoff.p0");
            check(cutoff != nullptr &&
                      std::lround(cutoff->load()) == 36958,
                  "an old state migrates the cutoff to 909 Hz");
            processor.setStateInformation(state.getData(),
                                          static_cast<int>(state.getSize()));
        }
    }

    {
        const std::vector<int32_t> params(
            static_cast<std::size_t>(kP1PartParamCount), 7);
        processor.presets().savePart(3, "smoke", params);
        processor.presets().saveMulti(
            2, "smoke multi", std::vector<uint8_t>{3, 3},
            std::vector<int32_t>{99, 99}, std::vector<int32_t>{0, 0},
            std::vector<int32_t>{0, 1}, std::vector<int32_t>{0, 0},
            std::vector<int32_t>{127, 127});
        processor.presets().setCurrentMulti(2);
        processor.presets().setPartSource(0, 3);
        processor.getStateInformation(state);
        processor.presets().savePart(
            3, "changed", std::vector<int32_t>(
                              static_cast<std::size_t>(kP1PartParamCount), 1));
        processor.presets().saveMulti(
            2, "changed multi", std::vector<uint8_t>{3, 3},
            std::vector<int32_t>{99, 99}, std::vector<int32_t>{0, 0},
            std::vector<int32_t>{0, 1}, std::vector<int32_t>{0, 0},
            std::vector<int32_t>{127, 127});
        processor.presets().setCurrentMulti(-1);
        processor.presets().setPartSource(0, -1);
        processor.setStateInformation(state.getData(),
                                      static_cast<int>(state.getSize()));
        check(processor.presets().partName(3) == "changed",
              "a project load leaves the part bank alone");
        check(processor.presets().multiUsed(2) &&
                  processor.presets().multiName(2) == "changed multi",
              "a project load leaves the multi bank alone");
        check(processor.presets().currentMulti() == 2 &&
                  processor.presets().partSource(0) == 3,
              "the state restores the current patch and part source");
    }

    {
        auto library = juce::File::getSpecialLocation(
                           juce::File::tempDirectory)
                           .getNonexistentChildFile("p1-library", ".p1lib", false);
        /* A processor carries ten scope rings and the engine state, which is
         * more than a 1 MB Windows stack wants several of at once, so these
         * instances live on the heap. */
        auto writer = std::make_unique<TypePAlphaAudioProcessor>();
        writer->setLibraryFileForTesting(library);
        writer->presets().savePart(
            9, "persisted", std::vector<int32_t>(
                               static_cast<std::size_t>(kP1PartParamCount), 5));
        writer->saveLibrary();

        auto reader = std::make_unique<TypePAlphaAudioProcessor>();
        reader->setLibraryFileForTesting(library);
        reader->ensureLibraryLoaded();
        check(reader->presets().partUsed(9) &&
                  reader->presets().partName(9) == "persisted",
              "the global library restores saved parts");

        /* A second instance adds a slot after the reader loaded its copy. The
         * reader then restores a project whose snapshot predates that slot. The
         * project must not write its old bank over the shared library. */
        auto second = std::make_unique<TypePAlphaAudioProcessor>();
        second->setLibraryFileForTesting(library);
        second->ensureLibraryLoaded();
        second->presets().savePart(
            10, "second", std::vector<int32_t>(
                              static_cast<std::size_t>(kP1PartParamCount), 6));
        second->saveLibrary();

        juce::MemoryBlock project;
        reader->getStateInformation(project);
        reader->setStateInformation(project.getData(),
                                    static_cast<int>(project.getSize()));
        reader->presets().savePart(
            11, "reader", std::vector<int32_t>(
                              static_cast<std::size_t>(kP1PartParamCount), 7));
        reader->saveLibrary();

        auto merged = std::make_unique<TypePAlphaAudioProcessor>();
        merged->setLibraryFileForTesting(library);
        merged->ensureLibraryLoaded();
        check(merged->presets().partUsed(10) &&
                  merged->presets().partName(10) == "second",
              "a project load does not overwrite the shared library");
        check(merged->presets().partUsed(11),
              "the reader's own slot still reaches the library");
        library.deleteFile();
    }

    auto automatedOwner = std::make_unique<TypePAlphaAudioProcessor>();
    auto& automated = *automatedOwner;
    automated.setRateAndBufferSizeDetails(48000.0, kFrames);
    automated.prepareToPlay(48000.0, kFrames);
    auto* automatedParameter = automated.parameters().getParameter("algorithm.p0");
    if (automatedParameter != nullptr)
        automatedParameter->setValueNotifyingHost(
            automatedParameter->convertTo0to1(7.0f));
    juce::AudioBuffer<float> automatedBlock(2, kFrames);
    juce::MidiBuffer automatedMidi;
    automatedMidi.addEvent(
        juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
    automated.processBlock(automatedBlock, automatedMidi);
    renderReference(reference, kFrames, 0, 7);
    check(matches(automatedBlock, reference, kFrames),
          "automation applies at the 32-sample control block");

    auto offsetOwner = std::make_unique<TypePAlphaAudioProcessor>();
    auto& offset = *offsetOwner;
    offset.setRateAndBufferSizeDetails(48000.0, kFrames);
    offset.prepareToPlay(48000.0, kFrames);
    juce::AudioBuffer<float> offsetBlock(2, kFrames);
    juce::MidiBuffer offsetMidi;
    offsetMidi.addEvent(
        juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 100);
    offset.processBlock(offsetBlock, offsetMidi);
    renderReference(reference, kFrames, 96, 4);
    check(matches(offsetBlock, reference, kFrames),
          "a MIDI offset applies at its 32-sample control block");

    processor.setRateAndBufferSizeDetails(44100.0, kFrames);
    processor.prepareToPlay(44100.0, kFrames);
    check(processor.getLatencySamples() == 58,
          "44.1 kHz rate bridge reports its latency");
    juce::AudioBuffer<float> hostBlock(2, kFrames);
    juce::MidiBuffer hostMidi;
    hostMidi.addEvent(
        juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
    processor.processBlock(hostBlock, hostMidi);
    bool finite = true;
    bool sounding = false;
    for (int i = 0; i < kFrames; ++i) {
        const float sample = hostBlock.getSample(0, i);
        finite = finite && std::isfinite(sample);
        sounding = sounding || sample > 0.0f || sample < 0.0f;
    }
    check(finite && sounding, "the 44.1 kHz placeholder path renders audio");

    {
        /* A short note spans the same core ticks at every rate. The position is
         * scaled by the rate so the note lasts about 65 core samples, which is
         * three control ticks: the old non-48 kHz path fed every event before the
         * block and cancelled the note. */
        const double rates[] = {44100.0, 48000.0, 96000.0, 192000.0};
        for (const double rate : rates) {
            const int frames = 512;
            const int off = static_cast<int>(
                std::lround(65.0 * rate / 48000.0));
            auto shortNote = std::make_unique<TypePAlphaAudioProcessor>();
            shortNote->setRateAndBufferSizeDetails(rate, frames);
            shortNote->prepareToPlay(rate, frames);
            if (auto* attack = shortNote->parameters().getParameter("attack.p0"))
                attack->setValueNotifyingHost(attack->convertTo0to1(0.0f));
            juce::AudioBuffer<float> shortBlock(2, frames);
            juce::MidiBuffer shortMidi;
            shortMidi.addEvent(
                juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)),
                0);
            shortMidi.addEvent(juce::MidiMessage::noteOff(1, 60), off);
            shortNote->processBlock(shortBlock, shortMidi);
            bool ok = true;
            bool heard = false;
            for (int i = 0; i < frames; ++i) {
                const float sample = shortBlock.getSample(0, i);
                ok = ok && std::isfinite(sample);
                heard = heard || std::abs(sample) > 1.0e-6f;
            }
            const auto label = "a short note renders at " +
                               std::to_string(static_cast<int>(rate)) + " Hz";
            check(ok && heard, label.c_str());
        }
    }

    {
        /* A note-on and note-off in one 32-sample tick still feed both events,
         * and the block stays finite. The envelope's first tick is silent, so the
         * pair is inaudible by construction rather than cancelled before the
         * tick. */
        auto sameTick = std::make_unique<TypePAlphaAudioProcessor>();
        sameTick->setRateAndBufferSizeDetails(48000.0, kFrames);
        sameTick->prepareToPlay(48000.0, kFrames);
        juce::AudioBuffer<float> pairBlock(2, kFrames);
        juce::MidiBuffer pairMidi;
        pairMidi.addEvent(
            juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        pairMidi.addEvent(juce::MidiMessage::noteOff(1, 60), 1);
        sameTick->processBlock(pairBlock, pairMidi);
        bool pairFinite = true;
        for (int i = 0; i < kFrames; ++i)
            pairFinite = pairFinite && std::isfinite(pairBlock.getSample(0, i));
        check(pairFinite, "a same-tick pair leaves a finite block");
    }

    {
        const double first = processor.specMorph(0, 0);
        check(first >= -1.0 && first <= 1.0,
              "the live morph reads a published value");
        auto morph = std::make_unique<TypePAlphaAudioProcessor>();
        morph->setRateAndBufferSizeDetails(48000.0, kFrames);
        morph->prepareToPlay(48000.0, kFrames);
        check(morph->specMorph(0, 0) == -1.0,
              "the live morph starts unpublished");
        juce::AudioBuffer<float> morphBlock(2, kFrames);
        juce::MidiBuffer morphMidi;
        morphMidi.addEvent(
            juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        morph->processBlock(morphBlock, morphMidi);
        const double live = morph->specMorph(0, 0);
        check(live >= -1.0 && live <= 1.0,
              "the live morph stays in range after a render");
    }

    {
        /* The phase display reads a published copy, not engine state. A fresh
         * processor has no map; a held note with phase.on builds one and the
         * newest voice supplies the live dot. */
        auto phase = std::make_unique<TypePAlphaAudioProcessor>();
        phase->setRateAndBufferSizeDetails(48000.0, kFrames);
        phase->prepareToPlay(48000.0, kFrames);
        p1::PhaseMapSnapshot unpublished;
        check(!phase->phaseMap(0, 0, unpublished),
              "the live phase map starts unpublished");
        if (auto* on = phase->parameters().getParameter(
                p1::params::opPhase(0, "on", 0))) {
            on->setValueNotifyingHost(1.0f);
        }
        juce::AudioBuffer<float> phaseBlock(2, kFrames);
        juce::MidiBuffer phaseMidi;
        phaseMidi.addEvent(
            juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        phase->processBlock(phaseBlock, phaseMidi);
        p1::PhaseMapSnapshot live;
        const bool published = phase->phaseMap(0, 0, live);
        check(published && live.valid && live.active == live.segments &&
                  live.segments >= 1 && live.segments <= 32,
              "the live phase map publishes composed geometry");
        check(live.voices > 0, "the live phase map sees the held voice");
        check(live.phaseQ <= 4095 && live.phaseY <= 4095,
              "the live phase dot stays inside the map");
        p1::PhaseMapSnapshot otherPart;
        check(!phase->phaseMap(0, 1, otherPart),
              "the live phase map is per part");
    }

    {
        /* The bridge renders several core chunks per host block. The tap scope
         * must collect every chunk, not only the last one, or the first half of
         * the display window is zero and the trace breaks at each block edge. */
        juce::AudioBuffer<float> tapBlock(2, kFrames);
        juce::MidiBuffer empty;
        for (int i = 0; i < 8; ++i) {
            tapBlock.clear();
            processor.processBlock(tapBlock, empty);
        }
        std::array<float, 512> taps{};
        processor.readTapScope(8, taps.data(), static_cast<int>(taps.size()));
        auto energy = [&taps](int start) {
            double sum = 0.0;
            for (int i = 0; i < 256; ++i) {
                const double value = taps[static_cast<std::size_t>(start + i)];
                sum += value * value;
            }
            return sum / 256.0;
        };
        check(energy(0) > 1.0e-8 && energy(256) > 1.0e-8,
              "the 44.1 kHz tap scope fills the whole window");
    }

    {
        auto sourceOwner = std::make_unique<TypePAlphaAudioProcessor>();
    auto& source = *sourceOwner;
        auto restoredOwner = std::make_unique<TypePAlphaAudioProcessor>();
    auto& restored = *restoredOwner;
        for (int i = 0; i < kP1ParamCount; ++i) {
            const auto& spec = kP1ParamSpecs[static_cast<std::size_t>(i)];
            auto* sourceParameter = source.parameters().getParameter(spec.id);
            if (sourceParameter == nullptr)
                continue;
            const int middle = spec.min + (spec.max - spec.min) * 2 / 3;
            sourceParameter->setValueNotifyingHost(
                sourceParameter->convertTo0to1(static_cast<float>(middle)));
        }
        juce::MemoryBlock fullState;
        source.getStateInformation(fullState);
        restored.setStateInformation(fullState.getData(),
                                     static_cast<int>(fullState.getSize()));
        bool allRestored = true;
        for (const auto& spec : kP1ParamSpecs) {
            auto* actual =
                restored.parameters().getRawParameterValue(spec.id);
            const int middle = spec.min + (spec.max - spec.min) * 2 / 3;
            allRestored = allRestored && actual != nullptr &&
                          std::lround(actual->load()) == middle;
        }
        check(allRestored, "the state restores every manifest parameter");
    }

    {
        auto chordOwner = std::make_unique<TypePAlphaAudioProcessor>();
    auto& chord = *chordOwner;
        chord.setRateAndBufferSizeDetails(48000.0, kFrames);
        chord.prepareToPlay(48000.0, kFrames);
        juce::AudioBuffer<float> chordBlock(2, kFrames);
        juce::MidiBuffer chordMidi;
        chordMidi.addEvent(
            juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        chordMidi.addEvent(
            juce::MidiMessage::noteOn(1, 64, static_cast<juce::uint8>(100)), 0);
        chordMidi.addEvent(
            juce::MidiMessage::noteOn(1, 67, static_cast<juce::uint8>(100)), 0);
        std::vector<float> capture;
        for (int i = 0; i < 24; ++i) {
            chordBlock.clear();
            chord.processBlock(chordBlock, chordMidi);
            chordMidi.clear();
            for (int frame = 0; frame < kFrames; ++frame)
                capture.push_back(chordBlock.getSample(0, frame));
        }
        const int window = 4096;
        const int start = static_cast<int>(capture.size()) - window;
        auto tone = [&capture, start](double frequency) {
            constexpr double pi = 3.14159265358979323846;
            double sum = 0.0;
            double re = 0.0;
            double im = 0.0;
            for (int i = 0; i < window; ++i) {
                const double w = 0.5 - 0.5 * std::cos(2.0 * pi * i / (window - 1));
                const double phase = 2.0 * pi * frequency * i / 48000.0;
                const auto sample =
                    static_cast<double>(capture[static_cast<std::size_t>(start + i)]);
                sum += w;
                re += sample * w * std::cos(phase);
                im += sample * w * std::sin(phase);
            }
            return 2.0 * std::hypot(re, im) / sum;
        };
        const double c4 = tone(261.625565);
        const double e4 = tone(329.627557);
        const double g4 = tone(391.995436);
        const double strongest = std::max(c4, std::max(e4, g4));
        check(strongest > 1e-5 && c4 > strongest * 0.15 &&
                  e4 > strongest * 0.15 && g4 > strongest * 0.15,
              "a three-note chord renders all three notes");
        chord.releaseResources();
    }

    processor.releaseResources();
    std::printf("plugin: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
