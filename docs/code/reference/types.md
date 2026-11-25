# Type, class, struct, union and enum index

Every type definition found in the tracked C, C++ and Python sources. The
field list is representative for the large generated state, which has its own
field index in `state-fields.md`.

| Type | Kind | File:Line | Fields | Description |
| --- | --- | --- | ---: | --- |
| `type_p_alpha_state` | typedef | `engine/gen/dsp.h:13` | 1 | The generated engine state. One instance per plugin instance. |
| `type_p_alpha_init_entry` | typedef | `engine/gen/dsp.h:1692` | 1 |  |
| `type_p_alpha_state` | typedef | `engine/gen/type_p_alpha_dsp.c:13` | 1 | The generated engine state. One instance per plugin instance. |
| `dsp_param` | typedef | `engine/gen/type_p_alpha_dsp.c:1744` | 1 |  |
| `type_p_alpha_slot_t` | typedef | `engine/gen/type_p_alpha_dsp.c:3801` | 1 |  |
| `p1_fft_cfg` | typedef | `engine/hal/p1_math.h:359` | 1 |  |
| `p1_fft_cpx` | typedef | `engine/hal/p1_math.h:360` | 1 |  |
| `synth_midi_ctx` | typedef | `engine/runtime/synth_midi.h:20` | 1 |  |
| `PhaseMapSnapshot` | struct | `plugin/engine/Engine.hpp:15` | 13 | The display view of one per-part phase map. It is a copy of the map pool geometry plus the newest voice's mapped phase, so the UI never reads the engine state directly. |
| `Engine` | class | `plugin/engine/Engine.hpp:34` | 16 | One generated engine state behind a small C++ seam. There is no JUCE and no audio backend here, because the wrappers are the only place the host touches the generated C API. |
| `Engine` | class forward declaration | `plugin/engine/MidiBridge.hpp:10` | 0 | The C++ seam over one generated engine state. |
| `MidiBridge` | class | `plugin/engine/MidiBridge.hpp:14` | 4 | The device MIDI handler for one engine. The sustain state is stored in the context, so each bridge keeps its own pedal state. |
| `Engine` | class forward declaration | `plugin/engine/RateBridge.hpp:10` | 0 | The C++ seam over one generated engine state. |
| `RateBridge` | class | `plugin/engine/RateBridge.hpp:15` | 17 | Output-only 128-tap Kaiser-windowed sinc polyphase resampler. The core runs at 48 kHz, whilst the bridge converts to the host rate, and at 48 kHz the processor bypasses it so the core output stays bit-exact there. |
| `ChunkNoteSet` | class | `plugin/juce/ChunkNoteSet.h:12` | 3 | Tracks the notes that started in the current control chunk, by MIDI channel and pitch, so a release in the same chunk waits for the chunk to render. Two channels can hold the same pitch without deferring each other's releases. |
| `ModeButton` | class | `plugin/juce/Pages.cpp:15` | 1 |  |
| `PartCard` | class | `plugin/juce/Pages.cpp:50` | 10 |  |
| `VoiceMode` | class | `plugin/juce/Pages.cpp:145` | 7 |  |
| `PatchPage` | class | `plugin/juce/Pages.cpp:231` | 2 |  |
| `InlineMuteSwitch` | class | `plugin/juce/Pages.h:39` | 2 |  |
| `CardHeader` | class | `plugin/juce/Pages.h:80` | 7 |  |
| `OscillatorCard` | class | `plugin/juce/Pages.h:138` | 6 |  |
| `AmplitudeEnvelopeCard` | class | `plugin/juce/Pages.h:198` | 3 |  |
| `FeedbackEnvelopeCard` | class | `plugin/juce/Pages.h:272` | 5 |  |
| `LfoCard` | class | `plugin/juce/Pages.h:359` | 7 |  |
| `FilterCard` | class | `plugin/juce/PagesFxMod.cpp:12` | 5 |  |
| `FilterEnvelopeCard` | class | `plugin/juce/PagesFxMod.cpp:63` | 7 |  |
| `PitchLfoCard` | class | `plugin/juce/PagesFxMod.cpp:144` | 9 |  |
| `PitchEnvelopeCard` | class | `plugin/juce/PagesFxMod.cpp:205` | 7 |  |
| `PanEnvelopeCard` | class | `plugin/juce/PagesFxMod.cpp:285` | 7 |  |
| `FollowCard` | class | `plugin/juce/PagesFxMod.cpp:363` | 13 |  |
| `RingGraph` | class | `plugin/juce/PagesFxMod.cpp:505` | 4 |  |
| `RingmodCard` | class | `plugin/juce/PagesFxMod.cpp:596` | 10 |  |
| `FxModPage` | class | `plugin/juce/PagesFxMod.cpp:759` | 8 |  |
| `MacroRange` | class | `plugin/juce/PagesMacros.cpp:16` | 7 | The compact bipolar range a macro card uses. The stored value sits at the right and the fill runs from the centre, so a destination or source range reads as a signed amount. |
| `MacroTargetChoice` | class | `plugin/juce/PagesMacros.cpp:111` | 5 | The target menu with its eight section headings. The item id is the target enum plus one, so `none` is 1 and the parameter value is the id minus one. |
| `MacroCard` | class | `plugin/juce/PagesMacros.cpp:162` | 13 |  |
| `MacrosPage` | class | `plugin/juce/PagesMacros.cpp:295` | 2 |  |
| `OpColumn` | class | `plugin/juce/PagesOperators.cpp:25` | 10 |  |
| `OpLfoColumn` | class | `plugin/juce/PagesOperators.cpp:107` | 10 |  |
| `OperatorsPage` | class | `plugin/juce/PagesOperators.cpp:205` | 2 |  |
| `OperatorLfosPage` | class | `plugin/juce/PagesOperators.cpp:238` | 2 |  |
| `PhaseMapView` | class | `plugin/juce/PagesPhase.cpp:36` | 6 | The static parameter preview is the fallback until the part's map has been seeded. Once the engine publishes one, the same plot uses the real composed geometry and the newest voice's mapped phase. |
| `PhaseCard` | class | `plugin/juce/PagesPhase.cpp:235` | 2 |  |
| `PhaseEnvelopeCard` | class | `plugin/juce/PagesPhase.cpp:290` | 5 |  |
| `CompactOscillator` | class | `plugin/juce/PagesPhase.cpp:380` | 5 |  |
| `PhaseColumn` | class | `plugin/juce/PagesPhase.cpp:430` | 9 |  |
| `PhaseMapPage` | class | `plugin/juce/PagesPhase.cpp:515` | 2 |  |
| `SpringSlider` | class | `plugin/juce/PagesSpectral.cpp:89` | 7 |  |
| `MaskRow` | class | `plugin/juce/PagesSpectral.cpp:185` | 2 |  |
| `MorphEnvelopeCard` | class | `plugin/juce/PagesSpectral.cpp:237` | 5 |  |
| `SpectrumRow` | class | `plugin/juce/PagesSpectral.cpp:325` | 23 |  |
| `SpectraOperatorCard` | class | `plugin/juce/PagesSpectral.cpp:732` | 8 |  |
| `SpectraModColumn` | class | `plugin/juce/PagesSpectral.cpp:806` | 8 |  |
| `OperatorSpectraPage` | class | `plugin/juce/PagesSpectral.cpp:898` | 4 |  |
| `OperatorSpectraModPage` | class | `plugin/juce/PagesSpectral.cpp:952` | 2 |  |
| `PatchButton` | class | `plugin/juce/PluginEditor.cpp:17` | 0 |  |
| `SegmentButton` | class | `plugin/juce/PluginEditor.cpp:38` | 1 |  |
| `PartChoiceButton` | class | `plugin/juce/PluginEditor.cpp:72` | 1 |  |
| `Header` | class | `plugin/juce/PluginEditor.cpp:108` | 14 |  |
| `Toolbar` | class | `plugin/juce/PluginEditor.cpp:269` | 6 |  |
| `ListBody` | class | `plugin/juce/PluginEditor.cpp:400` | 4 |  |
| `SlotListBody` | class | `plugin/juce/PluginEditor.cpp:462` | 2 |  |
| `ModeListBody` | class | `plugin/juce/PluginEditor.cpp:496` | 1 |  |
| `AlgTile` | class | `plugin/juce/PluginEditor.cpp:518` | 3 |  |
| `AlgPickerBody` | class | `plugin/juce/PluginEditor.cpp:564` | 3 |  |
| `RenameBody` | class | `plugin/juce/PluginEditor.cpp:612` | 2 |  |
| `Header` | class forward declaration | `plugin/juce/PluginEditor.h:16` | 0 |  |
| `Toolbar` | class forward declaration | `plugin/juce/PluginEditor.h:17` | 0 |  |
| `P1Canvas` | class | `plugin/juce/PluginEditor.h:21` | 1 |  |
| `PageBase` | class | `plugin/juce/PluginEditor.h:26` | 1 |  |
| `PluginEditor` | class | `plugin/juce/PluginEditor.h:34` | 72 | The JUCE editor for the whole panel. |
| `PartFxSection` | enum | `plugin/juce/PluginEditor.h:36` | 7 |  |
| `OperatorSection` | enum | `plugin/juce/PluginEditor.h:46` | 13 |  |
| `LockRelease` | struct | `plugin/juce/PluginProcessor.cpp:973` | 1 |  |
| `ScopeRing` | class | `plugin/juce/PluginProcessor.h:22` | 6 | The single-producer/single-consumer scope ring. |
| `LibrarySaveResult` | struct | `plugin/juce/PluginProcessor.h:39` | 7 | The outcome of a library save. A conflict means another instance wrote the slot first, so the disk version stayed and the local edit is still only in memory. `unreadable` means the file exists but could not be parsed, so nothing was written over it. |
| `TypePAlphaAudioProcessor` | class | `plugin/juce/PluginProcessor.h:49` | 81 | The JUCE audio processor and the host-facing plugin object. |
| `MacroFeedback` | struct | `plugin/juce/PluginProcessor.h:108` | 3 | What a macro destination does to one parameter right now. The stored value stays in the parameter. The `effective` value is the raw value the engine receives, which differs whenever a macro with a range deviates from its knob. |
| `ScheduledEvent` | struct | `plugin/juce/PluginProcessor.h:135` | 2 |  |
| `SpecBlock` | struct | `plugin/preset/PresetStore.cpp:723` | 3 | The spectral blocks: version 14 target, version 13 rates base and the separate version 13 depth. |
| `PresetStore` | class | `plugin/preset/PresetStore.hpp:15` | 62 | The in-memory preset bank. The byte layout comes from the engine's presets.c record functions, so a plugin bank round-trips with the device. |
| `Part` | struct | `plugin/preset/PresetStore.hpp:23` | 3 | One part record in the preset store. |
| `Multi` | struct | `plugin/preset/PresetStore.hpp:29` | 10 | One multi-patch record in the preset store. |
| `Conflict` | struct | `plugin/preset/PresetStore.hpp:87` | 3 | A slot that changed on disk since this instance loaded it. `multi` says which bank, and the name is the local name so the message can quote it. |
| `WaveTable` | struct | `plugin/tests/additive_bench.cpp:245` | 3 |  |
| `Spectrum` | struct | `plugin/tests/feedback_bipolar.cpp:85` | 4 |  |
| `Window` | struct | `plugin/tests/feedback_envelope.cpp:91` | 2 |  |
| `Ramp` | struct | `plugin/tests/lfo_fades.cpp:121` | 4 |  |
| `Scenario` | struct | `plugin/tests/phase_map_perceptual.cpp:54` | 24 |  |
| `Transfer` | struct | `plugin/tests/phase_map_perceptual.cpp:293` | 3 |  |
| `Case` | struct | `plugin/tests/phase_map_perceptual.cpp:373` | 3 |  |
| `Scenario` | struct | `plugin/tests/phase_map_perceptual_compare.cpp:52` | 16 |  |
| `Manifest` | struct | `plugin/tests/phase_map_perceptual_compare.cpp:71` | 4 |  |
| `Metrics` | struct | `plugin/tests/phase_map_perceptual_compare.cpp:78` | 19 |  |
| `Fft` | class | `plugin/tests/phase_map_perceptual_compare.cpp:242` | 3 |  |
| `Frame` | struct | `plugin/tests/phase_map_perceptual_compare.cpp:306` | 4 |  |
| `Analysis` | struct | `plugin/tests/phase_map_perceptual_compare.cpp:313` | 7 |  |
| `Gate` | struct | `plugin/tests/phase_map_perceptual_compare.cpp:734` | 3 |  |
| `Peak` | struct | `plugin/tests/plugin_stability.cpp:27` | 2 |  |
| `Measurement` | struct | `plugin/tests/rate_bridge_test.cpp:18` | 3 |  |
| `TickResult` | struct | `plugin/tests/spectral_bench.cpp:30` | 3 |  |
| `BlockResult` | struct | `plugin/tests/spectral_bench.cpp:112` | 3 |  |
| `ParamDoc` | class | `plugin/tools/gen_params.py:18` | 0 | One generated manifest row. |
| `ParamsDoc` | class | `plugin/tools/gen_params.py:35` | 0 | The generated params.json document. |
| `_TokenFile` | class | `plugin/tools/gen_tokens.py:15` | 0 |  |
| `FollowTarget` | struct | `plugin/ui/ParamIDs.h:344` | 4 |  |
| `P1LookAndFeel` | class | `plugin/ui/Theme.h:31` | 5 |  |
| `KnobLookAndFeel` | class | `plugin/ui/Theme.h:53` | 1 |  |
| `Item` | struct | `plugin/ui/Widgets.cpp:2133` | 2 |  |
| `Layout` | struct | `plugin/ui/Widgets.cpp:2185` | 6 |  |
| `RingBox` | struct | `plugin/ui/Widgets.cpp:2208` | 4 |  |
| `MacroFeedbackTarget` | class | `plugin/ui/Widgets.h:35` | 2 | A control whose parameter can be a macro destination. The editor asks the processor once per refresh and the control draws the stored value with the effective value beside it while a macro deviates. |
| `ParamValueLabel` | class | `plugin/ui/Widgets.h:46` | 6 | The value line of a knob. A plain value draws in its stored colour. When a targeted parameter has an effective value, the stored value keeps its unit and the effective value follows in smaller green type without a unit. |
| `ParamKnob` | class | `plugin/ui/Widgets.h:59` | 29 | A parameter-bound rotary knob with a value line and macro feedback. |
| `KnobSlider` | class | `plugin/ui/Widgets.h:83` | 4 | Arrow keys step a detented ratio knob to the next detent instead of nudging the raw value, which would snap straight back to where it was. |
| `P1Button` | class | `plugin/ui/Widgets.h:124` | 1 |  |
| `P1MuteTile` | class | `plugin/ui/Widgets.h:131` | 1 |  |
| `P1Toggle` | class | `plugin/ui/Widgets.h:139` | 3 |  |
| `P1Choice` | class | `plugin/ui/Widgets.h:151` | 0 |  |
| `ParamChoice` | class | `plugin/ui/Widgets.h:156` | 6 |  |
| `P1TabBar` | class | `plugin/ui/Widgets.h:171` | 11 |  |
| `Tab` | class | `plugin/ui/Widgets.h:182` | 3 |  |
| `P1Scope` | class | `plugin/ui/Widgets.h:198` | 3 |  |
| `P1Spectro` | class | `plugin/ui/Widgets.h:208` | 9 |  |
| `P1SpectralBars` | class | `plugin/ui/Widgets.h:224` | 22 | The editable 64-bar spectral level display. |
| `P1SpectrumDisplay` | class | `plugin/ui/Widgets.h:258` | 14 | The a/b spectral display with the live morph cursor. |
| `EnvelopeShape` | enum | `plugin/ui/Widgets.h:285` | 3 |  |
| `P1Envelope` | class | `plugin/ui/Widgets.h:291` | 15 | The envelope graph for the operator, filter, pan and spectral envelopes. |
| `P1Wave` | class | `plugin/ui/Widgets.h:320` | 7 | The small waveform preview. |
| `P1LevelSlider` | class | `plugin/ui/Widgets.h:334` | 18 |  |
| `P1VolumeSlider` | class | `plugin/ui/Widgets.h:366` | 10 | The master output fader. The parameter is the fader position, so the fill is linear in position whilst the gain follows the law in VolumeLaw.h, and the unity mark sits at 75 percent of the track. |
| `P1Card` | class | `plugin/ui/Widgets.h:384` | 10 | The shared card frame and header. |
| `P1AlgorithmBlock` | class | `plugin/ui/Widgets.h:403` | 14 |  |
| `P1Dialog` | class | `plugin/ui/Widgets.h:426` | 11 |  |
| `Function` | class | `tools/gen_code_docs.py:55` | 0 |  |
| `Type` | class | `tools/gen_code_docs.py:68` | 0 |  |
| `Symbol` | class | `tools/gen_code_docs.py:80` | 0 |  |

## Details

### `type_p_alpha_state`

`typedef` at `engine/gen/dsp.h:13` through line 1673.

The generated engine state. One instance per plugin instance.

Fields and enumerators:

- `struct type_p_alpha_state { uint32_t op1_ph_inc[NVOICE]; uint32_t op1_ph_phase[NVOICE]; uint32_t op1_ph_step[NVOICE]; uint32_t op1_ph_mip[NVOICE]; uint32_t op1_osc_wave[NVOICE]; uint32_t op1_osc_ba...`

### `type_p_alpha_init_entry`

`typedef` at `engine/gen/dsp.h:1692` through line 1692.

Fields and enumerators:

- `struct { uint16_t id; int32_t value; }`

### `type_p_alpha_state`

`typedef` at `engine/gen/type_p_alpha_dsp.c:13` through line 1673.

The generated engine state. One instance per plugin instance.

Fields and enumerators:

- `struct type_p_alpha_state { uint32_t op1_ph_inc[NVOICE]; uint32_t op1_ph_phase[NVOICE]; uint32_t op1_ph_step[NVOICE]; uint32_t op1_ph_mip[NVOICE]; uint32_t op1_osc_wave[NVOICE]; uint32_t op1_osc_ba...`

### `dsp_param`

`typedef` at `engine/gen/type_p_alpha_dsp.c:1744` through line 1745.

Fields and enumerators:

- `struct { uint32_t id; const char *name; int32_t min, max, def; uint8_t curve; uint8_t rate; const char *unit; }`

### `type_p_alpha_slot_t`

`typedef` at `engine/gen/type_p_alpha_dsp.c:3801` through line 3801.

Fields and enumerators:

- `struct { uint32_t off; uint8_t part; }`

### `p1_fft_cfg`

`typedef` at `engine/hal/p1_math.h:359` through line 359.

Fields and enumerators:

- `kiss_fft_cfg`

### `p1_fft_cpx`

`typedef` at `engine/hal/p1_math.h:360` through line 360.

Fields and enumerators:

- `kiss_fft_cpx`

### `synth_midi_ctx`

`typedef` at `engine/runtime/synth_midi.h:20` through line 32.

Fields and enumerators:

- `struct { void (*note_on)(uint32_t note, uint32_t velocity, uint32_t channel); void (*note_off)(uint32_t note, uint32_t channel); void (*all_off)(void); void (*all_off_ch)(uint32_t channel); int (*s...`

### `PhaseMapSnapshot`

`struct` at `plugin/engine/Engine.hpp:15` through line 29.

The display view of one per-part phase map. It is a copy of the map pool geometry plus the newest voice's mapped phase, so the UI never reads the engine state directly.

Fields and enumerators:

- `bool valid = false;`
- `int bp = 500;`
- `int segments = 8;`
- `int active = 8;`
- `int voices = 0;`
- `uint16_t phaseQ = 0;`
- `uint16_t phaseY = 0;`
- `std::array<uint16_t, 33> x{};`
- `std::array<uint16_t, 32> y0{};`
- `std::array<int32_t, 32> slope{};`
- `std::array<uint8_t, 32> flip{};`
- `std::array<uint8_t, 32> order{};`
- `std::array<uint8_t, 32> locked{};`

### `Engine`

`class` at `plugin/engine/Engine.hpp:34` through line 65.

One generated engine state behind a small C++ seam. There is no JUCE and no audio backend here, because the wrappers are the only place the host touches the generated C API.

Fields and enumerators:

- `void reset();`
- `int setParam(uint32_t id, int32_t value);`
- `int32_t getParam(uint32_t id);`
- `void noteOn(uint32_t note, uint32_t velocity, uint32_t channel = 0xFFFFFFFFu);`
- `void noteOff(uint32_t note, uint32_t channel = 0xFFFFFFFFu);`
- `void allNotesOff(uint32_t channel = 0xFFFFFFFFu);`
- `void setBend(uint32_t bend14, uint32_t channel = 0xFFFFFFFFu);`
- `void render(int32_t* out, uint32_t frames);`
- `uint32_t lit(uint32_t base);`
- `bool quiet();`
- `const int32_t* tap(uint32_t channel);`
- `int specMorph(int op, int part) const;`
- `bool phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const;`
- `void resetPhaseMaps();`
- `P1_STATE* state();`
- `std::unique_ptr<P1_STATE> state_;`

Methods:

- `Engine(const Engine&) = delete;`
- `Engine& operator=(const Engine&) = delete;`

### `Engine`

`class forward declaration` at `plugin/engine/MidiBridge.hpp:10` through line 10.

The C++ seam over one generated engine state.

### `MidiBridge`

`class` at `plugin/engine/MidiBridge.hpp:14` through line 24.

The device MIDI handler for one engine. The sustain state is stored in the context, so each bridge keeps its own pedal state.

Fields and enumerators:

- `void feed(uint8_t status, uint8_t data1, uint8_t data2);`
- `void reset();`
- `Engine& engine_;`
- `synth_midi_ctx ctx_{};`

### `Engine`

`class forward declaration` at `plugin/engine/RateBridge.hpp:10` through line 10.

The C++ seam over one generated engine state.

### `RateBridge`

`class` at `plugin/engine/RateBridge.hpp:15` through line 53.

Output-only 128-tap Kaiser-windowed sinc polyphase resampler. The core runs at 48 kHz, whilst the bridge converts to the host rate, and at 48 kHz the processor bypasses it so the core output stays bit-exact there.

Fields and enumerators:

- `void prepare(double hostRate, int maxBlock);`
- `void reset();`
- `void process(Engine& engine, float* left, float* right, int frames, CoreTapCallback onCore = nullptr, void* context = nullptr);`
- `void pushCore(const int32_t* interleaved, int frames);`
- `void pull(float* left, float* right, int frames);`
- `int latencySamples() const;`
- `bool hasCore(int hostFrames) const;`
- `static constexpr int kTaps = 128;`
- `static constexpr int kPhases = 1024;`
- `void renderChunk(Engine& engine, int frames, CoreTapCallback onCore, void* context);`
- `double hostRate_ = 48000.0;`
- `double ratio_ = 1.0;`
- `int ringSize_ = 0;`
- `std::vector<int32_t> ring_;`
- `std::vector<float> kernel_;`
- `int64_t coreGenerated_ = 0;`
- `int64_t outputIndex_ = 0;`

Methods:

- `double ratio() const`

### `ChunkNoteSet`

`class` at `plugin/juce/ChunkNoteSet.h:12` through line 35.

Tracks the notes that started in the current control chunk, by MIDI channel and pitch, so a release in the same chunk waits for the chunk to render. Two channels can hold the same pitch without deferring each other's releases.

Fields and enumerators:

- `static constexpr int kChannels = 16;`
- `static constexpr int kNotes = 128;`
- `std::array<uint8_t, kChannels * kNotes> held_{};`

Methods:

- `void reset()`
- `void noteOn(int channel, int note)`
- `bool held(int channel, int note) const`

### `ModeButton`

`class` at `plugin/juce/Pages.cpp:15` through line 48.

Fields and enumerators:

- `bool active_ = false;`

Methods:

- `explicit ModeButton(const juce::String& label) : juce::Button(label)`
- `void setActive(bool active)`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override`

### `PartCard`

`class` at `plugin/juce/Pages.cpp:50` through line 143.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int part_ = 0;`
- `bool active_ = false;`
- `CardHeader header_;`
- `std::array<std::unique_ptr<ParamKnob>, 7> knobs_{};`
- `std::unique_ptr<P1LevelSlider> level_;`
- `std::unique_ptr<P1LevelSlider> pan_;`
- `P1Scope scope_;`
- `P1Spectro spectro_;`
- `std::array<float, 512> scratch_{};`

Methods:

- `PartCard(PluginEditor& editor, int part, bool active) : editor_(editor), part_(part), active_(active), header_(true, true, [this] { editor_.showRenameDialog(true, part_); }, [this] { editor_.copyPa...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`

### `VoiceMode`

`class` at `plugin/juce/Pages.cpp:145` through line 229.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int part_ = 0;`
- `std::array<juce::String, 4> modeNames_{{"poly", "mono", "legato", "unison"}};`
- `std::array<std::unique_ptr<ModeButton>, 4> modes_{};`
- `std::unique_ptr<ParamKnob> unison_;`
- `std::unique_ptr<ParamKnob> detune_;`
- `std::unique_ptr<ParamKnob> porta_;`

Methods:

- `explicit VoiceMode(PluginEditor& editor) : editor_(editor), part_(editor.activePart())`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`

### `PatchPage`

`class` at `plugin/juce/Pages.cpp:231` through line 266.

Fields and enumerators:

- `std::array<std::unique_ptr<PartCard>, 2> parts_{};`
- `std::unique_ptr<VoiceMode> voice_;`

Methods:

- `explicit PatchPage(PluginEditor& editor)`
- `void resized() override`
- `void tick() override`

### `InlineMuteSwitch`

`class` at `plugin/juce/Pages.h:39` through line 78.

Fields and enumerators:

- `std::function<bool()> getMuted_;`
- `std::function<void(bool)> setMuted_;`

Methods:

- `InlineMuteSwitch(std::function<bool()> getMuted, std::function<void(bool)> setMuted) : getMuted_(std::move(getMuted)), setMuted_(std::move(setMuted))`
- `void paint(juce::Graphics& g) override`
- `void mouseUp(const juce::MouseEvent&) override`

### `CardHeader`

`class` at `plugin/juce/Pages.h:80` through line 136.

Fields and enumerators:

- `bool partLayout_ = true;`
- `bool withRename_ = true;`
- `bool wideHeader_ = false;`
- `P1Button rename_{"rename", true};`
- `P1Button copy_{"copy", true};`
- `P1Button paste_{"paste", true};`
- `std::unique_ptr<InlineMuteSwitch> mute_;`

Methods:

- `CardHeader(bool partLayout, bool withRename, std::function<void()> onRename, std::function<void()> onCopy, std::function<void()> onPaste, std::function<bool()> getMuted, std::function<void(bool)> s...`
- `void resized() override`
- `void tick()`

### `OscillatorCard`

`class` at `plugin/juce/Pages.h:138` through line 196.

Fields and enumerators:

- `ParamKnob ratio_;`
- `ParamKnob detune_;`
- `ParamKnob level_;`
- `ParamKnob feedback_;`
- `ParamChoice wave_;`
- `std::unique_ptr<ParamKnob> morph_;`

Methods:

- `OscillatorCard(PluginEditor& editor, int op, int part, bool withMorph = false) : P1Card("oscillator", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::osc...`
- `void resized() override`

### `AmplitudeEnvelopeCard`

`class` at `plugin/juce/Pages.h:198` through line 270.

Fields and enumerators:

- `P1Envelope envView_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 7> knobs_{};`

Methods:

- `AmplitudeEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("amp env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::amplitudeEnvelope); }, ...`
- `void resized() override`
- `void tick()`

### `FeedbackEnvelopeCard`

`class` at `plugin/juce/Pages.h:272` through line 357.

Fields and enumerators:

- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `FeedbackEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("fb env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::feedbackEnvelope); }, [&e...`
- `void resized() override`
- `void tick()`

### `LfoCard`

`class` at `plugin/juce/Pages.h:359` through line 417.

Fields and enumerators:

- `ParamKnob rate_;`
- `ParamKnob depth_;`
- `ParamKnob rateFade_;`
- `ParamKnob depthFade_;`
- `ParamChoice wave_;`
- `P1Toggle mode_;`
- `bool compact_ = false;`

Methods:

- `LfoCard(PluginEditor& editor, juce::String title, juce::String rateID, juce::String depthID, juce::String waveID, juce::String modeID, juce::String rateFadeID, juce::String depthFadeID, int op, int...`
- `void resized() override`

### `FilterCard`

`class` at `plugin/juce/PagesFxMod.cpp:12` through line 61.

Fields and enumerators:

- `ParamKnob cutoff_;`
- `ParamKnob res_;`
- `ParamChoice type_;`
- `P1Toggle on_;`
- `P1Button to_;`

Methods:

- `FilterCard(PluginEditor& editor, juce::String title, juce::String cutoffID, juce::String resID, juce::String typeID, juce::String onID, std::function<void()> onToPart = {}, juce::String toLabel = "...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`

### `FilterEnvelopeCard`

`class` at `plugin/juce/PagesFxMod.cpp:63` through line 142.

Fields and enumerators:

- `int part_ = 0;`
- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `P1Button to_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `FilterEnvelopeCard(PluginEditor& editor, int part, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("filter envelope"), part_(part), envView_(editor.state(), {params::fi...`
- `void resized() override`
- `void tick()`

### `PitchLfoCard`

`class` at `plugin/juce/PagesFxMod.cpp:144` through line 203.

Fields and enumerators:

- `int part_ = 0;`
- `P1Wave waveView_;`
- `ParamKnob rate_;`
- `ParamKnob depth_;`
- `ParamKnob rateFade_;`
- `ParamKnob depthFade_;`
- `ParamChoice wave_;`
- `P1Toggle mode_;`
- `P1Button to_;`

Methods:

- `explicit PitchLfoCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch lfo"), part_(editor.activePart()), waveView_(editor.state(), params::...`
- `void resized() override`
- `void tick()`

### `PitchEnvelopeCard`

`class` at `plugin/juce/PagesFxMod.cpp:205` through line 283.

Fields and enumerators:

- `int part_ = 0;`
- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `P1Button to_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `explicit PitchEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch envelope"), part_(editor.activePart()), envView_(editor.state(),...`
- `void resized() override`
- `void tick()`

### `PanEnvelopeCard`

`class` at `plugin/juce/PagesFxMod.cpp:285` through line 361.

Fields and enumerators:

- `int part_ = 0;`
- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `P1Button to_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `explicit PanEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pan envelope"), part_(editor.activePart()), envView_(editor.state(), {pa...`
- `void resized() override`
- `void tick()`

### `FollowCard`

`class` at `plugin/juce/PagesFxMod.cpp:363` through line 503.

Fields and enumerators:

- `PluginEditor& editor_;`
- `bool key_ = true;`
- `int part_ = 0;`
- `std::array<params::FollowTarget, 19> targets_{};`
- `P1Choice target_;`
- `P1Toggle on_;`
- `P1Button wipe_{"wipe all", true};`
- `P1Button to_;`
- `std::unique_ptr<ParamKnob> breakpoint_;`
- `std::unique_ptr<ParamKnob> below_;`
- `std::unique_ptr<ParamKnob> above_;`
- `std::unique_ptr<ParamKnob> amount_;`
- `int selected_ = 0;`

Methods:

- `FollowCard(PluginEditor& editor, bool key, juce::String title, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card(std::move(title)), editor_(editor), key_(key), part_(edit...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void refresh()`
- `juce::StringArray targetItems() const`
- `void updateTarget()`
- `void wipe()`

### `RingGraph`

`class` at `plugin/juce/PagesFxMod.cpp:505` through line 594.

Fields and enumerators:

- `int a_ = 0;`
- `int b_ = 0;`
- `bool directA_ = false;`
- `bool directB_ = false;`

Methods:

- `void setPair(int a, int b)`
- `void setDirect(bool a, bool b)`
- `void paint(juce::Graphics& g) override`

### `RingmodCard`

`class` at `plugin/juce/PagesFxMod.cpp:596` through line 757.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int part_ = 0;`
- `std::array<RingGraph, 3> graph_{};`
- `std::array<std::unique_ptr<P1Toggle>, 3> on_{};`
- `std::array<std::unique_ptr<ParamKnob>, 3> level_{};`
- `std::array<std::unique_ptr<P1Toggle>, 4> out_{};`
- `juce::Label noRing_;`
- `juce::Label original_;`
- `std::array<int, 4> carriers_{};`
- `int carrierCount_ = 0;`

Methods:

- `RingmodCard(PluginEditor& editor, int part) : P1Card("ringmod"), editor_(editor), part_(part)`
- `void resized() override`
- `void tick()`
- `bool carrierOutOn(int op) const`
- `bool isCarrier(int op) const`
- `void updateCarriers()`
- `void layout()`

### `FxModPage`

`class` at `plugin/juce/PagesFxMod.cpp:759` through line 839.

Fields and enumerators:

- `std::unique_ptr<FilterCard> filter1_;`
- `std::unique_ptr<FilterEnvelopeCard> filterEnv_;`
- `std::unique_ptr<PitchLfoCard> pitchLfo_;`
- `std::unique_ptr<PitchEnvelopeCard> pitchEnv_;`
- `std::unique_ptr<PanEnvelopeCard> panEnv_;`
- `std::unique_ptr<FollowCard> keyFollow_;`
- `std::unique_ptr<FollowCard> velFollow_;`
- `std::unique_ptr<RingmodCard> ringmod_;`

Methods:

- `explicit FxModPage(PluginEditor& editor)`
- `void resized() override`
- `void tick() override`

### `MacroRange`

`class` at `plugin/juce/PagesMacros.cpp:16` through line 107.

The compact bipolar range a macro card uses. The stored value sits at the right and the fill runs from the centre, so a destination or source range reads as a signed amount.

Fields and enumerators:

- `APVTS& state_;`
- `juce::String paramID_;`
- `juce::Colour fill_;`
- `const P1ParamSpec* spec_ = nullptr;`
- `float value_ = 0.0f;`
- `juce::Rectangle<int> track_;`
- `std::unique_ptr<juce::ParameterAttachment> attach_;`

Methods:

- `MacroRange(APVTS& state, juce::String paramID, juce::Colour fill) : state_(state), paramID_(std::move(paramID)), fill_(fill)`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void mouseDown(const juce::MouseEvent& event) override`
- `void mouseDrag(const juce::MouseEvent& event) override`
- `void mouseUp(const juce::MouseEvent&) override`
- `void setFromMouse(const juce::MouseEvent& event)`

### `MacroTargetChoice`

`class` at `plugin/juce/PagesMacros.cpp:111` through line 160.

The target menu with its eight section headings. The item id is the target enum plus one, so `none` is 1 and the parameter value is the id minus one.

Fields and enumerators:

- `APVTS& state_;`
- `juce::String paramID_;`
- `juce::ComboBox combo_;`
- `std::unique_ptr<juce::ParameterAttachment> attach_;`
- `bool updating_ = false;`

Methods:

- `MacroTargetChoice(APVTS& state, juce::String paramID) : state_(state), paramID_(std::move(paramID))`
- `void resized() override`

### `MacroCard`

`class` at `plugin/juce/PagesMacros.cpp:162` through line 293.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int index_ = 0;`
- `ParamKnob knob_;`
- `ParamChoice source_;`
- `MacroRange midiRange_;`
- `std::array<std::unique_ptr<MacroTargetChoice>, 3> targets_{};`
- `std::array<std::unique_ptr<MacroRange>, 3> ranges_{};`
- `P1Button rename_{"rename", true};`
- `juce::Rectangle<int> midiRow_;`
- `std::array<juce::Rectangle<int>, 3> destRows_{};`
- `std::array<bool, 3> destinationOn_{};`
- `bool midiOn_ = false;`
- `juce::String name_;`

Methods:

- `MacroCard(PluginEditor& editor, int index) : editor_(editor), index_(index), knob_(editor.state(), params::macroValue(index), "", 92.0f), source_(editor.state(), params::macroMidi(index), {"off", "...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`
- `juce::String title() const`

### `MacrosPage`

`class` at `plugin/juce/PagesMacros.cpp:295` through line 332.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<MacroCard>, 6> cards_{};`

Methods:

- `explicit MacrosPage(PluginEditor& editor) : editor_(editor)`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick() override`

### `OpColumn`

`class` at `plugin/juce/PagesOperators.cpp:25` through line 105.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `CardHeader header_;`
- `P1Scope scope_;`
- `P1Spectro spectro_;`
- `OscillatorCard osc_;`
- `AmplitudeEnvelopeCard amp_;`
- `FeedbackEnvelopeCard fb_;`
- `std::array<float, 512> scratch_{};`

Methods:

- `OpColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, on...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`

### `OpLfoColumn`

`class` at `plugin/juce/PagesOperators.cpp:107` through line 203.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `CardHeader header_;`
- `P1Scope scope_;`
- `P1Spectro spectro_;`
- `LfoCard ampLfo_;`
- `LfoCard feedbackLfo_;`
- `LfoCard detuneLfo_;`
- `std::array<float, 512> scratch_{};`

Methods:

- `OpLfoColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this,...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`

### `OperatorsPage`

`class` at `plugin/juce/PagesOperators.cpp:205` through line 236.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<OpColumn>, 4> columns_{};`

Methods:

- `explicit OperatorsPage(PluginEditor& editor) : editor_(editor)`
- `void resized() override`
- `void tick() override`

### `OperatorLfosPage`

`class` at `plugin/juce/PagesOperators.cpp:238` through line 269.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<OpLfoColumn>, 4> columns_{};`

Methods:

- `explicit OperatorLfosPage(PluginEditor& editor) : editor_(editor)`
- `void resized() override`
- `void tick() override`

### `PhaseMapView`

`class` at `plugin/juce/PagesPhase.cpp:36` through line 233.

The static parameter preview is the fallback until the part's map has been seeded. Once the engine publishes one, the same plot uses the real composed geometry and the newest voice's mapped phase.

Fields and enumerators:

- `APVTS& state_;`
- `TypePAlphaAudioProcessor& processor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `uint64_t lastSignature_ = 0;`
- `bool haveSignature_ = false;`

Methods:

- `PhaseMapView(PluginEditor& editor, int op, int part) : state_(editor.state()), processor_(editor.processor()), op_(op), part_(part)`
- `void tick()`
- `void paint(juce::Graphics& g) override`

### `PhaseCard`

`class` at `plugin/juce/PagesPhase.cpp:235` through line 288.

Fields and enumerators:

- `P1Toggle on_;`
- `std::array<std::unique_ptr<ParamKnob>, 9> knobs_{};`

Methods:

- `PhaseCard(PluginEditor& editor, int op, int part) : P1Card("phase", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phase); }, [&ed = editor, op, part] {...`
- `void resized() override`

### `PhaseEnvelopeCard`

`class` at `plugin/juce/PagesPhase.cpp:290` through line 378.

Fields and enumerators:

- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `PhaseEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("phase env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phaseEnvelope); }, [&ed =...`
- `void resized() override`
- `void tick()`

### `CompactOscillator`

`class` at `plugin/juce/PagesPhase.cpp:380` through line 428.

Fields and enumerators:

- `ParamKnob ratio_;`
- `ParamKnob detune_;`
- `ParamKnob level_;`
- `ParamKnob feedback_;`
- `ParamChoice wave_;`

Methods:

- `CompactOscillator(PluginEditor& editor, int op, int part) : ratio_(editor.state(), params::opRatio(op, part), "ratio", 22.0f), detune_(editor.state(), params::opDetune(op, part), "detune", 22.0f), ...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`

### `PhaseColumn`

`class` at `plugin/juce/PagesPhase.cpp:430` through line 513.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `CardHeader header_;`
- `CompactOscillator osc_;`
- `PhaseMapView map_;`
- `PhaseCard phase_;`
- `LfoCard lfo_;`
- `PhaseEnvelopeCard env_;`

Methods:

- `PhaseColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this,...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`

### `PhaseMapPage`

`class` at `plugin/juce/PagesPhase.cpp:515` through line 550.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<PhaseColumn>, 4> columns_{};`

Methods:

- `explicit PhaseMapPage(PluginEditor& editor) : editor_(editor)`
- `void resized() override`
- `void tick() override`

### `SpringSlider`

`class` at `plugin/juce/PagesSpectral.cpp:89` through line 183.

Fields and enumerators:

- `juce::String label_;`
- `double amount_ = 0.0;`
- `float startY_ = 0.0f;`
- `bool dragging_ = false;`
- `std::function<void()> onBegin_;`
- `std::function<void(double)> onChange_;`
- `std::function<void()> onEnd_;`

Methods:

- `SpringSlider(juce::String label, std::function<void()> onBegin, std::function<void(double)> onChange, std::function<void()> onEnd = {}) : label_(std::move(label)), onBegin_(std::move(onBegin)), onC...`
- `void paint(juce::Graphics& g) override`
- `void mouseDown(const juce::MouseEvent& event) override`
- `void mouseDrag(const juce::MouseEvent& event) override`
- `void mouseUp(const juce::MouseEvent&) override`
- `juce::String valueText() const`
- `void setAmount(double amount)`

### `MaskRow`

`class` at `plugin/juce/PagesSpectral.cpp:185` through line 235.

Fields and enumerators:

- `std::function<void(int)> onChange;`
- `int index_ = 2;`

Methods:

- `explicit MaskRow(int initial) : index_(juce::jlimit(0, 2, initial))`
- `int index() const`
- `void paint(juce::Graphics& g) override`
- `void mouseDown(const juce::MouseEvent& event) override`

### `MorphEnvelopeCard`

`class` at `plugin/juce/PagesSpectral.cpp:237` through line 323.

Fields and enumerators:

- `P1Envelope env_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `MorphEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("morph env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::spectrumEnvelope); }, [&e...`
- `void resized() override`
- `void tick()`

### `SpectrumRow`

`class` at `plugin/juce/PagesSpectral.cpp:325` through line 730.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `bool isA_ = true;`
- `P1SpectralBars bars_;`
- `ParamChoice range_;`
- `juce::Label rangeLabel_;`
- `MaskRow mask_;`
- `juce::Label maskLabel_;`
- `P1Choice selection_;`
- `juce::Label selectionLabel_;`
- `P1Choice group_;`
- `juce::Label groupLabel_;`
- `P1Button template_{"load spectrum template...", true};`
- `P1Button norm_{"norm", true};`
- `P1Button toOther_{"to b", true};`
- `SpringSlider tilt_;`
- `SpringSlider level_;`
- `SpringSlider formant_;`
- `juce::ComboBox centre_;`
- `juce::Label legend_;`
- `std::vector<double> snapshot_;`
- `int lastFirst_ = -1;`

Methods:

- `SpectrumRow(PluginEditor& editor, int op, int part, bool a) : P1Card("op" + juce::String(op + 1) + " spectrum " + (a ? "a" : "b"), [&ed = editor, op, part, a] { ed.copyOperatorSection( op, part, a ...`
- `void tick()`
- `void resized() override`
- `int firstHarmonic() const`
- `void refreshGroupTest()`
- `std::pair<int, int> windowFor(int index) const`
- `void refreshSelectionItems()`
- `void applySelection()`
- `bool editable(int harmonic, double level) const`
- `bool selected(int index) const`
- `void beginMacro()`
- `void endMacro()`
- `void applyTilt(double amount)`
- `void applyLevel(double amount)`
- `void applyFormant(double amount)`
- `void normalise()`
- `void copyToOther()`
- `void showTemplateMenu()`
- `void loadTemplate(int wave)`
- `void refreshLegend()`

### `SpectraOperatorCard`

`class` at `plugin/juce/PagesSpectral.cpp:732` through line 804.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `CardHeader header_;`
- `P1Scope scope_;`
- `P1Spectro spectro_;`
- `OscillatorCard osc_;`
- `std::array<float, 512> scratch_{};`

Methods:

- `SpectraOperatorCard(PluginEditor& editor, int op) : editor_(editor), op_(op), part_(editor.activePart()), header_( false, false, {}, [&ed = editor, op] { ed.copyOperator(op); }, [&ed = editor, op] ...`
- `void resized() override`
- `void tick()`
- `void paint(juce::Graphics& g) override`

### `SpectraModColumn`

`class` at `plugin/juce/PagesSpectral.cpp:806` through line 896.

Fields and enumerators:

- `PluginEditor& editor_;`
- `int op_ = 0;`
- `int part_ = 0;`
- `CardHeader header_;`
- `OscillatorCard osc_;`
- `P1SpectrumDisplay display_;`
- `MorphEnvelopeCard env_;`
- `LfoCard lfo_;`

Methods:

- `SpectraModColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [...`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`

### `OperatorSpectraPage`

`class` at `plugin/juce/PagesSpectral.cpp:898` through line 950.

Fields and enumerators:

- `std::array<std::unique_ptr<SpectraOperatorCard>, 4> opcards_{};`
- `std::array<std::unique_ptr<SpectrumRow>, 4> rows_{};`
- `juce::Component content_;`
- `juce::Viewport viewport_;`

Methods:

- `OperatorSpectraPage(PluginEditor& editor, bool a)`
- `void resized() override`
- `void tick() override`

### `OperatorSpectraModPage`

`class` at `plugin/juce/PagesSpectral.cpp:952` through line 984.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<SpectraModColumn>, 4> columns_{};`

Methods:

- `explicit OperatorSpectraModPage(PluginEditor& editor) : editor_(editor)`
- `void resized() override`
- `void tick() override`

### `PatchButton`

`class` at `plugin/juce/PluginEditor.cpp:17` through line 36.

Methods:

- `PatchButton() : juce::Button("multi-patch library...")`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override`

### `SegmentButton`

`class` at `plugin/juce/PluginEditor.cpp:38` through line 70.

Fields and enumerators:

- `bool active_ = false;`

Methods:

- `explicit SegmentButton(const juce::String& label) : juce::Button(label)`
- `void setActive(bool active)`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override`

### `PartChoiceButton`

`class` at `plugin/juce/PluginEditor.cpp:72` through line 104.

Fields and enumerators:

- `bool active_ = false;`

Methods:

- `explicit PartChoiceButton(const juce::String& label) : juce::Button(label)`
- `void setActive(bool active)`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override`

### `Header`

`class` at `plugin/juce/PluginEditor.cpp:108` through line 267.

Fields and enumerators:

- `PluginEditor& editor_;`
- `p1::ui::P1AlgorithmBlock alg_;`
- `PatchButton patch_;`
- `p1::ui::P1Button rename_{"rename"};`
- `p1::ui::P1Scope scopeView_;`
- `p1::ui::P1Spectro spectroView_;`
- `p1::ui::P1VolumeSlider volume_;`
- `SegmentButton scopeSeg_{"scope"};`
- `SegmentButton spectroSeg_{"spectro"};`
- `bool spectroMode_ = false;`
- `std::array<float, 512> scratch_{};`
- `std::array<float, 512> lastScratch_{};`
- `bool haveScratch_ = false;`
- `juce::String multiName_{"init patch"};`

Methods:

- `explicit Header(PluginEditor& editor) : editor_(editor), volume_(editor.state(), p1::kVolumeParamID)`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void tick()`
- `void setPatchName(const juce::String& name)`
- `void setSpectroMode(bool spectro)`
- `void drawWordmark(juce::Graphics& g)`

### `Toolbar`

`class` at `plugin/juce/PluginEditor.cpp:269` through line 393.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<p1::ui::P1MuteTile>, 4> opTiles_{};`
- `std::array<std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment>, 4> opAttach_{};`
- `std::array<std::unique_ptr<p1::ui::P1MuteTile>, 2> partTiles_{};`
- `std::array<std::unique_ptr<PartChoiceButton>, 2> partButtons_{};`
- `PartChoiceButton edit_{"part library actions..."};`

Methods:

- `explicit Toolbar(PluginEditor& editor) : editor_(editor)`
- `void resized() override`
- `void paint(juce::Graphics& g) override`
- `void refreshPartNames()`
- `void tick()`
- `void drawLegend(juce::Graphics& g)`

### `ListBody`

`class` at `plugin/juce/PluginEditor.cpp:400` through line 460.

Fields and enumerators:

- `virtual void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) = 0;`
- `int count_ = 0;`
- `int selected_ = 0;`
- `juce::ListBox list_;`

Methods:

- `ListBody(int count, int rowHeight, int selected, const juce::String& name) : count_(juce::jmax(0, count)), selected_(juce::jlimit(0, juce::jmax(0, count_ - 1), selected)), list_(name, this)`
- `int selected() const`
- `void resized() override`
- `int getNumRows() override`
- `void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override`
- `void listBoxItemClicked(int row, const juce::MouseEvent&) override`

### `SlotListBody`

`class` at `plugin/juce/PluginEditor.cpp:462` through line 494.

Fields and enumerators:

- `std::function<juce::String(int)> nameFor_;`
- `std::function<bool(int)> usedFor_;`

Methods:

- `SlotListBody(int count, std::function<juce::String(int)> nameFor, std::function<bool(int)> usedFor, int selected) : ListBody(count, 24, selected, "slots"), nameFor_(std::move(nameFor)), usedFor_(st...`
- `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override`

### `ModeListBody`

`class` at `plugin/juce/PluginEditor.cpp:496` through line 516.

Fields and enumerators:

- `juce::StringArray modes_;`

Methods:

- `ModeListBody(juce::StringArray modes, int selected) : ListBody(modes.size(), 28, selected, "modes"), modes_(std::move(modes))`
- `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override`

### `AlgTile`

`class` at `plugin/juce/PluginEditor.cpp:518` through line 562.

Fields and enumerators:

- `int index_ = 0;`
- `bool selected_ = false;`
- `std::function<void(int)> onSelect_;`

Methods:

- `AlgTile(int index, std::function<void(int)> onSelect) : juce::Button("alg"), index_(index), onSelect_(std::move(onSelect))`
- `void setSelected(bool selected)`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override`
- `void mouseUp(const juce::MouseEvent&) override`

### `AlgPickerBody`

`class` at `plugin/juce/PluginEditor.cpp:564` through line 610.

Fields and enumerators:

- `int selected_ = 4;`
- `std::array<std::unique_ptr<AlgTile>, 10> tiles_{};`
- `juce::Label note_;`

Methods:

- `explicit AlgPickerBody(int selected) : selected_(juce::jlimit(0, 9, selected))`
- `int selected() const`
- `void resized() override`
- `void select(int index)`

### `RenameBody`

`class` at `plugin/juce/PluginEditor.cpp:612` through line 645.

Fields and enumerators:

- `juce::TextEditor editor_;`
- `juce::Label note_;`

Methods:

- `explicit RenameBody(const juce::String& initial)`
- `juce::String text() const`
- `void focus()`
- `void resized() override`

### `Header`

`class forward declaration` at `plugin/juce/PluginEditor.h:16` through line 16.

### `Toolbar`

`class forward declaration` at `plugin/juce/PluginEditor.h:17` through line 17.

### `P1Canvas`

`class` at `plugin/juce/PluginEditor.h:21` through line 24.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`

### `PageBase`

`class` at `plugin/juce/PluginEditor.h:26` through line 30.

Fields and enumerators:

- `virtual void tick() = 0;`

Methods:

- `~PageBase() override = default;`

### `PluginEditor`

`class` at `plugin/juce/PluginEditor.h:34` through line 147.

The JUCE editor for the whole panel.

Fields and enumerators:

- `enum class PartFxSection { filter, filterEnvelope, pitchLfo, pitchEnvelope, panEnvelope, keyFollow, velocityFollow, };`
- `enum class OperatorSection { oscillator, amplitudeEnvelope, feedbackEnvelope, amplitudeLfo, feedbackLfo, detuneLfo, phase, phaseLfo, phaseEnvelope, spectrumA, spectrumB, spectrumEnvelope, spectrumL...`
- `void paint(juce::Graphics& g) override;`
- `void resized() override;`
- `void timerCallback() override;`
- `void setActivePart(int part);`
- `void showTab(int tab);`
- `void refreshNames();`
- `void reportLibrarySave(const LibrarySaveResult& result);`
- `void showAlgorithmPicker();`
- `void applyAlgorithm(int algorithm);`
- `void showRenameDialog(bool part, int index);`
- `void showRenameMacro(int index);`
- `void showSinglePartsBrowser();`
- `void exportLibrary();`
- `void importLibrary();`
- `void showPatchBrowser();`
- `void showPatchSaveMode(int slot);`
- `void copyPart(int part);`
- `void pastePart(int part);`
- `void copyOperator(int op);`
- `void pasteOperator(int op);`
- `void copyOperatorSection(int op, int part, OperatorSection section);`
- `void pasteOperatorSection(int op, int part, OperatorSection section);`
- `void copyPartFxSection(PartFxSection section);`
- `void initPart(int part);`
- `void initMultiPatch();`
- `void togglePartMute(int part, bool muted);`
- `void savePartToSlot(int part, int slot);`
- `void loadPartFromSlot(int part, int slot);`
- `void savePatchToSlot(int slot, int mode = 0);`
- `void loadPatchFromSlot(int slot);`
- `void showConfirm(const juce::String& message, std::function<void()> onConfirm, const juce::String& title = "confirm save", const juce::String& confirm = "save");`
- `void closeDialog();`
- `void updateVisualizers();`
- `void buildUi();`
- `void buildHeader();`
- `void buildToolbar();`
- `void buildPage();`
- `void refreshMacroFeedback(juce::Component& root);`
- ... and 32 more fields.

Methods:

- `TypePAlphaAudioProcessor& processor()`
- `juce::AudioProcessorValueTreeState& state()`
- `int activePart() const`
- `int activeTab() const`

### `PartFxSection`

`enum` at `plugin/juce/PluginEditor.h:36` through line 44.

Fields and enumerators:

- `filter`
- `filterEnvelope`
- `pitchLfo`
- `pitchEnvelope`
- `panEnvelope`
- `keyFollow`
- `velocityFollow`

### `OperatorSection`

`enum` at `plugin/juce/PluginEditor.h:46` through line 60.

Fields and enumerators:

- `oscillator`
- `amplitudeEnvelope`
- `feedbackEnvelope`
- `amplitudeLfo`
- `feedbackLfo`
- `detuneLfo`
- `phase`
- `phaseLfo`
- `phaseEnvelope`
- `spectrumA`
- `spectrumB`
- `spectrumEnvelope`
- `spectrumLfo`

### `LockRelease`

`struct` at `plugin/juce/PluginProcessor.cpp:973` through line 976.

Fields and enumerators:

- `juce::InterProcessLock& lock;`

Methods:

- `~LockRelease()`

### `ScopeRing`

`class` at `plugin/juce/PluginProcessor.h:22` through line 33.

The single-producer/single-consumer scope ring.

Fields and enumerators:

- `void push(const float* data, int count);`
- `void read(float* destination, int count);`
- `static constexpr int kCapacity = 2048;`
- `juce::AbstractFifo fifo_{kCapacity};`
- `std::array<float, kCapacity> buffer_{};`
- `std::array<float, kCapacity> scratch_{};`

### `LibrarySaveResult`

`struct` at `plugin/juce/PluginProcessor.h:39` through line 47.

The outcome of a library save. A conflict means another instance wrote the slot first, so the disk version stayed and the local edit is still only in memory. `unreadable` means the file exists but could not be parsed, so nothing was written over it.

Fields and enumerators:

- `bool written = false;`
- `bool unreadable = false;`
- `bool conflict = false;`
- `bool busy = false;`
- `bool multi = false;`
- `int slot = -1;`
- `std::string name;`

### `TypePAlphaAudioProcessor`

`class` at `plugin/juce/PluginProcessor.h:49` through line 193.

The JUCE audio processor and the host-facing plugin object.

Fields and enumerators:

- `void prepareToPlay(double sampleRate, int samplesPerBlock) override;`
- `void releaseResources() override;`
- `bool isBusesLayoutSupported(const BusesLayout& layouts) const override;`
- `void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override;`
- `juce::AudioProcessorEditor* createEditor() override;`
- `void getStateInformation(juce::MemoryBlock& destData) override;`
- `void setStateInformation(const void* data, int sizeInBytes) override;`
- `void ensureLibraryLoaded();`
- `void resetPhaseMaps();`
- `LibrarySaveResult saveLibrary();`
- `juce::File libraryFile() const;`
- `void readOutputScope(float* destination, int count);`
- `void readTapScope(int channel, float* destination, int count);`
- `double specMorph(int op, int part) const;`
- `bool phaseMap(int op, int part, p1::PhaseMapSnapshot& out) const;`
- `juce::String getPartName(int part) const;`
- `void setPartName(int part, const juce::String& name);`
- `juce::String getMacroName(int index) const;`
- `void setMacroName(int index, const juce::String& name);`
- `double macroEffectiveValue(int index) const;`
- `struct MacroFeedback { bool targeted = false; bool hasEffective = false; double effective = 0.0; };`
- `MacroFeedback macroFeedback(int index) const;`
- `static juce::AudioProcessorValueTreeState::ParameterLayout createLayout();`
- `static void feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message);`
- `void syncParams();`
- `void syncMacros();`
- `void renderCore(int32_t* out, int frames);`
- `void pushTapScopes(p1::Engine& engine, int frames);`
- `void publishMorph(p1::Engine& engine);`
- `void publishPhaseMaps(p1::Engine& engine);`
- `void feedScheduled(int64_t limit);`
- `void flushDeferred();`
- `static void coreChunk(void* context, p1::Engine& engine, int frames);`
- `struct ScheduledEvent { juce::MidiMessage message; int64_t coreFrame = 0; };`
- `p1::Engine engine_;`
- `p1::MidiBridge midi_;`
- `p1::RateBridge rateBridge_;`
- `juce::AudioProcessorValueTreeState apvts_;`
- `std::array<int32_t, kP1ParamCount> lastParams_{};`
- `std::array<std::atomic<float>*, kP1ParamCount> paramValues_{};`
- ... and 41 more fields.

Methods:

- `~TypePAlphaAudioProcessor() override = default;`
- `bool hasEditor() const override`
- `const juce::String getName() const override`
- `bool acceptsMidi() const override`
- `bool producesMidi() const override`
- `bool isMidiEffect() const override`
- `double getTailLengthSeconds() const override`
- `int getNumPrograms() override`
- `int getCurrentProgram() override`
- `void setCurrentProgram(int) override`
- `const juce::String getProgramName(int) override`
- `void changeProgramName(int, const juce::String&) override`
- `void setLibraryFileForTesting(const juce::File& file)`
- `juce::AudioProcessorValueTreeState& parameters()`
- `p1::PresetStore& presets()`
- `const p1::PresetStore& presets() const`
- `juce::String getPatchName() const`
- `void setPatchName(const juce::String& name)`
- `int getUiActivePart() const`
- `void setUiActivePart(int part)`
- `int getUiPage() const`
- `void setUiPage(int page)`

### `MacroFeedback`

`struct` at `plugin/juce/PluginProcessor.h:108` through line 112.

What a macro destination does to one parameter right now. The stored value stays in the parameter. The `effective` value is the raw value the engine receives, which differs whenever a macro with a range deviates from its knob.

Fields and enumerators:

- `bool targeted = false;`
- `bool hasEffective = false;`
- `double effective = 0.0;`

### `ScheduledEvent`

`struct` at `plugin/juce/PluginProcessor.h:135` through line 138.

Fields and enumerators:

- `juce::MidiMessage message;`
- `int64_t coreFrame = 0;`

### `SpecBlock`

`struct` at `plugin/preset/PresetStore.cpp:723` through line 727.

The spectral blocks: version 14 target, version 13 rates base and the separate version 13 depth.

Fields and enumerators:

- `int targetBase = -1;`
- `int oldBase = -1;`
- `int depthBase = -1;`

### `PresetStore`

`class` at `plugin/preset/PresetStore.hpp:15` through line 223.

The in-memory preset bank. The byte layout comes from the engine's presets.c record functions, so a plugin bank round-trips with the device.

Fields and enumerators:

- `static constexpr int kPartSlots = 128;`
- `static constexpr int kMultiSlots = 64;`
- `static constexpr int kNameLen = 32;`
- `static constexpr int kMacroCount = 6;`
- `static constexpr int kMacroParams = 54;`
- `struct Part { bool used = false; std::string name; std::vector<int32_t> params; };`
- `struct Multi { bool used = false; std::string name; std::vector<uint8_t> partSlot; std::vector<int32_t> level; std::vector<int32_t> delay; std::vector<int32_t> channel; std::vector<int32_t> low; st...`
- `std::size_t partRecordSize() const;`
- `std::size_t multiRecordSize() const;`
- `bool savePart(int slot, const std::string& name, const std::vector<int32_t>& params);`
- `bool loadPart(int slot, std::string& name, std::vector<int32_t>& params) const;`
- `bool partUsed(int slot) const;`
- `std::string partName(int slot) const;`
- `const std::vector<int32_t>& partParams(int slot) const;`
- `bool saveMulti(int slot, const std::string& name, const std::vector<uint8_t>& partSlot, const std::vector<int32_t>& level, const std::vector<int32_t>& delay, const std::vector<int32_t>& channel, co...`
- `bool loadMulti(int slot, Multi& out) const;`
- `bool multiUsed(int slot) const;`
- `std::string multiName(int slot) const;`
- `std::vector<uint8_t> packPartBank() const;`
- `bool loadPartBank(const uint8_t* data, std::size_t size);`
- `std::vector<uint8_t> packMultiBank() const;`
- `bool loadMultiBank(const uint8_t* data, std::size_t size);`
- `std::vector<uint8_t> packLibrary() const;`
- `bool loadLibrary(const uint8_t* data, std::size_t size);`
- `struct Conflict { bool multi = false; int slot = -1; std::string name; };`
- `std::vector<Conflict> mergeInto(PresetStore& disk);`
- `void adoptLibrary(const PresetStore& other);`
- `void markSaved();`
- `bool hasChanges() const;`
- `int findFreePartSlot() const;`
- `int findFreeMultiSlot() const;`
- `void setCurrentMulti(int slot);`
- `int partSource(int part) const;`
- `void setPartSource(int part, int slot);`
- `static std::string shortName(const std::string& name);`
- `int toVersion12Index(int targetIndex) const;`
- `int toVersion14Index(int targetIndex) const;`
- `int toVersion13Index(int targetIndex) const;`
- `int toVersion13aIndex(int targetIndex) const;`
- `bool migratePartBank(const uint8_t* data, uint32_t oldCount, std::vector<uint8_t>& out) const;`
- ... and 22 more fields.

Methods:

- `int partParamCount() const`
- `int partCount() const`
- `uint32_t revision() const`
- `void setRevision(uint32_t revision)`
- `void setOperatorEnvelopeIndices(const std::array<int, 4>& indices)`
- `void setPanEnvelopeIndex(int index)`
- `void setOffsetEnvelopeIndices(const std::array<int, 4>& feedback, int pitch)`
- `void setAbsEnvelopeIndices(int filter, const std::array<int, 4>& spectral)`
- `void setLfoFadeIndices(const std::array<int, 34>& indices)`
- `void setInsertionIndices(int algorithm, const std::array<int, 4>& waves)`
- `int currentMulti() const`

### `Part`

`struct` at `plugin/preset/PresetStore.hpp:23` through line 27.

One part record in the preset store.

Fields and enumerators:

- `bool used = false;`
- `std::string name;`
- `std::vector<int32_t> params;`

### `Multi`

`struct` at `plugin/preset/PresetStore.hpp:29` through line 42.

One multi-patch record in the preset store.

Fields and enumerators:

- `bool used = false;`
- `std::string name;`
- `std::vector<uint8_t> partSlot;`
- `std::vector<int32_t> level;`
- `std::vector<int32_t> delay;`
- `std::vector<int32_t> channel;`
- `std::vector<int32_t> low;`
- `std::vector<int32_t> high;`
- `std::vector<int32_t> macro;`
- `std::array<std::string, kMacroCount> macroNames;`

### `Conflict`

`struct` at `plugin/preset/PresetStore.hpp:87` through line 91.

A slot that changed on disk since this instance loaded it. `multi` says which bank, and the name is the local name so the message can quote it.

Fields and enumerators:

- `bool multi = false;`
- `int slot = -1;`
- `std::string name;`

### `WaveTable`

`struct` at `plugin/tests/additive_bench.cpp:245` through line 249.

Fields and enumerators:

- `int size = 0;`
- `int harmonics = 0;`
- `std::vector<std::vector<int16_t>> mips;`

### `Spectrum`

`struct` at `plugin/tests/feedback_bipolar.cpp:85` through line 90.

Fields and enumerators:

- `std::array<double, 9> a{};`
- `double dc = 0.0;`
- `double hf = 0.0;`
- `double peak = 0.0;`

### `Window`

`struct` at `plugin/tests/feedback_envelope.cpp:91` through line 94.

Fields and enumerators:

- `std::array<double, 7> a{};`
- `double centroid = 0.0;`

### `Ramp`

`struct` at `plugin/tests/lfo_fades.cpp:121` through line 126.

Fields and enumerators:

- `int rateEnd = -1;`
- `int depthEnd = -1;`
- `uint32_t rateFirst = 0;`
- `uint32_t depthFirst = 0;`

### `Scenario`

`struct` at `plugin/tests/phase_map_perceptual.cpp:54` through line 79.

Fields and enumerators:

- `std::string id;`
- `int note = 60;`
- `int note1 = -1;`
- `int chord = -1;`
- `int wave = 12;`
- `int bp = 500;`
- `int segments = 8;`
- `int flip = 0;`
- `int flipHz = 0;`
- `int reorder = 0;`
- `int reorderHz = 0;`
- `int jump = 1;`
- `int drift = 0;`
- `int driftHz = 0;`
- `int morph = 0;`
- `int morphSweep = 0;`
- `int lfoDepth = 0;`
- `int lfoHz = 0;`
- `int lfoWave = 0;`
- `int lfoMode = 0;`
- `int envOn = 0;`
- `int envMode = 0;`
- `int releaseAt = -1;`
- `int frames = 36000;`

### `Transfer`

`struct` at `plugin/tests/phase_map_perceptual.cpp:293` through line 297.

Fields and enumerators:

- `uint32_t maxAbs = 0u;`
- `double rms = 0.0;`
- `uint32_t worstX = 0u;`

### `Case`

`struct` at `plugin/tests/phase_map_perceptual.cpp:373` through line 377.

Fields and enumerators:

- `uint32_t n;`
- `uint32_t step;`
- `bool linear;`

### `Scenario`

`struct` at `plugin/tests/phase_map_perceptual_compare.cpp:52` through line 69.

Fields and enumerators:

- `std::string id;`
- `int note = 0;`
- `int note1 = -1;`
- `int chord = -1;`
- `int wave = 0;`
- `int bp = 0;`
- `int segments = 0;`
- `int flip = 0, flipHz = 0;`
- `int reorder = 0, reorderHz = 0, jump = 0;`
- `int drift = 0, driftHz = 0;`
- `int morph = 0, morphSweep = 0;`
- `int lfoDepth = 0, lfoHz = 0, lfoWave = 0, lfoMode = 0;`
- `int envOn = 0, envMode = 0;`
- `int releaseAt = -1;`
- `int frames = 0;`
- `double f0 = 0.0;`

### `Manifest`

`struct` at `plugin/tests/phase_map_perceptual_compare.cpp:71` through line 76.

Fields and enumerators:

- `int mode = -1;`
- `int sampleRate = 0;`
- `int channels = 0;`
- `std::vector<Scenario> scenarios;`

### `Metrics`

`struct` at `plugin/tests/phase_map_perceptual_compare.cpp:78` through line 98.

Fields and enumerators:

- `double nullDb = 0.0;`
- `double rmsDeltaDb = 0.0;`
- `double peakDeltaDb = 0.0;`
- `double lsdMax = 0.0;`
- `double lsdP95 = 0.0;`
- `double bandMax = 0.0;`
- `double centroidPct = 0.0;`
- `double rolloffPct = 0.0;`
- `double flatness = 0.0;`
- `double envMax = 0.0;`
- `double envP95 = 0.0;`
- `double harmMax = 0.0;`
- `double aliasDelta = 0.0;`
- `double attackMs = 0.0;`
- `double releaseMs = 0.0;`
- `bool haveAttack = false;`
- `bool haveRelease = false;`
- `int frames = 0;`
- `bool bitExact = false;`

### `Fft`

`class` at `plugin/tests/phase_map_perceptual_compare.cpp:242` through line 289.

Fields and enumerators:

- `int n_;`
- `std::vector<int> rev_;`
- `std::vector<std::complex<double>> tw_;`

Methods:

- `explicit Fft(int n) : n_(n), rev_(static_cast<std::size_t>(n)), tw_(static_cast<std::size_t>(n))`
- `void run(std::vector<std::complex<double>>& a) const`

### `Frame`

`struct` at `plugin/tests/phase_map_perceptual_compare.cpp:306` through line 311.

Fields and enumerators:

- `double centroid = 0.0;`
- `double rolloff = 0.0;`
- `double flatness = 0.0;`
- `std::vector<double> band;`

### `Analysis`

`struct` at `plugin/tests/phase_map_perceptual_compare.cpp:313` through line 321.

Fields and enumerators:

- `std::vector<double> rms;`
- `std::vector<double> peak;`
- `std::vector<Frame> frames;`
- `std::vector<std::vector<double>> harmonic;`
- `std::vector<double> alias;`
- `double totalPower = 0.0;`
- `double peakPower = 0.0;`

### `Gate`

`struct` at `plugin/tests/phase_map_perceptual_compare.cpp:734` through line 738.

Fields and enumerators:

- `const char* name;`
- `double value;`
- `double limit;`

### `Peak`

`struct` at `plugin/tests/plugin_stability.cpp:27` through line 30.

Fields and enumerators:

- `bool finite = true;`
- `float peak = 0.0f;`

### `Measurement`

`struct` at `plugin/tests/rate_bridge_test.cpp:18` through line 22.

Fields and enumerators:

- `std::vector<float> left;`
- `std::vector<float> right;`
- `int latency = 0;`

### `TickResult`

`struct` at `plugin/tests/spectral_bench.cpp:30` through line 34.

Fields and enumerators:

- `double mean = 0.0;`
- `double worst = 0.0;`
- `int over50us = 0;`

### `BlockResult`

`struct` at `plugin/tests/spectral_bench.cpp:112` through line 116.

Fields and enumerators:

- `double changeMean = 0.0;`
- `double changeWorst = 0.0;`
- `double steadyMean = 0.0;`

### `ParamDoc`

`class` at `plugin/tools/gen_params.py:18` through line 32.

One generated manifest row.

### `ParamsDoc`

`class` at `plugin/tools/gen_params.py:35` through line 40.

The generated params.json document.

### `_TokenFile`

`class` at `plugin/tools/gen_tokens.py:15` through line 21.

### `FollowTarget`

`struct` at `plugin/ui/ParamIDs.h:344` through line 349.

Fields and enumerators:

- `const char* name;`
- `juce::String keyLo;`
- `juce::String keyHi;`
- `juce::String velocity;`

### `P1LookAndFeel`

`class` at `plugin/ui/Theme.h:31` through line 51.

Fields and enumerators:

- `void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override;`
- `void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override;`
- `void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override;`
- `void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce...`
- `void drawScrollbar(juce::Graphics& g, juce::ScrollBar& bar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown)...`

### `KnobLookAndFeel`

`class` at `plugin/ui/Theme.h:53` through line 58.

Fields and enumerators:

- `void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;`

### `Item`

`struct` at `plugin/ui/Widgets.cpp:2133` through line 2136.

Fields and enumerators:

- `int index = 0;`
- `int width = 0;`

### `Layout`

`struct` at `plugin/ui/Widgets.cpp:2185` through line 2192.

Fields and enumerators:

- `float x[4];`
- `float y[4];`
- `int edges[4][2];`
- `int edgeCount;`
- `int cars[4];`
- `int carCount;`

### `RingBox`

`struct` at `plugin/ui/Widgets.cpp:2208` through line 2213.

Fields and enumerators:

- `float cx = 0.0f;`
- `float y = 0.0f;`
- `int a = 0;`
- `int b = 0;`

### `MacroFeedbackTarget`

`class` at `plugin/ui/Widgets.h:35` through line 41.

A control whose parameter can be a macro destination. The editor asks the processor once per refresh and the control draws the stored value with the effective value beside it while a macro deviates.

Fields and enumerators:

- `virtual int macroParamIndex() const = 0;`
- `virtual void setMacroFeedback(bool targeted, bool hasEffective, double effective) = 0;`

Methods:

- `virtual ~MacroFeedbackTarget() = default;`

### `ParamValueLabel`

`class` at `plugin/ui/Widgets.h:46` through line 57.

The value line of a knob. A plain value draws in its stored colour. When a targeted parameter has an effective value, the stored value keeps its unit and the effective value follows in smaller green type without a unit.

Fields and enumerators:

- `void setStoredColour(juce::Colour colour);`
- `void setEffectiveText(const juce::String& text, juce::Colour colour);`
- `void paint(juce::Graphics& g) override;`
- `juce::String effective_;`
- `juce::Colour storedColour_;`
- `juce::Colour effectiveColour_;`

### `ParamKnob`

`class` at `plugin/ui/Widgets.h:59` through line 122.

A parameter-bound rotary knob with a value line and macro feedback.

Fields and enumerators:

- `void setParamID(const juce::String& paramID);`
- `void setModArc(double fraction);`
- `void setMacroFeedback(bool targeted, bool hasEffective, double effective) override;`
- `void resized() override;`
- `void paint(juce::Graphics& g) override;`
- `std::function<void()> onUserEdit;`
- `std::function<void()> onUserEditEnd;`
- `class KnobSlider : public juce::Slider { public: bool keyPressed(const juce::KeyPress& key) override { if (onDetentKey && onDetentKey(key)) return true; return juce::Slider::keyPressed(key); } void...`
- `void updateValueText();`
- `void refreshValueDisplay();`
- `void commitValue();`
- `void snapValue();`
- `bool stepRatio(const juce::KeyPress& key);`
- `bool isRatio() const;`
- `APVTS& state_;`
- `juce::String paramID_;`
- `const P1ParamSpec* spec_ = nullptr;`
- `int specIndex_ = -1;`
- `KnobSlider slider_;`
- `juce::Label name_;`
- `ParamValueLabel value_;`
- `std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attach_;`
- `float dialSize_ = 42.0f;`
- `float labelSize_ = 0.0f;`
- `bool hasName_ = true;`
- `bool snapping_ = false;`
- `bool macroTargeted_ = false;`
- `bool macroHasEffective_ = false;`
- `double macroEffective_ = 0.0;`

Methods:

- `int macroParamIndex() const override`

### `KnobSlider`

`class` at `plugin/ui/Widgets.h:83` through line 98.

Arrow keys step a detented ratio knob to the next detent instead of nudging the raw value, which would snap straight back to where it was.

Fields and enumerators:

- `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;`
- `std::function<bool(const juce::KeyPress&)> onDetentKey;`
- `std::function<void()> onUserEdit;`
- `std::function<void()> onUserEditEnd;`

Methods:

- `bool keyPressed(const juce::KeyPress& key) override`

### `P1Button`

`class` at `plugin/ui/Widgets.h:124` through line 129.

Fields and enumerators:

- `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;`

### `P1MuteTile`

`class` at `plugin/ui/Widgets.h:131` through line 137.

Fields and enumerators:

- `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;`

### `P1Toggle`

`class` at `plugin/ui/Widgets.h:139` through line 149.

Fields and enumerators:

- `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;`
- `juce::String label_;`
- `std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> attach_;`

### `P1Choice`

`class` at `plugin/ui/Widgets.h:151` through line 154.

Methods:

- `P1Choice() = default;`

### `ParamChoice`

`class` at `plugin/ui/Widgets.h:156` through line 169.

Fields and enumerators:

- `void resized() override;`
- `void updateFromNormalized(float normalized);`
- `juce::ComboBox combo_;`
- `juce::RangedAudioParameter* param_ = nullptr;`
- `int min_ = 0;`
- `std::unique_ptr<juce::ParameterAttachment> attach_;`

### `P1TabBar`

`class` at `plugin/ui/Widgets.h:171` through line 196.

Fields and enumerators:

- `void setLabels(const juce::StringArray& labels);`
- `void setActive(int index);`
- `void setCompact(bool compact);`
- `void resized() override;`
- `void paint(juce::Graphics& g) override;`
- `std::function<void(int)> onTabChanged;`
- `class Tab : public juce::Button { public: Tab(const juce::String& text, int index, P1TabBar& owner); void paintButton(juce::Graphics& g, bool highlighted, bool down) override; private: int index_; ...`
- `std::vector<std::unique_ptr<Tab>> tabs_;`
- `juce::StringArray labels_;`
- `int active_ = 0;`
- `bool forcedCompact_ = false;`

### `Tab`

`class` at `plugin/ui/Widgets.h:182` through line 190.

Fields and enumerators:

- `void paintButton(juce::Graphics& g, bool highlighted, bool down) override;`
- `int index_;`
- `P1TabBar& owner_;`

### `P1Scope`

`class` at `plugin/ui/Widgets.h:198` through line 206.

Fields and enumerators:

- `void setSamples(const float* samples, int count);`
- `void paint(juce::Graphics& g) override;`
- `std::vector<float> samples_;`

### `P1Spectro`

`class` at `plugin/ui/Widgets.h:208` through line 222.

Fields and enumerators:

- `void setSamples(const float* samples, int count);`
- `void paint(juce::Graphics& g) override;`
- `static constexpr int kWindow = 512;`
- `static constexpr int kBars = 22;`
- `juce::dsp::FFT fft_{9};`
- `std::array<float, 1024> fftData_{};`
- `std::array<float, kWindow> window_{};`
- `std::array<int, kBars> barBin_{};`
- `std::vector<float> samples_;`

### `P1SpectralBars`

`class` at `plugin/ui/Widgets.h:224` through line 256.

The editable 64-bar spectral level display.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`
- `void mouseDown(const juce::MouseEvent& event) override;`
- `void mouseDrag(const juce::MouseEvent& event) override;`
- `std::vector<double> levels() const;`
- `void setLevels(const std::vector<double>& levels);`
- `void setFirstHarmonic(int first);`
- `void setGroupTest(std::function<bool(int)> test);`
- `void setEditWindow(int first, int last);`
- `void setEditTest(std::function<bool(int, double)> test);`
- `std::function<void()> onEdit;`
- `void setFromMouse(const juce::MouseEvent& event);`
- `double levelAt(int index) const;`
- `void setLevel(int index, double value);`
- `bool inWindow(int harmonic) const;`
- `APVTS& state_;`
- `juce::StringArray parameterIDs_;`
- `juce::Colour colour_;`
- `int firstHarmonic_ = 1;`
- `int windowFirst_ = 1;`
- `int windowLast_ = 128;`
- `std::function<bool(int)> groupTest_;`
- `std::function<bool(int, double)> editTest_;`

### `P1SpectrumDisplay`

`class` at `plugin/ui/Widgets.h:258` through line 283.

The a/b spectral display with the live morph cursor.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`
- `void tick();`
- `double levelAt(const juce::StringArray& ids, int index) const;`
- `APVTS& state_;`
- `juce::StringArray aIDs_;`
- `juce::StringArray bIDs_;`
- `juce::String morphID_;`
- `juce::String rangeAID_;`
- `juce::String rangeBID_;`
- `juce::Colour aColour_;`
- `juce::Colour bColour_;`
- `double liveMorph_ = -1.0;`
- `uint64_t lastSignature_ = 0;`
- `bool haveSignature_ = false;`

Methods:

- `void setLiveMorph(double value)`

### `EnvelopeShape`

`enum` at `plugin/ui/Widgets.h:285` through line 289.

Fields and enumerators:

- `legacyFourStage`
- `operatorAmp`
- `bipolar`

### `P1Envelope`

`class` at `plugin/ui/Widgets.h:291` through line 318.

The envelope graph for the operator, filter, pan and spectral envelopes.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`
- `void tick();`
- `APVTS& state_;`
- `juce::StringArray rateIDs_;`
- `juce::StringArray levelIDs_;`
- `juce::Colour colour_;`
- `double levelMin_ = 0.0;`
- `double levelMax_ = 99.0;`
- `bool zeroLine_ = false;`
- `EnvelopeShape shape_ = EnvelopeShape::legacyFourStage;`
- `juce::String modeID_;`
- `juce::String startID_;`
- `double startScale_ = 1.0;`
- `uint64_t lastSignature_ = 0;`
- `bool haveSignature_ = false;`

Methods:

- `void refresh()`

### `P1Wave`

`class` at `plugin/ui/Widgets.h:320` through line 332.

The small waveform preview.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`
- `void tick();`
- `APVTS& state_;`
- `juce::String waveID_;`
- `juce::Colour colour_;`
- `int lastWave_ = -1;`
- `bool haveWave_ = false;`

### `P1LevelSlider`

`class` at `plugin/ui/Widgets.h:334` through line 361.

Fields and enumerators:

- `void resized() override;`
- `void paint(juce::Graphics& g) override;`
- `void mouseDown(const juce::MouseEvent& event) override;`
- `void mouseDrag(const juce::MouseEvent& event) override;`
- `void setMacroFeedback(bool targeted, bool hasEffective, double effective) override;`
- `void setFromMouse(const juce::MouseEvent& event);`
- `APVTS& state_;`
- `juce::String paramID_;`
- `juce::String caption_;`
- `bool bipolar_ = false;`
- `const P1ParamSpec* spec_ = nullptr;`
- `int specIndex_ = -1;`
- `float value_ = 0.0f;`
- `juce::Rectangle<int> track_;`
- `std::unique_ptr<juce::ParameterAttachment> attach_;`
- `bool macroTargeted_ = false;`
- `bool macroHasEffective_ = false;`
- `double macroEffective_ = 0.0;`

Methods:

- `int macroParamIndex() const override`

### `P1VolumeSlider`

`class` at `plugin/ui/Widgets.h:366` through line 382.

The master output fader. The parameter is the fader position, so the fill is linear in position whilst the gain follows the law in VolumeLaw.h, and the unity mark sits at 75 percent of the track.

Fields and enumerators:

- `void resized() override;`
- `void paint(juce::Graphics& g) override;`
- `void mouseDown(const juce::MouseEvent& event) override;`
- `void mouseDrag(const juce::MouseEvent& event) override;`
- `void setFromMouse(const juce::MouseEvent& event);`
- `APVTS& state_;`
- `juce::String paramID_;`
- `int position_ = p1::kVolumeUnity;`
- `juce::Rectangle<int> track_;`
- `std::unique_ptr<juce::ParameterAttachment> attach_;`

### `P1Card`

`class` at `plugin/ui/Widgets.h:384` through line 401.

The shared card frame and header.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`
- `void resized() override;`
- `void setHeaderTrailingWidth(int width);`
- `juce::Rectangle<int> headerTrailingBounds() const;`
- `juce::Rectangle<int> contentArea() const;`
- `juce::String title_;`
- `bool hasActions_ = false;`
- `int trailingWidth_ = 0;`
- `P1Button copy_{"copy", true};`
- `P1Button paste_{"paste", true};`

### `P1AlgorithmBlock`

`class` at `plugin/ui/Widgets.h:403` through line 424.

Fields and enumerators:

- `void setAlgorithm(int algorithm);`
- `void setRingMask(uint8_t mask);`
- `void setDirectMask(uint8_t mask);`
- `void paint(juce::Graphics& g) override;`
- `void mouseUp(const juce::MouseEvent& event) override;`
- `void mouseEnter(const juce::MouseEvent& event) override;`
- `void mouseExit(const juce::MouseEvent& event) override;`
- `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;`
- `std::function<void()> onClick;`
- `std::function<void(int)> onWheel;`
- `int algorithm_ = 4;`
- `uint8_t ringMask_ = 0;`
- `uint8_t directMask_ = 0;`
- `bool hover_ = false;`

### `P1Dialog`

`class` at `plugin/ui/Widgets.h:426` through line 445.

Fields and enumerators:

- `void paint(juce::Graphics& g) override;`
- `void resized() override;`
- `void mouseUp(const juce::MouseEvent& event) override;`
- `juce::String title_;`
- `std::unique_ptr<juce::Component> body_;`
- `juce::StringArray buttons_;`
- `std::vector<std::unique_ptr<P1Button>> buttonComponents_;`
- `std::function<void(int)> onButton_;`
- `std::function<void()> onClose_;`
- `juce::Rectangle<int> panel_;`
- `juce::Rectangle<int> closeArea_;`

### `Function`

`class` at `tools/gen_code_docs.py:55` through line 64.

### `Type`

`class` at `tools/gen_code_docs.py:68` through line 76.

### `Symbol`

`class` at `tools/gen_code_docs.py:80` through line 86.

