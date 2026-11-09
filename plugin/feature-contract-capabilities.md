# Feature contract: plugin capabilities

## Identity

- **Feature:** build-time plugin capabilities with their own design, parameters, DSP and UI.
- **Brief section:** the JUCE app can carry tools that do not ship in the firmware.
- **Owner / date:** P1Onken, 2026-11-09.
- **Disposition:** accepted for the framework and the `tone` demo.

## Player outcome

- **New player action:** open a plugin-only page, adjust its controls, and hear the result.
- **Sound and behaviour:** the capability processes the final stereo output after the shared engine. The demo is an optional one-pole tone control with an on/off switch.
- **Failure mode:** a capability that fails validation is not built. A missing or stale generated file stops the plugin configure with the file name.

## Dependencies and interactions

- **Other features:** the generated engine, the APVTS, the plugin library, the editor tabs and the device preset records.
- **Platform/API/HAL changes:** none in the firmware. A small SysEx read tolerance is recorded in the verification section.
- **Persistence/version impact:** capability parameters are plugin-only and append after the 315 base parameters. The base IDs, the 141-param device part record and the `.p1lib` bank format do not change. The plugin state carries the capability values.
- **MIDI/clock impact:** none. Capability parameters are host-automatable APVTS parameters.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** declared in `capability.json`. The generator appends them after the base manifest and rejects duplicate names. The demo uses `cap_tone_on` (0/1, default 0) and `cap_tone` (0 to 100 percent, default 100).
- **Note/event ordering and ownership:** to be defined in the design interview.
- **Voice allocation and stealing:** may change for the plugin only. Plugin-only capabilities may be voice-level and modify the plugin's voice allocation.
- **Sustain/panic behaviour:** the engine owns sustain and panic. If there are plugin-only changes, they must be completely defined in the design interview.

## Bounds and resources

- **Maximum voices/events/effects:** to be defined in the design interview. The demo uses one one-pole filter per channel.
- **Per-sample and per-block operations:** depend on the capability, to be discussed in the design and implementation interviews, and during the implementation.
- **RAM, flash, stack, pool:** irrelevant, the firmware only handles new parameter discard and, if necessary, new parameter clamps when loading plugin-only patches or parts.
- **Overflow/overload policy:** the demo clamps its output to the int32 range. A capability must design its own overload policy during the interview and document.
- **Division, shift, and range analysis:** Must be brought up during DSP design. The demo computes its coefficient only when the tone parameter changes.

## Files and interfaces

- **Candidate files:** `plugin/capabilities/<id>/`, `plugin/tools/gen_capabilities.py`, `plugin/CMakeLists.txt`, `plugin/engine/Engine.cpp`, `plugin/juce/CapabilityPage.cpp`, `plugin/juce/PluginEditor.cpp`, `plugin/ui/Widgets.cpp`.
- **Public contract/header:** a capability package declares `capability.json`, a `dsp.c` with `<prefix>_init`, `<prefix>_set_param` and `<prefix>_process`, an optional `ui.cpp` with the declared C++ page factory, and `design.md`. The generator writes `CapabilityDSP.h`, `CapabilityPages.h`, `CapabilityRegistry.h`, `CapabilityMigrations.h` and `CapabilitySources.cmake`.
- **State owner and publication rule:** the capability owns its DSP state. The APVTS owns the parameter values. The host state owns persistence.
- **Forbidden dependencies (malloc, float, flash, display in audio):** no allocation, flash or display in the audio thread. The plugin-only DSP may use float. The firmware engine stays integer and is not rebuilt.

## Verification

- **Host vector(s):** `umetate.test_capability` checks the merged manifest, the generated headers and the migration table. `plugin_smoke` checks that the capability parameter reaches the DSP, that the default bypass is bit-exact and that the tone path changes the output.
- **Browser vector (if applicable):** not applicable. Capabilities are plugin-only.
- **Target package check:** the firmware package is rebuilt and its host WAV hash must not change.
- **Physical timing/device check:** not applicable to plugin-only capabilities.
- **Tolerance/metric:** the default capability state is bit-exact with the base engine. The demo (tone path) is checked for a measurable high-frequency loss and finite output.
- **Evidence artefact:** the test output, the plugin suite and the updated `editor-*.png` snapshots.

## Risks and fallback

- **Assumptions/open research:** the first demo is output-stage and global-param only. Per-part and base-param-target capabilities need a record extension and a migration pass.
- **Safe experiment and stop criterion:** if the generated capability changes the base engine render at default values, stop and fix the bypass. The default state must be neutral.
- **Smaller alternative:** a capability with UI and no DSP, or a capability with DSP and no UI.
- **Rollback:** remove the id from `plugin/capabilities/enabled.json`, or delete the package. The generated files and tabs disappear on the next configure.
