/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>
#include <cstdint>
#include <vector>

namespace p1 {

class Engine;

/* Output-only 128-tap Kaiser-windowed sinc polyphase resampler. The core runs at
 * 48 kHz, whilst the bridge converts to the host rate, and at 48 kHz the
 * processor bypasses it so the core output stays bit-exact there. */
class RateBridge {
public:
    using CoreTapCallback = void (*)(void* context, Engine& engine, int frames);

    void prepare(double hostRate, int maxBlock);
    void reset();

    /* The engine path: render core chunks, then pull host frames. The callback
     * runs after each core chunk, before the tap state is overwritten. */
    void process(Engine& engine, float* left, float* right, int frames,
                 CoreTapCallback onCore = nullptr, void* context = nullptr);

    /* Test and offline path that pushes interleaved int32 core frames and pulls
     * host frames. The ring keeps the history the 128-tap kernel needs. */
    void pushCore(const int32_t* interleaved, int frames);
    /* Reads host frames through the kernel. A null `right` folds the pair to
     * mono in `left`. */
    void pull(float* left, float* right, int frames);

    int latencySamples() const;
    double ratio() const { return ratio_; }

    /* True when a `process` call of `hostFrames` will render at least one core
     * chunk, so the caller can feed the first chunk's events before it runs. */
    bool hasCore(int hostFrames) const;

private:
    static constexpr int kTaps = 128;
    static constexpr int kPhases = 1024;

    void renderChunk(Engine& engine, int frames, CoreTapCallback onCore,
                     void* context);

    double hostRate_ = 48000.0;
    double ratio_ = 1.0;
    int ringSize_ = 0;
    std::vector<int32_t> ring_;
    std::vector<float> kernel_;
    int64_t coreGenerated_ = 0;
    int64_t outputIndex_ = 0;
};

} // namespace p1
