# Build and tests

The public build uses CMake and Ninja. The engine is generated C, the host is
JUCE and the UI is JUCE. The local `.dev` generator is optional for a fresh
clone because the generated files are checked in.

## The build graph

The root `CMakeLists.txt` adds `plugin/`. `plugin/CMakeLists.txt` compiles the
engine as C and the tests as C++. `plugin/juce/CMakeLists.txt` adds the JUCE
targets: VST3, CLAP and standalone. The generated headers are written into the
build directory:

| Generated file | Generator | Consumer |
| --- | --- | --- |
| `Params.h` | `plugin/tools/gen_params.py` | the APVTS and the UI |
| `P1Product.h` | `plugin/tools/gen_params.py` | the product name and the state tree |
| `MacroTargets.h` | `plugin/tools/gen_macro_targets.py` | the macro processor and the editor |
| `macro_target_ids.h` | `plugin/tools/gen_macro_targets.py` | the C and C++ macro tests |
| `UiTokens.h` | `plugin/tools/gen_tokens.py` | the panel theme |
| `P1Synth.h` | the root CMake alias | the engine API |
| `type_p_alpha_dsp.c` | checked in, from `.dev/generate.py` | the engine |
| `tables.c` | checked in, from `.dev/generate.py` | the routing and table data |
| `wavetable.h` | `tools/gen_wavetable.py` | the oscillator |
| `math_tables.h` | `tools/gen_math_tables.py` | the fixed-point helpers |

The checked-in generated engine is in `engine/gen`. The local generator is
`.dev/`, which is ignored by git. A fresh clone builds from the checked-in
`engine/gen`. A maintainer who changes a node or the graph runs

```sh
python .dev/generate.py
python tools/gen_wavetable.py
```

and commits the changed `engine/gen` files.

## Configure and build

The canonical build is clang with lld:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld \
  -DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld \
  -DCMAKE_MODULE_LINKER_FLAGS=-fuse-ld=lld
cmake --build build
ctest --test-dir build --output-on-failure
```

`P1_MODERN=ON` selects the AVX2 or arm-tuned build. On Windows the wrapper is

```powershell
.\windows\build.ps1
.\windows\build.ps1 -Modern
```

On Linux the container wrapper is

```sh
docker/build.sh linux/amd64
docker/build.sh linux/arm64
```

`docker/build.sh` builds the Debian trixie image, configures clang and lld,
builds into `build/linux-amd64-clang` and runs the tests. The image carries
Xvfb, so the editor snapshot test and the validators can run inside it.

## The tests

The CTest ladder has two layers. The portable engine and host tests are 17 on
Windows and 18 on Linux, because the snapshot diff is registered only on Linux.
The test targets are:

| Test | Source | What it proves |
| --- | --- | --- |
| `engine_parity` | `plugin/tests/engine_parity.cpp` | the generated C API matches the reference render, the note-derived path, the macro rows and the filter gate |
| `feedback_envelope` | `plugin/tests/feedback_envelope.cpp` | the signed feedback envelope changes the positive and negative families |
| `feedback_bipolar` | `plugin/tests/feedback_bipolar.cpp` | the bipolar feedback law and the negative pulse family |
| `pitch_envelope` | `plugin/tests/pitch_envelope.cpp` | the pitch envelope and its mode |
| `operator_lfos` | `plugin/tests/operator_lfos.cpp` | the amplitude, feedback and detune LFOs and their signs |
| `lfo_fades` | `plugin/tests/lfo_fades.cpp` | the rate and depth fades, the shared time scale and the phase reset |
| `filter_envelope` | `plugin/tests/filter_envelope.cpp` | the filter envelope rates, the absolute levels and the slowest rate |
| `phase_map` | `plugin/tests/phase_map.cpp` | the phase-map geometry, locks, flips, reorder, drift, key and velocity follow, the protected mask and the CPU ratio |
| `voice_alloc` | `plugin/tests/voice_alloc.cpp` | poly stealing, mono priority, two parts and the release tail |
| `wavetable` | `plugin/tests/wavetable.cpp` | the 14-wave table, the analysis table, the base offsets and the checksums |
| `filter_law` | `plugin/tests/filter_law.cpp` | the cutoff position law, the old-to-new conversion and the filter coefficients |
| `spectral` | `plugin/tests/spectral.cpp` | the additive bank, the a/b morph, the envelope, the LFO and the range |
| `rate_bridge` | `plugin/tests/rate_bridge_test.cpp` | the 48 kHz bypass, the ratios, the latency, the image rejection and the high-rate path |
| `preset_smoke` | `plugin/tests/preset_smoke.cpp` | the record layout, every migration, the merge, the library and the conflict rules |
| `plugin_smoke` | `plugin/tests/plugin_smoke.cpp` | the APVTS, the host state, the MIDI routing, the chunk notes, the macros, the ringmod and the library |
| `plugin_stability` | `plugin/tests/plugin_stability.cpp` | the processor survives repeated prepare, process, state save and state load |
| `plugin_editor_snapshot` | `plugin/tests/editor_snapshot.cpp` | renders every page and dialog |
| `plugin_editor_snapshot_diff` | the same renderer | compares the 31 images with the Linux goldens |

The manual and bench tools are built but not registered as gates:

- `phase_map_perceptual` renders the 140-scenario battery.
- `phase_map_perceptual_compare` compares two read modes.
- `spectral_bench` measures the bank generation.
- `additive_bench` and `additive_engine_bench` audition the additive path.
- `ui_frame_bench` measures the editor frame cost.
- `linux_daws.sh` renders in Ardour and Carla.

## CI

`.github/workflows/ci.yml` runs:

- `linux`: the Debian container, configure, build, test, bench.
- `linux` with `P1_SANITIZERS=address,undefined` and `thread`.
- `arm64`: the native arm runner and the container.
- `windows`: the baseline and modern AVX2 builds.
- `macos`: the universal baseline build, the validators and the DAW matrix where
  the runner has the hosts.
- `macos-arm`: the native arm build and the tests.
- `macos-modern`: the two slices joined with `lipo`, the x86 validator and the
  phase and spectral benches.

The CI uses `pluginval` 1.0.4 at strictness 5 and `clap-validator` 0.4.1. The
Linux DAW matrix needs Ardour, Carla, JACK and Xvfb, so it is a separate job.

## Snapshots

`plugin/tests/editor_snapshot.cpp` writes 31 PNGs to
`build/.../plugin/juce/snapshots`. The Linux golden set is
`plugin/tests/snapshots/linux-x86_64`. The diff gate is exact and is registered
only on Linux because font rasterisation differs on other hosts.

A deliberate UI change is accepted with

```sh
python3 plugin/tools/snapshot_check.py --update \
  --actual build/linux-amd64-clang/plugin/juce/snapshots \
  --golden plugin/tests/snapshots/linux-x86_64
```

The update deletes goldens that the new run no longer produces, copies the new
images and rewrites the manifest. A stale file in the build snapshot directory
can otherwise be mistaken for a real page. Remove the actual snapshot directory
before a clean update when the set changed.

## Release

`.github/workflows/release.yml` builds the Linux x86_64 and aarch64 packages, the
Windows baseline and modern AVX2 packages, the macOS universal baseline package
and the macOS universal modern package. It validates the macOS plugins with
pluginval and clap-validator, packages the licences and uploads every zip to the
GitHub release. The release job also attaches the source archive.

The Windows package contains the VST3 bundle, the CLAP, the standalone, `LICENSE`,
`LICENSING.md` and `licences/`. The macOS package contains the VST3, CLAP,
standalone app and the same licence files. The Linux package contains the VST3,
CLAP and standalone.

## Adding a parameter

A parameter needs a manifest entry in `synth/patch.json`, a target node input and
a parameter spec in the node. Then:

1. Run `python .dev/generate.py` if a node changed.
2. Run the build so `Params.h` and `Engine.cpp` see the new id.
3. Add the control in the page or card.
4. Add the parameter to `synth/macros.json` if a macro target should reach it.
5. Add a test for the host value, the state round trip or the DSP behaviour.
6. Run the full CTest set and the snapshot gate.
7. Update the feature contract and the generated parameter index.

The parameter id is assigned by the generator. Do not type an id by hand.

## Adding a node or an algorithm

A new node is a `.dev/modules/nodes/<name>/node.json` and `node.c`, plus a graph
instance in `synth/patch.json`. A new algorithm is a new case in the router node
`.dev/modules/nodes/route/node.c`, a new routing row in `engine/gen/tables.c`
through `.dev/umetate/_tables.py`, and the macro carrier mask follows from the
routing table. The generated engine, the parameter table and the macro target
table all change together.

A new waveform starts in `tools/gen_wavetable.py`, then the oscillator range, the
wave name list, the spectral mode index and the preset migration. The half square
was added this way at index 11, which shifted square, saw and spectrum to 12, 13
and 14.

## The code documentation generator

`tools/gen_code_docs.py` reads the tracked sources and rewrites
`docs/code/reference`. It needs the optional `tree-sitter` parsers:

```sh
python -m pip install tree-sitter tree-sitter-c tree-sitter-cpp tree-sitter-python
python tools/gen_code_docs.py
```

It writes the file, type, function, symbol, state-field, parameter, render-walk
and test indexes. The hand-written chapters are separate and are edited by hand.
Run the generator after a symbol change and commit the regenerated pages with the
code.
