# Feature contract: envelope semantics

## Identity

- **Feature:** Replace the four-rate/four-level envelope with two explicit
  families: an operator amplitude envelope with a composite attack, and a bipolar
  modulation envelope with three stages, a release, and four modes.
- **Brief section:** the operator envelope page, the part fx/mod cards and the
  research note on envelopes.
- **Owner / date:** P1Onken, 2026-11-20.
- **Disposition:** in progress. The semantics are agreed and the slow-start
  primitive is in `engine/hal/p1_math.h`. The node, macro, parameter, UI, graph
  and migration work is not done, and no layer is claimed as passed.

## Operator amplitude envelope

Eight controls per operator:

| Control | Range | Meaning |
| --- | --- | --- |
| attack warm-up rate | 0 off, 1 to 99 | Convex rise to 50% of the operator level, or to 100% when the surge is off. 99 is instant. |
| attack surge rate | 0 off, 1 to 99 | Concave rise to 100% of the operator level, starting from silence when the warm-up is off. 99 is instant. |
| decay1 rate | 1 to 99 | Moves to decay1 level. Falling is convex, rising is concave. 99 is instant. |
| decay1 level | 0 to 99 | 99 is the operator level, 0 is silence. |
| decay2 rate | 1 to 99 | Moves to decay2 level. Falling is convex, rising is concave. 99 is instant. |
| decay2 level | 0 to 99 | 99 is the operator level, 0 is silence. |
| release rate | 1 to 99 | Falls to silence. Concave falling. 99 is instant. |
| mode | 0 to 3 | 0 sustain off, 1 sustain on, 2 loop1, 3 loop2. Default 0. |

Both attack rates off means the attack is instant to full level.

Modes:

- **Sustain off:** the release starts at the end of decay2; a note-off earlier
  starts it at once.
- **Sustain on:** hold the decay2 level until note-off, then release. Ignored when
  the decay2 level is 0, which behaves like sustain off.
- **Loop1:** at the decay2 level, return to the decay1 level at the decay1 rate,
  then back to the decay2 level at the decay2 rate, repeating until note-off.
- **Loop2:** the same cycle with both directions at the decay2 rate.

## Bipolar modulation envelope

Eight controls per envelope plus its on switch:

| Control | Range | Meaning |
| --- | --- | --- |
| rate1 | 1 to 99 | Moves from the target value to level1. Falling convex, rising concave. |
| level1 | the current level range | First target, read as a position in the target's range. |
| rate2 | 1 to 99 | Moves to level2. |
| level2 | the current level range | Second target. |
| rate3 | 1 to 99 | Moves to level3. |
| level3 | the current level range | Third target, and the sustain point. |
| release rate | 1 to 99 | Falls to the release level. Concave falling. |
| release level | the current level range | The level the release ends at. |
| mode | 0 to 3 | Same four modes; sustain holds level3 and the loops cycle level2 and level3. |

The envelope starts from the target parameter's value at note-on. For the filter,
spectral and pan envelopes the levels are absolute positions in the target's range,
so the knob is the starting point and the depth control retires. The pitch envelope
stays an offset in Q15 cents because its target is the note, and keeps its depth.
Feedback keeps its Q15 levels as positions across the feedback range.

- **Sustain off:** release at the end of stage 3.
- **Sustain on:** hold level3 until note-off; ignored when level3 equals the release
  level.
- **Loop1:** level2 to level3 at rate2, back at rate3.
- **Loop2:** the same cycle with both directions at rate3.

## Curves

- `p1_onepole` is concave on a rise and convex on a fall. All surge, decay, loop
  and release segments use it.
- `p1_ease_in` is the new slow-start primitive for the warm-up: a squared progress
  over 0..65536, so the motion starts slowly and accelerates. It is the only new
  maths, and it is the convex rise the warm-up needs.

## Compatibility

The old operator envelope had r1 to r4 and l1 to l3. The migration maps:

- r1 to the surge rate, with the warm-up off, which keeps the old single one-pole
  attack from silence.
- r2 to the decay1 rate and l2 to its level.
- r3 to the decay2 rate and l3 to its level.
- r4 to the release rate.
- mode to sustain on.
- l1 and l4 have no new home and are dropped. The attack peak becomes the operator
  level and the release ends at silence.

The old bipolar envelope had r1 to r4, l1 to l4 and a depth. The migration maps:

- r1 to r3 and l1 to l3 across.
- r4 to the release rate and l4 to the release level.
- mode to sustain on.
- The old depth value is dropped. Absolute levels replace it, so a patch with any
  depth keeps its stage values but loses the depth scaling, which is the recorded
  approximation.
- The pitch envelope keeps its offset levels and depth, so it maps across with the
  new stage count and mode.

The parameter counts change, so `PRESET_VERSION` moves once and an old-layout table
drives a name-based remap in the same migration. Old libraries are remapped, not
rejected.

## Evidence to produce

- Engine tests per family: warm-up and surge shapes, direction-based decay bend,
  the four modes, release to silence or the release level, and a pitch envelope that
  still starts from zero offset.
- A migration test from a version 10 part record, covering the operator and bipolar
  remaps.
- Graph snapshots for the operator page and each bipolar card: the double bars at
  the sustain and loop boundaries, and the left arrow under the loop.
- The unchanged centre render reference for a default patch, which has depth 0 and
  the default mode.

## Limits and open work

- The graphs are drawn from the new mode and stage data; until that lands the
  snapshot set does not describe the new envelope.
- The feedback envelope has no depth today, so its Q15 levels become positions
  without losing a control.
- The web panel needs the same mode and stage display once the engine lands.

## Safe experiment and stop criterion

Play a note with warm-up at 90, surge at 60 and the decay levels high, and listen for
a slow start into a snappy finish. Then set sustain off with a decay2 level of 0 and
listen for the note to release without a key-off. Stop if the warm-up starts fast, if
a falling decay bends the wrong way, or if an old patch changes its attack peak.
