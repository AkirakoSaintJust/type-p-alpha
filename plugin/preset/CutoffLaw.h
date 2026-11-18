/* SPDX-License-Identifier: GPL-3.0-only */
/* The cutoff parameter is a log position, not a coefficient. Position 0 is
 * 3.5 Hz, position 55296 is 12 octaves above it (14.3 kHz), and the filter
 * node turns the position into the 48 kHz per-stage coefficient. These helpers
 * are for the panel, the host text and the one-time migration of patches saved
 * under the old coefficient law. */
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>

namespace p1 {

inline constexpr int32_t kCutoffPositionMax = 55296;
inline constexpr double kCutoffMinHz = 3.5;
inline constexpr double kCutoffOctaves = 12.0;
inline constexpr double kCutoffPi = 3.14159265358979323846;

inline double cutoffHzFromPosition(int32_t position)
{
    const double p = std::clamp(static_cast<double>(position), 0.0,
                                static_cast<double>(kCutoffPositionMax));
    return kCutoffMinHz * std::pow(2.0, kCutoffOctaves * p / kCutoffPositionMax);
}

inline int32_t cutoffPositionFromHz(double hz)
{
    if (hz <= kCutoffMinHz)
        return 0;
    const double p = std::log2(hz / kCutoffMinHz) / kCutoffOctaves *
                     static_cast<double>(kCutoffPositionMax);
    return static_cast<int32_t>(
        std::lround(std::clamp(p, 0.0, static_cast<double>(kCutoffPositionMax))));
}

/* The old cutoff was a coefficient in 0..55296, scaled by 60211/65536 to keep
 * the 44.1 kHz cutoff in hertz. Map the old coefficient to the frequency it
 * produced and then to the new log position, so a saved patch keeps its cutoff. */
inline int32_t cutoffOldToNew(int32_t oldValue)
{
    if (oldValue <= 0)
        return 0;
    const int32_t fc =
        static_cast<int32_t>((static_cast<int64_t>(oldValue) * 60211 + 32768) >> 16);
    if (fc <= 0)
        return 0;
    const double g = static_cast<double>(fc) / 65536.0;
    if (g >= 1.0)
        return kCutoffPositionMax;
    const double hz = -std::log(1.0 - g) * 48000.0 / (2.0 * kCutoffPi);
    return cutoffPositionFromHz(hz);
}

} // namespace p1
