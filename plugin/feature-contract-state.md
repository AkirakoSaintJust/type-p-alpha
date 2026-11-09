# Feature contract: instance-safe engine state

## Identity

- **Feature:** Milestone 0, instance-safe generated engine state.
- **Brief section:** `plugin/README.md` section 5, `docs/roadmap.md` 13.17.
- **Owner / date:** P1Onken, 2026-11-03.
- **Disposition:** accepted.

## Player outcome

- **New player action:** load two or more plugin instances in one DAW project
  and have each keep its own voices, parameters and preset state.
- **Sound and behaviour:** identical to the firmware at 44.1 kHz, whilst the
  firmware still runs one engine instance and keeps the current API.
- **Failure mode:** if state isolation fails, the two-instance test fails before
  any plugin work, and the firmware build fails closed if the
  generated struct or the legacy wrappers do not compile.

## Dependencies and interactions

- **Other features:** the JUCE host, the web lab, the host DSP tests, the firmware
  glue and the app init path.
- **Platform/API/HAL changes:** none in the HAL. The generated `dsp.h` gains an
  opaque-to-the-app state type and an instance API, whilst the firmware glue keeps
  the current calls through wrappers.
- **Persistence/version impact:** none. Parameter storage moves into the state
  struct, byte layout of presets is unchanged.
- **MIDI/clock impact:** none.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** unchanged. The state init sets
  every parameter to the value in the generated `dsp_params` table.
- **Note/event ordering and ownership:** unchanged. Note and parameter calls take
  a state pointer, and the generation keeps the same voice allocation and control
  block order.
- **Voice allocation and stealing:** unchanged.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged, 20 voices for type-p-fm1.
- **Per-sample and per-block operations:** unchanged. The state access uses a
  pointer indirection that the compiler resolves to a base register.
- **RAM, flash, stack, pool:** the state moves from 332 separate statics into one
  struct, and the struct is sorted by alignment to keep padding low. The target
  state size must stay within the current CPU1 budget. Pool nodes are not used by
  type-p-fm1, and a pool pointer field is reserved for a later patch that uses
  `delayN` or `comb`.
- **Overflow/overload policy:** unchanged.
- **Division, shift, and range analysis:** unchanged.

## Files and interfaces

- **Candidate files:** `umetate/_backend.py`, `umetate/_voice.py`,
  `umetate/generate.py`, the generated `gen/<name>_dsp.c` and `gen/dsp.h`,
  `firmware/gen/glue.h` (if the wrapper needs it), the host and web tests.
- **Public contract/header:** `gen/dsp.h`. New instance API:
  `<name>_dsp_state_init`, `<name>_dsp_reset_s`, `<name>_dsp_note_on_s`,
  `<name>_dsp_note_on_ch_s`, `<name>_dsp_note_off_s`,
  `<name>_dsp_all_notes_off_s`, `<name>_dsp_set_bend_q16_s`,
  `<name>_dsp_lit_s`, `<name>_dsp_quiet_s`, `<name>_dsp_set_param_s`,
  `<name>_dsp_get_param_s`, `<name>_dsp_render_s`, `<name>_dsp_tap_s`, and the
  legacy no-state functions remain for the firmware, the web lab and the
  current tests.
- **State owner and publication rule:** the caller owns the state. The firmware
  owns one static instance. The plugin owns one instance per plugin instance. The
  generated C uses a `UMETATE_STATE_PTR` accessor, empty for firmware and
  `_Thread_local` for the plugin build.
- **Forbidden dependencies:** no malloc, no float, no flash, no display in the
  audio path. No new dependency in the engine.

## Verification

- **Host vector(s):** the existing host DSP and generated-engine tests, plus a new
  two-instance test that renders two different patches and compares each with a
  single-instance run.
- **Browser vector:** the existing web parity check, unchanged behaviour.
- **Target package check:** the target build compiles and the state stays within
  the CPU1 budget.
- **Physical timing/device check:** not available. Recorded as blocked.
- **Tolerance/metric:** bit-exact at 44.1 kHz. State size and instructions per
  sample within 2 percent of the current generated engine.
- **Evidence artefact:** the host test output, the two-instance output files and
  the `umetate build` size report.

## Result (2026-11-03)

- Generated engine exposes `type_p_fm1_state` and the `_s` API.
- Legacy API wraps one static instance, so the firmware, the web lab and the
  existing tests keep their calls.
- The old and new host renders of the type-p-fm1 script are byte-identical
  (457772 byte WAV, `cmp` clean), and two states in one process, one with a
  different algorithm and note, stay isolated over four interleaved blocks.
- The full generator suite passes, including the sound, algorithm, mode, part,
  tap and ASan checks, and the full `umetate deep-check --all` gate passes, as
  does the web parity gate, host and WASM exact on 512 int32 samples.
- Host A/B with the same compiler and the same workload: the old generator at
  `4ba97fe` renders `render(256)` in 120.7 us and the new one in 115.1 us, about
  5 percent faster, and the container perf guard reports 104.5 us against a 400 us
  budget.
- The target package builds with the Docker/JieLi toolchain, and the performance
  profile reports 215544 B of 581564 for the image, RAM through `.bss` 96908 B of
  98304, pool 105536 B of 344064, and 18860 B of `.dsp_text` in SRAM.
- Physical device acceptance and timing remain blocked. They are recorded as
  blocked, not as a pass.

## Risks and fallback

- **Assumptions/open research:** the generator can prefix state access with one
  pointer without changing the arithmetic, and the macro approach keeps the node
  templates untouched.
- **Safe experiment and stop criterion:** generate the struct and the macros,
  compile the existing host tests, then add the two-instance test, and stop if any
  sample differs or the target state does not fit.
- **Smaller alternative:** compile a plugin-only generated variant with the state
  struct, leaving the firmware code untouched. This doubles the emitted engine
  and is a fallback only.
- **Rollback:** revert the generator commit, because the firmware generated files
  are regenerated from the same patch, so the rollback is deterministic.
