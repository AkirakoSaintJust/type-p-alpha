# Feature contract: operator feedback

## Identity

- **Feature:** One bipolar feedback knob per operator, with a per-operator signed
  envelope and a feedback LFO. The sign of the level selects the family, and the
  magnitude sets the amount.
- **Brief section:** the operator list in the README and research line 2, with the
  measurement in
  [`../docs/research/feedback-bipolar.md`](../docs/research/feedback-bipolar.md).
- **Owner / date:** P1Onken, 2026-11-12. Consolidated on 2026-11-20.
- **Disposition:** complete for the engine, the editor cards, the preset
  migration and the native tests.

## Player outcome

- **New player action:** turn an operator's feedback knob from -100 to +100
  percent. The sign picks the family, and the per-operator feedback envelope and
  LFO move the level inside that family.
- **Sound and behaviour:** the positive half is the traditional feedback path. The
  negative half takes the absolute value, negates it and uses the two-sample
  average, which makes it unruly at the low end and close to the positive
  character in the middle. The top is noisy by design. A zero level is silent.
- **Failure mode:** the positive path changes, the negative half is not the
  absolute-value family, the zero crossing clicks, or the top of the range runs
  away.

## Parameter and event semantics

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `opN_feedback` | `opN.fb.level` | -32767..32767 | 0 | `pct_bip` |
| `opN_fbenv_on` | `opN.fbenv.on` | 0..1 | 1 | `onoff` |
| `opN_fbenv_r1` ... `r3` | `opN.fbenv.rN` | 0..99 | 80, 60, 50 | raw |
| `opN_fbenv_l1` ... `l3` | `opN.fbenv.lN` | -32768..32767 | 99, 93, 87 percent | `pct_bip` |
| `opN_fbenv_relr`, `rell` | `opN.fbenv.*` | 0..99, -32768..32767 | 80, 0 | raw, `pct_bip` |
| `opN_fbenv_mode` | `opN.fbenv.mode` | 0..3 | 1 | raw |
| `opN_fblfo_rate`, `depth`, `wave`, `mode` | `opN.fblfo.*` | 0..20000000, -32767..32767, 0..11, 0..1 | 0, 0, 0, 0 | `hz`, `pct_bip`, `wave`, `onoff` |
| `opN_fb_ks_lo`, `hi` | `opN.fb_ks.*` | -100..100 | 0 | `pct_bip` |
| `opN_fb_vs` | `opN.fb_vs.amount` | -100..100 | 0 | `pct_bip` |

The level is signed. The magnitude drives the key and velocity followers, then the
per-operator envelope and the feedback LFO are added to the signed level:

```text
magnitude = abs(level)
followed = clamp(key_apply(magnitude, k + vk, 0, 32767), 0, 32767)
level_eff = clamp(followed + fbenv + fblfo, -32767, 32767)
```

The signal path is selected by the sign of `level_eff` after the sum:

```text
level_eff >= 0:  out = ((x + prev) * level_eff) >> 16;  prev = x
level_eff <  0:  shaped = -abs(x)
                 out = ((shaped + prev) * level_eff) >> 16;  prev = shaped
```

The negative half keeps the two-sample average, so it keeps the high-frequency
energy and the top-end noise of the positive half. The envelope and the LFO can
push the sum across zero, which flips the family. That is the current behaviour,
and it is why a deep envelope near zero is audible as a change of character rather
than a change of level.

## Research progress

The feedback feature went through three layers, and only the last is in the
product.

- **Shared envelope, 2026-11-11.** A per-part `fbenv` with a per-operator depth.
  The shared design and its measurements are in
  [`../docs/research/feedback-envelope.md`](../docs/research/feedback-envelope.md).
  It was superseded because an envelope per part cannot follow an operator's own
  amplitude and key tracking.
- **Polarity, 2026-11-12.** The sign experiment measured the negative family
  before exposing it. The sign flip is the current knob's negative half, so the
  experiment became the feature rather than a separate control.
- **Shaper, 2026-11-12.** The nonlinear feedback shaper and the one-pole filter in
  the feedback loop were measured as research line 2. They are not in the product.
  The shape and filter parameters were removed, and the evidence stays in
  [`../docs/research/feedback-shaper.md`](../docs/research/feedback-shaper.md)
  and [`../docs/research/feedback-shaper/`](../docs/research/feedback-shaper/).

The old contract note that the envelope cannot cross the sign is retired. The
signed envelope and the LFO are added before the path is chosen, so the family can
flip when the sum crosses zero.

## Dependencies and interactions

- **Other features:** `fb_tap`, `fm_op`, the per-operator `fbenv` and `fblfo`
  nodes, the operator card, the op envs page, the op lfos page and the
  preset migration.
- **Platform/API/HAL changes:** none. The engine stays integer C and the change
  sits inside the tap.
- **Persistence/version impact:** the feedback level is a signed Q15 pair per
  operator. The per-operator envelope and LFO arrived with the operator split, and
  the preset chain migrates the shared-envelope records to the per-operator
  layout.
- **MIDI/clock impact:** none. The feedback knob is host-automatable and stays out
  of the default CC map.

## Bounds and resources

- **Maximum voices/events/effects:** 24 voices and two parts.
- **Per-sample and per-block operations:** the existing two-sample average, plus
  one absolute value and one negation per operator per sample on the negative
  half. The positive half pays nothing for the branch.
- **Instance memory, CPU, preset size:** the `shaped` value and the previous
  sample are one `int32_t` each per operator per voice. The envelope and LFO state
  are part of the operator block.
- **Overflow/overload policy:** `x` is Q15, `abs(x)` is 0..32767, and the negation
  stays within the Q15 range. The level clamps to -32767..32767, and the tap shift
  follows.
- **Division, shift, and range analysis:** the absolute value has no multiply, the
  sign is applied after it, and the average follows.

## Files and interfaces

- `modules/nodes/fb_tap/node.json` and `node.c`, for the signed level and the
  absolute-value path.
- `modules/nodes/mod_env/node.json`, for the signed feedback envelope.
- `modules/nodes/lfo/node.json`, for the feedback LFO.
- `modules/macros/fm_op/macro.json`, for the per-operator wiring.
- `synth/patch.json`, for the ranges, the display and the init values.
- `engine/gen/*`, regenerated.
- `plugin/preset/part_migration.h` and `PresetStore.hpp`, for the older records.
- `plugin/tests/feedback_bipolar.cpp`, `feedback_envelope.cpp` and
  `operator_lfos.cpp`.
- `plugin/juce/Pages.h`, `PagesEnvs.cpp` and `PagesLfos.cpp`, for the cards.
- `plugin/tests/editor_snapshot.cpp` and the Linux goldens.

The public contract is the `fb_tap` node manifest and the generated parameter
manifest. The engine owns the shaped state, the APVTS is the host-facing owner,
and no malloc, float, file access or display enters the audio path.

## Editor control map

- The operator card carries the bipolar feedback knob with the `pct_bip` display.
- The op envs page carries the feedback envelope card: the on switch in the
  header, the graph, three rates and signed levels, the release rate and level,
  and the mode.
- The op lfos page carries the feedback LFO card: rate, depth, wave, mode,
  the rate and depth fades and their shared graph.

## Verification

- **Host vector:** `feedback_bipolar` renders one sine operator at note 60, ratio
  1, level 99, a flat operator envelope, the filter off and a fresh engine. It
  checks the positive and negative feedback at 25, 50, 75 and 100 percent and the
  envelope depth on the negative half. `feedback_envelope` checks that a flat
  envelope is bit-exact, that positive levels open the positive family and that
  negative levels cross below zero. `operator_lfos` checks the per-operator LFO
  routing.
- **Tolerance/metric:** the positive path stays bit-exact with the reference, the
  negative half stays the absolute-value family with the high-frequency energy
  close to the positive half, the zero crossing is silent, the envelope and LFO
  move the magnitude without an unintended family flip, the peak stays bounded,
  and no sample reaches `INT32_MIN` or `INT32_MAX`.
- **Host timing and platform check:** the Linux, Windows and macOS CTest runs, with
  pluginval and the CLAP validator covering the host path.
- **Target package check:** the VST3, CLAP and standalone builds expose every
  manifest parameter, and `preset_smoke` covers the older record layouts.
- **Editor check:** the snapshots show the bipolar knob, the feedback envelope card
  and the feedback LFO card.
- **Evidence artefact:** the native test output and the research pages listed
  above.

## Limits

- The top of the feedback range is noisy by design. The per-part filter after the
  operators is the tool for taming it.
- A signed envelope or LFO can flip the family near zero. That is intended, and
  the display keeps the signed value visible.
- The nonlinear shaper and the in-loop filter are research alternatives, not
  exposed parameters.

## Risks and fallback

- **Assumptions/open research:** the absolute value and the two-sample average are
  the chosen negative character. The square and the one-pole remain measured
  alternatives.
- **Safe experiment and stop criterion:** if the positive path changes, if the
  negative half is not the absolute-value family, or if the zero crossing clicks,
  stop and fix the path before the UI.
- **Smaller alternative:** keep the positive half only and leave the negative half
  to the research shaper.
- **Rollback:** restore the shared-envelope layout and the shaper page, and keep
  the measured WAVs and scopes.
