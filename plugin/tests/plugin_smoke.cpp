/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 2 to 6: the JUCE processor smoke, state and parameter test, which
 * needs no DAW. */
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <set>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Engine.hpp"
#include "Params.h"
#include "PluginProcessor.h"

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
    TypePAlphaAudioProcessor processor;

    check(processor.getParameters().size() == kP1ParamCount,
          "the APVTS exposes every manifest parameter");
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

    processor.setRateAndBufferSizeDetails(44100.0, kFrames);
    processor.prepareToPlay(44100.0, kFrames);
    check(processor.getLatencySamples() == 0,
          "44.1 kHz bypass reports zero latency");

    juce::AudioBuffer<float> block(2, kFrames);
    juce::MidiBuffer midi;
    midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
    processor.processBlock(block, midi);

    int32_t reference[kFrames * 2] = {};
    renderReference(reference, kFrames, 0, 4);
    check(matches(block, reference, kFrames), "44.1 kHz output is bit-exact with the engine");

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
        processor.presets().setCurrentMulti(-1);
        processor.presets().setPartSource(0, -1);
        processor.setStateInformation(state.getData(),
                                      static_cast<int>(state.getSize()));
        check(processor.presets().partName(3) == "smoke",
              "the state restores the part bank");
        check(processor.presets().multiUsed(2) &&
                  processor.presets().multiName(2) == "smoke multi",
              "the state restores the multi bank");
        check(processor.presets().currentMulti() == 2 &&
                  processor.presets().partSource(0) == 3,
              "the state restores the current patch and part source");
    }

    {
        auto library = juce::File::getSpecialLocation(
                           juce::File::tempDirectory)
                           .getNonexistentChildFile("p1-library", ".p1lib", false);
        TypePAlphaAudioProcessor writer;
        writer.setLibraryFileForTesting(library);
        writer.presets().savePart(
            9, "persisted", std::vector<int32_t>(
                               static_cast<std::size_t>(kP1PartParamCount), 5));
        writer.saveLibrary();

        TypePAlphaAudioProcessor reader;
        reader.setLibraryFileForTesting(library);
        reader.ensureLibraryLoaded();
        check(reader.presets().partUsed(9) &&
                  reader.presets().partName(9) == "persisted",
              "the global library restores saved parts");
        library.deleteFile();
    }

    TypePAlphaAudioProcessor automated;
    automated.setRateAndBufferSizeDetails(44100.0, kFrames);
    automated.prepareToPlay(44100.0, kFrames);
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

    TypePAlphaAudioProcessor offset;
    offset.setRateAndBufferSizeDetails(44100.0, kFrames);
    offset.prepareToPlay(44100.0, kFrames);
    juce::AudioBuffer<float> offsetBlock(2, kFrames);
    juce::MidiBuffer offsetMidi;
    offsetMidi.addEvent(
        juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 100);
    offset.processBlock(offsetBlock, offsetMidi);
    renderReference(reference, kFrames, 96, 4);
    check(matches(offsetBlock, reference, kFrames),
          "a MIDI offset applies at its 32-sample control block");

    processor.setRateAndBufferSizeDetails(48000.0, kFrames);
    processor.prepareToPlay(48000.0, kFrames);
    check(processor.getLatencySamples() == 17,
          "48 kHz rate bridge reports its latency");
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
    check(finite && sounding, "the 48 kHz placeholder path renders audio");

    {
        TypePAlphaAudioProcessor source;
        TypePAlphaAudioProcessor restored;
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

    processor.releaseResources();
    std::printf("plugin: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
