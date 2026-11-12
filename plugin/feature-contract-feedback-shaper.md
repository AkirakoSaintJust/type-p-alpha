# Feature contract: feedback shaper

## Identity

- **Feature:** A per-operator nonlinear feedback shaper and a stronger feedback
  filter.
- **Brief section:** README line of research 2, and the nonlinear path in
  [`../docs/research/README.md`](../docs/research/README.md).
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** research. The shape and filter controls were the experiment. The
  shipped control is the bipolar feedback knob in
  [`feature-contract-feedback-bipolar.md`](feature-contract-feedback-bipolar.md);
  its negative half uses the abs and the average; the square and the one-pole remain
  research variants. The measurements are in
  [`../docs/research/feedback-shaper.md`](../docs/research/feedback-shaper.md).

## Player outcome

- **New player action:** choose a feedback shape per operator on the feedback page.
  The shape encodes the polarity, so one control covers the square, abs, signed
  square and their negative forms.
- **Sound and behaviour:** the shaper turns the sine feedback into the
  Reface-style family. A positive square or abs should move toward the square or
  pulse shape, and the negative form should select the other side of the shaped
  wave. A stronger feedback lowpass should tame the noisy top of the range. The
  `off` shape is bit-exact with the current feedback path.
- **Failure mode:** the shaper is not sign-dependent, the lowpass does not reduce
  the high-frequency energy, the DC offset detunes the voice, or the peak runs away.

## Dependencies and interactions

- **Other features:** `fb_tap`, `fm_op`, the feedback envelope and the feedback
  page.
- **Platform/API/HAL changes:** none. The engine stays integer C.
- **Persistence/version impact:** the part block grows from 171 to 179 and the
  manifest from 356 to 372. The `.p1lib` reader accepts the 146, 160, 171 and 179
  layouts. The APVTS state is name-keyed.
- **MIDI/clock impact:** none. The shape choice is host-automatable and stays out of
  the default CC map.

## Parameter and event semantics

The experiment parameters, per operator and part:

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `op1_fb_shape` ... `op4_fb_shape` | `opN.fb.shape` | 0..6 | 0 | shape names |
| `op1_fb_filter` ... `op4_fb_filter` | `opN.fb.filter` | 0..1 | 0 | average or one-pole |

The shape choices are `off`, `square`, `square neg`, `abs`, `abs neg`, `signed`,
`signed neg`. The filter choices are `average` and `one-pole`. The shape and filter
are inserted after the depth parameter in each operator. The part block grows from
171 to 179. In part zero the depths are 19, 51, 83 and 115, the shapes are 20, 52,
84 and 116, the filters are 21, 53, 85 and 117, the envelope is 164..170 and the
mode block is 171..178. Part one starts at 179, its depths are 198, 230, 262 and
294, its shapes are 199, 231, 263 and 295, its filters are 200, 232, 264 and 296,
and its envelope is 343..349. The total is 372 and the routing base is 362.

The `fb_tap` gains `shape` and `filter` parameters. The shaped sample is

```text
off:            x
square:         (x*x) >> 15
abs:            abs(x)
signed:         (x*abs(x)) >> 15
```

and the negative choices invert the shaped sample. The shaped sample is what the
average stores in `prev`, and what the one-pole accumulates. The `signed` square is
odd and carries no magnitude change under the sign, so its negative form is a phase
variant. The `square` and `abs` are even and carry a DC component.

The filter is the one-pole `lp += (shaped - lp) >> 1`, a 0.5 coefficient. The
average remains the default, so the existing feedback path stays bit-exact.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** one branch and at most one multiply per
  operator per sample for the shape, plus the average or the one-pole. The one-pole
  adds one shift and one add per operator per sample.
- **Instance memory, CPU, preset size:** eight `int32_t` parameters, 32 bytes. The
  shape reuses the `prev` state. The `shaped`, `magnitude` and `lp` state fields add
  12 bytes per operator per voice, so 20 voices add 960 bytes. Each part record grows
  by 32 bytes, so a 128-slot part bank grows by 4,096 bytes.
- **Overflow/overload policy:** `x` is Q15, so `x*x` is below `2^30` and `>> 15`
  returns Q15. `abs(-32768)` is clamped to 32767. The shaped value feeds the
  existing level multiply and shift. The sign is applied after the shape, so the
  positive path stays exact.
- **Division, shift, and range analysis:** the square uses a 32-bit multiply and a
  shift of a non-negative product. The signed square multiplies two Q15 magnitudes
  and applies the sign afterwards.

## Files and interfaces

- `modules/nodes/fb_tap/node.json` and `node.c`, for the shape and filter
  parameters and the shaped feedback.
- `modules/macros/fm_op/macro.json`, for the `fb_shape` and `fb_filter` parameters.
- `synth/patch.json`, for the manifest entries and the init values.
- `engine/gen/*`, regenerated.
- `plugin/preset/part_migration.h`, `PresetStore.hpp`, `PresetStore.cpp` and
  `PluginProcessor.h`, for the 171, 160 and 146 legacy layouts.
- `plugin/tests/preset_smoke.cpp`, and the experiment test that wrote the WAVs
  (removed after the files were committed).
- `plugin/ui/ParamIDs.h`, `PagesFeedback.cpp`, and the `ParamChoice` names.
- `plugin/ui/Widgets.cpp`, for the shape display names.
- `plugin/tests/editor_snapshot.cpp` and the Linux goldens.
- `docs/research/feedback-shaper.md` and the README links.

The public contract is the `fb_tap` node manifest and `kP1ParamSpecs`. The engine
owns the shaped feedback state. The APVTS is the host-facing owner. No malloc,
float, file access or display enters the audio path.

## Editor control map

The experiment page carried three controls per operator: the depth knob, the shape
choice and the filter choice. The shipped page keeps the envelope and the depths,
and the bipolar feedback knob is on the operator card.

```text
operator depth
op1 [depth]        op2 [depth]        op3 [depth]        op4 [depth]
    [off v]            [square v]         [abs v]            [signed v]
    [average v]        [one-pole v]       [average v]        [one-pole v]
```

The depth knob keeps the `pct_bip` display. The shape is a `ParamChoice` with the
seven names, and the filter is a `ParamChoice` with the two names.

## Verification

- **Host vector:** the experiment test, `feedback_shaper`, used one sine operator at
  note 60, ratio 1, level 99, a flat operator envelope, the filter off and a fresh
  engine. It rendered 16,384 frames into a 2 x 16,384 buffer and analysed the left
  channel. The WAVs and scope images are committed. The shipped `feedback_bipolar`
  test covers the bipolar control.
- **Tolerance/metric:** the `off` shape and the `average` filter must be bit-exact
  with the current path. `square` and `abs` must change the spectrum relative to
  `off`, and their negative forms must differ from the positive forms. The one-pole
  must lower the energy above 8 kHz at 75 percent feedback. The signed square must
  carry less output DC than the even shapes. The peak must stay bounded and no
  sample may reach `INT32_MIN` or `INT32_MAX`.
- **Host timing and platform check:** the Linux, Windows and macOS CTest runs.
  pluginval and the CLAP validator cover the host path.
- **Target package check:** the experiment build exposed 372 parameters. The
  `preset_smoke` test covers the 146, 160, 171 and 179 layouts. The shipped build
  returns to 356 parameters with the bipolar feedback.
- **Editor check:** the snapshot renders the shape and filter choices and the new
  card height. The goldens are regenerated on the Linux container with 12 images.
- **Evidence artefact:** `docs/research/feedback-shaper.md`, with the spectral
  table, the sign comparison, the high-frequency energy and the build hash.

## Risks and fallback

- **Assumptions/open research:** the source is a Csound experiment and a reader
  comment, not a Reface teardown. The `abs` shape is a hypothesis. The square and
  the signed square cover the neighboring possibilities. The signed square is a
  phase variant in the magnitude spectrum, so its negative form is mostly a phase
  flip.
- **Safe experiment and stop criterion:** the measurement passed. The square and
  abs shapes change the spectrum, their negative forms differ, the one-pole lowers
  the high-frequency energy, and the peaks stay bounded.
- **Smaller alternative:** a per-part shape or filter instead of per operator. The
  per-operator controls are kept because the feedback amount is per operator.
- **Rollback:** remove the shape and filter parameters, the manifest entries and the
  migration, restore the 171 layout, and keep the measured table.
