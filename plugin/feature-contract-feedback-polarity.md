# Feature contract: negative feedback polarity

> Bounded experiment for research line 2. The sign flip is measured before it is
> exposed. If it fails the stop criterion, no parameter or editor control is added
> and the measured table is the answer to the research question.

## Identity

- **Feature:** Per-operator negative feedback polarity.
- **Brief section:** README lines of research 1 and 2,
  [`../design/research-feedback.md`](../design/research-feedback.md).
- **Owner / date:** P1Onken, 2026-11-11.
- **Disposition:** deferred by measurement. The stop criterion failed at 25 and 50
  percent feedback, so the parameter, the editor control and the migration were not
  shipped. The measured table is in `../plugin/evidence/feedback-polarity.md`.

## Player outcome

- **New player action:** flip `neg` on an operator card. The experiment did not
  pass, so this action is not exposed.
- **Sound and behaviour:** at the same feedback amount, the loop inverts. The third
  and fifth harmonics must rise by at least 25 percent relative to the second at
  25, 50 and 75 percent feedback. At 0 feedback there is no audible or numerical
  change. The positive path stays bit-exact.
- **Failure mode:** the sign flip does not raise the odd/even ratios, the positive
  path changes, or the peak grows beyond 10 percent at 100 percent feedback. Any
  one of these stops the feature.

## Dependencies and interactions

- **Other features:** `fm_op`, `fb_tap`, `tl`, the key and velocity followers, the
  output DC blocker, `PresetStore`, and the operator page.
- **Platform/API/HAL changes:** none. The engine is C and the sign is one
  conditional negate.
- **Persistence/version impact:** not executed. The experiment failed, so the part
  block stays at 160 and the total at 334. The ship phase would have grown the part
  block to 164 and the total to 342, and the `.p1lib` reader would have accepted the
  146 and 160 layouts in addition to the current 164. The APVTS state is name-keyed,
  so the ship phase would have loaded an old state with the four polarity parameters
  at 0.
- **MIDI/clock impact:** none. The parameters are host-automatable and stay out of
  the default CC map.

## Parameter and event semantics

The parameters that the ship phase would have added:

| Parameter | Target | Kind | Range | Default | Rate | Display |
| --- | --- | --- | --- | --- | --- | --- |
| `op1_fb_neg.p0` ... `op4_fb_neg.p0` | `opN.fb.neg.p0` | `uint32` | 0..1 | 0 | block | `polarity` |
| `op1_fb_neg.p1` ... `op4_fb_neg.p1` | `opN.fb.neg.p1` | `uint32` | 0..1 | 0 | block | `polarity` |

Zero is positive and one is negative. The parameters are linear and not
modulatable. The new part-zero engine ids would have been 19, 49, 79 and 109, one
slot after each `opN_feedback.p0`. The part-one ids would have been 183, 213, 243
and 273. The `polarity` display prints `pos` or `neg`, and the editor control is a
`neg` toggle next to the waveform choice. The experiment failed, so none of these
parameters are in the manifest.

The experiment phase adds the manifest entry and the node sign to a test build. It
does not add the editor control or the `.p1lib` migration. If the experiment
passes, those follow in the ship phase. If it fails, the manifest entry and the
node sign are reverted.

`level_eff` stays positive and block-derived. The sign is applied after the shift,
so the key and velocity followers keep working on the magnitude. The parameter is
read per sample inside the existing feedback gate. Note ordering, voice allocation,
stealing, sustain and panic are unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** the existing gate and multiply, plus one
  conditional negate when `level_eff` is nonzero. At 0 feedback the gate skips both.
  The block-derived `level_eff` costs the same as before.
- **Instance memory, CPU, preset size:** eight `int32_t` parameters, 32 bytes, plus
  the APVTS entries. No new voice state. Each part record grows by 16 bytes, so a
  128-slot part bank grows by 2,048 bytes.
- **Overflow/overload policy:** `audio` and `prev` are Q15, so the sum is in
  `[-65536, 65534]`. `level_eff` is at most 32767. The product is at most
  2,147,418,112, below `INT32_MAX`. After `>> 16`, the magnitude is at most 32766,
  so the negation is safe. There is no division.
- **Division, shift, and range analysis:** the shift stays on the positive product
  and the sign is applied afterwards, so no new negative shift is introduced.

## Files and interfaces

Experiment phase:

- `modules/nodes/fb_tap/node.json` and `node.c`, for the `neg` parameter and the
  conditional negate.
- `modules/macros/fm_op/macro.json`, for `fb_neg` on `fb.neg`.
- `synth/patch.json`, for the four manifest entries and the init values.
- `engine/gen/*` and the build-time `Params.h`, regenerated from the manifest.
- `plugin/tests/feedback_polarity.cpp`, registered in `plugin/CMakeLists.txt`.

Ship phase, not executed:

- `plugin/preset/part_migration.h`, for the 160 map and the 146 composition.
- `plugin/preset/PresetStore.hpp` and `PresetStore.cpp`, for a list of legacy
  layouts.
- `plugin/juce/PluginProcessor.h` and `plugin/tests/preset_smoke.cpp`.
- `plugin/ui/ParamIDs.h` and `plugin/juce/PagesOperators.cpp`, for the toggle.
- `plugin/ui/Widgets.cpp`, for the `polarity` display.
- `plugin/tests/snapshots/linux-x86_64`, for the operator page goldens.
- `README.md`, for the research links.

The public contract is the `fb_tap` node manifest and `kP1ParamSpecs`. The engine
owns the per-part polarity array and the APVTS is the host-facing owner. The
block-derived `level_eff` remains the magnitude owner. No malloc, float, file
access or display enters the audio path.

## Verification

- **Host vector:** a new CTest, `feedback_polarity`, renders the engine with one
  sine operator at ratio 1, level 99, note 60, 44.1 kHz, a flat DX envelope, the
  other operators muted and the filter off. It renders 8,192 samples, skips the
  first 2,048, and applies a Hann-windowed DFT at the exact harmonic frequencies.
  It measures feedback 0, 20, 25, 35, 50, 75 and 100 percent, positive and negative.
- **Tolerance/metric:** the test prints the fundamental to sixth harmonic, the
  second-to-first, third-to-second and fifth-to-second ratios, and the peak. At
  0 percent the positive and negative renders must be bit-identical. At 25, 50 and
  75 percent the negative third-to-second and fifth-to-second ratios must each be
  at least 1.25 times the positive ratio. At 100 percent the negative peak must be
  at most 1.1 times the positive peak, with no `INT32_MIN` or `INT32_MAX` sample.
- **Host timing and platform check:** the normal CTest run on Linux, Windows and
  macOS. If the experiment passes, pluginval at strictness 5 and the CLAP validator
  cover the host path, and the Linux editor snapshot covers the toggle.
- **Target package check:** the VST3, CLAP and standalone builds carry the new
  parameters through the generated manifest. The `.p1lib` test covers the 146 and
  160 layouts, a current save, and a round-trip.
- **Evidence artefact:** `plugin/evidence/feedback-polarity.md`, with the measured
  table, the build hash and the pass/fail decision. The test output is the primary
  record and the artefact is the edited summary. The file exists and carries the
  result.
- **Measured result:** the stop fired at 25 and 50 percent. At 25 percent the
  negative third-to-second ratio was 0.487 against 0.483 positive (1 percent
  higher), and the fifth-to-second ratio was 0.213 against 0.216 (1 percent lower).
  At 50 percent both ratios fell, to 0.184 from 0.239 and to 0.076 from 0.125. The
  75 percent row rose, but the positive second harmonic was nearly zero, so the row
  is not a stable control. The 100 percent negative peak was 3 percent lower. The 0
  percent renders were bit-identical and `engine_parity` stayed bit-exact. The sign
  flip is not a reliable odd-harmonic control.

## Risks and fallback

- **Assumptions/open research:** a sign flip of a phase-modulation loop may not
  change the magnitude spectrum, because the sideband magnitudes are even in the
  modulation index. The preliminary model in the design note supports that, and the
  engine measurement decides.
- **Safe experiment and stop criterion:** the experiment changes only the tap sign
  and the test build. It stops if any of the three amounts misses the 1.25 ratio,
  if the 0 percent renders differ, or if the 100 percent peak grows beyond 10
  percent.
- **Smaller alternative:** the shared feedback envelope with a per-operator depth.
  It modulates the feedback magnitude, which the spectrum follows, and it does not
  depend on the sign. It costs 11 parameters per part and needs its own contract.
- **Rollback:** the experiment code was removed. The manifest entries, the node
  parameter and the test are gone, the engine was regenerated, and the measured
  table stays in the evidence as the answer to research line 2.
