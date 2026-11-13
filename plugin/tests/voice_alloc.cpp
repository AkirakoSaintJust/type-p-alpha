/* SPDX-License-Identifier: GPL-3.0-only */
/* Voice allocation: a held chord or scale takes one voice per note, a release
 * takes only its own voice, and the same sequence repeats after the voices have
 * been recycled. */
#include <cstdio>
#include <vector>

#include "Engine.hpp"

namespace {

constexpr int kVoices = 24;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("voices: %-58s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

bool chord(p1::Engine& engine, uint32_t channel, int& gated, bool* notes)
{
    engine.noteOn(60, 100, channel);
    engine.noteOn(64, 100, channel);
    engine.noteOn(67, 100, channel);
    gated = 0;
    for (int i = 0; i < 3; ++i)
        notes[i] = false;
    for (int v = 0; v < kVoices; ++v) {
        const auto* state = engine.state();
        if (!state->voice_gate[v])
            continue;
        ++gated;
        if (state->voice_note[v] == 60)
            notes[0] = true;
        else if (state->voice_note[v] == 64)
            notes[1] = true;
        else if (state->voice_note[v] == 67)
            notes[2] = true;
    }
    return gated == 3 && notes[0] && notes[1] && notes[2];
}

} // namespace

int main()
{
    p1::Engine engine;
    int gated = 0;
    bool notes[3] = {};

    engine.reset();
    const bool first = chord(engine, 0, gated, notes);
    check(first, "a three-note chord takes three voices on part one");

    engine.noteOff(64);
    int remaining = 0;
    bool kept60 = false;
    bool kept67 = false;
    bool released64 = true;
    for (int v = 0; v < kVoices; ++v) {
        const auto* state = engine.state();
        if (state->voice_gate[v]) {
            ++remaining;
            kept60 = kept60 || state->voice_note[v] == 60;
            kept67 = kept67 || state->voice_note[v] == 67;
        }
        if (state->voice_note[v] == 64 && state->voice_gate[v])
            released64 = false;
    }
    check(remaining == 2 && kept60 && kept67 && released64,
          "a release keeps the other two chord notes");

    {
        engine.reset();
        const int scaleNotes[7] = {60, 62, 64, 65, 67, 69, 71};
        for (int note : scaleNotes)
            engine.noteOn(static_cast<uint32_t>(note), 100, 0);
        int scaleGated = 0;
        bool seen[7] = {};
        for (int v = 0; v < kVoices; ++v) {
            const auto* state = engine.state();
            if (!state->voice_gate[v])
                continue;
            ++scaleGated;
            for (int i = 0; i < 7; ++i)
                if (state->voice_note[v] == scaleNotes[i])
                    seen[i] = true;
        }
        bool allSeen = true;
        for (bool value : seen)
            allSeen = allSeen && value;
        check(scaleGated == 7 && allSeen,
              "a held seven-note scale takes seven voices");
    }

    {
        engine.reset();
        const int notes12[12] = {48, 50, 52, 53, 55, 57,
                                 59, 60, 62, 64, 65, 67};
        for (int note : notes12)
            engine.noteOn(static_cast<uint32_t>(note), 100, 0);
        int part0 = 0;
        for (int v = 0; v < kVoices; ++v) {
            const auto* state = engine.state();
            if (state->voice_gate[v] && state->voice_part[v] == 0)
                ++part0;
        }
        check(part0 == 12, "twelve notes fill twelve voices on part one");

        for (int note : notes12)
            engine.noteOn(static_cast<uint32_t>(note), 100, 1);
        int part1 = 0;
        part0 = 0;
        for (int v = 0; v < kVoices; ++v) {
            const auto* state = engine.state();
            if (!state->voice_gate[v])
                continue;
            if (state->voice_part[v] == 0)
                ++part0;
            else
                ++part1;
        }
        check(part0 == 12 && part1 == 12,
              "two twelve-note chords fill all 24 voices");
    }

    engine.reset();
    const bool second = chord(engine, 1, gated, notes);
    check(second, "the same chord takes three voices on part two");

    bool wrongChannel = false;
    engine.reset();
    engine.noteOn(60, 100, 5);
    for (int v = 0; v < kVoices; ++v)
        wrongChannel = wrongChannel || engine.state()->voice_gate[v];
    check(!wrongChannel, "a note on an unused channel takes no voice");

    bool recycled = true;
    engine.reset();
    for (int round = 0; round < 8; ++round) {
        recycled = chord(engine, 0, gated, notes) && recycled;
        engine.noteOff(60);
        engine.noteOff(64);
        engine.noteOff(67);
        std::vector<int32_t> out(2u * 256u, 0);
        engine.render(out.data(), 256);
    }
    check(recycled, "chords still allocate three voices after recycling");

    std::printf("voices: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
