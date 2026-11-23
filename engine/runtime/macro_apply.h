/* SPDX-License-Identifier: GPL-3.0-only */
#pragma once
#include <stdint.h>

#include "macro_target_ids.h"

/* The per-voice part of the macro layer: the velocity source on the all-operator
 * level group and the all-operator feedback group. The plugin writes the knob, CC,
 * wheel and the non-velocity source offsets at part level, whilst the velocity
 * source needs each voice's own velocity, which only the engine has. The level
 * helper returns a level offset in 0..99 units for the operator's total level, and
 * the `tl` node adds it before the key and velocity follow. The feedback helper
 * returns a raw offset over the feedback span, and the `fb_tap` node adds it to the
 * stored level before the key and velocity scaling. */
static inline int32_t p1_macro_velocity_offset(const type_p_alpha_state *state,
                                               int32_t wanted,
                                               int32_t velocity,
                                               int32_t span)
{
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
            if (target[m * 3 + d] != wanted)
                continue;
            int32_t macro = gain[m] + (velocity * midiRange[m]) / 127;
            if (macro > 100)
                macro = 100;
            else if (macro < -100)
                macro = -100;
            sum += ((int64_t)macro * range[m * 3 + d] * span) / 10000;
        }
    }
    if (sum > span)
        sum = span;
    else if (sum < -span)
        sum = -span;
    return (int32_t)sum;
}

static inline int32_t p1_macro_velocity_level(const type_p_alpha_state *state,
                                              uint32_t part, uint32_t voice)
{
    const int32_t wanted = part == 0u ? P1_MACRO_TARGET_P1_OP_LEVELS
                                      : P1_MACRO_TARGET_P2_OP_LEVELS;
    return p1_macro_velocity_offset(state, wanted, state->voice_velocity[voice],
                                    99);
}

static inline int32_t p1_macro_velocity_feedback(const type_p_alpha_state *state,
                                                 uint32_t part, uint32_t voice)
{
    const int32_t wanted = part == 0u ? P1_MACRO_TARGET_P1_OP_FEEDBACKS
                                      : P1_MACRO_TARGET_P2_OP_FEEDBACKS;
    return p1_macro_velocity_offset(state, wanted, state->voice_velocity[voice],
                                    65534);
}
