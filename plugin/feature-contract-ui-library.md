# Feature contract: single parts and editor text

## Identity

- **Feature:** The 32-character names, the DotGothic switch text and the single
  parts popup.
- **Brief section:** `design/README.md` Dialogs, State language and Copy.
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete. The record migration, the editor text, the popup,
  the mockup, the Linux goldens and the host tests are in.

## Player outcome

- **New player action:** type a name up to 32 characters, and open the single
  part library from `part library actions...`.
- **Sound and behaviour:** the on/off switches and the mute switch read their
  state in DotGothic. The bipolar switch reads `bipolar on` or `bipolar off`, and
  the mute switch reads `active` or `mute` to the right of the track. The single
  parts popup lists part 1 and part 2 with their names and source slots, and it
  carries export, import, init, save and load for each part. `cancel` is the
  rightmost button in every dialog, and the single parts buttons use two rows.
- **Failure mode:** a switch clips its text, a name stops at 15 characters, the
  context menu returns, `cancel` sits on the left, or a saved 32-character name
  comes back truncated.

## Dependencies and interactions

- **Other features:** `P1Toggle`, `InlineMuteSwitch`, `P1Dialog`, `PresetStore`,
  the engine preset records, `design/mockup.html` and the design notes.
- **Platform/API/HAL changes:** none. The change is JUCE and the shared record
  helpers.
- **Persistence/version impact:** the name field grows from 16 to 33 bytes,
  `PRESET_VERSION` becomes 2 and the library magic becomes `P1LIB2`. The reader
  accepts `P1LIB1` with the 16-byte name field, including the 146, 160 and 179
  part layouts. Each part and multi record grows by 17 bytes.
- **MIDI/clock impact:** none.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** no new engine parameters.
  `PresetStore::kNameLen` is 32. The bipolar mode stays 0 = unipolar and
  1 = bipolar.
- **Note/event ordering and ownership:** the single parts actions address part 1
  and part 2 directly and do not change the active part. A save writes the
  selected part to the chosen slot and sets its source. A load applies the slot
  and sets its source.
- **Voice allocation and stealing:** unchanged.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged.
- **Per-sample and per-block operations:** none. The change is at save, load and
  repaint time.
- **Instance memory, CPU, preset size:** the library grows by 17 bytes per part
  record and 17 bytes per multi record, which is 3,264 bytes across the full
  bank. The switch paint work is the same.
- **Overflow/overload policy:** the name is truncated to 32 characters on save
  and the record holds 32 characters plus the terminator.
- **Division, shift, and range analysis:** none.

## Files and interfaces

- **Candidate files:** `engine/runtime/presets.h`, `engine/runtime/presets.c`,
  `plugin/preset/PresetStore.hpp`, `plugin/preset/PresetStore.cpp`,
  `plugin/juce/PluginEditor.cpp`, `plugin/juce/PluginEditor.h`,
  `plugin/juce/Pages.cpp`, `plugin/juce/Pages.h`, `plugin/ui/Widgets.cpp`,
  `plugin/tests/preset_smoke.cpp`, `plugin/tests/editor_snapshot.cpp`,
  `design/mockup.html`, `design/README.md` and `design/juce.md`.
- **Public contract/header:** `PresetStore` and the record helpers keep their
  public names. The v1 record helpers are added for the old name field.
- **State owner and publication rule:** `PresetStore` owns the bank. The editor
  saves the library after a part or multi change, as before.
- **Forbidden dependencies:** unchanged.

## Verification

- **Host vector(s):** `preset_smoke` covers the 32-character round trip, the
  current v2 library, the v1-name current part layout and the 146, 160 and 179
  layouts. `plugin_smoke` covers the state and the global library. `p1_stability`
  is unchanged.
- **Browser vector (if applicable):** the mockup browser check covers the single
  parts popup, its two rows, the cancel order, the 32-character rename field, the
  LFO modes and the library flow.
- **Target package check:** the Linux build and the Windows build both link the
  editor, the smoke tests and the stability test.
- **Host timing and platform check:** Linux `ctest` passes 9 of 9, and the Linux
  snapshot diff passes with 14 images.
- **Tolerance/metric:** the snapshot diff is exact. The name field stores 32
  characters. The v1 libraries load with the same names and parameters.
- **Evidence artefact:** the Linux snapshots `editor-single-parts.png`,
  `editor-long-name.png`, `editor-operators.png`, `editor-patch.png`,
  `editor-confirm.png`, `editor-save-mode.png` and `editor-rename.png`, and the
  `plugin/tests/preset_smoke.cpp` output.

## Risks and fallback

- **Assumptions/open research:** the P1LIB1 reader keeps the old user libraries
  usable. A real macOS DAW and manual pass remains separate platform work.
- **Safe experiment and stop criterion:** if a v1 or v2 library fails to load, or
  a 32-character name returns shortened, stop and fix the record path before the
  UI.
- **Smaller alternative:** keep the name field at 16 bytes and truncate the
  display. This loses the 32-character names, so it is rejected.
- **Rollback:** the `P1LIB2` writer is one constant and the v1 reader is additive.
  Reverting the record helpers restores the 15-character limit.
