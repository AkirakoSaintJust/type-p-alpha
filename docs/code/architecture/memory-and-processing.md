# Memory and processing model

The engine is one generated C state machine at a fixed 48 kHz core rate. The
plugin wraps it in a C++ interface, feeds it host MIDI and parameter changes, and
either passes its output through at 48 kHz or resamples it at the host rate. This
chapter follows the actual objects, the state layout and the per-block order.

## The three owners

The host owns the `TypePAlphaAudioProcessor` instance. The processor owns four
things that this chapter follows:

```cpp
p1::Engine engine_;                 // one generated state
p1::MidiBridge midi_;               // C MIDI callback table
p1::RateBridge rateBridge_;         // host-rate output path
juce::AudioProcessorValueTreeState apvts_;
```

`p1::Engine` is a small class in `plugin/engine/Engine.hpp`. It holds

```cpp
std::unique_ptr<P1_STATE> state_;
```

and `P1_STATE` is an alias for `type_p_alpha_state`. The constructor allocates
the state once with `std::make_unique` and immediately calls
`P1_STATE_INIT`, which is the generated `type_p_alpha_dsp_state_init`. There is
no reallocation after construction. `prepareToPlay` calls `engine_.reset()`,
which clears the runtime voice state but keeps the same allocation.

The editor owns no engine state. It reads parameter values through the APVTS,
reads scope and snapshot data through the processor, and writes parameters
through the APVTS. The audio thread is the only writer of the engine state.

The library is separate from the engine. `p1::PresetStore` owns vectors of part
and multi records. It is used on the message thread and under the library lock.
It never shares memory with the audio callback.

## The generated state

`engine/gen/dsp.h` declares one struct. Its size is **492,900 bytes** on the
built Linux target (481.35 KiB). The field order is fixed by the generator and is
part of the ABI between the generated code and the engine API. The state starts
with per-voice parameter and node fields, then adds the part-level routing and
macro fields, the per-voice voice allocator fields, and finally the large
operator pools.

Representative offsets from `offsetof(type_p_alpha_state, ...)`:

| Field | Offset | Size | Role |
| --- | ---: | ---: | --- |
| `op1_ph_inc_ratio` | 44,188 | 8 | the two part values of the op1 ratio parameter |
| `voice_age` | 42,344 | 96 | the 24 voice ages |
| `op1_osc_bank` | 83,900 | 21,760 | the spectral bank for op1 |
| `op1_phase_map` | 105,660 | 36,864 | the phase-map pool for op1 |
| `op1_phase_lfo_pool` | 142,524 | 21,760 | the phase LFO pool for op1 |
| `op1_phase_env_pool` | 164,284 | 21,760 | the phase envelope pool for op1 |
| `op4_osc_bank` | 390,332 | 21,760 | the spectral bank for op4 |
| `voice_gate` | 492,524 | 24 | the 24 gate bytes |
| `mono_held` | 492,620 | 256 | the two 128-note mono-held tables |

The four operator pools account for 408,576 bytes. The first 83,900 bytes hold
the per-voice operator state, the per-part parameters and the voice allocator.
The last 376 bytes hold the voice gate, note, trigger, part, release and mono
fields.

### Per-operator fields

Each operator repeats the same field pattern. The pattern is easiest to read on
`op1_` and then apply to `op2_`, `op3_` and `op4_`:

| Group | Fields | Meaning |
| --- | --- | --- |
| phasor | `ph_inc`, `ph_phase`, `ph_step`, `ph_mip` | note increment, Q32 phase, per-sample increment and selected wavetable mip |
| oscillator | `osc_wave`, `osc_base`, `osc_bl`, `osc_tick`, `osc_live`, `osc_morph_macro` | selected waveform, table base, band-limited flag, bank tick, live morph and morph macro offset |
| amplitude envelope | `eg_stage`, `eg_level`, `eg_warm_start`, `eg_warm_progress`, `eg_warm_eff`, `eg_surge_eff`, `eg_d1_eff`, `eg_d2_eff`, `eg_rel_eff`, `eg_rate_macro`, `eg_*_base` | the composite envelope stage, level, warm-up progress, effective rates and macro bases |
| amplitude LFO | `lfo_phase`, `lfo_base`, `lfo_w`, `lfo_rate_level`, `lfo_depth_level`, `lfo_rate_step`, `lfo_depth_step`, `lfo_rate_ratio`, `lfo_depth_macro` | the free-running phase, wave sample, fade levels and per-voice rate and depth |
| feedback envelope | `fbenv_stage`, `fbenv_level`, `fbenv_r*_eff`, `fbenv_rate_macro`, `fbenv_*_base` | the signed feedback envelope |
| feedback LFO | `fblfo_*` | the feedback LFO |
| detune LFO | `dlfo_*` | the pitch LFO |
| level and feedback | `tl_level_eff`, `fb_signed`, `fb_level_eff`, `fb_negative`, `fb_prev`, `fb_shaped`, `fb_macro`, `fb_level_macro` | the level gain, the signed feedback sum and the two-sample feedback state |
| key and velocity | `level_ks_k`, `fb_ks_k`, `eg_ks_k`, `level_vs_k`, `fb_vs_k`, `eg_vs_k` | the Q15 key and velocity factors |
| one-pole history | `d1_prev` | the previous decay sample |
| spectral envelope | `specenv_stage`, `specenv_level`, `specenv_r*_eff`, `specenv_rate_macro`, `specenv_*_base` | the spectral morph envelope |
| spectral LFO | `speclfo_*` | the spectral morph LFO |

The full 1,654-field list is in
[`reference/state-fields.md`](../reference/state-fields.md).

### Per-part fields

The part-level arrays have two entries. `part_channel`, `part_low` and
`part_high` are the MIDI routing. `part_level`, `part_delay`, `part_bend_q16`,
`part_transpose`, `part_detune`, `part_mode`, `part_legato`,
`part_unison_count`, `part_unison_detune`, `part_portamento`,
`part_note_priority`, `part_pan` and the pan-envelope fields are the rest. The
`macro_*` arrays are six values, six source selects, six source ranges, eighteen
targets and eighteen destination ranges. The `vm_*` arrays are the per-part
voice mode values copied into the state by the parameter table.

### Voice allocator fields

The allocator uses

```c
uint32_t voice_age[NVOICE];
int32_t voice_velocity[NVOICE];
uint32_t voice_delay[NVOICE];
uint32_t voice_inc[NVOICE];
uint32_t voice_target_inc[NVOICE];
int32_t voice_detune[NVOICE];
uint8_t voice_note[NVOICE];
uint8_t voice_gate[NVOICE];
uint8_t voice_trig[NVOICE];
uint8_t voice_part[NVOICE];
uint8_t voice_rel[NVOICE];
```

plus the mono tables:

```c
uint8_t mono_held[N_PART][128];
uint32_t mono_order[N_PART][128];
uint8_t part_mono_voice[N_PART][8];
uint8_t part_mono_count[N_PART];
```

The note-on, note-off and render paths are in
[Voice allocation](voice-allocation.md).

## Parameter storage

The manifest has 2,053 parameters. The generated `type_p_alpha_dsp_params`
table stores `id`, `name`, `min`, `max`, `def`, `curve`, `rate`, `display` and
the automatable and modulatable flags. The generated `type_p_alpha_slots` table
stores each parameter's byte offset in the state and an optional part index:

```c
typedef struct { uint32_t off; uint8_t part; } type_p_alpha_slot_t;
```

A parameter with `part == 0xFF` is global. A parameter with `part == 0` or
`part == 1` is a per-part value, and the slot lookup adds `part * 4` to the
offset, which assumes the two part values are adjacent `int32_t` fields. That is
true for every per-part parameter in the generated layout. The slot lookup is

```c
static inline int32_t *type_p_alpha_param_slot(
    type_p_alpha_state *s, uint32_t id)
{
    const type_p_alpha_slot_t *p = &type_p_alpha_slots[id];
    return (int32_t *)((uint8_t *)s + p->off +
                       (p->part == 0xFFu ? 0u : (uint32_t)p->part * 4u));
}
```

`type_p_alpha_dsp_set_param_s` clamps to the manifest range and writes through
this slot. `type_p_alpha_dsp_get_param_s` reads the same slot. No parameter
write walks a switch or copies a struct.

The factory init table `type_p_alpha_init` is 3,680 bytes, 460 entries of
`{uint16_t id, int32_t value}`. `type_p_alpha_dsp_state_init` first zeroes the
whole state, then writes every parameter default from
`type_p_alpha_dsp_params[i].def`, then applies the init entries. The init table is
the factory patch, not the manifest defaults. For example the manifest default
for `algorithm` is 4, whilst the factory init patch selects algorithm 7.

## The fixed tables

The engine reads four groups of constant data:

| Table | Size | Source |
| --- | ---: | --- |
| `p1_wave` | 47,104 int16, 94,208 bytes | `tools/gen_wavetable.py`, `engine/gen/wavetable.h` |
| `p1_wave_base` | 14 bytes | the same generator |
| `p1_wave_harmonics` | 14 by 16 bytes | the same generator |
| `p1_sin_q15`, `p1_exp2_q16`, `p1_log2_q16`, `p1_semi_q16`, `p1_cent_q16`, `p1_rate_coeff`, `p1_level_gain`, `p1_filter_g_q16` | about 10 KiB | `tools/gen_math_tables.py`, `engine/gen/math_tables.h` |
| `p1_naive_wave` | 4 by 1,024 int16 | `tools/gen_math_tables.py` or the checked-in header |

The wavetable layout is one contiguous table. Waves 0 to 9 are one 1,024-sample
table each. Waves 10 to 13 are nine mip levels each, 1,024 samples per level, so
each band-limited wave occupies 9,216 samples. The base array is in 1,024-sample
units. The oscillator reads

```c
p1_wave[(p1_wave_base[wave] + (bl ? mip : 0)) << 10 | (phase >> 22)]
```

where `bl` is true for waves 10 to 13. The 14th waveform value is the additive
bank and has no table.

The spectral bank is a pool of `int16_t[10880]` per operator. The layout is

```c
int16_t op1_osc_bank[10880];
```

with the header at indices 0 to 15, the two a tables at 128, the two b tables
after them, and the shadow tables last. The bank is 21,760 bytes per operator.

The phase-map pool is `int16_t[18432]` per operator, 36,864 bytes. It holds the
geometry, the `F` table, the segment table, the protected mask, the locks and the
direct-read table. The pool word offsets are in
`plugin/engine/Engine.cpp` and the table sizes are in
`plugin/engine/PhaseMapLayout.h`.

The phase LFO and phase envelope pools are `int16_t[10880]` per operator each.
They hold one state per part plus the shared tables.

## The block model

The core rate is 48 kHz. The control block is 32 samples, so the control rate is
1,500 Hz. Everything that changes on a control tick reads the block-derived
values that the previous control pass wrote. The operator nodes run per sample.

`type_p_alpha_dsp_render_s` drives the block loop:

```c
S = state;
type_p_alpha_taps_clear(type_p_alpha_tap0);
for (uint32_t b = 0; b < frames; b += 32u) {
    type_p_alpha_render_pre_block();
    type_p_alpha_render_voices_block(
        type_p_alpha_render_bus0, type_p_alpha_render_bus1,
        type_p_alpha_tap0, 0u, NVOICE, b, frames);
    type_p_alpha_render_post_block(
        out, type_p_alpha_render_bus0, type_p_alpha_render_bus1, b, frames);
}
```

The three phases are:

1. **Pre-block.** Advance the control serial. Release delayed notes when their
   delay expires. Recompute the patch gate and trigger. Run the per-part pan
   envelope and update the constant-power pan gains. Apply mono portamento.
2. **Voices block.** For every gated or releasing voice, run the operator graph.
   The graph includes the phasors, oscillators, envelopes, LFOs, feedback,
   spectral bank, filter, algorithm router and carrier sum. The output is the
   stereo bus and the tap buffers.
3. **Post-block.** Run the one-pole DC blocker on each bus and write interleaved
   `int32_t` samples to `out`.

The generated `type_p_alpha_render_voices_block` is one large function with four
copies of the operator chain, one per operator. The canonical order is the
operator order in the state and the generator's node order. The block comments
in the generated file name each stage. The
[generated render walk](../reference/generated-render-walk.md) lists them in
source order.

## Host block to core block

`TypePAlphaAudioProcessor::processBlock` has two paths.

### At 48 kHz

The processor iterates the host buffer in 32-sample control blocks:

```cpp
while (pos < frames) {
    const int length = std::min(kControlBlock, frames - pos);
    const int end = pos + length;
    // consume the MIDI events whose sample position is before end
    syncParams();
    syncMacros();
    renderCore(directBuffer_.data(), length);
    // convert int32 to float and write left/right
    flushDeferred();
    outputScope_.push(left + pos, length);
    pushTapScopes(engine_, length);
    publishMorph(engine_);
    publishPhaseMaps(engine_);
    pos = end;
}
```

The output is bit-exact with the engine because there is no resampler in this
path. The final 0 to 31 samples of a host block are a partial control block only
if the host block is not a multiple of 32. The engine handles any frame count.

### At another host rate

The processor maps each host MIDI event to the core timeline:

```cpp
const int64_t coreFrame = floor(coreFrames_ +
    samplePosition * rateBridge_.ratio());
scheduled_.push_back({message, coreFrame});
```

`rateBridge_.process` renders 32-sample core chunks until it has enough core
frames for the host block, then pulls the host frames through the polyphase
kernel. The `coreChunk` callback runs after each core chunk and does the
per-chunk work:

```cpp
flushDeferred();
coreFrames_ += frames;
chunkNotes_.reset();
feedScheduled(coreFrames_ + kControlBlock);
pushTapScopes(engine, frames);
publishMorph(engine);
publishPhaseMaps(engine);
```

The callback is why a core chunk can publish a phase-map snapshot and a tap
window before the next chunk overwrites the tap state.

## Parameter synchronisation

`paramValues_` is an array of atomic pointers into the APVTS, one per manifest
parameter. `lastParams_` holds the last value sent to the engine. `syncParams`
walks all 2,053 entries, rounds the atomic float to an integer, clamps to the
manifest range, and calls `engine_.setParam` only when the value changed:

```cpp
if (clamped != lastParams_[i]) {
    engine_.setParam(spec.engineId, clamped);
    lastParams_[i] = clamped;
}
```

The first call after `prepareToPlay` writes every parameter because
`lastParams_` is filled with `INT32_MIN`. A host parameter change sets no flag
in the engine. The next control block sees the changed atomic and writes it. The
`paramsDirty_` flag is used when the processor state is replaced, so the next
block forces a full resend even when the host floats happen to compare equal.

The macro layer is a second pass. `syncMacros` computes the six effective macro
values, walks the 18 target slots, resolves the target row to its member pool,
and accumulates an offset per destination parameter. It then writes the
effective value for every member:

```cpp
engine_.setParam(spec.engineId, effective);
```

For a linear parameter the offset is `base + offset * (max - min)`. For a
logarithmic parameter the offset is added in the normalised domain, so a macro
on an LFO rate is musical rather than a raw Q32 phase delta. A macro at zero
writes the base value exactly, which is why the at-rest tests are bit-exact.

## Allocation policy

The audio thread does no allocation in the steady state. The allocations happen
in the constructor and in `prepareToPlay`:

- `Engine` allocates the 492,900-byte state once.
- `RateBridge::prepare` resizes the ring and the 128 by 1,024 kernel.
- `directBuffer_` is 32 stereo frames.
- `scheduled_` reserves twice the host block size plus 16.
- `deferredOffs_` reserves 64.
- `ScopeRing` has a fixed 2,048-float ring and a fixed scratch array.
- The tap scopes have ten fixed 2,048-float rings.
- The phase-map snapshots are fixed arrays.
- The editor allocates its components and its clipboards on the message thread.

`processBlock` clears the buffer at entry. It does not create a `juce::MidiBuffer`
or a `juce::AudioBuffer`. It uses the host buffer and the preallocated vectors.
The only branch that can grow a vector is the mono fold buffer if the host
changes the block size without calling `prepareToPlay`. The code guards that
resize.

## The output stage

The engine writes `int32_t` samples with a nominal full scale of 32,768. The
processor converts with

```cpp
constexpr float kIntToFloat = 1.0f / 32768.0f;
```

The master volume is applied after the rate bridge. The parameter is a fader
position from 0 to 1,000, not a gain. `p1::volumeGainFromPosition` maps position
0 to silence, 1 to 750 to -60 dB through unity at 750, and 751 to 1,000 to a
quadratic rise to +12 dB. If the gain is changing, the processor calls
`buffer.applyGainRamp` across the host block. If the gain is constant and not
unity, it calls `applyGain`. At unity it skips the multiply.

A mono host buffer takes the right channel through `monoFold_` and folds the
pair at 0.5. A hard-panned part therefore sits 6 dB down in mono, and a centred
part keeps its level.
