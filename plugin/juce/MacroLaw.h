/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <algorithm>

namespace p1 {

/* The macro source law. The knob, or the host automation on the same parameter, is
 * the centre. The selected source is 0 to 127 and the range is in macro points, so a
 * source at rest leaves the macro at the knob and a rising source adds up to its
 * range. The result is clamped to the macro's own -100 to 100. */
inline double macroEffective(int knob, int sourceValue, int range)
{
    const double source =
        static_cast<double>(std::clamp(sourceValue, 0, 127)) / 127.0;
    return std::clamp(static_cast<double>(knob) + source * range, -100.0, 100.0);
}

/* The macro destination law. The offset is a fraction of the destination's span,
 * so a macro at 100 with a range of 100 moves the whole span and half of each
 * moves half. The destination is clamped in the normalised domain, which keeps a
 * log parameter's offset musical instead of raw. */
inline double macroOffset(double macro, int range)
{
    return (macro / 100.0) * (static_cast<double>(range) / 100.0);
}

inline double macroTargetNormalised(double base, double offset)
{
    return std::clamp(base + offset, 0.0, 1.0);
}

} // namespace p1
