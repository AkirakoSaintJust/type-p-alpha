# Codebase inventory and audit

This is the reference map for the active Type P.alpha product tree. It describes
the hand-written types and functions, marks the generated code, and records the
duplication and legacy cleanup. The FM-1 tree stays in its archive tag and is out
of scope.

## Scope and method

The inventory covers the product repository at `type-p-alpha`: the engine runtime,
the plugin host, the preset store, the editor, the tests and the build tools. The
generated engine files are listed as generated and are not described function by
function, because the generator and the module sources are the authority.

The audit looked for four things:

- the same behaviour written twice;
- code that no caller reaches;
- preset and MIDI compatibility for the abandoned FM-1 device;
- the old Type P.alpha preset layouts and the old name field.

The current tree has one preset record layout, one part layout and one library
magic. A library made before `P1LIB2` no longer loads. The current layout stores
32-character names.

## Tree

| Path | Role | Source |
| --- | --- | --- |
| `engine/hal/` | integer math and table access | hand-written |
| `engine/runtime/` | preset records, MIDI parser | hand-written |
| `engine/gen/` | generated DSP, params, tables, wave data | generated |
| `plugin/engine/` | C++ seam, MIDI bridge, rate bridge | hand-written |
| `plugin/preset/` | preset bank, defaults, host record stubs | hand-written |
| `plugin/ui/` | theme, widgets, parameter ids | hand-written |
| `plugin/juce/` | processor, editor, pages, dialogs | hand-written |
| `plugin/tests/` | parity, DSP, preset, host, snapshot tests | hand-written |
| `plugin/tools/` | params, tokens, ratios, figures, snapshots | hand-written |
| `design/` | tokens, design notes, clickable mockup | hand-written |
| `synth/` | patch, panel and plugin descriptions | data |

## Engine HAL

`engine/hal/p1_math.h` is the fixed-point layer. `P1_MATH_INLINE` is the
always-inline attribute, and `P1_Q15_ONE` is 32768.

| Symbol | Purpose |
| --- | --- |
| `p1_q15_mul` | Multiply two Q15 values and return Q15. |
| `p1_q15_mul32` | Multiply a Q15 value by an int32 and shift by 15. |
| `p1_q16_mul` | Multiply two Q16 values and return Q16. |
| `p1_q31_mul` | Multiply two Q31 values and return Q31. |
| `p1_shr_round` | Arithmetic right shift with round-to-nearest. |
| `p1_sat_add32` | Saturating int32 addition. |
| `p1_sat_sub32` | Saturating int32 subtraction. |
| `p1_madd_sat` | Saturating multiply-add with a shift. |
| `p1_onepole` | One-pole smoothing step toward a target. |
| `p1_sin` | Sine from a 32-bit phase. |
| `p1_cos` | Cosine from a 32-bit phase. |
| `p1_sin_wave` | Waveform lookup through the wavetable and its mip. |
| `p1_wave_mip` | Choose the wavetable mip for a phase step. |
| `p1_key_scale` | Bipolar key-follow factor from note and breakpoint. |
| `p1_key_apply` | Apply a bipolar factor to a value and clamp. |
| `p1_vel_scale` | Velocity-follow factor from velocity and amount. |
| `p1_key_add` | Sum two bipolar key/velocity factors and clamp. |
| `p1_clamp` | Clamp to explicit bounds. |
| `p1_clamp16` | Clamp to the Q15 range. |
| `p1_clamp16_max` | Clamp to 0..max. |
| `p1_route4` | Four-way weighted operator routing. |
| `p1_tune_q16` | Octave, semitone and cent to a Q16 ratio. |
| `p1_poly_blep` | PolyBLEP correction for the saw and pulse waves. |

## Engine runtime

### Preset records

`engine/runtime/presets.h` and `presets.c` own the byte layout. The record is a
version byte, a 33-byte name field, the body and a CRC-32. Empty slots are
`0xFF`. The current version is 2.

| Symbol | Purpose |
| --- | --- |
| `PRESET_NAME_LEN` | 33 bytes, 32 characters and a terminator. |
| `PRESET_VERSION` | 2 for the current record. |
| `PRESET_EMPTY` | `0xFF`, the empty slot marker. |
| `PRESET_PART_SLOTS` | 128 single-part slots. |
| `PRESET_MULTI_SLOTS` | 64 multi-patch slots. |
| `PRESET_SLOT_MAX` | Storage slot byte ceiling for the engine runtime. |
| `SYNTH_PARAM_MAX` | Per-part parameter cap. |
| `PP_RECORD` | Part record size from the parameter count. |
| `MP_RECORD` | Multi record size from the part count. |
| `ST_RECORD` | Settings record size. |
| `PP_BANK` | Part bank size. |
| `MP_BANK` | Multi bank size. |
| `wr32` | Little-endian 32-bit write. |
| `rd32` | Little-endian 32-bit read. |
| `preset_crc32` | CRC-32 used by every record. |
| `preset_crc16` | CRC-16-CCITT used by the SysEx envelope. |
| `name_put` | Copy and truncate a name into the record field. |
| `seal` | Write the CRC at the end of the body. |
| `preset_part_pack` | Pack one part record. |
| `preset_multi_pack` | Pack one multi record. |
| `preset_settings_pack` | Pack the settings record. |
| `preset_part_unpack` | Validate and unpack one part record. |
| `preset_multi_unpack` | Validate and unpack one multi record. |
| `preset_settings_unpack` | Validate and unpack the settings record. |
| `bank_per` | Records that fit in one engine storage slot. |
| `bank_slots` | Storage slots a bank spans. |
| `bank_save` | Write a bank through `slot_save`. |
| `bank_load` | Read a bank through `slot_load`. |
| `preset_part_bank_save` | Save the part bank. |
| `preset_part_bank_load` | Load the part bank. |
| `preset_multi_bank_save` | Save the multi bank. |
| `preset_multi_bank_load` | Load the multi bank. |
| `preset_settings_save` | Save the settings record. |
| `preset_settings_load` | Load the settings record. |

The bank and settings functions serve the shared engine runtime and the device
build. The plugin host uses the pack and unpack functions, and the in-memory
store in `PresetStore`. The host build supplies empty storage hooks, so the bank
helpers are link-safe and unused by the plugin.

### MIDI parser

`engine/runtime/synth_midi.h` and `synth_midi.c` turn a raw MIDI stream into the
engine seam.

| Symbol | Purpose |
| --- | --- |
| `SYNTH_CC_NONE` | Sentinel for an unassigned CC. |
| `synth_midi_ctx` | Callback table, CC map, parameter ranges and sustain state. |
| `synth_midi_feed` | Handle one status byte and its two data bytes. |
| `synth_midi_poll` | Drain `midi_get` and feed each message. |

The sustain state sits in the context, so two engines can share the parser. The
plugin feeds the context directly and the `midi_get` stub remains for the shared
runtime.

## Generated engine

| Path | Generated from | Contents |
| --- | --- | --- |
| `engine/gen/type_p_alpha_dsp.c` | `synth/patch.json` and the umbrella modules | engine state, parameter application, render, taps, node graph |
| `engine/gen/dsp.h` | same | state struct and public engine functions |
| `engine/gen/wavetable.h` | waveform data | mip-mapped wavetables |
| `engine/gen/naive_wave.h` | waveform data | naive waveform references |
| `engine/gen/math_tables.h` | generator | sine, envelope and rate tables |
| `engine/gen/cc_map.h` | generator | CC to parameter map |
| `engine/gen/tables.c` and `tables.h` | generator | table objects |
| `engine/gen/params.json` | generator | manifest of every parameter |

These files are regenerated with `python .dev/generate.py` from the umbrella
module graph. They are not edited by hand.

## Plugin engine

### `p1::Engine`

`plugin/engine/Engine.hpp` and `Engine.cpp` put one `P1_STATE` behind a small C++
seam with no JUCE dependency.

| Method | Purpose |
| --- | --- |
| `Engine` | Allocate and initialise the generated state. |
| `reset` | Reset the generated state. |
| `setParam` | Apply one engine parameter by id. |
| `getParam` | Read one engine parameter by id. |
| `noteOn` | Start a note with velocity and an optional channel. |
| `noteOff` | Release a note. |
| `allNotesOff` | Release every voice. |
| `setBend` | Apply a 16-bit pitch bend. |
| `render` | Render interleaved int32 frames. |
| `lit` | Read a lit LED base for the UI. |
| `quiet` | Report whether the engine is silent. |
| `tap` | Read a per-channel tap buffer. |
| `state` | Return the raw generated state for the host wrappers. |

### `p1::MidiBridge`

`plugin/engine/MidiBridge.hpp` and `MidiBridge.cpp` connect the C MIDI parser to
the C++ engine.

| Method | Purpose |
| --- | --- |
| `MidiBridge` | Fill the parser context with the engine callbacks, CC map and ranges. |
| `feed` | Feed one MIDI message. |
| `reset` | Clear the sustain state. |

The free functions `p1_bridge_note_on`, `p1_bridge_note_off`,
`p1_bridge_all_off`, `p1_bridge_set_param` and `p1_bridge_set_bend` are the C
callbacks. A thread-local `current` engine pointer joins the context-free C
callback to the C++ object. It is safe because the bridge is fed on one thread.

### `p1::RateBridge`

`plugin/engine/RateBridge.hpp` and `RateBridge.cpp` resample the 48 kHz core to
the host rate. At 48 kHz the processor bypasses it, so the core stays exact.

| Symbol | Purpose |
| --- | --- |
| `besselI0` | Kaiser window helper. |
| `prepare` | Build the 128-tap, 1024-phase kernel and size the ring. |
| `reset` | Clear the ring and the frame counters. |
| `pushCore` | Append interleaved core frames to the ring. |
| `pull` | Produce host-rate frames from the polyphase kernel. |
| `renderChunk` | Render one core chunk and push it. |
| `process` | Render enough core frames for the block and pull the host frames. |
| `latencySamples` | Report the kernel delay in host samples. |

## Preset store

### `p1::PresetStore`

`plugin/preset/PresetStore.hpp` and `PresetStore.cpp` own the 128 part slots and
64 multi slots. A part slot stores the part sound and name. A multi stores the
part references and the routing. The session pointers are `currentMulti` and the
per-part source slot.

| Symbol | Purpose |
| --- | --- |
| `Part` | Used flag, name and parameter vector for one part slot. |
| `Multi` | Used flag, name, part references and the five routing vectors. |
| `PresetStore` | Build the two banks and the source vector. |
| `partParamCount` | Part parameter count. |
| `partCount` | Part count in one multi. |
| `partRecordSize` | Current part record size. |
| `multiRecordSize` | Current multi record size. |
| `savePart` | Store a name and parameter vector in a part slot. |
| `loadPart` | Read a part slot. |
| `partUsed` | Report a used part slot. |
| `partName` | Read a part slot name. |
| `partParams` | Read a part slot parameter vector. |
| `saveMulti` | Store a name, part references and routing. |
| `loadMulti` | Read a multi slot. |
| `multiUsed` | Report a used multi slot. |
| `multiName` | Read a multi slot name. |
| `packPartBank` | Pack all 128 part records. |
| `loadPartBank` | Load and validate a part bank. |
| `packMultiBank` | Pack all 64 multi records. |
| `loadMultiBank` | Load and validate a multi bank. |
| `packLibrary` | Write the `P1LIB2` part bank, multi bank and sizes. |
| `loadLibrary` | Validate and load a `P1LIB2` library. |
| `findFreePartSlot` | First empty part slot, or -1. |
| `findFreeMultiSlot` | First empty multi slot, or -1. |
| `currentMulti` | Current multi slot, or -1. |
| `setCurrentMulti` | Set or clear the current multi. |
| `partSource` | Part slot a part currently points at, or -1. |
| `setPartSource` | Set or clear a part source. |
| `shortName` | Truncate a name to 32 characters. |

`plugin/preset/part_defaults.h` holds `partParamDefaults`, which builds the
default vector from `kP1ParamSpecs`. It replaced the old migration header.

## UI layer

### Theme

`plugin/ui/Theme.h` and `Theme.cpp` own the fonts, colours and look and feels.

| Symbol | Purpose |
| --- | --- |
| `colour` | Convert a packed ARGB value to a JUCE colour. |
| `makeTypeface` | Load one embedded typeface. |
| `uiTypeface` | DotGothic16 face. |
| `displayTypeface` | Zen Dots face. |
| `brandTypeface` | Unbounded face. |
| `uiFont` | UI font at a height. |
| `displayFont` | Display font at a height. |
| `brandFont` | Brand font at a height. |
| `textWidth` | Measure a string in a font. |
| `drawSectionTitle` | Draw the `// title` section marker. |
| `P1LookAndFeel` | Shared combo, menu and scrollbar look. |
| `P1LookAndFeel::drawComboBox` | Draw the combo outline and arrow. |
| `P1LookAndFeel::positionComboBoxText` | Place the combo text. |
| `P1LookAndFeel::drawPopupMenuBackground` | Draw the menu surface. |
| `P1LookAndFeel::drawPopupMenuItem` | Draw a menu row. |
| `P1LookAndFeel::drawScrollbar` | Draw the thin scrollbar. |
| `KnobLookAndFeel` | Rotary knob look. |
| `KnobLookAndFeel::drawRotarySlider` | Draw the knob ring and pointer. |

### Widgets

`plugin/ui/Widgets.h` and `Widgets.cpp` hold the shared editor controls. The
parameter value formatter and parser are the bridge between the APVTS integers
and the panel readouts.

| Symbol | Purpose |
| --- | --- |
| `APVTS` | Alias for `juce::AudioProcessorValueTreeState`. |
| `findSpec` | Find a parameter spec by id. |
| `formatParamValue` | Format a parameter value for the panel. |
| `parseParamValue` | Parse a typed value back to a parameter value. |
| `signedNumber` | Format an explicit plus sign. |
| `snapRatio` | Snap a Q16 ratio to a detent. |
| `stepRatioDetent` | Move a Q16 ratio by one detent. |
| `notePair` | Format `note/name`, for example `60/c4`. |
| `percentage` | Convert a value to a percentage readout. |
| `knobLookAndFeel` | Shared knob look and feel instance. |
| `ParamKnob` | Knob, name, value and APVTS attachment. |
| `ParamKnob::KnobSlider` | Slider with ratio detent keys and wheel steps. |
| `ParamKnob::setParamID` | Rebind the knob to another parameter. |
| `ParamKnob::resized` | Place the dial, name and value. |
| `ParamKnob::paint` | Empty; the slider paints itself. |
| `ParamKnob::updateValueText` | Refresh the formatted value. |
| `ParamKnob::commitValue` | Parse and apply a typed value. |
| `ParamKnob::snapValue` | Snap the current ratio value. |
| `ParamKnob::stepRatio` | Apply a ratio detent key. |
| `ParamKnob::isRatio` | Report a ratio parameter. |
| `P1Button` | Flat text button with a mini variant. |
| `P1MuteTile` | Solid op or part mute tile. |
| `P1Toggle` | Track, thumb and `on` or `bipolar` state text. |
| `P1Choice` | Styled combo box. |
| `ParamChoice` | Combo box attached to an integer parameter. |
| `ParamChoice::resized` | Fill the component with the combo. |
| `ParamChoice::updateFromNormalized` | Apply an APVTS value to the combo. |
| `P1TabBar` | Tab strip. |
| `P1TabBar::Tab` | One cut-corner tab. |
| `P1TabBar::setLabels` | Rebuild the tabs. |
| `P1TabBar::setActive` | Select one tab. |
| `P1TabBar::resized` | Place the tabs. |
| `P1TabBar::paint` | Draw the tab baseline. |
| `P1Scope` | Green scope widget. |
| `P1Spectro` | Pink and purple spectrum widget. |
| `P1Envelope` | Four-rate, three-level envelope curve. |
| `P1Wave` | Waveform shape widget. |
| `P1LevelSlider` | Wide level slider with APVTS attachment. |
| `P1LevelSlider::setFromMouse` | Map a click to a level. |
| `P1Card` | Titled card surface with optional copy and paste header buttons. |
| `P1Card::contentArea` | Content rectangle below the title. |
| `P1Card::resized` | Place the copy and paste header buttons at the right. |
| `P1AlgorithmBlock` | Clickable algorithm number and graph. |
| `P1Dialog` | Modal dialog with a body and wrapping buttons. |
| `drawAlgorithmGraph` | Draw one of the eight routing graphs. |

## Parameter id helpers

`plugin/ui/ParamIDs.h` maps the panel vocabulary to APVTS ids. The functions are
mechanical, so the table lists each family and its target.

| Family | Target |
| --- | --- |
| `partSuffix`, `withPart` | `.pN` suffix and base id. |
| `algorithm`, `channel`, `keyLow`, `keyHigh`, `level`, `delay`, `detune`, `transpose`, `mode`, `legato`, `unisonCount`, `unisonDetune`, `portamento`, `notePriority` | part-level ids. |
| `opBase` | `opN_` prefix. |
| `opRatio`, `opDetune`, `opWaveform`, `opLevel`, `opFeedback` | operator oscillator ids. |
| `opTremRate`, `opTremDepth`, `opTremWave`, `opTremMode`, `opMute` | operator amplitude LFO and mute ids. |
| `opEnvRate`, `opEnvLevel` | operator amplitude envelope ids. |
| `opFeedbackOn` | per-operator feedback envelope on switch. |
| `opFeedbackRate`, `opFeedbackLevel` | per-operator feedback envelope rate and signed level ids. |
| `opFeedbackLfoRate`, `opFeedbackLfoDepth`, `opFeedbackLfoWave`, `opFeedbackLfoMode` | feedback LFO ids. |
| `opDetuneLfoRate`, `opDetuneLfoDepth`, `opDetuneLfoWave`, `opDetuneLfoMode` | detune LFO ids. |
| `pitchRate`, `pitchDepth`, `pitchWave`, `pitchMode`, `pitchEnvOn`, `pitchEnvRate`, `pitchEnvLevel` | pitch LFO and pitch envelope ids. |
| `filterCutoff`, `filterRes`, `filterType`, `filterOn`, `filterEnvDepth`, `filterRate`, `filterLevel` | filter and filter envelope ids. |
| `FollowTarget`, `followTargets` | key and velocity follow target table. |
| `waveNames`, `lfoWaveNames` | wave display names. |

## JUCE host and editor

### Processor

`plugin/juce/PluginProcessor.h` and `PluginProcessor.cpp` own the APVTS, the
engine, the MIDI bridge, the rate bridge, the preset bank and the UI state.

| Symbol | Purpose |
| --- | --- |
| `ScopeRing` | Single-producer ring for the scope taps. |
| `ScopeRing::push` | Append samples. |
| `ScopeRing::read` | Read the newest window. |
| `TypePAlphaAudioProcessor` | The JUCE processor. |
| `createLayout` | Build one `AudioParameterInt` per manifest parameter. |
| `prepareToPlay` | Reset the engine, bridge and rate bridge. |
| `releaseResources` | Release every note. |
| `isBusesLayoutSupported` | Accept mono or stereo output and no input. |
| `processBlock` | Feed MIDI, sync parameters, render and resample. |
| `syncParams` | Copy changed APVTS values into the engine. |
| `feedMessage` | Feed one raw MIDI message. |
| `renderCore` | Render one core block. |
| `createEditor` | Create the plugin editor. |
| `getStateInformation` | Serialise names, UI state and the library. |
| `setStateInformation` | Restore names, UI state and the library. |
| `ensureLibraryLoaded` | Load `library.p1lib` once. |
| `saveLibrary` | Write `library.p1lib`. |
| `libraryFile` | Resolve the library path. |
| `setLibraryFileForTesting` | Redirect the library path in tests. |
| `getPartName` / `setPartName` | Read or write a part name. |
| `getPatchName` / `setPatchName` | Read or write the patch name. |
| `getUiActivePart` / `setUiActivePart` | Persist the editing part. |
| `getUiPage` / `setUiPage` | Persist the page. |
| `readOutputScope` / `readTapScope` | Read scope taps. |
| `isEngineMessage` | Filter note, CC and bend messages. |
| `createPluginFilter` | JUCE entry point. |

### Editor classes

| Symbol | Purpose |
| --- | --- |
| `PatchButton` | Header multi-patch button with an ellipsised name. |
| `SegmentButton` | `scope` / `spectro` segment. |
| `PartChoiceButton` | Toolbar part selector. |
| `Header` | Wordmark, algorithm block, output scope and patch control. |
| `Toolbar` | Mute tiles, part selector and library action. |
| `SlotListBody` | Slot list built on the shared `ListBody`. |
| `ModeListBody` | Mode list built on the shared `ListBody`. |
| `ListBody` | Shared list surface, background, separator, selection and row click. |
| `AlgTile` | One algorithm tile. |
| `AlgPickerBody` | Eight-tile algorithm picker. |
| `RenameBody` | Name field and 32-character note. |
| `CardHeader` | Shared rename, copy, paste and mute controls for part and operator cards. |
| `P1Canvas` | Paints the editor frame and accents. |
| `PageBase` | Page interface with `tick`. |
| `PluginEditor` | Owns the pages, dialogs, active part and timer. |

`PluginEditor` methods are grouped by job:

| Group | Methods |
| --- | --- |
| Lifecycle | `paint`, `resized`, `timerCallback`, `fitToHostWindow`, `updateVisualizers`, `buildUi`, `buildHeader`, `buildToolbar`, `buildPage` |
| Part and page state | `setActivePart`, `showTab`, `refreshNames`, `setSpectroMode`, `setStatusMessage` |
| Algorithm | `showAlgorithmPicker`, `applyAlgorithm` |
| Names | `showRenameDialog` |
| Library | `showSinglePartsBrowser`, `exportLibrary`, `importLibrary`, `showPatchBrowser`, `showPatchSaveMode` |
| Part actions | `copyPart`, `pastePart`, `initPart`, `initMultiPatch`, `togglePartMute`, `savePartToSlot`, `loadPartFromSlot`, `copyOperator`, `pasteOperator`, `copyOperatorSection`, `pasteOperatorSection` |
| Multi actions | `savePatchToSlot`, `loadPatchFromSlot` |
| Parameter bridge | `currentPartParams`, `applyPartParams`, `currentRouting`, `applyRouting` |
| Dialog helpers | `showConfirm`, `closeDialog` |

### Pages

| Class | Purpose |
| --- | --- |
| `ModeButton` | Poly, mono, legato and unison button. |
| `PartCard` | One part card with name, actions, knobs, level, scope and spectro. |
| `VoiceMode` | Voice mode buttons and unison, detune and portamento knobs. |
| `PatchPage` | Two part cards and the voice mode block. |
| `OpColumn` | One operator column: oscillator, amplitude envelope, feedback envelope. |
| `OperatorsPage` | Four operator columns. |
| `OscillatorCard` | Shared operator oscillator card. |
| `AmplitudeEnvelopeCard` | Shared per-operator amplitude envelope card. |
| `FeedbackEnvelopeCard` | Per-operator signed feedback envelope graph, rates, levels and on switch. |
| `LfoCard` | Shared rate, depth, wave and bipolar card for the three LFO families. |
| `OpLfoColumn` | One operator LFO column: oscillator, amplitude LFO, feedback LFO, detune LFO. |
| `OperatorLfosPage` | Four operator LFO columns. |
| `FilterCard` | Filter cutoff, resonance, env amount, type and on switch. |
| `FilterEnvelopeCard` | Four-rate, three-level filter envelope. |
| `PitchLfoCard` | Pitch LFO wave, rate, depth, wave select and bipolar switch. |
| `PitchEnvelopeCard` | Pitch envelope graph, four rates, four levels and on switch. |
| `FollowCard` | Key or velocity follow card with target selector and wipe. |
| `FxModPage` | Filter, filter envelope, pitch LFO, pitch envelope and follow cards. |
| `setParam` / `getParam` | Inline helpers in `Pages.h`, used by every page. |
| `isCarrier` | Carrier test used for the operator name colour. |
| `makePatchPage`, `makeOperatorsPage`, `makeFxModPage`, `makeOperatorLfosPage` | Page factories. |

## Tests

| File | Purpose |
| --- | --- |
| `engine_parity.cpp` | Compare the generated engine against the independent reference for filters, key follow and render parity. |
| `feedback_envelope.cpp` | Measure the per-operator envelope family, the flat path, the partial negative level and the peak bound. |
| `feedback_bipolar.cpp` | Measure the positive path, the abs negative half, the signed envelope level and the peak bound. |
| `operator_lfos.cpp` | Check the feedback and detune LFO paths at depth 0, above, and with a negative depth. |
| `voice_alloc.cpp` | Check a three-note chord, a seven-note scale, one note's release, both part channels and recycled voices. |
| `additive_bench.cpp` | Headless additive table matrix, morph read and WAV audition. It is a console target, not a ctest gate. |
| `additive_engine_bench.cpp` | Render the build-tree additive engine prototype and write the engine audition. It is a console target, not a ctest gate. |
| `rate_bridge_test.cpp` | Check the resampler latency and spectral behaviour. |
| `preset_smoke.cpp` | Check the current record, the 32-character name and the `P1LIB2` library. |
| `plugin_smoke.cpp` | Check state restore, the global library, automation, the 48 kHz bridge and a three-note chord. |
| `plugin_stability.cpp` | Stress every parameter, polyphony, MIDI storms and truncated state. |
| `editor_snapshot.cpp` | Render the editor pages, the dialogs, the long name, the section paste and the half-size frame. |
| `engine_render_reference.h` | Independent reference render used by `engine_parity`. |

## Tools

| Tool | Purpose |
| --- | --- |
| `gen_params.py` | Generate `Params.h` from `params.json` and the ratio detents. |
| `ratio.py` | Compute the shared ratio detents. |
| `gen_tokens.py` | Generate `UiTokens.h` from `tokens.json`. |
| `snapshot_check.py` | Compare or accept the editor snapshot goldens. |
| `feedback_figures.py` | Render the feedback WAVs into scope PNGs. |
| `additive_prototype.py` | Generate the build-tree additive engine copy and tables. |

## Design reference

`design/mockup.html` is a clickable reference, not the product. Its JavaScript is
grouped into data, controls, views, library and dialogs:

| Area | Functions |
| --- | --- |
| Data | `initPartState`, `b`, `ba`, `defaultRouting` |
| Knobs and controls | `makeKnob`, `stepFor`, `editValue`, `finish`, `move`, `up`, `refreshKnob`, `makeSelect`, `makeToggle`, `makeMuteSwitch`, `makeLevelSlider`, `draw`, `setFromEvent` |
| Visuals | `drawScope`, `drawSpectro`, `tick`, `envSvg`, `penvSvg`, `lfoSvg`, `algSvg`, `wire`, `arrowDown`, `fit`, `renderAlg` |
| Sections and views | `knobRow`, `knobFor`, `makeSection`, `fxCard`, `opShell`, `oscSection`, `ampEnvSection`, `fbEnvSection`, `lfoSection`, `viewPatch`, `viewOps`, `viewOperatorLfos`, `viewFxMod`, `showTab`, `rerender` |
| Operator and part state | `findOp`, `setOpMute`, `setPartMute`, `bindPart`, `setActivePart`, `copyOp`, `pasteOp`, `copyPart`, `pastePart` |
| Toolbar and dialogs | `buildToolbar`, `openSingleParts`, `openDialog`, `toast`, `openRename`, `renamePart`, `openAlgPicker`, `setAlg` |
| Part slots | `partSnapshot`, `applyPartSnapshot`, `openSingleParts`, `confirmOverwritePart`, `savePartToSlot`, `loadPartFromSlot`, `initPart` |
| Multi slots | `initMultiPatch`, `openMultiInitConfirm`, `multiSnapshot`, `findFreePartSlot`, `savePatchToSlot`, `loadPatchFromSlot`, `openPatchBrowser`, `openPatchSaveMode`, `confirmOverwriteMulti` |
| Library | `exportLibrary`, `importLibrary`, `importLibraryData`, `normalisePart`, `normaliseMulti`, `showPatchName` |
| Navigation | `fromHash` |

The CSS is a token mirror of `design/tokens.css`; it is not duplicated logic.

## Findings

### Removed in this pass

| Item | Why |
| --- | --- |
| `Pages.cpp` local `setParam` / `getParam` | Duplicated the inline helpers in `Pages.h`. |
| One-argument `savePartToSlot` and `loadPartFromSlot` | Only the snapshot test used them; the explicit part form is the real API. |
| `P1AlgorithmGraph` | No caller; `P1AlgorithmBlock` and `drawAlgorithmGraph` cover the header. |
| `drawLabel` | No caller. |
| `plugin/engine/msvc_compat.h` | It was a no-op for the clang-cl build and only served a pure-MSVC path the project does not use. |
| `P1Card` panel-background flag | Every card uses the panel background; the false branch had no caller. |
| Duplicated `SlotListBody` and `ModeListBody` list machinery | A shared `ListBody` owns the list, row paint, separator, selection and click. |
| Repeated part and operator header setup | A shared `CardHeader` owns rename, copy, paste and the mute switch. |
| `PluginEditor::setStatusMessage` and `statusMessage_` | The string was never painted; the status strip shows only the MIDI and sync LEDs. |
| `P1Choice` string constructor | No caller; the default combo is the only use. |
| The `.slots` mockup CSS | It had no markup after the library rewrite. |
| `plugin/preset/part_migration.h` | Held the 146, 160 and 179 maps. The current tree has one part layout. |
| `legacy146PartMap`, `legacy160PartMap`, `legacy179PartMap`, `feedbackInsertionShift`, `shaperDropShift` | FM-1 and old Type P.alpha migration code with no current caller. |
| `PresetStore::legacyPartMaps_` and the legacy branch in the loaders | The store now accepts one record layout. |
| `preset_part_pack_v1`, `preset_multi_pack_v1`, `preset_part_unpack_v1`, `preset_multi_unpack_v1` | The 16-byte name reader is gone. |
| `PRESET_LEGACY_NAME_LEN`, `PRESET_VERSION_V1`, `PP_RECORD_V1`, `MP_RECORD_V1` | The old record constants are gone. |
| `P1LIB1` magic handling | The library is `P1LIB2` only. |
| The four legacy preset tests | They test code that no longer exists. |

The record format is now a single `P1LIB2` layout with a 33-byte name field and a
32-character limit. A pre-`P1LIB2` library is not read by the plugin.

### Duplication left on purpose

| Area | Observation | Decision |
| --- | --- | --- |
| `ParamIDs.h` accessors | Many one-line functions return `withPart` strings. | They are the readable parameter map. A macro would hide typos and gain little. |
| `pages` card layouts | Cards repeat `removeFromTop` and `removeFromLeft` arithmetic. | The layout is local to each card and the snapshots depend on it. Keep. |
| Mockup and JUCE | The mockup mirrors the editor layout. | The mockup is the design reference, so the duplication is deliberate. |

### Legacy left on purpose

| Item | Reason |
| --- | --- |
| `engine/runtime/presets.c` bank and settings helpers | They belong to the shared engine runtime and the device build. The plugin host links them through the same source and does not call them. Splitting the runtime would fork the engine. |
| `engine/runtime/synth_midi.c` | It is the live MIDI parser behind `MidiBridge`, not an FM-1 path. |
| `README` FM-1 history and `docs/fm1-scrub.md` | Project background and archive evidence. |
| Old feature contracts | They are dated records of shipped milestones. The audit note explains that their old-layout claims describe the past. |

### Follow-ups

1. A separate offline converter can read `P1LIB1` if old user banks need to move
   to `P1LIB2`. The plugin deliberately carries no legacy reader.
2. The macOS AU and the real DAW pass remain platform work.

## Verification for the cleanup

- `preset_smoke` passes with the current record and the 32-character name.
- `plugin_smoke` and `plugin_stability` pass on the current state and library.
- Linux `ctest` passes 11 of 11, including the 13-image snapshot diff.
- The Windows clang-cl build links without the MSVC shim.
- The umbrella `deep-check --all` is green.
