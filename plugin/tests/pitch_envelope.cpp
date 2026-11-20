/* SPDX-License-Identifier: GPL-3.0-only */
/* Pitch envelope: the offset stages, the sustain and release, the four modes,
 * and the zero start that keeps the pitch at the note before the first stage. */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <limits>
#include <vector>

#include "Engine.hpp"
#include "Params.h"

namespace {

constexpr int kBlock = 32;
constexpr int kBlocks = 512;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("pitch env: %-62s %s\n", what, ok ? "ok" : "FAIL");
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
        std::printf("pitch env: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool setup(p1::Engine& engine, int32_t l1, int32_t l2, int32_t l3,
           int32_t relLevel, int32_t mode, int32_t r1 = 99)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 7);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op2_level.p0", 0);
    ok &= set_param(engine, "op3_level.p0", 0);
    ok &= set_param(engine, "op4_level.p0", 0);
    ok &= set_param(engine, "op2_mute.p0", 1);
    ok &= set_param(engine, "op3_mute.p0", 1);
    ok &= set_param(engine, "op4_mute.p0", 1);
    ok &= set_param(engine, "attack.p0", 99);
    ok &= set_param(engine, "release.p0", 99);
    ok &= set_param(engine, "op1.eg.d1l.p0", 99);
    ok &= set_param(engine, "op1.eg.d2l.p0", 99);
    ok &= set_param(engine, "filter_on.p0", 0);
    ok &= set_param(engine, "penv_on.p0", 1);
    ok &= set_param(engine, "penv_r1.p0", r1);
    ok &= set_param(engine, "penv_r2.p0", 99);
    ok &= set_param(engine, "penv_r3.p0", 99);
    ok &= set_param(engine, "penv_l1.p0", l1);
    ok &= set_param(engine, "penv_l2.p0", l2);
    ok &= set_param(engine, "penv_l3.p0", l3);
    ok &= set_param(engine, "penv_relr.p0", 99);
    ok &= set_param(engine, "penv_rell.p0", relLevel);
    ok &= set_param(engine, "penv_mode.p0", mode);
    return ok;
}

void render(p1::Engine& engine, int blocks, int32_t* level)
{
    std::vector<int32_t> out(static_cast<std::size_t>(kBlock) * 2u);
    for (int b = 0; b < blocks; ++b) {
        engine.render(out.data(), static_cast<uint32_t>(kBlock));
        if (level != nullptr)
            *level = engine.state()->penv_level[0];
    }
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("pitch envelope experiment\n");

    /* The off switch and a zero envelope both leave the note alone. */
    {
        p1::Engine off;
        off.reset();
        bool ok = setup(off, 0, 0, 0, 0, 1);
        ok &= set_param(off, "penv_on.p0", 0);
        p1::Engine flat;
        flat.reset();
        ok &= setup(flat, 0, 0, 0, 0, 1);
        std::vector<int32_t> a(static_cast<std::size_t>(kBlock) * 2u);
        std::vector<int32_t> b(a.size());
        off.noteOn(60, 100, 0);
        flat.noteOn(60, 100, 0);
        off.render(a.data(), static_cast<uint32_t>(kBlock));
        flat.render(b.data(), static_cast<uint32_t>(kBlock));
        check(ok && a == b, "the off switch and a zero envelope are bit-exact");
    }

    /* The first block starts at zero offset and moves slowly. */
    {
        p1::Engine engine;
        engine.reset();
        bool ok = setup(engine, 8192, 8192, 8192, 0, 1, 40);
        engine.noteOn(60, 100, 0);
        int32_t level = 0;
        render(engine, 1, &level);
        check(ok && level > 0 && level < 8192,
              "the envelope starts at zero and moves toward level1");
        ok &= set_param(engine, "penv_r1.p0", 99);
        render(engine, 64, &level);
        check(ok && level == 8192, "sustain holds level3");
    }

    /* Sustain off releases after level3 with the gate still high, once. */
    {
        p1::Engine engine;
        engine.reset();
        bool ok = setup(engine, 8192, 8192, 8192, 0, 0);
        engine.noteOn(60, 100, 0);
        int32_t level = 0;
        render(engine, 64, &level);
        check(ok && level == 0, "sustain off releases after level3");
        render(engine, 256, &level);
        check(level == 0, "the sustain-off release does not restart itself");
    }

    /* A note-off release ends at the release level. */
    {
        p1::Engine engine;
        engine.reset();
        bool ok = setup(engine, 8192, 8192, 8192, -4096, 1);
        engine.noteOn(60, 100, 0);
        render(engine, 64, nullptr);
        engine.noteOff(60);
        int32_t level = 0;
        render(engine, 64, &level);
        check(ok && level == -4096, "the release ends at the release level");
    }

    /* Loop2 cycles between level2 and level3. */
    {
        p1::Engine engine;
        engine.reset();
        bool ok = setup(engine, 4096, 4096, 8192, 0, 3, 80);
        engine.noteOn(60, 100, 0);
        int32_t low = std::numeric_limits<int32_t>::max();
        int32_t high = std::numeric_limits<int32_t>::min();
        int32_t level = 0;
        for (int b = 0; b < kBlocks; ++b) {
            render(engine, 1, &level);
            low = level < low ? level : low;
            high = level > high ? level : high;
        }
        check(ok && low <= 4104 && high >= 8184,
              "loop2 cycles between level2 and level3");
    }

    if (failures != 0)
        std::printf("pitch env: FAILED\n");
    else
        std::printf("pitch env: passed\n");
    return failures == 0 ? 0 : 1;
}
