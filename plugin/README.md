# Type P.alpha plugin

This folder is the desktop plugin, and the pieces it builds come from three places:
the voice engine is generated into `../engine/gen`, the editor is split between
`juce/` and `ui/`, and the shared parameter table is built from
`engine/gen/params.json`, so nobody has to keep a second copy of the parameter list
in step with the first.

The feature contracts in this folder describe the current features. Each one carries
the current semantics, the measured evidence and a short research-progress note for
the layers it replaced. The research pages in `../docs/research/` keep the
measurements of the discarded experiments. The current engine is 24 voices at
48 kHz, and the current contracts include the filter law, the spectral oscillator,
operator feedback, the envelope family and the 48 kHz core.

## Build

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

The top-level `CMakeLists.txt` turns on the JUCE and CLAP builds and adds this
folder, so the configure step above is all this folder needs. The build fetches
JUCE 9.0.5 and the pinned `clap-juce-extensions` commit, and the artefacts are
`VST3/Type P.alpha.vst3`, `CLAP/Type P.alpha.clap` and `Standalone/Type P.alpha`
under `build/plugin/juce/type_p_alpha_artefacts/Release/`.

## Tests

```sh
ctest --test-dir build --output-on-failure
```

Each test names its own subject, so a failure tells you which layer moved before
you go digging.

- `engine_parity` compiles the engine and checks the MIDI path.
- `filter_law` checks the cutoff table, the resonance onset and the input soft knee.
- `feedback_envelope` checks the per-operator envelope, the flat path, the partial negative level and the peak bound.
- `feedback_bipolar` checks the bipolar positive and negative halves and the signed envelope level.
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

Every manifest parameter must reach the APVTS, and the generator is the only
reference, so add a parameter to the manifest rather than typing it twice. The
editor shows the player-facing controls, and the filter's internal makeup is the
one manifest parameter that stays in the host automation only. I'd leave that one
alone until there's a control to go with it, because a parameter with no panel
control is easy to forget.

## Snapshots

The golden images are `plugin/tests/snapshots/linux-x86_64`, and the set is pinned
to the Linux container because font rasterisation differs on other hosts, so a
render on your own desktop isn't the image the gate compares. After a deliberate
editor change, review the diff and accept it:

```sh
python3 plugin/tools/snapshot_check.py --update \
  --actual build/plugin/juce/snapshots \
  --golden plugin/tests/snapshots/linux-x86_64
```

## Release

The `.github/workflows/release.yml` builds the Linux and Windows artefacts on a
tag and attaches the VST3, CLAP, standalone and source packages, and each package
carries the `licences/` folder. Read `../LICENSING.md` before publishing.
