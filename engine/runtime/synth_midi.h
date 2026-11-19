/* SPDX-License-Identifier: GPL-3.0-only */
/* Copyright (C) 2026 P1Onken */
/* The MIDI handler: note on/off -> the seam's `note_on_ch`/`note_off_ch`, CC ->
 * the fixed cc-map's param (a 7-bit value scaled to the param's full [min, max]
 * range), CC 64 sustain (note-offs deferred until the pedal lifts on that
 * channel), CC 123 panic, and 14-bit pitch bend (±2 semitones) ->
 * `set_bend_q16`. Clock/transport (F8 FA FB FC) are left to the host. The
 * sustain state and the deferred notes are per channel, so two parts on
 * different channels do not hold or release each other's notes. */
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define SYNTH_CC_NONE 0xFFFFu

typedef struct {
    void (*note_on)(uint32_t note, uint32_t velocity, uint32_t channel);
    void (*note_off)(uint32_t note, uint32_t channel);
    void (*all_off)(void);
    int (*set_param)(uint32_t id, int32_t value);
    void (*set_bend)(uint32_t bend_q16);
    const uint16_t *cc_map;    /* cc_to_param[128], SYNTH_CC_NONE = unassigned */
    const int32_t *param_min;  /* per-param ranges, indexed by param id */
    const int32_t *param_max;
    uint8_t sustain_on[16];        /* per-channel pedal state */
    uint8_t sustain_pend[16][128]; /* per-channel deferred note-offs */
} synth_midi_ctx;

void synth_midi_poll(synth_midi_ctx *ctx);
void synth_midi_feed(synth_midi_ctx *ctx, uint8_t status, uint8_t data1, uint8_t data2);

#ifdef __cplusplus
}
#endif
