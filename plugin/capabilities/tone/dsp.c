/* SPDX-License-Identifier: GPL-3.0-only */
/* Plugin-only output tone control. The generated capability chain calls the three
 * functions below. The default is off, which leaves the engine render untouched. */

#include <stdint.h>

typedef struct {
    float z[2];
    float a;
    int on;
} tone_state;

static tone_state tone;

static float tone_coeff(int32_t value)
{
    float t = value <= 0 ? 0.0f : value >= 100 ? 1.0f : (float)value / 100.0f;
    float fc = 200.0f + t * t * 19800.0f;
    float a = 6.28318530718f * fc / 44100.0f;
    return a > 1.0f ? 1.0f : a;
}

static void tone_clear(void)
{
    tone.z[0] = 0.0f;
    tone.z[1] = 0.0f;
}

void tone_init(void)
{
    tone_clear();
    tone.a = tone_coeff(100);
    tone.on = 0;
}

void tone_set_param(uint32_t local_id, int32_t value)
{
    if (local_id == 0u) {
        int on = value != 0;
        if (on != tone.on)
            tone_clear();
        tone.on = on;
    } else if (local_id == 1u) {
        tone.a = tone_coeff(value);
    }
}

static int32_t tone_round(float value)
{
    if (value >= 32767.0f)
        return 32767;
    if (value <= -32768.0f)
        return -32768;
    return (int32_t)(value + (value >= 0.0f ? 0.5f : -0.5f));
}

void tone_process(int32_t *out, uint32_t frames)
{
    float a = tone.a;
    uint32_t i;
    if (!tone.on)
        return;
    for (i = 0; i < frames; i++) {
        float l = (float)out[2u * i];
        float r = (float)out[2u * i + 1u];
        tone.z[0] += a * (l - tone.z[0]);
        tone.z[1] += a * (r - tone.z[1]);
        out[2u * i] = tone_round(tone.z[0]);
        out[2u * i + 1u] = tone_round(tone.z[1]);
    }
}
