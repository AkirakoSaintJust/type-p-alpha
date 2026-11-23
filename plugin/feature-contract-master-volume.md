# Feature contract: master output volume

## Identity

- **Feature:** A `volume` fader in the header, under the output scope, that trims
  the plugin's stereo output from silence through unity to +12 dB.
- **Brief section:** the header in [`../design/README.md`](../design/README.md) and
  [`../design/juce.md`](../design/juce.md).
- **Owner / date:** P1Onken, 2026-11-23.
- **Disposition:** complete. The Linux suite passes with the fader, the law checks
  and the regenerated snapshots.

## Player outcome

- **New player action:** the output column gains a horizontal fader under the
  scope. Dragging right raises the level, dragging left lowers it, and the far left
  is silent.
- **Sound and behaviour:** the fader is a position from 0 to 1000 with unity at
  750. The bottom three quarters rise logarithmically through 60 dB to 0 dB, and
  the top quarter accelerates in dB to +12 dB, so the last part of the travel is
  the boost. The value reads in dB, and position 0 reads `silent`. At unity the
  output is bit-identical to the render without the fader, because the gain is
  exactly 1.0 and the ramp is skipped.
- **Failure mode:** a drag that zippers, a boost that wraps an int32, a fader that
  moves one channel only, a volume that changes the engine render or the part and
  multi records, or a value that reads `-0.0 dB` at unity.

## Dependencies and interactions

- **Other features:** the processor output stage, the rate bridge, the header, the
  APVTS state and the editor snapshots.
- **Platform/API/HAL changes:** none. The gain runs in float on the host buffer
  after the core render and after the rate bridge, so the engine and its 48 kHz
  parity path are untouched.
- **Persistence/version impact:** none. The parameter is stored in the plugin state
  by the APVTS, and it stays out of the part record, the multi record and the
  library, because a master volume belongs to the instance rather than the patch,
  and the manifest stays at 1,976 parameters with a part block of 981.
- **MIDI/clock impact:** none. The fader is a host parameter with no CC mapping.

## Parameter and event semantics

- `output_volume` is 0 to 1000, default 750, with the `volume` display form, and the
  law is in [`../juce/VolumeLaw.h`](../juce/VolumeLaw.h). Position 0 gives a gain of
  0, positions 1 to 750 give `-60 * (750 - p) / 750` dB, and positions 751 to 1000
  give `12 * ((p - 750) / 250)^2` dB. At 750 the dB value is 0.0 rather than a
  negative zero, so the text reads `0.0 dB`.
- The value text is `silent`, then one decimal place with a sign above 0 dB, and
  typing a value in the host parses the dB text back through the same law.
- `processBlock` reads the position once per block and applies a linear gain ramp
  from the previous block's gain to the new one, so a fast drag moves in the same
  block slices as the rest of the panel. `prepareToPlay` seeds the ramp at the
  current gain so the first block does not fade in.
- The fader is an output trim, so the `output` scope shows the signal before it,
  because a scope that shrank with the fader would hide the patch's own level.
- The fader sits outside the engine's mono global chain. The pan contract records
  that chain's limit: the DC blocker is duplicated for the right channel, and a
  second global audio effect would need a stereo path first.

## Evidence

- `plugin_smoke` passes `the APVTS exposes every manifest parameter and the output
  volume`, `the volume is silent at the floor and unity at 0 dB`, `the volume
  reaches +12 dB at the top` and `the volume text agrees with the law`, and its
  text round-trip check covers the new parameter at 21 normalised points.
- The editor snapshots are regenerated for all eight pages, because the header is
  on every page, and `editor-patch.png` shows the fader under the scope with the
  unity mark whilst `editor-half.png` shows it at the 50 percent scale.
- The fader at 750 applies no gain, so `plugin_smoke`, `plugin_stability` and
  `engine_parity` render the same samples as before, and no render reference
  changed.

## Limits

- **The boost can exceed full scale.** The gain runs after the core, in float, so
  +12 dB raises the host buffer above 0 dBFS and the host or the DAC clips it, which
  is the point of a master trim, and the engine's own saturation is not touched.
- **The scope is pre-fader**, so a silent fader still draws the patch's output.
- **There is no smoothing curve control**, because the ramp is one linear segment
  per block.
- **The fader is not part of a patch**, so loading a multi-patch or a part keeps
  the current master volume.

## Risks and fallback

- The law is duplicated in the header text and the gain, and both read
  `VolumeLaw.h`, so a change to one is a change to both, and the smoke checks pin
  the floor, unity, the ceiling and one text round-trip.
- The fallback is to leave the fader at unity, which is the default and applies no
  gain, so removing the control means dropping the parameter from `createLayout`,
  the header and the smoke count, with no record migration.

## Safe experiment and stop criterion

Set the fader to 750, render a note and compare it with the build before this
change. The samples must match exactly. Then drag to 0 and listen for silence, drag
to 1000 and listen for the boost, and stop if either channel moves alone, if the
ramp clicks, or if the value at unity reads anything other than `0.0 dB`.
