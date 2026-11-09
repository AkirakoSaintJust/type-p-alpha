# Feature contract: JUCE host skeleton

## Identity

- **Feature:** Milestone 2, the JUCE VST3 and standalone host skeleton.
- **Brief section:** `plugin/README.md` sections 4, 6, 7, 8, 10 and 13.
- **Owner / date:** P1Onken, 2026-11-03.
- **Disposition:** accepted.

## Player outcome

- **New player action:** load the Type P.fm1 engine as a VST3 instrument or a
  standalone app, play it from MIDI, automate the manifest parameters and save
  the session state.
- **Sound and behaviour:** the 44.1 kHz core renders bit-exactly with the
  firmware engine. Both parts stay addressable, and the device MIDI behaviour is
  reused.
- **Failure mode:** the smoke test fails on a parameter count, id, render or
  state mismatch, and a host that asks for an unsupported layout gets
  no bus. No code path writes files, allocates or locks on the audio thread.

## Dependencies and interactions

- **Other features:** the instance-safe generated engine, the engine bridge, the
  MIDI bridge, the parameter manifest and the rate bridge.
- **Platform/API/HAL changes:** a new host-native CMake path guarded by
  `P1_BUILD_JUCE`, whilst the firmware gate and the Docker target build are
  unchanged.
- **Persistence/version impact:** the plugin state is an APVTS XML blob with a
  `stateVersion` property. It stores the 303 parameters. The device preset banks
  and their byte layout are untouched.
- **MIDI/clock impact:** notes, CC, sustain, panic and bend use `synth_midi`,
  whilst clock and transport are ignored.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** generated from
  `firmware/gen/params.json` into `Params.h`. The APVTS key is the stable
  manifest name, for example `algorithm.p0`. The engine id is the manifest
  index, 0 to 308. The APVTS exposes the integer range and default, and the curve,
  unit and display fields are carried for the Milestone 3 editor, whilst the host
  automation lane uses the integer range in this milestone.
- **Note/event ordering and ownership:** the processor owns one engine and one
  MIDI bridge. Host events apply at the 32-sample control block that contains
  their 44.1 kHz sample position.
- **Voice allocation and stealing:** unchanged, through the generated seam.
- **Sustain/panic behaviour:** unchanged, and sustain state remains per bridge.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged, 20 voices.
- **Per-sample and per-block operations:** at 44.1 kHz the processor reads the
  APVTS and applies MIDI once per 32-frame control block, then calls the
  generated render for that block, and the conversion is one multiply per
  sample.
- **RAM, flash, stack, pool:** one engine state, one MIDI context, one APVTS and
  one 64-sample int32 scratch buffer per plugin instance. The rate bridge owns
  one small FIFO, so no target RAM is used.
- **Overflow/overload policy:** unchanged.
- **Division, shift, and range analysis:** the core arithmetic is unchanged. The
  float conversion is a power-of-two scale.

## Files and interfaces

- **Candidate files:** `plugin/CMakeLists.txt`, `plugin/juce/CMakeLists.txt`,
  `plugin/juce/PluginProcessor.h`, `plugin/juce/PluginProcessor.cpp`,
  `plugin/tools/gen_params.py`, `plugin/tests/plugin_smoke.cpp`,
  `plugin/engine/RateBridge.hpp`, `plugin/engine/RateBridge.cpp`.
- **Public contract/header:** the JUCE processor `TypePFm1AudioProcessor`,
  generated `Params.h` with `P1ParamSpec` and `kP1ParamSpecs`, and the
  `parameters()` accessor.
- **State owner and publication rule:** the processor owns the engine, the MIDI
  bridge and the APVTS, whilst the audio thread reads the APVTS atomic raw values
  and never takes a lock. `setStateInformation` replaces the APVTS and sets a
  dirty flag, and the audio thread resynchronises at its next block.
- **Forbidden dependencies:** no audio input, no sidechain, no file I/O on the
  audio thread, no CLAP or AU target in this milestone.

## Verification

- **Host vector(s):** `plugin_smoke` instantiates the processor, checks 303
  parameters with unique ids and contiguous engine ids, renders a note at
  44.1 kHz and compares every float with the engine reference, writes an
  automation value and compares the next render with the changed parameter,
  sends a note at sample 100 and compares it with a reference note at the
  containing control block, saves and restores the state, and renders the 48 kHz
  placeholder path.
- **Browser vector:** unchanged, the Milestone 0 web parity remains green.
- **Target package check:** unchanged, `deep-check --all` still builds the
  target package.
- **Physical timing/device check:** blocked, no hardware.
- **Tolerance/metric:** bit-exact floats at 44.1 kHz. Parameter state exact.
- **Evidence artefact:** `ctest` output for `engine_parity` and `plugin_smoke`,
  and the VST3 and standalone artefacts.

## Result (2026-11-03)

- `P1_BUILD_JUCE=ON` builds a VST3 and a standalone target against JUCE 8.0.9.
- The VST3 and standalone artefacts are produced under
  `build/plugin-juce/juce/type_p_fm1_artefacts/Release`.
- The APVTS exposes 303 parameters. The ids are the manifest names and are
  unique, and the engine ids cover 0 to 308 with none missing.
- The 44.1 kHz path is bit-exact with the engine reference on a note render.
- An automation write to `algorithm.p0` changes the next render exactly at the
  first 32-sample control block.
- A note at sample 100 applies at the control block that starts at sample 96,
  matching the reference render.
- A state save and restore puts `algorithm.p0` back to its default.
- The 48 kHz placeholder path produces finite, non-silent audio.
- `ctest` passes `engine_parity` and `plugin_smoke`.
- The Linux build used Debian 13, GCC 14.2 and CMake 3.31 in the existing host
  container, and the firmware Docker gate is not affected.
- Milestone 2 used JUCE's generic editor, which Milestone 3 replaces and records
  in `plugin/feature-contract-editor.md`.
- The rate bridge is still the temporary linear placeholder, so release-quality
  resampling is Milestone 5.
- Physical device acceptance and timing remain blocked.

## Risks and fallback

- **Assumptions/open research:** the host calls `processBlock` with at most the
  block size passed to `prepareToPlay`, and the generated engine handles any frame
  count and always chunks internally at 32 frames.
- **Safe experiment and stop criterion:** build the JUCE targets and run the
  smoke test first, and stop if the parameter count or a 44.1 kHz sample differs.
- **Smaller alternative:** keep the JUCE path behind `P1_BUILD_JUCE` and ship
  only the JUCE-free `p1_engine` gate until the editor lands, which is the current
  default.
- **Rollback:** disable `P1_BUILD_JUCE`, and the firmware gate and the bridge test
  do not depend on JUCE.
