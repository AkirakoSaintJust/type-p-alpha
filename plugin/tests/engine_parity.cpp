/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 1: the engine render reference and the state, filter and MIDI
 * isolation checks. The reference is a captured Type P.alpha render, not a
 * device render. */
#include <cmath>
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

double filter_tail_rms(int32_t resonance)
{
    const uint32_t on = param_id("filter_on.p0");
    const uint32_t cut = param_id("cutoff.p0");
    const uint32_t res = param_id("res.p0");
    const uint32_t op1 = param_id("op1_level.p0");
    const uint32_t op2 = param_id("op2_level.p0");
    const uint32_t op3 = param_id("op3_level.p0");
    const uint32_t op4 = param_id("op4_level.p0");
    const uint32_t part = param_id("level.p0");
    if (on == ~0u || cut == ~0u || res == ~0u || op1 == ~0u || op2 == ~0u ||
        op3 == ~0u || op4 == ~0u || part == ~0u)
        return 0.0;

    p1::Engine engine;
    int32_t buffer[512] = {};
    engine.reset();
    engine.setParam(on, 1);
    engine.setParam(cut, 40000);
    engine.setParam(res, resonance);
    engine.setParam(part, 99);
    engine.noteOn(60, 100, 0);
    engine.render(buffer, 256);
    engine.setParam(op1, 0);
    engine.setParam(op2, 0);
    engine.setParam(op3, 0);
    engine.setParam(op4, 0);
    engine.render(buffer, 256);

    std::int64_t sum = 0;
    for (int block = 0; block < 40; ++block) {
        engine.render(buffer, 256);
        for (int i = 0; i < 512; ++i)
            sum += static_cast<std::int64_t>(buffer[i]) * buffer[i];
    }
    return std::sqrt(static_cast<double>(sum) / (40.0 * 512.0));
}

void test_filter_self_oscillation()
{
    const double high = filter_tail_rms(37748); /* 90 percent */
    const double low = filter_tail_rms(20000);  /* 48 percent */
    check(high > 200.0,
          "the filter self-oscillates at 90 percent resonance");
    check(low < high * 0.1,
          "the resonance below onset decays");
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

void test_channel_separation()
{
    p1::Engine engine;
    p1::MidiBridge midi(engine);
    engine.reset();
    midi.reset();

    /* The patch puts part 1 on channel 0 and part 2 on channel 1. */
    midi.feed(0x90, 60, 100);
    midi.feed(0x91, 60, 100);
    const uint32_t bit = 1u << 7; /* note 60 from base 53 */
    check((engine.lit(53u) & bit) != 0,
          "both parts sound the same note on different channels");

    midi.feed(0x80, 60, 0);
    check((engine.lit(53u) & bit) != 0,
          "a note-off on one channel leaves the other part sounding");

    midi.feed(0x81, 60, 0);
    check((engine.lit(53u) & bit) == 0,
          "the second note-off releases its own part");
}

void test_sustain_per_channel()
{
    p1::Engine engine;
    p1::MidiBridge midi(engine);
    engine.reset();
    midi.reset();

    midi.feed(0x90, 64, 100);
    midi.feed(0x91, 65, 100);
    midi.feed(0xB0, 64, 127); /* sustain on channel 0 only */
    midi.feed(0x80, 64, 0);
    midi.feed(0x81, 65, 0);

    const uint32_t held = 1u << 11;   /* note 64 from base 53 */
    const uint32_t other = 1u << 12;  /* note 65 from base 53 */
    check((engine.lit(53u) & held) != 0 && (engine.lit(53u) & other) == 0,
          "sustain holds its own channel and not the other");

    midi.feed(0xB0, 64, 0);
    check((engine.lit(53u) & held) == 0,
          "lifting the pedal releases the notes it held");
}

void test_bend_per_part()
{
    p1::Engine engine;
    p1::MidiBridge midi(engine);
    engine.reset();
    midi.reset();

    /* A quarter of the default +/-2 semitone span, on channel 0. */
    midi.feed(0xE0, 0, 96);
    check(engine.state()->part_bend_q16[0] > 65536,
          "bend on channel 0 raises its part");
    check(engine.state()->part_bend_q16[1] == 65536,
          "bend on channel 0 leaves part 1 at centre");

    midi.feed(0xE1, 0, 32);
    check(engine.state()->part_bend_q16[1] < 65536,
          "bend on channel 1 lowers its own part");
    check(engine.state()->part_bend_q16[0] > 65536,
          "the first part keeps its bend");

    engine.setParam(param_id("bend_range.p0"), 12);
    midi.feed(0xE0, 127, 127);
    const int32_t wide = engine.state()->part_bend_q16[0];
    check(wide > 130000 && wide < 132000,
          "the part range sets the bend span");
}

void test_pan_per_part()
{
    p1::Engine engine;
    engine.reset();
    engine.setParam(param_id("pan.p0"), -100);
    engine.setParam(param_id("pan.p1"), 100);
    int32_t buffer[512] = {};
    engine.render(buffer, 256);
    check(engine.state()->pan_l_q16[0] > 90000 &&
              engine.state()->pan_r_q16[0] == 0,
          "hard left puts part 1 on the left");
    check(engine.state()->pan_l_q16[1] == 0 &&
              engine.state()->pan_r_q16[1] > 90000,
          "hard right puts part 2 on the right");

    engine.setParam(param_id("pan.p0"), 0);
    engine.setParam(param_id("pan.p1"), 0);
    engine.render(buffer, 256);
    check(engine.state()->pan_l_q16[0] == 65536 &&
              engine.state()->pan_r_q16[0] == 65536,
          "centre pan is exactly unity on both channels");
}

} // namespace

int main(int argc, char** argv)
{
    if (argc > 1 && std::strcmp(argv[1], "--dump") == 0) {
        p1::Engine engine;
        int32_t buffer[512] = {};
        engine.reset();
        engine.noteOn(60, 100);
        engine.render(buffer, 256);
        std::printf("NOTE\n");
        for (int32_t value : buffer)
            std::printf("%d\n", value);
        engine.noteOff(60);
        engine.render(buffer, 256);
        std::printf("RELEASE\n");
        for (int32_t value : buffer)
            std::printf("%d\n", value);
        return 0;
    }
    std::printf("engine render and isolation\n");
    test_engine_render();
    test_state_isolation();
    test_filter_is_per_part();
    test_filter_self_oscillation();
    test_sustain_isolation();
    test_channel_separation();
    test_sustain_per_channel();
    test_bend_per_part();
    test_pan_per_part();
    std::printf("engine: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
