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
#include "MacroLaw.h"
#include "MacroTargets.h"
#include "ParamIDs.h"
#include "Params.h"
#include "PluginProcessor.h"
#include "VolumeLaw.h"
#include "Widgets.h"
#include "macro_target_ids.h"

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

    check(processor.getParameters().size() == kP1ParamCount + 1,
          "the APVTS exposes every manifest parameter and the output volume");
    {
        int macroAutomatable = 0;
        for (auto* parameter : processor.getParameters()) {
            if (parameter->getName(64).startsWith("macro") &&
                parameter->isAutomatable())
                ++macroAutomatable;
        }
        check(macroAutomatable == 6,
              "only the six macro values are automatable");
    }
    check(p1::macros::kTargetCount == 58,
          "the macro target table has 58 rows");
    check(std::string(p1::macros::kTargets.front().name) ==
                  "part 1 filter cutoff" &&
              std::string(p1::macros::kTargets.back().name) ==
                  "part 2 modulator breakpoint xs",
          "the macro target table starts and ends in order");
    {
        std::set<std::string> names;
        bool sectionsKnown = true;
        for (const auto& target : p1::macros::kTargets) {
            names.insert(target.name);
            bool found = false;
            for (const auto* section : p1::macros::kSections)
                if (std::string(section) == target.section)
                    found = true;
            sectionsKnown = sectionsKnown && found;
        }
        check(names.size() == p1::macros::kTargetCount,
              "the macro target names are unique");
        check(sectionsKnown, "every macro target has a listed section");
    }
    if (const auto* targetSpec = p1::ui::findSpec("macro1_target1")) {
        check(targetSpec->max == p1::macros::kTargetCount,
              "the target enum matches the table");
    } else {
        check(false, "the macro target spec is present");
    }
    check(std::abs(p1::macroOffset(50, 50) - 0.25) < 1.0e-12 &&
              std::abs(p1::macroOffset(-50, 50) + 0.25) < 1.0e-12,
          "the macro offset is the value times the range");
    check(std::abs(p1::macroEffective(20, 127, 45) - 65.0) < 1.0e-12 &&
              std::abs(p1::macroEffective(20, 0, 45) - 20.0) < 1.0e-12 &&
              p1::macroEffective(20, 127, -100) == -80.0,
          "the source adds its range around the knob");
    check(std::abs(p1::macroTargetNormalised(0.5, 0.25) - 0.75) < 1.0e-12 &&
              p1::macroTargetNormalised(0.9, 0.5) == 1.0 &&
              p1::macroTargetNormalised(0.1, -0.5) == 0.0,
          "the macro target clamps in the normalised domain");
    {
        auto parameterIndex = [](const char* id) {
            const auto* spec = p1::ui::findSpec(id);
            return spec != nullptr
                       ? static_cast<int>(spec - kP1ParamSpecs.data())
                       : -1;
        };
        check(p1::macros::isEnvRateMember(parameterIndex("attack.p0")) &&
                  p1::macros::isEnvRateMember(
                      parameterIndex("op1_fbenv_r1.p0")) &&
                  p1::macros::isEnvRateMember(
                      parameterIndex("op1_spec_env_r1.p0")) &&
                  !p1::macros::isEnvRateMember(
                      parameterIndex("op1_phase_env_r1.p0")),
              "the engine env rate set takes the per-voice envelopes");
        check(p1::macros::isLfoRateMember(parameterIndex("op1_trem_rate.p0")) &&
                  p1::macros::isLfoRateMember(
                      parameterIndex("op1_spec_lfo_rate.p0")) &&
                  !p1::macros::isLfoRateMember(
                      parameterIndex("op1_phase_lfo_rate.p0")),
              "the engine lfo rate set takes the per-voice LFOs");
        check(p1::macros::isLfoDepthMember(parameterIndex("op1_trem_depth.p0")) &&
                  p1::macros::isLfoDepthMember(
                      parameterIndex("op1_spec_lfo_depth.p0")) &&
                  !p1::macros::isLfoDepthMember(
                      parameterIndex("op1_phase_lfo_depth.p0")),
              "the engine lfo depth set takes the per-voice LFOs");
    }
    {
        /* A targeted destination reports its stored value at rest and the
         * effective value while the macro deviates. A parameter outside the
         * table reports neither. */
        auto specIndex = [](const char* id) {
            for (int i = 0; i < kP1ParamCount; ++i)
                if (std::strcmp(kP1ParamSpecs[static_cast<std::size_t>(i)].id,
                                id) == 0)
                    return i;
            return -1;
        };
        auto setParamValue = [&processor](const char* id, float value) {
            if (auto* parameter = processor.parameters().getParameter(id))
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(value));
        };
        const int cutoff = specIndex("cutoff.p0");
        const int ratio = specIndex("op1_ratio.p0");
        const double stored =
            processor.parameters().getRawParameterValue("cutoff.p0")->load();
        setParamValue("macro1", 20.0f);
        setParamValue("macro1_target1", 1.0f);
        setParamValue("macro1_range1", 50.0f);
        auto feedback = processor.macroFeedback(cutoff);
        check(feedback.targeted && feedback.hasEffective &&
                  feedback.effective > stored,
              "a targeted parameter reports its effective value");
        setParamValue("macro1", 0.0f);
        feedback = processor.macroFeedback(cutoff);
        check(feedback.targeted && !feedback.hasEffective &&
                  feedback.effective == stored,
              "a targeted parameter at rest reports no effective value");
        check(!processor.macroFeedback(ratio).targeted,
              "a parameter outside the table is not targeted");
        setParamValue("macro1_target1", 0.0f);
        setParamValue("macro1_range1", 0.0f);
    }
    {
        auto* rate = processor.parameters().getParameter("pitch_rate.p0");
        const float norm = rate != nullptr
            ? rate->convertTo0to1(447392.0f)
            : -1.0f;
        check(rate != nullptr && norm > 0.55f && norm < 0.70f,
              "the LFO rate is logarithmic on the knob");
        check(rate != nullptr && rate->convertTo0to1(0.0f) == 0.0f,
              "the LFO rate keeps zero as the off stop");
        check(p1::macros::kTargetLog[10],
              "the part 1 pitch LFO rate row is log");
    }
    if (const auto* cutoff = p1::ui::findSpec("cutoff.p0")) {
        const int cutoffIndex =
            static_cast<int>(cutoff - kP1ParamSpecs.data());
        check(p1::macros::kTargetMemberOffsets[1] -
                      p1::macros::kTargetMemberOffsets[0] ==
                  1 &&
                  p1::macros::kTargetMemberPool[0].id == cutoffIndex,
              "the first target resolves to the part 1 cutoff");
    } else {
        check(false, "the cutoff spec is present");
    }
    {
        const int last = p1::macros::kTargetCount - 1;
        check(p1::macros::kTargetMemberOffsets[static_cast<std::size_t>(last) + 1] -
                      p1::macros::kTargetMemberOffsets[static_cast<std::size_t>(last)] ==
                  4 &&
                  p1::macros::kTargetRole[static_cast<std::size_t>(last)] == 2 &&
                  !p1::macros::kTargetLog[static_cast<std::size_t>(last)],
              "the last target is the modulator breakpoint group");
    }
    check(p1::volumeGainFromPosition(p1::kVolumeUnity) == 1.0f &&
              p1::volumeGainFromPosition(0) == 0.0f,
          "the volume is silent at the floor and unity at 0 dB");
    check(std::abs(p1::volumeDbFromPosition(1000) - 12.0) < 1.0e-9,
          "the volume reaches +12 dB at the top");
    check(p1::ui::volumeText(750) == "0.0 dB" &&
              p1::ui::volumePositionFromText("+6.0 dB") == 927,
          "the volume text agrees with the law");
    {
        /* The master fader trims every block, not only the block where the
         * parameter moves. A fresh instance per position keeps the comparison
         * free of a ramp from the previous value. */
        const auto renderVolume = [](int position) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            if (auto* volume =
                    instance->parameters().getParameter("output_volume")) {
                volume->setValueNotifyingHost(
                    volume->convertTo0to1(static_cast<float>(position)));
            }
            instance->setRateAndBufferSizeDetails(48000.0, kFrames);
            instance->prepareToPlay(48000.0, kFrames);
            juce::AudioBuffer<float> out(2, kFrames);
            juce::MidiBuffer events;
            events.addEvent(
                juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)),
                0);
            instance->processBlock(out, events);
            double energy = 0.0;
            for (int i = 0; i < kFrames; ++i)
                energy += std::abs(out.getSample(0, i));
            return energy;
        };
        const double silent = renderVolume(0);
        const double unity = renderVolume(p1::kVolumeUnity);
        const double loud = renderVolume(p1::kVolumePositions);
        check(silent < 1.0e-9 && unity > 1.0e-6 && loud > unity * 3.5,
              "the master volume scales every output block");
    }
    {
        /* A mono host must hear both sides of the pan. The 48 kHz path writes the
         * engine's two channels directly; the bridge path needs a real second
         * pointer rather than aliasing the mono buffer. */
        const auto renderPan = [](double rate, int pan, int channels) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            if (auto* panParameter =
                    instance->parameters().getParameter("pan.p0"))
                panParameter->setValueNotifyingHost(
                    panParameter->convertTo0to1(static_cast<float>(pan)));
            instance->setRateAndBufferSizeDetails(rate, 512);
            instance->prepareToPlay(rate, 512);
            juce::AudioBuffer<float> out(channels, 512);
            double energy = 0.0;
            for (int block = 0; block < 16; ++block) {
                out.clear();
                juce::MidiBuffer events;
                if (block == 0)
                    events.addEvent(
                        juce::MidiMessage::noteOn(1, 60,
                                                  static_cast<juce::uint8>(100)),
                        0);
                instance->processBlock(out, events);
                for (int channel = 0; channel < channels; ++channel)
                    for (int i = 0; i < 512; ++i)
                        energy += std::abs(out.getSample(channel, i));
            }
            return energy;
        };
        const double mono48Left = renderPan(48000.0, -100, 1);
        const double mono48Right = renderPan(48000.0, 100, 1);
        const double mono44Left = renderPan(44100.0, -100, 1);
        const double mono44Right = renderPan(44100.0, 100, 1);
        check(mono48Left > 1.0e-4 && mono48Right > 1.0e-4,
              "mono keeps a hard-left and hard-right part at 48 kHz");
        check(mono44Left > 1.0e-4 && mono44Right > 1.0e-4,
              "mono keeps a hard-left and hard-right part at 44.1 kHz");
        const double stereo48Left = renderPan(48000.0, -100, 2);
        check(mono48Left > stereo48Left * 0.4 &&
                  mono48Left < stereo48Left * 0.6,
              "the mono fold-down takes half of a hard-panned part");
    }

    {
        p1::ChunkNoteSet chunk;
        chunk.reset();
        chunk.noteOn(1, 60);
        check(chunk.held(1, 60),
              "the chunk note set keeps the channel and pitch");
        check(!chunk.held(2, 60),
              "a note on another channel does not hold the pitch");
        check(!chunk.held(1, 61), "a different pitch is not held");
        chunk.noteOn(16, 127);
        check(chunk.held(16, 127),
              "the last channel and pitch are both tracked");
        chunk.reset();
        check(!chunk.held(1, 60), "the chunk note set clears each block");
    }

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

    {
        const auto set = [](TypePAlphaAudioProcessor& target, const char* id,
                            int value) {
            if (auto* parameter = target.parameters().getParameter(id)) {
                parameter->setValueNotifyingHost(
                    parameter->convertTo0to1(static_cast<float>(value)));
            }
        };
        const auto sameBuffer = [](const juce::AudioBuffer<float>& a,
                                   const juce::AudioBuffer<float>& b) {
            for (int i = 0; i < a.getNumSamples(); ++i)
                if (!bitEqual(a.getSample(0, i), b.getSample(0, i)))
                    return false;
            return true;
        };
        const auto renderFirst = [&](int macro, int target = 35) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            instance->setRateAndBufferSizeDetails(48000.0, kFrames);
            instance->prepareToPlay(48000.0, kFrames);
            set(*instance, "macro1", macro);
            set(*instance, "macro1_target1", target);
            set(*instance, "macro1_range1", 50);
            check(std::lround(instance->parameters()
                                  .getRawParameterValue("macro1")
                                  ->load()) == macro &&
                      std::lround(instance->parameters()
                                      .getRawParameterValue("macro1_target1")
                                      ->load()) == target &&
                      std::lround(instance->parameters()
                                      .getRawParameterValue("macro1_range1")
                                      ->load()) == 50,
                  "the macro destination is set");
            juce::AudioBuffer<float> out(2, kFrames);
            juce::MidiBuffer events;
            events.addEvent(
                juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
            instance->processBlock(out, events);
            return out;
        };
        const auto macroAtZero = renderFirst(0);
        check(matches(macroAtZero, reference, kFrames),
              "a macro at 0 leaves the render bit-exact");
        const auto feedbackAtZero =
            renderFirst(0, P1_MACRO_TARGET_P1_OP_FEEDBACKS);
        check(matches(feedbackAtZero, reference, kFrames),
              "a feedback macro at 0 leaves the render bit-exact");
        const auto ratesAtZero =
            renderFirst(0, P1_MACRO_TARGET_P1_OP_ENV_RATES);
        check(matches(ratesAtZero, reference, kFrames),
              "an env rate macro at 0 leaves the render bit-exact");
        const auto depthsAtZero =
            renderFirst(0, P1_MACRO_TARGET_P1_OP_LFO_DEPTHS);
        check(matches(depthsAtZero, reference, kFrames),
              "an lfo depth macro at 0 leaves the render bit-exact");
        const auto macroAtFifty = renderFirst(50);
        check(!matches(macroAtFifty, reference, kFrames),
              "a macro moves its destination");

        const auto renderWithSource = [&](int source, int controller,
                                          int controllerValue, int velocity,
                                          int target = 35) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            instance->setRateAndBufferSizeDetails(48000.0, kFrames);
            instance->prepareToPlay(48000.0, kFrames);
            set(*instance, "macro1", 0);
            set(*instance, "macro1_target1", target);
            set(*instance, "macro1_range1", 50);
            set(*instance, "macro1_midi", source);
            set(*instance, "macro1_midi_range", 50);
            juce::AudioBuffer<float> out(2, kFrames);
            juce::MidiBuffer events;
            events.addEvent(
                juce::MidiMessage::noteOn(1, 60,
                                          static_cast<juce::uint8>(velocity)),
                0);
            if (controller >= 0) {
                events.addEvent(
                    juce::MidiMessage::controllerEvent(1, controller,
                                                       controllerValue),
                    0);
            }
            instance->processBlock(out, events);
            return out;
        };
        check(matches(renderWithSource(1, -1, 0, 100), reference, kFrames),
              "a cc source at rest leaves the macro at the knob");
        check(!matches(renderWithSource(1, 72, 127, 100), reference, kFrames),
              "the cc source moves the macro");
        check(!matches(renderWithSource(2, 1, 127, 100), reference, kFrames),
              "the wheel source moves the macro");
        check(!matches(renderWithSource(3, -1, 0, 100), reference, kFrames),
              "the velocity source moves the macro");
        check(!matches(renderWithSource(3, -1, 0, 100,
                                        P1_MACRO_TARGET_P1_OP_FEEDBACKS),
                       reference, kFrames),
              "the velocity source moves the feedback target");
        check(!matches(renderWithSource(3, -1, 0, 100,
                                        P1_MACRO_TARGET_P1_OP_ENV_RATES),
                       reference, kFrames),
              "the velocity source moves the env rate target");
        check(!matches(renderWithSource(3, -1, 0, 100,
                                        P1_MACRO_TARGET_P1_OP_LFO_DEPTHS),
                       reference, kFrames),
              "the velocity source moves the lfo depth target");
        check(!matches(renderWithSource(3, -1, 0, 100,
                                        P1_MACRO_TARGET_P1_CARRIER_LEVELS),
                       reference, kFrames),
              "the velocity source moves the carrier level target");

        /* The LFO rate is only audible through a depth, so this pair sets the
         * first operator's tremolo depth. */
        const auto renderLfo = [&](int velocity, bool targeted,
                                   int sourceRange = 50) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            instance->setRateAndBufferSizeDetails(48000.0, kFrames);
            instance->prepareToPlay(48000.0, kFrames);
            set(*instance, "op1_trem_depth.p0", 50);
            if (targeted) {
                set(*instance, "macro1", 0);
                set(*instance, "macro1_target1",
                    P1_MACRO_TARGET_P1_OP_LFO_RATES);
                set(*instance, "macro1_range1", 50);
                set(*instance, "macro1_midi", 3);
                set(*instance, "macro1_midi_range", sourceRange);
            }
            juce::AudioBuffer<float> out(2, kFrames);
            juce::MidiBuffer events;
            events.addEvent(
                juce::MidiMessage::noteOn(1, 60,
                                          static_cast<juce::uint8>(velocity)),
                0);
            instance->processBlock(out, events);
            return out;
        };
        const auto lfoReferenceHigh = renderLfo(100, false);
        const auto lfoReferenceLow = renderLfo(1, false);
        check(sameBuffer(renderLfo(1, true, 0), lfoReferenceLow),
              "an lfo rate macro at rest leaves the render bit-exact");
        check(!sameBuffer(renderLfo(100, true), lfoReferenceHigh),
              "the velocity source moves the lfo rate target");

        /* The morph only changes the sound in spectrum mode, so this pair sets
         * the waveform and compares two fresh instances. */
        const auto renderMorph = [&](int velocity, bool targeted,
                                     int sourceRange = 50) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            instance->setRateAndBufferSizeDetails(48000.0, kFrames);
            instance->prepareToPlay(48000.0, kFrames);
            set(*instance, "op1_waveform.p0", 13);
            if (targeted) {
                set(*instance, "macro1", 0);
                set(*instance, "macro1_target1",
                    P1_MACRO_TARGET_P1_OP_MORPHS);
                set(*instance, "macro1_range1", 50);
                set(*instance, "macro1_midi", 3);
                set(*instance, "macro1_midi_range", sourceRange);
            }
            juce::AudioBuffer<float> out(2, kFrames);
            juce::MidiBuffer events;
            events.addEvent(
                juce::MidiMessage::noteOn(1, 60,
                                          static_cast<juce::uint8>(velocity)),
                0);
            instance->processBlock(out, events);
            return out;
        };
        const auto morphReferenceHigh = renderMorph(100, false);
        const auto morphReferenceLow = renderMorph(1, false);
        check(sameBuffer(renderMorph(1, true, 0), morphReferenceLow),
              "a morph macro at rest leaves the render bit-exact");
        check(!sameBuffer(renderMorph(100, true), morphReferenceHigh),
              "the velocity source moves the morph target");

        /* Ring modulation is in the carrier sum, so the default pair state
         * matches the reference and each stage is checked on its own. */
        const auto renderRing = [&](int algorithm, int on, int level,
                                    int car2, int car4 = 1, int blocks = 1) {
            auto instance = std::make_unique<TypePAlphaAudioProcessor>();
            instance->setRateAndBufferSizeDetails(48000.0, kFrames);
            instance->prepareToPlay(48000.0, kFrames);
            set(*instance, "algorithm.p0", algorithm);
            set(*instance, "ring1_on.p0", on);
            set(*instance, "ring1_level.p0", level);
            set(*instance, "car2_out.p0", car2);
            set(*instance, "car4_out.p0", car4);
            juce::AudioBuffer<float> out(2, kFrames);
            for (int block = 0; block < blocks; ++block) {
                out.clear();
                juce::MidiBuffer events;
                if (block == 0) {
                    events.addEvent(juce::MidiMessage::noteOn(
                                        1, 60, static_cast<juce::uint8>(100)),
                                    0);
                }
                instance->processBlock(out, events);
            }
            return out;
        };
        std::array<int32_t, kFrames * 2> reference0{};
        std::array<int32_t, kFrames * 2> reference7{};
        renderReference(reference0.data(), kFrames, 0, 0);
        renderReference(reference7.data(), kFrames, 0, 7);
        check(matches(renderRing(4, 0, 99, 1), reference, kFrames),
              "the ring off leaves the render bit-exact");
        check(!matches(renderRing(4, 1, 99, 1), reference, kFrames),
              "a ring pair changes the render");
        check(matches(renderRing(4, 1, 0, 1), reference, kFrames),
              "a ring level of 0 adds nothing");
        check(matches(renderRing(4, 0, 99, 0), reference, kFrames),
              "the direct gates are ignored while the ring is off");
        check(!matches(renderRing(4, 1, 99, 0), reference, kFrames),
              "a direct gate applies while the ring is on");
        check(matches(renderRing(0, 1, 99, 1), reference0.data(), kFrames),
              "one carrier has no ring modulation");
        check(matches(renderRing(7, 0, 99, 1), reference7.data(), kFrames),
              "four carriers with the ring off match");
        check(!matches(renderRing(7, 1, 99, 1), reference7.data(), kFrames),
              "four carriers ring the first pair");
        {
            const auto rms = [](const juce::AudioBuffer<float>& buffer) {
                double sum = 0.0;
                for (int i = 0; i < buffer.getNumSamples(); ++i)
                    sum += static_cast<double>(buffer.getSample(0, i)) *
                           buffer.getSample(0, i);
                return std::sqrt(sum / buffer.getNumSamples());
            };
            const auto ringOnly = renderRing(4, 1, 99, 0, 0, 8);
            const auto directPair = renderRing(4, 0, 99, 1, 1, 8);
            const double ringLevel = rms(ringOnly);
            const double directLevel = rms(directPair);
            check(directLevel > 1.0e-6 && ringLevel > directLevel * 0.6 &&
                      ringLevel < directLevel * 1.6,
                  "the ring level 99 is in the ballpark of the direct pair");
        }
    }

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
            check(processor.getUiPage() == 8,
                  "the six-tab mod page moves to 8 on restore");

            tree.setProperty("uiPage", 5, nullptr);
            tree.setProperty("uiPageLayout", 3, nullptr);
            juce::MemoryBlock previous;
            processor.copyXmlToBinary(*tree.createXml(), previous);
            processor.setStateInformation(previous.getData(),
                                          static_cast<int>(previous.getSize()));
            check(processor.getUiPage() == 7,
                  "the layout-3 spectra b page moves to 7 on restore");

            tree.setProperty("uiPage", 5, nullptr);
            tree.setProperty("uiPageLayout", 4, nullptr);
            juce::MemoryBlock phaseLayout;
            processor.copyXmlToBinary(*tree.createXml(), phaseLayout);
            processor.setStateInformation(
                phaseLayout.getData(), static_cast<int>(phaseLayout.getSize()));
            check(processor.getUiPage() == 6,
                  "the layout-4 spectra b page moves to 6 on restore");

            tree.setProperty("uiPage", 3, nullptr);
            tree.removeProperty("uiPageLayout", nullptr);
            juce::MemoryBlock oldest;
            processor.copyXmlToBinary(*tree.createXml(), oldest);
            processor.setStateInformation(oldest.getData(),
                                          static_cast<int>(oldest.getSize()));
            check(processor.getUiPage() == 2,
                  "the four-tab fx page moves to 2 on restore");
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
        source.setMacroName(0, "macro test");
        source.getStateInformation(fullState);
        restored.setStateInformation(fullState.getData(),
                                     static_cast<int>(fullState.getSize()));
        check(restored.getMacroName(0) == "macro test",
              "the state restores a macro name");
        bool allRestored = true;
        for (const auto& spec : kP1ParamSpecs) {
            auto* actual =
                restored.parameters().getRawParameterValue(spec.id);
            const int middle = spec.min + (spec.max - spec.min) * 2 / 3;
            bool here = false;
            if (actual != nullptr) {
                const int raw = static_cast<int>(std::lround(actual->load()));
                const int difference = raw > middle ? raw - middle : middle - raw;
                /* The log rate parameters are floats over a 20,000,000 raw span,
                 * so the normalised state keeps them within a step or two. */
                here = difference <= (spec.logarithmic ? 4 : 0);
            }
            allRestored = allRestored && here;
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
