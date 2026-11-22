# Spec: per-operator stochastic phaseshaping

Status: implemented at 1×, 2026-11-22. The map, the 232 parameters, the editor
page, the preset migration and the engine tests are in the tree. The 2× read, the
half-band decimator and the 16-sample alignment delay are not built. The measured
1× cost is 1.76× the phase-off baseline, and the join aliasing is unmeasured; both
are recorded in
[`plugin/feature-contract-phase-map.md`](plugin/feature-contract-phase-map.md).
The rest of this file stays the design record, including the 2× section the build
did not reach.

Research: [`docs/research/stochastic-phaseshaping.md`](docs/research/stochastic-phaseshaping.md)
and the browser lab [`docs/research/phase-transfer-lab.html`](docs/research/phase-transfer-lab.html).
The lab is a listening experiment, not a measurement. Its zero-crossing rule, its
slope bounds and its segment map are the starting point; its five charts are the
display reference; its clamp is the behaviour this spec replaces.

The research note leaves four things open: aliasing, defaults, cost, evidence. This
spec closes defaults and cost with measurements. It does not close aliasing: the
2× path was the intended answer, and the measured 1× cost made it unviable, so the
join aliasing stays a recorded limit. The note's "sharing question" (phase
distortion and phase modulation through one map) is answered by where the map
sits: it warps the operator's own accumulator before the FM offset is added, so
the modulator is not warped.

The voice of this file is the repo's: first person, reasons before rules, and the
numbers that matter. Everything in the tables is a decision, not a suggestion.

## 1. What the player gets

A new tab, `op phasemap`, fourth in the strip after `operator lfos`. It shows the
current part's four operators as four columns, each with its own composed transfer
plot and its own controls. The tab edits the active part, like the other operator
pages.

Each operator gains one switch and one geometry. `phase on` turns the map on for
that operator. The map is a phase-distortion breakpoint followed by a segmented
phase map whose slopes drift, whose segments can flip, and whose order can change.
A dedicated LFO and a dedicated signed envelope move the breakpoint, and the two
follow cards gain a phase-breakpoint target so key and velocity can move it too.

The sound is the lab's, moved into the instrument: a sweepable phase-distortion
character at low settings, hard knees and odd harmonics when segments flip, and
inharmonic joins when the order jumps. Off, the operator is the current operator.
On, the map is the oscillator's geometry and the FM offset is added after it.

The tab's map display is live while a voice sounds and freezes on the last map
when the part falls silent. It is one picture per operator, not a second scope.

## 2. Signal path and placement

The current read is:

```text
acc += step
read = table(acc + (fm << 20))
```

The mapped read is:

```text
acc += step
x = acc >> 22                  // 10-bit input to the map
q = F(x)                       // pre-map, breakpoint xb
y = G(q)                       // segmented map, order and flips
read = table((y << 20) + (fm << 20))
```

`F` and `G` are evaluated per sample, per operator, after the accumulator and
before the FM offset. The FM offset therefore lands after the warp, exactly as the
interview chose: the map reshapes the operator's own phase, not the modulator's
deviation. Feedback is a separate offset in the same sum and also lands after the
warp.

The map applies to every wave, including `spectrum` (wave 13) and its morph read.
There is one read path; the additive bank is read at the mapped phase like the
wavetable bank.

The current read stays in the off path: with `phase on` off, the operator uses the
1× accumulator-add-read path and the render is bit-exact against the current
`engine_parity` reference. This is a hard check, not an aspiration.

### 2.1 The 2× option and the shipped 1× path

**This section is not built.** The shipped map runs at the core 48 kHz, and the
measured 1× cost (1.76×) is above the 1.6× budget set for the 2× design. The text
below stays as the design record for a future attempt, not as a description of the
current engine.

When `phase on` is set, the operator's map and table read would run at 2× and a
decimator would return the output to 48 kHz. The phase accumulator stays at 48 kHz
and supplies two read phases per sample: `acc` and `acc + step/2`. The FM and
feedback offsets would be held for the pair (zero-order hold); they are
control/audio sums from the 48 kHz frame, and holding them is the cheap choice
that does not change the existing modulation timing.

The decimator is a 65-tap half-band FIR, generated into `engine/gen/math_tables.h`
at build time. At the 96 kHz internal rate its passband reaches 20.5 kHz and its
stopband, at least 70 dB down, starts at 27.5 kHz; the 24–27.5 kHz transition
folds into 20.5–24 kHz, above the audible band. The filter's delay is 32 samples
at 96 kHz, exactly 16 samples at 48 kHz, so the compensation is an integer delay
rather than a fractional one. In polyphase form only the 17 non-zero taps per
output phase are multiplied, which is the cost the CPU budget measures. The 2×
path is skipped entirely when the switch is off, so an off operator costs exactly
what it costs today.

The decimator has a group delay. That delay is fine for a single operator, but it
would move a mapped carrier relative to an unmapped modulator inside one voice.
The build compensates: when a voice contains at least one mapped operator, the
non-mapped operators' routing taps get a 16-sample delay so the voice's internal
alignment is unchanged. The voice as a whole is then delayed by the filter, 16
samples at 48 kHz (0.33 ms); that uniform delay is inaudible, and the contract
records it. If the measurement shows the compensation
is unnecessary or costs too much, it is dropped and the delay is recorded as a
limit instead. Shipping with an uncompensated per-operator delay is not allowed.

The 2× decision is what the interview chose as "only while active": the switch is
the activity, not an automatic neutral-value test. On is 2× even when the map is
still the identity; off is the current path.

## 3. Engine design

### 3.1 One map per part, one modulation pair per part

The stochastic state, the LFO and the envelope live per part and per operator:
four operators × two parts = eight instances. Voices do not own phase-map state.
Every voice on a part reads the same map, which is what makes chords coherent and
what the display can show exactly.

The consequence, recorded under Limits: a new note can retrigger the part's phase
envelope and so reshape notes that are already sounding. That is the price of a
shared map, and it is the behaviour the interview chose.

The breakpoint, the LFO, the envelope and the follow amounts are also per part.
There is no per-voice phase offset and no per-voice map.

### 3.2 The 4096-point phase grid

Inside the map the phase is a 12-bit value, 0 to 4095, one point per 1/4096 of a
cycle. The grid is the lab's `N = 4096`, and it keeps every map multiply inside a
32-bit product:

- `q` and `y` are 0..4095.
- a segment's `x` boundaries are 0..4096.
- a slope is Q16 from 0.5 (32768) to 2.66 (174327).
- `(y - x) * slope` is at most `4096 * 174327`, which is below `2^31`.

The table read converts `y` back to Q32 with `y << 20`, then the existing table
index shift `>> 22` reads the 10-bit table. The grid is finer than the table, so
the map does not add table quantisation beyond what the lab had.

### 3.3 The pre-map `F`

`F` is the lab's one-breakpoint polyline, with the breakpoint `xb` in per-mille
from the `breakpoint` knob:

```text
xb = breakpoint / 1000                  // 0.001 .. 0.999
F(x) = 0.5 * x / xb                     for x <= xb
F(x) = 0.5 + 0.5 * (x - xb) / (1 - xb)  for x > xb
```

`F` is continuous at `xb` and never clicks. It is evaluated through a 1024-entry
LUT per operator, rebuilt when the folded `xb` changes on a control tick. The LUT
is 10-bit and its output is scaled by four into the 12-bit `G` input. A 10-bit
`F` is enough because `F` is a straight line on each side and the table itself is
1024 points.

The modulation is summed in per-mille before the LUT is rebuilt:

```text
xb_milli = fold(base_bp + env_milli + lfo_milli + follow_milli, 1, 999)
```

One percent is ten per-mille. A 100% envelope or LFO therefore moves the
breakpoint across the whole field, which is the lab's "full normalized range".

### 3.4 The anti-stick fold

The lab clamps `xb` to `[0.001, 0.999]`, so an LFO that pushes against an edge sits
there for the rest of its half-cycle. That is the wart this feature removes.

The fold reflects the excess instead:

```text
span = 999 - 1
v = xb_milli - 1
v = v mod (2 * span)          // positive modulo
if (v > span) v = 2 * span - v
xb_milli = v + 1
```

At `xb = 1000` the fold returns `998`; at `1100` it returns `898`. The position
turns around at the edge, so the geometry keeps moving while the LFO phase keeps
running. The fold is applied once, after the base, envelope, LFO and follow are
summed; the sources themselves are never clamped, and their phases are never
reset by a bound. The display plots the folded position, and the phase dot follows
it, so the screen agrees with the sound.

The fold is a triangle reflection, not a wrap: position is continuous at the
bound, only the velocity changes sign. That is the interview's "reflect only the
excess".

### 3.5 The segmented map `G`

`G` divides the table phase `y` into `M` equal spans of `1/M`, where `M` is the
`segments` knob from 1 to 32. The segment's `y` boundaries are therefore
`i / M`, fixed. The input span of segment `k` is `[x_k, x_{k+1}]` in the 12-bit
map grid; the `x` boundaries are the drifting part and start evenly spaced with
every slope at 1.

Each segment is linear:

```text
slope_k = (y_{k+1} - y_k) / (x_{k+1} - x_k)
y = y_k + ((q - x_k) * slope_k) >> 16
```

The slope is held between 0.5 and 2.66, the lab's bounds. A flip reverses the
segment's `y` span, so the segment is walked from its right boundary back to its
left and the slope is negative. The composed map is `T(x) = G(F(x))`.

`G` is evaluated per sample with:

- a 4096-entry `segment index` LUT built from the `x` boundaries, so finding the
  segment is one load, not a search;
- a 32-bit multiply for the interior, with the bounds above;
- a 32-bit add to the fixed `y_k`.

The LUT is rebuilt when the segment count, the drift or the order changes, not per
sample.

### 3.6 Zero-crossing locks, the full lab rule

The map must not flip or reorder through a zero crossing or a silent flat. This is
the lab's protection, kept in full:

1. Build a 4096-bit mask of the table phase for the current wave. A point is
   protected when it is inside a zero crossing, a touch at zero, or a silent run.
   A run of zero samples collapses to one protected span; two adjacent samples of
   opposite sign count as a crossing.
2. Snap the `y` boundaries away from protected points, at least two grid points
   from a protected edge. Keep the boundaries sorted and inside `[0, 4096]`.
3. Collapse a protected silent run into one segment. If the requested `M` cannot
   fit around the protected spans, the active count is lower than the knob; the
   display shows the active count.
4. Lock every segment whose `y` span touches a protected point. A locked segment
   never flips and never takes part in a reorder swap. The lock is the lab's
   half-peak rule: the sloped sides of a crossing are protected out to the point
   where the waveform reaches half its peak, so a boundary cannot sit on the
   steep shoulder of a crossing.

The fixed masks are precomputed at start-up from the 13 base waves' mip-0 tables.
The spectral case is in §3.10.

### 3.7 Drift

Drift moves the `x` boundaries, not the `y` spans. Each drift event adds a
Gaussian step to every segment's slope, scaled by the `drift` amount, reflects the
slope at 0.5 and 2.66, recomputes that segment's right-hand `x` from its fixed `y`
span, and renormalises the last `x` to 4096. The `y` boundaries stay at `i / M`,
so the map always spans 0 to 1.

The step is a reflected Gaussian random walk. The PRNG is a per-instance
xorshift32 seeded from `0x9E3779B9 + part * 4 + op`; it is reset only by the
engine reset, not by a note, so a fresh engine is deterministic for tests and a
playing engine keeps its texture.

One Gaussian is drawn per segment per event as the sum of twelve uniform 16-bit
draws minus six, which has unit variance and a ±6 bound. The step's standard
deviation in slope units is `0.40 * drift / 100`, so `drift = 100` steps with
σ = 0.40. At `drift rate = 1.2 Hz` six events take five seconds and put the
walk's spread at 0.98 slope units, while the lower bound is only 0.5 away; the
reflection at 0.5 and 2.66 turns the walk back, so the map keeps moving at the
edges instead of settling on them. At `drift = 0`, σ = 0 and the slopes stay at 1.

There is no pull toward 1. A pull would make the map settle toward the neutral
geometry, which is the one sound `drift = 0` already gives; the reflected walk is
stationary inside the bounded interval and keeps moving without one. The OU
process in the research note's next-experiment list stays an option if listening
asks for a more settled texture, but it is not in v1 and adds no parameter.

At `drift = 0` the slopes stay at 1 and the map is static.

### 3.8 Flips

At each flip event, every eligible segment rerolls its orientation against
`flip` probability. A segment is eligible when it is not locked. A flip reverses
the segment's `y` span and its slope; a second flip restores it. Flips are
computed on the 30 Hz stochastic tick, not per audio sample.

The flip event rate is the `flip rate` control, 0 to 30 Hz. At 0 no event fires;
at 30 Hz the texture moves at the control limit. A probability of 0 leaves every
segment forward; a probability of 100 flips every eligible segment on every event.

### 3.9 Reorder

Reorder changes the traversal order without changing which `y` spans exist. The
order starts natural.At each reorder event, with `reorder` probability, one swap is attempted:

1. Draw `i` uniformly from the active positions and `d` uniformly from
   `1..min(jump, active - 1)`.
2. `j = (i + d) mod active`, so the swap is forward and wraps at the cycle edge.
3. If either position is locked, redraw both up to three times; if all three
   draws touch a lock, the event makes no change.
4. Swap `order[i]` and `order[j]`.

`jump = 1` is the interview's adjacent swap, including the last-to-first wrap;
larger values widen the distance without returning to a full permutation. The
knob is kept because it is the only control over how local the reorder is. The
lab's full permutation was rejected because it changes every join at once (noise
at high rates) and cannot preserve the locks.

The event rate is `reorder rate`, 0 to 30 Hz. A swap makes the map discontinuous
at the join between the two positions: `G(q)` jumps because input segment `k` now
uses the `y` span of `order[k]`. The jumps are the inharmonicity, as in the lab,
and the display draws them as pink dashed verticals.

The composed map reads `order[k]` for the `y` span but keeps the input span
`[x_k, x_{k+1}]` natural, exactly the lab's "new order, same segments" rule.

### 3.10 Spectral locks

Wave 13's waveform is the additive bank and changes with the morph. The lab's
mask cannot be precomputed once for it. The interview's answer is to check
spectrum a and spectrum b and interpolate, just as the morph interpolates:

1. When the additive bank is regenerated, build a 4096-bit protected mask for
   spectrum a and one for spectrum b from the regenerated waveforms, using the
   same zero/silent/half-peak rule as the fixed tables.
2. Combine them at the map rebuild with the part's base `morph` value `m` in
   Q8: `lock(q) = (a_mask(q) * (255 - m) + b_mask(q) * m) >= 128`. A silent flat
   also needs both sides silent; a point silent in only one side is not collapsed.
3. Rebuild the combined mask when a/b levels, either range, or the base morph
   change, at most once per control tick. The mask is cached with the map.

The morph used here is the base `spectrum` knob, not a voice's live morph
modulation. The map is per part, so it cannot follow a per-voice morph; this is a
documented limit, and the display shows the same base morph the map used.

The 13 fixed waves use their precomputed masks and pay nothing per block.

### 3.11 The dedicated LFO

The phase LFO is the existing LFO shape, one instance per operator per part, with
the full card: rate, signed depth, wave, bipolar, rate fade and depth fade.

- Rate is the existing exponential `hz` range 0..20,000,000 raw, display `hz`;
  default 44,739 raw, which reads 0.5 Hz.
- Depth is -100..100 percent. In bipolar mode the output is `-depth..+depth`; in
  unipolar mode it is `0..depth`, matching the other LFO cards.
- Wave is 0..5: sine, triangle, saw, inverse saw, square, random, with the existing
  `lfoWaveNames` list. The new display type `lfo_wave` is needed because the
  existing `wave` display names the 14 oscillator waves.
- `rate fade` and `depth fade` are 0..99 with the existing `fade` law and shared
  graph.
- The LFO free-runs per part. It is not reset by a note.

The LFO is evaluated at the control tick (1,500 Hz at 48 kHz, `CONTROL_BLOCK 32`),
not per sample, because it only moves the breakpoint. The fade levels use the same
Q31 step law as every other LFO.

### 3.12 The dedicated signed envelope

The envelope is the feedback envelope's shape, reused:

- `on` switch;
- three rates `r1..r3`, 0..99, defaults 80, 60, 50;
- three signed levels `l1..l3`, -100..100, defaults 0;
- release rate `rel` 0..99, default 80, release level `rl` -100..100, default 0;
- mode 0..3, default 1, with the signed `mod_env` semantics.

The levels are absolute per-mille of the breakpoint field: `level 100` adds
+1,000 per-mille, `level -50` subtracts 500. There is no separate env-depth knob,
per the interview.

Triggering follows the part's voice mode, per the interview:

| Part mode | Envelope trigger |
| --- | --- |
| poly | every note-on on the part |
| unison | every note-on on the part |
| mono | the first note of a phrase, when the part had no sounding voice |
| legato | the first note of a phrase, when the part had no sounding voice |

Unison follows poly because the amplifier envelope retriggers on every note in
unison; the phase envelope stays aligned with it. Mono and legato keep the phrase
rule so a legato slide does not restart the map.

Release starts when the part's last voice releases. The envelope is per part and
runs at the control tick. A note that retriggers in poly mode reshapes held notes
too; that is the shared-map limit.

### 3.13 Key and velocity follow

The phase breakpoint joins the follow target set. Four targets are added, one per
operator, each with the existing three amounts:

- `opN_phase_ks_lo`, `opN_phase_ks_hi`, `opN_phase_vs`, targets
  `opN.phase_ks.lo`, `opN.phase_ks.hi`, `opN.phase_vs.amount`, -100..100,
  default 0.

`followTargets()` grows from 15 to 19 entries: op1 level, op1 feedback, op1 amp
env, op1 phase, op2 level, ..., pitch env, filter 1 cutoff, filter env. The follow
contribution is scaled by the key or velocity factor, converted to per-mille, and
added to the breakpoint sum before the fold. The wipe buttons clear the new
amounts along with the rest.

The follow cards' `wipe all` and target selector therefore cover phaseshaping
without a new card.

### 3.14 The stochastic clock

The engine control tick is 1,500 Hz (48 kHz / 32). The lab's stochastic controls
run at 30 Hz, so each source has a 32-bit accumulator advanced by
`rate_raw * CONTROL_BLOCK` per control tick; the top bit (or the wrap) is the
event. This gives 0..30 Hz from the existing `hz` scale, exact enough for the
rates that matter, and it keeps the audio path free of float.

Drift, flip and reorder each have their own accumulator. The map's `x` LUT and the
`segment index` LUT are rebuilt only after an event, not on every control tick;
the breakpoint's `F` LUT is rebuilt when the folded `xb` changes, which can be
every control tick under modulation.

### 3.15 State and reset

Per operator per part:

```c
struct phase_map_state {
    uint32_t seed;             // xorshift32
    uint16_t bp_milli;         // folded, 1..999
    uint16_t segments;         // requested 1..32
    uint16_t active;           // after locks and silent collapse
    uint16_t x[33];            // 12-bit input boundaries
    int16_t  slope_q16[32];    // 0.5..2.66
    uint8_t  flip[32];
    uint8_t  locked[32];
    uint8_t  order[32];
    uint8_t  seg_lut[4096];    // q -> input segment index
    uint16_t f_lut[1024];      // 10-bit F output
    uint32_t lfo_phase;
    uint32_t lfo_rate_level, lfo_depth_level;
    uint32_t lfo_rate_step, lfo_depth_step;
    int32_t  env_level;
    uint8_t  env_stage;
    uint32_t drift_acc, flip_acc, reorder_acc;
    uint32_t drift_phase, flip_phase, reorder_phase;
};
```

Eight instances is about 40 KB, dominated by the two LUTs. Everything is read on
the audio thread; nothing allocates, locks or touches a file. The state is reset
by the engine reset and by a part init, not by note events.

### 3.16 Display tap

The audio thread publishes a phase-map snapshot per operator per part at 30 Hz,
through a double buffer with an atomic index, the same pattern the scope and
morph displays use. The snapshot is:

```c
struct PhaseMapSnapshot {
    uint16_t xb_milli;
    uint16_t active;
    uint16_t segments;
    uint16_t phase_q, phase_y;   // newest voice's mapped dot, 12-bit
    uint16_t x[33];
    int16_t  slope_q16[32];
    uint8_t  flip[32];
    uint8_t  locked[32];
    uint8_t  order[32];
    uint8_t  voices;             // 0 freezes the display
};
```

The UI timer reads the latest snapshot at 30 Hz, reconstructs `T` from `x`,
`slope`, `flip`, `order` and draws it. It does not read engine internals. The
snapshot is written whether or not a voice sounds; `voices == 0` tells the UI to
freeze the last drawn map and stop advancing the phase dot.

No allocation, lock or file access on the audio thread.

### 3.17 CPU budget

The 1× path is the cost that shipped. The build measures:

- baseline: 24 voices, four operators, phase off, 48 kHz;
- worst case: 24 voices, four operators, all `phase on`, 32 segments, 30 Hz drift,
  100% flip and reorder, both modulation sources on, wave 13.

The measurement is a render of 1,000 blocks with the wall clock divided by the
baseline. The budget of 1.6× was set for the 2× design. The 1× path measures
1.76×, so the 2× design is not viable and is not built. The measured ratio is in
the feature contract.

The per-sample map cost is one `F` LUT load, one segment LUT load, one 32-bit
multiply and one add, plus the table read. There is no half-band and no second
read at 1×. The LUT rebuilds and the stochastic events are control-rate and
negligible beside that.

## 4. Parameters

Twenty-six per operator plus three follow amounts, so 29 × 4 = 116 per part and
232 overall. All are block-rate and host-automatable; none joins the default CC
map. The IDs follow the manifest's `<prefix>.<field>.pN` style with `.p0` and
`.p1`.

| # | Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- | --- |
| 1 | `opN_phase_on` | `opN.phase.on` | 0..1 | 0 | `onoff` |
| 2 | `opN_phase_bp` | `opN.phase.bp` | 1..999 | 500 | `frac3` |
| 3 | `opN_phase_segments` | `opN.phase.segments` | 1..32 | 8 | `raw` |
| 4 | `opN_phase_flip` | `opN.phase.flip` | 0..100 | 0 | `pct` |
| 5 | `opN_phase_flip_rate` | `opN.phase.flip_rate` | 0..2,684,355 | 178,956 | `hz` |
| 6 | `opN_phase_reorder` | `opN.phase.reorder` | 0..100 | 0 | `pct` |
| 7 | `opN_phase_reorder_rate` | `opN.phase.reorder_rate` | 0..2,684,355 | 0 | `hz` |
| 8 | `opN_phase_jump` | `opN.phase.jump` | 1..32 | 1 | `raw` |
| 9 | `opN_phase_drift` | `opN.phase.drift` | 0..100 | 0 | `pct` |
| 10 | `opN_phase_drift_rate` | `opN.phase.drift_rate` | 0..2,684,355 | 107,374 | `hz` |
| 11 | `opN_phase_lfo_rate` | `opN.phase.lfo.rate` | 0..20,000,000 | 44,739 | `hz` |
| 12 | `opN_phase_lfo_depth` | `opN.phase.lfo.depth` | -100..100 | 0 | `pct_bip` |
| 13 | `opN_phase_lfo_wave` | `opN.phase.lfo.wave` | 0..5 | 0 | `lfo_wave` |
| 14 | `opN_phase_lfo_mode` | `opN.phase.lfo.mode` | 0..1 | 1 | `onoff` |
| 15 | `opN_phase_lfo_rate_fade` | `opN.phase.lfo.rate_fade` | 0..99 | 0 | `fade` |
| 16 | `opN_phase_lfo_depth_fade` | `opN.phase.lfo.depth_fade` | 0..99 | 0 | `fade` |
| 17 | `opN_phase_env_on` | `opN.phase.env.on` | 0..1 | 1 | `onoff` |
| 18 | `opN_phase_env_r1` | `opN.phase.env.r1` | 0..99 | 80 | `raw` |
| 19 | `opN_phase_env_r2` | `opN.phase.env.r2` | 0..99 | 60 | `raw` |
| 20 | `opN_phase_env_r3` | `opN.phase.env.r3` | 0..99 | 50 | `raw` |
| 21 | `opN_phase_env_l1` | `opN.phase.env.l1` | -100..100 | 0 | `pct_bip` |
| 22 | `opN_phase_env_l2` | `opN.phase.env.l2` | -100..100 | 0 | `pct_bip` |
| 23 | `opN_phase_env_l3` | `opN.phase.env.l3` | -100..100 | 0 | `pct_bip` |
| 24 | `opN_phase_env_relr` | `opN.phase.env.relr` | 0..99 | 80 | `raw` |
| 25 | `opN_phase_env_rell` | `opN.phase.env.rell` | -100..100 | 0 | `pct_bip` |
| 26 | `opN_phase_env_mode` | `opN.phase.env.mode` | 0..3 | 1 | `raw` |
| 27 | `opN_phase_ks_lo` | `opN.phase_ks.lo` | -100..100 | 0 | `pct_bip` |
| 28 | `opN_phase_ks_hi` | `opN.phase_ks.hi` | -100..100 | 0 | `pct_bip` |
| 29 | `opN_phase_vs` | `opN.phase_vs.amount` | -100..100 | 0 | `pct_bip` |

The `hz` display is the existing one (`raw * 48000 / 2^32`). 2,684,355 is 30 Hz;
107,374 is 1.2 Hz; 178,956 is 2 Hz; 44,739 is 0.5 Hz.

`frac3` is new: `value / 1000` to three decimals, so the breakpoint knob reads
`0.500`.The map display's caption reads `bp 0.500 · π` beside the live map, using the
same value in radians (`bp * 2π`, shown as a multiple of π). `lfo_wave` is new:
it names the six entries of `lfoWaveNames`, not the 14 oscillator waves.

The segment count is the requested count. The active count can be lower when the
locks cannot fit the requested boundaries; the display shows `n/m seg` and the
contract records the rule.

## 5. Persistence

The manifest moves from 1,744 to 1,976 parameters and the part block from 865 to
981 (104 phase values plus 12 follow values per part). The two parts account for
1,962; the 14 global parameters bring the total to 1,976.

- `PRESET_VERSION` 15 → 16.
- `kStateVersion` 13 → 14.
- `kUiPageLayout` 3 → 4.
- Each part record grows by 116 values at the end, grouped per operator:
  op1's 26 phase parameters, op1's 3 follow amounts, op2's 29, and so on. Nothing
  existing moves, so the migration is an append rather than an index map.
- An older record loads with `phase on` off and the neutral defaults above, so
  the render is the current render.
- `preset_smoke` loads version 13, 14 and 15 records and checks the appended
  counts and the defaults, including `phase_on = 0` and `env_on = 1`.
- The plugin state migration sets the new parameters to their defaults; the
  `uiPage` remap is in §6.1.

The CC map is regenerated: `param_to_cc` grows to 1,976 entries and every new
parameter maps to 255. `CC_PARAMS` stays 55.

## 6. Editor

### 6.1 The tab and the page

The tab strip becomes eight labels:

0. `patch settings`
1. `part fx/mod`
2. `operator envs`
3. `operator lfos`
4. `op phasemap` (new)
5. `op spectra a`
6. `op spectra b`
7. `op spectra mod`

`op phasemap` is the name. It is short enough for the 1,238 px strip and it names
the thing, not the technique. The three spectra pages shift right by one.

The stored `uiPage` migration for layout 3 and older: pages 0..3 stay, 4→5, 5→6,
6→7. `kUiPageLayout = 4`. `PluginEditor::buildUi`, `buildPage`, `showTab` and
`PluginProcessor::getUiPage`/`setUiPage` all change their `jlimit(0, 6)` to 7.

The page is one row of four 294 px columns, the same rhythm as `operator lfos`.
Each column, top to bottom:

| Block | Height | Contents |
| --- | --- | --- |
| operator header | 26 | `opN`, copy, paste, mute |
| composed map | 99 | the plot, the caption line |
| `phase` card | 159 | `on` in the header, nine knobs in three rows |
| `phase lfo` card | 160 | the compact LFO row with fades |
| `phase env` card | 176 | graph, three rate/level rows, mode, `on` |

The cards are separated by 6 px (the 3 px column gap plus the 3 px card
margin), and the column padding is 8 px, so the column measures 660 px inside
the 669 px budget, the page's usable height at 1,240 × 985. The heights are
from the mockup's measured blocks, not estimates; if the JUCE pass finds a clip,
the map loses pixels before the value text does.

The `phase` card's nine knobs are, row by row:

1. `breakpoint`, `segments`, `flip`;
2. `flip rate`, `reorder`, `reorder rate`;
3. `jump`, `drift`, `drift rate`.

The `on` switch is in the card header. The `phase lfo` card is the existing
compact fade row: 16 px dials, rate/depth/wave/bipolar, `rate fade` and
`depth fade`, the two-line graph. The `phase env` card is the signed envelope:
20 px graph in purple, `r1..r3`, `l1..l3`, `rel`/`rl`/`mode` in compact rows, and
`on` in the header. The graph uses purple because it is a modulation
envelope; the LFO graph is green, matching the filter LFO.

The phase display is a `P1PhaseMap` component (new), borderless on black, with a
faint `line` grid and no drag. It draws:

- `T(x)` as a polyline sampled at 256 points, with each input segment's stroke in
  green when forward and purple when flipped;
- locked segments at 35% opacity, so the locks read as protected;
- a pink dashed vertical at each order jump, where `order[k] != k + 1`;
- the fixed `y` boundaries as faint horizontal rules;
- `F` as a faint `lineStrong` polyline, so the pre-map is visible but secondary;
- the breakpoint as a pink vertical line at `xb`, and the phase dot as a white
  2 px dot at `(phase_q, phase_y)`;
- the caption `bp 0.500 · π` and `n/m seg`, `k locked` at the bottom, in `dim`.

The colours are the interview's: green forward, purple flip, faint locked, pink
jumps and breakpoint, white phase dot. No fourth hue is added.

### 6.2 Live behaviour and the tap

The display follows the shared per-part map, so what it draws is what every voice
plays. It advances on the 30 Hz UI timer from the snapshot. When `voices == 0`
the last map stays on screen and the phase dot stops; the next note resumes it.
Parameter edits rebuild the static geometry immediately, before the next audio
tick, so a knob drag does not wait for the engine.

The display does not take a drag and does not show per-voice differences, because
there are none.

### 6.3 Copy and paste

- `OperatorSection::phase` copies the 26 phase parameters.
- `OperatorSection::phaseLfo` copies the six LFO parameters.
- `OperatorSection::phaseEnvelope` copies the ten envelope parameters.
- `copyOperator`/`pasteOperator` include all three, so an operator moves whole.
- The `phase`, `phase lfo` and `phase env` cards carry `copy` and `paste` in
  their headers, like the envelope and LFO cards.
- The section lists in `PluginEditor.cpp` gain the three cases; the snapshot test
  writes a value into each source and checks every destination, including
  `phase_on`, `phase_bp`, `phase_lfo_depth` and `phase_env_l1`.

### 6.4 Snapshot goldens

Two new goldens, regenerated deliberately with the Linux runner:

- `editor-op-phasemap.png`: the tab with all four operators off and the neutral
  geometry, so the empty state is covered.
- `editor-op-phasemap-live.png`: op1 on with 8 segments, a drift and a held note,
  so the composed plot, the locks and the phase dot are visible. The eight
  snapshot blocks do not reach the first flip or reorder event at the demo's 2 Hz
  and 1.5 Hz rates, so the still shows the forward segments only; the flip and
  reorder layers are covered by the engine test.

`manifest.json` is regenerated. The two phase pages are new. The pages that show
the scope trace moved with the audio change and were re-accepted in the same pass.

## 7. Verification

### 7.1 The engine test

New `plugin/tests/phase_map.cpp`, registered as `phase_map`:

- **Off path:** render one note with `phase on` off and compare against a stored
  reference; it is bit-exact. `engine_parity` stays bit-exact too.
- **Pre-map:** at breakpoint 250 and 750, check `F` at 0, 0.25, 0.5 and 1 against
  the polyline, including the half-gain at the breakpoint.
- **Fold:** push the breakpoint with a triangle at 2,000,000 raw and 100% depth
  over 15,000 control ticks; the folded position reaches 2..998, never stays at an
  edge for more than one tick, and reflects at the bound.
- **Segments and locks:** on a sine, the active count is at most the requested
  count; the protected spans are not cut by a boundary; no locked segment flips
  across 20 flip events at 100% probability; the segment spans still cover 0..1.
- **Flips:** at 100% probability every eligible segment flips and its slope is
  negative; flipping twice restores it.
- **Reorder:** the order stays a permutation of the active segments; every segment
  is visited once; `jump = 1` only swaps neighbours; locked segments never move.
- **Drift:** 120 events keep every slope in [0.5, 2.66] and every `x` sorted; the
  last `x` is 4096.
- **Modulation:** a positive envelope level moves the breakpoint right and a
  negative one left; the LFO depth sign inverts the sweep in bipolar mode; a fade
  reaches full on the computed tick.
- **Trigger:** poly retriggers on every note; mono and legato trigger only on the
  first note after silence; release starts with the last note-off.
- **Parts:** part 0 and part 1 keep separate maps, breakpoints and seeds.
- **Spectrum:** wave 13's lock mask follows a and b and the base morph; a morph
  between two spectra locks the union of the two protected spans at the midpoint.
- **Render:** the 1× mapped render is finite, has no `INT32_MIN`/`INT32_MAX` and is
  not silent. There is no 2× test because the 2× path is not built.
- **CPU:** print the worst-case ratio described in §3.17.

### 7.2 Existing checks

- `engine_parity`: the phase-off reference is unchanged, and the new parameter
  defaults do not move the existing renders.
- `preset_smoke`: version 13, 14 and 15 records load with the phase block appended
  and the defaults in place; the part count is 981.
- `editor_snapshot`: the copy/paste checks, the two new goldens, and the follow
  target count changing from 15 to 19.
- `plugin_smoke`, `rate_bridge`, `lfo_fades`, `spectral_bench`: unchanged, but run
  after the manifest and tab changes.
- The full Linux CTest suite and the macOS/Windows CI matrix.

### 7.3 What would make this fail

- The off path moves by one sample.
- The fold pokes the breakpoint to the other edge (that would be a wrap, not a
  reflection) or the position freezes at an edge.
- A locked segment flips or moves.
- A follow amount changes the breakpoint in the wrong direction or by the wrong
  scale.
- The worst-case 1× render exceeds the measured 1.76× ceiling.
- The four columns clip or the map is unreadable at 270 × 86.

## 8. Docs

- New `plugin/feature-contract-phase-map.md`, in the shape of the other contracts:
  identity, player outcome, parameters, engine, editor, persistence, evidence,
  limits, risks.
- `design/mockup.html`: the `op phasemap` tab, opened by `#phasemap`, with the
  measured block heights in §6.1 and the live caption. It is the layout
  reference for the JUCE page.
- `design/README.md`: the new tab in the Tabs list, a `Phase map` section under
  Operators, the nine knobs, the map colours and the two-line caption.
- `design/juce.md`: the eighth tab, the four-column page, the heights above, the
  colour mapping and the live tap.
- `docs/research/stochastic-phaseshaping.md`: status line changed from "experiment"
  to "in product" with the date, the 1× limit that replaced the 2× answer, the
  defaults, the measured CPU and the evidence pointer. The open questions that are
  now answered move to a short "what shipped" note; the sharing question stays
  open, and the 4× idea is recorded as unbuilt.
- `README.md` research line 1 gains a shipped note and points at the contract.
- `plugin/README.md` test list gains `phase_map`; the parameter total moves to
  1,976 and the part block to 981 wherever the current totals appear.
- `plugin/CMakeLists.txt` gains the test target and the new source file.

## 9. Limits

- **Aliasing is reduced, not removed.** 2× plus the half-band tames the joins;
  the filter passes to 20.5 kHz and is at least 70 dB down from 27.5 kHz, so the
  remaining fold-back is 20.5–24 kHz. Extreme reorder at 30 Hz still has content
  near Nyquist; the contract records the measured spectrum rather than claiming
  a clean result.
- **The map is per part.** A new note can reshape held notes in poly mode; the
  envelope cannot follow each voice separately. The alternative (per-voice maps)
  was rejected for cost and phase coherence.
- **The locks can reduce the active count.** A waveform with many protected spans
  may not fit 32 requested segments; the display shows the actual count.
- **The spectral mask uses the base morph.** A voice's live morph modulation is
  not in the lock mask; the map is shared and cannot follow it.
- **The breakpoint fold is a reflection.** At the edge the geometry turns around;
  it does not continue past the edge or wrap. That is the anti-stick behaviour.
- **The reorder is local swaps.** It is not the lab's full permutation or its
  forward jump; the `jump` knob widens the swap distance, and the sound is a
  drifting order rather than a new permutation every event.
- **The 2× path has a group delay.** It is compensated inside a voice; the voice
  as a whole is delayed by the filter, 16 samples at 48 kHz (0.33 ms).
- **The map is not a new oscillator.** The 13 waves and the additive bank are
  unchanged; only the phase walking through them moves.

## 10. Risks and fallbacks

- **CPU.** The 2× path plus the filter is the risk. Measure first; if the worst
  case exceeds 1.6×, fall back to the 1× map with a short join ramp and record the
  aliasing limit. Do not ship an unmeasured 2× path.
- **Timing.** If the decimator delay cannot be compensated cheaply, the fallback
  is to run all four operators of a voice through 2× whenever one is mapped, so
  they share the delay; the off operators keep their exact values but pay the
  filter. This is the second choice, not the first, because it spends CPU on
  operators the player switched off.
- **Locks.** If the full lab rule cannot fit the control budget for wave 13,
  compute the a/b masks on the base spectra and interpolate; do not drop the
  locks silently. If the mask cost is still too high, cache it and rebuild only on
  a spectrum edit, not on every control tick.
- **Clicks.** Reorder jumps are intentional. If they read as clicks rather than
  timbre at 30 Hz, the stop criterion is a short crossfade at the join; the
  contract records whether it was added.
- **Display.** If the 30 Hz snapshot reads as a stutter, raise the UI timer or
  interpolate the dot; do not move the map computation to the UI thread.
- **Rollback.** Remove the tab, the 232 parameters and the 2× branch; the preset
  version stays at 16 only if the record already shipped, so a pre-release
  rollback can return to 15.

## 11. Resolved questions

The interview left six questions. They are decided here; the build does not wait
on listening for any of them.

1. **Drift variance.** The step's standard deviation is `0.40 * drift / 100` slope
   units, drawn as the sum of twelve uniform 16-bit values minus six. At
   `drift = 100` and `drift rate = 1.2 Hz` the walk's spread passes one slope unit
   in about six events (five seconds), and the reflection at 0.5 and 2.66 turns it
   back. There is no pull toward 1: `drift = 0` already gives the neutral map, so a
   pull would only make the moving map settle where the static one already sits.
   The OU process stays a later option, not a hidden v1 behaviour.
2. **Half-band length.** 65 taps. Its 32-sample delay at 96 kHz is exactly 16
   samples at 48 kHz, so compensating the non-mapped operators is an integer delay
   line, not a fractional one. The passband reaches 20.5 kHz, the stopband is at
   least 70 dB down from 27.5 kHz, and the remaining fold-back is 20.5–24 kHz. 7,
   15 and 31 taps were rejected because their transition bands reach too far into
   the audible range, and their half-sample delays would need interpolation.
3. **Reorder `jump`.** It is the maximum forward swap distance in active positions,
   drawn uniformly from `1..min(jump, active - 1)`, with a wrap at the cycle edge.
   `jump = 1` is the adjacent swap; larger values widen the reorder. The knob stays
   because it is the only control over how local the order change is. The lab's
   full permutation is not used: it changes every join at once and cannot preserve
   the locks.
4. **Unison.** Every note retriggers, like poly. The amplifier envelope already
   retriggers per note in unison, and the phase envelope stays aligned with it.
   Mono and legato keep the first-note-of-phrase rule. The table in §3.12 is the
   implementation.
5. **`F` in the display.** It stays, as the faint `lineStrong` polyline and the
   pink breakpoint vertical. The plot's job is to show the composed map *and* where
   the pre-map bent it; dropping `F` would hide the one control that is visible on
   its own. At 270 × 86 the two-segment line costs nothing to draw and does not
   collide with the caption.
6. **Factory preset.** None in v1. The init state is the neutral, fully off map, and
   the feature has a tab with a live display plus two snapshot goldens; a factory
   slot would need a bank allocation and a name for a patch that is only a demo.
   The contract carries the exact demo settings instead, so a reviewer can dial
   them in without shipping a preset.

The research note's sharing question is answered by the placement in §2. The 2×
and 4× options were not built; a future oversampling attempt would need a fresh
CPU measurement, because the 1× path is already at 1.76×.

## 12. Implementation order

1. Parameters, manifest, CC map, counts and versions; regenerate `params.json`.
2. The engine bypass, the `F`/`G` path, the 4096 grid and the off bit-exact test.
3. Locks, drift, flips, reorder and the fold, with `phase_map` checks.
4. Per-part LFO and envelope, the trigger rules and the follow targets.
5. The spectral a/b masks and the morph interpolation.
6. ~~The 2× path, the half-band and the timing compensation; measure.~~ Not
   built. The 1× path measures 1.76×, above the 2× budget, so this step is
   deferred and the aliasing limit is recorded instead.
7. The snapshot tap and the `P1PhaseMap` component.
8. The `op phasemap` page, the cards and the copy/paste sections.
9. Goldens, copy/paste checks and the full suite.

### 12.1 Node graph changes

The engine graph lives under `.dev/modules`, so the implementation includes the
nodes and the macro, not only the generated C:

- new `.dev/modules/nodes/phase_map/` with `node.json` and `node.c`: the map's
  params, the per-part state and the per-sample `F`/`G` read. Its `on` param is the
  bypass, and its controls are the folded breakpoint sum;
- new `.dev/modules/nodes/phase_add/`: adds the FM and feedback offsets after the
  map, so the phase that reaches `osc` is `map(acc) + fm`;
- `.dev/modules/nodes/phasor/`: expose the pre-FM accumulator as an `acc` output
  beside the current `phase` output. The current `phase` stays for every existing
  wiring, and `fm_op` moves to `acc` only when the map is present;
- `.dev/modules/macros/fm_op/macro.json`: insert `phase_map` and `phase_add`
  between `phasor.acc` and `osc.phase`, add the 26 phase parameters and their
  targets, and add the phase LFO and phase envelope nodes with their wires.

The 2× read is outside the node graph's one-rate model. If the graph cannot express
it, the generated DSP carries the 2× block and the node files keep the 1×
description; that divergence is recorded in the node summary rather than hidden.
Otherwise the generated `engine/gen/type_p_alpha_dsp.c` is regenerated from the
nodes, not hand-edited.
10. Contracts and design docs.

## 13. Files

Engine and generation:

- `engine/gen/params.json`, `engine/gen/dsp.h`, `engine/gen/type_p_alpha_dsp.c`
- `engine/gen/math_tables.h` (half-band coefficients), `engine/gen/tables.c`
- `engine/gen/cc_map.h` (regenerated)
- `engine/hal/p1_math.h` if the fold or the hash needs a helper
- `plugin/tools/gen_params.py` for the manifest and IDs
- `plugin/tools/P1Synth.h.in` for the seam

Plugin and state:

- `plugin/engine/Engine.hpp`, `Engine.cpp` for the per-part state access
- `plugin/engine/MidiBridge.cpp` only if a new entry point is needed (none is
  planned; the map is parameter-driven)
- `plugin/juce/PluginProcessor.h/.cpp` for the tap, the state version and the
  page layout
- `plugin/preset/PresetStore.hpp/.cpp` for version 16; the migration chain is
  `PresetStore::migratePartBank`, there is no separate `part_migration.h`
- `plugin/ui/ParamIDs.h` for the 29 IDs and `followTargets`
- `plugin/ui/Widgets.h/.cpp` for `P1PhaseMap`, `frac3` and `lfo_wave`

Editor:

- `plugin/juce/Pages.h`
- `plugin/juce/PagesPhase.cpp` (new)
- `plugin/juce/PluginEditor.hpp/.cpp` for the tab, sections and copy lists
- `plugin/juce/PluginEditor.h` for `OperatorSection`

Tests and docs:

- `plugin/tests/phase_map.cpp` (new)
- `plugin/tests/engine_parity.cpp`, `preset_smoke.cpp`, `editor_snapshot.cpp`
- `plugin/CMakeLists.txt`, `plugin/tests/snapshots/linux-x86_64/`
- `plugin/feature-contract-phase-map.md` (new)
- `design/README.md`, `design/juce.md`, `docs/research/stochastic-phaseshaping.md`,
  `README.md`, `plugin/README.md`

## 14. Decisions from the interview

| Question | Decision |
| --- | --- |
| Map vs FM | Warp `acc` first, then add the FM offset. |
| Anti-stick | Reflect only the excess; the folded position never rests at a limit. |
| State scope | One map per part, shared by all voices. |
| Modulation sources | Dedicated phase LFO and dedicated signed envelope per operator. |
| Page layout | Four full columns with all displays and controls. |
| Map display | One composed plot: `F`, `G`, `T`, breakpoint, phase dot, order jumps. |
| Display liveness | Live control map, freeze on silence. |
| Colours | Green forward, purple flip, faint locked, pink jumps and breakpoint, white dot. |
| Zero locks | Full lab rule, including silent collapse and the half-peak protection. |
| Drift | Reflected Gaussian walk. |
| Reorder | Random swaps, `jump` sets the maximum swap distance. |
| Off state | Bit-exact bypass by the per-operator switch. |
| Envelope | Signed `mod_env` shape with absolute levels. |
| LFO | Full compact card with fades. |
| Aliasing | 2× map and table read, 65-tap half-band, measured. |
| Segments | 1 to 32. |
| Oversampling boundary | Map and table read only, not the accumulator or feedback. |
| Envelope trigger | Follows the part's voice mode. |
| Parameter order | Grouped per operator, appended at the end of the part block. |
| Spectrum | The map applies to all waves, including wave 13. |
| Spectrum locks | a and b masks interpolated by the base morph. |
| Tab | `op phasemap`, fourth, after `operator lfos`. |
| Init | Fully off. |
| Follow targets | Both key and velocity follow gain the phase breakpoint. |
