/* SPDX-License-Identifier: GPL-3.0-only */
#include "MidiBridge.hpp"

#include "Engine.hpp"
#include "cc_map.h"

namespace {
thread_local p1::Engine* current = nullptr;
}

extern "C" void p1_bridge_note_on(uint32_t note, uint32_t velocity, uint32_t channel)
{
    if (current)
        current->noteOn(note, velocity, channel);
}

extern "C" void p1_bridge_note_off(uint32_t note)
{
    if (current)
        current->noteOff(note);
}

extern "C" void p1_bridge_all_off()
{
    if (current)
        current->allNotesOff();
}

extern "C" int p1_bridge_set_param(uint32_t id, int32_t value)
{
    return current ? current->setParam(id, value) : 0;
}

extern "C" void p1_bridge_set_bend(uint32_t bend)
{
    if (current)
        current->setBend(bend);
}

namespace p1 {

MidiBridge::MidiBridge(Engine& engine) : engine_(engine)
{
    ctx_.note_on = p1_bridge_note_on;
    ctx_.note_off = p1_bridge_note_off;
    ctx_.all_off = p1_bridge_all_off;
    ctx_.set_param = p1_bridge_set_param;
    ctx_.set_bend = p1_bridge_set_bend;
    ctx_.cc_map = cc_to_param;
    ctx_.param_min = param_min;
    ctx_.param_max = param_max;
}

void MidiBridge::feed(uint8_t status, uint8_t data1, uint8_t data2)
{
    current = &engine_;
    synth_midi_feed(&ctx_, status, data1, data2);
    current = nullptr;
}

void MidiBridge::reset()
{
    ctx_.sustain_on = 0;
    for (auto& pending : ctx_.sustain_pend)
        pending = 0;
}

} // namespace p1
