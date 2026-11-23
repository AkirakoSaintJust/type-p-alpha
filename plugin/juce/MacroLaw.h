/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <algorithm>

#include "MacroTargets.h"

namespace p1 {

/* Whether a target row names one parameter. A carrier or modulator group only
 * names its own role for the current algorithm, and a row that names no member
 * returns false. The index is the manifest index, which is the member id. */
inline bool macroRowIncludes(int row, int index, int algorithm)
{
    if (row < 0 || row >= macros::kTargetCount)
        return false;
    const int role = macros::kTargetRole[row];
    const auto& mask = macros::kCarrierMasks[static_cast<std::size_t>(
        std::clamp(algorithm, 0, 7))];
    const int start = macros::kTargetMemberOffsets[row];
    const int end = macros::kTargetMemberOffsets[row + 1];
    for (int i = start; i < end; ++i) {
        const auto& member =
            macros::kTargetMemberPool[static_cast<std::size_t>(i)];
        if (member.id != index)
            continue;
        if (role == 1 && (member.op == 0 || !mask[member.op - 1]))
            return false;
        if (role == 2 && (member.op == 0 || mask[member.op - 1]))
            return false;
        return true;
    }
    return false;
}

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
