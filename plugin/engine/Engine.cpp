/* SPDX-License-Identifier: GPL-3.0-only */
#include "Engine.hpp"

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

P1_STATE* Engine::state()
{
    return state_.get();
}

} // namespace p1
