/* SPDX-License-Identifier: GPL-3.0-only */
/* LFO rate and depth fades: the Q31 ramp lands on the envelope rate table's
 * time, equal fade values end together, the phase resets only when a fade is
 * on, and a negative depth keeps its sign. The node state is read directly for
 * the timing, then the tremolo is rendered to hear the ramp open. */
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"

namespace {

constexpr uint32_t kFull = 0x80000000u;
constexpr int kVoices = 24;
constexpr int kTickFrames = 32;
constexpr int kMaxTicks = 60000;
/* The envelope table's 99 percent times at 48 kHz, in control ticks. */
constexpr int kTicks1 = 50298;
constexpr int kTicks50 = 3210;
constexpr int kTicks70 = 727;
constexpr int kTicks90 = 79;
constexpr int kTicks98 = 12;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("fades: %-62s %s\n", what, ok ? "ok" : "FAIL");
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
        std::printf("fades: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool base_setup(p1::Engine& engine)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 9);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_detune.p0", 0);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op1_feedback.p0", 0);
    ok &= set_param(engine, "op1_mute.p0", 0);
    ok &= set_param(engine, "op2_level.p0", 0);
    ok &= set_param(engine, "op2_mute.p0", 1);
    ok &= set_param(engine, "op3_level.p0", 0);
    ok &= set_param(engine, "op3_mute.p0", 1);
    ok &= set_param(engine, "op4_level.p0", 0);
    ok &= set_param(engine, "op4_mute.p0", 1);
    ok &= set_param(engine, "attack.p0", 99);
    ok &= set_param(engine, "release.p0", 99);
    ok &= set_param(engine, "op1.eg.d1r.p0", 99);
    ok &= set_param(engine, "op1.eg.d2r.p0", 99);
    ok &= set_param(engine, "op1.eg.d1l.p0", 99);
    ok &= set_param(engine, "op1.eg.d2l.p0", 99);
    ok &= set_param(engine, "filter_on.p0", 0);
    ok &= set_param(engine, "op1_trem_wave.p0", 0);
    ok &= set_param(engine, "op1_trem_mode.p0", 1);
    ok &= set_param(engine, "op1_trem_depth.p0", 0);
    ok &= set_param(engine, "op1_trem_rate.p0", 100000);
    return ok;
}

bool trem_setup(p1::Engine& engine, int32_t depth, int32_t rateFade,
                int32_t depthFade)
{
    bool ok = base_setup(engine);
    ok &= set_param(engine, "op1_trem_rate.p0", 2000000);
    ok &= set_param(engine, "op1_trem_depth.p0", depth);
    ok &= set_param(engine, "op1_trem_rate_fade.p0", rateFade);
    ok &= set_param(engine, "op1_trem_depth_fade.p0", depthFade);
    return ok;
}

int voice_of(p1::Engine& engine, uint32_t note)
{
    for (int v = 0; v < kVoices; ++v) {
        if (engine.state()->voice_gate[v] &&
            engine.state()->voice_note[v] == note)
            return v;
    }
    return -1;
}

void tick(p1::Engine& engine)
{
    std::vector<int32_t> out(2 * kTickFrames, 0);
    engine.render(out.data(), kTickFrames);
}

void render(p1::Engine& engine, int frames, std::vector<int32_t>& pcm)
{
    pcm.assign(2u * static_cast<std::size_t>(frames), 0);
    engine.render(pcm.data(), static_cast<uint32_t>(frames));
}

struct Ramp {
    int rateEnd = -1;
    int depthEnd = -1;
    uint32_t rateFirst = 0;
    uint32_t depthFirst = 0;
};

bool run_ramp(int32_t rateFade, int32_t depthFade, Ramp& ramp)
{
    p1::Engine engine;
    engine.reset();
    if (!trem_setup(engine, 0, rateFade, depthFade))
        return false;
    engine.noteOn(60, 100, 0);
    const int v = voice_of(engine, 60);
    if (v < 0)
        return false;
    auto* st = engine.state();
    for (int t = 1; t <= kMaxTicks; ++t) {
        tick(engine);
        if (t == 1) {
            ramp.rateFirst = st->op1_lfo_rate_level[v];
            ramp.depthFirst = st->op1_lfo_depth_level[v];
        }
        if (ramp.rateEnd < 0 && st->op1_lfo_rate_level[v] >= kFull)
            ramp.rateEnd = t;
        if (ramp.depthEnd < 0 && st->op1_lfo_depth_level[v] >= kFull)
            ramp.depthEnd = t;
        if (ramp.rateEnd > 0 && ramp.depthEnd > 0)
            break;
    }
    return ramp.rateEnd > 0 && ramp.depthEnd > 0;
}

bool near(int value, int expected, int slack)
{
    return std::abs(value - expected) <= slack;
}

double rms(const std::vector<int32_t>& pcm, int startFrame, int frames)
{
    double sum = 0.0;
    for (int i = 0; i < frames; ++i) {
        const double x = pcm[2u * static_cast<std::size_t>(startFrame + i)];
        sum += x * x;
    }
    return std::sqrt(sum / frames);
}

double spread(const std::vector<int32_t>& pcm, int startFrame, int endFrame)
{
    std::vector<double> windows;
    constexpr int kWindow = 240; /* 5 ms */
    for (int f = startFrame; f + kWindow <= endFrame; f += kWindow)
        windows.push_back(rms(pcm, f, kWindow));
    if (windows.empty())
        return 0.0;
    double mean = 0.0;
    for (double w : windows)
        mean += w;
    mean /= static_cast<double>(windows.size());
    if (mean <= 0.0)
        return 0.0;
    double variance = 0.0;
    for (double w : windows) {
        const double d = (w - mean) / mean;
        variance += d * d;
    }
    return std::sqrt(variance / static_cast<double>(windows.size()));
}

bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)
{
    return a.size() == b.size() &&
           std::memcmp(a.data(), b.data(), a.size() * sizeof(int32_t)) == 0;
}

bool all_family_levels(p1::Engine& engine, uint32_t& rate, uint32_t& depth)
{
    engine.noteOn(60, 100, 0);
    const int v = voice_of(engine, 60);
    if (v < 0)
        return false;
    auto* st = engine.state();
    tick(engine);
    const uint32_t levels[10] = {
        st->op1_lfo_rate_level[v],     st->op1_lfo_depth_level[v],
        st->op1_fblfo_rate_level[v],   st->op1_fblfo_depth_level[v],
        st->op1_dlfo_rate_level[v],    st->op1_dlfo_depth_level[v],
        st->op1_speclfo_rate_level[v], st->op1_speclfo_depth_level[v],
        st->plfo_rate_level[v],        st->plfo_depth_level[v]};
    rate = levels[0];
    depth = levels[1];
    for (uint32_t level : levels) {
        if (level == 0 || level >= kFull)
            return false;
    }
    return true;
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("lfo fades\n");

    /* Off is the current render: the levels sit at full from the trigger and
     * the fade parameters do nothing. */
    Ramp off;
    check(run_ramp(0, 0, off),
          "the off ramp reaches full on the first tick");
    check(off.rateFirst == kFull && off.depthFirst == kFull &&
              off.rateEnd == 1 && off.depthEnd == 1,
          "fade 0 leaves both levels at full");

    Ramp instant;
    check(run_ramp(99, 99, instant), "fade 99 ramps");
    check(instant.rateFirst == kFull && instant.depthFirst == kFull &&
              instant.rateEnd == 1 && instant.depthEnd == 1,
          "fade 99 is instant");

    /* Equal values share the step and end on the same tick. */
    Ramp sync;
    check(run_ramp(50, 50, sync), "fade 50 ramps");
    check(near(sync.rateEnd, kTicks50, 1) && sync.rateEnd == sync.depthEnd,
          "equal rate and depth fades end on the same tick");
    check(sync.rateFirst == sync.depthFirst && sync.rateFirst < kFull / 1000u,
          "the first ramp tick is one table step");

    /* The shorter fade ends first and leaves the computed separation. */
    Ramp split;
    check(run_ramp(50, 70, split), "two different fades ramp");
    check(near(split.depthEnd, kTicks70, 1),
          "fade 70 lands on the table time");
    check(split.depthEnd < split.rateEnd &&
              near(split.rateEnd - split.depthEnd, kTicks50 - kTicks70, 2),
          "the two ends keep the computed separation");

    /* The table's slow end, where a Q16 step would have run long. */
    Ramp slow;
    check(run_ramp(1, 1, slow), "the slowest fade ramps");
    check(near(slow.rateEnd, kTicks1, 1) && slow.rateEnd == slow.depthEnd,
          "fade 1 lands on the 33.5 s table time");

    Ramp slowTwo;
    check(run_ramp(2, 2, slowTwo), "fade 2 ramps");
    check(slowTwo.rateEnd == slow.rateEnd,
          "fade 1 and fade 2 share the table time");

    Ramp fast;
    check(run_ramp(90, 98, fast), "the fast fades ramp");
    check(near(fast.rateEnd, kTicks90, 1) && near(fast.depthEnd, kTicks98, 1),
          "fades 90 and 98 land on the table time");

    /* The phase free-runs when both fades are off, and resets when one is on.
     * The allocated voice is triggered again by setting its trigger input, so
     * the check does not depend on the allocator picking a particular slot. */
    {
        p1::Engine engine;
        engine.reset();
        if (!trem_setup(engine, 0, 0, 0)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        const int v = voice_of(engine, 60);
        if (v < 0) {
            std::printf("fades: no voice allocated\n");
            return 1;
        }
        for (int i = 0; i < 10; ++i)
            tick(engine);
        auto* st = engine.state();
        st->op1_lfo_phase[v] = 0x40000000u;
        tick(engine);
        const uint32_t delta = st->op1_lfo_phase[v] - 0x40000000u;
        st->op1_lfo_phase[v] = 0x40000000u;
        st->voice_trig[v] = 1;
        tick(engine);
        check(st->op1_lfo_phase[v] - 0x40000000u == delta && delta > 0,
              "both fades off keep the phase free run");
    }
    {
        p1::Engine engine;
        engine.reset();
        if (!trem_setup(engine, 0, 50, 0)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        const int v = voice_of(engine, 60);
        if (v < 0) {
            std::printf("fades: no voice allocated\n");
            return 1;
        }
        auto* st = engine.state();
        st->op1_lfo_phase[v] = 0x40000000u;
        st->voice_trig[v] = 1;
        tick(engine);
        const uint32_t after = st->op1_lfo_phase[v];
        tick(engine);
        const uint32_t second = st->op1_lfo_phase[v] - after;
        check(after < 0x40000000u / 100u,
              "a fade on resets the phase to the start");
        check(second > after, "the rate fade speeds the phase up");
    }

    /* The level feeds the audible tremolo: the depth fade opens the modulation
     * and the rate fade stretches the first cycle. */
    {
        std::vector<int32_t> pcmOff;
        std::vector<int32_t> pcmDepth;
        std::vector<int32_t> pcmRate;
        p1::Engine engine;
        engine.reset();
        if (!trem_setup(engine, 60, 0, 0)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 57600, pcmOff);

        engine.reset();
        if (!trem_setup(engine, 60, 0, 70)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 57600, pcmDepth);

        engine.reset();
        if (!trem_setup(engine, 60, 70, 0)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 57600, pcmRate);

        const int earlyStart = 960;   /* 20 ms */
        const int earlyEnd = 5760;    /* 120 ms */
        const int lateStart = 48000;  /* 1.0 s */
        const int lateEnd = 52800;    /* 1.1 s */
        const double offEarly = spread(pcmOff, earlyStart, earlyEnd);
        const double offLate = spread(pcmOff, lateStart, lateEnd);
        const double depthEarly = spread(pcmDepth, earlyStart, earlyEnd);
        const double depthLate = spread(pcmDepth, lateStart, lateEnd);
        const double rateEarly = spread(pcmRate, earlyStart, earlyEnd);
        const double rateLate = spread(pcmRate, lateStart, lateEnd);
        std::printf(
            "fades: spread off %.3f/%.3f depth %.3f/%.3f rate %.3f/%.3f\n",
            offEarly, offLate, depthEarly, depthLate, rateEarly, rateLate);
        check(offEarly > offLate * 0.5,
              "the unfaded tremolo has the same spread early and late");
        check(depthEarly < depthLate * 0.5,
              "the depth fade opens the tremolo");
        check(rateEarly < rateLate * 0.5,
              "the rate fade stretches the first cycle");
    }

    /* A negative depth keeps its sign through the depth ramp. */
    {
        std::vector<int32_t> positive;
        std::vector<int32_t> negative;
        p1::Engine engine;
        engine.reset();
        if (!trem_setup(engine, 60, 50, 50)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 48000, positive);
        engine.reset();
        if (!trem_setup(engine, -60, 50, 50)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 48000, negative);
        check(!same(positive, negative),
              "a negative depth survives the depth fade");
    }

    /* All five families take the trigger and the two levels. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        const char* const pairs[5][2] = {
            {"op1_trem_rate_fade.p0", "op1_trem_depth_fade.p0"},
            {"op1_fblfo_rate_fade.p0", "op1_fblfo_depth_fade.p0"},
            {"op1_dlfo_rate_fade.p0", "op1_dlfo_depth_fade.p0"},
            {"op1_spec_lfo_rate_fade.p0", "op1_spec_lfo_depth_fade.p0"},
            {"pitch_rate_fade.p0", "pitch_depth_fade.p0"}};
        bool ok = true;
        for (const auto& pair : pairs) {
            ok &= set_param(engine, pair[0], 50);
            ok &= set_param(engine, pair[1], 50);
        }
        uint32_t rate = 0;
        uint32_t depth = 0;
        const bool family = ok && all_family_levels(engine, rate, depth);
        check(family, "trem, feedback, detune, spectral and pitch all ramp");

        engine.reset();
        if (!base_setup(engine)) {
            std::printf("fades: setup failed\n");
            return 1;
        }
        for (const auto& pair : pairs) {
            ok &= set_param(engine, pair[0], 0);
            ok &= set_param(engine, pair[1], 0);
        }
        engine.noteOn(60, 100, 0);
        const int v = voice_of(engine, 60);
        tick(engine);
        auto* st = engine.state();
        check(ok && v >= 0 && st->op1_lfo_rate_level[v] == kFull &&
                  st->op1_fblfo_depth_level[v] == kFull &&
                  st->op1_dlfo_rate_level[v] == kFull &&
                  st->op1_speclfo_depth_level[v] == kFull &&
                  st->plfo_rate_level[v] == kFull,
              "all five families sit at full with the fades off");
    }

    std::printf("fades: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
