/* SPDX-License-Identifier: GPL-3.0-only */
/* The additive path: the spectrum waveform, the a/b morph, the morph envelope
 * and the morph LFO. The test renders one clean carrier and measures the
 * fundamental and the second harmonic. */
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"
#include "p1_math.h"

namespace {

constexpr double kRate = 48000.0;
constexpr double kPi = 3.14159265358979323846;
constexpr int kFrames = 44100;
constexpr int kWindow = 4096;
constexpr double kF0 = 261.6255653005986; // MIDI 60 in equal temperament.

bool setParam(p1::Engine& engine, const char* id, int32_t value)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (std::strcmp(spec.id, id) == 0) {
            engine.setParam(static_cast<uint32_t>(spec.engineId), value);
            return true;
        }
    }
    std::printf("spectral: missing parameter %s\n", id);
    return false;
}

bool setAll(p1::Engine& engine, const std::vector<std::pair<const char*, int32_t>>& values)
{
    for (const auto& [id, value] : values) {
        if (!setParam(engine, id, value))
            return false;
    }
    return true;
}

double magnitude(const std::vector<int32_t>& samples, int start, int count,
                 double frequency)
{
    double re = 0.0;
    double im = 0.0;
    double weight_sum = 0.0;
    for (int i = 0; i < count; ++i) {
        const double weight =
            0.5 - 0.5 * std::cos(2.0 * kPi * static_cast<double>(i) /
                                 static_cast<double>(count - 1));
        const double phase =
            2.0 * kPi * frequency * static_cast<double>(i) / kRate;
        const double value = static_cast<double>(samples[static_cast<std::size_t>(
            (start + i) * 2)]);
        re += weight * value * std::cos(phase);
        im -= weight * value * std::sin(phase);
        weight_sum += weight;
    }
    return std::sqrt(re * re + im * im) / weight_sum;
}

bool setupCarrier(p1::Engine& engine)
{
    return setAll(engine, {
        {"algorithm.p0", 7},
        {"level.p1", 0},
        {"filter_on.p0", 0},
        {"op1_mute.p0", 0},
        {"op1_level.p0", 99},
        {"op1_feedback.p0", 0},
        {"op1_waveform.p0", 13},
        {"op2_mute.p0", 1}, {"op3_mute.p0", 1}, {"op4_mute.p0", 1},
        {"op1_spec_morph.p0", 0},
        {"op1_spec_env_depth.p0", 0},
        {"op1_spec_lfo_depth.p0", 0},
    });
}

std::vector<int32_t> render(p1::Engine& engine, int frames = kFrames)
{
    std::vector<int32_t> output(static_cast<std::size_t>(frames) * 2u, 0);
    engine.render(output.data(), static_cast<uint32_t>(frames));
    return output;
}

bool checkMorph()
{
    p1::Engine engine;
    engine.reset();
    if (!setupCarrier(engine))
        return false;
    if (!setAll(engine, {
            {"op1_spec_a1.p0", 0}, {"op1_spec_a2.p0", 99},
            {"op1_spec_b1.p0", 99}, {"op1_spec_b2.p0", 0},
        })) {
        return false;
    }
    engine.noteOn(60, 100, 0);
    const auto a = render(engine);
    const double a1 = magnitude(a, kFrames - kWindow, kWindow, kF0);
    const double a2 = magnitude(a, kFrames - kWindow, kWindow, 2.0 * kF0);
    if (!(a2 > a1 * 4.0)) {
        std::printf("spectral: spectrum a is not the second harmonic (h1 %.2f, h2 %.2f)\n",
                    a1, a2);
        return false;
    }

    if (!setParam(engine, "op1_spec_morph.p0", 99))
        return false;
    const auto b = render(engine);
    const double b1 = magnitude(b, kFrames - kWindow, kWindow, kF0);
    const double b2 = magnitude(b, kFrames - kWindow, kWindow, 2.0 * kF0);
    if (!(b1 > b2 * 4.0)) {
        std::printf("spectral: spectrum b is not the fundamental (h1 %.2f, h2 %.2f)\n",
                    b1, b2);
        return false;
    }
    return true;
}

bool checkMorphEnvelope()
{
    p1::Engine engine;
    engine.reset();
    if (!setupCarrier(engine))
        return false;
    if (!setAll(engine, {
            {"op1_spec_a1.p0", 99}, {"op1_spec_a2.p0", 0},
            {"op1_spec_b1.p0", 0}, {"op1_spec_b2.p0", 99},
            {"op1_spec_morph.p0", 0},
            {"op1_spec_env_r1.p0", 99}, {"op1_spec_env_r2.p0", 99},
            {"op1_spec_env_r3.p0", 99},
            {"op1_spec_env_l1.p0", 99}, {"op1_spec_env_l2.p0", 99},
            {"op1_spec_env_l3.p0", 99},
            {"op1_spec_env_depth.p0", 99},
        })) {
        return false;
    }
    engine.noteOn(60, 100, 0);
    const auto out = render(engine, 22050);
    const double h1 = magnitude(out, 22050 - 2048, 2048, kF0);
    const double h2 = magnitude(out, 22050 - 2048, 2048, 2.0 * kF0);
    if (!(h2 > h1 * 2.0)) {
        std::printf("spectral: the morph envelope did not reach b (h1 %.2f, h2 %.2f)\n",
                    h1, h2);
        return false;
    }

    p1::Engine base_engine;
    base_engine.reset();
    if (!setupCarrier(base_engine))
        return false;
    if (!setAll(base_engine, {
            {"op1_spec_a1.p0", 99}, {"op1_spec_a2.p0", 0},
            {"op1_spec_b1.p0", 0}, {"op1_spec_b2.p0", 99},
            {"op1_spec_morph.p0", 0},
            {"op1_spec_env_depth.p0", 0},
        })) {
        return false;
    }
    base_engine.noteOn(60, 100, 0);
    const auto base = render(base_engine, 22050);
    const double b1 = magnitude(base, 22050 - 2048, 2048, kF0);
    const double b2 = magnitude(base, 22050 - 2048, 2048, 2.0 * kF0);
    if (!(b2 < b1 * 0.5)) {
        std::printf("spectral: depth zero still moves the morph (h1 %.2f, h2 %.2f)\n",
                    b1, b2);
        return false;
    }
    return true;
}

bool checkMorphLfo()
{
    p1::Engine engine;
    engine.reset();
    if (!setupCarrier(engine))
        return false;
    if (!setAll(engine, {
            {"op1_spec_a1.p0", 99}, {"op1_spec_a2.p0", 0},
            {"op1_spec_b1.p0", 0}, {"op1_spec_b2.p0", 99},
            {"op1_spec_morph.p0", 0},
            {"op1_spec_lfo_depth.p0", 50},
            {"op1_spec_lfo_wave.p0", 0},
            {"op1_spec_lfo_mode.p0", 1},
        })) {
        return false;
    }
    engine.noteOn(60, 100, 0);
    const auto out = render(engine, 22050);
    double high = 0.0;
    double low = 1e30;
    for (int start = 4096; start + 2048 <= 22050; start += 2048) {
        const double h2 = magnitude(out, start, 2048, 2.0 * kF0);
        high = std::max(high, h2);
        low = std::min(low, h2);
    }
    if (!(high > low * 1.5)) {
        std::printf("spectral: the morph LFO is flat (low %.2f, high %.2f)\n", low, high);
        return false;
    }
    return true;
}

} // namespace

bool checkTable()
{
    int16_t bank[P1_SPEC_BANK] = {0};
    int32_t a[16] = {99};
    p1_spectral_table(bank + P1_SPEC_SHADOW(0), a, 1);
    p1_spectral_copy(bank + P1_SPEC_A(0), bank + P1_SPEC_SHADOW(0));
    const int32_t v = p1_spectral_read(bank, 0, 0x20000000u, 0, 0);

    int16_t shifted[P1_SPEC_BANK] = {0};
    p1_spectral_table(shifted + P1_SPEC_SHADOW(0), a, 17);
    const bool differs =
        std::memcmp(shifted + P1_SPEC_SHADOW(0),
                    bank + P1_SPEC_SHADOW(0),
                    P1_SPEC_TABLE * sizeof(int16_t)) != 0;
    const bool hashesDiffer = p1_spectral_hash(a, 1) != p1_spectral_hash(a, 17);
    if (v <= 1000 || !differs || !hashesDiffer) {
        std::printf("spectral: the range did not move the table (v %d, differs %d, hash %d)\n",
                    v, differs, hashesDiffer);
        return false;
    }
    return true;
}

bool checkMip()
{
    const uint32_t step = static_cast<uint32_t>(
        std::llround(kF0 / kRate * 4294967296.0));
    const uint32_t mip = p1_wave_mip(step);
    if (mip != 2) {
        std::printf("spectral: note 60 selects mip %u, expected 2 for Nyquist/2.25\n",
                    mip);
        return false;
    }
    return true;
}

bool checkRange()
{
    p1::Engine engine;
    engine.reset();
    if (!setupCarrier(engine))
        return false;
    if (!setAll(engine, {
            {"op1_spec_range_a.p0", 1},
            {"op1_spec_a1.p0", 99}, {"op1_spec_a2.p0", 0},
            {"op1_spec_b1.p0", 0}, {"op1_spec_b2.p0", 0},
            {"op1_spec_morph.p0", 0},
        })) {
        return false;
    }
    engine.noteOn(60, 100, 0);
    const auto out = render(engine, 22050);
    const double h1 = magnitude(out, 22050 - 2048, 2048, kF0);
    const double h17 = magnitude(out, 22050 - 2048, 2048, 17.0 * kF0);
    if (!(h17 > 100.0 && h17 > h1 * 4.0)) {
        std::printf("spectral: range 17-32 did not move the partial (h1 %.2f, h17 %.2f)\n",
                    h1, h17);
        return false;
    }
    return true;
}

int main()
{
    if (!checkTable() || !checkMip() || !checkRange())
        return 1;
    if (!checkMorph() || !checkMorphEnvelope() || !checkMorphLfo())
        return 1;
    std::printf("spectral: spectrum mode, range, mip, morph, envelope and LFO pass\n");
    return 0;
}
