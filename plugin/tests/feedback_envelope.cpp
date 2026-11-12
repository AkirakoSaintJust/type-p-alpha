/* SPDX-License-Identifier: GPL-3.0-only */
/* Feedback envelope: each operator's internal fbenv node adds a signed offset to
 * that operator's signed feedback level. A flat envelope is bit-exact; positive
 * levels open the positive feedback family, and negative levels reduce the level
 * and cross into the negative family when the sum goes below zero. */
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"

namespace {

constexpr double kPi = 3.14159265358979323846;
constexpr double kSampleRate = 44100.0;
constexpr double kFundamental = 261.6255653005986; /* note 60 */
constexpr int kSamples = 16384;
constexpr int kWindow = 2048;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("envelope: %-64s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

uint32_t param_id(const char* name)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (std::strcmp(spec.id, name) == 0)
            return static_cast<uint32_t>(spec.engineId);
    }
    return ~0u;
}

bool set_param(p1::Engine& engine, const char* name, int32_t value)
{
    const uint32_t id = param_id(name);
    if (id == ~0u) {
        std::printf("envelope: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool setup(p1::Engine& engine, int32_t feedback, int32_t envL1, int32_t envR1,
           int32_t on)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 7);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_detune.p0", 0);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op1_feedback.p0", feedback);
    ok &= set_param(engine, "op1_mute.p0", 0);
    ok &= set_param(engine, "op2_level.p0", 0);
    ok &= set_param(engine, "op3_level.p0", 0);
    ok &= set_param(engine, "op4_level.p0", 0);
    ok &= set_param(engine, "op2_mute.p0", 1);
    ok &= set_param(engine, "op3_mute.p0", 1);
    ok &= set_param(engine, "op4_mute.p0", 1);
    ok &= set_param(engine, "attack.p0", 99);
    ok &= set_param(engine, "release.p0", 99);
    ok &= set_param(engine, "op1.eg.r2.p0", 99);
    ok &= set_param(engine, "op1.eg.r3.p0", 99);
    ok &= set_param(engine, "op1.eg.l1.p0", 99);
    ok &= set_param(engine, "op1.eg.l2.p0", 99);
    ok &= set_param(engine, "op1.eg.l3.p0", 99);
    ok &= set_param(engine, "op1_trem_depth.p0", 0);
    ok &= set_param(engine, "filter_on.p0", 0);
    ok &= set_param(engine, "op1_fbenv_on.p0", on);
    ok &= set_param(engine, "op1_fbenv_r1.p0", envR1);
    ok &= set_param(engine, "op1_fbenv_r2.p0", 99);
    ok &= set_param(engine, "op1_fbenv_r3.p0", 99);
    ok &= set_param(engine, "op1_fbenv_r4.p0", 99);
    ok &= set_param(engine, "op1_fbenv_l1.p0", envL1);
    ok &= set_param(engine, "op1_fbenv_l2.p0", envL1);
    ok &= set_param(engine, "op1_fbenv_l3.p0", envL1);
    ok &= set_param(engine, "op1_fbenv_l4.p0", envL1);
    return ok;
}

struct Window {
    std::array<double, 7> a{};
    double centroid = 0.0;
};

Window analyseWindow(const std::vector<int32_t>& samples, int start)
{
    std::vector<double> window(static_cast<std::size_t>(kWindow));
    double sum = 0.0;
    for (int i = 0; i < kWindow; ++i) {
        window[static_cast<std::size_t>(i)] =
            0.5 - 0.5 * std::cos(2.0 * kPi * i / (kWindow - 1));
        sum += window[static_cast<std::size_t>(i)];
    }

    Window out;
    double weighted = 0.0;
    double total = 0.0;
    for (int h = 1; h <= 6; ++h) {
        const double f = h * kFundamental;
        double re = 0.0;
        double im = 0.0;
        for (int i = 0; i < kWindow; ++i) {
            const double phase = 2.0 * kPi * f * i / kSampleRate;
            const double w = window[static_cast<std::size_t>(i)];
            const double x =
                samples[2u * static_cast<std::size_t>(start + i)];
            re += x * w * std::cos(phase);
            im -= x * w * std::sin(phase);
        }
        const double amplitude = 2.0 * std::hypot(re, im) / sum;
        out.a[static_cast<std::size_t>(h)] = amplitude;
        weighted += h * amplitude;
        total += amplitude;
    }
    out.centroid = total > 1e-12 ? weighted / total : 0.0;
    return out;
}

double peakOf(const std::vector<int32_t>& samples)
{
    int64_t peak = 0;
    for (int i = 0; i < kSamples; ++i) {
        const int32_t value = samples[2u * static_cast<std::size_t>(i)];
        const int64_t magnitude =
            value < 0 ? -static_cast<int64_t>(value) : static_cast<int64_t>(value);
        if (magnitude > peak)
            peak = magnitude;
    }
    return static_cast<double>(peak);
}

bool render(int32_t feedback, int32_t envL1, int32_t envR1, int32_t on,
            std::vector<int32_t>& samples, bool* negative = nullptr)
{
    p1::Engine engine;
    engine.reset();
    if (!setup(engine, feedback, envL1, envR1, on))
        return false;
    engine.noteOn(60, 100, 0);
    samples.assign(2u * static_cast<std::size_t>(kSamples), 0);
    engine.render(samples.data(), static_cast<uint32_t>(kSamples));
    if (negative != nullptr)
        *negative = engine.state()->op1_fb_negative[0] != 0;
    return true;
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("feedback envelope experiment\n");

    std::vector<int32_t> flat;
    std::vector<int32_t> bypassed;
    if (!render(0, 0, 50, 1, flat) || !render(0, 32767, 50, 0, bypassed)) {
        std::printf("envelope: setup failed\n");
        return 1;
    }
    check(std::memcmp(flat.data(), bypassed.data(),
                      2u * static_cast<std::size_t>(kSamples) *
                          sizeof(int32_t)) == 0,
          "a flat envelope and the off switch are bit-exact");

    std::vector<int32_t> partialNegative;
    std::vector<int32_t> steadyBase;
    if (!render(16384, -8192, 50, 1, partialNegative) ||
        !render(16384, 0, 50, 1, steadyBase)) {
        std::printf("envelope: setup failed\n");
        return 1;
    }
    const Window partialLast =
        analyseWindow(partialNegative, kSamples - kWindow);
    const Window steadyLast = analyseWindow(steadyBase, kSamples - kWindow);
    check(std::abs(partialLast.centroid - steadyLast.centroid) > 0.02 ||
              std::abs(partialLast.a[3] / partialLast.a[1] -
                       steadyLast.a[3] / steadyLast.a[1]) > 0.02,
          "a partial negative level changes an existing feedback amount");

    std::vector<int32_t> zeroNegative;
    std::vector<int32_t> zeroPositive;
    bool zeroNegativeFamily = false;
    bool zeroPositiveFamily = false;
    if (!render(0, -8192, 99, 1, zeroNegative, &zeroNegativeFamily) ||
        !render(0, 8192, 99, 1, zeroPositive, &zeroPositiveFamily)) {
        std::printf("envelope: setup failed\n");
        return 1;
    }
    check(zeroNegativeFamily && !zeroPositiveFamily,
          "the envelope sign chooses the feedback family from a zero base");
    check(peakOf(zeroNegative) > 0.0,
          "a negative envelope from a zero base is audible");
    check(std::memcmp(zeroNegative.data(), zeroPositive.data(),
                      2u * static_cast<std::size_t>(kSamples) *
                          sizeof(int32_t)) != 0,
          "the negative and positive envelopes differ");

    std::vector<int32_t> plus;
    std::vector<int32_t> minus;
    std::vector<int32_t> staticFull;
    std::vector<int32_t> staticHalf;
    bool minusFamily = false;
    if (!render(0, 8192, 80, 1, plus) ||
        !render(16384, -32767, 50, 1, minus, &minusFamily) ||
        !render(32767, 0, 50, 1, staticFull) ||
        !render(16384, 0, 50, 1, staticHalf)) {
        std::printf("envelope: setup failed\n");
        return 1;
    }

    const Window plusFirst = analyseWindow(plus, 2048);
    const Window plusLast = analyseWindow(plus, kSamples - kWindow);
    const Window minusFirst = analyseWindow(minus, 2048);
    const Window minusLast = analyseWindow(minus, kSamples - kWindow);

    std::printf("envelope: +25 first centroid %.3f A3/A1 %.3f\n",
                plusFirst.centroid, plusFirst.a[3] / plusFirst.a[1]);
    std::printf("envelope: +25 last  centroid %.3f A3/A1 %.3f\n",
                plusLast.centroid, plusLast.a[3] / plusLast.a[1]);
    std::printf("envelope: -100 first centroid %.3f A3/A1 %.3f\n",
                minusFirst.centroid, minusFirst.a[3] / minusFirst.a[1]);
    std::printf("envelope: -100 last  centroid %.3f A3/A1 %.3f\n",
                minusLast.centroid, minusLast.a[3] / minusLast.a[1]);

    check(plusLast.centroid > plusFirst.centroid * 1.10 ||
              plusLast.a[3] / plusLast.a[1] >
                  plusFirst.a[3] / plusFirst.a[1] * 1.10,
          "positive levels brighten the last window");
    check(minusFamily,
          "a negative level crosses the positive base into the negative family");

    const double plusPeak = peakOf(plus);
    const double minusPeak = peakOf(minus);
    const double fullPeak = peakOf(staticFull);
    const double halfPeak = peakOf(staticHalf);
    std::printf("envelope: peaks +25 %.0f -100 %.0f full %.0f half %.0f\n",
                plusPeak, minusPeak, fullPeak, halfPeak);
    check(plusPeak <= fullPeak * 1.50,
          "the positive level peak stays within 50 percent");
    check(minusPeak <= halfPeak * 1.50,
          "the negative level peak stays within 50 percent");

    std::printf("envelope: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
