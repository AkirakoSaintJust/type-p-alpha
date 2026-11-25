/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once
#include <math.h>
#include <stdint.h>

#include "macro_target_ids.h"

/* The carrier masks are in the generated routing table. The engine compiles it as
 * C, so the C++ tests need the C linkage. */
#ifdef __cplusplus
extern "C" {
#endif
extern const int16_t table_routing[200];
#ifdef __cplusplus
}
#endif

/* The per-voice part of the macro layer: the velocity source on the all-operator
 * and role groups. The plugin writes the knob, CC, wheel and the non-velocity
 * source offsets at part level, whilst the velocity source needs each voice's own
 * velocity, which only the engine has. The helpers return offsets in the
 * destination's units: a level in 0..99 steps, a raw feedback offset, a morph
 * position in 0..99, an envelope rate in 0..99, an LFO rate ratio in Q16 and an
 * LFO depth over the parameter's own span. The nodes add the offset before their
 * own key and velocity follow, and each node clamps at its destination range.
 *
 * The role groups use the part's algorithm: an operator is either a carrier or a
 * modulator, so a carrier row only reaches carriers and a modulator row only
 * reaches modulators. The all row reaches every operator. */

static inline int32_t p1_macro_velocity_role_offset(
    const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op,
    int32_t span, int32_t all, int32_t carrier, int32_t modulator)
{
    if (op < 1 || op > 4)
        return 0;
    int32_t algorithm = state->algo_algorithm[part];
    if (algorithm < 0)
        algorithm = 0;
    else if (algorithm > 9)
        algorithm = 9;
    const int is_carrier =
        table_routing[algorithm * 20 + 16 + (op - 1)] != 0;

    const int32_t gain[6] = {state->macro_g1, state->macro_g2, state->macro_g3,
                             state->macro_g4, state->macro_g5, state->macro_g6};
    const int32_t source[6] = {state->macro_s1, state->macro_s2,
                               state->macro_s3, state->macro_s4,
                               state->macro_s5, state->macro_s6};
    const int32_t midiRange[6] = {
        state->macro_sr1, state->macro_sr2, state->macro_sr3,
        state->macro_sr4, state->macro_sr5, state->macro_sr6};
    const int32_t target[18] = {
        state->macro_t1a, state->macro_t1b, state->macro_t1c,
        state->macro_t2a, state->macro_t2b, state->macro_t2c,
        state->macro_t3a, state->macro_t3b, state->macro_t3c,
        state->macro_t4a, state->macro_t4b, state->macro_t4c,
        state->macro_t5a, state->macro_t5b, state->macro_t5c,
        state->macro_t6a, state->macro_t6b, state->macro_t6c};
    const int32_t range[18] = {
        state->macro_r1a, state->macro_r1b, state->macro_r1c,
        state->macro_r2a, state->macro_r2b, state->macro_r2c,
        state->macro_r3a, state->macro_r3b, state->macro_r3c,
        state->macro_r4a, state->macro_r4b, state->macro_r4c,
        state->macro_r5a, state->macro_r5b, state->macro_r5c,
        state->macro_r6a, state->macro_r6b, state->macro_r6c};
    int64_t sum = 0;
    for (int m = 0; m < 6; m++) {
        if (source[m] != 3)
            continue;
        for (int d = 0; d < 3; d++) {
            const int32_t slot = m * 3 + d;
            const int32_t wanted = target[slot];
            if (wanted != all && !(wanted == carrier && is_carrier) &&
                !(wanted == modulator && !is_carrier))
                continue;
            int32_t macro = gain[m] + (state->voice_velocity[voice] *
                                       midiRange[m]) / 127;
            if (macro > 100)
                macro = 100;
            else if (macro < -100)
                macro = -100;
            sum += ((int64_t)macro * range[slot] * span) / 10000;
        }
    }
    if (sum > span)
        sum = span;
    else if (sum < -span)
        sum = -span;
    return (int32_t)sum;
}

static inline int32_t p1_macro_velocity_level(const type_p_alpha_state *state,
                                              uint32_t part, uint32_t voice,
                                              int32_t op)
{
    return p1_macro_velocity_role_offset(
        state, part, voice, op, 99,
        part == 0u ? P1_MACRO_TARGET_P1_OP_LEVELS
                   : P1_MACRO_TARGET_P2_OP_LEVELS,
        part == 0u ? P1_MACRO_TARGET_P1_CARRIER_LEVELS
                   : P1_MACRO_TARGET_P2_CARRIER_LEVELS,
        part == 0u ? P1_MACRO_TARGET_P1_MODULATOR_LEVELS
                   : P1_MACRO_TARGET_P2_MODULATOR_LEVELS);
}

static inline int32_t p1_macro_velocity_feedback(const type_p_alpha_state *state,
                                                 uint32_t part, uint32_t voice,
                                                 int32_t op)
{
    return p1_macro_velocity_role_offset(
        state, part, voice, op, 65534,
        part == 0u ? P1_MACRO_TARGET_P1_OP_FEEDBACKS
                   : P1_MACRO_TARGET_P2_OP_FEEDBACKS,
        part == 0u ? P1_MACRO_TARGET_P1_CARRIER_FEEDBACKS
                   : P1_MACRO_TARGET_P2_CARRIER_FEEDBACKS,
        part == 0u ? P1_MACRO_TARGET_P1_MODULATOR_FEEDBACKS
                   : P1_MACRO_TARGET_P2_MODULATOR_FEEDBACKS);
}

static inline int32_t p1_macro_velocity_morph(const type_p_alpha_state *state,
                                              uint32_t part, uint32_t voice,
                                              int32_t op)
{
    return p1_macro_velocity_role_offset(
        state, part, voice, op, 99,
        part == 0u ? P1_MACRO_TARGET_P1_OP_MORPHS
                   : P1_MACRO_TARGET_P2_OP_MORPHS,
        part == 0u ? P1_MACRO_TARGET_P1_CARRIER_MORPHS
                   : P1_MACRO_TARGET_P2_CARRIER_MORPHS,
        part == 0u ? P1_MACRO_TARGET_P1_MODULATOR_MORPHS
                   : P1_MACRO_TARGET_P2_MODULATOR_MORPHS);
}

static inline int32_t p1_macro_velocity_env_rate(const type_p_alpha_state *state,
                                                 uint32_t part, uint32_t voice,
                                                 int32_t op)
{
    return p1_macro_velocity_role_offset(
        state, part, voice, op, 99,
        part == 0u ? P1_MACRO_TARGET_P1_OP_ENV_RATES
                   : P1_MACRO_TARGET_P2_OP_ENV_RATES,
        part == 0u ? P1_MACRO_TARGET_P1_CARRIER_ENV_RATES
                   : P1_MACRO_TARGET_P2_CARRIER_ENV_RATES,
        part == 0u ? P1_MACRO_TARGET_P1_MODULATOR_ENV_RATES
                   : P1_MACRO_TARGET_P2_MODULATOR_ENV_RATES);
}

/* The LFO rate is a Q32 phase increment, and the panel range is logarithmic
 * above the off stop, so the macro offset is a frequency ratio rather than a raw
 * delta. The base of the exponent is the panel's 223 Hz top over its 0.01 Hz
 * floor, the same ratio the parameter range uses. A zero offset returns unity
 * exactly, which keeps a macro at 0 bit-exact. */
static inline uint32_t p1_macro_velocity_lfo_rate_ratio(
    const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)
{
    const int32_t offset = p1_macro_velocity_role_offset(
        state, part, voice, op, 10000,
        part == 0u ? P1_MACRO_TARGET_P1_OP_LFO_RATES
                   : P1_MACRO_TARGET_P2_OP_LFO_RATES,
        part == 0u ? P1_MACRO_TARGET_P1_CARRIER_LFO_RATES
                   : P1_MACRO_TARGET_P2_CARRIER_LFO_RATES,
        part == 0u ? P1_MACRO_TARGET_P1_MODULATOR_LFO_RATES
                   : P1_MACRO_TARGET_P2_MODULATOR_LFO_RATES);
    if (offset == 0)
        return 65536u;
    const double ratio =
        pow(20000000.0 / 894.0, (double)offset / 10000.0);
    const double scaled = ratio * 65536.0;
    if (scaled >= 4294967295.0)
        return 4294967295u;
    return (uint32_t)(scaled + 0.5);
}

static inline int32_t p1_macro_velocity_lfo_depth(const type_p_alpha_state *state,
                                                  uint32_t part, uint32_t voice,
                                                  int32_t op, int32_t span)
{
    return p1_macro_velocity_role_offset(
        state, part, voice, op, span,
        part == 0u ? P1_MACRO_TARGET_P1_OP_LFO_DEPTHS
                   : P1_MACRO_TARGET_P2_OP_LFO_DEPTHS,
        part == 0u ? P1_MACRO_TARGET_P1_CARRIER_LFO_DEPTHS
                   : P1_MACRO_TARGET_P2_CARRIER_LFO_DEPTHS,
        part == 0u ? P1_MACRO_TARGET_P1_MODULATOR_LFO_DEPTHS
                   : P1_MACRO_TARGET_P2_MODULATOR_LFO_DEPTHS);
}
