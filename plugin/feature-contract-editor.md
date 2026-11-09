# Feature contract: the designed editor

## Identity

- **Feature:** Milestone 3, the designed JUCE editor for Type P.fm1.
- **Brief section:** `plugin/README.md` section 9, `design/README.md`,
  `design/juce.md`, and `docs/roadmap.md` 13.19.
- **Owner / date:** P1Onken, 2026-11-04.
- **Disposition:** accepted.

## Player outcome

- **New player action:** open the plugin and edit the synth from the three
  designed pages instead of JUCE's generic parameter list.
- **Sound and behaviour:** every knob and switch is attached to the same
  APVTS parameter the host automates, and the editor never writes engine state
  directly.
- **Failure mode:** a missing parameter leaves its control inert, and a scope
  with no audio draws a flat line. The editor never allocates,
  locks or reads files on the audio thread.

## Dependencies and interactions

- **Other features:** the 303-parameter manifest, the engine bridge, the
  processor scope rings, the design tokens, the OFL fonts and the plugin state.
- **Platform/API/HAL changes:** new host-native UI sources, a generated
  `UiTokens.h`, bundled font resources and a JUCE DSP dependency for the FFT
  spectro, whilst the firmware gate is unchanged.
- **Persistence/version impact:** patch name, part names, active part and active
  page are stored as properties in the plugin state blob, whilst preset banks
  remain Milestone 4.
- **MIDI/clock impact:** none. The editor only observes the existing scope rings.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** the manifest stays the
  authority, and controls use the manifest IDs and display metadata. The key and
  velocity target selectors only choose which parameter pair is visible, whilst
  the hidden targets remain host-automatable.
- **Note/event ordering and ownership:** unchanged. The editor does not touch
  the MIDI or audio paths.
- **Voice allocation and stealing:** unchanged.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged, 20 voices.
- **Per-sample and per-block operations:** the audio thread pushes the output
  and ten tap channels into lock-free scope rings. The editor timer runs at
  30 Hz for scope, spectro and envelope repaint.
- **RAM, flash, stack, pool:** the editor owns one canvas, three pages, the
  controls and seven 2 KiB scope rings, and fonts add about 2.1 MB of read-only
  binary data, so no target RAM changes.
- **Overflow/overload policy:** unchanged.
- **Division, shift, and range analysis:** the core arithmetic is unchanged.

## Files and interfaces

- **Files:** `plugin/tools/gen_tokens.py`, `plugin/ui/Theme.h/.cpp`,
  `plugin/ui/Widgets.h/.cpp`, `plugin/ui/ParamIDs.h`,
  `plugin/juce/PluginEditor.h/.cpp`, `plugin/juce/Pages.h`,
  `plugin/juce/Pages.cpp`, `plugin/juce/PagesOperators.cpp`,
  `plugin/juce/PagesFxMod.cpp`, `plugin/juce/resources/fonts/`,
  `plugin/tests/editor_snapshot.cpp`, `plugin/juce/CMakeLists.txt`,
  `plugin/juce/PluginProcessor.h/.cpp`.
- **Public contract/header:** `PluginEditor`, `UiTokens.h`, `Params.h` and the
  processor scope methods `readOutputScope` and `readTapScope`.
- **State owner and publication rule:** the processor owns the APVTS, the scope
  rings and the UI names, whilst the editor owns the canvas and page components.
  The audio thread is the only scope writer, and the message thread is the only
  scope reader.
- **Forbidden dependencies:** no file I/O, allocation or locks on the audio
  thread. No web fonts at runtime.

## Verification

- **Host vector(s):** `plugin_editor_snapshot` creates the real processor and
  editor, renders the patch, operators, fx / mod, algorithm picker, rename
  dialog and the 50 percent scale to PNG, and checks the algorithm and part
  mute parameters, and `plugin_smoke` continues to check the APVTS and audio
  path.
- **Browser vector:** the mockup remains the visual reference, and the design
  language screenshots are current.
- **Target package check:** unchanged, `deep-check --all` still builds the
  target package.
- **Physical timing/device check:** blocked, no hardware.
- **Tolerance/metric:** the editor canvas is 1240 by 985 at 1x, a 620 by 493
  snapshot must show the same layout at 50 percent with no reflow or clipping,
  and every visible parameter maps to a manifest ID.
- **Evidence artefact:** the editor PNGs under `juce/snapshots`, the CTest
  output, and the full `deep-check --all`.

## Result (2026-11-04)

- `UiTokens.h` is generated from `design/tokens.json` at configure time.
- DotGothic16 and Zen Dots are bundled as binary resources, and the OFL texts are
  in `plugin/juce/resources/fonts`.
- Three pages use the v1.1 design: patch settings, operators and fx / mod.
- Header algorithm block opens the eight-tile picker, and the wheel cycles
  the active part's algorithm.
- The output, operator and part visualisers read lock-free scope rings filled by
  the audio thread, and the spectros use a 512-point FFT at 30 Hz.
- The key and velocity follow cards have on switches, target selectors and wipe
  buttons, and the operator envelope uses separate rate and level rows.
- The algorithm picker and patch/part rename dialogs are in, and the part menu
  supports rename, copy, paste and init.
- The editor scales the fixed 1240 by 985 canvas uniformly, and the 50 percent
  snapshot is unclipped.
- `plugin_editor_snapshot` and `plugin_smoke` pass under Xvfb, and the full plugin
  CTest suite is green.
- Physical device acceptance and timing remain blocked.

## Risks and fallback

- **Assumptions/open research:** the host provides a window large enough for the
  fixed aspect ratio, and the default is 1240 by 985.
- **Safe experiment and stop criterion:** render the editor snapshot before
  judging the page layout, and stop if a parameter has no control or a visualiser
  reads outside its ring.
- **Smaller alternative:** keep the generic editor behind a build flag until the
  designed editor is stable, though the current build replaces it.
- **Rollback:** revert the Milestone 3 commit and restore
  `GenericAudioProcessorEditor` in `createEditor`.
