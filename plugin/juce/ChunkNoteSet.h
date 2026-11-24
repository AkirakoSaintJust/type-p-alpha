/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <cstdint>

namespace p1 {

/* Tracks the notes that started in the current control chunk, by MIDI channel
 * and pitch, so a release in the same chunk waits for the chunk to render. Two
 * channels can hold the same pitch without deferring each other's releases. */
class ChunkNoteSet {
public:
    static constexpr int kChannels = 16;
    static constexpr int kNotes = 128;

    void reset() { held_.fill(0); }

    void noteOn(int channel, int note)
    {
        if (channel < 1 || channel > kChannels || note < 0 || note >= kNotes)
            return;
        held_[static_cast<std::size_t>((channel - 1) * kNotes + note)] = 1;
    }

    bool held(int channel, int note) const
    {
        if (channel < 1 || channel > kChannels || note < 0 || note >= kNotes)
            return false;
        return held_[static_cast<std::size_t>((channel - 1) * kNotes + note)] != 0;
    }

private:
    std::array<uint8_t, kChannels * kNotes> held_{};
};

} // namespace p1
