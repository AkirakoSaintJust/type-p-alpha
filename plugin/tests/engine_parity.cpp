/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 1: the engine render reference and the state, filter and MIDI
 * isolation checks. The reference is a captured Type P.alpha render, not a
 * device render. */
#include <cstdint>
#include <cstdio>
#include <cstring>

#include "Engine.hpp"
#include "MidiBridge.hpp"
#include "Params.h"
#include "engine_render_reference.h"

extern "C" int midi_get(uint8_t*, uint8_t*, uint8_t*)
{
    return 0; /* the test feeds the MIDI bridge directly */
}

namespace {

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("engine: %-64s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

bool same(const int32_t* a, const int32_t* b, std::size_t n)
{
    return std::memcmp(a, b, n * sizeof a[0]) == 0;
}

uint32_t param_id(const char* name)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (std::strcmp(spec.id, name) == 0)
            return static_cast<uint32_t>(spec.engineId);
    }
    return ~0u;
}

void test_engine_render()
{
    p1::Engine engine;
    int32_t mine[512] = {};

    engine.reset();
    engine.noteOn(60, 100);
    engine.render(mine, 256);
    check(same(mine, kEngineNoteOn, 512),
          "note-on render matches the captured reference");

    engine.noteOff(60);
    engine.render(mine, 256);
    check(same(mine, kEngineRelease, 512),
          "release render matches the captured reference");
}

void test_state_isolation()
{
    p1::Engine a;
    p1::Engine b;
    p1::Engine c;
    int32_t ra[512] = {};
    int32_t rb[512] = {};
    int32_t rc[512] = {};

    a.reset();
    b.reset();
    c.reset();
    b.setParam(112u, 0u); /* a different algorithm */
    a.noteOn(60, 100);
    b.noteOn(72, 100);
    c.noteOn(60, 100);
    for (int block = 0; block < 4; ++block) {
        a.render(ra, 256);
        b.render(rb, 256);
        c.render(rc, 256);
        if (!same(ra, rc, 512)) {
            check(false, "two engine states stay isolated");
            return;
        }
    }
    check(true, "two engine states stay isolated");
}

void test_filter_is_per_part()
{
    const uint32_t on0 = param_id("filter_on.p0");
    const uint32_t on1 = param_id("filter_on.p1");
    const uint32_t cut0 = param_id("cutoff.p0");
    const uint32_t cut1 = param_id("cutoff.p1");
    check(on0 != ~0u && on1 != ~0u && cut0 != ~0u && cut1 != ~0u,
          "the filter parameters are per part");
    if (on0 == ~0u || on1 == ~0u || cut0 == ~0u || cut1 == ~0u)
        return;

    p1::Engine bypass;
    p1::Engine filtered;
    int32_t r_bypass[512] = {};
    int32_t r_filtered[512] = {};
    bypass.reset();
    filtered.reset();
    filtered.setParam(on0, 1);
    filtered.setParam(cut0, 1500);
    bypass.noteOn(60, 100, 0);
    filtered.noteOn(60, 100, 0);
    bypass.render(r_bypass, 256);
    filtered.render(r_filtered, 256);
    check(!same(r_bypass, r_filtered, 512),
          "the filter changes the part 0 render");

    p1::Engine a;
    p1::Engine b;
    int32_t ra[512] = {};
    int32_t rb[512] = {};
    a.reset();
    b.reset();
    a.setParam(on0, 1);
    a.setParam(on1, 1);
    b.setParam(on0, 1);
    b.setParam(on1, 1);
    a.setParam(cut0, 1500);
    a.setParam(cut1, 50000);
    b.setParam(cut0, 50000);
    b.setParam(cut1, 1500);
    a.noteOn(60, 100, 0);
    b.noteOn(60, 100, 0);
    a.render(ra, 256);
    b.render(rb, 256);
    check(!same(ra, rb, 512), "part 0 uses its own cutoff");

    a.reset();
    b.reset();
    a.setParam(on0, 1);
    a.setParam(on1, 1);
    b.setParam(on0, 1);
    b.setParam(on1, 1);
    a.setParam(cut0, 1500);
    a.setParam(cut1, 50000);
    b.setParam(cut0, 50000);
    b.setParam(cut1, 1500);
    a.noteOn(60, 100, 1);
    b.noteOn(60, 100, 1);
    a.render(ra, 256);
    b.render(rb, 256);
    check(!same(ra, rb, 512), "part 1 uses its own cutoff");
}

void test_sustain_isolation()
{
    p1::Engine a;
    p1::Engine b;
    p1::MidiBridge ba(a);
    p1::MidiBridge bb(b);

    a.reset();
    b.reset();
    ba.reset();
    bb.reset();
    ba.feed(0x90, 60, 100);
    bb.feed(0x90, 60, 100);
    ba.feed(0xB0, 64, 127); /* sustain on only A */
    ba.feed(0x80, 60, 0);
    bb.feed(0x80, 60, 0);

    const uint32_t bit = 1u << 7; /* note 60 from base 53 */
    const bool a_lit = (a.lit(53u) & bit) != 0;
    const bool b_lit = (b.lit(53u) & bit) != 0;
    check(a_lit && !b_lit, "sustain state stays per MIDI bridge");
}

} // namespace

int main()
{
    std::printf("engine render and isolation\n");
    test_engine_render();
    test_state_isolation();
    test_filter_is_per_part();
    test_sustain_isolation();
    std::printf("engine: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
