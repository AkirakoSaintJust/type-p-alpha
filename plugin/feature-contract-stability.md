# Feature contract: stability sweep

## Identity

- **Feature:** Milestone 6b, the plugin stability and edge-case sweep.
- **Brief section:** `plugin/README.md` §12 rung 11 and §13 milestone 6b.
- **Owner / date:** P1Onken, 2026-11-04.
- **Disposition:** accepted.

## Player outcome

- **New player action:** push every control to its limit, play dense chords,
  change the session rate, and load a damaged state without the plugin going
  silent, hanging, or leaving the DAW with a non-finite sample.
- **Sound and behaviour:** extreme patches may clip, as any synth may. They
  stay finite and the plugin keeps rendering, because a damaged state is rejected
  or repaired, never trusted blindly.
- **Failure mode:** the audio stays finite, and a rejected state leaves the
  previous parameters in place.

## Dependencies and interactions

- **Other features:** the engine bridge, the parameter map, the rate bridge, the
  preset store, and the host wrapper.
- **Platform/API/HAL changes (if any):** none.
- **Persistence/version impact:** none. The test reads and writes the version 2
  state but does not change the format.
- **MIDI/clock impact:** none. The test feeds the existing MIDI bridge.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** every manifest parameter is
  driven to its minimum and its maximum through the host parameter object.
- **Note/event ordering and ownership:** 256 note-ons, then note-offs, then a
  deterministic 9600-event storm of note on, note off, CC, pitch bend, channel
  pressure, and program change, though the engine receives note, CC, and bend
  only.
- **Voice allocation and stealing:** the stress exceeds the 20-voice pool on
  purpose, so the allocator must stay finite and must not crash.
- **Sustain/panic behaviour:** not changed. The storm includes note-offs, and the
  existing panic path is untouched.

## Bounds and resources

- **Maximum voices/events/effects:** the stress sends 256 held notes and 24 events
  per block for 400 blocks.
- **Per-sample and per-block operations:** unchanged by the test.
- **RAM, flash, stack, pool:** the test allocates small standard containers on the
  test thread only, and the audio callback still does no allocation.
- **Overflow/overload policy:** finite output is the gate, and a loud, clipped
  signal is allowed, whilst NaN, infinity, or a hang is a failure.
- **Division, shift, and range analysis:** no new arithmetic in the plugin.

## Files and interfaces

- **Candidate files:** `plugin/tests/plugin_stability.cpp`,
  `plugin/juce/CMakeLists.txt`, `plugin/README.md`.
- **Public contract/header:** none. The test uses the existing
  `TypePFm1AudioProcessor` interface and the generated parameter table.
- **State owner and publication rule:** the processor owns the state. The test
  round-trips it through `getStateInformation` and `setStateInformation`.
- **Forbidden dependencies (malloc, float, flash, display in audio):** the test is
  not in the audio callback, so there is no new audio-thread allocation.

## Verification

- **Host vector(s):** `plugin_stability` runs the full sweep at 44.1, 48, 96, and
  192 kHz, and it checks the reported latency at each rate.
- **Browser vector (if applicable):** none.
- **Target package check:** none. This is a host-side test.
- **Physical timing/device check:** blocked, no hardware.
- **Tolerance/metric:** every rendered sample is finite, the parameter extremes
  stay below an absolute peak of 100.0, and the voice tail after 16 seconds is
  below 0.05.
- **Evidence artefact:** the `plugin_stability` CTest output in the Linux
  container, where the test passes in 0.14 seconds, and every check reports `ok`,
  including the parameter extremes, the polyphony burst, the MIDI storm, the rate
  changes, the state fuzz, and the voice tail.

## Risks and fallback

- **Assumptions/open research:** a finite output does not prove musical
  correctness, though the parity and validator tests still carry that evidence.
- **Safe experiment and stop criterion:** run the sweep on every plugin change,
  and stop on any non-finite sample, crash, hang, or latency mismatch.
- **Smaller alternative:** keep the smoke test and add only the state fuzz and
  rate changes, because the full sweep is cheap enough to keep.
- **Rollback:** remove the `plugin_stability` test target. No production code
  depends on it.
