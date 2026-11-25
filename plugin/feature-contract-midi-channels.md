# Feature contract: MIDI channels and bend

## Identity

- **Feature:** Carry a MIDI channel through note-off, sustain and pitch bend, route
  each to the parts that listen on that channel, and give each part a bend range.
- **Brief section:** README line 28, where parts can be assigned to different MIDI
  channels, and the part settings page in [`../design/README.md`](../design/README.md).
- **Owner / date:** P1Onken, 2026-11-19.
- **Disposition:** complete. The engine and preset tests pass in the Linux build
  image, and the CI run is still to follow. The controller map stays global, as
  recorded under Limits.

## Player outcome

- **New player action:** two parts can share a patch and still behave as two
  instruments, because a note-off on one channel no longer releases the other
  part's note. The pedal holds the channel it was pressed on. The part settings
  page gains a `bend` knob, 0 to 24 semitones, defaulting to 2.
- **Sound and behaviour:** pitch bend now moves the parts on its channel, with the
  part's range setting the span. A bend on channel 1 leaves a part on channel 0 at
  centre. The wheel is applied on the control tick, so a held note glides.
- **Failure mode:** a note-off on channel 0 releasing part 2, a pedal on one channel
  holding the other, a bend moving both parts, or a bend that steps only when the
  next note starts.

## Dependencies and interactions

- **Other features:** the engine generator, the MIDI handler, the plugin bridge, the
  parameter manifest, the part record and the editor's part card.
- **Platform/API/HAL changes:** the engine seam gains
  `note_off_ch(note, channel)` and `set_bend_ch(value14, channel)`. The omni entry
  points remain and pass `0xFFFFFFFF`.
- **Persistence/version impact:** the manifest moves from 1,614 to 1,616 parameters
  and the part block from 800 to 801, because `bend_range` is the last mode
  parameter. `PRESET_VERSION` moves from 7 to 8 and the plugin state version from 5
  to 6. State version 5 is the cutoff-law version, so the cutoff conversion no
  longer keys off `kStateVersion` and cannot run twice.
- **MIDI/clock impact:** note-off, pedal and CC 123 are channel-aware. The mapped
  controllers stay global.

## Parameter and event semantics

- `bend_range.p0` and `bend_range.p1` are 0 to 24 semitones, default 2, per part,
  stored in the part record with the rest of the mode block.
- A note-on routes by the part channel and key zone. A note-off now routes by the
  part channel as well, so a part only releases voices it started.
- Sustain is a per-channel flag with a per-channel deferred-note table. CC 123
  releases the notes and the pedal on its own channel. The global panic still
  stops every part.
- The plugin's same-chunk note tracking keys on channel and pitch, so a note-on on
  one channel does not hold a same-pitch release on another. The plugin defers the
  release only when that channel started the pitch in the same chunk.
- The 14-bit wheel is converted per part: `cents = (value - 8192) * range * 100 /
  8192`, then `p1_tune_q16`. Centre is 8192.

## Evidence

- `engine_parity` passes `both parts sound the same note on different channels`, `a
  note-off on one channel leaves the other part sounding`, `the second note-off
  releases its own part`, `sustain holds its own channel and not the other`,
  `lifting the pedal releases the notes it held`, the four bend routing checks, and
  the engine render reference, which is unchanged at centre bend.
- `plugin_smoke` passes `the chunk note set keeps the channel and pitch`, `a note on
  another channel does not hold the pitch`, `a different pitch is not held`, `the
  last channel and pitch are both tracked` and `the chunk note set clears each
  block`.
- `engine_parity` passes `CC 123 releases only the part on its channel`, `CC 123
  on the second channel releases the second part` and `the global panic releases
  every part`.
- `preset_smoke` passes the old-record expansion, where the range defaults to 2 and
  a current-law cutoff is not converted twice.
- The Linux image builds the plugin and runs 14 of 14 tests, including the editor
  snapshots with the new knob.

## Limits

- **The controller map is global.** A controller message sets the mapped parameter
  whatever channel it arrives on. Notes, pedal, CC 123 and bend are channel-aware.
  The mapped controllers are not.
- **The wheel updates on the control tick.** A 32-sample step at 48 kHz is fast
  enough to hear as a glide, and it is not sample-accurate.
- **A part on omni (`-1`) receives every channel.** An omni note-off releases every
  part, which is what keeps the single-part and omni renders exact.
- **The range is a mode parameter.** It travels in the part patch rather than the
  multi patch, so two parts of one multi can carry different ranges.

## Risks and fallback

- A channel change during a held note leaves the voice on the old part until its
  note-off. The part channel is read when the message arrives.
- The fallback is the omni path: restoring the old note-off and one sustain flag
  returns the previous behaviour without touching the record layout. The preset
  version would stay at 8 because the extra byte is harmless.

## Safe experiment and stop criterion

Send the same note on two channels with different parts, release one, and listen for
the other. Stop if the other part drops, if the pedal crosses channels, or if the
centre-bend render differs from the reference by one sample.
