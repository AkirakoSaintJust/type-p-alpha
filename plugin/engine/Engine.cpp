/* SPDX-License-Identifier: GPL-3.0-only */
#include "Engine.hpp"

#include "p1_math.h"  /* brings in spectral.h for the bank offsets */

namespace p1 {

Engine::Engine() : state_(std::make_unique<P1_STATE>())
{
    P1_STATE_INIT(state_.get());
}

Engine::~Engine() = default;

void Engine::reset()
{
    P1_RESET(state_.get());
}

int Engine::setParam(uint32_t id, int32_t value)
{
    return P1_SET_PARAM(state_.get(), id, value);
}

int32_t Engine::getParam(uint32_t id)
{
    return P1_GET_PARAM(state_.get(), id);
}

void Engine::noteOn(uint32_t note, uint32_t velocity, uint32_t channel)
{
    P1_NOTE_ON_CH(state_.get(), note, velocity, channel);
}

void Engine::noteOff(uint32_t note)
{
    P1_NOTE_OFF(state_.get(), note);
}

void Engine::allNotesOff()
{
    P1_ALL_NOTES_OFF(state_.get());
}

void Engine::setBend(uint32_t q16)
{
    P1_SET_BEND(state_.get(), q16);
}

void Engine::render(int32_t* out, uint32_t frames)
{
    P1_RENDER(state_.get(), out, frames);
}

uint32_t Engine::lit(uint32_t base)
{
    return P1_LIT(state_.get(), base);
}

bool Engine::quiet()
{
    return P1_QUIET(state_.get()) != 0;
}

const int32_t* Engine::tap(uint32_t channel)
{
    return P1_TAP(state_.get(), channel);
}

int Engine::specMorph(int op, int part) const
{
    if (part < 0 || part >= 2)
        return -1;
    const int32_t* live = nullptr;
    switch (op) {
    case 0: live = state_->op1_osc_live; break;
    case 1: live = state_->op2_osc_live; break;
    case 2: live = state_->op3_osc_live; break;
    case 3: live = state_->op4_osc_live; break;
    default: return -1;
    }
    /* The newest voice still rendering for the part wins, so a chord shows the
     * note that was played last rather than whichever voice ran last. */
    int newest = -1;
    uint32_t best = 0;
    for (int voice = 0; voice < NVOICE; ++voice) {
        if (state_->voice_part[voice] != part)
            continue;
        if (!state_->voice_gate[voice] && state_->voice_rel[voice] == 0)
            continue;
        const uint32_t age = state_->voice_age[voice];
        if (newest < 0 || age > best) {
            best = age;
            newest = voice;
        }
    }
    if (newest < 0)
        return -1;
    return live[newest] >= 0 ? live[newest] : -1;
}

P1_STATE* Engine::state()
{
    return state_.get();
}

} // namespace p1
