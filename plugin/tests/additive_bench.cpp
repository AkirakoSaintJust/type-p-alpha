/* SPDX-License-Identifier: GPL-3.0-only */
/* Headless additive table experiment.
 *
 * The baseline is the real engine at 24 voices and four operators. The table
 * generator is the proposed shared-part algorithm, measured in isolation at the
 * 16/32 harmonic and 256/512 sample matrix. The read model measures the extra
 * table read and morph blend that a per-voice morph adds. The audition writes a
 * short WAV from a simple table oscillator, not from the engine.
 */
#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "Engine.hpp"

namespace {

constexpr int kRate = 48000;
constexpr int kVoices = 24;
constexpr int kOperators = 4;
constexpr int kParts = 2;
constexpr int kTables = kOperators * kParts;
constexpr double kPi = 3.14159265358979323846;

double secondsSince(std::chrono::steady_clock::time_point start)
{
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start)
        .count();
}

int16_t clampQ15(int32_t value)
{
    return static_cast<int16_t>(std::max(-32768, std::min(32767, value)));
}

std::vector<int16_t> sineTable(int size)
{
    std::vector<int16_t> table(static_cast<std::size_t>(size));
    for (int i = 0; i < size; ++i) {
        const double phase = 2.0 * kPi * i / size;
        table[static_cast<std::size_t>(i)] =
            static_cast<int16_t>(std::lround(std::sin(phase) * 32767.0));
    }
    return table;
}

std::vector<int16_t> recipe(int harmonics, double tilt, int formant, double peak)
{
    std::vector<double> raw(static_cast<std::size_t>(harmonics));
    double total = 0.0;
    for (int h = 1; h <= harmonics; ++h) {
        double value = std::pow(static_cast<double>(h), -tilt);
        if (formant > 0) {
            const double distance = static_cast<double>(h - formant);
            value += peak * std::exp(-distance * distance / 8.0);
        }
        raw[static_cast<std::size_t>(h - 1)] = value;
        total += value;
    }
    const double scale = total > 0.0 ? 0.95 / total : 0.0;
    std::vector<int16_t> amps(static_cast<std::size_t>(harmonics));
    for (int h = 1; h <= harmonics; ++h) {
        amps[static_cast<std::size_t>(h - 1)] = static_cast<int16_t>(
            std::lround(raw[static_cast<std::size_t>(h - 1)] * scale * 32767.0));
    }
    return amps;
}

void generateTable(std::vector<int16_t>& table, int harmonics, int size,
                   const std::vector<int16_t>& amps,
                   const std::vector<int16_t>& sine, int updates,
                   uint64_t& checksum)
{
    const int mask = size - 1;
    for (int update = 0; update < updates; ++update) {
        for (int i = 0; i < size; ++i) {
            int32_t sum = 0;
            for (int h = 1; h <= harmonics; ++h) {
                sum += (static_cast<int32_t>(amps[static_cast<std::size_t>(h - 1)]) *
                        sine[static_cast<std::size_t>((h * i) & mask)]) >>
                       15;
            }
            table[static_cast<std::size_t>(i)] = clampQ15(sum);
            checksum += static_cast<uint16_t>(table[static_cast<std::size_t>(i)]);
        }
    }
}

double baselineSeconds()
{
    p1::Engine engine;
    engine.reset();
    for (int i = 0; i < kVoices; ++i)
        engine.noteOn(static_cast<uint32_t>(48 + i * 2), 100, 0);
    std::vector<int32_t> output(static_cast<std::size_t>(kRate) * 2u);
    engine.render(output.data(), kRate); /* settle the envelopes */

    constexpr int kSeconds = 20;
    double best = 1e9;
    for (int run = 0; run < 3; ++run) {
        output.assign(static_cast<std::size_t>(kRate) * 2u * kSeconds, 0);
        const auto start = std::chrono::steady_clock::now();
        engine.render(output.data(), static_cast<uint32_t>(kRate * kSeconds));
        best = std::min(best, secondsSince(start));
    }
    return best;
}

double readModelSeconds(int tableSize)
{
    std::vector<int16_t> a(static_cast<std::size_t>(tableSize), 0);
    std::vector<int16_t> b(static_cast<std::size_t>(tableSize), 0);
    for (int i = 0; i < tableSize; ++i) {
        a[static_cast<std::size_t>(i)] =
            static_cast<int16_t>(std::lround(std::sin(2.0 * kPi * i / tableSize) * 12000.0));
        b[static_cast<std::size_t>(i)] =
            static_cast<int16_t>(std::lround(std::cos(2.0 * kPi * i / tableSize) * 12000.0));
    }
    const int shift = tableSize == 256 ? 24 : 23;
    const int mask = tableSize - 1;
    const int samples = kRate * 10;
    int64_t checksum = 0;
    double best = 1e9;
    for (int run = 0; run < 3; ++run) {
        const auto start = std::chrono::steady_clock::now();
        for (int voice = 0; voice < kOperators * kVoices; ++voice) {
            uint32_t phase = static_cast<uint32_t>(voice * 104729);
            const uint32_t step = static_cast<uint32_t>(30000 + voice * 137);
            for (int i = 0; i < samples; ++i) {
                const int index = static_cast<int>(phase >> shift) & mask;
                const int mix = 16384;
                const int32_t value =
                    (static_cast<int32_t>(a[static_cast<std::size_t>(index)]) *
                         (32768 - mix) +
                     static_cast<int32_t>(b[static_cast<std::size_t>(index)]) * mix) >>
                    15;
                checksum += value;
                phase += step;
            }
        }
        best = std::min(best, secondsSince(start));
    }
    volatile int64_t sink = checksum;
    (void)sink;
    return best;
}

double readTable(const std::vector<int16_t>& table, double phase)
{
    const double position = phase * table.size();
    const auto index = static_cast<std::size_t>(position) % table.size();
    const auto next = (index + 1u) % table.size();
    const double fraction = position - std::floor(position);
    return table[index] + (table[next] - table[index]) * fraction;
}

void writeWav(const std::string& path, const std::vector<int16_t>& left,
              const std::vector<int16_t>& right)
{
    std::ofstream file(path, std::ios::binary);
    if (!file)
        return;
    const auto dataBytes = static_cast<uint32_t>(left.size() * 2u * 2u);
    auto write32 = [&file](uint32_t value) {
        const char bytes[4] = {static_cast<char>(value & 0xFF),
                               static_cast<char>((value >> 8) & 0xFF),
                               static_cast<char>((value >> 16) & 0xFF),
                               static_cast<char>((value >> 24) & 0xFF)};
        file.write(bytes, 4);
    };
    auto write16 = [&file](uint16_t value) {
        const char bytes[2] = {static_cast<char>(value & 0xFF),
                               static_cast<char>((value >> 8) & 0xFF)};
        file.write(bytes, 2);
    };
    file.write("RIFF", 4);
    write32(36u + dataBytes);
    file.write("WAVEfmt ", 8);
    write32(16);
    write16(1);
    write16(2);
    write32(kRate);
    write32(kRate * 4);
    write16(4);
    write16(16);
    file.write("data", 4);
    write32(dataBytes);
    for (std::size_t i = 0; i < left.size(); ++i) {
        write16(static_cast<uint16_t>(left[i]));
        write16(static_cast<uint16_t>(right[i]));
    }
}

void audition(const std::string& path)
{
    constexpr int kSize = 512;
    constexpr int kHarmonics = 16;
    constexpr double kSeconds = 4.0;
    const auto sine = sineTable(kSize);
    const auto darkAmps = recipe(kHarmonics, 2.2, 0, 0.0);
    const auto brightAmps = recipe(kHarmonics, 0.7, 6, 1.2);
    std::vector<int16_t> dark(static_cast<std::size_t>(kSize));
    std::vector<int16_t> bright(static_cast<std::size_t>(kSize));
    uint64_t checksum = 0;
    generateTable(dark, kHarmonics, kSize, darkAmps, sine, 1, checksum);
    generateTable(bright, kHarmonics, kSize, brightAmps, sine, 1, checksum);

    const int samples = static_cast<int>(kRate * kSeconds);
    const int notes[4] = {60, 64, 67, 71};
    std::array<double, 4> phase{};
    std::vector<int16_t> left(static_cast<std::size_t>(samples));
    std::vector<int16_t> right(static_cast<std::size_t>(samples));
    for (int i = 0; i < samples; ++i) {
        const double t = static_cast<double>(i) / kRate;
        const double mix = std::min(1.0, t / kSeconds);
        double sum = 0.0;
        for (int n = 0; n < 4; ++n) {
            const double frequency = 440.0 * std::pow(2.0, (notes[n] - 69) / 12.0);
            const double a = readTable(dark, phase[static_cast<std::size_t>(n)]);
            const double b = readTable(bright, phase[static_cast<std::size_t>(n)]);
            sum += a * (1.0 - mix) + b * mix;
            phase[static_cast<std::size_t>(n)] += frequency / kRate;
            if (phase[static_cast<std::size_t>(n)] >= 1.0)
                phase[static_cast<std::size_t>(n)] -= 1.0;
        }
        const double fade = std::min(1.0, t / 0.02) *
                            std::min(1.0, (kSeconds - t) / 0.02);
        const double value = sum / (4.0 * 32768.0) * 0.7 * fade;
        const auto pcm = static_cast<int16_t>(
            std::max(-32768.0, std::min(32767.0, value * 32767.0)));
        left[static_cast<std::size_t>(i)] = pcm;
        right[static_cast<std::size_t>(i)] = pcm;
    }
    writeWav(path, left, right);
    std::printf("audition: %s (%d harmonics, dark to bright morph)\n",
                path.c_str(), kHarmonics);
}

struct WaveTable {
    int size = 0;
    int harmonics = 0;
    std::vector<std::vector<int16_t>> mips;
};

int mipLevels(int harmonics)
{
    int levels = 1;
    while ((harmonics >> (levels - 1)) > 1)
        ++levels;
    return levels;
}

int mipFor(double fundamental, int harmonics, int levels)
{
    int mip = 0;
    while (mip + 1 < levels && (harmonics >> mip) * fundamental > kRate * 0.25)
        ++mip;
    return mip;
}

WaveTable buildWaveTable(int harmonics, int size,
                         const std::vector<int16_t>& amps)
{
    WaveTable table;
    table.size = size;
    table.harmonics = harmonics;
    const int levels = mipLevels(harmonics);
    const auto sine = sineTable(size);
    table.mips.resize(static_cast<std::size_t>(levels));
    for (int level = 0; level < levels; ++level) {
        const int limit = std::max(1, harmonics >> level);
        auto& mip = table.mips[static_cast<std::size_t>(level)];
        mip.assign(static_cast<std::size_t>(size), 0);
        uint64_t checksum = 0;
        generateTable(mip, limit, size, amps, sine, 1, checksum);
    }
    return table;
}

double readWave(const WaveTable& table, int mip, double phase, bool linear)
{
    const auto& samples = table.mips[static_cast<std::size_t>(mip)];
    const double position = phase * table.size;
    const auto index = static_cast<std::size_t>(position) % table.size;
    double value = samples[index];
    if (linear) {
        const auto next = (index + 1u) % table.size;
        const double fraction = position - std::floor(position);
        value += (samples[next] - value) * fraction;
    }
    return value / 32768.0;
}

double magnitude(const std::vector<double>& samples, double frequency)
{
    constexpr int kWindow = 4096;
    double re = 0.0;
    double im = 0.0;
    double weight = 0.0;
    for (int i = 0; i < kWindow; ++i) {
        const double w = 0.5 - 0.5 * std::cos(2.0 * kPi * i / (kWindow - 1));
        const double phase = 2.0 * kPi * frequency * i / kRate;
        re += samples[static_cast<std::size_t>(i)] * w * std::cos(phase);
        im -= samples[static_cast<std::size_t>(i)] * w * std::sin(phase);
        weight += w;
    }
    return 2.0 * std::hypot(re, im) / weight;
}

double aliasFloor(const WaveTable& table, int mip, bool linear,
                  double fundamental, int firstAlias, int harmonics)
{
    constexpr int kWindow = 4096;
    std::vector<double> output(static_cast<std::size_t>(kWindow));
    double phase = 0.0;
    const double step = fundamental / kRate;
    for (int i = 0; i < kWindow; ++i) {
        output[static_cast<std::size_t>(i)] = readWave(table, mip, phase, linear);
        phase += step;
        if (phase >= 1.0)
            phase -= 1.0;
    }
    const double fundamentalMagnitude = magnitude(output, fundamental);
    if (fundamentalMagnitude < 1e-12)
        return 0.0;
    double aliasEnergy = 0.0;
    for (int h = firstAlias; h <= harmonics; ++h) {
        double alias = std::fmod(h * fundamental, static_cast<double>(kRate));
        if (alias > kRate * 0.5)
            alias = kRate - alias;
        if (alias < 20.0)
            continue;
        const double m = magnitude(output, alias);
        aliasEnergy += m * m;
    }
    return 10.0 * std::log10(aliasEnergy /
                                 (fundamentalMagnitude * fundamentalMagnitude) +
                             1e-30);
}

void aliasQuality()
{
    std::printf("\nalias floor at C7, dB relative to the fundamental\n");
    std::printf("  H   N  mip  nearest  linear\n");
    const double fundamental = 440.0 * std::pow(2.0, (96.0 - 69.0) / 12.0);
    const int legal = static_cast<int>(kRate * 0.5 / fundamental);
    for (int harmonics : {16, 32}) {
        const auto amps = recipe(harmonics, 1.0, 6, 0.8);
        for (int size : {256, 512}) {
            const auto table = buildWaveTable(harmonics, size, amps);
            const int levels = mipLevels(harmonics);
            const int mip = mipFor(fundamental, harmonics, levels);
            for (int on = 1; on >= 0; --on) {
                const int level = on != 0 ? mip : 0;
                const double nearest =
                    aliasFloor(table, level, false, fundamental, legal + 1,
                               harmonics);
                const double linear =
                    aliasFloor(table, level, true, fundamental, legal + 1,
                               harmonics);
                std::printf(" %2d %3d  %3s  %7.1f  %7.1f\n", harmonics, size,
                            on != 0 ? "on" : "off", nearest, linear);
            }
        }
    }
}

int mipForBright(double fundamental, int harmonics, int levels)
{
    int mip = 0;
    while (mip + 1 < levels && (harmonics >> mip) * fundamental > kRate * 0.5)
        ++mip;
    return mip;
}

double harmonicEnergy(const WaveTable& table, int mip, bool linear,
                      double fundamental, int first, int last)
{
    constexpr int kWindow = 4096;
    std::vector<double> output(static_cast<std::size_t>(kWindow));
    double phase = 0.0;
    const double step = fundamental / kRate;
    for (int i = 0; i < kWindow; ++i) {
        output[static_cast<std::size_t>(i)] = readWave(table, mip, phase, linear);
        phase += step;
        if (phase >= 1.0)
            phase -= 1.0;
    }
    const double base = magnitude(output, fundamental);
    if (base < 1e-12)
        return -120.0;
    double energy = 0.0;
    for (int h = first; h <= last; ++h) {
        const double m = magnitude(output, h * fundamental);
        energy += m * m;
    }
    if (energy < 1e-24)
        return -120.0;
    return 10.0 * std::log10(energy / (base * base));
}

double tableError(const WaveTable& table, int mip, bool linear,
                  double fundamental, const std::vector<int16_t>& amps,
                  int limit)
{
    constexpr int kWindow = 4096;
    std::vector<double> output(static_cast<std::size_t>(kWindow));
    double phase = 0.0;
    const double step = fundamental / kRate;
    for (int i = 0; i < kWindow; ++i) {
        output[static_cast<std::size_t>(i)] = readWave(table, mip, phase, linear);
        phase += step;
        if (phase >= 1.0)
            phase -= 1.0;
    }
    double difference = 0.0;
    double ideal = 0.0;
    phase = 0.0;
    for (int i = 0; i < kWindow; ++i) {
        double reference = 0.0;
        for (int h = 1; h <= limit; ++h) {
            reference += amps[static_cast<std::size_t>(h - 1)] / 32768.0 *
                         std::sin(2.0 * kPi * h * phase);
        }
        const double error = output[static_cast<std::size_t>(i)] - reference;
        difference += error * error;
        ideal += reference * reference;
        phase += step;
        if (phase >= 1.0)
            phase -= 1.0;
    }
    if (ideal < 1e-24)
        return -120.0;
    return 10.0 * std::log10(difference / ideal + 1e-30);
}

void qualityExtras()
{
    std::printf("\nmip brightness at C7, dB relative to the fundamental\n");
    std::printf("  H   N  conservative  bright\n");
    const double c7 = 440.0 * std::pow(2.0, (96.0 - 69.0) / 12.0);
    for (int harmonics : {16, 32}) {
        const auto amps = recipe(harmonics, 1.0, 6, 0.8);
        for (int size : {256, 512}) {
            const auto table = buildWaveTable(harmonics, size, amps);
            const int levels = mipLevels(harmonics);
            const int conservative = mipFor(c7, harmonics, levels);
            const int bright = mipForBright(c7, harmonics, levels);
            const double c = harmonicEnergy(table, conservative, true, c7, 5, 8);
            const double b = harmonicEnergy(table, bright, true, c7, 5, 8);
            std::printf(" %2d %3d  %7.1f       %7.1f\n", harmonics, size, c, b);
        }
    }

    std::printf("\nlow-note interpolation, 32/256, dB relative to the ideal\n");
    std::printf(" note  nearest  linear\n");
    const auto amps = recipe(32, 1.0, 6, 0.8);
    const auto table = buildWaveTable(32, 256, amps);
    const int levels = mipLevels(32);
    for (int note : {48, 60}) {
        const double fundamental =
            440.0 * std::pow(2.0, (note - 69) / 12.0);
        const int mip = mipFor(fundamental, 32, levels);
        const int limit = 32 >> mip;
        const double nearest =
            tableError(table, mip, false, fundamental, amps, limit);
        const double linear =
            tableError(table, mip, true, fundamental, amps, limit);
        std::printf(" %4d  %7.1f  %7.1f\n", note, nearest, linear);
    }
}

double oscillatorSeconds(bool additive)
{
    constexpr int kHarmonics = 16;
    constexpr int kSize = 256;
    constexpr int kSeconds = 10;
    const auto dark = buildWaveTable(kHarmonics, kSize,
                                     recipe(kHarmonics, 2.2, 0, 0.0));
    const auto bright = buildWaveTable(kHarmonics, kSize,
                                       recipe(kHarmonics, 0.7, 6, 1.2));
    const int levels = mipLevels(kHarmonics);
    std::array<uint32_t, 80> phase{};
    std::array<uint32_t, 80> step{};
    std::array<int, 80> mip{};
    for (int voice = 0; voice < kVoices; ++voice) {
        const double fundamental =
            440.0 * std::pow(2.0, (48.0 + voice * 2.0 - 69.0) / 12.0);
        for (int op = 0; op < kOperators; ++op) {
            const auto index = static_cast<std::size_t>(voice * kOperators + op);
            step[index] =
                static_cast<uint32_t>(fundamental / kRate * 4294967296.0);
            mip[index] = mipFor(fundamental, kHarmonics, levels);
        }
    }
    const int samples = kRate * kSeconds;
    double best = 1e9;
    int64_t checksum = 0;
    for (int run = 0; run < 3; ++run) {
        phase.fill(0);
        const auto start = std::chrono::steady_clock::now();
        for (int i = 0; i < samples; ++i) {
            for (int voice = 0; voice < kVoices; ++voice) {
                for (int op = 0; op < kOperators; ++op) {
                    const auto index =
                        static_cast<std::size_t>(voice * kOperators + op);
                    const double p = phase[index] / 4294967296.0;
                    if (additive) {
                        const double a = readWave(dark, mip[index], p, true);
                        const double b = readWave(bright, mip[index], p, true);
                        checksum += static_cast<int64_t>((a + b) * 1000.0);
                    } else {
                        checksum += static_cast<int64_t>(
                            readWave(dark, 0, p, false) * 1000.0);
                    }
                    phase[index] += step[index];
                }
            }
        }
        best = std::min(best, secondsSince(start));
    }
    volatile int64_t sink = checksum;
    (void)sink;
    return best;
}

void auditionOscillator(const std::string& path)
{
    constexpr int kHarmonics = 16;
    constexpr int kSize = 512;
    constexpr double kSeconds = 4.0;
    const auto dark = buildWaveTable(kHarmonics, kSize,
                                     recipe(kHarmonics, 2.2, 0, 0.0));
    const auto bright = buildWaveTable(kHarmonics, kSize,
                                       recipe(kHarmonics, 0.7, 6, 1.2));
    const int levels = mipLevels(kHarmonics);
    const int notes[4] = {60, 64, 67, 71};
    std::array<double, 4> phase{};
    std::array<int, 4> mip{};
    for (int n = 0; n < 4; ++n) {
        const double f = 440.0 * std::pow(2.0, (notes[n] - 69) / 12.0);
        mip[static_cast<std::size_t>(n)] = mipFor(f, kHarmonics, levels);
    }
    const int samples = static_cast<int>(kRate * kSeconds);
    std::vector<int16_t> left(static_cast<std::size_t>(samples));
    std::vector<int16_t> right(static_cast<std::size_t>(samples));
    double mix = 0.0;
    double target = 0.0;
    double start = 0.0;
    int blockPosition = 0;
    for (int i = 0; i < samples; ++i) {
        const double t = static_cast<double>(i) / kRate;
        if (i % 128 == 0) {
            start = mix;
            target = std::min(1.0, t / kSeconds);
            blockPosition = 0;
        }
        const double mixNow =
            start + (target - start) * (blockPosition / 128.0);
        mix = mixNow;
        double sum = 0.0;
        for (int n = 0; n < 4; ++n) {
            const double f = 440.0 * std::pow(2.0, (notes[n] - 69) / 12.0);
            const auto index = static_cast<std::size_t>(n);
            const double a = readWave(dark, mip[index], phase[index], true);
            const double b = readWave(bright, mip[index], phase[index], true);
            sum += a * (1.0 - mixNow) + b * mixNow;
            phase[index] += f / kRate;
            if (phase[index] >= 1.0)
                phase[index] -= 1.0;
        }
        const double fade = std::min(1.0, t / 0.02) *
                            std::min(1.0, (kSeconds - t) / 0.02);
        const double value = sum / 4.0 * 0.7 * fade;
        const auto pcm = static_cast<int16_t>(
            std::max(-32768.0, std::min(32767.0, value * 32767.0)));
        left[static_cast<std::size_t>(i)] = pcm;
        right[static_cast<std::size_t>(i)] = pcm;
        ++blockPosition;
    }
    writeWav(path, left, right);
    std::printf("audition: %s (16 harmonics, 344 Hz morph, 128-sample ramp)\n",
                path.c_str());
}

} // namespace

int main(int argc, char** argv)
{
    const std::string wavPath = argc > 1 ? argv[1] : "additive-headless.wav";
    const std::string oscWav = argc > 2 ? argv[2] : "additive-oscillator.wav";
    std::printf("additive synthesis headless experiment\n");
    std::printf("voices %d, operators %d, parts %d, rate %d\n", kVoices,
                kOperators, kParts, kRate);

    const double base = baselineSeconds();
    std::printf("baseline: 24 voices, 20 s render: %.1f ms, 60 s: %.1f ms, "
                "%.2f%% of realtime\n",
                base * 1000.0, base * 3000.0, base / 20.0 * 100.0);

    const int harmonicsList[2] = {16, 32};
    const int sizeList[2] = {256, 512};
    const int rates[4] = {172, 344, 689, 1378};
    constexpr int kUpdateSeconds = 10;
    uint64_t checksum = 0;

    std::printf("\ngeneration matrix, %d tables, %d s of audio\n", kTables,
                kUpdateSeconds);
    std::printf("  H   N  rate   gen ms  60 s ms  %% realtime  work M/s  meas M/s\n");
    for (int harmonics : harmonicsList) {
        for (int size : sizeList) {
            const auto amps = recipe(harmonics, 1.0, 6, 0.8);
            const auto sine = sineTable(size);
            std::vector<int16_t> table(static_cast<std::size_t>(size));
            for (int rate : rates) {
                const int updates = rate * kUpdateSeconds;
                double elapsed = 1e9;
                for (int run = 0; run < 3; ++run) {
                    const auto start = std::chrono::steady_clock::now();
                    for (int tableIndex = 0; tableIndex < kTables; ++tableIndex)
                        generateTable(table, harmonics, size, amps, sine, updates,
                                      checksum);
                    elapsed = std::min(elapsed, secondsSince(start));
                }
                const double per60 = elapsed * 6.0;
                const double work = static_cast<double>(kTables) * harmonics *
                                    size * rate / 1e6;
                const double measured = static_cast<double>(kTables) * harmonics *
                                        size * updates / elapsed / 1e6;
                std::printf(" %2d %3d %5d  %7.1f  %7.1f  %9.2f  %8.1f  %7.0f\n",
                            harmonics, size, rate, elapsed * 1000.0,
                            per60 * 1000.0, per60 / 60.0 * 100.0, work,
                            measured);
            }
        }
    }

    const double read = readModelSeconds(256);
    std::printf("\nmorph read model: 80 operator voices, 2 tables, 10 s render: "
                "%.1f ms, 60 s: %.1f ms, %.2f%% of realtime\n",
                read * 1000.0, read * 6000.0, read / 10.0 * 100.0);

    const double single = oscillatorSeconds(false);
    const double additive = oscillatorSeconds(true);
    std::printf("\noscillator read, 24 voices, 4 operators, 16/256 tables\n");
    std::printf("  single table, nearest: %.1f ms / 60 s, %.2f%% of realtime\n",
                single * 6000.0, single / 10.0 * 100.0);
    std::printf("  two tables, linear, mip: %.1f ms / 60 s, %.2f%% of realtime\n",
                additive * 6000.0, additive / 10.0 * 100.0);
    std::printf("  extra for the morph path: %.2f%% of realtime\n",
                (additive - single) / 10.0 * 100.0);

    aliasQuality();
    qualityExtras();

    audition(wavPath);
    auditionOscillator(oscWav);
    std::printf("checksum %llu\n", static_cast<unsigned long long>(checksum));
    return 0;
}
