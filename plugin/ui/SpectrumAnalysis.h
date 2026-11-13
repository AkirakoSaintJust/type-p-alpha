/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <array>

namespace p1::ui {

/* Sixteen harmonic magnitudes of one mip-0 wave table, normalised to 0..99.
 * Wave 13 has no table and returns zeros. */
std::array<double, 16> analyseWaveTable(int wave);

} // namespace p1::ui
