/* SPDX-License-Identifier: GPL-3.0-only */
/* The current part block defaults, indexed by the part-major parameter id. */
#pragma once

#include <cstdint>
#include <vector>

#include "Params.h"

namespace p1 {

inline std::vector<int32_t> partParamDefaults()
{
    std::vector<int32_t> values(static_cast<std::size_t>(kP1PartParamCount));
    for (int i = 0; i < kP1PartParamCount; ++i)
        values[static_cast<std::size_t>(i)] =
            kP1ParamSpecs[static_cast<std::size_t>(i)].def;
    return values;
}

} // namespace p1
