# Feature contract: generated plugin projects

## Identity

- **Feature:** Milestone 7, the generated plugin project and the `plugin-*` CLI.
- **Brief section:** `plugin/README.md` section 11, `docs/roadmap.md` 13.31.
- **Owner / date:** P1Onken, 2026-11-04.
- **Disposition:** accepted.

## Player outcome

- **New player action:** point one command at a synth project and get a plugin
  project that builds against the reusable host.
- **Sound and behaviour:** the plugin and the firmware render the same
  generated C from the same `patch.json`, `panel.json`, and `app.c`, and the
  44.1 kHz core stays bit-exact.
- **Failure mode:** a missing project, an unknown key in `plugin.json`, a
  four-character code that is not four ASCII letters or digits, or a stale
  generated tree stops with a non-zero exit before CMake runs.

## Dependencies and interactions

- **Other features:** `umetate build --synth`, the JUCE host, the publish path.
- **Platform/API/HAL changes:** none in the HAL. The generated project sets
  `P1_SYNTH_GEN`, `P1_SYNTH_CNAME`, `P1_PRODUCT_NAME`, `P1_PLUGIN_TARGET`,
  `P1_BUNDLE_ID`, `P1_MANUFACTURER_CODE`, `P1_PLUGIN_CODE`, and
  `P1_STATE_TREE_ID` before it adds `plugin/` as a subdirectory.
- **Persistence/version impact:** none. The APVTS state tree id is the synth C
  name. The preset record layout is unchanged.
- **MIDI/clock impact:** none.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** unchanged. `Params.h` is
  generated from the project's `params.json`, and every manifest parameter is
  exposed.
- **Note/event ordering and ownership:** unchanged. The engine seam and the
  32-sample control block are the same.
- **Voice allocation and stealing:** unchanged. The voice count comes from the
  patch.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** from the patch. Type P.fm1 keeps 20 voices.
- **Per-sample and per-block operations:** unchanged. The generated project
  compiles the same C with the same flags.
- **RAM, flash, stack, pool:** unchanged on the device. The generated project
  duplicates the synth's rendered C under the git-ignored `build/` tree for the
  host build.
- **Overflow/overload policy:** unchanged.
- **Division, shift, and range analysis:** unchanged.

## Files and interfaces

- **Candidate files:** `umetate/_synthgen.py`, `umetate/plugin_cli.py`,
  `umetate/build.py`, `umetate/publish.py`, `umetate/cli.py`,
  `plugin/CMakeLists.txt`, `plugin/juce/CMakeLists.txt`,
  `plugin/tools/P1Synth.h.in`, `plugin/tools/P1Product.h.in`,
  `templates/plugin/CMakeLists.txt`, `templates/plugin/README.md`,
  `examples/type-p-fm1/plugin.json`.
- **Public contract/header:** the generated `P1Project.cmake` variables,
  `P1Synth.h` with the `P1_STATE`, `P1_RESET`, `P1_SET_PARAM`, and render aliases,
  and `P1Product.h` with the product name and the state tree id.
- **State owner and publication rule:** the plugin processor owns one engine
  state per plugin instance. The firmware owns one static instance.
- **Forbidden dependencies:** no new dependency in the engine, no malloc, float,
  flash, or display in the audio path. The generator stays pure Python.

## Verification

- **Host vector(s):** the new `umetate.test_cli` cases run the generate/check
  round trip, a stale-file failure, the identity override, the four-character
  rule, and the quote rejection, and the plugin CTest suite runs `engine_parity`,
  `rate_bridge`, `preset_smoke`, `plugin_smoke`, `plugin_stability`, the editor
  snapshots, and `linux_daws`.
- **Browser vector:** not applicable. The generator change does not touch the web
  path, and the existing web parity check is unchanged.
- **Target package check:** no firmware change. `umetate build --synth` still
  composes the `.fwsc` and the host WAV.
- **Physical timing/device check:** not available. Recorded as blocked.
- **Tolerance/metric:** the generated files match byte-for-byte on `--check`, and
  the 44.1 kHz render stays bit-exact. A generated project must configure and
  build with its rendered C in `gen/` and only the tracked
  `firmware/gen/math_tables.h` from the source tree.
- **Evidence artefact:** `umetate/plugin_cli.py`, `plugin/feature-contract-generator.md`,
  and the roadmap 13.31 entry.

## Result (2026-11-04)

- `umetate plugin-generate` writes `build/plugin/type_p_fm1/` with `gen/`,
  `P1Project.cmake`, `CMakeLists.txt`, and `README.md`. `--check` passes on the
  fresh tree and fails after a generated file is changed.
- `umetate.test_cli` passes the generate/check round trip, the identity override,
  the four-character rule, and the quote rejection.
- The plain `cmake -S plugin` build configures and links the VST3, CLAP and
  standalone host with JUCE 8.0.9 and the pinned CLAP wrapper, and the ctest
  suite passes `engine_parity`, `rate_bridge`, `preset_smoke`,
  `plugin_smoke`, `plugin_stability`, and both editor snapshot tests.
- `linux_daws` passes Ardour and both Carla renders, and the Carla VST3 and CLAP
  renders are 132300 frames at 44100 Hz, 24-bit stereo, peak 0.609467.
- `publish --plugins` copies `plugin.json` into `src/` and stages one VST3 and one
  CLAP archive, and it ignores `_deps`, hidden folders, and the Linux DAW test's
  isolated HOME.
- The generated project built against the same `_deps` tree and passed all eight
  CTest cases, including `linux_daws`. `plugin-build` defaults to four jobs
  because an unbounded compile can wedge a small Docker VM.
- The physical device pass remains blocked until hardware is available.

## Risks and fallback

- **Assumptions/open research:** a generated project can override the host
  defaults before `add_subdirectory` without changing the checked-in default
  build. CMake 3.22 with CMP0126 NEW keeps the normal variables over the cache
  defaults.
- **Safe experiment and stop criterion:** render the project, configure it, and
  build the engine bridge first. Stop if any generated file differs from the
  firmware render or the 44.1 kHz parity test fails.
- **Smaller alternative:** keep the fixed Type P.fm1 source and add only the CLI
  wrapper, which does not generalise and was rejected.
- **Rollback:** revert the Milestone 7 commit. The firmware generator is the same
  code path, and `umetate build --synth` regenerates the deterministic output.
