# Feature contract: single parts and editor text

## Identity

- **Feature:** The 32-character names, the DotGothic switch text and the single
  parts popup.
- **Brief section:** `design/README.md` Dialogs, State language and Copy.
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete, with the record format, the editor text, the popup, the
  mockup, the Linux goldens and the host tests in. The name field, the switch text and
  the popup are three separate pieces of work in one milestone, and I've put the record
  format first because that's the part that can break a stored bank.

## Player outcome

- **New player action:** type a name up to 32 characters, and open the single part
  library from `single part library...`.
- **Sound and behaviour:** the on/off switches and the mute switch read their state in
  DotGothic, so the bipolar switch reads `bipolar on` or `bipolar off` and the mute
  switch reads `active` or `mute` to the right of the track. The single parts library
  opens on the 128-slot scroller and carries `export bank`, `import bank`,
  `init single part 1`, `init single part 2`, `save single part 1`,
  `save single part 2`, `load single part 1`, `load single part 2` and `cancel`, and
  because the buttons choose the part and the action, the slot is selected first, as
  in the multi-patch library.
  `cancel` is the rightmost button in every dialog.
- **Failure mode:** a switch clips its text, a name stops at 15 characters, the context
  menu returns, `cancel` sits on the left, the part library hides the scroller behind a
  part summary, or a saved 32-character name comes back truncated.

## Dependencies and interactions

- **Other features:** `P1Toggle`, `InlineMuteSwitch`, `P1Dialog`, `PresetStore`, the
  engine preset records, `design/mockup.html` and the design notes.
- **Platform/API/HAL changes:** none, because the change is JUCE and the shared record
  helpers.
- **Persistence/version impact:** the name field grows from 16 to 33 bytes,
  `PRESET_VERSION` becomes 2 and the library magic becomes `P1LIB2`, and the reader
  accepts the `P1LIB2` layout only at this slice, since the 146, 160 and 179 layouts and the
  16-byte `P1LIB1` name field are removed. Each part and multi record grows by 17
  bytes, so a library made before `P1LIB2` needs conversion outside the plugin.
  (The current record is version 18 and the library magic is `P1LIB3`; this is the
  UI-library slice.)
- **MIDI/clock impact:** none.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** no new engine parameters,
  `PresetStore::kNameLen` is 32, and the bipolar mode stays 0 = unipolar and 1 =
  bipolar.
- **Note/event ordering and ownership:** the single parts actions address part 1 and
  part 2 directly and do not change the active part, so a save writes the selected part
  to the chosen slot and sets its source, whilst a load applies the slot and sets its
  source.
- **Voice allocation and stealing:** unchanged.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged.
- **Per-sample and per-block operations:** none, because the change is at save, load
  and repaint time.
- **Instance memory, CPU, preset size:** the library grows by 17 bytes per part record
  and 17 bytes per multi record, which is 3,264 bytes across the full bank, and the
  switch paint work is the same.
- **Overflow/overload policy:** the name is truncated to 32 characters on save, and the
  record holds 32 characters plus the terminator.
- **Division, shift, and range analysis:** none.

## Files and interfaces

- **Candidate files:** `engine/runtime/presets.h`, `engine/runtime/presets.c`,
  `plugin/preset/PresetStore.hpp`, `plugin/preset/PresetStore.cpp`,
  `plugin/preset/part_defaults.h`, `plugin/juce/PluginEditor.cpp`,
  `plugin/juce/PluginEditor.h`, `plugin/juce/Pages.cpp`, `plugin/juce/Pages.h`,
  `plugin/ui/Widgets.cpp`, `plugin/tests/preset_smoke.cpp`,
  `plugin/tests/editor_snapshot.cpp`, `design/mockup.html`, `design/README.md`
  and `design/juce.md`.
- **Public contract/header:** `PresetStore` and the record helpers keep their
  public names, and there is one current record layout.
- **State owner and publication rule:** `PresetStore` owns the bank, and the editor
  saves the library after a part or multi change, as before.
- **Forbidden dependencies:** unchanged.

## Verification

- **Host vector(s):** `preset_smoke` covers the 32-character round trip and the current
  `P1LIB3` library (the `P1LIB2` file still loads as revision 0), `plugin_smoke`
  covers the state and the global library, and `p1_stability` is unchanged.
- **Target package check:** the Linux build and the Windows build both link the editor,
  the smoke tests and the stability test.
- **Host timing and platform check:** the full Linux `ctest` suite passes, and the
  Linux snapshot diff passes with 31 images.
- **Tolerance/metric:** the snapshot diff is exact, the name field stores 32
  characters, and the current `P1LIB3` library loads with the same names and
  parameters.
- **Evidence artefact:** the Linux snapshots `editor-single-parts.png`,
  `editor-long-name.png`, `editor-operators.png`, `editor-patch.png`,
  `editor-confirm.png`, `editor-save-mode.png` and `editor-rename.png`, and the
  `plugin/tests/preset_smoke.cpp` output.

## Risks and fallback

- **Assumptions/open research:** the current `P1LIB3` format is the only one the plugin
  reads, so a pre-`P1LIB2` library needs conversion outside the plugin, and the macOS
  DAW pass and the manual belong to separate platform work.
- **Safe experiment and stop criterion:** if a `P1LIB2` library fails to load, or a
  32-character name returns shortened, stop and fix the record path before the UI.
- **Smaller alternative:** keep the name field at 16 bytes and truncate the display,
  which loses the 32-character names, so it is rejected.
- **Rollback:** the previous commit restores the old-layout reader, and that is a
  compatibility branch, so I keep it out of the clean tree.
