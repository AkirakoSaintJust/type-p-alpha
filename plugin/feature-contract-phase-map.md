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
- **Owner / date:** P1Onken, 2026-11-21, with implementation evidence on 2026-11-22.
- **Disposition:** implemented at 1×. The engine nodes, the 232 parameters, the
  editor page, the preset migration, the engine test and the live snapshot are in
  the tree. The 2× read, the half-band decimator and the 16-sample alignment delay
  are not built. The measured 1× cost and the aliasing limit are recorded below.
  No layer is called passed beyond its measured evidence.

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
- **Off behaviour:** `phase on` off uses the current accumulator-add-read path
  and is bit-exact against the current `engine_parity` reference. The map does
  not run and its LUTs are not read when it is off. The rendered path is 1×. The
  2× option is not built.
- **Failure mode:** the off path moves by one sample, the fold wraps to the other
  edge or freezes, a locked segment flips or moves, a follow amount is inert or
  scaled wrong, the map leaks between operators or parts, the four columns clip at
  the design size, or the 1× alias limit is left unrecorded.

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
x12 = acc >> 20                // 12-bit input to the map, 0..4095
q = F(x12)                     // pre-map, 1024-entry LUT, output 0..4096
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
   silent run. Adjacent samples of opposite sign count as a crossing. The
   sloped sides of a crossing are protected out to the point where the waveform
   reaches half its peak.
2. Snap each y boundary to a point at least two grid steps away from any
   protected point, keep the boundaries sorted and inside 0..4096, and keep two
   grid steps between neighbours.
3. If no candidate fits, lower the active count by one and try the next
   boundary. A protected run is never cut by a boundary. The display shows the
   actual count rather than silently slicing a crossing.
4. Lock every span whose y range contains a protected point. A locked segment
   never flips and never takes part in a reorder swap.

The 13 fixed waves build their mask from their mip-0 table on the first use of
an operator and part. Wave 13 builds two masks from the regenerated additive
bank, one for spectrum a and one for b, and combines them with the part's base
`morph` `m` in Q8:

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
are scaled by the existing key or velocity factor, a Q15 push, and converted to
per-mille:

```text
follow_q15 = key_scale(note, breakpoint, ks_lo, ks_hi)
           + vel_scale(velocity, vs)            // both Q15, summed
follow_milli = follow_q15 * 1000 / 32768
```

The result is added to the breakpoint sum before the fold. `followTargets()`
grows from 15 to 19 entries. The follow cards' `wipe all` clears the new
amounts with the rest. The velocity factor had an inert `p1_vel_scale` (a divisor of
3,276,700 made every velocity follow 32,767 times too small. The corrected
`amount * vel / 100` returns Q15, and the phase, level, feedback and envelope
follows all move again. The default patch has zero follow amounts, so the
existing renders do not move.

### 1× read and the recorded aliasing limit

The map and the table read run at the core's 48 kHz. The 2× read, the 65-tap
half-band decimator and the 16-sample alignment delay in §2.1 of the spec are
not built. The node graph is one-rate, and the generated DSP follows the graph.
A 2× path would need a second map and table read plus a filter for every mapped
operator, so it is a separate piece of work rather than a flag.

The measured 1× worst case is 1.75× the phase-off baseline: 24 voices, four
operators on, 32 segments, 30 Hz drift, 100% flip and reorder, both modulation
sources, wave 13, 1,000 blocks. The 1.6× figure in §3.17 of the spec was the
budget for the 2× design. The 1× path already costs more than that, so the 2×
design is not viable on this evidence and is not shipped.

The joins are step discontinuities. The table is band-limited and the map is not,
so the 20.5–24 kHz fold-back is unmeasured and is recorded as a limit rather
than claimed clean. No join ramp is added: a reorder jump is the intended
inharmonic event, and a short crossfade stays a future option if a listening
test asks for it.

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

The graph is one-rate and the shipped map is the 1× node. The 2× block, the
half-band filter and the alignment delay are not present in the generated DSP;
the node summary records that limit rather than implying a hidden second path.
`engine/gen/type_p_alpha_dsp.c` is regenerated from the nodes, not hand-edited.

The macro expander (`_frontend.py` in the generator, mirrored in the framework
repository) keeps a list of inner destinations for each macro input. A single
destination lost the fan-out from `bp`, `kon` and `von`: the phase map and the
level and feedback follows were left at their input defaults. The list fixes
that, and the key and velocity follow test now covers it.

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
- `editor-op-phasemap-live.png`: the demo above. The captured state shows the
  composed plot, the locked spans at 35% and the LFO-moved breakpoint. At 2 Hz and
  1.5 Hz the eight snapshot blocks do not reach the first flip or reorder event,
  so the still shows the forward green segments only. The flip and reorder
  layers are covered by the engine test rather than by this still.

`manifest.json` is regenerated. The two phase pages are new. The pages that show
the scope trace moved with the audio change, so the whole set was reviewed and
re-accepted in one pass rather than leaving stale traces behind.

## Verification

### The new engine test

`plugin/tests/phase_map.cpp`, registered as `phase_map`, runs 40 checks in one
process and prints the measured fold range, active and locked counts, drift
slopes, follow positions and CPU ratio:

| Check | What it renders or reads | Result |
| --- | --- | --- |
| off path | two fresh engines, map parameters inert | 0 differing samples in 4,096 frames |
| `F` at 0.500 | breakpoint 500 | 0 at 0, 512 at 0.5, 1023 at the end |
| `F` at 0.750 | breakpoint 750 | 512 at 0.75, 170 at 0.25 |
| fold | 100% triangle LFO, 2,000,000 raw | 2..998, one tick at each edge, reflects |
| locks | sine, 32 requested | 31 active, 3 locked, crossing span locked |
| flips | 100%, 20 events | every unlocked span flips, the second event restores |
| reorder | 16 segments, `jump` 1, 400 events | permutation held, neighbours only, locked spans still |
| drift | 16 segments, 120 events | slopes 0.5001..2.6584, x sorted |
| envelope | +25% and -25%, mode hold | breakpoint 750 and 250 |
| LFO | +50% and -50% | the pooled value inverts exactly |
| fades | rate and depth fade 99 | both full on the first tick |
| key follow | notes 48 and 72, amounts -100/+100 | 301 and 679 |
| velocity follow | velocities 20 and 120, amount 25 | 539 and 736 |
| trigger | poly and legato | poly resets, legato holds |
| parts | part 0 and part 1 | separate breakpoints, counts and seeds |
| wave 13 | a/b spectra and base morph | mask non-empty and unchanged by segment count |
| render | 48,000 frames | finite and not silent |
| CPU | 24 voices, four operators, worst case | 1.75× the phase-off baseline |

The velocity row is measured after the `p1_vel_scale` fix. The old divisor made
the Q15 factor 32,767 times too small, so every velocity follow, including the
existing level, feedback and envelope follows, was inert. The corrected factor
is `amount * vel / 100`, and the default patch has zero amounts, so the existing
renders do not move.

The 2× row in the old draft is not a test because the path does not exist. The
render is the 1× render, and the aliasing limit is recorded below.

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
  baseline. The measured 1× ratio is **1.76×**. The 1.6× gate was the budget for
  the 2× design; the 1× map does not meet it, and the 2× path is not shipped.
  The ratio is recorded rather than rounded down.
- **Aliasing:** the 2× read and the half-band filter are not built, so the
  20.5–24 kHz fold-back is **unmeasured**. The joins are step discontinuities and
  the feature does not claim a clean spectrum.
- **Timing:** a mapped carrier and an unmapped modulator stay aligned at 1×
  because there is one read and no filter delay. The 16-sample alignment problem
  belongs to the unbuilt 2× path.
- **Display:** the four columns do not clip at 1,240 × 985, and the map stays
  readable at 270 × 86. The snapshot at 1,240 × 985 is the evidence.

## Evidence

The measured state on 2026-11-22:

| Evidence | Target | Result |
| --- | --- | --- |
| Off-path render | bit-exact against the current reference | pass: 0 differing samples in 4,096 frames, and `engine_parity` is unchanged |
| `preset_smoke` | versions 13/14/15 load at 981 values | pass |
| `phase_map` | all checks above | pass: 40 checks |
| worst-case CPU | 1.6× baseline for the 2× design | **1.76× on the 1× path; the 2× design is not built** |
| alias spectrum | measured; 20.5–24 kHz recorded | **not measured; the 1× joins are unband-limited** |
| snapshots | two new goldens, shared traces re-accepted | pass: 24 PNGs and the manifest; the two phase pages are new |
| manifest | 1,976 parameters, 981-value part block | pass |

The old baseline (1,744 parameters, 865 values, `PRESET_VERSION` 15, state 13,
`kUiPageLayout` 3) moved to 1,976, 981, 16, 14 and 4 as planned. The 2× path is
the missing layer, and no row above calls it passed.

## Limits

- **The read is 1×, not 2×.** The 2× map read, the half-band decimator and the
  16-sample alignment delay are not built. The join aliasing (20.5–24 kHz
  fold-back) is therefore unmeasured, and the CPU cost is 1.76× the phase-off
  baseline.
- **No join ramp.** A reorder jump is a step discontinuity and is heard as the
  intended inharmonic event. A short crossfade remains a future option; no
  listening test has asked for one.
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
- **The map is not a new oscillator.** The 13 waves and the additive bank are
  unchanged; only the phase walking through them moves.
- **No factory preset.** The init state is fully off; the demo settings above
  are for the golden, not a bank slot.

## Risks and fallback

- **CPU.** The 2× path plus the filter was the original risk. The 1× path measures
  1.76× the phase-off baseline, above the 1.6× target for 2×, so the 2× branch is
  not built. If the 1× cost needs to fall, the map's per-block rebuild is the
  first place to look; the per-sample read is one F lookup, one segment lookup
  and two multiplies.
- **Timing.** At 1× there is one read and no filter delay, so a mapped carrier
  and an unmapped modulator stay aligned. A future 2× path would need the
  per-voice alignment delay described in the spec.
- **Locks.** The full lab rule is in the node: the fixed waves build a mask from
  their mip-0 table, and wave 13 interpolates the a and b masks at the base
  morph. A voice's live morph is not in the mask; that is the recorded limit.
- **Clicks.** Reorder jumps are intentional and no crossfade was added. If a
  listening test asks for one, add a short crossfade at the join and re-measure;
  the contract would then record the ramp.
- **Display.** If the 30 Hz snapshot reads as a stutter, raise the UI timer or
  interpolate the dot; do not move the map computation to the UI thread.
- **Migration.** The 116 values are appended, but the existing fade insertion
  uses the current target count. Set `v15Count` and `v14Count` as above and test
  v15 before v14/v13; otherwise the fade slots land in the wrong place.
- **Rollback.** Remove the tab and the 232 parameters. A pre-release rollback can
  return `PRESET_VERSION` to 15; a shipped v16 record must keep the version.

## Files

- **Engine and generation:** `engine/gen/params.json`, `dsp.h`,
  `type_p_alpha_dsp.c`, `tables.c`, `cc_map.h`, `engine/hal/p1_math.h`;
  `.dev/modules/nodes/phase_map/`, `phase_add/`, `phasor/` and
  `macros/fm_op/macro.json`; `_frontend.py` for the macro fan-out.
- **Plugin and state:** `plugin/ui/ParamIDs.h`, `plugin/ui/Widgets.h/.cpp`,
  `plugin/juce/PluginProcessor.h/.cpp`, `plugin/juce/PluginEditor.h/.cpp`,
  `plugin/preset/PresetStore.hpp/.cpp`.
- **Editor:** `plugin/juce/Pages.h`, `plugin/juce/PagesPhase.cpp` (new),
  `plugin/juce/CMakeLists.txt`.
- **Tests:** `plugin/tests/phase_map.cpp` (new), `engine_parity.cpp`,
  `preset_smoke.cpp`, `editor_snapshot.cpp`, `plugin/tests/snapshots/linux-x86_64/`.
- **Docs:** this contract, `stochastic-phaseshaping-spec.md`, `design/mockup.html`,
  `design/README.md`, `design/juce.md`, `docs/research/stochastic-phaseshaping.md`,
  `docs/research/phase-transfer-lab.html`, `README.md`, `plugin/README.md`.
