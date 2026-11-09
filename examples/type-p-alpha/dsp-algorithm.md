# DSP algorithm spec, type-p-fm1

> One section per DSP component (oscillator, filter, envelope, …). This is the
> *math* the `patch.json`/`node.c` translation follows: when it is complete, a fresh
> agent can write the code without re-deriving anything.

> **Research first.** Record what was consulted and **give credit**: distinguish
> *consulting* (learn the technique) from *copying/deriving* (preserve the source's
> notice and record it in `LICENSING.md`). Check each source's licence before
> copying. Incompatible licences are consulted, never copied.

> **Performance + memory safety are standing constraints:** bounded loops, no
> allocation, no 64-bit division, no `double`, no per-sample dispatch in the hot
> loop. One writer per shared value or a snapshot protocol.

## Default reference points

| Source | URL | Licence (verify at use) |
| --- | --- | --- |
| ymfm (MAME 3rdparty) | https://github.com/mamedev/mame/tree/master/3rdparty/ymfm | BSD-3-Clause |
| SuperCollider DSP (UGens) | https://github.com/supercollider/supercollider | GPL-3.0-or-later |
| Mutable Instruments (eurorack) | https://github.com/pichenettes/eurorack | MIT |
| Synthesis Toolkit (STK) | https://github.com/thestk/stk | permissive (see LICENSE) |
| maximilian | https://github.com/micknoise/Maximilian | see LICENSE |
| music dsp archives | https://www.musicdsp.org/en/latest/ | per-snippet (see each) |

In-repo study: [`docs/dsp-study.md`](../../docs/dsp-study.md).

---

## Component: `dx_env` (rate/level envelope)

- **Purpose:** the FM operator envelope, the DX/TX rate/level shape that gives
  4-op FM its attack/decay/sustain/release character, and it replaces the shipped
  `env` node's linear attack/release.

- **Algorithm** (fixed-point, exactly):
  `level` is a Q15 gain (0…32767), and four stages each run an exponential one-pole
  toward a target (`level += (target − level)·rate / 2¹⁶`, via `fm1_onepole`):

  | stage | target | advance when | on advance |
  | --- | --- | --- | --- |
  | 0 attack | `l1` (peak) | `level ≥ l1−8` | level := l1, stage := 1 |
  | 1 decay1 | `l2` | `level ≤ l2+8` | level := l2, stage := 2 |
  | 2 decay2 | `l3` (sustain) | `level ≤ l3+8` | level := l3, stage := 3 |
  | 3 sustain |, | hold | level := l3 |
  | 4 release | `0` | `level < 8` | level := 0 |

  Gate edges drive the stage, so gate↑ from release goes to stage 0 (level := 0) and
  gate↓ from any non-release stage goes to stage 4, whilst a `trigger` edge (default
  `voice_trig[v]`) resets to stage 0 regardless of stage, because a note-on
  retriggers even a stolen voice.

- **Inputs / outputs:** `gate` (event, default `voice_gate[v]`), `trigger` (event,
  default `voice_trig[v]`), `rate_scale` (control, default 32768, scales every
  stage's one-pole coefficient, e.g. velocity → envelope rate), `cv` (control, Q15).

- **Parameters** (Q-format, range, default, rate, modulatable):

  | param | kind | range | meaning |
  | --- | --- | --- | --- |
  | `r1 r2 r3 r4` | rate | 0…99 | one-pole coefficient `fm1_rate_coeff[rate]` (non-uniform Yamaha-style: 99 = instant ≈ 0.73 ms, 0 ≈ 7 s) |
  | `l1` | level | 0…99 | attack peak (→ `fm1_level_gain[l1]`) |
  | `l2` | level | 0…99 | decay1 target |
  | `l3` | level | 0…99 | sustain level |

  All block rate and modulatable, with the rate as a table-looked one-pole
  coefficient and the level as a table-looked gain (0.75 dB/step: 99 = max, 50 ≈
  −37 dB, 0 = silence), so the UI knobs are linear 0–99.

- **Coefficients / precompute:** `fm1_rate_coeff[100]` (Q16) and
  `fm1_level_gain[100]` (Q15) are the fixed tables (`gen/math_tables.h`). The
  per-tick work is one `fm1_onepole` (a Q15×Q16 multiply-add, with a ±1 floor so
  the stage always converges) + two table loads.

- **State:** `stage` (uint32, 0…4), `level` (q15).

- **Edge cases:** `level` is clamped to [0, 32767] by the stage thresholds. Release
  from a held level, a gate↑ during release restarts attack from 0, and the +8/−8
  hysteresis prevents asymptotic creep.

- **Reference + credit:** stage structure and rate/level semantics from **ymfm**
  (`fm_operator::clock_envelope`, BSD-3-Clause, consulted for the
  attack/decay/sustain/release decomposition and the "attack is the only stage that
  raises the level" rule), cross-checked against **Dexed/msfa** (GPL-3.0, consulted
  only). **Deviation:** ymfm/Dexed advance a log-domain *attenuation* by a table
  increment and convert via a power table, whilst here the rate is a 0–99 index into
  a one-pole coefficient table and the level a 0–99 index into a gain table, with the
  same exponential character (rate→time, level→dB) for one multiply-add plus two
  table loads. Recorded as *learned, re-implemented*.

- **Performance note:** control rate (no audio ports), one `fm1_onepole` + one
  compare per tick. Runs in the control prologue, not the sample loop.

---

## Component: `osc.waveform` (the 12-wave OPZ + OPL union)

- **Purpose:** give each operator one of 12 waveforms, the **OPZ (YM2414)** 8 plus
  the **OPL2/OPL3 (YM3812)** shapes the OPZ doesn't already produce, from one
  quarter-sine table + a `waveform` select, and waveform 0 is the plain sine, so the
  `osc` node stays backward-compatible.

- **Algorithm** (fixed-point, exactly): `phase` is Q32 (full cycle). `s = fm1_sin(phase)`,
  `a = |s|`, `half = phase>>31`, `q = phase>>30`. The `ws` selects:

  | ws | name | source | formula |
  | --- | --- | --- | --- |
  | 0 | sine | both | `s` |
  | 1 | triangle | OPZ | `s·a >> 15` (sin·\|sin\|, the "trianglish" sin²) |
  | 2 | half sine | both | `half ? 0 : a` |
  | 3 | half triangle | OPZ | `half ? 0 : s·a>>15` |
  | 4 | double sine | both | `half ? 0 : fm1_sin(phase<<1)` |
  | 5 | double triangle | OPZ | `half ? 0 : s2·\|s2\|>>15`, `s2 = fm1_sin(phase<<1)` |
  | 6 | double half sine | both | `half ? 0 : \|s2\|` (rectified 2× sine) |
  | 7 | double half triangle | OPZ | `half ? 0 : s2²>>15` (sin² of 2×, always +) |
  | 8 | rectified | OPL | `a` (\|sin\| at 1×) |
  | 9 | pulse | OPL | `(q&1) ? 0 : a` (half-rectified) |
  | 10 | square | OPL | `half ? −32767 : 32767` |
  | 11 | saw | OPL | `(phase>>16) − 32768` |

- **Inputs / outputs:** `phase` (audio, Q32 `phase`), `audio` (audio, Q15).

- **Parameters:** `waveform` (uint32, 0…11, default 0).

- **Coefficients / precompute:** none, the quarter-sine table is the existing
  `fm1_sin_q15`. The select is a few multiplies and one switch, inlined via
  `fm1_sin_wave` in `hal/fm1_math.h`.

- **State:** none (memoryless).

- **Edge cases:** `ws` is masked `&15` (a bad value degrades to sine). `phase<<1`
  wraps naturally in Q32. The triangle's `s·a>>15` peaks at `32766` (sin², one LSB
  under full scale).

- **Reference + credit:** 0–7 from **ymfm**'s `opz_registers` constructor and 8–11
  from its `opl_registers_base` constructor (`ymfm_opz.cpp`/`ymfm_opl.cpp`,
  BSD-3-Clause), waveform 1 = the "trianglish" sin² (ymfm's log-domain `2×value`
  = linear sin²). The OPL's `wf2/wf3/wf6/wf7` are the rectified / pulse / square /
  saw appended here. **Re-implemented in linear Q15 from the definitions, not
  copied.** Two deliberate choices: the saw is a clean linear ramp (the OPL's exact
  form is a log-domain quantized ramp), and the pulse uses the OPL's bit-8 (quarter)
  mute exactly. The OPL set itself (the `docs/dsp-study.md` §2.7 bit trick) is
  distinct from the OPZ's, the union dedupes the 4 shared waves.

- **Performance note:** per-sample, memoryless, a few branches, one `fm1_sin`
  (one LUT read) for ws 0–3 and 8–11, two LUT reads for ws 4–7, plus at most one
  `fm1_q15_mul`.

---

## Component: `mix4` + `route4` + `route` + `algo` (the runtime-algorithm routing)

- **Purpose:** make the 8 algorithms a *runtime* selection, not a compile-time
  graph. `mix4` is the plain 4-input sum. `route4` is the general matrix-row mixer
  that reads a 4-weight row of the routing table and applies it. `route` is the
  ergonomic **audio-rate** 4-op router the patch uses, reads the `alg` control
  input (the selected algorithm) and all five rows (4 modulation + the carrier
  sum) at once. `algo` is the **control-rate** selector: it owns the `algorithm`
  param and outputs it as `alg` for `route`.

- **Algorithm** (fixed-point, exactly): all four operators compute in parallel, and
  every modulation reads the **1-sample-delayed** output of its sources (so the
  graph is a DAG regardless of the selected algorithm, the same 1-sample delay as
  feedback, ~23 µs, inaudible). Each operator's phase modulation is
  `mod_i = Σ_j routing[alg][i][j] · op_j_delayed`, and the carrier sum is
  `Σ_i routing[alg][carrier][i] · op_i_delayed`. `mix4` is `in0+in1+in2+in3`.
  `route4` is `fm1_route4(&routing[(alg·5+dest)·4], a, b, c, d)` = the four-input
  weighted sum `a·w0 + b·w1 + c·w2 + d·w3`.

- **Inputs / outputs:** `mix4`: `in0..in3` (audio) → `audio`. `route4`: `a b c d`
  (audio) → `audio`. `route`: `alg` (control, the selected algorithm) + `a b c d`
  (audio, the four delayed operator outputs) → `mod2 mod3 mod4 car` (audio,
  modulation into op2/op3/op4 and the carrier sum). `algo`: (none) → `alg`
  (control, = `algorithm`).

- **Parameters:** `route4`: `which` (table, the routing table), `algorithm`
  (uint32, 0…7), `dest` (uint32, 0…4: 0–3 = modulation rows, 4 = carrier mask).
  `route`: (none, the 8 routings are hardcoded in the node's `switch`). `algo`:
  `algorithm` (uint32, 0…7). `algo.algorithm` is the one shared selection
  (broadcast as `alg` to `route`).

- **Coefficients / precompute:** the routing table is the `tables:` entry
  `{ "kind": "routing" }` → 8 algorithms × 5 rows × 4 weights (160 Q15 0/1). The
  8 rows are ymfm's `s_algorithm_ops` {0,1,2,3,4,5,6,7}, the full standard
  TX81Z/DX100/DX21 4-op set, enumerated in `_tables.py::_ROUTING`.

- **State:** none (memoryless).

- **Edge cases:** weights are 0 or 32767, so a connected input is ×0.99997 (a
  0.003% error, inaudible for routing). `algorithm` is masked in the `_ROUTING`
  index (a bad value reads a valid row).

- **Reference + credit:** routing structure from **ymfm** (`s_algorithm_ops` +
  `output_4op`, BSD-3-Clause). **Deviation:** ymfm evaluates operators in order
  with current-sample modulation. Here all modulation uses the 1-sample-delayed
  output, so the graph stays acyclic and the algorithm is a runtime matrix. The
  matrix-as-data idea is umetate's own. The acyclicity trick mirrors feedback.

- **Performance note:** `route4` = four `fm1_q15_mul` + one table row read per
  sample (per destination). `route` is **specialised**: an 8-algorithm `switch`
  whose 1-weights are `fm1_q15_mul(x, 32767)` (the compiler folds the constant to a
  32-bit shift/sub/add) and whose 0-weights are dropped, ~4–6 multiplies per
  sample, not the five generic `fm1_route4` rows (20 multiplies + 20 table loads).
  `algo` runs at control rate (once per block): one clamp read, no audio-loop
  work. `mix4` is the general 4-sum (used by the standalone routing test, not the
  patch, which takes its carrier sum from `route.car`).
  Memoryless, no branching.

---

## Component: `fm_op` (the DX-style operator)

- **Purpose:** one FM operator = `phasor → osc → dx_env → vca`, with per-op
  1-sample self-feedback (`feedback`, a param), a q16 `ratio`, a per-op amplitude
  LFO (tremolo), a per-op `mute` gate, and a `pitch` input (the shared vibrato
  LFO). The complete DX-style building block. An N-operator voice is N `fm_op`
  wired by the routing matrix (§`route`).

- **Algorithm** (fixed-point, exactly):
  ```
  trem   = lfo                               (the amplitude LFO, in level steps)
  level_t = clamp(level + trem, 0, 99)        (the LFO offset is added after key/velocity)
  sum    = external_fm + feedback_prev             (Q15)
  inc_eff = inc · (32768 + pitch) >> 15            (relative pitch; pitch = vibrato)
  phase += inc_eff + (sum << 16)                   (FM index: full-scale = ±π phase)
  sine   = fm1_sin_wave(phase, waveform)          (12-wave OPZ+OPL union)
  out    = sine · eg · level_gain[level_t]         (Q15)
  feedback_prev = (out · feedback) delayed 1 sample                (per-op feedback)
  audio  = mute ? 0 : out                          (the per-op mute gate)
  ```
  `inc = note_to_inc(note) · ratio >> 16` (a q16 frequency multiplier, runtime,
  `ratio` 65536 = 1.0, 131072 = 2.0, 32768 = 0.5, so non-integer ratios stay in
  tune). Feedback taps the *enveloped, leveled* output (matching ymfm, which feeds
  back `compute_volume` = sine × envelope). The amplitude LFO adds a signed level
  offset in the same 0..99 units as `LEVEL` before the key/velocity push, so a
  bipolar depth swings around the level and a unipolar depth starts at the level
  and moves toward the depth. Pitch *scales the increment*
  (`inc · (1 + pitch/32768)`), relative pitch, so vibrato is constant cents across
  the keyboard, and it too is silenced with the envelope (the output is gated, not
  the phase). `mute` gates the operator's *output* (after the feedback tap), so a
  muted operator contributes nothing to the routing while its feedback loop still
  runs internally.

- **Inputs / outputs:** `fm` (audio, external modulation), `pitch` (control, the
  shared vibrato LFO) → `audio` (audio).

- **Parameters** (mapped inward):
  `ratio` (q16, the frequency multiplier, runtime), `waveform` (uint32 0–11),
  `level` (q15, the FM index / output amplitude), `feedback` (q15, the per-op
  self-modulation depth), `mute` (uint32, 0/1, the output gate), `trem_rate` (q32,
  the per-op LFO increment), `trem_depth` (q15, the tremolo depth), and the
  `dx_env` params `r1..r4, l1..l3`.

- **Coefficients / precompute:** none. `inc` is note-derived at note-on (from the
  runtime `ratio`), the feedback is a `delay1` register, the LFO is a control-rate
  phase accumulator.

- **State:** per-op `phase` (q32), `stage`/`level` (the dx_env), `lfo_phase` (the
  tremolo), `prev` (delay1).

- **Edge cases:** feedback = 0 → no self-modulation. `trem_depth` = 0 → `eg_t = eg`
  (no tremolo). `pitch` = 0 (depth 0) → `inc_eff = inc` (no vibrato). The
  `(sum << 16)` scale keeps a full-scale modulator at a ±π phase swing, so the
  modulator's `level` is the FM depth. The `(32768 + pitch)` scale keeps a
  full-scale pitch offset at ±1 octave.

- **Reference + credit:** operator decomposition (phasor/osc/env/vca) from
  ymfm/Dexed (dsp-study §2.6). Feedback tap + 1-sample delay from ymfm
  (`output_4op`). The `<<16` FM-index scale is umetate's own fixed-point choice
  (ymfm scales by `>>1` on a 10.10 phase, a different range for a different phase
  width). The tremolo-multiplies-envelope and pitch-scales-inc forms are umetate's
  own.

- **Performance note:** audio rate. Per sample = 1 phase add + 1 LUT read + 1
  envelope multiply + 3 gain multiplies + the feedback multiply (the tremolo add is
  a `mix`). The LFO runs at control rate (once per sub-block). The `delay1` is the
  cycle-breaker (read first, write last).

---

## Component: `lfo` (the vibrato/tremolo source)

- **Purpose:** a free-running low-frequency oscillator, shared as the *one global*
  pitch LFO (vibrato) and instantiated *per operator* for amplitude LFO (tremolo).
  Control-rate: one update per `CONTROL_BLOCK` samples.

- **Algorithm** (fixed-point, exactly):
  ```
  phase += rate · CONTROL_BLOCK       (Q32; rate = f · 2³² / 44100 per sample)
  wave  = fm1_wave[base + (phase >> 22)]
  bipolar: cv = wave · depth
  unipolar: cv = ((wave + 32768) >> 1) · depth
  ```
  `rate` is a Q32 phase increment *per sample* (the `note_to_inc` convention), so
  the per-control-tick advance is `rate × CONTROL_BLOCK`. A 1 Hz LFO is
  `rate = 2³² / 44100 ≈ 97392`. In bipolar mode the output is ±`depth` around the
  target value. In unipolar mode the output starts at the target value and moves
  toward `depth`, so a negative depth moves down.

- **Inputs / outputs:** none → `cv` (control). The output is in the target
  parameter's units: level steps for the amplitude LFO, Q15 semitones for pitch.

- **Parameters:** `rate` (q32, the increment per sample), `depth` (int32, the
  output amplitude, 0 disables the LFO), `mode` (uint32, 0 = unipolar, 1 =
  bipolar). For the amplitude LFO `depth` is a signed level offset in the same
  units as `tl.level` (-99..+99). For the pitch LFO `depth` is a signed Q15
  semitone offset (-32768..32768, or -12.00..+12.00 st).

- **Coefficients / precompute:** none, `fm1_sin` is the existing quarter-sine LUT.

- **State:** `phase` (q32).

- **Edge cases:** `depth` = 0 → `cv` = 0 (off). `rate` = 0 → frozen phase (a
  constant). The control-rate staircase is ~1.4 kHz (`44100/CONTROL_BLOCK`), smooth
  enough for sub-audio LFO frequencies. At 5 Hz the staircase is still far above the
  modulation rate, and at 0.1 Hz the same accumulator gives a slow sweep with the
  same one-cent pitch resolution.

- **Reference + credit:** a textbook LFO (phase accumulator + sine LUT), expressed
  as a control-rate node. The `rate × CONTROL_BLOCK` advance is the control-rate
  accumulator form, umetate's own.

- **Performance note:** control rate, one phase add + one LUT read per sub-block,
  not per sample. A global LFO runs once per sub-block (scalar). A per-op LFO runs
  once per voice per sub-block.

---

## Component: `pitch_env` (the per-note pitch envelope)

- **Purpose:** a per-note pitch envelope, on every note-on each operator's pitch
  runs a rate/level envelope (attack → decay1 → decay2 → sustain → release) with
  *signed* Q15 level targets, retriggered by `voice_trig[v]` and gated by `on`
  (off by default). Summed with the vibrato (`csum`) into the operators' `pitch`
  input.

- **Algorithm** (fixed-point, exactly):
  ```
  on trigger:  stage = 0; level = 0
  on gate-off: stage = 4 (release toward l4)
  stage 0: level -> l1 (attack)   stage 1: l1 -> l2 (decay1)
  stage 2: l2 -> l3 (decay2)      stage 3: hold l3    stage 4: level -> l4
  cv = on ? level : 0               (q15 bipolar; feeds the pitch input)
  ```
  `l1..l3` are the signed Q15 pitch offsets (the same ±1 octave scale as
  `phasor.pitch`: 32768 = +1 octave, 2731 ≈ +1 semitone), and `l4` is the offset the
  release segment ends at. `r1..r4` are 0–99 rates (the `fm1_rate_coeff` one-pole
  table).

- **Inputs / outputs:** `gate` (event, default `voice_gate[v]`), `trigger` (event,
  default `voice_trig[v]`) → `cv` (control, q15 bipolar).

- **Parameters:** `on` (uint32, 0/1), `r1..r4` (rate 0–99), `l1..l4` (q15, the
  signed pitch targets and the release end).

- **Coefficients / precompute:** `fm1_rate_coeff[100]` (Q16), one table load +
  one `int64` multiply-add per stage tick.

- **State:** `stage` (uint32, 0–4), `level` (q15, the current offset).

- **Edge cases:** `on = 0` → `cv = 0` (no pitch change). A signed `l1` bends down
  (the transition check uses the absolute distance). Release returns to the note's
  pitch (0 offset).

- **Reference + credit:** the DX7's pitch EG (rates + levels on the patch's pitch),
  re-shaped as a per-note rate/level envelope, the same 0–99 rate/level tables as
  `dx_env`. Per-note retrigger (via `voice_trig`) is umetate's own.

- **Performance note:** control rate (per voice), one compare + one `int64`
  multiply per sub-block. `csum` (control summer) is a single add per sub-block.



