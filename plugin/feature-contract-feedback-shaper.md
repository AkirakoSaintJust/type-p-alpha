# Feature contract: feedback shaper

## Identity

- **Feature:** A per-operator nonlinear feedback shaper and a stronger feedback
  filter.
- **Brief section:** README line of research 2, and the nonlinear path in
  [`../design/research-feedback.md`](../design/research-feedback.md).
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** accepted as a bounded experiment. The parameter and the UI ship
  only if the measurement passes.

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
- **Persistence/version impact:** if the shaper ships, the part block grows from 171
  to 175 and the manifest from 356 to 360. The `.p1lib` reader accepts the 146, 160,
  171 and 175 layouts. The APVTS state is name-keyed.
- **MIDI/clock impact:** none. The shape choice is host-automatable and stays out of
  the default CC map.

## Parameter and event semantics

The ship phase parameter, per operator and part:

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `op1_fb_shape` ... `op4_fb_shape` | `opN.fb.shape` | 0..6 | 0 | shape names |

The choices are `off`, `square`, `square neg`, `abs`, `abs neg`, `signed square`,
`signed square neg`. The shape is inserted after the depth parameter in each
operator. The part block grows from 171 to 175. The four shapes land at 20, 51, 82
and 113 in part zero, the depths move to 19, 50, 81 and 112, the envelope moves to
160..166 and the mode block to 167..174. Part one starts at 175, its depths are
194, 225, 256 and 287, its shapes are 195, 226, 257 and 288, and its envelope is
335..341. The total is 360 and the routing base is 350.

The `fb_tap` gains a `shape` parameter. The shaped sample is

```text
off:            x
square:         (x*x) >> 15
abs:            abs(x)
signed square:  (x*abs(x)) >> 15
```

and the negative choices invert the shaped sample. The shaped sample is what the
two-sample average stores in `prev`, so the filter sees the shaped signal. The
`signed square` is odd and carries no DC. The `square` and `abs` are even and carry
a DC component, which the output blocker removes but which can shift the loop. The
experiment measures that.

The filter experiment uses a compile-time one-pole with `y += a*(x-y)` and `a` at
0.5 in one build, against the current two-sample average. A filter parameter is
added only if the measurement and the listening test show that it earns one.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** one branch and at most one multiply per
  operator per sample for the shape, plus the existing average. The one-pole adds
  one multiply and one add per operator per sample in the experiment build.
- **Instance memory, CPU, preset size:** four `int32_t` parameters, 16 bytes, in the
  ship phase. The shape reuses the `prev` state, so there is no new voice state. The
  one-pole adds one `int32_t` state per operator per voice if it ships. Each part
  record grows by 16 bytes, so a 128-slot part bank grows by 2,048 bytes.
- **Overflow/overload policy:** `x` is Q15, so `x*x` is below `2^30` and `>> 15`
  returns Q15. `abs(-32768)` is clamped to 32767. The shaped value feeds the
  existing level multiply and shift. The sign is applied after the shape, so the
  positive path stays exact.
- **Division, shift, and range analysis:** the square uses a 32-bit multiply and a
  shift of a non-negative product. The signed square multiplies two Q15 magnitudes
  and applies the sign afterwards.

## Files and interfaces

- `modules/nodes/fb_tap/node.json` and `node.c`, for the shape parameter and the
  shaped feedback.
- `modules/macros/fm_op/macro.json`, for the `fb_shape` parameter.
- `synth/patch.json`, for the manifest entries and the init values.
- `engine/gen/*`, regenerated.
- `plugin/preset/part_migration.h`, `PresetStore.hpp`, `PresetStore.cpp` and
  `PluginProcessor.h`, for the 171 legacy map.
- `plugin/tests/preset_smoke.cpp`, and a new `plugin/tests/feedback_shaper.cpp`.
- `plugin/ui/ParamIDs.h`, `PagesFeedback.cpp`, and the `ParamChoice` names.
- `plugin/ui/Widgets.cpp`, for the shape display names.
- `plugin/tests/editor_snapshot.cpp` and the Linux goldens.
- `plugin/evidence/feedback-shaper.md` and the README links.

The public contract is the `fb_tap` node manifest and `kP1ParamSpecs`. The engine
owns the shaped feedback state. The APVTS is the host-facing owner. No malloc,
float, file access or display enters the audio path.

## Editor control map

The feedback page's operator depth card gets a shape choice under each depth knob:

```text
operator depth
op1 [depth]        op2 [depth]        op3 [depth]        op4 [depth]
    [off v]            [square v]         [abs v]            [signed v]
```

The depth knob keeps the `pct_bip` display. The shape is a `ParamChoice` with the
seven names. The card height grows to fit the choice row.

## Verification

- **Host vector:** a new CTest, `feedback_shaper`, uses one sine operator at note
  60, ratio 1, level 99, a flat operator envelope, the filter off and a fresh
  engine. It renders 16,384 frames into a 2 x 16,384 buffer and analyses the left
  channel. It measures the shape choices at 25, 50 and 75 percent feedback, with the
  current average and the one-pole build.
- **Tolerance/metric:** the `off` shape must be bit-identical to the current path.
  `square` and `abs` must change the spectrum relative to `off`, and their negative
  forms must differ from the positive forms. The one-pole must lower the energy
  above 8 kHz at 75 percent feedback without moving the fundamental. The peak must
  stay inside a 50 percent margin of the static endpoint. No sample may reach
  `INT32_MIN` or `INT32_MAX`.
- **Host timing and platform check:** the Linux, Windows and macOS CTest runs.
  pluginval and the CLAP validator cover the host path.
- **Target package check:** the VST3, CLAP and standalone builds expose 360
  parameters. The `preset_smoke` test covers the 146, 160, 171 and 175 layouts.
- **Editor check:** the snapshot renders the shape choices and the new card height.
  The goldens are regenerated on the Linux container.
- **Evidence artefact:** `plugin/evidence/feedback-shaper.md`, with the spectral
  table, the sign comparison, the high-frequency energy and the build hash.

## Risks and fallback

- **Assumptions/open research:** the source is a Csound experiment and a reader
  comment, not a Reface teardown. The `abs` shape is a hypothesis. The square and
  the signed square cover the neighboring possibilities.
- **Safe experiment and stop criterion:** if the shaper is not sign-dependent, if
  the one-pole does not reduce the high-frequency energy, or if the even shapes
  detune the voice enough to hear, stop and do not ship the parameter.
- **Smaller alternative:** a fixed shaper or a fixed lowpass with no parameter, or
  a per-part shape instead of per operator.
- **Rollback:** remove the shape parameter, the manifest entries and the migration,
  restore the 171 layout, and keep the measured table.
