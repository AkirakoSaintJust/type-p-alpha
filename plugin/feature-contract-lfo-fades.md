# Feature contract: LFO rate and depth fades

## Identity

- **Feature:** Per-LFO rate fade-in and depth fade-in. Each fades its target
  from zero over a linear attack, so an LFO can speed up or open up after the
  note starts.
- **Brief section:** the operator lfos, spectral mod and part fx/mod cards, and
  the LFO entries in the README.
- **Owner / date:** P1Onken, 2026-11-20.
- **Disposition:** in progress. Contract first; the engine, the cards, the
  snapshots and the tests follow.

## Player outcome

- **New player action:** every LFO card gains `rate fade` and `depth fade`. At
  `off` the LFO behaves as it does now. From 1 to 99 the LFO attacks from zero,
  with 1 the longest and 99 instant.
- **Sound and behaviour:** the rate fade runs the LFO from 0 Hz to its rate, so
  the first cycle stretches as the LFO speeds up. The depth fade runs the
  modulation depth from 0 to its target, so a tremolo, vibrato or morph opens
  up. Both start on the note-on trigger, and the LFO phase resets to zero when
  either fade is on, so the attack starts at a known point.
- **Failure mode:** the off path changes the render, the fades restart on the
  wrong edge, the two fades cannot be lined up, a negative depth loses its sign,
  or the rate fade freezes the LFO after the attack.

## Scope

Five LFO families and 34 instances:

| Family | Per operator | Per part | Instances |
| --- | --- | --- | --- |
| amplitude (trem) | yes | no | 8 |
| feedback | yes | no | 8 |
| detune | yes | no | 8 |
| spectral | yes | no | 8 |
| pitch | no | yes | 2 |
| total | | | 34 |

Two parameters per instance is 68 new parameters.

## Parameters

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `opN_trem_rate_fade` | `opN.lfo.rate_fade` | 0..99 | 0 | `fade` |
| `opN_trem_depth_fade` | `opN.lfo.depth_fade` | 0..99 | 0 | `fade` |
| `opN_fblfo_rate_fade` | `opN.fblfo.rate_fade` | 0..99 | 0 | `fade` |
| `opN_fblfo_depth_fade` | `opN.fblfo.depth_fade` | 0..99 | 0 | `fade` |
| `opN_dlfo_rate_fade` | `opN.dlfo.rate_fade` | 0..99 | 0 | `fade` |
| `opN_dlfo_depth_fade` | `opN.dlfo.depth_fade` | 0..99 | 0 | `fade` |
| `opN_spec_lfo_rate_fade` | `opN.speclfo.rate_fade` | 0..99 | 0 | `fade` |
| `opN_spec_lfo_depth_fade` | `opN.speclfo.depth_fade` | 0..99 | 0 | `fade` |
| `pitch_rate_fade` | `plfo.rate_fade` | 0..99 | 0 | `fade` |
| `pitch_depth_fade` | `plfo.depth_fade` | 0..99 | 0 | `fade` |

The `fade` display shows `off` at 0 and a time at 1 to 99: `instant` at 99,
about 8 ms at 98, about 53 ms at 90, about 2.3 s at 50 and about 33.5 s at 1.
The time is the envelope rate table's 99 percent time, so a fade value means the
same time as an envelope rate value.

## Fade law

The two levels are Q16 and start at 0 when their fade is on. They rise by one
step per control tick:

```text
ticks(rate) = 4605 * 65536 / (1000 * p1_rate_coeff[rate])
step(rate)  = 65536 / ticks(rate)
level       = min(65536, level + step)
```

The 99 setting skips the ramp and sets the level to 65536 on the trigger. The
ramp is linear, so the level reaches the target on the computed tick and the
separation between the two fades is exact. The rate and depth targets are
scaled by their levels:

```text
phase += (rate * rate_level >> 16) * CONTROL_BLOCK;
depth_eff = depth * depth_level >> 16;
```

The rate fade starts at 0 Hz, so the phase stands still until the rate moves.
The depth fade scales the signed depth, so a negative depth grows in magnitude
and keeps its sign. In unipolar mode the output still moves from the target to
the depth, and at level 0 it is the target.

Both fades start on the note-on trigger, the same `voice_trig[v]` edge the
envelopes use. If either fade is on, the phase resets to 0 on that edge. If both
are off, the phase keeps its free run and the render stays bit-exact.

## Synchronisation and separation

Both knobs use the same time table. Setting them to the same value ends both
fades on the same tick; setting different values puts the shorter fade first and
leaves the separation in the difference. The LFO card carries a two-line fade
graph, green for the rate and purple for the depth, with the end points marked,
and a time readout for both fades. The graph is the shared scale, so the player
can see where the two attacks line up and where they separate.

## Persistence

- **Counts:** the manifest moves from 1,676 to 1,744 parameters and the part
  block from 831 to 865.
- **Order:** each fade pair is inserted after its LFO's mode parameter, and the
  part pitch pair after `pitch_mode`.
- **Versions:** `PRESET_VERSION` 14 to 15, the plugin state version 12 to 13.
- **Migration:** the new parameters default to 0, so an older record loads with
  the fades off and the current render. The pairs sit inside the part block, so
  the record grows by 34 values per part and the migration maps each old index to
  its new index, inserting 0 at the 34 new positions. The record-size append is
  not enough; the version 14 map follows the version 13 map's pattern.

## Engine

- **Node:** `modules/nodes/lfo` gains `rate_fade` and `depth_fade` parameters, a
  `trigger` event input defaulting to `voice_trig[v]`, the two Q16 levels and the
  two steps in its state.
- **Wiring:** `modules/macros/fm_op` maps the trem, feedback and detune pairs;
  the `osc` spectral LFO maps its pair; `synth/patch.json` maps the part pitch
  pair.
- **CPU:** one add, one compare and two Q16 multiplies per LFO per control tick,
  plus the existing phase and depth work. The levels are block-derived.
- **State:** four `int32_t` per LFO per voice, so 136 bytes per voice at 34
  instances, and 3,264 bytes across 24 voices.

## Editor

- `LfoCard` and `PitchLfoCard` gain a row with the two knobs, labelled `rate
  fade` and `depth fade`, and a 24px two-line fade graph under the oscillator
  or wave view.
- The knobs show `off` at 0 and the time at 1 to 99.
- The spectral LFO card on the spectra mod page gets the same row and graph.
- The card heights and the page grids are re-measured, and the snapshot goldens
  are regenerated deliberately.

## Verification

- **Off path:** a new `lfo_fades` CTest renders every LFO with both fades at 0
  and compares the engine with the current capture. It must be bit-exact.
- **Rate fade:** a tone with a fast LFO and a long rate fade shows the phase
  frozen at the first tick and the first cycle stretching, and reaches the
  target rate on the computed tick.
- **Depth fade:** the modulation depth rises linearly and reaches the target on
  the computed tick, and a negative depth keeps its sign.
- **Synchronisation:** equal values end on the same tick; a shorter second value
  ends earlier by the computed difference.
- **Phase reset:** a fade-on note resets the phase; a fade-off note keeps the
  free run.
- **Pitch and spectral:** the part pitch LFO and one operator spectral LFO are
  checked as well as the operator LFOs.
- **UI:** the snapshot shows the two knobs, the time readout and the two-line
  graph on the operator, spectral and part cards, and the snapshot diff is
  accepted.

## Limits

- The fade is linear, whilst the envelope stages use a one-pole. The end times
  match the envelope's 99 percent time; the curves do not.
- The rate fade starts at 0 Hz, so the first cycle stretches. A patch that wants
  the LFO at its target speed from the first tick leaves the fade off.
- The 0, 1 and 2 values share the same table time, because the envelope rate
  table does.
- The fade is per note. A held legato note that retriggers the trigger restarts
  the fade; a note without a new trigger does not.

## Risks and fallback

- **Bit-exactness:** if the off path changes the render, stop and check the
  phase reset and the level multipliers before the UI.
- **Parameter map:** if the inserted pairs shift an existing index, fix the map
  before the snapshot pass, not the test.
- **Card density:** if the two knobs and the graph do not fit, move the fade row
  under the wave view and keep the graph at 20px before shrinking the knobs.
- **Rollback:** remove the two parameters and the trigger input, and restore the
  current `lfo` node; the old presets load with the fades off either way.
