# Feature contract: operator LFOs and per-operator envelopes

## Identity

- **Feature:** Per-operator amplitude and feedback envelopes, plus the amplitude,
  feedback and detune LFOs on the new `operator lfos` tab.
- **Brief section:** `README.md` line of research 1 and 9, and
  [`../design/README.md`](../design/README.md) Operators and Operator LFOs.
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete. The engine, the two pages, the tests and the Linux
  goldens are in.

## Player outcome

- **New player action:** open `operators` for the oscillator, amplitude envelope
  and feedback envelope, or open `operator lfos` for the oscillator and the three
  per-operator LFOs.
- **Sound and behaviour:** each operator owns its feedback envelope. The feedback
  LFO adds a signed offset to the feedback level, and the detune LFO adds a signed
  pitch offset to that operator. Depth 0 keeps both LFO paths and an untouched
  feedback envelope bit-exact.
- **Failure mode:** the operator envelope leaks between operators, depth 0 changes
  the render, the detune LFO shifts another operator, or a full-depth LFO runs
  away.

## Dependencies and interactions

- **Other features:** `fm_op`, `fb_tap`, the `lfo` and `dx_env` nodes, the shared
  pitch input, the editor pages and the preset store.
- **Platform/API/HAL changes:** none. The engine stays integer C.
- **Persistence/version impact:** the part block grows from 171 to 224 parameters
  and the manifest from 356 to 462. The library stays `P1LIB2`, with the new record
  size. The 171 layout is not read; a pre-change library needs conversion outside
  the plugin.
- **MIDI/clock impact:** none. The new parameters are host-automatable and stay
  out of the default CC map.

## Parameter and event semantics

The envelope, feedback LFO and detune LFO are per operator and per part.

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `opN_fbenv_r1` ... `opN_fbenv_r4` | `opN.fbenv.r1` ... `r4` | 0..99 | 80, 60, 50, 80 | raw |
| `opN_fbenv_l1` ... `opN_fbenv_l3` | `opN.fbenv.l1` ... `l3` | 0..99 | 99, 93, 87 | raw |
| `opN_fb_depth` | `opN.fb.depth` | -32767..32767 | 0 | `pct_bip` |
| `opN_fblfo_rate` | `opN.fblfo.rate` | 0..20000000 | 389566 | `hz` |
| `opN_fblfo_depth` | `opN.fblfo.depth` | -32767..32767 | 0 | `pct_bip` |
| `opN_fblfo_wave` | `opN.fblfo.wave` | 0..11 | 0 | `wave` |
| `opN_fblfo_mode` | `opN.fblfo.mode` | 0..1 | 1 | `onoff` |
| `opN_dlfo_rate` | `opN.dlfo.rate` | 0..20000000 | 389566 | `hz` |
| `opN_dlfo_depth` | `opN.dlfo.depth` | -32768..32768 | 0 | `semi_lfo` |
| `opN_dlfo_wave` | `opN.dlfo.wave` | 0..11 | 0 | `wave` |
| `opN_dlfo_mode` | `opN.dlfo.mode` | 0..1 | 1 | `onoff` |

The feedback LFO is added after the feedback envelope term:

```c
level_eff = p1_clamp(
    p1_key_apply(abs(level), p1_key_add(k, vk), 0, 32767)
    + p1_q15_mul(fbenv, depth)
    + fblfo,
    0, 32767)
```

The detune LFO is added to the operator pitch input:

```c
pitch = p1_clamp(pitch_shared + dlfo, -32768, 32767)
```

The `fm_op` macro owns one `dx_env` for the feedback envelope, one `lfo` for each
new family, and a `csum` for the pitch pair. The `fb_tap` node gains the `lfo`
input. At depth 0 each LFO outputs 0, so the existing render stays exact.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** four feedback envelopes and eight LFO
  nodes per part, all at control rate. The feedback envelope body is the existing
  `dx_env` shape; the LFO body is the existing `lfo` shape. The feedback level adds
  one Q15 input; the pitch adds one saturation.
- **Instance memory, CPU, preset size:** the part block adds 53 parameters, so 212
  bytes per part and 424 bytes across the two parts. Four `dx_env` states replace
  one per voice, and each operator adds two LFO states, so the voice state grows by
  42 `int32_t` fields, 3,360 bytes at 20 voices. Each record grows by 212 bytes, so
  the 128-slot part bank grows by 27,136 bytes.
- **Overflow/overload policy:** the feedback LFO is Q15 and the feedback level is
  clamped to 0..32767. The detune LFO is Q15 and the pitch sum is clamped. The
  existing peak-bound tests cover the extremes.
- **Division, shift, and range analysis:** no new division or shift. The LFO and
  csum helpers are existing code.

## Files and interfaces

- **Engine:** `modules/nodes/fb_tap/node.json` and `node.c`,
  `modules/macros/fm_op/macro.json`, `synth/patch.json`, and the regenerated
  `engine/gen/*`.
- **Editor:** `plugin/ui/ParamIDs.h`, `plugin/juce/Pages.h`,
  `plugin/juce/PagesOperators.cpp`, `plugin/juce/PluginEditor.cpp`,
  `plugin/juce/PluginEditor.h` and `plugin/juce/CMakeLists.txt`.
- **Tests:** `plugin/tests/operator_lfos.cpp`, `plugin/tests/feedback_envelope.cpp`,
  `plugin/tests/preset_smoke.cpp`, `plugin/tests/plugin_smoke.cpp`,
  `plugin/tests/plugin_stability.cpp` and `plugin/tests/editor_snapshot.cpp`.
- **Design:** `design/README.md`, `design/juce.md` and `design/mockup.html`.
- **Public contract:** the `fm_op` and `fb_tap` manifests and `kP1ParamSpecs`.

## Verification

- **Host vector(s):** `operator_lfos` checks the bit-exact depth-0 path and the
  changed render for both LFOs. `feedback_envelope` checks the per-operator envelope
  direction, depth and peak bound. `plugin_smoke` restores every manifest parameter
  and the global library. `plugin_stability` drives every parameter to its bounds.
- **Browser vector (if applicable):** the mockup check covers the operators tab,
  the operator lfos tab, the four feedback envelopes and the two new LFO cards.
- **Target package check:** the Linux and Windows builds link the editor, the
  engine and the tests.
- **Host timing and platform check:** Linux `ctest` passes 10 of 10, including the
  14-image snapshot diff.
- **Tolerance/metric:** the depth-0 renders are bit-exact. The nonzero LFO renders
  differ. The feedback envelope direction and peak bound keep their existing
  tolerances. The snapshot diff is exact.
- **Evidence artefact:** `plugin/tests/operator_lfos.cpp`,
  `plugin/tests/feedback_envelope.cpp`, the Linux snapshots
  `editor-operators.png` and `editor-operator-lfos.png`, and the mockup check.

## Risks and fallback

- **Assumptions/open research:** the detune LFO uses the shared pitch scale, so
  full depth is one octave. The feedback LFO is a percentage of the Q15 feedback
  range. The per-operator envelope is the chosen design.
- **Safe experiment and stop criterion:** if depth 0 changes the render, if one
  operator envelope or LFO reaches another operator, or if the pitch sum wraps,
  stop and fix the graph before the UI.
- **Smaller alternative:** keep the shared feedback envelope and omit the two new
  LFOs. This was rejected because the new page needs both LFO families and the
  envelope belongs to the operator.
- **Rollback:** restore the shared `fbenv` node and the 171 layout, and remove the
  `fblfo`, `dlfo` and `dsum` nodes. The old contract remains as history.
