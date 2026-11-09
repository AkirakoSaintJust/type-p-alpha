/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once

#include <memory>

#include "PluginEditor.h"

namespace p1::ui {

std::unique_ptr<PageBase> makeCapabilitySchemaPage(int index,
                                                   PluginEditor& editor);

} // namespace p1::ui
