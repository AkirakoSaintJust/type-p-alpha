# Feature contract: in-plugin preset banks

## Identity

- **Feature:** Milestone 4, the 64 multi and 128 part slots.
- **Brief section:** `plugin/README.md` section 10 and `docs/roadmap.md` 13.20.
- **Owner / date:** P1Onken, 2026-11-04.
- **Disposition:** accepted.

## Player outcome

- **New player action:** save and load whole multi patches and individual part
  timbres inside the plugin.
- **Sound and behaviour:** a loaded part or multi writes the same parameters
  the host automates, because the record bytes come from the firmware preset code.
  A bank round-trips with the device once the import and export path lands.
- **Failure mode:** an empty or corrupt record is skipped, a save into a used
  slot asks for a second press before overwriting, and the plugin
  never writes a device bank by itself.

## Dependencies and interactions

- **Other features:** the 303-parameter APVTS, the designed editor, the plugin
  state blob, `presets.c` and the generated sysex layout.
- **Platform/API/HAL changes:** the plugin compiles the firmware `presets.c`
  record functions for the host with stub flash services, so no firmware change
  is needed.
- **Persistence/version impact:** the plugin state version is 2. It stores the
  part bank, multi bank, current multi slot and part source slots in base64
  properties.
- **MIDI/clock impact:** none. Program change still waits for a later preset
  milestone.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** the manifest stays the
  authority. A part record stores the 134 per-part timbre parameters in manifest
  order, whilst a multi record stores two part-slot references and the routing
  values channel, low, high, level and delay.
- **Note/event ordering and ownership:** preset loads run on the message thread
  and write the APVTS, which the audio thread sees at its next control block.
- **Voice allocation and stealing:** unchanged.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged, 20 voices.
- **Per-sample and per-block operations:** no audio-thread work, because the
  editor packs and unpacks on save and load only.
- **RAM, flash, stack, pool:** the part bank is 128 × 557 = 71,296 bytes. The
  multi bank is 64 × 63 = 4,032 bytes. The plugin state base64 adds about 100 KiB,
  so no target RAM changes.
- **Overflow/overload policy:** a full bank refuses a new slot. The editor has
  128 part slots and the multi save finds a free part slot when a part has no
  source.
- **Division, shift, and range analysis:** unchanged.

## Files and interfaces

- **Files:** `plugin/preset/PresetStore.hpp`, `plugin/preset/PresetStore.cpp`,
  `plugin/preset/presets_host.c`, `plugin/juce/PluginProcessor.h/.cpp`,
  `plugin/juce/PluginEditor.h/.cpp`, `plugin/tests/preset_smoke.cpp`,
  `plugin/tests/plugin_smoke.cpp`, `plugin/tests/editor_snapshot.cpp`,
  `plugin/CMakeLists.txt`.
- **Public contract/header:** `p1::PresetStore` with `savePart`, `loadPart`,
  `saveMulti`, `loadMulti`, the bank pack and load calls, the current multi and
  the part source slots. `PluginProcessor::presets`.
- **State owner and publication rule:** the processor owns the store, the editor
  mutates it on the message thread, and the state blob is the only persistence
  path.
- **Forbidden dependencies:** no flash calls in the plugin, no file access on the
  audio thread, no silent device bank rewrite.

## Verification

- **Host vector(s):** `preset_smoke` saves and loads part and multi records with
  the device `preset_part_pack`, `preset_part_unpack`, `preset_multi_pack` and
  `preset_multi_unpack` functions. It checks the record sizes, the empty marker,
  the name and parameter round trip, the routing round trip and the name limit.
  `plugin_smoke` checks that the plugin state restores both banks and the current
  slot. `plugin_editor_snapshot` saves and loads a part and a multi through the
  editor, then checks the parameter and patch name round trip.
- **Browser vector:** unchanged, the web parity remains green.
- **Target package check:** unchanged, `deep-check --all` still builds the
  target package.
- **Physical timing/device check:** blocked, no hardware.
- **Tolerance/metric:** byte-identical device records through the C functions,
  and the APVTS values and names match after a load.
- **Evidence artefact:** the CTest output and the editor snapshot PNGs.

## Result (2026-11-04)

- `PresetStore` reuses the device `presets.c` pack and unpack functions. The
  part record is 557 bytes and the multi record is 63 bytes.
- The store has 128 part slots and 64 multi slots. Empty slots are `0xFF` and a
  corrupt record is skipped.
- The plugin state version is 2. It restores the part bank, multi bank, current
  multi and part source slots.
- The patch button opens the 64-slot browser with save and load, and the part
  menu opens the 128-slot browser in save or load mode.
- Saved parts use the saved part source slots, and a part with no source is saved
  to a free part slot first. The part browser save and multi browser save open a
  confirmation dialog before overwriting a used slot.
- Names are limited to 15 characters in the store and the rename dialog.
- `preset_smoke`, `plugin_smoke`, `engine_parity` and
  `plugin_editor_snapshot` pass, and the editor snapshots include the patch and
  part browsers.
- Device SysEx import and export remain a later milestone. Physical device
  acceptance remains blocked.

## Risks and fallback

- **Assumptions/open research:** the per-part timbre params stay contiguous in
  the manifest. `kP1PartParamCount`, `kP1PartCount` and `kP1RoutingBase` are
  generated into `Params.h` to catch drift.
- **Safe experiment and stop criterion:** round-trip one part and one multi
  through the C functions before adding the UI. Stop if any byte or field
  differs.
- **Smaller alternative:** keep only part slots and no multi browser. The current
  build has both.
- **Rollback:** remove the preset store from the processor and the two browser
  actions. The device preset code is unchanged.
