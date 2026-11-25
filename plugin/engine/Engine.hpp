/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <cstdint>
#include <memory>

#include "P1Synth.h"

namespace p1 {

/* The display view of one per-part phase map. It is a copy of the map pool
 * geometry plus the newest voice's mapped phase, so the UI never reads the
 * engine state directly. */
struct PhaseMapSnapshot {
    bool valid = false;
    int bp = 500;
    int segments = 8;
    int active = 8;
    int voices = 0;
    uint16_t phaseQ = 0;
    uint16_t phaseY = 0;
    std::array<uint16_t, 33> x{};
    std::array<uint16_t, 32> y0{};
    std::array<int32_t, 32> slope{};
    std::array<uint8_t, 32> flip{};
    std::array<uint8_t, 32> order{};
    std::array<uint8_t, 32> locked{};
};

/* One generated engine state behind a small C++ interface. There is no JUCE and no
 * audio backend here, because the wrappers are the only place the host touches
 * the generated C API. */
class Engine {
public:
    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    void reset();
    int setParam(uint32_t id, int32_t value);
    int32_t getParam(uint32_t id);
    void noteOn(uint32_t note, uint32_t velocity, uint32_t channel = 0xFFFFFFFFu);
    void noteOff(uint32_t note, uint32_t channel = 0xFFFFFFFFu);
    /* Releases the notes on one channel, or every channel for the panic. */
    void allNotesOff(uint32_t channel = 0xFFFFFFFFu);
    void setBend(uint32_t bend14, uint32_t channel = 0xFFFFFFFFu);
    void render(int32_t* out, uint32_t frames);
    uint32_t lit(uint32_t base);
    bool quiet();
    const int32_t* tap(uint32_t channel);
    /* The last morph the oscillator read for an operator and part, 0 to 99, or
     * -1 when the operator has not read the bank since the state reset. */
    int specMorph(int op, int part) const;
    bool phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const;
    /* Clear the per-part phase map, LFO and envelope pools so a patch load, or
     * switching the map off, starts from the current controls. */
    void resetPhaseMaps();

    P1_STATE* state();

private:
    std::unique_ptr<P1_STATE> state_;
};

} // namespace p1
