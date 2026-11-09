/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace p1 {

class Engine;

/* Output-only 32-tap Kaiser-windowed sinc polyphase resampler. The core runs at
 * 44.1 kHz, whilst the bridge converts to the host rate, and at 44.1 kHz the
 * processor bypasses it so the core output stays bit-exact there. */
class RateBridge {
public:
    void prepare(double hostRate, int maxBlock);
    void reset();

    /* The engine path: render core chunks, then pull host frames. */
    void process(Engine& engine, float* left, float* right, int frames);

    /* Test and offline path that pushes interleaved int32 core frames and pulls
     * host frames. The ring keeps the history the 32-tap kernel needs. */
    void pushCore(const int32_t* interleaved, int frames);
    void pull(float* left, float* right, int frames);

    int latencySamples() const;
    double ratio() const { return ratio_; }

private:
    static constexpr int kTaps = 32;
    static constexpr int kPhases = 1024;

    void renderChunk(Engine& engine, int frames);

    double hostRate_ = 44100.0;
    double ratio_ = 1.0;
    int ringSize_ = 0;
    std::vector<int32_t> ring_;
    std::vector<float> kernel_;
    int64_t coreGenerated_ = 0;
    int64_t outputIndex_ = 0;
};

} // namespace p1
