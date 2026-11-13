/* SPDX-License-Identifier: GPL-3.0-only */
/* The web build's narrow C seam. The engine owns one static state, which is
 * initialised once outside the render loop. The worklet calls p1_render and
 * reads the interleaved int32 output at p1_output. */
#include <math.h>
#include <stdint.h>

#include "dsp.h"
#include "wavetable.h"

#define P1_WEB_MAX_FRAMES 4096
#define P1_WEB_PI 3.14159265358979323846

typedef struct {
    type_p_alpha_state dsp;
    int32_t out[P1_WEB_MAX_FRAMES * 2];
    int32_t analysis[16];
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
    for (int i = 0; i < 16; ++i)
        g_state.analysis[i] = 0;
    if (wave > 12u)
        return;
    const int16_t *table = p1_wave + ((int)p1_wave_base[wave] << 10);
    double peak = 0.0;
    for (int h = 0; h < 16; ++h) {
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
        for (int h = 0; h < 16; ++h)
            g_state.analysis[h] =
                (int32_t)(g_state.analysis[h] * 99.0 / peak + 0.5);
    }
}
