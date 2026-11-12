/* SPDX-License-Identifier: GPL-3.0-only */
/* The feedback shaper changed the per-part parameter block. This header carries
 * the bridge for libraries saved before that change, and for the envelope and
 * filter layouts before it. */
#pragma once

#include <cstdint>
#include <vector>

#include "Params.h"

namespace p1 {

/* The current part block defaults, indexed by the part-major parameter id. */
inline std::vector<int32_t> partParamDefaults()
{
    std::vector<int32_t> values(static_cast<std::size_t>(kP1PartParamCount));
    for (int i = 0; i < kP1PartParamCount; ++i)
        values[static_cast<std::size_t>(i)] =
            kP1ParamSpecs[static_cast<std::size_t>(i)].def;
    return values;
}

/* The feedback envelope moved four operator depths after the feedback levels and
 * seven envelope parameters before the mode block. This is the 160 to 171 step. */
inline int feedbackInsertionShift(int old160Index)
{
    int dest = old160Index;
    for (int point : {18, 47, 76, 105}) {
        if (old160Index > point)
            ++dest;
    }
    if (old160Index >= 152)
        dest += 7;
    return dest;
}

/* The shaper moved two parameters, shape and filter, after each operator depth.
 * This is the 171 to 179 step. */
inline int shaperInsertionShift(int old171Index)
{
    int dest = old171Index;
    for (int point : {19, 49, 79, 109}) {
        if (old171Index > point)
            dest += 2;
    }
    return dest;
}

/* The Type P.alpha layout with the envelope and without the shaper. */
inline std::vector<int> legacy171PartMap()
{
    constexpr int kOldPartCount = 171;
    std::vector<int> map(static_cast<std::size_t>(kOldPartCount), -1);
    for (int i = 0; i < kOldPartCount; ++i)
        map[static_cast<std::size_t>(i)] = shaperInsertionShift(i);
    return map;
}

/* The Type P.alpha layout before the feedback envelope. */
inline std::vector<int> legacy160PartMap()
{
    constexpr int kOldPartCount = 160;
    std::vector<int> map(static_cast<std::size_t>(kOldPartCount), -1);
    for (int i = 0; i < kOldPartCount; ++i)
        map[static_cast<std::size_t>(i)] =
            shaperInsertionShift(feedbackInsertionShift(i));
    return map;
}

/* The FM-1 layout. The first 133 kept their index, the 19 filter parameters are
 * new at 133..151, and the last 8 moved to 152..159 in the 160 layout. The five
 * routing values at 141..145 are stored by the multi patch and are dropped. */
inline std::vector<int> legacy146PartMap()
{
    constexpr int kOldPartCount = 146;
    constexpr int kOldHead = 133;
    constexpr int kOldTail = 8;
    constexpr int kOld160Tail = 152;
    std::vector<int> map(static_cast<std::size_t>(kOldPartCount), -1);
    for (int i = 0; i < kOldHead; ++i)
        map[static_cast<std::size_t>(i)] =
            shaperInsertionShift(feedbackInsertionShift(i));
    for (int i = 0; i < kOldTail; ++i)
        map[static_cast<std::size_t>(kOldHead + i)] =
            shaperInsertionShift(feedbackInsertionShift(kOld160Tail + i));
    return map;
}

/* The legacy layouts, newest first. The store matches a bank by record size. */
inline std::vector<std::vector<int>> legacyPartParamMaps()
{
    return {legacy171PartMap(), legacy160PartMap(), legacy146PartMap()};
}

} // namespace p1
