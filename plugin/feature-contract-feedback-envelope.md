# Feature contract: feedback envelope

> Superseded by
> [`feature-contract-operator-lfos.md`](feature-contract-operator-lfos.md). The
> feedback envelope is now per operator; this contract keeps the shared-envelope
> design and measurement as history.

## Identity

- **Feature:** A shared per-part feedback envelope with a per-operator depth.
- **Brief section:** README line of research 1, and the envelope decision in
  [`../docs/research/README.md`](../docs/research/README.md).
- **Owner / date:** P1Onken, 2026-11-12.
- **Disposition:** complete. The engine, the 146 and 160 migrations, the page and
  the Linux snapshots are in and measured.

## Player outcome

- **New player action:** open the `feedback` page, shape a four-rate, three-level
  envelope, and set a depth for each operator.
- **Sound and behaviour:** the feedback amount follows the envelope. A positive
  depth opens the feedback as the envelope rises, a negative depth closes it, and
  the existing feedback knob sets the base amount. The envelope retriggers with the
  voice and is shared by the four operators of a part. At depth 0 the engine render
  is bit-exact.
- **Failure mode:** the sweep is inaudible at a useful feedback amount, the
  envelope retriggers globally instead of per voice, depth 0 changes the render, or
  a fast depth step clicks.

## Dependencies and interactions

- **Other features:** `fm_op`, `fb_tap`, `dx_env`, the existing per-part filter
  envelope, `PresetStore`, and the editor.
- **Platform/API/HAL changes:** none. The engine stays integer C.
- **Persistence/version impact:** the part block grows from 160 to 171 parameters
  and the engine manifest from 334 to 356. The `.p1lib` reader accepts the 146, 160
  and 171 layouts. The APVTS state is name-keyed, so an old state loads with the
  envelope at its defaults and the depths at 0.
- **MIDI/clock impact:** none. The new parameters are host-automatable and stay out
  of the default CC map.

## Parameter and event semantics

The envelope parameters, per part:

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `fbenv_r1` ... `fbenv_r4` | `fbenv.r1` ... `fbenv.r4` | 0..99 | 80, 60, 50, 80 | raw |
| `fbenv_l1` ... `fbenv_l3` | `fbenv.l1` ... `fbenv.l3` | 0..99 | 99, 93, 87 | raw |

The depth parameters, per operator and part:

| Parameter | Target | Range | Default | Display |
| --- | --- | --- | --- | --- |
| `op1_fb_depth` ... `op4_fb_depth` | `opN.fb.depth` | -32767..32767 | 0 | `pct_bip` |

The new part-zero engine ids are 19, 49, 79 and 109 for the four depths, and
156..162 for the seven envelope parameters. The part-one ids are 190, 220, 250 and
280 for the depths, and 327..333 for the envelope. The part block starts at 171 for
part one, the routing base moves from 324 to 346, and the total is 356.

The `fbenv` node is a voice-zone `dx_env` with the default `gate` of
`voice_gate[v]` and the default `trigger` of `voice_trig[v]`, so it is shared by the
four operators of a part and retriggers with each voice. The `fm_op` macro gains a
`fbenv` control input and a `fb_depth` parameter. The patch wires `fbenv.cv` to each
`opN.fbenv`.

`fb_tap` gains a Q15 `depth` parameter and a Q15 `fbenv` input. The block-derived
level becomes

```c
p1_clamp(
    p1_key_apply(param.level, p1_key_add(in.k, in.vk), 0, 32767)
    + p1_q15_mul(in.fbenv, param.depth),
    0, 32767)
```

At depth 0 the added term is 0, so the existing render is unchanged. The gate still
skips the body when the effective level is 0. Note ordering, voice allocation,
stealing, sustain and panic are unchanged.

### Persistence

The four depths are inserted after each operator's feedback parameter, at the old
part indices 18, 47, 76 and 105. The seven envelope parameters are inserted before
the per-part mode block, at the old index 152. The old indices 152..159 move to
163..170, so the part block is 171.

The 160 map keeps the old 0..151 with the four depth shifts, moves the old
152..159 to 163..170, and fills the new depths and the envelope at 156..162 with
their defaults. The 146 map keeps its head 0..132 at 0..136, moves the filter block
133..151 to 137..155, moves the tail 133..140 to 163..170, and fills the new depths
and the envelope. The `PresetStore` takes a list of legacy maps, 146 and 160, and
picks the one whose record size matches the bank. The current save is 171.

## Bounds and resources

- **Maximum voices/events/effects:** 20 voices and two parts, unchanged.
- **Per-sample and per-block operations:** one `dx_env` body per voice, which is the
  same shape as the filter envelope, plus one block-derived Q15 multiply per
  operator. The depth does not add a per-sample multiply.
- **Instance memory, CPU, preset size:** 22 `int32_t` parameters, 88 bytes, plus
  the APVTS entries. The `dx_env` state adds six `int32_t` fields per voice, 24
  bytes, so 20 voices add 480 bytes. Each part record grows by 44 bytes, so a
  128-slot part bank grows by 5,632 bytes.
- **Overflow/overload policy:** `fbenv` is 0..32767 and `depth` is -32767..32767.
  The Q15 product is in `[-32766, 32766]`. The sum with the 0..32767 level is in
  `[-32766, 65533]` and is clamped to 0..32767. The multiply uses the 64-bit
  helper. There is no division.
- **Division, shift, and range analysis:** no new shift. The Q15 multiply and the
  clamp are the existing helpers.

## Files and interfaces

- `modules/nodes/fb_tap/node.json` and `node.c`, for the `depth` parameter, the
  `fbenv` input and the level formula.
- `modules/macros/fm_op/macro.json`, for the `fbenv` input, the `fb_depth`
  parameter and the wire to `fb.fbenv`.
- `synth/patch.json`, for the `fbenv` node, the four wires, the manifest entries and
  the init values.
- `engine/gen/*`, regenerated from the patch.
- `plugin/preset/part_migration.h`, `PresetStore.hpp`, `PresetStore.cpp` and
  `PluginProcessor.h`, for the 146 and 160 legacy layouts.
- `plugin/tests/preset_smoke.cpp` and a new `plugin/tests/feedback_envelope.cpp`.
- `plugin/ui/ParamIDs.h`, `Pages.h` and a new `PagesFeedback.cpp`, for the page.
- `plugin/juce/PluginEditor.cpp`, `PluginProcessor.h` and `PluginProcessor.cpp`,
  for the fourth tab.
- `plugin/tests/editor_snapshot.cpp` and the Linux goldens.
- `README.md`, `plugin/README.md` and `design/mockup.html`, for the docs and the
  page mockup.

The public contract is the `fb_tap` and `fbenv` node manifests and
`kP1ParamSpecs`. The engine owns the `dx_env` voice state and the block-derived
level. The APVTS is the host-facing owner. No malloc, float, file access or display
enters the audio path.

## Editor control map

The new `feedback` tab sits after `fx / mod`:

```text
┌──────────────────────────────────────────────────────────────┐
│ feedback envelope                                            │
│                                                              │
│ [ four-stage envelope graph ]                                │
│                                                              │
│  r1    r2    r3    r4          l1    l2    l3                │
├──────────────────────────────────────────────────────────────┤
│ operator depth                                               │
│                                                              │
│  op1 [-100..+100]   op2 [...]   op3 [...]   op4 [...]        │
└──────────────────────────────────────────────────────────────┘
```

The envelope card reuses the `P1Card`, `P1Envelope` and `ParamKnob` pattern from
the filter envelope card. The depth knobs use the `pct_bip` display, so 0 is
centred and the sign is visible. The page is a full-width envelope card and a
four-knob depth card. The tab labels become `patch settings`, `operators`,
`fx / mod` and `feedback`.

## Verification

- **Host vector:** a new CTest, `feedback_envelope`, uses one sine operator at note
  60, ratio 1, level 99, a flat operator envelope, the filter off and a fresh
  engine. It renders 16,384 frames into a 2 x 16,384 buffer and analyses the left
  channel. It compares depth 0 against the current render, then a slow envelope at
  depths 0, +100 and -100 percent with a base feedback of 0 and 50 percent.
- **Tolerance/metric:** depth 0 must be bit-identical. At a 50 percent base feedback
  and +100 percent depth, the third-to-fundamental ratio or the spectral centroid in
  the last window must differ from the first by at least 10 percent. At -100 percent
  the same measure must move the other way. The dynamic peak must stay below 1.5
  times the endpoint static render and no sample may reach `INT32_MIN` or
  `INT32_MAX`.
- **Host timing and platform check:** the Linux, Windows and macOS CTest runs.
  pluginval at strictness 5 and the CLAP validator cover the host path.
- **Target package check:** the VST3, CLAP and standalone builds expose 356
  parameters. The `preset_smoke` test covers the 146, 160 and 171 layouts, the
  current round-trip and the depth and envelope defaults.
- **Editor check:** the snapshot renders the feedback page and the depth knobs.
  The goldens are regenerated on the Linux container and the diff passes with the
  12-image set.
- **Evidence artefact:** `docs/research/feedback-envelope.md`, with the
  spectral sweep table, the depth 0 bit-exact check and the build hash. The page
  snapshot is committed in the 12-image golden set.

## Risks and fallback

- **Assumptions/open research:** the envelope may be inaudible at low feedback
  because the feedback amount is already small. The test uses a 50 percent base and
  a full depth to make the effect measurable, and the listening check decides the
  useful range.
- **Safe experiment and stop criterion:** if the depth 0 render changes, if the
  +100 and -100 sweeps do not move in opposite directions, or if the dynamic peak
  grows beyond 50 percent of the endpoint static render, stop and fix the range or
  the envelope before the UI.
- **Smaller alternative:** one shared depth for the part, which costs 8 parameters
  per part instead of 11. The per-operator depth is kept because the four operators
  have different feedback amounts and the Reface-style feedback is per operator.
- **Rollback:** remove the `fbenv` node, the four wires and the eleven parameters,
  regenerate the engine, and restore the 160 layout. The UI tab and the goldens go
  with it.
