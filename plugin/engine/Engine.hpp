/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <cstdint>
#include <memory>

#include "P1Synth.h"

namespace p1 {

/* One generated engine state behind a small C++ seam. There is no JUCE and no
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
    void noteOff(uint32_t note);
    void allNotesOff();
    void setBend(uint32_t q16);
    void render(int32_t* out, uint32_t frames);
    uint32_t lit(uint32_t base);
    bool quiet();
    const int32_t* tap(uint32_t channel);

    P1_STATE* state();

private:
    std::unique_ptr<P1_STATE> state_;
};

} // namespace p1
