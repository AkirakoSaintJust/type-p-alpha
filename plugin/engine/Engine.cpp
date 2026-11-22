/* SPDX-License-Identifier: GPL-3.0-only */
#include "Engine.hpp"

#include <cstring>

#include "p1_math.h"  /* brings in spectral.h for the bank offsets */

namespace p1 {
namespace {

/* The phase_map pool layout. These offsets are the node's fixed
 * `int16_t[10880]` layout, indexed by part. */
constexpr uint32_t kPmWords = 2560u;
constexpr uint32_t kPmNx = 0u;
constexpr uint32_t kPmNflip = 65u;
constexpr uint32_t kPmOrder = 97u;
constexpr uint32_t kPmPy0 = 129u;
constexpr uint32_t kPmPslope = 161u;
constexpr uint32_t kPmSeg = 193u;
constexpr uint32_t kPmF = 1217u;
constexpr uint32_t kPmSeed = 2242u;
constexpr uint32_t kPmSeeded = 2243u;
constexpr uint32_t kPmBp = 2244u;
constexpr uint32_t kPmLastSegs = 2245u;
constexpr uint32_t kPmNy = 2249u;
constexpr uint32_t kPmMask = 2282u;
constexpr uint32_t kPmLock = 2411u;
constexpr uint32_t kPmActive = 2412u;

} // namespace

Engine::Engine() : state_(std::make_unique<P1_STATE>())
{
    P1_STATE_INIT(state_.get());
}

Engine::~Engine() = default;

void Engine::reset()
{
    P1_RESET(state_.get());
}

void Engine::resetPhaseMaps()
{
    std::memset(state_->op1_phase_map, 0, sizeof(state_->op1_phase_map));
    std::memset(state_->op2_phase_map, 0, sizeof(state_->op2_phase_map));
    std::memset(state_->op3_phase_map, 0, sizeof(state_->op3_phase_map));
    std::memset(state_->op4_phase_map, 0, sizeof(state_->op4_phase_map));
    std::memset(state_->op1_phase_lfo_pool, 0, sizeof(state_->op1_phase_lfo_pool));
    std::memset(state_->op2_phase_lfo_pool, 0, sizeof(state_->op2_phase_lfo_pool));
    std::memset(state_->op3_phase_lfo_pool, 0, sizeof(state_->op3_phase_lfo_pool));
    std::memset(state_->op4_phase_lfo_pool, 0, sizeof(state_->op4_phase_lfo_pool));
    std::memset(state_->op1_phase_env_pool, 0, sizeof(state_->op1_phase_env_pool));
    std::memset(state_->op2_phase_env_pool, 0, sizeof(state_->op2_phase_env_pool));
    std::memset(state_->op3_phase_env_pool, 0, sizeof(state_->op3_phase_env_pool));
    std::memset(state_->op4_phase_env_pool, 0, sizeof(state_->op4_phase_env_pool));
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

void Engine::noteOff(uint32_t note, uint32_t channel)
{
    P1_NOTE_OFF_CH(state_.get(), note, channel);
}

void Engine::allNotesOff(uint32_t channel)
{
    P1_ALL_NOTES_OFF_CH(state_.get(), channel);
}

void Engine::setBend(uint32_t bend14, uint32_t channel)
{
    P1_SET_BEND_CH(state_.get(), bend14, channel);
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

bool Engine::phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const
{
    out = PhaseMapSnapshot{};
    if (op < 0 || op > 3 || part < 0 || part >= 2)
        return false;

    const int16_t* pool = nullptr;
    const uint32_t* phase = nullptr;
    switch (op) {
    case 0:
        pool = state_->op1_phase_map;
        phase = state_->op1_ph_phase;
        break;
    case 1:
        pool = state_->op2_phase_map;
        phase = state_->op2_ph_phase;
        break;
    case 2:
        pool = state_->op3_phase_map;
        phase = state_->op3_ph_phase;
        break;
    case 3:
        pool = state_->op4_phase_map;
        phase = state_->op4_ph_phase;
        break;
    default:
        return false;
    }

    const uint32_t* pm = reinterpret_cast<const uint32_t*>(pool) +
                         static_cast<uint32_t>(part) * kPmWords;
    if (!pm[kPmSeeded])
        return false;

    uint32_t segments = pm[kPmLastSegs];
    if (segments < 1u)
        segments = 1u;
    if (segments > 32u)
        segments = 32u;
    uint32_t active = pm[kPmActive];
    if (active < 1u)
        active = 1u;
    if (active > segments)
        active = segments;

    out.valid = true;
    out.bp = static_cast<int>(pm[kPmBp]);
    out.segments = static_cast<int>(segments);
    out.active = static_cast<int>(active);
    for (uint32_t i = 0u; i <= 32u; ++i)
        out.x[i] = static_cast<uint16_t>(pm[kPmNx + i]);
    const uint32_t locks = pm[kPmLock];
    for (uint32_t k = 0u; k < segments; ++k) {
        uint32_t order = pm[kPmOrder + k];
        if (order < 1u || order > active)
            order = 1u;
        out.order[k] = static_cast<uint8_t>(order - 1u);
        out.flip[k] = static_cast<uint8_t>(
            pm[kPmNflip + order - 1u] & 1u);
        out.locked[k] = static_cast<uint8_t>(
            (locks >> (order - 1u)) & 1u);
        out.y0[k] = static_cast<uint16_t>(pm[kPmPy0 + k]);
        out.slope[k] = static_cast<int32_t>(pm[kPmPslope + k]);
    }

    int newest = -1;
    uint32_t best = 0;
    for (int voice = 0; voice < NVOICE; ++voice) {
        if (state_->voice_part[voice] != part)
            continue;
        if (!state_->voice_gate[voice] && state_->voice_rel[voice] == 0)
            continue;
        ++out.voices;
        const uint32_t age = state_->voice_age[voice];
        if (newest < 0 || age > best) {
            best = age;
            newest = voice;
        }
    }
    if (newest < 0)
        return true;

    const uint32_t x = phase[newest] >> 20;
    uint32_t q = pm[kPmF + (x >> 2)] << 2;
    if (q > 4095u)
        q = 4095u;
    uint32_t k = (pm[kPmSeg + (q >> 2)] >> ((q & 3u) * 8u)) & 0xFFu;
    if (k >= active)
        k = active - 1u;
    int32_t y = static_cast<int32_t>(pm[kPmPy0 + k]) +
                ((static_cast<int32_t>(q) -
                  static_cast<int32_t>(pm[kPmNx + k])) *
                 static_cast<int32_t>(pm[kPmPslope + k]) >> 16);
    if (y < 0)
        y = 0;
    if (y > 4095)
        y = 4095;
    out.phaseQ = static_cast<uint16_t>(q);
    out.phaseY = static_cast<uint16_t>(y);
    return true;
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
