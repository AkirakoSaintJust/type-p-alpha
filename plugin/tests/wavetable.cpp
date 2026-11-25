/* SPDX-License-Identifier: GPL-3.0-only */
/* Checks the generated 14-wave table and the analysis table. The checksums are
 * the pre-change table, so the current waves stay byte for byte after the half
 * square moves square and saw to 12 and 13. */
#include <cstddef>
#include <cstdint>
#include <cstdio>

#include "wave_harmonics.h"
#include "wavetable.h"

namespace {

constexpr uint64_t kFnvOffset = 0xcbf29ce484222325ull;
constexpr uint64_t kFnvPrime = 0x100000001b3ull;

uint64_t fnv(const int16_t* data, size_t count)
{
    uint64_t hash = kFnvOffset;
    for (size_t i = 0; i < count; ++i) {
        hash ^= static_cast<uint16_t>(data[i]);
        hash *= kFnvPrime;
    }
    return hash;
}

bool checkBase()
{
    const uint8_t expected[14] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 19, 28, 37};
    for (size_t i = 0; i < 14; ++i) {
        if (p1_wave_base[i] != expected[i]) {
            std::printf("wavetable: base %zu is %u, expected %u\n", i,
                        static_cast<unsigned>(p1_wave_base[i]),
                        static_cast<unsigned>(expected[i]));
            return false;
        }
    }
    return true;
}

bool checkHarmonics()
{
    if (p1_wave_harmonics[0][0] != 99 || p1_wave_harmonics[0][1] != 0) {
        std::printf("wavetable: the sine analysis changed\n");
        return false;
    }
    if (p1_wave_harmonics[8][0] != 0 || p1_wave_harmonics[8][1] != 99) {
        std::printf("wavetable: the rectified sine analysis changed\n");
        return false;
    }
    if (p1_wave_harmonics[9][0] != 0 || p1_wave_harmonics[9][1] != 99 ||
        p1_wave_harmonics[9][5] != 11) {
        std::printf("wavetable: the rectified triangle analysis changed\n");
        return false;
    }
    if (p1_wave_harmonics[11][0] != 99 || p1_wave_harmonics[11][1] != 0 ||
        p1_wave_harmonics[11][2] != 33) {
        std::printf("wavetable: the half square analysis changed\n");
        return false;
    }
    if (p1_wave_harmonics[12][0] != 99 || p1_wave_harmonics[12][1] != 0 ||
        p1_wave_harmonics[12][2] != 33) {
        std::printf("wavetable: the square analysis changed\n");
        return false;
    }
    if (p1_wave_harmonics[13][0] != 99 || p1_wave_harmonics[13][1] != 49 ||
        p1_wave_harmonics[13][2] != 33) {
        std::printf("wavetable: the saw analysis changed\n");
        return false;
    }
    return true;
}

bool checkSamples()
{
    constexpr size_t kWaves0To8 = 9 * 1024;
    constexpr size_t kRectTri = 9 * 1024;
    constexpr size_t kPulse = 10 * 1024;
    constexpr size_t kHalfSquare = 19 * 1024;
    constexpr size_t kSquare = 28 * 1024;
    constexpr size_t kSaw = 37 * 1024;
    if (fnv(p1_wave, kWaves0To8) != 0xcb3338ebe43ab531ull) {
        std::printf("wavetable: waves 0 to 8 do not match the old table\n");
        return false;
    }
    if (fnv(p1_wave + kPulse, 9 * 1024) != 0x556a200d14d701b4ull) {
        std::printf("wavetable: the pulse table changed\n");
        return false;
    }
    if (fnv(p1_wave + kHalfSquare, 9 * 1024) != 0x805f0ed9d6dc08b7ull) {
        std::printf("wavetable: the half square table changed\n");
        return false;
    }
    if (fnv(p1_wave + kSquare, 9 * 1024) != 0x6b392ed210eb9524ull) {
        std::printf("wavetable: the square table changed\n");
        return false;
    }
    if (fnv(p1_wave + kSaw, 9 * 1024) != 0xfad16a10324b3941ull) {
        std::printf("wavetable: the saw table changed\n");
        return false;
    }
    if (fnv(p1_wave + kRectTri, 1024) != 0x752b7188e38a5725ull) {
        std::printf("wavetable: the rectified triangle table changed\n");
        return false;
    }
    int16_t peak = 0;
    int16_t low = 0;
    for (size_t i = kRectTri; i < kRectTri + 1024; ++i) {
        if (p1_wave[i] > peak)
            peak = p1_wave[i];
        if (p1_wave[i] < low)
            low = p1_wave[i];
    }
    if (peak != 32767 || low != 0) {
        std::printf("wavetable: rectified triangle range is %d..%d\n", low, peak);
        return false;
    }
    return true;
}

} // namespace

int main()
{
    if (!checkBase() || !checkSamples() || !checkHarmonics())
        return 1;
    std::printf("wavetable: 14 waves, analysis and checksums pass\n");
    return 0;
}
