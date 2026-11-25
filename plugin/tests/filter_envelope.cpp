/* SPDX-License-Identifier: GPL-3.0-only */
/* Filter envelope rate response: the slow settings must stay slow. The
 * envelope's levels are 0..99, so the engine scales them internally by 256;
 * without that scale the one-pole's one-unit minimum step made every rate
 * finish in about 66 ms. */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"

namespace {
constexpr int kBlock = 32;
constexpr int32_t kTarget = 99 * 256;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("filter env: %-58s %s\n", what, ok ? "ok" : "FAIL");
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

void set_param(p1::Engine& engine, const char* name, int32_t value)
{
    const uint32_t id = param_id(name);
    if (id == ~0u) {
        std::printf("filter env: missing parameter %s\n", name);
        return;
    }
    engine.setParam(id, value);
}

void setup(p1::Engine& engine, int32_t r1)
{
    set_param(engine, "algorithm.p0", 9);
    set_param(engine, "op1_waveform.p0", 0);
    set_param(engine, "op1_ratio.p0", 65536);
    set_param(engine, "op1_level.p0", 99);
    set_param(engine, "op2_mute.p0", 1);
    set_param(engine, "op3_mute.p0", 1);
    set_param(engine, "op4_mute.p0", 1);
    set_param(engine, "attack.p0", 99);
    set_param(engine, "release.p0", 99);
    set_param(engine, "op1.eg.d1l.p0", 99);
    set_param(engine, "op1.eg.d2l.p0", 99);
    set_param(engine, "filter_on.p0", 1);
    set_param(engine, "cutoff.p0", 0);
    set_param(engine, "fenv_on.p0", 1);
    set_param(engine, "fenv_r1.p0", r1);
    set_param(engine, "fenv_r2.p0", 99);
    set_param(engine, "fenv_r3.p0", 99);
    set_param(engine, "fenv_l1.p0", 99);
    set_param(engine, "fenv_l2.p0", 99);
    set_param(engine, "fenv_l3.p0", 99);
    set_param(engine, "fenv_relr.p0", 99);
    set_param(engine, "fenv_rell.p0", 0);
    set_param(engine, "fenv_mode.p0", 1);
}

int32_t run(int32_t r1, int blocks)
{
    p1::Engine engine;
    engine.reset();
    setup(engine, r1);
    engine.noteOn(60, 100, 0);
    std::vector<int32_t> out(2u * kBlock);
    for (int b = 0; b < blocks; ++b)
        engine.render(out.data(), kBlock);
    return engine.state()->fenv_level[0];
}
} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("filter envelope rates\n");

    const int32_t slow = run(0, 100);
    const int32_t slowLong = run(0, 1000);
    const int32_t fast = run(80, 100);
    const int32_t instant = run(99, 10);
    std::printf("filter env: r0 %d/%d, r80 %d/%d, r99 %d/%d\n", slow,
                kTarget, fast, kTarget, instant, kTarget);

    check(slow > 0 && slow < kTarget / 10,
          "the slowest rate is still moving slowly after 100 blocks");
    check(slowLong < kTarget / 8,
          "the slowest rate has not snapped to the target");
    check(fast > kTarget / 2, "a fast rate is well advanced after 100 blocks");
    check(instant == kTarget, "the instant rate reaches the target");

    if (failures == 0)
        std::printf("filter env: all checks passed\n");
    else
        std::printf("filter env: %d check(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
