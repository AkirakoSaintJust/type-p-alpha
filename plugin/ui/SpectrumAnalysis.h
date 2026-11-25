/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>

namespace p1::ui {

/* Sixty-four harmonic magnitudes of one mip-0 wave table, normalised to 0..99.
 * `firstHarmonic` is the 1-based harmonic of the first bar. Wave 14 has no table
 * and returns zeros. */
std::array<double, 64> analyseWaveTable(int wave, int firstHarmonic = 1);

} // namespace p1::ui
