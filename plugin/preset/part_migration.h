/* SPDX-License-Identifier: GPL-3.0-only */
/* The feedback envelope changed the per-part parameter block. This header
 * carries the bridge for libraries saved before that change, so a part bank
 * written by an earlier release still loads. */
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

/* The four operator depths are inserted after each operator's feedback
 * parameter, at the old part indices 18, 47, 76 and 105. The seven feedback
 * envelope parameters are inserted before the per-part mode block, at the old
 * index 152. The old indices 152..159 move to 163..170. */
inline int feedbackInsertionShift(int oldIndex)
{
    int dest = oldIndex;
    for (int point : {18, 47, 76, 105}) {
        if (oldIndex > point)
            ++dest;
    }
    if (oldIndex >= 152)
        dest += 7;
    return dest;
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
 * new at 133..151, and the last 8 moved to 152..159. The five routing values at
 * 141..145 are stored by the multi patch and are dropped. */
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
    return {legacy160PartMap(), legacy146PartMap()};
}

} // namespace p1
