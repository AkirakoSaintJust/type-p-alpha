/* SPDX-License-Identifier: GPL-3.0-only */
/* Bipolar feedback: the positive half is the current path, the negative half is the
 * square family with the same average, and the envelope level moves the signed
 * feedback level toward or across zero. */
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
constexpr int kWindowStart = 8192;
constexpr int kWindow = 8192;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("bipolar: %-66s %s\n", what, ok ? "ok" : "FAIL");
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
        std::printf("bipolar: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool setup(p1::Engine& engine, int32_t feedback, int32_t envLevel)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 7);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_detune.p0", 0);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op1_feedback.p0", feedback);
    ok &= set_param(engine, "op1_fbenv_l1.p0", envLevel);
    ok &= set_param(engine, "op1_fbenv_l2.p0", envLevel);
    ok &= set_param(engine, "op1_fbenv_l3.p0", envLevel);
    ok &= set_param(engine, "op1_fbenv_l4.p0", envLevel);
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
    return ok;
}

struct Spectrum {
    std::array<double, 9> a{};
    double dc = 0.0;
    double hf = 0.0;
    double peak = 0.0;
};

double magnitudeAt(const std::vector<int32_t>& samples, double frequency,
                   const std::vector<double>& window, double windowSum)
{
    double re = 0.0;
    double im = 0.0;
    for (int i = 0; i < kWindow; ++i) {
        const double phase = 2.0 * kPi * frequency * i / kSampleRate;
        const double w = window[static_cast<std::size_t>(i)];
        const double x =
            samples[2u * static_cast<std::size_t>(kWindowStart + i)];
        re += x * w * std::cos(phase);
        im -= x * w * std::sin(phase);
    }
    return 2.0 * std::hypot(re, im) / windowSum;
}

Spectrum analyse(const std::vector<int32_t>& samples)
{
    std::vector<double> window(static_cast<std::size_t>(kWindow));
    double sum = 0.0;
    for (int i = 0; i < kWindow; ++i) {
        window[static_cast<std::size_t>(i)] =
            0.5 - 0.5 * std::cos(2.0 * kPi * i / (kWindow - 1));
        sum += window[static_cast<std::size_t>(i)];
    }

    Spectrum out;
    double mean = 0.0;
    for (int i = 0; i < kWindow; ++i)
        mean += samples[2u * static_cast<std::size_t>(kWindowStart + i)];
    out.dc = mean / kWindow;

    for (int h = 1; h <= 8; ++h)
        out.a[static_cast<std::size_t>(h)] =
            magnitudeAt(samples, h * kFundamental, window, sum);

    for (double frequency : {8000.0, 10000.0, 12000.0, 14000.0, 16000.0,
                             18000.0, 20000.0}) {
        out.hf += magnitudeAt(samples, frequency, window, sum);
    }

    int64_t peak = 0;
    for (int i = 0; i < kSamples; ++i) {
        const int32_t value = samples[2u * static_cast<std::size_t>(i)];
        const int64_t magnitude =
            value < 0 ? -static_cast<int64_t>(value) : static_cast<int64_t>(value);
        if (magnitude > peak)
            peak = magnitude;
    }
    out.peak = static_cast<double>(peak);
    return out;
}

bool render(int32_t feedback, int32_t envLevel, std::vector<int32_t>& samples)
{
    p1::Engine engine;
    engine.reset();
    if (!setup(engine, feedback, envLevel))
        return false;
    engine.noteOn(60, 100, 0);
    samples.assign(2u * static_cast<std::size_t>(kSamples), 0);
    engine.render(samples.data(), static_cast<uint32_t>(kSamples));
    return true;
}

double ratio(const Spectrum& s, int numerator, int denominator)
{
    return s.a[static_cast<std::size_t>(denominator)] > 1e-12
               ? s.a[static_cast<std::size_t>(numerator)] /
                     s.a[static_cast<std::size_t>(denominator)]
               : 0.0;
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("bipolar feedback experiment\n");
    std::printf("feedback   A1       A3/A1   A5/A1   dc      hf      peak\n");

    std::array<Spectrum, 4> positive{};
    std::array<Spectrum, 4> negative{};
    const std::array<int32_t, 4> levels{8192, 16384, 24576, 32767};
    for (std::size_t i = 0; i < levels.size(); ++i) {
        std::vector<int32_t> plus;
        std::vector<int32_t> minus;
        if (!render(levels[i], 0, plus) || !render(-levels[i], 0, minus)) {
            std::printf("bipolar: setup failed\n");
            return 1;
        }
        positive[i] = analyse(plus);
        negative[i] = analyse(minus);
        std::printf("%+9d   %6.0f  %6.3f  %6.3f  %6.1f  %6.1f  %6.0f\n",
                    levels[i], positive[i].a[1], ratio(positive[i], 3, 1),
                    ratio(positive[i], 5, 1), positive[i].dc, positive[i].hf,
                    positive[i].peak);
        std::printf("%+9d   %6.0f  %6.3f  %6.3f  %6.1f  %6.1f  %6.0f\n",
                    -levels[i], negative[i].a[1], ratio(negative[i], 3, 1),
                    ratio(negative[i], 5, 1), negative[i].dc, negative[i].hf,
                    negative[i].peak);
    }

    check(std::abs(ratio(positive[1], 3, 1) - 0.132) < 0.01 &&
              positive[1].hf > 700.0 && positive[1].hf < 1000.0,
          "the positive 50 percent half matches the current path");
    check(negative[1].hf > 400.0 && negative[1].hf < positive[1].hf,
          "the negative half is the unruly abs family");
    check(negative[3].hf > negative[1].hf,
          "the negative top end keeps the noise");
    check(std::abs(ratio(negative[1], 3, 1) - 0.391) < 0.03,
          "the negative 50 percent third harmonic matches the abs neg");

    std::vector<int32_t> envOff;
    std::vector<int32_t> envOn;
    if (!render(-16384, 0, envOff) || !render(-16384, 8192, envOn)) {
        std::printf("bipolar: setup failed\n");
        return 1;
    }
    const Spectrum off = analyse(envOff);
    const Spectrum on = analyse(envOn);
    check(std::abs(ratio(on, 3, 1) - ratio(off, 3, 1)) > 0.02 ||
              std::abs(on.hf - off.hf) > off.hf * 0.10,
          "the envelope level moves the negative magnitude");

    bool bounded = true;
    for (const auto& s : positive)
        bounded = bounded && s.peak < 200000.0;
    for (const auto& s : negative)
        bounded = bounded && s.peak < 200000.0;
    check(bounded, "the bipolar peaks stay bounded");

    std::printf("bipolar: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
