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

- `struct type_p_alpha_state { uint32_t op1_ph_inc[NVOICE]; uint32_t op1_ph_phase[NVOICE]; uint32_t op1_ph_step[NVOICE]; uint32_t op1_ph_mip[NVOICE]; uint32_t op1_osc_wave[NVOICE]; uint32_t op1_osc_base[NVOICE]; uint32_t op1_osc_bl[NVOICE]; uint32_t op1_osc_tick[NVOICE]; int32_t op1_osc_live[NVOICE]; int32_t op1_osc_morph_macro[NVOICE]; uint32_t op1_eg_stage[NVOICE]; int32_t op1_eg_level[NVOICE]; int32_t op1_eg_warm_start[NVOICE]; uint32_t op1_eg_warm_progress[NVOICE]; uint32_t op1_eg_warm_eff[NVOICE]; uint32_t op1_eg_surge_eff[NVOICE]; uint32_t op1_eg_d1_eff[NVOICE]; uint32_t op1_eg_d2_eff[NVOICE]; uint32_t op1_eg_rel_eff[NVOICE]; int32_t op1_eg_rate_macro[NVOICE]; int32_t op1_eg_warm_base[NVOICE]; int32_t op1_eg_surge_base[NVOICE]; int32_t op1_eg_d1_base[NVOICE]; int32_t op1_eg_d2_base[NVOICE]; int32_t op1_eg_rel_base[NVOICE]; uint32_t op1_lfo_phase[NVOICE]; uint32_t op1_lfo_base[NVOICE]; int32_t op1_lfo_w[NVOICE]; uint32_t op1_lfo_rate_level[NVOICE]; uint32_t op1_lfo_depth_level[NVOICE]; uint32_t op1_lfo_rate_step[NVOICE]; uint32_t op1_lfo_depth_step[NVOICE]; uint32_t op1_lfo_rate_ratio[NVOICE]; int32_t op1_lfo_depth_macro[NVOICE]; uint32_t op1_fbenv_stage[NVOICE]; int32_t op1_fbenv_level[NVOICE]; uint32_t op1_fbenv_r1_eff[NVOICE]; uint32_t op1_fbenv_r2_eff[NVOICE]; uint32_t op1_fbenv_r3_eff[NVOICE]; uint32_t op1_fbenv_r4_eff[NVOICE]; int32_t op1_fbenv_rate_macro[NVOICE]; int32_t op1_fbenv_r1_base[NVOICE]; int32_t op1_fbenv_r2_base[NVOICE]; int32_t op1_fbenv_r3_base[NVOICE]; int32_t op1_fbenv_relr_base[NVOICE]; uint32_t op1_fblfo_phase[NVOICE]; uint32_t op1_fblfo_base[NVOICE]; int32_t op1_fblfo_w[NVOICE]; uint32_t op1_fblfo_rate_level[NVOICE]; uint32_t op1_fblfo_depth_level[NVOICE]; uint32_t op1_fblfo_rate_step[NVOICE]; uint32_t op1_fblfo_depth_step[NVOICE]; uint32_t op1_fblfo_rate_ratio[NVOICE]; int32_t op1_fblfo_depth_macro[NVOICE]; uint32_t op1_dlfo_phase[NVOICE]; uint32_t op1_dlfo_base[NVOICE]; int32_t op1_dlfo_w[NVOICE]; uint32_t op1_dlfo_rate_level[NVOICE]; uint32_t op1_dlfo_depth_level[NVOICE]; uint32_t op1_dlfo_rate_step[NVOICE]; uint32_t op1_dlfo_depth_step[NVOICE]; uint32_t op1_dlfo_rate_ratio[NVOICE]; int32_t op1_dlfo_depth_macro[NVOICE]; uint32_t op1_tl_level_eff[NVOICE]; int32_t op1_fb_signed[NVOICE]; int32_t op1_fb_level_eff[NVOICE]; uint32_t op1_fb_negative[NVOICE]; int32_t op1_fb_prev[NVOICE]; int32_t op1_fb_shaped[NVOICE]; int32_t op1_fb_macro[NVOICE]; int32_t op1_fb_level_macro[NVOICE]; int32_t op1_level_ks_k[NVOICE]; int32_t op1_fb_ks_k[NVOICE]; int32_t op1_eg_ks_k[NVOICE]; int32_t op1_level_vs_k[NVOICE]; int32_t op1_fb_vs_k[NVOICE]; int32_t op1_eg_vs_k[NVOICE]; int32_t op1_d1_prev[NVOICE]; uint32_t op1_specenv_stage[NVOICE]; int32_t op1_specenv_level[NVOICE]; uint32_t op1_specenv_r1_eff[NVOICE]; uint32_t op1_specenv_r2_eff[NVOICE]; uint32_t op1_specenv_r3_eff[NVOICE]; uint32_t op1_specenv_r4_eff[NVOICE]; int32_t op1_specenv_rate_macro[NVOICE]; int32_t op1_specenv_r1_base[NVOICE]; int32_t op1_specenv_r2_base[NVOICE]; int32_t op1_specenv_r3_base[NVOICE]; int32_t op1_specenv_relr_base[NVOICE]; uint32_t op1_speclfo_phase[NVOICE]; uint32_t op1_speclfo_base[NVOICE]; int32_t op1_speclfo_w[NVOICE]; uint32_t op1_speclfo_rate_level[NVOICE]; uint32_t op1_speclfo_depth_level[NVOICE]; uint32_t op1_speclfo_rate_step[NVOICE]; uint32_t op1_speclfo_depth_step[NVOICE]; uint32_t op1_speclfo_rate_ratio[NVOICE]; int32_t op1_speclfo_depth_macro[NVOICE]; uint32_t op2_ph_inc[NVOICE]; uint32_t op2_ph_phase[NVOICE]; uint32_t op2_ph_step[NVOICE]; uint32_t op2_ph_mip[NVOICE]; uint32_t op2_osc_wave[NVOICE]; uint32_t op2_osc_base[NVOICE]; uint32_t op2_osc_bl[NVOICE]; uint32_t op2_osc_tick[NVOICE]; int32_t op2_osc_live[NVOICE]; int32_t op2_osc_morph_macro[NVOICE]; uint32_t op2_eg_stage[NVOICE]; int32_t op2_eg_level[NVOICE]; int32_t op2_eg_warm_start[NVOICE]; uint32_t op2_eg_warm_progress[NVOICE]; uint32_t op2_eg_warm_eff[NVOICE]; uint32_t op2_eg_surge_eff[NVOICE]; uint32_t op2_eg_d1_eff[NVOICE]; uint32_t op2_eg_d2_eff[NVOICE]; uint32_t op2_eg_rel_eff[NVOICE]; int32_t op2_eg_rate_macro[NVOICE]; int32_t op2_eg_warm_base[NVOICE]; int32_t op2_eg_surge_base[NVOICE]; int32_t op2_eg_d1_base[NVOICE]; int32_t op2_eg_d2_base[NVOICE]; int32_t op2_eg_rel_base[NVOICE]; uint32_t op2_lfo_phase[NVOICE]; uint32_t op2_lfo_base[NVOICE]; int32_t op2_lfo_w[NVOICE]; uint32_t op2_lfo_rate_level[NVOICE]; uint32_t op2_lfo_depth_level[NVOICE]; uint32_t op2_lfo_rate_step[NVOICE]; uint32_t op2_lfo_depth_step[NVOICE]; uint32_t op2_lfo_rate_ratio[NVOICE]; int32_t op2_lfo_depth_macro[NVOICE]; uint32_t op2_fbenv_stage[NVOICE]; int32_t op2_fbenv_level[NVOICE]; uint32_t op2_fbenv_r1_eff[NVOICE]; uint32_t op2_fbenv_r2_eff[NVOICE]; uint32_t op2_fbenv_r3_eff[NVOICE]; uint32_t op2_fbenv_r4_eff[NVOICE]; int32_t op2_fbenv_rate_macro[NVOICE]; int32_t op2_fbenv_r1_base[NVOICE]; int32_t op2_fbenv_r2_base[NVOICE]; int32_t op2_fbenv_r3_base[NVOICE]; int32_t op2_fbenv_relr_base[NVOICE]; uint32_t op2_fblfo_phase[NVOICE]; uint32_t op2_fblfo_base[NVOICE]; int32_t op2_fblfo_w[NVOICE]; uint32_t op2_fblfo_rate_level[NVOICE]; uint32_t op2_fblfo_depth_level[NVOICE]; uint32_t op2_fblfo_rate_step[NVOICE]; uint32_t op2_fblfo_depth_step[NVOICE]; uint32_t op2_fblfo_rate_ratio[NVOICE]; int32_t op2_fblfo_depth_macro[NVOICE]; uint32_t op2_dlfo_phase[NVOICE]; uint32_t op2_dlfo_base[NVOICE]; int32_t op2_dlfo_w[NVOICE]; uint32_t op2_dlfo_rate_level[NVOICE]; uint32_t op2_dlfo_depth_level[NVOICE]; uint32_t op2_dlfo_rate_step[NVOICE]; uint32_t op2_dlfo_depth_step[NVOICE]; uint32_t op2_dlfo_rate_ratio[NVOICE]; int32_t op2_dlfo_depth_macro[NVOICE]; uint32_t op2_tl_level_eff[NVOICE]; int32_t op2_fb_signed[NVOICE]; int32_t op2_fb_level_eff[NVOICE]; uint32_t op2_fb_negative[NVOICE]; int32_t op2_fb_prev[NVOICE]; int32_t op2_fb_shaped[NVOICE]; int32_t op2_fb_macro[NVOICE]; int32_t op2_fb_level_macro[NVOICE]; int32_t op2_level_ks_k[NVOICE]; int32_t op2_fb_ks_k[NVOICE]; int32_t op2_eg_ks_k[NVOICE]; int32_t op2_level_vs_k[NVOICE]; int32_t op2_fb_vs_k[NVOICE]; int32_t op2_eg_vs_k[NVOICE]; int32_t op2_d1_prev[NVOICE]; uint32_t op2_specenv_stage[NVOICE]; int32_t op2_specenv_level[NVOICE]; uint32_t op2_specenv_r1_eff[NVOICE]; uint32_t op2_specenv_r2_eff[NVOICE]; uint32_t op2_specenv_r3_eff[NVOICE]; uint32_t op2_specenv_r4_eff[NVOICE]; int32_t op2_specenv_rate_macro[NVOICE]; int32_t op2_specenv_r1_base[NVOICE]; int32_t op2_specenv_r2_base[NVOICE]; int32_t op2_specenv_r3_base[NVOICE]; int32_t op2_specenv_relr_base[NVOICE]; uint32_t op2_speclfo_phase[NVOICE]; uint32_t op2_speclfo_base[NVOICE]; int32_t op2_speclfo_w[NVOICE]; uint32_t op2_speclfo_rate_level[NVOICE]; uint32_t op2_speclfo_depth_level[NVOICE]; uint32_t op2_speclfo_rate_step[NVOICE]; uint32_t op2_speclfo_depth_step[NVOICE]; uint32_t op2_speclfo_rate_ratio[NVOICE]; int32_t op2_speclfo_depth_macro[NVOICE]; uint32_t op3_ph_inc[NVOICE]; uint32_t op3_ph_phase[NVOICE]; uint32_t op3_ph_step[NVOICE]; uint32_t op3_ph_mip[NVOICE]; uint32_t op3_osc_wave[NVOICE]; uint32_t op3_osc_base[NVOICE]; uint32_t op3_osc_bl[NVOICE]; uint32_t op3_osc_tick[NVOICE]; int32_t op3_osc_live[NVOICE]; int32_t op3_osc_morph_macro[NVOICE]; uint32_t op3_eg_stage[NVOICE]; int32_t op3_eg_level[NVOICE]; int32_t op3_eg_warm_start[NVOICE]; uint32_t op3_eg_warm_progress[NVOICE]; uint32_t op3_eg_warm_eff[NVOICE]; uint32_t op3_eg_surge_eff[NVOICE]; uint32_t op3_eg_d1_eff[NVOICE]; uint32_t op3_eg_d2_eff[NVOICE]; uint32_t op3_eg_rel_eff[NVOICE]; int32_t op3_eg_rate_macro[NVOICE]; int32_t op3_eg_warm_base[NVOICE]; int32_t op3_eg_surge_base[NVOICE]; int32_t op3_eg_d1_base[NVOICE]; int32_t op3_eg_d2_base[NVOICE]; int32_t op3_eg_rel_base[NVOICE]; uint32_t op3_lfo_phase[NVOICE]; uint32_t op3_lfo_base[NVOICE]; int32_t op3_lfo_w[NVOICE]; uint32_t op3_lfo_rate_level[NVOICE]; uint32_t op3_lfo_depth_level[NVOICE]; uint32_t op3_lfo_rate_step[NVOICE]; uint32_t op3_lfo_depth_step[NVOICE]; uint32_t op3_lfo_rate_ratio[NVOICE]; int32_t op3_lfo_depth_macro[NVOICE]; uint32_t op3_fbenv_stage[NVOICE]; int32_t op3_fbenv_level[NVOICE]; uint32_t op3_fbenv_r1_eff[NVOICE]; uint32_t op3_fbenv_r2_eff[NVOICE]; uint32_t op3_fbenv_r3_eff[NVOICE]; uint32_t op3_fbenv_r4_eff[NVOICE]; int32_t op3_fbenv_rate_macro[NVOICE]; int32_t op3_fbenv_r1_base[NVOICE]; int32_t op3_fbenv_r2_base[NVOICE]; int32_t op3_fbenv_r3_base[NVOICE]; int32_t op3_fbenv_relr_base[NVOICE]; uint32_t op3_fblfo_phase[NVOICE]; uint32_t op3_fblfo_base[NVOICE]; int32_t op3_fblfo_w[NVOICE]; uint32_t op3_fblfo_rate_level[NVOICE]; uint32_t op3_fblfo_depth_level[NVOICE]; uint32_t op3_fblfo_rate_step[NVOICE]; uint32_t op3_fblfo_depth_step[NVOICE]; uint32_t op3_fblfo_rate_ratio[NVOICE]; int32_t op3_fblfo_depth_macro[NVOICE]; uint32_t op3_dlfo_phase[NVOICE]; uint32_t op3_dlfo_base[NVOICE]; int32_t op3_dlfo_w[NVOICE]; uint32_t op3_dlfo_rate_level[NVOICE]; uint32_t op3_dlfo_depth_level[NVOICE]; uint32_t op3_dlfo_rate_step[NVOICE]; uint32_t op3_dlfo_depth_step[NVOICE]; uint32_t op3_dlfo_rate_ratio[NVOICE]; int32_t op3_dlfo_depth_macro[NVOICE]; uint32_t op3_tl_level_eff[NVOICE]; int32_t op3_fb_signed[NVOICE]; int32_t op3_fb_level_eff[NVOICE]; uint32_t op3_fb_negative[NVOICE]; int32_t op3_fb_prev[NVOICE]; int32_t op3_fb_shaped[NVOICE]; int32_t op3_fb_macro[NVOICE]; int32_t op3_fb_level_macro[NVOICE]; int32_t op3_level_ks_k[NVOICE]; int32_t op3_fb_ks_k[NVOICE]; int32_t op3_eg_ks_k[NVOICE]; int32_t op3_level_vs_k[NVOICE]; int32_t op3_fb_vs_k[NVOICE]; int32_t op3_eg_vs_k[NVOICE]; int32_t op3_d1_prev[NVOICE]; uint32_t op3_specenv_stage[NVOICE]; int32_t op3_specenv_level[NVOICE]; uint32_t op3_specenv_r1_eff[NVOICE]; uint32_t op3_specenv_r2_eff[NVOICE]; uint32_t op3_specenv_r3_eff[NVOICE]; uint32_t op3_specenv_r4_eff[NVOICE]; int32_t op3_specenv_rate_macro[NVOICE]; int32_t op3_specenv_r1_base[NVOICE]; int32_t op3_specenv_r2_base[NVOICE]; int32_t op3_specenv_r3_base[NVOICE]; int32_t op3_specenv_relr_base[NVOICE]; uint32_t op3_speclfo_phase[NVOICE]; uint32_t op3_speclfo_base[NVOICE]; int32_t op3_speclfo_w[NVOICE]; uint32_t op3_speclfo_rate_level[NVOICE]; uint32_t op3_speclfo_depth_level[NVOICE]; uint32_t op3_speclfo_rate_step[NVOICE]; uint32_t op3_speclfo_depth_step[NVOICE]; uint32_t op3_speclfo_rate_ratio[NVOICE]; int32_t op3_speclfo_depth_macro[NVOICE]; uint32_t op4_ph_inc[NVOICE]; uint32_t op4_ph_phase[NVOICE]; uint32_t op4_ph_step[NVOICE]; uint32_t op4_ph_mip[NVOICE]; uint32_t op4_osc_wave[NVOICE]; uint32_t op4_osc_base[NVOICE]; uint32_t op4_osc_bl[NVOICE]; uint32_t op4_osc_tick[NVOICE]; int32_t op4_osc_live[NVOICE]; int32_t op4_osc_morph_macro[NVOICE]; uint32_t op4_eg_stage[NVOICE]; int32_t op4_eg_level[NVOICE]; int32_t op4_eg_warm_start[NVOICE]; uint32_t op4_eg_warm_progress[NVOICE]; uint32_t op4_eg_warm_eff[NVOICE]; uint32_t op4_eg_surge_eff[NVOICE]; uint32_t op4_eg_d1_eff[NVOICE]; uint32_t op4_eg_d2_eff[NVOICE]; uint32_t op4_eg_rel_eff[NVOICE]; int32_t op4_eg_rate_macro[NVOICE]; int32_t op4_eg_warm_base[NVOICE]; int32_t op4_eg_surge_base[NVOICE]; int32_t op4_eg_d1_base[NVOICE]; int32_t op4_eg_d2_base[NVOICE]; int32_t op4_eg_rel_base[NVOICE]; uint32_t op4_lfo_phase[NVOICE]; uint32_t op4_lfo_base[NVOICE]; int32_t op4_lfo_w[NVOICE]; uint32_t op4_lfo_rate_level[NVOICE]; uint32_t op4_lfo_depth_level[NVOICE]; uint32_t op4_lfo_rate_step[NVOICE]; uint32_t op4_lfo_depth_step[NVOICE]; uint32_t op4_lfo_rate_ratio[NVOICE]; int32_t op4_lfo_depth_macro[NVOICE]; uint32_t op4_fbenv_stage[NVOICE]; int32_t op4_fbenv_level[NVOICE]; uint32_t op4_fbenv_r1_eff[NVOICE]; uint32_t op4_fbenv_r2_eff[NVOICE]; uint32_t op4_fbenv_r3_eff[NVOICE]; uint32_t op4_fbenv_r4_eff[NVOICE]; int32_t op4_fbenv_rate_macro[NVOICE]; int32_t op4_fbenv_r1_base[NVOICE]; int32_t op4_fbenv_r2_base[NVOICE]; int32_t op4_fbenv_r3_base[NVOICE]; int32_t op4_fbenv_relr_base[NVOICE]; uint32_t op4_fblfo_phase[NVOICE]; uint32_t op4_fblfo_base[NVOICE]; int32_t op4_fblfo_w[NVOICE]; uint32_t op4_fblfo_rate_level[NVOICE]; uint32_t op4_fblfo_depth_level[NVOICE]; uint32_t op4_fblfo_rate_step[NVOICE]; uint32_t op4_fblfo_depth_step[NVOICE]; uint32_t op4_fblfo_rate_ratio[NVOICE]; int32_t op4_fblfo_depth_macro[NVOICE]; uint32_t op4_dlfo_phase[NVOICE]; uint32_t op4_dlfo_base[NVOICE]; int32_t op4_dlfo_w[NVOICE]; uint32_t op4_dlfo_rate_level[NVOICE]; uint32_t op4_dlfo_depth_level[NVOICE]; uint32_t op4_dlfo_rate_step[NVOICE]; uint32_t op4_dlfo_depth_step[NVOICE]; uint32_t op4_dlfo_rate_ratio[NVOICE]; int32_t op4_dlfo_depth_macro[NVOICE]; uint32_t op4_tl_level_eff[NVOICE]; int32_t op4_fb_signed[NVOICE]; int32_t op4_fb_level_eff[NVOICE]; uint32_t op4_fb_negative[NVOICE]; int32_t op4_fb_prev[NVOICE]; int32_t op4_fb_shaped[NVOICE]; int32_t op4_fb_macro[NVOICE]; int32_t op4_fb_level_macro[NVOICE]; int32_t op4_level_ks_k[NVOICE]; int32_t op4_fb_ks_k[NVOICE]; int32_t op4_eg_ks_k[NVOICE]; int32_t op4_level_vs_k[NVOICE]; int32_t op4_fb_vs_k[NVOICE]; int32_t op4_eg_vs_k[NVOICE]; int32_t op4_d1_prev[NVOICE]; uint32_t op4_specenv_stage[NVOICE]; int32_t op4_specenv_level[NVOICE]; uint32_t op4_specenv_r1_eff[NVOICE]; uint32_t op4_specenv_r2_eff[NVOICE]; uint32_t op4_specenv_r3_eff[NVOICE]; uint32_t op4_specenv_r4_eff[NVOICE]; int32_t op4_specenv_rate_macro[NVOICE]; int32_t op4_specenv_r1_base[NVOICE]; int32_t op4_specenv_r2_base[NVOICE]; int32_t op4_specenv_r3_base[NVOICE]; int32_t op4_specenv_relr_base[NVOICE]; uint32_t op4_speclfo_phase[NVOICE]; uint32_t op4_speclfo_base[NVOICE]; int32_t op4_speclfo_w[NVOICE]; uint32_t op4_speclfo_rate_level[NVOICE]; uint32_t op4_speclfo_depth_level[NVOICE]; uint32_t op4_speclfo_rate_step[NVOICE]; uint32_t op4_speclfo_depth_step[NVOICE]; uint32_t op4_speclfo_rate_ratio[NVOICE]; int32_t op4_speclfo_depth_macro[NVOICE]; int32_t d1_prev[NVOICE]; int32_t d2_prev[NVOICE]; int32_t d3_prev[NVOICE]; int32_t d4_prev[NVOICE]; uint32_t penv_stage[NVOICE]; int32_t penv_level[NVOICE]; uint32_t penv_r1_eff[NVOICE]; uint32_t penv_r2_eff[NVOICE]; uint32_t penv_r3_eff[NVOICE]; uint32_t penv_r4_eff[NVOICE]; int32_t penv_rate_macro[NVOICE]; int32_t penv_r1_base[NVOICE]; int32_t penv_r2_base[NVOICE]; int32_t penv_r3_base[NVOICE]; int32_t penv_relr_base[NVOICE]; int32_t penv_ks_k[NVOICE]; int32_t penv_vs_k[NVOICE]; uint32_t plfo_phase[NVOICE]; uint32_t plfo_base[NVOICE]; int32_t plfo_w[NVOICE]; uint32_t plfo_rate_level[NVOICE]; uint32_t plfo_depth_level[NVOICE]; uint32_t plfo_rate_step[NVOICE]; uint32_t plfo_depth_step[NVOICE]; uint32_t plfo_rate_ratio[NVOICE]; int32_t plfo_depth_macro[NVOICE]; int32_t f1_ks_k[NVOICE]; int32_t fenv_ks_k[NVOICE]; int32_t f1_vs_k[NVOICE]; int32_t fenv_vs_k[NVOICE]; int32_t lp_z1[NVOICE]; int32_t lp_z2[NVOICE]; int32_t lp_z3[NVOICE]; int32_t lp_z4[NVOICE]; int32_t lp_fc[NVOICE]; int32_t lp_res_k[NVOICE]; int32_t lp_env[NVOICE]; int32_t lp_gate[NVOICE]; uint32_t fenv_stage[NVOICE]; int32_t fenv_level[NVOICE]; uint32_t fenv_r1_eff[NVOICE]; uint32_t fenv_r2_eff[NVOICE]; uint32_t fenv_r3_eff[NVOICE]; uint32_t fenv_r4_eff[NVOICE]; int32_t fenv_rate_macro[NVOICE]; int32_t fenv_r1_base[NVOICE]; int32_t fenv_r2_base[NVOICE]; int32_t fenv_r3_base[NVOICE]; int32_t fenv_relr_base[NVOICE]; int32_t dc_x1; int32_t dc_y1; uint32_t voice_age[NVOICE]; int32_t voice_velocity[NVOICE]; uint32_t voice_delay[NVOICE]; uint32_t voice_inc[NVOICE]; uint32_t voice_target_inc[NVOICE]; int32_t voice_detune[NVOICE]; uint32_t mono_order[N_PART][128]; uint32_t mono_serial[N_PART]; int32_t part_bend_q16[N_PART]; int32_t pan_l_q16[N_PART]; int32_t pan_r_q16[N_PART]; int32_t pan_env_level[N_PART]; uint32_t ages; int32_t vm_mode[2]; int32_t vm_unison_count[2]; int32_t vm_unison_detune[2]; int32_t vm_detune[2]; int32_t vm_transpose[2]; int32_t vm_legato[2]; int32_t vm_portamento[2]; int32_t vm_note_priority[2]; int32_t vm_bend_range[2]; int32_t vm_pan[2]; int32_t vm_pan_env_on[2]; int32_t vm_pan_env_r1[2]; int32_t vm_pan_env_r2[2]; int32_t vm_pan_env_r3[2]; int32_t vm_pan_env_l1[2]; int32_t vm_pan_env_l2[2]; int32_t vm_pan_env_l3[2]; int32_t vm_pan_env_relr[2]; int32_t vm_pan_env_rell[2]; int32_t vm_pan_env_mode[2]; int32_t part_channel[2]; int32_t part_low[2]; int32_t part_high[2]; int32_t part_level[2]; int32_t part_delay[2]; int32_t op1_ph_inc_ratio[2]; int32_t op1_ph_detune[2]; int32_t op1_osc_waveform[2]; int32_t op1_osc_spec_range_a[2]; int32_t op1_osc_spec_range_b[2]; int32_t op1_osc_spec_a1[2]; int32_t op1_osc_spec_a2[2]; int32_t op1_osc_spec_a3[2]; int32_t op1_osc_spec_a4[2]; int32_t op1_osc_spec_a5[2]; int32_t op1_osc_spec_a6[2]; int32_t op1_osc_spec_a7[2]; int32_t op1_osc_spec_a8[2]; int32_t op1_osc_spec_a9[2]; int32_t op1_osc_spec_a10[2]; int32_t op1_osc_spec_a11[2]; int32_t op1_osc_spec_a12[2]; int32_t op1_osc_spec_a13[2]; int32_t op1_osc_spec_a14[2]; int32_t op1_osc_spec_a15[2]; int32_t op1_osc_spec_a16[2]; int32_t op1_osc_spec_a17[2]; int32_t op1_osc_spec_a18[2]; int32_t op1_osc_spec_a19[2]; int32_t op1_osc_spec_a20[2]; int32_t op1_osc_spec_a21[2]; int32_t op1_osc_spec_a22[2]; int32_t op1_osc_spec_a23[2]; int32_t op1_osc_spec_a24[2]; int32_t op1_osc_spec_a25[2]; int32_t op1_osc_spec_a26[2]; int32_t op1_osc_spec_a27[2]; int32_t op1_osc_spec_a28[2]; int32_t op1_osc_spec_a29[2]; int32_t op1_osc_spec_a30[2]; int32_t op1_osc_spec_a31[2]; int32_t op1_osc_spec_a32[2]; int32_t op1_osc_spec_a33[2]; int32_t op1_osc_spec_a34[2]; int32_t op1_osc_spec_a35[2]; int32_t op1_osc_spec_a36[2]; int32_t op1_osc_spec_a37[2]; int32_t op1_osc_spec_a38[2]; int32_t op1_osc_spec_a39[2]; int32_t op1_osc_spec_a40[2]; int32_t op1_osc_spec_a41[2]; int32_t op1_osc_spec_a42[2]; int32_t op1_osc_spec_a43[2]; int32_t op1_osc_spec_a44[2]; int32_t op1_osc_spec_a45[2]; int32_t op1_osc_spec_a46[2]; int32_t op1_osc_spec_a47[2]; int32_t op1_osc_spec_a48[2]; int32_t op1_osc_spec_a49[2]; int32_t op1_osc_spec_a50[2]; int32_t op1_osc_spec_a51[2]; int32_t op1_osc_spec_a52[2]; int32_t op1_osc_spec_a53[2]; int32_t op1_osc_spec_a54[2]; int32_t op1_osc_spec_a55[2]; int32_t op1_osc_spec_a56[2]; int32_t op1_osc_spec_a57[2]; int32_t op1_osc_spec_a58[2]; int32_t op1_osc_spec_a59[2]; int32_t op1_osc_spec_a60[2]; int32_t op1_osc_spec_a61[2]; int32_t op1_osc_spec_a62[2]; int32_t op1_osc_spec_a63[2]; int32_t op1_osc_spec_a64[2]; int32_t op1_osc_spec_b1[2]; int32_t op1_osc_spec_b2[2]; int32_t op1_osc_spec_b3[2]; int32_t op1_osc_spec_b4[2]; int32_t op1_osc_spec_b5[2]; int32_t op1_osc_spec_b6[2]; int32_t op1_osc_spec_b7[2]; int32_t op1_osc_spec_b8[2]; int32_t op1_osc_spec_b9[2]; int32_t op1_osc_spec_b10[2]; int32_t op1_osc_spec_b11[2]; int32_t op1_osc_spec_b12[2]; int32_t op1_osc_spec_b13[2]; int32_t op1_osc_spec_b14[2]; int32_t op1_osc_spec_b15[2]; int32_t op1_osc_spec_b16[2]; int32_t op1_osc_spec_b17[2]; int32_t op1_osc_spec_b18[2]; int32_t op1_osc_spec_b19[2]; int32_t op1_osc_spec_b20[2]; int32_t op1_osc_spec_b21[2]; int32_t op1_osc_spec_b22[2]; int32_t op1_osc_spec_b23[2]; int32_t op1_osc_spec_b24[2]; int32_t op1_osc_spec_b25[2]; int32_t op1_osc_spec_b26[2]; int32_t op1_osc_spec_b27[2]; int32_t op1_osc_spec_b28[2]; int32_t op1_osc_spec_b29[2]; int32_t op1_osc_spec_b30[2]; int32_t op1_osc_spec_b31[2]; int32_t op1_osc_spec_b32[2]; int32_t op1_osc_spec_b33[2]; int32_t op1_osc_spec_b34[2]; int32_t op1_osc_spec_b35[2]; int32_t op1_osc_spec_b36[2]; int32_t op1_osc_spec_b37[2]; int32_t op1_osc_spec_b38[2]; int32_t op1_osc_spec_b39[2]; int32_t op1_osc_spec_b40[2]; int32_t op1_osc_spec_b41[2]; int32_t op1_osc_spec_b42[2]; int32_t op1_osc_spec_b43[2]; int32_t op1_osc_spec_b44[2]; int32_t op1_osc_spec_b45[2]; int32_t op1_osc_spec_b46[2]; int32_t op1_osc_spec_b47[2]; int32_t op1_osc_spec_b48[2]; int32_t op1_osc_spec_b49[2]; int32_t op1_osc_spec_b50[2]; int32_t op1_osc_spec_b51[2]; int32_t op1_osc_spec_b52[2]; int32_t op1_osc_spec_b53[2]; int32_t op1_osc_spec_b54[2]; int32_t op1_osc_spec_b55[2]; int32_t op1_osc_spec_b56[2]; int32_t op1_osc_spec_b57[2]; int32_t op1_osc_spec_b58[2]; int32_t op1_osc_spec_b59[2]; int32_t op1_osc_spec_b60[2]; int32_t op1_osc_spec_b61[2]; int32_t op1_osc_spec_b62[2]; int32_t op1_osc_spec_b63[2]; int32_t op1_osc_spec_b64[2]; int32_t op1_morphbase_value[2]; int32_t op1_eg_warm[2]; int32_t op1_eg_surge[2]; int32_t op1_eg_d1r[2]; int32_t op1_eg_d1l[2]; int32_t op1_eg_d2r[2]; int32_t op1_eg_d2l[2]; int32_t op1_eg_rel[2]; int32_t op1_eg_mode[2]; int32_t op1_lfo_rate[2]; int32_t op1_lfo_depth[2]; int32_t op1_lfo_wave[2]; int32_t op1_lfo_mode[2]; int32_t op1_lfo_rate_fade[2]; int32_t op1_lfo_depth_fade[2]; int32_t op1_fbenv_on[2]; int32_t op1_fbenv_r1[2]; int32_t op1_fbenv_r2[2]; int32_t op1_fbenv_r3[2]; int32_t op1_fbenv_l1[2]; int32_t op1_fbenv_l2[2]; int32_t op1_fbenv_l3[2]; int32_t op1_fbenv_relr[2]; int32_t op1_fbenv_rell[2]; int32_t op1_fbenv_mode[2]; int32_t op1_fblfo_rate[2]; int32_t op1_fblfo_depth[2]; int32_t op1_fblfo_wave[2]; int32_t op1_fblfo_mode[2]; int32_t op1_fblfo_rate_fade[2]; int32_t op1_fblfo_depth_fade[2]; int32_t op1_dlfo_rate[2]; int32_t op1_dlfo_depth[2]; int32_t op1_dlfo_wave[2]; int32_t op1_dlfo_mode[2]; int32_t op1_dlfo_rate_fade[2]; int32_t op1_dlfo_depth_fade[2]; int32_t op1_velamp_depth[2]; int32_t op1_velfm_depth[2]; int32_t op1_velrate_depth[2]; int32_t op1_tl_level[2]; int32_t op1_fb_level[2]; int32_t op1_level_ks_lo[2]; int32_t op1_level_ks_hi[2]; int32_t op1_fb_ks_lo[2]; int32_t op1_fb_ks_hi[2]; int32_t op1_eg_ks_lo[2]; int32_t op1_eg_ks_hi[2]; int32_t op1_level_vs_amount[2]; int32_t op1_fb_vs_amount[2]; int32_t op1_eg_vs_amount[2]; int32_t op1_mute_mute[2]; int32_t op1_specenv_on[2]; int32_t op1_specenv_r1[2]; int32_t op1_specenv_r2[2]; int32_t op1_specenv_r3[2]; int32_t op1_specenv_l1[2]; int32_t op1_specenv_l2[2]; int32_t op1_specenv_l3[2]; int32_t op1_specenv_relr[2]; int32_t op1_specenv_rell[2]; int32_t op1_specenv_mode[2]; int32_t op1_speclfo_rate[2]; int32_t op1_speclfo_depth[2]; int32_t op1_speclfo_wave[2]; int32_t op1_speclfo_mode[2]; int32_t op1_speclfo_rate_fade[2]; int32_t op1_speclfo_depth_fade[2]; int32_t op1_phase_on[2]; int32_t op1_phase_bp[2]; int32_t op1_phase_segments[2]; int32_t op1_phase_flip[2]; int32_t op1_phase_flip_rate[2]; int32_t op1_phase_reorder[2]; int32_t op1_phase_reorder_rate[2]; int32_t op1_phase_jump[2]; int32_t op1_phase_drift[2]; int32_t op1_phase_drift_rate[2]; int32_t op1_phase_ks_lo[2]; int32_t op1_phase_ks_hi[2]; int32_t op1_phase_vs[2]; int32_t op1_phase_lfo_rate[2]; int32_t op1_phase_lfo_depth[2]; int32_t op1_phase_lfo_wave[2]; int32_t op1_phase_lfo_mode[2]; int32_t op1_phase_lfo_rate_fade[2]; int32_t op1_phase_lfo_depth_fade[2]; int32_t op1_phase_env_on[2]; int32_t op1_phase_env_r1[2]; int32_t op1_phase_env_r2[2]; int32_t op1_phase_env_r3[2]; int32_t op1_phase_env_l1[2]; int32_t op1_phase_env_l2[2]; int32_t op1_phase_env_l3[2]; int32_t op1_phase_env_relr[2]; int32_t op1_phase_env_rell[2]; int32_t op1_phase_env_mode[2]; int32_t op2_ph_inc_ratio[2]; int32_t op2_ph_detune[2]; int32_t op2_osc_waveform[2]; int32_t op2_osc_spec_range_a[2]; int32_t op2_osc_spec_range_b[2]; int32_t op2_osc_spec_a1[2]; int32_t op2_osc_spec_a2[2]; int32_t op2_osc_spec_a3[2]; int32_t op2_osc_spec_a4[2]; int32_t op2_osc_spec_a5[2]; int32_t op2_osc_spec_a6[2]; int32_t op2_osc_spec_a7[2]; int32_t op2_osc_spec_a8[2]; int32_t op2_osc_spec_a9[2]; int32_t op2_osc_spec_a10[2]; int32_t op2_osc_spec_a11[2]; int32_t op2_osc_spec_a12[2]; int32_t op2_osc_spec_a13[2]; int32_t op2_osc_spec_a14[2]; int32_t op2_osc_spec_a15[2]; int32_t op2_osc_spec_a16[2]; int32_t op2_osc_spec_a17[2]; int32_t op2_osc_spec_a18[2]; int32_t op2_osc_spec_a19[2]; int32_t op2_osc_spec_a20[2]; int32_t op2_osc_spec_a21[2]; int32_t op2_osc_spec_a22[2]; int32_t op2_osc_spec_a23[2]; int32_t op2_osc_spec_a24[2]; int32_t op2_osc_spec_a25[2]; int32_t op2_osc_spec_a26[2]; int32_t op2_osc_spec_a27[2]; int32_t op2_osc_spec_a28[2]; int32_t op2_osc_spec_a29[2]; int32_t op2_osc_spec_a30[2]; int32_t op2_osc_spec_a31[2]; int32_t op2_osc_spec_a32[2]; int32_t op2_osc_spec_a33[2]; int32_t op2_osc_spec_a34[2]; int32_t op2_osc_spec_a35[2]; int32_t op2_osc_spec_a36[2]; int32_t op2_osc_spec_a37[2]; int32_t op2_osc_spec_a38[2]; int32_t op2_osc_spec_a39[2]; int32_t op2_osc_spec_a40[2]; int32_t op2_osc_spec_a41[2]; int32_t op2_osc_spec_a42[2]; int32_t op2_osc_spec_a43[2]; int32_t op2_osc_spec_a44[2]; int32_t op2_osc_spec_a45[2]; int32_t op2_osc_spec_a46[2]; int32_t op2_osc_spec_a47[2]; int32_t op2_osc_spec_a48[2]; int32_t op2_osc_spec_a49[2]; int32_t op2_osc_spec_a50[2]; int32_t op2_osc_spec_a51[2]; int32_t op2_osc_spec_a52[2]; int32_t op2_osc_spec_a53[2]; int32_t op2_osc_spec_a54[2]; int32_t op2_osc_spec_a55[2]; int32_t op2_osc_spec_a56[2]; int32_t op2_osc_spec_a57[2]; int32_t op2_osc_spec_a58[2]; int32_t op2_osc_spec_a59[2]; int32_t op2_osc_spec_a60[2]; int32_t op2_osc_spec_a61[2]; int32_t op2_osc_spec_a62[2]; int32_t op2_osc_spec_a63[2]; int32_t op2_osc_spec_a64[2]; int32_t op2_osc_spec_b1[2]; int32_t op2_osc_spec_b2[2]; int32_t op2_osc_spec_b3[2]; int32_t op2_osc_spec_b4[2]; int32_t op2_osc_spec_b5[2]; int32_t op2_osc_spec_b6[2]; int32_t op2_osc_spec_b7[2]; int32_t op2_osc_spec_b8[2]; int32_t op2_osc_spec_b9[2]; int32_t op2_osc_spec_b10[2]; int32_t op2_osc_spec_b11[2]; int32_t op2_osc_spec_b12[2]; int32_t op2_osc_spec_b13[2]; int32_t op2_osc_spec_b14[2]; int32_t op2_osc_spec_b15[2]; int32_t op2_osc_spec_b16[2]; int32_t op2_osc_spec_b17[2]; int32_t op2_osc_spec_b18[2]; int32_t op2_osc_spec_b19[2]; int32_t op2_osc_spec_b20[2]; int32_t op2_osc_spec_b21[2]; int32_t op2_osc_spec_b22[2]; int32_t op2_osc_spec_b23[2]; int32_t op2_osc_spec_b24[2]; int32_t op2_osc_spec_b25[2]; int32_t op2_osc_spec_b26[2]; int32_t op2_osc_spec_b27[2]; int32_t op2_osc_spec_b28[2]; int32_t op2_osc_spec_b29[2]; int32_t op2_osc_spec_b30[2]; int32_t op2_osc_spec_b31[2]; int32_t op2_osc_spec_b32[2]; int32_t op2_osc_spec_b33[2]; int32_t op2_osc_spec_b34[2]; int32_t op2_osc_spec_b35[2]; int32_t op2_osc_spec_b36[2]; int32_t op2_osc_spec_b37[2]; int32_t op2_osc_spec_b38[2]; int32_t op2_osc_spec_b39[2]; int32_t op2_osc_spec_b40[2]; int32_t op2_osc_spec_b41[2]; int32_t op2_osc_spec_b42[2]; int32_t op2_osc_spec_b43[2]; int32_t op2_osc_spec_b44[2]; int32_t op2_osc_spec_b45[2]; int32_t op2_osc_spec_b46[2]; int32_t op2_osc_spec_b47[2]; int32_t op2_osc_spec_b48[2]; int32_t op2_osc_spec_b49[2]; int32_t op2_osc_spec_b50[2]; int32_t op2_osc_spec_b51[2]; int32_t op2_osc_spec_b52[2]; int32_t op2_osc_spec_b53[2]; int32_t op2_osc_spec_b54[2]; int32_t op2_osc_spec_b55[2]; int32_t op2_osc_spec_b56[2]; int32_t op2_osc_spec_b57[2]; int32_t op2_osc_spec_b58[2]; int32_t op2_osc_spec_b59[2]; int32_t op2_osc_spec_b60[2]; int32_t op2_osc_spec_b61[2]; int32_t op2_osc_spec_b62[2]; int32_t op2_osc_spec_b63[2]; int32_t op2_osc_spec_b64[2]; int32_t op2_morphbase_value[2]; int32_t op2_eg_warm[2]; int32_t op2_eg_surge[2]; int32_t op2_eg_d1r[2]; int32_t op2_eg_d1l[2]; int32_t op2_eg_d2r[2]; int32_t op2_eg_d2l[2]; int32_t op2_eg_rel[2]; int32_t op2_eg_mode[2]; int32_t op2_lfo_rate[2]; int32_t op2_lfo_depth[2]; int32_t op2_lfo_wave[2]; int32_t op2_lfo_mode[2]; int32_t op2_lfo_rate_fade[2]; int32_t op2_lfo_depth_fade[2]; int32_t op2_fbenv_on[2]; int32_t op2_fbenv_r1[2]; int32_t op2_fbenv_r2[2]; int32_t op2_fbenv_r3[2]; int32_t op2_fbenv_l1[2]; int32_t op2_fbenv_l2[2]; int32_t op2_fbenv_l3[2]; int32_t op2_fbenv_relr[2]; int32_t op2_fbenv_rell[2]; int32_t op2_fbenv_mode[2]; int32_t op2_fblfo_rate[2]; int32_t op2_fblfo_depth[2]; int32_t op2_fblfo_wave[2]; int32_t op2_fblfo_mode[2]; int32_t op2_fblfo_rate_fade[2]; int32_t op2_fblfo_depth_fade[2]; int32_t op2_dlfo_rate[2]; int32_t op2_dlfo_depth[2]; int32_t op2_dlfo_wave[2]; int32_t op2_dlfo_mode[2]; int32_t op2_dlfo_rate_fade[2]; int32_t op2_dlfo_depth_fade[2]; int32_t op2_velamp_depth[2]; int32_t op2_velfm_depth[2]; int32_t op2_velrate_depth[2]; int32_t op2_tl_level[2]; int32_t op2_fb_level[2]; int32_t op2_level_ks_lo[2]; int32_t op2_level_ks_hi[2]; int32_t op2_fb_ks_lo[2]; int32_t op2_fb_ks_hi[2]; int32_t op2_eg_ks_lo[2]; int32_t op2_eg_ks_hi[2]; int32_t op2_level_vs_amount[2]; int32_t op2_fb_vs_amount[2]; int32_t op2_eg_vs_amount[2]; int32_t op2_mute_mute[2]; int32_t op2_specenv_on[2]; int32_t op2_specenv_r1[2]; int32_t op2_specenv_r2[2]; int32_t op2_specenv_r3[2]; int32_t op2_specenv_l1[2]; int32_t op2_specenv_l2[2]; int32_t op2_specenv_l3[2]; int32_t op2_specenv_relr[2]; int32_t op2_specenv_rell[2]; int32_t op2_specenv_mode[2]; int32_t op2_speclfo_rate[2]; int32_t op2_speclfo_depth[2]; int32_t op2_speclfo_wave[2]; int32_t op2_speclfo_mode[2]; int32_t op2_speclfo_rate_fade[2]; int32_t op2_speclfo_depth_fade[2]; int32_t op2_phase_on[2]; int32_t op2_phase_bp[2]; int32_t op2_phase_segments[2]; int32_t op2_phase_flip[2]; int32_t op2_phase_flip_rate[2]; int32_t op2_phase_reorder[2]; int32_t op2_phase_reorder_rate[2]; int32_t op2_phase_jump[2]; int32_t op2_phase_drift[2]; int32_t op2_phase_drift_rate[2]; int32_t op2_phase_ks_lo[2]; int32_t op2_phase_ks_hi[2]; int32_t op2_phase_vs[2]; int32_t op2_phase_lfo_rate[2]; int32_t op2_phase_lfo_depth[2]; int32_t op2_phase_lfo_wave[2]; int32_t op2_phase_lfo_mode[2]; int32_t op2_phase_lfo_rate_fade[2]; int32_t op2_phase_lfo_depth_fade[2]; int32_t op2_phase_env_on[2]; int32_t op2_phase_env_r1[2]; int32_t op2_phase_env_r2[2]; int32_t op2_phase_env_r3[2]; int32_t op2_phase_env_l1[2]; int32_t op2_phase_env_l2[2]; int32_t op2_phase_env_l3[2]; int32_t op2_phase_env_relr[2]; int32_t op2_phase_env_rell[2]; int32_t op2_phase_env_mode[2]; int32_t op3_ph_inc_ratio[2]; int32_t op3_ph_detune[2]; int32_t op3_osc_waveform[2]; int32_t op3_osc_spec_range_a[2]; int32_t op3_osc_spec_range_b[2]; int32_t op3_osc_spec_a1[2]; int32_t op3_osc_spec_a2[2]; int32_t op3_osc_spec_a3[2]; int32_t op3_osc_spec_a4[2]; int32_t op3_osc_spec_a5[2]; int32_t op3_osc_spec_a6[2]; int32_t op3_osc_spec_a7[2]; int32_t op3_osc_spec_a8[2]; int32_t op3_osc_spec_a9[2]; int32_t op3_osc_spec_a10[2]; int32_t op3_osc_spec_a11[2]; int32_t op3_osc_spec_a12[2]; int32_t op3_osc_spec_a13[2]; int32_t op3_osc_spec_a14[2]; int32_t op3_osc_spec_a15[2]; int32_t op3_osc_spec_a16[2]; int32_t op3_osc_spec_a17[2]; int32_t op3_osc_spec_a18[2]; int32_t op3_osc_spec_a19[2]; int32_t op3_osc_spec_a20[2]; int32_t op3_osc_spec_a21[2]; int32_t op3_osc_spec_a22[2]; int32_t op3_osc_spec_a23[2]; int32_t op3_osc_spec_a24[2]; int32_t op3_osc_spec_a25[2]; int32_t op3_osc_spec_a26[2]; int32_t op3_osc_spec_a27[2]; int32_t op3_osc_spec_a28[2]; int32_t op3_osc_spec_a29[2]; int32_t op3_osc_spec_a30[2]; int32_t op3_osc_spec_a31[2]; int32_t op3_osc_spec_a32[2]; int32_t op3_osc_spec_a33[2]; int32_t op3_osc_spec_a34[2]; int32_t op3_osc_spec_a35[2]; int32_t op3_osc_spec_a36[2]; int32_t op3_osc_spec_a37[2]; int32_t op3_osc_spec_a38[2]; int32_t op3_osc_spec_a39[2]; int32_t op3_osc_spec_a40[2]; int32_t op3_osc_spec_a41[2]; int32_t op3_osc_spec_a42[2]; int32_t op3_osc_spec_a43[2]; int32_t op3_osc_spec_a44[2]; int32_t op3_osc_spec_a45[2]; int32_t op3_osc_spec_a46[2]; int32_t op3_osc_spec_a47[2]; int32_t op3_osc_spec_a48[2]; int32_t op3_osc_spec_a49[2]; int32_t op3_osc_spec_a50[2]; int32_t op3_osc_spec_a51[2]; int32_t op3_osc_spec_a52[2]; int32_t op3_osc_spec_a53[2]; int32_t op3_osc_spec_a54[2]; int32_t op3_osc_spec_a55[2]; int32_t op3_osc_spec_a56[2]; int32_t op3_osc_spec_a57[2]; int32_t op3_osc_spec_a58[2]; int32_t op3_osc_spec_a59[2]; int32_t op3_osc_spec_a60[2]; int32_t op3_osc_spec_a61[2]; int32_t op3_osc_spec_a62[2]; int32_t op3_osc_spec_a63[2]; int32_t op3_osc_spec_a64[2]; int32_t op3_osc_spec_b1[2]; int32_t op3_osc_spec_b2[2]; int32_t op3_osc_spec_b3[2]; int32_t op3_osc_spec_b4[2]; int32_t op3_osc_spec_b5[2]; int32_t op3_osc_spec_b6[2]; int32_t op3_osc_spec_b7[2]; int32_t op3_osc_spec_b8[2]; int32_t op3_osc_spec_b9[2]; int32_t op3_osc_spec_b10[2]; int32_t op3_osc_spec_b11[2]; int32_t op3_osc_spec_b12[2]; int32_t op3_osc_spec_b13[2]; int32_t op3_osc_spec_b14[2]; int32_t op3_osc_spec_b15[2]; int32_t op3_osc_spec_b16[2]; int32_t op3_osc_spec_b17[2]; int32_t op3_osc_spec_b18[2]; int32_t op3_osc_spec_b19[2]; int32_t op3_osc_spec_b20[2]; int32_t op3_osc_spec_b21[2]; int32_t op3_osc_spec_b22[2]; int32_t op3_osc_spec_b23[2]; int32_t op3_osc_spec_b24[2]; int32_t op3_osc_spec_b25[2]; int32_t op3_osc_spec_b26[2]; int32_t op3_osc_spec_b27[2]; int32_t op3_osc_spec_b28[2]; int32_t op3_osc_spec_b29[2]; int32_t op3_osc_spec_b30[2]; int32_t op3_osc_spec_b31[2]; int32_t op3_osc_spec_b32[2]; int32_t op3_osc_spec_b33[2]; int32_t op3_osc_spec_b34[2]; int32_t op3_osc_spec_b35[2]; int32_t op3_osc_spec_b36[2]; int32_t op3_osc_spec_b37[2]; int32_t op3_osc_spec_b38[2]; int32_t op3_osc_spec_b39[2]; int32_t op3_osc_spec_b40[2]; int32_t op3_osc_spec_b41[2]; int32_t op3_osc_spec_b42[2]; int32_t op3_osc_spec_b43[2]; int32_t op3_osc_spec_b44[2]; int32_t op3_osc_spec_b45[2]; int32_t op3_osc_spec_b46[2]; int32_t op3_osc_spec_b47[2]; int32_t op3_osc_spec_b48[2]; int32_t op3_osc_spec_b49[2]; int32_t op3_osc_spec_b50[2]; int32_t op3_osc_spec_b51[2]; int32_t op3_osc_spec_b52[2]; int32_t op3_osc_spec_b53[2]; int32_t op3_osc_spec_b54[2]; int32_t op3_osc_spec_b55[2]; int32_t op3_osc_spec_b56[2]; int32_t op3_osc_spec_b57[2]; int32_t op3_osc_spec_b58[2]; int32_t op3_osc_spec_b59[2]; int32_t op3_osc_spec_b60[2]; int32_t op3_osc_spec_b61[2]; int32_t op3_osc_spec_b62[2]; int32_t op3_osc_spec_b63[2]; int32_t op3_osc_spec_b64[2]; int32_t op3_morphbase_value[2]; int32_t op3_eg_warm[2]; int32_t op3_eg_surge[2]; int32_t op3_eg_d1r[2]; int32_t op3_eg_d1l[2]; int32_t op3_eg_d2r[2]; int32_t op3_eg_d2l[2]; int32_t op3_eg_rel[2]; int32_t op3_eg_mode[2]; int32_t op3_lfo_rate[2]; int32_t op3_lfo_depth[2]; int32_t op3_lfo_wave[2]; int32_t op3_lfo_mode[2]; int32_t op3_lfo_rate_fade[2]; int32_t op3_lfo_depth_fade[2]; int32_t op3_fbenv_on[2]; int32_t op3_fbenv_r1[2]; int32_t op3_fbenv_r2[2]; int32_t op3_fbenv_r3[2]; int32_t op3_fbenv_l1[2]; int32_t op3_fbenv_l2[2]; int32_t op3_fbenv_l3[2]; int32_t op3_fbenv_relr[2]; int32_t op3_fbenv_rell[2]; int32_t op3_fbenv_mode[2]; int32_t op3_fblfo_rate[2]; int32_t op3_fblfo_depth[2]; int32_t op3_fblfo_wave[2]; int32_t op3_fblfo_mode[2]; int32_t op3_fblfo_rate_fade[2]; int32_t op3_fblfo_depth_fade[2]; int32_t op3_dlfo_rate[2]; int32_t op3_dlfo_depth[2]; int32_t op3_dlfo_wave[2]; int32_t op3_dlfo_mode[2]; int32_t op3_dlfo_rate_fade[2]; int32_t op3_dlfo_depth_fade[2]; int32_t op3_velamp_depth[2]; int32_t op3_velfm_depth[2]; int32_t op3_velrate_depth[2]; int32_t op3_tl_level[2]; int32_t op3_fb_level[2]; int32_t op3_level_ks_lo[2]; int32_t op3_level_ks_hi[2]; int32_t op3_fb_ks_lo[2]; int32_t op3_fb_ks_hi[2]; int32_t op3_eg_ks_lo[2]; int32_t op3_eg_ks_hi[2]; int32_t op3_level_vs_amount[2]; int32_t op3_fb_vs_amount[2]; int32_t op3_eg_vs_amount[2]; int32_t op3_mute_mute[2]; int32_t op3_specenv_on[2]; int32_t op3_specenv_r1[2]; int32_t op3_specenv_r2[2]; int32_t op3_specenv_r3[2]; int32_t op3_specenv_l1[2]; int32_t op3_specenv_l2[2]; int32_t op3_specenv_l3[2]; int32_t op3_specenv_relr[2]; int32_t op3_specenv_rell[2]; int32_t op3_specenv_mode[2]; int32_t op3_speclfo_rate[2]; int32_t op3_speclfo_depth[2]; int32_t op3_speclfo_wave[2]; int32_t op3_speclfo_mode[2]; int32_t op3_speclfo_rate_fade[2]; int32_t op3_speclfo_depth_fade[2]; int32_t op3_phase_on[2]; int32_t op3_phase_bp[2]; int32_t op3_phase_segments[2]; int32_t op3_phase_flip[2]; int32_t op3_phase_flip_rate[2]; int32_t op3_phase_reorder[2]; int32_t op3_phase_reorder_rate[2]; int32_t op3_phase_jump[2]; int32_t op3_phase_drift[2]; int32_t op3_phase_drift_rate[2]; int32_t op3_phase_ks_lo[2]; int32_t op3_phase_ks_hi[2]; int32_t op3_phase_vs[2]; int32_t op3_phase_lfo_rate[2]; int32_t op3_phase_lfo_depth[2]; int32_t op3_phase_lfo_wave[2]; int32_t op3_phase_lfo_mode[2]; int32_t op3_phase_lfo_rate_fade[2]; int32_t op3_phase_lfo_depth_fade[2]; int32_t op3_phase_env_on[2]; int32_t op3_phase_env_r1[2]; int32_t op3_phase_env_r2[2]; int32_t op3_phase_env_r3[2]; int32_t op3_phase_env_l1[2]; int32_t op3_phase_env_l2[2]; int32_t op3_phase_env_l3[2]; int32_t op3_phase_env_relr[2]; int32_t op3_phase_env_rell[2]; int32_t op3_phase_env_mode[2]; int32_t op4_ph_inc_ratio[2]; int32_t op4_ph_detune[2]; int32_t op4_osc_waveform[2]; int32_t op4_osc_spec_range_a[2]; int32_t op4_osc_spec_range_b[2]; int32_t op4_osc_spec_a1[2]; int32_t op4_osc_spec_a2[2]; int32_t op4_osc_spec_a3[2]; int32_t op4_osc_spec_a4[2]; int32_t op4_osc_spec_a5[2]; int32_t op4_osc_spec_a6[2]; int32_t op4_osc_spec_a7[2]; int32_t op4_osc_spec_a8[2]; int32_t op4_osc_spec_a9[2]; int32_t op4_osc_spec_a10[2]; int32_t op4_osc_spec_a11[2]; int32_t op4_osc_spec_a12[2]; int32_t op4_osc_spec_a13[2]; int32_t op4_osc_spec_a14[2]; int32_t op4_osc_spec_a15[2]; int32_t op4_osc_spec_a16[2]; int32_t op4_osc_spec_a17[2]; int32_t op4_osc_spec_a18[2]; int32_t op4_osc_spec_a19[2]; int32_t op4_osc_spec_a20[2]; int32_t op4_osc_spec_a21[2]; int32_t op4_osc_spec_a22[2]; int32_t op4_osc_spec_a23[2]; int32_t op4_osc_spec_a24[2]; int32_t op4_osc_spec_a25[2]; int32_t op4_osc_spec_a26[2]; int32_t op4_osc_spec_a27[2]; int32_t op4_osc_spec_a28[2]; int32_t op4_osc_spec_a29[2]; int32_t op4_osc_spec_a30[2]; int32_t op4_osc_spec_a31[2]; int32_t op4_osc_spec_a32[2]; int32_t op4_osc_spec_a33[2]; int32_t op4_osc_spec_a34[2]; int32_t op4_osc_spec_a35[2]; int32_t op4_osc_spec_a36[2]; int32_t op4_osc_spec_a37[2]; int32_t op4_osc_spec_a38[2]; int32_t op4_osc_spec_a39[2]; int32_t op4_osc_spec_a40[2]; int32_t op4_osc_spec_a41[2]; int32_t op4_osc_spec_a42[2]; int32_t op4_osc_spec_a43[2]; int32_t op4_osc_spec_a44[2]; int32_t op4_osc_spec_a45[2]; int32_t op4_osc_spec_a46[2]; int32_t op4_osc_spec_a47[2]; int32_t op4_osc_spec_a48[2]; int32_t op4_osc_spec_a49[2]; int32_t op4_osc_spec_a50[2]; int32_t op4_osc_spec_a51[2]; int32_t op4_osc_spec_a52[2]; int32_t op4_osc_spec_a53[2]; int32_t op4_osc_spec_a54[2]; int32_t op4_osc_spec_a55[2]; int32_t op4_osc_spec_a56[2]; int32_t op4_osc_spec_a57[2]; int32_t op4_osc_spec_a58[2]; int32_t op4_osc_spec_a59[2]; int32_t op4_osc_spec_a60[2]; int32_t op4_osc_spec_a61[2]; int32_t op4_osc_spec_a62[2]; int32_t op4_osc_spec_a63[2]; int32_t op4_osc_spec_a64[2]; int32_t op4_osc_spec_b1[2]; int32_t op4_osc_spec_b2[2]; int32_t op4_osc_spec_b3[2]; int32_t op4_osc_spec_b4[2]; int32_t op4_osc_spec_b5[2]; int32_t op4_osc_spec_b6[2]; int32_t op4_osc_spec_b7[2]; int32_t op4_osc_spec_b8[2]; int32_t op4_osc_spec_b9[2]; int32_t op4_osc_spec_b10[2]; int32_t op4_osc_spec_b11[2]; int32_t op4_osc_spec_b12[2]; int32_t op4_osc_spec_b13[2]; int32_t op4_osc_spec_b14[2]; int32_t op4_osc_spec_b15[2]; int32_t op4_osc_spec_b16[2]; int32_t op4_osc_spec_b17[2]; int32_t op4_osc_spec_b18[2]; int32_t op4_osc_spec_b19[2]; int32_t op4_osc_spec_b20[2]; int32_t op4_osc_spec_b21[2]; int32_t op4_osc_spec_b22[2]; int32_t op4_osc_spec_b23[2]; int32_t op4_osc_spec_b24[2]; int32_t op4_osc_spec_b25[2]; int32_t op4_osc_spec_b26[2]; int32_t op4_osc_spec_b27[2]; int32_t op4_osc_spec_b28[2]; int32_t op4_osc_spec_b29[2]; int32_t op4_osc_spec_b30[2]; int32_t op4_osc_spec_b31[2]; int32_t op4_osc_spec_b32[2]; int32_t op4_osc_spec_b33[2]; int32_t op4_osc_spec_b34[2]; int32_t op4_osc_spec_b35[2]; int32_t op4_osc_spec_b36[2]; int32_t op4_osc_spec_b37[2]; int32_t op4_osc_spec_b38[2]; int32_t op4_osc_spec_b39[2]; int32_t op4_osc_spec_b40[2]; int32_t op4_osc_spec_b41[2]; int32_t op4_osc_spec_b42[2]; int32_t op4_osc_spec_b43[2]; int32_t op4_osc_spec_b44[2]; int32_t op4_osc_spec_b45[2]; int32_t op4_osc_spec_b46[2]; int32_t op4_osc_spec_b47[2]; int32_t op4_osc_spec_b48[2]; int32_t op4_osc_spec_b49[2]; int32_t op4_osc_spec_b50[2]; int32_t op4_osc_spec_b51[2]; int32_t op4_osc_spec_b52[2]; int32_t op4_osc_spec_b53[2]; int32_t op4_osc_spec_b54[2]; int32_t op4_osc_spec_b55[2]; int32_t op4_osc_spec_b56[2]; int32_t op4_osc_spec_b57[2]; int32_t op4_osc_spec_b58[2]; int32_t op4_osc_spec_b59[2]; int32_t op4_osc_spec_b60[2]; int32_t op4_osc_spec_b61[2]; int32_t op4_osc_spec_b62[2]; int32_t op4_osc_spec_b63[2]; int32_t op4_osc_spec_b64[2]; int32_t op4_morphbase_value[2]; int32_t op4_eg_warm[2]; int32_t op4_eg_surge[2]; int32_t op4_eg_d1r[2]; int32_t op4_eg_d1l[2]; int32_t op4_eg_d2r[2]; int32_t op4_eg_d2l[2]; int32_t op4_eg_rel[2]; int32_t op4_eg_mode[2]; int32_t op4_lfo_rate[2]; int32_t op4_lfo_depth[2]; int32_t op4_lfo_wave[2]; int32_t op4_lfo_mode[2]; int32_t op4_lfo_rate_fade[2]; int32_t op4_lfo_depth_fade[2]; int32_t op4_fbenv_on[2]; int32_t op4_fbenv_r1[2]; int32_t op4_fbenv_r2[2]; int32_t op4_fbenv_r3[2]; int32_t op4_fbenv_l1[2]; int32_t op4_fbenv_l2[2]; int32_t op4_fbenv_l3[2]; int32_t op4_fbenv_relr[2]; int32_t op4_fbenv_rell[2]; int32_t op4_fbenv_mode[2]; int32_t op4_fblfo_rate[2]; int32_t op4_fblfo_depth[2]; int32_t op4_fblfo_wave[2]; int32_t op4_fblfo_mode[2]; int32_t op4_fblfo_rate_fade[2]; int32_t op4_fblfo_depth_fade[2]; int32_t op4_dlfo_rate[2]; int32_t op4_dlfo_depth[2]; int32_t op4_dlfo_wave[2]; int32_t op4_dlfo_mode[2]; int32_t op4_dlfo_rate_fade[2]; int32_t op4_dlfo_depth_fade[2]; int32_t op4_velamp_depth[2]; int32_t op4_velfm_depth[2]; int32_t op4_velrate_depth[2]; int32_t op4_tl_level[2]; int32_t op4_fb_level[2]; int32_t op4_level_ks_lo[2]; int32_t op4_level_ks_hi[2]; int32_t op4_fb_ks_lo[2]; int32_t op4_fb_ks_hi[2]; int32_t op4_eg_ks_lo[2]; int32_t op4_eg_ks_hi[2]; int32_t op4_level_vs_amount[2]; int32_t op4_fb_vs_amount[2]; int32_t op4_eg_vs_amount[2]; int32_t op4_mute_mute[2]; int32_t op4_specenv_on[2]; int32_t op4_specenv_r1[2]; int32_t op4_specenv_r2[2]; int32_t op4_specenv_r3[2]; int32_t op4_specenv_l1[2]; int32_t op4_specenv_l2[2]; int32_t op4_specenv_l3[2]; int32_t op4_specenv_relr[2]; int32_t op4_specenv_rell[2]; int32_t op4_specenv_mode[2]; int32_t op4_speclfo_rate[2]; int32_t op4_speclfo_depth[2]; int32_t op4_speclfo_wave[2]; int32_t op4_speclfo_mode[2]; int32_t op4_speclfo_rate_fade[2]; int32_t op4_speclfo_depth_fade[2]; int32_t op4_phase_on[2]; int32_t op4_phase_bp[2]; int32_t op4_phase_segments[2]; int32_t op4_phase_flip[2]; int32_t op4_phase_flip_rate[2]; int32_t op4_phase_reorder[2]; int32_t op4_phase_reorder_rate[2]; int32_t op4_phase_jump[2]; int32_t op4_phase_drift[2]; int32_t op4_phase_drift_rate[2]; int32_t op4_phase_ks_lo[2]; int32_t op4_phase_ks_hi[2]; int32_t op4_phase_vs[2]; int32_t op4_phase_lfo_rate[2]; int32_t op4_phase_lfo_depth[2]; int32_t op4_phase_lfo_wave[2]; int32_t op4_phase_lfo_mode[2]; int32_t op4_phase_lfo_rate_fade[2]; int32_t op4_phase_lfo_depth_fade[2]; int32_t op4_phase_env_on[2]; int32_t op4_phase_env_r1[2]; int32_t op4_phase_env_r2[2]; int32_t op4_phase_env_r3[2]; int32_t op4_phase_env_l1[2]; int32_t op4_phase_env_l2[2]; int32_t op4_phase_env_l3[2]; int32_t op4_phase_env_relr[2]; int32_t op4_phase_env_rell[2]; int32_t op4_phase_env_mode[2]; int32_t algo_algorithm[2]; int32_t route_ring1_on[2]; int32_t route_ring1_level[2]; int32_t route_ring2_on[2]; int32_t route_ring2_level[2]; int32_t route_ring3_on[2]; int32_t route_ring3_level[2]; int32_t route_car1_out[2]; int32_t route_car2_out[2]; int32_t route_car3_out[2]; int32_t route_car4_out[2]; int32_t penv_on[2]; int32_t penv_r1[2]; int32_t penv_r2[2]; int32_t penv_r3[2]; int32_t penv_l1[2]; int32_t penv_l2[2]; int32_t penv_l3[2]; int32_t penv_relr[2]; int32_t penv_rell[2]; int32_t penv_mode[2]; int32_t penv_ks_lo[2]; int32_t penv_ks_hi[2]; int32_t penv_vs_amount[2]; int32_t plfo_rate[2]; int32_t plfo_depth[2]; int32_t plfo_wave[2]; int32_t plfo_mode[2]; int32_t plfo_rate_fade[2]; int32_t plfo_depth_fade[2]; int32_t f1_ks_lo[2]; int32_t f1_ks_hi[2]; int32_t fenv_ks_lo[2]; int32_t fenv_ks_hi[2]; int32_t f1_vs_amount[2]; int32_t fenv_vs_amount[2]; int32_t cutoff_pos_cutoff[2]; int32_t lp_res[2]; int32_t lp_comp[2]; int32_t lp_mode[2]; int32_t lp_on[2]; int32_t fenv_on[2]; int32_t fenv_r1[2]; int32_t fenv_r2[2]; int32_t fenv_r3[2]; int32_t fenv_l1[2]; int32_t fenv_l2[2]; int32_t fenv_l3[2]; int32_t fenv_relr[2]; int32_t fenv_rell[2]; int32_t fenv_mode[2]; int32_t key_bp_bp[2]; int32_t key_on_on[2]; int32_t vel_on_on[2]; int32_t dc_r; int32_t macro_g1; int32_t macro_g2; int32_t macro_g3; int32_t macro_g4; int32_t macro_g5; int32_t macro_g6; int32_t macro_s1; int32_t macro_s2; int32_t macro_s3; int32_t macro_s4; int32_t macro_s5; int32_t macro_s6; int32_t macro_sr1; int32_t macro_sr2; int32_t macro_sr3; int32_t macro_sr4; int32_t macro_sr5; int32_t macro_sr6; int32_t macro_t1a; int32_t macro_t1b; int32_t macro_t1c; int32_t macro_t2a; int32_t macro_t2b; int32_t macro_t2c; int32_t macro_t3a; int32_t macro_t3b; int32_t macro_t3c; int32_t macro_t4a; int32_t macro_t4b; int32_t macro_t4c; int32_t macro_t5a; int32_t macro_t5b; int32_t macro_t5c; int32_t macro_t6a; int32_t macro_t6b; int32_t macro_t6c; int32_t macro_r1a; int32_t macro_r1b; int32_t macro_r1c; int32_t macro_r2a; int32_t macro_r2b; int32_t macro_r2c; int32_t macro_r3a; int32_t macro_r3b; int32_t macro_r3c; int32_t macro_r4a; int32_t macro_r4b; int32_t macro_r4c; int32_t macro_r5a; int32_t macro_r5b; int32_t macro_r5c; int32_t macro_r6a; int32_t macro_r6b; int32_t macro_r6c; uint32_t ctrl_serial; uint32_t patch_gate; uint32_t patch_trig; int32_t dcr_x1; int32_t dcr_y1; int32_t vel_on_on_x[NVOICE]; int32_t penv_vs_k_x[NVOICE]; int32_t op4_level_vs_k_x[NVOICE]; int32_t op4_fb_vs_k_x[NVOICE]; int32_t op4_eg_vs_k_x[NVOICE]; int32_t op3_level_vs_k_x[NVOICE]; int32_t op3_fb_vs_k_x[NVOICE]; int32_t op3_eg_vs_k_x[NVOICE]; int32_t op2_level_vs_k_x[NVOICE]; int32_t op2_fb_vs_k_x[NVOICE]; int32_t op2_eg_vs_k_x[NVOICE]; int32_t op1_level_vs_k_x[NVOICE]; int32_t op1_fb_vs_k_x[NVOICE]; int32_t op1_eg_vs_k_x[NVOICE]; int32_t fenv_vs_k_x[NVOICE]; int32_t f1_vs_k_x[NVOICE]; int32_t key_on_on_x[NVOICE]; int32_t key_bp_bp_x[NVOICE]; int32_t penv_ks_k_x[NVOICE]; int32_t penv_cv_x[NVOICE]; int32_t penv_active_x[NVOICE]; int32_t op4_level_ks_k_x[NVOICE]; int32_t op4_fb_ks_k_x[NVOICE]; int32_t op4_eg_ks_k_x[NVOICE]; int32_t op3_level_ks_k_x[NVOICE]; int32_t op3_fb_ks_k_x[NVOICE]; int32_t op3_eg_ks_k_x[NVOICE]; int32_t op2_level_ks_k_x[NVOICE]; int32_t op2_fb_ks_k_x[NVOICE]; int32_t op2_eg_ks_k_x[NVOICE]; int32_t op1_level_ks_k_x[NVOICE]; int32_t op1_fb_ks_k_x[NVOICE]; int32_t op1_eg_ks_k_x[NVOICE]; int32_t fenv_ks_k_x[NVOICE]; int32_t f1_ks_k_x[NVOICE]; int32_t cutoff_pos_f_x[NVOICE]; int32_t cutoff_pos_pos_x[NVOICE]; int32_t fenv_cv_x[NVOICE]; int32_t fenv_active_x[NVOICE]; int32_t plfo_cv_x[NVOICE]; int32_t psum_cv_x[NVOICE]; int32_t algo_alg_x[NVOICE]; int32_t op4_phase_env_cv_x[NVOICE]; int32_t op4_phase_lfo_cv_x[NVOICE]; int32_t op4_phase_mod_cv_x[NVOICE]; int32_t op4_speclfo_cv_x[NVOICE]; int32_t op4_velrate_cv_x[NVOICE]; int32_t op4_eg_cv_x[NVOICE]; int32_t op4_velfm_cv_x[NVOICE]; int32_t op4_velamp_cv_x[NVOICE]; int32_t op4_ampvel_cv_x[NVOICE]; int32_t op4_dlfo_cv_x[NVOICE]; int32_t op4_dsum_cv_x[NVOICE]; int32_t op4_fblfo_cv_x[NVOICE]; int32_t op4_fbenv_cv_x[NVOICE]; int32_t op4_fbenv_active_x[NVOICE]; int32_t op4_lfo_cv_x[NVOICE]; int32_t op4_morphbase_out_x[NVOICE]; int32_t op4_specenv_cv_x[NVOICE]; int32_t op4_specenv_active_x[NVOICE]; int32_t op3_phase_env_cv_x[NVOICE]; int32_t op3_phase_lfo_cv_x[NVOICE]; int32_t op3_phase_mod_cv_x[NVOICE]; int32_t op3_speclfo_cv_x[NVOICE]; int32_t op3_velrate_cv_x[NVOICE]; int32_t op3_eg_cv_x[NVOICE]; int32_t op3_velfm_cv_x[NVOICE]; int32_t op3_velamp_cv_x[NVOICE]; int32_t op3_ampvel_cv_x[NVOICE]; int32_t op3_dlfo_cv_x[NVOICE]; int32_t op3_dsum_cv_x[NVOICE]; int32_t op3_fblfo_cv_x[NVOICE]; int32_t op3_fbenv_cv_x[NVOICE]; int32_t op3_fbenv_active_x[NVOICE]; int32_t op3_lfo_cv_x[NVOICE]; int32_t op3_morphbase_out_x[NVOICE]; int32_t op3_specenv_cv_x[NVOICE]; int32_t op3_specenv_active_x[NVOICE]; int32_t op2_phase_env_cv_x[NVOICE]; int32_t op2_phase_lfo_cv_x[NVOICE]; int32_t op2_phase_mod_cv_x[NVOICE]; int32_t op2_speclfo_cv_x[NVOICE]; int32_t op2_velrate_cv_x[NVOICE]; int32_t op2_eg_cv_x[NVOICE]; int32_t op2_velfm_cv_x[NVOICE]; int32_t op2_velamp_cv_x[NVOICE]; int32_t op2_ampvel_cv_x[NVOICE]; int32_t op2_dlfo_cv_x[NVOICE]; int32_t op2_dsum_cv_x[NVOICE]; int32_t op2_fblfo_cv_x[NVOICE]; int32_t op2_fbenv_cv_x[NVOICE]; int32_t op2_fbenv_active_x[NVOICE]; int32_t op2_lfo_cv_x[NVOICE]; int32_t op2_morphbase_out_x[NVOICE]; int32_t op2_specenv_cv_x[NVOICE]; int32_t op2_specenv_active_x[NVOICE]; int32_t op1_phase_env_cv_x[NVOICE]; int32_t op1_phase_lfo_cv_x[NVOICE]; int32_t op1_phase_mod_cv_x[NVOICE]; int32_t op1_speclfo_cv_x[NVOICE]; int32_t op1_velrate_cv_x[NVOICE]; int32_t op1_eg_cv_x[NVOICE]; int32_t op1_velfm_cv_x[NVOICE]; int32_t op1_velamp_cv_x[NVOICE]; int32_t op1_ampvel_cv_x[NVOICE]; int32_t op1_dlfo_cv_x[NVOICE]; int32_t op1_dsum_cv_x[NVOICE]; int32_t op1_fblfo_cv_x[NVOICE]; int32_t op1_fbenv_cv_x[NVOICE]; int32_t op1_fbenv_active_x[NVOICE]; int32_t op1_lfo_cv_x[NVOICE]; int32_t op1_morphbase_out_x[NVOICE]; int32_t op1_specenv_cv_x[NVOICE]; int32_t op1_specenv_active_x[NVOICE]; int32_t type_p_alpha_render_bus0[CONTROL_BLOCK]; int32_t type_p_alpha_render_bus1[CONTROL_BLOCK]; int32_t type_p_alpha_tap0[N_TAP_CH][DSP_TAP_FRAMES]; int32_t type_p_alpha_tap1[N_TAP_CH][DSP_TAP_FRAMES]; int16_t op1_osc_bank[10880]; int16_t op1_phase_map[18432]; int16_t op1_phase_lfo_pool[10880]; int16_t op1_phase_env_pool[10880]; int16_t op2_osc_bank[10880]; int16_t op2_phase_map[18432]; int16_t op2_phase_lfo_pool[10880]; int16_t op2_phase_env_pool[10880]; int16_t op3_osc_bank[10880]; int16_t op3_phase_map[18432]; int16_t op3_phase_lfo_pool[10880]; int16_t op3_phase_env_pool[10880]; int16_t op4_osc_bank[10880]; int16_t op4_phase_map[18432]; int16_t op4_phase_lfo_pool[10880]; int16_t op4_phase_env_pool[10880]; uint16_t voice_rel[NVOICE]; uint8_t voice_gate[NVOICE]; uint8_t voice_note[NVOICE]; uint8_t voice_trig[NVOICE]; uint8_t voice_part[NVOICE]; uint8_t mono_held[N_PART][128]; uint8_t part_mono_voice[N_PART][8]; uint8_t part_mono_count[N_PART]; uint8_t pan_env_stage[N_PART]; uint8_t pan_env_trig[N_PART]; }`

### `type_p_alpha_init_entry`

`typedef` at `engine/gen/dsp.h:1692` through line 1692.

Fields and enumerators:

- `struct { uint16_t id; int32_t value; }`

### `type_p_alpha_state`

`typedef` at `engine/gen/type_p_alpha_dsp.c:13` through line 1673.

The generated engine state. One instance per plugin instance.

Fields and enumerators:

- `struct type_p_alpha_state { uint32_t op1_ph_inc[NVOICE]; uint32_t op1_ph_phase[NVOICE]; uint32_t op1_ph_step[NVOICE]; uint32_t op1_ph_mip[NVOICE]; uint32_t op1_osc_wave[NVOICE]; uint32_t op1_osc_base[NVOICE]; uint32_t op1_osc_bl[NVOICE]; uint32_t op1_osc_tick[NVOICE]; int32_t op1_osc_live[NVOICE]; int32_t op1_osc_morph_macro[NVOICE]; uint32_t op1_eg_stage[NVOICE]; int32_t op1_eg_level[NVOICE]; int32_t op1_eg_warm_start[NVOICE]; uint32_t op1_eg_warm_progress[NVOICE]; uint32_t op1_eg_warm_eff[NVOICE]; uint32_t op1_eg_surge_eff[NVOICE]; uint32_t op1_eg_d1_eff[NVOICE]; uint32_t op1_eg_d2_eff[NVOICE]; uint32_t op1_eg_rel_eff[NVOICE]; int32_t op1_eg_rate_macro[NVOICE]; int32_t op1_eg_warm_base[NVOICE]; int32_t op1_eg_surge_base[NVOICE]; int32_t op1_eg_d1_base[NVOICE]; int32_t op1_eg_d2_base[NVOICE]; int32_t op1_eg_rel_base[NVOICE]; uint32_t op1_lfo_phase[NVOICE]; uint32_t op1_lfo_base[NVOICE]; int32_t op1_lfo_w[NVOICE]; uint32_t op1_lfo_rate_level[NVOICE]; uint32_t op1_lfo_depth_level[NVOICE]; uint32_t op1_lfo_rate_step[NVOICE]; uint32_t op1_lfo_depth_step[NVOICE]; uint32_t op1_lfo_rate_ratio[NVOICE]; int32_t op1_lfo_depth_macro[NVOICE]; uint32_t op1_fbenv_stage[NVOICE]; int32_t op1_fbenv_level[NVOICE]; uint32_t op1_fbenv_r1_eff[NVOICE]; uint32_t op1_fbenv_r2_eff[NVOICE]; uint32_t op1_fbenv_r3_eff[NVOICE]; uint32_t op1_fbenv_r4_eff[NVOICE]; int32_t op1_fbenv_rate_macro[NVOICE]; int32_t op1_fbenv_r1_base[NVOICE]; int32_t op1_fbenv_r2_base[NVOICE]; int32_t op1_fbenv_r3_base[NVOICE]; int32_t op1_fbenv_relr_base[NVOICE]; uint32_t op1_fblfo_phase[NVOICE]; uint32_t op1_fblfo_base[NVOICE]; int32_t op1_fblfo_w[NVOICE]; uint32_t op1_fblfo_rate_level[NVOICE]; uint32_t op1_fblfo_depth_level[NVOICE]; uint32_t op1_fblfo_rate_step[NVOICE]; uint32_t op1_fblfo_depth_step[NVOICE]; uint32_t op1_fblfo_rate_ratio[NVOICE]; int32_t op1_fblfo_depth_macro[NVOICE]; uint32_t op1_dlfo_phase[NVOICE]; uint32_t op1_dlfo_base[NVOICE]; int32_t op1_dlfo_w[NVOICE]; uint32_t op1_dlfo_rate_level[NVOICE]; uint32_t op1_dlfo_depth_level[NVOICE]; uint32_t op1_dlfo_rate_step[NVOICE]; uint32_t op1_dlfo_depth_step[NVOICE]; uint32_t op1_dlfo_rate_ratio[NVOICE]; int32_t op1_dlfo_depth_macro[NVOICE]; uint32_t op1_tl_level_eff[NVOICE]; int32_t op1_fb_signed[NVOICE]; int32_t op1_fb_level_eff[NVOICE]; uint32_t op1_fb_negative[NVOICE]; int32_t op1_fb_prev[NVOICE]; int32_t op1_fb_shaped[NVOICE]; int32_t op1_fb_macro[NVOICE]; int32_t op1_fb_level_macro[NVOICE]; int32_t op1_level_ks_k[NVOICE]; int32_t op1_fb_ks_k[NVOICE]; int32_t op1_eg_ks_k[NVOICE]; int32_t op1_level_vs_k[NVOICE]; int32_t op1_fb_vs_k[NVOICE]; int32_t op1_eg_vs_k[NVOICE]; int32_t op1_d1_prev[NVOICE]; uint32_t op1_specenv_stage[NVOICE]; int32_t op1_specenv_level[NVOICE]; uint32_t op1_specenv_r1_eff[NVOICE]; uint32_t op1_specenv_r2_eff[NVOICE]; uint32_t op1_specenv_r3_eff[NVOICE]; uint32_t op1_specenv_r4_eff[NVOICE]; int32_t op1_specenv_rate_macro[NVOICE]; int32_t op1_specenv_r1_base[NVOICE]; int32_t op1_specenv_r2_base[NVOICE]; int32_t op1_specenv_r3_base[NVOICE]; int32_t op1_specenv_relr_base[NVOICE]; uint32_t op1_speclfo_phase[NVOICE]; uint32_t op1_speclfo_base[NVOICE]; int32_t op1_speclfo_w[NVOICE]; uint32_t op1_speclfo_rate_level[NVOICE]; uint32_t op1_speclfo_depth_level[NVOICE]; uint32_t op1_speclfo_rate_step[NVOICE]; uint32_t op1_speclfo_depth_step[NVOICE]; uint32_t op1_speclfo_rate_ratio[NVOICE]; int32_t op1_speclfo_depth_macro[NVOICE]; uint32_t op2_ph_inc[NVOICE]; uint32_t op2_ph_phase[NVOICE]; uint32_t op2_ph_step[NVOICE]; uint32_t op2_ph_mip[NVOICE]; uint32_t op2_osc_wave[NVOICE]; uint32_t op2_osc_base[NVOICE]; uint32_t op2_osc_bl[NVOICE]; uint32_t op2_osc_tick[NVOICE]; int32_t op2_osc_live[NVOICE]; int32_t op2_osc_morph_macro[NVOICE]; uint32_t op2_eg_stage[NVOICE]; int32_t op2_eg_level[NVOICE]; int32_t op2_eg_warm_start[NVOICE]; uint32_t op2_eg_warm_progress[NVOICE]; uint32_t op2_eg_warm_eff[NVOICE]; uint32_t op2_eg_surge_eff[NVOICE]; uint32_t op2_eg_d1_eff[NVOICE]; uint32_t op2_eg_d2_eff[NVOICE]; uint32_t op2_eg_rel_eff[NVOICE]; int32_t op2_eg_rate_macro[NVOICE]; int32_t op2_eg_warm_base[NVOICE]; int32_t op2_eg_surge_base[NVOICE]; int32_t op2_eg_d1_base[NVOICE]; int32_t op2_eg_d2_base[NVOICE]; int32_t op2_eg_rel_base[NVOICE]; uint32_t op2_lfo_phase[NVOICE]; uint32_t op2_lfo_base[NVOICE]; int32_t op2_lfo_w[NVOICE]; uint32_t op2_lfo_rate_level[NVOICE]; uint32_t op2_lfo_depth_level[NVOICE]; uint32_t op2_lfo_rate_step[NVOICE]; uint32_t op2_lfo_depth_step[NVOICE]; uint32_t op2_lfo_rate_ratio[NVOICE]; int32_t op2_lfo_depth_macro[NVOICE]; uint32_t op2_fbenv_stage[NVOICE]; int32_t op2_fbenv_level[NVOICE]; uint32_t op2_fbenv_r1_eff[NVOICE]; uint32_t op2_fbenv_r2_eff[NVOICE]; uint32_t op2_fbenv_r3_eff[NVOICE]; uint32_t op2_fbenv_r4_eff[NVOICE]; int32_t op2_fbenv_rate_macro[NVOICE]; int32_t op2_fbenv_r1_base[NVOICE]; int32_t op2_fbenv_r2_base[NVOICE]; int32_t op2_fbenv_r3_base[NVOICE]; int32_t op2_fbenv_relr_base[NVOICE]; uint32_t op2_fblfo_phase[NVOICE]; uint32_t op2_fblfo_base[NVOICE]; int32_t op2_fblfo_w[NVOICE]; uint32_t op2_fblfo_rate_level[NVOICE]; uint32_t op2_fblfo_depth_level[NVOICE]; uint32_t op2_fblfo_rate_step[NVOICE]; uint32_t op2_fblfo_depth_step[NVOICE]; uint32_t op2_fblfo_rate_ratio[NVOICE]; int32_t op2_fblfo_depth_macro[NVOICE]; uint32_t op2_dlfo_phase[NVOICE]; uint32_t op2_dlfo_base[NVOICE]; int32_t op2_dlfo_w[NVOICE]; uint32_t op2_dlfo_rate_level[NVOICE]; uint32_t op2_dlfo_depth_level[NVOICE]; uint32_t op2_dlfo_rate_step[NVOICE]; uint32_t op2_dlfo_depth_step[NVOICE]; uint32_t op2_dlfo_rate_ratio[NVOICE]; int32_t op2_dlfo_depth_macro[NVOICE]; uint32_t op2_tl_level_eff[NVOICE]; int32_t op2_fb_signed[NVOICE]; int32_t op2_fb_level_eff[NVOICE]; uint32_t op2_fb_negative[NVOICE]; int32_t op2_fb_prev[NVOICE]; int32_t op2_fb_shaped[NVOICE]; int32_t op2_fb_macro[NVOICE]; int32_t op2_fb_level_macro[NVOICE]; int32_t op2_level_ks_k[NVOICE]; int32_t op2_fb_ks_k[NVOICE]; int32_t op2_eg_ks_k[NVOICE]; int32_t op2_level_vs_k[NVOICE]; int32_t op2_fb_vs_k[NVOICE]; int32_t op2_eg_vs_k[NVOICE]; int32_t op2_d1_prev[NVOICE]; uint32_t op2_specenv_stage[NVOICE]; int32_t op2_specenv_level[NVOICE]; uint32_t op2_specenv_r1_eff[NVOICE]; uint32_t op2_specenv_r2_eff[NVOICE]; uint32_t op2_specenv_r3_eff[NVOICE]; uint32_t op2_specenv_r4_eff[NVOICE]; int32_t op2_specenv_rate_macro[NVOICE]; int32_t op2_specenv_r1_base[NVOICE]; int32_t op2_specenv_r2_base[NVOICE]; int32_t op2_specenv_r3_base[NVOICE]; int32_t op2_specenv_relr_base[NVOICE]; uint32_t op2_speclfo_phase[NVOICE]; uint32_t op2_speclfo_base[NVOICE]; int32_t op2_speclfo_w[NVOICE]; uint32_t op2_speclfo_rate_level[NVOICE]; uint32_t op2_speclfo_depth_level[NVOICE]; uint32_t op2_speclfo_rate_step[NVOICE]; uint32_t op2_speclfo_depth_step[NVOICE]; uint32_t op2_speclfo_rate_ratio[NVOICE]; int32_t op2_speclfo_depth_macro[NVOICE]; uint32_t op3_ph_inc[NVOICE]; uint32_t op3_ph_phase[NVOICE]; uint32_t op3_ph_step[NVOICE]; uint32_t op3_ph_mip[NVOICE]; uint32_t op3_osc_wave[NVOICE]; uint32_t op3_osc_base[NVOICE]; uint32_t op3_osc_bl[NVOICE]; uint32_t op3_osc_tick[NVOICE]; int32_t op3_osc_live[NVOICE]; int32_t op3_osc_morph_macro[NVOICE]; uint32_t op3_eg_stage[NVOICE]; int32_t op3_eg_level[NVOICE]; int32_t op3_eg_warm_start[NVOICE]; uint32_t op3_eg_warm_progress[NVOICE]; uint32_t op3_eg_warm_eff[NVOICE]; uint32_t op3_eg_surge_eff[NVOICE]; uint32_t op3_eg_d1_eff[NVOICE]; uint32_t op3_eg_d2_eff[NVOICE]; uint32_t op3_eg_rel_eff[NVOICE]; int32_t op3_eg_rate_macro[NVOICE]; int32_t op3_eg_warm_base[NVOICE]; int32_t op3_eg_surge_base[NVOICE]; int32_t op3_eg_d1_base[NVOICE]; int32_t op3_eg_d2_base[NVOICE]; int32_t op3_eg_rel_base[NVOICE]; uint32_t op3_lfo_phase[NVOICE]; uint32_t op3_lfo_base[NVOICE]; int32_t op3_lfo_w[NVOICE]; uint32_t op3_lfo_rate_level[NVOICE]; uint32_t op3_lfo_depth_level[NVOICE]; uint32_t op3_lfo_rate_step[NVOICE]; uint32_t op3_lfo_depth_step[NVOICE]; uint32_t op3_lfo_rate_ratio[NVOICE]; int32_t op3_lfo_depth_macro[NVOICE]; uint32_t op3_fbenv_stage[NVOICE]; int32_t op3_fbenv_level[NVOICE]; uint32_t op3_fbenv_r1_eff[NVOICE]; uint32_t op3_fbenv_r2_eff[NVOICE]; uint32_t op3_fbenv_r3_eff[NVOICE]; uint32_t op3_fbenv_r4_eff[NVOICE]; int32_t op3_fbenv_rate_macro[NVOICE]; int32_t op3_fbenv_r1_base[NVOICE]; int32_t op3_fbenv_r2_base[NVOICE]; int32_t op3_fbenv_r3_base[NVOICE]; int32_t op3_fbenv_relr_base[NVOICE]; uint32_t op3_fblfo_phase[NVOICE]; uint32_t op3_fblfo_base[NVOICE]; int32_t op3_fblfo_w[NVOICE]; uint32_t op3_fblfo_rate_level[NVOICE]; uint32_t op3_fblfo_depth_level[NVOICE]; uint32_t op3_fblfo_rate_step[NVOICE]; uint32_t op3_fblfo_depth_step[NVOICE]; uint32_t op3_fblfo_rate_ratio[NVOICE]; int32_t op3_fblfo_depth_macro[NVOICE]; uint32_t op3_dlfo_phase[NVOICE]; uint32_t op3_dlfo_base[NVOICE]; int32_t op3_dlfo_w[NVOICE]; uint32_t op3_dlfo_rate_level[NVOICE]; uint32_t op3_dlfo_depth_level[NVOICE]; uint32_t op3_dlfo_rate_step[NVOICE]; uint32_t op3_dlfo_depth_step[NVOICE]; uint32_t op3_dlfo_rate_ratio[NVOICE]; int32_t op3_dlfo_depth_macro[NVOICE]; uint32_t op3_tl_level_eff[NVOICE]; int32_t op3_fb_signed[NVOICE]; int32_t op3_fb_level_eff[NVOICE]; uint32_t op3_fb_negative[NVOICE]; int32_t op3_fb_prev[NVOICE]; int32_t op3_fb_shaped[NVOICE]; int32_t op3_fb_macro[NVOICE]; int32_t op3_fb_level_macro[NVOICE]; int32_t op3_level_ks_k[NVOICE]; int32_t op3_fb_ks_k[NVOICE]; int32_t op3_eg_ks_k[NVOICE]; int32_t op3_level_vs_k[NVOICE]; int32_t op3_fb_vs_k[NVOICE]; int32_t op3_eg_vs_k[NVOICE]; int32_t op3_d1_prev[NVOICE]; uint32_t op3_specenv_stage[NVOICE]; int32_t op3_specenv_level[NVOICE]; uint32_t op3_specenv_r1_eff[NVOICE]; uint32_t op3_specenv_r2_eff[NVOICE]; uint32_t op3_specenv_r3_eff[NVOICE]; uint32_t op3_specenv_r4_eff[NVOICE]; int32_t op3_specenv_rate_macro[NVOICE]; int32_t op3_specenv_r1_base[NVOICE]; int32_t op3_specenv_r2_base[NVOICE]; int32_t op3_specenv_r3_base[NVOICE]; int32_t op3_specenv_relr_base[NVOICE]; uint32_t op3_speclfo_phase[NVOICE]; uint32_t op3_speclfo_base[NVOICE]; int32_t op3_speclfo_w[NVOICE]; uint32_t op3_speclfo_rate_level[NVOICE]; uint32_t op3_speclfo_depth_level[NVOICE]; uint32_t op3_speclfo_rate_step[NVOICE]; uint32_t op3_speclfo_depth_step[NVOICE]; uint32_t op3_speclfo_rate_ratio[NVOICE]; int32_t op3_speclfo_depth_macro[NVOICE]; uint32_t op4_ph_inc[NVOICE]; uint32_t op4_ph_phase[NVOICE]; uint32_t op4_ph_step[NVOICE]; uint32_t op4_ph_mip[NVOICE]; uint32_t op4_osc_wave[NVOICE]; uint32_t op4_osc_base[NVOICE]; uint32_t op4_osc_bl[NVOICE]; uint32_t op4_osc_tick[NVOICE]; int32_t op4_osc_live[NVOICE]; int32_t op4_osc_morph_macro[NVOICE]; uint32_t op4_eg_stage[NVOICE]; int32_t op4_eg_level[NVOICE]; int32_t op4_eg_warm_start[NVOICE]; uint32_t op4_eg_warm_progress[NVOICE]; uint32_t op4_eg_warm_eff[NVOICE]; uint32_t op4_eg_surge_eff[NVOICE]; uint32_t op4_eg_d1_eff[NVOICE]; uint32_t op4_eg_d2_eff[NVOICE]; uint32_t op4_eg_rel_eff[NVOICE]; int32_t op4_eg_rate_macro[NVOICE]; int32_t op4_eg_warm_base[NVOICE]; int32_t op4_eg_surge_base[NVOICE]; int32_t op4_eg_d1_base[NVOICE]; int32_t op4_eg_d2_base[NVOICE]; int32_t op4_eg_rel_base[NVOICE]; uint32_t op4_lfo_phase[NVOICE]; uint32_t op4_lfo_base[NVOICE]; int32_t op4_lfo_w[NVOICE]; uint32_t op4_lfo_rate_level[NVOICE]; uint32_t op4_lfo_depth_level[NVOICE]; uint32_t op4_lfo_rate_step[NVOICE]; uint32_t op4_lfo_depth_step[NVOICE]; uint32_t op4_lfo_rate_ratio[NVOICE]; int32_t op4_lfo_depth_macro[NVOICE]; uint32_t op4_fbenv_stage[NVOICE]; int32_t op4_fbenv_level[NVOICE]; uint32_t op4_fbenv_r1_eff[NVOICE]; uint32_t op4_fbenv_r2_eff[NVOICE]; uint32_t op4_fbenv_r3_eff[NVOICE]; uint32_t op4_fbenv_r4_eff[NVOICE]; int32_t op4_fbenv_rate_macro[NVOICE]; int32_t op4_fbenv_r1_base[NVOICE]; int32_t op4_fbenv_r2_base[NVOICE]; int32_t op4_fbenv_r3_base[NVOICE]; int32_t op4_fbenv_relr_base[NVOICE]; uint32_t op4_fblfo_phase[NVOICE]; uint32_t op4_fblfo_base[NVOICE]; int32_t op4_fblfo_w[NVOICE]; uint32_t op4_fblfo_rate_level[NVOICE]; uint32_t op4_fblfo_depth_level[NVOICE]; uint32_t op4_fblfo_rate_step[NVOICE]; uint32_t op4_fblfo_depth_step[NVOICE]; uint32_t op4_fblfo_rate_ratio[NVOICE]; int32_t op4_fblfo_depth_macro[NVOICE]; uint32_t op4_dlfo_phase[NVOICE]; uint32_t op4_dlfo_base[NVOICE]; int32_t op4_dlfo_w[NVOICE]; uint32_t op4_dlfo_rate_level[NVOICE]; uint32_t op4_dlfo_depth_level[NVOICE]; uint32_t op4_dlfo_rate_step[NVOICE]; uint32_t op4_dlfo_depth_step[NVOICE]; uint32_t op4_dlfo_rate_ratio[NVOICE]; int32_t op4_dlfo_depth_macro[NVOICE]; uint32_t op4_tl_level_eff[NVOICE]; int32_t op4_fb_signed[NVOICE]; int32_t op4_fb_level_eff[NVOICE]; uint32_t op4_fb_negative[NVOICE]; int32_t op4_fb_prev[NVOICE]; int32_t op4_fb_shaped[NVOICE]; int32_t op4_fb_macro[NVOICE]; int32_t op4_fb_level_macro[NVOICE]; int32_t op4_level_ks_k[NVOICE]; int32_t op4_fb_ks_k[NVOICE]; int32_t op4_eg_ks_k[NVOICE]; int32_t op4_level_vs_k[NVOICE]; int32_t op4_fb_vs_k[NVOICE]; int32_t op4_eg_vs_k[NVOICE]; int32_t op4_d1_prev[NVOICE]; uint32_t op4_specenv_stage[NVOICE]; int32_t op4_specenv_level[NVOICE]; uint32_t op4_specenv_r1_eff[NVOICE]; uint32_t op4_specenv_r2_eff[NVOICE]; uint32_t op4_specenv_r3_eff[NVOICE]; uint32_t op4_specenv_r4_eff[NVOICE]; int32_t op4_specenv_rate_macro[NVOICE]; int32_t op4_specenv_r1_base[NVOICE]; int32_t op4_specenv_r2_base[NVOICE]; int32_t op4_specenv_r3_base[NVOICE]; int32_t op4_specenv_relr_base[NVOICE]; uint32_t op4_speclfo_phase[NVOICE]; uint32_t op4_speclfo_base[NVOICE]; int32_t op4_speclfo_w[NVOICE]; uint32_t op4_speclfo_rate_level[NVOICE]; uint32_t op4_speclfo_depth_level[NVOICE]; uint32_t op4_speclfo_rate_step[NVOICE]; uint32_t op4_speclfo_depth_step[NVOICE]; uint32_t op4_speclfo_rate_ratio[NVOICE]; int32_t op4_speclfo_depth_macro[NVOICE]; int32_t d1_prev[NVOICE]; int32_t d2_prev[NVOICE]; int32_t d3_prev[NVOICE]; int32_t d4_prev[NVOICE]; uint32_t penv_stage[NVOICE]; int32_t penv_level[NVOICE]; uint32_t penv_r1_eff[NVOICE]; uint32_t penv_r2_eff[NVOICE]; uint32_t penv_r3_eff[NVOICE]; uint32_t penv_r4_eff[NVOICE]; int32_t penv_rate_macro[NVOICE]; int32_t penv_r1_base[NVOICE]; int32_t penv_r2_base[NVOICE]; int32_t penv_r3_base[NVOICE]; int32_t penv_relr_base[NVOICE]; int32_t penv_ks_k[NVOICE]; int32_t penv_vs_k[NVOICE]; uint32_t plfo_phase[NVOICE]; uint32_t plfo_base[NVOICE]; int32_t plfo_w[NVOICE]; uint32_t plfo_rate_level[NVOICE]; uint32_t plfo_depth_level[NVOICE]; uint32_t plfo_rate_step[NVOICE]; uint32_t plfo_depth_step[NVOICE]; uint32_t plfo_rate_ratio[NVOICE]; int32_t plfo_depth_macro[NVOICE]; int32_t f1_ks_k[NVOICE]; int32_t fenv_ks_k[NVOICE]; int32_t f1_vs_k[NVOICE]; int32_t fenv_vs_k[NVOICE]; int32_t lp_z1[NVOICE]; int32_t lp_z2[NVOICE]; int32_t lp_z3[NVOICE]; int32_t lp_z4[NVOICE]; int32_t lp_fc[NVOICE]; int32_t lp_res_k[NVOICE]; int32_t lp_env[NVOICE]; int32_t lp_gate[NVOICE]; uint32_t fenv_stage[NVOICE]; int32_t fenv_level[NVOICE]; uint32_t fenv_r1_eff[NVOICE]; uint32_t fenv_r2_eff[NVOICE]; uint32_t fenv_r3_eff[NVOICE]; uint32_t fenv_r4_eff[NVOICE]; int32_t fenv_rate_macro[NVOICE]; int32_t fenv_r1_base[NVOICE]; int32_t fenv_r2_base[NVOICE]; int32_t fenv_r3_base[NVOICE]; int32_t fenv_relr_base[NVOICE]; int32_t dc_x1; int32_t dc_y1; uint32_t voice_age[NVOICE]; int32_t voice_velocity[NVOICE]; uint32_t voice_delay[NVOICE]; uint32_t voice_inc[NVOICE]; uint32_t voice_target_inc[NVOICE]; int32_t voice_detune[NVOICE]; uint32_t mono_order[N_PART][128]; uint32_t mono_serial[N_PART]; int32_t part_bend_q16[N_PART]; int32_t pan_l_q16[N_PART]; int32_t pan_r_q16[N_PART]; int32_t pan_env_level[N_PART]; uint32_t ages; int32_t vm_mode[2]; int32_t vm_unison_count[2]; int32_t vm_unison_detune[2]; int32_t vm_detune[2]; int32_t vm_transpose[2]; int32_t vm_legato[2]; int32_t vm_portamento[2]; int32_t vm_note_priority[2]; int32_t vm_bend_range[2]; int32_t vm_pan[2]; int32_t vm_pan_env_on[2]; int32_t vm_pan_env_r1[2]; int32_t vm_pan_env_r2[2]; int32_t vm_pan_env_r3[2]; int32_t vm_pan_env_l1[2]; int32_t vm_pan_env_l2[2]; int32_t vm_pan_env_l3[2]; int32_t vm_pan_env_relr[2]; int32_t vm_pan_env_rell[2]; int32_t vm_pan_env_mode[2]; int32_t part_channel[2]; int32_t part_low[2]; int32_t part_high[2]; int32_t part_level[2]; int32_t part_delay[2]; int32_t op1_ph_inc_ratio[2]; int32_t op1_ph_detune[2]; int32_t op1_osc_waveform[2]; int32_t op1_osc_spec_range_a[2]; int32_t op1_osc_spec_range_b[2]; int32_t op1_osc_spec_a1[2]; int32_t op1_osc_spec_a2[2]; int32_t op1_osc_spec_a3[2]; int32_t op1_osc_spec_a4[2]; int32_t op1_osc_spec_a5[2]; int32_t op1_osc_spec_a6[2]; int32_t op1_osc_spec_a7[2]; int32_t op1_osc_spec_a8[2]; int32_t op1_osc_spec_a9[2]; int32_t op1_osc_spec_a10[2]; int32_t op1_osc_spec_a11[2]; int32_t op1_osc_spec_a12[2]; int32_t op1_osc_spec_a13[2]; int32_t op1_osc_spec_a14[2]; int32_t op1_osc_spec_a15[2]; int32_t op1_osc_spec_a16[2]; int32_t op1_osc_spec_a17[2]; int32_t op1_osc_spec_a18[2]; int32_t op1_osc_spec_a19[2]; int32_t op1_osc_spec_a20[2]; int32_t op1_osc_spec_a21[2]; int32_t op1_osc_spec_a22[2]; int32_t op1_osc_spec_a23[2]; int32_t op1_osc_spec_a24[2]; int32_t op1_osc_spec_a25[2]; int32_t op1_osc_spec_a26[2]; int32_t op1_osc_spec_a27[2]; int32_t op1_osc_spec_a28[2]; int32_t op1_osc_spec_a29[2]; int32_t op1_osc_spec_a30[2]; int32_t op1_osc_spec_a31[2]; int32_t op1_osc_spec_a32[2]; int32_t op1_osc_spec_a33[2]; int32_t op1_osc_spec_a34[2]; int32_t op1_osc_spec_a35[2]; int32_t op1_osc_spec_a36[2]; int32_t op1_osc_spec_a37[2]; int32_t op1_osc_spec_a38[2]; int32_t op1_osc_spec_a39[2]; int32_t op1_osc_spec_a40[2]; int32_t op1_osc_spec_a41[2]; int32_t op1_osc_spec_a42[2]; int32_t op1_osc_spec_a43[2]; int32_t op1_osc_spec_a44[2]; int32_t op1_osc_spec_a45[2]; int32_t op1_osc_spec_a46[2]; int32_t op1_osc_spec_a47[2]; int32_t op1_osc_spec_a48[2]; int32_t op1_osc_spec_a49[2]; int32_t op1_osc_spec_a50[2]; int32_t op1_osc_spec_a51[2]; int32_t op1_osc_spec_a52[2]; int32_t op1_osc_spec_a53[2]; int32_t op1_osc_spec_a54[2]; int32_t op1_osc_spec_a55[2]; int32_t op1_osc_spec_a56[2]; int32_t op1_osc_spec_a57[2]; int32_t op1_osc_spec_a58[2]; int32_t op1_osc_spec_a59[2]; int32_t op1_osc_spec_a60[2]; int32_t op1_osc_spec_a61[2]; int32_t op1_osc_spec_a62[2]; int32_t op1_osc_spec_a63[2]; int32_t op1_osc_spec_a64[2]; int32_t op1_osc_spec_b1[2]; int32_t op1_osc_spec_b2[2]; int32_t op1_osc_spec_b3[2]; int32_t op1_osc_spec_b4[2]; int32_t op1_osc_spec_b5[2]; int32_t op1_osc_spec_b6[2]; int32_t op1_osc_spec_b7[2]; int32_t op1_osc_spec_b8[2]; int32_t op1_osc_spec_b9[2]; int32_t op1_osc_spec_b10[2]; int32_t op1_osc_spec_b11[2]; int32_t op1_osc_spec_b12[2]; int32_t op1_osc_spec_b13[2]; int32_t op1_osc_spec_b14[2]; int32_t op1_osc_spec_b15[2]; int32_t op1_osc_spec_b16[2]; int32_t op1_osc_spec_b17[2]; int32_t op1_osc_spec_b18[2]; int32_t op1_osc_spec_b19[2]; int32_t op1_osc_spec_b20[2]; int32_t op1_osc_spec_b21[2]; int32_t op1_osc_spec_b22[2]; int32_t op1_osc_spec_b23[2]; int32_t op1_osc_spec_b24[2]; int32_t op1_osc_spec_b25[2]; int32_t op1_osc_spec_b26[2]; int32_t op1_osc_spec_b27[2]; int32_t op1_osc_spec_b28[2]; int32_t op1_osc_spec_b29[2]; int32_t op1_osc_spec_b30[2]; int32_t op1_osc_spec_b31[2]; int32_t op1_osc_spec_b32[2]; int32_t op1_osc_spec_b33[2]; int32_t op1_osc_spec_b34[2]; int32_t op1_osc_spec_b35[2]; int32_t op1_osc_spec_b36[2]; int32_t op1_osc_spec_b37[2]; int32_t op1_osc_spec_b38[2]; int32_t op1_osc_spec_b39[2]; int32_t op1_osc_spec_b40[2]; int32_t op1_osc_spec_b41[2]; int32_t op1_osc_spec_b42[2]; int32_t op1_osc_spec_b43[2]; int32_t op1_osc_spec_b44[2]; int32_t op1_osc_spec_b45[2]; int32_t op1_osc_spec_b46[2]; int32_t op1_osc_spec_b47[2]; int32_t op1_osc_spec_b48[2]; int32_t op1_osc_spec_b49[2]; int32_t op1_osc_spec_b50[2]; int32_t op1_osc_spec_b51[2]; int32_t op1_osc_spec_b52[2]; int32_t op1_osc_spec_b53[2]; int32_t op1_osc_spec_b54[2]; int32_t op1_osc_spec_b55[2]; int32_t op1_osc_spec_b56[2]; int32_t op1_osc_spec_b57[2]; int32_t op1_osc_spec_b58[2]; int32_t op1_osc_spec_b59[2]; int32_t op1_osc_spec_b60[2]; int32_t op1_osc_spec_b61[2]; int32_t op1_osc_spec_b62[2]; int32_t op1_osc_spec_b63[2]; int32_t op1_osc_spec_b64[2]; int32_t op1_morphbase_value[2]; int32_t op1_eg_warm[2]; int32_t op1_eg_surge[2]; int32_t op1_eg_d1r[2]; int32_t op1_eg_d1l[2]; int32_t op1_eg_d2r[2]; int32_t op1_eg_d2l[2]; int32_t op1_eg_rel[2]; int32_t op1_eg_mode[2]; int32_t op1_lfo_rate[2]; int32_t op1_lfo_depth[2]; int32_t op1_lfo_wave[2]; int32_t op1_lfo_mode[2]; int32_t op1_lfo_rate_fade[2]; int32_t op1_lfo_depth_fade[2]; int32_t op1_fbenv_on[2]; int32_t op1_fbenv_r1[2]; int32_t op1_fbenv_r2[2]; int32_t op1_fbenv_r3[2]; int32_t op1_fbenv_l1[2]; int32_t op1_fbenv_l2[2]; int32_t op1_fbenv_l3[2]; int32_t op1_fbenv_relr[2]; int32_t op1_fbenv_rell[2]; int32_t op1_fbenv_mode[2]; int32_t op1_fblfo_rate[2]; int32_t op1_fblfo_depth[2]; int32_t op1_fblfo_wave[2]; int32_t op1_fblfo_mode[2]; int32_t op1_fblfo_rate_fade[2]; int32_t op1_fblfo_depth_fade[2]; int32_t op1_dlfo_rate[2]; int32_t op1_dlfo_depth[2]; int32_t op1_dlfo_wave[2]; int32_t op1_dlfo_mode[2]; int32_t op1_dlfo_rate_fade[2]; int32_t op1_dlfo_depth_fade[2]; int32_t op1_velamp_depth[2]; int32_t op1_velfm_depth[2]; int32_t op1_velrate_depth[2]; int32_t op1_tl_level[2]; int32_t op1_fb_level[2]; int32_t op1_level_ks_lo[2]; int32_t op1_level_ks_hi[2]; int32_t op1_fb_ks_lo[2]; int32_t op1_fb_ks_hi[2]; int32_t op1_eg_ks_lo[2]; int32_t op1_eg_ks_hi[2]; int32_t op1_level_vs_amount[2]; int32_t op1_fb_vs_amount[2]; int32_t op1_eg_vs_amount[2]; int32_t op1_mute_mute[2]; int32_t op1_specenv_on[2]; int32_t op1_specenv_r1[2]; int32_t op1_specenv_r2[2]; int32_t op1_specenv_r3[2]; int32_t op1_specenv_l1[2]; int32_t op1_specenv_l2[2]; int32_t op1_specenv_l3[2]; int32_t op1_specenv_relr[2]; int32_t op1_specenv_rell[2]; int32_t op1_specenv_mode[2]; int32_t op1_speclfo_rate[2]; int32_t op1_speclfo_depth[2]; int32_t op1_speclfo_wave[2]; int32_t op1_speclfo_mode[2]; int32_t op1_speclfo_rate_fade[2]; int32_t op1_speclfo_depth_fade[2]; int32_t op1_phase_on[2]; int32_t op1_phase_bp[2]; int32_t op1_phase_segments[2]; int32_t op1_phase_flip[2]; int32_t op1_phase_flip_rate[2]; int32_t op1_phase_reorder[2]; int32_t op1_phase_reorder_rate[2]; int32_t op1_phase_jump[2]; int32_t op1_phase_drift[2]; int32_t op1_phase_drift_rate[2]; int32_t op1_phase_ks_lo[2]; int32_t op1_phase_ks_hi[2]; int32_t op1_phase_vs[2]; int32_t op1_phase_lfo_rate[2]; int32_t op1_phase_lfo_depth[2]; int32_t op1_phase_lfo_wave[2]; int32_t op1_phase_lfo_mode[2]; int32_t op1_phase_lfo_rate_fade[2]; int32_t op1_phase_lfo_depth_fade[2]; int32_t op1_phase_env_on[2]; int32_t op1_phase_env_r1[2]; int32_t op1_phase_env_r2[2]; int32_t op1_phase_env_r3[2]; int32_t op1_phase_env_l1[2]; int32_t op1_phase_env_l2[2]; int32_t op1_phase_env_l3[2]; int32_t op1_phase_env_relr[2]; int32_t op1_phase_env_rell[2]; int32_t op1_phase_env_mode[2]; int32_t op2_ph_inc_ratio[2]; int32_t op2_ph_detune[2]; int32_t op2_osc_waveform[2]; int32_t op2_osc_spec_range_a[2]; int32_t op2_osc_spec_range_b[2]; int32_t op2_osc_spec_a1[2]; int32_t op2_osc_spec_a2[2]; int32_t op2_osc_spec_a3[2]; int32_t op2_osc_spec_a4[2]; int32_t op2_osc_spec_a5[2]; int32_t op2_osc_spec_a6[2]; int32_t op2_osc_spec_a7[2]; int32_t op2_osc_spec_a8[2]; int32_t op2_osc_spec_a9[2]; int32_t op2_osc_spec_a10[2]; int32_t op2_osc_spec_a11[2]; int32_t op2_osc_spec_a12[2]; int32_t op2_osc_spec_a13[2]; int32_t op2_osc_spec_a14[2]; int32_t op2_osc_spec_a15[2]; int32_t op2_osc_spec_a16[2]; int32_t op2_osc_spec_a17[2]; int32_t op2_osc_spec_a18[2]; int32_t op2_osc_spec_a19[2]; int32_t op2_osc_spec_a20[2]; int32_t op2_osc_spec_a21[2]; int32_t op2_osc_spec_a22[2]; int32_t op2_osc_spec_a23[2]; int32_t op2_osc_spec_a24[2]; int32_t op2_osc_spec_a25[2]; int32_t op2_osc_spec_a26[2]; int32_t op2_osc_spec_a27[2]; int32_t op2_osc_spec_a28[2]; int32_t op2_osc_spec_a29[2]; int32_t op2_osc_spec_a30[2]; int32_t op2_osc_spec_a31[2]; int32_t op2_osc_spec_a32[2]; int32_t op2_osc_spec_a33[2]; int32_t op2_osc_spec_a34[2]; int32_t op2_osc_spec_a35[2]; int32_t op2_osc_spec_a36[2]; int32_t op2_osc_spec_a37[2]; int32_t op2_osc_spec_a38[2]; int32_t op2_osc_spec_a39[2]; int32_t op2_osc_spec_a40[2]; int32_t op2_osc_spec_a41[2]; int32_t op2_osc_spec_a42[2]; int32_t op2_osc_spec_a43[2]; int32_t op2_osc_spec_a44[2]; int32_t op2_osc_spec_a45[2]; int32_t op2_osc_spec_a46[2]; int32_t op2_osc_spec_a47[2]; int32_t op2_osc_spec_a48[2]; int32_t op2_osc_spec_a49[2]; int32_t op2_osc_spec_a50[2]; int32_t op2_osc_spec_a51[2]; int32_t op2_osc_spec_a52[2]; int32_t op2_osc_spec_a53[2]; int32_t op2_osc_spec_a54[2]; int32_t op2_osc_spec_a55[2]; int32_t op2_osc_spec_a56[2]; int32_t op2_osc_spec_a57[2]; int32_t op2_osc_spec_a58[2]; int32_t op2_osc_spec_a59[2]; int32_t op2_osc_spec_a60[2]; int32_t op2_osc_spec_a61[2]; int32_t op2_osc_spec_a62[2]; int32_t op2_osc_spec_a63[2]; int32_t op2_osc_spec_a64[2]; int32_t op2_osc_spec_b1[2]; int32_t op2_osc_spec_b2[2]; int32_t op2_osc_spec_b3[2]; int32_t op2_osc_spec_b4[2]; int32_t op2_osc_spec_b5[2]; int32_t op2_osc_spec_b6[2]; int32_t op2_osc_spec_b7[2]; int32_t op2_osc_spec_b8[2]; int32_t op2_osc_spec_b9[2]; int32_t op2_osc_spec_b10[2]; int32_t op2_osc_spec_b11[2]; int32_t op2_osc_spec_b12[2]; int32_t op2_osc_spec_b13[2]; int32_t op2_osc_spec_b14[2]; int32_t op2_osc_spec_b15[2]; int32_t op2_osc_spec_b16[2]; int32_t op2_osc_spec_b17[2]; int32_t op2_osc_spec_b18[2]; int32_t op2_osc_spec_b19[2]; int32_t op2_osc_spec_b20[2]; int32_t op2_osc_spec_b21[2]; int32_t op2_osc_spec_b22[2]; int32_t op2_osc_spec_b23[2]; int32_t op2_osc_spec_b24[2]; int32_t op2_osc_spec_b25[2]; int32_t op2_osc_spec_b26[2]; int32_t op2_osc_spec_b27[2]; int32_t op2_osc_spec_b28[2]; int32_t op2_osc_spec_b29[2]; int32_t op2_osc_spec_b30[2]; int32_t op2_osc_spec_b31[2]; int32_t op2_osc_spec_b32[2]; int32_t op2_osc_spec_b33[2]; int32_t op2_osc_spec_b34[2]; int32_t op2_osc_spec_b35[2]; int32_t op2_osc_spec_b36[2]; int32_t op2_osc_spec_b37[2]; int32_t op2_osc_spec_b38[2]; int32_t op2_osc_spec_b39[2]; int32_t op2_osc_spec_b40[2]; int32_t op2_osc_spec_b41[2]; int32_t op2_osc_spec_b42[2]; int32_t op2_osc_spec_b43[2]; int32_t op2_osc_spec_b44[2]; int32_t op2_osc_spec_b45[2]; int32_t op2_osc_spec_b46[2]; int32_t op2_osc_spec_b47[2]; int32_t op2_osc_spec_b48[2]; int32_t op2_osc_spec_b49[2]; int32_t op2_osc_spec_b50[2]; int32_t op2_osc_spec_b51[2]; int32_t op2_osc_spec_b52[2]; int32_t op2_osc_spec_b53[2]; int32_t op2_osc_spec_b54[2]; int32_t op2_osc_spec_b55[2]; int32_t op2_osc_spec_b56[2]; int32_t op2_osc_spec_b57[2]; int32_t op2_osc_spec_b58[2]; int32_t op2_osc_spec_b59[2]; int32_t op2_osc_spec_b60[2]; int32_t op2_osc_spec_b61[2]; int32_t op2_osc_spec_b62[2]; int32_t op2_osc_spec_b63[2]; int32_t op2_osc_spec_b64[2]; int32_t op2_morphbase_value[2]; int32_t op2_eg_warm[2]; int32_t op2_eg_surge[2]; int32_t op2_eg_d1r[2]; int32_t op2_eg_d1l[2]; int32_t op2_eg_d2r[2]; int32_t op2_eg_d2l[2]; int32_t op2_eg_rel[2]; int32_t op2_eg_mode[2]; int32_t op2_lfo_rate[2]; int32_t op2_lfo_depth[2]; int32_t op2_lfo_wave[2]; int32_t op2_lfo_mode[2]; int32_t op2_lfo_rate_fade[2]; int32_t op2_lfo_depth_fade[2]; int32_t op2_fbenv_on[2]; int32_t op2_fbenv_r1[2]; int32_t op2_fbenv_r2[2]; int32_t op2_fbenv_r3[2]; int32_t op2_fbenv_l1[2]; int32_t op2_fbenv_l2[2]; int32_t op2_fbenv_l3[2]; int32_t op2_fbenv_relr[2]; int32_t op2_fbenv_rell[2]; int32_t op2_fbenv_mode[2]; int32_t op2_fblfo_rate[2]; int32_t op2_fblfo_depth[2]; int32_t op2_fblfo_wave[2]; int32_t op2_fblfo_mode[2]; int32_t op2_fblfo_rate_fade[2]; int32_t op2_fblfo_depth_fade[2]; int32_t op2_dlfo_rate[2]; int32_t op2_dlfo_depth[2]; int32_t op2_dlfo_wave[2]; int32_t op2_dlfo_mode[2]; int32_t op2_dlfo_rate_fade[2]; int32_t op2_dlfo_depth_fade[2]; int32_t op2_velamp_depth[2]; int32_t op2_velfm_depth[2]; int32_t op2_velrate_depth[2]; int32_t op2_tl_level[2]; int32_t op2_fb_level[2]; int32_t op2_level_ks_lo[2]; int32_t op2_level_ks_hi[2]; int32_t op2_fb_ks_lo[2]; int32_t op2_fb_ks_hi[2]; int32_t op2_eg_ks_lo[2]; int32_t op2_eg_ks_hi[2]; int32_t op2_level_vs_amount[2]; int32_t op2_fb_vs_amount[2]; int32_t op2_eg_vs_amount[2]; int32_t op2_mute_mute[2]; int32_t op2_specenv_on[2]; int32_t op2_specenv_r1[2]; int32_t op2_specenv_r2[2]; int32_t op2_specenv_r3[2]; int32_t op2_specenv_l1[2]; int32_t op2_specenv_l2[2]; int32_t op2_specenv_l3[2]; int32_t op2_specenv_relr[2]; int32_t op2_specenv_rell[2]; int32_t op2_specenv_mode[2]; int32_t op2_speclfo_rate[2]; int32_t op2_speclfo_depth[2]; int32_t op2_speclfo_wave[2]; int32_t op2_speclfo_mode[2]; int32_t op2_speclfo_rate_fade[2]; int32_t op2_speclfo_depth_fade[2]; int32_t op2_phase_on[2]; int32_t op2_phase_bp[2]; int32_t op2_phase_segments[2]; int32_t op2_phase_flip[2]; int32_t op2_phase_flip_rate[2]; int32_t op2_phase_reorder[2]; int32_t op2_phase_reorder_rate[2]; int32_t op2_phase_jump[2]; int32_t op2_phase_drift[2]; int32_t op2_phase_drift_rate[2]; int32_t op2_phase_ks_lo[2]; int32_t op2_phase_ks_hi[2]; int32_t op2_phase_vs[2]; int32_t op2_phase_lfo_rate[2]; int32_t op2_phase_lfo_depth[2]; int32_t op2_phase_lfo_wave[2]; int32_t op2_phase_lfo_mode[2]; int32_t op2_phase_lfo_rate_fade[2]; int32_t op2_phase_lfo_depth_fade[2]; int32_t op2_phase_env_on[2]; int32_t op2_phase_env_r1[2]; int32_t op2_phase_env_r2[2]; int32_t op2_phase_env_r3[2]; int32_t op2_phase_env_l1[2]; int32_t op2_phase_env_l2[2]; int32_t op2_phase_env_l3[2]; int32_t op2_phase_env_relr[2]; int32_t op2_phase_env_rell[2]; int32_t op2_phase_env_mode[2]; int32_t op3_ph_inc_ratio[2]; int32_t op3_ph_detune[2]; int32_t op3_osc_waveform[2]; int32_t op3_osc_spec_range_a[2]; int32_t op3_osc_spec_range_b[2]; int32_t op3_osc_spec_a1[2]; int32_t op3_osc_spec_a2[2]; int32_t op3_osc_spec_a3[2]; int32_t op3_osc_spec_a4[2]; int32_t op3_osc_spec_a5[2]; int32_t op3_osc_spec_a6[2]; int32_t op3_osc_spec_a7[2]; int32_t op3_osc_spec_a8[2]; int32_t op3_osc_spec_a9[2]; int32_t op3_osc_spec_a10[2]; int32_t op3_osc_spec_a11[2]; int32_t op3_osc_spec_a12[2]; int32_t op3_osc_spec_a13[2]; int32_t op3_osc_spec_a14[2]; int32_t op3_osc_spec_a15[2]; int32_t op3_osc_spec_a16[2]; int32_t op3_osc_spec_a17[2]; int32_t op3_osc_spec_a18[2]; int32_t op3_osc_spec_a19[2]; int32_t op3_osc_spec_a20[2]; int32_t op3_osc_spec_a21[2]; int32_t op3_osc_spec_a22[2]; int32_t op3_osc_spec_a23[2]; int32_t op3_osc_spec_a24[2]; int32_t op3_osc_spec_a25[2]; int32_t op3_osc_spec_a26[2]; int32_t op3_osc_spec_a27[2]; int32_t op3_osc_spec_a28[2]; int32_t op3_osc_spec_a29[2]; int32_t op3_osc_spec_a30[2]; int32_t op3_osc_spec_a31[2]; int32_t op3_osc_spec_a32[2]; int32_t op3_osc_spec_a33[2]; int32_t op3_osc_spec_a34[2]; int32_t op3_osc_spec_a35[2]; int32_t op3_osc_spec_a36[2]; int32_t op3_osc_spec_a37[2]; int32_t op3_osc_spec_a38[2]; int32_t op3_osc_spec_a39[2]; int32_t op3_osc_spec_a40[2]; int32_t op3_osc_spec_a41[2]; int32_t op3_osc_spec_a42[2]; int32_t op3_osc_spec_a43[2]; int32_t op3_osc_spec_a44[2]; int32_t op3_osc_spec_a45[2]; int32_t op3_osc_spec_a46[2]; int32_t op3_osc_spec_a47[2]; int32_t op3_osc_spec_a48[2]; int32_t op3_osc_spec_a49[2]; int32_t op3_osc_spec_a50[2]; int32_t op3_osc_spec_a51[2]; int32_t op3_osc_spec_a52[2]; int32_t op3_osc_spec_a53[2]; int32_t op3_osc_spec_a54[2]; int32_t op3_osc_spec_a55[2]; int32_t op3_osc_spec_a56[2]; int32_t op3_osc_spec_a57[2]; int32_t op3_osc_spec_a58[2]; int32_t op3_osc_spec_a59[2]; int32_t op3_osc_spec_a60[2]; int32_t op3_osc_spec_a61[2]; int32_t op3_osc_spec_a62[2]; int32_t op3_osc_spec_a63[2]; int32_t op3_osc_spec_a64[2]; int32_t op3_osc_spec_b1[2]; int32_t op3_osc_spec_b2[2]; int32_t op3_osc_spec_b3[2]; int32_t op3_osc_spec_b4[2]; int32_t op3_osc_spec_b5[2]; int32_t op3_osc_spec_b6[2]; int32_t op3_osc_spec_b7[2]; int32_t op3_osc_spec_b8[2]; int32_t op3_osc_spec_b9[2]; int32_t op3_osc_spec_b10[2]; int32_t op3_osc_spec_b11[2]; int32_t op3_osc_spec_b12[2]; int32_t op3_osc_spec_b13[2]; int32_t op3_osc_spec_b14[2]; int32_t op3_osc_spec_b15[2]; int32_t op3_osc_spec_b16[2]; int32_t op3_osc_spec_b17[2]; int32_t op3_osc_spec_b18[2]; int32_t op3_osc_spec_b19[2]; int32_t op3_osc_spec_b20[2]; int32_t op3_osc_spec_b21[2]; int32_t op3_osc_spec_b22[2]; int32_t op3_osc_spec_b23[2]; int32_t op3_osc_spec_b24[2]; int32_t op3_osc_spec_b25[2]; int32_t op3_osc_spec_b26[2]; int32_t op3_osc_spec_b27[2]; int32_t op3_osc_spec_b28[2]; int32_t op3_osc_spec_b29[2]; int32_t op3_osc_spec_b30[2]; int32_t op3_osc_spec_b31[2]; int32_t op3_osc_spec_b32[2]; int32_t op3_osc_spec_b33[2]; int32_t op3_osc_spec_b34[2]; int32_t op3_osc_spec_b35[2]; int32_t op3_osc_spec_b36[2]; int32_t op3_osc_spec_b37[2]; int32_t op3_osc_spec_b38[2]; int32_t op3_osc_spec_b39[2]; int32_t op3_osc_spec_b40[2]; int32_t op3_osc_spec_b41[2]; int32_t op3_osc_spec_b42[2]; int32_t op3_osc_spec_b43[2]; int32_t op3_osc_spec_b44[2]; int32_t op3_osc_spec_b45[2]; int32_t op3_osc_spec_b46[2]; int32_t op3_osc_spec_b47[2]; int32_t op3_osc_spec_b48[2]; int32_t op3_osc_spec_b49[2]; int32_t op3_osc_spec_b50[2]; int32_t op3_osc_spec_b51[2]; int32_t op3_osc_spec_b52[2]; int32_t op3_osc_spec_b53[2]; int32_t op3_osc_spec_b54[2]; int32_t op3_osc_spec_b55[2]; int32_t op3_osc_spec_b56[2]; int32_t op3_osc_spec_b57[2]; int32_t op3_osc_spec_b58[2]; int32_t op3_osc_spec_b59[2]; int32_t op3_osc_spec_b60[2]; int32_t op3_osc_spec_b61[2]; int32_t op3_osc_spec_b62[2]; int32_t op3_osc_spec_b63[2]; int32_t op3_osc_spec_b64[2]; int32_t op3_morphbase_value[2]; int32_t op3_eg_warm[2]; int32_t op3_eg_surge[2]; int32_t op3_eg_d1r[2]; int32_t op3_eg_d1l[2]; int32_t op3_eg_d2r[2]; int32_t op3_eg_d2l[2]; int32_t op3_eg_rel[2]; int32_t op3_eg_mode[2]; int32_t op3_lfo_rate[2]; int32_t op3_lfo_depth[2]; int32_t op3_lfo_wave[2]; int32_t op3_lfo_mode[2]; int32_t op3_lfo_rate_fade[2]; int32_t op3_lfo_depth_fade[2]; int32_t op3_fbenv_on[2]; int32_t op3_fbenv_r1[2]; int32_t op3_fbenv_r2[2]; int32_t op3_fbenv_r3[2]; int32_t op3_fbenv_l1[2]; int32_t op3_fbenv_l2[2]; int32_t op3_fbenv_l3[2]; int32_t op3_fbenv_relr[2]; int32_t op3_fbenv_rell[2]; int32_t op3_fbenv_mode[2]; int32_t op3_fblfo_rate[2]; int32_t op3_fblfo_depth[2]; int32_t op3_fblfo_wave[2]; int32_t op3_fblfo_mode[2]; int32_t op3_fblfo_rate_fade[2]; int32_t op3_fblfo_depth_fade[2]; int32_t op3_dlfo_rate[2]; int32_t op3_dlfo_depth[2]; int32_t op3_dlfo_wave[2]; int32_t op3_dlfo_mode[2]; int32_t op3_dlfo_rate_fade[2]; int32_t op3_dlfo_depth_fade[2]; int32_t op3_velamp_depth[2]; int32_t op3_velfm_depth[2]; int32_t op3_velrate_depth[2]; int32_t op3_tl_level[2]; int32_t op3_fb_level[2]; int32_t op3_level_ks_lo[2]; int32_t op3_level_ks_hi[2]; int32_t op3_fb_ks_lo[2]; int32_t op3_fb_ks_hi[2]; int32_t op3_eg_ks_lo[2]; int32_t op3_eg_ks_hi[2]; int32_t op3_level_vs_amount[2]; int32_t op3_fb_vs_amount[2]; int32_t op3_eg_vs_amount[2]; int32_t op3_mute_mute[2]; int32_t op3_specenv_on[2]; int32_t op3_specenv_r1[2]; int32_t op3_specenv_r2[2]; int32_t op3_specenv_r3[2]; int32_t op3_specenv_l1[2]; int32_t op3_specenv_l2[2]; int32_t op3_specenv_l3[2]; int32_t op3_specenv_relr[2]; int32_t op3_specenv_rell[2]; int32_t op3_specenv_mode[2]; int32_t op3_speclfo_rate[2]; int32_t op3_speclfo_depth[2]; int32_t op3_speclfo_wave[2]; int32_t op3_speclfo_mode[2]; int32_t op3_speclfo_rate_fade[2]; int32_t op3_speclfo_depth_fade[2]; int32_t op3_phase_on[2]; int32_t op3_phase_bp[2]; int32_t op3_phase_segments[2]; int32_t op3_phase_flip[2]; int32_t op3_phase_flip_rate[2]; int32_t op3_phase_reorder[2]; int32_t op3_phase_reorder_rate[2]; int32_t op3_phase_jump[2]; int32_t op3_phase_drift[2]; int32_t op3_phase_drift_rate[2]; int32_t op3_phase_ks_lo[2]; int32_t op3_phase_ks_hi[2]; int32_t op3_phase_vs[2]; int32_t op3_phase_lfo_rate[2]; int32_t op3_phase_lfo_depth[2]; int32_t op3_phase_lfo_wave[2]; int32_t op3_phase_lfo_mode[2]; int32_t op3_phase_lfo_rate_fade[2]; int32_t op3_phase_lfo_depth_fade[2]; int32_t op3_phase_env_on[2]; int32_t op3_phase_env_r1[2]; int32_t op3_phase_env_r2[2]; int32_t op3_phase_env_r3[2]; int32_t op3_phase_env_l1[2]; int32_t op3_phase_env_l2[2]; int32_t op3_phase_env_l3[2]; int32_t op3_phase_env_relr[2]; int32_t op3_phase_env_rell[2]; int32_t op3_phase_env_mode[2]; int32_t op4_ph_inc_ratio[2]; int32_t op4_ph_detune[2]; int32_t op4_osc_waveform[2]; int32_t op4_osc_spec_range_a[2]; int32_t op4_osc_spec_range_b[2]; int32_t op4_osc_spec_a1[2]; int32_t op4_osc_spec_a2[2]; int32_t op4_osc_spec_a3[2]; int32_t op4_osc_spec_a4[2]; int32_t op4_osc_spec_a5[2]; int32_t op4_osc_spec_a6[2]; int32_t op4_osc_spec_a7[2]; int32_t op4_osc_spec_a8[2]; int32_t op4_osc_spec_a9[2]; int32_t op4_osc_spec_a10[2]; int32_t op4_osc_spec_a11[2]; int32_t op4_osc_spec_a12[2]; int32_t op4_osc_spec_a13[2]; int32_t op4_osc_spec_a14[2]; int32_t op4_osc_spec_a15[2]; int32_t op4_osc_spec_a16[2]; int32_t op4_osc_spec_a17[2]; int32_t op4_osc_spec_a18[2]; int32_t op4_osc_spec_a19[2]; int32_t op4_osc_spec_a20[2]; int32_t op4_osc_spec_a21[2]; int32_t op4_osc_spec_a22[2]; int32_t op4_osc_spec_a23[2]; int32_t op4_osc_spec_a24[2]; int32_t op4_osc_spec_a25[2]; int32_t op4_osc_spec_a26[2]; int32_t op4_osc_spec_a27[2]; int32_t op4_osc_spec_a28[2]; int32_t op4_osc_spec_a29[2]; int32_t op4_osc_spec_a30[2]; int32_t op4_osc_spec_a31[2]; int32_t op4_osc_spec_a32[2]; int32_t op4_osc_spec_a33[2]; int32_t op4_osc_spec_a34[2]; int32_t op4_osc_spec_a35[2]; int32_t op4_osc_spec_a36[2]; int32_t op4_osc_spec_a37[2]; int32_t op4_osc_spec_a38[2]; int32_t op4_osc_spec_a39[2]; int32_t op4_osc_spec_a40[2]; int32_t op4_osc_spec_a41[2]; int32_t op4_osc_spec_a42[2]; int32_t op4_osc_spec_a43[2]; int32_t op4_osc_spec_a44[2]; int32_t op4_osc_spec_a45[2]; int32_t op4_osc_spec_a46[2]; int32_t op4_osc_spec_a47[2]; int32_t op4_osc_spec_a48[2]; int32_t op4_osc_spec_a49[2]; int32_t op4_osc_spec_a50[2]; int32_t op4_osc_spec_a51[2]; int32_t op4_osc_spec_a52[2]; int32_t op4_osc_spec_a53[2]; int32_t op4_osc_spec_a54[2]; int32_t op4_osc_spec_a55[2]; int32_t op4_osc_spec_a56[2]; int32_t op4_osc_spec_a57[2]; int32_t op4_osc_spec_a58[2]; int32_t op4_osc_spec_a59[2]; int32_t op4_osc_spec_a60[2]; int32_t op4_osc_spec_a61[2]; int32_t op4_osc_spec_a62[2]; int32_t op4_osc_spec_a63[2]; int32_t op4_osc_spec_a64[2]; int32_t op4_osc_spec_b1[2]; int32_t op4_osc_spec_b2[2]; int32_t op4_osc_spec_b3[2]; int32_t op4_osc_spec_b4[2]; int32_t op4_osc_spec_b5[2]; int32_t op4_osc_spec_b6[2]; int32_t op4_osc_spec_b7[2]; int32_t op4_osc_spec_b8[2]; int32_t op4_osc_spec_b9[2]; int32_t op4_osc_spec_b10[2]; int32_t op4_osc_spec_b11[2]; int32_t op4_osc_spec_b12[2]; int32_t op4_osc_spec_b13[2]; int32_t op4_osc_spec_b14[2]; int32_t op4_osc_spec_b15[2]; int32_t op4_osc_spec_b16[2]; int32_t op4_osc_spec_b17[2]; int32_t op4_osc_spec_b18[2]; int32_t op4_osc_spec_b19[2]; int32_t op4_osc_spec_b20[2]; int32_t op4_osc_spec_b21[2]; int32_t op4_osc_spec_b22[2]; int32_t op4_osc_spec_b23[2]; int32_t op4_osc_spec_b24[2]; int32_t op4_osc_spec_b25[2]; int32_t op4_osc_spec_b26[2]; int32_t op4_osc_spec_b27[2]; int32_t op4_osc_spec_b28[2]; int32_t op4_osc_spec_b29[2]; int32_t op4_osc_spec_b30[2]; int32_t op4_osc_spec_b31[2]; int32_t op4_osc_spec_b32[2]; int32_t op4_osc_spec_b33[2]; int32_t op4_osc_spec_b34[2]; int32_t op4_osc_spec_b35[2]; int32_t op4_osc_spec_b36[2]; int32_t op4_osc_spec_b37[2]; int32_t op4_osc_spec_b38[2]; int32_t op4_osc_spec_b39[2]; int32_t op4_osc_spec_b40[2]; int32_t op4_osc_spec_b41[2]; int32_t op4_osc_spec_b42[2]; int32_t op4_osc_spec_b43[2]; int32_t op4_osc_spec_b44[2]; int32_t op4_osc_spec_b45[2]; int32_t op4_osc_spec_b46[2]; int32_t op4_osc_spec_b47[2]; int32_t op4_osc_spec_b48[2]; int32_t op4_osc_spec_b49[2]; int32_t op4_osc_spec_b50[2]; int32_t op4_osc_spec_b51[2]; int32_t op4_osc_spec_b52[2]; int32_t op4_osc_spec_b53[2]; int32_t op4_osc_spec_b54[2]; int32_t op4_osc_spec_b55[2]; int32_t op4_osc_spec_b56[2]; int32_t op4_osc_spec_b57[2]; int32_t op4_osc_spec_b58[2]; int32_t op4_osc_spec_b59[2]; int32_t op4_osc_spec_b60[2]; int32_t op4_osc_spec_b61[2]; int32_t op4_osc_spec_b62[2]; int32_t op4_osc_spec_b63[2]; int32_t op4_osc_spec_b64[2]; int32_t op4_morphbase_value[2]; int32_t op4_eg_warm[2]; int32_t op4_eg_surge[2]; int32_t op4_eg_d1r[2]; int32_t op4_eg_d1l[2]; int32_t op4_eg_d2r[2]; int32_t op4_eg_d2l[2]; int32_t op4_eg_rel[2]; int32_t op4_eg_mode[2]; int32_t op4_lfo_rate[2]; int32_t op4_lfo_depth[2]; int32_t op4_lfo_wave[2]; int32_t op4_lfo_mode[2]; int32_t op4_lfo_rate_fade[2]; int32_t op4_lfo_depth_fade[2]; int32_t op4_fbenv_on[2]; int32_t op4_fbenv_r1[2]; int32_t op4_fbenv_r2[2]; int32_t op4_fbenv_r3[2]; int32_t op4_fbenv_l1[2]; int32_t op4_fbenv_l2[2]; int32_t op4_fbenv_l3[2]; int32_t op4_fbenv_relr[2]; int32_t op4_fbenv_rell[2]; int32_t op4_fbenv_mode[2]; int32_t op4_fblfo_rate[2]; int32_t op4_fblfo_depth[2]; int32_t op4_fblfo_wave[2]; int32_t op4_fblfo_mode[2]; int32_t op4_fblfo_rate_fade[2]; int32_t op4_fblfo_depth_fade[2]; int32_t op4_dlfo_rate[2]; int32_t op4_dlfo_depth[2]; int32_t op4_dlfo_wave[2]; int32_t op4_dlfo_mode[2]; int32_t op4_dlfo_rate_fade[2]; int32_t op4_dlfo_depth_fade[2]; int32_t op4_velamp_depth[2]; int32_t op4_velfm_depth[2]; int32_t op4_velrate_depth[2]; int32_t op4_tl_level[2]; int32_t op4_fb_level[2]; int32_t op4_level_ks_lo[2]; int32_t op4_level_ks_hi[2]; int32_t op4_fb_ks_lo[2]; int32_t op4_fb_ks_hi[2]; int32_t op4_eg_ks_lo[2]; int32_t op4_eg_ks_hi[2]; int32_t op4_level_vs_amount[2]; int32_t op4_fb_vs_amount[2]; int32_t op4_eg_vs_amount[2]; int32_t op4_mute_mute[2]; int32_t op4_specenv_on[2]; int32_t op4_specenv_r1[2]; int32_t op4_specenv_r2[2]; int32_t op4_specenv_r3[2]; int32_t op4_specenv_l1[2]; int32_t op4_specenv_l2[2]; int32_t op4_specenv_l3[2]; int32_t op4_specenv_relr[2]; int32_t op4_specenv_rell[2]; int32_t op4_specenv_mode[2]; int32_t op4_speclfo_rate[2]; int32_t op4_speclfo_depth[2]; int32_t op4_speclfo_wave[2]; int32_t op4_speclfo_mode[2]; int32_t op4_speclfo_rate_fade[2]; int32_t op4_speclfo_depth_fade[2]; int32_t op4_phase_on[2]; int32_t op4_phase_bp[2]; int32_t op4_phase_segments[2]; int32_t op4_phase_flip[2]; int32_t op4_phase_flip_rate[2]; int32_t op4_phase_reorder[2]; int32_t op4_phase_reorder_rate[2]; int32_t op4_phase_jump[2]; int32_t op4_phase_drift[2]; int32_t op4_phase_drift_rate[2]; int32_t op4_phase_ks_lo[2]; int32_t op4_phase_ks_hi[2]; int32_t op4_phase_vs[2]; int32_t op4_phase_lfo_rate[2]; int32_t op4_phase_lfo_depth[2]; int32_t op4_phase_lfo_wave[2]; int32_t op4_phase_lfo_mode[2]; int32_t op4_phase_lfo_rate_fade[2]; int32_t op4_phase_lfo_depth_fade[2]; int32_t op4_phase_env_on[2]; int32_t op4_phase_env_r1[2]; int32_t op4_phase_env_r2[2]; int32_t op4_phase_env_r3[2]; int32_t op4_phase_env_l1[2]; int32_t op4_phase_env_l2[2]; int32_t op4_phase_env_l3[2]; int32_t op4_phase_env_relr[2]; int32_t op4_phase_env_rell[2]; int32_t op4_phase_env_mode[2]; int32_t algo_algorithm[2]; int32_t route_ring1_on[2]; int32_t route_ring1_level[2]; int32_t route_ring2_on[2]; int32_t route_ring2_level[2]; int32_t route_ring3_on[2]; int32_t route_ring3_level[2]; int32_t route_car1_out[2]; int32_t route_car2_out[2]; int32_t route_car3_out[2]; int32_t route_car4_out[2]; int32_t penv_on[2]; int32_t penv_r1[2]; int32_t penv_r2[2]; int32_t penv_r3[2]; int32_t penv_l1[2]; int32_t penv_l2[2]; int32_t penv_l3[2]; int32_t penv_relr[2]; int32_t penv_rell[2]; int32_t penv_mode[2]; int32_t penv_ks_lo[2]; int32_t penv_ks_hi[2]; int32_t penv_vs_amount[2]; int32_t plfo_rate[2]; int32_t plfo_depth[2]; int32_t plfo_wave[2]; int32_t plfo_mode[2]; int32_t plfo_rate_fade[2]; int32_t plfo_depth_fade[2]; int32_t f1_ks_lo[2]; int32_t f1_ks_hi[2]; int32_t fenv_ks_lo[2]; int32_t fenv_ks_hi[2]; int32_t f1_vs_amount[2]; int32_t fenv_vs_amount[2]; int32_t cutoff_pos_cutoff[2]; int32_t lp_res[2]; int32_t lp_comp[2]; int32_t lp_mode[2]; int32_t lp_on[2]; int32_t fenv_on[2]; int32_t fenv_r1[2]; int32_t fenv_r2[2]; int32_t fenv_r3[2]; int32_t fenv_l1[2]; int32_t fenv_l2[2]; int32_t fenv_l3[2]; int32_t fenv_relr[2]; int32_t fenv_rell[2]; int32_t fenv_mode[2]; int32_t key_bp_bp[2]; int32_t key_on_on[2]; int32_t vel_on_on[2]; int32_t dc_r; int32_t macro_g1; int32_t macro_g2; int32_t macro_g3; int32_t macro_g4; int32_t macro_g5; int32_t macro_g6; int32_t macro_s1; int32_t macro_s2; int32_t macro_s3; int32_t macro_s4; int32_t macro_s5; int32_t macro_s6; int32_t macro_sr1; int32_t macro_sr2; int32_t macro_sr3; int32_t macro_sr4; int32_t macro_sr5; int32_t macro_sr6; int32_t macro_t1a; int32_t macro_t1b; int32_t macro_t1c; int32_t macro_t2a; int32_t macro_t2b; int32_t macro_t2c; int32_t macro_t3a; int32_t macro_t3b; int32_t macro_t3c; int32_t macro_t4a; int32_t macro_t4b; int32_t macro_t4c; int32_t macro_t5a; int32_t macro_t5b; int32_t macro_t5c; int32_t macro_t6a; int32_t macro_t6b; int32_t macro_t6c; int32_t macro_r1a; int32_t macro_r1b; int32_t macro_r1c; int32_t macro_r2a; int32_t macro_r2b; int32_t macro_r2c; int32_t macro_r3a; int32_t macro_r3b; int32_t macro_r3c; int32_t macro_r4a; int32_t macro_r4b; int32_t macro_r4c; int32_t macro_r5a; int32_t macro_r5b; int32_t macro_r5c; int32_t macro_r6a; int32_t macro_r6b; int32_t macro_r6c; uint32_t ctrl_serial; uint32_t patch_gate; uint32_t patch_trig; int32_t dcr_x1; int32_t dcr_y1; int32_t vel_on_on_x[NVOICE]; int32_t penv_vs_k_x[NVOICE]; int32_t op4_level_vs_k_x[NVOICE]; int32_t op4_fb_vs_k_x[NVOICE]; int32_t op4_eg_vs_k_x[NVOICE]; int32_t op3_level_vs_k_x[NVOICE]; int32_t op3_fb_vs_k_x[NVOICE]; int32_t op3_eg_vs_k_x[NVOICE]; int32_t op2_level_vs_k_x[NVOICE]; int32_t op2_fb_vs_k_x[NVOICE]; int32_t op2_eg_vs_k_x[NVOICE]; int32_t op1_level_vs_k_x[NVOICE]; int32_t op1_fb_vs_k_x[NVOICE]; int32_t op1_eg_vs_k_x[NVOICE]; int32_t fenv_vs_k_x[NVOICE]; int32_t f1_vs_k_x[NVOICE]; int32_t key_on_on_x[NVOICE]; int32_t key_bp_bp_x[NVOICE]; int32_t penv_ks_k_x[NVOICE]; int32_t penv_cv_x[NVOICE]; int32_t penv_active_x[NVOICE]; int32_t op4_level_ks_k_x[NVOICE]; int32_t op4_fb_ks_k_x[NVOICE]; int32_t op4_eg_ks_k_x[NVOICE]; int32_t op3_level_ks_k_x[NVOICE]; int32_t op3_fb_ks_k_x[NVOICE]; int32_t op3_eg_ks_k_x[NVOICE]; int32_t op2_level_ks_k_x[NVOICE]; int32_t op2_fb_ks_k_x[NVOICE]; int32_t op2_eg_ks_k_x[NVOICE]; int32_t op1_level_ks_k_x[NVOICE]; int32_t op1_fb_ks_k_x[NVOICE]; int32_t op1_eg_ks_k_x[NVOICE]; int32_t fenv_ks_k_x[NVOICE]; int32_t f1_ks_k_x[NVOICE]; int32_t cutoff_pos_f_x[NVOICE]; int32_t cutoff_pos_pos_x[NVOICE]; int32_t fenv_cv_x[NVOICE]; int32_t fenv_active_x[NVOICE]; int32_t plfo_cv_x[NVOICE]; int32_t psum_cv_x[NVOICE]; int32_t algo_alg_x[NVOICE]; int32_t op4_phase_env_cv_x[NVOICE]; int32_t op4_phase_lfo_cv_x[NVOICE]; int32_t op4_phase_mod_cv_x[NVOICE]; int32_t op4_speclfo_cv_x[NVOICE]; int32_t op4_velrate_cv_x[NVOICE]; int32_t op4_eg_cv_x[NVOICE]; int32_t op4_velfm_cv_x[NVOICE]; int32_t op4_velamp_cv_x[NVOICE]; int32_t op4_ampvel_cv_x[NVOICE]; int32_t op4_dlfo_cv_x[NVOICE]; int32_t op4_dsum_cv_x[NVOICE]; int32_t op4_fblfo_cv_x[NVOICE]; int32_t op4_fbenv_cv_x[NVOICE]; int32_t op4_fbenv_active_x[NVOICE]; int32_t op4_lfo_cv_x[NVOICE]; int32_t op4_morphbase_out_x[NVOICE]; int32_t op4_specenv_cv_x[NVOICE]; int32_t op4_specenv_active_x[NVOICE]; int32_t op3_phase_env_cv_x[NVOICE]; int32_t op3_phase_lfo_cv_x[NVOICE]; int32_t op3_phase_mod_cv_x[NVOICE]; int32_t op3_speclfo_cv_x[NVOICE]; int32_t op3_velrate_cv_x[NVOICE]; int32_t op3_eg_cv_x[NVOICE]; int32_t op3_velfm_cv_x[NVOICE]; int32_t op3_velamp_cv_x[NVOICE]; int32_t op3_ampvel_cv_x[NVOICE]; int32_t op3_dlfo_cv_x[NVOICE]; int32_t op3_dsum_cv_x[NVOICE]; int32_t op3_fblfo_cv_x[NVOICE]; int32_t op3_fbenv_cv_x[NVOICE]; int32_t op3_fbenv_active_x[NVOICE]; int32_t op3_lfo_cv_x[NVOICE]; int32_t op3_morphbase_out_x[NVOICE]; int32_t op3_specenv_cv_x[NVOICE]; int32_t op3_specenv_active_x[NVOICE]; int32_t op2_phase_env_cv_x[NVOICE]; int32_t op2_phase_lfo_cv_x[NVOICE]; int32_t op2_phase_mod_cv_x[NVOICE]; int32_t op2_speclfo_cv_x[NVOICE]; int32_t op2_velrate_cv_x[NVOICE]; int32_t op2_eg_cv_x[NVOICE]; int32_t op2_velfm_cv_x[NVOICE]; int32_t op2_velamp_cv_x[NVOICE]; int32_t op2_ampvel_cv_x[NVOICE]; int32_t op2_dlfo_cv_x[NVOICE]; int32_t op2_dsum_cv_x[NVOICE]; int32_t op2_fblfo_cv_x[NVOICE]; int32_t op2_fbenv_cv_x[NVOICE]; int32_t op2_fbenv_active_x[NVOICE]; int32_t op2_lfo_cv_x[NVOICE]; int32_t op2_morphbase_out_x[NVOICE]; int32_t op2_specenv_cv_x[NVOICE]; int32_t op2_specenv_active_x[NVOICE]; int32_t op1_phase_env_cv_x[NVOICE]; int32_t op1_phase_lfo_cv_x[NVOICE]; int32_t op1_phase_mod_cv_x[NVOICE]; int32_t op1_speclfo_cv_x[NVOICE]; int32_t op1_velrate_cv_x[NVOICE]; int32_t op1_eg_cv_x[NVOICE]; int32_t op1_velfm_cv_x[NVOICE]; int32_t op1_velamp_cv_x[NVOICE]; int32_t op1_ampvel_cv_x[NVOICE]; int32_t op1_dlfo_cv_x[NVOICE]; int32_t op1_dsum_cv_x[NVOICE]; int32_t op1_fblfo_cv_x[NVOICE]; int32_t op1_fbenv_cv_x[NVOICE]; int32_t op1_fbenv_active_x[NVOICE]; int32_t op1_lfo_cv_x[NVOICE]; int32_t op1_morphbase_out_x[NVOICE]; int32_t op1_specenv_cv_x[NVOICE]; int32_t op1_specenv_active_x[NVOICE]; int32_t type_p_alpha_render_bus0[CONTROL_BLOCK]; int32_t type_p_alpha_render_bus1[CONTROL_BLOCK]; int32_t type_p_alpha_tap0[N_TAP_CH][DSP_TAP_FRAMES]; int32_t type_p_alpha_tap1[N_TAP_CH][DSP_TAP_FRAMES]; int16_t op1_osc_bank[10880]; int16_t op1_phase_map[18432]; int16_t op1_phase_lfo_pool[10880]; int16_t op1_phase_env_pool[10880]; int16_t op2_osc_bank[10880]; int16_t op2_phase_map[18432]; int16_t op2_phase_lfo_pool[10880]; int16_t op2_phase_env_pool[10880]; int16_t op3_osc_bank[10880]; int16_t op3_phase_map[18432]; int16_t op3_phase_lfo_pool[10880]; int16_t op3_phase_env_pool[10880]; int16_t op4_osc_bank[10880]; int16_t op4_phase_map[18432]; int16_t op4_phase_lfo_pool[10880]; int16_t op4_phase_env_pool[10880]; uint16_t voice_rel[NVOICE]; uint8_t voice_gate[NVOICE]; uint8_t voice_note[NVOICE]; uint8_t voice_trig[NVOICE]; uint8_t voice_part[NVOICE]; uint8_t mono_held[N_PART][128]; uint8_t part_mono_voice[N_PART][8]; uint8_t part_mono_count[N_PART]; uint8_t pan_env_stage[N_PART]; uint8_t pan_env_trig[N_PART]; }`

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

- `struct { void (*note_on)(uint32_t note, uint32_t velocity, uint32_t channel); void (*note_off)(uint32_t note, uint32_t channel); void (*all_off)(void); void (*all_off_ch)(uint32_t channel); int (*set_param)(uint32_t id, int32_t value); void (*set_bend)(uint32_t bend14, uint32_t channel); const uint16_t *cc_map; /* cc_to_param[128], SYNTH_CC_NONE = unassigned */ const int32_t *param_min; /* per-param ranges, indexed by param id */ const int32_t *param_max; uint8_t sustain_on[16]; /* per-channel pedal state */ uint8_t sustain_pend[16][128]; /* per-channel deferred note-offs */ }`

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

- `double ratio() const { return ratio_; }`

### `ChunkNoteSet`

`class` at `plugin/juce/ChunkNoteSet.h:12` through line 35.

Tracks the notes that started in the current control chunk, by MIDI channel and pitch, so a release in the same chunk waits for the chunk to render. Two channels can hold the same pitch without deferring each other's releases.

Fields and enumerators:

- `static constexpr int kChannels = 16;`
- `static constexpr int kNotes = 128;`
- `std::array<uint8_t, kChannels * kNotes> held_{};`

Methods:

- `void reset() { held_.fill(0); }`
- `void noteOn(int channel, int note) { if (channel < 1 \|\| channel > kChannels \|\| note < 0 \|\| note >= kNotes) return; held_[static_cast<std::size_t>((channel - 1) * kNotes + note)] = 1; }`
- `bool held(int channel, int note) const { if (channel < 1 \|\| channel > kChannels \|\| note < 0 \|\| note >= kNotes) return false; return held_[static_cast<std::size_t>((channel - 1) * kNotes + note)] != 0; }`

### `ModeButton`

`class` at `plugin/juce/Pages.cpp:15` through line 48.

Fields and enumerators:

- `bool active_ = false;`

Methods:

- `explicit ModeButton(const juce::String& label) : juce::Button(label) { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void setActive(bool active) { if (active_ != active) { active_ = active; repaint(); } }`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override { g.setColour(colour(tokens::bg)); g.fillRect(getLocalBounds()); g.setColour(colour(active_ ? tokens::green : highlighted ? tokens::pink : tokens::lineStrong)); g.drawRect(getLocalBounds(), 1); g.setFont(uiFont(tokens::textButton)); g.setColour(colour(active_ ? tokens::green : highlighted ? tokens::white : tokens::dim)); g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred, false); }`

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

- `PartCard(PluginEditor& editor, int part, bool active) : editor_(editor), part_(part), active_(active), header_(true, true, [this] { editor_.showRenameDialog(true, part_); }, [this] { editor_.copyPart(part_); }, [this] { editor_.pastePart(part_); }, [this] { return getParam(editor_, params::level(part_)) <= 0.0; }, [this](bool muted) { editor_.togglePartMute(part_, muted); }) { addAndMakeVisible(header_); const std::array<juce::String, 7> labels = { "midi channel", "keyzone start", "keyzone end", "tune", "transpose", "delay", "bend"}; const std::array<juce::String, 7> ids = { params::channel(part_), params::keyLow(part_), params::keyHigh(part_), params::detune(part_), params::transpose(part_), params::delay(part_), params::bendRange(part_)}; for (size_t i = 0; i < knobs_.size(); ++i) { knobs_[i] = std::make_unique<ParamKnob>(editor_.state(), ids[i], labels[i], 38.0f); addAndMakeVisible(*knobs_[i]); } level_ = std::make_unique<P1LevelSlider>(editor_.state(), params::level(part_)); addAndMakeVisible(*level_); pan_ = std::make_unique<P1LevelSlider>(editor_.state(), params::pan(part_), "pan", true); addAndMakeVisible(*pan_); addAndMakeVisible(scope_); addAndMakeVisible(spectro_); }`
- `void resized() override { auto area = getLocalBounds().reduced(10); header_.setBounds(area.removeFromTop(24)); auto knobRow = area.removeFromTop(92); const int cell = knobRow.getWidth() / static_cast<int>(knobs_.size()); for (auto& knob : knobs_) knob->setBounds(knobRow.removeFromLeft(cell).withTrimmedTop(6)); level_->setBounds(area.removeFromTop(38).reduced(4, 0)); area.removeFromTop(2); pan_->setBounds(area.removeFromTop(30).reduced(4, 0)); area.removeFromTop(4); const int visualWidth = (area.getWidth() - 8) / 2; scope_.setBounds(area.removeFromLeft(visualWidth)); area.removeFromLeft(8); spectro_.setBounds(area); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); if (active_) { g.setColour(colour(tokens::green)); g.drawRect(getLocalBounds(), 1); } g.setFont(displayFont(tokens::textPartName)); g.setColour(colour(active_ ? tokens::green : tokens::text)); g.drawText("part " + juce::String(part_ + 1) + ": " + editor_.processor().getPartName(part_), getLocalBounds().reduced(12).removeFromTop(20).withTrimmedRight(300), juce::Justification::centredLeft, true); }`
- `void tick() { editor_.processor().readTapScope(8 + part_, scratch_.data(), static_cast<int>(scratch_.size())); scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); header_.tick(); repaint(); }`

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

- `explicit VoiceMode(PluginEditor& editor) : editor_(editor), part_(editor.activePart()) { for (int i = 0; i < 4; ++i) { modes_[static_cast<size_t>(i)] = std::make_unique<ModeButton>(modeNames_[static_cast<size_t>(i)]); addAndMakeVisible(*modes_[static_cast<size_t>(i)]); } modes_[0]->onClick = [this] { setParam(editor_, params::mode(part_), 0); }; modes_[1]->onClick = [this] { setParam(editor_, params::mode(part_), 1); }; modes_[2]->onClick = [this] { setParam(editor_, params::legato(part_), getParam(editor_, params::legato(part_)) > 0.5 ? 0 : 1); }; modes_[3]->onClick = [this] { setParam(editor_, params::unisonCount(part_), getParam(editor_, params::unisonCount(part_)) > 1.5 ? 1 : 2); }; unison_ = std::make_unique<ParamKnob>( editor_.state(), params::unisonCount(part_), "unison", 42.0f); detune_ = std::make_unique<ParamKnob>( editor_.state(), params::unisonDetune(part_), "detune", 42.0f); porta_ = std::make_unique<ParamKnob>( editor_.state(), params::portamento(part_), "porta", 42.0f); addAndMakeVisible(*unison_); addAndMakeVisible(*detune_); addAndMakeVisible(*porta_); }`
- `void resized() override { auto area = getLocalBounds().reduced(10); area.removeFromTop(20); auto row = area.removeFromTop(28); for (auto& mode : modes_) mode->setBounds(row.removeFromLeft(58).reduced(1, 2)); /* Sit the three knobs next to the mode buttons rather than at the card edge, with a breathing gap so the groups stay separate. */ const int x = 10 + 4 * 58 + 32; unison_->setBounds(x, 34, 74, 76); detune_->setBounds(x + 78, 34, 74, 76); porta_->setBounds(x + 156, 34, 74, 76); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); g.setFont(displayFont(tokens::textSectionTitle)); g.setColour(colour(tokens::pink)); g.drawText("voice mode", 10, 4, 160, 18, juce::Justification::centredLeft, false); }`
- `void tick() { const bool mono = getParam(editor_, params::mode(part_)) > 0.5; modes_[0]->setActive(!mono); modes_[1]->setActive(mono); modes_[2]->setActive(mono && getParam(editor_, params::legato(part_)) > 0.5); modes_[3]->setActive(mono && getParam(editor_, params::unisonCount(part_)) > 1.5); for (int i = 2; i < 4; ++i) { modes_[static_cast<size_t>(i)]->setEnabled(mono); modes_[static_cast<size_t>(i)]->setAlpha(mono ? 1.0f : 0.35f); } unison_->setEnabled(mono); detune_->setEnabled(mono); porta_->setEnabled(mono); unison_->setAlpha(mono ? 1.0f : 0.35f); detune_->setAlpha(mono ? 1.0f : 0.35f); porta_->setAlpha(mono ? 1.0f : 0.35f); }`

### `PatchPage`

`class` at `plugin/juce/Pages.cpp:231` through line 266.

Fields and enumerators:

- `std::array<std::unique_ptr<PartCard>, 2> parts_{};`
- `std::unique_ptr<VoiceMode> voice_;`

Methods:

- `explicit PatchPage(PluginEditor& editor) { for (int i = 0; i < 2; ++i) { parts_[static_cast<size_t>(i)] = std::make_unique<PartCard>( editor, i, editor.activePart() == i); addAndMakeVisible(*parts_[static_cast<size_t>(i)]); } voice_ = std::make_unique<VoiceMode>(editor); addAndMakeVisible(*voice_); }`
- `void resized() override { auto area = getLocalBounds(); auto parts = area.removeFromTop(550); const int width = (parts.getWidth() - 10) / 2; parts_[0]->setBounds(parts.removeFromLeft(width)); parts.removeFromLeft(10); parts_[1]->setBounds(parts); area.removeFromTop(10); voice_->setBounds(area); }`
- `void tick() override { for (auto& part : parts_) part->tick(); voice_->tick(); }`

### `InlineMuteSwitch`

`class` at `plugin/juce/Pages.h:39` through line 78.

Fields and enumerators:

- `std::function<bool()> getMuted_;`
- `std::function<void(bool)> setMuted_;`

Methods:

- `InlineMuteSwitch(std::function<bool()> getMuted, std::function<void(bool)> setMuted) : getMuted_(std::move(getMuted)), setMuted_(std::move(setMuted)) { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void paint(juce::Graphics& g) override { const bool muted = getMuted_ ? getMuted_() : false; const auto track = juce::Rectangle<float>( 0.0f, (static_cast<float>(getHeight()) - 12.0f) * 0.5f, 26.0f, 12.0f); g.setColour(colour(tokens::bg)); g.fillRect(track); g.setColour(colour(muted ? tokens::lineStrong : tokens::pink)); g.drawRect(track, 1.0f); g.setColour(colour(muted ? tokens::lineStrong : tokens::pink)); g.fillRect(juce::Rectangle<float>(muted ? track.getRight() - 11.0f : track.getX() + 1.0f, track.getY() + 1.0f, 10.0f, 8.0f)); g.setFont(uiFont(tokens::textLegend)); g.setColour(colour(tokens::pink)); g.drawText(muted ? "mute" : "active", 32, 0, getWidth() - 32, getHeight(), juce::Justification::centredLeft, true); }`
- `void mouseUp(const juce::MouseEvent&) override { if (setMuted_) setMuted_(!(getMuted_ && getMuted_())); repaint(); }`

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

- `CardHeader(bool partLayout, bool withRename, std::function<void()> onRename, std::function<void()> onCopy, std::function<void()> onPaste, std::function<bool()> getMuted, std::function<void(bool)> setMuted, bool wideHeader = false) : partLayout_(partLayout), withRename_(withRename), wideHeader_(wideHeader) { rename_.onClick = std::move(onRename); copy_.onClick = std::move(onCopy); paste_.onClick = std::move(onPaste); addAndMakeVisible(rename_); addAndMakeVisible(copy_); addAndMakeVisible(paste_); rename_.setVisible(withRename_); mute_ = std::make_unique<InlineMuteSwitch>(std::move(getMuted), std::move(setMuted)); addAndMakeVisible(*mute_); }`
- `void resized() override { auto bar = getLocalBounds(); if (partLayout_) { mute_->setBounds(bar.removeFromRight(108).reduced(0, 3)); auto actions = bar.removeFromRight(172); actions.removeFromRight(16); rename_.setBounds(actions.removeFromLeft(52).reduced(2, 3)); copy_.setBounds(actions.removeFromLeft(52).reduced(2, 3)); paste_.setBounds(actions.removeFromLeft(52).reduced(2, 3)); } else if (wideHeader_) { bar.removeFromLeft(56); copy_.setBounds(bar.removeFromLeft(52).reduced(2, 3)); paste_.setBounds(bar.removeFromLeft(52).reduced(2, 3)); mute_->setBounds(bar.removeFromRight(104).reduced(0, 3)); } else { mute_->setBounds(bar.removeFromRight(104).reduced(0, 3)); bar.removeFromRight(16); paste_.setBounds(bar.removeFromRight(52).reduced(2, 3)); bar.removeFromRight(2); copy_.setBounds(bar.removeFromRight(52).reduced(2, 3)); } }`
- `void tick() { mute_->repaint(); }`

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

- `OscillatorCard(PluginEditor& editor, int op, int part, bool withMorph = false) : P1Card("oscillator", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::oscillator); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::oscillator); }), ratio_(editor.state(), params::opRatio(op, part), "ratio", 28.0f), detune_(editor.state(), params::opDetune(op, part), "detune", 28.0f), level_(editor.state(), params::opLevel(op, part), "level", 28.0f), feedback_(editor.state(), params::opFeedback(op, part), "feedback", 28.0f), wave_(editor.state(), params::opWaveform(op, part), params::waveNames()) { addAndMakeVisible(ratio_); addAndMakeVisible(detune_); addAndMakeVisible(level_); addAndMakeVisible(feedback_); addAndMakeVisible(wave_); if (withMorph) { morph_ = std::make_unique<ParamKnob>(editor.state(), params::opSpecMorph(op, part), "morph", 28.0f); addAndMakeVisible(*morph_); } }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); area.removeFromTop(morph_ != nullptr ? 4 : 8); auto row = area.removeFromTop(morph_ != nullptr ? 68 : 70); const int cells = morph_ != nullptr ? 5 : 4; const int cell = row.getWidth() / cells; ratio_.setBounds(row.removeFromLeft(cell)); detune_.setBounds(row.removeFromLeft(cell)); level_.setBounds(row.removeFromLeft(cell)); feedback_.setBounds(row.removeFromLeft(cell)); if (morph_ != nullptr) morph_->setBounds(row); wave_.setBounds(area.removeFromTop(tokens::choiceHeight) .withWidth(tokens::choiceWidth)); }`

### `AmplitudeEnvelopeCard`

`class` at `plugin/juce/Pages.h:198` through line 270.

Fields and enumerators:

- `P1Envelope envView_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 7> knobs_{};`

Methods:

- `AmplitudeEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("amp env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::amplitudeEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::amplitudeEnvelope); }), envView_(editor.state(), {params::opEnv(op, "warm", part), params::opEnv(op, "surge", part), params::opEnv(op, "d1r", part), params::opEnv(op, "d2r", part), params::opEnv(op, "rel", part)}, {params::opEnv(op, "d1l", part), params::opEnv(op, "d2l", part)}, colour(tokens::purple), 0.0, 99.0, false, EnvelopeShape::operatorAmp, params::opEnv(op, "mode", part)), mode_(editor.state(), params::opEnv(op, "mode", part), {"sustain off", "sustain on", "loop 1", "loop 2"}) { addAndMakeVisible(envView_); addAndMakeVisible(mode_); const std::array<const char*, 7> fields = { "warm", "surge", "d1r", "d2r", "d1l", "d2l", "rel"}; const std::array<const char*, 7> labels = { "atk warmup", "atk surge", "d1r", "d2r", "d1l", "d2l", "rel rate"}; for (std::size_t i = 0; i < fields.size(); ++i) { knobs_[i] = std::make_unique<ParamKnob>( editor.state(), params::opEnv(op, fields[i], part), labels[i], 28.0f); addAndMakeVisible(*knobs_[i]); } }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); area.removeFromTop(4); envView_.setBounds(area.removeFromTop(32)); area.removeFromTop(2); auto rates = area.removeFromTop(64); const int rateCell = rates.getWidth() / 4; for (int i = 0; i < 4; ++i) knobs_[static_cast<std::size_t>(i)]->setBounds( rates.removeFromLeft(rateCell)); area.removeFromTop(2); auto levels = area.removeFromTop(64); const int levelCell = levels.getWidth() / 3; for (int i = 4; i < 7; ++i) knobs_[static_cast<std::size_t>(i)]->setBounds( levels.removeFromLeft(levelCell)); area.removeFromTop(4); auto modeRow = area.removeFromTop(tokens::choiceHeight); mode_.setBounds(modeRow.removeFromLeft(tokens::choiceWidth)); }`
- `void tick() { envView_.repaint(); }`

### `FeedbackEnvelopeCard`

`class` at `plugin/juce/Pages.h:272` through line 357.

Fields and enumerators:

- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `FeedbackEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("fb env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::feedbackEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::feedbackEnvelope); }), envView_(editor.state(), {params::opFeedbackRate(op, 0, part), params::opFeedbackRate(op, 1, part), params::opFeedbackRate(op, 2, part), params::opFeedbackRelRate(op, part)}, {params::opFeedbackLevel(op, 0, part), params::opFeedbackLevel(op, 1, part), params::opFeedbackLevel(op, 2, part), params::opFeedbackRelLevel(op, part)}, colour(tokens::purple), -32768.0, 32767.0, true, EnvelopeShape::bipolar, params::opFeedbackMode(op, part)), on_(editor.state(), params::opFeedbackOn(op, part), "on"), mode_(editor.state(), params::opFeedbackMode(op, part), {"sustain off", "sustain on", "loop 1", "loop 2"}) { addAndMakeVisible(envView_); addAndMakeVisible(on_); addAndMakeVisible(mode_); for (int i = 0; i < 3; ++i) { rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::opFeedbackRate(op, i, part), "r" + juce::String(i + 1), 20.0f); addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]); } rates_[3] = std::make_unique<ParamKnob>( editor.state(), params::opFeedbackRelRate(op, part), "rel rate", 20.0f); addAndMakeVisible(*rates_[3]); for (int i = 0; i < 3; ++i) { levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::opFeedbackLevel(op, i, part), "l" + juce::String(i + 1), 20.0f); addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]); } levels_[3] = std::make_unique<ParamKnob>( editor.state(), params::opFeedbackRelLevel(op, part), "rl", 20.0f); addAndMakeVisible(*levels_[3]); setHeaderTrailingWidth(48); }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); area.removeFromTop(8); envView_.setBounds(area.removeFromTop(32)); area.removeFromTop(2); auto rates = area.removeFromTop(56); const int rateCell = rates.getWidth() / 4; for (auto& knob : rates_) knob->setBounds(rates.removeFromLeft(rateCell)); area.removeFromTop(2); auto levels = area.removeFromTop(56); const int levelCell = levels.getWidth() / 4; for (auto& knob : levels_) knob->setBounds(levels.removeFromLeft(levelCell)); area.removeFromTop(4); auto modeRow = area.removeFromTop(tokens::choiceHeight); mode_.setBounds(modeRow.removeFromLeft(tokens::choiceWidth)); on_.setBounds(headerTrailingBounds()); }`
- `void tick() { envView_.repaint(); }`

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

- `LfoCard(PluginEditor& editor, juce::String title, juce::String rateID, juce::String depthID, juce::String waveID, juce::String modeID, juce::String rateFadeID, juce::String depthFadeID, int op, int part, PluginEditor::OperatorSection section, std::function<void()> onDepthEdit = {}, bool compact = false, juce::String modeLabel = "bipolar") : P1Card(std::move(title), [&ed = editor, op, part, section] { ed.copyOperatorSection(op, part, section); }, [&ed = editor, op, part, section] { ed.pasteOperatorSection(op, part, section); }), rate_(editor.state(), rateID, "rate", 28.0f), depth_(editor.state(), depthID, "depth", 28.0f), rateFade_(editor.state(), rateFadeID, "rate fade", compact ? 22.0f : 28.0f), depthFade_(editor.state(), depthFadeID, "depth fade", compact ? 22.0f : 28.0f), wave_(editor.state(), std::move(waveID), params::lfoWaveNames()), mode_(editor.state(), std::move(modeID), std::move(modeLabel)) { addAndMakeVisible(rate_); addAndMakeVisible(depth_); addAndMakeVisible(rateFade_); addAndMakeVisible(depthFade_); addAndMakeVisible(wave_); addAndMakeVisible(mode_); depth_.onUserEditEnd = std::move(onDepthEdit); }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); area.removeFromTop(4); auto row = area.removeFromTop(compact_ ? 60 : 63); const int cell = row.getWidth() / 4; rate_.setBounds(row.removeFromLeft(cell)); depth_.setBounds(row.removeFromLeft(cell)); rateFade_.setBounds(row.removeFromLeft(cell)); depthFade_.setBounds(row); auto bottom = area.removeFromBottom(tokens::choiceHeight); wave_.setBounds(bottom.removeFromLeft(140)); bottom.removeFromLeft(10); mode_.setBounds(bottom.removeFromLeft(110)); }`

### `FilterCard`

`class` at `plugin/juce/PagesFxMod.cpp:12` through line 61.

Fields and enumerators:

- `ParamKnob cutoff_;`
- `ParamKnob res_;`
- `ParamChoice type_;`
- `P1Toggle on_;`
- `P1Button to_;`

Methods:

- `FilterCard(PluginEditor& editor, juce::String title, juce::String cutoffID, juce::String resID, juce::String typeID, juce::String onID, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card(std::move(title)), cutoff_(editor.state(), std::move(cutoffID), "cutoff", 42.0f), res_(editor.state(), std::move(resID), "res", 42.0f), type_(editor.state(), std::move(typeID), {"lpf", "bpf", "hpf"}), on_(editor.state(), std::move(onID), "on"), to_(std::move(toLabel), true) { addAndMakeVisible(cutoff_); addAndMakeVisible(res_); addAndMakeVisible(type_); addAndMakeVisible(on_); to_.onClick = std::move(onToPart); addAndMakeVisible(to_); setHeaderTrailingWidth(48); }`
- `void resized() override { auto area = contentArea(); auto row = area.removeFromTop(84); const int cell = row.getWidth() / 2; cutoff_.setBounds(row.removeFromLeft(cell)); res_.setBounds(row.removeFromLeft(cell)); type_.setBounds(44, 118, 150, 18); const auto onBounds = headerTrailingBounds(); on_.setBounds(onBounds); to_.setBounds(onBounds.getX() - 16 - 32, onBounds.getY(), 32, 18); }`
- `void paint(juce::Graphics& g) override { P1Card::paint(g); g.setFont(uiFont(tokens::textButton)); g.setColour(colour(tokens::dim)); g.drawText("type", 10, 118, 32, 18, juce::Justification::centredLeft, false); }`

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

- `FilterEnvelopeCard(PluginEditor& editor, int part, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("filter envelope"), part_(part), envView_(editor.state(), {params::filterRate(0, part), params::filterRate(1, part), params::filterRate(2, part), params::filterEnvRelRate(part)}, {params::filterLevel(0, part), params::filterLevel(1, part), params::filterLevel(2, part), params::filterEnvRelLevel(part)}, colour(tokens::green), 0.0, 99.0, false, EnvelopeShape::bipolar, params::filterEnvMode(part), params::filterCutoff(part), 99.0 / 55296.0), on_(editor.state(), params::filterEnvOn(part), "on"), mode_(editor.state(), params::filterEnvMode(part), {"sustain off", "sustain on", "loop 1", "loop 2"}), to_(std::move(toLabel), true) { addAndMakeVisible(envView_); addAndMakeVisible(on_); addAndMakeVisible(mode_); to_.onClick = std::move(onToPart); addAndMakeVisible(to_); for (int i = 0; i < 3; ++i) { rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::filterRate(i, part), "r" + juce::String(i + 1), 42.0f); addAndMakeVisible(*rates_[static_cast<size_t>(i)]); } rates_[3] = std::make_unique<ParamKnob>( editor.state(), params::filterEnvRelRate(part), "rel rate", 42.0f); addAndMakeVisible(*rates_[3]); for (int i = 0; i < 3; ++i) { levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::filterLevel(i, part), "l" + juce::String(i + 1), 42.0f); addAndMakeVisible(*levels_[static_cast<size_t>(i)]); } levels_[3] = std::make_unique<ParamKnob>( editor.state(), params::filterEnvRelLevel(part), "rl", 42.0f); addAndMakeVisible(*levels_[3]); }`
- `void resized() override { auto area = contentArea(); envView_.setBounds(area.removeFromTop(56)); area.removeFromTop(4); auto row = area.removeFromTop(84); const int cell = row.getWidth() / 4; for (auto& knob : rates_) knob->setBounds(row.removeFromLeft(cell)); auto levelRow = area.removeFromTop(84); const int levelCell = levelRow.getWidth() / 4; for (auto& knob : levels_) knob->setBounds(levelRow.removeFromLeft(levelCell)); auto modeRow = area.removeFromTop(84); mode_.setBounds(modeRow.removeFromLeft(levelCell * 2) .withSizeKeepingCentre(levelCell * 2 - 8, tokens::choiceHeight)); on_.setBounds(getWidth() - 56, 1, 48, 18); to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18); }`
- `void tick() { envView_.tick(); }`

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

- `explicit PitchLfoCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch lfo"), part_(editor.activePart()), waveView_(editor.state(), params::pitchWave(part_), colour(tokens::green)), rate_(editor.state(), params::pitchRate(part_), "rate", 36.0f), depth_(editor.state(), params::pitchDepth(part_), "depth", 36.0f), rateFade_(editor.state(), params::pitchRateFade(part_), "rate fade", 36.0f), depthFade_(editor.state(), params::pitchDepthFade(part_), "depth fade", 36.0f), wave_(editor.state(), params::pitchWave(part_), params::lfoWaveNames()), mode_(editor.state(), params::pitchMode(part_), "bipolar"), to_(std::move(toLabel), true) { addAndMakeVisible(waveView_); addAndMakeVisible(rate_); addAndMakeVisible(depth_); addAndMakeVisible(rateFade_); addAndMakeVisible(depthFade_); addAndMakeVisible(wave_); addAndMakeVisible(mode_); to_.onClick = std::move(onToPart); addAndMakeVisible(to_); }`
- `void resized() override { auto area = contentArea(); to_.setBounds(getWidth() - 8 - 32, 1, 32, 18); waveView_.setBounds(area.removeFromTop(88)); area.removeFromTop(10); auto row = area.removeFromTop(84); rate_.setBounds(row.removeFromLeft(row.getWidth() / 2)); depth_.setBounds(row); auto fadeRow = area.removeFromTop(80); rateFade_.setBounds(fadeRow.removeFromLeft(fadeRow.getWidth() / 2)); depthFade_.setBounds(fadeRow); area.removeFromTop(10); auto bottom = area.removeFromTop(tokens::choiceHeight); wave_.setBounds(bottom.removeFromLeft(150)); mode_.setBounds(bottom.removeFromLeft(110)); }`
- `void tick() { waveView_.tick(); }`

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

- `explicit PitchEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch envelope"), part_(editor.activePart()), envView_(editor.state(), {params::pitchEnvRate(0, part_), params::pitchEnvRate(1, part_), params::pitchEnvRate(2, part_), params::pitchEnvRelRate(part_)}, {params::pitchEnvLevel(0, part_), params::pitchEnvLevel(1, part_), params::pitchEnvLevel(2, part_), params::pitchEnvRelLevel(part_)}, colour(tokens::purple), -32768.0, 32767.0, true, EnvelopeShape::bipolar, params::pitchEnvMode(part_)), on_(editor.state(), params::pitchEnvOn(part_), "on"), mode_(editor.state(), params::pitchEnvMode(part_), {"sustain off", "sustain on", "loop 1", "loop 2"}), to_(std::move(toLabel), true) { addAndMakeVisible(envView_); addAndMakeVisible(on_); addAndMakeVisible(mode_); to_.onClick = std::move(onToPart); addAndMakeVisible(to_); for (int i = 0; i < 3; ++i) { rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::pitchEnvRate(i, part_), "r" + juce::String(i + 1), 42.0f); addAndMakeVisible(*rates_[static_cast<size_t>(i)]); } rates_[3] = std::make_unique<ParamKnob>( editor.state(), params::pitchEnvRelRate(part_), "rel rate", 42.0f); addAndMakeVisible(*rates_[3]); for (int i = 0; i < 3; ++i) { levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::pitchEnvLevel(i, part_), "l" + juce::String(i + 1), 42.0f); addAndMakeVisible(*levels_[static_cast<size_t>(i)]); } levels_[3] = std::make_unique<ParamKnob>( editor.state(), params::pitchEnvRelLevel(part_), "rl", 42.0f); addAndMakeVisible(*levels_[3]); }`
- `void resized() override { auto area = contentArea(); envView_.setBounds(area.removeFromTop(56)); area.removeFromTop(4); auto row = area.removeFromTop(84); const int cell = row.getWidth() / 4; for (auto& knob : rates_) knob->setBounds(row.removeFromLeft(cell)); auto levelRow = area.removeFromTop(84); const int levelCell = levelRow.getWidth() / 4; for (auto& knob : levels_) knob->setBounds(levelRow.removeFromLeft(levelCell)); auto modeRow = area.removeFromTop(84); mode_.setBounds(modeRow.removeFromLeft(levelCell * 2) .withSizeKeepingCentre(levelCell * 2 - 8, tokens::choiceHeight)); on_.setBounds(getWidth() - 56, 1, 48, 18); to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18); }`
- `void tick() { envView_.tick(); }`

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

- `explicit PanEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pan envelope"), part_(editor.activePart()), envView_(editor.state(), {params::panEnvRate(0, part_), params::panEnvRate(1, part_), params::panEnvRate(2, part_), params::panEnvRelRate(part_)}, {params::panEnvLevel(0, part_), params::panEnvLevel(1, part_), params::panEnvLevel(2, part_), params::panEnvRelLevel(part_)}, colour(tokens::purple), -99.0, 99.0, true, EnvelopeShape::bipolar, params::panEnvMode(part_), params::pan(part_)), on_(editor.state(), params::panEnvOn(part_), "on"), mode_(editor.state(), params::panEnvMode(part_), {"sustain off", "sustain on", "loop 1", "loop 2"}), to_(std::move(toLabel), true) { addAndMakeVisible(envView_); addAndMakeVisible(on_); addAndMakeVisible(mode_); to_.onClick = std::move(onToPart); addAndMakeVisible(to_); for (int i = 0; i < 4; ++i) { const juce::String id = i < 3 ? params::panEnvRate(i, part_) : params::panEnvRelRate(part_); rates_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), id, i < 3 ? "r" + juce::String(i + 1) : "rel rate", 42.0f); addAndMakeVisible(*rates_[static_cast<size_t>(i)]); } for (int i = 0; i < 4; ++i) { const juce::String id = i < 3 ? params::panEnvLevel(i, part_) : params::panEnvRelLevel(part_); levels_[static_cast<size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), id, i < 3 ? "l" + juce::String(i + 1) : "rl", 42.0f); addAndMakeVisible(*levels_[static_cast<size_t>(i)]); } }`
- `void resized() override { auto area = contentArea(); envView_.setBounds(area.removeFromTop(56)); area.removeFromTop(4); auto row = area.removeFromTop(84); const int cell = row.getWidth() / 4; for (auto& knob : rates_) knob->setBounds(row.removeFromLeft(cell)); auto levelRow = area.removeFromTop(84); const int levelCell = levelRow.getWidth() / 4; for (auto& knob : levels_) knob->setBounds(levelRow.removeFromLeft(levelCell)); auto modeRow = area.removeFromTop(84); mode_.setBounds(modeRow.removeFromLeft(cell * 2) .withSizeKeepingCentre(cell * 2 - 8, tokens::choiceHeight)); on_.setBounds(getWidth() - 56, 1, 48, 18); to_.setBounds(on_.getX() - 16 - 32, 1, 32, 18); }`
- `void tick() { envView_.tick(); }`

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

- `FollowCard(PluginEditor& editor, bool key, juce::String title, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card(std::move(title)), editor_(editor), key_(key), part_(editor.activePart()), targets_(params::followTargets(part_)), on_(editor.state(), params::withPart(key ? "key_sens_on" : "vel_sens_on", part_), "on"), to_(std::move(toLabel), true) { wipe_.setButtonText("wipe all"); target_.addItemList(targetItems(), 1); target_.setSelectedItemIndex(0, juce::dontSendNotification); target_.onChange = [this] { selected_ = target_.getSelectedItemIndex(); updateTarget(); }; addAndMakeVisible(target_); addAndMakeVisible(on_); wipe_.onClick = [this] { wipe(); }; addAndMakeVisible(wipe_); to_.onClick = std::move(onToPart); addAndMakeVisible(to_); if (key_) { breakpoint_ = std::make_unique<ParamKnob>( editor.state(), params::withPart("breakpoint", part_), "breakpoint", 42.0f); addAndMakeVisible(*breakpoint_); } below_ = std::make_unique<ParamKnob>( editor.state(), targets_[0].keyLo, "below", 42.0f); above_ = std::make_unique<ParamKnob>( editor.state(), targets_[0].keyHi, "above", 42.0f); amount_ = std::make_unique<ParamKnob>( editor.state(), targets_[0].velocity, "amount", 42.0f); if (key_) { addAndMakeVisible(*below_); addAndMakeVisible(*above_); amount_->setVisible(false); } else { addAndMakeVisible(*amount_); below_->setVisible(false); above_->setVisible(false); } updateTarget(); }`
- `void resized() override { const int width = getWidth(); on_.setBounds(width - 56, 1, 48, 18); wipe_.setBounds(width - 120, 1, 56, 18); to_.setBounds(wipe_.getX() - 4 - 30, 1, 30, 18); if (key_) { breakpoint_->setBounds(8, 26, 80, 82); target_.setBounds(96, 50, 180, 18); below_->setBounds(8, 112, 80, 82); above_->setBounds(96, 112, 80, 82); } else { target_.setBounds(8, 50, 270, 18); amount_->setBounds(8, 76, 80, 82); } }`
- `void paint(juce::Graphics& g) override { P1Card::paint(g); g.setFont(uiFont(tokens::textButton)); g.setColour(colour(tokens::dim)); if (key_) g.drawText("target", 96, 30, 60, 16, juce::Justification::centredLeft, false); else g.drawText("target", 8, 30, 60, 16, juce::Justification::centredLeft, false); }`
- `void refresh() { const bool enabled = getParam( editor_, params::withPart(key_ ? "key_sens_on" : "vel_sens_on", part_)) > 0.5; const float alpha = enabled ? 1.0f : 0.45f; target_.setAlpha(alpha); if (breakpoint_ != nullptr) breakpoint_->setAlpha(alpha); below_->setAlpha(alpha); above_->setAlpha(alpha); amount_->setAlpha(alpha); repaint(); }`
- `juce::StringArray targetItems() const { juce::StringArray items; for (const auto& target : targets_) items.add(target.name); return items; }`
- `void updateTarget() { const auto& target = targets_[static_cast<size_t>(juce::jlimit( 0, static_cast<int>(targets_.size()) - 1, selected_))]; below_->setParamID(target.keyLo); above_->setParamID(target.keyHi); amount_->setParamID(target.velocity); }`
- `void wipe() { for (const auto& target : targets_) { if (key_) { setParam(editor_, target.keyLo, 0.0); setParam(editor_, target.keyHi, 0.0); } else { setParam(editor_, target.velocity, 0.0); } } }`

### `RingGraph`

`class` at `plugin/juce/PagesFxMod.cpp:505` through line 594.

Fields and enumerators:

- `int a_ = 0;`
- `int b_ = 0;`
- `bool directA_ = false;`
- `bool directB_ = false;`

Methods:

- `void setPair(int a, int b) { a_ = a; b_ = b; repaint(); }`
- `void setDirect(bool a, bool b) { if (directA_ == a && directB_ == b) return; directA_ = a; directB_ = b; repaint(); }`
- `void paint(juce::Graphics& g) override { constexpr int boxW = 24; constexpr int boxH = 16; const int ax = 0, ay = 4; const int bx = 0, by = 34; const int rx = 46, ry = 16, rw = 22, rh = 22; const auto drawBox = [&](int x, int y, int op) { g.setColour(colour(tokens::pink)); g.fillRect(x, y, boxW, boxH); g.setColour(juce::Colours::black); g.setFont(uiFont(9.0f)); g.drawText("op" + juce::String(op + 1), x, y, boxW, boxH, juce::Justification::centred, false); }; const auto arrow = [&](int x1, int y1, int x2, int y2) { g.setColour(colour(tokens::dim)); g.drawLine(static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(x2 - 5), static_cast<float>(y2), 1.2f); juce::Path head; head.addTriangle(static_cast<float>(x2), static_cast<float>(y2), static_cast<float>(x2 - 6), static_cast<float>(y2 - 3), static_cast<float>(x2 - 6), static_cast<float>(y2 + 3)); g.fillPath(head); }; drawBox(ax, ay, a_); drawBox(bx, by, b_); arrow(ax + boxW, ay + boxH / 2, rx, ry + 5); arrow(bx + boxW, by + boxH / 2, rx, ry + rh - 5); g.setColour(colour(tokens::lineStrong)); g.drawRect(rx, ry, rw, rh, 1); g.setColour(colour(tokens::pink)); drawBoldText(g, displayFont(7.0f), "R", juce::Rectangle<float>(static_cast<float>(rx), static_cast<float>(ry), static_cast<float>(rw), static_cast<float>(rh)), 1.0f); const int outX = getWidth() - 2; const int outY = ry + rh / 2; g.setColour(colour(tokens::purple)); g.drawLine(static_cast<float>(rx + rw), static_cast<float>(outY), static_cast<float>(outX - 5), static_cast<float>(outY), 1.4f); juce::Path head; head.addTriangle(static_cast<float>(outX), static_cast<float>(outY), static_cast<float>(outX - 6), static_cast<float>(outY - 3), static_cast<float>(outX - 6), static_cast<float>(outY + 3)); g.fillPath(head); const auto directArrow = [&](int y) { g.setColour(colour(tokens::purple)); g.drawLine(static_cast<float>(boxW + 2), static_cast<float>(y), static_cast<float>(outX - 5), static_cast<float>(y), 1.4f); juce::Path directHead; directHead.addTriangle(static_cast<float>(outX), static_cast<float>(y), static_cast<float>(outX - 6), static_cast<float>(y - 3), static_cast<float>(outX - 6), static_cast<float>(y + 3)); g.fillPath(directHead); }; if (directA_) directArrow(ay + boxH / 2); if (directB_) directArrow(by + boxH / 2); }`

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

- `RingmodCard(PluginEditor& editor, int part) : P1Card("ringmod"), editor_(editor), part_(part) { for (int i = 0; i < 3; ++i) { addAndMakeVisible(graph_[static_cast<std::size_t>(i)]); on_[static_cast<std::size_t>(i)] = std::make_unique<P1Toggle>( editor.state(), params::ringOn(i, part_), "on"); addAndMakeVisible(*on_[static_cast<std::size_t>(i)]); level_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::ringLevel(i, part_), "level", 42.0f); addAndMakeVisible(*level_[static_cast<std::size_t>(i)]); } for (int op = 0; op < 4; ++op) { out_[static_cast<std::size_t>(op)] = std::make_unique<P1Toggle>( editor.state(), params::carrierOut(op, part_), "op" + juce::String(op + 1)); addAndMakeVisible(*out_[static_cast<std::size_t>(op)]); } noRing_.setText("no ringmod", juce::dontSendNotification); noRing_.setFont(uiFont(tokens::textInlineLabel)); noRing_.setColour(juce::Label::textColourId, colour(tokens::faint)); noRing_.setJustificationType(juce::Justification::centred); addAndMakeVisible(noRing_); original_.setText("original", juce::dontSendNotification); original_.setFont(uiFont(10.0f)); original_.setColour(juce::Label::textColourId, colour(tokens::dim)); original_.setJustificationType(juce::Justification::centredLeft); addAndMakeVisible(original_); updateCarriers(); }`
- `void resized() override { layout(); }`
- `void tick() { const int algorithm = static_cast<int>( std::lround(getParam(editor_, params::algorithm(part_)))); const auto& mask = p1::macros::kCarrierMasks[static_cast<std::size_t>( juce::jlimit(0, 9, algorithm))]; int next[4] = {0, 0, 0, 0}; int count = 0; for (int op = 0; op < 4; ++op) if (mask[static_cast<std::size_t>(op)] != 0) next[count++] = op; bool changed = count != carrierCount_; for (int i = 0; i < count && !changed; ++i) changed = next[i] != carriers_[static_cast<std::size_t>(i)]; if (changed) { carrierCount_ = count; for (int i = 0; i < 4; ++i) carriers_[static_cast<std::size_t>(i)] = next[i]; updateCarriers(); } for (int i = 0; i < 3; ++i) { const bool on = std::lround( getParam(editor_, params::ringOn(i, part_))) != 0; auto& level = *level_[static_cast<std::size_t>(i)]; level.setEnabled(on); level.setAlpha(on ? 1.0f : 0.35f); if (i + 1 < carrierCount_) graph_[static_cast<std::size_t>(i)].setDirect( on && carrierOutOn(carriers_[static_cast<std::size_t>(i)]), on && carrierOutOn( carriers_[static_cast<std::size_t>(i + 1)])); } bool anyOn = false; const int pairs = juce::jmax(0, carrierCount_ - 1); for (int i = 0; i < pairs; ++i) anyOn = anyOn \|\| std::lround( getParam(editor_, params::ringOn(i, part_))) != 0; original_.setAlpha(anyOn ? 1.0f : 0.35f); for (auto& toggle : out_) { toggle->setEnabled(anyOn); toggle->setAlpha(anyOn ? 1.0f : 0.35f); } }`
- `bool carrierOutOn(int op) const { return std::lround(getParam(editor_, params::carrierOut(op, part_))) != 0; }`
- `bool isCarrier(int op) const { for (int i = 0; i < carrierCount_; ++i) if (carriers_[static_cast<std::size_t>(i)] == op) return true; return false; }`
- `void updateCarriers() { const int pairs = juce::jmax(0, carrierCount_ - 1); for (int i = 0; i < 3; ++i) { const auto row = static_cast<std::size_t>(i); const bool show = i < pairs; graph_[row].setVisible(show); on_[row]->setVisible(show); level_[row]->setVisible(show); if (show) { const bool on = std::lround( getParam(editor_, params::ringOn(i, part_))) != 0; graph_[row].setPair(carriers_[row], carriers_[static_cast<std::size_t>(i + 1)]); graph_[row].setDirect( on && carrierOutOn(carriers_[row]), on && carrierOutOn( carriers_[static_cast<std::size_t>(i + 1)])); } } const bool controls = carrierCount_ > 1; noRing_.setVisible(!controls); original_.setVisible(controls); for (int op = 0; op < 4; ++op) out_[static_cast<std::size_t>(op)]->setVisible(controls && isCarrier(op)); layout(); }`
- `void layout() { auto area = contentArea(); if (carrierCount_ < 2) { noRing_.setBounds(area); return; } auto outRow = area.removeFromBottom(22); original_.setBounds(outRow.removeFromLeft(44)); int x = outRow.getX(); for (int op = 0; op < 4; ++op) { auto& toggle = *out_[static_cast<std::size_t>(op)]; if (!toggle.isVisible()) continue; toggle.setBounds(x, outRow.getY() + 2, 52, 18); x += 54; } const int pairs = carrierCount_ - 1; for (int i = 0; i < pairs; ++i) { auto row = area.removeFromTop(82); graph_[static_cast<std::size_t>(i)].setBounds( row.getX(), row.getY() + 13, 112, 56); on_[static_cast<std::size_t>(i)]->setBounds( row.getX() + 118, row.getY() + 32, 52, 18); level_[static_cast<std::size_t>(i)]->setBounds( row.getRight() - 56, row.getY(), 56, 82); } }`

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

- `explicit FxModPage(PluginEditor& editor) { const int part = editor.activePart(); PluginEditor* ed = &editor; const juce::String toLabel = part == 0 ? "to 2" : "to 1"; const auto copy = [ed](PluginEditor::PartFxSection section) { return [ed, section] { ed->copyPartFxSection(section); }; }; filter1_ = std::make_unique<FilterCard>( editor, "filter", params::filterCutoff(part), params::filterRes(part), params::filterType(part), params::filterOn(part), copy(PluginEditor::PartFxSection::filter), toLabel); filterEnv_ = std::make_unique<FilterEnvelopeCard>( editor, part, copy(PluginEditor::PartFxSection::filterEnvelope), toLabel); pitchLfo_ = std::make_unique<PitchLfoCard>( editor, copy(PluginEditor::PartFxSection::pitchLfo), toLabel); pitchEnv_ = std::make_unique<PitchEnvelopeCard>( editor, copy(PluginEditor::PartFxSection::pitchEnvelope), toLabel); panEnv_ = std::make_unique<PanEnvelopeCard>( editor, copy(PluginEditor::PartFxSection::panEnvelope), toLabel); keyFollow_ = std::make_unique<FollowCard>( editor, true, "key follow", copy(PluginEditor::PartFxSection::keyFollow), toLabel); velFollow_ = std::make_unique<FollowCard>( editor, false, "velocity", copy(PluginEditor::PartFxSection::velocityFollow), toLabel); ringmod_ = std::make_unique<RingmodCard>(editor, part); addAndMakeVisible(*filter1_); addAndMakeVisible(*filterEnv_); addAndMakeVisible(*pitchLfo_); addAndMakeVisible(*pitchEnv_); addAndMakeVisible(*panEnv_); addAndMakeVisible(*keyFollow_); addAndMakeVisible(*velFollow_); addAndMakeVisible(*ringmod_); }`
- `void resized() override { auto area = getLocalBounds(); const int columns = 4; const int width = (area.getWidth() - (columns - 1) * 10) / columns; const int height = (area.getHeight() - 10) / 2; std::array<P1Card*, 8> cards = { filter1_.get(), filterEnv_.get(), pitchLfo_.get(), pitchEnv_.get(), panEnv_.get(), keyFollow_.get(), velFollow_.get(), ringmod_.get()}; for (int i = 0; i < 8; ++i) { if (cards[static_cast<size_t>(i)] == nullptr) continue; const int column = i % columns; const int row = i / columns; cards[static_cast<size_t>(i)]->setBounds( column * (width + 10), row * (height + 10), width, height); } }`
- `void tick() override { filterEnv_->tick(); pitchLfo_->tick(); pitchEnv_->tick(); panEnv_->tick(); keyFollow_->refresh(); velFollow_->refresh(); ringmod_->tick(); }`

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

- `MacroRange(APVTS& state, juce::String paramID, juce::Colour fill) : state_(state), paramID_(std::move(paramID)), fill_(fill) { spec_ = findSpec(paramID_); if (auto* param = state_.getParameter(paramID_)) { attach_ = std::make_unique<juce::ParameterAttachment>( *param, [this](float value) { value_ = value; repaint(); }); attach_->sendInitialUpdate(); } setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void resized() override { track_ = getLocalBounds().withTrimmedRight(46).reduced(0, 2); }`
- `void paint(juce::Graphics& g) override { if (spec_ == nullptr) return; g.setColour(colour(tokens::lineStrong)); g.drawRect(track_, 1); const auto inner = track_.reduced(1); const float span = static_cast<float>(spec_->max - spec_->min); const float norm = span > 0.0f ? juce::jlimit(0.0f, 1.0f, (value_ - static_cast<float>(spec_->min)) / span) : 0.0f; const int centre = inner.getX() + inner.getWidth() / 2; const int edge = inner.getX() + static_cast<int>(std::round( static_cast<float>(inner.getWidth()) * norm)); const int x = juce::jmin(centre, edge); const int width = juce::jmax(centre, edge) - x; g.setColour(fill_); g.fillRect(juce::Rectangle<int>(x, inner.getY(), width, inner.getHeight())); g.setColour(colour(tokens::lineStrong)); g.drawVerticalLine(centre, static_cast<float>(inner.getY()), static_cast<float>(inner.getBottom())); g.setFont(uiFont(tokens::textInlineLabel)); g.setColour(colour(tokens::pinkBright)); g.drawText(formatParamValue(*spec_, value_), getLocalBounds().removeFromRight(44), juce::Justification::centredRight, false); }`
- `void mouseDown(const juce::MouseEvent& event) override { if (attach_ == nullptr) return; attach_->beginGesture(); setFromMouse(event); }`
- `void mouseDrag(const juce::MouseEvent& event) override { setFromMouse(event); }`
- `void mouseUp(const juce::MouseEvent&) override { if (attach_ != nullptr) attach_->endGesture(); }`
- `void setFromMouse(const juce::MouseEvent& event) { if (attach_ == nullptr \|\| spec_ == nullptr \|\| track_.getWidth() <= 0) return; const float norm = juce::jlimit( 0.0f, 1.0f, (static_cast<float>(event.x) - static_cast<float>(track_.getX())) / static_cast<float>(track_.getWidth())); attach_->setValueAsPartOfGesture( static_cast<float>(spec_->min) + norm * static_cast<float>(spec_->max - spec_->min)); }`

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

- `MacroTargetChoice(APVTS& state, juce::String paramID) : state_(state), paramID_(std::move(paramID)) { combo_.addItem("none", 1); juce::String lastSection; for (int i = 0; i < p1::macros::kTargetCount; ++i) { const auto& target = p1::macros::kTargets[static_cast<std::size_t>(i)]; const juce::String section(target.section); if (section != lastSection) { combo_.addSectionHeading(section); lastSection = section; } combo_.addItem(target.name, i + 2); } combo_.onChange = [this] { if (updating_) return; const int id = combo_.getSelectedId(); if (id <= 0) return; if (auto* parameter = state_.getParameter(paramID_)) parameter->setValueNotifyingHost( parameter->convertTo0to1(static_cast<float>(id - 1))); }; addAndMakeVisible(combo_); if (auto* parameter = state_.getParameter(paramID_)) { attach_ = std::make_unique<juce::ParameterAttachment>( *parameter, [this](float value) { const juce::ScopedValueSetter<bool> guard(updating_, true); combo_.setSelectedId( juce::jlimit(0, p1::macros::kTargetCount, static_cast<int>(std::lround(value))) + 1, juce::dontSendNotification); }); attach_->sendInitialUpdate(); } }`
- `void resized() override { combo_.setBounds(getLocalBounds()); }`

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

- `MacroCard(PluginEditor& editor, int index) : editor_(editor), index_(index), knob_(editor.state(), params::macroValue(index), "", 92.0f), source_(editor.state(), params::macroMidi(index), {"off", "cc " + juce::String(72 + index), "mod wheel", "velocity"}), midiRange_(editor.state(), params::macroMidiRange(index), colour(tokens::green)), rename_("rename", true) { addAndMakeVisible(knob_); addAndMakeVisible(source_); addAndMakeVisible(midiRange_); for (int destination = 0; destination < 3; ++destination) { targets_[static_cast<std::size_t>(destination)] = std::make_unique<MacroTargetChoice>( editor.state(), params::macroTarget(index, destination)); ranges_[static_cast<std::size_t>(destination)] = std::make_unique<MacroRange>( editor.state(), params::macroRange(index, destination), colour(tokens::purple)); addAndMakeVisible(*targets_[static_cast<std::size_t>(destination)]); addAndMakeVisible(*ranges_[static_cast<std::size_t>(destination)]); } rename_.onClick = [this] { editor_.showRenameMacro(index_); }; addAndMakeVisible(rename_); }`
- `void resized() override { auto area = getLocalBounds().reduced(9); auto head = area.removeFromTop(22); rename_.setBounds(head.removeFromRight(60).withSizeKeepingCentre(58, 18)); auto left = area.removeFromLeft(120); knob_.setBounds(left.withSizeKeepingCentre(116, 116)); auto right = area; constexpr int rowHeight = 26; const int spacing = juce::jmax(4, (right.getHeight() - rowHeight * 4) / 5); int y = right.getY() + spacing; midiRow_ = juce::Rectangle<int>(right.getX(), y, right.getWidth(), rowHeight); const int selectWidth = juce::jlimit(120, 176, right.getWidth() - 50 - 190 - 14); source_.setBounds(midiRow_.getX() + 52, midiRow_.getY() + 3, selectWidth, 20); midiRange_.setBounds(midiRow_.getRight() - 190, midiRow_.getY(), 190, rowHeight); y += rowHeight + spacing; for (int destination = 0; destination < 3; ++destination) { auto row = juce::Rectangle<int>(right.getX(), y, right.getWidth(), rowHeight); destRows_[static_cast<std::size_t>(destination)] = row; targets_[static_cast<std::size_t>(destination)]->setBounds( row.getX() + 18, row.getY() + 3, juce::jmax(80, row.getWidth() - 18 - 190 - 8), 20); ranges_[static_cast<std::size_t>(destination)]->setBounds( row.getRight() - 190, row.getY(), 190, rowHeight); y += rowHeight + spacing; } }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::panel)); drawSectionTitle(g, getLocalBounds().removeFromTop(20).reduced(8, 0), title()); g.setFont(uiFont(tokens::textInlineLabel)); g.setColour(colour(tokens::dim)); g.drawText("midi in", midiRow_.getX(), midiRow_.getY() + 4, 50, 16, juce::Justification::centredLeft, false); for (int destination = 0; destination < 3; ++destination) { const auto& row = destRows_[static_cast<std::size_t>(destination)]; g.setColour(destinationOn_[static_cast<std::size_t>(destination)] ? colour(tokens::faint) : colour(tokens::lineStrong)); g.drawText(juce::String(destination + 1), row.getX(), row.getY() + 4, 14, 16, juce::Justification::centredRight, false); } }`
- `void tick() { const int source = static_cast<int>(std::lround( getParam(editor_, params::macroMidi(index_)))); const bool midiOn = source != 0; midiRange_.setEnabled(midiOn); midiRange_.setAlpha(midiOn ? 1.0f : 0.35f); bool changed = midiOn != midiOn_; midiOn_ = midiOn; for (int destination = 0; destination < 3; ++destination) { const auto& row = static_cast<std::size_t>(destination); const bool on = std::lround(getParam( editor_, params::macroTarget(index_, destination))) != 0; ranges_[row]->setEnabled(on); ranges_[row]->setAlpha(on ? 1.0f : 0.35f); changed = changed \|\| on != destinationOn_[row]; destinationOn_[row] = on; } const double effective = editor_.processor().macroEffectiveValue(index_); knob_.setModArc((effective + 100.0) / 200.0); if (changed \|\| name_ != title()) { name_ = title(); repaint(); } }`
- `juce::String title() const { const auto name = editor_.processor().getMacroName(index_); return name.isNotEmpty() ? name : "macro " + juce::String(index_ + 1); }`

### `MacrosPage`

`class` at `plugin/juce/PagesMacros.cpp:295` through line 332.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<MacroCard>, 6> cards_{};`

Methods:

- `explicit MacrosPage(PluginEditor& editor) : editor_(editor) { for (int i = 0; i < 6; ++i) { cards_[static_cast<std::size_t>(i)] = std::make_unique<MacroCard>(editor, i); addAndMakeVisible(*cards_[static_cast<std::size_t>(i)]); } }`
- `void resized() override { const auto area = getLocalBounds(); constexpr int gap = 10; const int columnWidth = (area.getWidth() - gap) / 2; const int rowHeight = (area.getHeight() - 2 * gap) / 3; for (int i = 0; i < 6; ++i) { const int column = i % 2; const int row = i / 2; cards_[static_cast<std::size_t>(i)]->setBounds( area.getX() + column * (columnWidth + gap), area.getY() + row * (rowHeight + gap), columnWidth, rowHeight); } }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); }`
- `void tick() override { for (auto& card : cards_) card->tick(); }`

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

- `OpColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), osc_(editor, op, part_), amp_(editor, op, part_), fb_(editor, op, part_) { addAndMakeVisible(header_); addAndMakeVisible(scope_); addAndMakeVisible(spectro_); addAndMakeVisible(osc_); addAndMakeVisible(amp_); addAndMakeVisible(fb_); }`
- `void resized() override { auto area = getLocalBounds().reduced(8); header_.setBounds(area.removeFromTop(24)); auto visual = area.removeFromTop(40); const int half = (visual.getWidth() - 6) / 2; scope_.setBounds(visual.removeFromLeft(half)); visual.removeFromLeft(6); spectro_.setBounds(visual); area.removeFromTop(6); osc_.setBounds(area.removeFromTop(140)); area.removeFromTop(6); amp_.setBounds(area.removeFromTop(222)); area.removeFromTop(6); fb_.setBounds(area.removeFromTop(222)); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); const int algorithm = static_cast<int>( std::lround(getParam(editor_, params::algorithm(part_)))); g.setFont(displayFont(tokens::textOperatorName)); g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink : tokens::white)); g.drawText("op" + juce::String(op_ + 1), 8, 10, 64, 20, juce::Justification::centredLeft, false); }`
- `void tick() { editor_.processor().readTapScope( op_ * 2 + part_, scratch_.data(), static_cast<int>(scratch_.size())); scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); amp_.tick(); fb_.tick(); header_.tick(); repaint(); }`

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

- `OpLfoColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), ampLfo_(editor, "amp lfo", params::opTremRate(op, part_), params::opTremDepth(op, part_), params::opTremWave(op, part_), params::opTremMode(op, part_), params::opTremRateFade(op, part_), params::opTremDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::amplitudeLfo), feedbackLfo_(editor, "fb lfo", params::opFeedbackLfoRate(op, part_), params::opFeedbackLfoDepth(op, part_), params::opFeedbackLfoWave(op, part_), params::opFeedbackLfoMode(op, part_), params::opFeedbackLfoRateFade(op, part_), params::opFeedbackLfoDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::feedbackLfo), detuneLfo_(editor, "detune lfo", params::opDetuneLfoRate(op, part_), params::opDetuneLfoDepth(op, part_), params::opDetuneLfoWave(op, part_), params::opDetuneLfoMode(op, part_), params::opDetuneLfoRateFade(op, part_), params::opDetuneLfoDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::detuneLfo) { addAndMakeVisible(header_); addAndMakeVisible(scope_); addAndMakeVisible(spectro_); addAndMakeVisible(ampLfo_); addAndMakeVisible(feedbackLfo_); addAndMakeVisible(detuneLfo_); }`
- `void resized() override { auto area = getLocalBounds().reduced(8); header_.setBounds(area.removeFromTop(24)); auto visual = area.removeFromTop(240); const int half = (visual.getWidth() - 6) / 2; scope_.setBounds(visual.removeFromLeft(half)); visual.removeFromLeft(6); spectro_.setBounds(visual); area.removeFromTop(12); ampLfo_.setBounds(area.removeFromTop(113)); area.removeFromTop(12); feedbackLfo_.setBounds(area.removeFromTop(113)); area.removeFromTop(12); detuneLfo_.setBounds(area.removeFromTop(113)); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); const int algorithm = static_cast<int>( std::lround(getParam(editor_, params::algorithm(part_)))); g.setFont(displayFont(tokens::textOperatorName)); g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink : tokens::white)); g.drawText("op" + juce::String(op_ + 1), 8, 10, 64, 20, juce::Justification::centredLeft, false); }`
- `void tick() { editor_.processor().readTapScope( op_ * 2 + part_, scratch_.data(), static_cast<int>(scratch_.size())); scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); header_.tick(); repaint(); }`

### `OperatorsPage`

`class` at `plugin/juce/PagesOperators.cpp:205` through line 236.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<OpColumn>, 4> columns_{};`

Methods:

- `explicit OperatorsPage(PluginEditor& editor) : editor_(editor) { for (int i = 0; i < 4; ++i) { columns_[static_cast<size_t>(i)] = std::make_unique<OpColumn>( editor, i, [this](int op) { editor_.copyOperator(op); }, [this](int op) { editor_.pasteOperator(op); }); addAndMakeVisible(*columns_[static_cast<size_t>(i)]); } }`
- `void resized() override { auto area = getLocalBounds(); const int width = (area.getWidth() - 30) / 4; for (auto& column : columns_) { column->setBounds(area.removeFromLeft(width)); area.removeFromLeft(10); } }`
- `void tick() override { for (auto& column : columns_) column->tick(); }`

### `OperatorLfosPage`

`class` at `plugin/juce/PagesOperators.cpp:238` through line 269.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<OpLfoColumn>, 4> columns_{};`

Methods:

- `explicit OperatorLfosPage(PluginEditor& editor) : editor_(editor) { for (int i = 0; i < 4; ++i) { columns_[static_cast<size_t>(i)] = std::make_unique<OpLfoColumn>( editor, i, [this](int op) { editor_.copyOperator(op); }, [this](int op) { editor_.pasteOperator(op); }); addAndMakeVisible(*columns_[static_cast<size_t>(i)]); } }`
- `void resized() override { auto area = getLocalBounds(); const int width = (area.getWidth() - 30) / 4; for (auto& column : columns_) { column->setBounds(area.removeFromLeft(width)); area.removeFromLeft(10); } }`
- `void tick() override { for (auto& column : columns_) column->tick(); }`

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

- `PhaseMapView(PluginEditor& editor, int op, int part) : state_(editor.state()), processor_(editor.processor()), op_(op), part_(part) { setInterceptsMouseClicks(false, false); }`
- `void tick() { p1::PhaseMapSnapshot live; const bool hasLive = processor_.phaseMap(op_, part_, live); const int on = static_cast<int>( std::lround(rawValue(state_, params::opPhase(op_, "on", part_)))); const int fallbackSegments = juce::jlimit( 1, 32, static_cast<int>(std::lround( rawValue(state_, params::opPhase(op_, "segments", part_))))); const bool liveMap = hasLive && on != 0; const int segments = liveMap ? live.segments : fallbackSegments; const float bp = liveMap ? static_cast<float>(live.bp) / 1000.0f : rawValue(state_, params::opPhase(op_, "bp", part_)) / 1000.0f; const auto mix = [](uint64_t h, uint64_t value) { h ^= value; h *= 1099511628211ull; return h; }; uint64_t h = 1469598103934665603ull; h = mix(h, static_cast<uint64_t>(on)); h = mix(h, liveMap ? 1u : 0u); h = mix(h, static_cast<uint64_t>(segments)); h = mix(h, std::bit_cast<uint32_t>(bp)); if (liveMap) { h = mix(h, static_cast<uint64_t>(live.segments)); h = mix(h, static_cast<uint64_t>(live.active)); h = mix(h, static_cast<uint64_t>(live.voices)); if (live.voices > 0) { h = mix(h, static_cast<uint64_t>(live.phaseQ)); h = mix(h, static_cast<uint64_t>(live.phaseY)); } const auto active = static_cast<std::size_t>(live.active); for (std::size_t k = 0; k < active; ++k) { h = mix(h, static_cast<uint64_t>(live.x[k])); h = mix(h, static_cast<uint64_t>(live.x[k + 1])); h = mix(h, static_cast<uint64_t>(live.y0[k])); h = mix(h, static_cast<uint64_t>( static_cast<uint32_t>(live.slope[k]))); h = mix(h, static_cast<uint64_t>(live.flip[k])); h = mix(h, static_cast<uint64_t>(live.order[k])); h = mix(h, static_cast<uint64_t>(live.locked[k])); } } if (!haveSignature_) { haveSignature_ = true; lastSignature_ = h; repaint(); return; } if (h != lastSignature_) { lastSignature_ = h; repaint(); } }`
- `void paint(juce::Graphics& g) override { auto area = getLocalBounds().toFloat().reduced(2.0f); auto captionArea = area.toNearestInt().removeFromBottom(13); g.fillAll(colour(tokens::bg)); p1::PhaseMapSnapshot live; const bool hasLive = processor_.phaseMap(op_, part_, live); const int on = static_cast<int>( std::lround(rawValue(state_, params::opPhase(op_, "on", part_)))); const int fallbackSegments = juce::jlimit( 1, 32, static_cast<int>(std::lround( rawValue(state_, params::opPhase(op_, "segments", part_))))); /* Off is the neutral map, not the last live state. The engine also * clears the pool on the switch edge and on a patch load. */ const bool liveMap = hasLive && on != 0; const int segments = liveMap ? live.segments : fallbackSegments; const float bp = liveMap ? static_cast<float>(live.bp) / 1000.0f : rawValue(state_, params::opPhase(op_, "bp", part_)) / 1000.0f; const auto alpha = on ? 1.0f : 0.45f; g.setColour(colour(tokens::lineStrong).withAlpha(0.28f)); for (int i = 1; i < 4; ++i) { const float x = area.getX() + area.getWidth() * static_cast<float>(i) / 4.0f; const float y = area.getY() + area.getHeight() * static_cast<float>(i) / 4.0f; g.drawVerticalLine(juce::roundToInt(x), area.getY(), area.getBottom()); g.drawHorizontalLine(juce::roundToInt(y), area.getX(), area.getRight()); } const auto point = [&area](float x, float y) { return juce::Point<float>(area.getX() + x * area.getWidth(), area.getBottom() - y * area.getHeight()); }; /* F: 0 -> 0.5 at the folded breakpoint, then 0.5 -> 1. */ const float xb = juce::jlimit(0.001f, 0.999f, bp); juce::Path f; f.startNewSubPath(point(0.0f, 0.0f)); f.lineTo(point(xb, 0.5f)); f.lineTo(point(1.0f, 1.0f)); g.setColour(colour(tokens::lineStrong).withAlpha(0.55f * alpha)); g.strokePath(f, juce::PathStrokeType(1.0f)); if (liveMap) { const float dashes[] = {2.0f, 2.0f}; for (int i = 1; i < live.segments; ++i) { const float y = static_cast<float>(i) / static_cast<float>(live.segments); g.setColour(colour(tokens::lineStrong).withAlpha(0.10f * alpha)); g.drawHorizontalLine(juce::roundToInt(area.getBottom() - y * area.getHeight()), area.getX(), area.getRight()); } for (std::size_t k = 0; k < static_cast<std::size_t>(live.active); ++k) { const float x0 = static_cast<float>(live.x[k]) / 4096.0f; const float x1 = static_cast<float>(live.x[k + 1]) / 4096.0f; const float y0 = static_cast<float>(live.y0[k]) / 4096.0f; const int32_t dy = static_cast<int32_t>(( static_cast<int64_t>(live.x[k + 1] - live.x[k]) * live.slope[k]) >> 16); const float y1 = static_cast<float>(juce::jlimit( 0, 4095, static_cast<int>(live.y0[k]) + dy)) / 4096.0f; const auto segmentColour = live.flip[k] ? tokens::purple : tokens::green; g.setColour(colour(segmentColour).withAlpha( alpha * (live.locked[k] ? 0.35f : 1.0f))); g.drawLine(juce::Line<float>(point(x0, y0), point(x1, y1)), 1.4f); if (live.order[k] != static_cast<uint8_t>(k)) { g.setColour(colour(tokens::pink).withAlpha(alpha)); g.drawDashedLine( juce::Line<float>(point(x0, 0.0f), point(x0, 1.0f)), dashes, 2, 1.0f); } } if (live.voices > 0) { const auto dot = point(static_cast<float>(live.phaseQ) / 4096.0f, static_cast<float>(live.phaseY) / 4096.0f); g.setColour(colour(tokens::white)); g.fillEllipse(dot.x - 2.0f, dot.y - 2.0f, 4.0f, 4.0f); } } else { juce::Path map; map.startNewSubPath(point(0.0f, 0.0f)); for (int i = 1; i <= segments; ++i) { const float x = static_cast<float>(i) / static_cast<float>(segments); const float y = static_cast<float>(i) / static_cast<float>(segments); map.lineTo(point(x, y)); } g.setColour(colour(tokens::green).withAlpha(alpha)); g.strokePath(map, juce::PathStrokeType(1.4f)); } g.setColour(colour(tokens::pink).withAlpha(alpha)); g.drawVerticalLine(juce::roundToInt(area.getX() + xb * area.getWidth()), area.getY(), area.getBottom()); int locked = 0; if (liveMap) for (std::size_t k = 0; k < static_cast<std::size_t>(live.active); ++k) locked += live.locked[k] ? 1 : 0; g.setFont(uiFont(tokens::textLegend)); g.setColour(colour(tokens::dim)); const auto caption = "bp " + juce::String(bp, 3) + " · π"; g.drawText(caption, captionArea, juce::Justification::centredLeft, false); auto count = juce::String(liveMap ? live.active : segments) + "/" + juce::String(segments) + " seg"; if (locked > 0) count += " " + juce::String(locked) + " locked"; g.drawText(count, captionArea, juce::Justification::centredRight, false); }`

### `PhaseCard`

`class` at `plugin/juce/PagesPhase.cpp:235` through line 288.

Fields and enumerators:

- `P1Toggle on_;`
- `std::array<std::unique_ptr<ParamKnob>, 9> knobs_{};`

Methods:

- `PhaseCard(PluginEditor& editor, int op, int part) : P1Card("phase", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phase); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::phase); }), on_(editor.state(), params::opPhase(op, "on", part), "on") { static constexpr const char* fields[9] = { "bp", "segments", "flip", "flip_rate", "reorder", "reorder_rate", "jump", "drift", "drift_rate"}; static constexpr const char* labels[9] = { "breakpoint x", "segments", "flip", "flip rate", "reorder", "reorder rate", "reorder jump", "drift", "drift rate"}; for (std::size_t i = 0; i < knobs_.size(); ++i) { knobs_[i] = std::make_unique<ParamKnob>( editor.state(), params::opPhase(op, fields[i], part), labels[i], 28.0f, 10.0f); addAndMakeVisible(*knobs_[i]); } addAndMakeVisible(on_); setHeaderTrailingWidth(48); }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); area.removeFromTop(3); for (int row = 0; row < 2; ++row) { if (row == 1) area.removeFromTop(4); auto line = area.removeFromTop(63); const int count = row == 0 ? 4 : 5; const int cell = line.getWidth() / count; for (int col = 0; col < count; ++col) { const int index = row == 0 ? col : 4 + col; knobs_[static_cast<std::size_t>(index)]->setBounds( line.removeFromLeft(cell)); } } on_.setBounds(headerTrailingBounds()); }`

### `PhaseEnvelopeCard`

`class` at `plugin/juce/PagesPhase.cpp:290` through line 378.

Fields and enumerators:

- `P1Envelope envView_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `PhaseEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("phase env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phaseEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::phaseEnvelope); }), envView_(editor.state(), {params::opPhaseEnv(op, "r1", part), params::opPhaseEnv(op, "r2", part), params::opPhaseEnv(op, "r3", part), params::opPhaseEnv(op, "relr", part)}, {params::opPhaseEnv(op, "l1", part), params::opPhaseEnv(op, "l2", part), params::opPhaseEnv(op, "l3", part), params::opPhaseEnv(op, "rell", part)}, colour(tokens::purple), -100.0, 100.0, true, EnvelopeShape::bipolar, params::opPhaseEnv(op, "mode", part)), on_(editor.state(), params::opPhaseEnv(op, "on", part), "on"), mode_(editor.state(), params::opPhaseEnv(op, "mode", part), {"sustain off", "sustain on", "loop 1", "loop 2"}) { addAndMakeVisible(envView_); addAndMakeVisible(on_); addAndMakeVisible(mode_); for (int i = 0; i < 3; ++i) { rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::opPhaseEnv( op, i == 0 ? "r1" : i == 1 ? "r2" : "r3", part), "r" + juce::String(i + 1), 18.0f); addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]); } rates_[3] = std::make_unique<ParamKnob>( editor.state(), params::opPhaseEnv(op, "relr", part), "rel rate", 18.0f); addAndMakeVisible(*rates_[3]); for (int i = 0; i < 3; ++i) { levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::opPhaseEnv( op, i == 0 ? "l1" : i == 1 ? "l2" : "l3", part), "l" + juce::String(i + 1), 18.0f); addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]); } levels_[3] = std::make_unique<ParamKnob>( editor.state(), params::opPhaseEnv(op, "rell", part), "rl", 18.0f); addAndMakeVisible(*levels_[3]); setHeaderTrailingWidth(48); }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); envView_.setBounds(area.removeFromTop(48)); area.removeFromTop(2); auto rates = area.removeFromTop(54); const int rateCell = rates.getWidth() / 4; for (auto& knob : rates_) knob->setBounds(rates.removeFromLeft(rateCell)); area.removeFromTop(2); auto levels = area.removeFromTop(54); const int levelCell = levels.getWidth() / 4; for (auto& knob : levels_) knob->setBounds(levels.removeFromLeft(levelCell)); area.removeFromTop(2); auto modeRow = area.removeFromTop(tokens::choiceHeight); mode_.setBounds(modeRow.removeFromLeft(levelCell * 2)); on_.setBounds(headerTrailingBounds()); }`
- `void tick() { envView_.tick(); }`

### `CompactOscillator`

`class` at `plugin/juce/PagesPhase.cpp:380` through line 428.

Fields and enumerators:

- `ParamKnob ratio_;`
- `ParamKnob detune_;`
- `ParamKnob level_;`
- `ParamKnob feedback_;`
- `ParamChoice wave_;`

Methods:

- `CompactOscillator(PluginEditor& editor, int op, int part) : ratio_(editor.state(), params::opRatio(op, part), "ratio", 22.0f), detune_(editor.state(), params::opDetune(op, part), "detune", 22.0f), level_(editor.state(), params::opLevel(op, part), "level", 22.0f), feedback_(editor.state(), params::opFeedback(op, part), "feedback", 22.0f), wave_(editor.state(), params::opWaveform(op, part), params::waveNames()) { addAndMakeVisible(ratio_); addAndMakeVisible(detune_); addAndMakeVisible(level_); addAndMakeVisible(feedback_); addAndMakeVisible(wave_); }`
- `void resized() override { auto area = getLocalBounds().reduced(8, 0); area.removeFromTop(2); auto waveArea = area.removeFromRight(70); area.removeFromRight(6); const int cell = area.getWidth() / 4; ratio_.setBounds(area.removeFromLeft(cell)); detune_.setBounds(area.removeFromLeft(cell)); level_.setBounds(area.removeFromLeft(cell)); feedback_.setBounds(area.removeFromLeft(cell)); waveArea.removeFromTop(26); wave_.setBounds(waveArea.removeFromTop(18)); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::panel)); g.setColour(colour(tokens::dim)); g.setFont(uiFont(tokens::textLegend)); g.drawText("wave", getWidth() - 78, 6, 70, 12, juce::Justification::centred, false); }`

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

- `PhaseColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), osc_(editor, op, part_), map_(editor, op, part_), phase_(editor, op, part_), lfo_(editor, "phase lfo", params::opPhaseLfo(op, "rate", part_), params::opPhaseLfo(op, "depth", part_), params::opPhaseLfo(op, "wave", part_), params::opPhaseLfo(op, "mode", part_), params::opPhaseLfo(op, "rate_fade", part_), params::opPhaseLfo(op, "depth_fade", part_), op, part_, PluginEditor::OperatorSection::phaseLfo, {}, true, "bipolar"), env_(editor, op, part_) { addAndMakeVisible(header_); addAndMakeVisible(osc_); addAndMakeVisible(map_); addAndMakeVisible(phase_); addAndMakeVisible(lfo_); addAndMakeVisible(env_); }`
- `void resized() override { auto area = getLocalBounds().reduced(8); area.setBottom(area.getBottom() + 4); header_.setBounds(area.removeFromTop(26)); osc_.setBounds(area.removeFromTop(60)); area.removeFromTop(8); map_.setBounds(area.removeFromTop(88)); area.removeFromTop(8); phase_.setBounds(area.removeFromTop(155)); area.removeFromTop(8); lfo_.setBounds(area.removeFromTop(106)); area.removeFromTop(8); env_.setBounds(area.removeFromTop(206)); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); const int algorithm = static_cast<int>( std::lround(getParam(editor_, params::algorithm(part_)))); g.setFont(displayFont(tokens::textOperatorName)); g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink : tokens::white)); g.drawText("op" + juce::String(op_ + 1), 8, 11, 64, 20, juce::Justification::centredLeft, false); }`
- `void tick() { header_.tick(); map_.tick(); env_.tick(); }`

### `PhaseMapPage`

`class` at `plugin/juce/PagesPhase.cpp:515` through line 550.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<PhaseColumn>, 4> columns_{};`

Methods:

- `explicit PhaseMapPage(PluginEditor& editor) : editor_(editor) { for (int i = 0; i < 4; ++i) { columns_[static_cast<std::size_t>(i)] = std::make_unique<PhaseColumn>( editor, i, [this](int op) { editor_.copyOperator(op); }, [this](int op) { editor_.pasteOperator(op); }); addAndMakeVisible(*columns_[static_cast<std::size_t>(i)]); } }`
- `void resized() override { auto area = getLocalBounds(); constexpr int gap = 10; const int width = (area.getWidth() - gap * 3) / 4; for (int i = 0; i < 4; ++i) { columns_[static_cast<std::size_t>(i)]->setBounds( area.removeFromLeft(width)); if (i < 3) area.removeFromLeft(gap); } }`
- `void tick() override { for (auto& column : columns_) column->tick(); }`

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

- `SpringSlider(juce::String label, std::function<void()> onBegin, std::function<void(double)> onChange, std::function<void()> onEnd = {}) : label_(std::move(label)), onBegin_(std::move(onBegin)), onChange_(std::move(onChange)), onEnd_(std::move(onEnd)) { setMouseCursor(juce::MouseCursor::UpDownResizeCursor); }`
- `void paint(juce::Graphics& g) override { auto area = getLocalBounds(); g.setFont(uiFont(tokens::textLegend)); g.setColour(colour(tokens::dim)); g.drawText(label_, area.removeFromTop(12), juce::Justification::centred, false); auto valueArea = area.removeFromBottom(12); g.setColour(colour(tokens::pink)); g.drawText(valueText(), valueArea, juce::Justification::centred, false); auto track = area.reduced(0, 1).withSizeKeepingCentre( 18, juce::jmax(12, area.getHeight() - 2)); g.setColour(colour(tokens::bg)); g.fillRect(track); g.setColour(colour(tokens::lineStrong)); g.drawRect(track, 1); const float centre = static_cast<float>(track.getCentreY()); g.drawHorizontalLine(juce::roundToInt(centre), static_cast<float>(track.getX() + 2), static_cast<float>(track.getRight() - 2)); const float travel = static_cast<float>(track.getHeight()) * 0.5f - 4.0f; const float thumb = centre - static_cast<float>(amount_) * travel; g.setColour(colour(tokens::pink)); g.fillRect(juce::Rectangle<float>( static_cast<float>(track.getX() + 2), thumb - 1.5f, static_cast<float>(track.getWidth() - 4), 3.0f)); }`
- `void mouseDown(const juce::MouseEvent& event) override { if (event.mods.isPopupMenu()) return; dragging_ = true; startY_ = event.position.y; if (onBegin_) onBegin_(); setAmount(0.0); }`
- `void mouseDrag(const juce::MouseEvent& event) override { if (!dragging_ \|\| event.mods.isPopupMenu()) return; setAmount((startY_ - event.position.y) / 32.0); }`
- `void mouseUp(const juce::MouseEvent&) override { if (!dragging_) return; dragging_ = false; amount_ = 0.0; repaint(); if (onEnd_) onEnd_(); }`
- `juce::String valueText() const { const int percent = static_cast<int>(std::lround(amount_ * 100.0)); return (percent > 0 ? "+" : "") + juce::String(percent) + "%"; }`
- `void setAmount(double amount) { amount_ = juce::jlimit(-1.0, 1.0, amount); repaint(); if (onChange_) onChange_(amount_); }`

### `MaskRow`

`class` at `plugin/juce/PagesSpectral.cpp:185` through line 235.

Fields and enumerators:

- `std::function<void(int)> onChange;`
- `int index_ = 2;`

Methods:

- `explicit MaskRow(int initial) : index_(juce::jlimit(0, 2, initial)) { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `int index() const { return index_; }`
- `void paint(juce::Graphics& g) override { auto area = getLocalBounds(); const int cell = area.getWidth() / 3; for (int i = 0; i < 3; ++i) { auto r = (i == 2 ? area : area.removeFromLeft(cell)).reduced(1); g.setColour(colour(tokens::bg)); g.fillRect(r); g.setColour(i == index_ ? colour(tokens::green) : colour(tokens::lineStrong)); g.drawRect(r, 1); g.setFont(uiFont(10.0f)); g.setColour(i == index_ ? colour(tokens::green) : colour(tokens::dim)); g.drawText(kSpectrumMaskNames[i], r, juce::Justification::centred, false); if (i == 2) break; } }`
- `void mouseDown(const juce::MouseEvent& event) override { if (event.mods.isPopupMenu()) return; const int next = juce::jlimit( 0, 2, static_cast<int>(event.position.x * 3.0f / static_cast<float>( juce::jmax(1, getWidth())))); if (next == index_) return; index_ = next; repaint(); if (onChange) onChange(index_); }`

### `MorphEnvelopeCard`

`class` at `plugin/juce/PagesSpectral.cpp:237` through line 323.

Fields and enumerators:

- `P1Envelope env_;`
- `P1Toggle on_;`
- `ParamChoice mode_;`
- `std::array<std::unique_ptr<ParamKnob>, 4> rates_{};`
- `std::array<std::unique_ptr<ParamKnob>, 4> levels_{};`

Methods:

- `MorphEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("morph env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::spectrumEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::spectrumEnvelope); }), env_(editor.state(), {params::opSpecEnvRate(op, 0, part), params::opSpecEnvRate(op, 1, part), params::opSpecEnvRate(op, 2, part), params::opSpecEnvRelRate(op, part)}, {params::opSpecEnvLevel(op, 0, part), params::opSpecEnvLevel(op, 1, part), params::opSpecEnvLevel(op, 2, part), params::opSpecEnvRelLevel(op, part)}, colour(tokens::purple), 0.0, 99.0, false, EnvelopeShape::bipolar, params::opSpecEnvMode(op, part), params::opSpecMorph(op, part)), on_(editor.state(), params::opSpecEnvOn(op, part), "on"), mode_(editor.state(), params::opSpecEnvMode(op, part), {"sustain off", "sustain on", "loop 1", "loop 2"}) { addAndMakeVisible(env_); addAndMakeVisible(on_); addAndMakeVisible(mode_); on_.onClick = [&ed = editor, op, part] { ensureSpectrumMode(ed, op, part, params::opSpecEnvOn(op, part)); }; for (int i = 0; i < 3; ++i) { rates_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::opSpecEnvRate(op, i, part), "r" + juce::String(i + 1), 28.0f); addAndMakeVisible(*rates_[static_cast<std::size_t>(i)]); } rates_[3] = std::make_unique<ParamKnob>( editor.state(), params::opSpecEnvRelRate(op, part), "rel rate", 28.0f); addAndMakeVisible(*rates_[3]); for (int i = 0; i < 3; ++i) { levels_[static_cast<std::size_t>(i)] = std::make_unique<ParamKnob>( editor.state(), params::opSpecEnvLevel(op, i, part), "l" + juce::String(i + 1), 28.0f); addAndMakeVisible(*levels_[static_cast<std::size_t>(i)]); } levels_[3] = std::make_unique<ParamKnob>( editor.state(), params::opSpecEnvRelLevel(op, part), "rl", 28.0f); addAndMakeVisible(*levels_[3]); setHeaderTrailingWidth(48); }`
- `void resized() override { P1Card::resized(); auto area = contentArea(); area.removeFromTop(8); env_.setBounds(area.removeFromTop(38)); auto rates = area.removeFromTop(60); const int rateCell = rates.getWidth() / 4; for (auto& knob : rates_) knob->setBounds(rates.removeFromLeft(rateCell)); auto levels = area.removeFromTop(60); const int levelCell = levels.getWidth() / 4; for (auto& knob : levels_) knob->setBounds(levels.removeFromLeft(levelCell)); area.removeFromTop(4); auto modeRow = area.removeFromTop(tokens::choiceHeight); mode_.setBounds(modeRow.removeFromLeft(96)); on_.setBounds(headerTrailingBounds()); }`
- `void tick() { env_.tick(); }`

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

- `SpectrumRow(PluginEditor& editor, int op, int part, bool a) : P1Card("op" + juce::String(op + 1) + " spectrum " + (a ? "a" : "b"), [&ed = editor, op, part, a] { ed.copyOperatorSection( op, part, a ? PluginEditor::OperatorSection::spectrumA : PluginEditor::OperatorSection::spectrumB); }, [&ed = editor, op, part, a] { ed.pasteOperatorSection( op, part, a ? PluginEditor::OperatorSection::spectrumA : PluginEditor::OperatorSection::spectrumB); }), editor_(editor), op_(op), part_(part), isA_(a), bars_(editor.state(), spectrumIDs(op, part, a), colour(a ? tokens::purple : tokens::pink)), range_(editor.state(), a ? params::opSpecRangeA(op, part) : params::opSpecRangeB(op, part), rangeNames()), mask_(2), tilt_( "tilt", [this] { beginMacro(); }, [this](double value) { applyTilt(value); }, [this] { endMacro(); }), level_( "level", [this] { beginMacro(); }, [this](double value) { applyLevel(value); }, [this] { endMacro(); }), formant_( "formant", [this] { beginMacro(); }, [this](double value) { applyFormant(value); }, [this] { endMacro(); }) { addAndMakeVisible(bars_); addAndMakeVisible(range_); rangeLabel_.setText("range", juce::dontSendNotification); rangeLabel_.setFont(uiFont(tokens::textLegend)); rangeLabel_.setColour(juce::Label::textColourId, colour(tokens::dim)); addAndMakeVisible(rangeLabel_); maskLabel_.setText("mask", juce::dontSendNotification); maskLabel_.setFont(uiFont(tokens::textLegend)); maskLabel_.setColour(juce::Label::textColourId, colour(tokens::dim)); addAndMakeVisible(maskLabel_); addAndMakeVisible(mask_); selectionLabel_.setText("selection", juce::dontSendNotification); selectionLabel_.setFont(uiFont(tokens::textLegend)); selectionLabel_.setColour(juce::Label::textColourId, colour(tokens::dim)); addAndMakeVisible(selectionLabel_); addAndMakeVisible(selection_); groupLabel_.setText("group", juce::dontSendNotification); groupLabel_.setFont(uiFont(tokens::textLegend)); groupLabel_.setColour(juce::Label::textColourId, colour(tokens::dim)); addAndMakeVisible(groupLabel_); addAndMakeVisible(group_); template_.onClick = [this] { showTemplateMenu(); }; addAndMakeVisible(template_); addAndMakeVisible(norm_); addAndMakeVisible(toOther_); addAndMakeVisible(tilt_); addAndMakeVisible(level_); addAndMakeVisible(formant_); addAndMakeVisible(centre_); legend_.setFont(uiFont(tokens::textLegend)); legend_.setColour(juce::Label::textColourId, colour(tokens::dim)); legend_.setMinimumHorizontalScale(0.6f); legend_.setInterceptsMouseClicks(false, false); addAndMakeVisible(legend_); group_.addItemList( juce::StringArray(kSpectrumGroupLabels, kSpectrumGroupCount), 1); group_.setSelectedItemIndex(0, juce::dontSendNotification); group_.onChange = [this] { refreshGroupTest(); refreshLegend(); }; mask_.onChange = [this](int) { refreshGroupTest(); refreshLegend(); }; selection_.onChange = [this] { applySelection(); }; centre_.addItemList( juce::StringArray([] { juce::StringArray names; for (int h = 1; h <= 64; ++h) names.add("h" + juce::String(h)); return names; }()), 1); centre_.setSelectedItemIndex(4, juce::dontSendNotification); norm_.onClick = [this] { normalise(); }; toOther_.onClick = [this] { copyToOther(); }; toOther_.setButtonText(isA_ ? "to b" : "to a"); refreshSelectionItems(); applySelection(); refreshGroupTest(); refreshLegend(); }`
- `void tick() { const int first = firstHarmonic(); bars_.setFirstHarmonic(first); if (first != lastFirst_) { lastFirst_ = first; refreshSelectionItems(); applySelection(); } refreshLegend(); bars_.repaint(); }`
- `void resized() override { P1Card::resized(); /* Header controls sit to the left of the card's copy and paste pair. */ auto header = getLocalBounds().removeFromTop(20).reduced(4, 1); header.removeFromRight(112); toOther_.setBounds(header.removeFromRight(44)); header.removeFromRight(4); norm_.setBounds(header.removeFromRight(44)); header.removeFromRight(6); template_.setBounds(header.removeFromRight(150)); header.removeFromRight(6); group_.setBounds(header.removeFromRight(120)); header.removeFromRight(3); groupLabel_.setBounds(header.removeFromRight(30)); header.removeFromRight(6); selection_.setBounds(header.removeFromRight(104)); header.removeFromRight(3); selectionLabel_.setBounds(header.removeFromRight(48)); header.removeFromRight(6); mask_.setBounds(header.removeFromRight(112)); header.removeFromRight(3); maskLabel_.setBounds(header.removeFromRight(26)); header.removeFromRight(6); range_.setBounds(header.removeFromRight(64)); header.removeFromRight(3); rangeLabel_.setBounds(header.removeFromRight(30)); auto area = contentArea(); auto controls = area.removeFromRight(280); area.removeFromRight(8); bars_.setBounds(area.removeFromTop(82)); legend_.setBounds(area.removeFromTop(14)); controls.removeFromTop(6); tilt_.setBounds(controls.removeFromLeft(84)); controls.removeFromLeft(4); level_.setBounds(controls.removeFromLeft(84)); controls.removeFromLeft(4); formant_.setBounds(controls.removeFromLeft(50)); centre_.setBounds( controls.removeFromLeft(58).withTrimmedBottom(78)); }`
- `int firstHarmonic() const { const int range = juce::jlimit( 0, 1, static_cast<int>(std::lround(getParam( editor_, isA_ ? params::opSpecRangeA(op_, part_) : params::opSpecRangeB(op_, part_))))); return range * 64 + 1; }`
- `void refreshGroupTest() { bars_.setGroupTest([this](int harmonic) { return group_.getSelectedItemIndex() != 0 && groupHas(group_.getSelectedItemIndex(), harmonic); }); }`
- `std::pair<int, int> windowFor(int index) const { static constexpr int kOffsets[7][2] = { {0, 63}, {0, 31}, {32, 63}, {0, 15}, {16, 31}, {32, 47}, {48, 63}}; const int choice = juce::jlimit(0, 6, index); const int first = firstHarmonic(); return {first + kOffsets[choice][0], first + kOffsets[choice][1]}; }`
- `void refreshSelectionItems() { const int choice = juce::jlimit(0, 6, selection_.getSelectedItemIndex()); juce::StringArray labels; for (int i = 0; i < 7; ++i) { const auto [first, last] = windowFor(i); labels.add("edit " + juce::String(first) + "-" + juce::String(last)); } selection_.clear(juce::dontSendNotification); selection_.addItemList(labels, 1); selection_.setSelectedItemIndex(choice, juce::dontSendNotification); }`
- `void applySelection() { const int choice = juce::jlimit(0, 6, selection_.getSelectedItemIndex()); const auto [first, last] = windowFor(choice); bars_.setEditWindow(first, last); bars_.setEditTest( [this](int harmonic, double level) { return editable(harmonic, level); }); }`
- `bool editable(int harmonic, double level) const { const auto [first, last] = windowFor(selection_.getSelectedItemIndex()); if (harmonic < first \|\| harmonic > last) return false; const int mask = mask_.index(); if (mask == 2) return true; const bool live = level > 0.0; return mask == 0 ? live : !live; }`
- `bool selected(int index) const { if (snapshot_.empty()) return false; const int harmonic = firstHarmonic() + index; const auto [first, last] = windowFor(selection_.getSelectedItemIndex()); if (harmonic < first \|\| harmonic > last) return false; if (!groupHas(group_.getSelectedItemIndex(), harmonic)) return false; const int mask = mask_.index(); if (mask == 2) return true; const bool live = snapshot_[static_cast<std::size_t>(index)] > 0.0; return mask == 0 ? live : !live; }`
- `void beginMacro() { snapshot_ = bars_.levels(); }`
- `void endMacro() { snapshot_.clear(); }`
- `void applyTilt(double amount) { if (snapshot_.empty()) return; auto levels = snapshot_; const double centre = (static_cast<double>(levels.size()) - 1.0) * 0.5; for (int i = 0; i < static_cast<int>(levels.size()); ++i) { if (!selected(i)) continue; levels[static_cast<std::size_t>(i)] = juce::jlimit( 0.0, 99.0, levels[static_cast<std::size_t>(i)] * (1.0 + amount * (static_cast<double>(i) - centre) / centre)); } bars_.setLevels(levels); }`
- `void applyLevel(double amount) { if (snapshot_.empty()) return; auto levels = snapshot_; for (int i = 0; i < static_cast<int>(levels.size()); ++i) { if (!selected(i)) continue; levels[static_cast<std::size_t>(i)] = juce::jlimit( 0.0, 99.0, levels[static_cast<std::size_t>(i)] + amount * 99.0); } bars_.setLevels(levels); }`
- `void applyFormant(double amount) { if (snapshot_.empty()) return; auto levels = snapshot_; const int formantCentre = centre_.getSelectedItemIndex() + 1; const auto [winFirst, winLast] = windowFor(selection_.getSelectedItemIndex()); for (int i = 0; i < static_cast<int>(levels.size()); ++i) { const int h = firstHarmonic() + i; if (h < winFirst \|\| h > winLast) continue; levels[static_cast<std::size_t>(i)] = juce::jlimit( 0.0, 99.0, levels[static_cast<std::size_t>(i)] + amount * 99.0 * std::exp(-((h - formantCentre) * (h - formantCentre)) / 4.5)); } bars_.setLevels(levels); }`
- `void normalise() { auto levels = bars_.levels(); double peak = 0.0; for (double level : levels) peak = juce::jmax(peak, level); if (peak <= 0.0) return; for (double& level : levels) level = level * 99.0 / peak; bars_.setLevels(levels); }`
- `void copyToOther() { const auto levels = bars_.levels(); const auto other = spectrumIDs(op_, part_, !isA_); for (int i = 0; i < other.size() && i < static_cast<int>(levels.size()); ++i) { if (auto* parameter = editor_.state().getParameter(other[i])) { parameter->setValueNotifyingHost(parameter->convertTo0to1( static_cast<float>(levels[static_cast<std::size_t>(i)]))); } } }`
- `void showTemplateMenu() { juce::PopupMenu menu; const auto names = params::waveNames(); for (int i = 0; i < 13 && i < names.size(); ++i) menu.addItem(i + 1, names[i]); menu.showMenuAsync( juce::PopupMenu::Options().withTargetComponent(&template_) .withMinimumWidth(150), [this](int result) { if (result > 0) loadTemplate(result - 1); }); }`
- `void loadTemplate(int wave) { const auto values = analyseWaveTable(wave, firstHarmonic()); const auto ids = spectrumIDs(op_, part_, isA_); for (int i = 0; i < ids.size() && i < 64; ++i) setParam(editor_, ids[i], static_cast<float>( std::lround(values[static_cast<std::size_t>(i)]))); bars_.repaint(); }`
- `void refreshLegend() { const int first = firstHarmonic(); const int group = group_.getSelectedItemIndex(); juce::String text(kSpectrumGroupNames[group]); text << " \xc2\xb7 " << kSpectrumMaskNames[mask_.index()]; const auto [winFirst, winLast] = windowFor(selection_.getSelectedItemIndex()); text << " \xc2\xb7 edit " << winFirst << "-" << winLast; int count = 0; for (int i = 0; i < 64; ++i) { const int harmonic = first + i; if (!groupHas(group, harmonic)) continue; ++count; if (count <= 24) text << " " << juce::String(harmonic); } if (count == 0) text << " none in range"; else if (count > 24) text << " " << juce::String(count) << " members"; if (legend_.getText() != text) legend_.setText(text, juce::dontSendNotification); }`

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

- `SpectraOperatorCard(PluginEditor& editor, int op) : editor_(editor), op_(op), part_(editor.activePart()), header_( false, false, {}, [&ed = editor, op] { ed.copyOperator(op); }, [&ed = editor, op] { ed.pasteOperator(op); }, [&ed = editor, op] { return getParam(ed, params::opMute(op, ed.activePart())) > 0.5; }, [&ed = editor, op](bool muted) { setParam(ed, params::opMute(op, ed.activePart()), muted ? 1 : 0); }, true), osc_(editor, op, editor.activePart(), true) { addAndMakeVisible(header_); addAndMakeVisible(scope_); addAndMakeVisible(spectro_); addAndMakeVisible(osc_); }`
- `void resized() override { auto area = getLocalBounds().reduced(8, 0); header_.setBounds(area.removeFromTop(24)); auto visual = area.removeFromTop(30); const int half = (visual.getWidth() - 6) / 2; scope_.setBounds(visual.removeFromLeft(half)); visual.removeFromLeft(6); spectro_.setBounds(visual); area.removeFromTop(3); osc_.setBounds(area.removeFromTop(112)); }`
- `void tick() { editor_.processor().readTapScope( op_ * 2 + part_, scratch_.data(), static_cast<int>(scratch_.size())); scope_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); spectro_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); header_.tick(); repaint(); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); const int algorithm = static_cast<int>( std::lround(getParam(editor_, params::algorithm(part_)))); g.setFont(displayFont(tokens::textOperatorName)); g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink : tokens::white)); g.drawText("op" + juce::String(op_ + 1), 8, 8, 64, 20, juce::Justification::centredLeft, false); }`

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

- `SpectraModColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), osc_(editor, op, part_, true), display_(editor.state(), spectrumIDs(op, part_, true), spectrumIDs(op, part_, false), params::opSpecMorph(op, part_), params::opSpecRangeA(op, part_), params::opSpecRangeB(op, part_), colour(tokens::purple), colour(tokens::pink)), env_(editor, op, part_), lfo_(editor, "morph lfo", params::opSpecLfoRate(op, part_), params::opSpecLfoDepth(op, part_), params::opSpecLfoWave(op, part_), params::opSpecLfoMode(op, part_), params::opSpecLfoRateFade(op, part_), params::opSpecLfoDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::spectrumLfo, [&ed = editor, op] { ensureSpectrumMode(ed, op, ed.activePart(), params::opSpecLfoDepth(op, ed.activePart())); }, true) { addAndMakeVisible(header_); addAndMakeVisible(osc_); addAndMakeVisible(display_); addAndMakeVisible(env_); addAndMakeVisible(lfo_); }`
- `void resized() override { auto area = getLocalBounds().reduced(8); header_.setBounds(area.removeFromTop(24)); area.removeFromTop(10); osc_.setBounds(area.removeFromTop(120)); area.removeFromTop(10); display_.setBounds(area.removeFromTop(130)); area.removeFromTop(10); env_.setBounds(area.removeFromTop(220)); area.removeFromTop(10); lfo_.setBounds(area.removeFromTop(106)); }`
- `void paint(juce::Graphics& g) override { g.fillAll(colour(tokens::bg)); const int algorithm = static_cast<int>( std::lround(getParam(editor_, params::algorithm(part_)))); g.setFont(displayFont(tokens::textOperatorName)); g.setColour(colour(isCarrier(algorithm, op_) ? tokens::pink : tokens::white)); g.drawText("op" + juce::String(op_ + 1), 8, 10, 64, 20, juce::Justification::centredLeft, false); }`
- `void tick() { header_.tick(); display_.setLiveMorph(editor_.processor().specMorph(op_, part_)); display_.tick(); env_.tick(); repaint(); }`

### `OperatorSpectraPage`

`class` at `plugin/juce/PagesSpectral.cpp:898` through line 950.

Fields and enumerators:

- `std::array<std::unique_ptr<SpectraOperatorCard>, 4> opcards_{};`
- `std::array<std::unique_ptr<SpectrumRow>, 4> rows_{};`
- `juce::Component content_;`
- `juce::Viewport viewport_;`

Methods:

- `OperatorSpectraPage(PluginEditor& editor, bool a) { for (int i = 0; i < 4; ++i) { opcards_[static_cast<std::size_t>(i)] = std::make_unique<SpectraOperatorCard>(editor, i); addAndMakeVisible(*opcards_[static_cast<std::size_t>(i)]); rows_[static_cast<std::size_t>(i)] = std::make_unique<SpectrumRow>(editor, i, editor.activePart(), a); content_.addAndMakeVisible(*rows_[static_cast<std::size_t>(i)]); } viewport_.setViewedComponent(&content_, false); viewport_.setScrollBarsShown(false, false); addAndMakeVisible(viewport_); }`
- `void resized() override { auto area = getLocalBounds(); auto top = area.removeFromTop(170); const int width = (top.getWidth() - 30) / 4; for (auto& card : opcards_) { card->setBounds(top.removeFromLeft(width)); top.removeFromLeft(10); } viewport_.setBounds(area); const int contentWidth = juce::jmax(320, viewport_.getWidth()); constexpr int pairHeight = 124; constexpr int pairGap = 4; content_.setSize(contentWidth, 4 * pairHeight + 3 * pairGap + 6); int y = 6; for (auto& row : rows_) { row->setBounds(0, y, contentWidth, pairHeight); y += pairHeight + pairGap; } }`
- `void tick() override { for (auto& card : opcards_) card->tick(); for (auto& row : rows_) row->tick(); }`

### `OperatorSpectraModPage`

`class` at `plugin/juce/PagesSpectral.cpp:952` through line 984.

Fields and enumerators:

- `PluginEditor& editor_;`
- `std::array<std::unique_ptr<SpectraModColumn>, 4> columns_{};`

Methods:

- `explicit OperatorSpectraModPage(PluginEditor& editor) : editor_(editor) { for (int i = 0; i < 4; ++i) { columns_[static_cast<std::size_t>(i)] = std::make_unique<SpectraModColumn>( editor, i, [this](int op) { editor_.copyOperator(op); }, [this](int op) { editor_.pasteOperator(op); }); addAndMakeVisible(*columns_[static_cast<std::size_t>(i)]); } }`
- `void resized() override { auto area = getLocalBounds(); const int width = (area.getWidth() - 30) / 4; for (auto& column : columns_) { column->setBounds(area.removeFromLeft(width)); area.removeFromLeft(10); } }`
- `void tick() override { for (auto& column : columns_) column->tick(); }`

### `PatchButton`

`class` at `plugin/juce/PluginEditor.cpp:17` through line 36.

Methods:

- `PatchButton() : juce::Button("multi-patch library...") { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override { g.setColour(p1::ui::colour(p1::tokens::bg)); g.fillRect(getLocalBounds()); g.setColour(p1::ui::colour(highlighted ? p1::tokens::pink : p1::tokens::lineStrong)); g.drawRect(getLocalBounds(), 1); g.setFont(p1::ui::uiFont(p1::tokens::textButton)); g.setColour(p1::ui::colour(p1::tokens::white)); g.drawText(getButtonText(), getLocalBounds().reduced(8, 0), juce::Justification::centredLeft, true); }`

### `SegmentButton`

`class` at `plugin/juce/PluginEditor.cpp:38` through line 70.

Fields and enumerators:

- `bool active_ = false;`

Methods:

- `explicit SegmentButton(const juce::String& label) : juce::Button(label) { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void setActive(bool active) { if (active_ != active) { active_ = active; repaint(); } }`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override { g.setColour(p1::ui::colour(p1::tokens::bg)); g.fillRect(getLocalBounds()); g.setColour(p1::ui::colour(active_ ? p1::tokens::purple : highlighted ? p1::tokens::pink : p1::tokens::lineStrong)); g.drawRect(getLocalBounds(), 1); g.setFont(p1::ui::uiFont(8.0f)); g.setColour(p1::ui::colour(active_ ? p1::tokens::purple : p1::tokens::dim)); g.drawText(getButtonText(), getLocalBounds(), juce::Justification::centred, false); }`

### `PartChoiceButton`

`class` at `plugin/juce/PluginEditor.cpp:72` through line 104.

Fields and enumerators:

- `bool active_ = false;`

Methods:

- `explicit PartChoiceButton(const juce::String& label) : juce::Button(label) { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void setActive(bool active) { if (active_ != active) { active_ = active; repaint(); } }`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override { g.setColour(p1::ui::colour(p1::tokens::bg)); g.fillRect(getLocalBounds()); g.setColour(p1::ui::colour(active_ ? p1::tokens::green : highlighted ? p1::tokens::pink : p1::tokens::lineStrong)); g.drawRect(getLocalBounds(), 1); g.setFont(p1::ui::uiFont(p1::tokens::textButton)); g.setColour(p1::ui::colour(active_ ? p1::tokens::green : p1::tokens::dim)); g.drawText(getButtonText(), getLocalBounds().reduced(4, 0), juce::Justification::centred, true); }`

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

- `explicit Header(PluginEditor& editor) : editor_(editor), volume_(editor.state(), p1::kVolumeParamID) { alg_.onClick = [this] { editor_.showAlgorithmPicker(); }; alg_.onWheel = [this](int direction) { const int current = static_cast<int>(std::lround(p1::ui::getParam( editor_, p1::params::algorithm(editor_.activePart())))); editor_.applyAlgorithm((current + direction + 10) % 10); }; addAndMakeVisible(alg_); patch_.onClick = [this] { editor_.showPatchBrowser(); }; addAndMakeVisible(patch_); rename_.onClick = [this] { editor_.showRenameDialog(false, 0); }; addAndMakeVisible(rename_); addAndMakeVisible(scopeView_); addAndMakeVisible(spectroView_); scopeSeg_.onClick = [this] { setSpectroMode(false); }; spectroSeg_.onClick = [this] { setSpectroMode(true); }; addAndMakeVisible(scopeSeg_); addAndMakeVisible(spectroSeg_); addAndMakeVisible(volume_); setPatchName(editor_.processor().getPatchName()); setSpectroMode(false); }`
- `void resized() override { alg_.setBounds(300, 15, 330, 130); patch_.setBounds(1000, 54, 150, 35); rename_.setBounds(1156, 54, 64, 35); scopeSeg_.setBounds(876, 22, 44, 18); spectroSeg_.setBounds(920, 22, 56, 18); scopeView_.setBounds(650, 44, 330, 50); spectroView_.setBounds(650, 44, 330, 50); volume_.setBounds(650, 100, 330, 38); }`
- `void paint(juce::Graphics& g) override { drawWordmark(g); g.setFont(p1::ui::uiFont(p1::tokens::textBrandLine)); g.setColour(p1::ui::colour(p1::tokens::dim)); g.drawText(juce::String::fromUTF8( " p1onken // パンク派第１音響合成研究班"), 16, 80, 320, 18, juce::Justification::centredLeft, false); g.setFont(p1::ui::displayFont(p1::tokens::textToolbarLabel)); g.setColour(p1::ui::colour(p1::tokens::dim)); g.drawText("loaded multi-patch:", 998, 18, 236, 14, juce::Justification::centredLeft, false); g.setFont(p1::ui::uiFont(p1::tokens::textButton)); g.setColour(p1::ui::colour(p1::tokens::white)); g.drawText(multiName_, 998, 32, 236, 16, juce::Justification::centredLeft, true); g.setFont(p1::ui::displayFont(p1::tokens::textAlgorithmLabel)); g.setColour(p1::ui::colour(p1::tokens::green)); g.drawText("output", 650, 22, 120, 16, juce::Justification::centredLeft, false); }`
- `void tick() { const int part = editor_.activePart(); const int algorithm = static_cast<int>( std::lround(p1::ui::getParam(editor_, p1::params::algorithm(part)))); alg_.setAlgorithm(algorithm); uint8_t ring = 0; for (int i = 0; i < 3; ++i) { if (std::lround( p1::ui::getParam(editor_, p1::params::ringOn(i, part))) != 0) ring \|= static_cast<uint8_t>(1u << i); } alg_.setRingMask(ring); uint8_t direct = 0; for (int i = 0; i < 4; ++i) { if (std::lround( p1::ui::getParam(editor_, p1::params::carrierOut(i, part))) != 0) direct \|= static_cast<uint8_t>(1u << i); } alg_.setDirectMask(direct); setPatchName(editor_.processor().getPatchName()); editor_.processor().readOutputScope(scratch_.data(), static_cast<int>(scratch_.size())); if (!haveScratch_ \|\| !std::equal(scratch_.begin(), scratch_.end(), lastScratch_.begin())) { haveScratch_ = true; lastScratch_ = scratch_; scopeView_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); spectroView_.setSamples(scratch_.data(), static_cast<int>(scratch_.size())); } }`
- `void setPatchName(const juce::String& name) { if (name == multiName_) return; multiName_ = name; repaint(); }`
- `void setSpectroMode(bool spectro) { spectroMode_ = spectro; scopeView_.setVisible(!spectroMode_); spectroView_.setVisible(spectroMode_); scopeSeg_.setActive(!spectroMode_); spectroSeg_.setActive(spectroMode_); }`
- `void drawWordmark(juce::Graphics& g) { const auto display = p1::ui::displayFont(p1::tokens::textWordmark); const auto brand = p1::ui::brandFont(p1::tokens::textWordmark); const auto ink = p1::ui::colour(p1::tokens::brandInk); const auto shadow = p1::ui::colour(p1::tokens::brandShadow); const auto pink = p1::ui::colour(p1::tokens::pink); const float baseY = 42.0f; const juce::String name = "type P"; const float wordWidth = p1::ui::textWidth(display, name); const float dotWidth = p1::ui::textWidth(display, "."); const float x = 16.0f; auto draw = [&g, baseY](const juce::String& text, const juce::Font& font, juce::Colour colour, float dx, float dy) { g.setFont(font); g.setColour(colour); g.drawText(text, juce::Rectangle<float>( dx, baseY + dy, p1::ui::textWidth(font, text) + 4.0f, 34.0f), juce::Justification::bottomLeft, false); }; draw(name, display, shadow, x + 4.0f, 4.0f); draw(".", display, shadow, x + wordWidth + 4.0f, 4.0f); draw("a", brand, shadow, x + wordWidth + dotWidth + 4.0f, 4.0f); draw(name, display, ink, x, 0.0f); draw(".", display, pink, x + wordWidth, 0.0f); draw("a", brand, ink, x + wordWidth + dotWidth, 0.0f); }`

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

- `explicit Toolbar(PluginEditor& editor) : editor_(editor) { const int part = editor_.activePart(); for (int i = 0; i < 4; ++i) { opTiles_[static_cast<size_t>(i)] = std::make_unique<p1::ui::P1MuteTile>( "op" + juce::String(i + 1), false); opAttach_[static_cast<size_t>(i)] = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>( editor_.state(), p1::params::opMute(i, part), *opTiles_[static_cast<size_t>(i)]); addAndMakeVisible(*opTiles_[static_cast<size_t>(i)]); } partTiles_[0] = std::make_unique<p1::ui::P1MuteTile>( "p1", p1::ui::getParam(editor_, p1::params::level(0)) <= 0.0, [this](bool muted) { editor_.togglePartMute(0, muted); }); partTiles_[1] = std::make_unique<p1::ui::P1MuteTile>( "p2", p1::ui::getParam(editor_, p1::params::level(1)) <= 0.0, [this](bool muted) { editor_.togglePartMute(1, muted); }); addAndMakeVisible(*partTiles_[0]); addAndMakeVisible(*partTiles_[1]); partButtons_[0] = std::make_unique<PartChoiceButton>("part 1: init"); partButtons_[1] = std::make_unique<PartChoiceButton>("part 2: init"); partButtons_[0]->onClick = [this] { editor_.setActivePart(0); }; partButtons_[1]->onClick = [this] { editor_.setActivePart(1); }; addAndMakeVisible(*partButtons_[0]); addAndMakeVisible(*partButtons_[1]); edit_.setButtonText("single part library..."); edit_.onClick = [this] { editor_.showSinglePartsBrowser(); }; addAndMakeVisible(edit_); refreshPartNames(); }`
- `void resized() override { int x = 160; for (auto& tile : opTiles_) { tile->setBounds(x, 11, 36, 22); x += 40; } x += 12; partTiles_[0]->setBounds(x, 11, 36, 22); partTiles_[1]->setBounds(x + 40, 11, 36, 22); const int right = getWidth() - 16; const int actionsWidth = 176; edit_.setBounds(right - actionsWidth, 10, actionsWidth, 24); partButtons_[1]->setBounds(right - actionsWidth - 8 - 108, 10, 108, 24); partButtons_[0]->setBounds(right - actionsWidth - 8 - 108 - 8 - 108, 10, 108, 24); }`
- `void paint(juce::Graphics& g) override { g.fillAll(p1::ui::colour(p1::tokens::bg)); g.setColour(p1::ui::colour(p1::tokens::lineStrong)); g.fillRect(0, 0, getWidth(), 1); g.fillRect(0, getHeight() - 1, getWidth(), 1); drawLegend(g); g.setFont(p1::ui::uiFont(p1::tokens::textToolbarLabel)); g.setColour(p1::ui::colour(p1::tokens::dim)); g.drawText("now editing part:", getWidth() - 582, 0, 150, getHeight(), juce::Justification::centredRight, false); }`
- `void refreshPartNames() { for (int i = 0; i < 2; ++i) { const auto text = "part " + juce::String(i + 1) + ": " + editor_.processor().getPartName(i); if (partButtons_[static_cast<size_t>(i)]->getButtonText() != text) partButtons_[static_cast<size_t>(i)]->setButtonText(text); } }`
- `void tick() { refreshPartNames(); for (int i = 0; i < 2; ++i) { const bool muted = p1::ui::getParam(editor_, p1::params::level(i)) <= 0.0; if (partTiles_[static_cast<size_t>(i)]->getToggleState() != muted) { partTiles_[static_cast<size_t>(i)]->setToggleState( muted, juce::dontSendNotification); partTiles_[static_cast<size_t>(i)]->repaint(); } partButtons_[static_cast<size_t>(i)]->setActive( editor_.activePart() == i); } }`
- `void drawLegend(juce::Graphics& g) { g.setColour(p1::ui::colour(p1::tokens::bg)); g.fillRect(16, 14, 15, 15); g.setColour(p1::ui::colour(p1::tokens::pink)); g.drawRect(16, 14, 15, 15, 1); g.setFont(p1::ui::uiFont(11.0f)); g.drawText(juce::String::fromUTF8("\xe7\x8c\xab"), 16, 14, 15, 15, juce::Justification::centred, false); g.setColour(p1::ui::colour(p1::tokens::dim)); g.drawText("muted", 36, 14, 50, 15, juce::Justification::centredLeft, false); g.setColour(p1::ui::colour(p1::tokens::pink)); g.fillRect(90, 14, 15, 15); g.setColour(p1::ui::colour(p1::tokens::bg)); g.drawText(juce::String::fromUTF8("\xe7\x8c\xab"), 90, 14, 15, 15, juce::Justification::centred, false); g.setColour(p1::ui::colour(p1::tokens::dim)); g.drawText("active", 110, 14, 50, 15, juce::Justification::centredLeft, false); }`

### `ListBody`

`class` at `plugin/juce/PluginEditor.cpp:400` through line 460.

Fields and enumerators:

- `virtual void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) = 0;`
- `int count_ = 0;`
- `int selected_ = 0;`
- `juce::ListBox list_;`

Methods:

- `ListBody(int count, int rowHeight, int selected, const juce::String& name) : count_(juce::jmax(0, count)), selected_(juce::jlimit(0, juce::jmax(0, count_ - 1), selected)), list_(name, this) { addAndMakeVisible(list_); list_.setRowHeight(rowHeight); list_.setColour(juce::ListBox::backgroundColourId, colour(tokens::popup)); list_.setColour(juce::ListBox::outlineColourId, juce::Colours::transparentBlack); list_.setColour(juce::ListBox::textColourId, colour(tokens::text)); list_.selectRow(selected_, true, true); }`
- `int selected() const { return selected_; }`
- `void resized() override { list_.setBounds(getLocalBounds()); list_.scrollToEnsureRowIsOnscreen(selected_); }`
- `int getNumRows() override { return count_; }`
- `void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override { const auto area = juce::Rectangle<int>(0, 0, width, height); if (selected) { g.setColour(colour(tokens::purple).withAlpha(0.08f)); g.fillRect(area); g.setColour(colour(tokens::purple)); g.drawRect(area, 1); } else { g.setColour(colour(tokens::popup)); g.fillRect(area); } g.setColour(colour(tokens::line)); g.fillRect(8, height - 1, width - 16, 1); paintRow(g, row, area, selected); }`
- `void listBoxItemClicked(int row, const juce::MouseEvent&) override { selected_ = juce::jlimit(0, juce::jmax(0, count_ - 1), row); list_.selectRow(selected_, true, true); repaint(); }`

### `SlotListBody`

`class` at `plugin/juce/PluginEditor.cpp:462` through line 494.

Fields and enumerators:

- `std::function<juce::String(int)> nameFor_;`
- `std::function<bool(int)> usedFor_;`

Methods:

- `SlotListBody(int count, std::function<juce::String(int)> nameFor, std::function<bool(int)> usedFor, int selected) : ListBody(count, 24, selected, "slots"), nameFor_(std::move(nameFor)), usedFor_(std::move(usedFor)) { }`
- `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override { const bool used = usedFor_ && usedFor_(row); g.setFont(uiFont(tokens::textSlotRow)); g.setColour(colour(selected ? tokens::purple : used ? tokens::dim : tokens::faint)); g.drawText(juce::String(row + 1).paddedLeft('0', 2), area.reduced(8, 0), juce::Justification::centredLeft, false); juce::String name = used && nameFor_ ? nameFor_(row) : "empty"; if (name.isEmpty()) name = "empty"; g.setColour(colour(selected ? tokens::purple : used ? tokens::text : tokens::faint)); g.drawText(name, area.withTrimmedLeft(42).withTrimmedRight(8), juce::Justification::centredLeft, true); }`

### `ModeListBody`

`class` at `plugin/juce/PluginEditor.cpp:496` through line 516.

Fields and enumerators:

- `juce::StringArray modes_;`

Methods:

- `ModeListBody(juce::StringArray modes, int selected) : ListBody(modes.size(), 28, selected, "modes"), modes_(std::move(modes)) { }`
- `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override { g.setFont(uiFont(tokens::textSlotRow)); g.setColour(colour(selected ? tokens::purple : tokens::text)); g.drawText(modes_[row], area.reduced(8, 0), juce::Justification::centredLeft, true); }`

### `AlgTile`

`class` at `plugin/juce/PluginEditor.cpp:518` through line 562.

Fields and enumerators:

- `int index_ = 0;`
- `bool selected_ = false;`
- `std::function<void(int)> onSelect_;`

Methods:

- `AlgTile(int index, std::function<void(int)> onSelect) : juce::Button("alg"), index_(index), onSelect_(std::move(onSelect)) { setMouseCursor(juce::MouseCursor::PointingHandCursor); }`
- `void setSelected(bool selected) { if (selected_ != selected) { selected_ = selected; repaint(); } }`
- `void paintButton(juce::Graphics& g, bool highlighted, bool) override { g.setColour(p1::ui::colour(p1::tokens::bg)); g.fillRect(getLocalBounds()); g.setColour(p1::ui::colour(selected_ ? p1::tokens::green : highlighted ? p1::tokens::pink : p1::tokens::lineStrong)); g.drawRect(getLocalBounds(), 1); auto area = getLocalBounds().reduced(6, 4); auto caption = area.removeFromBottom(18); p1::ui::drawAlgorithmGraph(g, area.toFloat(), index_); g.setFont(p1::ui::uiFont(11.0f)); g.setColour(p1::ui::colour(selected_ ? p1::tokens::green : p1::tokens::dim)); g.drawText("alg " + juce::String(index_), caption, juce::Justification::centred, false); }`
- `void mouseUp(const juce::MouseEvent&) override { if (onSelect_) onSelect_(index_); }`

### `AlgPickerBody`

`class` at `plugin/juce/PluginEditor.cpp:564` through line 610.

Fields and enumerators:

- `int selected_ = 4;`
- `std::array<std::unique_ptr<AlgTile>, 10> tiles_{};`
- `juce::Label note_;`

Methods:

- `explicit AlgPickerBody(int selected) : selected_(juce::jlimit(0, 9, selected)) { for (int i = 0; i < 10; ++i) { tiles_[static_cast<size_t>(i)] = std::make_unique<AlgTile>( i, [this](int index) { select(index); }); tiles_[static_cast<size_t>(i)]->setSelected(i == selected_); addAndMakeVisible(*tiles_[static_cast<size_t>(i)]); } note_.setText("pink operators are carriers.", juce::dontSendNotification); note_.setFont(p1::ui::uiFont(13.0f)); note_.setColour(juce::Label::textColourId, p1::ui::colour(p1::tokens::dim)); addAndMakeVisible(note_); }`
- `int selected() const { return selected_; }`
- `void resized() override { auto area = getLocalBounds(); auto grid = area.removeFromTop(3 * 140 + 16); const int width = (grid.getWidth() - 24) / 4; for (int i = 0; i < 10; ++i) { const int column = i % 4; const int row = i / 4; tiles_[static_cast<size_t>(i)]->setBounds( column * (width + 8), row * 148, width, 140); } note_.setBounds(area.removeFromTop(22)); }`
- `void select(int index) { selected_ = juce::jlimit(0, 9, index); for (int i = 0; i < 10; ++i) tiles_[static_cast<size_t>(i)]->setSelected(i == selected_); }`

### `RenameBody`

`class` at `plugin/juce/PluginEditor.cpp:612` through line 645.

Fields and enumerators:

- `juce::TextEditor editor_;`
- `juce::Label note_;`

Methods:

- `explicit RenameBody(const juce::String& initial) { editor_.setText(initial, false); editor_.setFont(p1::ui::uiFont(14.0f)); editor_.setColour(juce::TextEditor::textColourId, p1::ui::colour(p1::tokens::green)); editor_.setInputRestrictions( 32, "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 _-."); editor_.onReturnKey = [] {}; addAndMakeVisible(editor_); note_.setText("max 32 characters.", juce::dontSendNotification); note_.setFont(p1::ui::uiFont(12.0f)); note_.setColour(juce::Label::textColourId, p1::ui::colour(p1::tokens::dim)); addAndMakeVisible(note_); }`
- `juce::String text() const { return editor_.getText().trim(); }`
- `void focus() { editor_.grabKeyboardFocus(); }`
- `void resized() override { auto area = getLocalBounds(); editor_.setBounds(area.removeFromTop(30)); area.removeFromTop(6); note_.setBounds(area.removeFromTop(18)); }`

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
- `enum class OperatorSection { oscillator, amplitudeEnvelope, feedbackEnvelope, amplitudeLfo, feedbackLfo, detuneLfo, phase, phaseLfo, phaseEnvelope, spectrumA, spectrumB, spectrumEnvelope, spectrumLfo, };`
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
- `void setSpectroMode(bool spectro);`
- `void fitToHostWindow();`
- `std::vector<int32_t> currentPartParams(int part);`
- `void applyPartParams(int part, const std::vector<int32_t>& params);`
- `int currentRouting(int field, int part);`
- `void applyRouting(int field, int part, int value);`
- `std::vector<int32_t> currentMacroParams();`
- `void applyMacroParams(const std::vector<int32_t>& params);`
- `TypePAlphaAudioProcessor& processor_;`
- `p1::ui::P1LookAndFeel lookAndFeel_;`
- `p1::ui::P1Canvas canvas_;`
- `std::unique_ptr<Header> header_;`
- `std::unique_ptr<Toolbar> toolbar_;`
- `p1::ui::P1TabBar tabs_;`
- `std::unique_ptr<p1::ui::PageBase> page_;`
- `std::unique_ptr<p1::ui::P1Dialog> dialog_;`
- `std::unique_ptr<juce::FileChooser> fileChooser_;`
- `int activePart_ = 0;`
- `int activeTab_ = 0;`
- `bool spectroMode_ = false;`
- `bool switchingTab_ = false;`
- `bool startupFitPending_ = true;`
- `int startupFitTicks_ = 4;`
- `int startupFitAttempts_ = 90;`
- `std::array<int, 2> mutedLevel_{99, 99};`
- `std::map<juce::String, double> partClipboard_;`
- `std::map<juce::String, double> operatorClipboard_;`
- `int operatorClipboardOp_ = -1;`
- `std::vector<double> operatorSectionClipboard_;`
- `int operatorSectionClipboardOp_ = -1;`
- `OperatorSection operatorSectionClipboardKind_ = OperatorSection::amplitudeEnvelope;`
- `bool operatorSectionClipboardValid_ = false;`

Methods:

- `TypePAlphaAudioProcessor& processor() { return processor_; }`
- `juce::AudioProcessorValueTreeState& state() { return processor_.parameters(); }`
- `int activePart() const { return activePart_; }`
- `int activeTab() const { return activeTab_; }`

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

- `~LockRelease() { lock.exit(); }`

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
- `std::array<std::atomic<float>*, 6> macroValueValues_{};`
- `std::array<std::atomic<float>*, 6> macroMidiValues_{};`
- `std::array<std::atomic<float>*, 6> macroMidiRangeValues_{};`
- `std::array<std::atomic<float>*, 18> macroTargetValues_{};`
- `std::array<std::atomic<float>*, 18> macroRangeValues_{};`
- `std::array<std::atomic<int>, 6> macroCc_{};`
- `std::atomic<int> modWheel_{0};`
- `std::atomic<int> noteVelocity_{0};`
- `std::array<std::atomic<float>*, 2> algorithmValues_{};`
- `std::vector<int32_t> macroMemberIds_;`
- `std::vector<juce::RangedAudioParameter*> macroMemberParams_;`
- `std::vector<uint8_t> macroMemberLog_;`
- `std::vector<int> macroMemberIndex_;`
- `std::vector<double> macroOffsets_;`
- `bool macroApplied_ = false;`
- `std::atomic<float>* outputVolume_ = nullptr;`
- `float volumeGain_ = 1.0f;`
- `std::vector<int32_t> directBuffer_;`
- `std::vector<float> monoFold_;`
- `std::atomic<bool> paramsDirty_{false};`
- `double hostRate_ = 48000.0;`
- `std::vector<ScheduledEvent> scheduled_;`
- `std::size_t nextScheduled_ = 0;`
- `p1::ChunkNoteSet chunkNotes_{};`
- `std::vector<juce::MidiMessage> deferredOffs_;`
- `uint64_t coreFrames_ = 0;`
- `std::array<std::atomic<int>, 8> liveMorph_{};`
- `std::array<std::array<p1::PhaseMapSnapshot, 8>, 3> phaseMapSnapshots_{};`
- `mutable std::atomic<uint32_t> phaseMapMiddle_{1};`
- `uint32_t phaseMapBack_ = 0;`
- `mutable uint32_t phaseMapFront_ = 2;`
- `ScopeRing outputScope_;`
- `std::array<ScopeRing, 10> tapScopes_;`
- `juce::String patchName_{"init patch"};`
- `std::array<juce::String, 2> partNames_{{"init", "init"}};`
- `std::array<juce::String, 6> macroNames_{};`
- `int uiActivePart_ = 0;`
- `int uiPage_ = 0;`
- `bool libraryLoaded_ = false;`
- `juce::File libraryFileOverride_;`
- `p1::PresetStore presets_{kP1PartParamCount, kP1PartCount, p1::partParamDefaults(), kP1PartCutoffIndex};`

Methods:

- `~TypePAlphaAudioProcessor() override = default;`
- `bool hasEditor() const override { return true; }`
- `const juce::String getName() const override { return P1_PRODUCT_NAME; }`
- `bool acceptsMidi() const override { return true; }`
- `bool producesMidi() const override { return false; }`
- `bool isMidiEffect() const override { return false; }`
- `double getTailLengthSeconds() const override { return 8.0; }`
- `int getNumPrograms() override { return 1; }`
- `int getCurrentProgram() override { return 0; }`
- `void setCurrentProgram(int) override {}`
- `const juce::String getProgramName(int) override { return {}; }`
- `void changeProgramName(int, const juce::String&) override {}`
- `void setLibraryFileForTesting(const juce::File& file) { libraryFileOverride_ = file; }`
- `juce::AudioProcessorValueTreeState& parameters() { return apvts_; }`
- `p1::PresetStore& presets() { return presets_; }`
- `const p1::PresetStore& presets() const { return presets_; }`
- `juce::String getPatchName() const { return patchName_; }`
- `void setPatchName(const juce::String& name) { patchName_ = name; }`
- `int getUiActivePart() const { return uiActivePart_; }`
- `void setUiActivePart(int part) { uiActivePart_ = juce::jlimit(0, 1, part); }`
- `int getUiPage() const { return uiPage_; }`
- `void setUiPage(int page) { uiPage_ = juce::jlimit(0, 8, page); }`

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
- `struct Multi { bool used = false; std::string name; std::vector<uint8_t> partSlot; std::vector<int32_t> level; std::vector<int32_t> delay; std::vector<int32_t> channel; std::vector<int32_t> low; std::vector<int32_t> high; /* The macro block, in manifest order. Empty means a record from before the * block, so the caller applies the macro defaults. */ std::vector<int32_t> macro; std::array<std::string, kMacroCount> macroNames; };`
- `std::size_t partRecordSize() const;`
- `std::size_t multiRecordSize() const;`
- `bool savePart(int slot, const std::string& name, const std::vector<int32_t>& params);`
- `bool loadPart(int slot, std::string& name, std::vector<int32_t>& params) const;`
- `bool partUsed(int slot) const;`
- `std::string partName(int slot) const;`
- `const std::vector<int32_t>& partParams(int slot) const;`
- `bool saveMulti(int slot, const std::string& name, const std::vector<uint8_t>& partSlot, const std::vector<int32_t>& level, const std::vector<int32_t>& delay, const std::vector<int32_t>& channel, const std::vector<int32_t>& low, const std::vector<int32_t>& high, const std::vector<int32_t>& macro = {}, const std::array<std::string, kMacroCount>& macroNames = {});`
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
- `int cutoffIndex_ = -1;`
- `int partParamCount_ = 0;`
- `int partCount_ = 0;`
- `std::vector<int32_t> defaults_;`
- `std::vector<Part> parts_;`
- `std::vector<Multi> multis_;`
- `int currentMulti_ = -1;`
- `std::vector<int> partSource_;`
- `uint32_t revision_ = 0;`
- `std::array<bool, kPartSlots> dirtyParts_{};`
- `std::array<bool, kMultiSlots> dirtyMultis_{};`
- `std::array<Part, kPartSlots> baseParts_{};`
- `std::array<Multi, kMultiSlots> baseMultis_{};`
- `std::array<int, 4> opEnvWarm_{{ -1, -1, -1, -1 }};`
- `int panEnvBase_ = -1;`
- `std::array<int, 4> fbEnvBase_{{ -1, -1, -1, -1 }};`
- `int pitchEnvBase_ = -1;`
- `int filterEnvBase_ = -1;`
- `std::array<int, 4> specEnvBase_{{ -1, -1, -1, -1 }};`
- `std::array<int, 34> fadeBases_{};`
- `int algorithmIndex_ = -1;`
- `std::array<int, 4> waveIndices_{{ -1, -1, -1, -1 }};`

Methods:

- `int partParamCount() const { return partParamCount_; }`
- `int partCount() const { return partCount_; }`
- `uint32_t revision() const { return revision_; }`
- `void setRevision(uint32_t revision) { revision_ = revision; }`
- `void setOperatorEnvelopeIndices(const std::array<int, 4>& indices) { opEnvWarm_ = indices; }`
- `void setPanEnvelopeIndex(int index) { panEnvBase_ = index; }`
- `void setOffsetEnvelopeIndices(const std::array<int, 4>& feedback, int pitch) { fbEnvBase_ = feedback; pitchEnvBase_ = pitch; }`
- `void setAbsEnvelopeIndices(int filter, const std::array<int, 4>& spectral) { filterEnvBase_ = filter; specEnvBase_ = spectral; }`
- `void setLfoFadeIndices(const std::array<int, 34>& indices) { fadeBases_ = indices; std::sort(fadeBases_.begin(), fadeBases_.end()); }`
- `void setInsertionIndices(int algorithm, const std::array<int, 4>& waves) { algorithmIndex_ = algorithm; waveIndices_ = waves; }`
- `int currentMulti() const { return currentMulti_; }`

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

- `explicit Fft(int n) : n_(n), rev_(static_cast<std::size_t>(n)), tw_(static_cast<std::size_t>(n)) { int bits = 0; while ((1 << bits) < n) ++bits; for (int i = 0; i < n; ++i) { int r = 0; for (int b = 0; b < bits; ++b) if (i & (1 << b)) r \|= 1 << (bits - 1 - b); rev_[static_cast<std::size_t>(i)] = r; tw_[static_cast<std::size_t>(i)] = std::polar(1.0, -2.0 * kPi * static_cast<double>(i) / n); } }`
- `void run(std::vector<std::complex<double>>& a) const { for (int i = 0; i < n_; ++i) { const int j = rev_[static_cast<std::size_t>(i)]; if (j > i) std::swap(a[static_cast<std::size_t>(i)], a[static_cast<std::size_t>(j)]); } for (int len = 2; len <= n_; len <<= 1) { const int half = len >> 1; const int step = n_ / len; for (int i = 0; i < n_; i += len) { for (int j = 0; j < half; ++j) { const std::complex<double> w = tw_[static_cast<std::size_t>(j * step)]; const std::complex<double> u = a[static_cast<std::size_t>(i + j)]; const std::complex<double> v = a[static_cast<std::size_t>(i + j + half)] * w; a[static_cast<std::size_t>(i + j)] = u + v; a[static_cast<std::size_t>(i + j + half)] = u - v; } } } }`

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
- `void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override;`
- `void drawScrollbar(juce::Graphics& g, juce::ScrollBar& bar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override;`

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
- `class KnobSlider : public juce::Slider { public: bool keyPressed(const juce::KeyPress& key) override { if (onDetentKey && onDetentKey(key)) return true; return juce::Slider::keyPressed(key); } void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override; std::function<bool(const juce::KeyPress&)> onDetentKey; std::function<void()> onUserEdit; std::function<void()> onUserEditEnd; };`
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

- `int macroParamIndex() const override { return specIndex_; }`

### `KnobSlider`

`class` at `plugin/ui/Widgets.h:83` through line 98.

Arrow keys step a detented ratio knob to the next detent instead of nudging the raw value, which would snap straight back to where it was.

Fields and enumerators:

- `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override;`
- `std::function<bool(const juce::KeyPress&)> onDetentKey;`
- `std::function<void()> onUserEdit;`
- `std::function<void()> onUserEditEnd;`

Methods:

- `bool keyPressed(const juce::KeyPress& key) override { if (onDetentKey && onDetentKey(key)) return true; return juce::Slider::keyPressed(key); }`

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
- `class Tab : public juce::Button { public: Tab(const juce::String& text, int index, P1TabBar& owner); void paintButton(juce::Graphics& g, bool highlighted, bool down) override; private: int index_; P1TabBar& owner_; };`
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

- `void setLiveMorph(double value) { liveMorph_ = value; }`

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

- `void refresh() { repaint(); }`

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

- `int macroParamIndex() const override { return specIndex_; }`

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

