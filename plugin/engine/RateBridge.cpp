/* SPDX-License-Identifier: GPL-3.0-only */
#include "RateBridge.hpp"

#include <algorithm>
#include <cmath>

#include "Engine.hpp"

namespace p1 {
namespace {

constexpr double kPi = 3.14159265358979323846;

double besselI0(double x)
{
    double sum = 1.0;
    double term = 1.0;
    for (int k = 1; k < 32; ++k) {
        const double factor = x / (2.0 * static_cast<double>(k));
        term *= factor * factor;
        sum += term;
        if (term < 1.0e-12 * sum)
            break;
    }
    return sum;
}

} // namespace

void RateBridge::prepare(double hostRate, int maxBlock)
{
    hostRate_ = hostRate > 0.0 ? hostRate : 44100.0;
    ratio_ = 44100.0 / hostRate_;
    const int needed = static_cast<int>(std::ceil(
        static_cast<double>(std::max(1, maxBlock)) * ratio_)) + 128;
    ringSize_ = std::max(64, needed);
    ring_.assign(static_cast<std::size_t>(ringSize_) * 2, 0);
    kernel_.assign(static_cast<std::size_t>(kTaps) * kPhases, 0.0f);

    const double cutoff = std::min(1.0, hostRate_ / 44100.0);
    constexpr double beta = 8.6;
    const double i0Beta = besselI0(beta);
    for (int phase = 0; phase < kPhases; ++phase) {
        const double frac = static_cast<double>(phase) / kPhases;
        const double centre = kTaps * 0.5;
        double sum = 0.0;
        for (int tap = 0; tap < kTaps; ++tap) {
            const double distance = (kTaps - 1 - tap) + frac;
            const double x = cutoff * (distance - centre);
            const double sinc =
                std::abs(x) < 1.0e-12 ? 1.0 : std::sin(kPi * x) / (kPi * x);
            const double t = (distance - centre) / (kTaps * 0.5);
            const double window =
                t < -1.0 || t > 1.0
                    ? 0.0
                    : besselI0(beta * std::sqrt(std::max(0.0, 1.0 - t * t))) /
                          i0Beta;
            const double value = cutoff * sinc * window;
            kernel_[static_cast<std::size_t>(phase) * kTaps + tap] =
                static_cast<float>(value);
            sum += value;
        }
        if (sum != 0.0) {
            const float scale = static_cast<float>(1.0 / sum);
            for (int tap = 0; tap < kTaps; ++tap) {
                kernel_[static_cast<std::size_t>(phase) * kTaps + tap] *= scale;
            }
        }
    }
    reset();
}

void RateBridge::reset()
{
    std::fill(ring_.begin(), ring_.end(), 0);
    coreGenerated_ = 0;
    outputIndex_ = 0;
}

void RateBridge::pushCore(const int32_t* interleaved, int frames)
{
    if (interleaved == nullptr || ringSize_ <= 0 || frames <= 0)
        return;
    for (int i = 0; i < frames; ++i) {
        const int index = static_cast<int>(coreGenerated_ % ringSize_);
        ring_[static_cast<std::size_t>(index) * 2] = interleaved[i * 2];
        ring_[static_cast<std::size_t>(index) * 2 + 1] = interleaved[i * 2 + 1];
        ++coreGenerated_;
    }
}

void RateBridge::pull(float* left, float* right, int frames)
{
    if (left == nullptr || right == nullptr || frames <= 0)
        return;
    if (ringSize_ <= 0 || kernel_.empty()) {
        std::fill(left, left + frames, 0.0f);
        std::fill(right, right + frames, 0.0f);
        return;
    }
    constexpr float scale = 1.0f / 32768.0f;
    for (int i = 0; i < frames; ++i) {
        const double position = static_cast<double>(outputIndex_) * ratio_;
        const int64_t n = static_cast<int64_t>(std::floor(position));
        const double fraction = position - static_cast<double>(n);
        int phase = static_cast<int>(std::llround(fraction * kPhases));
        phase = std::clamp(phase, 0, kPhases - 1);
        const float* weights =
            kernel_.data() + static_cast<std::size_t>(phase) * kTaps;
        float l = 0.0f;
        float r = 0.0f;
        for (int tap = 0; tap < kTaps; ++tap) {
            const int64_t sample = n - (kTaps - 1) + tap;
            if (sample < 0 || sample >= coreGenerated_)
                continue;
            const int index = static_cast<int>(sample % ringSize_);
            const std::size_t offset = static_cast<std::size_t>(index) * 2;
            l += static_cast<float>(ring_[offset]) * weights[tap];
            r += static_cast<float>(ring_[offset + 1]) * weights[tap];
        }
        left[i] = l * scale;
        right[i] = r * scale;
        ++outputIndex_;
    }
}

void RateBridge::renderChunk(Engine& engine, int frames)
{
    int32_t chunk[2 * 32] = {};
    const int count = std::min(frames, 32);
    engine.render(chunk, static_cast<uint32_t>(count));
    pushCore(chunk, count);
}

void RateBridge::process(Engine& engine, float* left, float* right, int frames)
{
    if (frames <= 0)
        return;
    const int64_t last = static_cast<int64_t>(std::floor(
        static_cast<double>(outputIndex_ + frames - 1) * ratio_));
    while (coreGenerated_ <= last)
        renderChunk(engine, 32);
    pull(left, right, frames);
}

int RateBridge::latencySamples() const
{
    return static_cast<int>(
        std::lround((kTaps - 1) * 0.5 * hostRate_ / 44100.0));
}

} // namespace p1
