/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <cstdint>

#include "synth_midi.h"

namespace p1 {

class Engine;

/* The device MIDI handler for one engine. The sustain state is stored in the
 * context, so each bridge keeps its own pedal state. */
class MidiBridge {
public:
    explicit MidiBridge(Engine& engine);

    void feed(uint8_t status, uint8_t data1, uint8_t data2);
    void reset();

private:
    Engine& engine_;
    synth_midi_ctx ctx_{};
};

} // namespace p1
