/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <memory>

#include "Pages.h"

namespace p1::ui {

std::unique_ptr<PageBase> makeToneCurvePage(PluginEditor& editor);

} // namespace p1::ui
