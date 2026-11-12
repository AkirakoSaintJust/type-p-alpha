/* SPDX-License-Identifier: GPL-3.0-only */
/* Operator LFOs: the feedback LFO adds to the feedback level and the detune LFO
 * adds to the operator pitch. At depth 0 both paths are bit-exact. */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"

namespace {

constexpr int kSamples = 16384;
int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("lfos: %-60s %s\n", what, ok ? "ok" : "FAIL");
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
        std::printf("lfos: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool setup(p1::Engine& engine, int32_t feedbackLfoDepth, int32_t detuneLfoDepth,
           int32_t feedbackLfoRate, int32_t detuneLfoRate)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 7);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_detune.p0", 0);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op1_feedback.p0", 16384);
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

    ok &= set_param(engine, "op1_fblfo_depth.p0", feedbackLfoDepth);
    ok &= set_param(engine, "op1_fblfo_rate.p0", feedbackLfoRate);
    ok &= set_param(engine, "op1_fblfo_wave.p0", 0);
    ok &= set_param(engine, "op1_fblfo_mode.p0", 1);
    ok &= set_param(engine, "op1_dlfo_depth.p0", detuneLfoDepth);
    ok &= set_param(engine, "op1_dlfo_rate.p0", detuneLfoRate);
    ok &= set_param(engine, "op1_dlfo_wave.p0", 0);
    ok &= set_param(engine, "op1_dlfo_mode.p0", 1);
    return ok;
}

bool render(int32_t feedbackLfoDepth, int32_t detuneLfoDepth,
            int32_t feedbackLfoRate, int32_t detuneLfoRate,
            std::vector<int32_t>& samples)
{
    p1::Engine engine;
    engine.reset();
    if (!setup(engine, feedbackLfoDepth, detuneLfoDepth, feedbackLfoRate,
               detuneLfoRate))
        return false;
    engine.noteOn(60, 100, 0);
    samples.assign(2u * static_cast<std::size_t>(kSamples), 0);
    engine.render(samples.data(), static_cast<uint32_t>(kSamples));
    return true;
}

bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)
{
    return a.size() == b.size() &&
           std::memcmp(a.data(), b.data(), a.size() * sizeof(int32_t)) == 0;
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("operator lfos\n");

    std::vector<int32_t> base;
    std::vector<int32_t> baseOther;
    std::vector<int32_t> feedbackLfo;
    std::vector<int32_t> detuneLfo;
    if (!render(0, 0, 100000, 100000, base) ||
        !render(0, 0, 200000, 50000, baseOther) ||
        !render(20000, 0, 100000, 100000, feedbackLfo) ||
        !render(0, 2000, 100000, 100000, detuneLfo)) {
        std::printf("lfos: setup failed\n");
        return 1;
    }

    check(same(base, baseOther),
          "depth 0 keeps both LFO paths bit-exact");
    check(!same(base, feedbackLfo),
          "the feedback LFO changes the render");
    check(!same(base, detuneLfo),
          "the detune LFO changes the render");

    std::printf("lfos: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
