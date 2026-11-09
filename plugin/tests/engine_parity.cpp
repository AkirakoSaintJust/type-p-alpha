/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 1: engine and MIDI parity without JUCE. */
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "MidiBridge.hpp"
#include "P1Synth.h"
#include "cc_map.h"
#include "synth_midi.h"

namespace {

struct Msg {
    uint8_t status;
    uint8_t data1;
    uint8_t data2;
};

std::vector<Msg> queue;
std::size_t queue_pos = 0;
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

} // namespace

extern "C" int midi_get(uint8_t* status, uint8_t* data1, uint8_t* data2)
{
    if (queue_pos >= queue.size())
        return 0;
    const Msg& m = queue[queue_pos++];
    *status = m.status;
    *data1 = m.data1;
    *data2 = m.data2;
    return 1;
}

namespace {

p1::Engine* ref_engine = nullptr;

} // namespace

extern "C" void ref_note_on(uint32_t note, uint32_t velocity, uint32_t channel)
{
    if (ref_engine)
        ref_engine->noteOn(note, velocity, channel);
}

extern "C" void ref_note_off(uint32_t note)
{
    if (ref_engine)
        ref_engine->noteOff(note);
}

extern "C" void ref_all_off()
{
    if (ref_engine)
        ref_engine->allNotesOff();
}

extern "C" int ref_set_param(uint32_t id, int32_t value)
{
    return ref_engine ? ref_engine->setParam(id, value) : 0;
}

extern "C" void ref_set_bend(uint32_t bend)
{
    if (ref_engine)
        ref_engine->setBend(bend);
}

namespace {

synth_midi_ctx ref_context()
{
    synth_midi_ctx ctx{};
    ctx.note_on = ref_note_on;
    ctx.note_off = ref_note_off;
    ctx.all_off = ref_all_off;
    ctx.set_param = ref_set_param;
    ctx.set_bend = ref_set_bend;
    ctx.cc_map = cc_to_param;
    ctx.param_min = param_min;
    ctx.param_max = param_max;
    return ctx;
}

void test_engine_render()
{
    p1::Engine engine;
    int32_t mine[512] = {};
    int32_t firmware[512] = {};

    P1_LEGACY_RESET();
    engine.reset();
    P1_LEGACY_NOTE_ON(60, 100);
    engine.noteOn(60, 100);

    P1_LEGACY_RENDER(firmware, 256);
    engine.render(mine, 256);
    check(same(mine, firmware, 512), "engine render matches the legacy firmware render");

    P1_LEGACY_NOTE_OFF(60);
    engine.noteOff(60);
    P1_LEGACY_RENDER(firmware, 256);
    engine.render(mine, 256);
    check(same(mine, firmware, 512), "release render matches the legacy firmware render");
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

void test_midi_parity()
{
    p1::Engine engine;
    p1::Engine reference;
    p1::MidiBridge bridge(engine);
    synth_midi_ctx ref = ref_context();
    int32_t mine[512] = {};
    int32_t firmware[512] = {};

    ref_engine = &reference;
    engine.reset();
    reference.reset();
    bridge.reset();

    const std::vector<Msg> script = {
        {0x90, 60, 100},
        {0x91, 64, 90},
        {0xB0, 16, 64},
        {0xB0, 64, 127},
        {0xE0, 0, 64},
        {0xE0, 127, 127},
        {0x80, 60, 0},
        {0xB0, 64, 0},
        {0xB1, 17, 127},
        {0xB0, 123, 0},
        {0x90, 127, 127},
        {0x80, 127, 0},
    };

    for (const Msg& m : script) {
        queue = {m};
        queue_pos = 0;
        synth_midi_poll(&ref);
        bridge.feed(m.status, m.data1, m.data2);

        uint32_t n = P1_PARAM_COUNT();
        for (uint32_t id = 0; id < n; ++id) {
            if (engine.getParam(id) != reference.getParam(id)) {
                check(false, "MIDI parameter state matches after every message");
                return;
            }
        }
        if (engine.lit(53u) != reference.lit(53u) ||
            engine.quiet() != reference.quiet()) {
            check(false, "MIDI note state matches after every message");
            return;
        }
    }

    engine.render(mine, 256);
    reference.render(firmware, 256);
    check(same(mine, firmware, 512), "MIDI render matches the reference MIDI path");
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
    std::printf("engine bridge parity\n");
    test_engine_render();
    test_state_isolation();
    test_midi_parity();
    test_sustain_isolation();
    std::printf("engine: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
