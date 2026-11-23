/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <algorithm>

namespace p1 {

/* The macro destination law. The offset is a fraction of the destination's span,
 * so a macro at 100 with a range of 100 moves the whole span and half of each
 * moves half. The destination is clamped in the normalised domain, which keeps a
 * log parameter's offset musical instead of raw. */
inline double macroOffset(int macro, int range)
{
    return (static_cast<double>(macro) / 100.0) *
           (static_cast<double>(range) / 100.0);
}

inline double macroTargetNormalised(double base, double offset)
{
    return std::clamp(base + offset, 0.0, 1.0);
}

} // namespace p1
