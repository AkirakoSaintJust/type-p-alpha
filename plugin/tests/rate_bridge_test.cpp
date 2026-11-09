/* SPDX-License-Identifier: GPL-3.0-only */
/* Milestone 5: measure the 32-tap polyphase rate bridge. */
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <vector>

#include "RateBridge.hpp"

namespace {

constexpr double kCoreRate = 44100.0;
constexpr double kPi = 3.14159265358979323846;
constexpr double kAmplitude = 16000.0;

struct Measurement {
    std::vector<float> left;
    std::vector<float> right;
    int latency = 0;
};

double amplitude(const std::vector<float>& samples, int start, int count,
                 double frequency, double sampleRate)
{
    double re = 0.0;
    double im = 0.0;
    double weightSum = 0.0;
    for (int i = 0; i < count; ++i) {
        const double window =
            0.5 - 0.5 * std::cos(2.0 * kPi * i / static_cast<double>(count - 1));
        const double phase =
            2.0 * kPi * frequency * static_cast<double>(i) / sampleRate;
        re += window * samples[static_cast<std::size_t>(start + i)] * std::cos(phase);
        im += window * samples[static_cast<std::size_t>(start + i)] * std::sin(phase);
        weightSum += window;
    }
    return 2.0 * std::sqrt(re * re + im * im) / weightSum;
}

double rms(const std::vector<float>& samples, int start, int count)
{
    double energy = 0.0;
    for (int i = 0; i < count; ++i) {
        const double value = samples[static_cast<std::size_t>(start + i)];
        energy += value * value;
    }
    return std::sqrt(energy / count);
}

Measurement run(double hostRate, double frequency, int hostFrames, int blockSize)
{
    p1::RateBridge bridge;
    bridge.prepare(hostRate, blockSize);
    const double ratio = bridge.ratio();
    Measurement result;
    result.latency = bridge.latencySamples();
    result.left.reserve(static_cast<std::size_t>(hostFrames));
    result.right.reserve(static_cast<std::size_t>(hostFrames));
    std::vector<float> left(static_cast<std::size_t>(blockSize));
    std::vector<float> right(static_cast<std::size_t>(blockSize));
    int64_t pushed = 0;
    int64_t output = 0;
    while (output < hostFrames) {
        const int frames = static_cast<int>(
            std::min<int64_t>(blockSize, hostFrames - output));
        const int64_t last = static_cast<int64_t>(std::floor(
            static_cast<double>(output + frames - 1) * ratio));
        while (pushed <= last) {
            const int count = static_cast<int>(std::min<int64_t>(32, last + 1 - pushed));
            int32_t chunk[64] = {};
            for (int i = 0; i < count; ++i) {
                const double phase =
                    2.0 * kPi * frequency * static_cast<double>(pushed + i) /
                    kCoreRate;
                const int32_t sample = static_cast<int32_t>(
                    std::lround(kAmplitude * std::sin(phase)));
                chunk[i * 2] = sample;
                chunk[i * 2 + 1] = sample;
            }
            bridge.pushCore(chunk, count);
            pushed += count;
        }
        bridge.pull(left.data(), right.data(), frames);
        result.left.insert(result.left.end(), left.begin(), left.begin() + frames);
        result.right.insert(result.right.end(), right.begin(), right.begin() + frames);
        output += frames;
    }
    return result;
}

Measurement run(double hostRate, double frequency, int hostFrames)
{
    return run(hostRate, frequency, hostFrames, 256);
}

std::vector<double> imageFrequencies(double hostRate, double frequency)
{
    std::vector<double> images;
    for (int harmonic = 1; harmonic <= 4; ++harmonic) {
        for (int sign = -1; sign <= 1; sign += 2) {
            double image = harmonic * kCoreRate + sign * frequency;
            image = std::fmod(std::abs(image), hostRate);
            if (image > hostRate * 0.5)
                image = hostRate - image;
            if (image > 200.0 && image < hostRate * 0.5 - 200.0)
                images.push_back(image);
        }
    }
    return images;
}

bool check(bool condition, const char* what)
{
    std::printf("rate: %-64s %s\n", what, condition ? "ok" : "FAIL");
    return condition;
}

} // namespace

int main()
{
    bool ok = true;
    const std::array<double, 3> rates = {48000.0, 96000.0, 192000.0};
    for (double rate : rates) {
        const int frames = static_cast<int>(rate * 0.5);
        const auto base = run(rate, 1000.0, frames);
        const int window = 8192;
        const int start = frames / 4;
        const double gain1k = amplitude(base.left, start, window, 1000.0, rate) /
                              (kAmplitude / 32768.0);
        const auto mid = run(rate, 10000.0, frames);
        const double gain10k = amplitude(mid.left, start, window, 10000.0, rate) /
                               (kAmplitude / 32768.0);
        const auto high = run(rate, 20000.0, frames);
        const double fundamental =
            amplitude(high.left, start, window, 20000.0, rate);
        double image = 0.0;
        for (double candidate : imageFrequencies(rate, 20000.0))
            image = std::max(image, amplitude(high.left, start, window, candidate,
                                              rate));
        const double aliasDb = 20.0 * std::log10(std::max(1.0e-12, image / fundamental));
        const double gain1kDb = 20.0 * std::log10(gain1k);
        const double gain10kDb = 20.0 * std::log10(gain10k);
        const double gain20k = fundamental / (kAmplitude / 32768.0);
        const double gain20kDb = 20.0 * std::log10(std::max(1.0e-12, gain20k));
        const double total = rms(high.left, start, window);
        const double fundamentalRms = fundamental / std::sqrt(2.0);
        const double thd = std::sqrt(std::max(
            0.0, total * total - fundamentalRms * fundamentalRms)) /
                           std::max(1.0e-12, fundamentalRms);
        const int expectedLatency = static_cast<int>(
            std::lround(15.5 * rate / kCoreRate));
        std::printf("rate: %5.0f Hz latency=%d gain1k=%+.2f dB gain10k=%+.2f dB "
                    "gain20k=%+.2f dB alias20k=%+.1f dB thd=%+.1f dB\n",
                    rate, base.latency, gain1kDb, gain10kDb, gain20kDb, aliasDb,
                    20.0 * std::log10(std::max(1.0e-12, thd)));
        ok = check(base.latency == expectedLatency,
                   "the reported latency matches the 15.5-sample group delay") &&
             ok;
        ok = check(std::abs(gain1kDb) < 0.5, "the 1 kHz gain is within 0.5 dB") && ok;
        ok = check(std::abs(gain10kDb) < 1.5, "the 10 kHz gain is within 1.5 dB") &&
             ok;
        ok = check(aliasDb < -10.0, "the 20 kHz image is at least 10 dB down") && ok;
    }

    bool blocksOk = true;
    const std::array<int, 6> blockSizes = {32, 64, 128, 256, 512, 1024};
    for (double rate : rates) {
        for (int blockSize : blockSizes) {
            const int frames = static_cast<int>(rate * 0.25);
            const auto measurement = run(rate, 1000.0, frames, blockSize);
            bool finite = true;
            for (float sample : measurement.left)
                finite = finite && std::isfinite(sample);
            const double level = rms(measurement.left, frames / 4, frames / 2);
            blocksOk = blocksOk && finite && level > 0.01;
        }
    }
    ok = check(blocksOk, "every 32 to 1024 frame block renders") && ok;

    std::printf("rate: %s\n", ok ? "passed" : "FAILED");
    return ok ? 0 : 1;
}
