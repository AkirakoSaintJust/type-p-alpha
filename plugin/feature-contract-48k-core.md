# Feature contract: 48 kHz native core

## Identity

- **Feature:** Move the engine core from 44.1 kHz to 48 kHz, and regenerate every
  rate-derived table and constant that the new rate touches.
- **Brief section:** README line of research 7, the rate bridge, and the audio
  section of [`../design/juce.md`](../design/juce.md).
- **Owner / date:** P1Onken, 2026-11-13.
- **Disposition:** complete, and the Windows CTest gate passes. This conversion did
  not change the panel, whilst the later spectral UI pass does, and that pass needs
  the Linux golden set regenerated before its goldens mean anything. I've kept the
  two apart here, since the rate change is measured against the engine render and
  the UI pass is measured against the goldens.

## Player outcome

- **New player action:** none of the controls move, because the rate change is
  carried by the tables underneath them.
  Existing patches keep their pitch, envelope times, LFO rates and part delays, and
  their filter cutoffs stay close across the range.
- **Sound and behaviour:** the core renders at 48 kHz, so a 48 kHz host takes the
  core output untouched.
  A 44.1, 96 or 192 kHz host is resampled at the output with the 128-tap polyphase
  bridge, so the old 44.1 kHz rule is withdrawn, the engine has one musical base,
  and I'm glad to see the second one go.
- **Failure mode:** a pitch offset, an envelope or LFO that changes speed, a filter
  cutoff that moves, part delay that drifts, resampling that images, or a 48 kHz
  render that isn't bit-exact with the core.

## Dependencies and interactions

- **Other features:** the control-rate math tables, the note increment table, the
  portamento step, the part delay counter, the voice release hold, the liquid filter
  cutoff coefficient, the DC blocker, the spectral bank crossfade, the rate bridge,
  the plugin processor, the LFO Hz display, the preset version and the engine parity
  reference.
- **Platform/API/HAL changes:** none, since the engine stays integer C and the rate
  adaptation stays at the output boundary, which is what keeps the core at one rate.
- **Persistence/version impact:** `PRESET_VERSION` moves from 3 to 4 and the plugin
  state version from 2 to 3. The later harmonic range feature moves the preset
  version to 5, the 64-harmonic feature takes it to 6 and the state version to 4,
  and the project had not released at the time, so this pass wrote no migration and
  regenerated the older test banks instead. The later filter-law change added the
  cutoff migration, with preset version 7 and state version 5. The manifest moves from 830 to 1,614 parameters, the part block from 408 to 800
  and the spectral block from 47 to 143 per operator.
- **MIDI/clock impact:** none, because the 4 Hz LFO defaults keep their displayed
  value even though the raw number behind them moves.

## Parameter and event semantics

The parameters and ranges do not change, and that's on purpose: the raw defaults
that carry a rate are rescaled so the displayed value stays the same, which is the
only way a rate change can leave an existing patch sounding like itself:

| Parameter group | Old raw | New raw | Display |
| --- | --- | --- | --- |
| amplitude and spectral LFO rate | 389566 | 357914 | 4.0 Hz |
| pitch LFO rate | 486958 | 447392 | 4.0 Hz |
| DC blocker `r` | 32735 | 32738 | none |
| part delay scale | 44 samples per ms | 48 samples per ms | milliseconds |

The LFO display and its inverse use 48000 in `plugin/ui/Widgets.cpp`.

The envelope rate coefficients are stored per 32-sample control block, and the
44.1 kHz table is the musical base, so each coefficient `a` is transformed to the
48 kHz block period with `1 - (1 - a)^(44100/48000)`, which preserves the one-pole
time constant in seconds. The control block is 32 samples at both rates, which means
the block period falls from 725.6 to 666.7 microseconds and the coefficient shrinks
accordingly, and that's why the table isn't a straight copy of the old one. Examples:
rate 0 goes from 7 to 6, rate 50 from 102 to 94, and rate 98 from 26008 to 24350.
The third-rate table follows the same transform, so rate 50 goes from 487 to 448 and
rate 98 from 59297 to 57983. The Q16 step quantises the slowest coefficient from 7
to 6, so that end runs about 7 percent long, whilst the rest of the table stays
within a few percent, which I'd call close enough for a change that has to keep an
old patch sounding like itself.

The other rate-derived constants:

- Note increment at C-1 (MIDI 0) starts at 749115498, after scaling the 44.1 kHz
  table by 44100/48000.
- Portamento step constant becomes 21845, from `32768 * 32 * 1000 / 48000`.
- Voice release hold becomes 8916 control blocks, from `8192 * 48000 / 44100`, so
  the 5.9 second hold stays where it was.
- Part delay multiplies milliseconds by 48.
- The `liquid_filter` cutoff coefficient is scaled by 60211 in Q16, the integer
  closest to `44100/48000`, which keeps the cutoff close in hertz. The scale is
  linear in the coefficient: it is exact at low cutoffs, and the exact one-pole
  transform would sit up to about 5 percent higher at the top of the range. The
  default cutoff is within 2 percent.
- The spectral bank crossfade step becomes 17 per control tick, so 256 steps take
  about 10 ms at 48 kHz.

## MIDI timing

The core 32-sample grid is the timeline at every host rate, rather than only at
48 kHz. Events are mapped into core frames and dispatched per core tick:

- At 48 kHz the processor drains the events for each 32-sample chunk before that
  chunk renders.
- At 44.1, 96 and 192 kHz each host position maps to a core frame with the bridge
  ratio, and the bridge callback feeds the next core chunk before it renders. At 96
  and 192 kHz a block generates fewer core frames than host frames, so an event can
  belong to a tick the next block renders and waits in the schedule until then.
- A note-on and its note-off in the same tick dispatch the on before the render and
  hold the off until after it, so the note sounds for one control tick instead of
  cancelling before any audio. An ordinary note-off keeps its own tick.

The placement error is at most 31 samples, 0.65 ms at 48 kHz, and the same grid at
44.1 kHz removes the old whole-block collapse, which was up to a host block early.
The envelope's first tick is silent with the default attack, so a note shorter than
a tick is inaudible by construction rather than swallowed by the event order.

Evidence is in `plugin_smoke`: `a short note renders at 44100 Hz`, and the same at
48, 96 and 192 kHz, where the note spans about 65 core samples so the old non-48 kHz
path would have cancelled it. `a same-tick pair leaves a finite block` covers the
degenerate case.

## Bounds and resources

- **Maximum voices/events/effects:** 24 voices, two parts and four operators per
  voice.
- **Per-sample and per-block operations:** unchanged, because the rate change moves
  table values and leaves the graph alone.
- **Instance memory, CPU, preset size:** the 48 kHz conversion leaves the manifest,
  part block and preset layout unchanged. The 64-harmonic feature grows them to
  1,614 parameters and 800 per part, and the rate bridge grows from 32 to 128 taps,
  so its kernel is 128 taps by 1024 phases.
- **Overflow/overload policy:** the rate transform clamps to the Q16 range, and the
  note increment table rounds each 44.1 kHz value to the nearest integer after the
  rate scale.
- **Division, shift, and range analysis:** the transform is computed once by
  `tools/gen_math_tables.py`, so no division is added to the audio path, and the
  bridge kernel is built in double precision in `prepare`, outside the render path.

## Files and interfaces

- `tools/gen_math_tables.py`, the generator for `engine/gen/math_tables.h`.
- `engine/gen/math_tables.h`, `engine/gen/type_p_alpha_dsp.c`, `engine/gen/dsp.h`
  and `engine/gen/params.json`, regenerated.
- `engine/hal/spectral.h`, for the 17-step crossfade.
- `engine/runtime/presets.h`, for `PRESET_VERSION` 4, 5 after the harmonic
  range feature, and 6 after the 64-harmonic feature.
- `synth/patch.json`, for the LFO defaults and the DC blocker.
- `plugin/engine/RateBridge.hpp` and `RateBridge.cpp`, for the 48 kHz core, the
  128-tap kernel, the cutoff and the latency.
- `plugin/juce/PluginProcessor.cpp` and `PluginProcessor.h`, for the core rate and
  state version 4 (3 before the 64-harmonic feature).
- `plugin/ui/Widgets.cpp`, for the LFO Hz display.
- `plugin/tests/engine_parity.cpp`, `engine_render_reference.h`, `spectral.cpp`,
  `rate_bridge_test.cpp`, `plugin_smoke.cpp`, `plugin_stability.cpp`,
  `feedback_envelope.cpp`, `feedback_bipolar.cpp`, `additive_bench.cpp`,
  `additive_engine_bench.cpp` and `editor_snapshot.cpp`.
- `plugin/tools/additive_prototype.py`, for the regenerated oscillator call shape.

The public contract is the manifest, the engine owns the fixed-point state and the
rate tables, the plugin owns the host rate and the output resampler, and no malloc,
float, file access or display enters the audio path.

## Verification

- **Host vector:** `engine_parity` renders note 60 at velocity 100 for 256 frames,
  then the release, and compares the 48 kHz capture with
  `engine_render_reference.h`. `spectral` runs at 48 kHz. `rate_bridge_test` runs
  44.1, 48, 96 and 192 kHz, and `plugin_smoke` checks the 48 kHz bypass, the
  44.1 kHz bridge and the tap scope window. `plugin_stability` walks 44.1, 48, 96,
  192 and back to 44.1.
- **Tolerance/metric:** the 1 kHz gain stays within 0.5 dB, the 10 kHz gain within
  1.5 dB, and the 20 kHz image at least 10 dB down at every non-core host rate. The
  measured image is -95.9 dB at 44.1, -108.0 dB at 96 and -104.3 dB at 192 kHz.
  The bridge latency is 58 host samples at 44.1, 0 at the 48 kHz bypass, 127 at 96
  and 254 at 192, from the 63.5 core-sample kernel delay.
- **Target package check:** the VST3, CLAP and standalone targets build, and the
  editor snapshot test passes against the current goldens.
- **Host timing and platform check:** the Windows clang-cl/lld build passes all 12
  CTest entries, and the Linux build passes all 13, including the golden diff. The
  Linux DAW matrix is not run in this tree, because it belongs to the Linux
  container.
- **Sanitizer vector:** the Linux clang build with
  `-DP1_SANITIZERS=address,undefined` in `build/linux-amd64-clang-sanitize` passes
  all 13 CTest entries with leak detection on and `halt_on_error` set. The first run
  found a signed overflow in the MIDI CC scaling in `engine/runtime/synth_midi.c`,
  where `(max - min) * value` overflowed for the 20,000,000-unit rate range, and the
  scale now multiplies in 64 bits. I'd rather fix the multiply than narrow the
  range, since the range is part of the parameter.
- **Evidence artefact:** this contract, the regenerated 48 kHz reference, and the
  measured rate table in `rate_bridge_test`. The feedback research notes keep their
  44.1 kHz measurements as the historical evidence for that rate.

## Risks and fallback

- **Assumptions/open research:** the output resampler now carries the 44.1 kHz
  host, so that path is no longer bit-exact with the core. Only the 48 kHz path is
  exact, and that is the cost of a single native rate, not an accident of the
  bridge.
- **Filter cutoff approximation:** the 48 kHz conversion scaled the old
  coefficient by 60211/65536 to keep the 44.1 kHz cutoff, which was exact at low
  cutoffs and about 5 percent low at the top of the range against the
  time-preserving transform. The SH-101 filter law in
  [`feature-contract-filter-law.md`](feature-contract-filter-law.md) removes that
  scale and reads the cutoff from a 257-entry table, so the 48 kHz core now has
  the one-pole coefficient at every position. The old scale remains only in the
  migration of patches saved before the change.
- **Safe experiment and stop criterion:** if the 48 kHz core drifts in pitch,
  envelope time, filter cutoff or LFO rate, stop and regenerate the table rather
  than tuning the render. If the bridge images above the gate, fix the kernel
  before widening the test.
- **Smaller alternative:** keep the 44.1 kHz core and resample every other host,
  which preserves the old exact path at the cost of two musical bases to maintain.
- **Rollback:** restore the 44.1 kHz tables and the 32-tap bridge, return
  `PRESET_VERSION` to 3 and the state version to 2, and regenerate the test banks.

## Tap scope capture

The 128-tap bridge renders more than one 32-sample core chunk per host block, and
the old processor copied only the last chunk into the ten tap rings, so the scope
window was mostly zeros and the trace broke at the block edges. `RateBridge::process`
now takes a per-chunk callback, and the processor copies every tap chunk, which is
what `plugin_smoke` checks when it looks for energy in both halves of the 44.1 kHz
tap window.

## Measured feedback change

The feedback loop is chaotic, so the two-sample average in the feedback path does
not follow a smooth rate transform. At the 50 percent positive setting the third
harmonic ratio moves from 0.132 to 0.090 and the 8 to 20 kHz energy from the 700 to
1000 band to 547. The negative half moves its third harmonic ratio from 0.391 to
0.418 and keeps its HF energy within 25 percent of the positive half. The family
behaviour still holds, in that the positive half is the smoother path, the negative
half is the absolute-value family, and the top of the negative range stays noisy.
The `feedback_bipolar` test records the 48 kHz capture in a comment so the next
reader can see the old values beside the new ones.
