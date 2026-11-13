/* SPDX-License-Identifier: GPL-3.0-only
 * Copyright (C) 2026 P1Onken
 *
 * Fixed-point math contract (docs/roadmap.md P2-3). Integer-first, "Q in the
 * name" (dsp-study §6). The saturating Q arithmetic and the generated tables are
 * integer only. The FFT (kiss_fft, float) is opt-in: define P1_MATH_FFT in the
 * one TU that needs it. The plugin uses the portable integer path.
 */
#pragma once
#include <stdint.h>
#include "math_tables.h"                       /* p1_sin_q15 / p1_exp2_q16 / p1_log2_q16 */
#include "wavetable.h"                         /* p1_wave (the 13-wave OPZ+OPL union) */
#include "naive_wave.h"                        /* p1_naive_wave (the 4 naive shapes) */

/* These helpers belong in the hot loop, so they are always inline. The plugin
 * host inlines them at the call site. */
#define P1_MATH_INLINE static inline __attribute__((always_inline))

/* --------------------------------------------------- saturating Q arithmetic */

/* Q15 x Q15 -> Q15, rounded. The per-sample multiply for Q15 signals/levels. */
P1_MATH_INLINE int32_t p1_q15_mul(int32_t a, int32_t b)
{
    return (int32_t)(((int64_t)a * b + (1 << 14)) >> 15);
}

/* Q15 x Q15 -> Q15, rounded, for operands already known to fit the Q15 range
 * (-32768..32768, including the 32768 unity gain). The product is below 2^30,
 * so a 32-bit multiply is exact and avoids the 64-bit pair MAC. The caller owns
 * the range proof. */
P1_MATH_INLINE int32_t p1_q15_mul32(int32_t a, int32_t b)
{
    return (int32_t)((a * b + (1 << 14)) >> 15);
}

/* Q16 x Q16 -> Q16, rounded (gains, coefficients). */
P1_MATH_INLINE int32_t p1_q16_mul(int32_t a, int32_t b)
{
    return (int32_t)(((int64_t)a * b + (1 << 15)) >> 16);
}

/* Q31 x Q31 -> Q31, rounded. */
P1_MATH_INLINE int32_t p1_q31_mul(int32_t a, int32_t b)
{
    return (int32_t)(((int64_t)a * b + (1 << 30)) >> 31);
}

/* Rounding right shift (the SDK's PSHR). */
P1_MATH_INLINE int32_t p1_shr_round(int32_t x, int shift)
{
    return (x + (1 << (shift - 1))) >> shift;
}

/* Saturating 32-bit add/sub (accumulation never wraps). */
P1_MATH_INLINE int32_t p1_sat_add32(int32_t a, int32_t b)
{
    int32_t r = (int32_t)((uint32_t)a + (uint32_t)b);
    if (((a ^ r) & (b ^ r)) < 0)
        return a < 0 ? INT32_MIN : INT32_MAX;
    return r;
}

P1_MATH_INLINE int32_t p1_sat_sub32(int32_t a, int32_t b)
{
    int32_t r = (int32_t)((uint32_t)a - (uint32_t)b);
    if (((a ^ b) & (a ^ r)) < 0)
        return a < 0 ? INT32_MIN : INT32_MAX;
    return r;
}

/* Saturating multiply-accumulate: acc += (a * b) >> shift. */
P1_MATH_INLINE int32_t p1_madd_sat(int32_t acc, int32_t a, int32_t b, int shift)
{
    int32_t p = (int32_t)(((int64_t)a * b + ((int64_t)1 << (shift - 1))) >> shift);
    return p1_sat_add32(acc, p);
}

/* A one-pole toward `target`: level += (target - level) * coeff / 65536.
 * When the rounded step is 0 (a small coeff times a small remaining delta) the
 * step is forced to ±1, so the envelope always converges to `target` instead of
 * stalling just short of it. That stall is what shows up as "DECAY holds
 * forever" at low rates. */
P1_MATH_INLINE int32_t p1_onepole(int32_t level, int32_t target, int32_t coeff)
{
    int32_t d = target - level;
    int32_t u = (int32_t)(((int64_t)d * coeff + ((int64_t)1 << 15)) >> 16);
    if (u == 0) {
        if (d > 0)
            u = 1;
        else if (d < 0)
            u = -1;
        else
            return level;                       /* already at target */
    }
    return p1_sat_add32(level, u);
}

/* ------------------------------------------------------ transcendental tables */

/* Full-cycle sine, Q15, from the quarter table (p1_sin_q15, midpoint-sampled).
 * `phase` is Q32 (wrapping accumulator: inc = f * 2^32 / FS). */
P1_MATH_INLINE int32_t p1_sin(uint32_t phase)
{
    uint32_t q = phase >> 30;                 /* quadrant 0..3 */
    uint32_t i = (phase >> 22) & 255u;        /* 8-bit index into the 256-entry table */
    int32_t v = p1_sin_q15[q & 1u ? 255u - i : i];
    return (q & 2u) ? -v : v;
}

P1_MATH_INLINE int32_t p1_cos(uint32_t phase) { return p1_sin(phase + 0x40000000u); }

/* The 13-waveform union of the OPZ (YM2414) and OPL2/OPL3 (YM3812) sets, from
   ymfm's `opz_registers` and `opl_registers_base` constructors (BSD-3-Clause,
   re-implemented in linear Q15), plus the rectified triangle. `phase` is Q32
   (full cycle), and `ws` is 0..12.
   0-7 are the OPZ set (sine, triangle, half sine, half triangle, double sine,
   double triangle, double half sine, double half triangle), 8 is the rectified
   sine, 9 is the rectified triangle, and 10-12 are the OPL-only shapes (pulse,
   square, saw). A bad `ws` degrades to sine.
   Precomputed into gen/wavetable.h (`p1_wave`, tools/gen_wavetable.py): waves
   0-9 are one 1024-sample table each, whilst 10 (pulse) / 11 (square) / 12 (saw)
   are nine band-limited tables each (harmonic counts 256..1) for the top-octave
   anti-aliasing. `p1_wave_base[ws]` is ws's mip-0 offset, and this helper reads
   the richest (mip-0) table. The oscillator node adds the `mip` selected from
   its note's increment. */
P1_MATH_INLINE int32_t p1_sin_wave(uint32_t phase, uint32_t ws)
{
    if (ws > 12u) ws = 0u;   /* out of range degrades to sine (not a masked OOB read) */
    return p1_wave[((uint32_t)p1_wave_base[ws] << 10) | (phase >> 22)];
}

/* The band-limited-table mip index for a carrier increment `step` (Q32, samples
   per cycle): the level whose top harmonic stays at or below Nyquist/4. The
   harmonic count per level is 2^(8-mip) (256..1), and the selection is one octave
   conservative, so a vibrato or detune of up to +/-1 octave never folds a harmonic
   above Nyquist. The shift loop keeps the block-rate code small, because this runs
   while the flash is busy. */
P1_MATH_INLINE uint32_t p1_wave_mip(uint32_t step)
{
    uint32_t mip = 0u;
    while (step >= 0x00400000u && mip < 8u) {
        step >>= 1;
        ++mip;
    }
    return mip;
}

/* ------------------------------------------------------ key scaling (key-follow) */
/* The bipolar key-follow factor (Q15): 0 at the breakpoint, ramping linearly to
 * `hi` at note 127 (the top octave) or `lo` at note 0 (the lowest octave).
 * `lo`/`hi` are signed percent (-100..+100): +N pushes the target toward its
 * maximum, -N toward its minimum. `note` and `bp` are 0..127 MIDI notes. */
P1_MATH_INLINE int32_t p1_key_scale(uint32_t note, uint32_t bp, int32_t lo, int32_t hi)
{
    if (note > bp) {
        uint32_t t = (uint32_t)(((note - bp) << 15) / (127u - bp));   /* 0..32767 */
        return (int32_t)(((int64_t)hi * (int32_t)t) / 100);
    }
    if (note < bp) {
        uint32_t t = (uint32_t)(((bp - note) << 15) / bp);            /* 0..32767 */
        return (int32_t)(((int64_t)lo * (int32_t)t) / 100);
    }
    return 0;                                 /* at the breakpoint: no deviation */
}

/* Apply a bipolar key-follow factor `k` (Q15, p1_key_scale) to a target value
 * `v` in [min, max]: +k pushes toward `max`, -k toward `min` (proportional to the
 * remaining range, so the value clamps at the extreme). */
P1_MATH_INLINE int32_t p1_key_apply(int32_t v, int32_t k, int32_t min, int32_t max)
{
    if (k >= 0) {
        int32_t push = (int32_t)(((int64_t)(max - v) * k) >> 15);
        return v + push;
    }
    int32_t push = (int32_t)(((int64_t)(v - min) * k) >> 15);   /* k < 0: toward min */
    return v + push;
}

/* The velocity follow factor (Q15): 0 at velocity 0, ramping linearly to `amount`
 * at maximum velocity. `vel` is `voice_velocity[v]` (velocity × 258, so 127 →
 * ~32766, ≈ Q15 normalised), and `amount` is a signed percent (-100..+100). */
P1_MATH_INLINE int32_t p1_vel_scale(int32_t vel, int32_t amount)
{
    return (int32_t)(((int64_t)amount * vel) / 3276700);
}

/* Sum two bipolar Q15 push factors (key-follow + velocity-follow), clamped to the
 * Q15 range so `p1_key_apply` never overshoots its target extreme. */
P1_MATH_INLINE int32_t p1_key_add(int32_t a, int32_t b)
{
    int32_t s = a + b;
    if (s > 32767) s = 32767;
    else if (s < -32767) s = -32767;
    return s;
}

/* Clamp an int32 to [lo, hi]. */
P1_MATH_INLINE int32_t p1_clamp(int32_t v, int32_t lo, int32_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

/* Clamp an int32 to the Q16 range [0, 65535] (the SVF cutoff coefficient). */
P1_MATH_INLINE int32_t p1_clamp16(int32_t v)
{
    if (v < 0) return 0;
    if (v > 65535) return 65535;
    return v;
}

/* Clamp an int32 to [0, max]. The ladder filter uses this for the one-pole
   coefficient, which is unstable in the feedback loop above its own ceiling. */
P1_MATH_INLINE int32_t p1_clamp16_max(int32_t v, int32_t max)
{
    if (v < 0) return 0;
    if (v > max) return max;
    return v;
}

/* 4-input weighted sum against a routing-matrix row (a `const int16_t *` of four
   0/1 Q15 weights): out = a*w0 + b*w1 + c*w2 + d*w3. The multiply stays, because
   the weights are runtime table data, so a select would be a data-dependent
   branch per term and measured slower than the straight-line MAC. */
P1_MATH_INLINE int32_t p1_route4(const int16_t *w, int32_t a, int32_t b, int32_t c, int32_t d)
{
    return p1_q15_mul(a, w[0]) + p1_q15_mul(b, w[1])
         + p1_q15_mul(c, w[2]) + p1_q15_mul(d, w[3]);
}

/* p1_exp2_q16[i] = 2^(-i/256) in Q16  (exponential decay: envelopes, note-to-inc).
 * p1_log2_q16[i] = log2(1 + i/256) in Q16  (log-domain FM). Both indexed directly. */

/* 2^(octave + semi/12 + cent/1200) in Q16: the note-derived tuning ratio (the
 * naive oscillator's octave/semi/cent knobs). `octave` -3..3, `semi` -11..11,
 * `cent` -100..100 (clamped, with the octave part an exact shift and the semitone
 * and cent parts from the p1_semi_q16 / p1_cent_q16 tables). */
P1_MATH_INLINE int32_t p1_tune_q16(int32_t octave, int32_t semi, int32_t cent)
{
    if (octave < -3) octave = -3; else if (octave > 3) octave = 3;
    if (semi < -11) semi = -11; else if (semi > 11) semi = 11;
    if (cent < -100) cent = -100; else if (cent > 100) cent = 100;
    int32_t cents = octave * 1200 + semi * 100 + cent;   /* -4800..4800 */
    int32_t oct = cents / 1200;                          /* floor division */
    int32_t rem = cents - oct * 1200;
    if (rem < 0) { rem += 1200; oct -= 1; }              /* rem in [0, 1200) */
    int32_t r = p1_semi_q16[rem / 100];
    r = (int32_t)(((int64_t)r * p1_cent_q16[rem % 100]) >> 16);
    if (oct >= 0) return (int32_t)((uint32_t)r << oct);
    return (int32_t)((uint32_t)r >> (-oct));
}

/* The 2nd-order PolyBLEP residual (Tale / Martin Finke, "Making Audio Plugins
 * Part 18"). Returns the Q15 correction for a unit step at phase 0: the value to
 * layer on the naive waveform around its discontinuity. `p` is the Q32 phase
 * position (0..2^32), `inc` the Q32 increment, `inv_inc` = 2^32/inc (note-on
 * reciprocal) so u = p/inc is `(uint64)p * inv_inc` with no per-sample divide.
 *
 *   saw:     out = 2t - 1 - blep(t)
 *   square:  out = (t < 0.5 ? 1 : -1) + blep(t) - blep((t + 0.5) mod 1)
 *   pulse:   the second blep moves to the pulse width: blep((t + 1 - pw) mod 1)
 *   triangle: the polyBLEP'd square through a leaky integrator (one extra state).
 */
P1_MATH_INLINE int32_t p1_poly_blep(uint32_t p, uint32_t inc, uint32_t inv_inc)
{
    if (p < inc) {
        /* just after the wrap: u = p/inc (Q32), residual 2u - u^2 - 1 (negative). */
        uint32_t u = (uint32_t)((uint64_t)p * inv_inc);
        int64_t q = (int64_t)(2ull * u - (((uint64_t)u * u) >> 32)); /* 2u - u^2 */
        return (int32_t)((q - 0x100000000LL) >> 17);
    }
    if (p > 0u - inc) {
        /* just before the wrap: w = 2^32 - p, residual (1 - w/inc)^2. */
        uint32_t w = 0u - p;
        uint32_t u = (uint32_t)((uint64_t)w * inv_inc);
        uint64_t omt = 0x100000000ull - u;          /* 1 - w/inc */
        return (int32_t)((omt * omt) >> 49);
    }
    return 0;
}


/* ------------------------------------------------------- additive bank */
#include "spectral.h"


/* ------------------------------------------------------- FFT (float, opt-in) */
#ifdef P1_MATH_FFT
#include "kiss_fft/kiss_fft.h"
typedef kiss_fft_cfg p1_fft_cfg;
typedef kiss_fft_cpx p1_fft_cpx;
#define p1_fft_alloc(nfft, inverse, mem, lenmem) kiss_fft_alloc(nfft, inverse, mem, lenmem)
#define p1_fft(cfg, in, out) kiss_fft(cfg, in, out)
#define p1_fft_free kiss_fft_free
#define p1_fft_next_fast_size kiss_fft_next_fast_size
#endif
