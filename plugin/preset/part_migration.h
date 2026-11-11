/* SPDX-License-Identifier: GPL-3.0-only */
/* The move of the filter, its envelope and the two followers into the voice
 * zone changed the per-part parameter block. This header carries the bridge for
 * libraries saved before that change, so a part bank written by the previous
 * release still loads. */
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

/* The old part block was 146 parameters. The first 133 kept their index, the
 * 19 filter parameters are new, and the last 8 moved to 152..159. The five
 * routing values at the old 141..145 are stored by the multi patch and are not
 * part parameters any more, so they are dropped. */
inline std::vector<int> legacyPartParamMap()
{
    constexpr int kOldPartCount = 146;
    constexpr int kOldHead = 133;
    constexpr int kOldTail = 8;
    constexpr int kNewTail = 152;
    std::vector<int> map(static_cast<std::size_t>(kOldPartCount), -1);
    for (int i = 0; i < kOldHead; ++i)
        map[static_cast<std::size_t>(i)] = i;
    for (int i = 0; i < kOldTail; ++i)
        map[static_cast<std::size_t>(kOldHead + i)] = kNewTail + i;
    return map;
}

} // namespace p1
