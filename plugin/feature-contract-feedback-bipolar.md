# Feature contract: bipolar feedback

## Identity

- **Feature:** One bipolar feedback knob per operator, with the Reface-style square
  on the negative half.
- **Brief section:** README line of research 2, and the bipolar design in
  [`../docs/research/feedback-bipolar.md`](../docs/research/feedback-bipolar.md).
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete. The engine, the depth-only page, the 179 legacy
  migration and the Linux goldens are in. The CI run follows.

## Player outcome

- **New player action:** turn an operator's feedback knob from -100 to +100 percent.
- **Sound and behaviour:** the positive half is the current feedback path. The
  negative half is the square shaper with the two-sample average, so it moves from
  a rounded square through a harder square to the noisy top of the range. The
  shared envelope offsets the magnitude. The zero crossing is silent.
- **Failure mode:** the positive path changes, the negative half is not the square
  family, the zero crossing clicks, or the top of the range runs away.

## Dependencies and interactions

- **Other features:** `fb_tap`, `fm_op`, the shared `fbenv`, the feedback page and
  the operator card.
- **Platform/API/HAL changes:** none. The engine stays integer C.
- **Persistence/version impact:** the feedback parameter range becomes
  -32767..32767, and the shape and filter parameters are removed. The part block
  returns from 179 to 171 and the manifest from 372 to 356. The `.p1lib` reader
  accepts the 146, 160, 171 and 179 layouts.
- **MIDI/clock impact:** none. The feedback knob is host-automatable and stays out
  of the default CC map.

## Parameter and event semantics

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `op1_feedback` ... `op4_feedback` | `opN.fb.level` | -32767..32767 | 0 | `pct_bip` |
| `op1_fb_depth` ... `op4_fb_depth` | `opN.fb.depth` | -32767..32767 | 0 | `pct_bip` |
| `fbenv_r1` ... `fbenv_l3` | `fbenv.*` | 0..99 | 80, 60, 50, 80, 99, 93, 87 | raw |

The feedback level is signed. The magnitude drives the key and velocity followers
and the envelope offset:

```text
magnitude = abs(level)
level_eff = clamp(key_apply(magnitude, k + vk, 0, 32767) + q15_mul(fbenv, depth),
                  0, 32767)
```

The signal path is selected by the sign:

```text
level >= 0:  out = ((x + prev) * level_eff) >> 16;  prev = x
level <  0:  shaped = -(x*x >> 15)
             out = ((shaped + prev) * level_eff) >> 16;  prev = shaped
```

The same average is used on both halves, so the negative half keeps the top-end
noise. The one-pole filter and the abs shaper remain research alternatives. The
envelope depth cannot cross the sign, because it offsets the magnitude after the
path is chosen.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** the existing average, plus one square
  multiply and one negation per operator per sample on the negative half. The
  positive half is the current path.
- **Instance memory, CPU, preset size:** the feedback and depth parameters are
  already in the 171 block. The shape and filter parameters free 32 bytes per part.
  The `shaped` state is one `int32_t` per operator per voice; the `lp` state is
  removed. Each part record loses 32 bytes.
- **Overflow/overload policy:** `x` is Q15, so `x*x` is below `2^30` and `>> 15`
  returns Q15. The negation is within the Q15 range. The magnitude of -32767 is
  32767. The level is clamped to 0..32767 and the existing tap shift applies.
- **Division, shift, and range analysis:** the square uses a 32-bit multiply and a
  shift of a non-negative product. The sign is applied after the square.

## Files and interfaces

- `modules/nodes/fb_tap/node.json` and `node.c`, for the signed level and the
  square path.
- `modules/macros/fm_op/macro.json`, for the bipolar feedback parameter.
- `synth/patch.json`, for the range, the display and the init values.
- `engine/gen/*`, regenerated.
- `plugin/preset/part_migration.h` and `PresetStore.hpp`, for the 179 layout and the
  146, 160 and 171 layouts.
- `plugin/tests/preset_smoke.cpp` and a new `plugin/tests/feedback_bipolar.cpp`.
- `plugin/juce/PagesFeedback.cpp`, for the depth-only feedback page.
- `plugin/tests/editor_snapshot.cpp` and the Linux goldens.
- `docs/research/feedback-bipolar.md` and the index.

The public contract is the `fb_tap` node manifest and `kP1ParamSpecs`. The engine
owns the shaped feedback state. The APVTS is the host-facing owner. No malloc,
float, file access or display enters the audio path.

## Editor control map

The operator card's feedback knob becomes bipolar with the `pct_bip` display. The
feedback page keeps the envelope card and the four depth knobs. The shape and filter
rows are removed.

## Verification

- **Host vector:** a new CTest, `feedback_bipolar`, uses one sine operator at note
  60, ratio 1, level 99, a flat operator envelope, the filter off and a fresh
  engine. It renders the positive and negative feedback at 25, 50, 75 and 100
  percent, and the envelope depth at the negative half.
- **Tolerance/metric:** the positive path must stay bit-exact with the current
  capture. The negative half must be the square family, with the lower
  high-frequency energy of the square and the top-end noise at 100 percent. The
  zero crossing must be silent. The envelope depth must move the magnitude, not the
  sign. The peak must stay bounded and no sample may reach `INT32_MIN` or
  `INT32_MAX`.
- **Host timing and platform check:** the Linux, Windows and macOS CTest runs.
  pluginval and the CLAP validator cover the host path.
- **Target package check:** the VST3, CLAP and standalone builds expose 356
  parameters. The `preset_smoke` test covers the 146, 160, 171 and 179 layouts.
- **Editor check:** the snapshot shows the bipolar knob and the depth-only feedback
  page. The goldens are regenerated on the Linux container.
- **Evidence artefact:** `docs/research/feedback-bipolar.md`, with the positive and
  negative table, the bit-exact check, the WAVs and scope images in
  `docs/research/feedback-shaper/`.

## Risks and fallback

- **Assumptions/open research:** the square and the average are the chosen negative
  character. The abs and the one-pole remain research variants. The top-end noise
  is intentional.
- **Safe experiment and stop criterion:** if the positive path changes, if the
  negative half is not the square family, or if the zero crossing clicks, stop and
  fix the path before the UI.
- **Smaller alternative:** keep the positive half only and leave the negative half
  as the research shaper.
- **Rollback:** restore the 179 shape and filter layout and the shaper page, and
  keep the measured WAVs and scopes.
