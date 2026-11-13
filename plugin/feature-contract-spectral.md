# Feature contract: spectral oscillator and spectrum wave

## Identity

- **Feature:** A per-operator additive oscillator with two sixteen-harmonic spectra,
  a morph, a morph envelope and a morph LFO, reached through the `spectrum` entry in
  the wave list, with two editor pages and a starting spectrum analysed from the
  current wave.
- **Brief section:** [`../design/spectral.md`](../design/spectral.md),
  [`../design/waveforms.md`](../design/waveforms.md),
  [`../design/README.md`](../design/README.md) and
  [`../design/juce.md`](../design/juce.md).
- **Owner / date:** P1Onken, 2026-11-13.
- **Disposition:** accepted, in progress. The work is staged: the wave table and
  `rect tri` first, then the engine bank and the spectrum mode, then the editor
  pages and the snapshot pass.

## Player outcome

- **New player action:** open `spectrum a` or `spectrum b` on the operator spectra
  page, draw sixteen harmonic levels, use the `odd`, `even`, `tilt` and `formant`
  macros, and choose `spectrum` in the wave select. The `op spectra mod` page carries
  the spectrum display, the morph, the morph envelope and the morph LFO.
- **Sound and behaviour:** the wave select is the mode. Values 0 to 12 read the wave
  table, and value 13 (`spectrum`) reads the additive bank. The mode is saved.
  Because the wave select is the mode, choosing `spectrum` after an edit restores the
  stored pair, choosing any other wave keeps the pair for the next selection, and the
  analysed preview is written only when a spectral control changes, so the player can
  move between the wave table and the additive bank without losing either sound. The
  operator morph blends spectrum a and b, and the envelope and LFO move the blend per
  voice. While the waveform is not `spectrum`, the spectral pages show a preview of
  the selected wave's sixteen harmonic levels. The first edit or morph move commits
  that preview, writes the levels and then the waveform as 13. Selecting `spectrum`
  without an edit uses the stored a and b spectra. The mode switch crossfades over
  10 ms.
- **Failure mode:** the additive table clips, the morph leaks between voices, the
  preview writes the patch on open, the wave values shift again, a stored spectrum
  is overwritten by a mode change, or the existing waves change their samples.

## Dependencies and interactions

- **Other features:** the `fm_op` macro and its `osc` node, the `lfo` and
  `pitch_env` shapes, the table generator, the preset store, the editor pages, the
  analysis table and the editor snapshot set.
- **Platform/API/HAL changes:** none. The engine stays integer C, and the plugin
  stays at 44.1 kHz with output-only rate adaptation.
- **Persistence/version impact:** `rect tri` is inserted at waveform 9, so pulse,
  square and saw move to 10, 11 and 12, and `spectrum` is 13. `PRESET_VERSION`
  moves to 3 and the old test banks are regenerated. The project has no release, so
  no migration path is provided. The values are stable. The part block grows from
  228 to 408 parameters, the manifest from 470 to 830, and each part record by 720
  bytes. The plugin's `uiPage` needs the `part fx/mod` remap.
- **MIDI/clock impact:** none. The new parameters are host-automatable and stay out
  of the default CC map. The spectrum mode is the waveform parameter, so a host can
  automate the mode.

## Parameter and event semantics

The spectral parameters are per operator and per part, at block rate. The existing
waveform parameter grows from 0 to 11 to 0 to 13.

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `opN_waveform` | `opN.osc.waveform` | 0..13 | 0 | `wave` |
| `opN_spec_a1` ... `opN_spec_a16` | `opN.osc.spec_a1` ... `a16` | 0..99 | 99, then 0 | raw |
| `opN_spec_b1` ... `opN_spec_b16` | `opN.osc.spec_b1` ... `b16` | 0..99 | formant at h5 | raw |
| `opN_spec_morph` | `opN.osc.spec_morph` | 0..99 | 0 | raw |
| `opN_spec_env_r1` ... `r4` | `opN.specenv.r1` ... `r4` | 0..99 | 80, 60, 50, 80 | raw |
| `opN_spec_env_l1` ... `l3` | `opN.specenv.l1` ... `l3` | 0..99 | 99, 60, 0 | raw |
| `opN_spec_env_depth` | `opN.osc.spec_env_depth` | -99..99 | 0 | raw |
| `opN_spec_lfo_rate` | `opN.speclfo.rate` | 0..20 Hz | 4.0 Hz | `hz` |
| `opN_spec_lfo_depth` | `opN.speclfo.depth` | -99..99 | 0 | raw |
| `opN_spec_lfo_wave` | `opN.speclfo.wave` | six waves | sine | `wave` |
| `opN_spec_lfo_mode` | `opN.speclfo.mode` | 0..1 | 1 | `onoff` |

The morph is:

```c
m = clamp(morph / 99 + (envDepth / 99) * (env / 99)
          + (lfoDepth / 99) * lfo, 0, 1);
sample = ((1 - m) * tableA + m * tableB) >> 15;
```

The envelope and LFO run per voice. The a and b tables are per part. The wave
values are the mode: 13 reads the additive bank, and 0 to 12 read the wave table.

The editor commits the analysis preview by writing the sixteen a levels, then the
waveform value 13. A first edit on b or the morph commits the a preview too.
Selecting a wave keeps the stored spectra. The preview is not written on page open.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** two table reads and one linear blend per
  spectral oscillator sample. The table generation runs at the 344 Hz control tick.
  The analysis preview is a table lookup in the editor, not a runtime DFT.
- **Instance memory, CPU, preset size:** the spectral bank is 20,480 bytes per part
  plus a 20,480-byte shadow per part, so 81,920 bytes for both parts. The part block
  grows by 180 parameters, 720 bytes per part. The measured costs are 1.7% of one
  core for 16 tables at 344 Hz without mips, 3.4% with five mips, and 2.46% for the
  full 20-voice graph with the linear read. The 13-wave table is 37,888 samples,
  75,776 bytes. The bank is bounded.
- **Overflow/overload policy:** the generated table is peak normalised to Q15, so
  the blend of two normalised tables stays in range. The morph is clamped. A dirty
  operator regenerates on the control tick and crossfades, so a level change cannot
  interrupt the read. The table update and the read run on the same core, so the
  344 Hz tick has to fit inside the block budget.
- **Division, shift, and range analysis:** no per-sample division. The blend uses a
  Q15 multiply and shift. The table index is the existing 10-bit phase and mip
  layout.

## Files and interfaces

- **Engine:** `tools/gen_wavetable.py`, `tools/audit_modules.py`,
  `engine/gen/wavetable.h`, `engine/gen/wave_harmonics.h`, `engine/hal/spectral.h`,
  `synth/patch.json`, `modules/nodes/osc/node.json` and `node.c`,
  `modules/macros/fm_op/macro.json` (local generator modules), `engine/gen/*` and
  `engine/runtime/presets.c`.
- **Editor:** `plugin/ui/ParamIDs.h`, `plugin/ui/Widgets.cpp`,
  `plugin/juce/Pages.h`, `plugin/juce/PagesOperators.cpp`,
  `plugin/juce/PluginEditor.cpp`, `plugin/juce/PluginEditor.h`,
  `plugin/juce/CMakeLists.txt`, and new spectrum components.
- **Tests:** `plugin/tests/wavetable.cpp`, `plugin/tests/spectral.cpp`,
  `engine_parity`, `plugin_smoke`, `plugin_stability`, `preset_smoke` and
  `editor_snapshot`.
- **Design:** `design/spectral.md`, `design/waveforms.md`, `design/README.md`,
  `design/juce.md` and `design/mockup.html`.
- **Public contract:** `kP1ParamSpecs`, the generated `p1_wave_base` and
  `p1_wave_harmonics` arrays, and the `fm_op`/spectral manifest.

## Verification

- **Host vector(s):** build the VST3 and standalone on Windows, run `ctest`, load
  the VST3 in two DAWs and run pluginval. The local `tools/audit_modules.py` check
  confirms that every Type P type and node parameter resolves to the `.dev` module
  library, and the Windows `ctest` run is 12 of 12.
- **Browser vector:** the mockup at `#opspec` and `#opspecmod`, with the `from wave`
  preview and the `spectrum` wave.
- **Target package check:** the Linux build and the pluginval pass.
- **Host timing and platform check:** the 20-voice benchmark and a real Linux and
  Windows DAW pass.
- **Tolerance/metric:** the existing waves 0 to 8 and the shifted pulse, square and
  saw must match the current header sample for sample. The checks are exact. The
  spectra page must render the analysis preview without writing a parameter. The
  part block and manifest counts must match 408 and 830.
- **Evidence artefact:** a wave table checksum test, a spectral table and morph
  test, an editor page snapshot, a `ctest` log, a pluginval log and the measured
  table/CPU figures. The golden images are regenerated in the Linux pass.

### Staged evidence

1. **Wave list and rect tri:** in for waveform 0 to 12. The two headers, the `osc`
   node limits, the waveform range, the wave names, the preset version and the
   wavetable test are in. The Windows `ctest` run is 11 of 11, and the table test
   confirms the current waves byte for byte.
2. **Spectral engine:** in. The 180 parameters, the bank, the spectrum mode 13,
   the morph, the envelope and the LFO are in. The 256-sample table reads with a
   24-bit phase fraction; the first pass used the 1024-sample shift and played the
   table four times too high. The Windows `ctest` run is 12 of 12.
3. **Editor pages:** the two tabs, the bar editor, the macros, the preview and the
   copy and paste. The snapshots and the page index remap are in this stage.
4. **Release evidence:** the Linux goldens, the DAW pass and the pluginval report.

## Risks and fallback

- **Assumptions/open research:** the spectrum starting analysis is a sixteen
  harmonic truncation, so a saw or square is darker than its full-band table. The
  half and rectified shapes lose DC, and the transform drops phase.
- **Safe experiment and stop criterion:** the wave table and the analysis table are
  verified against the current header before the engine changes. If the table
  comparison fails, the header swap stops and the old 12-wave header stays.
- **Smaller alternative:** ship the wave list with rect tri and no spectral oscillator
  if the table bank cannot meet the CPU or memory bound, which keeps every stored
  waveform honest, leaves the two pages out of the release, and avoids shipping a
  mode that would play the default table when the additive bank is not there. No
  silent fallback.
- **Rollback:** restore `engine/gen/wavetable.h`, `synth/patch.json`, the `osc` node
  and `PRESET_VERSION`, then regenerate. The stored wave values return to the 0..11
  layout. The spectral parameters and pages are removed as one change.
