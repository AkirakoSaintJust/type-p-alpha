/* SPDX-License-Identifier: GPL-3.0-only */
/* The web build's narrow C seam. The engine owns one static state, which is
 * initialised once outside the render loop. The worklet calls p1_render and
 * reads the interleaved int32 output at p1_output. */
#include <math.h>
#include <stdint.h>

#include "dsp.h"
#include "p1_math.h"  /* the bank offsets in spectral.h */
#include "wavetable.h"

#define P1_WEB_MAX_FRAMES 4096
#define P1_WEB_PI 3.14159265358979323846

typedef struct {
    type_p_alpha_state dsp;
    int32_t out[P1_WEB_MAX_FRAMES * 2];
    int32_t analysis[64];
} p1_web_state;

static p1_web_state g_state;

void *p1_create(void)
{
    type_p_alpha_dsp_state_init(&g_state.dsp);
    return &g_state;
}

void p1_reset(void)
{
    type_p_alpha_dsp_reset_s(&g_state.dsp);
}

void p1_set_param(uint32_t id, int32_t value)
{
    type_p_alpha_dsp_set_param_s(&g_state.dsp, id, value);
}

int32_t p1_get_param(uint32_t id)
{
    return type_p_alpha_dsp_get_param_s(&g_state.dsp, id);
}

void p1_note_on(uint32_t note, uint32_t velocity, uint32_t part)
{
    type_p_alpha_dsp_note_on_ch_s(&g_state.dsp, note, velocity, part);
}

void p1_note_off(uint32_t note)
{
    type_p_alpha_dsp_note_off_s(&g_state.dsp, note);
}

void p1_all_notes_off(void)
{
    type_p_alpha_dsp_all_notes_off_s(&g_state.dsp);
}

void p1_set_bend(uint32_t q16)
{
    type_p_alpha_dsp_set_bend_q16_s(&g_state.dsp, q16);
}

void p1_render(uint32_t frames)
{
    if (frames > P1_WEB_MAX_FRAMES)
        frames = P1_WEB_MAX_FRAMES;
    type_p_alpha_dsp_render_s(&g_state.dsp, g_state.out, frames);
}

int32_t *p1_output(void)
{
    return g_state.out;
}

const int32_t *p1_tap(uint32_t channel)
{
    return type_p_alpha_dsp_tap_s(&g_state.dsp, channel);
}

/* The last morph the oscillator read for an operator and part, 0 to 99, or -1
 * when the operator has not read the bank. The mod display follows it. */
int32_t p1_spec_morph(uint32_t op, uint32_t part)
{
    const int16_t *bank;
    if (part > 1u)
        return -1;
    switch (op) {
    case 0: bank = g_state.dsp.op1_osc_bank; break;
    case 1: bank = g_state.dsp.op2_osc_bank; break;
    case 2: bank = g_state.dsp.op3_osc_bank; break;
    case 3: bank = g_state.dsp.op4_osc_bank; break;
    default: return -1;
    }
    const int live = bank[P1_SPEC_LIVE + part];
    return live > 0 ? live - 1 : -1;
}

uint32_t p1_param_count(void)
{
    return type_p_alpha_dsp_param_count();
}

const int32_t *p1_analysis(void)
{
    return g_state.analysis;
}

void p1_analyze(uint32_t wave, uint32_t first)
{
    for (int i = 0; i < 64; ++i)
        g_state.analysis[i] = 0;
    if (wave > 12u)
        return;
    const int16_t *table = p1_wave + ((int)p1_wave_base[wave] << 10);
    double peak = 0.0;
    for (int h = 0; h < 64; ++h) {
        const int harmonic = (int)first + h;
        double re = 0.0;
        double im = 0.0;
        for (int n = 0; n < 1024; ++n) {
            const double x = table[n];
            const double phase =
                2.0 * P1_WEB_PI * harmonic * n / 1024.0;
            re += x * cos(phase);
            im += x * sin(phase);
        }
        const double magnitude = sqrt(re * re + im * im);
        g_state.analysis[h] = (int32_t)(magnitude + 0.5);
        if (magnitude > peak)
            peak = magnitude;
    }
    if (peak > 0.0) {
        for (int h = 0; h < 64; ++h)
            g_state.analysis[h] =
                (int32_t)(g_state.analysis[h] * 99.0 / peak + 0.5);
    }
}
