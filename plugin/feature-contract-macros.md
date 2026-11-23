# Feature contract: patch macros

## Identity

- **Feature:** Six patch-level macros, each a bipolar offset with up to three
  destinations, a MIDI source and a name, where the destination keeps its stored
  value so a macro at 0 renders the patch exactly as saved.
- **Brief section:** [`../design/macros.md`](../design/macros.md), with the research
  in [`../docs/research/macros.md`](../docs/research/macros.md).
- **Owner / date:** P1Onken, 2026-11-23.
- **Disposition:** the 54 parameters, the CC map, the automation flags and the multi
  record are built. The offset layer and the panel are outstanding, so the evidence
  below is partly run and partly a list.

## Player outcome

- **New player action:** a `macros` tab with six cards. Each card carries a bipolar
  knob, a name, a MIDI source with a range, and three destination rows, each with a
  target and a range. The knob is host-automatable, and the source adds around it.
- **Sound and behaviour:** a destination moves by a percentage of its own span and
  clamps at the ends, so it stays there until the macro returns past the point where
  it hit and then returns exactly to the stored value. The source range is bipolar
  and signed, so a negative range inverts the direction. Velocity follows each note,
  which makes the sound per voice and the display newest voice.
- **Failure mode:** a macro at 0 that changes the render, a destination that
  accumulates instead of returning, a group that moves a member whose stored amount
  is 0, a log parameter that jumps in raw units, a name that vanishes when a
  multi-patch is saved, or a source that fights host automation.

## Dependencies and interactions

- **Other features:** the generator and the engine voice path, the parameter
  manifest and the APVTS, the multi record and the library, the generated CC map,
  the editor and its snapshots, and the key follow and velocity cards.
- **Platform/API/HAL changes:** none. The macro values and config stay in the
  engine's global zone, and the offsets are computed in the existing control block.
- **Persistence/version impact:** the six values and the 48 config values are global
  manifest parameters, so the part record stays at 981. The multi record gains 54
  ints and six 32-character names, which is about 414 extra bytes per record, and the
  loader accepts the old multi size because the library header carries the multi
  bank size. The six values are saved in the APVTS state as well, like the routing
  parameters.
- **MIDI/clock impact:** six CCs, appended to the generated map, which lands on CC72
  to CC77 after the current last CC of 71. The mod wheel is CC1 and needs its own
  capture in the bridge. Velocity comes from the note-on, as it already does for the
  follow cards.

## Parameter and event semantics

| Parameter | Range | Default | Automatable |
| --- | --- | --- | --- |
| `macro1` to `macro6` | -100 to 100 | 0 | yes |
| `macroN_midi` | 0 off, 1 cc, 2 wheel, 3 velocity | 1 | no |
| `macroN_midi_range` | -100 to 100 | 0 | no |
| `macroN_target1` to `macroN_target3` | the target enum, 0 is none | 0 | no |
| `macroN_range1` to `macroN_range3` | -100 to 100 | 0 | no |

- The table is 54 parameters. The six values are the only automatable ones, and the
  other 48 use `AudioParameterIntAttributes::withAutomatable(false)`.
- The effective macro is `clamp(knob + source * midi_range, -100, 100)`, where the
  source is 0 to 1. The CC and the wheel are clamped to 0 to 1, and the velocity is
  the note's velocity over 127.
- A destination with a linear parameter is
  `clamp(base + (effective / 100) * (range / 100) * span)`. The base is the stored
  parameter value, never the last effective one.
- A destination with a log parameter, which is every LFO rate, applies the same
  fraction to the normalised knob value and converts back. The raw 0 to 20,000,000
  LFO rate span makes a raw percentage useless.
- A group target applies the same range fraction to every member against that
  member's own span, and every member clamps on its own. The carrier and modulator
  roles come from the algorithm's routing table at runtime, and an empty role group
  does nothing.
- The `velocity depth` group skips a member whose stored amount is 0, and every
  member returns to its stored value whenever the macro returns to 0.
- Two macros on one destination sum their offsets before the clamp. The macro at 0
  contributes 0, so the patch's stored values are the reference.
- The six names live in the multi record as fixed 32-byte fields. An empty name
  draws `macro n`, so a patch only stores a name when the player renames it.
- The panel draws a targeted parameter as its stored value in pink with the effective
  value beside it in smaller green type, and as purple while a macro has a non-zero
  range but sits at 0. The macro knob carries the knob value in its text and a
  dotted green arc to the effective value.

## Evidence

Run on the parameter layer and the multi record:

- `plugin_smoke` passes `the APVTS exposes every manifest parameter and the output
  volume` and `only the six macro values are automatable`, and its text round-trip
  covers the 54 new parameters.
- `preset_smoke` saves a multi with a 54-value macro block and six names and checks
  that both round-trip, then loads a hand-built multi bank with the old record size
  and checks that the macro vector arrives empty, which is how the editor detects a
  record from before the block.
- `engine_parity` and the other 16 tests pass, because the macro node carries
  parameters only and changes no audio.

Still outstanding:

- The parameter layer has no offset, so the macro-at-0 render sweep has not run yet.
  It is the first test for the offset slice.
- A render test sweeps a cutoff macro from 0 to 100 and back and checks that the
  return is bit-identical to the start, and a second test drives a group of envelope
  rates from two voices at different velocities and checks that the rates differ per
  voice.
- The editor snapshots show the macros page with an empty card, a card with three
  targets, the disabled MIDI range, the target menu with its eight section headings,
  the purple stored value at rest and the smaller green effective value while a
  macro deviates.

## Limits

- **The parameter layer and the multi record are built.** The offsets and the panel
  are not.
- **The display follows the newest voice.** A velocity macro differs per voice, and
  the stored value, the effective value and the dotted green arc can only show one of
  them.
- **Pan is patch-level for velocity.** There is one pan per part, so a velocity
  macro on pan would need a note policy. Pan stays out of the follow cards.
- **The per-voice rate and LFO layer is the largest item**, because the envelope
  nodes read their rates from parameters, so a per-voice rate needs a new read in the
  amp, feedback, morph, phase, filter and pitch envelopes before the env rate and LFO
  groups can ship.
- **The macro CC is relative.** A mapped fader sweeps from the knob's centre to the
  range, not across the whole bipolar span. A full sweep means the knob at -100 and
  the range at 100.
- **The macro targets are a curated list.** There is no generic mod matrix, and a
  parameter outside the eight sections cannot be reached.

## Risks and fallback

- The hard requirement is the macro at 0. Any offset that is added before the clamp,
  or a base read from the last effective value, breaks it, so the parity test is the
  first thing to write.
- The per-voice rate work gates the env rate and LFO groups. The fallback is to ship
  the six macros with the CC, wheel and automation sources and the level, feedback,
  morph, breakpoint, pan and detune targets first, then add velocity and the rate
  groups as a second milestone. The storage and the menu are ready for the second
  half either way.
- The name block changes the multi record. A reader that rejects an unknown size
  without the size from the library header would drop old multis, so the migration
  gets its own test.

## Safe experiment and stop criterion

Set a macro to target cutoff with a 50 percent range, render a held note at 0,
sweep the macro to 100 and back to 0, and compare the first and last renders. They
must be bit-identical, and the macro at 0 must match the build before the feature.
Stop if the 0 render moves by one sample, if the return does not restore the start,
or if a second voice at a different velocity leaves the first voice's rate unchanged
when the target is a rate group.
