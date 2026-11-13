/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 P1Onken
 *
 * The per-operator additive bank. The bank holds two sixteen-harmonic spectra
 * for each part, five mip levels of 256 signed 16-bit samples, plus one shadow
 * spectrum for the 10 ms crossfade. The oscillator reads the a and b tables and
 * blends them with the per-voice morph; the bank is regenerated at the control
 * rate when a level changes.
 *
 * The bank is one int16_t[8192] per operator. A 256-sample table is used for
 * every mip, and the levels carry 16, 8, 4, 2 and 1 harmonics.
 */
#pragma once
#include <stdint.h>

#define P1_SPEC_PARTS 2
#define P1_SPEC_SIZE 256
#define P1_SPEC_LEVELS 5
#define P1_SPEC_TABLE (P1_SPEC_LEVELS * P1_SPEC_SIZE)
#define P1_SPEC_HEADER 128
#define P1_SPEC_BANK 8192

#define P1_SPEC_A(p) (P1_SPEC_HEADER + (p) * P1_SPEC_TABLE)
#define P1_SPEC_B(p) \
    (P1_SPEC_HEADER + P1_SPEC_PARTS * P1_SPEC_TABLE + (p) * P1_SPEC_TABLE)
#define P1_SPEC_SHADOW(p)                                            \
    (P1_SPEC_HEADER + 2 * P1_SPEC_PARTS * P1_SPEC_TABLE +             \
     (p) * P1_SPEC_TABLE)

#define P1_SPEC_XFADE 0
#define P1_SPEC_TARGET 2
#define P1_SPEC_HASH 4
#define P1_SPEC_TARGET_HASH 8

static inline int16_t p1_spectral_clamp16(int32_t v)
{
    return (int16_t)(v > 32767 ? 32767 : (v < -32768 ? -32768 : v));
}

static inline int32_t p1_spectral_clamp99(int32_t v)
{
    return v < 0 ? 0 : (v > 99 ? 99 : v);
}

/* The morph in the 0..99 panel scale. The envelope is a Q15 gain from the
 * dx_env node, and the LFO is already in panel units because the lfo node's
 * depth is the spec_lfo_depth parameter. */
static inline int32_t p1_spectral_morph(int32_t base, int32_t depth,
                                        int32_t env_q15, int32_t lfo)
{
    const int32_t env99 = (env_q15 * 99) >> 15;
    const int32_t env_contrib = (depth * env99 * 662) >> 16;
    return p1_spectral_clamp99(base + env_contrib + lfo);
}

/* One spectrum, all five mips, peak normalised to Q15. The levels are the
 * sixteen harmonic amplitudes in 0..99. */
static inline void p1_spectral_table(int16_t *out, const int32_t *levels)
{
    int32_t raw[P1_SPEC_SIZE];
    for (int level = 0; level < P1_SPEC_LEVELS; ++level) {
        const int harmonics = 16 >> level;
        int32_t peak = 0;
        for (int i = 0; i < P1_SPEC_SIZE; ++i) {
            int32_t sum = 0;
            for (int n = 0; n < harmonics; ++n)
                sum += levels[n] * p1_sin((uint32_t)(i * (n + 1)) << 24);
            raw[i] = sum;
            const int32_t magnitude = sum < 0 ? -sum : sum;
            if (magnitude > peak)
                peak = magnitude;
        }
        const int32_t scale =
            peak > 0 ? (int32_t)((32767LL << 16) / peak) : 0;
        for (int i = 0; i < P1_SPEC_SIZE; ++i) {
            const int64_t value = (int64_t)raw[i] * scale >> 16;
            out[level * P1_SPEC_SIZE + i] = p1_spectral_clamp16((int32_t)value);
        }
    }
}

static inline uint16_t p1_spectral_hash(const int32_t *levels)
{
    uint32_t h = 0;
    for (int i = 0; i < 16; ++i)
        h = (h * 31u + (uint32_t)levels[i]) & 0xffffu;
    return (uint16_t)h;
}

static inline void p1_spectral_copy(int16_t *dst, const int16_t *src)
{
    for (int i = 0; i < P1_SPEC_TABLE; ++i)
        dst[i] = src[i];
}

/* Generate or advance the bank for both parts. Called once per control block
 * at the first voice. One spectrum changes at a time per part. */
static inline void p1_spectral_tick(
    int16_t *bank, const int32_t *waveform,
    const int32_t *a1, const int32_t *a2, const int32_t *a3,
    const int32_t *a4, const int32_t *a5, const int32_t *a6,
    const int32_t *a7, const int32_t *a8, const int32_t *a9,
    const int32_t *a10, const int32_t *a11, const int32_t *a12,
    const int32_t *a13, const int32_t *a14, const int32_t *a15,
    const int32_t *a16, const int32_t *b1, const int32_t *b2,
    const int32_t *b3, const int32_t *b4, const int32_t *b5,
    const int32_t *b6, const int32_t *b7, const int32_t *b8,
    const int32_t *b9, const int32_t *b10, const int32_t *b11,
    const int32_t *b12, const int32_t *b13, const int32_t *b14,
    const int32_t *b15, const int32_t *b16)
{
    for (int part = 0; part < P1_SPEC_PARTS; ++part) {
        if (waveform[part] != 13)
            continue;

        const int16_t xfade = bank[P1_SPEC_XFADE + part];
        if (xfade > 0) {
            const int16_t next = (int16_t)(xfade - 64);
            if (next <= 0) {
                const int target = bank[P1_SPEC_TARGET + part];
                int16_t *dst = bank + (target ? P1_SPEC_B(part) : P1_SPEC_A(part));
                p1_spectral_copy(dst, bank + P1_SPEC_SHADOW(part));
                bank[P1_SPEC_HASH + part * 2 + target] =
                    bank[P1_SPEC_TARGET_HASH + part * 2 + target];
                bank[P1_SPEC_XFADE + part] = 0;
            } else {
                bank[P1_SPEC_XFADE + part] = next;
            }
            continue;
        }

        const int32_t *const sets[2][16] = {
            {a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14,
             a15, a16},
            {b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14,
             b15, b16}};
        for (int spectrum = 0; spectrum < 2; ++spectrum) {
            int32_t levels[16];
            for (int i = 0; i < 16; ++i)
                levels[i] = sets[spectrum][i][part];
            const uint16_t hash = p1_spectral_hash(levels);
            const uint16_t committed = (uint16_t)bank[P1_SPEC_HASH +
                                                      part * 2 + spectrum];
            if (hash == committed)
                continue;
            p1_spectral_table(bank + P1_SPEC_SHADOW(part), levels);
            bank[P1_SPEC_TARGET + part] = (int16_t)spectrum;
            bank[P1_SPEC_TARGET_HASH + part * 2 + spectrum] = (int16_t)hash;
            bank[P1_SPEC_XFADE + part] = 256;
            break;
        }
    }
}

/* The table is 256 samples, so the phase fraction is 24 bits. The 1024-sample
 * wave table uses 22 bits; using that shift here plays the table four times too
 * high. */
static inline int32_t p1_spectral_lerp(const int16_t *table, uint32_t phase)
{
    const uint32_t index = phase >> 24;
    const uint32_t next = (index + 1u) & (P1_SPEC_SIZE - 1u);
    const uint32_t fraction = phase & 0x00ffffffu;
    const int32_t a = table[index];
    const int32_t b = table[next];
    return a + (int32_t)(((int64_t)(b - a) * fraction) >> 24);
}

static inline int32_t p1_spectral_blend(int32_t a, int32_t b, int32_t weight)
{
    return (a * (32768 - weight) + b * weight) >> 15;
}

/* Read the a and b tables at the mip and blend them with the morph. The mip
 * input is the phasor's nine-level selector: 0..4 use the 16-harmonic table,
 * 5 uses 8, 6 uses 4, 7 uses 2 and 8 uses 1. */
static inline int32_t p1_spectral_read(const int16_t *bank, int part,
                                       uint32_t phase, uint32_t mip,
                                       int32_t morph)
{
    const int level = mip > 8u ? 4 : (mip > 4u ? (int)(mip - 4u) : 0);
    const int16_t *a = bank + P1_SPEC_A(part) + level * P1_SPEC_SIZE;
    const int16_t *b = bank + P1_SPEC_B(part) + level * P1_SPEC_SIZE;
    int32_t av = p1_spectral_lerp(a, phase);
    int32_t bv = p1_spectral_lerp(b, phase);

    const int xfade = bank[P1_SPEC_XFADE + part];
    if (xfade > 0) {
        const int target = bank[P1_SPEC_TARGET + part];
        const int16_t *shadow =
            bank + P1_SPEC_SHADOW(part) + level * P1_SPEC_SIZE;
        const int32_t sv = p1_spectral_lerp(shadow, phase);
        const int32_t xw = xfade * 128;
        if (target == 0)
            av = p1_spectral_blend(av, sv, xw);
        else
            bv = p1_spectral_blend(bv, sv, xw);
    }
    return p1_spectral_blend(av, bv, p1_spectral_clamp99(morph) * 331);
}
