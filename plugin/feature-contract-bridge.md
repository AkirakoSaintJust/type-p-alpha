# Feature contract: engine bridge and MIDI bridge

## Identity

- **Feature:** Milestone 1, JUCE-free engine bridge and MIDI bridge.
- **Brief section:** `plugin/README.md` sections 6 and 8, Milestone 1.
- **Owner / date:** P1Onken, 2026-11-03.
- **Disposition:** accepted.

## Player outcome

- **New player action:** the future plugin host can drive the generated engine and
  the device MIDI behaviour without the firmware app, the panel or the screen.
- **Sound and behaviour:** identical to the firmware at 44.1 kHz for the same
  parameters, notes and MIDI messages.
- **Failure mode:** the parity harness fails if the engine,
  parameter state or rendered samples differ, and the firmware app is unchanged.

## Dependencies and interactions

- **Other features:** Milestone 0 instance state, the generated parameter
  manifest, the device preset store, the JUCE host.
- **Platform/API/HAL changes:** `synth_midi` gains a per-context sustain state and
  a `synth_midi_feed` entry point, and the generated `dsp.h` gains C++ linkage
  guards.
- **Persistence/version impact:** none.
- **MIDI/clock impact:** note, CC, sustain, panic and bend keep the device
  semantics, whilst clock and transport stay with the app.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** from the generated manifest,
  unchanged.
- **Note/event ordering and ownership:** the caller feeds MIDI messages in order,
  and the bridge applies them to one engine state.
- **Voice allocation and stealing:** unchanged, through the generated seam.
- **Sustain/panic behaviour:** unchanged, but the sustain state is per MIDI
  context instead of a file-scope static.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged, 20 voices.
- **Per-sample and per-block operations:** the bridge adds one function call per
  block and no per-sample work beyond the engine.
- **RAM, flash, stack, pool:** one engine state per host instance. The MIDI
  context adds 129 bytes per instance.
- **Overflow/overload policy:** unchanged.
- **Division, shift, and range analysis:** unchanged.

## Files and interfaces

- **Candidate files:** `firmware/src/synth_midi.c`, `firmware/src/synth_midi.h`,
  `firmware/tests/midi_test.c`, `umetate/generate.py` for the C++ guards,
  `plugin/engine/Engine.hpp`, `plugin/engine/Engine.cpp`,
  `plugin/engine/MidiBridge.hpp`, `plugin/engine/MidiBridge.cpp`,
  `plugin/tests/engine_parity.cpp`, `plugin/CMakeLists.txt`.
- **Public contract/header:** `Engine` with `reset`, `setParam`, `getParam`,
  `noteOn`, `noteOff`, `allNotesOff`, `setBend`, `render`, `lit`, `quiet`, `tap`.
  `MidiBridge::feed` and `reset`.
- **State owner and publication rule:** one `type_p_fm1_state` per `Engine`, one
  `synth_midi_ctx` per `MidiBridge`, and the generated C uses a thread-local
  current state pointer in the plugin build.
- **Forbidden dependencies:** no JUCE yet, no malloc on the audio thread, no
  locks in the render path.

## Verification

- **Host vector(s):** engine render against the legacy firmware API, sample for
  sample at 44.1 kHz, whilst the MIDI script runs through `synth_midi_poll` as the
  reference and through the bridge, comparing all parameters, note lights, quiet
  state and rendered samples.
- **Browser vector:** the Milestone 0 web parity remains green.
- **Target package check:** the full `deep-check --all` target build remains green.
- **Physical timing/device check:** blocked, no hardware.
- **Tolerance/metric:** bit-exact int32 samples at 44.1 kHz. MIDI state and params
  exact.
- **Evidence artefact:** the `engine_parity` test output and the full gate.

## Result (2026-11-03)

- `Engine` wraps the generated instance API and renders sample-for-sample
  identical to the legacy firmware API, including the release tail, and two
  `Engine` instances stay isolated.
- A MIDI script through `synth_midi_poll` and the same script through
  `MidiBridge` produce the same 303 parameters, note state and rendered samples.
- Sustain state is per MIDI bridge, so one instance's pedal does not affect
  another.
- `engine_parity` passes under CMake and CTest, and `umetate deep-check --all`
  remains green, including the target package and web parity, whilst physical
  device acceptance and timing stay blocked.

## Risks and fallback

- **Assumptions/open research:** the C function pointer types and C++ linkage work
  on the target host compilers, and the MIDI feed remains synchronous.
- **Safe experiment and stop criterion:** build the bridge and the parity
  test first, and stop if any sample or parameter differs.
- **Smaller alternative:** keep the MIDI parser in the firmware app and let the
  plugin feed raw MIDI through a stub `midi_get`, which keeps the app coupling.
- **Rollback:** revert the bridge commit. The firmware `synth_midi` change is
  independent and can be reverted separately.
