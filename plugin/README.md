# Type P.alpha plugin

This folder is the desktop plugin. The voice engine is generated into
`../engine/gen`, the editor is in `juce/` and `ui/`, and the shared parameter
table is generated from `engine/gen/params.json`.

## Build

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The top-level `CMakeLists.txt` turns on the JUCE and CLAP builds and adds this
folder. The build fetches JUCE 9.0.5 and the pinned `clap-juce-extensions` commit.
The artefacts are `VST3/Type P.alpha.vst3`, `CLAP/Type P.alpha.clap` and
`Standalone/Type P.alpha` under `build/plugin/juce/type_p_alpha_artefacts/Release/`.

## Tests

```sh
ctest --test-dir build --output-on-failure
```

- `engine_parity` compiles the engine and checks the MIDI path.
- `rate_bridge` checks the output-rate adaptation and the exact core-rate bypass.
- `preset_smoke` checks the preset records.
- `plugin_smoke` checks the APVTS, the state and the automation.
- `plugin_stability` checks repeated load and render.
- `plugin_editor_snapshot` renders every page, and `plugin_editor_snapshot_diff`
  compares the goldens.
- `linux_daws` loads the VST3 and CLAP builds in Ardour and Carla.
  Use `ctest -E linux_daws` where those hosts are absent.

## Parameters

`plugin/tools/gen_params.py` turns `engine/gen/params.json` into the C++ table, and
`gen_tokens.py` turns `design/tokens.json` into `UiTokens.h`.

Every manifest parameter must reach the APVTS and the editor. The generator is the
only reference, so add a parameter to the manifest rather than typing it twice.

## Snapshots

The golden images are `plugin/tests/snapshots/linux-x86_64`. The set is pinned to
the Linux container because font rasterisation differs on other hosts. After a
deliberate editor change, review the diff and accept it:

```sh
python3 plugin/tools/snapshot_check.py --update \
  --actual build/plugin/juce/snapshots \
  --golden plugin/tests/snapshots/linux-x86_64
```

## Release

The `.github/workflows/release.yml` builds the Linux and Windows artefacts on a
tag and attaches the VST3, CLAP, standalone and source archives. Each archive
carries the `licences/` folder. Read `../LICENSING.md` before publishing.


