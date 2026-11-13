# Feature contract: 48 kHz native core

## Identity

- **Feature:** Move the engine core from 44.1 kHz to 48 kHz and regenerate every
  rate-derived table and constant.
- **Brief section:** README line of research 7, the rate bridge, and the audio
  section of [`../design/juce.md`](../design/juce.md).
- **Owner / date:** P1Onken, 2026-11-13.
- **Disposition:** complete. The Windows CTest gate passes. The editor panel is
  unchanged, so the Linux golden set keeps its current images.

## Player outcome

- **New player action:** none. Existing patches keep their pitch, envelope times,
  LFO rates and part delays, and their filter cutoffs stay close across the range.
- **Sound and behaviour:** the core renders at 48 kHz. A 48 kHz host takes the core
  output untouched. A 44.1, 96 or 192 kHz host is resampled at the output with the
  128-tap polyphase bridge. The old 44.1 kHz rule is withdrawn, so the engine has
  one musical base.
- **Failure mode:** a pitch offset, an envelope or LFO that changes speed, a filter
  cutoff that moves, a part delay that drifts, a resampler that images, or a 48 kHz
  render that is not bit-exact with the core.

## Dependencies and interactions

- **Other features:** the control-rate math tables, the note increment table, the
  portamento step, the part delay counter, the voice release hold, the liquid filter
  cutoff coefficient, the DC blocker, the spectral bank crossfade, the rate bridge,
  the plugin processor, the LFO Hz display, the preset version and the engine parity
  reference.
- **Platform/API/HAL changes:** none. The engine stays integer C. The rate
  adaptation stays at the output boundary.
- **Persistence/version impact:** `PRESET_VERSION` moves from 3 to 4 and the plugin
  state version from 2 to 3. The project has not released, so no migration is
  written and older test banks are regenerated. The manifest stays at 830
  parameters, the part block at 408 and the spectral block at 45 parameters per
  operator.
- **MIDI/clock impact:** none. The 4 Hz LFO defaults keep their displayed value.

## Parameter and event semantics

The parameters and ranges do not change. The raw defaults that carry a rate are
rescaled so the displayed value stays the same:

| Parameter group | Old raw | New raw | Display |
| --- | --- | --- | --- |
| amplitude and spectral LFO rate | 389566 | 357914 | 4.0 Hz |
| pitch LFO rate | 486958 | 447392 | 4.0 Hz |
| DC blocker `r` | 32735 | 32738 | none |
| part delay scale | 44 samples per ms | 48 samples per ms | milliseconds |

The LFO display and its inverse use 48000 in `plugin/ui/Widgets.cpp`.

The envelope rate coefficients are stored per 32-sample control block. The 44.1 kHz
table is the musical base, and each coefficient `a` is transformed to the 48 kHz
block period with `1 - (1 - a)^(44100/48000)`, which preserves the one-pole time
constant in seconds. The control block is 32 samples at both rates, so the block
period falls from 725.6 to 666.7 microseconds and the coefficient shrinks
accordingly. Examples: rate 0 goes from 7 to 6, rate 50 from 102 to 94, and rate 98
from 26008 to 24350. The third-rate table follows the same transform, so rate 50
goes from 487 to 448 and rate 98 from 59297 to 57983. The Q16 step quantises the
slowest coefficient from 7 to 6, so that end runs about 7 percent long; the rest of
the table is within a few percent.

The other rate-derived constants are:

- Note increment at C-1 (MIDI 0) starts at 749115498, after scaling the 44.1 kHz
  table by 44100/48000.
- Portamento step constant becomes 21845, from `32768 * 32 * 1000 / 48000`.
- Voice release hold becomes 8916 control blocks, from `8192 * 48000 / 44100`, so
  the 5.9 second hold stays.
- Part delay multiplies milliseconds by 48.
- The `liquid_filter` cutoff coefficient is scaled by 60211 in Q16, the integer
  closest to `44100/48000`, which keeps the cutoff close in hertz. The scale is
  linear in the coefficient: it is exact at low cutoffs, and the exact one-pole
  transform would sit up to about 5 percent higher at the top of the range. The
  default cutoff is within 2 percent.
- The spectral bank crossfade step becomes 17 per control tick, so 256 steps take
  about 10 ms at 48 kHz.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices, two parts, four operators per
  voice, unchanged.
- **Per-sample and per-block operations:** unchanged. The rate change moves table
  values, not the graph.
- **Instance memory, CPU, preset size:** the manifest, part block and preset layout
  are unchanged. The rate bridge grows from 32 to 128 taps, so its kernel is 128
  taps by 1024 phases.
- **Overflow/overload policy:** the rate transform clamps to the Q16 range, and the
  note increment table rounds each 44.1 kHz value to the nearest integer after the
  rate scale.
- **Division, shift, and range analysis:** the transform is computed once by
  `tools/gen_math_tables.py`, so no division is added to the audio path. The bridge
  kernel is built in double precision in `prepare`, outside the render path.

## Files and interfaces

- `tools/gen_math_tables.py`, the generator for `engine/gen/math_tables.h`.
- `engine/gen/math_tables.h`, `engine/gen/type_p_alpha_dsp.c`, `engine/gen/dsp.h`
  and `engine/gen/params.json`, regenerated.
- `engine/hal/spectral.h`, for the 17-step crossfade.
- `engine/runtime/presets.h`, for `PRESET_VERSION` 4.
- `synth/patch.json`, for the LFO defaults and the DC blocker.
- `plugin/engine/RateBridge.hpp` and `RateBridge.cpp`, for the 48 kHz core, the
  128-tap kernel, the cutoff and the latency.
- `plugin/juce/PluginProcessor.cpp` and `PluginProcessor.h`, for the core rate and
  state version 3.
- `plugin/ui/Widgets.cpp`, for the LFO Hz display.
- `plugin/tests/engine_parity.cpp`, `engine_render_reference.h`, `spectral.cpp`,
  `rate_bridge_test.cpp`, `plugin_smoke.cpp`, `plugin_stability.cpp`,
  `feedback_envelope.cpp`, `feedback_bipolar.cpp`, `additive_bench.cpp`,
  `additive_engine_bench.cpp` and `editor_snapshot.cpp`.
- `plugin/tools/additive_prototype.py`, for the regenerated oscillator call shape.

The public contract is the manifest. The engine owns the fixed-point state and the
rate tables. The plugin owns the host rate and the output resampler. No malloc,
float, file access or display enters the audio path.

## Verification

- **Host vector:** `engine_parity` renders note 60 at velocity 100 for 256 frames,
  then the release, and compares the 48 kHz capture with
  `engine_render_reference.h`. `spectral` runs at 48 kHz. `rate_bridge_test` runs
  44.1, 48, 96 and 192 kHz, and `plugin_smoke` checks the 48 kHz bypass and the
  44.1 kHz bridge. `plugin_stability` walks 44.1, 48, 96, 192 and back to 44.1.
- **Tolerance/metric:** the 1 kHz gain stays within 0.5 dB, the 10 kHz gain within
  1.5 dB, and the 20 kHz image at least 10 dB down at every non-core host rate. The
  measured image is -95.9 dB at 44.1, -108.0 dB at 96 and -104.3 dB at 192 kHz.
  The bridge latency is 58 host samples at 44.1, 0 at the 48 kHz bypass, 127 at 96
  and 254 at 192, from the 63.5 core-sample kernel delay.
- **Target package check:** the VST3, CLAP and standalone targets build, and the
  editor snapshot test passes. The panel did not change, so the golden images
  stay.
- **Host timing and platform check:** the Windows clang-cl/lld build passes all 12
  CTest entries. The Linux DAW matrix is not run in this tree; it belongs to the
  Linux container. The editor panel did not change, so the golden images stay.
- **Evidence artefact:** this contract, the regenerated 48 kHz reference, and the
  measured rate table in `rate_bridge_test`. The feedback research notes keep their
  44.1 kHz measurements as the historical evidence for that rate.

## Risks and fallback

- **Assumptions/open research:** the output resampler now carries the 44.1 kHz
  host, so that path is no longer bit-exact with the core. Only the 48 kHz path is
  exact. This is the cost of a single native rate.
- **Filter cutoff approximation:** the linear coefficient scale is exact at low
  cutoffs and about 5 percent low at the top of the range against the
  time-preserving transform. The default cutoff is within 2 percent. A lookup
  table can replace the scale if a listening test finds the top dull.
- **Safe experiment and stop criterion:** if the 48 kHz core drifts in pitch,
  envelope time, filter cutoff or LFO rate, stop and regenerate the table rather
  than tuning the render. If the bridge images above the gate, fix the kernel
  before widening the test.
- **Smaller alternative:** keep the 44.1 kHz core and resample every other host,
  which preserves the old exact path but keeps two musical bases.
- **Rollback:** restore the 44.1 kHz tables and the 32-tap bridge, return
  `PRESET_VERSION` to 3 and the state version to 2, and regenerate the test banks.

## Measured feedback change

The feedback loop is chaotic, so the two-sample average in the feedback path does
not follow a smooth rate transform. At the 50 percent positive setting the third
harmonic ratio moves from 0.132 to 0.090 and the 8 to 20 kHz energy from the 700 to
1000 band to 547. The negative half moves its third harmonic ratio from 0.391 to
0.418 and keeps its HF energy within 25 percent of the positive half. The family
claims still hold: the positive half is the smoother path, the negative half is the
absolute-value family, and the top of the negative range stays noisy. The
`feedback_bipolar` test records the 48 kHz capture in a comment so the next reader
can see the old values beside the new ones.
