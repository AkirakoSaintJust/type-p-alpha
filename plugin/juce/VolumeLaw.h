/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <algorithm>
#include <cmath>

namespace p1 {

/* The master volume fader. The host parameter is the fader position rather than
 * a gain, so the law lives here and the processor and the header slider read the
 * same numbers. Position 0 is silent, positions 1 to 750 rise logarithmically
 * through 60 dB to unity at 750, and positions 751 to 1000 accelerate in dB to
 * +12 dB. Above unity the curve is quadratic, so the gain rises ever faster. */
inline constexpr int kVolumePositions = 1000;
inline constexpr int kVolumeUnity = 750;
inline constexpr double kVolumeFloorDb = -60.0;
inline constexpr double kVolumeCeilingDb = 12.0;

/* The host parameter id. It sits outside the manifest because the volume trims
 * the plugin output rather than a patch or a part. */
inline constexpr const char* kVolumeParamID = "output_volume";

inline double volumeDbFromPosition(int position)
{
    const int pos = std::clamp(position, 0, kVolumePositions);
    if (pos <= 0)
        return -INFINITY;
    if (pos == kVolumeUnity)
        return 0.0;
    if (pos < kVolumeUnity)
        return kVolumeFloorDb * static_cast<double>(kVolumeUnity - pos) /
               static_cast<double>(kVolumeUnity);
    const double t = static_cast<double>(pos - kVolumeUnity) /
                     static_cast<double>(kVolumePositions - kVolumeUnity);
    return kVolumeCeilingDb * t * t;
}

inline float volumeGainFromPosition(int position)
{
    const int pos = std::clamp(position, 0, kVolumePositions);
    if (pos <= 0)
        return 0.0f;
    return static_cast<float>(std::pow(10.0, volumeDbFromPosition(pos) / 20.0));
}

inline int volumePositionFromDb(double db)
{
    if (db <= kVolumeFloorDb)
        return 1;
    if (db <= 0.0)
        return std::clamp(static_cast<int>(std::lround(
                              kVolumeUnity + kVolumeUnity * db / -kVolumeFloorDb)),
                          1, kVolumeUnity);
    return std::clamp(static_cast<int>(std::lround(
                          kVolumeUnity + (kVolumePositions - kVolumeUnity) *
                                             std::sqrt(db / kVolumeCeilingDb))),
                      kVolumeUnity, kVolumePositions);
}

} // namespace p1
