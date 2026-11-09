# Synth brief: Type P.fm1

> One brief as the only reference. `patch.json` + `panel.json` are the
> translation of this brief. `dsp-algorithm.md` holds the math.

## 1. Instrument and player

- **Working name:** type-p-fm1 (a 4-operator Yamaha FM synth).
- **Who plays it and in what setting:** a desktop/tabletop FM voice in the
  OPZ/DX21 lineage, with pads, bells, EPs and basses played from the keys or MIDI.
- **What should it feel like to play:** immediate, bright-but-tameable, with the
  algorithm knob as the headline gesture and the filter as the softening stage.
- **Three reference sounds:** (1) a DX7-style E-piano (algorithm 4, two
  2-operator stacks), (2) a clangorous bell (algorithm 0, deep stack + feedback), and
  (3) an additive organ pad (algorithm 7, all four carriers).
- **Explicit non-goals:** no effects beyond the filter (patch
  persistence, SysEx, and velocity scaling are now shipped, see §4/§5).

## 2. Sound and voice architecture

- **Synthesis method(s):** 4-operator phase-modulation FM, per-operator
  self-feedback, 12 waveforms per operator (the OPZ + OPL union), rate/level
  envelopes.
- **Polyphony and voice stealing:** 20 voices. Oldest-steal on overflow.
- **Pitch range and tuning:** 12-TET, full MIDI note range. Per-op `ratio`
  (note-derived, non-integer ratios stay in tune).
- **Oscillators / operators / noise:** 4 operators, each `phasor → osc →
  dx_env → vca` with 1-sample self-feedback (`fm_op` macro).
- **Filter and resonance:** one global `liquid_filter` ladder (FILTER 1), `mode`
  selectable (LP4/BP/HP), `res` = ladder feedback, with an **on/off bypass** and an
  **ADSR envelope routed to cutoff**.
- **Envelopes and velocity:** DX rate/level (`dx_env`, 4 rates + 3 levels).
  velocity → amplitude/FM/envelope-rate is wired, but the test project defaults
  the depths to 0 (off), settable via `set_param` (`opN.velamp.depth` etc.),
  not on the panel.
- **Modulation:** the routing matrix (8 algorithms) selects which operator
  phase-modulates which, feedback is self-modulation, and there is one **per-part
  pitch LFO** (vibrato), one **per-note pitch envelope** (the "pitch blip": a `base`
  offset that decays back at note-on), and one **amplitude LFO per operator**
  (tremolo, per-voice).
- **Effects and output stage:** carrier sum → FILTER 1 (`liquid_filter`: LP4/BP/HP
  + bypass + a cutoff ADSR) → `dc_block` → `out`, where the `MASTER` pot is the
  volume and there is no output VCA.

### 2.1a The output chain (filter), specified

- **Filter (`liquid_filter`).** Params: `mode` (0 = LP4, 1 = the 2-pole band
  tap, 2 = the 2-pole HP tap), `on` (0 = bypass = pass the input straight
  through), `f` (the per-stage coefficient, Q16), `res` (the ladder feedback,
  Q16), `comp` (the passband makeup, Q15). The ladder reaches its strongest
  stable resonance at the top of `res`. A **cutoff envelope** (F-ENV), a global `dx_env` (the Yamaha
  rate/level envelope: 4 rates + 3 levels, gated by `patch_gate` and retriggered
  by `patch_trig` on every note-on) whose Q15 output is doubled to Q16, scales a
  `fenv_depth`, and is added to `f`. The sum is clamped at the ladder's stable
  coefficient ceiling. On a note-on it restarts and opens the filter. On note-off
  it closes. The filter and the F-ENV are global, so a new note restarts the
  shared shape.

### 2.1b Per-part voice modes (moved from global)

`mode` (0 = poly, 1 = mono), `unison_count` (the mono stack size, 1 = plain mono),
`unison_detune` (the spread between the stacked copies, in cents), `detune` (the
part detune in cents, -50..50), `transpose` (the part transpose in semitones,
-24..24), `legato`, `portamento` (time), `note_priority` become **per-part** params
(each part its own poly/mono, stack, tuning, and times), exposed in `GLO`. This is
a breaking change to the voice-mode param layout: they move from the global set to
a per-part array.

### 2.1 Voice architecture (the node/macro graph)

- **Voice-zone graph** (20 voices): four `fm_op` instances `op1..op4`, four
  `delay1` instances `d1..d4`, one `route` instance:
  ```
  op1.audio → d1.in      op2.audio → d2.in
  op3.audio → d3.in      op4.audio → d4.in
  d1.out → route.a   d2.out → route.b   d3.out → route.c   d4.out → route.d
  route.mod2 → op2.fm   route.mod3 → op3.fm   route.mod4 → op4.fm
  voice_out = route.car
  ```
  The four `delay1` make the graph acyclic for *every* algorithm (all modulation
  reads the 1-sample-delayed output of its sources).
- **Global-zone graph:** `algo` (the algorithm selector) → `alg` into the `route`
  node. `lp` (`liquid_filter`) → `dc` (`dc_block`) → `out`:
  `voice → lp.audio`, `lp.out → dc.in`, `dc.out → out`.
- **Voice-zone additions:** `penv` (a per-note `pitch_env`, retriggered on every
  note-on) + `plfo` (the per-part vibrato LFO) + `psum` (a `csum`):
  `plfo.cv + penv.cv → op1..op4.pitch`.
- **Macros:** `fm_op` (shipped, `modules/macros/fm_op/`), the operator, with
  params `ratio` (note-derived), `detune` (cents −50..50), `waveform`, `level`,
  `feedback` (per-op self-modulation), `trem_rate`, `trem_depth`, `r1..r4`,
  `l1..l3` mapped inward, and inputs `fm` (audio) + `pitch` (control).
- **Voice count / sample rate:** 20 voices / 44100 Hz (the default).
- **Feedback paths:** each operator's self-feedback is one `delay1` (the
  **oscillator** output × feedback → sum with external fm + pitch → `ph.fm`,
  tapped *pre*-envelope/pre-tremolo so the amplitude LFO never leaks into pitch).
  Feedback is **per-op only**: `opN_feedback` (its own self-modulation, on the op
  page) scales that operator's delayed oscillator output. (The earlier
  carrier-gated `feedback` master, the scrapped HOME knob that pushed a master
  amount onto whichever operator was the current carrier, is gone. It left a
  spurious feedback floor on every carrier even at zero.)

### 2.2 Parameter manifest

> The friendly names below are what the panel exposes. The engine is
> **two-timbre** (`parts: 2`, channels 0 and 1): every voice-zone param is a
> per-part array, so the full id space is **141 params per part** × 2 parts +
> **23 global** (the output chain and the shared settings) + 10 routing = **315
> `set_param` ids**, part-major (`part × param`). The patch's
> `cc:` list exposes 55 of them (part 0's timbre + the global FX + both parts'
> mode params) as the fixed CC surface. Part 1's timbre comes from the
> multi-patch / SysEx. The eight voice-mode params (`mode`, `unison_count`,
> `unison_detune`, `detune`, `transpose`, `legato`, `portamento`,
> `note_priority`) are **per-part** (each part its own poly/mono, stack, tuning,
> and times).

| Param (name) | Target | Min | Max | Curve | Unit | Display |
| --- | --- | --- | --- | --- | --- | --- |
| algorithm | algo.algorithm | 0 | 7 | linear | | raw (0–7) |
| cutoff | lp.f | 0 | 55296 | log | | off_pct ("off" at 0) |
| res | lp.res | 0 | 41942 | log | | pct_exp (0.00–100.00%, fine near the top) |
| filter_type | lp.mode | 0 | 2 | linear | | raw (0=LP 1=BP 2=HP) |
| filter_on | lp.on | 0 | 1 | linear | | onoff (off/on) |
| fenv_r1 | fenv.r1 | 0 | 99 | linear | | raw (Yamaha rate) |
| fenv_r2 | fenv.r2 | 0 | 99 | linear | | raw (Yamaha rate) |
| fenv_r3 | fenv.r3 | 0 | 99 | linear | | raw (Yamaha rate) |
| fenv_r4 | fenv.r4 | 0 | 99 | linear | | raw (Yamaha rate) |
| fenv_l1 | fenv.l1 | 0 | 99 | linear | | raw (Yamaha level) |
| fenv_l2 | fenv.l2 | 0 | 99 | linear | | raw (Yamaha level) |
| fenv_l3 | fenv.l3 | 0 | 99 | linear | | raw (Yamaha level) |
| fenv_depth | lp.fenv_depth | 0 | 65535 | linear | | raw |
| attack | op1.eg.r1 | 0 | 99 | linear | | raw (0–99 rate) |
| release | op1.eg.r4 | 0 | 99 | linear | | raw (0–99 rate) |
| pitch_rate | plfo.rate | 0 | 20000000 | log | | hz |
| pitch_depth | plfo.depth | -32768 | 32768 | linear | | semi_lfo (-12.00–+12.00 st) |
| pitch_mode | plfo.mode | 0 | 1 | linear | | onoff (uni/bi) |
| penv_on | penv.on | 0 | 1 | linear | | onoff (off/on) |
| penv_r1 … penv_r4 | penv.rN | 0 | 99 | linear | | raw (0–99 rate) |
| penv_l1 … penv_l3 | penv.lN | -32768 | 32767 | linear | | semi (±st) |
| penv_l4 | penv.l4 | -32768 | 32767 | linear | | semi (±st, release end) |
| op1_level … op4_level | opN.tl.level | 0 | 99 | linear | | raw (0–99) |
| op1_waveform … op4_waveform | opN.osc.waveform | 0 | 11 | linear | | raw (0–11) |
| op1_ratio … op4_ratio | opN.ph.inc_ratio | 32768 | 1048576 | linear | | ratio (1.00 = base) |
| op1_detune … op4_detune | opN.ph.detune | -50 | 50 | linear | | cent (±c) |
| op1_mute … op4_mute | opN.mute.mute | 0 | 1 | linear | | onoff (off/on) |
| op1_feedback … op4_feedback | opN.fb.level | 0 | 32767 | linear | | pct |
| op1_trem_rate … op4_trem_rate | opN.lfo.rate | 0 | 20000000 | log | | hz |
| op1_trem_depth … op4_trem_depth | opN.lfo.depth | -99 | 99 | linear | | raw (level steps) |
| op1_trem_mode … op4_trem_mode | opN.lfo.mode | 0 | 1 | linear | | onoff (uni/bi) |

The `display` column is the per-param on-screen form (the `params:` `display`
field → a `ui_param_disp` entry in the generated panel). A relative encoder steps
per the curve (linear = a fraction of the range, log = multiplicative), so each
knob reaches its whole range in a sane number of turns and clamps at the ends.

(`level` is the Yamaha total-level (0–99, 0.75 dB/step: 99 = max, 50 ≈ −37 dB,
0 = silence) that gates the operator's
output with −12 dB single-operator headroom (`fm1_level_gain[level] >> 2`), so a
solo carrier never clips and a four-carrier additive algorithm sums to ~0 dBFS,
whilst `opN_feedback` is each operator's own self-modulation depth. The DX rates
(`r1..r4`, `penv_r1..r4`) are 0–99 → a one-pole coefficient via
`fm1_rate_coeff` (a non-uniform Yamaha-style curve: 99 = instant ≈ 0.73 ms, 0 ≈ 7 s),
and the DX levels (`l1..l3`) are 0–99 → Q15 gain via `fm1_level_gain` (0.75 dB/step).
`rate` is a Q32 phase increment per sample, `f · 2³² / 44100`, so ~97392 = 1 Hz,
whilst `depth` is in the target's units. For `pitch_depth` it is a Q15 semitone
offset (-32768..32768, displayed as -12.00..+12.00 st), and for `trem_depth` it is
a signed operator-level offset in the same 0..99 units as `LEVEL`. The LFO `mode`
is 0 for unipolar (start at the target and move to the depth) and 1 for bipolar
(swing ±depth around the target). `ratio` is the q16 frequency multiplier,
`ratio/65536` = the multiplier (65536 = 1.0, 32768 = 0.5), and the per-note pitch
envelope (`penv`) retriggers on every note-on, releases on note-off to `l4`, and is
off by default. Its levels `l1..l4` are q15 pitch offsets (32768 = +1 octave,
2731 ≈ +1 semitone), and all of it runs at block rate.)

The init patch sets `op1_level_ks_hi` to -50, `op1_level_ks_lo` to -50, and
`op3_level_ks_lo` to -50, so a modulator narrows at the top and the bottom of the
keyboard. In the FM algorithms operators 1 and 3 are modulators, so the sidebands
fold less at C5 to C7 and the high sidebands at the low notes drop by 10 to 20 dB.
The OPM converter still fits the `KS` field per voice, and a hand-made patch can
change the amount or turn it off.

### 2.3 Tables

- **`routing` → kind `routing`**, the 8-algorithm 4-op FM matrix (8 rows × 5
  sub-rows × 4 Q15 0/1 weights = 160 entries), ymfm's `s_algorithm_ops`
  {0,1,2,3,4,5,6,7} (the full standard TX81Z/DX100/DX21 4-op set).
- **Wave table:** waves 9 to 11 (pulse-sine, square, saw) each carry a nine-step
  mip map, from 256 harmonics at the bottom of the keyboard down to one at the
  top. The table is const XIP, and the read is still one load.

### 2.4 Scope taps

`taps: ["op1.audio", "op2.audio", "op3.audio", "op4.audio"]`, four per-operator
audio tap points (resolved to each operator's post-mute output), exposed per part
as `N_TAP × N_PART` = 8 channels via `type_p_fm1_dsp_tap(ch)` (`ch = tap × 2 +
part`, the last block's Q15 samples). These feed the HOME 4-op scope/spectro
pages (§3.3): the active part's four operators are its four channels, so the
"active part" label stays true when both parts layer.

## 3. Controls and display

### 3.1 The control surface (fixed)

`MASTER` = volume (platform), `ALGORITHM` = always-on algorithm encoder, `KNOB1–4`
= the current page's params, and `SELECT` = page up/down (wrapping). The buttons:
`EDIT` (operator pitch), `ENV` (envelopes), `LFO` (modulation), `FX` (filter), `GLO` (parts + patch), `HOME` (the visual pages), `ARP`/`SEQ`/`PLAY`/`REC`
(operator mutes 1–4), `SEL` (operator select + part switch + part mute combos),
`PRESETS` (the patch-browser popup), `OCT−`/`OCT+` (shift the keybed a whole
octave, ±2, firmware-level). The keybed plays notes, and `SEL`+key is the mute
chord.

### 3.2 Always visible (every page, a fixed header)

The header is two lines and line 1 shows, at all times, every **part** (`P1 P2`) and
**operator** (`op 1 2 3 4`), each with its **ON/OFF** (muted, red) state and the
active one highlighted, the **current algorithm** (`ALG n`), and the **current menu
name + page number/total** (`EDIT 1/2`). Line 2 is the **page description** (the
page's first widget, e.g. `OSCILLATOR`, `OP DETUNES`), and the colours run parts +
operators white, the selected part/operator light blue, `ALG` + values hot pink,
muted red, menu/page purple, and the description bright green. No page may hide any
of it.

### 3.3 Contexts (menus) and their pages

- **HOME** (visual, column layout, each widget a row, part 1 / op 1 on top):
  1. envelope graphs of all 4 operators of the active part.
  2. oscilloscope of all 4 operators of the active part.
  3. spectrogram of all 4 operators of the active part.
  4. oscilloscope of both parts.
  5. spectrogram of both parts.
- **EDIT** (the active operator): ratio · level · feedback · waveform + a live
  scope/spectro (one page). Then **PART n OP DETUNES**, all four operators'
  detunes (±50 cents) of the active part.
- **ENV**: the active operator's **amplitude envelope** in two pages, rates
  (r1/r2/r3/r4) then levels (l1/l2/l3). Then the **per-part pitch envelope**
  (base + rate). The envelope graph is drawn on both amplitude-envelope pages.
- **LFO**: the active operator's **amplitude LFO** (rate + depth). Then the
  **per-part pitch LFO** (vibrato rate + depth).
- **FX** (the output chain): **filter**, on/off, cutoff, rez, type, and an ADSR
  envelope routed to cutoff.
- **GLO** (parts + patch): per-part **level** (scales the part's maximum
  amplitude) and **delay**. Per-part **voice mode** (poly / mono, with the mono
  stack's voice count + detune, legato, portamento time). Part
  **name**, MIDI **channel**, **keyboard split**, **INIT PART**. **save/load a
  single part into either part** from the preset store. Multi-patch settings,
  **multi-patch name**, **INIT PATCH** (additive algorithm, gate-type envelopes,
  waveform 0, no feedback, ratio 1, all ops ON, part 2 OFF, part 1 active,
  level 75).

### 3.4 The shortcuts and modifiers

`SEL` **taps** the next active operator. A **double-tap** switches the active
part. **Holding `SEL` + a key** mutes that operator (chord, not a note). **Holding
`SEL` + `ARP` or `PLAY`** toggles part 1 mute. **`SEL` + `SEQ` or `REC`** toggles
part 2 mute. `ARP`/`SEQ`/`PLAY`/`REC` (plain) toggle operator mutes 1–4, the
old mute sub-page is gone. The `PRESETS` encoder opens the **multi-patch (2-part)
preset browser** as a popup. After 2 s with no preset-encoder input, the selected
patch loads and the popup clears (unless it is already loaded, then it just
clears). Single-part presets load from the `GLO` menu. `SAVE` opens the **save
chooser**, a two-row `single part` / `multi-patch` dialog (`KNOB1` = row,
`KNOB2` = slot, `SAVE` = commit, `HOME` = cancel. No idle timeout). Saving a
multi-patch then asks what to do with the active part's live edits, **discard
edits**, **save as a new part**, or **mutate in place** (`KNOB1` picks, `SAVE`
commits, `HOME` goes back).

### 3.5 Screen rules

Retained-mode, ~30 Hz redraw, no display in the audio callback, full clear on
page/context change, held notes stay playable. Per-param display scale (ratio
`1.00`, on/off, `%`, Hz, semitones) as §2.2.

## 4. MIDI and connectivity

- **MIDI in / out:** in, notes, CC (the `cc:` list, sustain CC64, panic CC123),
  pitch bend (±2 semitones). Out, the local keys echo on part 0's channel.
- **Channels:** two parts, part 0 = channel 0, part 1 = channel 1 (a note
  routes to every part whose channel + keyzone match. Layering allowed).
- **Velocity-zero note-off, sustain, panic:** velocity-zero = note-off. Sustain
  defers note-offs until the pedal lifts. `all_notes_off` (CC123) = panic.
- **Clock and transport:** none (ignored).
- **USB roles:** MIDI in/out. Firmware update via the normal flash path.
- **Overflow and back-pressure:** voice stealing (oldest) on >20 voices.

## 5. State and persistence

- **What survives reboot:** 128 single parts + 64 multi patches + the settings
  record (base channel, master tune), saved to flash **only when silent** and
  only on an explicit save. The multi-patch in slot 0 applies at boot.
- **Patch/sequence format and version:** the preset records (`docs/roadmap.md`
  §6.11.4): `u8 version | name[16] | params | crc32`, 128 single-part slots +
  64 multi-patch slots (chunked across the slot area's measured 16 372 B
  ceiling).
- **Storage budget and CRC/commit rule:** per-record CRC-32 + the area CRC.
  A corrupt record is skipped, never the bank.
- **Backup, import/export, migration, rollback:** `umetate preset export/import`
  (the bank ↔ `.syx`), and the five SysEx commands (identity / param dump /
  single-part / multi-patch / settings).

## 6. Browser prototype

- **What you want to hear or compare locally:** the 8 algorithms side-by-side.
  Confirm the routing matrix sounds like the DX algorithm family.
- **Which parameters and MIDI controls the page needs:** algorithm + the 5
  output-chain params. Keys via mouse/MIDI.
- **Supported browsers and offline expectation:** any WebAudio browser. Offline
  once built.
- **What the browser must not be used to prove:** device performance, the WASM
  build is the parity lab, not the device.

## 7. Acceptance and evidence

- **Must-have features:** 8 runtime algorithms. 12 waveforms. Per-op + carrier
  feedback. Rate/level envelopes. A per-part pitch LFO + per-note pitch envelope +
  per-op amplitude LFO. The filter output chain. A panel. **two
  timbres** (parts), **presets** (128+64 + settings), **SysEx**, **MIDI CC /
  sustain / panic / bend**.
- **Optional features (shipped, defaulted off):** velocity scaling (the depths
  default to 0).
- **Host tests:** `umetate deep-check --all`, mypy + ruff + pyright, the four
  test modules (`test_generate` renders the patch + proves the 8 algorithms +
  resolves the panel. `test_cli` runs the `build --synth` composition), plus
  `umetate budget` (the measured polyphony figure).
- **Target package checks:** `umetate build --synth examples/type-p-fm1` →
  `umetate-fm1-pro-type_p_fm1.fwsc` (82 060 B image, `.dsp_text` in SRAM). The
  JieLi build's own checks green.
- **Emulator:** `umetate emu --check --mhz 240` boots the package and holds a
  chord, no halt, full-scale audio peak.
- **Physical timing and device checks:** device acceptance is `blocked`
  (hardware ~10 days out).
- **Known unknowns:** the liquid ladder's coefficient ceiling at 55296. Above it
  the feedback loop breaks into a parasitic oscillation at moderate resonance, so
  the node clamps there. The top of the `res` range is the strongest stable
  resonance.

## 8. Confirmation

- **Brief owner:** P1Onken.
- **Date confirmed:** 2026-10-27.
- **Open questions that still change architecture:** none, the 8-algorithm set
  (ymfm `s_algorithm_ops` 0..7) is locked in `dsp-algorithm.md`. The F7 additions
  (two timbres, presets, SysEx, the `cc:` surface, the app glue) are locked in
  the roadmap §6.11.
