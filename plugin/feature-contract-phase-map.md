# Feature contract: per-operator stochastic phase map

## Identity

- **Feature:** Per-operator stochastic phaseshaping. Each operator gains a phase
  map that warps its own accumulator before the FM offset is added: a breakpoint
  pre-map, a segmented map whose slopes drift, whose segments can flip, and whose
  order can change, plus a dedicated LFO and signed envelope on the new
  `op phasemap` tab.
- **Brief section:** [`../README.md`](../README.md) research line 1, the
  `op phasemap` tab in [`../design/mockup.html`](../design/mockup.html), and the
  resolved design in
  [`../stochastic-phaseshaping-spec.md`](../stochastic-phaseshaping-spec.md).
- **Owner / date:** P1Onken, 2026-11-21.
- **Disposition:** contract. The engine and editor are not implemented yet. This
  file fixes the semantics, counts and acceptance evidence; the implementation
  order is §12 of the spec and no part of this contract is a completion claim.

## Player outcome

- **New player action:** open `op phasemap`, the fourth tab after `operator
  lfos`. Each of the four operators has a live composed transfer plot, a `phase`
  card with nine knobs and an `on` switch, a `phase lfo` card with fades, and a
  signed `phase env` card. `copy` and `paste` work per card and for the whole
  operator.
- **Sound and behaviour:** the map reshapes the operator's own phase, then the
  FM and feedback offsets are added. A low breakpoint gives a sweepable
  phase-distortion character; flipping segments adds hard knees and odd
  harmonics; reordering them adds inharmonic joins as the order jumps. The
  breakpoint is moved by a dedicated LFO, a dedicated signed envelope, and the
  existing key/velocity follow system. The position reflects at the edges instead
  of sticking, so an LFO at full depth keeps moving through the turn.
- **Off behaviour:** `phase on` off uses the current 1× accumulator-add-read path
  and is bit-exact against the current `engine_parity` reference. The map does
  not run and its LUTs are not read when it is off; the 2× path is skipped.
- **Failure mode:** the off path moves by one sample, the fold wraps to the other
  edge or freezes, a locked segment flips or moves, the 2× decimator shifts a
  mapped carrier against an unmapped modulator, the map leaks between operators
  or parts, or the four columns clip at the design size.

## Scope and dependencies

The feature covers four operators in each of the two parts:

| Scope | Per operator | Per part | Total |
| --- | --- | --- | --- |
| phase maps | 1 | 4 | 8 |
| phase LFOs | 1 | 4 | 8 |
| phase envelopes | 1 | 4 | 8 |
| phase parameters | 26 | 104 | 208 |
| follow amounts | 3 | 12 | 24 |
| new parameters | 29 | 116 | 232 |

- **Waves:** every wave, including wave 13 (`spectrum`) and its morph read. The
  map applies to the oscillator's own phase, not to the modulator.
- **Voice modes:** poly and unison retrigger the phase envelope on every note;
  mono and legato trigger it only on the first note of a phrase. The amp
  envelope's behaviour is the reference.
- **Dependencies:** `fm_op`, `fb_tap`, the `lfo` node, the `mod_env` node, the
  follow-target list, the additive bank's a/b spectra, and the existing
  `engine_parity` and preset migrations.
- **No MIDI or clock change:** the map is parameter-driven. `CC_PARAMS` stays 55
  and no new parameter joins the default CC map.
- **Shared map:** one map, one LFO and one envelope per operator per part, shared
  by every voice on that part. Voices do not own phase-map state. A new note can
  therefore retrigger the part's envelope and reshape notes that are already
  sounding; that is a recorded limit, not a bug.

## Parameters

Twenty-nine parameters per operator, 116 per part and 232 overall. The IDs use
the manifest's `<prefix>.<field>.pN` form with `.p0` and `.p1`; all are block-rate
and host-automatable, and none joins the default CC map.

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

The `hz` display is the existing one (`raw * 48000 / 2^32`): 2,684,355 is 30 Hz,
178,956 is 2 Hz, 107,374 is 1.2 Hz and 44,739 is 0.5 Hz.

`frac3` is new: `value / 1000` to three decimals, so the breakpoint knob reads
`0.500` and the caption reads `bp 0.500 · π` (`bp * 2π`, shown as a multiple of
π). `lfo_wave` is new: it names the six entries of `lfoWaveNames`, not the 14
oscillator waves. `phase_env_mode` is stored as `raw` 0..3 and drawn with the
four `mod_env` mode names; mode 1 is hold, the default.

The 26 phase parameters are the nine knobs plus the `on` switch, the six LFO
parameters and the ten envelope parameters. The three follow amounts are added
to `followTargets()`, which grows from 15 to 19 entries: each operator's phase
target follows its level, feedback and amp-env targets, and the existing `wipe
all` clears the new amounts as well. The target names are
`opN.phase_ks.lo`, `opN.phase_ks.hi` and `opN.phase_vs.amount`.

## Signal path

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
before the FM offset. The FM offset and feedback are therefore not warped by the
map. The mapped path is used for every wave, including the additive bank's
morph read.

With `phase on` off, the operator must use the current 1× path unchanged. The
`phase_map` node bypasses to `acc`; `phase_add` adds the same integer FM and
feedback sum the current read adds. The off path is a hard bit-exact check, not
a tolerance.

## Engine

### One map per part and operator

The stochastic state, LFO and envelope live per part and per operator: four
operators × two parts = eight instances. Voices read the same map. The
breakpoint, LFO, envelope and follow amounts are also per part and operator.
There is no per-voice phase offset and no per-voice map.

The state is reset by the engine reset and by a part init, not by a note. The
per-instance xorshift32 seed is `0x9E3779B9 + part * 4 + op`; a fresh engine is
deterministic for tests and a playing engine keeps its texture. The stochastic
clock is the 1,500 Hz control tick: drift, flip and reorder each own a 32-bit
accumulator advanced by `rate_raw * CONTROL_BLOCK`, and the wrap is the event,
giving 0..30 Hz on the existing `hz` scale.

### The 4096 grid and `F`

The map works on a 12-bit phase, 0..4095, one point per 1/4096 cycle. A
segment's x boundaries are 0..4096; a slope is Q16 from 0.5 (32768) to 2.66
(174327); `(y - x) * slope` stays below 2^31. The table read converts `y` back
with `y << 20`, then the existing `>> 22` table index.

`F` is the lab's one-breakpoint polyline with `xb = bp / 1000`:

```text
F(x) = 0.5 * x / xb                     for x <= xb
F(x) = 0.5 + 0.5 * (x - xb) / (1 - xb)  for x > xb
```

It is continuous at `xb`, including the half-gain point. It is evaluated through
a 1024-entry, 10-bit LUT per operator, rebuilt when the folded `xb` changes on a
control tick; the 10-bit output is scaled by four into the 12-bit `G` input. The
modulation is summed in per-mille:

```text
xb_milli = fold(base_bp + env_milli + lfo_milli + follow_milli, 1, 999)
```

One percent is ten per-mille; a 100% envelope or LFO covers the whole field.

### The anti-stick fold

The fold reflects only the excess; it is not a wrap and never clamps the source
phases:

```text
span = 999 - 1
v = xb_milli - 1
v = v mod (2 * span)          // positive modulo
if (v > span) v = 2 * span - v
xb_milli = v + 1
```

At 1000 the fold returns 998; at 1100 it returns 898. Position is continuous at
the bound and only its velocity reverses. The display plots the folded position
and the phase dot follows it, so the screen agrees with the sound.

### The segmented map `G`

`G` divides the table phase into `M` equal y spans, `M` = 1..32, with y
boundaries `i / M` fixed. The input boundaries `x_k` drift; a segment is linear:

```text
slope_k = (y_{k+1} - y_k) / (x_{k+1} - x_k)
y = y_k + ((q - x_k) * slope_k) >> 16
```

A flip reverses the segment's y span, so the segment is walked from its right
boundary back to its left and the slope is negative. `order` is a one-based
permutation of the active segments: at rest `order[k] = k + 1`; `G` reads
`order[k] - 1` for the y span, and the display marks a jump where
`order[k] != k + 1`. The input span of position `k` stays natural, exactly the
lab's "new order, same segments" rule.

`G` uses a 4096-entry segment-index LUT built from the x boundaries, one 32-bit
multiply and one add. The index LUT is rebuilt on a segment, drift or order
change, not per sample.

### Locks

The map must not flip or reorder through a zero crossing or a silent flat:

1. Build a 4096-bit protected mask of the table phase for the current wave.
   A point is protected inside a zero crossing, at a touch at zero, or in a
   silent run. A run of zero samples collapses to one span; adjacent samples of
   opposite sign count as a crossing.
2. Snap the y boundaries at least two grid points away from protected edges,
   keep them sorted and inside 0..4096.
3. Collapse a protected silent run into one segment. If the requested count
   cannot fit around the protected spans, the active count is lower than the
   knob; the display shows the actual count.
4. Lock every segment whose y span touches a protected point. A locked segment
   never flips and never takes part in a reorder swap. The lock uses the lab's
   half-peak rule: the sloped sides of a crossing are protected out to the point
   where the waveform reaches half its peak.

The 13 fixed waves use masks precomputed at start-up from their mip-0 tables.
Wave 13 uses two masks built when the additive bank is regenerated, one for
spectrum a and one for b, combined at map rebuild with the part's base `morph`
`m` in Q8:

```text
lock(q) = (a_mask(q) * (255 - m) + b_mask(q) * m) >= 128
```

A silent flat needs both sides silent; a point silent in only one side is not
collapsed. The combined mask is rebuilt when a/b levels, either range, or the
base morph changes, at most once per control tick. The map follows the base
morph, not a voice's live morph modulation; that is a limit.

### Drift

Each drift event adds a Gaussian step to every segment's slope, scaled by
`drift`, reflects the slope at 0.5 and 2.66, recomputes that segment's right x
from its fixed y span, and renormalises the last x to 4096. The y spans stay at
`i / M`; the map still spans 0 to 1.

The reflected Gaussian walk uses one draw per segment per event: twelve uniform
[0,1) values summed and shifted by six (the integer form sums 16-bit draws and
scales by 2^-16). The step's standard deviation in slope units is
`0.40 * drift / 100`, so `drift = 100` steps with σ = 0.40. At 1.2 Hz
the walk's spread passes one slope unit in about six events, and the reflection
turns it back at the bounds. At `drift = 0` σ = 0 and the slopes stay at 1.

There is no pull toward 1. The OU option in the research note is not in v1.

### Flips and reorder

At each flip event, every eligible segment rerolls its orientation against the
`flip` probability. A flip reverses the segment's y span and slope; a second
flip restores it. `flip = 0` leaves every eligible segment forward; `flip = 100`
flips every eligible segment on every event. Events run on the 30 Hz stochastic
tick, not per sample.

At each reorder event, with `reorder` probability, one swap is attempted:

1. Draw `i` uniformly from the active positions and `d` uniformly from
   `1..min(jump, active - 1)`.
2. `j = (i + d) mod active`; the swap is forward and wraps at the cycle edge.
3. If either position is locked, redraw both up to three times; if all three
   draws touch a lock, the event makes no change.
4. Swap `order[i]` and `order[j]`.

With fewer than two active segments the event does nothing. `jump = 1` is the
adjacent swap, including the last-to-first wrap; larger values widen the swap
without returning to a full permutation. The lab's full permutation is not used
because it changes every join at once and cannot preserve the locks. A swap is
discontinuous at the join; that is the inharmonicity, and the display draws it
as a pink dashed vertical.

### The dedicated LFO

The phase LFO is the existing LFO shape, one instance per operator per part:

- Rate 0..20,000,000 raw, display `hz`; default 44,739 (0.5 Hz).
- Depth -100..100%; bipolar is `-depth..+depth`, unipolar is `0..depth`.
- Wave 0..5: sine, triangle, saw, inverse saw, square, random.
- `rate fade` and `depth fade` 0..99 with the existing fade law and shared graph.
- The LFO free-runs per part; a note does not reset it.

It runs at the control tick (1,500 Hz at 48 kHz, `CONTROL_BLOCK 32`), because it
only moves the breakpoint. The fade levels use the same Q31 step law as every
other LFO.

### The dedicated signed envelope

The envelope is the feedback envelope's shape:

- `on`; three rates `r1..r3` 0..99, defaults 80, 60, 50;
- three signed levels `l1..l3` -100..100, defaults 0;
- release rate `rel` 0..99, default 80; release level `rl` -100..100, default 0;
- mode 0..3, default 1, with the signed `mod_env` semantics.

The levels are absolute per-mille of the breakpoint field: level 100 adds
+1,000 per-mille, level -50 subtracts 500. There is no separate depth knob.

| Part mode | Envelope trigger |
| --- | --- |
| poly | every note-on on the part |
| unison | every note-on on the part |
| mono | the first note of a phrase, when the part had no sounding voice |
| legato | the first note of a phrase, when the part had no sounding voice |

Release starts when the part's last voice releases. The envelope is per part and
runs at the control tick. A poly retrigger reshapes held notes too; that is the
shared-map limit.

### Key and velocity follow

The phase breakpoint joins the follow-target set. Each operator's three amounts
are scaled by the existing key or velocity factor, converted to per-mille, and
added to the breakpoint sum before the fold. `followTargets()` grows from 15 to
19 entries; the follow cards' `wipe all` clears the new amounts with the rest.

### 2× read and decimation

When `phase on` is set, the map and table read run at 2×; the accumulator stays
at 48 kHz and supplies `acc` and `acc + step/2`. The FM and feedback offsets are
held for the pair (zero-order hold). The decimator is a 65-tap half-band FIR
generated into `engine/gen/math_tables.h`, with a 20.5 kHz passband and at least
70 dB of stopband from 27.5 kHz at 96 kHz. Its 32-sample delay at 96 kHz is
exactly 16 samples at 48 kHz, so the compensation is an integer delay.

When a voice contains at least one mapped operator, the unmapped operators'
routing taps get a 16-sample delay so the voice's internal alignment is
unchanged. The voice as a whole is then delayed by 16 samples at 48 kHz
(0.33 ms); that uniform delay is recorded. If the compensation is dropped, the
per-operator delay is a limit, not a silent behaviour.

The 2× path is skipped entirely when the switch is off. On is 2× even when the
map is still the identity; the switch is the activity, not a neutral-value test.
The 65-tap half-band's non-zero taps are the only products; the actual cost is
the measured CPU budget, not a hand count.

### Node graph changes

The engine graph lives under `.dev/modules`, so the implementation includes the
nodes and the macro, not only the generated C:

- new `.dev/modules/nodes/phase_map/` with `node.json` and `node.c`: the map's
  parameters, per-part state and per-sample `F`/`G` read; its `on` parameter is
  the bypass and its controls are the folded breakpoint sum;
- new `.dev/modules/nodes/phase_add/`: adds the FM and feedback offsets after
  the map, so `osc.phase` receives `map(acc) + fm`;
- `.dev/modules/nodes/phasor/`: expose the pre-FM accumulator as an `acc`
  output beside the current `phase` output; existing wiring keeps `phase`, and
  `fm_op` moves to `acc` only when the map is present;
- `.dev/modules/macros/fm_op/macro.json`: insert `phase_map` and `phase_add`
  between `phasor.acc` and `osc.phase`, add the 26 phase parameters and their
  targets, and add the phase LFO and phase envelope nodes with their wires.

The 2× read is outside the node graph's one-rate model. If the graph cannot
express it, the generated DSP carries the 2× block and the node files keep the
1× description; the divergence is recorded in the node summary rather than
hidden. `engine/gen/type_p_alpha_dsp.c` is regenerated from the nodes, not
hand-edited.

## Persistence

The manifest moves from 1,744 to 1,976 parameters; the part block moves from 865
to 981. The two parts account for 1,962 and the 14 global parameters bring the
total to 1,976.

- `PRESET_VERSION` 15 → 16.
- `kStateVersion` 13 → 14.
- `kUiPageLayout` 3 → 4.
- Each part record grows by 116 values at the end, grouped per operator: op1's
  26 phase parameters and 3 follow amounts, then op2's 29, and so on. Nothing
  existing moves, so the v15→v16 migration is an append rather than an index
  map.
- The migration chain runs in `PresetStore::migratePartBank` (there is no
  separate `part_migration.h`). Set `v15Count = kPartParamCount - 116` and
  `v14Count = v15Count - 34`; run the existing v14→v15 fade insertion to reach
  `v15Count`, then append 116 defaults. Older records (v13, v14, v15) still
  load; the new values default to phase off, env on and the neutral geometry, so
  an old record renders the current render.
- `preset_smoke` loads version 13, 14 and 15 records and checks the appended
  counts, `phase_on = 0`, `phase_env_on = 1`, `phase_bp = 500` and the 981 part
  block.
- The plugin state migration sets the new parameters to their defaults. The
  `uiPage` remap is in the Editor section.
- The CC map is regenerated: `param_to_cc` grows to 1,976 entries and every new
  parameter maps to 255. `CC_PARAMS` stays 55.

## Editor

### Tab and page

The strip becomes eight labels:

0. `patch settings`
1. `part fx/mod`
2. `operator envs`
3. `operator lfos`
4. `op phasemap` (new)
5. `op spectra a`
6. `op spectra b`
7. `op spectra mod`

`PluginEditor::buildUi`, `buildPage`, `showTab` and
`PluginProcessor::getUiPage`/`setUiPage` change their `jlimit(0, 6)` to 7.
`kUiPageLayout = 4`. `buildPage` case 4 becomes the new page and cases 5, 6
and 7 call the three spectra pages. The stored `uiPage` migration for layout 3
and older is pages 0..3 stay, 4→5, 5→6, 6→7; the existing six-tab remap still
runs first. That remap must test `pageLayout < 3`, not `< kUiPageLayout`;
otherwise a layout-3 page 5 (`op spectra b`) is moved to 6 before the new
shift.

The page is one row of four 294 px columns, the same rhythm as `operator lfos`:

| Block | Height | Contents |
| --- | --- | --- |
| operator header | 26 | `opN`, copy, paste, mute |
| composed map | 99 | the plot and the caption line |
| `phase` card | 159 | `on`, nine knobs in three rows |
| `phase lfo` card | 160 | the compact LFO row with fades |
| `phase env` card | 176 | graph, three rate/level rows, mode, `on` |

The cards are separated by 6 px (3 px column gap plus 3 px card margin), the
column padding is 8 px, and the measured total is 660 px inside the 669 px page
budget. The heights come from the mockup's measured blocks; if JUCE finds a
clip, the map loses pixels before the value text does.

### Controls

The `phase` card's nine knobs are, row by row:

1. `breakpoint`, `segments`, `flip`;
2. `flip rate`, `reorder`, `reorder rate`;
3. `jump`, `drift`, `drift rate`.

The `on` switch is in the card header. The `phase lfo` card uses the existing
compact fade row: 16 px dials, rate/depth/wave/bipolar, `rate fade` and
`depth fade`, and the two-line graph. The `phase env` card uses a 20 px purple
graph, `r1..r3`, `l1..l3`, `rel`/`rl`/`mode` in compact rows, and `on` in the
header. The LFO graph is green; the envelope graph is purple.

### Map display

`P1PhaseMap` is a new borderless component on black with a faint `line` grid and
no drag. It draws:

- `T(x)` as a 256-point polyline, each input segment green when forward and
  purple when flipped;
- locked segments at 35% opacity;
- a pink dashed vertical at each order jump (`order[k] != k + 1`);
- the fixed y boundaries as faint horizontal rules;
- `F` as a faint `lineStrong` polyline;
- the breakpoint as a pink vertical at the folded `xb`, and the phase dot as a
  white 2 px dot at `(phase_q, phase_y)` for the newest voice;
- the caption `bp 0.500 · π`, `n/m seg` and `k locked` at the bottom, in `dim`.

The snapshot is published per operator per part at 30 Hz through a double buffer
with an atomic index, the same pattern as the scope and morph taps. The audio
thread writes only the fixed snapshot: no allocation, lock or file access. The
UI timer reads the latest snapshot and draws; it does not read engine internals. When
`voices == 0` the last map stays on screen and the dot stops; the next note
resumes it. Parameter edits rebuild the static geometry immediately.

### Copy and paste

- `OperatorSection::phase` copies the 26 phase parameters.
- `OperatorSection::phaseLfo` copies the six LFO parameters.
- `OperatorSection::phaseEnvelope` copies the ten envelope parameters.
- `copyOperator`/`pasteOperator` include all three, so an operator moves whole.
- The `phase`, `phase lfo` and `phase env` cards carry `copy` and `paste` in
  their headers.
- `PluginEditor::OperatorSection` gains the three cases and the three lists in
  the copy/paste code; the existing sweep in `editor_snapshot` writes a value
  into each source and checks every destination, including `phase_on`,
  `phase_bp`, `phase_lfo_depth` and `phase_env_l1`.

### Demo settings for the live golden

The feature has no factory preset. The live golden uses these exact values on
op1; op2–4 stay off with the neutral defaults:

```text
phase on, bp 620, segments 8, flip 25%, flip rate 2.0 Hz,
reorder 30%, reorder rate 1.5 Hz, jump 2, drift 35%, drift rate 1.2 Hz;
phase lfo rate 0.5 Hz, depth +22%, sine, bipolar, rate fade 20, depth fade 40;
phase env on, r 80/60/50, l 12/28/0, rel 80, rel level 0, mode hold.
```

### Goldens

- `editor-op-phasemap.png`: all four operators off with the neutral geometry,
  covering the empty state.
- `editor-op-phasemap-live.png`: the demo above, so the composed plot, the green
  and purple segments, a lock, a jump and the phase dot are all visible.

`manifest.json` is regenerated. The other 22 goldens must not change.

## Verification

### The new engine test

`plugin/tests/phase_map.cpp`, registered as `phase_map`:

- **Off path:** render one note with `phase on` off and compare against a stored
  reference; it is bit-exact. `engine_parity` stays bit-exact.
- **Pre-map:** at breakpoint 250 and 750, check `F` at 0, 0.25, 0.5 and 1,
  including the half-gain at the breakpoint.
- **Fold:** drive the breakpoint with a 100% LFO at 0.5 Hz for ten seconds; the
  folded position never stays at 1 or 999 for more than one control tick while
  the LFO is moving, and it reflects at the bound.
- **Segments and locks:** on a sine, the active count is at most the requested
  count; protected spans collapse; no locked segment flips in 10,000 events at
  100%; the y spans still cover 0..1.
- **Flips:** at 100% every eligible segment flips and its slope is negative;
  flipping twice restores it.
- **Reorder:** the order stays a one-based permutation of the active segments;
  every segment is visited once; `jump = 1` only swaps neighbours; locked
  segments never move; fewer than two active segments is a no-op.
- **Drift:** 10,000 events keep every slope in [0.5, 2.66], every x sorted and
  the last x at 4096.
- **Modulation:** a positive envelope level moves the breakpoint right and a
  negative one left; the LFO sign inverts the sweep in bipolar mode; a fade
  reaches full on the computed tick.
- **Trigger:** poly and unison retrigger on every note; mono and legato trigger
  only on the first note after silence; release starts with the last note-off.
- **Parts:** part 0 and part 1 keep separate maps, breakpoints and seeds.
- **Spectrum:** wave 13's mask follows a and b and the base morph; a midpoint
  morph locks the union of the protected spans.
- **2×:** the mapped render is finite, has no `INT32_MIN`/`INT32_MAX`, and its
  RMS is within the same bound as the 1× render; a slow map matches the 1×
  render within the filter's tolerance.
- **CPU:** print the worst-case ratio described below.

### Existing checks

- `engine_parity`: the phase-off reference is unchanged and the new parameter
  defaults do not move existing renders.
- `preset_smoke`: versions 13, 14 and 15 load with the phase block appended;
  the part count is 981 and the defaults are in place.
- `editor_snapshot`: the copy/paste checks, the two new goldens and the follow
  target count changing from 15 to 19.
- `plugin_smoke`, `rate_bridge`, `lfo_fades`, `spectral_bench`: unchanged, run
  after the manifest and tab changes.
- The full Linux CTest suite and the macOS/Windows CI matrix.

### Gates and failure criteria

- **CPU:** baseline 24 voices, four operators, phase off, 48 kHz; worst case 24
  voices, four operators all on, 32 segments, 30 Hz drift, 100% flip and reorder,
  both modulation sources, wave 13. Render 1,000 blocks and divide by the
  baseline. Gate: **1.6× baseline**. If over, fall back to the 1× map with a
  short join ramp and record the aliasing limit; do not ship an unmeasured 2×
  path.
- **Aliasing:** measure the worst-case spectrum and record the 20.5–24 kHz
  fold-back. The feature reduces aliasing; it does not claim it is removed.
- **Timing:** a mapped carrier and an unmapped modulator must stay aligned; if
  the compensation cannot be done cheaply, run all four operators of a voice
  through 2× so they share the delay.
- **Display:** the four columns must not clip at 1,206 × 685, and the map must
  stay readable at 270 × 86.

## Evidence

The engine is unchanged, so there is no phase-map measurement yet. The current
baseline is 1,744 manifest parameters, an 865-value part block,
`PRESET_VERSION` 15, state version 13 and `kUiPageLayout` 3. The current
`engine_parity` reference is the off-path target.

The build must add these measurements to this section; until then, none of them
is a pass:

| Evidence | Target | Status |
| --- | --- | --- |
| Off-path render | bit-exact against the current reference | pending |
| `preset_smoke` | versions 13/14/15 load at 981 values | pending |
| `phase_map` | all checks above | pending |
| worst-case CPU | ≤ 1.6× baseline | pending |
| alias spectrum | measured; 20.5–24 kHz recorded | pending |
| snapshots | two new goldens, 22 unchanged | pending |

## Limits

- **Aliasing is reduced, not removed.** 2× plus the half-band passes to
  20.5 kHz and is at least 70 dB down from 27.5 kHz, leaving 20.5–24 kHz
  fold-back. Extreme reorder at 30 Hz still has content near Nyquist; the
  contract records the spectrum rather than claiming a clean result.
- **The map is per part.** A new note can reshape held notes in poly mode; the
  envelope cannot follow each voice separately.
- **The locks can reduce the active count.** A waveform with many protected
  spans may not fit 32 requested segments; the display shows the actual count.
- **The spectral mask uses the base morph.** A voice's live morph modulation is
  not in the lock mask.
- **The breakpoint fold is a reflection.** At the edge the geometry turns
  around; it does not continue past the edge or wrap.
- **Reorder is local swaps.** It is not the lab's full permutation or its
  forward jump; the `jump` knob widens the swap distance.
- **The 2× path has a group delay.** It is compensated inside a voice; the voice
  as a whole is delayed by 16 samples at 48 kHz (0.33 ms).
- **The map is not a new oscillator.** The 13 waves and the additive bank are
  unchanged; only the phase walking through them moves.
- **No factory preset.** The init state is fully off; the demo settings above
  are for the golden, not a bank slot.

## Risks and fallback

- **CPU.** The 2× path plus the filter is the risk. Measure first; if the worst
  case exceeds 1.6×, fall back to the 1× map with a short join ramp and record
  the aliasing limit. Do not ship an unmeasured 2× path.
- **Timing.** If the decimator delay cannot be compensated cheaply, run all four
  operators of a voice through 2× whenever one is mapped, so they share the
  delay; the off operators keep their values but pay the filter.
- **Locks.** If the full lab rule cannot fit the control budget for wave 13,
  compute the a/b masks on the base spectra and interpolate. Do not drop the
  locks silently.
- **Clicks.** Reorder jumps are intentional. If they read as clicks at 30 Hz,
  the fallback is a short crossfade at the join; the contract records whether it
  was added.
- **Display.** If the 30 Hz snapshot reads as a stutter, raise the UI timer or
  interpolate the dot; do not move the map computation to the UI thread.
- **Migration.** The 116 values are appended, but the existing fade insertion
  uses the current target count. Set `v15Count` and `v14Count` as above and test
  v15 before v14/v13; otherwise the fade slots land in the wrong place.
- **Rollback.** Remove the tab, the 232 parameters and the 2× branch. A
  pre-release rollback can return `PRESET_VERSION` to 15; a shipped v16 record
  must keep the version.

## Files

- **Engine and generation:** `engine/gen/params.json`, `dsp.h`,
  `type_p_alpha_dsp.c`, `math_tables.h`, `tables.c`, `cc_map.h`;
  `.dev/modules/nodes/phase_map/`, `phase_add/`, `phasor/` and
  `macros/fm_op/macro.json`.
- **Plugin and state:** `plugin/ui/ParamIDs.h`, `plugin/ui/Widgets.h/.cpp`,
  `plugin/juce/PluginProcessor.h/.cpp`, `plugin/juce/PluginEditor.h/.cpp`,
  `plugin/preset/PresetStore.hpp/.cpp`.
- **Editor:** `plugin/juce/Pages.h`, `plugin/juce/PagesPhase.cpp` (new),
  `plugin/juce/CMakeLists.txt`.
- **Tests:** `plugin/tests/phase_map.cpp` (new), `engine_parity.cpp`,
  `preset_smoke.cpp`, `editor_snapshot.cpp`, `plugin/tests/snapshots/linux-x86_64/`.
- **Docs:** this contract, `design/mockup.html`, `design/README.md`,
  `design/juce.md`, `docs/research/stochastic-phaseshaping.md`, `README.md`,
  `plugin/README.md`.
