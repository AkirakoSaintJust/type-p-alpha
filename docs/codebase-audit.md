# Codebase map

This is the map I use when I come back to the tree after a few weeks away, usually
because a test has gone red or because I want to add a parameter without re-reading
the whole engine. It says which parts are hand-written and which are generated,
where the main seams sit, and what each test is for. The feature contracts carry the
dated evidence, so this page stays a map rather than a second history, and I'd
rather keep it short enough that I actually read it.

The engine is integer C and runs at 48 kHz. The plugin, the standalone app and the
web build all wrap the same generated state, which is the whole reason the parity
tests matter: if a wrapper and the core disagree, I want to know before a release
rather than after a bug report. The FM-1 tree stays in its archive tag and is out of
scope, so anything that only makes sense for the old device belongs in that history.

## Tree

| Path | Role | Source |
| --- | --- | --- |
| `engine/hal/` | integer maths, wave tables and the filter coefficient | hand-written |
| `engine/runtime/` | preset records and the MIDI parser | hand-written |
| `engine/gen/` | generated DSP, parameters, tables and wave data | generated |
| `plugin/engine/` | C++ engine seam, MIDI bridge and rate bridge | hand-written |
| `plugin/preset/` | preset bank, defaults and the host record stubs | hand-written |
| `plugin/ui/` | theme, widgets and parameter id helpers | hand-written |
| `plugin/juce/` | processor, editor, pages and dialogs | hand-written |
| `plugin/tests/` | parity, DSP, preset, host and snapshot tests | hand-written |
| `plugin/tools/` | parameter, token, ratio, figure and snapshot tools | hand-written |
| `design/` | tokens, design notes and the clickable mockup | hand-written |
| `synth/` | patch and plugin descriptions | data |

## Engine HAL

`engine/hal/p1_math.h` is the fixed-point layer, and it is the first file I open
when a coefficient looks wrong. `P1_MATH_INLINE` is the always-inline attribute, and
Q15 unity is 32768 by convention. The filter helpers are new enough that they earn a
line here: `p1_soft_clip` is the input's fifth-order knee, and
`p1_filter_coeff_q16` turns the log cutoff position into the per-stage coefficient.
The rest of the table is the arithmetic I keep reaching for, and none of it is worth
a second implementation in a node.

| Symbol | Job |
| --- | --- |
| `p1_q15_mul`, `p1_q15_mul32`, `p1_q16_mul`, `p1_q31_mul` | Q15, Q16 and Q31 multiplies. |
| `p1_soft_clip` | The filter input's soft knee. |
| `p1_filter_coeff_q16` | The VCF coefficient from the cutoff position. |
| `p1_shr_round` | Arithmetic shift with round-to-nearest. |
| `p1_sat_add32`, `p1_sat_sub32` | Saturating add and subtract. |
| `p1_madd_sat` | Saturating multiply-add with a shift. |
| `p1_onepole` | One-pole smoothing step. |
| `p1_sin`, `p1_cos`, `p1_sin_wave`, `p1_wave_mip` | Sine, cosine, waveform lookup and mip choice. |
| `p1_key_scale`, `p1_key_apply`, `p1_vel_scale`, `p1_key_add` | Key and velocity follow. |
| `p1_clamp`, `p1_clamp16`, `p1_clamp16_max` | Range clamps. |
| `p1_route4` | Four-way operator routing. |
| `p1_tune_q16` | Octave, semitone and cent to a Q16 ratio. |
| `p1_poly_blep` | PolyBLEP correction for the saw and pulse waves. |

## Engine runtime

`engine/runtime/presets.h` and `presets.c` own the record layout, and the format is
deliberately boring because it has to survive a decade of saves. A record is a
version byte, a 33-byte name field, the body and a CRC-32, and an empty slot is
`0xFF`. The current version is 17, the part record holds 991 values, the multi
record carries the macro block, and the library magic is `P1LIB3`. A part or patch
name holds 32 characters. If you change the layout, bump the version and write the
migration in the same commit, because the CRC will not save you from a mismatched
body. The loader also keys the older part layouts on the record size, because the
phase and ringmod blocks were appended before the version byte moved.

The part bank has 128 slots and the multi bank has 64. The plugin host uses the pack
and unpack functions and keeps the banks in `PresetStore`. The bank and settings
helpers stay in the shared runtime because the plugin links the same source and
splitting it would fork the engine, and a fork is how two record readers drift
apart.

`engine/runtime/synth_midi.c` turns a raw MIDI stream into the engine seam. The
sustain state sits in the parser context, so two engines can share one parser
without borrowing each other's pedal. The plugin feeds the context directly and the
`midi_get` stub remains for the shared runtime.

## Generated engine

| Path | Generated from | Contents |
| --- | --- | --- |
| `engine/gen/type_p_alpha_dsp.c` | `synth/patch.json` and the modules | state, parameter application, render, taps and node graph |
| `engine/gen/dsp.h` | same | state struct and public engine functions |
| `engine/gen/wavetable.h`, `naive_wave.h` | waveform data | mip-mapped and naive wave tables |
| `engine/gen/math_tables.h` | generator | sine, envelope, rate and filter tables |
| `engine/gen/cc_map.h` | generator | CC to parameter map |
| `engine/gen/tables.c`, `tables.h` | generator | table objects |
| `engine/gen/params.json` | generator | manifest of every parameter |

`python .dev/generate.py` writes these from the module graph, and I do not edit them
by hand. When a generated file and a module source disagree, the module source is
the one to fix, because the next generation will overwrite the other. The feature
contracts are the evidence for what changed and why.

## Plugin engine

`p1::Engine` in `plugin/engine/Engine.hpp` and `Engine.cpp` puts one generated state
behind a small C++ seam with no JUCE dependency. It owns reset, parameter access,
note on and off, pitch bend, render, the lit LED base, the quiet check and the taps.
The seam is intentionally thin, because every host wrapper should be able to drive
the same engine without learning the generated state.

`p1::MidiBridge` connects the C parser to the C++ engine. The free `p1_bridge_*`
functions are the callbacks, and a thread-local `current` pointer joins the
context-free C callback to the C++ object. The bridge is fed on one thread, so that
pointer is safe, and I'd keep it that way rather than adding a lock to a path that
runs on every MIDI event.

`p1::RateBridge` resamples the 48 kHz core to the host rate. At 48 kHz the processor
bypasses it, so the core stays exact. The kernel is 128 taps by 1024 phases, and
`latencySamples` reports its delay in host samples. The bypass is the part to test
first, because a resampler that is almost right is worse than one that is obviously
wrong.

## Preset store

`plugin/preset/PresetStore.hpp` and `PresetStore.cpp` own the 128 part slots and the
64 multi slots. A part slot stores the sound and its name. A multi stores the part
references and the routing. The session pointers are `currentMulti` and the per-part
source slot, and the per-part source is what lets a patch point at a stored part
without copying it.

The store packs and loads the two banks, writes and reads `P1LIB3`, finds free slots
and truncates names to 32 characters. `plugin/preset/part_defaults.h` builds the
default vector from `kP1ParamSpecs`, so a new parameter gets a default in the store
without a second list. The store is also where the cutoff migration runs when an old
part bank comes in, because that is the seam that knows about old records.

## UI layer

`plugin/ui/Theme.h` and `Theme.cpp` own the fonts, colours and look and feels. The
three faces are DotGothic16 for the interface, Zen Dots for display and Unbounded for
the brand. `P1LookAndFeel` covers the combo, menu and scrollbar, and
`KnobLookAndFeel` draws the rotary knob. The theme is the one place where a colour
change should happen, so a raw colour in a page is a bug rather than a style choice.

`plugin/ui/Widgets.h` and `Widgets.cpp` hold the shared controls and the bridge
between the APVTS integers and the panel readouts. `formatParamValue` and
`parseParamValue` are the pair that matters, because the host text, the panel values
and the typed values all pass through them. A one-way formatter is easy, and the
round trip is where the CLAP validator finds you, so the pair belongs together in the
same file.

The main widgets are `ParamKnob`, `P1Button`, `P1MuteTile`, `P1Toggle`, `P1Choice`,
`ParamChoice`, `P1TabBar`, `P1Scope`, `P1Spectro`, `P1SpectralBars`,
`P1SpectrumDisplay`, `P1Envelope`, `P1Wave`, `P1LevelSlider`, `P1Card`,
`P1AlgorithmBlock` and `P1Dialog`. The spectral widgets are worth naming separately:
`P1SpectralBars` carries the 64-harmonic editor, the selection window and the live or
dead mask, and `P1SpectrumDisplay` carries the two outlines, the faint phase-zero
waveform and the live morph cursor. Those two are the ones I would read before
changing the spectra pages.

## Pages

The tab bar has nine pages: patch settings, macros, part params, op envs, op lfos,
op phasemap, op spectra a, op spectra b and op spectra mod. The processor writes
`uiPageLayout` 3 with the page and remaps the four-tab order and the old mod page
position when it restores an older state, so an old session lands on the page it
meant rather than the page with the same index.

`plugin/juce/PagesSpectral.cpp` holds the three spectra pages. Each spectrum page has
four operator and oscillator cards, a 6 px gap, then four spectrum cards. A card
header carries the range, mask, selection, group, template, norm and copy controls,
with the 64-bar editor on the left and the springs on the right. The mod page shows
the two spectra, the faint phase-zero waveform and the live morph cursor, followed by
the morph envelope and the morph LFO. The layout is tight on purpose, because the
spectrum pair and the operator pair have to be visible at once.

## Parameter ids

`plugin/ui/ParamIDs.h` maps the panel vocabulary to APVTS ids. The functions are
mechanical, so the file is the readable parameter map rather than a table I want to
duplicate here. The families cover the part, the operator oscillator and envelopes,
the amplitude, feedback and detune LFOs, the spectra and their morph envelope and
LFO, the pitch LFO and envelope, the filter and its envelope, the pan envelope, and
the key and
velocity follow targets. A macro would hide typos and gain very little, so I keep the
one-line functions and let the compiler catch the misspelt id.

## Tests

| File | Job |
| --- | --- |
| `engine_parity.cpp` | Compare the generated engine with the independent reference and check the filter output gate. |
| `filter_law.cpp` | Check the cutoff table, the resonance onset, the input knee and the SH-101 passband. |
| `filter_envelope.cpp` | Check the filter envelope rate resolution from the slowest setting to instant. |
| `feedback_envelope.cpp` | Check the per-operator envelope family, the flat path, the partial negative level and the peak bound. |
| `feedback_bipolar.cpp` | Check the positive path, the rectified negative half, the signed envelope level and the peak bound. |
| `operator_lfos.cpp` | Check the feedback and detune LFOs at depth 0, above 0 and with a negative depth. |
| `pitch_envelope.cpp` | Check the pitch envelope rates, levels and release. |
| `lfo_fades.cpp` | Check the LFO rate and depth fades and the shared time scale. |
| `voice_alloc.cpp` | Check the chord, scale, part channel and voice recycling cases. |
| `wavetable.cpp` | Check the waveform bank, the mip selection and the spectral morph read. |
| `spectral.cpp` | Check the additive spectrum, morph, morph envelope and morph LFO. |
| `spectral_bench.cpp` | Measure the spectral table generation and the engine block cost. |
| `rate_bridge_test.cpp` | Check the resampler latency and spectral behaviour. |
| `preset_smoke.cpp` | Check the current record, the 32-character name, the `P1LIB3` library and the old part and multi records, including the version 17 to 18 key-follow append. |
| `plugin_smoke.cpp` | Check state restore, the global library, automation, the 48 kHz bridge and a chord. |
| `plugin_stability.cpp` | Stress every parameter, polyphony, MIDI storms and truncated state. |
| `editor_snapshot.cpp` | Render the pages, dialogs, long name, section paste and half-size frame. |
| `phase_map.cpp` | Check the phase map, the per-part key/velocity follow and the exact post-F table. |
| `phase_map_perceptual.cpp` | Render the pre-seeded perceptual battery. |
| `phase_map_perceptual_compare.cpp` | Compare two perceptual dumps with the spectral, band, harmonic, alias and envelope gate. |
| `ui_frame_bench.cpp` | Measure the live scope, spectrogram, phase page and spectra page. |
| `engine_render_reference.h` | The independent render used by `engine_parity`. |

`additive_bench.cpp` and `additive_engine_bench.cpp` are audition tools, not ctest
gates.

## Tools

| Tool | Job |
| --- | --- |
| `gen_params.py` | Generate `Params.h` from `params.json` and the ratio detents. |
| `gen_macro_targets.py` | Generate `MacroTargets.h` and the macro target id map from the manifest. |
| `ratio.py` | Compute the shared ratio detents. |
| `gen_tokens.py` | Generate `UiTokens.h` from `tokens.json`. |
| `snapshot_check.py` | Compare or accept the editor snapshot goldens. |
| `feedback_figures.py` | Render the feedback WAVs into scope PNGs. |
| `additive_prototype.py` | Generate the build-tree additive engine copy and tables. |

## Design reference

`design/mockup.html` is a clickable reference, not the product. It carries the data,
controls, views, library and dialogs in one file, and the CSS mirrors
`design/tokens.css` rather than duplicating the logic. The shipped panel is the
JUCE editor, so where the mockup and the editor differ, the editor and
`design/spectral.md` carry the current layout.

## Current evidence

The feature contracts describe the current features. Each one carries the current
semantics, the measured evidence and a short research-progress note for the layers
it replaced. The research pages in `docs/research/` carry the measurements of the
discarded experiments. If a contract and this page disagree, the code and the
current contract win, and the research page keeps the old number as history. The
current manifest is 2,053 parameters with a 994-value part block and preset
version 18.

## History

The 2026-10 cleanup removed the FM-1 migration maps, the old 146, 160 and 179 part
layouts, the 16-byte name reader, the `P1LIB1` loader and the duplicated list and
card header machinery. The tree then had one part layout, one multi layout and one
`P1LIB2` magic; the library magic is `P1LIB3` now, and a pre-`P1LIB2` library is not
read by the plugin. The removed
symbols and the old tests are in git history, which is the right place for a list
that no longer describes the current tree. I wrote the removed table once, read it
once, and then stopped needing it.
