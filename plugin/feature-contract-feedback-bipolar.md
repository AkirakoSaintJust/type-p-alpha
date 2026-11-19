# Feature contract: bipolar feedback

> The envelope and depth rows below are the 171 shared-envelope design. The
> current per-operator signed envelope and LFOs are in
> [`feature-contract-operator-lfos.md`](feature-contract-operator-lfos.md).

## Identity

- **Feature:** One bipolar feedback knob per operator, with the Reface-style
  absolute-value pulse on the negative half.
- **Brief section:** README line of research 2, and the bipolar design in
  [`../docs/research/feedback-bipolar.md`](../docs/research/feedback-bipolar.md).
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete, with the engine, the depth-only page, the 179 legacy
  migration and the Linux goldens in, and the CI run still to follow.
  I left the page at the envelope card and the four depth knobs, because the shape
  and filter rows belong to the research contract.

## Player outcome

- **New player action:** turn an operator's feedback knob from -100 to +100 percent,
  and the sign you land on picks the family.
- **Sound and behaviour:** the positive half is the current feedback path, whilst the
  negative half takes the absolute value, negates it and uses the two-sample average,
  which makes it unruly from the low end and close to the positive character in the
  middle. The top is noisy. The shared envelope offsets the magnitude, and the zero
  crossing is silent, so a player who wants to tame the top can reach for the
  per-part filter after the operators.
- **Failure mode:** the positive path changes, the negative half is not the abs
  family, the zero crossing clicks, or the top of the range runs away.

## Dependencies and interactions

- **Other features:** `fb_tap`, `fm_op`, the shared `fbenv`, the feedback page and
  the operator card.
- **Platform/API/HAL changes:** none, because the engine stays integer C and the
  whole change sits inside the tap.
- **Persistence/version impact:** the feedback parameter range becomes
  -32767..32767, the shape and filter parameters are removed, the part block returns
  from 179 to 171 and the manifest from 372 to 356, and the `.p1lib` reader accepts
  the 146, 160, 171 and 179 layouts.
- **MIDI/clock impact:** none, since the feedback knob is host-automatable and stays
  out of the default CC map.

## Parameter and event semantics

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `op1_feedback` ... `op4_feedback` | `opN.fb.level` | -32767..32767 | 0 | `pct_bip` |
| `op1_fb_depth` ... `op4_fb_depth` | `opN.fb.depth` | -32767..32767 | 0 | `pct_bip` |
| `fbenv_r1` ... `fbenv_l3` | `fbenv.*` | 0..99 | 80, 60, 50, 80, 99, 93, 87 | raw |

The feedback level is signed, and the magnitude drives the key and velocity followers
and the envelope offset:

```text
magnitude = abs(level)
level_eff = clamp(key_apply(magnitude, k + vk, 0, 32767) + q15_mul(fbenv, depth),
                  0, 32767)
```

The signal path is selected by the sign:

```text
level >= 0:  out = ((x + prev) * level_eff) >> 16;  prev = x
level <  0:  shaped = -abs(x)
             out = ((shaped + prev) * level_eff) >> 16;  prev = shaped
```

The same average is used on both halves, so the negative half keeps the top-end
noise, and the one-pole filter and the abs shaper remain research alternatives. The
envelope depth cannot cross the sign, because it offsets the magnitude after the path
is chosen, so a deep envelope can add feedback without flipping the family.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** the existing average, plus one absolute
  value and one negation per operator per sample on the negative half, whilst the
  positive half is the current path and pays nothing for the new branch.
- **Instance memory, CPU, preset size:** the feedback and depth parameters are
  already in the 171 block, the shape and filter parameters free 32 bytes per part,
  the `shaped` state is one `int32_t` per operator per voice, and the `lp` state is
  removed, so each part record loses 32 bytes.
- **Overflow/overload policy:** `x` is Q15 and `abs(x)` is 0..32767, the negation
  stays within the Q15 range, the magnitude of -32767 is 32767, and the level is
  clamped to 0..32767 with the existing tap shift on top.
- **Division, shift, and range analysis:** the abs has no multiply, the sign is
  applied after the abs, and the average follows.

## Files and interfaces

- `modules/nodes/fb_tap/node.json` and `node.c`, for the signed level and the abs
  path.
- `modules/macros/fm_op/macro.json`, for the bipolar feedback parameter.
- `synth/patch.json`, for the range, the display and the init values.
- `engine/gen/*`, regenerated.
- `plugin/preset/part_migration.h` and `PresetStore.hpp`, for the 179 layout and the
  146, 160 and 171 layouts.
- `plugin/tests/preset_smoke.cpp` and a new `plugin/tests/feedback_bipolar.cpp`.
- `plugin/juce/PagesFeedback.cpp`, for the depth-only feedback page.
- `plugin/tests/editor_snapshot.cpp` and the Linux goldens.
- `docs/research/feedback-bipolar.md` and the index.

The public contract is the `fb_tap` node manifest and `kP1ParamSpecs`, the engine
owns the shaped feedback state, the APVTS is the host-facing owner, and no malloc,
float, file access or display enters the audio path.

## Editor control map

The operator card's feedback knob becomes bipolar with the `pct_bip` display, and the
feedback page keeps the envelope card and the four depth knobs, whilst the shape and
filter rows are removed.

## Verification

- **Host vector:** a new CTest, `feedback_bipolar`, uses one sine operator at note
  60, ratio 1, level 99, a flat operator envelope, the filter off and a fresh engine.
  It renders the positive and negative feedback at 25, 50, 75 and 100 percent, and
  the envelope depth at the negative half.
- **Tolerance/metric:** the positive path must stay bit-exact with the current
  capture, the negative half must be the abs family with the high-frequency energy
  close to the positive half and the top-end noise at 100 percent, and the zero
  crossing must be silent. The envelope depth must move the magnitude without
  touching the sign, the peak must stay bounded, and no sample may reach `INT32_MIN`
  or `INT32_MAX`.
- **Host timing and platform check:** the Linux, Windows and macOS CTest runs, with
  pluginval and the CLAP validator covering the host path.
- **Target package check:** the VST3, CLAP and standalone builds expose 356
  parameters, and the `preset_smoke` test covers the 146, 160, 171 and 179 layouts.
- **Editor check:** the snapshot shows the bipolar knob and the depth-only feedback
  page, and the goldens are regenerated on the Linux container.
- **Evidence artefact:** `docs/research/feedback-bipolar.md`, with the positive and
  negative table, the bit-exact check, the WAVs and scope images in
  `docs/research/feedback-shaper/`.

## Risks and fallback

- **Assumptions/open research:** the abs and the average are the chosen negative
  character, the square and the one-pole remain research variants, and the top-end
  noise is intentional, which is the sort of choice I'd rather write down here than
  rediscover in a year.
- **Safe experiment and stop criterion:** if the positive path changes, if the
  negative half is not the abs family, or if the zero crossing clicks, stop and fix
  the path before the UI.
- **Smaller alternative:** keep the positive half only and leave the negative half as
  the research shaper.
- **Rollback:** restore the 179 shape and filter layout and the shaper page, and keep
  the measured WAVs and scopes.
