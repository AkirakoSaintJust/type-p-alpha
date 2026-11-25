# File index

Every tracked source and build file, with its line count and role.

| File | Lines | Language | Role |
| --- | ---: | --- | --- |
| `CMakeLists.txt` | 32 | CMake | project source |
| `docker/build.ps1` | 36 | PowerShell | Linux container build |
| `docker/build.sh` | 37 | shell | Linux container build |
| `engine/gen/cc_map.h` | 721 | C/C++ header | The fixed cc -> param map (roadmap §6.11.4): one CC per exposed param, ascending from CC_BASE, skipping the reserved CCs |
| `engine/gen/dsp.h` | 2158 | C/C++ header | generated state struct, parameter table, slot table and engine API |
| `engine/gen/math_tables.h` | 217 | C/C++ header | generated fixed-point tables for the 48 kHz core |
| `engine/gen/naive_wave.h` | 915 | C/C++ header | The classic oscillator shapes, Q15 |
| `engine/gen/tables.c` | 4 | C | generated algorithm routing weights |
| `engine/gen/tables.h` | 6 | C/C++ header | generated routing table declaration |
| `engine/gen/type_p_alpha_dsp.c` | 14865 | C | generated parameter table, init table, note allocation and render code |
| `engine/gen/wave_harmonics.h` | 25 | C/C++ header | generated harmonic magnitude table for the 14 waves |
| `engine/gen/wavetable.h` | 2964 | C/C++ header | generated 14-waveform oscillator union, additive bank included |
| `engine/hal/p1_math.h` | 365 | C/C++ header | fixed-point helpers, level gains, rate tables and wavetable mip selection |
| `engine/hal/spectral.h` | 293 | C/C++ header | additive spectral bank generator, morph reader and crossfade |
| `engine/runtime/macro_apply.h` | 187 | C/C++ header | per-voice macro offsets for the velocity source |
| `engine/runtime/presets.c` | 261 | C | preset record layout, CRC and bank pack functions |
| `engine/runtime/presets.h` | 74 | C/C++ header | preset record declarations and sizes |
| `engine/runtime/synth_midi.c` | 60 | C | MIDI handler with sustain, channel routing and panic |
| `engine/runtime/synth_midi.h` | 39 | C/C++ header | MIDI handler interface, context and callback table |
| `manual/build.py` | 145 | Python | manual pipeline |
| `manual/figures.py` | 45 | Python | manual pipeline |
| `plugin/CMakeLists.txt` | 296 | CMake | project source |
| `plugin/engine/Engine.cpp` | 251 | C++ | engine interface implementation, parameter writes and phase-map snapshot copy |
| `plugin/engine/Engine.hpp` | 67 | C++ header | C++ interface over the generated state, phase-map snapshot and render call |
| `plugin/engine/MidiBridge.cpp` | 77 | C++ | MIDI bridge implementation and the engine callback table |
| `plugin/engine/MidiBridge.hpp` | 26 | C++ header | MIDI bridge interface between the host queue and the engine |
| `plugin/engine/PhaseMapLayout.h` | 95 | C/C++ header | phase-map pool layout shared by the engine snapshot and tests |
| `plugin/engine/RateBridge.cpp` | 165 | C++ | resampler kernel, ring, pull path and latency |
| `plugin/engine/RateBridge.hpp` | 55 | C++ header | output-only 128-tap polyphase resampler from 48 kHz to the host rate |
| `plugin/juce/ChunkNoteSet.h` | 37 | C/C++ header | notes started in the current control chunk, keyed by channel and pitch |
| `plugin/juce/CMakeLists.txt` | 208 | CMake | JUCE host, editor, pages and processor |
| `plugin/juce/MacroLaw.h` | 61 | C/C++ header | macro source and destination laws and the target table view |
| `plugin/juce/Pages.cpp` | 275 | C++ | patch settings page, part cards and voice mode |
| `plugin/juce/Pages.h` | 428 | C/C++ header | shared page base, card frame and page factory declarations |
| `plugin/juce/PagesFxMod.cpp` | 848 | C++ | part filter, envelopes, follow and ring-modulation cards |
| `plugin/juce/PagesMacros.cpp` | 341 | C++ | macro cards, target picker and range controls |
| `plugin/juce/PagesOperators.cpp` | 283 | C++ | operator oscillator, envelope and LFO columns |
| `plugin/juce/PagesPhase.cpp` | 559 | C++ | phase-map page, transfer-curve view and phase controls |
| `plugin/juce/PagesSpectral.cpp` | 998 | C++ | spectral a/b editors, spectral modulation page and displays |
| `plugin/juce/PluginEditor.cpp` | 1810 | C++ | JUCE editor, header, toolbar, dialogs and page switching |
| `plugin/juce/PluginEditor.h` | 147 | C/C++ header | JUCE editor declaration and page state |
| `plugin/juce/PluginProcessor.cpp` | 1166 | C++ | JUCE processor, block path, MIDI scheduling, parameter sync and library save |
| `plugin/juce/PluginProcessor.h` | 193 | C/C++ header | JUCE processor declaration, scope rings and snapshot buffers |
| `plugin/juce/VolumeLaw.h` | 60 | C/C++ header | master volume fader law and dB text |
| `plugin/preset/CutoffLaw.h` | 55 | C/C++ header | cutoff position law and the old-to-new conversion |
| `plugin/preset/part_defaults.h` | 21 | C/C++ header | factory defaults for one part |
| `plugin/preset/presets_host.c` | 38 | C | host stubs for the preset record layer |
| `plugin/preset/PresetStore.cpp` | 1138 | C++ | library store, record migrations, merge and file I/O |
| `plugin/preset/PresetStore.hpp` | 225 | C++ header | in-memory part and multi library with migrations and merge |
| `plugin/tests/additive_bench.cpp` | 666 | C++ | Headless additive table experiment |
| `plugin/tests/additive_engine_bench.cpp` | 155 | C++ | Headless additive engine prototype: the real voice graph with the additive oscillator and the per-voice envelope morph |
| `plugin/tests/editor_snapshot.cpp` | 933 | C++ | Milestone 3: render the designed editor to PNG for visual review |
| `plugin/tests/engine_parity.cpp` | 491 | C++ | Milestone 1: the engine render reference and the state, filter and MIDI isolation checks |
| `plugin/tests/engine_render_reference.h` | 98 | C/C++ header | The captured Type P.alpha engine render at 48 kHz: note on at 60, velocity 100, then the release |
| `plugin/tests/feedback_bipolar.cpp` | 230 | C++ | Bipolar feedback: the positive half is the current path, the negative half is the square family with the same average, and the envelope level moves the signed feedback level toward or across zero |
| `plugin/tests/feedback_envelope.cpp` | 256 | C++ | Feedback envelope: each operator's internal fbenv node adds a signed offset to that operator's signed feedback level |
| `plugin/tests/filter_envelope.cpp` | 110 | C++ | Filter envelope rate response: the slow settings must stay slow |
| `plugin/tests/filter_law.cpp` | 86 | C++ | The filter's cutoff law, resonance onset and input soft knee |
| `plugin/tests/lfo_fades.cpp` | 451 | C++ | engine, host, preset, UI and audio tests |
| `plugin/tests/linux_daws.sh` | 411 | shell | engine, host, preset, UI and audio tests |
| `plugin/tests/operator_lfos.cpp` | 139 | C++ | Operator LFOs: the feedback LFO adds to the feedback level and the detune LFO adds to the operator pitch |
| `plugin/tests/phase_map.cpp` | 946 | C++ | engine, host, preset, UI and audio tests |
| `plugin/tests/phase_map_perceptual.cpp` | 853 | C++ | engine, host, preset, UI and audio tests |
| `plugin/tests/phase_map_perceptual_compare.cpp` | 910 | C++ | engine, host, preset, UI and audio tests |
| `plugin/tests/pitch_envelope.cpp` | 178 | C++ | Pitch envelope: the offset stages, the sustain and release, the four modes, and the zero start that keeps the pitch at the note before the first stage |
| `plugin/tests/plugin_smoke.cpp` | 1104 | C++ | Milestone 2 to 6: the JUCE processor smoke, state and parameter test, which needs no DAW |
| `plugin/tests/plugin_stability.cpp` | 256 | C++ | Stability and edge-case sweep for the JUCE processor, which needs no DAW |
| `plugin/tests/preset_smoke.cpp` | 1008 | C++ | engine, host, preset, UI and audio tests |
| `plugin/tests/rate_bridge_test.cpp` | 192 | C++ | Milestone 5: measure the 128-tap polyphase rate bridge |
| `plugin/tests/spectral.cpp` | 384 | C++ | The additive path: the spectrum waveform, the a/b morph, the morph envelope and the morph LFO |
| `plugin/tests/spectral_bench.cpp` | 265 | C++ | Measures the additive table regeneration and the control tick |
| `plugin/tests/ui_frame_bench.cpp` | 186 | C++ | engine, host, preset, UI and audio tests |
| `plugin/tests/voice_alloc.cpp` | 155 | C++ | Voice allocation: a held chord or scale takes one voice per note, a release takes only its own voice, and the same sequence repeats after the voices have been recycled |
| `plugin/tests/wavetable.cpp` | 129 | C++ | Checks the generated 14-wave table and the analysis table |
| `plugin/tools/additive_prototype.py` | 174 | Python | parameter, macro, token and snapshot tools |
| `plugin/tools/feedback_figures.py` | 124 | Python | parameter, macro, token and snapshot tools |
| `plugin/tools/gen_macro_targets.py` | 431 | Python | parameter, macro, token and snapshot tools |
| `plugin/tools/gen_params.py` | 250 | Python | parameter, macro, token and snapshot tools |
| `plugin/tools/gen_tokens.py` | 94 | Python | parameter, macro, token and snapshot tools |
| `plugin/tools/ratio.py` | 50 | Python | parameter, macro, token and snapshot tools |
| `plugin/tools/snapshot_check.py` | 207 | Python | parameter, macro, token and snapshot tools |
| `plugin/ui/ParamIDs.h` | 432 | C/C++ header | generated parameter id helpers |
| `plugin/ui/SpectrumAnalysis.cpp` | 44 | C++ | wavetable harmonic analysis for the spectral editor |
| `plugin/ui/SpectrumAnalysis.h` | 13 | C/C++ header | wavetable harmonic analysis interface |
| `plugin/ui/Theme.cpp` | 248 | C++ | font loading, palette and the panel look and feel |
| `plugin/ui/Theme.h` | 60 | C/C++ header | font, colour and look-and-feel declarations |
| `plugin/ui/Widgets.cpp` | 2350 | C++ | panel widget painting, parameter binding and edit gestures |
| `plugin/ui/Widgets.h` | 451 | C/C++ header | panel widget declarations: cards, knobs, toggles, bars and scopes |
| `tools/audit_modules.py` | 135 | Python | wavetable and maths table generators |
| `tools/gen_code_docs.py` | 1568 | Python | wavetable and maths table generators |
| `tools/gen_math_tables.py` | 172 | Python | wavetable and maths table generators |
| `tools/gen_wavetable.py` | 241 | Python | wavetable and maths table generators |
| `windows/bench-isa.ps1` | 86 | PowerShell | Windows build and benchmark scripts |
| `windows/build.ps1` | 91 | PowerShell | Windows build and benchmark scripts |
