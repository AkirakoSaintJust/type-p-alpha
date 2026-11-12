# Feedback research: envelopes and negative feedback

This is the design note for lines 1 and 2 of the research list:

1. Feedback envelopes. Are they worth it, and where do they belong without
   overcomplicating patch design?
2. Is negative feedback viable?

The first plan was to answer line 2 with one polarity bit per operator and then
design the envelope. A preliminary check and the engine measurement change the
expectation. The sign flip does not give a reliable odd-harmonic rise in the useful
feedback range, so the polarity is a bounded experiment rather than a promised
feature. The experiment failed its stop criterion at 25 and 50 percent feedback, so
the toggle is not shipped. The envelope modulates the feedback magnitude, which the
spectrum does follow, so it is the next feature. The contract is
[`../plugin/feature-contract-feedback-polarity.md`](../plugin/feature-contract-feedback-polarity.md)
and the measured table is
[`../plugin/evidence/feedback-polarity.md`](../plugin/evidence/feedback-polarity.md).

## What the engine does now

The `fm_op` macro contains the operator's phasor, wave reader, DX envelope, total
level and self-feedback. The feedback path is:

```
tl.audio -> fb_tap.audio -> d1.in -> d1.out -> sum.in1
fmvel.audio -> sum.in0
sum.audio -> ph.fm
```

`fb_tap` is the operator's level-gained output, averaged over the current and
previous sample, scaled by `level_eff`, and delayed one sample before it is added
to the external modulation. The two-sample average puts a zero at Nyquist, which
damps the one-sample loop at the top of the band. The level gate makes feedback at
0 cost nothing.

`level_eff` is

```c
p1_key_apply(param.level, p1_key_add(in.k, in.vk), 0, 32767)
```

so the feedback amount is `0..32767`, and the key and velocity followers push it
toward 0 or 32767. The `tl` level is already applied before the tap, so the
feedback follows the operator's own output level. The feedback amount is a
fraction of that output, not a separate gain after the operator.

The current feedback is positive. The README says it follows Yamaha's traditional
one, with no negative feedback. There is no feedback envelope. The amplitude LFO
can reach the total level, and the key and velocity followers can reach the
feedback amount, but the feedback amount itself is static for the note.

## Negative feedback

The term covers three different things, and the difference matters here.

In an amplifier, negative feedback subtracts a fraction of the output from the
input. It lowers the gain and the distortion, and the residual distortion is often
odd-order because a symmetric push-pull or differential stage cancels the even-order
terms. That loop is error correction around a gain stage.

In a self-oscillating loop, the sign sets the phase of the loop. An odd-symmetric
nonlinearity with an inverting loop can settle into a square-like limit cycle with
odd harmonics, whilst a non-inverting loop with an asymmetric nonlinearity can
produce even harmonics. The waveform symmetry, rather than the sign alone, decides
the harmonic series.

This synth's feedback is neither of those. The operator output is added to its own
phase, with no error amplifier and no separate nonlinearity. The sign is the sign of
the modulation index `b`. For a sine carrier the sideband amplitudes are `|J_n(b)|`,
and `J_n(-b) = (-1)^n J_n(b)`. The sign flips the phases of the odd sidebands and
leaves the magnitudes. It does not turn a sine into an odd-harmonic series.

A sign flip can still be audible through the one-sample delay and the two-sample
average, which make the loop causal and slightly asymmetric in time. The model below
measures how much. A different tap, such as a one-pole lowpass, would break the
symmetry more and make the sign a real timbre control, but that is a different
feedback character. The same is true of an odd-symmetric nonlinearity in the loop,
such as a hard clip or a wavefolder. Those are separate experiments, and they are
where the odd-harmonic association actually comes from.

The README notes that the Reface DX and the Seqtrak use a different feedback from
the traditional Yamaha one. Without their implementation, a sign flip of this tap is
the smallest test of the research question, not a claim about those instruments.

The hypothesis was that the positive loop reinforces the saw-like family and the
negative loop reinforces the square-like family, so the third and fifth harmonics
would rise relative to the second.

That hypothesis does not survive for the reason above. The one-sample delay and the
two-sample average break the symmetry slightly, because the loop is causal, but the
break is small where the feedback is useful.

A floating-point model of the tap and the phasor puts numbers on this. The `tl`
level 99 output is about 8191 in Q15 after the `>>2` headroom, and the tap at 100
percent produces about 8190 in Q15. The phasor scales the FM input by 2^20, so that
is a phase offset of about two cycles at full feedback. The model renders a sine at
note 60, applies the two-sample average and the one-sample delay, and measures the
second and third harmonics with a Hann-windowed DFT. The feedback amount is the
knob percentage.

| Feedback | A2/A1 positive | A2/A1 negative | A3/A2 positive | A3/A2 negative |
| --- | --- | --- | --- | --- |
| 20% | 0.485 | 0.485 | 0.560 | 0.560 |
| 25% | 0.613 | 0.613 | 0.521 | 0.521 |
| 35% | 0.600 | 0.874 | 1.363 | 1.432 |
| 50% | 0.746 | 0.726 | 0.220 | 0.106 |
| 75% | 2.024 | 5.186 | 0.477 | 0.450 |
| 100% | 0.452 | 0.438 | 0.579 | 0.698 |

The 20 and 25 percent rows are identical to three decimals. At 35 percent the
second-harmonic amplitude is five percent lower, but the fundamental drops by a
third, so the ratios move apart and the third harmonic is about 1.4 times the
second for both signs. At 50 percent the negative third harmonic is half the
positive one. At 75 percent and 100 percent the waveform is chaotic, and the ratios
do not form a clean odd-harmonic rise. The model is a preliminary check, not the
product evidence, but it points one way: a single sign bit does not give a
consistent odd-harmonic rise. The engine measurement below confirms the
inconsistency and adds a regime change the model does not show.

The safe seam, if the experiment proceeds, is the `fb_tap` output:

```c
int32_t out = ((audio + prev) * level_eff) >> 16;
if (param.neg)
    out = -out;
```

This keeps the positive path bit-exact. It also applies the sign after the
magnitude is formed, so the key and velocity followers and the range proof are
unchanged. The maximum product is the current one, `(audio + prev) * 32767`, which
stays inside `int32_t`. The output DC blocker handles any offset the loop
introduces.

### The bounded experiment

The experiment renders the engine with the sign flip, measures the same table, and
applies a stop criterion. It passes only if the third-to-second and
fifth-to-second ratios rise by at least 25 percent relative to the positive loop at
25, 50 and 75 percent feedback. If any of those three amounts fails, the sign flip
is not shipped. The parameter, the toggle and the migration are not added, and the
measurement is kept as the answer to line 2.

### The engine measurement

The engine measurement confirms the model's direction. At 20 and 25 percent the two
signs are the same within 2 percent. At 35 and 50 percent the negative feedback
lowers the odd ratios. At 75 percent the negative ratios rise, but the positive
second harmonic is nearly zero, so the ratio is unstable and the row is a regime
artefact. At 100 percent the negative ratios fall again. The stop fires at 25 and
50 percent. The full table is in
[`../plugin/evidence/feedback-polarity.md`](../plugin/evidence/feedback-polarity.md).

The positive path stayed bit-exact. `engine_parity` matched the captured reference,
and the 0 percent positive and negative renders were bit-identical. The 100 percent
negative peak was 3 percent lower than the positive peak, so there is no
instability.

The sign flip is therefore not a reliable odd-harmonic control. The 25 and 50
percent failures stop the feature. The parameter, the toggle and the 160-to-164
migration are not shipped. The 160 layout stays current.

### Parameter and UI, not shipped

The experiment failed, so none of this was added. The details remain as the record
of what the ship phase would have needed.

One switch per operator:

| Parameter | Target | Range | Default |
| --- | --- | --- | --- |
| `op1_fb_neg.p0` ... `op4_fb_neg.p1` | `opN.fb.neg` | 0 = positive, 1 = negative | 0 |

The operator card already has the feedback knob. The switch would sit next to the
waveform choice as a small `neg` toggle, with the `polarity` display showing `pos`
or `neg`. The parameter count would grow by 4 per part, 160 to 164, and the total
from 334 to 342.

### Persistence, not shipped

The experiment failed, so the migration was not used. The details remain as the
record of what the ship phase would have needed.

The new parameters are inserted by the `fm_op` macro after each operator's
`feedback` parameter, so the part record grows from 160 to 164 parameters. The
insertion points are the old part indices 18, 47, 76 and 105, which are the four
`opN_feedback.p0` parameters. Every old index above an insertion point moves up by
one, and the four new slots take the default 0.

The `.p1lib` reader currently accepts the 146-parameter FM-1 layout. It must also
accept the 160-parameter Type P.alpha layout. The clean fix is a list of legacy
layouts in `PresetStore`, each with an index map. The 146 map is composed with the
160-to-164 shift, because the old 146 head also crosses the four insertion points.
The 146 tail lands at the new 156..163 instead of 152..159, and the filter block
moves to 137..155. The reader picks the layout by the bank's record size. The test
covers the 146 and 160 layouts, a current save, and a round-trip after the
migration.

## The nonlinear feedback path

The sign-flip result closes the plain sign flip, not negative feedback as a whole.
The primary source points to a different mechanism. The kunstmusik Csound example
that makes a sine into a square does not use a negative sign. It squares the
feedback signal before the phase input:

```text
acar = tablei(aphs + acar*acar*kfb, 1, 1, 0, 1)
```

The 2024 comment on that article adds the important distinction. With the normal,
unshaped PM feedback, a negative beta only reverses the phase. With an absolute
value or another odd phaseshaper, the negative beta selects a different part of the
shaped wave, so positive and negative produce different shapes. The article's
author also says the Reface's smoothing filter is more aggressive than the DX7
two-sample average.

That gives a real experiment. The feedback path can carry a shaper, and the sign
becomes meaningful inside the shaper. The candidate shapers are:

| Shaper | Formula | Character |
| --- | --- | --- |
| off | `x` | The current path |
| square | `x*x` | Even, adds DC, the article's square |
| abs | `abs(x)` | Even, adds DC, the comment's guess for the Reface |
| signed square | `x*abs(x)` | Odd, no DC, a symmetric alternative |

The filter is the second half. The current tap averages two samples. A one-pole
lowpass with a coefficient around 0.5 is more aggressive and should tame the noisy
top of the feedback range. The experiment can use a compile-time one-pole first, so
the shaper is the only new parameter until the filter earns one.

The parameter cost is a shape choice and a filter choice per operator, eight per
part. The part block grows from 171 to 179 and the manifest from 356 to 372, with a
171-to-179 migration. The three controls, depth, shape and filter, fit on the
feedback page's four-column operator card.

The measurement passed. At 50 percent feedback the square moves the
high-frequency energy from 858 to 5.6 and the abs moves the third harmonic from
0.132 to 0.263, while their negative forms differ. The one-pole lowers the 75
percent high-frequency energy from 975.7 to 578.9 for the abs and from 113.2 to 11.3
for the square. The signed square keeps the same magnitude for both signs, so it is
a phase variant rather than a tone control. The peaks stay bounded. The shaper and
the filter are shipped, and the evidence is in
[`../plugin/evidence/feedback-shaper.md`](../plugin/evidence/feedback-shaper.md).
The contract is
[`../plugin/feature-contract-feedback-shaper.md`](../plugin/feature-contract-feedback-shaper.md).

## Feedback envelopes

The question is where a feedback envelope belongs. Three placements fit the
current architecture:

| Placement | Parameters per part | Flexibility | UI cost |
| --- | --- | --- | --- |
| One envelope per operator | 7 x 4 = 28 | Highest, every operator has its own shape | Four envelope cards or a deep operator page |
| One shared envelope and one depth | 7 + 1 = 8 | The shape is shared, the depth is global | One card, one depth |
| One shared envelope and one depth per operator | 7 + 4 = 11 | The shape is shared, each operator has its own depth | One card, four depth knobs |

The per-operator envelope is the most flexible and the least attractive. It adds
28 parameters per part and four shapes to edit, which is the opposite of the
small-parameter design rule. A player who wants a different feedback shape on each
operator is already in a niche of a niche.

The shared envelope with a per-operator depth is the best balance. It uses the
existing voice-zone `dx_env` (four rates, three levels), retriggers with the voice,
and feeds each `fm_op`'s feedback path. The depth is in feedback-level units, so
the envelope can open or close the feedback amount over the note. The shared
envelope with one depth is the smaller alternative if the per-operator depth turns
out to be unnecessary.

Placement in the graph:

```
fbenv (dx_env, voice zone) -> fbenv.cv -> op1.fb_env, op2.fb_env, ...
opN.fb_env (depth Q15) -> opN.fb.audio level sum
```

The envelope is per part in its settings and per voice in its state, like the
pitch envelope. It is not a global envelope, because the feedback is per voice and
a global envelope would make four operators share a shape across all voices.

The `fb_tap` level becomes

```
level_eff = clamp(
    p1_key_apply(level, k + vk, 0, 32767)
    + p1_q15_mul(fbenv, depth),
    0, 32767)
```

The envelope depth can be positive or negative. A negative depth closes the
feedback as the envelope opens. The existing feedback level is the base, so the
envelope is an offset rather than a replacement. With depth 0 the positive path is
bit-exact, because the added term is 0 and the clamp sees the existing value.

The envelope is worth implementing because the spectrum follows the feedback
magnitude, which the sign flip does not change. The open question is whether the
sweep earns 11 parameters and a new page, and a listening test decides the useful
range and shape. The polarity experiment comes first because it is the research
question, and the envelope contract follows the measurement. The envelope design
does not depend on the polarity result.

### UI placement

The FX/MOD page already has six cards in a three by two grid. The feedback
envelope could replace the pitch envelope on that page and move the pitch envelope
to its own page, or the envelope card could sit on a new `feedback` page under
FX/MOD. The new page is the cleaner option: the envelope shape is easier to read at
full width, and the operator cards only gain one depth knob. The control map and
the mockup are part of the envelope contract.

### Envelope measurement

The shared envelope is now measured on the engine. The test uses one sine operator
at note 60, `fbenv` r1 at 50, r2..r4 at 99 and l1..l3 at 99, and 16,384 samples.
Depth 0 is bit-identical. A +100 percent depth with a base of 0 raises the
third-to-fundamental ratio by 22 percent. A -100 percent depth with a base of 50
percent lowers the spectral centroid by 65 percent. The dynamic peaks are 35 and 27
percent above the endpoint statics, inside the 50 percent margin. The 146 and 160
libraries load, the current layout round-trips, and the 356 parameters reach the
APVTS. The engine side is done. The page, the mockup and the snapshots are in, and
the evidence is in
[`../plugin/evidence/feedback-envelope.md`](../plugin/evidence/feedback-envelope.md).

## Measurement method

The polarity experiment uses one sine operator at ratio 1, level 99, note 60,
44.1 kHz, with a flat DX envelope (`r1..r4` and `l1..l3` at 99), the other three
operators muted, and the filter off. It renders 8,192 samples and skips the first
2,048 so the attack and the phase transient do not enter the window. It measures
the fundamental and the second to sixth harmonics at the exact harmonic
frequencies with a Hann-windowed DFT, at feedback 0, 20, 25, 35, 50, 75 and 100
percent, positive and negative.

The numbers are the second-to-first, third-to-second and fifth-to-second ratios,
plus the peak absolute sample. At 0 percent the positive and negative renders must
be bit-identical. The pass condition is a 25 percent relative rise in the
third-to-second and fifth-to-second ratios at 25, 50 and 75 percent, with no peak
growth beyond 10 percent at 100 percent. A failure at any of the three amounts stops
the feature.

The test ran once as the experiment, not as a permanent suite entry. The parameter
and the test were removed after the stop. The evidence artefact keeps the measured
table and the stop decision for line 2. `engine_parity` stays in the suite and
confirms the positive path against the captured reference.

## Decision

The sign flip failed at 25 and 50 percent feedback. It is not shipped, and the
160-to-164 migration was not used. The measured table is in
[`../plugin/evidence/feedback-polarity.md`](../plugin/evidence/feedback-polarity.md).
The shared feedback envelope with a per-operator depth is implemented on the engine
side and measured in
[`../plugin/evidence/feedback-envelope.md`](../plugin/evidence/feedback-envelope.md).
The page, the mockup and the snapshots are in. The Reface-style nonlinear shaper and
the one-pole filter are shipped too, with the evidence in
[`../plugin/evidence/feedback-shaper.md`](../plugin/evidence/feedback-shaper.md).
The negative-feedback line now has the shape and filter controls. A different tap
topology or a listening comparison with the Reface remains open, and it needs its
own contract.
