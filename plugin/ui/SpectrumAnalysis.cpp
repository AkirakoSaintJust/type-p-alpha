/* SPDX-License-Identifier: GPL-3.0-only */
#include "SpectrumAnalysis.h"

#include <algorithm>
#include <cmath>

#include "wavetable.h"

namespace p1::ui {
namespace {

constexpr double kPi = 3.14159265358979323846;

} // namespace

std::array<double, 16> analyseWaveTable(int wave, int firstHarmonic)
{
    std::array<double, 16> levels{};
    if (wave < 0 || wave > 12)
        return levels;
    const int base = static_cast<int>(p1_wave_base[wave]) << 10;
    for (int h = 0; h < 16; ++h) {
        const int harmonic = firstHarmonic + h;
        const double step = 2.0 * kPi * harmonic / 1024.0;
        double re = 0.0;
        double im = 0.0;
        for (int n = 0; n < 1024; ++n) {
            const double x = p1_wave[base + n];
            re += x * std::cos(step * n);
            im += x * std::sin(step * n);
        }
        levels[static_cast<std::size_t>(h)] = std::hypot(re, im);
    }
    double peak = 0.0;
    for (double level : levels)
        peak = std::max(peak, level);
    if (peak > 0.0) {
        for (double& level : levels)
            level = level * 99.0 / peak;
    }
    return levels;
}

} // namespace p1::ui
