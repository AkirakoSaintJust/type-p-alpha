# 0.1.0 reliability decisions

The interview was on 2026-11-19, after the audit of the MIDI path, the spectral
display and the preset library. The code references are against `092045a`. The
answers are the author's. The order at the end is my proposal, because the
interview settled the scope and left the sequencing open.

## The bar

0.1.0 waits for all four items in the audit: channel identity, the MIDI timing
grid, the display snapshot and the library revision. The release does not ship
with one of them recorded as a known bug.

## Timing

The core 32-sample grid is the timeline at every host rate: 44.1, 48, 96 and
192 kHz. Host events are mapped into core ticks and dispatched per core tick, so
44.1 kHz no longer collapses a whole host block onto the first sample.

The same-tick pair is the one exception to plain dispatch. A note-on goes to the
engine before the tick renders, and the matching note-off goes after that tick, so
the note sounds for 32 samples and then releases. An ordinary note-off keeps its
own tick.

Recorded limit: an event can sit up to 31 samples from its host position, which is
0.65 ms at 48 kHz, and a note shorter than a tick becomes a click rather than a
note. The partial-block engine work is out of scope.

`RateBridge::process` already calls back after each core chunk, so the first tick
is fed before the call and the rest from the callback, with the core position
carried between blocks in the bridge.

## Channels

Notes, sustain and pitch bend carry channel identity and route per part. Note-off
gains a channel, sustain and the pending notes are tracked per channel, and bend
applies to the parts on that channel.

CC stays a global mapping in 0.1.0: a controller message sets the mapped parameter
whatever channel it arrives on. That is recorded as a limit in the contract and the
manual.

Omni stays as it is today. A part set to -1 receives every channel, and an omni
note-off releases both parts. That keeps the existing parity renders exact.

The fix lives in the umetate generator, because `engine/gen` is generated and
says not to edit it. The regenerated engine and the plugin-side bridge land in one
pass, with the fixtures updated rather than weakened.

## Bend

Each part gets a range parameter from 0 to 24 semitones, defaulting to 2 to match
the assumption the parser already makes. The bend is applied per control tick, so
held notes glide instead of stepping only on new notes.

The control sits on the part settings page next to tune and transpose, which
changes that page's layout, the design note and the snapshot goldens.

## Format

One record and state version bump carries both the bend range and the library
revision. Old banks and projects migrate: the range defaults to 2, and a file
without a revision is treated as revision zero.

## Library

The shared library gains a revision id. Each instance records the revision it
loaded, reloads before a save, and writes only the slots it changed. When the same
slot changed on disk, the disk version stays and the editor reports the slot and
its name. Nothing is lost and the rule is one sentence.

A project never writes the shared library. It may still carry a copy for
portability, and restoring that copy is an explicit action that warns first.

The write is already atomic through `juce::File::replaceWithData`, so that part of
the audit recommendation is already in place and stays.

## Display

The eight live morph values, four operators across two parts, are published from
the audio thread as per-value atomics. The editor reads the published values and
never scans the voice arrays. No mutex goes near the render state.

## Evidence

Two new contracts:

- `plugin/feature-contract-midi-channels.md`: channel routing, sustain per
  channel, bend and range, CC recorded as global.
- `plugin/feature-contract-library-revision.md`: revision, merge, conflict report
  and the project separation.

Two extended contracts:

- `plugin/feature-contract-48k-core.md`: the timing grid at every rate and the
  same-tick policy.
- `plugin/feature-contract-spectral.md`: the published live morph values.

Tests:

- `plugin_smoke` gains four-rate cases: events on tick boundaries, a note that
  crosses a tick, and a same-tick pair that must sound for one tick.
- An engine test beside `engine_parity` covers two parts on different channels at
  the same note, sustain on one channel, and bend on one part.
- `preset_smoke` gains two stores over one temporary file: independent slot saves,
  a conflicting slot, and a project copy that never reaches the file.
- The display test reads the published values after a render and checks the
  editor's value.

CI gains a Linux ThreadSanitizer job that builds and runs the engine, preset, MIDI
and processor tests, not pluginval. The macOS job sets the wall clock at about 29
minutes, so a job of this size does not lengthen the run.

## Proposed order

1. The format change and its migration, because both the bend range and the
   library revision need it.
2. Library separation, revision and merge. Data safety first.
3. Channel identity and bend in the generator, with the regenerated fixtures.
4. The timing grid, the same-tick policy and the display snapshot, which share the
   processor and bridge.

## Risks

- Regenerating the engine moves the fixtures. Parity has to stay exact for omni
  renders and centre bend, or the change has altered an existing sound.
- ThreadSanitizer may report JUCE internals. The job is scoped to the test targets
  for that reason.
- The bend adds per-part work on every control tick. It is small, and it gets
  measured rather than assumed.
- The part settings page change needs the control map, the design note and the
  goldens updated together.

## Still open

The release text. The README currently says parts can be assigned to different
MIDI channels, which is true only after step 3, and it does not mention bend. The
wording change belongs with the release, not before it.
