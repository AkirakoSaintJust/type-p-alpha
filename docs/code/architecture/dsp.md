# DSP

The DSP is one generated C render path. The graph has a global zone, a voice
zone and two parts. Each part has four operators. The generated code is in
`engine/gen/type_p_alpha_dsp.c`, the fixed-point helpers are in
`engine/hal/p1_math.h`, the spectral bank is in `engine/hal/spectral.h`, and the
phase-map layout is in `plugin/engine/PhaseMapLayout.h`.

This chapter follows the signal order in the generated per-voice loop. The
canonical block list with source lines is in
[`reference/generated-render-walk.md`](../reference/generated-render-walk.md).

## The per-sample order

At the top of each sample the voice block reads the previous operator outputs
and the previous feedback samples:

```c
int32_t d4_out_x = d4_prev[v];
int32_t d3_out_x = d3_prev[v];
int32_t d2_out_x = d2_prev[v];
int32_t d1_out_x = d1_prev[v];
int32_t op4_d1_out_x = op4_d1_prev[v];
int32_t op3_d1_out_x = op3_d1_prev[v];
int32_t op2_d1_out_x = op2_d1_prev[v];
int32_t op1_d1_out_x = op1_d1_prev[v];
```

`d1` to `d4` are the muted operator outputs from the previous sample. The
`opN_d1` values are the feedback taps from the previous sample. The router reads
the `d` values, the filter reads the router carrier sum, and then the four
operator chains compute the current sample. At the end of the sample the code
stores the new values:

```c
d1_prev[v] = op1_mute_audio_x;
op1_d1_prev[v] = op1_fb_audio_x;
```

The one-sample delay is intentional. It breaks the phase-modulation feedback
loop and makes the router and filter read a complete previous sample rather than
a partially updated operator. It also gives the generated graph a fixed
evaluation order.

The order inside the sample is:

1. The algorithm router selects the four modulation inputs and the carrier sum.
2. The filter processes the carrier sum from the previous sample.
3. The op4 chain, then op3, op2 and op1.
4. The voice mix applies the part level and the constant-power pan.
5. The tap buffers collect the four operator outputs and the carrier sum.
6. The delay state is updated.

## The phasor

Each operator has a Q32 phase accumulator. The note-derived increment is

```c
op1_ph_inc[v] = (voice_inc[v] * op1_ph_inc_ratio[part]) >> 16;
```

where `voice_inc` is the note increment after the part bend, transpose and
detune. The phasor node turns the increment into a per-sample step from the
operator pitch LFO:

```c
op4_ph_acc_x = op4_ph_phase[v];
op4_ph_phase_x = op4_ph_acc_x + ((uint32_t)op4_sum_audio_x << 20);
op4_ph_phase[v] += op4_ph_step[v];
```

`sum_audio_x` is the FM input from the router plus the feedback tap. It is a
phase offset, not a frequency offset. The phase offset is scaled by `2^20`, so a
full-scale Q15 FM input is eight cycles of phase deviation. The accumulator
advances by `ph_step` alone. That is the Yamaha model:

```text
out = wave(acc + modulator)
acc += step
```

Accumulating the modulator instead would integrate it into the frequency and
turn any modulator DC into a detune.

`ph_step` is block-derived from the base increment and the pitch LFO. The
per-sample body is one add. The selected mip is also block-derived and copied to
the oscillator. The phasor's own `ph_phase_x` (the unmapped sum) is left unused
in the shipped graph, and the generated line is cast to `(void)`.

The phase-map node sits after the accumulator and before the FM offset. It reads
the pre-FM `acc` phase and returns a warped phase in the same Q32 domain. The
`phase_add` node then forms `padd_phase = mapped + (sum << 20)`, and the
oscillator indexes with `padd_phase >> 22`. A modulator cannot warp the shape of
the map that feeds it. This is the one-rate shipping path. The designed 2x path
is not built.

## The wavetable oscillator

For waveform values 0 to 13 the oscillator reads the band-limited table:

```c
p1_wave[(p1_wave_base[wave] + (bl ? mip : 0)) << 10 | (phase >> 22)]
```

The 14 waveforms are:

| Value | Wave | Table |
| ---: | --- | --- |
| 0 | sine | 1,024 samples |
| 1 | triangle | 1,024 |
| 2 | half sine | 1,024 |
| 3 | half tri | 1,024 |
| 4 | 2x sine | 1,024 |
| 5 | 2x tri | 1,024 |
| 6 | 2x half sine | 1,024 |
| 7 | 2x half tri | 1,024 |
| 8 | rect sine | 1,024 |
| 9 | rect tri | 1,024 |
| 10 | pulse | 9 mips |
| 11 | half square | 9 mips |
| 12 | square | 9 mips |
| 13 | saw | 9 mips |
| 14 | spectrum | additive bank |

Waves 0 to 7 are the OPZ set (sine, triangle, half sine, half triangle and
their doubled pairs), 8 and 9 are the rectified sine and triangle, and 10 to 13
are the OPL-only shapes (pulse, half square, square and saw). Waves 10 to 13
carry nine band-limited mips, from a truncated Fourier synthesis with harmonic
counts 256, 128, 64, 32, 16, 8, 4, 2 and 1. The oscillator selects a mip from
`p1_wave_mip(step)`. A step is a Q32 increment, so the mip loop shifts it down
until the top harmonic sits at or below `Nyquist / 2.25`. Waves 0 to 9 have a
single 1,024-sample table and rely on their own band-limited construction.

The half square is the first half of a square held positive and the second half
held at zero. It has a DC term by construction, like the pulse and the rectified
shapes. The analysis and the spectral template drop DC.

## The additive spectral oscillator

Waveform 14 reads the additive bank instead of the wavetable. Each operator has
one `int16_t[10880]` bank. The bank holds two 64-harmonic spectra for each part,
seven mip levels of 256 samples and one shadow table for the crossfade.

The table generator `p1_spectral_table` sums the 64 harmonic levels:

```c
for (int i = 0; i < 256; ++i) {
    int32_t sum = 0;
    for (int n = 0; n < harmonics; ++n)
        sum += levels[n] * p1_sin((i * (first + n)) << 24);
    raw[i] = sum;
}
```

It then peak-normalises each mip to Q15. The level count is `64 >> mip`, so the
mip levels carry 64, 32, 16, 8, 4, 2 and 1 harmonics. The range control selects
harmonic 1 or harmonic 65. `p1_spectral_lerp` linearly interpolates the 256-sample
table with a 24-bit phase fraction. The phase shift is 24 bits, not the 22 bits
of the 1,024-sample waveform table.

The bank is regenerated on the control tick when a level or range changes.
`p1_spectral_tick` compares a 16-bit hash of the levels and the range. A changed
spectrum is generated into the shadow table. If the part has been read since the
last tick, the new table crossfades over 256 units at 17 per tick, about 15
control ticks or 10 ms. If the part has not been read, the shadow is copied
straight into place, so a cold operator has the new spectrum before its next
note. The read path sets the `USED` flag on every sample, and `p1_spectral_read`
blends the a and b tables with the per-voice morph.

The morph is `p1_spectral_clamp99(position + lfo)`. The position is the morph
knob or the absolute morph envelope. The LFO is already in panel units. The
blend weight is `morph * 331`, which maps 99 to 32769 and the Q15 blend to unity.

## The amplitude envelope

The amplitude envelope is a composite attack and two decays. The attack has a
convex warm-up from 0 to half level and a concave surge from half to full. The
warm-up uses `p1_ease_in`, whose squared progress starts slowly and accelerates.
The surge uses the one-pole. The two decays and the release use the one-pole in
the opposite direction.

The rates are 0 to 99. A rate is converted through `p1_rate_coeff`, then scaled
by the key and velocity factors and the macro rate offset. A rate of 0 on the
warm or surge stage means the stage is off. The envelope stage numbers are:

| Stage | Meaning |
| ---: | --- |
| 0 | warm-up |
| 1 | surge |
| 2 | decay 1 |
| 3 | decay 2 |
| 4 | sustain hold |
| 5 | release |
| 6 | loop up |
| 7 | loop down |

The exact generated stage logic is in the block comments at the operator
envelope. The envelope level is a 0 to 99 panel level converted through
`p1_level_gain`. The one-pole forces a minimum step of one LSB, so a slow
envelope always reaches its target rather than stalling.

## The feedback envelope and LFO

The feedback envelope is the same `mod_env` block with signed levels from
`-100` to `100`. It adds to the operator feedback level. The feedback LFO adds
its bipolar output to the same sum. The feedback tap itself is the block at the
end of each operator chain:

```c
if (op1_fb_level_eff[v]) {
    if (op1_fb_negative[v]) {
        op1_fb_shaped[v] = op1_tl_audio_x < 0 ? -op1_tl_audio_x : op1_tl_audio_x;
        op1_fb_shaped[v] = -op1_fb_shaped[v];
        op1_fb_audio_x =
            ((op1_fb_shaped[v] + op1_fb_prev[v]) * op1_fb_level_eff[v]) >> 16;
        op1_fb_prev[v] = op1_fb_shaped[v];
    } else {
        op1_fb_audio_x =
            ((op1_tl_audio_x + op1_fb_prev[v]) * op1_fb_level_eff[v]) >> 16;
        op1_fb_prev[v] = op1_tl_audio_x;
    }
}
```

A non-negative feedback level uses the signed output. A negative level takes the
absolute value, negates it and uses the same average. That is the Reface-style
pulse family. The two-sample average is the OPM's `(o + Inp_last) >> Fl`. It has a
zero at Nyquist, so it damps the one-sample feedback loop at the top of the band.
The feedback state holds its last value when the tap is gated, so turning
feedback up fades in from the previous setting rather than jumping.

## The LFOs and fades

The operator amplitude, feedback, detune and spectral LFOs share one `lfo` node.
The phase advances by `rate * CONTROL_BLOCK` per control tick. The rate is a Q32
phase increment, so the panel rate is logarithmic above the 0.01 Hz floor. The
wave is one of six LFO shapes: sine, triangle, saw, inverse saw, square and
random.

The LFO has a rate fade and a depth fade. A fade value of 0 is off, 1 is the
longest and 99 is instant. `p1_fade_step` maps the value through the envelope rate
table's 99 percent time, so a fade and an envelope rate value mean the same
duration. The fade level runs from 0 to 2^31. The LFO phase resets when either
fade is on.

The phase LFO and phase envelope are per part, not per voice. The first active
voice advances the phase LFO once per control block, and every voice reads the
held value. The phase envelope is per part and per operator, with one state for
all voices. A note trigger on any voice restarts it, and the release starts when
the part's last gate falls. The phase map uses these values as breakpoint
modulators.

## The filter

The filter is a four-pole resonant lowpass ladder in the spirit of the Ripples
Liquid Filter and the SH-101 IR3109. Four one-pole OTA stages run in series with
a negative feedback path. The cutoff is a log position over 12 octaves.
`p1_filter_coeff_q16` converts the position to the 48 kHz per-stage coefficient.
The resonance is linear from 0 to 41,942 and maps to a feedback coefficient of
6.25 at the top, so the four-pole loop reaches unity at about 79 percent and
self-oscillates through the last fifth.

The feedback path has its own soft clip. The input has a fifth-order soft knee
before the feedback subtraction, so a hot mixer compresses like the OTA pair and
the self-oscillation amplitude does not follow the input level. The passband
drops by `1 / (1 + res_k)`, so the `comp` control applies 0 to 100 percent of a
`1 + res_k` makeup to the LP4 output. The default 22 percent matches the
reference's partial compensation.

The output gate follows the carrier input. The follower has a 1.3 ms attack and a
42 ms release, a -60 dBFS threshold, and a gate that fades over 0.3 ms in and
10.7 ms out. The gate scales all four outputs. The filter therefore cannot
self-oscillate on its own after the carriers stop.

The filter modes are:

| Mode | Output |
| ---: | --- |
| 0 | LP4 |
| 1 | the 2-pole band tap |
| 2 | the 2-pole highpass, `(1 - L)^2` |

The 2-pole highpass corner lines up with the LP pole rather than the 1.55x
offset of the four-pole `(1 - L)^4` tap.

## The algorithm router and carrier sum

The router is a switch over the 10 routings. The operators are O1 to O4, and the
carriers are the operators the algorithm sums to the output. The weights are 0
or 1, so a 1-weight is the exact `p1_q15_mul(x, 32767)` scale and a 0-weight is
nothing. The switch is block-constant, so the branch predictor sees one case.

The 10 routings are:

| Alg | Routing | Carriers |
| ---: | --- | --- |
| 0 | `O1→O2→O3→O4` | O4 |
| 1 | `(O1+O2)→O3→O4` | O4 |
| 2 | `(O1+(O2→O3))→O4` | O4 |
| 3 | `O1→O2`, `O1→O3`, `O2+O3→O4` | O4 |
| 4 | `(O1→O2)+(O3→O4)` | O2, O4 |
| 5 | `O1→O3`, `O3→O2`, `O3→O4` | O2, O4 |
| 6 | `O1→O2`, `O1→O3`, `O1→O4` | O2, O3, O4 |
| 7 | `O1→O2`, O3, O4 | O2, O3, O4 |
| 8 | `O1→O2`, `O1→O3`, O4 | O2, O3, O4 |
| 9 | O1, O2, O3, O4 | all four |

The modulator outputs are multiplied by two. The operators run at -12 dB
headroom (`tl >> 2`), and the phasor scales the FM input by `2^20`. The ×2 brings
a level-99 modulator to four cycles of phase deviation, which is the OPM's
full-scale index at TL 0.

The carrier sum carries the ring modulation. The carriers are the operators the
algorithm marks as carriers, in operator order. Consecutive carriers are paired
`c1×c2`, `c2×c3`, `c3×c4`. Each product is scaled by the pair level and gated by
the pair on/off. The product is multiplied by 64 (36 dB), because the raw product
is the square of the already attenuated carriers. The direct gates apply only
while at least one active pair is on, so the feature off is exactly the
pre-feature carrier sum. A one-carrier routing keeps its single direct term and
has no ring modulation.

The carrier sum then goes to the filter. The filter output, not the raw carrier
sum, is mixed into the bus. The tap buffers keep the raw carrier sum for the
scope.

## The voice mix and output

The voice mix is

```c
int32_t _mix = p1_q15_mul(lp_out_x,
    p1_level_gain[part_level[voice_part[v]]]);
bus[f - b] = p1_sat_add32(bus[f - b],
    (_mix * pan_l_q16[part]) >> 16);
bus_r[f - b] = p1_sat_add32(bus_r[f - b],
    (_mix * pan_r_q16[part]) >> 16);
```

The part level is a 0 to 99 level through `p1_level_gain`. The pan gains are
constant-power Q16 values from the 201-entry tables. The two buses are summed
with saturation, so a chord cannot wrap.

The post block runs a one-pole DC blocker on each bus:

```c
dc_out_x = bus - dc_x1 + p1_q15_mul(dc_r, dc_y1);
dc_x1 = bus;
dc_y1 = dc_out_x;
```

The pole is about 0.999. It removes the DC of an asymmetric pulse or half square
without touching the audio band. The output is interleaved `int32_t` at the
engine boundary.

## The tap buffers and scopes

The render clears ten tap buffers before the block loop. Each voice adds its
four operator outputs and the carrier sum into the tap slot for its part:

```c
taps[4 * 2 + part][f] = p1_sat_add32(
    taps[4 * 2 + part][f],
    p1_q15_mul(route_car_x, p1_level_gain[part_level[part]]));
```

The first eight taps are the four operators for each of the two parts. Tap 8 and
9 are the part carrier sums. The processor copies them into the fixed
`ScopeRing` buffers after each core chunk. The UI reads the rings, never the
engine state.

## Fixed-point and anti-aliasing

The engine uses Q15 for signals and levels, Q16 for gains and filter
coefficients, Q31 for the LFO fade levels, and Q32 for phase. The helpers in
`p1_math.h` saturate instead of wrapping. A Q15 unity gain is 32768, so the
multiplication helpers use 64-bit intermediates and round.

The wavetable and the additive bank are the anti-aliasing layer. The wavetable
band-limited waves select a mip whose top harmonic is below `Nyquist / 2.25`. The
additive bank has seven mip levels with 64 to 1 harmonics, and it also selects
the level from the phasor mip. The naive oscillator helper in
`engine/gen/naive_wave.h` is for the prototype and the bench, not the shipped
operator path. `p1_poly_blep` is available for a naive saw, square, pulse or
triangle, but the shipped oscillator uses the mip tables.

The phase map replaces the per-sample segment lookup with an exact 1,024-entry
post-F table in the default build. `P1_PHASE_READ=11` selects the table. The
phase map aliasing limit, the 2x path and the perceptual battery are in the
phase-map contract. The direct table is sample-identical to the F/segment
reference on the 140-scenario battery.

## Limits

- Engine: 48 kHz, with the host rate adapted at the output boundary.
- Core: 24 voices and two parts. A voice is four operators.
- Additive bank: 64 harmonics per spectrum and two spectra per operator.
- Phase map: up to 32 active segments and a 4,096-point grid.
- Filter: self-oscillates through the last fifth of the resonance range, but
  the output gate follows the carriers, so it does not sound on its own.
- Ring product: not oversampled. The 6× half-square/saw texture is an
  audio-rate sideband source, not a slow gate, and high notes can fold.
