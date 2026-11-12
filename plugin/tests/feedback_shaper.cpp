/* SPDX-License-Identifier: GPL-3.0-only */
/* Feedback shaper experiment: measure the square, abs and signed-square feedback
 * shapes, the encoded polarity, and the one-pole filter against the two-sample
 * average. The table is the evidence for the shaper contract. */
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
    std::printf("shaper: %-66s %s\n", what, ok ? "ok" : "FAIL");
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
        std::printf("shaper: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool setup(p1::Engine& engine, int32_t feedback, int32_t shape, int32_t filter)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 7);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_detune.p0", 0);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op1_feedback.p0", feedback);
    ok &= set_param(engine, "op1_fb_depth.p0", 0);
    ok &= set_param(engine, "op1_fb_shape.p0", shape);
    ok &= set_param(engine, "op1_fb_filter.p0", filter);
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

bool render(int32_t feedback, int32_t shape, int32_t filter,
            std::vector<int32_t>& samples)
{
    p1::Engine engine;
    engine.reset();
    if (!setup(engine, feedback, shape, filter))
        return false;
    engine.noteOn(60, 100, 0);
    samples.assign(2u * static_cast<std::size_t>(kSamples), 0);
    engine.render(samples.data(), static_cast<uint32_t>(kSamples));
    return true;
}

const char* shape_name(int shape)
{
    static const char* const names[7] = {"off", "square", "square neg", "abs",
                                         "abs neg", "signed", "signed neg"};
    return shape >= 0 && shape < 7 ? names[shape] : "?";
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
    std::printf("feedback shaper experiment\n");
    std::printf("feedback shape        filt   A1     A3/A1   A5/A1   dc      hf      peak\n");

    std::array<Spectrum, 7> at50{};
    std::array<Spectrum, 7> at75{};
    std::array<Spectrum, 7> at50lp{};
    std::array<Spectrum, 7> at75lp{};
    for (int shape = 0; shape < 7; ++shape) {
        std::vector<int32_t> average;
        std::vector<int32_t> onePole;
        if (!render(16384, shape, 0, average) ||
            !render(16384, shape, 1, onePole)) {
            std::printf("shaper: setup failed\n");
            return 1;
        }
        at50[static_cast<std::size_t>(shape)] = analyse(average);
        at50lp[static_cast<std::size_t>(shape)] = analyse(onePole);

        std::vector<int32_t> average75;
        std::vector<int32_t> onePole75;
        if (!render(24576, shape, 0, average75) ||
            !render(24576, shape, 1, onePole75)) {
            std::printf("shaper: setup failed\n");
            return 1;
        }
        at75[static_cast<std::size_t>(shape)] = analyse(average75);
        at75lp[static_cast<std::size_t>(shape)] = analyse(onePole75);

        for (int filter = 0; filter < 2; ++filter) {
            const Spectrum& s = filter == 0
                                    ? at50[static_cast<std::size_t>(shape)]
                                    : at50lp[static_cast<std::size_t>(shape)];
            std::printf("%8d %-12s %-5s %6.0f %7.3f %7.3f %7.1f %7.1f %8.0f\n",
                        50, shape_name(shape), filter == 0 ? "avg" : "lp",
                        s.a[1], ratio(s, 3, 1), ratio(s, 5, 1), s.dc, s.hf,
                        s.peak);
        }
    }

    check(at50[0].hf > 0.0 && at50[0].peak > 0.0,
          "the off shape renders a signal");
    check(std::abs(at50[1].hf - at50[0].hf) > at50[0].hf * 0.10 ||
              std::abs(ratio(at50[1], 3, 1) - ratio(at50[0], 3, 1)) >
                  ratio(at50[0], 3, 1) * 0.10,
          "the square shape changes the spectrum");
    check(std::abs(at50[3].hf - at50[0].hf) > at50[0].hf * 0.10 ||
              std::abs(ratio(at50[3], 3, 1) - ratio(at50[0], 3, 1)) >
                  ratio(at50[0], 3, 1) * 0.10,
          "the abs shape changes the spectrum");

    const bool squareSign =
        std::abs(at50[2].hf - at50[1].hf) > at50[1].hf * 0.10 ||
        std::abs(ratio(at50[2], 3, 1) - ratio(at50[1], 3, 1)) >
            ratio(at50[1], 3, 1) * 0.10;
    const bool absSign =
        std::abs(at50[4].hf - at50[3].hf) > at50[3].hf * 0.10 ||
        std::abs(ratio(at50[4], 3, 1) - ratio(at50[3], 3, 1)) >
            ratio(at50[3], 3, 1) * 0.10;
    const bool signedSign =
        std::abs(at50[6].hf - at50[5].hf) > at50[5].hf * 0.10 ||
        std::abs(ratio(at50[6], 3, 1) - ratio(at50[5], 3, 1)) >
            ratio(at50[5], 3, 1) * 0.10;
    std::printf("shaper: sign dependence square %d abs %d signed %d\n",
                squareSign ? 1 : 0, absSign ? 1 : 0, signedSign ? 1 : 0);
    check(squareSign || absSign || signedSign,
          "at least one negative form differs from its positive form");

    std::printf("shaper: 75%% abs average hf %.1f one-pole hf %.1f\n",
                at75[3].hf, at75lp[3].hf);
    check(at75lp[3].hf < at75[3].hf * 0.90,
          "the one-pole lowers the abs high-frequency energy");
    std::printf("shaper: 75%% square average hf %.1f one-pole hf %.1f\n",
                at75[1].hf, at75lp[1].hf);
    check(at75lp[1].hf < at75[1].hf * 0.90,
          "the one-pole lowers the square high-frequency energy");

    const bool oddDc = std::abs(at50[5].dc) < std::abs(at50[1].dc) &&
                       std::abs(at50[5].dc) < std::abs(at50[3].dc);
    std::printf("shaper: dc off %.1f square %.1f abs %.1f signed %.1f\n",
                at50[0].dc, at50[1].dc, at50[3].dc, at50[5].dc);
    check(oddDc, "the signed square carries less DC than the even shapes");

    bool bounded = true;
    for (const auto& s : at50)
        bounded = bounded && s.peak < 200000.0;
    for (const auto& s : at75)
        bounded = bounded && s.peak < 200000.0;
    check(bounded, "the shaper peaks stay bounded");

    std::printf("shaper: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
