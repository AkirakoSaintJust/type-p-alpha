# Feature contract: operator LFOs and per-operator envelopes

## Identity

- **Feature:** Per-operator amplitude and signed feedback envelopes, plus the
  amplitude, feedback and detune LFOs on the new `operator lfos` tab.
- **Brief section:** `README.md` line of research 1 and 9, and
  [`../design/README.md`](../design/README.md) Operators and Operator LFOs.
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete. The engine, the two pages, the tests and the Linux
  goldens are in.

## Player outcome

- **New player action:** open `operators` for the oscillator, amplitude envelope
  and feedback envelope, or open `operator lfos` for the oscillator and the three
  per-operator LFOs. Each envelope and LFO card carries `copy` and `paste` in its
  header, so a shape or an LFO moves between operators on its own.
- **Sound and behaviour:** each operator owns its feedback envelope. The feedback
  knob, the envelope and the LFO are summed as a signed feedback level, and the sign
  chooses the family: a non-negative sum uses the current path, and a negative sum
  uses the negative feedback family. A negative envelope or LFO level can therefore
  cross zero and is audible from a zero feedback knob. The detune LFO adds a signed
  pitch offset to that operator. A negative LFO depth in bipolar mode inverts the LFO
  phase. A flat envelope, the envelope off switch and both LFOs at depth 0 keep the
  render bit-exact.
- **Failure mode:** the operator envelope leaks between operators, the flat or off
  path changes the render, the detune LFO shifts another operator, or a full-level
  envelope or LFO runs away.

## Dependencies and interactions

- **Other features:** `fm_op`, `fb_tap`, the `lfo`, `dx_env` and `pitch_env` nodes,
  the shared pitch input, the editor pages and the preset store.
- **Platform/API/HAL changes:** none. The engine stays integer C.
- **Persistence/version impact:** the part block grows from 171 to 228 parameters
  and the manifest from 356 to 470. The library stays `P1LIB2`, with the new record
  size. The 171 layout is not read; a pre-change library needs conversion outside
  the plugin.
- **MIDI/clock impact:** none. The new parameters are host-automatable and stay
  out of the default CC map.

## Parameter and event semantics

The envelope, feedback LFO and detune LFO are per operator and per part.

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `opN_fbenv_on` | `opN.fbenv.on` | 0..1 | 1 | `onoff` |
| `opN_fbenv_r1` ... `opN_fbenv_r4` | `opN.fbenv.r1` ... `r4` | 0..99 | 80, 60, 50, 80 | raw |
| `opN_fbenv_l1` ... `opN_fbenv_l4` | `opN.fbenv.l1` ... `l4` | -32768..32767 | 0, 0, 0, 0 | `pct_bip` |
| `opN_fblfo_rate` | `opN.fblfo.rate` | 0..20000000 | 389566 | `hz` |
| `opN_fblfo_depth` | `opN.fblfo.depth` | -32767..32767 | 0 | `pct_bip` |
| `opN_fblfo_wave` | `opN.fblfo.wave` | 0..11 | 0 | `wave` |
| `opN_fblfo_mode` | `opN.fblfo.mode` | 0..1 | 1 | `onoff` |
| `opN_dlfo_rate` | `opN.dlfo.rate` | 0..20000000 | 389566 | `hz` |
| `opN_dlfo_depth` | `opN.dlfo.depth` | -32768..32768 | 0 | `semi_lfo` |
| `opN_dlfo_wave` | `opN.dlfo.wave` | 0..11 | 0 | `wave` |
| `opN_dlfo_mode` | `opN.dlfo.mode` | 0..1 | 1 | `onoff` |

The feedback level is signed. The base level keeps its sign after the key and
velocity scaling, the envelope and LFO offsets are added, and the sign of the sum
chooses the family:

```c
signed = (level < 0 ? -1 : 1) * p1_key_apply(abs(level), k + vk, 0, 32767)
         + fbenv
         + fblfo;
negative = signed < 0;
level_eff = p1_clamp(abs(signed), 0, 32767);
```

The `fb_tap` node uses `negative` for the family, so a negative envelope or LFO can
cross zero even when the feedback knob is at zero. The detune LFO is added to the
operator pitch input:

```c
pitch = p1_clamp(pitch_shared + dlfo, -32768, 32767)
```

The `fm_op` macro owns one `pitch_env` for the feedback envelope, one `lfo` for each
new family, and a `csum` for the pitch pair. The feedback envelope keeps the pitch
envelope's signed Q15 levels, four rates and on switch, with its release level fixed
at zero. The `fb_tap` node gains the `lfo` input, sums both control inputs with the
signed base, and uses the sign of that sum for the family. The bipolar `lfo` mode
uses the signed depth, so a negative depth inverts the phase. At depth 0 each LFO
outputs 0, and a flat envelope outputs 0, so the existing render stays exact.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** four feedback envelopes and eight LFO
  nodes per part, all at control rate. The feedback envelope body is the existing
  `pitch_env` shape; the LFO body is the existing `lfo` shape. The feedback level adds
  one Q15 input; the pitch adds one saturation.
- **Instance memory, CPU, preset size:** the part block adds 57 parameters, so 228
  bytes per part and 456 bytes across the two parts. Four `pitch_env` states replace
  one shared state per voice, and each operator adds two LFO states, so the voice
  state grows by 42 `int32_t` fields, 3,360 bytes at 20 voices. Each library record
  grows by 228 bytes, so the 128-slot part bank grows by 29,184 bytes.
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

- **Host vector(s):** `operator_lfos` checks the bit-exact depth-0 path, the changed
  render for both LFOs, and the phase inversion from a negative depth.
  `feedback_envelope` checks the flat and off bit-exact path, the positive and
  negative family selection from a zero base, a partial negative level against an
  existing feedback amount, a negative level crossing a positive base, and the peak
  bound. `feedback_bipolar` checks the negative family and the envelope level.
  `voice_alloc` checks a three-note chord, one note's release, both part channels
  and recycled voices. `plugin_smoke` checks a three-note chord through the host
  block and restores every manifest parameter. `plugin_stability` drives every
  parameter to its bounds. `editor_snapshot` checks that the oscillator, amp
  envelope and detune LFO sections paste to another operator.
- **Browser vector (if applicable):** the mockup check covers the operators tab,
  the operator lfos tab, the four feedback envelopes with four signed levels, and
  the two new LFO cards.
- **Target package check:** the Linux and Windows builds link the editor, the
  engine and the tests.
- **Host timing and platform check:** Linux `ctest` passes 11 of 11, including the
  13-image snapshot diff, and the Windows suite passes 10 of 10.
- **Tolerance/metric:** the flat and off envelope renders and the depth-0 LFO
  renders are bit-exact. The nonzero LFO and envelope renders differ. The family
  selection and the peak bound keep their existing tolerances. The snapshot diff is
  exact.
- **Evidence artefact:** `plugin/tests/operator_lfos.cpp`,
  `plugin/tests/feedback_envelope.cpp`, `plugin/tests/voice_alloc.cpp`, the Linux
  snapshots `editor-operators.png` and `editor-operator-lfos.png`, and the mockup
  check.

## Risks and fallback

- **Assumptions/open research:** the detune LFO uses the shared pitch scale, so
  full depth is one octave. The feedback LFO is a percentage of the Q15 feedback
  range. The feedback envelope uses the pitch envelope's signed levels and on
  switch, with a fixed zero release level. The per-operator envelope is the chosen
  design.
- **Safe experiment and stop criterion:** if a flat envelope or the off switch
  changes the render, if one operator envelope or LFO reaches another operator, or
  if the pitch sum wraps, stop and fix the graph before the UI.
- **Smaller alternative:** keep the unipolar envelope and its depth knob. This was
  rejected because the feedback path is bipolar and the depth only rescales the
  same signed offset.
- **Rollback:** restore the shared `fbenv` node and the 171 layout, and remove the
  `pitch_env`, `fblfo`, `dlfo` and `dsum` nodes. The old contract remains as history.
