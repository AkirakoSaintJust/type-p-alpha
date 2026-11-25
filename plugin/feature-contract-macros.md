# Feature contract: patch macros

## Identity

- **Feature:** Six patch-level macros, each a bipolar offset with up to three
  destinations, a MIDI source and a name, where the destination keeps its stored
  value so a macro at 0 renders the patch exactly as saved.
- **Brief section:** [`../design/macros.md`](../design/macros.md), with the research
  in [`../docs/research/macros.md`](../docs/research/macros.md).
- **Owner / date:** P1Onken, 2026-11-23.
- **Disposition:** the 54 parameters, the CC map, the automation flags, the multi
  record, the target table, the linear and log destination offsets, the relative CC,
  wheel and velocity sources, the per-voice velocity source on the operator levels,
  feedbacks, morphs, envelope rates, LFO rates and LFO depths for the all-operator,
  carrier and modulator rows, the macros page and the target-parameter feedback are
  built. The rest of the per-voice layer is outstanding, so the evidence below is
  partly run and partly a list.

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
  manifest parameters, so this feature does not change the part record. The
  current part block is 994 (981 at this slice; version 18 later appended the three
  key-follow rows). The multi record gains 54
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
- The source law is built. The six values are no longer in the generated CC map, and
  the processor captures CC72 to CC77, CC1 and the note-on velocity as live values.
  The source select and range are per macro, and a source at rest leaves the macro at
  the knob. The velocity source uses the newest note until the per-voice layer lands.
- A destination with a linear parameter is
  `clamp(base + (effective / 100) * (range / 100) * span)`. The base is the stored
  parameter value, never the last effective one.
- A destination with a log parameter, which is every LFO rate, applies the same
  fraction to the parameter's normalised range. The hz parameters carry a custom range
  where 0 stays the off stop and 0.01 Hz to 223 Hz is logarithmic, so a 4 Hz rate
  sits near the middle of the knob and the macro offset is musical. The portamento
  keeps its linear range because it is not a macro target.
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

Run on the parameter layer, the multi record, the target table and the offset layer:

- `plugin_smoke` passes `the APVTS exposes every manifest parameter and the output
  volume`, `only the six macro values are automatable`, `the macro target table has
  58 rows` and `the target enum matches the table`, and its text round-trip covers
  the 54 new parameters.
- The target table is generated from `synth/macros.json` against the manifest names,
  so a renamed member fails the build, and its first row is `part 1 filter cutoff`
  with the last `part 2 modulator breakpoint xs`.
- `preset_smoke` saves a multi with a 54-value macro block and six names and checks
  that both round-trip, then loads a hand-built multi bank with the old record size
  and checks that the macro vector arrives empty, which is how the editor detects a
  record from before the block.
- `plugin_smoke` renders the first block on a fresh processor with a macro at 0, a
  target and a 50 percent range, and matches the engine reference bit for bit. The
  same setup at 50 changes the render, so the offset reaches the destination.
- `plugin_smoke` also pins the offset law (`50` and a `50` range give `0.25`, and the
  target clamps at 0 and 1) and checks that the first target resolves to the part 1
  cutoff and the last to the four modulator breakpoint members.
- `plugin_smoke` passes `the LFO rate is logarithmic on the knob`, `the LFO rate
  keeps zero as the off stop` and `the part 1 pitch LFO rate row is log`. The state
  restore check allows a four-step raw tolerance for the log rate floats, because the
  normalised state cannot hold every raw step of a 20,000,000 span.
- The nine editor snapshots with an LFO rate knob are regenerated. At 4 to 5 Hz the
  rate knobs now sit near the middle instead of the bottom two percent.
- `plugin_smoke` passes `the source adds its range around the knob`, `a cc source at
  rest leaves the macro at the knob`, `the cc source moves the macro`, `the wheel
  source moves the macro` and `the velocity source moves the macro`. The last three
  render a fresh first block with the macro knob at 0 and the source at 127 or 100,
  and each differs from the engine reference.
- `engine_parity` checks the per-voice operator level helper with two velocities,
  `20` and `120`, and the hard note's level offset is larger than the soft note's. It
  checks the feedback, morph, envelope rate, LFO rate and LFO depth helpers the same
  way. It also checks a carrier level row against algorithm 4: the carrier operator
  moves and the modulator operator stays at 0. The velocity source render therefore
  reaches every engine path and both roles.
- `plugin_smoke` renders the first block with a feedback, envelope rate or LFO depth
  target and a macro at 0, and each matches the engine reference bit for bit. The same
  targets with the velocity source move the render, so their offsets reach the engine.
  A carrier level target also moves the render. The LFO rate pair sets the tremolo
  depth; the at-rest render matches the no-macro render bit for bit and a velocity of
  100 moves it. A spectrum-mode morph pair checks the morph path the same way.
- `plugin_smoke` checks the generated engine rate sets: the envelope set takes the
  amplitude, feedback and spectral envelope rates and leaves the phase envelope rates
  out, the LFO rate set takes the per-voice LFO rates and leaves the phase LFO rate
  out, and the LFO depth set takes the per-voice depths and leaves the phase LFO depth
  out.
- The editor snapshots gain `editor-macros.png` for the empty cards,
  `editor-macros-targets.png` for the renamed cards with three destinations and a
  green or purple range in each row, and `editor-macros-live.png` for a mod wheel at
  100, where the dotted green arc leaves the knob value on the first card.
  `editor-part-params-macros.png` shows the cutoff as `900Hz 2.0k` and the resonance
  as `38% 33` in green beside the stored value, with the pitch LFO depth in purple
  while its macro sits at 0.
- `plugin_smoke` passes `a targeted parameter reports its effective value`, `a
  targeted parameter at rest reports no effective value` and `a parameter outside the
  table is not targeted`. The state restore check also returns the macro name.
- The remaining goldens are regenerated because the macro demo now resets after the
  macros page, so the later pages show the neutral patch rather than the demo's
  offsets. Below the header, `editor-part-params.png` is pixel-identical to the
  previous golden.
- `plugin_smoke` passes the tab restore checks for the new layout: the six-tab mod
  page moves to 8, the layout-3 spectra b page to 7, the layout-4 one to 6 and the
  four-tab fx page to 2, so an old project opens on the page it was saved on.
- `engine_parity` and the other 17 tests pass.
- The Windows build from the final head passes its 17 tests. `pluginval` 1.0.4 at
  strictness level 5 with the GUI tests skipped reports `SUCCESS` on the VST3. The
  release package `Type-P.alpha-windows-x86_64.zip` (20,199,823 bytes, SHA-256
  `399792109BBF7C7731B20DA774ABF88DB92261227AF5BB40D11B64044A3CE14E`) carries the
  VST3 (15,797,760 bytes, `998B8168E689547AF59E64ACBF766E5CCDF410D37996680502E1452097535987`),
  the CLAP (15,776,768 bytes,
  `B3E5D731168B4A1EEDE2C5AB7F9E9EE4574D66AFEE3BB532DB543DB48F150B59`), the standalone
  (16,147,456 bytes, `F0DA9FB19BFDB11C5CFCD221BF0D7BC724E96EC6F36C1DDF8C02E915C26CA73F`)
  and the licences. The CLAP validator runs in the macOS CI.

Still outstanding:

- The rest of the per-voice layer: the breakpoints and the pan note policy. The all,
  carrier and modulator rows of the level, feedback, morph, envelope rate, LFO rate and
  LFO depth groups are per voice. The phase envelope and phase LFO stay part-level
  because they hold one state per part, as do the phase map breakpoints and the pan.
- A snapshot of the target menu, because the component snapshot does not capture an
  open popup. The eight headings come from the generated table, which the table test
  pins.

## Limits

- **The parameter layer, the multi record, the target table, the offsets, the
  relative sources, the per-voice operator levels, feedbacks, morphs, envelope rates,
  LFO rates and LFO depths for the all-operator, carrier and modulator rows, the macros
  page and the feedback are built.** The rest of the per-voice layer is not.
- **The velocity source is per voice on the groups and rows above and newest-note on
  the rest.** The other targets move the whole part with the last note's velocity until
  their per-voice paths land. The feedback beside a target reads the same newest-note
  velocity.
- **The source values are live, not patch state.** The source select and range are
  saved with the patch, whilst the CC, wheel and note velocity arrive at runtime and
  start at rest.
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
