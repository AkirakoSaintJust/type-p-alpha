/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 P1Onken */
/* The oscilloscope / spectrogram analysis core (integer, CPU0 UI path). */
#pragma once
#include <stdint.h>

#define SCOPE_N 256u          /* the snapshot window (one render block) */
#define SCOPE_FS 44100u       /* the fixed DSP sample rate */
#define SCOPE_BINS 120u       /* the spectrogram bar count (2 px each on 240 px) */
#define SCOPE_CH 12u          /* the simultaneous per-op/per-part scope channels
                               * (ch 0 = the mix + 1..N_TAP_CH taps, and it must cover
                               * 1 + the synth's N_TAP_CH, type-p-fm1 has 10) */

/* Channel 0 (the legacy single-channel entry points) is the final mixed output.
 * Channels 1..SCOPE_CH-1 are the synth's scope taps (`<name>_dsp_tap`, §6.11.4). */
void scope_push(const int32_t *stereo, uint32_t n);      /* the audio ISR stores the left channel -> ch 0 */
void scope_push_ch(uint32_t ch, const int32_t *mono, uint32_t n);  /* store one tap channel */
int16_t scope_at(uint32_t i);                            /* ch 0 sample (downsampled) */
int16_t scope_at_ch(uint32_t ch, uint32_t i);            /* ch `ch` sample */
void scope_spectrum(uint16_t *mag, uint32_t nbins);      /* ch 0 Goertzel magnitudes */
void scope_spectrum_ch(uint32_t ch, uint16_t *mag, uint32_t nbins); /* ch `ch` */
uint32_t scope_seq(void);                                /* snapshot sequence (for the UI dirty check) */
