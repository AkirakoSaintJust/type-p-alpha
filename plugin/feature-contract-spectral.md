# Feature contract: spectral oscillator and spectrum wave

## Identity

- **Feature:** A per-operator additive oscillator with two 64-harmonic spectra,
  a morph, a morph envelope and a morph LFO, reached through the `spectrum` entry in
  the wave list, with three editor pages and a starting spectrum analysed from the
  current wave.
- **Brief section:** [`../design/spectral.md`](../design/spectral.md),
  [`../design/waveforms.md`](../design/waveforms.md),
  [`../design/README.md`](../design/README.md) and
  [`../design/juce.md`](../design/juce.md).
- **Owner / date:** P1Onken, 2026-11-13.
- **Disposition:** complete for the engine and the seven-tab editor pages. Each row
  loads a wave analysis through its `load spectrum template...` button, the wave
  select is the only mode switch, and a morph depth moved away from zero also
  selects `spectrum`. The cold commit and the crossfade direction are in, with the
  schedule bench in `plugin/tests/spectral_bench.cpp`. The Linux snapshot goldens
  were regenerated for the 64-bar bands, the new header and toolbar text, the
  envelope graph and the live morph cursor.

## Player outcome

- **New player action:** open `op spectra a` or `op spectra b`, load a wave with
  `load spectrum template...`, draw 64 harmonic levels, use the group, mask and
  spring controls, and choose `spectrum` in the wave select. The `op spectra mod`
  page carries the spectrum display, the morph, the morph envelope and the morph
  LFO.
- **Sound and behaviour:** the wave select is the mode. Values 0 to 12 read the wave
  table, and value 13 (`spectrum`) reads the additive bank. The mode is saved.
  Because the wave select is the mode, choosing `spectrum` after an edit restores the
  stored pair, choosing any other wave keeps the pair for the next selection, and a
  template load writes only the chosen row, so the player can move between the wave
  table and the additive bank without losing either sound. The operator morph blends
  spectrum a and b, and the envelope and LFO move the blend per voice. Each spectrum
  carries its own 64-harmonic range, so it addresses 1 to 64 or 65 to 128. The mod
  display zooms to the union of the two spans when a and b differ. A template load
  does not change the waveform. The wave select, a spectrum paste, or a morph depth
  moved away from zero selects `spectrum`. The mode switch crossfades over 10 ms. A
  silent part commits a new spectrum at once, so its next note starts on the new
  table. An audible part ramps from the old table to the new one. The mod display
  reads the morph the newest rendering voice wrote, so its waveform and cursor
  follow the envelope and the LFO without jumping between chord voices.
- **Failure mode:** the additive table clips, the morph leaks between voices, a
  template load changes the waveform, the wave values shift again, a stored
  spectrum is overwritten by a mode change, or the existing waves change their
  samples.

## Dependencies and interactions

- **Other features:** the `fm_op` macro and its `osc` node, the `lfo` and
  `pitch_env` shapes, the table generator, the preset store, the editor pages, the
  analysis table and the editor snapshot set.
- **Platform/API/HAL changes:** none. The engine stays integer C, the core stays at
  48 kHz, and the plugin adapts the rate at the output.
- **Persistence/version impact:** `rect tri` is inserted at waveform 9, so pulse,
  square and saw move to 10, 11 and 12, and `spectrum` is 13. `PRESET_VERSION`
  moves to 3 for this feature, to 4 for the 48 kHz core change, to 5 for the
  harmonic range change, and to 6 for the 64-harmonic change, and the old test
  banks are regenerated. The plugin state version moves to 4. The project has no
  release, so no migration path is provided. The values are stable. The part block
  grows from 416 to 800 parameters, the manifest from 846 to 1,614, and each part
  record by 1,536 bytes. The plugin's `uiPage` needs the `part fx/mod` remap and
the split remap for the mod page, keyed on `uiPageLayout` 3.
- **MIDI/clock impact:** none. The new parameters are host-automatable and stay out
  of the default CC map. The spectrum mode is the waveform parameter, so a host can
  automate the mode.

## Parameter and event semantics

The spectral parameters are per operator and per part, at block rate. The existing
waveform parameter grows from 0 to 11 to 0 to 13.

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `opN_waveform` | `opN.osc.waveform` | 0..13 | 0 | `wave` |
| `opN_spec_range_a` | `opN.osc.spec_range_a` | 0..1 | 0 | `range` |
| `opN_spec_range_b` | `opN.osc.spec_range_b` | 0..1 | 0 | `range` |
| `opN_spec_a1` ... `opN_spec_a64` | `opN.osc.spec_a1` ... `a64` | 0..99 | 99, then 0 | raw |
| `opN_spec_b1` ... `opN_spec_b64` | `opN.osc.spec_b1` ... `b64` | 0..99 | formant at h5 | raw |
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

The envelope and LFO run per voice. The a and b tables are per part, and each
spectrum has its own 64-harmonic range. The wave values are the mode: 13 reads the
additive bank, and 0 to 12 read the wave table.

The editor writes a template into the chosen row only, 64 parameters at a time.
Selecting a wave keeps the stored spectra, and the editor does not write the
waveform when a template loads. A morph envelope or morph LFO depth edit that ends
away from zero writes the waveform as 13, so the modulation has something to move.

## Bounds and resources

- **Maximum voices/events/effects:** 24 voices and two parts.
- **Per-sample and per-block operations:** two table reads and one linear blend per
  spectral oscillator sample. The table generation runs on the control tick.
  The template analysis is a table lookup in the editor, not a runtime DFT.
- **Instance memory, CPU, preset size:** the spectral bank is 10,880 signed
  16-bit samples per operator, so 21,760 bytes per operator and 87,040 bytes for
  four operators. Each part reads its two tables and the shadow at the selected mip,
  so a voice touches 3 x 256 samples for the operator. The part block grows by 384
  spectral parameters, 1,536 bytes per part, and reaches 800 parameters. The
  13-wave table is 37,888 samples, 75,776 bytes. The bank is bounded.
- **Table update schedule:** `spectral_bench` measures about 60 µs to generate one
  spectrum (seven mips, 127 harmonic sums per sample) on the Windows clang-cl build.
  A block that changes all eight spectra costs about 482 µs of generation, and about
  850 µs with the copies and the voice work. A 512-sample table is about 120 µs per
  spectrum and 959 µs for eight. The control block is 667 µs, so the 256-sample
  generation fits but the whole block needs a 128 frame callback or larger. A voice
  reads 1.5 KB of tables and a generation writes 3.5 KB of shadow plus 3.5 KB of
  target. The edit-to-audio path and the 10 ms crossfade are unchanged.
- **Overflow/overload policy:** the generated table is peak normalised to Q15, so
  the blend of two normalised tables stays in range. The morph is clamped. A dirty
  operator regenerates on the control tick. An audible part crossfades from the old
  table to the new one over about 15 control ticks (256 units at 17 per tick, 480
  samples, 10 ms), then copies the shadow into place. A part that has not been read since the last tick commits the new table at
  once, without the crossfade, so a cold operator's next note starts on the new
  spectrum. The table update and the read run on the same core, so the table update
  tick has to fit inside the block budget.
- **Division, shift, and range analysis:** no per-sample division. The blend uses a
  Q15 multiply and shift. The table index is the existing 10-bit phase and mip
  layout.

## Files and interfaces

- **Engine:** `tools/gen_wavetable.py`, `tools/audit_modules.py`,
  `engine/gen/wavetable.h`, `engine/gen/wave_harmonics.h`, `engine/hal/spectral.h`,
  `synth/patch.json`, `modules/nodes/osc/node.json` and `node.c`,
  `modules/macros/fm_op/macro.json` (local generator modules), `engine/gen/*` and
  `engine/runtime/presets.c`.
- **Editor:** `plugin/ui/ParamIDs.h`, `plugin/ui/Widgets.h`,
  `plugin/ui/Widgets.cpp`, `plugin/ui/SpectrumAnalysis.cpp`,
  `plugin/ui/SpectrumAnalysis.h`, `plugin/juce/Pages.h`,
  `plugin/juce/PagesOperators.cpp`, `plugin/juce/PagesSpectral.cpp`,
  `plugin/juce/PluginEditor.cpp`, `plugin/juce/PluginEditor.h`,
  `plugin/juce/PluginProcessor.cpp`, `plugin/juce/PluginProcessor.h` and
  `plugin/juce/CMakeLists.txt`.
- **Tests:** `plugin/tests/wavetable.cpp`, `plugin/tests/spectral.cpp`,
  `plugin/tests/spectral_bench.cpp`, `engine_parity`, `plugin_smoke`,
  `plugin_stability`, `preset_smoke` and `editor_snapshot`.
- **Design:** `design/spectral.md`, `design/waveforms.md`, `design/README.md`,
  `design/juce.md` and `design/mockup.html`.
- **Public contract:** `kP1ParamSpecs`, the generated `p1_wave_base` and
  `p1_wave_harmonics` arrays, and the `fm_op`/spectral manifest.

## Verification

- **Host vector(s):** build the VST3, CLAP and standalone on Windows and run
  `ctest`. The local `tools/audit_modules.py` check confirms that every Type P type
  and node parameter resolves to the `.dev` module library, and the Windows `ctest`
  run is 12 of 12. The Linux CI build and ctest also pass on commit `98a1cc7`.
  pluginval and the Linux DAW matrix remain for the platform pass.
- **Browser vector:** the seven-tab panel in `web/panel.html` is the design
  reference, and the mockup is the first-pass prototype. The editor snapshot covers
  the two spectrum pages and the mod page.
- **Target package check:** the Windows VST3, CLAP and standalone build. The Linux
  build and the pluginval pass remain.
- **Host timing and platform check:** the 20-voice benchmark and a real Linux and
  Windows DAW pass remain.
- **Tolerance/metric:** the existing waves 0 to 8 and the shifted pulse, square and
  saw must match the current header sample for sample. The checks are exact. A
  template load must write the 64 levels of the chosen row and leave the waveform
  alone, and the mod display must show both outlines, the morph marker and the
  waveform trace. `spectral` also checks that the engine publishes the live morph
  for the newest voice, and that the base morph stays still with zero envelope
  depth. The part block and
  manifest counts must match 800 and 1,614.
- **Evidence artefact:** a wave table checksum test, a spectral table and morph
  test, a cold-commit and crossfade-direction test, the schedule bench, an editor
  page snapshot, a `ctest` log, a pluginval log and the measured table/CPU figures.
  The golden images are regenerated in the Linux pass.

### Staged evidence

1. **Wave list and rect tri:** in for waveform 0 to 12. The two headers, the `osc`
   node limits, the waveform range, the wave names, the preset version and the
   wavetable test are in. The Windows `ctest` run is 11 of 11, and the table test
   confirms the current waves byte for byte.
2. **Spectral engine:** in. The 64 levels per spectrum and the two range controls
   per operator, the bank, the spectrum mode 13, the morph, the envelope and the LFO
   are in. The 256-sample table reads with a
   24-bit phase fraction, and the first pass used the 1024-sample shift and played
   the table four times too high. The Windows `ctest` run is 12 of 12.
   The cold commit and the crossfade direction are in: a silent part commits at
   once, and an audible part ramps from the old table to the new one over 256 ticks.
   `spectral_bench` measures about 60 µs per generated spectrum and about 850 µs
   for the block that changes all eight spectra.
3. **Editor pages:** in. The two spectrum tabs (`op spectra a` and `op spectra b`),
   the seven-tab bar, the 64-bar editor, the group and mask selectors, the
   select-group legend, the spring-loaded tilt, level and formant controls, the
   `load spectrum template...` menu on each row, the morph depth mode switch, the
   copy and paste, and the page index remap. The A and B tabs each carry four
   bands of a wide row and its control card, and the four bands fit without a
   scroll. The shaped spectrum snapshot is
   `editor-operator-spectra-levels.png`, the B page snapshot is
   `editor-operator-spectra-b.png`, and the mod display snapshot is
   `editor-op-spectra-mod-levels.png`. The Linux goldens were regenerated for the
   header and toolbar text, the 6 px band, the envelope graph and the 64-bar layout.
4. **Library:** in. The part record carries the 64 levels of both spectra, and
   the `.p1lib` library packs the part bank and the multi bank in one file. A multi
   save materialises any part that has no slot, and an import rejects a used multi
   whose part reference is empty or out of range. `preset_smoke` checks the spectrum
   level round-trip and the rejection.
5. **Release evidence:** the Linux goldens, the DAW pass and the pluginval report.

## Risks and fallback

- **Range and aliasing:** the bank shifts the 64 partials by the range offset,
  but the read still chooses its mip from the note step. A high range at a high note
  can fold because the top partial passes Nyquist. The intended use is a high range
  on low notes. A range-aware harmonic limit in the read is the next refinement.
- **Top range edge:** the bank table is 256 samples, so harmonic 128 sits exactly at
  that table's Nyquist and comes out silent. The rest of the 65-128 range is
  representable. A 512-sample bank would close the last bar.
- **Assumptions/open research:** the spectrum starting analysis is a 64
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
