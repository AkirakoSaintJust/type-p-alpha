/* SPDX-License-Identifier: GPL-3.0-only */
/* The bipolar feedback change widened the feedback range and removed the shaper
 * and filter parameters. This header carries the bridges for the layout with those
 * controls and for the envelope, 160 and FM-1 layouts. */
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

/* The shaper layout inserted a shape and a filter after each depth, at 20 and 21,
 * 52 and 53, 84 and 85, and 116 and 117. Dropping them maps 179 back to 171. */
inline int shaperDropShift(int old179Index)
{
    if (old179Index == 20 || old179Index == 21 || old179Index == 52 ||
        old179Index == 53 || old179Index == 84 || old179Index == 85 ||
        old179Index == 116 || old179Index == 117)
        return -1;
    int dest = old179Index;
    for (int point : {21, 53, 85, 117}) {
        if (old179Index > point)
            dest -= 2;
    }
    return dest;
}

/* The shaper layout, with the separate shape and filter controls. */
inline std::vector<int> legacy179PartMap()
{
    constexpr int kOldPartCount = 179;
    std::vector<int> map(static_cast<std::size_t>(kOldPartCount), -1);
    for (int i = 0; i < kOldPartCount; ++i)
        map[static_cast<std::size_t>(i)] = shaperDropShift(i);
    return map;
}

/* The Type P.alpha layout before the feedback envelope. */
inline std::vector<int> legacy160PartMap()
{
    constexpr int kOldPartCount = 160;
    std::vector<int> map(static_cast<std::size_t>(kOldPartCount), -1);
    for (int i = 0; i < kOldPartCount; ++i)
        map[static_cast<std::size_t>(i)] = feedbackInsertionShift(i);
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
        map[static_cast<std::size_t>(i)] = feedbackInsertionShift(i);
    for (int i = 0; i < kOldTail; ++i)
        map[static_cast<std::size_t>(kOldHead + i)] =
            feedbackInsertionShift(kOld160Tail + i);
    return map;
}

/* The legacy layouts, newest first. The store matches a bank by record size. */
inline std::vector<std::vector<int>> legacyPartParamMaps()
{
    return {legacy179PartMap(), legacy160PartMap(), legacy146PartMap()};
}

} // namespace p1
