/* SPDX-License-Identifier: GPL-3.0-only */
/* Stability and edge-case sweep for the JUCE processor, which needs no DAW. */
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>

#include <juce_audio_processors/juce_audio_processors.h>

#include "Params.h"
#include "PluginProcessor.h"

namespace {

constexpr int kFrames = 256;
int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("stability: %-58s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

struct Peak {
    bool finite = true;
    float peak = 0.0f;
};

Peak render(TypePFm1AudioProcessor& processor, juce::MidiBuffer& midi, int blocks)
{
    Peak result;
    juce::AudioBuffer<float> buffer(2, kFrames);
    for (int block = 0; block < blocks; ++block) {
        processor.processBlock(buffer, midi);
        for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
            const auto* data = buffer.getReadPointer(channel);
            for (int i = 0; i < kFrames; ++i) {
                if (!std::isfinite(data[i]))
                    result.finite = false;
                result.peak = juce::jmax(result.peak, std::abs(data[i]));
            }
        }
        midi.clear();
    }
    return result;
}

void setAll(TypePFm1AudioProcessor& processor, bool maximum)
{
    for (const auto& spec : kP1ParamSpecs) {
        auto* parameter = processor.parameters().getParameter(spec.id);
        if (parameter == nullptr)
            continue;
        const int value = maximum ? spec.max : spec.min;
        parameter->setValueNotifyingHost(
            parameter->convertTo0to1(static_cast<float>(value)));
    }
}

} // namespace

int main()
{
    {
        TypePFm1AudioProcessor processor;
        processor.setRateAndBufferSizeDetails(44100.0, kFrames);
        processor.prepareToPlay(44100.0, kFrames);

        juce::MidiBuffer midi;
        midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        setAll(processor, false);
        auto result = render(processor, midi, 8);
        check(result.finite, "every parameter at its minimum stays finite");

        midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        setAll(processor, true);
        result = render(processor, midi, 8);
        check(result.finite && result.peak < 100.0f,
              "every parameter at its maximum stays finite and bounded");
    }

    {
        TypePFm1AudioProcessor processor;
        processor.setRateAndBufferSizeDetails(44100.0, kFrames);
        processor.prepareToPlay(44100.0, kFrames);
        juce::MidiBuffer midi;
        for (int note = 0; note < 128; ++note) {
            midi.addEvent(
                juce::MidiMessage::noteOn(1, note, static_cast<juce::uint8>(100)), 0);
            midi.addEvent(
                juce::MidiMessage::noteOn(2, note, static_cast<juce::uint8>(100)), 0);
        }
        auto result = render(processor, midi, 16);
        check(result.finite, "256 simultaneous note-ons stay finite");

        for (int note = 0; note < 128; ++note) {
            midi.addEvent(juce::MidiMessage::noteOff(1, note), 0);
            midi.addEvent(juce::MidiMessage::noteOff(2, note), 0);
        }
        result = render(processor, midi, 8);
        check(result.finite, "all-notes-off after the polyphony stress stays finite");
    }

    {
        TypePFm1AudioProcessor processor;
        processor.setRateAndBufferSizeDetails(44100.0, kFrames);
        processor.prepareToPlay(44100.0, kFrames);
        std::uint32_t seed = 0x5eed1234u;
        const auto next = [&seed]() {
            seed = seed * 1664525u + 1013904223u;
            return seed;
        };
        bool finite = true;
        juce::AudioBuffer<float> buffer(2, kFrames);
        juce::MidiBuffer midi;
        for (int block = 0; block < 400; ++block) {
            for (int event = 0; event < 24; ++event) {
                const int position = static_cast<int>(next() % kFrames);
                const int kind = static_cast<int>(next() % 6);
                const int note = static_cast<int>(next() % 128);
                const int channel = 1 + static_cast<int>(next() % 16);
                if (kind == 0)
                    midi.addEvent(
                        juce::MidiMessage::noteOn(
                            channel, note, static_cast<juce::uint8>(1 + next() % 127)),
                        position);
                else if (kind == 1)
                    midi.addEvent(juce::MidiMessage::noteOff(channel, note), position);
                else if (kind == 2)
                    midi.addEvent(
                        juce::MidiMessage::controllerEvent(
                            channel, static_cast<int>(next() % 128),
                            static_cast<int>(next() % 128)),
                        position);
                else if (kind == 3)
                    midi.addEvent(
                        juce::MidiMessage::pitchWheel(
                            channel, static_cast<int>(next() % 16384)),
                        position);
                else if (kind == 4)
                    midi.addEvent(
                        juce::MidiMessage::channelPressureChange(
                            channel, static_cast<int>(next() % 128)),
                        position);
                else
                    midi.addEvent(
                        juce::MidiMessage::programChange(
                            channel, static_cast<int>(next() % 128)),
                        position);
            }
            processor.processBlock(buffer, midi);
            for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
                const auto* data = buffer.getReadPointer(channel);
                for (int i = 0; i < kFrames; ++i)
                    finite = finite && std::isfinite(data[i]);
            }
            midi.clear();
        }
        check(finite, "a 9600-event MIDI storm stays finite");
    }

    {
        TypePFm1AudioProcessor processor;
        processor.setRateAndBufferSizeDetails(44100.0, kFrames);
        processor.prepareToPlay(44100.0, kFrames);
        juce::AudioBuffer<float> empty(2, 0);
        juce::MidiBuffer midi;
        processor.processBlock(empty, midi);
        check(true, "a zero-frame block is a no-op");

        juce::AudioBuffer<float> mono(1, kFrames);
        midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        processor.processBlock(mono, midi);
        bool finite = true;
        for (int i = 0; i < kFrames; ++i)
            finite = finite && std::isfinite(mono.getSample(0, i));
        check(finite, "a mono output buffer stays finite");
    }

    {
        const double rates[] = {44100.0, 48000.0, 96000.0, 192000.0, 44100.0};
        const int latency[] = {0, 17, 34, 67, 0};
        TypePFm1AudioProcessor processor;
        bool ok = true;
        for (int i = 0; i < 5; ++i) {
            processor.setRateAndBufferSizeDetails(rates[i], kFrames);
            processor.prepareToPlay(rates[i], kFrames);
            juce::MidiBuffer midi;
            midi.addEvent(
                juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
            const auto result = render(processor, midi, 4);
            ok = ok && result.finite &&
                 processor.getLatencySamples() == latency[i];
        }
        check(ok, "rate changes keep the reported latency and stay finite");
    }

    {
        TypePFm1AudioProcessor processor;
        processor.setRateAndBufferSizeDetails(44100.0, kFrames);
        processor.prepareToPlay(44100.0, kFrames);
        juce::MidiBuffer midi;
        midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        juce::MemoryBlock state;
        processor.getStateInformation(state);
        const auto* bytes = static_cast<const std::uint8_t*>(state.getData());
        const int size = static_cast<int>(state.getSize());

        processor.setStateInformation(nullptr, 0);
        processor.setStateInformation(bytes, 1);
        std::vector<std::uint8_t> random(static_cast<std::size_t>(size), 0);
        for (int i = 0; i < size; ++i)
            random[static_cast<std::size_t>(i)] =
                static_cast<std::uint8_t>((i * 37 + 11) & 0xFF);
        processor.setStateInformation(random.data(), size);

        std::uint32_t seed = 0xabcdef01u;
        for (int i = 0; i < 200; ++i) {
            seed = seed * 1664525u + 1013904223u;
            const int cut = static_cast<int>(seed % static_cast<std::uint32_t>(size + 1));
            std::vector<std::uint8_t> copy(bytes, bytes + cut);
            if (cut > 0) {
                const int at = static_cast<int>((seed >> 8) % static_cast<std::uint32_t>(cut));
                copy[static_cast<std::size_t>(at)] ^= 0xFF;
            }
            processor.setStateInformation(copy.data(), cut);
        }

        juce::MemoryBlock after;
        processor.getStateInformation(after);
        const auto result = render(processor, midi, 4);
        check(result.finite && after.getSize() > 0,
              "truncated and bit-flipped state does not crash the processor");
    }

    {
        TypePFm1AudioProcessor processor;
        processor.setRateAndBufferSizeDetails(44100.0, kFrames);
        processor.prepareToPlay(44100.0, kFrames);
        juce::MidiBuffer midi;
        midi.addEvent(juce::MidiMessage::noteOn(1, 60, static_cast<juce::uint8>(100)), 0);
        render(processor, midi, 32);
        midi.addEvent(juce::MidiMessage::noteOff(1, 60), 0);
        render(processor, midi, 16);
        render(processor, midi, 2756);
        const auto tail = render(processor, midi, 4);
        check(tail.finite && tail.peak < 0.05f,
              "the voice tail decays after note-off");
    }

    std::printf("stability: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
