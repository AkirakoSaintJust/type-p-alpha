# Feature contract: LFO rate and depth fades

## Identity

- **Feature:** Per-LFO rate fade-in and depth fade-in. Each fades its target
  from zero over a linear attack, so an LFO can speed up or open up after the
  note starts.
- **Brief section:** the operator lfos, spectral mod and part fx/mod cards, and
  the LFO entries in the README.
- **Owner / date:** P1Onken, 2026-11-20.
- **Disposition:** complete. The engine, the five card families, the `lfo_fades`
  test, the copy and paste lists and the Linux goldens are in.

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

The `fade` display shows `off` at 0 and the rate value at 1 to 99. The number
rises with the dial, so 1 is the slowest and 99 is instant. The rate table's 99
percent times are about 33.5 s at 1, 2.1 s at 50, 53 ms at 90 and 8 ms at 98.
The value sits under each knob, so the two fades read together.

## Fade law

The two levels are Q31. When a fade is on, the trigger loads the first step, and
the level then rises by one step per control tick:

```text
ticks(rate) = 4605 * 65536 / (1000 * p1_rate_coeff[rate])
step(rate)  = round(2^31 / ticks(rate))
level       = min(2^31, level + step)
```

The 99 setting sets the level to 2^31 on the trigger, so it is instant. The
first step lands on the trigger block rather than one block later. The Q31
fraction keeps the slow settings on the table time: a Q16 level would round fade
1 from 33.5 s to about 43.7 s. The rate and depth targets are scaled by the
level in Q16:

```text
phase += (rate * (rate_level >> 15) >> 16) * CONTROL_BLOCK;
depth_eff = depth * (depth_level >> 15) >> 16;
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
leaves the separation in the difference. The two values sit under their knobs,
so the player reads the two times directly.

The operator cards keep the four knobs in one row with the wave select and
`bipolar` below. The spectral morph card uses the compact row: rate, depth,
`rate fade` and `depth fade` as four 22 px dials, with the wave select and
`bipolar` below.

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
- **Checks:** `preset_smoke` loads version 10, 12 and 13 records and checks the
  part and absolute envelope expansion before the fade insertion.

## Engine

- **Node:** `modules/nodes/lfo` gains `rate_fade` and `depth_fade` parameters, a
  `trigger` event input defaulting to `voice_trig[v]`, two Q31 levels and two Q31
  steps. The first step is loaded on the trigger.
- **Wiring:** `modules/macros/fm_op` maps the trem, feedback and detune pairs;
  the `osc` spectral LFO maps its pair; `synth/patch.json` maps the part pitch
  pair.
- **CPU:** one add, one compare and two Q31-to-Q16 multiplies per LFO per control
  tick, plus the existing phase and depth work.
- **State:** four `uint32_t` per instance, 16 bytes. A voice carries 17 instances
  (four operators across four families, plus its part's pitch LFO), so 272 bytes
  per voice and 6,528 bytes across 24 voices. The 34 pairs cover all four
  operators and both parts.
- **Timing:** the two steps are block-derived; the two levels move per control
  tick. The trigger block is not lost.

## Editor

- `LfoCard` and `PitchLfoCard` gain a row with the two knobs, labelled `rate
  fade` and `depth fade`.
- The operator LFO cards are 107 px, with rate, depth, `rate fade` and
  `depth fade` as four knobs in one row, and the wave select and `bipolar`
  below. The 12 px gaps and the 240 px scope and spectro carry the breathing
  space. The pitch LFO card keeps its own 36 px layout and its wave view.
- The spectral morph LFO card uses the compact 102 px row described above.
- The knobs show `off` at 0 and the rate at 1 to 99; 99 is instant.
- The section copy and paste lists include both fade parameters for the
  amplitude, feedback, detune and spectral LFOs.
- The operator lfos page no longer repeats the oscillator. The oscillator is on
  the operator envs page; the operator lfos page carries the three LFO cards.
- The card heights and the page grids are re-measured, and the snapshot goldens
  are regenerated deliberately.

## Verification

- **Off path:** `lfo_fades` renders the LFOs with both fades at 0 and checks the
  levels sit at full from the trigger. `engine_parity` stays bit-exact.
- **Rate fade:** a tone with a fast LFO and a long rate fade freezes the phase at
  the first tick, stretches the first cycle, and reaches the target rate on the
  computed tick.
- **Depth fade:** the modulation depth rises linearly and reaches the target on
  the computed tick, and a negative depth keeps its sign.
- **Synchronisation:** equal values end on the same tick; a shorter second value
  ends earlier by the computed difference.
- **Phase reset:** a fade-on note resets the phase; a fade-off note keeps the
  free run.
- **Pitch and spectral:** the part pitch LFO and one operator spectral LFO are
  checked as well as the operator LFOs.
- **Copy and paste:** `editor_snapshot` writes a value into each source LFO
  section and checks every destination parameter, including both fades.
- **UI:** `editor-operator-lfos.png` shows the three cards and their fade rows,
  `editor-operator-lfos-fades.png` shows two different fade times in the value
  text, and `editor-op-spectra-mod.png` shows the compact row. The snapshot diff
  is exact.

## Evidence

`lfo_fades` reports 25 checks. The timing is read from the node state:

| Fade | Table 99 percent time | Table ticks | Reaches full |
| --- | --- | --- | --- |
| 1 | 33.532 s | 50,298 | 50,299 |
| 2 | 33.532 s | 50,298 | 50,299 |
| 50 | 2.140 s | 3,210 | 3,211 |
| 70 | 0.485 s | 727 | 727 |
| 90 | 0.053 s | 79 | 80 |
| 98 | 0.008 s | 12 | 12 |
| 99 | instant | 1 | 1 |

The rounded Q31 step can fall one tick short, so the contract's tolerance is one
tick. The audible checks measure the 5 ms RMS spread across 20 to 120 ms and
1.0 to 1.1 s:

```text
off   0.745 / 0.674   level early and late
depth 0.229 / 0.674   the depth fade opens the tremolo
rate  0.039 / 0.688   the rate fade stretches the first cycle
```

The negative depth render differs from the positive render, and all five
families show a nonzero level on the first tick and full level with the fades
off.

## Limits

- The fade is linear, whilst the envelope stages use a one-pole. The end times
  match the envelope's 99 percent time; the curves do not.
- The rate fade starts at 0 Hz, so the first cycle stretches. A patch that wants
  the LFO at its target speed from the first tick leaves the fade off.
- The 0, 1 and 2 values share the same table time, because the envelope rate
  table does.
- The fade is per note. A held legato note that retriggers the trigger restarts
  the fade; a note without a new trigger does not.
- The time is carried by the value under each knob. The shared table decides the
  order; the numbers carry the separation.

## Risks and fallback

- **Bit-exactness:** the off path keeps the render and `engine_parity` passes. If
  it changes, stop and check the phase reset and the level multipliers before
  the UI.
- **Parameter map:** the inserted pairs shift an existing index. Fix the map
  before the golden pass, not the test. `preset_smoke` covers the old records.
- **Card density:** the operator cards are 107 px and the spectral card uses the
  compact 102 px row. If a card still clips, take the height from the empty tail
  of the neighbour card rather than shrinking the value text.
- **Rollback:** remove the two parameters and the trigger input, and restore the
  current `lfo` node; the presets load with the fades off either way.
