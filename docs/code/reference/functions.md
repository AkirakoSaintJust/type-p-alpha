# Function and method index

Every function definition and declaration found in the tracked C, C++ and
Python sources. A description is the comment above the declaration, the
comment at the top of the body, or a short purpose derived from the name,
the scope and the return type. The hand-written chapters explain the core
paths in depth.

| Function | Scope | File:Line | Signature | Description |
| --- | --- | --- | --- | --- |
| `note_to_inc` |  | `engine/gen/type_p_alpha_dsp.c:1680` | `static inline __attribute__((always_inline)) uint32_t note_to_inc(uint32_t note)` | Converts a MIDI note to a Q32 phase increment. |
| `type_p_alpha_param_slot` |  | `engine/gen/type_p_alpha_dsp.c:5860` | `static inline __attribute__((always_inline)) int32_t * type_p_alpha_param_slot(type_p_alpha_state *s, uint32_t id)` | Maps a manifest id to its state slot. |
| `type_p_alpha_dsp_state_init` |  | `engine/gen/type_p_alpha_dsp.c:5867` | `P1_DSP_API void type_p_alpha_dsp_state_init(type_p_alpha_state *s)` | Initialises the state and applies the factory init table. |
| `voice_mono_active` |  | `engine/gen/type_p_alpha_dsp.c:7548` | `static inline __attribute__((always_inline)) uint32_t voice_mono_active( uint32_t p, int32_t priority)` | Selects the active note under the mono priority rule. |
| `voice_highest_note` |  | `engine/gen/type_p_alpha_dsp.c:7561` | `static inline __attribute__((always_inline)) uint32_t voice_highest_note(void)` | Returns the highest held note across the voice pool. |
| `voice_highest_velocity` |  | `engine/gen/type_p_alpha_dsp.c:7568` | `static inline __attribute__((always_inline)) uint32_t voice_highest_velocity(void)` | Returns the highest held velocity across the voice pool. |
| `type_p_alpha_note_derived` |  | `engine/gen/type_p_alpha_dsp.c:7575` | `static inline __attribute__((always_inline)) void type_p_alpha_note_derived(uint32_t v)` | Derives the four operator increments from the voice pitch and bend. |
| `type_p_alpha_taps_clear` |  | `engine/gen/type_p_alpha_dsp.c:7583` | `static inline __attribute__((always_inline)) void type_p_alpha_taps_clear( int32_t (*taps)[DSP_TAP_FRAMES])` | Clears the ten tap buffers before a render block. |
| `type_p_alpha_render_pre_block` |  | `engine/gen/type_p_alpha_dsp.c:7591` | `P1_DSP_API void type_p_alpha_render_pre_block(void)` | Runs the control-rate gate, pan, delay and portamento work. |
| `type_p_alpha_render_voices_block` |  | `engine/gen/type_p_alpha_dsp.c:7700` | `P1_DSP_API void type_p_alpha_render_voices_block(int32_t *bus, int32_t *bus_r, int32_t (*taps)[DSP_TAP_FRAMES], uint32_t v0, uint32_t v1, uint32_t b, uint32_t frames)` | Runs the per-voice node graph and the audio render. |
| `type_p_alpha_render_post_block` |  | `engine/gen/type_p_alpha_dsp.c:14594` | `P1_DSP_API void type_p_alpha_render_post_block(int32_t *out, const int32_t *src_l, const int32_t *src_r, uint32_t b, uint32_t frames)` | Applies the DC blocker and writes interleaved output. |
| `type_p_alpha_dsp_render_s` |  | `engine/gen/type_p_alpha_dsp.c:14612` | `P1_DSP_API void type_p_alpha_dsp_render_s(type_p_alpha_state *state, int32_t *out, uint32_t frames)` | Drives 32-sample blocks through pre, voices and post. |
| `type_p_alpha_dsp_reset_s` |  | `engine/gen/type_p_alpha_dsp.c:14622` | `P1_DSP_API void type_p_alpha_dsp_reset_s(type_p_alpha_state *state)` | Clears the voice, delay and mono state. |
| `type_p_alpha_dsp_note_on_ch_s` |  | `engine/gen/type_p_alpha_dsp.c:14633` | `P1_DSP_API void type_p_alpha_dsp_note_on_ch_s(type_p_alpha_state *state, uint32_t note, uint32_t velocity, uint32_t channel)` | Routes a note to the matching parts and allocates poly or mono voices. |
| `type_p_alpha_dsp_note_on_s` |  | `engine/gen/type_p_alpha_dsp.c:14745` | `P1_DSP_API void type_p_alpha_dsp_note_on_s(type_p_alpha_state *state, uint32_t note, uint32_t velocity)` | Omni wrapper for the note-on path. |
| `type_p_alpha_dsp_note_off_ch_s` |  | `engine/gen/type_p_alpha_dsp.c:14750` | `P1_DSP_API void type_p_alpha_dsp_note_off_ch_s(type_p_alpha_state *state, uint32_t note, uint32_t channel)` | Releases matching voices and applies the mono fallback. |
| `type_p_alpha_dsp_note_off_s` |  | `engine/gen/type_p_alpha_dsp.c:14789` | `P1_DSP_API void type_p_alpha_dsp_note_off_s(type_p_alpha_state *state, uint32_t note)` | Omni wrapper for the note-off path. |
| `type_p_alpha_dsp_all_notes_off_ch_s` |  | `engine/gen/type_p_alpha_dsp.c:14793` | `P1_DSP_API void type_p_alpha_dsp_all_notes_off_ch_s(type_p_alpha_state *state, uint32_t channel)` | Releases or kills the voices on one channel. |
| `type_p_alpha_dsp_all_notes_off_s` |  | `engine/gen/type_p_alpha_dsp.c:14808` | `P1_DSP_API void type_p_alpha_dsp_all_notes_off_s(type_p_alpha_state *state)` | Global panic wrapper. |
| `type_p_alpha_dsp_set_bend_ch_q16_s` |  | `engine/gen/type_p_alpha_dsp.c:14812` | `P1_DSP_API void type_p_alpha_dsp_set_bend_ch_q16_s(type_p_alpha_state *state, uint32_t bend, uint32_t channel)` | Converts a 14-bit bend to the part ratio and retunes held voices. |
| `type_p_alpha_dsp_set_bend_q16_s` |  | `engine/gen/type_p_alpha_dsp.c:14828` | `P1_DSP_API void type_p_alpha_dsp_set_bend_q16_s(type_p_alpha_state *state, uint32_t bend)` | Omni wrapper for the bend path. |
| `type_p_alpha_dsp_lit_s` |  | `engine/gen/type_p_alpha_dsp.c:14832` | `P1_DSP_API uint32_t type_p_alpha_dsp_lit_s(type_p_alpha_state *state, uint32_t base)` | Returns the 27-note activity mask for the keyboard display. |
| `type_p_alpha_dsp_quiet_s` |  | `engine/gen/type_p_alpha_dsp.c:14841` | `P1_DSP_API int type_p_alpha_dsp_quiet_s(type_p_alpha_state *state)` | Reports whether any voice gate is open. |
| `type_p_alpha_dsp_set_param_s` |  | `engine/gen/type_p_alpha_dsp.c:14847` | `P1_DSP_API int type_p_alpha_dsp_set_param_s(type_p_alpha_state *state, uint32_t id, int32_t value)` | Clamps and writes one parameter through the slot table. |
| `type_p_alpha_dsp_get_param_s` |  | `engine/gen/type_p_alpha_dsp.c:14855` | `P1_DSP_API int32_t type_p_alpha_dsp_get_param_s(type_p_alpha_state *state, uint32_t id)` | Reads one parameter through the slot table. |
| `type_p_alpha_dsp_param_count` |  | `engine/gen/type_p_alpha_dsp.c:14859` | `P1_DSP_API uint32_t type_p_alpha_dsp_param_count(void)` | Returns the manifest parameter count. |
| `type_p_alpha_dsp_tap_s` |  | `engine/gen/type_p_alpha_dsp.c:14861` | `P1_DSP_API const int32_t *type_p_alpha_dsp_tap_s(type_p_alpha_state *state, uint32_t ch)` | Returns one tap buffer by channel. |
| `p1_q15_mul` |  | `engine/hal/p1_math.h:22` | `P1_MATH_INLINE int32_t p1_q15_mul(int32_t a, int32_t b)` | Q15 x Q15 -> Q15, rounded. The per-sample multiply for Q15 signals/levels. |
| `p1_q15_mul32` |  | `engine/hal/p1_math.h:31` | `P1_MATH_INLINE int32_t p1_q15_mul32(int32_t a, int32_t b)` | Q15 x Q15 -> Q15, rounded, for operands already known to fit the Q15 range (-32768..32768, including the 32768 unity gain). The product is below 2^30, so a 32-bit multiply is exact and avoids the 64-bit pair MAC. The caller owns the range proof. |
| `p1_q16_mul` |  | `engine/hal/p1_math.h:37` | `P1_MATH_INLINE int32_t p1_q16_mul(int32_t a, int32_t b)` | Q16 x Q16 -> Q16, rounded (gains, coefficients). |
| `p1_q31_mul` |  | `engine/hal/p1_math.h:43` | `P1_MATH_INLINE int32_t p1_q31_mul(int32_t a, int32_t b)` | Q31 x Q31 -> Q31, rounded. |
| `p1_soft_clip` |  | `engine/hal/p1_math.h:52` | `P1_MATH_INLINE int32_t p1_soft_clip(int32_t x)` | The filter input's soft knee: y = x - x^5/8, with x clamped at full scale. The fifth-order term is 0.2 percent at -12 dBFS, 2.5 percent at -3 dBFS and 12.5 percent at 0 dBFS, so a normal level stays clean and a hot mixer compresses like the VCF's input pair. |
| `p1_shr_round` |  | `engine/hal/p1_math.h:63` | `P1_MATH_INLINE int32_t p1_shr_round(int32_t x, int shift)` | Rounding right shift (the SDK's PSHR). |
| `p1_sat_add32` |  | `engine/hal/p1_math.h:69` | `P1_MATH_INLINE int32_t p1_sat_add32(int32_t a, int32_t b)` | Saturating 32-bit add/sub (accumulation never wraps). |
| `p1_sat_sub32` |  | `engine/hal/p1_math.h:77` | `P1_MATH_INLINE int32_t p1_sat_sub32(int32_t a, int32_t b)` | Returns the sat sub 32 value. |
| `p1_madd_sat` |  | `engine/hal/p1_math.h:86` | `P1_MATH_INLINE int32_t p1_madd_sat(int32_t acc, int32_t a, int32_t b, int shift)` | Saturating multiply-accumulate: acc += (a * b) >> shift. |
| `p1_onepole` |  | `engine/hal/p1_math.h:97` | `P1_MATH_INLINE int32_t p1_onepole(int32_t level, int32_t target, int32_t coeff)` | A one-pole toward `target`: level += (target - level) * coeff / 65536. When the rounded step is 0 (a small coeff times a small remaining delta) the step is forced to ±1, so the envelope always converges to `target` instead of stalling just short of it. That stall is what shows up as "DECAY holds forever" at low rates. |
| `p1_ease_in` |  | `engine/hal/p1_math.h:116` | `P1_MATH_INLINE int32_t p1_ease_in(int32_t start, int32_t target, uint32_t progress)` | Slow-start (ease-in) interpolation for the attack warm-up. `progress` runs 0..65536, and the squared progress makes the motion start slowly and accelerate toward the target, which is the convex "slow then abrupt" shape. At progress 32768 the value is a quarter of the way, not a half. |
| `p1_fade_step` |  | `engine/hal/p1_math.h:130` | `P1_MATH_INLINE uint32_t p1_fade_step(int32_t rate)` | The linear step for an LFO's rate or depth fade, in Q31 per control tick. The 1..99 values use the envelope rate table's 99 percent time, so a fade value and an envelope rate value mean the same duration. 0 is off and 99 is instant. The level runs 0 to 2^31, and the 31-bit fraction keeps the ramp on the table time for the slow settings as well as the fast ones. |
| `p1_sin` |  | `engine/hal/p1_math.h:151` | `P1_MATH_INLINE int32_t p1_sin(uint32_t phase)` | Full-cycle sine, Q15, from the quarter table (p1_sin_q15, midpoint-sampled). `phase` is Q32 (wrapping accumulator: inc = f * 2^32 / FS). |
| `p1_cos` |  | `engine/hal/p1_math.h:159` | `P1_MATH_INLINE int32_t p1_cos(uint32_t phase)` | Returns the cos value. |
| `p1_sin_wave` |  | `engine/hal/p1_math.h:175` | `P1_MATH_INLINE int32_t p1_sin_wave(uint32_t phase, uint32_t ws)` | The 14-waveform union of the OPZ (YM2414) and OPL2/OPL3 (YM3812) sets, from ymfm's `opz_registers` and `opl_registers_base` constructors (BSD-3-Clause, re-implemented in linear Q15), plus the rectified triangle and the half square. `phase` is Q32 (full cycle), and `ws` is 0..13. 0-7 are the OPZ set (sine, triangle, half sine, half triangle, double sine, double triangle, double half sine, double half triangle), 8 is the rectified sine, 9 is the rectified triangle, and 10-13 are the OPL-only shapes (pulse, half square, square, saw). A bad `ws` degrades to sine. Precomputed into gen/wavetable.h (`p1_wave`, tools/gen_wavetable.py): waves 0-9 are one 1024-sample table each, whilst 10 (pulse) / 11 (half square) / 12 (square) / 13 (saw) are nine band-limited tables each (harmonic counts 256..1) for the top-octave anti-aliasing. `p1_wave_base[ws]` is ws's mip-0 offset, and this helper reads the richest (mip-0) table. The oscillator node adds the `mip` selected from its note's increment. |
| `p1_wave_mip` |  | `engine/hal/p1_math.h:187` | `P1_MATH_INLINE uint32_t p1_wave_mip(uint32_t step)` | The band-limited-table mip index for a carrier increment `step` (Q32, samples per cycle): the level whose top harmonic stays at or below Nyquist/2.25. The harmonic count per level is 2^(8-mip) (256..1). The threshold is the brightness setting: Nyquist/4 was the conservative choice, and Nyquist/2.25 recovers more of the top octave with less alias headroom. The shift loop keeps the block-rate code small, because this runs while the flash is busy. |
| `p1_key_scale` |  | `engine/hal/p1_math.h:202` | `P1_MATH_INLINE int32_t p1_key_scale(uint32_t note, uint32_t bp, int32_t lo, int32_t hi)` | The bipolar key-follow factor (Q15): 0 at the breakpoint, ramping linearly to `hi` at note 127 (the top octave) or `lo` at note 0 (the lowest octave). `lo`/`hi` are signed percent (-100..+100): +N pushes the target toward its maximum, -N toward its minimum. `note` and `bp` are 0..127 MIDI notes. |
| `p1_key_apply` |  | `engine/hal/p1_math.h:218` | `P1_MATH_INLINE int32_t p1_key_apply(int32_t v, int32_t k, int32_t min, int32_t max)` | Apply a bipolar key-follow factor `k` (Q15, p1_key_scale) to a target value `v` in [min, max]: +k pushes toward `max`, -k toward `min` (proportional to the remaining range, so the value clamps at the extreme). |
| `p1_vel_scale` |  | `engine/hal/p1_math.h:234` | `P1_MATH_INLINE int32_t p1_vel_scale(int32_t vel, int32_t amount)` | The velocity follow factor (Q15): 0 at velocity 0, ramping linearly to `amount` percent of full Q15 at maximum velocity. `vel` is `voice_velocity[v]` (velocity × 258, so 127 → about 32766), which is already Q15-normalised, so dividing by 100 turns the signed percent `amount` into the same Q15 scale that `p1_key_scale` returns. The old divisor 3,276,700 made the factor 32767 times too small, so every velocity follow was inert. |
| `p1_key_add` |  | `engine/hal/p1_math.h:241` | `P1_MATH_INLINE int32_t p1_key_add(int32_t a, int32_t b)` | Sum two bipolar Q15 push factors (key-follow + velocity-follow), clamped to the Q15 range so `p1_key_apply` never overshoots its target extreme. |
| `p1_clamp` |  | `engine/hal/p1_math.h:250` | `P1_MATH_INLINE int32_t p1_clamp(int32_t v, int32_t lo, int32_t hi)` | Clamp an int32 to [lo, hi]. |
| `p1_clamp16` |  | `engine/hal/p1_math.h:258` | `P1_MATH_INLINE int32_t p1_clamp16(int32_t v)` | Clamp an int32 to the Q16 range [0, 65535] (the SVF cutoff coefficient). |
| `p1_clamp16_max` |  | `engine/hal/p1_math.h:267` | `P1_MATH_INLINE int32_t p1_clamp16_max(int32_t v, int32_t max)` | Clamp an int32 to [0, max]. The ladder filter uses this for the one-pole coefficient, which is unstable in the feedback loop above its own ceiling. |
| `p1_route4` |  | `engine/hal/p1_math.h:278` | `P1_MATH_INLINE int32_t p1_route4(const int16_t *w, int32_t a, int32_t b, int32_t c, int32_t d)` | 4-input weighted sum against a routing-matrix row (a `const int16_t *` of four 0/1 Q15 weights): out = a*w0 + b*w1 + c*w2 + d*w3. The multiply stays, because the weights are runtime table data, so a select would be a data-dependent branch per term and measured slower than the straight-line MAC. |
| `p1_filter_coeff_q16` |  | `engine/hal/p1_math.h:292` | `P1_MATH_INLINE int32_t p1_filter_coeff_q16(int32_t pos)` | The VCF's per-stage coefficient from the log cutoff position. The 257-entry table is the exact 48 kHz one-pole transform at every 1/256 of the 12-octave range. Linear interpolation between entries keeps the error below a cent. The top entry is clamped at 55296, the highest coefficient the feedback loop takes without a parasitic oscillation. |
| `p1_tune_q16` |  | `engine/hal/p1_math.h:307` | `P1_MATH_INLINE int32_t p1_tune_q16(int32_t octave, int32_t semi, int32_t cent)` | 2^(octave + semi/12 + cent/1200) in Q16: the note-derived tuning ratio (the naive oscillator's octave/semi/cent knobs). `octave` -3..3, `semi` -11..11, `cent` -100..100 (clamped, with the octave part an exact shift and the semitone and cent parts from the p1_semi_q16 / p1_cent_q16 tables). |
| `p1_poly_blep` |  | `engine/hal/p1_math.h:333` | `P1_MATH_INLINE int32_t p1_poly_blep(uint32_t p, uint32_t inc, uint32_t inv_inc)` | The 2nd-order PolyBLEP residual (Tale / Martin Finke, "Making Audio Plugins Part 18"). Returns the Q15 correction for a unit step at phase 0: the value to layer on the naive waveform around its discontinuity. `p` is the Q32 phase position (0..2^32), `inc` the Q32 increment, `inv_inc` = 2^32/inc (note-on reciprocal) so u = p/inc is `(uint64)p * inv_inc` with no per-sample divide. saw:     out = 2t - 1 - blep(t) square:  out = (t < 0.5 ? 1 : -1) + blep(t) - blep((t + 0.5) mod 1) pulse:   the second blep moves to the pulse width: blep((t + 1 - pw) mod 1) triangle: the polyBLEP'd square through a leaky integrator (one extra state). |
| `p1_spectral_clamp16` |  | `engine/hal/spectral.h:42` | `static inline int16_t p1_spectral_clamp16(int32_t v)` | Returns the spectral clamp 16 value. |
| `p1_spectral_clamp99` |  | `engine/hal/spectral.h:47` | `static inline int32_t p1_spectral_clamp99(int32_t v)` | Returns the spectral clamp 99 value. |
| `p1_spectral_morph` |  | `engine/hal/spectral.h:55` | `static inline int32_t p1_spectral_morph(int32_t position, int32_t lfo)` | The morph in the 0..99 panel scale. `position` is the absolute morph envelope (or the knob when the envelope is off), and the LFO is already in panel units because the lfo node's depth is the spec_lfo_depth parameter. |
| `p1_spectral_table` |  | `engine/hal/spectral.h:63` | `static inline void p1_spectral_table(int16_t *out, const int32_t *levels, int firstHarmonic)` | One spectrum, all seven mips, peak normalised to Q15. The levels are the sixty-four harmonic amplitudes in 0..99, starting at `firstHarmonic` (1 or 65). The range shifts every partial by the same offset. |
| `p1_spectral_range_first` |  | `engine/hal/spectral.h:89` | `static inline int p1_spectral_range_first(int32_t range)` | Returns the spectral range first value. |
| `p1_spectral_hash` |  | `engine/hal/spectral.h:95` | `static inline uint16_t p1_spectral_hash(const int32_t *levels, int firstHarmonic)` | Returns the spectral hash value. |
| `p1_spectral_copy` |  | `engine/hal/spectral.h:104` | `static inline void p1_spectral_copy(int16_t *dst, const int16_t *src)` | Runs the spectral copy step for the surrounding code. |
| `p1_spectral_tick` |  | `engine/hal/spectral.h:115` | `static inline void p1_spectral_tick( int16_t *bank, const int32_t *waveform, const int32_t *range_a, const int32_t *range_b, const int32_t *a1, const int32_t *a2, const int32_t ...` | Generate or advance the bank for both parts. Called once per control block at the first voice. One spectrum changes at a time per part. The oscillator sets the used flag on every read, so a silent part commits its new table at once and an audible part crossfades over about 15 control ticks (256 units at 17 per tick, 480 samples, 10 ms). |
| `p1_spectral_lerp` |  | `engine/hal/spectral.h:247` | `static inline int32_t p1_spectral_lerp(const int16_t *table, uint32_t phase)` | The table is 256 samples, so the phase fraction is 24 bits. The 1024-sample wave table uses 22 bits. Using that shift here plays the table four times too high. The mip is the phasor's nine-level selector. Mips 0 to 2 keep all 64 harmonics. Mips 3 to 8 carry 32, 16, 8, 4, 2 and 1. |
| `p1_spectral_blend` |  | `engine/hal/spectral.h:257` | `static inline int32_t p1_spectral_blend(int32_t a, int32_t b, int32_t weight)` | Returns the spectral blend value. |
| `p1_spectral_read` |  | `engine/hal/spectral.h:263` | `static inline int32_t p1_spectral_read(int16_t *bank, int part, uint32_t phase, uint32_t mip, int32_t morph)` | Read the a and b tables at the mip and blend them with the morph. |
| `p1_macro_velocity_role_offset` |  | `engine/runtime/macro_apply.h:31` | `static inline int32_t p1_macro_velocity_role_offset( const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op, int32_t span, int32_t all, int32_t carrier, int3...` | Returns the macro velocity role offset value. |
| `p1_macro_velocity_level` |  | `engine/runtime/macro_apply.h:93` | `static inline int32_t p1_macro_velocity_level(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)` | Returns the macro velocity level value. |
| `p1_macro_velocity_feedback` |  | `engine/runtime/macro_apply.h:107` | `static inline int32_t p1_macro_velocity_feedback(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)` | Returns the macro velocity feedback value. |
| `p1_macro_velocity_morph` |  | `engine/runtime/macro_apply.h:121` | `static inline int32_t p1_macro_velocity_morph(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)` | Returns the macro velocity morph value. |
| `p1_macro_velocity_env_rate` |  | `engine/runtime/macro_apply.h:135` | `static inline int32_t p1_macro_velocity_env_rate(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)` | Returns the macro velocity env rate value. |
| `p1_macro_velocity_lfo_rate_ratio` |  | `engine/runtime/macro_apply.h:154` | `static inline uint32_t p1_macro_velocity_lfo_rate_ratio( const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)` | The LFO rate is a Q32 phase increment, and the panel range is logarithmic above the off stop, so the macro offset is a frequency ratio rather than a raw delta. The base of the exponent is the panel's 223 Hz top over its 0.01 Hz floor, the same ratio the parameter range uses. A zero offset returns unity exactly, which keeps a macro at 0 bit-exact. |
| `p1_macro_velocity_lfo_depth` |  | `engine/runtime/macro_apply.h:175` | `static inline int32_t p1_macro_velocity_lfo_depth(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op, int32_t span)` | Returns the macro velocity LFO depth value. |
| `wr32` |  | `engine/runtime/presets.c:9` | `static void wr32(uint8_t *p, uint32_t u)` | Runs the wr 32 step for the surrounding code. |
| `rd32` |  | `engine/runtime/presets.c:17` | `static uint32_t rd32(const uint8_t *p)` | Returns the rd 32 value. |
| `preset_crc32` |  | `engine/runtime/presets.c:22` | `uint32_t preset_crc32(const uint8_t *p, uint32_t n)` | Computes the reflected CRC-32 over a record body. |
| `preset_crc16` |  | `engine/runtime/presets.c:33` | `uint16_t preset_crc16(const uint8_t *p, uint32_t n)` | Returns the preset CRC 16 value. |
| `name_put` |  | `engine/runtime/presets.c:45` | `static void name_put(uint8_t *out, const char *name)` | Copies or trims a fixed-width record name field. |
| `seal` |  | `engine/runtime/presets.c:55` | `static void seal(uint8_t *out, uint32_t body)` | Writes the record CRC into the trailing four bytes. |
| `preset_part_pack` |  | `engine/runtime/presets.c:60` | `uint32_t preset_part_pack(uint8_t *out, const char *name, const int32_t *params, uint32_t n)` | Pack -> the record length, the name is truncated to PRESET_NAME_LEN-1 chars. |
| `preset_multi_pack` |  | `engine/runtime/presets.c:71` | `uint32_t preset_multi_pack(uint8_t *out, const char *name, const uint8_t *part_slot, const int32_t *level, const int32_t *delay, const int32_t *channel, const int32_t *low, cons...` | Packs a record into its little-endian bytes and appends the CRC. |
| `preset_settings_pack` |  | `engine/runtime/presets.c:115` | `uint32_t preset_settings_pack(uint8_t *out, int32_t base_channel, int32_t master_tune)` | Packs a record into its little-endian bytes and appends the CRC. |
| `preset_part_unpack` |  | `engine/runtime/presets.c:124` | `int preset_part_unpack(const uint8_t *raw, uint32_t n, char *name, int32_t *params)` | Unpack -> 1 and fills the outputs, 0 when the slot is empty or the CRC fails (the outputs are then left untouched). |
| `preset_multi_unpack` |  | `engine/runtime/presets.c:140` | `int preset_multi_unpack(const uint8_t *raw, uint32_t p, int has_macro, char *name, uint8_t *part_slot, int32_t *level, int32_t *delay, int32_t *channel, int32_t *low, int32_t *h...` | Unpacks and CRC-checks a record. |
| `preset_settings_unpack` |  | `engine/runtime/presets.c:192` | `int preset_settings_unpack(const uint8_t *raw, int32_t *base_channel, int32_t *master_tune)` | Unpacks and CRC-checks a record. |
| `bank_per` |  | `engine/runtime/presets.c:207` | `static uint32_t bank_per(uint32_t rec)` | Returns the bank per value. |
| `bank_slots` |  | `engine/runtime/presets.c:210` | `static uint32_t bank_slots(uint32_t count, uint32_t rec)` | The number of storage slots a bank of `count` records of `rec` bytes spans. |
| `bank_save` |  | `engine/runtime/presets.c:216` | `static int bank_save(const uint8_t *bank, uint32_t count, uint32_t rec, uint32_t slot0)` | Writes a bank or record through the host store. |
| `bank_load` |  | `engine/runtime/presets.c:228` | `static int bank_load(uint8_t *bank, uint32_t count, uint32_t rec, uint32_t slot0)` | Reads a bank, record or library file and migrates it. |
| `preset_part_bank_save` |  | `engine/runtime/presets.c:242` | `int preset_part_bank_save(const uint8_t *bank, uint32_t n)` | Banks, over the platform's `slot_save`/`slot_load` services. `n` = the part-patch param count (PP_RECORD) and `p` = the multi-patch part count (MP_RECORD). The multi bank's `n` is carried too so its flash slot can follow the part bank. |
| `preset_part_bank_load` |  | `engine/runtime/presets.c:246` | `int preset_part_bank_load(uint8_t *bank, uint32_t n)` | Reads a bank, record or library file and migrates it. |
| `preset_multi_bank_save` |  | `engine/runtime/presets.c:250` | `int preset_multi_bank_save(const uint8_t *bank, uint32_t n, uint32_t p)` | Writes a bank or record through the host store. |
| `preset_multi_bank_load` |  | `engine/runtime/presets.c:255` | `int preset_multi_bank_load(uint8_t *bank, uint32_t n, uint32_t p)` | Reads a bank, record or library file and migrates it. |
| `preset_settings_save` |  | `engine/runtime/presets.c:260` | `int preset_settings_save(const uint8_t *rec)` | Writes a bank or record through the host store. |
| `preset_settings_load` |  | `engine/runtime/presets.c:261` | `int preset_settings_load(uint8_t *rec)` | Reads a bank, record or library file and migrates it. |
| `preset_crc32` |  | `engine/runtime/presets.h:46` | `uint32_t preset_crc32(const uint8_t *p, uint32_t n)` | Computes the reflected CRC-32 over a record body. |
| `preset_crc16` |  | `engine/runtime/presets.h:47` | `uint16_t preset_crc16(const uint8_t *p, uint32_t n)` | Returns the preset CRC 16 value. |
| `preset_part_pack` |  | `engine/runtime/presets.h:50` | `uint32_t preset_part_pack(uint8_t *out, const char *name, const int32_t *params, uint32_t n)` | Pack -> the record length, the name is truncated to PRESET_NAME_LEN-1 chars. |
| `preset_multi_pack` |  | `engine/runtime/presets.h:51` | `uint32_t preset_multi_pack(uint8_t *out, const char *name, const uint8_t *part_slot, const int32_t *level, const int32_t *delay, const int32_t *channel, const int32_t *low, cons...` | Packs a record into its little-endian bytes and appends the CRC. |
| `preset_settings_pack` |  | `engine/runtime/presets.h:55` | `uint32_t preset_settings_pack(uint8_t *out, int32_t base_channel, int32_t master_tune)` | Packs a record into its little-endian bytes and appends the CRC. |
| `preset_part_unpack` |  | `engine/runtime/presets.h:59` | `int preset_part_unpack(const uint8_t *raw, uint32_t n, char *name, int32_t *params)` | Unpack -> 1 and fills the outputs, 0 when the slot is empty or the CRC fails (the outputs are then left untouched). |
| `preset_multi_unpack` |  | `engine/runtime/presets.h:60` | `int preset_multi_unpack(const uint8_t *raw, uint32_t p, int has_macro, char *name, uint8_t *part_slot, int32_t *level, int32_t *delay, int32_t *channel, int32_t *low, int32_t *h...` | Unpacks and CRC-checks a record. |
| `preset_settings_unpack` |  | `engine/runtime/presets.h:64` | `int preset_settings_unpack(const uint8_t *raw, int32_t *base_channel, int32_t *master_tune)` | Unpacks and CRC-checks a record. |
| `preset_part_bank_save` |  | `engine/runtime/presets.h:69` | `int preset_part_bank_save(const uint8_t *bank, uint32_t n)` | Banks, over the platform's `slot_save`/`slot_load` services. `n` = the part-patch param count (PP_RECORD) and `p` = the multi-patch part count (MP_RECORD). The multi bank's `n` is carried too so its flash slot can follow the part bank. |
| `preset_part_bank_load` |  | `engine/runtime/presets.h:70` | `int preset_part_bank_load(uint8_t *bank, uint32_t n)` | Reads a bank, record or library file and migrates it. |
| `preset_multi_bank_save` |  | `engine/runtime/presets.h:71` | `int preset_multi_bank_save(const uint8_t *bank, uint32_t n, uint32_t p)` | Writes a bank or record through the host store. |
| `preset_multi_bank_load` |  | `engine/runtime/presets.h:72` | `int preset_multi_bank_load(uint8_t *bank, uint32_t n, uint32_t p)` | Reads a bank, record or library file and migrates it. |
| `preset_settings_save` |  | `engine/runtime/presets.h:73` | `int preset_settings_save(const uint8_t *rec)` | Writes a bank or record through the host store. |
| `preset_settings_load` |  | `engine/runtime/presets.h:74` | `int preset_settings_load(uint8_t *rec)` | Reads a bank, record or library file and migrates it. |
| `midi_get` |  | `engine/runtime/synth_midi.c:9` | `int midi_get(uint8_t *st, uint8_t *d1, uint8_t *d2)` | The host hook that pops one raw MIDI message. |
| `synth_midi_feed` |  | `engine/runtime/synth_midi.c:12` | `void synth_midi_feed(synth_midi_ctx *ctx, uint8_t st, uint8_t d1, uint8_t d2)` | Applies one MIDI message, including sustain and all-notes-off. |
| `synth_midi_poll` |  | `engine/runtime/synth_midi.c:55` | `void synth_midi_poll(synth_midi_ctx *ctx)` | Drains the host MIDI queue into the handler. |
| `note_on` |  | `engine/runtime/synth_midi.h:21` | `void (*note_on)(uint32_t note, uint32_t velocity, uint32_t channel)` | Runs the note on step for the surrounding code. |
| `note_off` |  | `engine/runtime/synth_midi.h:22` | `void (*note_off)(uint32_t note, uint32_t channel)` | Runs the note off step for the surrounding code. |
| `all_off` |  | `engine/runtime/synth_midi.h:23` | `void (*all_off)(void)` | Runs the all off step for the surrounding code. |
| `all_off_ch` |  | `engine/runtime/synth_midi.h:24` | `void (*all_off_ch)(uint32_t channel)` | Runs the all off ch step for the surrounding code. |
| `set_param` |  | `engine/runtime/synth_midi.h:25` | `int (*set_param)(uint32_t id, int32_t value)` | Writes the param. |
| `set_bend` |  | `engine/runtime/synth_midi.h:26` | `void (*set_bend)(uint32_t bend14, uint32_t channel)` | Writes the bend. |
| `strip_toc` |  | `manual/build.py:49` | `def strip_toc(toc: str)` | Drop Python-Markdown's outer TOC div, because the template owns the nav. |
| `find_browser` |  | `manual/build.py:59` | `def find_browser()` | Finds the browser. |
| `main` |  | `manual/build.py:63` | `def main()` | Runs the test or tool entry point. |
| `write` |  | `manual/figures.py:37` | `def write(out: Path)` | Write the figures under `out/figures`. |
| `Engine` |  | `plugin/engine/Engine.cpp:34` | `Engine::Engine() : state_(std::make_unique<P1_STATE>())` | Returns the engine value. |
| `~Engine` |  | `plugin/engine/Engine.cpp:39` | `Engine::~Engine() = default` | Destroys the object. |
| `reset` |  | `plugin/engine/Engine.cpp:41` | `void Engine::reset()` | Clears the resampler ring and counters. |
| `resetPhaseMaps` |  | `plugin/engine/Engine.cpp:46` | `void Engine::resetPhaseMaps()` | Clears the phase-map, LFO and envelope pools. |
| `setParam` |  | `plugin/engine/Engine.cpp:62` | `int Engine::setParam(uint32_t id, int32_t value)` | Writes the param. |
| `getParam` |  | `plugin/engine/Engine.cpp:67` | `int32_t Engine::getParam(uint32_t id)` | Reads the param. |
| `noteOn` |  | `plugin/engine/Engine.cpp:72` | `void Engine::noteOn(uint32_t note, uint32_t velocity, uint32_t channel)` | Returns the note on value. |
| `noteOff` |  | `plugin/engine/Engine.cpp:77` | `void Engine::noteOff(uint32_t note, uint32_t channel)` | Returns the note off value. |
| `allNotesOff` |  | `plugin/engine/Engine.cpp:82` | `void Engine::allNotesOff(uint32_t channel)` | Releases the notes on one channel, or every channel for the panic. |
| `setBend` |  | `plugin/engine/Engine.cpp:87` | `void Engine::setBend(uint32_t bend14, uint32_t channel)` | Writes the bend. |
| `render` |  | `plugin/engine/Engine.cpp:92` | `void Engine::render(int32_t* out, uint32_t frames)` | Returns the render value. |
| `lit` |  | `plugin/engine/Engine.cpp:97` | `uint32_t Engine::lit(uint32_t base)` | Returns the lit value. |
| `quiet` |  | `plugin/engine/Engine.cpp:102` | `bool Engine::quiet()` | Tests the quiet condition. |
| `tap` |  | `plugin/engine/Engine.cpp:107` | `const int32_t* Engine::tap(uint32_t channel)` | Returns the tap value. |
| `phaseMapSnapshot` |  | `plugin/engine/Engine.cpp:112` | `bool Engine::phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const` | Tests the phase map snapshot condition. |
| `specMorph` |  | `plugin/engine/Engine.cpp:214` | `int Engine::specMorph(int op, int part) const` | Reads the live spectral morph for the editor. |
| `state` |  | `plugin/engine/Engine.cpp:246` | `P1_STATE* Engine::state()` | Returns the APVTS. |
| `Engine` | Engine | `plugin/engine/Engine.hpp:38` | `Engine(const Engine&) = delete` | Constructs the object. |
| `operator=` | Engine | `plugin/engine/Engine.hpp:39` | `Engine& operator=(const Engine&) = delete` | Returns the operator value. |
| `reset` | Engine | `plugin/engine/Engine.hpp:41` | `void reset()` | Clears the resampler ring and counters. |
| `setParam` | Engine | `plugin/engine/Engine.hpp:42` | `int setParam(uint32_t id, int32_t value)` | Writes the param. |
| `getParam` | Engine | `plugin/engine/Engine.hpp:43` | `int32_t getParam(uint32_t id)` | Reads the param. |
| `noteOn` | Engine | `plugin/engine/Engine.hpp:44` | `void noteOn(uint32_t note, uint32_t velocity, uint32_t channel = 0xFFFFFFFFu)` | Runs the note on step for the surrounding code. |
| `noteOff` | Engine | `plugin/engine/Engine.hpp:45` | `void noteOff(uint32_t note, uint32_t channel = 0xFFFFFFFFu)` | Runs the note off step for the surrounding code. |
| `allNotesOff` | Engine | `plugin/engine/Engine.hpp:47` | `void allNotesOff(uint32_t channel = 0xFFFFFFFFu)` | Releases the notes on one channel, or every channel for the panic. |
| `setBend` | Engine | `plugin/engine/Engine.hpp:48` | `void setBend(uint32_t bend14, uint32_t channel = 0xFFFFFFFFu)` | Writes the bend. |
| `render` | Engine | `plugin/engine/Engine.hpp:49` | `void render(int32_t* out, uint32_t frames)` | Runs the render step for the surrounding code. |
| `lit` | Engine | `plugin/engine/Engine.hpp:50` | `uint32_t lit(uint32_t base)` | Returns the lit value. |
| `quiet` | Engine | `plugin/engine/Engine.hpp:51` | `bool quiet()` | Tests the quiet condition. |
| `tap` | Engine | `plugin/engine/Engine.hpp:52` | `const int32_t* tap(uint32_t channel)` | Returns the tap value. |
| `specMorph` | Engine | `plugin/engine/Engine.hpp:55` | `int specMorph(int op, int part) const` | The last morph the oscillator read for an operator and part, 0 to 99, or -1 when the operator has not read the bank since the state reset. |
| `phaseMapSnapshot` | Engine | `plugin/engine/Engine.hpp:56` | `bool phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const` | Tests the phase map snapshot condition. |
| `resetPhaseMaps` | Engine | `plugin/engine/Engine.hpp:59` | `void resetPhaseMaps()` | Clear the per-part phase map, LFO and envelope pools so a patch load, or switching the map off, starts from the current controls. |
| `state` | Engine | `plugin/engine/Engine.hpp:61` | `P1_STATE* state()` | Returns the generated state pointer. |
| `p1_bridge_note_on` |  | `plugin/engine/MidiBridge.cpp:11` | `void p1_bridge_note_on(uint32_t note, uint32_t velocity, uint32_t channel)` | Runs the bridge note on step for the surrounding code. |
| `p1_bridge_note_off` |  | `plugin/engine/MidiBridge.cpp:17` | `void p1_bridge_note_off(uint32_t note, uint32_t channel)` | Runs the bridge note off step for the surrounding code. |
| `p1_bridge_all_off` |  | `plugin/engine/MidiBridge.cpp:23` | `void p1_bridge_all_off()` | Runs the bridge all off step for the surrounding code. |
| `p1_bridge_all_off_ch` |  | `plugin/engine/MidiBridge.cpp:29` | `void p1_bridge_all_off_ch(uint32_t channel)` | Runs the bridge all off ch step for the surrounding code. |
| `p1_bridge_set_param` |  | `plugin/engine/MidiBridge.cpp:35` | `int p1_bridge_set_param(uint32_t id, int32_t value)` | Returns the bridge set param value. |
| `p1_bridge_set_bend` |  | `plugin/engine/MidiBridge.cpp:40` | `void p1_bridge_set_bend(uint32_t bend, uint32_t channel)` | Runs the bridge set bend step for the surrounding code. |
| `MidiBridge` |  | `plugin/engine/MidiBridge.cpp:48` | `MidiBridge::MidiBridge(Engine& engine) : engine_(engine)` | Returns the MIDI bridge value. |
| `feed` |  | `plugin/engine/MidiBridge.cpp:61` | `void MidiBridge::feed(uint8_t status, uint8_t data1, uint8_t data2)` | Returns the feed value. |
| `reset` |  | `plugin/engine/MidiBridge.cpp:68` | `void MidiBridge::reset()` | Clears the resampler ring and counters. |
| `feed` | MidiBridge | `plugin/engine/MidiBridge.hpp:18` | `void feed(uint8_t status, uint8_t data1, uint8_t data2)` | Runs the feed step for the surrounding code. |
| `reset` | MidiBridge | `plugin/engine/MidiBridge.hpp:19` | `void reset()` | Clears the resampler ring and counters. |
| `besselI0` |  | `plugin/engine/RateBridge.cpp:14` | `double besselI0(double x)` | Returns the bessel i 0 value. |
| `prepare` |  | `plugin/engine/RateBridge.cpp:30` | `void RateBridge::prepare(double hostRate, int maxBlock)` | Builds the resampler kernel and ring for the host rate. |
| `reset` |  | `plugin/engine/RateBridge.cpp:73` | `void RateBridge::reset()` | Clears the resampler ring and counters. |
| `pushCore` |  | `plugin/engine/RateBridge.cpp:80` | `void RateBridge::pushCore(const int32_t* interleaved, int frames)` | Writes interleaved core frames into the ring. |
| `pull` |  | `plugin/engine/RateBridge.cpp:92` | `void RateBridge::pull(float* left, float* right, int frames)` | Reads host frames through the polyphase kernel. |
| `renderChunk` |  | `plugin/engine/RateBridge.cpp:135` | `void RateBridge::renderChunk(Engine& engine, int frames, CoreTapCallback onCore, void* context)` | Renders the chunk. |
| `process` |  | `plugin/engine/RateBridge.cpp:146` | `void RateBridge::process(Engine& engine, float* left, float* right, int frames, CoreTapCallback onCore, void* context)` | Renders core chunks and pulls the host-rate output. |
| `latencySamples` |  | `plugin/engine/RateBridge.cpp:158` | `int RateBridge::latencySamples() const` | Returns the bridge group delay in host samples. |
| `hasCore` |  | `plugin/engine/RateBridge.cpp:164` | `bool RateBridge::hasCore(int hostFrames) const` | Reports whether a host block will render at least one core chunk. |
| `prepare` | RateBridge | `plugin/engine/RateBridge.hpp:19` | `void prepare(double hostRate, int maxBlock)` | Builds the resampler kernel and ring for the host rate. |
| `reset` | RateBridge | `plugin/engine/RateBridge.hpp:20` | `void reset()` | Clears the resampler ring and counters. |
| `process` | RateBridge | `plugin/engine/RateBridge.hpp:24` | `void process(Engine& engine, float* left, float* right, int frames, CoreTapCallback onCore = nullptr, void* context = nullptr)` | The engine path: render core chunks, then pull host frames. The callback runs after each core chunk, before the tap state is overwritten. |
| `pushCore` | RateBridge | `plugin/engine/RateBridge.hpp:29` | `void pushCore(const int32_t* interleaved, int frames)` | Test and offline path that pushes interleaved int32 core frames and pulls host frames. The ring keeps the history the 128-tap kernel needs. |
| `pull` | RateBridge | `plugin/engine/RateBridge.hpp:32` | `void pull(float* left, float* right, int frames)` | Reads host frames through the kernel. A null `right` folds the pair to mono in `left`. |
| `latencySamples` | RateBridge | `plugin/engine/RateBridge.hpp:34` | `int latencySamples() const` | Returns the bridge group delay in host samples. |
| `ratio` | RateBridge | `plugin/engine/RateBridge.hpp:35` | `double ratio() const` | Returns the ratio value. |
| `hasCore` | RateBridge | `plugin/engine/RateBridge.hpp:39` | `bool hasCore(int hostFrames) const` | True when a `process` call of `hostFrames` will render at least one core chunk, so the caller can feed the first chunk's events before it runs. |
| `renderChunk` | RateBridge | `plugin/engine/RateBridge.hpp:45` | `void renderChunk(Engine& engine, int frames, CoreTapCallback onCore, void* context)` | Renders the chunk. |
| `reset` | ChunkNoteSet | `plugin/juce/ChunkNoteSet.h:17` | `void reset()` | Clears the resampler ring and counters. |
| `noteOn` | ChunkNoteSet | `plugin/juce/ChunkNoteSet.h:19` | `void noteOn(int channel, int note)` | Runs the note on step for the surrounding code. |
| `held` | ChunkNoteSet | `plugin/juce/ChunkNoteSet.h:26` | `bool held(int channel, int note) const` | Tests the held condition. |
| `macroRowIncludes` |  | `plugin/juce/MacroLaw.h:13` | `inline bool macroRowIncludes(int row, int index, int algorithm)` | Whether a target row names one parameter. A carrier or modulator group only names its own role for the current algorithm, and a row that names no member returns false. The index is the manifest index, which is the member id. |
| `macroEffective` |  | `plugin/juce/MacroLaw.h:40` | `inline double macroEffective(int knob, int sourceValue, int range)` | The macro source law. The knob, or the host automation on the same parameter, is the centre. The selected source is 0 to 127 and the range is in macro points, so a source at rest leaves the macro at the knob and a rising source adds up to its range. The result is clamped to the macro's own -100 to 100. |
| `macroOffset` |  | `plugin/juce/MacroLaw.h:51` | `inline double macroOffset(double macro, int range)` | The macro destination law. The offset is a fraction of the destination's span, so a macro at 100 with a range of 100 moves the whole span and half of each moves half. The destination is clamped in the normalised domain, which keeps a log parameter's offset musical instead of raw. |
| `macroTargetNormalised` |  | `plugin/juce/MacroLaw.h:56` | `inline double macroTargetNormalised(double base, double offset)` | Returns the macro target normalised value. |
| `ModeButton` | ModeButton | `plugin/juce/Pages.cpp:17` | `explicit ModeButton(const juce::String& label) : juce::Button(label)` | Constructs the object. |
| `setActive` | ModeButton | `plugin/juce/Pages.cpp:22` | `void setActive(bool active)` | Sets the active flag. |
| `paintButton` | ModeButton | `plugin/juce/Pages.cpp:30` | `void paintButton(juce::Graphics& g, bool highlighted, bool) override` | Draws the button in its normal, highlighted and down states. |
| `PartCard` | PartCard | `plugin/juce/Pages.cpp:52` | `PartCard(PluginEditor& editor, int part, bool active) : editor_(editor), part_(part), active_(active), header_(true, true, [this] { editor_.showRenameDialog(true, part_); }, [th...` | Constructs the object. |
| `resized` | PartCard | `plugin/juce/Pages.cpp:89` | `void resized() override` | Lays out the child components. |
| `paint` | PartCard | `plugin/juce/Pages.cpp:107` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | PartCard | `plugin/juce/Pages.cpp:122` | `void tick()` | Refreshes the live display state. |
| `VoiceMode` | VoiceMode | `plugin/juce/Pages.cpp:147` | `explicit VoiceMode(PluginEditor& editor) : editor_(editor), part_(editor.activePart())` | Constructs the object. |
| `resized` | VoiceMode | `plugin/juce/Pages.cpp:176` | `void resized() override` | Lays out the child components. |
| `paint` | VoiceMode | `plugin/juce/Pages.cpp:191` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | VoiceMode | `plugin/juce/Pages.cpp:200` | `void tick()` | Refreshes the live display state. |
| `PatchPage` | PatchPage | `plugin/juce/Pages.cpp:233` | `explicit PatchPage(PluginEditor& editor)` | Constructs the object. |
| `resized` | PatchPage | `plugin/juce/Pages.cpp:244` | `void resized() override` | Lays out the child components. |
| `tick` | PatchPage | `plugin/juce/Pages.cpp:256` | `void tick() override` | Refreshes the live display state. |
| `makePatchPage` |  | `plugin/juce/Pages.cpp:270` | `std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor)` | Creates the patch page. |
| `setParam` |  | `plugin/juce/Pages.h:11` | `inline void setParam(PluginEditor& editor, const juce::String& id, double value)` | Writes the param. |
| `getParam` |  | `plugin/juce/Pages.h:18` | `inline double getParam(PluginEditor& editor, const juce::String& id)` | Reads the param. |
| `ensureSpectrumMode` |  | `plugin/juce/Pages.h:28` | `inline void ensureSpectrumMode(PluginEditor& editor, int op, int part, const juce::String& valueID)` | A depth edit or an envelope switch that ends away from zero puts the operator in spectrum mode, so the envelope or LFO the player just set has something to move. |
| `InlineMuteSwitch` | InlineMuteSwitch | `plugin/juce/Pages.h:41` | `InlineMuteSwitch(std::function<bool()> getMuted, std::function<void(bool)> setMuted) : getMuted_(std::move(getMuted)), setMuted_(std::move(setMuted))` | Constructs the object. |
| `paint` | InlineMuteSwitch | `plugin/juce/Pages.h:48` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseUp` | InlineMuteSwitch | `plugin/juce/Pages.h:68` | `void mouseUp(const juce::MouseEvent&) override` | Handles a mouse interaction. |
| `CardHeader` | CardHeader | `plugin/juce/Pages.h:82` | `CardHeader(bool partLayout, bool withRename, std::function<void()> onRename, std::function<void()> onCopy, std::function<void()> onPaste, std::function<bool()> getMuted, std::fu...` | Constructs the object. |
| `resized` | CardHeader | `plugin/juce/Pages.h:102` | `void resized() override` | Lays out the child components. |
| `tick` | CardHeader | `plugin/juce/Pages.h:126` | `void tick()` | Refreshes the live display state. |
| `OscillatorCard` | OscillatorCard | `plugin/juce/Pages.h:140` | `OscillatorCard(PluginEditor& editor, int op, int part, bool withMorph = false) : P1Card("oscillator", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::...` | Constructs the object. |
| `resized` | OscillatorCard | `plugin/juce/Pages.h:171` | `void resized() override` | Lays out the child components. |
| `AmplitudeEnvelopeCard` | AmplitudeEnvelopeCard | `plugin/juce/Pages.h:200` | `AmplitudeEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("amp env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::amp...` | Constructs the object. |
| `resized` | AmplitudeEnvelopeCard | `plugin/juce/Pages.h:241` | `void resized() override` | Lays out the child components. |
| `tick` | AmplitudeEnvelopeCard | `plugin/juce/Pages.h:264` | `void tick()` | Refreshes the live display state. |
| `FeedbackEnvelopeCard` | FeedbackEnvelopeCard | `plugin/juce/Pages.h:274` | `FeedbackEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("fb env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::feedb...` | Constructs the object. |
| `resized` | FeedbackEnvelopeCard | `plugin/juce/Pages.h:327` | `void resized() override` | Lays out the child components. |
| `tick` | FeedbackEnvelopeCard | `plugin/juce/Pages.h:349` | `void tick()` | Refreshes the live display state. |
| `LfoCard` | LfoCard | `plugin/juce/Pages.h:361` | `LfoCard(PluginEditor& editor, juce::String title, juce::String rateID, juce::String depthID, juce::String waveID, juce::String modeID, juce::String rateFadeID, juce::String dept...` | Constructs the object. |
| `resized` | LfoCard | `plugin/juce/Pages.h:392` | `void resized() override` | Lays out the child components. |
| `makePatchPage` |  | `plugin/juce/Pages.h:419` | `std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor)` | Creates the patch page. |
| `makeMacrosPage` |  | `plugin/juce/Pages.h:420` | `std::unique_ptr<PageBase> makeMacrosPage(PluginEditor& editor)` | Creates the macros page. |
| `makeOperatorsPage` |  | `plugin/juce/Pages.h:421` | `std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor)` | Creates the operators page. |
| `makeFxModPage` |  | `plugin/juce/Pages.h:422` | `std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor)` | Creates the FX mod page. |
| `makeOperatorLfosPage` |  | `plugin/juce/Pages.h:423` | `std::unique_ptr<PageBase> makeOperatorLfosPage(PluginEditor& editor)` | Creates the operator LFOs page. |
| `makePhaseMapPage` |  | `plugin/juce/Pages.h:424` | `std::unique_ptr<PageBase> makePhaseMapPage(PluginEditor& editor)` | Creates the phase map page. |
| `makeOperatorSpectraPage` |  | `plugin/juce/Pages.h:425` | `std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor, bool a)` | Creates the operator spectra page. |
| `makeOperatorSpectraModPage` |  | `plugin/juce/Pages.h:426` | `std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)` | Creates the operator spectra mod page. |
| `FilterCard` | FilterCard | `plugin/juce/PagesFxMod.cpp:14` | `FilterCard(PluginEditor& editor, juce::String title, juce::String cutoffID, juce::String resID, juce::String typeID, juce::String onID, std::function<void()> onToPart = {}, juce...` | Constructs the object. |
| `resized` | FilterCard | `plugin/juce/PagesFxMod.cpp:34` | `void resized() override` | Lays out the child components. |
| `paint` | FilterCard | `plugin/juce/PagesFxMod.cpp:47` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `FilterEnvelopeCard` | FilterEnvelopeCard | `plugin/juce/PagesFxMod.cpp:65` | `FilterEnvelopeCard(PluginEditor& editor, int part, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("filter envelope"), part_(part), envView_(editor....` | Constructs the object. |
| `resized` | FilterEnvelopeCard | `plugin/juce/PagesFxMod.cpp:111` | `void resized() override` | Lays out the child components. |
| `tick` | FilterEnvelopeCard | `plugin/juce/PagesFxMod.cpp:132` | `void tick()` | Refreshes the live display state. |
| `PitchLfoCard` | PitchLfoCard | `plugin/juce/PagesFxMod.cpp:146` | `explicit PitchLfoCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch lfo"), part_(editor.activePart()), waveView_(edit...` | Constructs the object. |
| `resized` | PitchLfoCard | `plugin/juce/PagesFxMod.cpp:173` | `void resized() override` | Lays out the child components. |
| `tick` | PitchLfoCard | `plugin/juce/PagesFxMod.cpp:191` | `void tick()` | Refreshes the live display state. |
| `PitchEnvelopeCard` | PitchEnvelopeCard | `plugin/juce/PagesFxMod.cpp:207` | `explicit PitchEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch envelope"), part_(editor.activePart()), envV...` | Constructs the object. |
| `resized` | PitchEnvelopeCard | `plugin/juce/PagesFxMod.cpp:252` | `void resized() override` | Lays out the child components. |
| `tick` | PitchEnvelopeCard | `plugin/juce/PagesFxMod.cpp:273` | `void tick()` | Refreshes the live display state. |
| `PanEnvelopeCard` | PanEnvelopeCard | `plugin/juce/PagesFxMod.cpp:287` | `explicit PanEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pan envelope"), part_(editor.activePart()), envView_...` | Constructs the object. |
| `resized` | PanEnvelopeCard | `plugin/juce/PagesFxMod.cpp:330` | `void resized() override` | Lays out the child components. |
| `tick` | PanEnvelopeCard | `plugin/juce/PagesFxMod.cpp:351` | `void tick()` | Refreshes the live display state. |
| `FollowCard` | FollowCard | `plugin/juce/PagesFxMod.cpp:365` | `FollowCard(PluginEditor& editor, bool key, juce::String title, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card(std::move(title)), editor_(editor), k...` | Constructs the object. |
| `resized` | FollowCard | `plugin/juce/PagesFxMod.cpp:415` | `void resized() override` | Lays out the child components. |
| `paint` | FollowCard | `plugin/juce/PagesFxMod.cpp:432` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `refresh` | FollowCard | `plugin/juce/PagesFxMod.cpp:445` | `void refresh()` | Refreshes the display. |
| `targetItems` | FollowCard | `plugin/juce/PagesFxMod.cpp:461` | `juce::StringArray targetItems() const` | Returns the target items value. |
| `updateTarget` | FollowCard | `plugin/juce/PagesFxMod.cpp:469` | `void updateTarget()` | Updates the target. |
| `wipe` | FollowCard | `plugin/juce/PagesFxMod.cpp:478` | `void wipe()` | Runs the wipe step for the surrounding code. |
| `setPair` | RingGraph | `plugin/juce/PagesFxMod.cpp:507` | `void setPair(int a, int b)` | Writes the pair. |
| `setDirect` | RingGraph | `plugin/juce/PagesFxMod.cpp:514` | `void setDirect(bool a, bool b)` | Writes the direct. |
| `paint` | RingGraph | `plugin/juce/PagesFxMod.cpp:523` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `RingmodCard` | RingmodCard | `plugin/juce/PagesFxMod.cpp:598` | `RingmodCard(PluginEditor& editor, int part) : P1Card("ringmod"), editor_(editor), part_(part)` | Constructs the object. |
| `resized` | RingmodCard | `plugin/juce/PagesFxMod.cpp:629` | `void resized() override` | Lays out the child components. |
| `tick` | RingmodCard | `plugin/juce/PagesFxMod.cpp:631` | `void tick()` | Refreshes the live display state. |
| `carrierOutOn` | RingmodCard | `plugin/juce/PagesFxMod.cpp:676` | `bool carrierOutOn(int op) const` | Tests the carrier out on condition. |
| `isCarrier` | RingmodCard | `plugin/juce/PagesFxMod.cpp:681` | `bool isCarrier(int op) const` | Tests whether the the carrier. |
| `updateCarriers` | RingmodCard | `plugin/juce/PagesFxMod.cpp:689` | `void updateCarriers()` | Updates the carriers. |
| `layout` | RingmodCard | `plugin/juce/PagesFxMod.cpp:718` | `void layout()` | Runs the layout step for the surrounding code. |
| `FxModPage` | FxModPage | `plugin/juce/PagesFxMod.cpp:761` | `explicit FxModPage(PluginEditor& editor)` | Constructs the object. |
| `resized` | FxModPage | `plugin/juce/PagesFxMod.cpp:799` | `void resized() override` | Lays out the child components. |
| `tick` | FxModPage | `plugin/juce/PagesFxMod.cpp:819` | `void tick() override` | Refreshes the live display state. |
| `makeFxModPage` |  | `plugin/juce/PagesFxMod.cpp:843` | `std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor)` | Creates the FX mod page. |
| `MacroRange` | MacroRange | `plugin/juce/PagesMacros.cpp:18` | `MacroRange(APVTS& state, juce::String paramID, juce::Colour fill) : state_(state), paramID_(std::move(paramID)), fill_(fill)` | Constructs the object. |
| `resized` | MacroRange | `plugin/juce/PagesMacros.cpp:33` | `void resized() override` | Lays out the child components. |
| `paint` | MacroRange | `plugin/juce/PagesMacros.cpp:38` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseDown` | MacroRange | `plugin/juce/PagesMacros.cpp:70` | `void mouseDown(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseDrag` | MacroRange | `plugin/juce/PagesMacros.cpp:78` | `void mouseDrag(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseUp` | MacroRange | `plugin/juce/PagesMacros.cpp:80` | `void mouseUp(const juce::MouseEvent&) override` | Handles a mouse interaction. |
| `setFromMouse` | MacroRange | `plugin/juce/PagesMacros.cpp:87` | `void setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `MacroTargetChoice` | MacroTargetChoice | `plugin/juce/PagesMacros.cpp:113` | `MacroTargetChoice(APVTS& state, juce::String paramID) : state_(state), paramID_(std::move(paramID))` | Constructs the object. |
| `resized` | MacroTargetChoice | `plugin/juce/PagesMacros.cpp:152` | `void resized() override` | Lays out the child components. |
| `MacroCard` | MacroCard | `plugin/juce/PagesMacros.cpp:164` | `MacroCard(PluginEditor& editor, int index) : editor_(editor), index_(index), knob_(editor.state(), params::macroValue(index), "", 92.0f), source_(editor.state(), params::macroMi...` | Constructs the object. |
| `resized` | MacroCard | `plugin/juce/PagesMacros.cpp:193` | `void resized() override` | Lays out the child components. |
| `paint` | MacroCard | `plugin/juce/PagesMacros.cpp:226` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | MacroCard | `plugin/juce/PagesMacros.cpp:246` | `void tick()` | Refreshes the live display state. |
| `title` | MacroCard | `plugin/juce/PagesMacros.cpp:274` | `juce::String title() const` | Returns the title value. |
| `MacrosPage` | MacrosPage | `plugin/juce/PagesMacros.cpp:297` | `explicit MacrosPage(PluginEditor& editor) : editor_(editor)` | Constructs the object. |
| `resized` | MacrosPage | `plugin/juce/PagesMacros.cpp:306` | `void resized() override` | Lays out the child components. |
| `paint` | MacrosPage | `plugin/juce/PagesMacros.cpp:321` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | MacrosPage | `plugin/juce/PagesMacros.cpp:323` | `void tick() override` | Refreshes the live display state. |
| `makeMacrosPage` |  | `plugin/juce/PagesMacros.cpp:336` | `std::unique_ptr<PageBase> makeMacrosPage(PluginEditor& editor)` | Creates the macros page. |
| `isCarrier` |  | `plugin/juce/PagesOperators.cpp:11` | `bool isCarrier(int algorithm, int op)` | Tests whether the the carrier. |
| `OpColumn` | OpColumn | `plugin/juce/PagesOperators.cpp:27` | `OpColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, ...` | Constructs the object. |
| `resized` | OpColumn | `plugin/juce/PagesOperators.cpp:54` | `void resized() override` | Lays out the child components. |
| `paint` | OpColumn | `plugin/juce/PagesOperators.cpp:71` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | OpColumn | `plugin/juce/PagesOperators.cpp:82` | `void tick()` | Refreshes the live display state. |
| `OpLfoColumn` | OpLfoColumn | `plugin/juce/PagesOperators.cpp:109` | `OpLfoColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(fals...` | Constructs the object. |
| `resized` | OpLfoColumn | `plugin/juce/PagesOperators.cpp:154` | `void resized() override` | Lays out the child components. |
| `paint` | OpLfoColumn | `plugin/juce/PagesOperators.cpp:171` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | OpLfoColumn | `plugin/juce/PagesOperators.cpp:182` | `void tick()` | Refreshes the live display state. |
| `OperatorsPage` | OperatorsPage | `plugin/juce/PagesOperators.cpp:207` | `explicit OperatorsPage(PluginEditor& editor) : editor_(editor)` | Constructs the object. |
| `resized` | OperatorsPage | `plugin/juce/PagesOperators.cpp:217` | `void resized() override` | Lays out the child components. |
| `tick` | OperatorsPage | `plugin/juce/PagesOperators.cpp:227` | `void tick() override` | Refreshes the live display state. |
| `OperatorLfosPage` | OperatorLfosPage | `plugin/juce/PagesOperators.cpp:240` | `explicit OperatorLfosPage(PluginEditor& editor) : editor_(editor)` | Constructs the object. |
| `resized` | OperatorLfosPage | `plugin/juce/PagesOperators.cpp:250` | `void resized() override` | Lays out the child components. |
| `tick` | OperatorLfosPage | `plugin/juce/PagesOperators.cpp:260` | `void tick() override` | Refreshes the live display state. |
| `makeOperatorsPage` |  | `plugin/juce/PagesOperators.cpp:273` | `std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor)` | Creates the operators page. |
| `makeOperatorLfosPage` |  | `plugin/juce/PagesOperators.cpp:278` | `std::unique_ptr<PageBase> makeOperatorLfosPage(PluginEditor& editor)` | Creates the operator LFOs page. |
| `rawValue` |  | `plugin/juce/PagesPhase.cpp:13` | `float rawValue(APVTS& state, const juce::String& id)` | Returns the raw value value. |
| `isCarrier` |  | `plugin/juce/PagesPhase.cpp:20` | `bool isCarrier(int algorithm, int op)` | Tests whether the the carrier. |
| `PhaseMapView` | PhaseMapView | `plugin/juce/PagesPhase.cpp:38` | `PhaseMapView(PluginEditor& editor, int op, int part) : state_(editor.state()), processor_(editor.processor()), op_(op), part_(part)` | Constructs the object. |
| `tick` | PhaseMapView | `plugin/juce/PagesPhase.cpp:47` | `void tick()` | Refreshes the live display state. |
| `paint` | PhaseMapView | `plugin/juce/PagesPhase.cpp:104` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `PhaseCard` | PhaseCard | `plugin/juce/PagesPhase.cpp:237` | `PhaseCard(PluginEditor& editor, int op, int part) : P1Card("phase", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phase); }, [&ed =...` | Constructs the object. |
| `resized` | PhaseCard | `plugin/juce/PagesPhase.cpp:265` | `void resized() override` | Lays out the child components. |
| `PhaseEnvelopeCard` | PhaseEnvelopeCard | `plugin/juce/PagesPhase.cpp:292` | `PhaseEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("phase env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phase...` | Constructs the object. |
| `resized` | PhaseEnvelopeCard | `plugin/juce/PagesPhase.cpp:349` | `void resized() override` | Lays out the child components. |
| `tick` | PhaseEnvelopeCard | `plugin/juce/PagesPhase.cpp:370` | `void tick()` | Refreshes the live display state. |
| `CompactOscillator` | CompactOscillator | `plugin/juce/PagesPhase.cpp:382` | `CompactOscillator(PluginEditor& editor, int op, int part) : ratio_(editor.state(), params::opRatio(op, part), "ratio", 22.0f), detune_(editor.state(), params::opDetune(op, part)...` | Constructs the object. |
| `resized` | CompactOscillator | `plugin/juce/PagesPhase.cpp:398` | `void resized() override` | Lays out the child components. |
| `paint` | CompactOscillator | `plugin/juce/PagesPhase.cpp:413` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `PhaseColumn` | PhaseColumn | `plugin/juce/PagesPhase.cpp:432` | `PhaseColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(fals...` | Constructs the object. |
| `resized` | PhaseColumn | `plugin/juce/PagesPhase.cpp:468` | `void resized() override` | Lays out the child components. |
| `paint` | PhaseColumn | `plugin/juce/PagesPhase.cpp:484` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | PhaseColumn | `plugin/juce/PagesPhase.cpp:496` | `void tick()` | Refreshes the live display state. |
| `PhaseMapPage` | PhaseMapPage | `plugin/juce/PagesPhase.cpp:517` | `explicit PhaseMapPage(PluginEditor& editor) : editor_(editor)` | Constructs the object. |
| `resized` | PhaseMapPage | `plugin/juce/PagesPhase.cpp:528` | `void resized() override` | Lays out the child components. |
| `tick` | PhaseMapPage | `plugin/juce/PagesPhase.cpp:541` | `void tick() override` | Refreshes the live display state. |
| `makePhaseMapPage` |  | `plugin/juce/PagesPhase.cpp:554` | `std::unique_ptr<PageBase> makePhaseMapPage(PluginEditor& editor)` | Creates the phase map page. |
| `isCarrier` |  | `plugin/juce/PagesSpectral.cpp:15` | `bool isCarrier(int algorithm, int op)` | Tests whether the the carrier. |
| `spectrumIDs` |  | `plugin/juce/PagesSpectral.cpp:29` | `juce::StringArray spectrumIDs(int op, int part, bool a)` | Returns the spectrum i ds value. |
| `rangeNames` |  | `plugin/juce/PagesSpectral.cpp:38` | `juce::StringArray rangeNames()` | Returns the range names value. |
| `seriesHarmonic` |  | `plugin/juce/PagesSpectral.cpp:57` | `bool seriesHarmonic(int base, int harmonic)` | Tests the series harmonic condition. |
| `groupHas` |  | `plugin/juce/PagesSpectral.cpp:68` | `bool groupHas(int group, int harmonic)` | Tests the group has condition. |
| `SpringSlider` | SpringSlider | `plugin/juce/PagesSpectral.cpp:91` | `SpringSlider(juce::String label, std::function<void()> onBegin, std::function<void(double)> onChange, std::function<void()> onEnd = {}) : label_(std::move(label)), onBegin_(std:...` | Constructs the object. |
| `paint` | SpringSlider | `plugin/juce/PagesSpectral.cpp:102` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseDown` | SpringSlider | `plugin/juce/PagesSpectral.cpp:131` | `void mouseDown(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseDrag` | SpringSlider | `plugin/juce/PagesSpectral.cpp:142` | `void mouseDrag(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseUp` | SpringSlider | `plugin/juce/PagesSpectral.cpp:149` | `void mouseUp(const juce::MouseEvent&) override` | Handles a mouse interaction. |
| `valueText` | SpringSlider | `plugin/juce/PagesSpectral.cpp:161` | `juce::String valueText() const` | Returns the value text value. |
| `setAmount` | SpringSlider | `plugin/juce/PagesSpectral.cpp:168` | `void setAmount(double amount)` | Writes the amount. |
| `MaskRow` | MaskRow | `plugin/juce/PagesSpectral.cpp:187` | `explicit MaskRow(int initial) : index_(juce::jlimit(0, 2, initial))` | Constructs the object. |
| `index` | MaskRow | `plugin/juce/PagesSpectral.cpp:192` | `int index() const` | Returns the index value. |
| `paint` | MaskRow | `plugin/juce/PagesSpectral.cpp:194` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseDown` | MaskRow | `plugin/juce/PagesSpectral.cpp:215` | `void mouseDown(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `MorphEnvelopeCard` | MorphEnvelopeCard | `plugin/juce/PagesSpectral.cpp:239` | `MorphEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("morph env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::spect...` | Constructs the object. |
| `resized` | MorphEnvelopeCard | `plugin/juce/PagesSpectral.cpp:295` | `void resized() override` | Lays out the child components. |
| `tick` | MorphEnvelopeCard | `plugin/juce/PagesSpectral.cpp:315` | `void tick()` | Refreshes the live display state. |
| `SpectrumRow` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:327` | `SpectrumRow(PluginEditor& editor, int op, int part, bool a) : P1Card("op" + juce::String(op + 1) + " spectrum " + (a ? "a" : "b"), [&ed = editor, op, part, a] { ed.copyOperatorS...` | Constructs the object. |
| `tick` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:432` | `void tick()` | Refreshes the live display state. |
| `resized` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:445` | `void resized() override` | Lays out the child components. |
| `firstHarmonic` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:489` | `int firstHarmonic() const` | Returns the first harmonic value. |
| `refreshGroupTest` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:499` | `void refreshGroupTest()` | Refreshes the group test display. |
| `windowFor` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:507` | `std::pair<int, int> windowFor(int index) const` | Returns the window for value. |
| `refreshSelectionItems` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:517` | `void refreshSelectionItems()` | Refreshes the selection items display. |
| `applySelection` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:530` | `void applySelection()` | Applies the selection. |
| `editable` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:539` | `bool editable(int harmonic, double level) const` | Tests the editable condition. |
| `selected` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:551` | `bool selected(int index) const` | Selects the ed. |
| `beginMacro` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:570` | `void beginMacro()` | Runs the begin macro step for the surrounding code. |
| `endMacro` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:572` | `void endMacro()` | Runs the end macro step for the surrounding code. |
| `applyTilt` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:574` | `void applyTilt(double amount)` | Applies the tilt. |
| `applyLevel` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:593` | `void applyLevel(double amount)` | Applies the level. |
| `applyFormant` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:608` | `void applyFormant(double amount)` | Applies the formant. |
| `normalise` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:630` | `void normalise()` | Runs the normalise step for the surrounding code. |
| `copyToOther` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:643` | `void copyToOther()` | Copies the to other. |
| `showTemplateMenu` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:656` | `void showTemplateMenu()` | Opens the template menu. |
| `loadTemplate` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:671` | `void loadTemplate(int wave)` | Loads the template. |
| `refreshLegend` | SpectrumRow | `plugin/juce/PagesSpectral.cpp:682` | `void refreshLegend()` | Refreshes the legend display. |
| `SpectraOperatorCard` | SpectraOperatorCard | `plugin/juce/PagesSpectral.cpp:734` | `SpectraOperatorCard(PluginEditor& editor, int op) : editor_(editor), op_(op), part_(editor.activePart()), header_( false, false, {}, [&ed = editor, op] { ed.copyOperator(op); },...` | Constructs the object. |
| `resized` | SpectraOperatorCard | `plugin/juce/PagesSpectral.cpp:759` | `void resized() override` | Lays out the child components. |
| `tick` | SpectraOperatorCard | `plugin/juce/PagesSpectral.cpp:772` | `void tick()` | Refreshes the live display state. |
| `paint` | SpectraOperatorCard | `plugin/juce/PagesSpectral.cpp:783` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `SpectraModColumn` | SpectraModColumn | `plugin/juce/PagesSpectral.cpp:808` | `SpectraModColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_...` | Constructs the object. |
| `resized` | SpectraModColumn | `plugin/juce/PagesSpectral.cpp:852` | `void resized() override` | Lays out the child components. |
| `paint` | SpectraModColumn | `plugin/juce/PagesSpectral.cpp:866` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | SpectraModColumn | `plugin/juce/PagesSpectral.cpp:878` | `void tick()` | Refreshes the live display state. |
| `OperatorSpectraPage` | OperatorSpectraPage | `plugin/juce/PagesSpectral.cpp:900` | `OperatorSpectraPage(PluginEditor& editor, bool a)` | Constructs the object. |
| `resized` | OperatorSpectraPage | `plugin/juce/PagesSpectral.cpp:916` | `void resized() override` | Lays out the child components. |
| `tick` | OperatorSpectraPage | `plugin/juce/PagesSpectral.cpp:937` | `void tick() override` | Refreshes the live display state. |
| `OperatorSpectraModPage` | OperatorSpectraModPage | `plugin/juce/PagesSpectral.cpp:954` | `explicit OperatorSpectraModPage(PluginEditor& editor) : editor_(editor)` | Constructs the object. |
| `resized` | OperatorSpectraModPage | `plugin/juce/PagesSpectral.cpp:965` | `void resized() override` | Lays out the child components. |
| `tick` | OperatorSpectraModPage | `plugin/juce/PagesSpectral.cpp:975` | `void tick() override` | Refreshes the live display state. |
| `makeOperatorSpectraPage` |  | `plugin/juce/PagesSpectral.cpp:988` | `std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor, bool a)` | Creates the operator spectra page. |
| `makeOperatorSpectraModPage` |  | `plugin/juce/PagesSpectral.cpp:993` | `std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)` | Creates the operator spectra mod page. |
| `PatchButton` | PatchButton | `plugin/juce/PluginEditor.cpp:19` | `PatchButton() : juce::Button("multi-patch library...")` | Constructs the object. |
| `paintButton` | PatchButton | `plugin/juce/PluginEditor.cpp:24` | `void paintButton(juce::Graphics& g, bool highlighted, bool) override` | Draws the button in its normal, highlighted and down states. |
| `SegmentButton` | SegmentButton | `plugin/juce/PluginEditor.cpp:40` | `explicit SegmentButton(const juce::String& label) : juce::Button(label)` | Constructs the object. |
| `setActive` | SegmentButton | `plugin/juce/PluginEditor.cpp:45` | `void setActive(bool active)` | Sets the active flag. |
| `paintButton` | SegmentButton | `plugin/juce/PluginEditor.cpp:53` | `void paintButton(juce::Graphics& g, bool highlighted, bool) override` | Draws the button in its normal, highlighted and down states. |
| `PartChoiceButton` | PartChoiceButton | `plugin/juce/PluginEditor.cpp:74` | `explicit PartChoiceButton(const juce::String& label) : juce::Button(label)` | Constructs the object. |
| `setActive` | PartChoiceButton | `plugin/juce/PluginEditor.cpp:79` | `void setActive(bool active)` | Sets the active flag. |
| `paintButton` | PartChoiceButton | `plugin/juce/PluginEditor.cpp:87` | `void paintButton(juce::Graphics& g, bool highlighted, bool) override` | Draws the button in its normal, highlighted and down states. |
| `Header` | Header | `plugin/juce/PluginEditor.cpp:110` | `explicit Header(PluginEditor& editor) : editor_(editor), volume_(editor.state(), p1::kVolumeParamID)` | Constructs the object. |
| `resized` | Header | `plugin/juce/PluginEditor.cpp:137` | `void resized() override` | Lays out the child components. |
| `paint` | Header | `plugin/juce/PluginEditor.cpp:149` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | Header | `plugin/juce/PluginEditor.cpp:171` | `void tick()` | Refreshes the live display state. |
| `setPatchName` | Header | `plugin/juce/PluginEditor.cpp:205` | `void setPatchName(const juce::String& name)` | Writes the patch name. |
| `setSpectroMode` | Header | `plugin/juce/PluginEditor.cpp:213` | `void setSpectroMode(bool spectro)` | Writes the spectro mode. |
| `drawWordmark` | Header | `plugin/juce/PluginEditor.cpp:223` | `void drawWordmark(juce::Graphics& g)` | Draws the wordmark. |
| `Toolbar` | Toolbar | `plugin/juce/PluginEditor.cpp:271` | `explicit Toolbar(PluginEditor& editor) : editor_(editor)` | Constructs the object. |
| `resized` | Toolbar | `plugin/juce/PluginEditor.cpp:306` | `void resized() override` | Lays out the child components. |
| `paint` | Toolbar | `plugin/juce/PluginEditor.cpp:324` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `refreshPartNames` | Toolbar | `plugin/juce/PluginEditor.cpp:337` | `void refreshPartNames()` | Refreshes the part names display. |
| `tick` | Toolbar | `plugin/juce/PluginEditor.cpp:347` | `void tick()` | Refreshes the live display state. |
| `drawLegend` | Toolbar | `plugin/juce/PluginEditor.cpp:364` | `void drawLegend(juce::Graphics& g)` | Draws the legend. |
| `ListBody` | ListBody | `plugin/juce/PluginEditor.cpp:402` | `ListBody(int count, int rowHeight, int selected, const juce::String& name) : count_(juce::jmax(0, count)), selected_(juce::jlimit(0, juce::jmax(0, count_ - 1), selected)), list_...` | Constructs the object. |
| `selected` | ListBody | `plugin/juce/PluginEditor.cpp:417` | `int selected() const` | Selects the ed. |
| `resized` | ListBody | `plugin/juce/PluginEditor.cpp:419` | `void resized() override` | Lays out the child components. |
| `getNumRows` | ListBody | `plugin/juce/PluginEditor.cpp:425` | `int getNumRows() override` | Reads the num rows. |
| `paintListBoxItem` | ListBody | `plugin/juce/PluginEditor.cpp:427` | `void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override` | Draws one list-box row. |
| `listBoxItemClicked` | ListBody | `plugin/juce/PluginEditor.cpp:445` | `void listBoxItemClicked(int row, const juce::MouseEvent&) override` | Handles a list-box row click. |
| `paintRow` | ListBody | `plugin/juce/PluginEditor.cpp:453` | `virtual void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) = 0` | Draws one row of the control. |
| `SlotListBody` | SlotListBody | `plugin/juce/PluginEditor.cpp:464` | `SlotListBody(int count, std::function<juce::String(int)> nameFor, std::function<bool(int)> usedFor, int selected) : ListBody(count, 24, selected, "slots"), nameFor_(std::move(na...` | Constructs the object. |
| `paintRow` | SlotListBody | `plugin/juce/PluginEditor.cpp:473` | `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override` | Draws one row of the control. |
| `ModeListBody` | ModeListBody | `plugin/juce/PluginEditor.cpp:498` | `ModeListBody(juce::StringArray modes, int selected) : ListBody(modes.size(), 28, selected, "modes"), modes_(std::move(modes))` | Constructs the object. |
| `paintRow` | ModeListBody | `plugin/juce/PluginEditor.cpp:505` | `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override` | Draws one row of the control. |
| `AlgTile` | AlgTile | `plugin/juce/PluginEditor.cpp:520` | `AlgTile(int index, std::function<void(int)> onSelect) : juce::Button("alg"), index_(index), onSelect_(std::move(onSelect))` | Constructs the object. |
| `setSelected` | AlgTile | `plugin/juce/PluginEditor.cpp:526` | `void setSelected(bool selected)` | Sets the selected flag. |
| `paintButton` | AlgTile | `plugin/juce/PluginEditor.cpp:534` | `void paintButton(juce::Graphics& g, bool highlighted, bool) override` | Draws the button in its normal, highlighted and down states. |
| `mouseUp` | AlgTile | `plugin/juce/PluginEditor.cpp:552` | `void mouseUp(const juce::MouseEvent&) override` | Handles a mouse interaction. |
| `AlgPickerBody` | AlgPickerBody | `plugin/juce/PluginEditor.cpp:566` | `explicit AlgPickerBody(int selected) : selected_(juce::jlimit(0, 9, selected))` | Constructs the object. |
| `selected` | AlgPickerBody | `plugin/juce/PluginEditor.cpp:583` | `int selected() const` | Selects the ed. |
| `resized` | AlgPickerBody | `plugin/juce/PluginEditor.cpp:585` | `void resized() override` | Lays out the child components. |
| `select` | AlgPickerBody | `plugin/juce/PluginEditor.cpp:600` | `void select(int index)` | Runs the select step for the surrounding code. |
| `RenameBody` | RenameBody | `plugin/juce/PluginEditor.cpp:614` | `explicit RenameBody(const juce::String& initial)` | Constructs the object. |
| `text` | RenameBody | `plugin/juce/PluginEditor.cpp:631` | `juce::String text() const` | Returns the text value. |
| `focus` | RenameBody | `plugin/juce/PluginEditor.cpp:632` | `void focus()` | Moves keyboard focus to the control. |
| `resized` | RenameBody | `plugin/juce/PluginEditor.cpp:634` | `void resized() override` | Lays out the child components. |
| `PluginEditor` |  | `plugin/juce/PluginEditor.cpp:649` | `PluginEditor::PluginEditor(TypePAlphaAudioProcessor& audioProcessor) : AudioProcessorEditor(&audioProcessor), processor_(audioProcessor)` | Builds the editor, the header, the toolbar, the tabs and the first page, then starts the 30 Hz timer. |
| `~PluginEditor` |  | `plugin/juce/PluginEditor.cpp:681` | `PluginEditor::~PluginEditor()` | Destroys the object. |
| `paint` |  | `plugin/juce/PluginEditor.cpp:688` | `void PluginEditor::paint(juce::Graphics& g)` | Draws the component. |
| `resized` |  | `plugin/juce/PluginEditor.cpp:693` | `void PluginEditor::resized()` | Lays out the child components. |
| `timerCallback` |  | `plugin/juce/PluginEditor.cpp:709` | `void PluginEditor::timerCallback()` | Runs the startup fit and refreshes the live visualisers. |
| `fitToHostWindow` |  | `plugin/juce/PluginEditor.cpp:726` | `void PluginEditor::fitToHostWindow()` | Measures the host chrome and sizes the editor to the display work area. |
| `updateVisualizers` |  | `plugin/juce/PluginEditor.cpp:765` | `void PluginEditor::updateVisualizers()` | Refreshes the header, toolbar and page once per timer tick. |
| `refreshMacroFeedback` |  | `plugin/juce/PluginEditor.cpp:787` | `void PluginEditor::refreshMacroFeedback(juce::Component& root)` | Walks the component tree and updates every macro feedback target. |
| `buildUi` |  | `plugin/juce/PluginEditor.cpp:800` | `void PluginEditor::buildUi()` | Creates the header, toolbar and tab bar and rebuilds the current page. |
| `buildPage` |  | `plugin/juce/PluginEditor.cpp:834` | `void PluginEditor::buildPage()` | Destroys the old page and creates the page for the active tab. |
| `setActivePart` |  | `plugin/juce/PluginEditor.cpp:876` | `void PluginEditor::setActivePart(int part)` | Writes the active part. |
| `showTab` |  | `plugin/juce/PluginEditor.cpp:886` | `void PluginEditor::showTab(int tab)` | Opens the tab. |
| `refreshNames` |  | `plugin/juce/PluginEditor.cpp:899` | `void PluginEditor::refreshNames()` | Refreshes the names display. |
| `showAlgorithmPicker` |  | `plugin/juce/PluginEditor.cpp:906` | `void PluginEditor::showAlgorithmPicker()` | Opens the ten-tile algorithm picker. |
| `applyAlgorithm` |  | `plugin/juce/PluginEditor.cpp:926` | `void PluginEditor::applyAlgorithm(int algorithm)` | Writes the selected algorithm to the active part. |
| `showRenameDialog` |  | `plugin/juce/PluginEditor.cpp:934` | `void PluginEditor::showRenameDialog(bool part, int index)` | Opens the rename dialog for a part or the patch. |
| `showRenameMacro` |  | `plugin/juce/PluginEditor.cpp:961` | `void PluginEditor::showRenameMacro(int index)` | Opens the rename dialog for a macro. |
| `showSinglePartsBrowser` |  | `plugin/juce/PluginEditor.cpp:984` | `void PluginEditor::showSinglePartsBrowser()` | Opens the single-part library browser. |
| `exportLibrary` |  | `plugin/juce/PluginEditor.cpp:1062` | `void PluginEditor::exportLibrary()` | Exports the library to a chosen file. |
| `importLibrary` |  | `plugin/juce/PluginEditor.cpp:1082` | `void PluginEditor::importLibrary()` | Imports a library file. |
| `fxSectionParamIDs` |  | `plugin/juce/PluginEditor.cpp:1113` | `static std::vector<juce::String> fxSectionParamIDs( int part, PluginEditor::PartFxSection section)` | Returns the FX section param i ds value. |
| `copyPart` |  | `plugin/juce/PluginEditor.cpp:1180` | `void PluginEditor::copyPart(int part)` | Copies the part parameters and routing into the clipboard. |
| `pastePart` |  | `plugin/juce/PluginEditor.cpp:1193` | `void PluginEditor::pastePart(int part)` | Writes the part clipboard to the current part. |
| `copyPartFxSection` |  | `plugin/juce/PluginEditor.cpp:1204` | `void PluginEditor::copyPartFxSection(PartFxSection section)` | Copies one part fx/mod card to the other part. |
| `copyOperator` |  | `plugin/juce/PluginEditor.cpp:1220` | `void PluginEditor::copyOperator(int op)` | Copies one operator's parameters. |
| `pasteOperator` |  | `plugin/juce/PluginEditor.cpp:1242` | `void PluginEditor::pasteOperator(int op)` | Writes one operator clipboard to the current operator. |
| `sectionParamIDs` |  | `plugin/juce/PluginEditor.cpp:1275` | `std::vector<juce::String> sectionParamIDs(int op, int part, PluginEditor::OperatorSection section)` | The parameters each operator card owns, in its display order. The list is the card's contract: copy fills the clipboard from it and paste writes back through it, so a card cannot copy some of its controls and miss others. The amplitude envelope's surge and release are the op1-only `attack` and `release` names, and opEnv maps them for the source and destination op. |
| `copyOperatorSection` |  | `plugin/juce/PluginEditor.cpp:1385` | `void PluginEditor::copyOperatorSection(int op, int part, OperatorSection section)` | Copies one operator section, such as the oscillator or an envelope. |
| `pasteOperatorSection` |  | `plugin/juce/PluginEditor.cpp:1398` | `void PluginEditor::pasteOperatorSection(int op, int part, OperatorSection section)` | Writes one operator section and, for a spectrum section, selects spectrum mode. |
| `initPart` |  | `plugin/juce/PluginEditor.cpp:1426` | `void PluginEditor::initPart(int part)` | Resets a part to the factory init values. |
| `initMultiPatch` |  | `plugin/juce/PluginEditor.cpp:1442` | `void PluginEditor::initMultiPatch()` | Resets the current multi-patch. |
| `togglePartMute` |  | `plugin/juce/PluginEditor.cpp:1450` | `void PluginEditor::togglePartMute(int part, bool muted)` | Mutes or restores a part through its level parameter. |
| `currentPartParams` |  | `plugin/juce/PluginEditor.cpp:1466` | `std::vector<int32_t> PluginEditor::currentPartParams(int part)` | Collects the part-block parameters for save or copy. |
| `applyPartParams` |  | `plugin/juce/PluginEditor.cpp:1480` | `void PluginEditor::applyPartParams(int part, const std::vector<int32_t>& params)` | Writes a part-block parameter vector. |
| `currentRouting` |  | `plugin/juce/PluginEditor.cpp:1496` | `int PluginEditor::currentRouting(int field, int part)` | Reads one per-part routing value. |
| `applyRouting` |  | `plugin/juce/PluginEditor.cpp:1507` | `void PluginEditor::applyRouting(int field, int part, int value)` | Writes one per-part routing value. |
| `currentMacroParams` |  | `plugin/juce/PluginEditor.cpp:1518` | `std::vector<int32_t> PluginEditor::currentMacroParams()` | Collects the 54 macro parameters. |
| `applyMacroParams` |  | `plugin/juce/PluginEditor.cpp:1530` | `void PluginEditor::applyMacroParams(const std::vector<int32_t>& params)` | Writes the 54 macro parameters. |
| `savePartToSlot` |  | `plugin/juce/PluginEditor.cpp:1548` | `void PluginEditor::savePartToSlot(int part, int slot)` | Saves the current part to a library slot. |
| `loadPartFromSlot` |  | `plugin/juce/PluginEditor.cpp:1558` | `void PluginEditor::loadPartFromSlot(int part, int slot)` | Loads a library part into the current part. |
| `savePatchToSlot` |  | `plugin/juce/PluginEditor.cpp:1571` | `void PluginEditor::savePatchToSlot(int slot, int mode)` | Saves the current patch to a multi slot. |
| `reportLibrarySave` |  | `plugin/juce/PluginEditor.cpp:1620` | `void PluginEditor::reportLibrarySave(const LibrarySaveResult& result)` | Reports a library save result to the player. |
| `loadPatchFromSlot` |  | `plugin/juce/PluginEditor.cpp:1647` | `void PluginEditor::loadPatchFromSlot(int slot)` | Loads a multi slot into both parts. |
| `showConfirm` |  | `plugin/juce/PluginEditor.cpp:1682` | `void PluginEditor::showConfirm(const juce::String& message, std::function<void()> onConfirm, const juce::String& title, const juce::String& confirm)` | Opens a confirmation dialog. |
| `showPatchBrowser` |  | `plugin/juce/PluginEditor.cpp:1706` | `void PluginEditor::showPatchBrowser()` | Opens the multi-patch browser. |
| `showPatchSaveMode` |  | `plugin/juce/PluginEditor.cpp:1749` | `void PluginEditor::showPatchSaveMode(int slot)` | Opens the patch save-mode dialog. |
| `closeDialog` |  | `plugin/juce/PluginEditor.cpp:1781` | `void PluginEditor::closeDialog()` | Closes the current dialog. |
| `paint` |  | `plugin/juce/PluginEditor.cpp:1789` | `void p1::ui::P1Canvas::paint(juce::Graphics& g)` | Draws the component. |
| `paint` | P1Canvas | `plugin/juce/PluginEditor.h:23` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `~PageBase` | PageBase | `plugin/juce/PluginEditor.h:28` | `~PageBase() override = default` | Destroys the object. |
| `tick` | PageBase | `plugin/juce/PluginEditor.h:29` | `virtual void tick() = 0` | Refreshes the live display state. |
| `paint` | PluginEditor | `plugin/juce/PluginEditor.h:65` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `resized` | PluginEditor | `plugin/juce/PluginEditor.h:66` | `void resized() override` | Scales the fixed design canvas into the host window. |
| `timerCallback` | PluginEditor | `plugin/juce/PluginEditor.h:67` | `void timerCallback() override` | Runs the startup fit and refreshes the live visualisers. |
| `processor` | PluginEditor | `plugin/juce/PluginEditor.h:69` | `TypePAlphaAudioProcessor& processor()` | Returns the audio processor. |
| `state` | PluginEditor | `plugin/juce/PluginEditor.h:70` | `juce::AudioProcessorValueTreeState& state()` | Returns the APVTS. |
| `activePart` | PluginEditor | `plugin/juce/PluginEditor.h:71` | `int activePart() const` | Returns the active part index. |
| `activeTab` | PluginEditor | `plugin/juce/PluginEditor.h:72` | `int activeTab() const` | Returns the active page index. |
| `setActivePart` | PluginEditor | `plugin/juce/PluginEditor.h:74` | `void setActivePart(int part)` | Writes the active part. |
| `showTab` | PluginEditor | `plugin/juce/PluginEditor.h:75` | `void showTab(int tab)` | Opens the tab. |
| `refreshNames` | PluginEditor | `plugin/juce/PluginEditor.h:76` | `void refreshNames()` | Refreshes the names display. |
| `reportLibrarySave` | PluginEditor | `plugin/juce/PluginEditor.h:77` | `void reportLibrarySave(const LibrarySaveResult& result)` | Reports a library save result to the player. |
| `showAlgorithmPicker` | PluginEditor | `plugin/juce/PluginEditor.h:78` | `void showAlgorithmPicker()` | Opens the ten-tile algorithm picker. |
| `applyAlgorithm` | PluginEditor | `plugin/juce/PluginEditor.h:79` | `void applyAlgorithm(int algorithm)` | Writes the selected algorithm to the active part. |
| `showRenameDialog` | PluginEditor | `plugin/juce/PluginEditor.h:80` | `void showRenameDialog(bool part, int index)` | Opens the rename dialog for a part or the patch. |
| `showRenameMacro` | PluginEditor | `plugin/juce/PluginEditor.h:81` | `void showRenameMacro(int index)` | Opens the rename dialog for a macro. |
| `showSinglePartsBrowser` | PluginEditor | `plugin/juce/PluginEditor.h:82` | `void showSinglePartsBrowser()` | Opens the single-part library browser. |
| `exportLibrary` | PluginEditor | `plugin/juce/PluginEditor.h:83` | `void exportLibrary()` | Exports the library to a chosen file. |
| `importLibrary` | PluginEditor | `plugin/juce/PluginEditor.h:84` | `void importLibrary()` | Imports a library file. |
| `showPatchBrowser` | PluginEditor | `plugin/juce/PluginEditor.h:85` | `void showPatchBrowser()` | Opens the multi-patch browser. |
| `showPatchSaveMode` | PluginEditor | `plugin/juce/PluginEditor.h:86` | `void showPatchSaveMode(int slot)` | Opens the patch save-mode dialog. |
| `copyPart` | PluginEditor | `plugin/juce/PluginEditor.h:87` | `void copyPart(int part)` | Copies the part parameters and routing into the clipboard. |
| `pastePart` | PluginEditor | `plugin/juce/PluginEditor.h:88` | `void pastePart(int part)` | Writes the part clipboard to the current part. |
| `copyOperator` | PluginEditor | `plugin/juce/PluginEditor.h:89` | `void copyOperator(int op)` | Copies one operator's parameters. |
| `pasteOperator` | PluginEditor | `plugin/juce/PluginEditor.h:90` | `void pasteOperator(int op)` | Writes one operator clipboard to the current operator. |
| `copyOperatorSection` | PluginEditor | `plugin/juce/PluginEditor.h:91` | `void copyOperatorSection(int op, int part, OperatorSection section)` | Copies one operator section, such as the oscillator or an envelope. |
| `pasteOperatorSection` | PluginEditor | `plugin/juce/PluginEditor.h:92` | `void pasteOperatorSection(int op, int part, OperatorSection section)` | Writes one operator section and, for a spectrum section, selects spectrum mode. |
| `copyPartFxSection` | PluginEditor | `plugin/juce/PluginEditor.h:94` | `void copyPartFxSection(PartFxSection section)` | Copy one part fx/mod card from the active part to the other part. |
| `initPart` | PluginEditor | `plugin/juce/PluginEditor.h:95` | `void initPart(int part)` | Resets a part to the factory init values. |
| `initMultiPatch` | PluginEditor | `plugin/juce/PluginEditor.h:96` | `void initMultiPatch()` | Resets the current multi-patch. |
| `togglePartMute` | PluginEditor | `plugin/juce/PluginEditor.h:97` | `void togglePartMute(int part, bool muted)` | Mutes or restores a part through its level parameter. |
| `savePartToSlot` | PluginEditor | `plugin/juce/PluginEditor.h:98` | `void savePartToSlot(int part, int slot)` | Saves the current part to a library slot. |
| `loadPartFromSlot` | PluginEditor | `plugin/juce/PluginEditor.h:99` | `void loadPartFromSlot(int part, int slot)` | Loads a library part into the current part. |
| `savePatchToSlot` | PluginEditor | `plugin/juce/PluginEditor.h:100` | `void savePatchToSlot(int slot, int mode = 0)` | Saves the current patch to a multi slot. |
| `loadPatchFromSlot` | PluginEditor | `plugin/juce/PluginEditor.h:101` | `void loadPatchFromSlot(int slot)` | Loads a multi slot into both parts. |
| `showConfirm` | PluginEditor | `plugin/juce/PluginEditor.h:102` | `void showConfirm(const juce::String& message, std::function<void()> onConfirm, const juce::String& title = "confirm save", const juce::String& confirm = "save")` | Opens a confirmation dialog. |
| `closeDialog` | PluginEditor | `plugin/juce/PluginEditor.h:105` | `void closeDialog()` | Closes the current dialog. |
| `updateVisualizers` | PluginEditor | `plugin/juce/PluginEditor.h:106` | `void updateVisualizers()` | Refreshes the header, toolbar and page once per timer tick. |
| `buildUi` | PluginEditor | `plugin/juce/PluginEditor.h:108` | `void buildUi()` | Creates the header, toolbar and tab bar and rebuilds the current page. |
| `buildHeader` | PluginEditor | `plugin/juce/PluginEditor.h:109` | `void buildHeader()` | Creates the header component. |
| `buildToolbar` | PluginEditor | `plugin/juce/PluginEditor.h:110` | `void buildToolbar()` | Creates the toolbar component. |
| `buildPage` | PluginEditor | `plugin/juce/PluginEditor.h:111` | `void buildPage()` | Destroys the old page and creates the page for the active tab. |
| `refreshMacroFeedback` | PluginEditor | `plugin/juce/PluginEditor.h:112` | `void refreshMacroFeedback(juce::Component& root)` | Walks the component tree and updates every macro feedback target. |
| `setSpectroMode` | PluginEditor | `plugin/juce/PluginEditor.h:113` | `void setSpectroMode(bool spectro)` | Writes the spectro mode. |
| `fitToHostWindow` | PluginEditor | `plugin/juce/PluginEditor.h:114` | `void fitToHostWindow()` | Measures the host chrome and sizes the editor to the display work area. |
| `currentPartParams` | PluginEditor | `plugin/juce/PluginEditor.h:115` | `std::vector<int32_t> currentPartParams(int part)` | Collects the part-block parameters for save or copy. |
| `applyPartParams` | PluginEditor | `plugin/juce/PluginEditor.h:116` | `void applyPartParams(int part, const std::vector<int32_t>& params)` | Writes a part-block parameter vector. |
| `currentRouting` | PluginEditor | `plugin/juce/PluginEditor.h:117` | `int currentRouting(int field, int part)` | Reads one per-part routing value. |
| `applyRouting` | PluginEditor | `plugin/juce/PluginEditor.h:118` | `void applyRouting(int field, int part, int value)` | Writes one per-part routing value. |
| `currentMacroParams` | PluginEditor | `plugin/juce/PluginEditor.h:119` | `std::vector<int32_t> currentMacroParams()` | Collects the 54 macro parameters. |
| `applyMacroParams` | PluginEditor | `plugin/juce/PluginEditor.h:120` | `void applyMacroParams(const std::vector<int32_t>& params)` | Writes the 54 macro parameters. |
| `isEngineMessage` |  | `plugin/juce/PluginProcessor.cpp:36` | `bool isEngineMessage(const juce::MidiMessage& message)` | Tests whether the the engine message. |
| `engineHandlesVelocityTarget` |  | `plugin/juce/PluginProcessor.cpp:48` | `bool engineHandlesVelocityTarget(int target, int memberId)` | The engine applies the velocity source itself for the operator level, feedback, morph, envelope rate, LFO rate and LFO depth groups, on the all-operator and the carrier and modulator rows. The phase envelope and phase LFO and the part-level pitch and filter envelopes stay in the plugin's part-level layer. |
| `operatorEnvelopeIndices` |  | `plugin/juce/PluginProcessor.cpp:69` | `std::array<int, 4> operatorEnvelopeIndices()` | The part-block indices of the four operator warm-up rates. The migration needs them because the operator envelope grew from seven to eight parameters inside each operator block. |
| `partParamIndex` |  | `plugin/juce/PluginProcessor.cpp:87` | `int partParamIndex(const std::string& wanted)` | The part-block index of a `.p0` parameter, or -1. The preset migration needs these for the remaps that cannot be expressed as an append. |
| `panEnvelopeIndex` |  | `plugin/juce/PluginProcessor.cpp:97` | `int panEnvelopeIndex()` | The part-block index of the pan envelope, for the version 11 remap. |
| `feedbackEnvelopeIndices` |  | `plugin/juce/PluginProcessor.cpp:101` | `std::array<int, 4> feedbackEnvelopeIndices()` | The part-block indices of each operator feedback envelope, for the version 12 remap. |
| `configurePresetStore` |  | `plugin/juce/PluginProcessor.cpp:114` | `void configurePresetStore(p1::PresetStore& store)` | Every PresetStore that reads an old library needs the same block indices for the remaps. The in-memory store used to get these, whilst the temporary disk store in saveLibrary did not, so an old record was migrated with -1 bases and then written back corrupted. Configure both through this one function. |
| `midi_get` |  | `plugin/juce/PluginProcessor.cpp:152` | `int midi_get(uint8_t* status, uint8_t* data1, uint8_t* data2)` | Returns the MIDI get value. |
| `midi_get` |  | `plugin/juce/PluginProcessor.cpp:154` | `int midi_get(uint8_t*, uint8_t*, uint8_t*)` | Returns the MIDI get value. |
| `ScopeRing` |  | `plugin/juce/PluginProcessor.cpp:159` | `ScopeRing::ScopeRing() = default` | Returns the scope ring value. |
| `push` |  | `plugin/juce/PluginProcessor.cpp:161` | `void ScopeRing::push(const float* data, int count)` | Returns the push value. |
| `read` |  | `plugin/juce/PluginProcessor.cpp:174` | `void ScopeRing::read(float* destination, int count)` | Returns the read value. |
| `getPartName` |  | `plugin/juce/PluginProcessor.cpp:194` | `juce::String TypePAlphaAudioProcessor::getPartName(int part) const` | Reads the part name. |
| `setPartName` |  | `plugin/juce/PluginProcessor.cpp:199` | `void TypePAlphaAudioProcessor::setPartName(int part, const juce::String& name)` | Writes the part name. |
| `getMacroName` |  | `plugin/juce/PluginProcessor.cpp:204` | `juce::String TypePAlphaAudioProcessor::getMacroName(int index) const` | Reads the macro name. |
| `setMacroName` |  | `plugin/juce/PluginProcessor.cpp:209` | `void TypePAlphaAudioProcessor::setMacroName(int index, const juce::String& name)` | Writes the macro name. |
| `macroEffectiveValue` |  | `plugin/juce/PluginProcessor.cpp:215` | `double TypePAlphaAudioProcessor::macroEffectiveValue(int index) const` | Returns the macro effective value value. |
| `macroFeedback` |  | `plugin/juce/PluginProcessor.cpp:239` | `TypePAlphaAudioProcessor::MacroFeedback TypePAlphaAudioProcessor::macroFeedback(int index) const` | Returns the macro feedback value. |
| `readOutputScope` |  | `plugin/juce/PluginProcessor.cpp:303` | `void TypePAlphaAudioProcessor::readOutputScope(float* destination, int count)` | Reads the output scope. |
| `readTapScope` |  | `plugin/juce/PluginProcessor.cpp:308` | `void TypePAlphaAudioProcessor::readTapScope(int channel, float* destination, int count)` | Reads the tap scope. |
| `TypePAlphaAudioProcessor` |  | `plugin/juce/PluginProcessor.cpp:317` | `TypePAlphaAudioProcessor::TypePAlphaAudioProcessor() : AudioProcessor(BusesProperties().withOutput( "Output", juce::AudioChannelSet::stereo(), true)), midi_(engine_), apvts_(*th...` | Constructs the processor, the engine interface, the MIDI bridge, the APVTS and the library. |
| `createLayout` |  | `plugin/juce/PluginProcessor.cpp:372` | `juce::AudioProcessorValueTreeState::ParameterLayout TypePAlphaAudioProcessor::createLayout()` | Creates the layout. |
| `prepareToPlay` |  | `plugin/juce/PluginProcessor.cpp:446` | `void TypePAlphaAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)` | Resets the engine, sizes the rate bridge and buffers, and syncs the parameters. |
| `releaseResources` |  | `plugin/juce/PluginProcessor.cpp:478` | `void TypePAlphaAudioProcessor::releaseResources()` | Releases every engine voice. |
| `isBusesLayoutSupported` |  | `plugin/juce/PluginProcessor.cpp:483` | `bool TypePAlphaAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const` | Accepts a disabled input and a mono or stereo output. |
| `syncParams` |  | `plugin/juce/PluginProcessor.cpp:492` | `void TypePAlphaAudioProcessor::syncParams()` | Writes the changed APVTS values to the engine. |
| `syncMacros` |  | `plugin/juce/PluginProcessor.cpp:513` | `void TypePAlphaAudioProcessor::syncMacros()` | Resolves the six macros to destination offsets and writes the effective values. |
| `feedMessage` |  | `plugin/juce/PluginProcessor.cpp:619` | `void TypePAlphaAudioProcessor::feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message)` | Feeds the message. |
| `renderCore` |  | `plugin/juce/PluginProcessor.cpp:626` | `void TypePAlphaAudioProcessor::renderCore(int32_t* out, int frames)` | Renders one core chunk through the engine interface. |
| `pushTapScopes` |  | `plugin/juce/PluginProcessor.cpp:631` | `void TypePAlphaAudioProcessor::pushTapScopes(p1::Engine& engine, int frames)` | Copies the ten engine tap buffers into the scope rings. |
| `publishMorph` |  | `plugin/juce/PluginProcessor.cpp:646` | `void TypePAlphaAudioProcessor::publishMorph(p1::Engine& engine)` | Publishes the eight live spectral morphs. |
| `publishPhaseMaps` |  | `plugin/juce/PluginProcessor.cpp:654` | `void TypePAlphaAudioProcessor::publishPhaseMaps(p1::Engine& engine)` | Publishes the eight phase-map snapshots through the three-buffer handoff. |
| `feedScheduled` |  | `plugin/juce/PluginProcessor.cpp:672` | `void TypePAlphaAudioProcessor::feedScheduled(int64_t limit)` | Feeds the scheduled MIDI events whose core frame is before the limit. |
| `flushDeferred` |  | `plugin/juce/PluginProcessor.cpp:697` | `void TypePAlphaAudioProcessor::flushDeferred()` | Feeds the note-offs that waited for the chunk to render. |
| `coreChunk` |  | `plugin/juce/PluginProcessor.cpp:704` | `void TypePAlphaAudioProcessor::coreChunk(void* context, p1::Engine& engine, int frames)` | Runs the per-core-chunk MIDI, tap and snapshot work. |
| `processBlock` |  | `plugin/juce/PluginProcessor.cpp:717` | `void TypePAlphaAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)` | Runs the host block through the 48 kHz core path or the rate bridge. |
| `phaseMap` |  | `plugin/juce/PluginProcessor.cpp:884` | `bool TypePAlphaAudioProcessor::phaseMap(int op, int part, p1::PhaseMapSnapshot& out) const` | Reads the current phase-map snapshot for the editor. |
| `specMorph` |  | `plugin/juce/PluginProcessor.cpp:899` | `double TypePAlphaAudioProcessor::specMorph(int op, int part) const` | Reads the live spectral morph for the editor. |
| `libraryFile` |  | `plugin/juce/PluginProcessor.cpp:909` | `juce::File TypePAlphaAudioProcessor::libraryFile() const` | Returns the shared library path. |
| `resetPhaseMaps` |  | `plugin/juce/PluginProcessor.cpp:920` | `void TypePAlphaAudioProcessor::resetPhaseMaps()` | Clears the phase-map, LFO and envelope pools. |
| `ensureLibraryLoaded` |  | `plugin/juce/PluginProcessor.cpp:925` | `void TypePAlphaAudioProcessor::ensureLibraryLoaded()` | Loads the shared library once. |
| `saveLibrary` |  | `plugin/juce/PluginProcessor.cpp:947` | `LibrarySaveResult TypePAlphaAudioProcessor::saveLibrary()` | Runs the locked read-merge-write of the shared library. |
| `~LockRelease` | LockRelease | `plugin/juce/PluginProcessor.cpp:969` | `~LockRelease()` | Destroys the object. |
| `createEditor` |  | `plugin/juce/PluginProcessor.cpp:1024` | `juce::AudioProcessorEditor* TypePAlphaAudioProcessor::createEditor()` | Creates the plugin editor. |
| `getStateInformation` |  | `plugin/juce/PluginProcessor.cpp:1030` | `void TypePAlphaAudioProcessor::getStateInformation(juce::MemoryBlock& destData)` | Serialises the APVTS, names, UI state, routing and library snapshot. |
| `setStateInformation` |  | `plugin/juce/PluginProcessor.cpp:1063` | `void TypePAlphaAudioProcessor::setStateInformation(const void* data, int sizeInBytes)` | Restores the project state and migrates older versions. |
| `createPluginFilter` |  | `plugin/juce/PluginProcessor.cpp:1157` | `juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()` | Creates the plugin filter. |
| `push` | ScopeRing | `plugin/juce/PluginProcessor.h:25` | `void push(const float* data, int count)` | Runs the push step for the surrounding code. |
| `read` | ScopeRing | `plugin/juce/PluginProcessor.h:26` | `void read(float* destination, int count)` | Runs the read step for the surrounding code. |
| `~TypePAlphaAudioProcessor` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:52` | `~TypePAlphaAudioProcessor() override = default` | Destroys the object. |
| `prepareToPlay` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:54` | `void prepareToPlay(double sampleRate, int samplesPerBlock) override` | Resets the engine, sizes the rate bridge and buffers, and syncs the parameters. |
| `releaseResources` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:55` | `void releaseResources() override` | Releases every engine voice. |
| `isBusesLayoutSupported` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:56` | `bool isBusesLayoutSupported(const BusesLayout& layouts) const override` | Accepts a disabled input and a mono or stereo output. |
| `processBlock` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:58` | `void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override` | Runs the host block through the 48 kHz core path or the rate bridge. |
| `createEditor` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:60` | `juce::AudioProcessorEditor* createEditor() override` | Creates the plugin editor. |
| `hasEditor` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:61` | `bool hasEditor() const override` | Tests whether the the editor. |
| `getName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:63` | `const juce::String getName() const override` | Reads the name. |
| `acceptsMidi` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:64` | `bool acceptsMidi() const override` | Tests the accepts MIDI condition. |
| `producesMidi` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:65` | `bool producesMidi() const override` | Tests the produces MIDI condition. |
| `isMidiEffect` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:66` | `bool isMidiEffect() const override` | Tests whether the the MIDI effect. |
| `getTailLengthSeconds` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:67` | `double getTailLengthSeconds() const override` | Reads the tail length seconds. |
| `getNumPrograms` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:69` | `int getNumPrograms() override` | Reads the num programs. |
| `getCurrentProgram` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:70` | `int getCurrentProgram() override` | Reads the current program. |
| `setCurrentProgram` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:71` | `void setCurrentProgram(int) override` | Writes the current program. |
| `getProgramName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:72` | `const juce::String getProgramName(int) override` | Reads the program name. |
| `changeProgramName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:73` | `void changeProgramName(int, const juce::String&) override` | Runs the change program name step for the surrounding code. |
| `getStateInformation` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:75` | `void getStateInformation(juce::MemoryBlock& destData) override` | Serialises the APVTS, names, UI state, routing and library snapshot. |
| `setStateInformation` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:76` | `void setStateInformation(const void* data, int sizeInBytes) override` | Restores the project state and migrates older versions. |
| `ensureLibraryLoaded` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:78` | `void ensureLibraryLoaded()` | Loads the shared library once. |
| `resetPhaseMaps` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:79` | `void resetPhaseMaps()` | Clears the phase-map, LFO and envelope pools. |
| `saveLibrary` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:80` | `LibrarySaveResult saveLibrary()` | Runs the locked read-merge-write of the shared library. |
| `libraryFile` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:81` | `juce::File libraryFile() const` | Returns the shared library path. |
| `setLibraryFileForTesting` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:82` | `void setLibraryFileForTesting(const juce::File& file)` | Writes the library file for testing. |
| `parameters` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:87` | `juce::AudioProcessorValueTreeState& parameters()` | Returns the parameters value. |
| `presets` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:88` | `p1::PresetStore& presets()` | Returns the presets value. |
| `presets` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:89` | `const p1::PresetStore& presets() const` | Returns the presets value. |
| `readOutputScope` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:91` | `void readOutputScope(float* destination, int count)` | Reads the output scope. |
| `readTapScope` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:92` | `void readTapScope(int channel, float* destination, int count)` | Reads the tap scope. |
| `specMorph` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:95` | `double specMorph(int op, int part) const` | The live morph for an operator and part, 0 to 1, or -1 when the operator has not read the bank since the reset. |
| `phaseMap` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:96` | `bool phaseMap(int op, int part, p1::PhaseMapSnapshot& out) const` | Reads the current phase-map snapshot for the editor. |
| `getPatchName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:98` | `juce::String getPatchName() const` | Reads the patch name. |
| `setPatchName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:99` | `void setPatchName(const juce::String& name)` | Writes the patch name. |
| `getPartName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:100` | `juce::String getPartName(int part) const` | Reads the part name. |
| `setPartName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:101` | `void setPartName(int part, const juce::String& name)` | Writes the part name. |
| `getMacroName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:102` | `juce::String getMacroName(int index) const` | Reads the macro name. |
| `setMacroName` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:103` | `void setMacroName(int index, const juce::String& name)` | Writes the macro name. |
| `macroEffectiveValue` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:104` | `double macroEffectiveValue(int index) const` | Returns the macro effective value value. |
| `macroFeedback` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:113` | `MacroFeedback macroFeedback(int index) const` | Returns the macro feedback value. |
| `getUiActivePart` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:114` | `int getUiActivePart() const` | Reads the UI active part. |
| `setUiActivePart` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:115` | `void setUiActivePart(int part)` | Writes the UI active part. |
| `getUiPage` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:116` | `int getUiPage() const` | Reads the UI page. |
| `setUiPage` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:117` | `void setUiPage(int page)` | Writes the UI page. |
| `createLayout` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:123` | `static juce::AudioProcessorValueTreeState::ParameterLayout createLayout()` | Creates the layout. |
| `feedMessage` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:124` | `static void feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message)` | Feeds the message. |
| `syncParams` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:125` | `void syncParams()` | Writes the changed APVTS values to the engine. |
| `syncMacros` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:126` | `void syncMacros()` | Resolves the six macros to destination offsets and writes the effective values. |
| `renderCore` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:127` | `void renderCore(int32_t* out, int frames)` | Renders one core chunk through the engine interface. |
| `pushTapScopes` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:128` | `void pushTapScopes(p1::Engine& engine, int frames)` | Copies the ten engine tap buffers into the scope rings. |
| `publishMorph` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:129` | `void publishMorph(p1::Engine& engine)` | Publishes the eight live spectral morphs. |
| `publishPhaseMaps` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:130` | `void publishPhaseMaps(p1::Engine& engine)` | Publishes the eight phase-map snapshots through the three-buffer handoff. |
| `feedScheduled` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:131` | `void feedScheduled(int64_t limit)` | Feeds the scheduled MIDI events whose core frame is before the limit. |
| `flushDeferred` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:132` | `void flushDeferred()` | Feeds the note-offs that waited for the chunk to render. |
| `coreChunk` | TypePAlphaAudioProcessor | `plugin/juce/PluginProcessor.h:133` | `static void coreChunk(void* context, p1::Engine& engine, int frames)` | Runs the per-core-chunk MIDI, tap and snapshot work. |
| `volumeDbFromPosition` |  | `plugin/juce/VolumeLaw.h:23` | `inline double volumeDbFromPosition(int position)` | Returns the volume dB from position value. |
| `volumeGainFromPosition` |  | `plugin/juce/VolumeLaw.h:38` | `inline float volumeGainFromPosition(int position)` | Returns the volume gain from position value. |
| `volumePositionFromDb` |  | `plugin/juce/VolumeLaw.h:46` | `inline int volumePositionFromDb(double db)` | Returns the volume position from dB value. |
| `cutoffHzFromPosition` |  | `plugin/preset/CutoffLaw.h:20` | `inline double cutoffHzFromPosition(int32_t position)` | Returns the cutoff Hz from position value. |
| `cutoffPositionFromHz` |  | `plugin/preset/CutoffLaw.h:27` | `inline int32_t cutoffPositionFromHz(double hz)` | Returns the cutoff position from Hz value. |
| `cutoffOldToNew` |  | `plugin/preset/CutoffLaw.h:40` | `inline int32_t cutoffOldToNew(int32_t oldValue)` | The old cutoff was a coefficient in 0..55296, scaled by 60211/65536 to keep the 44.1 kHz cutoff in hertz. Map the old coefficient to the frequency it produced and then to the new log position, so a saved patch keeps its cutoff. |
| `remapPanBlock` |  | `plugin/preset/PresetStore.cpp:49` | `void remapPanBlock(std::vector<int32_t>& params, int base)` | The version 11 pan envelope block: on, r1, r2, r3, r4, l1, l2, l3, l4, depth. The version 12 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode. The release rate and level come from the old r4 and l4, and the depth becomes the mode: a non-zero depth was a live envelope, which maps to sustain on. |
| `remapOffsetBlock` |  | `plugin/preset/PresetStore.cpp:74` | `void remapOffsetBlock(const std::vector<int32_t>& in, int oldBase, std::vector<int32_t>& out, int newBase)` | The version 12 offset envelope block: on, r1, r2, r3, r4, l1, l2, l3, l4. The version 13 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode. The old r4 and l4 are the release rate and level. The old envelope always sustained at l3, so the mode is sustain on. |
| `remapAbsSpecBlock` |  | `plugin/preset/PresetStore.cpp:99` | `void remapAbsSpecBlock(const std::vector<int32_t>& in, int oldBase, int depthIndex, std::vector<int32_t>& out, int newBase)` | The version 13 spectral envelope block: r1, r2, r3, r4, l1, l2, l3 at the block, and `depth` far before it. The version 14 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode. A non-zero depth meant a live envelope, so the mode is sustain on, r4 becomes the release rate and r4 is dropped. |
| `remapAbsFilterBlock` |  | `plugin/preset/PresetStore.cpp:122` | `void remapAbsFilterBlock(const std::vector<int32_t>& in, int oldBase, std::vector<int32_t>& out, int newBase)` | The version 13 filter envelope block: depth, r1, r2, r3, r4, l1, l2, l3. The depth sits before the block, so the new block starts where it was. |
| `remapWaveform19` |  | `plugin/preset/PresetStore.cpp:145` | `int32_t remapWaveform19(int32_t value)` | Version 19 inserts the half square at waveform 11, so the old square, saw and spectrum move up one. Values below 11 keep their meaning. |
| `remapAlgorithm19` |  | `plugin/preset/PresetStore.cpp:152` | `int32_t remapAlgorithm19(int32_t value)` | Version 19 inserts one algorithm after 4 and another after 6, so the old algorithms 5, 6 and 7 become 6, 7 and 9. |
| `remapInsertionValues` |  | `plugin/preset/PresetStore.cpp:166` | `void remapInsertionValues(std::vector<int32_t>& params, uint8_t version, int algorithmIndex, const std::array<int, 4>& waveIndices)` | Shift the algorithm and waveform values of a version 18 (or earlier) record. The record length is unchanged, so this runs on both the grow path and the same-size load path. |
| `PresetStore` |  | `plugin/preset/PresetStore.cpp:185` | `PresetStore::PresetStore(int partParamCount, int partCount, std::vector<int32_t> defaults, int cutoffIndex) : cutoffIndex_(cutoffIndex >= 0 && cutoffIndex < partParamCount ? cut...` | Returns the preset store value. |
| `partRecordSize` |  | `plugin/preset/PresetStore.cpp:202` | `std::size_t PresetStore::partRecordSize() const` | Returns the part record size value. |
| `multiRecordSize` |  | `plugin/preset/PresetStore.cpp:207` | `std::size_t PresetStore::multiRecordSize() const` | Returns the multi record size value. |
| `savePart` |  | `plugin/preset/PresetStore.cpp:212` | `bool PresetStore::savePart(int slot, const std::string& name, const std::vector<int32_t>& params)` | Stores one part record in the in-memory bank. |
| `loadPart` |  | `plugin/preset/PresetStore.cpp:229` | `bool PresetStore::loadPart(int slot, std::string& name, std::vector<int32_t>& params) const` | Reads one part record from the in-memory bank. |
| `partUsed` |  | `plugin/preset/PresetStore.cpp:240` | `bool PresetStore::partUsed(int slot) const` | Tests the part used condition. |
| `partName` |  | `plugin/preset/PresetStore.cpp:246` | `std::string PresetStore::partName(int slot) const` | Returns the part name value. |
| `partParams` |  | `plugin/preset/PresetStore.cpp:253` | `const std::vector<int32_t>& PresetStore::partParams(int slot) const` | Returns the part params value. |
| `saveMulti` |  | `plugin/preset/PresetStore.cpp:261` | `bool PresetStore::saveMulti(int slot, const std::string& name, const std::vector<uint8_t>& partSlot, const std::vector<int32_t>& level, const std::vector<int32_t>& delay, const ...` | Saves the multi. |
| `loadMulti` |  | `plugin/preset/PresetStore.cpp:295` | `bool PresetStore::loadMulti(int slot, Multi& out) const` | Loads the multi. |
| `multiUsed` |  | `plugin/preset/PresetStore.cpp:303` | `bool PresetStore::multiUsed(int slot) const` | Tests the multi used condition. |
| `multiName` |  | `plugin/preset/PresetStore.cpp:309` | `std::string PresetStore::multiName(int slot) const` | Returns the multi name value. |
| `packPartBank` |  | `plugin/preset/PresetStore.cpp:316` | `std::vector<uint8_t> PresetStore::packPartBank() const` | Packs all 128 part records. |
| `loadPartBank` |  | `plugin/preset/PresetStore.cpp:331` | `bool PresetStore::loadPartBank(const uint8_t* data, std::size_t size)` | Loads and migrates a part bank. |
| `packMultiBank` |  | `plugin/preset/PresetStore.cpp:385` | `std::vector<uint8_t> PresetStore::packMultiBank() const` | Packs all 64 multi records. |
| `loadMultiBank` |  | `plugin/preset/PresetStore.cpp:410` | `bool PresetStore::loadMultiBank(const uint8_t* data, std::size_t size)` | Loads a multi bank, including the old macro-less size. |
| `packLibrary` |  | `plugin/preset/PresetStore.cpp:470` | `std::vector<uint8_t> PresetStore::packLibrary() const` | Packs the library header, part bank and multi bank. |
| `loadLibrary` |  | `plugin/preset/PresetStore.cpp:492` | `bool PresetStore::loadLibrary(const uint8_t* data, std::size_t size)` | Loads a library file and migrates its part records. |
| `toVersion14Index` |  | `plugin/preset/PresetStore.cpp:590` | `int PresetStore::toVersion14Index(int targetIndex) const` | The part-block index in the version 14 layout, before the 34 LFO fade slots were inserted. |
| `toVersion13Index` |  | `plugin/preset/PresetStore.cpp:599` | `int PresetStore::toVersion13Index(int targetIndex) const` | Version 13 to 14 removed each spectral depth (134, 325, 516, 707), grew * each spectral block by three slots at `on`, and then grew the filter * block by two at its own base. |
| `toVersion13aIndex` |  | `plugin/preset/PresetStore.cpp:624` | `int PresetStore::toVersion13aIndex(int targetIndex) const` | The version 14 layout before the filter block became absolute. |
| `toVersion12Index` |  | `plugin/preset/PresetStore.cpp:643` | `int PresetStore::toVersion12Index(int targetIndex) const` | The part-block index in the version 12 layout, before the five offset slots and the five absolute blocks were inserted. |
| `migratePartBank` |  | `plugin/preset/PresetStore.cpp:663` | `bool PresetStore::migratePartBank(const uint8_t* data, uint32_t oldCount, std::vector<uint8_t>& out) const` | Expands a part bank through the version 10 to 19 chain. |
| `hasChanges` |  | `plugin/preset/PresetStore.cpp:1025` | `bool PresetStore::hasChanges() const` | Reports whether any local slot is dirty. |
| `mergeInto` |  | `plugin/preset/PresetStore.cpp:1033` | `std::vector<PresetStore::Conflict> PresetStore::mergeInto(PresetStore& disk)` | Merges the local change set into a disk store and reports conflicts. |
| `adoptLibrary` |  | `plugin/preset/PresetStore.cpp:1080` | `void PresetStore::adoptLibrary(const PresetStore& other)` | Takes the merged banks and keeps the routing state. |
| `markSaved` |  | `plugin/preset/PresetStore.cpp:1091` | `void PresetStore::markSaved()` | Clears the local change set. |
| `findFreePartSlot` |  | `plugin/preset/PresetStore.cpp:1097` | `int PresetStore::findFreePartSlot() const` | Finds an unused part slot. |
| `findFreeMultiSlot` |  | `plugin/preset/PresetStore.cpp:1105` | `int PresetStore::findFreeMultiSlot() const` | Finds an unused multi slot. |
| `setCurrentMulti` |  | `plugin/preset/PresetStore.cpp:1113` | `void PresetStore::setCurrentMulti(int slot)` | Sets the current multi-patch slot. |
| `partSource` |  | `plugin/preset/PresetStore.cpp:1118` | `int PresetStore::partSource(int part) const` | Returns the part source value. |
| `setPartSource` |  | `plugin/preset/PresetStore.cpp:1125` | `void PresetStore::setPartSource(int part, int slot)` | Sets the library part source for a part. |
| `shortName` |  | `plugin/preset/PresetStore.cpp:1133` | `std::string PresetStore::shortName(const std::string& name)` | Returns the short name value. |
| `partParamCount` | PresetStore | `plugin/preset/PresetStore.hpp:47` | `int partParamCount() const` | Returns the part param count value. |
| `partCount` | PresetStore | `plugin/preset/PresetStore.hpp:48` | `int partCount() const` | Returns the part count value. |
| `partRecordSize` | PresetStore | `plugin/preset/PresetStore.hpp:49` | `std::size_t partRecordSize() const` | Returns the part record size value. |
| `multiRecordSize` | PresetStore | `plugin/preset/PresetStore.hpp:50` | `std::size_t multiRecordSize() const` | Returns the multi record size value. |
| `savePart` | PresetStore | `plugin/preset/PresetStore.hpp:52` | `bool savePart(int slot, const std::string& name, const std::vector<int32_t>& params)` | Stores one part record in the in-memory bank. |
| `loadPart` | PresetStore | `plugin/preset/PresetStore.hpp:54` | `bool loadPart(int slot, std::string& name, std::vector<int32_t>& params) const` | Reads one part record from the in-memory bank. |
| `partUsed` | PresetStore | `plugin/preset/PresetStore.hpp:55` | `bool partUsed(int slot) const` | Tests the part used condition. |
| `partName` | PresetStore | `plugin/preset/PresetStore.hpp:56` | `std::string partName(int slot) const` | Returns the part name value. |
| `partParams` | PresetStore | `plugin/preset/PresetStore.hpp:57` | `const std::vector<int32_t>& partParams(int slot) const` | Returns the part params value. |
| `saveMulti` | PresetStore | `plugin/preset/PresetStore.hpp:59` | `bool saveMulti(int slot, const std::string& name, const std::vector<uint8_t>& partSlot, const std::vector<int32_t>& level, const std::vector<int32_t>& delay, const std::vector<i...` | Saves the multi. |
| `loadMulti` | PresetStore | `plugin/preset/PresetStore.hpp:68` | `bool loadMulti(int slot, Multi& out) const` | Loads the multi. |
| `multiUsed` | PresetStore | `plugin/preset/PresetStore.hpp:69` | `bool multiUsed(int slot) const` | Tests the multi used condition. |
| `multiName` | PresetStore | `plugin/preset/PresetStore.hpp:70` | `std::string multiName(int slot) const` | Returns the multi name value. |
| `packPartBank` | PresetStore | `plugin/preset/PresetStore.hpp:72` | `std::vector<uint8_t> packPartBank() const` | Packs all 128 part records. |
| `loadPartBank` | PresetStore | `plugin/preset/PresetStore.hpp:73` | `bool loadPartBank(const uint8_t* data, std::size_t size)` | Loads and migrates a part bank. |
| `packMultiBank` | PresetStore | `plugin/preset/PresetStore.hpp:74` | `std::vector<uint8_t> packMultiBank() const` | Packs all 64 multi records. |
| `loadMultiBank` | PresetStore | `plugin/preset/PresetStore.hpp:75` | `bool loadMultiBank(const uint8_t* data, std::size_t size)` | Loads a multi bank, including the old macro-less size. |
| `packLibrary` | PresetStore | `plugin/preset/PresetStore.hpp:77` | `std::vector<uint8_t> packLibrary() const` | Packs the library header, part bank and multi bank. |
| `loadLibrary` | PresetStore | `plugin/preset/PresetStore.hpp:78` | `bool loadLibrary(const uint8_t* data, std::size_t size)` | Loads a library file and migrates its part records. |
| `revision` | PresetStore | `plugin/preset/PresetStore.hpp:82` | `uint32_t revision() const` | The library revision written into the file header. A v2 file has no revision field, so it loads as revision 0. |
| `setRevision` | PresetStore | `plugin/preset/PresetStore.hpp:83` | `void setRevision(uint32_t revision)` | Writes the revision. |
| `mergeInto` | PresetStore | `plugin/preset/PresetStore.hpp:98` | `std::vector<Conflict> mergeInto(PresetStore& disk)` | Copy the slots this instance changed into `disk`, which must be a fresh store loaded from the current file. A slot the other side changed comes back as a conflict and is left out, so the disk version stays. The local store keeps its value and its queued change is cleared, so the edit can be saved to another slot. |
| `adoptLibrary` | PresetStore | `plugin/preset/PresetStore.hpp:101` | `void adoptLibrary(const PresetStore& other)` | Take the merged banks back, keeping the routing state the user chose. |
| `setOperatorEnvelopeIndices` | PresetStore | `plugin/preset/PresetStore.hpp:107` | `void setOperatorEnvelopeIndices(const std::array<int, 4>& indices)` | The part-block indices of the four operator warm-up rates, used to remap a version 10 record where the operator envelope grew by one parameter. -1 in any slot means the caller has no layout, so the older append-only migration runs instead. |
| `setPanEnvelopeIndex` | PresetStore | `plugin/preset/PresetStore.hpp:115` | `void setPanEnvelopeIndex(int index)` | The part-block index of `pan_env_on`, used to remap a version 11 record where the pan envelope dropped the depth and gained a release level and a mode. -1 means no layout was supplied. |
| `setOffsetEnvelopeIndices` | PresetStore | `plugin/preset/PresetStore.hpp:121` | `void setOffsetEnvelopeIndices(const std::array<int, 4>& feedback, int pitch)` | The part-block indices of each operator feedback envelope's `on` param and of `penv_on`, used to remap a version 12 record where the pitch and feedback envelopes gained a mode and swapped the fourth level for a release level. -1 in any slot means no layout was supplied. |
| `setAbsEnvelopeIndices` | PresetStore | `plugin/preset/PresetStore.hpp:132` | `void setAbsEnvelopeIndices(int filter, const std::array<int, 4>& spectral)` | The part-block index of `fenv_on` and of each operator's `spec_env_on`, used to remap a version 13 record where the filter and spectral envelopes became absolute and gained on, release and mode controls. -1 in any slot means no layout was supplied. |
| `setLfoFadeIndices` | PresetStore | `plugin/preset/PresetStore.hpp:141` | `void setLfoFadeIndices(const std::array<int, 34>& indices)` | The part-block indices of the 34 LFO fade parameters, used to remap a version 14 record to the version 15 layout. -1 in any slot means the caller has no layout, so the older append-only migration runs instead. |
| `setInsertionIndices` | PresetStore | `plugin/preset/PresetStore.hpp:151` | `void setInsertionIndices(int algorithm, const std::array<int, 4>& waves)` | The part-block index of `algorithm` and of each operator's `waveform`, used to remap a version 18 record: version 19 inserts the half square after pulse and two algorithms after 4 and 6. -1 means the caller has no layout, so the values stay as they are. |
| `markSaved` | PresetStore | `plugin/preset/PresetStore.hpp:158` | `void markSaved()` | Drop the queued changes after the file holds them. |
| `hasChanges` | PresetStore | `plugin/preset/PresetStore.hpp:160` | `bool hasChanges() const` | Reports whether any local slot is dirty. |
| `findFreePartSlot` | PresetStore | `plugin/preset/PresetStore.hpp:162` | `int findFreePartSlot() const` | Finds an unused part slot. |
| `findFreeMultiSlot` | PresetStore | `plugin/preset/PresetStore.hpp:163` | `int findFreeMultiSlot() const` | Finds an unused multi slot. |
| `currentMulti` | PresetStore | `plugin/preset/PresetStore.hpp:165` | `int currentMulti() const` | Returns the current multi value. |
| `setCurrentMulti` | PresetStore | `plugin/preset/PresetStore.hpp:166` | `void setCurrentMulti(int slot)` | Sets the current multi-patch slot. |
| `partSource` | PresetStore | `plugin/preset/PresetStore.hpp:167` | `int partSource(int part) const` | Returns the part source value. |
| `setPartSource` | PresetStore | `plugin/preset/PresetStore.hpp:168` | `void setPartSource(int part, int slot)` | Sets the library part source for a part. |
| `shortName` | PresetStore | `plugin/preset/PresetStore.hpp:171` | `static std::string shortName(const std::string& name)` | Returns the short name value. |
| `toVersion12Index` | PresetStore | `plugin/preset/PresetStore.hpp:175` | `int toVersion12Index(int targetIndex) const` | The part-block index in the version 12 layout, before the five offset slots and the five absolute blocks were inserted. |
| `toVersion14Index` | PresetStore | `plugin/preset/PresetStore.hpp:179` | `int toVersion14Index(int targetIndex) const` | The part-block index in the version 14 layout, before the 34 LFO fade slots were inserted. |
| `toVersion13Index` | PresetStore | `plugin/preset/PresetStore.hpp:183` | `int toVersion13Index(int targetIndex) const` | The part-block index in the version 13 layout, before the ten absolute envelope slots were inserted. |
| `toVersion13aIndex` | PresetStore | `plugin/preset/PresetStore.hpp:187` | `int toVersion13aIndex(int targetIndex) const` | The part-block index in the intermediate version 14 layout, after the spectral envelope blocks changed but before the filter block did. |
| `migratePartBank` | PresetStore | `plugin/preset/PresetStore.hpp:193` | `bool migratePartBank(const uint8_t* data, uint32_t oldCount, std::vector<uint8_t>& out) const` | Expand a part bank saved with `oldCount` params per record to the current count. The chain runs version 10 operator blocks, the version 11 pan reorder, the version 12 offset envelope insert, and the version 13 filter and spectral insert. |
| `partParamDefaults` |  | `plugin/preset/part_defaults.h:12` | `inline std::vector<int32_t> partParamDefaults()` | Returns the part param defaults value. |
| `slot_save` |  | `plugin/preset/presets_host.c:8` | `int slot_save(uint32_t i, const void *p, uint32_t n)` | Writes a bank or record through the host store. |
| `slot_load` |  | `plugin/preset/presets_host.c:16` | `int slot_load(uint32_t i, void *p, uint32_t n)` | Reads a bank, record or library file and migrates it. |
| `settings_save` |  | `plugin/preset/presets_host.c:24` | `int settings_save(const void *p, uint32_t n)` | Writes a bank or record through the host store. |
| `settings_load` |  | `plugin/preset/presets_host.c:31` | `int settings_load(void *p, uint32_t n)` | Reads a bank, record or library file and migrates it. |
| `secondsSince` |  | `plugin/tests/additive_bench.cpp:31` | `double secondsSince(std::chrono::steady_clock::time_point start)` | Returns the seconds since value. |
| `clampQ15` |  | `plugin/tests/additive_bench.cpp:37` | `int16_t clampQ15(int32_t value)` | Clamps the q 15. |
| `sineTable` |  | `plugin/tests/additive_bench.cpp:42` | `std::vector<int16_t> sineTable(int size)` | Returns the sine table value. |
| `recipe` |  | `plugin/tests/additive_bench.cpp:53` | `std::vector<int16_t> recipe(int harmonics, double tilt, int formant, double peak)` | Returns the recipe value. |
| `generateTable` |  | `plugin/tests/additive_bench.cpp:75` | `void generateTable(std::vector<int16_t>& table, int harmonics, int size, const std::vector<int16_t>& amps, const std::vector<int16_t>& sine, int updates, uint64_t& checksum)` | Runs the generate table step for the surrounding code. |
| `baselineSeconds` |  | `plugin/tests/additive_bench.cpp:95` | `double baselineSeconds()` | Returns the baseline seconds value. |
| `readModelSeconds` |  | `plugin/tests/additive_bench.cpp:115` | `double readModelSeconds(int tableSize)` | Reads the model seconds capture. |
| `readTable` |  | `plugin/tests/additive_bench.cpp:154` | `double readTable(const std::vector<int16_t>& table, double phase)` | Reads the table capture. |
| `writeWav` |  | `plugin/tests/additive_bench.cpp:163` | `void writeWav(const std::string& path, const std::vector<int16_t>& left, const std::vector<int16_t>& right)` | Writes the WAV capture. |
| `audition` |  | `plugin/tests/additive_bench.cpp:200` | `void audition(const std::string& path)` | Runs the audition step for the surrounding code. |
| `mipLevels` |  | `plugin/tests/additive_bench.cpp:251` | `int mipLevels(int harmonics)` | Returns the mip levels value. |
| `mipFor` |  | `plugin/tests/additive_bench.cpp:259` | `int mipFor(double fundamental, int harmonics, int levels)` | Returns the mip for value. |
| `buildWaveTable` |  | `plugin/tests/additive_bench.cpp:267` | `WaveTable buildWaveTable(int harmonics, int size, const std::vector<int16_t>& amps)` | Builds the wave table. |
| `readWave` |  | `plugin/tests/additive_bench.cpp:286` | `double readWave(const WaveTable& table, int mip, double phase, bool linear)` | Reads the wave capture. |
| `magnitude` |  | `plugin/tests/additive_bench.cpp:300` | `double magnitude(const std::vector<double>& samples, double frequency)` | Returns the magnitude value. |
| `aliasFloor` |  | `plugin/tests/additive_bench.cpp:316` | `double aliasFloor(const WaveTable& table, int mip, bool linear, double fundamental, int firstAlias, int harmonics)` | Returns the alias floor value. |
| `aliasQuality` |  | `plugin/tests/additive_bench.cpp:347` | `void aliasQuality()` | Runs the alias quality step for the surrounding code. |
| `mipForBright` |  | `plugin/tests/additive_bench.cpp:374` | `int mipForBright(double fundamental, int harmonics, int levels)` | Returns the mip for bright value. |
| `harmonicEnergy` |  | `plugin/tests/additive_bench.cpp:382` | `double harmonicEnergy(const WaveTable& table, int mip, bool linear, double fundamental, int first, int last)` | Returns the harmonic energy value. |
| `tableError` |  | `plugin/tests/additive_bench.cpp:408` | `double tableError(const WaveTable& table, int mip, bool linear, double fundamental, const std::vector<int16_t>& amps, int limit)` | Returns the table error value. |
| `qualityExtras` |  | `plugin/tests/additive_bench.cpp:443` | `void qualityExtras()` | Runs the quality extras step for the surrounding code. |
| `oscillatorSeconds` |  | `plugin/tests/additive_bench.cpp:479` | `double oscillatorSeconds(bool additive)` | Returns the oscillator seconds value. |
| `auditionOscillator` |  | `plugin/tests/additive_bench.cpp:534` | `void auditionOscillator(const std::string& path)` | Runs the audition oscillator step for the surrounding code. |
| `main` |  | `plugin/tests/additive_bench.cpp:595` | `int main(int argc, char** argv)` | Runs the test or tool entry point. |
| `secondsSince` |  | `plugin/tests/additive_engine_bench.cpp:28` | `double secondsSince(std::chrono::steady_clock::time_point start)` | Returns the seconds since value. |
| `setEngineParam` |  | `plugin/tests/additive_engine_bench.cpp:34` | `bool setEngineParam(p1::Engine& engine, const char* id, int32_t value)` | Writes the engine param. |
| `writeWav` |  | `plugin/tests/additive_engine_bench.cpp:45` | `void writeWav(const std::string& path, const std::vector<int32_t>& samples)` | Writes the WAV capture. |
| `main` |  | `plugin/tests/additive_engine_bench.cpp:89` | `int main(int argc, char** argv)` | Runs the test or tool entry point. |
| `saveSnapshot` |  | `plugin/tests/editor_snapshot.cpp:16` | `bool saveSnapshot(juce::AudioProcessorEditor& editor, const juce::File& file)` | Saves the snapshot. |
| `main` |  | `plugin/tests/editor_snapshot.cpp:32` | `int main(int argc, char** argv)` | Runs the test or tool entry point. |
| `midi_get` |  | `plugin/tests/engine_parity.cpp:18` | `int midi_get(uint8_t*, uint8_t*, uint8_t*)` | Returns the MIDI get value. |
| `check` |  | `plugin/tests/engine_parity.cpp:27` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `same` |  | `plugin/tests/engine_parity.cpp:34` | `bool same(const int32_t* a, const int32_t* b, std::size_t n)` | Compares a rendered result with its reference. |
| `param_id` |  | `plugin/tests/engine_parity.cpp:39` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `test_engine_render` |  | `plugin/tests/engine_parity.cpp:48` | `void test_engine_render()` | Runs the `test_engine_render` case. |
| `test_state_isolation` |  | `plugin/tests/engine_parity.cpp:65` | `void test_state_isolation()` | Runs the `test_state_isolation` case. |
| `test_filter_is_per_part` |  | `plugin/tests/engine_parity.cpp:93` | `void test_filter_is_per_part()` | Runs the `test_filter_is_per_part` case. |
| `filter_rms` |  | `plugin/tests/engine_parity.cpp:156` | `double filter_rms(int32_t resonance, bool carriers)` | Returns the filter RMS value. |
| `test_filter_output_gate` |  | `plugin/tests/engine_parity.cpp:199` | `void test_filter_output_gate()` | Runs the `test_filter_output_gate` case. |
| `test_sustain_isolation` |  | `plugin/tests/engine_parity.cpp:214` | `void test_sustain_isolation()` | Runs the `test_sustain_isolation` case. |
| `test_channel_separation` |  | `plugin/tests/engine_parity.cpp:237` | `void test_channel_separation()` | Runs the `test_channel_separation` case. |
| `test_sustain_per_channel` |  | `plugin/tests/engine_parity.cpp:260` | `void test_sustain_per_channel()` | Runs the `test_sustain_per_channel` case. |
| `test_all_notes_off_per_channel` |  | `plugin/tests/engine_parity.cpp:283` | `void test_all_notes_off_per_channel()` | Runs the `test_all_notes_off_per_channel` case. |
| `test_bend_per_part` |  | `plugin/tests/engine_parity.cpp:309` | `void test_bend_per_part()` | Runs the `test_bend_per_part` case. |
| `test_pan_per_part` |  | `plugin/tests/engine_parity.cpp:336` | `void test_pan_per_part()` | Runs the `test_pan_per_part` case. |
| `test_pan_envelope` |  | `plugin/tests/engine_parity.cpp:359` | `void test_pan_envelope()` | Runs the `test_pan_envelope` case. |
| `main` |  | `plugin/tests/engine_parity.cpp:389` | `int main(int argc, char** argv)` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/feedback_bipolar.cpp:26` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/feedback_bipolar.cpp:33` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/feedback_bipolar.cpp:42` | `bool set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `setup` |  | `plugin/tests/feedback_bipolar.cpp:53` | `bool setup(p1::Engine& engine, int32_t feedback, int32_t envLevel)` | Prepares or releases the shared test fixtures. |
| `magnitudeAt` |  | `plugin/tests/feedback_bipolar.cpp:92` | `double magnitudeAt(const std::vector<int32_t>& samples, double frequency, const std::vector<double>& window, double windowSum)` | Returns the magnitude at value. |
| `analyse` |  | `plugin/tests/feedback_bipolar.cpp:108` | `Spectrum analyse(const std::vector<int32_t>& samples)` | Returns the analyse value. |
| `render` |  | `plugin/tests/feedback_bipolar.cpp:145` | `bool render(int32_t feedback, int32_t envLevel, std::vector<int32_t>& samples)` | Tests the render condition. |
| `ratio` |  | `plugin/tests/feedback_bipolar.cpp:157` | `double ratio(const Spectrum& s, int numerator, int denominator)` | Returns the ratio value. |
| `main` |  | `plugin/tests/feedback_bipolar.cpp:167` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/feedback_envelope.cpp:26` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/feedback_envelope.cpp:33` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/feedback_envelope.cpp:42` | `bool set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `setup` |  | `plugin/tests/feedback_envelope.cpp:53` | `bool setup(p1::Engine& engine, int32_t feedback, int32_t envL1, int32_t envR1, int32_t on)` | Prepares or releases the shared test fixtures. |
| `analyseWindow` |  | `plugin/tests/feedback_envelope.cpp:96` | `Window analyseWindow(const std::vector<int32_t>& samples, int start)` | Analyses the window. |
| `peakOf` |  | `plugin/tests/feedback_envelope.cpp:130` | `double peakOf(const std::vector<int32_t>& samples)` | Returns the peak of value. |
| `render` |  | `plugin/tests/feedback_envelope.cpp:143` | `bool render(int32_t feedback, int32_t envL1, int32_t envR1, int32_t on, std::vector<int32_t>& samples, bool* negative = nullptr)` | Tests the render condition. |
| `main` |  | `plugin/tests/feedback_envelope.cpp:160` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/filter_envelope.cpp:20` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/filter_envelope.cpp:27` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/filter_envelope.cpp:36` | `void set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `setup` |  | `plugin/tests/filter_envelope.cpp:46` | `void setup(p1::Engine& engine, int32_t r1)` | Prepares or releases the shared test fixtures. |
| `run` |  | `plugin/tests/filter_envelope.cpp:73` | `int32_t run(int32_t r1, int blocks)` | Runs one measured case and returns its result. |
| `main` |  | `plugin/tests/filter_envelope.cpp:86` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/filter_law.cpp:14` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `res_k` |  | `plugin/tests/filter_law.cpp:21` | `int32_t res_k(int32_t res)` | Returns the res k value. |
| `main` |  | `plugin/tests/filter_law.cpp:29` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/lfo_fades.cpp:31` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/lfo_fades.cpp:38` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/lfo_fades.cpp:47` | `bool set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `base_setup` |  | `plugin/tests/lfo_fades.cpp:58` | `bool base_setup(p1::Engine& engine)` | Tests the base setup condition. |
| `trem_setup` |  | `plugin/tests/lfo_fades.cpp:88` | `bool trem_setup(p1::Engine& engine, int32_t depth, int32_t rateFade, int32_t depthFade)` | Tests the trem setup condition. |
| `voice_of` |  | `plugin/tests/lfo_fades.cpp:99` | `int voice_of(p1::Engine& engine, uint32_t note)` | Returns the voice of value. |
| `tick` |  | `plugin/tests/lfo_fades.cpp:109` | `void tick(p1::Engine& engine)` | Refreshes the live display state. |
| `render` |  | `plugin/tests/lfo_fades.cpp:115` | `void render(p1::Engine& engine, int frames, std::vector<int32_t>& pcm)` | Runs the render step for the surrounding code. |
| `run_ramp` |  | `plugin/tests/lfo_fades.cpp:128` | `bool run_ramp(int32_t rateFade, int32_t depthFade, Ramp& ramp)` | Tests the run ramp condition. |
| `near` |  | `plugin/tests/lfo_fades.cpp:155` | `bool near(int value, int expected, int slack)` | Tests the near condition. |
| `rms` |  | `plugin/tests/lfo_fades.cpp:160` | `double rms(const std::vector<int32_t>& pcm, int startFrame, int frames)` | Returns the RMS value. |
| `spread` |  | `plugin/tests/lfo_fades.cpp:170` | `double spread(const std::vector<int32_t>& pcm, int startFrame, int endFrame)` | Returns the spread value. |
| `same` |  | `plugin/tests/lfo_fades.cpp:192` | `bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)` | Compares a rendered result with its reference. |
| `all_family_levels` |  | `plugin/tests/lfo_fades.cpp:198` | `bool all_family_levels(p1::Engine& engine, uint32_t& rate, uint32_t& depth)` | Tests the all family levels condition. |
| `main` |  | `plugin/tests/lfo_fades.cpp:223` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/operator_lfos.cpp:17` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/operator_lfos.cpp:24` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/operator_lfos.cpp:33` | `bool set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `setup` |  | `plugin/tests/operator_lfos.cpp:44` | `bool setup(p1::Engine& engine, int32_t feedbackLfoDepth, int32_t detuneLfoDepth, int32_t feedbackLfoRate, int32_t detuneLfoRate)` | Prepares or releases the shared test fixtures. |
| `render` |  | `plugin/tests/operator_lfos.cpp:82` | `bool render(int32_t feedbackLfoDepth, int32_t detuneLfoDepth, int32_t feedbackLfoRate, int32_t detuneLfoRate, std::vector<int32_t>& samples)` | Tests the render condition. |
| `same` |  | `plugin/tests/operator_lfos.cpp:97` | `bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)` | Compares a rendered result with its reference. |
| `main` |  | `plugin/tests/operator_lfos.cpp:105` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/phase_map.cpp:52` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/phase_map.cpp:59` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/phase_map.cpp:68` | `bool set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `base_setup` |  | `plugin/tests/phase_map.cpp:79` | `bool base_setup(p1::Engine& engine)` | Tests the base setup condition. |
| `pm` |  | `plugin/tests/phase_map.cpp:107` | `uint32_t* pm(p1::Engine& engine, int op, int part)` | Returns the pm value. |
| `lfo_pool` |  | `plugin/tests/phase_map.cpp:120` | `uint32_t* lfo_pool(p1::Engine& engine, int op, int part)` | Returns the LFO pool value. |
| `env_pool` |  | `plugin/tests/phase_map.cpp:133` | `uint32_t* env_pool(p1::Engine& engine, int op, int part)` | Returns the env pool value. |
| `map_on` |  | `plugin/tests/phase_map.cpp:146` | `bool map_on(p1::Engine& engine)` | Maps the on. |
| `tick` |  | `plugin/tests/phase_map.cpp:158` | `void tick(p1::Engine& engine)` | Refreshes the live display state. |
| `render` |  | `plugin/tests/phase_map.cpp:164` | `void render(p1::Engine& engine, int frames, std::vector<int32_t>& pcm)` | Runs the render step for the surrounding code. |
| `same` |  | `plugin/tests/phase_map.cpp:170` | `bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)` | Compares a rendered result with its reference. |
| `sorted_x` |  | `plugin/tests/phase_map.cpp:176` | `bool sorted_x(const uint32_t* p, uint32_t active)` | Sorts the ed x. |
| `permutes` |  | `plugin/tests/phase_map.cpp:185` | `bool permutes(const uint32_t* p, uint32_t active)` | Tests the permutes condition. |
| `mask_nonempty` |  | `plugin/tests/phase_map.cpp:199` | `bool mask_nonempty(const uint32_t* p)` | Tests the mask nonempty condition. |
| `locked_flip_free` |  | `plugin/tests/phase_map.cpp:207` | `bool locked_flip_free(const uint32_t* p)` | Tests the locked flip free condition. |
| `wait_event` |  | `plugin/tests/phase_map.cpp:220` | `void wait_event(p1::Engine& engine, uint32_t* p, uint32_t accIndex, int maxTicks, p1::PhaseMapSnapshot* snap = nullptr)` | Runs the wait event step for the surrounding code. |
| `rms` |  | `plugin/tests/phase_map.cpp:234` | `double rms(const std::vector<int32_t>& pcm)` | Returns the RMS value. |
| `main` |  | `plugin/tests/phase_map.cpp:246` | `int main()` | Runs the test or tool entry point. |
| `midiHz` |  | `plugin/tests/phase_map_perceptual.cpp:81` | `double midiHz(int note)` | Returns the MIDI Hz value. |
| `rateQ32` |  | `plugin/tests/phase_map_perceptual.cpp:86` | `uint32_t rateQ32(double hz)` | Returns the rate q 32 value. |
| `paramId` |  | `plugin/tests/phase_map_perceptual.cpp:96` | `uint32_t paramId(const char* name)` | Returns the param id value. |
| `setParam` |  | `plugin/tests/phase_map_perceptual.cpp:105` | `bool setParam(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `setPart` |  | `plugin/tests/phase_map_perceptual.cpp:116` | `bool setPart(p1::Engine& engine, int part, const char* stem, int32_t value)` | Writes the part. |
| `configurePart` |  | `plugin/tests/phase_map_perceptual.cpp:123` | `bool configurePart(p1::Engine& engine, int part, const Scenario& s)` | Tests the configure part condition. |
| `pool` |  | `plugin/tests/phase_map_perceptual.cpp:202` | `uint32_t* pool(p1::Engine& engine, int part)` | Returns the pool value. |
| `mixWord` |  | `plugin/tests/phase_map_perceptual.cpp:208` | `uint64_t mixWord(uint64_t h, uint32_t w)` | Mixes the word. |
| `geometryHash` |  | `plugin/tests/phase_map_perceptual.cpp:219` | `uint64_t geometryHash(const uint32_t* pm)` | The geometry hash excludes the table (which differs by design) and the per-block clocks, but keeps the seed, so a stochastic event shows up. |
| `exactY` |  | `plugin/tests/phase_map_perceptual.cpp:230` | `uint32_t exactY(const uint32_t* pm, uint32_t x)` | Returns the exact y value. |
| `tableY` |  | `plugin/tests/phase_map_perceptual.cpp:251` | `uint32_t tableY(const uint32_t* pm, uint32_t x, int mode)` | Returns the table y value. |
| `transferError` |  | `plugin/tests/phase_map_perceptual.cpp:299` | `Transfer transferError(const uint32_t* pm, int mode)` | Returns the transfer error value. |
| `exactYq` |  | `plugin/tests/phase_map_perceptual.cpp:319` | `uint32_t exactYq(const uint32_t* pm, uint32_t q)` | The post-F map y(q) is piecewise linear. A table in q does not inherit the F compression at extreme breakpoints. This is the audit for that variant. |
| `qTransferError` |  | `plugin/tests/phase_map_perceptual.cpp:339` | `Transfer qTransferError(const uint32_t* pm, uint32_t n, uint32_t step, bool linear)` | Returns the q transfer error value. |
| `writeQAudit` |  | `plugin/tests/phase_map_perceptual.cpp:370` | `void writeQAudit(std::FILE* f, const char* id, int block, int part, const uint32_t* pm)` | Writes the q audit capture. |
| `actualQError` |  | `plugin/tests/phase_map_perceptual.cpp:392` | `Transfer actualQError(const uint32_t* pm, int mode)` | Read the compiled table back and compare it with the exact q map. This catches a rebuild that was skipped, which a model-only audit cannot see. |
| `buildScenarios` |  | `plugin/tests/phase_map_perceptual.cpp:429` | `std::vector<Scenario> buildScenarios()` | Builds the scenarios. |
| `main` |  | `plugin/tests/phase_map_perceptual.cpp:660` | `int main(int argc, char** argv)` | Runs the test or tool entry point. |
| `split` |  | `plugin/tests/phase_map_perceptual_compare.cpp:100` | `std::vector<std::string> split(const std::string& line)` | Returns the split value. |
| `readManifest` |  | `plugin/tests/phase_map_perceptual_compare.cpp:116` | `bool readManifest(const std::string& dir, Manifest& m)` | Reads the manifest capture. |
| `readPcm` |  | `plugin/tests/phase_map_perceptual_compare.cpp:186` | `bool readPcm(const std::string& dir, const std::string& id, std::vector<int32_t>& out, int frames, int channels)` | Reads the PCM capture. |
| `compareLogs` |  | `plugin/tests/phase_map_perceptual_compare.cpp:206` | `bool compareLogs(const std::string& refDir, const std::string& candDir, const char* name)` | Tests the compare logs condition. |
| `Fft` | Fft | `plugin/tests/phase_map_perceptual_compare.cpp:244` | `explicit Fft(int n) : n_(n), rev_(static_cast<std::size_t>(n)), tw_(static_cast<std::size_t>(n))` | Constructs the object. |
| `run` | Fft | `plugin/tests/phase_map_perceptual_compare.cpp:260` | `void run(std::vector<std::complex<double>>& a) const` | Runs one measured case and returns its result. |
| `percentile` |  | `plugin/tests/phase_map_perceptual_compare.cpp:291` | `double percentile(std::vector<double> v, double q)` | Returns the percentile value. |
| `toDb` |  | `plugin/tests/phase_map_perceptual_compare.cpp:301` | `double toDb(double power, double floorPower)` | Returns the to dB value. |
| `bandCenters` |  | `plugin/tests/phase_map_perceptual_compare.cpp:324` | `std::vector<double> bandCenters()` | One-third-octave centers from 25 Hz to 20 kHz. |
| `analyze` |  | `plugin/tests/phase_map_perceptual_compare.cpp:332` | `Analysis analyze(const std::vector<int32_t>& pcm, int frames, int channels, int sr, double f0, bool harmonics)` | Returns the analyze value. |
| `rmsOf` |  | `plugin/tests/phase_map_perceptual_compare.cpp:470` | `double rmsOf(const std::vector<int32_t>& pcm)` | Returns the RMS of value. |
| `peakOf` |  | `plugin/tests/phase_map_perceptual_compare.cpp:478` | `double peakOf(const std::vector<int32_t>& pcm)` | Returns the peak of value. |
| `crossing` |  | `plugin/tests/phase_map_perceptual_compare.cpp:486` | `bool crossing(const std::vector<double>& env, int from, int to, double level, bool rising, double blockMs, double& outMs)` | Tests the crossing condition. |
| `compareScenario` |  | `plugin/tests/phase_map_perceptual_compare.cpp:515` | `Metrics compareScenario(const Scenario& s, const std::vector<int32_t>& ref, const std::vector<int32_t>& cand, int sr, int channels)` | Returns the compare scenario value. |
| `metricFails` |  | `plugin/tests/phase_map_perceptual_compare.cpp:731` | `bool metricFails(const Metrics& m, const Scenario& s, const char*& what, double& value, double& limit)` | Tests the metric fails condition. |
| `gateScore` |  | `plugin/tests/phase_map_perceptual_compare.cpp:772` | `double gateScore(const Metrics& m)` | Returns the gate score value. |
| `printScenario` |  | `plugin/tests/phase_map_perceptual_compare.cpp:791` | `void printScenario(const char* tag, const Scenario& s, const Metrics& m)` | Runs the print scenario step for the surrounding code. |
| `main` |  | `plugin/tests/phase_map_perceptual_compare.cpp:805` | `int main(int argc, char** argv)` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/pitch_envelope.cpp:20` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `param_id` |  | `plugin/tests/pitch_envelope.cpp:27` | `uint32_t param_id(const char* name)` | Returns the param id value. |
| `set_param` |  | `plugin/tests/pitch_envelope.cpp:36` | `bool set_param(p1::Engine& engine, const char* name, int32_t value)` | Writes the param. |
| `setup` |  | `plugin/tests/pitch_envelope.cpp:47` | `bool setup(p1::Engine& engine, int32_t l1, int32_t l2, int32_t l3, int32_t relLevel, int32_t mode, int32_t r1 = 99)` | Prepares or releases the shared test fixtures. |
| `render` |  | `plugin/tests/pitch_envelope.cpp:79` | `void render(p1::Engine& engine, int blocks, int32_t* level)` | Runs the render step for the surrounding code. |
| `main` |  | `plugin/tests/pitch_envelope.cpp:91` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/plugin_smoke.cpp:32` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `bitEqual` |  | `plugin/tests/plugin_smoke.cpp:39` | `bool bitEqual(float a, float b)` | Tests the bit equal condition. |
| `renderReference` |  | `plugin/tests/plugin_smoke.cpp:44` | `void renderReference(int32_t* out, int frames, int noteOffset, int32_t algorithm)` | Renders the reference. |
| `matches` |  | `plugin/tests/plugin_smoke.cpp:58` | `bool matches(const juce::AudioBuffer<float>& buffer, const int32_t* reference, int frames)` | Compares a rendered result with its reference. |
| `main` |  | `plugin/tests/plugin_smoke.cpp:75` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/plugin_stability.cpp:20` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `render` |  | `plugin/tests/plugin_stability.cpp:32` | `Peak render(TypePAlphaAudioProcessor& processor, juce::MidiBuffer& midi, int blocks)` | Returns the render value. |
| `setAll` |  | `plugin/tests/plugin_stability.cpp:51` | `void setAll(TypePAlphaAudioProcessor& processor, bool maximum)` | Writes the all. |
| `main` |  | `plugin/tests/plugin_stability.cpp:65` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/preset_smoke.cpp:25` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `pattern` |  | `plugin/tests/preset_smoke.cpp:32` | `std::vector<int32_t> pattern(int seed)` | Returns the pattern value. |
| `setRecordVersion` |  | `plugin/tests/preset_smoke.cpp:42` | `void setRecordVersion(uint8_t* record, uint32_t count, uint8_t version)` | `preset_part_pack` seals a current-version record. An older record needs the version byte and then a fresh CRC. |
| `lfoFadeIndices` |  | `plugin/tests/preset_smoke.cpp:52` | `std::array<int, 34> lfoFadeIndices()` | Returns the LFO fade indices value. |
| `main` |  | `plugin/tests/preset_smoke.cpp:70` | `int main()` | Runs the test or tool entry point. |
| `amplitude` |  | `plugin/tests/rate_bridge_test.cpp:24` | `double amplitude(const std::vector<float>& samples, int start, int count, double frequency, double sampleRate)` | Returns the amplitude value. |
| `rms` |  | `plugin/tests/rate_bridge_test.cpp:42` | `double rms(const std::vector<float>& samples, int start, int count)` | Returns the RMS value. |
| `run` |  | `plugin/tests/rate_bridge_test.cpp:52` | `Measurement run(double hostRate, double frequency, int hostFrames, int blockSize)` | Runs one measured case and returns its result. |
| `run` |  | `plugin/tests/rate_bridge_test.cpp:93` | `Measurement run(double hostRate, double frequency, int hostFrames)` | Runs one measured case and returns its result. |
| `imageFrequencies` |  | `plugin/tests/rate_bridge_test.cpp:98` | `std::vector<double> imageFrequencies(double hostRate, double frequency)` | Returns the image frequencies value. |
| `check` |  | `plugin/tests/rate_bridge_test.cpp:120` | `bool check(bool condition, const char* what)` | Records a test assertion. |
| `main` |  | `plugin/tests/rate_bridge_test.cpp:128` | `int main()` | Runs the test or tool entry point. |
| `setParam` |  | `plugin/tests/spectral.cpp:24` | `bool setParam(p1::Engine& engine, const char* id, int32_t value)` | Writes the param. |
| `setAll` |  | `plugin/tests/spectral.cpp:36` | `bool setAll(p1::Engine& engine, const std::vector<std::pair<const char*, int32_t>>& values)` | Writes the all. |
| `magnitude` |  | `plugin/tests/spectral.cpp:45` | `double magnitude(const std::vector<int32_t>& samples, int start, int count, double frequency)` | Returns the magnitude value. |
| `setupCarrier` |  | `plugin/tests/spectral.cpp:66` | `bool setupCarrier(p1::Engine& engine)` | Writes the up carrier. |
| `render` |  | `plugin/tests/spectral.cpp:83` | `std::vector<int32_t> render(p1::Engine& engine, int frames = kFrames)` | Returns the render value. |
| `checkMorph` |  | `plugin/tests/spectral.cpp:90` | `bool checkMorph()` | Checks the morph result. |
| `checkMorphEnvelope` |  | `plugin/tests/spectral.cpp:125` | `bool checkMorphEnvelope()` | Checks the morph envelope result. |
| `checkMorphLfo` |  | `plugin/tests/spectral.cpp:206` | `bool checkMorphLfo()` | Checks the morph LFO result. |
| `checkColdCommit` |  | `plugin/tests/spectral.cpp:238` | `bool checkColdCommit()` | Checks the cold commit result. |
| `checkCrossfadeDirection` |  | `plugin/tests/spectral.cpp:270` | `bool checkCrossfadeDirection()` | Checks the crossfade direction result. |
| `checkTable` |  | `plugin/tests/spectral.cpp:312` | `bool checkTable()` | Checks the table result. |
| `checkMip` |  | `plugin/tests/spectral.cpp:336` | `bool checkMip()` | Checks the mip result. |
| `checkRange` |  | `plugin/tests/spectral.cpp:349` | `bool checkRange()` | Checks the range result. |
| `main` |  | `plugin/tests/spectral.cpp:375` | `int main()` | Runs the test or tool entry point. |
| `secondsSince` |  | `plugin/tests/spectral_bench.cpp:24` | `double secondsSince(std::chrono::steady_clock::time_point start)` | Returns the seconds since value. |
| `runTickSeries` |  | `plugin/tests/spectral_bench.cpp:36` | `TickResult runTickSeries(int ticks, bool reads)` | Returns the run tick series value. |
| `setParam` |  | `plugin/tests/spectral_bench.cpp:75` | `bool setParam(p1::Engine& engine, const char* id, int32_t value)` | Writes the param. |
| `setAll` |  | `plugin/tests/spectral_bench.cpp:86` | `void setAll(p1::Engine& engine, const std::vector<std::pair<const char*, int32_t>>& values)` | Writes the all. |
| `setupSpectrumEngine` |  | `plugin/tests/spectral_bench.cpp:94` | `void setupSpectrumEngine(p1::Engine& engine)` | Four operators, both parts, spectrum mode, one clean carrier per part. |
| `runEngineChanges` |  | `plugin/tests/spectral_bench.cpp:119` | `BlockResult runEngineChanges(int blocks, int parts)` | Parts is the number of parts with notes: 0, 1 or 2. |
| `generateWide` |  | `plugin/tests/spectral_bench.cpp:173` | `void generateWide(int16_t *out, const int32_t *levels, int first, int size, int shift)` | A 64-harmonic spectrum with seven mips (64, 32, 16, 8, 4, 2, 1). This is the production layout. The bench keeps a local copy so the 512-sample comparison does not depend on the engine build. |
| `main` |  | `plugin/tests/spectral_bench.cpp:201` | `int main()` | Runs the test or tool entry point. |
| `makeSignal` |  | `plugin/tests/ui_frame_bench.cpp:24` | `std::array<float, 512> makeSignal()` | Creates the signal. |
| `measure` |  | `plugin/tests/ui_frame_bench.cpp:39` | `double measure(Fn&& fn)` | Warm the caches and the FFT plan. |
| `measureEditor` |  | `plugin/tests/ui_frame_bench.cpp:52` | `double measureEditor(int page)` | Measures the editor path. |
| `measureComponent` |  | `plugin/tests/ui_frame_bench.cpp:73` | `double measureComponent(juce::Component& component, int frames)` | Measures the component path. |
| `reportTree` |  | `plugin/tests/ui_frame_bench.cpp:89` | `void reportTree(juce::Component& root, int depth)` | Runs the report tree step for the surrounding code. |
| `reportEditorTree` |  | `plugin/tests/ui_frame_bench.cpp:108` | `void reportEditorTree(int page)` | Runs the report editor tree step for the surrounding code. |
| `main` |  | `plugin/tests/ui_frame_bench.cpp:123` | `int main()` | Runs the test or tool entry point. |
| `check` |  | `plugin/tests/voice_alloc.cpp:16` | `void check(bool ok, const char* what)` | Records a test assertion. |
| `chord` |  | `plugin/tests/voice_alloc.cpp:23` | `bool chord(p1::Engine& engine, uint32_t channel, int& gated, bool* notes)` | Tests the chord condition. |
| `main` |  | `plugin/tests/voice_alloc.cpp:48` | `int main()` | Runs the test or tool entry point. |
| `fnv` |  | `plugin/tests/wavetable.cpp:17` | `uint64_t fnv(const int16_t* data, size_t count)` | Returns the fnv value. |
| `checkBase` |  | `plugin/tests/wavetable.cpp:27` | `bool checkBase()` | Checks the base result. |
| `checkHarmonics` |  | `plugin/tests/wavetable.cpp:41` | `bool checkHarmonics()` | Checks the harmonics result. |
| `checkSamples` |  | `plugin/tests/wavetable.cpp:74` | `bool checkSamples()` | Checks the samples result. |
| `main` |  | `plugin/tests/wavetable.cpp:123` | `int main()` | Runs the test or tool entry point. |
| `_recipe` |  | `plugin/tools/additive_prototype.py:25` | `def _recipe(harmonics: int, tilt: float, formant: int, peak: float)` | Implements the recipe helper. |
| `_table` |  | `plugin/tools/additive_prototype.py:39` | `def _table(amps: list[float], level: int)` | Implements the table helper. |
| `_emit` |  | `plugin/tools/additive_prototype.py:51` | `def _emit(name: str, table: list[int])` | Implements the emit helper. |
| `_header` |  | `plugin/tools/additive_prototype.py:60` | `def _header()` | Implements the header helper. |
| `_patch` |  | `plugin/tools/additive_prototype.py:113` | `def _patch(source: str)` | Implements the patch helper. |
| `replace` |  | `plugin/tools/additive_prototype.py:120` | `def replace(match: re.Match[str])` | Implements the replace helper. |
| `main` |  | `plugin/tools/additive_prototype.py:157` | `def main()` | Runs the test or tool entry point. |
| `_read_wav` |  | `plugin/tools/feedback_figures.py:41` | `def _read_wav(path: Path)` | Implements the read WAV helper. |
| `_chunk` |  | `plugin/tools/feedback_figures.py:50` | `def _chunk(tag: bytes, data: bytes)` | Implements the chunk helper. |
| `_write_png` |  | `plugin/tools/feedback_figures.py:59` | `def _write_png(path: Path, width: int, height: int, rows: list[bytearray])` | Implements the write png helper. |
| `_pixel` |  | `plugin/tools/feedback_figures.py:71` | `def _pixel(row: bytearray, x: int, y: int, colour: tuple[int, int, int])` | Implements the pixel helper. |
| `_panel` |  | `plugin/tools/feedback_figures.py:79` | `def _panel(samples: list[int])` | Implements the panel helper. |
| `main` |  | `plugin/tools/feedback_figures.py:105` | `def main()` | Runs the test or tool entry point. |
| `op_env_rates` |  | `plugin/tools/gen_macro_targets.py:20` | `def op_env_rates(op: int)` | The amp, feedback, morph and phase rate names for one operator. |
| `c_string` |  | `plugin/tools/gen_macro_targets.py:92` | `def c_string(value: str)` | Implements the c string helper. |
| `is_ring_param` |  | `plugin/tools/gen_macro_targets.py:96` | `def is_ring_param(name: str)` | Tests whether the the ring param. |
| `reorder_ring` |  | `plugin/tools/gen_macro_targets.py:102` | `def reorder_ring(params: list[dict[str, Any]])` | Move the ringmod part rows to the end of each part group. |
| `move` |  | `plugin/tools/gen_macro_targets.py:123` | `def move(group: list[dict[str, Any]])` | Implements the move helper. |
| `load_carriers` |  | `plugin/tools/gen_macro_targets.py:134` | `def load_carriers(tables_path: Path)` | Read the carrier masks from the generated routing table. |
| `union_members` |  | `plugin/tools/gen_macro_targets.py:150` | `def union_members(entry: dict[str, Any], part_suffix: str)` | Every member any algorithm can select, with its operator index. |
| `main` |  | `plugin/tools/gen_macro_targets.py:169` | `def main()` | Runs the test or tool entry point. |
| `display_name` |  | `plugin/tools/gen_params.py:43` | `def display_name(name: str)` | Implements the display name helper. |
| `is_ring_param` |  | `plugin/tools/gen_params.py:52` | `def is_ring_param(name: str)` | Tests whether the the ring param. |
| `reorder_ring` |  | `plugin/tools/gen_params.py:58` | `def reorder_ring(params: list[ParamDoc])` | Move the ringmod part rows to the end of each part group. |
| `move` |  | `plugin/tools/gen_params.py:81` | `def move(group: list[ParamDoc])` | Implements the move helper. |
| `reorder_key` |  | `plugin/tools/gen_params.py:92` | `def reorder_key(params: list[ParamDoc])` | Move the key-follow part rows to the end of each part group. |
| `is_key` |  | `plugin/tools/gen_params.py:101` | `def is_key(name: str)` | Tests whether the the key. |
| `move` |  | `plugin/tools/gen_params.py:119` | `def move(group: list[ParamDoc])` | Implements the move helper. |
| `c_string` |  | `plugin/tools/gen_params.py:130` | `def c_string(value: str)` | Implements the c string helper. |
| `main` |  | `plugin/tools/gen_params.py:134` | `def main()` | Runs the test or tool entry point. |
| `_argb` |  | `plugin/tools/gen_tokens.py:24` | `def _argb(hex_colour: str)` | Implements the argb helper. |
| `main` |  | `plugin/tools/gen_tokens.py:31` | `def main()` | Runs the test or tool entry point. |
| `_display` |  | `plugin/tools/ratio.py:26` | `def _display(value: int)` | The 2-decimal readout and the ideal it was rounded from. |
| `ratio_detents` |  | `plugin/tools/ratio.py:33` | `def ratio_detents(lo: int, hi: int)` | The sorted Q16 ratio detents in [lo, hi]. Two series that meet inside the. |
| `_sha256` |  | `plugin/tools/snapshot_check.py:29` | `def _sha256(path: Path)` | Implements the sha 256 helper. |
| `_png_size` |  | `plugin/tools/snapshot_check.py:37` | `def _png_size(path: Path)` | Read the IHDR width and height from a PNG without decoding it. |
| `_images` |  | `plugin/tools/snapshot_check.py:47` | `def _images(directory: Path)` | Implements the images helper. |
| `_load_manifest` |  | `plugin/tools/snapshot_check.py:53` | `def _load_manifest(golden: Path)` | Implements the load manifest helper. |
| `_write_manifest` |  | `plugin/tools/snapshot_check.py:64` | `def _write_manifest(golden: Path, images: dict[str, Path])` | Implements the write manifest helper. |
| `_diff_images` |  | `plugin/tools/snapshot_check.py:79` | `def _diff_images(actual: Path, golden: Path, out: Path, names: list[str])` | Write review images for changed snapshots when Pillow is installed. |
| `main` |  | `plugin/tools/snapshot_check.py:117` | `def main()` | Runs the test or tool entry point. |
| `partSuffix` |  | `plugin/ui/ParamIDs.h:10` | `inline juce::String partSuffix(int part)` | Returns the `.pN` suffix for a part index. |
| `withPart` |  | `plugin/ui/ParamIDs.h:15` | `inline juce::String withPart(const juce::String& base, int part)` | Appends the part suffix to a base parameter id. |
| `algorithm` |  | `plugin/ui/ParamIDs.h:20` | `inline juce::String algorithm(int part)` | Returns the `algorithm` parameter id with the part suffix. |
| `channel` |  | `plugin/ui/ParamIDs.h:21` | `inline juce::String channel(int part)` | Returns the `channel` parameter id with the part suffix. |
| `pan` |  | `plugin/ui/ParamIDs.h:22` | `inline juce::String pan(int part)` | Returns the `pan` parameter id with the part suffix. |
| `panEnvOn` |  | `plugin/ui/ParamIDs.h:23` | `inline juce::String panEnvOn(int part)` | Returns the `pan_env_on` parameter id with the part suffix. |
| `panEnvRelRate` |  | `plugin/ui/ParamIDs.h:24` | `inline juce::String panEnvRelRate(int part)` | Returns the `pan_env_relr` parameter id with the part suffix. |
| `panEnvRelLevel` |  | `plugin/ui/ParamIDs.h:28` | `inline juce::String panEnvRelLevel(int part)` | Returns the `pan_env_rell` parameter id with the part suffix. |
| `panEnvMode` |  | `plugin/ui/ParamIDs.h:32` | `inline juce::String panEnvMode(int part)` | Returns the `pan_env_mode` parameter id with the part suffix. |
| `panEnvRate` |  | `plugin/ui/ParamIDs.h:33` | `inline juce::String panEnvRate(int index, int part)` | Returns the `pan_env_r` parameter id with the part suffix. |
| `panEnvLevel` |  | `plugin/ui/ParamIDs.h:37` | `inline juce::String panEnvLevel(int index, int part)` | Returns the `pan_env_l` parameter id with the part suffix. |
| `bendRange` |  | `plugin/ui/ParamIDs.h:41` | `inline juce::String bendRange(int part)` | Returns the `bend_range` parameter id with the part suffix. |
| `keyLow` |  | `plugin/ui/ParamIDs.h:42` | `inline juce::String keyLow(int part)` | Returns the `low` parameter id with the part suffix. |
| `keyHigh` |  | `plugin/ui/ParamIDs.h:43` | `inline juce::String keyHigh(int part)` | Returns the `high` parameter id with the part suffix. |
| `level` |  | `plugin/ui/ParamIDs.h:44` | `inline juce::String level(int part)` | Returns the `level` parameter id with the part suffix. |
| `delay` |  | `plugin/ui/ParamIDs.h:45` | `inline juce::String delay(int part)` | Returns the `delay` parameter id with the part suffix. |
| `detune` |  | `plugin/ui/ParamIDs.h:46` | `inline juce::String detune(int part)` | Returns the `detune` parameter id with the part suffix. |
| `transpose` |  | `plugin/ui/ParamIDs.h:47` | `inline juce::String transpose(int part)` | Returns the `transpose` parameter id with the part suffix. |
| `mode` |  | `plugin/ui/ParamIDs.h:48` | `inline juce::String mode(int part)` | Returns the `mode` parameter id with the part suffix. |
| `legato` |  | `plugin/ui/ParamIDs.h:49` | `inline juce::String legato(int part)` | Returns the `legato` parameter id with the part suffix. |
| `unisonCount` |  | `plugin/ui/ParamIDs.h:50` | `inline juce::String unisonCount(int part)` | Returns the `unison_count` parameter id with the part suffix. |
| `unisonDetune` |  | `plugin/ui/ParamIDs.h:51` | `inline juce::String unisonDetune(int part)` | Returns the `unison_detune` parameter id with the part suffix. |
| `portamento` |  | `plugin/ui/ParamIDs.h:52` | `inline juce::String portamento(int part)` | Returns the `portamento` parameter id with the part suffix. |
| `notePriority` |  | `plugin/ui/ParamIDs.h:53` | `inline juce::String notePriority(int part)` | Returns the `note_priority` parameter id with the part suffix. |
| `opBase` |  | `plugin/ui/ParamIDs.h:55` | `inline juce::String opBase(int op, const char* field)` | Builds an operator parameter id from the operator index and field. |
| `opRatio` |  | `plugin/ui/ParamIDs.h:60` | `inline juce::String opRatio(int op, int part)` | Builds the operator parameter id for `_ratio`. |
| `opDetune` |  | `plugin/ui/ParamIDs.h:64` | `inline juce::String opDetune(int op, int part)` | Builds the operator parameter id for `_detune`. |
| `opWaveform` |  | `plugin/ui/ParamIDs.h:68` | `inline juce::String opWaveform(int op, int part)` | Builds the operator parameter id for `_waveform`. |
| `opLevel` |  | `plugin/ui/ParamIDs.h:72` | `inline juce::String opLevel(int op, int part)` | Builds the operator parameter id for `_level`. |
| `opFeedback` |  | `plugin/ui/ParamIDs.h:76` | `inline juce::String opFeedback(int op, int part)` | Builds the operator parameter id for `_feedback`. |
| `opFeedbackOn` |  | `plugin/ui/ParamIDs.h:80` | `inline juce::String opFeedbackOn(int op, int part)` | Builds the operator parameter id for `_fbenv_on`. |
| `opFeedbackLfoRate` |  | `plugin/ui/ParamIDs.h:84` | `inline juce::String opFeedbackLfoRate(int op, int part)` | Builds the operator parameter id for `_fblfo_rate`. |
| `opFeedbackLfoDepth` |  | `plugin/ui/ParamIDs.h:88` | `inline juce::String opFeedbackLfoDepth(int op, int part)` | Builds the operator parameter id for `_fblfo_depth`. |
| `opFeedbackLfoWave` |  | `plugin/ui/ParamIDs.h:92` | `inline juce::String opFeedbackLfoWave(int op, int part)` | Builds the operator parameter id for `_fblfo_wave`. |
| `opFeedbackLfoMode` |  | `plugin/ui/ParamIDs.h:96` | `inline juce::String opFeedbackLfoMode(int op, int part)` | Builds the operator parameter id for `_fblfo_mode`. |
| `opFeedbackLfoRateFade` |  | `plugin/ui/ParamIDs.h:100` | `inline juce::String opFeedbackLfoRateFade(int op, int part)` | Builds the operator parameter id for `_fblfo_rate_fade`. |
| `opFeedbackLfoDepthFade` |  | `plugin/ui/ParamIDs.h:104` | `inline juce::String opFeedbackLfoDepthFade(int op, int part)` | Builds the operator parameter id for `_fblfo_depth_fade`. |
| `opDetuneLfoRate` |  | `plugin/ui/ParamIDs.h:108` | `inline juce::String opDetuneLfoRate(int op, int part)` | Builds the operator parameter id for `_dlfo_rate`. |
| `opDetuneLfoDepth` |  | `plugin/ui/ParamIDs.h:112` | `inline juce::String opDetuneLfoDepth(int op, int part)` | Builds the operator parameter id for `_dlfo_depth`. |
| `opDetuneLfoWave` |  | `plugin/ui/ParamIDs.h:116` | `inline juce::String opDetuneLfoWave(int op, int part)` | Builds the operator parameter id for `_dlfo_wave`. |
| `opDetuneLfoMode` |  | `plugin/ui/ParamIDs.h:120` | `inline juce::String opDetuneLfoMode(int op, int part)` | Builds the operator parameter id for `_dlfo_mode`. |
| `opDetuneLfoRateFade` |  | `plugin/ui/ParamIDs.h:124` | `inline juce::String opDetuneLfoRateFade(int op, int part)` | Builds the operator parameter id for `_dlfo_rate_fade`. |
| `opDetuneLfoDepthFade` |  | `plugin/ui/ParamIDs.h:128` | `inline juce::String opDetuneLfoDepthFade(int op, int part)` | Builds the operator parameter id for `_dlfo_depth_fade`. |
| `opTremRate` |  | `plugin/ui/ParamIDs.h:132` | `inline juce::String opTremRate(int op, int part)` | Builds the operator parameter id for `_trem_rate`. |
| `opTremDepth` |  | `plugin/ui/ParamIDs.h:136` | `inline juce::String opTremDepth(int op, int part)` | Builds the operator parameter id for `_trem_depth`. |
| `opTremWave` |  | `plugin/ui/ParamIDs.h:140` | `inline juce::String opTremWave(int op, int part)` | Builds the operator parameter id for `_trem_wave`. |
| `opTremMode` |  | `plugin/ui/ParamIDs.h:144` | `inline juce::String opTremMode(int op, int part)` | Builds the operator parameter id for `_trem_mode`. |
| `opTremRateFade` |  | `plugin/ui/ParamIDs.h:148` | `inline juce::String opTremRateFade(int op, int part)` | Builds the operator parameter id for `_trem_rate_fade`. |
| `opTremDepthFade` |  | `plugin/ui/ParamIDs.h:152` | `inline juce::String opTremDepthFade(int op, int part)` | Builds the operator parameter id for `_trem_depth_fade`. |
| `opMute` |  | `plugin/ui/ParamIDs.h:156` | `inline juce::String opMute(int op, int part)` | Builds the operator parameter id for `_mute`. |
| `opPhase` |  | `plugin/ui/ParamIDs.h:161` | `inline juce::String opPhase(int op, const char* field, int part)` | Builds the operator parameter id for `_phase_`. |
| `opPhaseLfo` |  | `plugin/ui/ParamIDs.h:165` | `inline juce::String opPhaseLfo(int op, const char* field, int part)` | Builds the operator parameter id for `_phase_lfo_`. |
| `opPhaseEnv` |  | `plugin/ui/ParamIDs.h:169` | `inline juce::String opPhaseEnv(int op, const char* field, int part)` | Builds the operator parameter id for `_phase_env_`. |
| `opSpecA` |  | `plugin/ui/ParamIDs.h:174` | `inline juce::String opSpecA(int op, int harmonic, int part)` | Builds the operator parameter id for `_spec_a`. |
| `opSpecB` |  | `plugin/ui/ParamIDs.h:178` | `inline juce::String opSpecB(int op, int harmonic, int part)` | Builds the operator parameter id for `_spec_b`. |
| `opSpecMorph` |  | `plugin/ui/ParamIDs.h:182` | `inline juce::String opSpecMorph(int op, int part)` | Builds the operator parameter id for `_spec_morph`. |
| `opSpecRangeA` |  | `plugin/ui/ParamIDs.h:186` | `inline juce::String opSpecRangeA(int op, int part)` | Builds the operator parameter id for `_spec_range_a`. |
| `opSpecRangeB` |  | `plugin/ui/ParamIDs.h:190` | `inline juce::String opSpecRangeB(int op, int part)` | Builds the operator parameter id for `_spec_range_b`. |
| `opSpecEnvRate` |  | `plugin/ui/ParamIDs.h:194` | `inline juce::String opSpecEnvRate(int op, int index, int part)` | Builds the operator parameter id for `_spec_env_r`. |
| `opSpecEnvLevel` |  | `plugin/ui/ParamIDs.h:198` | `inline juce::String opSpecEnvLevel(int op, int index, int part)` | Builds the operator parameter id for `_spec_env_l`. |
| `opSpecEnvOn` |  | `plugin/ui/ParamIDs.h:202` | `inline juce::String opSpecEnvOn(int op, int part)` | Builds the operator parameter id for `_spec_env_on`. |
| `opSpecEnvRelRate` |  | `plugin/ui/ParamIDs.h:206` | `inline juce::String opSpecEnvRelRate(int op, int part)` | Builds the operator parameter id for `_spec_env_relr`. |
| `opSpecEnvRelLevel` |  | `plugin/ui/ParamIDs.h:210` | `inline juce::String opSpecEnvRelLevel(int op, int part)` | Builds the operator parameter id for `_spec_env_rell`. |
| `opSpecEnvMode` |  | `plugin/ui/ParamIDs.h:214` | `inline juce::String opSpecEnvMode(int op, int part)` | Builds the operator parameter id for `_spec_env_mode`. |
| `opSpecLfoRate` |  | `plugin/ui/ParamIDs.h:218` | `inline juce::String opSpecLfoRate(int op, int part)` | Builds the operator parameter id for `_spec_lfo_rate`. |
| `opSpecLfoDepth` |  | `plugin/ui/ParamIDs.h:222` | `inline juce::String opSpecLfoDepth(int op, int part)` | Builds the operator parameter id for `_spec_lfo_depth`. |
| `opSpecLfoWave` |  | `plugin/ui/ParamIDs.h:226` | `inline juce::String opSpecLfoWave(int op, int part)` | Builds the operator parameter id for `_spec_lfo_wave`. |
| `opSpecLfoMode` |  | `plugin/ui/ParamIDs.h:230` | `inline juce::String opSpecLfoMode(int op, int part)` | Builds the operator parameter id for `_spec_lfo_mode`. |
| `opSpecLfoRateFade` |  | `plugin/ui/ParamIDs.h:234` | `inline juce::String opSpecLfoRateFade(int op, int part)` | Builds the operator parameter id for `_spec_lfo_rate_fade`. |
| `opSpecLfoDepthFade` |  | `plugin/ui/ParamIDs.h:238` | `inline juce::String opSpecLfoDepthFade(int op, int part)` | Builds the operator parameter id for `_spec_lfo_depth_fade`. |
| `opEnv` |  | `plugin/ui/ParamIDs.h:243` | `inline juce::String opEnv(int op, const char* field, int part)` | Returns the `attack` parameter id with the part suffix. |
| `opFeedbackRate` |  | `plugin/ui/ParamIDs.h:252` | `inline juce::String opFeedbackRate(int op, int index, int part)` | Returns the op feedback rate value. |
| `opFeedbackLevel` |  | `plugin/ui/ParamIDs.h:257` | `inline juce::String opFeedbackLevel(int op, int index, int part)` | Returns the op feedback level value. |
| `opFeedbackRelRate` |  | `plugin/ui/ParamIDs.h:262` | `inline juce::String opFeedbackRelRate(int op, int part)` | Returns the op feedback rel rate value. |
| `opFeedbackRelLevel` |  | `plugin/ui/ParamIDs.h:266` | `inline juce::String opFeedbackRelLevel(int op, int part)` | Returns the op feedback rel level value. |
| `opFeedbackMode` |  | `plugin/ui/ParamIDs.h:270` | `inline juce::String opFeedbackMode(int op, int part)` | Returns the op feedback mode value. |
| `pitchRate` |  | `plugin/ui/ParamIDs.h:275` | `inline juce::String pitchRate(int part)` | Returns the `pitch_rate` parameter id with the part suffix. |
| `pitchDepth` |  | `plugin/ui/ParamIDs.h:276` | `inline juce::String pitchDepth(int part)` | Returns the `pitch_depth` parameter id with the part suffix. |
| `pitchWave` |  | `plugin/ui/ParamIDs.h:277` | `inline juce::String pitchWave(int part)` | Returns the `pitch_wave` parameter id with the part suffix. |
| `pitchMode` |  | `plugin/ui/ParamIDs.h:278` | `inline juce::String pitchMode(int part)` | Returns the `pitch_mode` parameter id with the part suffix. |
| `pitchRateFade` |  | `plugin/ui/ParamIDs.h:279` | `inline juce::String pitchRateFade(int part)` | Returns the `pitch_rate_fade` parameter id with the part suffix. |
| `pitchDepthFade` |  | `plugin/ui/ParamIDs.h:283` | `inline juce::String pitchDepthFade(int part)` | Returns the `pitch_depth_fade` parameter id with the part suffix. |
| `pitchEnvOn` |  | `plugin/ui/ParamIDs.h:287` | `inline juce::String pitchEnvOn(int part)` | Returns the `penv_on` parameter id with the part suffix. |
| `pitchEnvRate` |  | `plugin/ui/ParamIDs.h:288` | `inline juce::String pitchEnvRate(int index, int part)` | Returns the `penv_r` parameter id with the part suffix. |
| `pitchEnvLevel` |  | `plugin/ui/ParamIDs.h:292` | `inline juce::String pitchEnvLevel(int index, int part)` | Returns the `penv_l` parameter id with the part suffix. |
| `pitchEnvRelRate` |  | `plugin/ui/ParamIDs.h:296` | `inline juce::String pitchEnvRelRate(int part)` | Returns the `penv_relr` parameter id with the part suffix. |
| `pitchEnvRelLevel` |  | `plugin/ui/ParamIDs.h:300` | `inline juce::String pitchEnvRelLevel(int part)` | Returns the `penv_rell` parameter id with the part suffix. |
| `pitchEnvMode` |  | `plugin/ui/ParamIDs.h:304` | `inline juce::String pitchEnvMode(int part)` | Returns the `penv_mode` parameter id with the part suffix. |
| `filterCutoff` |  | `plugin/ui/ParamIDs.h:306` | `inline juce::String filterCutoff(int part)` | Returns the `cutoff` parameter id with the part suffix. |
| `filterRes` |  | `plugin/ui/ParamIDs.h:307` | `inline juce::String filterRes(int part)` | Returns the `res` parameter id with the part suffix. |
| `filterType` |  | `plugin/ui/ParamIDs.h:308` | `inline juce::String filterType(int part)` | Returns the `filter_type` parameter id with the part suffix. |
| `filterOn` |  | `plugin/ui/ParamIDs.h:309` | `inline juce::String filterOn(int part)` | Returns the `filter_on` parameter id with the part suffix. |
| `ringOn` |  | `plugin/ui/ParamIDs.h:311` | `inline juce::String ringOn(int pair, int part)` | Returns the `ring` parameter id with the part suffix. |
| `ringLevel` |  | `plugin/ui/ParamIDs.h:316` | `inline juce::String ringLevel(int pair, int part)` | Returns the `ring` parameter id with the part suffix. |
| `carrierOut` |  | `plugin/ui/ParamIDs.h:321` | `inline juce::String carrierOut(int op, int part)` | Returns the `car` parameter id with the part suffix. |
| `filterEnvOn` |  | `plugin/ui/ParamIDs.h:325` | `inline juce::String filterEnvOn(int part)` | Returns the `fenv_on` parameter id with the part suffix. |
| `filterEnvRelRate` |  | `plugin/ui/ParamIDs.h:326` | `inline juce::String filterEnvRelRate(int part)` | Returns the `fenv_relr` parameter id with the part suffix. |
| `filterEnvRelLevel` |  | `plugin/ui/ParamIDs.h:330` | `inline juce::String filterEnvRelLevel(int part)` | Returns the `fenv_rell` parameter id with the part suffix. |
| `filterEnvMode` |  | `plugin/ui/ParamIDs.h:334` | `inline juce::String filterEnvMode(int part)` | Returns the `fenv_mode` parameter id with the part suffix. |
| `filterRate` |  | `plugin/ui/ParamIDs.h:335` | `inline juce::String filterRate(int index, int part)` | Returns the `fenv_r` parameter id with the part suffix. |
| `filterLevel` |  | `plugin/ui/ParamIDs.h:339` | `inline juce::String filterLevel(int index, int part)` | Returns the `fenv_l` parameter id with the part suffix. |
| `followTargets` |  | `plugin/ui/ParamIDs.h:351` | `inline std::array<FollowTarget, 19> followTargets(int part)` | Returns the `op1_level_ks_lo` parameter id with the part suffix. |
| `macroValue` |  | `plugin/ui/ParamIDs.h:395` | `inline juce::String macroValue(int index)` | Returns the macro value value. |
| `macroMidi` |  | `plugin/ui/ParamIDs.h:400` | `inline juce::String macroMidi(int index)` | Returns the macro MIDI value. |
| `macroMidiRange` |  | `plugin/ui/ParamIDs.h:405` | `inline juce::String macroMidiRange(int index)` | Returns the macro MIDI range value. |
| `macroTarget` |  | `plugin/ui/ParamIDs.h:410` | `inline juce::String macroTarget(int index, int destination)` | Returns the macro target value. |
| `macroRange` |  | `plugin/ui/ParamIDs.h:415` | `inline juce::String macroRange(int index, int destination)` | Returns the macro range value. |
| `waveNames` |  | `plugin/ui/ParamIDs.h:420` | `inline juce::StringArray waveNames()` | Returns the wave names value. |
| `lfoWaveNames` |  | `plugin/ui/ParamIDs.h:427` | `inline juce::StringArray lfoWaveNames()` | Returns the LFO wave names value. |
| `analyseWaveTable` |  | `plugin/ui/SpectrumAnalysis.cpp:16` | `std::array<double, 64> analyseWaveTable(int wave, int firstHarmonic)` | Sixty-four harmonic magnitudes of one mip-0 wave table, normalised to 0..99. `firstHarmonic` is the 1-based harmonic of the first bar. Wave 14 has no table and returns zeros. |
| `analyseWaveTable` |  | `plugin/ui/SpectrumAnalysis.h:11` | `std::array<double, 64> analyseWaveTable(int wave, int firstHarmonic = 1)` | Sixty-four harmonic magnitudes of one mip-0 wave table, normalised to 0..99. `firstHarmonic` is the 1-based harmonic of the first bar. Wave 14 has no table and returns zeros. |
| `makeTypeface` |  | `plugin/ui/Theme.cpp:11` | `juce::Typeface::Ptr makeTypeface(const void* data, int size)` | Creates the typeface. |
| `colour` |  | `plugin/ui/Theme.cpp:18` | `juce::Colour colour(std::uint32_t argb)` | Returns the colour value. |
| `uiTypeface` |  | `plugin/ui/Theme.cpp:23` | `juce::Typeface::Ptr uiTypeface()` | Returns the UI typeface value. |
| `displayTypeface` |  | `plugin/ui/Theme.cpp:30` | `juce::Typeface::Ptr displayTypeface()` | Returns the display typeface value. |
| `brandTypeface` |  | `plugin/ui/Theme.cpp:37` | `juce::Typeface::Ptr brandTypeface()` | The provisional wordmark's alpha takes the bold cut, so it has real * weight rather than a second draw at an offset. |
| `uiFont` |  | `plugin/ui/Theme.cpp:46` | `juce::Font uiFont(float height)` | Returns the UI font value. |
| `displayFont` |  | `plugin/ui/Theme.cpp:51` | `juce::Font displayFont(float height)` | Returns the display font value. |
| `brandFont` |  | `plugin/ui/Theme.cpp:56` | `juce::Font brandFont(float height)` | Returns the brand font value. |
| `drawBoldText` |  | `plugin/ui/Theme.cpp:61` | `void drawBoldText(juce::Graphics& g, const juce::Font& font, const juce::String& text, juce::Rectangle<float> area, float extraWeight)` | Zen Dots has one weight, so the R boxes widen the outline to read bold. |
| `textWidth` |  | `plugin/ui/Theme.cpp:77` | `float textWidth(const juce::Font& font, const juce::String& text)` | Returns the text width value. |
| `drawSectionTitle` |  | `plugin/ui/Theme.cpp:86` | `void drawSectionTitle(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& title)` | Draws the section title. |
| `P1LookAndFeel` |  | `plugin/ui/Theme.cpp:97` | `P1LookAndFeel::P1LookAndFeel()` | Returns the p 1 look and feel value. |
| `drawComboBox` |  | `plugin/ui/Theme.cpp:116` | `void P1LookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox&)` | Draws the combo box. |
| `positionComboBoxText` |  | `plugin/ui/Theme.cpp:132` | `void P1LookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label)` | Returns the position combo box text value. |
| `drawPopupMenuBackground` |  | `plugin/ui/Theme.cpp:140` | `void P1LookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height)` | Draws the popup menu background. |
| `drawPopupMenuItem` |  | `plugin/ui/Theme.cpp:148` | `void P1LookAndFeel::drawPopupMenuItem( juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool, const juce:...` | Draws the popup menu item. |
| `drawScrollbar` |  | `plugin/ui/Theme.cpp:174` | `void P1LookAndFeel::drawScrollbar(juce::Graphics& g, juce::ScrollBar&, int, int, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool, bool)` | Draws the scrollbar. |
| `drawRotarySlider` |  | `plugin/ui/Theme.cpp:187` | `void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)` | Draws the rotary slider. |
| `colour` |  | `plugin/ui/Theme.h:12` | `juce::Colour colour(std::uint32_t argb)` | Returns the colour value. |
| `uiTypeface` |  | `plugin/ui/Theme.h:14` | `juce::Typeface::Ptr uiTypeface()` | Returns the UI typeface value. |
| `displayTypeface` |  | `plugin/ui/Theme.h:15` | `juce::Typeface::Ptr displayTypeface()` | Returns the display typeface value. |
| `brandTypeface` |  | `plugin/ui/Theme.h:16` | `juce::Typeface::Ptr brandTypeface()` | The provisional wordmark's alpha takes the bold cut, so it has real * weight rather than a second draw at an offset. |
| `uiFont` |  | `plugin/ui/Theme.h:18` | `juce::Font uiFont(float height)` | Returns the UI font value. |
| `displayFont` |  | `plugin/ui/Theme.h:19` | `juce::Font displayFont(float height)` | Returns the display font value. |
| `brandFont` |  | `plugin/ui/Theme.h:20` | `juce::Font brandFont(float height)` | Returns the brand font value. |
| `textWidth` |  | `plugin/ui/Theme.h:21` | `float textWidth(const juce::Font& font, const juce::String& text)` | Returns the text width value. |
| `drawBoldText` |  | `plugin/ui/Theme.h:24` | `void drawBoldText(juce::Graphics& g, const juce::Font& font, const juce::String& text, juce::Rectangle<float> area, float extraWeight)` | Zen Dots has one weight, so the R boxes widen the outline to read bold. |
| `drawSectionTitle` |  | `plugin/ui/Theme.h:28` | `void drawSectionTitle(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& title)` | Draws the section title. |
| `drawComboBox` | P1LookAndFeel | `plugin/ui/Theme.h:35` | `void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override` | Draws the combo box. |
| `positionComboBoxText` | P1LookAndFeel | `plugin/ui/Theme.h:38` | `void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override` | Runs the position combo box text step for the surrounding code. |
| `drawPopupMenuBackground` | P1LookAndFeel | `plugin/ui/Theme.h:39` | `void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override` | Draws the popup menu background. |
| `drawPopupMenuItem` | P1LookAndFeel | `plugin/ui/Theme.h:40` | `void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::Stri...` | Draws the popup menu item. |
| `drawScrollbar` | P1LookAndFeel | `plugin/ui/Theme.h:47` | `void drawScrollbar(juce::Graphics& g, juce::ScrollBar& bar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOve...` | Draws the scrollbar. |
| `drawRotarySlider` | KnobLookAndFeel | `plugin/ui/Theme.h:55` | `void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override` | Draws the rotary slider. |
| `knobLookAndFeel` |  | `plugin/ui/Widgets.cpp:17` | `KnobLookAndFeel& knobLookAndFeel()` | Returns the knob look and feel value. |
| `signedNumber` |  | `plugin/ui/Widgets.cpp:28` | `juce::String signedNumber(int value)` | Returns the signed number value. |
| `snapRatio` |  | `plugin/ui/Widgets.cpp:33` | `int32_t snapRatio(int32_t value)` | Returns the snap ratio value. |
| `stepRatioDetent` |  | `plugin/ui/Widgets.cpp:51` | `int32_t stepRatioDetent(int32_t value, int dir)` | The detent one step above (dir > 0) or one step below the current value. An off-detent value moves to the neighbour on that side, so a typed 1.37 followed by an up-arrow lands on 1.41 rather than 1.50. |
| `notePair` |  | `plugin/ui/Widgets.cpp:67` | `juce::String notePair(int note)` | Returns the note pair value. |
| `percentage` |  | `plugin/ui/Widgets.cpp:74` | `juce::String percentage(const P1ParamSpec& spec, double value, bool inverse)` | Returns the percentage value. |
| `findSpec` |  | `plugin/ui/Widgets.cpp:88` | `const P1ParamSpec* findSpec(const juce::String& id)` | Finds the spec. |
| `volumeText` |  | `plugin/ui/Widgets.cpp:97` | `juce::String volumeText(int position)` | The master volume fader position as dB text, and back. The law is in VolumeLaw.h, which the processor and the header slider share. |
| `volumePositionFromText` |  | `plugin/ui/Widgets.cpp:106` | `int volumePositionFromText(const juce::String& text)` | Returns the volume position from text value. |
| `formatParamValue` |  | `plugin/ui/Widgets.cpp:121` | `juce::String formatParamValue(const P1ParamSpec& spec, double value)` | Formats the param value. |
| `formatParamValuePlain` |  | `plugin/ui/Widgets.cpp:222` | `juce::String formatParamValuePlain(const P1ParamSpec& spec, double value)` | The same value without its unit, for the effective value beside a stored one. |
| `parseParamValue` |  | `plugin/ui/Widgets.cpp:237` | `double parseParamValue(const P1ParamSpec& spec, const juce::String& text)` | Parses the param value. |
| `ParamValueLabel` |  | `plugin/ui/Widgets.cpp:338` | `ParamValueLabel::ParamValueLabel() : storedColour_(juce::Colours::white), effectiveColour_(juce::Colours::white)` | Returns the param value label value. |
| `setStoredColour` |  | `plugin/ui/Widgets.cpp:343` | `void ParamValueLabel::setStoredColour(juce::Colour colour)` | Writes the stored colour. |
| `setEffectiveText` |  | `plugin/ui/Widgets.cpp:352` | `void ParamValueLabel::setEffectiveText(const juce::String& text, juce::Colour colour)` | Writes the effective text. |
| `paint` |  | `plugin/ui/Widgets.cpp:362` | `void ParamValueLabel::paint(juce::Graphics& g)` | Draws the component. |
| `ParamKnob` |  | `plugin/ui/Widgets.cpp:402` | `ParamKnob::ParamKnob(APVTS& state, juce::String paramID, juce::String label, float dialSize, float labelSize) : state_(state), paramID_(std::move(paramID)), dialSize_(dialSize),...` | Returns the param knob value. |
| `~ParamKnob` |  | `plugin/ui/Widgets.cpp:462` | `ParamKnob::~ParamKnob()` | Destroys the object. |
| `mouseWheelMove` |  | `plugin/ui/Widgets.cpp:467` | `void ParamKnob::KnobSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)` | Handles a mouse interaction. |
| `setParamID` |  | `plugin/ui/Widgets.cpp:501` | `void ParamKnob::setParamID(const juce::String& paramID)` | Writes the param id. |
| `setMacroFeedback` |  | `plugin/ui/Widgets.cpp:519` | `void ParamKnob::setMacroFeedback(bool targeted, bool hasEffective, double effective)` | Writes the macro feedback. |
| `setModArc` |  | `plugin/ui/Widgets.cpp:531` | `void ParamKnob::setModArc(double fraction)` | Writes the mod arc. |
| `resized` |  | `plugin/ui/Widgets.cpp:542` | `void ParamKnob::resized()` | Lays out the child components. |
| `paint` |  | `plugin/ui/Widgets.cpp:555` | `void ParamKnob::paint(juce::Graphics&)` | Draws the component. |
| `updateValueText` |  | `plugin/ui/Widgets.cpp:557` | `void ParamKnob::updateValueText()` | Updates the value text. |
| `refreshValueDisplay` |  | `plugin/ui/Widgets.cpp:565` | `void ParamKnob::refreshValueDisplay()` | Refreshes the value display display. |
| `isRatio` |  | `plugin/ui/Widgets.cpp:578` | `bool ParamKnob::isRatio() const` | Tests whether the the ratio. |
| `snapValue` |  | `plugin/ui/Widgets.cpp:583` | `void ParamKnob::snapValue()` | Returns the snap value value. |
| `stepRatio` |  | `plugin/ui/Widgets.cpp:595` | `bool ParamKnob::stepRatio(const juce::KeyPress& key)` | Tests the step ratio condition. |
| `commitValue` |  | `plugin/ui/Widgets.cpp:613` | `void ParamKnob::commitValue()` | Returns the commit value value. |
| `P1Button` |  | `plugin/ui/Widgets.cpp:625` | `P1Button::P1Button(const juce::String& label, bool mini) : juce::Button(label)` | Returns the p 1 button value. |
| `paintButton` |  | `plugin/ui/Widgets.cpp:633` | `void P1Button::paintButton(juce::Graphics& g, bool highlighted, bool down)` | Draws the button in its normal, highlighted and down states. |
| `P1MuteTile` |  | `plugin/ui/Widgets.cpp:649` | `P1MuteTile::P1MuteTile(const juce::String& label, bool muted, std::function<void(bool)> onToggle) : juce::Button(label)` | Returns the p 1 mute tile value. |
| `paintButton` |  | `plugin/ui/Widgets.cpp:664` | `void P1MuteTile::paintButton(juce::Graphics& g, bool highlighted, bool)` | Draws the button in its normal, highlighted and down states. |
| `P1Toggle` |  | `plugin/ui/Widgets.cpp:680` | `P1Toggle::P1Toggle(APVTS& state, const juce::String& paramID, const juce::String& label) : juce::Button(label), label_(label)` | Returns the p 1 toggle value. |
| `paintButton` |  | `plugin/ui/Widgets.cpp:690` | `void P1Toggle::paintButton(juce::Graphics& g, bool highlighted, bool)` | Draws the button in its normal, highlighted and down states. |
| `ParamChoice` |  | `plugin/ui/Widgets.cpp:713` | `ParamChoice::ParamChoice(APVTS& state, const juce::String& paramID, juce::StringArray items)` | Returns the param choice value. |
| `resized` |  | `plugin/ui/Widgets.cpp:733` | `void ParamChoice::resized()` | Lays out the child components. |
| `updateFromNormalized` |  | `plugin/ui/Widgets.cpp:738` | `void ParamChoice::updateFromNormalized(float normalized)` | Updates the from normalized. |
| `Tab` |  | `plugin/ui/Widgets.cpp:747` | `P1TabBar::Tab::Tab(const juce::String& label, int index, P1TabBar& owner) : juce::Button(label), index_(index), owner_(owner)` | Returns the tab value. |
| `paintButton` |  | `plugin/ui/Widgets.cpp:756` | `void P1TabBar::Tab::paintButton(juce::Graphics& g, bool highlighted, bool)` | Draws the button in its normal, highlighted and down states. |
| `P1TabBar` |  | `plugin/ui/Widgets.cpp:784` | `P1TabBar::P1TabBar() = default` | Returns the p 1 tab bar value. |
| `setLabels` |  | `plugin/ui/Widgets.cpp:786` | `void P1TabBar::setLabels(const juce::StringArray& labels)` | Writes the labels. |
| `setCompact` |  | `plugin/ui/Widgets.cpp:802` | `void P1TabBar::setCompact(bool compact)` | Sets the compact flag. |
| `setActive` |  | `plugin/ui/Widgets.cpp:811` | `void P1TabBar::setActive(int index)` | Sets the active flag. |
| `resized` |  | `plugin/ui/Widgets.cpp:824` | `void P1TabBar::resized()` | Lays out the child components. |
| `paint` |  | `plugin/ui/Widgets.cpp:870` | `void P1TabBar::paint(juce::Graphics& g)` | Draws the component. |
| `P1Scope` |  | `plugin/ui/Widgets.cpp:877` | `P1Scope::P1Scope()` | Returns the p 1 scope value. |
| `setSamples` |  | `plugin/ui/Widgets.cpp:882` | `void P1Scope::setSamples(const float* samples, int count)` | Writes the samples. |
| `paint` |  | `plugin/ui/Widgets.cpp:892` | `void P1Scope::paint(juce::Graphics& g)` | Draws the component. |
| `P1Spectro` |  | `plugin/ui/Widgets.cpp:927` | `P1Spectro::P1Spectro()` | Returns the p 1 spectro value. |
| `setSamples` |  | `plugin/ui/Widgets.cpp:942` | `void P1Spectro::setSamples(const float* samples, int count)` | Writes the samples. |
| `paint` |  | `plugin/ui/Widgets.cpp:952` | `void P1Spectro::paint(juce::Graphics& g)` | Draws the component. |
| `P1SpectralBars` |  | `plugin/ui/Widgets.cpp:982` | `P1SpectralBars::P1SpectralBars(APVTS& state, juce::StringArray parameterIDs, juce::Colour barColour) : state_(state), parameterIDs_(std::move(parameterIDs)), colour_(barColour)` | Returns the p 1 spectral bars value. |
| `levelAt` |  | `plugin/ui/Widgets.cpp:989` | `double P1SpectralBars::levelAt(int index) const` | Returns the level at value. |
| `levels` |  | `plugin/ui/Widgets.cpp:998` | `std::vector<double> P1SpectralBars::levels() const` | Returns the levels value. |
| `setFirstHarmonic` |  | `plugin/ui/Widgets.cpp:1007` | `void P1SpectralBars::setFirstHarmonic(int first)` | Writes the first harmonic. |
| `setEditWindow` |  | `plugin/ui/Widgets.cpp:1015` | `void P1SpectralBars::setEditWindow(int first, int last)` | The selection window in absolute harmonics, and the live/dead test that composes with it. Freehand editing only writes a bar both allow. |
| `setEditTest` |  | `plugin/ui/Widgets.cpp:1022` | `void P1SpectralBars::setEditTest(std::function<bool(int, double)> test)` | Writes the edit test. |
| `inWindow` |  | `plugin/ui/Widgets.cpp:1027` | `bool P1SpectralBars::inWindow(int harmonic) const` | Tests the in window condition. |
| `setGroupTest` |  | `plugin/ui/Widgets.cpp:1032` | `void P1SpectralBars::setGroupTest(std::function<bool(int)> test)` | Writes the group test. |
| `setLevel` |  | `plugin/ui/Widgets.cpp:1038` | `void P1SpectralBars::setLevel(int index, double value)` | Writes the level. |
| `setLevels` |  | `plugin/ui/Widgets.cpp:1047` | `void P1SpectralBars::setLevels(const std::vector<double>& levels)` | Writes the levels. |
| `setFromMouse` |  | `plugin/ui/Widgets.cpp:1056` | `void P1SpectralBars::setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `mouseDown` |  | `plugin/ui/Widgets.cpp:1082` | `void P1SpectralBars::mouseDown(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `mouseDrag` |  | `plugin/ui/Widgets.cpp:1089` | `void P1SpectralBars::mouseDrag(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `paint` |  | `plugin/ui/Widgets.cpp:1096` | `void P1SpectralBars::paint(juce::Graphics& g)` | Draws the component. |
| `P1SpectrumDisplay` |  | `plugin/ui/Widgets.cpp:1166` | `P1SpectrumDisplay::P1SpectrumDisplay(APVTS& state, juce::StringArray aIDs, juce::StringArray bIDs, juce::String morphID, juce::String rangeAID, juce::String rangeBID, juce::Colo...` | Returns the p 1 spectrum display value. |
| `tick` |  | `plugin/ui/Widgets.cpp:1185` | `void P1SpectrumDisplay::tick()` | Refreshes the live display state. |
| `levelAt` |  | `plugin/ui/Widgets.cpp:1218` | `double P1SpectrumDisplay::levelAt(const juce::StringArray& ids, int index) const` | Returns the level at value. |
| `paint` |  | `plugin/ui/Widgets.cpp:1227` | `void P1SpectrumDisplay::paint(juce::Graphics& g)` | Draws the component. |
| `P1Envelope` |  | `plugin/ui/Widgets.cpp:1400` | `P1Envelope::P1Envelope(APVTS& state, juce::StringArray rateIDs, juce::StringArray levelIDs, juce::Colour curveColour, double levelMin, double levelMax, bool zeroLine, EnvelopeSh...` | Returns the p 1 envelope value. |
| `tick` |  | `plugin/ui/Widgets.cpp:1420` | `void P1Envelope::tick()` | Refreshes the live display state. |
| `paint` |  | `plugin/ui/Widgets.cpp:1456` | `void P1Envelope::paint(juce::Graphics& g)` | Draws the component. |
| `P1Wave` |  | `plugin/ui/Widgets.cpp:1689` | `P1Wave::P1Wave(APVTS& state, juce::String waveID, juce::Colour curveColour) : state_(state), waveID_(std::move(waveID)), colour_(curveColour)` | Returns the p 1 wave value. |
| `tick` |  | `plugin/ui/Widgets.cpp:1695` | `void P1Wave::tick()` | Refreshes the live display state. |
| `paint` |  | `plugin/ui/Widgets.cpp:1712` | `void P1Wave::paint(juce::Graphics& g)` | Draws the component. |
| `P1LevelSlider` |  | `plugin/ui/Widgets.cpp:1749` | `P1LevelSlider::P1LevelSlider(APVTS& state, const juce::String& paramID, juce::String caption, bool bipolar) : state_(state), paramID_(paramID), caption_(std::move(caption)), bip...` | Returns the p 1 level slider value. |
| `setMacroFeedback` |  | `plugin/ui/Widgets.cpp:1771` | `void P1LevelSlider::setMacroFeedback(bool targeted, bool hasEffective, double effective)` | Writes the macro feedback. |
| `resized` |  | `plugin/ui/Widgets.cpp:1783` | `void P1LevelSlider::resized()` | Lays out the child components. |
| `paint` |  | `plugin/ui/Widgets.cpp:1788` | `void P1LevelSlider::paint(juce::Graphics& g)` | Draws the component. |
| `mouseDown` |  | `plugin/ui/Widgets.cpp:1855` | `void P1LevelSlider::mouseDown(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `mouseDrag` |  | `plugin/ui/Widgets.cpp:1860` | `void P1LevelSlider::mouseDrag(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `setFromMouse` |  | `plugin/ui/Widgets.cpp:1865` | `void P1LevelSlider::setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `P1VolumeSlider` |  | `plugin/ui/Widgets.cpp:1878` | `P1VolumeSlider::P1VolumeSlider(APVTS& state, const juce::String& paramID) : state_(state), paramID_(paramID)` | Returns the p 1 volume slider value. |
| `resized` |  | `plugin/ui/Widgets.cpp:1894` | `void P1VolumeSlider::resized()` | Lays out the child components. |
| `paint` |  | `plugin/ui/Widgets.cpp:1899` | `void P1VolumeSlider::paint(juce::Graphics& g)` | Draws the component. |
| `mouseDown` |  | `plugin/ui/Widgets.cpp:1930` | `void P1VolumeSlider::mouseDown(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `mouseDrag` |  | `plugin/ui/Widgets.cpp:1935` | `void P1VolumeSlider::mouseDrag(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `setFromMouse` |  | `plugin/ui/Widgets.cpp:1940` | `void P1VolumeSlider::setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `P1Card` |  | `plugin/ui/Widgets.cpp:1952` | `P1Card::P1Card(juce::String title, std::function<void()> onCopy, std::function<void()> onPaste) : title_(std::move(title))` | Returns the p 1 card value. |
| `paint` |  | `plugin/ui/Widgets.cpp:1966` | `void P1Card::paint(juce::Graphics& g)` | Draws the component. |
| `resized` |  | `plugin/ui/Widgets.cpp:1972` | `void P1Card::resized()` | Lays out the child components. |
| `setHeaderTrailingWidth` |  | `plugin/ui/Widgets.cpp:1984` | `void P1Card::setHeaderTrailingWidth(int width)` | Writes the header trailing width. |
| `headerTrailingBounds` |  | `plugin/ui/Widgets.cpp:1990` | `juce::Rectangle<int> P1Card::headerTrailingBounds() const` | Returns the header trailing bounds value. |
| `contentArea` |  | `plugin/ui/Widgets.cpp:1996` | `juce::Rectangle<int> P1Card::contentArea() const` | Returns the content area value. |
| `P1AlgorithmBlock` |  | `plugin/ui/Widgets.cpp:2001` | `P1AlgorithmBlock::P1AlgorithmBlock()` | Returns the p 1 algorithm block value. |
| `setAlgorithm` |  | `plugin/ui/Widgets.cpp:2006` | `void P1AlgorithmBlock::setAlgorithm(int algorithm)` | Writes the algorithm. |
| `setRingMask` |  | `plugin/ui/Widgets.cpp:2012` | `void P1AlgorithmBlock::setRingMask(uint8_t mask)` | Writes the ring mask. |
| `setDirectMask` |  | `plugin/ui/Widgets.cpp:2020` | `void P1AlgorithmBlock::setDirectMask(uint8_t mask)` | Writes the direct mask. |
| `paint` |  | `plugin/ui/Widgets.cpp:2028` | `void P1AlgorithmBlock::paint(juce::Graphics& g)` | Draws the component. |
| `mouseUp` |  | `plugin/ui/Widgets.cpp:2049` | `void P1AlgorithmBlock::mouseUp(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `mouseEnter` |  | `plugin/ui/Widgets.cpp:2055` | `void P1AlgorithmBlock::mouseEnter(const juce::MouseEvent&)` | Handles a mouse interaction. |
| `mouseExit` |  | `plugin/ui/Widgets.cpp:2061` | `void P1AlgorithmBlock::mouseExit(const juce::MouseEvent&)` | Handles a mouse interaction. |
| `mouseWheelMove` |  | `plugin/ui/Widgets.cpp:2067` | `void P1AlgorithmBlock::mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)` | Handles a mouse interaction. |
| `P1Dialog` |  | `plugin/ui/Widgets.cpp:2074` | `P1Dialog::P1Dialog(juce::String title, std::unique_ptr<juce::Component> body, int width, int height, juce::StringArray buttons, std::function<void(int)> onButton, std::function<...` | Returns the p 1 dialog value. |
| `paint` |  | `plugin/ui/Widgets.cpp:2107` | `void P1Dialog::paint(juce::Graphics& g)` | Draws the component. |
| `resized` |  | `plugin/ui/Widgets.cpp:2126` | `void P1Dialog::resized()` | Lays out the child components. |
| `mouseUp` |  | `plugin/ui/Widgets.cpp:2170` | `void P1Dialog::mouseUp(const juce::MouseEvent& event)` | Handles a mouse interaction. |
| `drawAlgorithmGraph` |  | `plugin/ui/Widgets.cpp:2182` | `void drawAlgorithmGraph(juce::Graphics& g, juce::Rectangle<float> area, int algorithm, uint8_t ringMask, uint8_t directMask)` | Draws the algorithm graph. |
| `findSpec` |  | `plugin/ui/Widgets.h:21` | `const P1ParamSpec* findSpec(const juce::String& id)` | Finds the spec. |
| `formatParamValue` |  | `plugin/ui/Widgets.h:22` | `juce::String formatParamValue(const P1ParamSpec& spec, double value)` | Formats the param value. |
| `formatParamValuePlain` |  | `plugin/ui/Widgets.h:24` | `juce::String formatParamValuePlain(const P1ParamSpec& spec, double value)` | The same value without its unit, for the effective value beside a stored one. |
| `parseParamValue` |  | `plugin/ui/Widgets.h:25` | `double parseParamValue(const P1ParamSpec& spec, const juce::String& text)` | Parses the param value. |
| `volumeText` |  | `plugin/ui/Widgets.h:29` | `juce::String volumeText(int position)` | The master volume fader position as dB text, and back. The law is in VolumeLaw.h, which the processor and the header slider share. |
| `volumePositionFromText` |  | `plugin/ui/Widgets.h:30` | `int volumePositionFromText(const juce::String& text)` | Returns the volume position from text value. |
| `~MacroFeedbackTarget` | MacroFeedbackTarget | `plugin/ui/Widgets.h:37` | `virtual ~MacroFeedbackTarget() = default` | Destroys the object. |
| `macroParamIndex` | MacroFeedbackTarget | `plugin/ui/Widgets.h:38` | `virtual int macroParamIndex() const = 0` | Returns the manifest index used for macro feedback. |
| `setMacroFeedback` | MacroFeedbackTarget | `plugin/ui/Widgets.h:39` | `virtual void setMacroFeedback(bool targeted, bool hasEffective, double effective) = 0` | Writes the macro feedback. |
| `setStoredColour` | ParamValueLabel | `plugin/ui/Widgets.h:49` | `void setStoredColour(juce::Colour colour)` | Writes the stored colour. |
| `setEffectiveText` | ParamValueLabel | `plugin/ui/Widgets.h:50` | `void setEffectiveText(const juce::String& text, juce::Colour colour)` | Writes the effective text. |
| `paint` | ParamValueLabel | `plugin/ui/Widgets.h:51` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `setParamID` | ParamKnob | `plugin/ui/Widgets.h:65` | `void setParamID(const juce::String& paramID)` | Writes the param id. |
| `setModArc` | ParamKnob | `plugin/ui/Widgets.h:66` | `void setModArc(double fraction)` | Writes the mod arc. |
| `macroParamIndex` | ParamKnob | `plugin/ui/Widgets.h:67` | `int macroParamIndex() const override` | Returns the manifest index used for macro feedback. |
| `setMacroFeedback` | ParamKnob | `plugin/ui/Widgets.h:68` | `void setMacroFeedback(bool targeted, bool hasEffective, double effective) override` | Writes the macro feedback. |
| `resized` | ParamKnob | `plugin/ui/Widgets.h:70` | `void resized() override` | Lays out the child components. |
| `paint` | ParamKnob | `plugin/ui/Widgets.h:71` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `keyPressed` | KnobSlider | `plugin/ui/Widgets.h:85` | `bool keyPressed(const juce::KeyPress& key) override` | Handles a key press. |
| `mouseWheelMove` | KnobSlider | `plugin/ui/Widgets.h:92` | `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override` | Handles a mouse interaction. |
| `updateValueText` | ParamKnob | `plugin/ui/Widgets.h:100` | `void updateValueText()` | Updates the value text. |
| `refreshValueDisplay` | ParamKnob | `plugin/ui/Widgets.h:101` | `void refreshValueDisplay()` | Refreshes the value display display. |
| `commitValue` | ParamKnob | `plugin/ui/Widgets.h:102` | `void commitValue()` | Runs the commit value step for the surrounding code. |
| `snapValue` | ParamKnob | `plugin/ui/Widgets.h:103` | `void snapValue()` | Runs the snap value step for the surrounding code. |
| `stepRatio` | ParamKnob | `plugin/ui/Widgets.h:104` | `bool stepRatio(const juce::KeyPress& key)` | Tests the step ratio condition. |
| `isRatio` | ParamKnob | `plugin/ui/Widgets.h:105` | `bool isRatio() const` | Tests whether the the ratio. |
| `paintButton` | P1Button | `plugin/ui/Widgets.h:127` | `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override` | Draws the button in its normal, highlighted and down states. |
| `paintButton` | P1MuteTile | `plugin/ui/Widgets.h:135` | `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override` | Draws the button in its normal, highlighted and down states. |
| `paintButton` | P1Toggle | `plugin/ui/Widgets.h:143` | `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override` | Draws the button in its normal, highlighted and down states. |
| `P1Choice` | P1Choice | `plugin/ui/Widgets.h:153` | `P1Choice() = default` | Constructs the object. |
| `resized` | ParamChoice | `plugin/ui/Widgets.h:160` | `void resized() override` | Lays out the child components. |
| `updateFromNormalized` | ParamChoice | `plugin/ui/Widgets.h:163` | `void updateFromNormalized(float normalized)` | Updates the from normalized. |
| `setLabels` | P1TabBar | `plugin/ui/Widgets.h:174` | `void setLabels(const juce::StringArray& labels)` | Writes the labels. |
| `setActive` | P1TabBar | `plugin/ui/Widgets.h:175` | `void setActive(int index)` | Sets the active flag. |
| `setCompact` | P1TabBar | `plugin/ui/Widgets.h:176` | `void setCompact(bool compact)` | Sets the compact flag. |
| `resized` | P1TabBar | `plugin/ui/Widgets.h:177` | `void resized() override` | Lays out the child components. |
| `paint` | P1TabBar | `plugin/ui/Widgets.h:178` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `paintButton` | Tab | `plugin/ui/Widgets.h:185` | `void paintButton(juce::Graphics& g, bool highlighted, bool down) override` | Draws the button in its normal, highlighted and down states. |
| `setSamples` | P1Scope | `plugin/ui/Widgets.h:201` | `void setSamples(const float* samples, int count)` | Writes the samples. |
| `paint` | P1Scope | `plugin/ui/Widgets.h:202` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `setSamples` | P1Spectro | `plugin/ui/Widgets.h:211` | `void setSamples(const float* samples, int count)` | Writes the samples. |
| `paint` | P1Spectro | `plugin/ui/Widgets.h:212` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `paint` | P1SpectralBars | `plugin/ui/Widgets.h:228` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseDown` | P1SpectralBars | `plugin/ui/Widgets.h:229` | `void mouseDown(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseDrag` | P1SpectralBars | `plugin/ui/Widgets.h:230` | `void mouseDrag(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `levels` | P1SpectralBars | `plugin/ui/Widgets.h:232` | `std::vector<double> levels() const` | Returns the levels value. |
| `setLevels` | P1SpectralBars | `plugin/ui/Widgets.h:233` | `void setLevels(const std::vector<double>& levels)` | Writes the levels. |
| `setFirstHarmonic` | P1SpectralBars | `plugin/ui/Widgets.h:234` | `void setFirstHarmonic(int first)` | Writes the first harmonic. |
| `setGroupTest` | P1SpectralBars | `plugin/ui/Widgets.h:235` | `void setGroupTest(std::function<bool(int)> test)` | Writes the group test. |
| `setEditWindow` | P1SpectralBars | `plugin/ui/Widgets.h:238` | `void setEditWindow(int first, int last)` | The selection window in absolute harmonics, and the live/dead test that composes with it. Freehand editing only writes a bar both allow. |
| `setEditTest` | P1SpectralBars | `plugin/ui/Widgets.h:239` | `void setEditTest(std::function<bool(int, double)> test)` | Writes the edit test. |
| `setFromMouse` | P1SpectralBars | `plugin/ui/Widgets.h:243` | `void setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `levelAt` | P1SpectralBars | `plugin/ui/Widgets.h:244` | `double levelAt(int index) const` | Returns the level at value. |
| `setLevel` | P1SpectralBars | `plugin/ui/Widgets.h:245` | `void setLevel(int index, double value)` | Writes the level. |
| `inWindow` | P1SpectralBars | `plugin/ui/Widgets.h:246` | `bool inWindow(int harmonic) const` | Tests the in window condition. |
| `paint` | P1SpectrumDisplay | `plugin/ui/Widgets.h:264` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `setLiveMorph` | P1SpectrumDisplay | `plugin/ui/Widgets.h:266` | `void setLiveMorph(double value)` | The morph the engine last read, 0 to 1, or -1 for the base parameter. |
| `tick` | P1SpectrumDisplay | `plugin/ui/Widgets.h:267` | `void tick()` | Repaint only when the drawn parameter state changed. |
| `levelAt` | P1SpectrumDisplay | `plugin/ui/Widgets.h:270` | `double levelAt(const juce::StringArray& ids, int index) const` | Returns the level at value. |
| `paint` | P1Envelope | `plugin/ui/Widgets.h:299` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `refresh` | P1Envelope | `plugin/ui/Widgets.h:300` | `void refresh()` | Refreshes the display. |
| `tick` | P1Envelope | `plugin/ui/Widgets.h:302` | `void tick()` | Repaint only when the drawn parameter state changed. |
| `paint` | P1Wave | `plugin/ui/Widgets.h:323` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `tick` | P1Wave | `plugin/ui/Widgets.h:324` | `void tick()` | Repaint only when the drawn parameter state changed. |
| `resized` | P1LevelSlider | `plugin/ui/Widgets.h:338` | `void resized() override` | Lays out the child components. |
| `paint` | P1LevelSlider | `plugin/ui/Widgets.h:339` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseDown` | P1LevelSlider | `plugin/ui/Widgets.h:340` | `void mouseDown(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseDrag` | P1LevelSlider | `plugin/ui/Widgets.h:341` | `void mouseDrag(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `macroParamIndex` | P1LevelSlider | `plugin/ui/Widgets.h:342` | `int macroParamIndex() const override` | Returns the manifest index used for macro feedback. |
| `setMacroFeedback` | P1LevelSlider | `plugin/ui/Widgets.h:343` | `void setMacroFeedback(bool targeted, bool hasEffective, double effective) override` | Writes the macro feedback. |
| `setFromMouse` | P1LevelSlider | `plugin/ui/Widgets.h:347` | `void setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `resized` | P1VolumeSlider | `plugin/ui/Widgets.h:369` | `void resized() override` | Lays out the child components. |
| `paint` | P1VolumeSlider | `plugin/ui/Widgets.h:370` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseDown` | P1VolumeSlider | `plugin/ui/Widgets.h:371` | `void mouseDown(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseDrag` | P1VolumeSlider | `plugin/ui/Widgets.h:372` | `void mouseDrag(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `setFromMouse` | P1VolumeSlider | `plugin/ui/Widgets.h:375` | `void setFromMouse(const juce::MouseEvent& event)` | Writes the from mouse. |
| `paint` | P1Card | `plugin/ui/Widgets.h:388` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `resized` | P1Card | `plugin/ui/Widgets.h:389` | `void resized() override` | Lays out the child components. |
| `setHeaderTrailingWidth` | P1Card | `plugin/ui/Widgets.h:391` | `void setHeaderTrailingWidth(int width)` | Writes the header trailing width. |
| `headerTrailingBounds` | P1Card | `plugin/ui/Widgets.h:392` | `juce::Rectangle<int> headerTrailingBounds() const` | Returns the header trailing bounds value. |
| `contentArea` | P1Card | `plugin/ui/Widgets.h:393` | `juce::Rectangle<int> contentArea() const` | Returns the content area value. |
| `setAlgorithm` | P1AlgorithmBlock | `plugin/ui/Widgets.h:406` | `void setAlgorithm(int algorithm)` | Writes the algorithm. |
| `setRingMask` | P1AlgorithmBlock | `plugin/ui/Widgets.h:407` | `void setRingMask(uint8_t mask)` | Writes the ring mask. |
| `setDirectMask` | P1AlgorithmBlock | `plugin/ui/Widgets.h:408` | `void setDirectMask(uint8_t mask)` | Writes the direct mask. |
| `paint` | P1AlgorithmBlock | `plugin/ui/Widgets.h:409` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `mouseUp` | P1AlgorithmBlock | `plugin/ui/Widgets.h:410` | `void mouseUp(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseEnter` | P1AlgorithmBlock | `plugin/ui/Widgets.h:411` | `void mouseEnter(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseExit` | P1AlgorithmBlock | `plugin/ui/Widgets.h:412` | `void mouseExit(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `mouseWheelMove` | P1AlgorithmBlock | `plugin/ui/Widgets.h:413` | `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override` | Handles a mouse interaction. |
| `paint` | P1Dialog | `plugin/ui/Widgets.h:432` | `void paint(juce::Graphics& g) override` | Draws the component. |
| `resized` | P1Dialog | `plugin/ui/Widgets.h:433` | `void resized() override` | Lays out the child components. |
| `mouseUp` | P1Dialog | `plugin/ui/Widgets.h:434` | `void mouseUp(const juce::MouseEvent& event) override` | Handles a mouse interaction. |
| `drawAlgorithmGraph` |  | `plugin/ui/Widgets.h:447` | `void drawAlgorithmGraph(juce::Graphics& g, juce::Rectangle<float> area, int algorithm, uint8_t ringMask = 0, uint8_t directMask = 0)` | Draws the algorithm graph. |
| `load` |  | `tools/audit_modules.py:23` | `def load(path: Path)` | Implements the load helper. |
| `module_spec` |  | `tools/audit_modules.py:27` | `def module_spec(kind: str)` | Implements the module spec helper. |
| `target_node` |  | `tools/audit_modules.py:35` | `def target_node(value: object)` | Implements the target node helper. |
| `main` |  | `tools/audit_modules.py:41` | `def main()` | Runs the test or tool entry point. |
| `text` |  | `tools/gen_code_docs.py:86` | `def text(node, src: bytes)` | Implements the text helper. |
| `name_of` |  | `tools/gen_code_docs.py:90` | `def name_of(node, src: bytes)` | The last declarator name, resolving qualified names and pointers. |
| `clean_comment` |  | `tools/gen_code_docs.py:119` | `def clean_comment(lines: list[str], start_line: int)` | The comment block immediately above `start_line`, collapsed to prose. |
| `clean_signature` |  | `tools/gen_code_docs.py:150` | `def clean_signature(value: str)` | Implements the clean signature helper. |
| `parser_for` |  | `tools/gen_code_docs.py:156` | `def parser_for(path: Path)` | Parses the r for. |
| `is_global_scope` |  | `tools/gen_code_docs.py:164` | `def is_global_scope(node)` | Tests whether the the global scope. |
| `class_scope` |  | `tools/gen_code_docs.py:194` | `def class_scope(node, src: bytes)` | Implements the class scope helper. |
| `collect_c_family` |  | `tools/gen_code_docs.py:203` | `def collect_c_family(path: Path)` | Implements the collect c family helper. |
| `add_function` |  | `tools/gen_code_docs.py:221` | `def add_function(node)` | Adds the function. |
| `add_type` |  | `tools/gen_code_docs.py:260` | `def add_type(node)` | Adds the type. |
| `walk` |  | `tools/gen_code_docs.py:307` | `def walk(node)` | Implements the walk helper. |
| `collect_python` |  | `tools/gen_code_docs.py:417` | `def collect_python(path: Path)` | Implements the collect python helper. |
| `docstring` |  | `tools/gen_code_docs.py:427` | `def docstring(node)` | Implements the docstring helper. |
| `walk` |  | `tools/gen_code_docs.py:439` | `def walk(node)` | Implements the walk helper. |
| `source_files` |  | `tools/gen_code_docs.py:503` | `def source_files(include_dev: bool)` | The tracked source and CMake files, from the git index. |
| `md_escape` |  | `tools/gen_code_docs.py:526` | `def md_escape(value: str)` | Implements the md escape helper. |
| `file_summary` |  | `tools/gen_code_docs.py:530` | `def file_summary(path: Path)` | The first prose sentence of the file's leading comment or heading. |
| `write_files` |  | `tools/gen_code_docs.py:642` | `def write_files(paths: list[Path], out: Path)` | Writes the files. |
| `write_types` |  | `tools/gen_code_docs.py:707` | `def write_types(types: list[Type], out: Path)` | Writes the types. |
| `split_words` |  | `tools/gen_code_docs.py:919` | `def split_words(name: str)` | Splits a camelCase or snake_case identifier into lower-case words. |
| `inner_comment` |  | `tools/gen_code_docs.py:927` | `def inner_comment(f: Function, lines: list[str])` | The prose comment at the top of the function body, as one sentence. |
| `describe_function` |  | `tools/gen_code_docs.py:965` | `def describe_function(f: Function, lines: list[str])` | Implements the describe function helper. |
| `write_functions` |  | `tools/gen_code_docs.py:1239` | `def write_functions(functions: list[Function], out: Path)` | Writes the functions. |
| `write_symbols` |  | `tools/gen_code_docs.py:1299` | `def write_symbols(symbols: list[Symbol], out: Path)` | Writes the symbols. |
| `state_fields` |  | `tools/gen_code_docs.py:1324` | `def state_fields()` | Implements the state fields helper. |
| `write_state` |  | `tools/gen_code_docs.py:1339` | `def write_state(fields: list[tuple[str, str]], out: Path)` | Writes the state. |
| `write_parameters` |  | `tools/gen_code_docs.py:1354` | `def write_parameters(out: Path)` | Writes the parameters. |
| `render_walk` |  | `tools/gen_code_docs.py:1382` | `def render_walk()` | Renders the walk. |
| `write_nodes` |  | `tools/gen_code_docs.py:1465` | `def write_nodes(out: Path)` | Writes the nodes. |
| `write_tests` |  | `tools/gen_code_docs.py:1498` | `def write_tests(out: Path)` | Writes the tests. |
| `main` |  | `tools/gen_code_docs.py:1541` | `def main()` | Runs the test or tool entry point. |
| `q15` |  | `tools/gen_math_tables.py:40` | `def q15(x: float)` | Implements the q 15 helper. |
| `q16` |  | `tools/gen_math_tables.py:45` | `def q16(x: float)` | Implements the q 16 helper. |
| `c_array` |  | `tools/gen_math_tables.py:50` | `def c_array(ctype: str, name: str, values: list[int], width: int)` | Implements the c array helper. |
| `rate_base` |  | `tools/gen_math_tables.py:64` | `def rate_base()` | Implements the rate base helper. |
| `rate_r3_base` |  | `tools/gen_math_tables.py:71` | `def rate_r3_base()` | Implements the rate r 3 base helper. |
| `keep_time` |  | `tools/gen_math_tables.py:78` | `def keep_time(coefficient: int, old_rate: float, new_rate: float)` | Transform a Q16 per-block coefficient to the new block period. |
| `filter_g_q16` |  | `tools/gen_math_tables.py:85` | `def filter_g_q16(rate: float)` | The per-stage VCF coefficient at each log cutoff position. |
| `build` |  | `tools/gen_math_tables.py:96` | `def build(rate: float)` | Implements the build helper. |
| `render` |  | `tools/gen_math_tables.py:126` | `def render(rate: float)` | Implements the render helper. |
| `main` |  | `tools/gen_math_tables.py:161` | `def main()` | Runs the test or tool entry point. |
| `q15` |  | `tools/gen_wavetable.py:38` | `def q15(x: float)` | Implements the q 15 helper. |
| `clamp15` |  | `tools/gen_wavetable.py:43` | `def clamp15(x: float)` | Round an already-Q15 value and clamp it into [-32768, 32767]. |
| `sin_q15` |  | `tools/gen_wavetable.py:53` | `def sin_q15(phase: int)` | Implements the sin q 15 helper. |
| `q15_mul` |  | `tools/gen_wavetable.py:60` | `def q15_mul(a: int, b: int)` | Implements the q 15 mul helper. |
| `sin_wave` |  | `tools/gen_wavetable.py:64` | `def sin_wave(phase: int, ws: int)` | The naive shape as a pure function of the top 10 phase bits. |
| `band_limited` |  | `tools/gen_wavetable.py:107` | `def band_limited(ws: int, harm: int)` | The band-limited Q15 table for waveform `ws`, harmonics 1..harm. |
| `c_array` |  | `tools/gen_wavetable.py:134` | `def c_array(name: str, ctype: str, values: list[int])` | Implements the c array helper. |
| `build` |  | `tools/gen_wavetable.py:148` | `def build()` | Implements the build helper. |
| `harmonics` |  | `tools/gen_wavetable.py:162` | `def harmonics(wave: list[int], base: list[int])` | Implements the harmonics helper. |
| `render_harmonics` |  | `tools/gen_wavetable.py:181` | `def render_harmonics(rows: list[list[int]])` | Renders the harmonics. |
| `render` |  | `tools/gen_wavetable.py:200` | `def render(wave: list[int], base: list[int])` | Implements the render helper. |
| `main` |  | `tools/gen_wavetable.py:223` | `def main()` | Runs the test or tool entry point. |

## Details by file


### `engine/gen/type_p_alpha_dsp.c`

- `note_to_inc` at line 1680 through 1682: `static inline __attribute__((always_inline)) uint32_t note_to_inc(uint32_t note)`
  Converts a MIDI note to a Q32 phase increment.
- `type_p_alpha_param_slot` at line 5860 through 5865: `static inline __attribute__((always_inline)) int32_t * type_p_alpha_param_slot(type_p_alpha_state *s, uint32_t id)`
  Maps a manifest id to its state slot.
- `type_p_alpha_dsp_state_init` at line 5867 through 5880: `P1_DSP_API void type_p_alpha_dsp_state_init(type_p_alpha_state *s)`
  Initialises the state and applies the factory init table.
- `voice_mono_active` at line 7548 through 7559: `static inline __attribute__((always_inline)) uint32_t voice_mono_active( uint32_t p, int32_t priority)`
  Selects the active note under the mono priority rule.
- `voice_highest_note` at line 7561 through 7566: `static inline __attribute__((always_inline)) uint32_t voice_highest_note(void)`
  Returns the highest held note across the voice pool.
- `voice_highest_velocity` at line 7568 through 7573: `static inline __attribute__((always_inline)) uint32_t voice_highest_velocity(void)`
  Returns the highest held velocity across the voice pool.
- `type_p_alpha_note_derived` at line 7575 through 7581: `static inline __attribute__((always_inline)) void type_p_alpha_note_derived(uint32_t v)`
  Derives the four operator increments from the voice pitch and bend.
- `type_p_alpha_taps_clear` at line 7583 through 7588: `static inline __attribute__((always_inline)) void type_p_alpha_taps_clear( int32_t (*taps)[DSP_TAP_FRAMES])`
  Clears the ten tap buffers before a render block.
- `type_p_alpha_render_pre_block` at line 7591 through 7698: `P1_DSP_API void type_p_alpha_render_pre_block(void)`
  Runs the control-rate gate, pan, delay and portamento work.
- `type_p_alpha_render_voices_block` at line 7700 through 14592: `P1_DSP_API void type_p_alpha_render_voices_block(int32_t *bus, int32_t *bus_r, int32_t (*taps)[DSP_TAP_FRAMES], uint32_t v0, uint32_t v1, uint32_t b, uint32_t frames)`
  Runs the per-voice node graph and the audio render.
- `type_p_alpha_render_post_block` at line 14594 through 14610: `P1_DSP_API void type_p_alpha_render_post_block(int32_t *out, const int32_t *src_l, const int32_t *src_r, uint32_t b, uint32_t frames)`
  Applies the DC blocker and writes interleaved output.
- `type_p_alpha_dsp_render_s` at line 14612 through 14620: `P1_DSP_API void type_p_alpha_dsp_render_s(type_p_alpha_state *state, int32_t *out, uint32_t frames)`
  Drives 32-sample blocks through pre, voices and post.
- `type_p_alpha_dsp_reset_s` at line 14622 through 14631: `P1_DSP_API void type_p_alpha_dsp_reset_s(type_p_alpha_state *state)`
  Clears the voice, delay and mono state.
- `type_p_alpha_dsp_note_on_ch_s` at line 14633 through 14743: `P1_DSP_API void type_p_alpha_dsp_note_on_ch_s(type_p_alpha_state *state, uint32_t note, uint32_t velocity, uint32_t channel)`
  Routes a note to the matching parts and allocates poly or mono voices.
- `type_p_alpha_dsp_note_on_s` at line 14745 through 14748: `P1_DSP_API void type_p_alpha_dsp_note_on_s(type_p_alpha_state *state, uint32_t note, uint32_t velocity)`
  Omni wrapper for the note-on path.
- `type_p_alpha_dsp_note_off_ch_s` at line 14750 through 14787: `P1_DSP_API void type_p_alpha_dsp_note_off_ch_s(type_p_alpha_state *state, uint32_t note, uint32_t channel)`
  Releases matching voices and applies the mono fallback.
- `type_p_alpha_dsp_note_off_s` at line 14789 through 14791: `P1_DSP_API void type_p_alpha_dsp_note_off_s(type_p_alpha_state *state, uint32_t note)`
  Omni wrapper for the note-off path.
- `type_p_alpha_dsp_all_notes_off_ch_s` at line 14793 through 14806: `P1_DSP_API void type_p_alpha_dsp_all_notes_off_ch_s(type_p_alpha_state *state, uint32_t channel)`
  Releases or kills the voices on one channel.
- `type_p_alpha_dsp_all_notes_off_s` at line 14808 through 14810: `P1_DSP_API void type_p_alpha_dsp_all_notes_off_s(type_p_alpha_state *state)`
  Global panic wrapper.
- `type_p_alpha_dsp_set_bend_ch_q16_s` at line 14812 through 14826: `P1_DSP_API void type_p_alpha_dsp_set_bend_ch_q16_s(type_p_alpha_state *state, uint32_t bend, uint32_t channel)`
  Converts a 14-bit bend to the part ratio and retunes held voices.
- `type_p_alpha_dsp_set_bend_q16_s` at line 14828 through 14830: `P1_DSP_API void type_p_alpha_dsp_set_bend_q16_s(type_p_alpha_state *state, uint32_t bend)`
  Omni wrapper for the bend path.
- `type_p_alpha_dsp_lit_s` at line 14832 through 14839: `P1_DSP_API uint32_t type_p_alpha_dsp_lit_s(type_p_alpha_state *state, uint32_t base)`
  Returns the 27-note activity mask for the keyboard display.
- `type_p_alpha_dsp_quiet_s` at line 14841 through 14845: `P1_DSP_API int type_p_alpha_dsp_quiet_s(type_p_alpha_state *state)`
  Reports whether any voice gate is open.
- `type_p_alpha_dsp_set_param_s` at line 14847 through 14853: `P1_DSP_API int type_p_alpha_dsp_set_param_s(type_p_alpha_state *state, uint32_t id, int32_t value)`
  Clamps and writes one parameter through the slot table.
- `type_p_alpha_dsp_get_param_s` at line 14855 through 14857: `P1_DSP_API int32_t type_p_alpha_dsp_get_param_s(type_p_alpha_state *state, uint32_t id)`
  Reads one parameter through the slot table.
- `type_p_alpha_dsp_param_count` at line 14859 through 14859: `P1_DSP_API uint32_t type_p_alpha_dsp_param_count(void)`
  Returns the manifest parameter count.
- `type_p_alpha_dsp_tap_s` at line 14861 through 14864: `P1_DSP_API const int32_t *type_p_alpha_dsp_tap_s(type_p_alpha_state *state, uint32_t ch)`
  Returns one tap buffer by channel.

### `engine/hal/p1_math.h`

- `p1_q15_mul` at line 22 through 25: `P1_MATH_INLINE int32_t p1_q15_mul(int32_t a, int32_t b)`
  Q15 x Q15 -> Q15, rounded. The per-sample multiply for Q15 signals/levels.
- `p1_q15_mul32` at line 31 through 34: `P1_MATH_INLINE int32_t p1_q15_mul32(int32_t a, int32_t b)`
  Q15 x Q15 -> Q15, rounded, for operands already known to fit the Q15 range (-32768..32768, including the 32768 unity gain). The product is below 2^30, so a 32-bit multiply is exact and avoids the 64-bit pair MAC. The caller owns the range proof.
- `p1_q16_mul` at line 37 through 40: `P1_MATH_INLINE int32_t p1_q16_mul(int32_t a, int32_t b)`
  Q16 x Q16 -> Q16, rounded (gains, coefficients).
- `p1_q31_mul` at line 43 through 46: `P1_MATH_INLINE int32_t p1_q31_mul(int32_t a, int32_t b)`
  Q31 x Q31 -> Q31, rounded.
- `p1_soft_clip` at line 52 through 60: `P1_MATH_INLINE int32_t p1_soft_clip(int32_t x)`
  The filter input's soft knee: y = x - x^5/8, with x clamped at full scale. The fifth-order term is 0.2 percent at -12 dBFS, 2.5 percent at -3 dBFS and 12.5 percent at 0 dBFS, so a normal level stays clean and a hot mixer compresses like the VCF's input pair.
- `p1_shr_round` at line 63 through 66: `P1_MATH_INLINE int32_t p1_shr_round(int32_t x, int shift)`
  Rounding right shift (the SDK's PSHR).
- `p1_sat_add32` at line 69 through 75: `P1_MATH_INLINE int32_t p1_sat_add32(int32_t a, int32_t b)`
  Saturating 32-bit add/sub (accumulation never wraps).
- `p1_sat_sub32` at line 77 through 83: `P1_MATH_INLINE int32_t p1_sat_sub32(int32_t a, int32_t b)`
  Returns the sat sub 32 value.
- `p1_madd_sat` at line 86 through 90: `P1_MATH_INLINE int32_t p1_madd_sat(int32_t acc, int32_t a, int32_t b, int shift)`
  Saturating multiply-accumulate: acc += (a * b) >> shift.
- `p1_onepole` at line 97 through 110: `P1_MATH_INLINE int32_t p1_onepole(int32_t level, int32_t target, int32_t coeff)`
  A one-pole toward `target`: level += (target - level) * coeff / 65536. When the rounded step is 0 (a small coeff times a small remaining delta) the step is forced to ±1, so the envelope always converges to `target` instead of stalling just short of it. That stall is what shows up as "DECAY holds forever" at low rates.
- `p1_ease_in` at line 116 through 123: `P1_MATH_INLINE int32_t p1_ease_in(int32_t start, int32_t target, uint32_t progress)`
  Slow-start (ease-in) interpolation for the attack warm-up. `progress` runs 0..65536, and the squared progress makes the motion start slowly and accelerate toward the target, which is the convex "slow then abrupt" shape. At progress 32768 the value is a quarter of the way, not a half.
- `p1_fade_step` at line 130 through 145: `P1_MATH_INLINE uint32_t p1_fade_step(int32_t rate)`
  The linear step for an LFO's rate or depth fade, in Q31 per control tick. The 1..99 values use the envelope rate table's 99 percent time, so a fade value and an envelope rate value mean the same duration. 0 is off and 99 is instant. The level runs 0 to 2^31, and the 31-bit fraction keeps the ramp on the table time for the slow settings as well as the fast ones.
- `p1_sin` at line 151 through 157: `P1_MATH_INLINE int32_t p1_sin(uint32_t phase)`
  Full-cycle sine, Q15, from the quarter table (p1_sin_q15, midpoint-sampled). `phase` is Q32 (wrapping accumulator: inc = f * 2^32 / FS).
- `p1_cos` at line 159 through 159: `P1_MATH_INLINE int32_t p1_cos(uint32_t phase)`
  Returns the cos value.
- `p1_sin_wave` at line 175 through 179: `P1_MATH_INLINE int32_t p1_sin_wave(uint32_t phase, uint32_t ws)`
  The 14-waveform union of the OPZ (YM2414) and OPL2/OPL3 (YM3812) sets, from ymfm's `opz_registers` and `opl_registers_base` constructors (BSD-3-Clause, re-implemented in linear Q15), plus the rectified triangle and the half square. `phase` is Q32 (full cycle), and `ws` is 0..13. 0-7 are the OPZ set (sine, triangle, half sine, half triangle, double sine, double triangle, double half sine, double half triangle), 8 is the rectified sine, 9 is the rectified triangle, and 10-13 are the OPL-only shapes (pulse, half square, square, saw). A bad `ws` degrades to sine. Precomputed into gen/wavetable.h (`p1_wave`, tools/gen_wavetable.py): waves 0-9 are one 1024-sample table each, whilst 10 (pulse) / 11 (half square) / 12 (square) / 13 (saw) are nine band-limited tables each (harmonic counts 256..1) for the top-octave anti-aliasing. `p1_wave_base[ws]` is ws's mip-0 offset, and this helper reads the richest (mip-0) table. The oscillator node adds the `mip` selected from its note's increment.
- `p1_wave_mip` at line 187 through 195: `P1_MATH_INLINE uint32_t p1_wave_mip(uint32_t step)`
  The band-limited-table mip index for a carrier increment `step` (Q32, samples per cycle): the level whose top harmonic stays at or below Nyquist/2.25. The harmonic count per level is 2^(8-mip) (256..1). The threshold is the brightness setting: Nyquist/4 was the conservative choice, and Nyquist/2.25 recovers more of the top octave with less alias headroom. The shift loop keeps the block-rate code small, because this runs while the flash is busy.
- `p1_key_scale` at line 202 through 213: `P1_MATH_INLINE int32_t p1_key_scale(uint32_t note, uint32_t bp, int32_t lo, int32_t hi)`
  The bipolar key-follow factor (Q15): 0 at the breakpoint, ramping linearly to `hi` at note 127 (the top octave) or `lo` at note 0 (the lowest octave). `lo`/`hi` are signed percent (-100..+100): +N pushes the target toward its maximum, -N toward its minimum. `note` and `bp` are 0..127 MIDI notes.
- `p1_key_apply` at line 218 through 226: `P1_MATH_INLINE int32_t p1_key_apply(int32_t v, int32_t k, int32_t min, int32_t max)`
  Apply a bipolar key-follow factor `k` (Q15, p1_key_scale) to a target value `v` in [min, max]: +k pushes toward `max`, -k toward `min` (proportional to the remaining range, so the value clamps at the extreme).
- `p1_vel_scale` at line 234 through 237: `P1_MATH_INLINE int32_t p1_vel_scale(int32_t vel, int32_t amount)`
  The velocity follow factor (Q15): 0 at velocity 0, ramping linearly to `amount` percent of full Q15 at maximum velocity. `vel` is `voice_velocity[v]` (velocity × 258, so 127 → about 32766), which is already Q15-normalised, so dividing by 100 turns the signed percent `amount` into the same Q15 scale that `p1_key_scale` returns. The old divisor 3,276,700 made the factor 32767 times too small, so every velocity follow was inert.
- `p1_key_add` at line 241 through 247: `P1_MATH_INLINE int32_t p1_key_add(int32_t a, int32_t b)`
  Sum two bipolar Q15 push factors (key-follow + velocity-follow), clamped to the Q15 range so `p1_key_apply` never overshoots its target extreme.
- `p1_clamp` at line 250 through 255: `P1_MATH_INLINE int32_t p1_clamp(int32_t v, int32_t lo, int32_t hi)`
  Clamp an int32 to [lo, hi].
- `p1_clamp16` at line 258 through 263: `P1_MATH_INLINE int32_t p1_clamp16(int32_t v)`
  Clamp an int32 to the Q16 range [0, 65535] (the SVF cutoff coefficient).
- `p1_clamp16_max` at line 267 through 272: `P1_MATH_INLINE int32_t p1_clamp16_max(int32_t v, int32_t max)`
  Clamp an int32 to [0, max]. The ladder filter uses this for the one-pole coefficient, which is unstable in the feedback loop above its own ceiling.
- `p1_route4` at line 278 through 282: `P1_MATH_INLINE int32_t p1_route4(const int16_t *w, int32_t a, int32_t b, int32_t c, int32_t d)`
  4-input weighted sum against a routing-matrix row (a `const int16_t *` of four 0/1 Q15 weights): out = a*w0 + b*w1 + c*w2 + d*w3. The multiply stays, because the weights are runtime table data, so a select would be a data-dependent branch per term and measured slower than the straight-line MAC.
- `p1_filter_coeff_q16` at line 292 through 301: `P1_MATH_INLINE int32_t p1_filter_coeff_q16(int32_t pos)`
  The VCF's per-stage coefficient from the log cutoff position. The 257-entry table is the exact 48 kHz one-pole transform at every 1/256 of the 12-octave range. Linear interpolation between entries keeps the error below a cent. The top entry is clamped at 55296, the highest coefficient the feedback loop takes without a parasitic oscillation.
- `p1_tune_q16` at line 307 through 320: `P1_MATH_INLINE int32_t p1_tune_q16(int32_t octave, int32_t semi, int32_t cent)`
  2^(octave + semi/12 + cent/1200) in Q16: the note-derived tuning ratio (the naive oscillator's octave/semi/cent knobs). `octave` -3..3, `semi` -11..11, `cent` -100..100 (clamped, with the octave part an exact shift and the semitone and cent parts from the p1_semi_q16 / p1_cent_q16 tables).
- `p1_poly_blep` at line 333 through 349: `P1_MATH_INLINE int32_t p1_poly_blep(uint32_t p, uint32_t inc, uint32_t inv_inc)`
  The 2nd-order PolyBLEP residual (Tale / Martin Finke, "Making Audio Plugins Part 18"). Returns the Q15 correction for a unit step at phase 0: the value to layer on the naive waveform around its discontinuity. `p` is the Q32 phase position (0..2^32), `inc` the Q32 increment, `inv_inc` = 2^32/inc (note-on reciprocal) so u = p/inc is `(uint64)p * inv_inc` with no per-sample divide. saw:     out = 2t - 1 - blep(t) square:  out = (t < 0.5 ? 1 : -1) + blep(t) - blep((t + 0.5) mod 1) pulse:   the second blep moves to the pulse width: blep((t + 1 - pw) mod 1) triangle: the polyBLEP'd square through a leaky integrator (one extra state).

### `engine/hal/spectral.h`

- `p1_spectral_clamp16` at line 42 through 45: `static inline int16_t p1_spectral_clamp16(int32_t v)`
  Returns the spectral clamp 16 value.
- `p1_spectral_clamp99` at line 47 through 50: `static inline int32_t p1_spectral_clamp99(int32_t v)`
  Returns the spectral clamp 99 value.
- `p1_spectral_morph` at line 55 through 58: `static inline int32_t p1_spectral_morph(int32_t position, int32_t lfo)`
  The morph in the 0..99 panel scale. `position` is the absolute morph envelope (or the knob when the envelope is off), and the LFO is already in panel units because the lfo node's depth is the spec_lfo_depth parameter.
- `p1_spectral_table` at line 63 through 87: `static inline void p1_spectral_table(int16_t *out, const int32_t *levels, int firstHarmonic)`
  One spectrum, all seven mips, peak normalised to Q15. The levels are the sixty-four harmonic amplitudes in 0..99, starting at `firstHarmonic` (1 or 65). The range shifts every partial by the same offset.
- `p1_spectral_range_first` at line 89 through 93: `static inline int p1_spectral_range_first(int32_t range)`
  Returns the spectral range first value.
- `p1_spectral_hash` at line 95 through 102: `static inline uint16_t p1_spectral_hash(const int32_t *levels, int firstHarmonic)`
  Returns the spectral hash value.
- `p1_spectral_copy` at line 104 through 108: `static inline void p1_spectral_copy(int16_t *dst, const int16_t *src)`
  Runs the spectral copy step for the surrounding code.
- `p1_spectral_tick` at line 115 through 241: `static inline void p1_spectral_tick( int16_t *bank, const int32_t *waveform, const int32_t *range_a, const int32_t *range_b, const int32_t *a1, const int32_t *a2, const int32_t *a3, const int32_t *a4, const int32_t *a5, const int32_t *a6, const int32_t *a7, const int32_t *a8, const int32_t *a9, const int32_t *a10, const int32_t *a11, const int32_t *a12, const int32_t *a13, const int32_t *a14, const int32_t *a15, const int32_t *a16, const int32_t *a17, const int32_t *a18, const int32_t *a19, const int32_t *a20, const int32_t *a21, const int32_t *a22, const int32_t *a23, const int32_t *a24, const int32_t *a25, const int32_t *a26, const int32_t *a27, const int32_t *a28, const int32_t *a29, const int32_t *a30, const int32_t *a31, const int32_t *a32, const int32_t *a33, const int32_t *a34, const int32_t *a35, const int32_t *a36, const int32_t *a37, const int32_t *a38, const int32_t *a39, const int32_t *a40, const int32_t *a41, const int32_t *a42, const int32_t *a43, const int32_t *a44, const int32_t *a45, const int32_t *a46, const int32_t *a47, const int32_t *a48, const int32_t *a49, const int32_t *a50, const int32_t *a51, const int32_t *a52, const int32_t *a53, const int32_t *a54, const int32_t *a55, const int32_t *a56, const int32_t *a57, const int32_t *a58, const int32_t *a59, const int32_t *a60, const int32_t *a61, const int32_t *a62, const int32_t *a63, const int32_t *a64, const int32_t *b1, const int32_t *b2, const int32_t *b3, const int32_t *b4, const int32_t *b5, const int32_t *b6, const int32_t *b7, const int32_t *b8, const int32_t *b9, const int32_t *b10, const int32_t *b11, const int32_t *b12, const int32_t *b13, const int32_t *b14, const int32_t *b15, const int32_t *b16, const int32_t *b17, const int32_t *b18, const int32_t *b19, const int32_t *b20, const int32_t *b21, const int32_t *b22, const int32_t *b23, const int32_t *b24, const int32_t *b25, const int32_t *b26, const int32_t *b27, const int32_t *b28, const int32_t *b29, const int32_t *b30, const int32_t *b31, const int32_t *b32, const int32_t *b33, const int32_t *b34, const int32_t *b35, const int32_t *b36, const int32_t *b37, const int32_t *b38, const int32_t *b39, const int32_t *b40, const int32_t *b41, const int32_t *b42, const int32_t *b43, const int32_t *b44, const int32_t *b45, const int32_t *b46, const int32_t *b47, const int32_t *b48, const int32_t *b49, const int32_t *b50, const int32_t *b51, const int32_t *b52, const int32_t *b53, const int32_t *b54, const int32_t *b55, const int32_t *b56, const int32_t *b57, const int32_t *b58, const int32_t *b59, const int32_t *b60, const int32_t *b61, const int32_t *b62, const int32_t *b63, const int32_t *b64)`
  Generate or advance the bank for both parts. Called once per control block at the first voice. One spectrum changes at a time per part. The oscillator sets the used flag on every read, so a silent part commits its new table at once and an audible part crossfades over about 15 control ticks (256 units at 17 per tick, 480 samples, 10 ms).
- `p1_spectral_lerp` at line 247 through 255: `static inline int32_t p1_spectral_lerp(const int16_t *table, uint32_t phase)`
  The table is 256 samples, so the phase fraction is 24 bits. The 1024-sample wave table uses 22 bits. Using that shift here plays the table four times too high. The mip is the phasor's nine-level selector. Mips 0 to 2 keep all 64 harmonics. Mips 3 to 8 carry 32, 16, 8, 4, 2 and 1.
- `p1_spectral_blend` at line 257 through 260: `static inline int32_t p1_spectral_blend(int32_t a, int32_t b, int32_t weight)`
  Returns the spectral blend value.
- `p1_spectral_read` at line 263 through 293: `static inline int32_t p1_spectral_read(int16_t *bank, int part, uint32_t phase, uint32_t mip, int32_t morph)`
  Read the a and b tables at the mip and blend them with the morph.

### `engine/runtime/macro_apply.h`

- `p1_macro_velocity_role_offset` at line 31 through 91: `static inline int32_t p1_macro_velocity_role_offset( const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op, int32_t span, int32_t all, int32_t carrier, int32_t modulator)`
  Returns the macro velocity role offset value.
- `p1_macro_velocity_level` at line 93 through 105: `static inline int32_t p1_macro_velocity_level(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)`
  Returns the macro velocity level value.
- `p1_macro_velocity_feedback` at line 107 through 119: `static inline int32_t p1_macro_velocity_feedback(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)`
  Returns the macro velocity feedback value.
- `p1_macro_velocity_morph` at line 121 through 133: `static inline int32_t p1_macro_velocity_morph(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)`
  Returns the macro velocity morph value.
- `p1_macro_velocity_env_rate` at line 135 through 147: `static inline int32_t p1_macro_velocity_env_rate(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)`
  Returns the macro velocity env rate value.
- `p1_macro_velocity_lfo_rate_ratio` at line 154 through 173: `static inline uint32_t p1_macro_velocity_lfo_rate_ratio( const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op)`
  The LFO rate is a Q32 phase increment, and the panel range is logarithmic above the off stop, so the macro offset is a frequency ratio rather than a raw delta. The base of the exponent is the panel's 223 Hz top over its 0.01 Hz floor, the same ratio the parameter range uses. A zero offset returns unity exactly, which keeps a macro at 0 bit-exact.
- `p1_macro_velocity_lfo_depth` at line 175 through 187: `static inline int32_t p1_macro_velocity_lfo_depth(const type_p_alpha_state *state, uint32_t part, uint32_t voice, int32_t op, int32_t span)`
  Returns the macro velocity LFO depth value.

### `engine/runtime/presets.c`

- `wr32` at line 9 through 15: `static void wr32(uint8_t *p, uint32_t u)`
  Runs the wr 32 step for the surrounding code.
- `rd32` at line 17 through 20: `static uint32_t rd32(const uint8_t *p)`
  Returns the rd 32 value.
- `preset_crc32` at line 22 through 31: `uint32_t preset_crc32(const uint8_t *p, uint32_t n)`
  Computes the reflected CRC-32 over a record body.
- `preset_crc16` at line 33 through 43: `uint16_t preset_crc16(const uint8_t *p, uint32_t n)`
  Returns the preset CRC 16 value.
- `name_put` at line 45 through 53: `static void name_put(uint8_t *out, const char *name)`
  Copies or trims a fixed-width record name field.
- `seal` at line 55 through 58: `static void seal(uint8_t *out, uint32_t body)`
  Writes the record CRC into the trailing four bytes.
- `preset_part_pack` at line 60 through 69: `uint32_t preset_part_pack(uint8_t *out, const char *name, const int32_t *params, uint32_t n)`
  pack -> the record length, the name is truncated to PRESET_NAME_LEN-1 chars.
- `preset_multi_pack` at line 71 through 113: `uint32_t preset_multi_pack(uint8_t *out, const char *name, const uint8_t *part_slot, const int32_t *level, const int32_t *delay, const int32_t *channel, const int32_t *low, const int32_t *high, const int32_t *macro, const char *const *macro_name, uint32_t p)`
  Packs a record into its little-endian bytes and appends the CRC.
- `preset_settings_pack` at line 115 through 122: `uint32_t preset_settings_pack(uint8_t *out, int32_t base_channel, int32_t master_tune)`
  Packs a record into its little-endian bytes and appends the CRC.
- `preset_part_unpack` at line 124 through 138: `int preset_part_unpack(const uint8_t *raw, uint32_t n, char *name, int32_t *params)`
  unpack -> 1 and fills the outputs, 0 when the slot is empty or the CRC fails (the outputs are then left untouched).
- `preset_multi_unpack` at line 140 through 190: `int preset_multi_unpack(const uint8_t *raw, uint32_t p, int has_macro, char *name, uint8_t *part_slot, int32_t *level, int32_t *delay, int32_t *channel, int32_t *low, int32_t *high, int32_t *macro, char *macro_name)`
  Unpacks and CRC-checks a record.
- `preset_settings_unpack` at line 192 through 201: `int preset_settings_unpack(const uint8_t *raw, int32_t *base_channel, int32_t *master_tune)`
  Unpacks and CRC-checks a record.
- `bank_per` at line 207 through 207: `static uint32_t bank_per(uint32_t rec)`
  Returns the bank per value.
- `bank_slots` at line 210 through 214: `static uint32_t bank_slots(uint32_t count, uint32_t rec)`
  the number of storage slots a bank of `count` records of `rec` bytes spans
- `bank_save` at line 216 through 226: `static int bank_save(const uint8_t *bank, uint32_t count, uint32_t rec, uint32_t slot0)`
  Writes a bank or record through the host store.
- `bank_load` at line 228 through 238: `static int bank_load(uint8_t *bank, uint32_t count, uint32_t rec, uint32_t slot0)`
  Reads a bank, record or library file and migrates it.
- `preset_part_bank_save` at line 242 through 245: `int preset_part_bank_save(const uint8_t *bank, uint32_t n)`
  banks, over the platform's `slot_save`/`slot_load` services. `n` = the part-patch param count (PP_RECORD) and `p` = the multi-patch part count (MP_RECORD). The multi bank's `n` is carried too so its flash slot can follow the part bank.
- `preset_part_bank_load` at line 246 through 249: `int preset_part_bank_load(uint8_t *bank, uint32_t n)`
  Reads a bank, record or library file and migrates it.
- `preset_multi_bank_save` at line 250 through 254: `int preset_multi_bank_save(const uint8_t *bank, uint32_t n, uint32_t p)`
  Writes a bank or record through the host store.
- `preset_multi_bank_load` at line 255 through 259: `int preset_multi_bank_load(uint8_t *bank, uint32_t n, uint32_t p)`
  Reads a bank, record or library file and migrates it.
- `preset_settings_save` at line 260 through 260: `int preset_settings_save(const uint8_t *rec)`
  Writes a bank or record through the host store.
- `preset_settings_load` at line 261 through 261: `int preset_settings_load(uint8_t *rec)`
  Reads a bank, record or library file and migrates it.

### `engine/runtime/presets.h`

- `preset_crc32` at line 46 through 46: `uint32_t preset_crc32(const uint8_t *p, uint32_t n)`
  Computes the reflected CRC-32 over a record body.
- `preset_crc16` at line 47 through 47: `uint16_t preset_crc16(const uint8_t *p, uint32_t n)`
  Returns the preset CRC 16 value.
- `preset_part_pack` at line 50 through 50: `uint32_t preset_part_pack(uint8_t *out, const char *name, const int32_t *params, uint32_t n)`
  pack -> the record length, the name is truncated to PRESET_NAME_LEN-1 chars.
- `preset_multi_pack` at line 51 through 54: `uint32_t preset_multi_pack(uint8_t *out, const char *name, const uint8_t *part_slot, const int32_t *level, const int32_t *delay, const int32_t *channel, const int32_t *low, const int32_t *high, const int32_t *macro, const char *const *macro_name, uint32_t p)`
  Packs a record into its little-endian bytes and appends the CRC.
- `preset_settings_pack` at line 55 through 55: `uint32_t preset_settings_pack(uint8_t *out, int32_t base_channel, int32_t master_tune)`
  Packs a record into its little-endian bytes and appends the CRC.
- `preset_part_unpack` at line 59 through 59: `int preset_part_unpack(const uint8_t *raw, uint32_t n, char *name, int32_t *params)`
  unpack -> 1 and fills the outputs, 0 when the slot is empty or the CRC fails (the outputs are then left untouched).
- `preset_multi_unpack` at line 60 through 63: `int preset_multi_unpack(const uint8_t *raw, uint32_t p, int has_macro, char *name, uint8_t *part_slot, int32_t *level, int32_t *delay, int32_t *channel, int32_t *low, int32_t *high, int32_t *macro, char *macro_name)`
  Unpacks and CRC-checks a record.
- `preset_settings_unpack` at line 64 through 64: `int preset_settings_unpack(const uint8_t *raw, int32_t *base_channel, int32_t *master_tune)`
  Unpacks and CRC-checks a record.
- `preset_part_bank_save` at line 69 through 69: `int preset_part_bank_save(const uint8_t *bank, uint32_t n)`
  banks, over the platform's `slot_save`/`slot_load` services. `n` = the part-patch param count (PP_RECORD) and `p` = the multi-patch part count (MP_RECORD). The multi bank's `n` is carried too so its flash slot can follow the part bank.
- `preset_part_bank_load` at line 70 through 70: `int preset_part_bank_load(uint8_t *bank, uint32_t n)`
  Reads a bank, record or library file and migrates it.
- `preset_multi_bank_save` at line 71 through 71: `int preset_multi_bank_save(const uint8_t *bank, uint32_t n, uint32_t p)`
  Writes a bank or record through the host store.
- `preset_multi_bank_load` at line 72 through 72: `int preset_multi_bank_load(uint8_t *bank, uint32_t n, uint32_t p)`
  Reads a bank, record or library file and migrates it.
- `preset_settings_save` at line 73 through 73: `int preset_settings_save(const uint8_t *rec)`
  Writes a bank or record through the host store.
- `preset_settings_load` at line 74 through 74: `int preset_settings_load(uint8_t *rec)`
  Reads a bank, record or library file and migrates it.

### `engine/runtime/synth_midi.c`

- `midi_get` at line 9 through 9: `int midi_get(uint8_t *st, uint8_t *d1, uint8_t *d2)`
  The host hook that pops one raw MIDI message.
- `synth_midi_feed` at line 12 through 53: `void synth_midi_feed(synth_midi_ctx *ctx, uint8_t st, uint8_t d1, uint8_t d2)`
  Applies one MIDI message, including sustain and all-notes-off.
- `synth_midi_poll` at line 55 through 60: `void synth_midi_poll(synth_midi_ctx *ctx)`
  Drains the host MIDI queue into the handler.

### `engine/runtime/synth_midi.h`

- `note_on` at line 21 through 21: `void (*note_on)(uint32_t note, uint32_t velocity, uint32_t channel)`
  Runs the note on step for the surrounding code.
- `note_off` at line 22 through 22: `void (*note_off)(uint32_t note, uint32_t channel)`
  Runs the note off step for the surrounding code.
- `all_off` at line 23 through 23: `void (*all_off)(void)`
  Runs the all off step for the surrounding code.
- `all_off_ch` at line 24 through 24: `void (*all_off_ch)(uint32_t channel)`
  Runs the all off ch step for the surrounding code.
- `set_param` at line 25 through 25: `int (*set_param)(uint32_t id, int32_t value)`
  Writes the param.
- `set_bend` at line 26 through 26: `void (*set_bend)(uint32_t bend14, uint32_t channel)`
  Writes the bend.

### `manual/build.py`

- `strip_toc` at line 49 through 56: `def strip_toc(toc: str)`
  Drop Python-Markdown's outer TOC div, because the template owns the nav.
- `find_browser` at line 59 through 60: `def find_browser()`
  Finds the browser.
- `main` at line 63 through 141: `def main()`
  Runs the test or tool entry point.

### `manual/figures.py`

- `write` at line 37 through 45: `def write(out: Path)`
  Write the figures under `out/figures`.

### `plugin/engine/Engine.cpp`

- `Engine` at line 34 through 37: `Engine::Engine() : state_(std::make_unique<P1_STATE>())`
  Returns the engine value.
- `~Engine` at line 39 through 39: `Engine::~Engine() = default`
  Destroys the object.
- `reset` at line 41 through 44: `void Engine::reset()`
  Clears the resampler ring and counters.
- `resetPhaseMaps` at line 46 through 60: `void Engine::resetPhaseMaps()`
  Clears the phase-map, LFO and envelope pools.
- `setParam` at line 62 through 65: `int Engine::setParam(uint32_t id, int32_t value)`
  Writes the param.
- `getParam` at line 67 through 70: `int32_t Engine::getParam(uint32_t id)`
  Reads the param.
- `noteOn` at line 72 through 75: `void Engine::noteOn(uint32_t note, uint32_t velocity, uint32_t channel)`
  Returns the note on value.
- `noteOff` at line 77 through 80: `void Engine::noteOff(uint32_t note, uint32_t channel)`
  Returns the note off value.
- `allNotesOff` at line 82 through 85: `void Engine::allNotesOff(uint32_t channel)`
  Releases the notes on one channel, or every channel for the panic.
- `setBend` at line 87 through 90: `void Engine::setBend(uint32_t bend14, uint32_t channel)`
  Writes the bend.
- `render` at line 92 through 95: `void Engine::render(int32_t* out, uint32_t frames)`
  Returns the render value.
- `lit` at line 97 through 100: `uint32_t Engine::lit(uint32_t base)`
  Returns the lit value.
- `quiet` at line 102 through 105: `bool Engine::quiet()`
  Tests the quiet condition.
- `tap` at line 107 through 110: `const int32_t* Engine::tap(uint32_t channel)`
  Returns the tap value.
- `phaseMapSnapshot` at line 112 through 212: `bool Engine::phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const`
  Tests the phase map snapshot condition.
- `specMorph` at line 214 through 244: `int Engine::specMorph(int op, int part) const`
  Reads the live spectral morph for the editor.
- `state` at line 246 through 249: `P1_STATE* Engine::state()`
  Returns the APVTS.

### `plugin/engine/Engine.hpp`

- `Engine` at line 38 through 38: `Engine(const Engine&) = delete`
  Constructs the object.
- `operator=` at line 39 through 39: `Engine& operator=(const Engine&) = delete`
  Returns the operator value.
- `reset` at line 41 through 41: `void reset()`
  Clears the resampler ring and counters.
- `setParam` at line 42 through 42: `int setParam(uint32_t id, int32_t value)`
  Writes the param.
- `getParam` at line 43 through 43: `int32_t getParam(uint32_t id)`
  Reads the param.
- `noteOn` at line 44 through 44: `void noteOn(uint32_t note, uint32_t velocity, uint32_t channel = 0xFFFFFFFFu)`
  Runs the note on step for the surrounding code.
- `noteOff` at line 45 through 45: `void noteOff(uint32_t note, uint32_t channel = 0xFFFFFFFFu)`
  Runs the note off step for the surrounding code.
- `allNotesOff` at line 47 through 47: `void allNotesOff(uint32_t channel = 0xFFFFFFFFu)`
  Releases the notes on one channel, or every channel for the panic.
- `setBend` at line 48 through 48: `void setBend(uint32_t bend14, uint32_t channel = 0xFFFFFFFFu)`
  Writes the bend.
- `render` at line 49 through 49: `void render(int32_t* out, uint32_t frames)`
  Runs the render step for the surrounding code.
- `lit` at line 50 through 50: `uint32_t lit(uint32_t base)`
  Returns the lit value.
- `quiet` at line 51 through 51: `bool quiet()`
  Tests the quiet condition.
- `tap` at line 52 through 52: `const int32_t* tap(uint32_t channel)`
  Returns the tap value.
- `specMorph` at line 55 through 55: `int specMorph(int op, int part) const`
  The last morph the oscillator read for an operator and part, 0 to 99, or -1 when the operator has not read the bank since the state reset.
- `phaseMapSnapshot` at line 56 through 56: `bool phaseMapSnapshot(int op, int part, PhaseMapSnapshot& out) const`
  Tests the phase map snapshot condition.
- `resetPhaseMaps` at line 59 through 59: `void resetPhaseMaps()`
  Clear the per-part phase map, LFO and envelope pools so a patch load, or switching the map off, starts from the current controls.
- `state` at line 61 through 61: `P1_STATE* state()`
  Returns the generated state pointer.

### `plugin/engine/MidiBridge.cpp`

- `p1_bridge_note_on` at line 11 through 15: `void p1_bridge_note_on(uint32_t note, uint32_t velocity, uint32_t channel)`
  Runs the bridge note on step for the surrounding code.
- `p1_bridge_note_off` at line 17 through 21: `void p1_bridge_note_off(uint32_t note, uint32_t channel)`
  Runs the bridge note off step for the surrounding code.
- `p1_bridge_all_off` at line 23 through 27: `void p1_bridge_all_off()`
  Runs the bridge all off step for the surrounding code.
- `p1_bridge_all_off_ch` at line 29 through 33: `void p1_bridge_all_off_ch(uint32_t channel)`
  Runs the bridge all off ch step for the surrounding code.
- `p1_bridge_set_param` at line 35 through 38: `int p1_bridge_set_param(uint32_t id, int32_t value)`
  Returns the bridge set param value.
- `p1_bridge_set_bend` at line 40 through 44: `void p1_bridge_set_bend(uint32_t bend, uint32_t channel)`
  Runs the bridge set bend step for the surrounding code.
- `MidiBridge` at line 48 through 59: `MidiBridge::MidiBridge(Engine& engine) : engine_(engine)`
  Returns the MIDI bridge value.
- `feed` at line 61 through 66: `void MidiBridge::feed(uint8_t status, uint8_t data1, uint8_t data2)`
  Returns the feed value.
- `reset` at line 68 through 75: `void MidiBridge::reset()`
  Clears the resampler ring and counters.

### `plugin/engine/MidiBridge.hpp`

- `feed` at line 18 through 18: `void feed(uint8_t status, uint8_t data1, uint8_t data2)`
  Runs the feed step for the surrounding code.
- `reset` at line 19 through 19: `void reset()`
  Clears the resampler ring and counters.

### `plugin/engine/RateBridge.cpp`

- `besselI0` at line 14 through 26: `double besselI0(double x)`
  Returns the bessel i 0 value.
- `prepare` at line 30 through 71: `void RateBridge::prepare(double hostRate, int maxBlock)`
  Builds the resampler kernel and ring for the host rate.
- `reset` at line 73 through 78: `void RateBridge::reset()`
  Clears the resampler ring and counters.
- `pushCore` at line 80 through 90: `void RateBridge::pushCore(const int32_t* interleaved, int frames)`
  Writes interleaved core frames into the ring.
- `pull` at line 92 through 133: `void RateBridge::pull(float* left, float* right, int frames)`
  Reads host frames through the polyphase kernel.
- `renderChunk` at line 135 through 144: `void RateBridge::renderChunk(Engine& engine, int frames, CoreTapCallback onCore, void* context)`
  Renders the chunk.
- `process` at line 146 through 156: `void RateBridge::process(Engine& engine, float* left, float* right, int frames, CoreTapCallback onCore, void* context)`
  Renders core chunks and pulls the host-rate output.
- `latencySamples` at line 158 through 162: `int RateBridge::latencySamples() const`
  Returns the bridge group delay in host samples.
- `hasCore` at line 164 through 171: `bool RateBridge::hasCore(int hostFrames) const`
  Reports whether a host block will render at least one core chunk.

### `plugin/engine/RateBridge.hpp`

- `prepare` at line 19 through 19: `void prepare(double hostRate, int maxBlock)`
  Builds the resampler kernel and ring for the host rate.
- `reset` at line 20 through 20: `void reset()`
  Clears the resampler ring and counters.
- `process` at line 24 through 25: `void process(Engine& engine, float* left, float* right, int frames, CoreTapCallback onCore = nullptr, void* context = nullptr)`
  The engine path: render core chunks, then pull host frames. The callback runs after each core chunk, before the tap state is overwritten.
- `pushCore` at line 29 through 29: `void pushCore(const int32_t* interleaved, int frames)`
  Test and offline path that pushes interleaved int32 core frames and pulls host frames. The ring keeps the history the 128-tap kernel needs.
- `pull` at line 32 through 32: `void pull(float* left, float* right, int frames)`
  Reads host frames through the kernel. A null `right` folds the pair to mono in `left`.
- `latencySamples` at line 34 through 34: `int latencySamples() const`
  Returns the bridge group delay in host samples.
- `ratio` at line 35 through 35: `double ratio() const`
  Returns the ratio value.
- `hasCore` at line 39 through 39: `bool hasCore(int hostFrames) const`
  True when a `process` call of `hostFrames` will render at least one core chunk, so the caller can feed the first chunk's events before it runs.
- `renderChunk` at line 45 through 46: `void renderChunk(Engine& engine, int frames, CoreTapCallback onCore, void* context)`
  Renders the chunk.

### `plugin/juce/ChunkNoteSet.h`

- `reset` at line 17 through 17: `void reset()`
  Clears the resampler ring and counters.
- `noteOn` at line 19 through 24: `void noteOn(int channel, int note)`
  Runs the note on step for the surrounding code.
- `held` at line 26 through 31: `bool held(int channel, int note) const`
  Tests the held condition.

### `plugin/juce/MacroLaw.h`

- `macroRowIncludes` at line 13 through 34: `inline bool macroRowIncludes(int row, int index, int algorithm)`
  Whether a target row names one parameter. A carrier or modulator group only names its own role for the current algorithm, and a row that names no member returns false. The index is the manifest index, which is the member id.
- `macroEffective` at line 40 through 45: `inline double macroEffective(int knob, int sourceValue, int range)`
  The macro source law. The knob, or the host automation on the same parameter, is the centre. The selected source is 0 to 127 and the range is in macro points, so a source at rest leaves the macro at the knob and a rising source adds up to its range. The result is clamped to the macro's own -100 to 100.
- `macroOffset` at line 51 through 54: `inline double macroOffset(double macro, int range)`
  The macro destination law. The offset is a fraction of the destination's span, so a macro at 100 with a range of 100 moves the whole span and half of each moves half. The destination is clamped in the normalised domain, which keeps a log parameter's offset musical instead of raw.
- `macroTargetNormalised` at line 56 through 59: `inline double macroTargetNormalised(double base, double offset)`
  Returns the macro target normalised value.

### `plugin/juce/Pages.cpp`

- `ModeButton` at line 17 through 20: `explicit ModeButton(const juce::String& label) : juce::Button(label)`
  Constructs the object.
- `setActive` at line 22 through 28: `void setActive(bool active)`
  Sets the active flag.
- `paintButton` at line 30 through 44: `void paintButton(juce::Graphics& g, bool highlighted, bool) override`
  Draws the button in its normal, highlighted and down states.
- `PartCard` at line 52 through 87: `PartCard(PluginEditor& editor, int part, bool active) : editor_(editor), part_(part), active_(active), header_(true, true, [this] { editor_.showRenameDialog(true, part_); }, [this] { editor_.copyPart(part_); }, [this] { editor_.pastePart(part_); }, [this] { return getParam(editor_, params::level(part_)) <= 0.0; }, [this](bool muted) { editor_.togglePartMute(part_, muted); })`
  Constructs the object.
- `resized` at line 89 through 105: `void resized() override`
  Lays out the child components.
- `paint` at line 107 through 120: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 122 through 130: `void tick()`
  Refreshes the live display state.
- `VoiceMode` at line 147 through 174: `explicit VoiceMode(PluginEditor& editor) : editor_(editor), part_(editor.activePart())`
  Constructs the object.
- `resized` at line 176 through 189: `void resized() override`
  Lays out the child components.
- `paint` at line 191 through 198: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 200 through 219: `void tick()`
  Refreshes the live display state.
- `PatchPage` at line 233 through 242: `explicit PatchPage(PluginEditor& editor)`
  Constructs the object.
- `resized` at line 244 through 254: `void resized() override`
  Lays out the child components.
- `tick` at line 256 through 261: `void tick() override`
  Refreshes the live display state.
- `makePatchPage` at line 270 through 273: `std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor)`
  Creates the patch page.

### `plugin/juce/Pages.h`

- `setParam` at line 11 through 16: `inline void setParam(PluginEditor& editor, const juce::String& id, double value)`
  Writes the param.
- `getParam` at line 18 through 23: `inline double getParam(PluginEditor& editor, const juce::String& id)`
  Reads the param.
- `ensureSpectrumMode` at line 28 through 37: `inline void ensureSpectrumMode(PluginEditor& editor, int op, int part, const juce::String& valueID)`
  A depth edit or an envelope switch that ends away from zero puts the operator in spectrum mode, so the envelope or LFO the player just set has something to move.
- `InlineMuteSwitch` at line 41 through 46: `InlineMuteSwitch(std::function<bool()> getMuted, std::function<void(bool)> setMuted) : getMuted_(std::move(getMuted)), setMuted_(std::move(setMuted))`
  Constructs the object.
- `paint` at line 48 through 66: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseUp` at line 68 through 73: `void mouseUp(const juce::MouseEvent&) override`
  Handles a mouse interaction.
- `CardHeader` at line 82 through 100: `CardHeader(bool partLayout, bool withRename, std::function<void()> onRename, std::function<void()> onCopy, std::function<void()> onPaste, std::function<bool()> getMuted, std::function<void(bool)> setMuted, bool wideHeader = false) : partLayout_(partLayout), withRename_(withRename), wideHeader_(wideHeader)`
  Constructs the object.
- `resized` at line 102 through 124: `void resized() override`
  Lays out the child components.
- `tick` at line 126 through 126: `void tick()`
  Refreshes the live display state.
- `OscillatorCard` at line 140 through 169: `OscillatorCard(PluginEditor& editor, int op, int part, bool withMorph = false) : P1Card("oscillator", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::oscillator); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::oscillator); }), ratio_(editor.state(), params::opRatio(op, part), "ratio", 28.0f), detune_(editor.state(), params::opDetune(op, part), "detune", 28.0f), level_(editor.state(), params::opLevel(op, part), "level", 28.0f), feedback_(editor.state(), params::opFeedback(op, part), "feedback", 28.0f), wave_(editor.state(), params::opWaveform(op, part), params::waveNames())`
  Constructs the object.
- `resized` at line 171 through 187: `void resized() override`
  Lays out the child components.
- `AmplitudeEnvelopeCard` at line 200 through 239: `AmplitudeEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("amp env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::amplitudeEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::amplitudeEnvelope); }), envView_(editor.state(), {params::opEnv(op, "warm", part), params::opEnv(op, "surge", part), params::opEnv(op, "d1r", part), params::opEnv(op, "d2r", part), params::opEnv(op, "rel", part)}, {params::opEnv(op, "d1l", part), params::opEnv(op, "d2l", part)}, colour(tokens::purple), 0.0, 99.0, false, EnvelopeShape::operatorAmp, params::opEnv(op, "mode", part)), mode_(editor.state(), params::opEnv(op, "mode", part), {"sustain off", "sustain on", "loop 1", "loop 2"})`
  Constructs the object.
- `resized` at line 241 through 262: `void resized() override`
  Lays out the child components.
- `tick` at line 264 through 264: `void tick()`
  Refreshes the live display state.
- `FeedbackEnvelopeCard` at line 274 through 325: `FeedbackEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("fb env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::feedbackEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::feedbackEnvelope); }), envView_(editor.state(), {params::opFeedbackRate(op, 0, part), params::opFeedbackRate(op, 1, part), params::opFeedbackRate(op, 2, part), params::opFeedbackRelRate(op, part)}, {params::opFeedbackLevel(op, 0, part), params::opFeedbackLevel(op, 1, part), params::opFeedbackLevel(op, 2, part), params::opFeedbackRelLevel(op, part)}, colour(tokens::purple), -32768.0, 32767.0, true, EnvelopeShape::bipolar, params::opFeedbackMode(op, part)), on_(editor.state(), params::opFeedbackOn(op, part), "on"), mode_(editor.state(), params::opFeedbackMode(op, part), {"sustain off", "sustain on", "loop 1", "loop 2"})`
  Constructs the object.
- `resized` at line 327 through 347: `void resized() override`
  Lays out the child components.
- `tick` at line 349 through 349: `void tick()`
  Refreshes the live display state.
- `LfoCard` at line 361 through 390: `LfoCard(PluginEditor& editor, juce::String title, juce::String rateID, juce::String depthID, juce::String waveID, juce::String modeID, juce::String rateFadeID, juce::String depthFadeID, int op, int part, PluginEditor::OperatorSection section, std::function<void()> onDepthEdit = {}, bool compact = false, juce::String modeLabel = "bipolar") : P1Card(std::move(title), [&ed = editor, op, part, section] { ed.copyOperatorSection(op, part, section); }, [&ed = editor, op, part, section] { ed.pasteOperatorSection(op, part, section); }), rate_(editor.state(), rateID, "rate", 28.0f), depth_(editor.state(), depthID, "depth", 28.0f), rateFade_(editor.state(), rateFadeID, "rate fade", compact ? 22.0f : 28.0f), depthFade_(editor.state(), depthFadeID, "depth fade", compact ? 22.0f : 28.0f), wave_(editor.state(), std::move(waveID), params::lfoWaveNames()), mode_(editor.state(), std::move(modeID), std::move(modeLabel))`
  Constructs the object.
- `resized` at line 392 through 407: `void resized() override`
  Lays out the child components.
- `makePatchPage` at line 419 through 419: `std::unique_ptr<PageBase> makePatchPage(PluginEditor& editor)`
  Creates the patch page.
- `makeMacrosPage` at line 420 through 420: `std::unique_ptr<PageBase> makeMacrosPage(PluginEditor& editor)`
  Creates the macros page.
- `makeOperatorsPage` at line 421 through 421: `std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor)`
  Creates the operators page.
- `makeFxModPage` at line 422 through 422: `std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor)`
  Creates the FX mod page.
- `makeOperatorLfosPage` at line 423 through 423: `std::unique_ptr<PageBase> makeOperatorLfosPage(PluginEditor& editor)`
  Creates the operator LFOs page.
- `makePhaseMapPage` at line 424 through 424: `std::unique_ptr<PageBase> makePhaseMapPage(PluginEditor& editor)`
  Creates the phase map page.
- `makeOperatorSpectraPage` at line 425 through 425: `std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor, bool a)`
  Creates the operator spectra page.
- `makeOperatorSpectraModPage` at line 426 through 426: `std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)`
  Creates the operator spectra mod page.

### `plugin/juce/PagesFxMod.cpp`

- `FilterCard` at line 14 through 32: `FilterCard(PluginEditor& editor, juce::String title, juce::String cutoffID, juce::String resID, juce::String typeID, juce::String onID, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card(std::move(title)), cutoff_(editor.state(), std::move(cutoffID), "cutoff", 42.0f), res_(editor.state(), std::move(resID), "res", 42.0f), type_(editor.state(), std::move(typeID), {"lpf", "bpf", "hpf"}), on_(editor.state(), std::move(onID), "on"), to_(std::move(toLabel), true)`
  Constructs the object.
- `resized` at line 34 through 45: `void resized() override`
  Lays out the child components.
- `paint` at line 47 through 53: `void paint(juce::Graphics& g) override`
  Draws the component.
- `FilterEnvelopeCard` at line 65 through 109: `FilterEnvelopeCard(PluginEditor& editor, int part, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("filter envelope"), part_(part), envView_(editor.state(), {params::filterRate(0, part), params::filterRate(1, part), params::filterRate(2, part), params::filterEnvRelRate(part)}, {params::filterLevel(0, part), params::filterLevel(1, part), params::filterLevel(2, part), params::filterEnvRelLevel(part)}, colour(tokens::green), 0.0, 99.0, false, EnvelopeShape::bipolar, params::filterEnvMode(part), params::filterCutoff(part), 99.0 / 55296.0), on_(editor.state(), params::filterEnvOn(part), "on"), mode_(editor.state(), params::filterEnvMode(part), {"sustain off", "sustain on", "loop 1", "loop 2"}), to_(std::move(toLabel), true)`
  Constructs the object.
- `resized` at line 111 through 130: `void resized() override`
  Lays out the child components.
- `tick` at line 132 through 132: `void tick()`
  Refreshes the live display state.
- `PitchLfoCard` at line 146 through 171: `explicit PitchLfoCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch lfo"), part_(editor.activePart()), waveView_(editor.state(), params::pitchWave(part_), colour(tokens::green)), rate_(editor.state(), params::pitchRate(part_), "rate", 36.0f), depth_(editor.state(), params::pitchDepth(part_), "depth", 36.0f), rateFade_(editor.state(), params::pitchRateFade(part_), "rate fade", 36.0f), depthFade_(editor.state(), params::pitchDepthFade(part_), "depth fade", 36.0f), wave_(editor.state(), params::pitchWave(part_), params::lfoWaveNames()), mode_(editor.state(), params::pitchMode(part_), "bipolar"), to_(std::move(toLabel), true)`
  Constructs the object.
- `resized` at line 173 through 189: `void resized() override`
  Lays out the child components.
- `tick` at line 191 through 191: `void tick()`
  Refreshes the live display state.
- `PitchEnvelopeCard` at line 207 through 250: `explicit PitchEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pitch envelope"), part_(editor.activePart()), envView_(editor.state(), {params::pitchEnvRate(0, part_), params::pitchEnvRate(1, part_), params::pitchEnvRate(2, part_), params::pitchEnvRelRate(part_)}, {params::pitchEnvLevel(0, part_), params::pitchEnvLevel(1, part_), params::pitchEnvLevel(2, part_), params::pitchEnvRelLevel(part_)}, colour(tokens::purple), -32768.0, 32767.0, true, EnvelopeShape::bipolar, params::pitchEnvMode(part_)), on_(editor.state(), params::pitchEnvOn(part_), "on"), mode_(editor.state(), params::pitchEnvMode(part_), {"sustain off", "sustain on", "loop 1", "loop 2"}), to_(std::move(toLabel), true)`
  Constructs the object.
- `resized` at line 252 through 271: `void resized() override`
  Lays out the child components.
- `tick` at line 273 through 273: `void tick()`
  Refreshes the live display state.
- `PanEnvelopeCard` at line 287 through 328: `explicit PanEnvelopeCard(PluginEditor& editor, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card("pan envelope"), part_(editor.activePart()), envView_(editor.state(), {params::panEnvRate(0, part_), params::panEnvRate(1, part_), params::panEnvRate(2, part_), params::panEnvRelRate(part_)}, {params::panEnvLevel(0, part_), params::panEnvLevel(1, part_), params::panEnvLevel(2, part_), params::panEnvRelLevel(part_)}, colour(tokens::purple), -99.0, 99.0, true, EnvelopeShape::bipolar, params::panEnvMode(part_), params::pan(part_)), on_(editor.state(), params::panEnvOn(part_), "on"), mode_(editor.state(), params::panEnvMode(part_), {"sustain off", "sustain on", "loop 1", "loop 2"}), to_(std::move(toLabel), true)`
  Constructs the object.
- `resized` at line 330 through 349: `void resized() override`
  Lays out the child components.
- `tick` at line 351 through 351: `void tick()`
  Refreshes the live display state.
- `FollowCard` at line 365 through 413: `FollowCard(PluginEditor& editor, bool key, juce::String title, std::function<void()> onToPart = {}, juce::String toLabel = "to 2") : P1Card(std::move(title)), editor_(editor), key_(key), part_(editor.activePart()), targets_(params::followTargets(part_)), on_(editor.state(), params::withPart(key ? "key_sens_on" : "vel_sens_on", part_), "on"), to_(std::move(toLabel), true)`
  Constructs the object.
- `resized` at line 415 through 430: `void resized() override`
  Lays out the child components.
- `paint` at line 432 through 443: `void paint(juce::Graphics& g) override`
  Draws the component.
- `refresh` at line 445 through 458: `void refresh()`
  Refreshes the display.
- `targetItems` at line 461 through 467: `juce::StringArray targetItems() const`
  Returns the target items value.
- `updateTarget` at line 469 through 476: `void updateTarget()`
  Updates the target.
- `wipe` at line 478 through 488: `void wipe()`
  Runs the wipe step for the surrounding code.
- `setPair` at line 507 through 512: `void setPair(int a, int b)`
  Writes the pair.
- `setDirect` at line 514 through 521: `void setDirect(bool a, bool b)`
  Writes the direct.
- `paint` at line 523 through 587: `void paint(juce::Graphics& g) override`
  Draws the component.
- `RingmodCard` at line 598 through 627: `RingmodCard(PluginEditor& editor, int part) : P1Card("ringmod"), editor_(editor), part_(part)`
  Constructs the object.
- `resized` at line 629 through 629: `void resized() override`
  Lays out the child components.
- `tick` at line 631 through 673: `void tick()`
  Refreshes the live display state.
- `carrierOutOn` at line 676 through 679: `bool carrierOutOn(int op) const`
  Tests the carrier out on condition.
- `isCarrier` at line 681 through 687: `bool isCarrier(int op) const`
  Tests whether the the carrier.
- `updateCarriers` at line 689 through 716: `void updateCarriers()`
  Updates the carriers.
- `layout` at line 718 through 745: `void layout()`
  Runs the layout step for the surrounding code.
- `FxModPage` at line 761 through 797: `explicit FxModPage(PluginEditor& editor)`
  Constructs the object.
- `resized` at line 799 through 817: `void resized() override`
  Lays out the child components.
- `tick` at line 819 through 828: `void tick() override`
  Refreshes the live display state.
- `makeFxModPage` at line 843 through 846: `std::unique_ptr<PageBase> makeFxModPage(PluginEditor& editor)`
  Creates the FX mod page.

### `plugin/juce/PagesMacros.cpp`

- `MacroRange` at line 18 through 31: `MacroRange(APVTS& state, juce::String paramID, juce::Colour fill) : state_(state), paramID_(std::move(paramID)), fill_(fill)`
  Constructs the object.
- `resized` at line 33 through 36: `void resized() override`
  Lays out the child components.
- `paint` at line 38 through 68: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseDown` at line 70 through 76: `void mouseDown(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseDrag` at line 78 through 78: `void mouseDrag(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseUp` at line 80 through 84: `void mouseUp(const juce::MouseEvent&) override`
  Handles a mouse interaction.
- `setFromMouse` at line 87 through 98: `void setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `MacroTargetChoice` at line 113 through 150: `MacroTargetChoice(APVTS& state, juce::String paramID) : state_(state), paramID_(std::move(paramID))`
  Constructs the object.
- `resized` at line 152 through 152: `void resized() override`
  Lays out the child components.
- `MacroCard` at line 164 through 191: `MacroCard(PluginEditor& editor, int index) : editor_(editor), index_(index), knob_(editor.state(), params::macroValue(index), "", 92.0f), source_(editor.state(), params::macroMidi(index), {"off", "cc " + juce::String(72 + index), "mod wheel", "velocity"}), midiRange_(editor.state(), params::macroMidiRange(index), colour(tokens::green)), rename_("rename", true)`
  Constructs the object.
- `resized` at line 193 through 224: `void resized() override`
  Lays out the child components.
- `paint` at line 226 through 244: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 246 through 271: `void tick()`
  Refreshes the live display state.
- `title` at line 274 through 278: `juce::String title() const`
  Returns the title value.
- `MacrosPage` at line 297 through 304: `explicit MacrosPage(PluginEditor& editor) : editor_(editor)`
  Constructs the object.
- `resized` at line 306 through 319: `void resized() override`
  Lays out the child components.
- `paint` at line 321 through 321: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 323 through 327: `void tick() override`
  Refreshes the live display state.
- `makeMacrosPage` at line 336 through 339: `std::unique_ptr<PageBase> makeMacrosPage(PluginEditor& editor)`
  Creates the macros page.

### `plugin/juce/PagesOperators.cpp`

- `isCarrier` at line 11 through 23: `bool isCarrier(int algorithm, int op)`
  Tests whether the the carrier.
- `OpColumn` at line 27 through 52: `OpColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), osc_(editor, op, part_), amp_(editor, op, part_), fb_(editor, op, part_)`
  Constructs the object.
- `resized` at line 54 through 69: `void resized() override`
  Lays out the child components.
- `paint` at line 71 through 80: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 82 through 92: `void tick()`
  Refreshes the live display state.
- `OpLfoColumn` at line 109 through 152: `OpLfoColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), ampLfo_(editor, "amp lfo", params::opTremRate(op, part_), params::opTremDepth(op, part_), params::opTremWave(op, part_), params::opTremMode(op, part_), params::opTremRateFade(op, part_), params::opTremDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::amplitudeLfo), feedbackLfo_(editor, "fb lfo", params::opFeedbackLfoRate(op, part_), params::opFeedbackLfoDepth(op, part_), params::opFeedbackLfoWave(op, part_), params::opFeedbackLfoMode(op, part_), params::opFeedbackLfoRateFade(op, part_), params::opFeedbackLfoDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::feedbackLfo), detuneLfo_(editor, "detune lfo", params::opDetuneLfoRate(op, part_), params::opDetuneLfoDepth(op, part_), params::opDetuneLfoWave(op, part_), params::opDetuneLfoMode(op, part_), params::opDetuneLfoRateFade(op, part_), params::opDetuneLfoDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::detuneLfo)`
  Constructs the object.
- `resized` at line 154 through 169: `void resized() override`
  Lays out the child components.
- `paint` at line 171 through 180: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 182 through 190: `void tick()`
  Refreshes the live display state.
- `OperatorsPage` at line 207 through 215: `explicit OperatorsPage(PluginEditor& editor) : editor_(editor)`
  Constructs the object.
- `resized` at line 217 through 225: `void resized() override`
  Lays out the child components.
- `tick` at line 227 through 231: `void tick() override`
  Refreshes the live display state.
- `OperatorLfosPage` at line 240 through 248: `explicit OperatorLfosPage(PluginEditor& editor) : editor_(editor)`
  Constructs the object.
- `resized` at line 250 through 258: `void resized() override`
  Lays out the child components.
- `tick` at line 260 through 264: `void tick() override`
  Refreshes the live display state.
- `makeOperatorsPage` at line 273 through 276: `std::unique_ptr<PageBase> makeOperatorsPage(PluginEditor& editor)`
  Creates the operators page.
- `makeOperatorLfosPage` at line 278 through 281: `std::unique_ptr<PageBase> makeOperatorLfosPage(PluginEditor& editor)`
  Creates the operator LFOs page.

### `plugin/juce/PagesPhase.cpp`

- `rawValue` at line 13 through 18: `float rawValue(APVTS& state, const juce::String& id)`
  Returns the raw value value.
- `isCarrier` at line 20 through 31: `bool isCarrier(int algorithm, int op)`
  Tests whether the the carrier.
- `PhaseMapView` at line 38 through 45: `PhaseMapView(PluginEditor& editor, int op, int part) : state_(editor.state()), processor_(editor.processor()), op_(op), part_(part)`
  Constructs the object.
- `tick` at line 47 through 102: `void tick()`
  Refreshes the live display state.
- `paint` at line 104 through 224: `void paint(juce::Graphics& g) override`
  Draws the component.
- `PhaseCard` at line 237 through 263: `PhaseCard(PluginEditor& editor, int op, int part) : P1Card("phase", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phase); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::phase); }), on_(editor.state(), params::opPhase(op, "on", part), "on")`
  Constructs the object.
- `resized` at line 265 through 283: `void resized() override`
  Lays out the child components.
- `PhaseEnvelopeCard` at line 292 through 347: `PhaseEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("phase env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::phaseEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::phaseEnvelope); }), envView_(editor.state(), {params::opPhaseEnv(op, "r1", part), params::opPhaseEnv(op, "r2", part), params::opPhaseEnv(op, "r3", part), params::opPhaseEnv(op, "relr", part)}, {params::opPhaseEnv(op, "l1", part), params::opPhaseEnv(op, "l2", part), params::opPhaseEnv(op, "l3", part), params::opPhaseEnv(op, "rell", part)}, colour(tokens::purple), -100.0, 100.0, true, EnvelopeShape::bipolar, params::opPhaseEnv(op, "mode", part)), on_(editor.state(), params::opPhaseEnv(op, "on", part), "on"), mode_(editor.state(), params::opPhaseEnv(op, "mode", part), {"sustain off", "sustain on", "loop 1", "loop 2"})`
  Constructs the object.
- `resized` at line 349 through 368: `void resized() override`
  Lays out the child components.
- `tick` at line 370 through 370: `void tick()`
  Refreshes the live display state.
- `CompactOscillator` at line 382 through 396: `CompactOscillator(PluginEditor& editor, int op, int part) : ratio_(editor.state(), params::opRatio(op, part), "ratio", 22.0f), detune_(editor.state(), params::opDetune(op, part), "detune", 22.0f), level_(editor.state(), params::opLevel(op, part), "level", 22.0f), feedback_(editor.state(), params::opFeedback(op, part), "feedback", 22.0f), wave_(editor.state(), params::opWaveform(op, part), params::waveNames())`
  Constructs the object.
- `resized` at line 398 through 411: `void resized() override`
  Lays out the child components.
- `paint` at line 413 through 420: `void paint(juce::Graphics& g) override`
  Draws the component.
- `PhaseColumn` at line 432 through 466: `PhaseColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), osc_(editor, op, part_), map_(editor, op, part_), phase_(editor, op, part_), lfo_(editor, "phase lfo", params::opPhaseLfo(op, "rate", part_), params::opPhaseLfo(op, "depth", part_), params::opPhaseLfo(op, "wave", part_), params::opPhaseLfo(op, "mode", part_), params::opPhaseLfo(op, "rate_fade", part_), params::opPhaseLfo(op, "depth_fade", part_), op, part_, PluginEditor::OperatorSection::phaseLfo, {}, true, "bipolar"), env_(editor, op, part_)`
  Constructs the object.
- `resized` at line 468 through 482: `void resized() override`
  Lays out the child components.
- `paint` at line 484 through 494: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 496 through 501: `void tick()`
  Refreshes the live display state.
- `PhaseMapPage` at line 517 through 526: `explicit PhaseMapPage(PluginEditor& editor) : editor_(editor)`
  Constructs the object.
- `resized` at line 528 through 539: `void resized() override`
  Lays out the child components.
- `tick` at line 541 through 545: `void tick() override`
  Refreshes the live display state.
- `makePhaseMapPage` at line 554 through 557: `std::unique_ptr<PageBase> makePhaseMapPage(PluginEditor& editor)`
  Creates the phase map page.

### `plugin/juce/PagesSpectral.cpp`

- `isCarrier` at line 15 through 27: `bool isCarrier(int algorithm, int op)`
  Tests whether the the carrier.
- `spectrumIDs` at line 29 through 36: `juce::StringArray spectrumIDs(int op, int part, bool a)`
  Returns the spectrum i ds value.
- `rangeNames` at line 38 through 41: `juce::StringArray rangeNames()`
  Returns the range names value.
- `seriesHarmonic` at line 57 through 66: `bool seriesHarmonic(int base, int harmonic)`
  Tests the series harmonic condition.
- `groupHas` at line 68 through 87: `bool groupHas(int group, int harmonic)`
  Tests the group has condition.
- `SpringSlider` at line 91 through 100: `SpringSlider(juce::String label, std::function<void()> onBegin, std::function<void(double)> onChange, std::function<void()> onEnd = {}) : label_(std::move(label)), onBegin_(std::move(onBegin)), onChange_(std::move(onChange)), onEnd_(std::move(onEnd))`
  Constructs the object.
- `paint` at line 102 through 129: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseDown` at line 131 through 140: `void mouseDown(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseDrag` at line 142 through 147: `void mouseDrag(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseUp` at line 149 through 158: `void mouseUp(const juce::MouseEvent&) override`
  Handles a mouse interaction.
- `valueText` at line 161 through 166: `juce::String valueText() const`
  Returns the value text value.
- `setAmount` at line 168 through 174: `void setAmount(double amount)`
  Writes the amount.
- `MaskRow` at line 187 through 190: `explicit MaskRow(int initial) : index_(juce::jlimit(0, 2, initial))`
  Constructs the object.
- `index` at line 192 through 192: `int index() const`
  Returns the index value.
- `paint` at line 194 through 213: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseDown` at line 215 through 229: `void mouseDown(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `MorphEnvelopeCard` at line 239 through 293: `MorphEnvelopeCard(PluginEditor& editor, int op, int part) : P1Card("morph env", [&ed = editor, op, part] { ed.copyOperatorSection( op, part, PluginEditor::OperatorSection::spectrumEnvelope); }, [&ed = editor, op, part] { ed.pasteOperatorSection( op, part, PluginEditor::OperatorSection::spectrumEnvelope); }), env_(editor.state(), {params::opSpecEnvRate(op, 0, part), params::opSpecEnvRate(op, 1, part), params::opSpecEnvRate(op, 2, part), params::opSpecEnvRelRate(op, part)}, {params::opSpecEnvLevel(op, 0, part), params::opSpecEnvLevel(op, 1, part), params::opSpecEnvLevel(op, 2, part), params::opSpecEnvRelLevel(op, part)}, colour(tokens::purple), 0.0, 99.0, false, EnvelopeShape::bipolar, params::opSpecEnvMode(op, part), params::opSpecMorph(op, part)), on_(editor.state(), params::opSpecEnvOn(op, part), "on"), mode_(editor.state(), params::opSpecEnvMode(op, part), {"sustain off", "sustain on", "loop 1", "loop 2"})`
  Constructs the object.
- `resized` at line 295 through 313: `void resized() override`
  Lays out the child components.
- `tick` at line 315 through 315: `void tick()`
  Refreshes the live display state.
- `SpectrumRow` at line 327 through 430: `SpectrumRow(PluginEditor& editor, int op, int part, bool a) : P1Card("op" + juce::String(op + 1) + " spectrum " + (a ? "a" : "b"), [&ed = editor, op, part, a] { ed.copyOperatorSection( op, part, a ? PluginEditor::OperatorSection::spectrumA : PluginEditor::OperatorSection::spectrumB); }, [&ed = editor, op, part, a] { ed.pasteOperatorSection( op, part, a ? PluginEditor::OperatorSection::spectrumA : PluginEditor::OperatorSection::spectrumB); }), editor_(editor), op_(op), part_(part), isA_(a), bars_(editor.state(), spectrumIDs(op, part, a), colour(a ? tokens::purple : tokens::pink)), range_(editor.state(), a ? params::opSpecRangeA(op, part) : params::opSpecRangeB(op, part), rangeNames()), mask_(2), tilt_( "tilt", [this] { beginMacro(); }, [this](double value) { applyTilt(value); }, [this] { endMacro(); }), level_( "level", [this] { beginMacro(); }, [this](double value) { applyLevel(value); }, [this] { endMacro(); }), formant_( "formant", [this] { beginMacro(); }, [this](double value) { applyFormant(value); }, [this] { endMacro(); })`
  Constructs the object.
- `tick` at line 432 through 443: `void tick()`
  Refreshes the live display state.
- `resized` at line 445 through 486: `void resized() override`
  Lays out the child components.
- `firstHarmonic` at line 489 through 497: `int firstHarmonic() const`
  Returns the first harmonic value.
- `refreshGroupTest` at line 499 through 505: `void refreshGroupTest()`
  Refreshes the group test display.
- `windowFor` at line 507 through 515: `std::pair<int, int> windowFor(int index) const`
  Returns the window for value.
- `refreshSelectionItems` at line 517 through 528: `void refreshSelectionItems()`
  Refreshes the selection items display.
- `applySelection` at line 530 through 537: `void applySelection()`
  Applies the selection.
- `editable` at line 539 through 549: `bool editable(int harmonic, double level) const`
  Tests the editable condition.
- `selected` at line 551 through 568: `bool selected(int index) const`
  Selects the ed.
- `beginMacro` at line 570 through 570: `void beginMacro()`
  Runs the begin macro step for the surrounding code.
- `endMacro` at line 572 through 572: `void endMacro()`
  Runs the end macro step for the surrounding code.
- `applyTilt` at line 574 through 591: `void applyTilt(double amount)`
  Applies the tilt.
- `applyLevel` at line 593 through 606: `void applyLevel(double amount)`
  Applies the level.
- `applyFormant` at line 608 through 628: `void applyFormant(double amount)`
  Applies the formant.
- `normalise` at line 630 through 641: `void normalise()`
  Runs the normalise step for the surrounding code.
- `copyToOther` at line 643 through 654: `void copyToOther()`
  Copies the to other.
- `showTemplateMenu` at line 656 through 669: `void showTemplateMenu()`
  Opens the template menu.
- `loadTemplate` at line 671 through 680: `void loadTemplate(int wave)`
  Loads the template.
- `refreshLegend` at line 682 through 705: `void refreshLegend()`
  Refreshes the legend display.
- `SpectraOperatorCard` at line 734 through 757: `SpectraOperatorCard(PluginEditor& editor, int op) : editor_(editor), op_(op), part_(editor.activePart()), header_( false, false, {}, [&ed = editor, op] { ed.copyOperator(op); }, [&ed = editor, op] { ed.pasteOperator(op); }, [&ed = editor, op] { return getParam(ed, params::opMute(op, ed.activePart())) > 0.5; }, [&ed = editor, op](bool muted) { setParam(ed, params::opMute(op, ed.activePart()), muted ? 1 : 0); }, true), osc_(editor, op, editor.activePart(), true)`
  Constructs the object.
- `resized` at line 759 through 770: `void resized() override`
  Lays out the child components.
- `tick` at line 772 through 781: `void tick()`
  Refreshes the live display state.
- `paint` at line 783 through 793: `void paint(juce::Graphics& g) override`
  Draws the component.
- `SpectraModColumn` at line 808 through 850: `SpectraModColumn(PluginEditor& editor, int op, std::function<void(int)> onCopy, std::function<void(int)> onPaste) : editor_(editor), op_(op), part_(editor.activePart()), header_(false, false, {}, [this, onCopy] { onCopy(op_); }, [this, onPaste] { onPaste(op_); }, [this] { return getParam(editor_, params::opMute(op_, part_)) > 0.5; }, [this](bool muted) { setParam(editor_, params::opMute(op_, part_), muted ? 1 : 0); }), osc_(editor, op, part_, true), display_(editor.state(), spectrumIDs(op, part_, true), spectrumIDs(op, part_, false), params::opSpecMorph(op, part_), params::opSpecRangeA(op, part_), params::opSpecRangeB(op, part_), colour(tokens::purple), colour(tokens::pink)), env_(editor, op, part_), lfo_(editor, "morph lfo", params::opSpecLfoRate(op, part_), params::opSpecLfoDepth(op, part_), params::opSpecLfoWave(op, part_), params::opSpecLfoMode(op, part_), params::opSpecLfoRateFade(op, part_), params::opSpecLfoDepthFade(op, part_), op, part_, PluginEditor::OperatorSection::spectrumLfo, [&ed = editor, op] { ensureSpectrumMode(ed, op, ed.activePart(), params::opSpecLfoDepth(op, ed.activePart())); }, true)`
  Constructs the object.
- `resized` at line 852 through 864: `void resized() override`
  Lays out the child components.
- `paint` at line 866 through 876: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 878 through 885: `void tick()`
  Refreshes the live display state.
- `OperatorSpectraPage` at line 900 through 914: `OperatorSpectraPage(PluginEditor& editor, bool a)`
  Constructs the object.
- `resized` at line 916 through 935: `void resized() override`
  Lays out the child components.
- `tick` at line 937 through 943: `void tick() override`
  Refreshes the live display state.
- `OperatorSpectraModPage` at line 954 through 963: `explicit OperatorSpectraModPage(PluginEditor& editor) : editor_(editor)`
  Constructs the object.
- `resized` at line 965 through 973: `void resized() override`
  Lays out the child components.
- `tick` at line 975 through 979: `void tick() override`
  Refreshes the live display state.
- `makeOperatorSpectraPage` at line 988 through 991: `std::unique_ptr<PageBase> makeOperatorSpectraPage(PluginEditor& editor, bool a)`
  Creates the operator spectra page.
- `makeOperatorSpectraModPage` at line 993 through 996: `std::unique_ptr<PageBase> makeOperatorSpectraModPage(PluginEditor& editor)`
  Creates the operator spectra mod page.

### `plugin/juce/PluginEditor.cpp`

- `PatchButton` at line 19 through 22: `PatchButton() : juce::Button("multi-patch library...")`
  Constructs the object.
- `paintButton` at line 24 through 35: `void paintButton(juce::Graphics& g, bool highlighted, bool) override`
  Draws the button in its normal, highlighted and down states.
- `SegmentButton` at line 40 through 43: `explicit SegmentButton(const juce::String& label) : juce::Button(label)`
  Constructs the object.
- `setActive` at line 45 through 51: `void setActive(bool active)`
  Sets the active flag.
- `paintButton` at line 53 through 66: `void paintButton(juce::Graphics& g, bool highlighted, bool) override`
  Draws the button in its normal, highlighted and down states.
- `PartChoiceButton` at line 74 through 77: `explicit PartChoiceButton(const juce::String& label) : juce::Button(label)`
  Constructs the object.
- `setActive` at line 79 through 85: `void setActive(bool active)`
  Sets the active flag.
- `paintButton` at line 87 through 100: `void paintButton(juce::Graphics& g, bool highlighted, bool) override`
  Draws the button in its normal, highlighted and down states.
- `Header` at line 110 through 135: `explicit Header(PluginEditor& editor) : editor_(editor), volume_(editor.state(), p1::kVolumeParamID)`
  Constructs the object.
- `resized` at line 137 through 147: `void resized() override`
  Lays out the child components.
- `paint` at line 149 through 169: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 171 through 203: `void tick()`
  Refreshes the live display state.
- `setPatchName` at line 205 through 211: `void setPatchName(const juce::String& name)`
  Writes the patch name.
- `setSpectroMode` at line 213 through 220: `void setSpectroMode(bool spectro)`
  Writes the spectro mode.
- `drawWordmark` at line 223 through 251: `void drawWordmark(juce::Graphics& g)`
  Draws the wordmark.
- `Toolbar` at line 271 through 304: `explicit Toolbar(PluginEditor& editor) : editor_(editor)`
  Constructs the object.
- `resized` at line 306 through 322: `void resized() override`
  Lays out the child components.
- `paint` at line 324 through 335: `void paint(juce::Graphics& g) override`
  Draws the component.
- `refreshPartNames` at line 337 through 345: `void refreshPartNames()`
  Refreshes the part names display.
- `tick` at line 347 through 361: `void tick()`
  Refreshes the live display state.
- `drawLegend` at line 364 through 383: `void drawLegend(juce::Graphics& g)`
  Draws the legend.
- `ListBody` at line 402 through 415: `ListBody(int count, int rowHeight, int selected, const juce::String& name) : count_(juce::jmax(0, count)), selected_(juce::jlimit(0, juce::jmax(0, count_ - 1), selected)), list_(name, this)`
  Constructs the object.
- `selected` at line 417 through 417: `int selected() const`
  Selects the ed.
- `resized` at line 419 through 423: `void resized() override`
  Lays out the child components.
- `getNumRows` at line 425 through 425: `int getNumRows() override`
  Reads the num rows.
- `paintListBoxItem` at line 427 through 443: `void paintListBoxItem(int row, juce::Graphics& g, int width, int height, bool selected) override`
  Draws one list-box row.
- `listBoxItemClicked` at line 445 through 450: `void listBoxItemClicked(int row, const juce::MouseEvent&) override`
  Handles a list-box row click.
- `paintRow` at line 453 through 454: `virtual void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) = 0`
  Draws one row of the control.
- `SlotListBody` at line 464 through 470: `SlotListBody(int count, std::function<juce::String(int)> nameFor, std::function<bool(int)> usedFor, int selected) : ListBody(count, 24, selected, "slots"), nameFor_(std::move(nameFor)), usedFor_(std::move(usedFor))`
  Constructs the object.
- `paintRow` at line 473 through 489: `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override`
  Draws one row of the control.
- `ModeListBody` at line 498 through 502: `ModeListBody(juce::StringArray modes, int selected) : ListBody(modes.size(), 28, selected, "modes"), modes_(std::move(modes))`
  Constructs the object.
- `paintRow` at line 505 through 512: `void paintRow(juce::Graphics& g, int row, juce::Rectangle<int> area, bool selected) override`
  Draws one row of the control.
- `AlgTile` at line 520 through 524: `AlgTile(int index, std::function<void(int)> onSelect) : juce::Button("alg"), index_(index), onSelect_(std::move(onSelect))`
  Constructs the object.
- `setSelected` at line 526 through 532: `void setSelected(bool selected)`
  Sets the selected flag.
- `paintButton` at line 534 through 550: `void paintButton(juce::Graphics& g, bool highlighted, bool) override`
  Draws the button in its normal, highlighted and down states.
- `mouseUp` at line 552 through 556: `void mouseUp(const juce::MouseEvent&) override`
  Handles a mouse interaction.
- `AlgPickerBody` at line 566 through 581: `explicit AlgPickerBody(int selected) : selected_(juce::jlimit(0, 9, selected))`
  Constructs the object.
- `selected` at line 583 through 583: `int selected() const`
  Selects the ed.
- `resized` at line 585 through 597: `void resized() override`
  Lays out the child components.
- `select` at line 600 through 605: `void select(int index)`
  Runs the select step for the surrounding code.
- `RenameBody` at line 614 through 629: `explicit RenameBody(const juce::String& initial)`
  Constructs the object.
- `text` at line 631 through 631: `juce::String text() const`
  Returns the text value.
- `focus` at line 632 through 632: `void focus()`
  Moves keyboard focus to the control.
- `resized` at line 634 through 640: `void resized() override`
  Lays out the child components.
- `PluginEditor` at line 649 through 679: `PluginEditor::PluginEditor(TypePAlphaAudioProcessor& audioProcessor) : AudioProcessorEditor(&audioProcessor), processor_(audioProcessor)`
  Builds the editor, the header, the toolbar, the tabs and the first page, then starts the 30 Hz timer.
- `~PluginEditor` at line 681 through 686: `PluginEditor::~PluginEditor()`
  Destroys the object.
- `paint` at line 688 through 691: `void PluginEditor::paint(juce::Graphics& g)`
  Draws the component.
- `resized` at line 693 through 707: `void PluginEditor::resized()`
  Lays out the child components.
- `timerCallback` at line 709 through 724: `void PluginEditor::timerCallback()`
  Runs the startup fit and refreshes the live visualisers.
- `fitToHostWindow` at line 726 through 763: `void PluginEditor::fitToHostWindow()`
  Measures the host chrome and sizes the editor to the display work area.
- `updateVisualizers` at line 765 through 785: `void PluginEditor::updateVisualizers()`
  Refreshes the header, toolbar and page once per timer tick.
- `refreshMacroFeedback` at line 787 through 798: `void PluginEditor::refreshMacroFeedback(juce::Component& root)`
  Walks the component tree and updates every macro feedback target.
- `buildUi` at line 800 through 832: `void PluginEditor::buildUi()`
  Creates the header, toolbar and tab bar and rebuilds the current page.
- `buildPage` at line 834 through 874: `void PluginEditor::buildPage()`
  Destroys the old page and creates the page for the active tab.
- `setActivePart` at line 876 through 884: `void PluginEditor::setActivePart(int part)`
  Writes the active part.
- `showTab` at line 886 through 897: `void PluginEditor::showTab(int tab)`
  Opens the tab.
- `refreshNames` at line 899 through 904: `void PluginEditor::refreshNames()`
  Refreshes the names display.
- `showAlgorithmPicker` at line 906 through 924: `void PluginEditor::showAlgorithmPicker()`
  Opens the ten-tile algorithm picker.
- `applyAlgorithm` at line 926 through 932: `void PluginEditor::applyAlgorithm(int algorithm)`
  Writes the selected algorithm to the active part.
- `showRenameDialog` at line 934 through 959: `void PluginEditor::showRenameDialog(bool part, int index)`
  Opens the rename dialog for a part or the patch.
- `showRenameMacro` at line 961 through 982: `void PluginEditor::showRenameMacro(int index)`
  Opens the rename dialog for a macro.
- `showSinglePartsBrowser` at line 984 through 1060: `void PluginEditor::showSinglePartsBrowser()`
  Opens the single-part library browser.
- `exportLibrary` at line 1062 through 1080: `void PluginEditor::exportLibrary()`
  Exports the library to a chosen file.
- `importLibrary` at line 1082 through 1111: `void PluginEditor::importLibrary()`
  Imports a library file.
- `fxSectionParamIDs` at line 1113 through 1178: `static std::vector<juce::String> fxSectionParamIDs( int part, PluginEditor::PartFxSection section)`
  Returns the FX section param i ds value.
- `copyPart` at line 1180 through 1191: `void PluginEditor::copyPart(int part)`
  Copies the part parameters and routing into the clipboard.
- `pastePart` at line 1193 through 1202: `void PluginEditor::pastePart(int part)`
  Writes the part clipboard to the current part.
- `copyPartFxSection` at line 1204 through 1218: `void PluginEditor::copyPartFxSection(PartFxSection section)`
  Copies one part fx/mod card to the other part.
- `copyOperator` at line 1220 through 1240: `void PluginEditor::copyOperator(int op)`
  Copies one operator's parameters.
- `pasteOperator` at line 1242 through 1266: `void PluginEditor::pasteOperator(int op)`
  Writes one operator clipboard to the current operator.
- `sectionParamIDs` at line 1275 through 1381: `std::vector<juce::String> sectionParamIDs(int op, int part, PluginEditor::OperatorSection section)`
  The parameters each operator card owns, in its display order. The list is the card's contract: copy fills the clipboard from it and paste writes back through it, so a card cannot copy some of its controls and miss others. The amplitude envelope's surge and release are the op1-only `attack` and `release` names, and opEnv maps them for the source and destination op.
- `copyOperatorSection` at line 1385 through 1396: `void PluginEditor::copyOperatorSection(int op, int part, OperatorSection section)`
  Copies one operator section, such as the oscillator or an envelope.
- `pasteOperatorSection` at line 1398 through 1424: `void PluginEditor::pasteOperatorSection(int op, int part, OperatorSection section)`
  Writes one operator section and, for a spectrum section, selects spectrum mode.
- `initPart` at line 1426 through 1440: `void PluginEditor::initPart(int part)`
  Resets a part to the factory init values.
- `initMultiPatch` at line 1442 through 1448: `void PluginEditor::initMultiPatch()`
  Resets the current multi-patch.
- `togglePartMute` at line 1450 through 1464: `void PluginEditor::togglePartMute(int part, bool muted)`
  Mutes or restores a part through its level parameter.
- `currentPartParams` at line 1466 through 1478: `std::vector<int32_t> PluginEditor::currentPartParams(int part)`
  Collects the part-block parameters for save or copy.
- `applyPartParams` at line 1480 through 1494: `void PluginEditor::applyPartParams(int part, const std::vector<int32_t>& params)`
  Writes a part-block parameter vector.
- `currentRouting` at line 1496 through 1505: `int PluginEditor::currentRouting(int field, int part)`
  Reads one per-part routing value.
- `applyRouting` at line 1507 through 1516: `void PluginEditor::applyRouting(int field, int part, int value)`
  Writes one per-part routing value.
- `currentMacroParams` at line 1518 through 1528: `std::vector<int32_t> PluginEditor::currentMacroParams()`
  Collects the 54 macro parameters.
- `applyMacroParams` at line 1530 through 1546: `void PluginEditor::applyMacroParams(const std::vector<int32_t>& params)`
  Writes the 54 macro parameters.
- `savePartToSlot` at line 1548 through 1556: `void PluginEditor::savePartToSlot(int part, int slot)`
  Saves the current part to a library slot.
- `loadPartFromSlot` at line 1558 through 1569: `void PluginEditor::loadPartFromSlot(int part, int slot)`
  Loads a library part into the current part.
- `savePatchToSlot` at line 1571 through 1618: `void PluginEditor::savePatchToSlot(int slot, int mode)`
  Saves the current patch to a multi slot.
- `reportLibrarySave` at line 1620 through 1645: `void PluginEditor::reportLibrarySave(const LibrarySaveResult& result)`
  Reports a library save result to the player.
- `loadPatchFromSlot` at line 1647 through 1680: `void PluginEditor::loadPatchFromSlot(int slot)`
  Loads a multi slot into both parts.
- `showConfirm` at line 1682 through 1704: `void PluginEditor::showConfirm(const juce::String& message, std::function<void()> onConfirm, const juce::String& title, const juce::String& confirm)`
  Opens a confirmation dialog.
- `showPatchBrowser` at line 1706 through 1747: `void PluginEditor::showPatchBrowser()`
  Opens the multi-patch browser.
- `showPatchSaveMode` at line 1749 through 1779: `void PluginEditor::showPatchSaveMode(int slot)`
  Opens the patch save-mode dialog.
- `closeDialog` at line 1781 through 1787: `void PluginEditor::closeDialog()`
  Closes the current dialog.
- `paint` at line 1789 through 1810: `void p1::ui::P1Canvas::paint(juce::Graphics& g)`
  Draws the component.

### `plugin/juce/PluginEditor.h`

- `paint` at line 23 through 23: `void paint(juce::Graphics& g) override`
  Draws the component.
- `~PageBase` at line 28 through 28: `~PageBase() override = default`
  Destroys the object.
- `tick` at line 29 through 29: `virtual void tick() = 0`
  Refreshes the live display state.
- `paint` at line 65 through 65: `void paint(juce::Graphics& g) override`
  Draws the component.
- `resized` at line 66 through 66: `void resized() override`
  Scales the fixed design canvas into the host window.
- `timerCallback` at line 67 through 67: `void timerCallback() override`
  Runs the startup fit and refreshes the live visualisers.
- `processor` at line 69 through 69: `TypePAlphaAudioProcessor& processor()`
  Returns the audio processor.
- `state` at line 70 through 70: `juce::AudioProcessorValueTreeState& state()`
  Returns the APVTS.
- `activePart` at line 71 through 71: `int activePart() const`
  Returns the active part index.
- `activeTab` at line 72 through 72: `int activeTab() const`
  Returns the active page index.
- `setActivePart` at line 74 through 74: `void setActivePart(int part)`
  Writes the active part.
- `showTab` at line 75 through 75: `void showTab(int tab)`
  Opens the tab.
- `refreshNames` at line 76 through 76: `void refreshNames()`
  Refreshes the names display.
- `reportLibrarySave` at line 77 through 77: `void reportLibrarySave(const LibrarySaveResult& result)`
  Reports a library save result to the player.
- `showAlgorithmPicker` at line 78 through 78: `void showAlgorithmPicker()`
  Opens the ten-tile algorithm picker.
- `applyAlgorithm` at line 79 through 79: `void applyAlgorithm(int algorithm)`
  Writes the selected algorithm to the active part.
- `showRenameDialog` at line 80 through 80: `void showRenameDialog(bool part, int index)`
  Opens the rename dialog for a part or the patch.
- `showRenameMacro` at line 81 through 81: `void showRenameMacro(int index)`
  Opens the rename dialog for a macro.
- `showSinglePartsBrowser` at line 82 through 82: `void showSinglePartsBrowser()`
  Opens the single-part library browser.
- `exportLibrary` at line 83 through 83: `void exportLibrary()`
  Exports the library to a chosen file.
- `importLibrary` at line 84 through 84: `void importLibrary()`
  Imports a library file.
- `showPatchBrowser` at line 85 through 85: `void showPatchBrowser()`
  Opens the multi-patch browser.
- `showPatchSaveMode` at line 86 through 86: `void showPatchSaveMode(int slot)`
  Opens the patch save-mode dialog.
- `copyPart` at line 87 through 87: `void copyPart(int part)`
  Copies the part parameters and routing into the clipboard.
- `pastePart` at line 88 through 88: `void pastePart(int part)`
  Writes the part clipboard to the current part.
- `copyOperator` at line 89 through 89: `void copyOperator(int op)`
  Copies one operator's parameters.
- `pasteOperator` at line 90 through 90: `void pasteOperator(int op)`
  Writes one operator clipboard to the current operator.
- `copyOperatorSection` at line 91 through 91: `void copyOperatorSection(int op, int part, OperatorSection section)`
  Copies one operator section, such as the oscillator or an envelope.
- `pasteOperatorSection` at line 92 through 92: `void pasteOperatorSection(int op, int part, OperatorSection section)`
  Writes one operator section and, for a spectrum section, selects spectrum mode.
- `copyPartFxSection` at line 94 through 94: `void copyPartFxSection(PartFxSection section)`
  Copy one part fx/mod card from the active part to the other part.
- `initPart` at line 95 through 95: `void initPart(int part)`
  Resets a part to the factory init values.
- `initMultiPatch` at line 96 through 96: `void initMultiPatch()`
  Resets the current multi-patch.
- `togglePartMute` at line 97 through 97: `void togglePartMute(int part, bool muted)`
  Mutes or restores a part through its level parameter.
- `savePartToSlot` at line 98 through 98: `void savePartToSlot(int part, int slot)`
  Saves the current part to a library slot.
- `loadPartFromSlot` at line 99 through 99: `void loadPartFromSlot(int part, int slot)`
  Loads a library part into the current part.
- `savePatchToSlot` at line 100 through 100: `void savePatchToSlot(int slot, int mode = 0)`
  Saves the current patch to a multi slot.
- `loadPatchFromSlot` at line 101 through 101: `void loadPatchFromSlot(int slot)`
  Loads a multi slot into both parts.
- `showConfirm` at line 102 through 104: `void showConfirm(const juce::String& message, std::function<void()> onConfirm, const juce::String& title = "confirm save", const juce::String& confirm = "save")`
  Opens a confirmation dialog.
- `closeDialog` at line 105 through 105: `void closeDialog()`
  Closes the current dialog.
- `updateVisualizers` at line 106 through 106: `void updateVisualizers()`
  Refreshes the header, toolbar and page once per timer tick.
- `buildUi` at line 108 through 108: `void buildUi()`
  Creates the header, toolbar and tab bar and rebuilds the current page.
- `buildHeader` at line 109 through 109: `void buildHeader()`
  Creates the header component.
- `buildToolbar` at line 110 through 110: `void buildToolbar()`
  Creates the toolbar component.
- `buildPage` at line 111 through 111: `void buildPage()`
  Destroys the old page and creates the page for the active tab.
- `refreshMacroFeedback` at line 112 through 112: `void refreshMacroFeedback(juce::Component& root)`
  Walks the component tree and updates every macro feedback target.
- `setSpectroMode` at line 113 through 113: `void setSpectroMode(bool spectro)`
  Writes the spectro mode.
- `fitToHostWindow` at line 114 through 114: `void fitToHostWindow()`
  Measures the host chrome and sizes the editor to the display work area.
- `currentPartParams` at line 115 through 115: `std::vector<int32_t> currentPartParams(int part)`
  Collects the part-block parameters for save or copy.
- `applyPartParams` at line 116 through 116: `void applyPartParams(int part, const std::vector<int32_t>& params)`
  Writes a part-block parameter vector.
- `currentRouting` at line 117 through 117: `int currentRouting(int field, int part)`
  Reads one per-part routing value.
- `applyRouting` at line 118 through 118: `void applyRouting(int field, int part, int value)`
  Writes one per-part routing value.
- `currentMacroParams` at line 119 through 119: `std::vector<int32_t> currentMacroParams()`
  Collects the 54 macro parameters.
- `applyMacroParams` at line 120 through 120: `void applyMacroParams(const std::vector<int32_t>& params)`
  Writes the 54 macro parameters.

### `plugin/juce/PluginProcessor.cpp`

- `isEngineMessage` at line 36 through 42: `bool isEngineMessage(const juce::MidiMessage& message)`
  Tests whether the the engine message.
- `engineHandlesVelocityTarget` at line 48 through 64: `bool engineHandlesVelocityTarget(int target, int memberId)`
  The engine applies the velocity source itself for the operator level, feedback, morph, envelope rate, LFO rate and LFO depth groups, on the all-operator and the carrier and modulator rows. The phase envelope and phase LFO and the part-level pitch and filter envelopes stay in the plugin's part-level layer.
- `operatorEnvelopeIndices` at line 69 through 83: `std::array<int, 4> operatorEnvelopeIndices()`
  The part-block indices of the four operator warm-up rates. The migration needs them because the operator envelope grew from seven to eight parameters inside each operator block.
- `partParamIndex` at line 87 through 94: `int partParamIndex(const std::string& wanted)`
  The part-block index of a `.p0` parameter, or -1. The preset migration needs these for the remaps that cannot be expressed as an append.
- `panEnvelopeIndex` at line 97 through 97: `int panEnvelopeIndex()`
  The part-block index of the pan envelope, for the version 11 remap.
- `feedbackEnvelopeIndices` at line 101 through 108: `std::array<int, 4> feedbackEnvelopeIndices()`
  The part-block indices of each operator feedback envelope, for the version 12 remap.
- `configurePresetStore` at line 114 through 148: `void configurePresetStore(p1::PresetStore& store)`
  Every PresetStore that reads an old library needs the same block indices for the remaps. The in-memory store used to get these, whilst the temporary disk store in saveLibrary did not, so an old record was migrated with -1 bases and then written back corrupted. Configure both through this one function.
- `midi_get` at line 152 through 152: `int midi_get(uint8_t* status, uint8_t* data1, uint8_t* data2)`
  Returns the MIDI get value.
- `midi_get` at line 154 through 157: `int midi_get(uint8_t*, uint8_t*, uint8_t*)`
  Returns the MIDI get value.
- `ScopeRing` at line 159 through 159: `ScopeRing::ScopeRing() = default`
  Returns the scope ring value.
- `push` at line 161 through 172: `void ScopeRing::push(const float* data, int count)`
  Returns the push value.
- `read` at line 174 through 192: `void ScopeRing::read(float* destination, int count)`
  Returns the read value.
- `getPartName` at line 194 through 197: `juce::String TypePAlphaAudioProcessor::getPartName(int part) const`
  Reads the part name.
- `setPartName` at line 199 through 202: `void TypePAlphaAudioProcessor::setPartName(int part, const juce::String& name)`
  Writes the part name.
- `getMacroName` at line 204 through 207: `juce::String TypePAlphaAudioProcessor::getMacroName(int index) const`
  Reads the macro name.
- `setMacroName` at line 209 through 213: `void TypePAlphaAudioProcessor::setMacroName(int index, const juce::String& name)`
  Writes the macro name.
- `macroEffectiveValue` at line 215 through 237: `double TypePAlphaAudioProcessor::macroEffectiveValue(int index) const`
  Returns the macro effective value value.
- `macroFeedback` at line 239 through 301: `TypePAlphaAudioProcessor::MacroFeedback TypePAlphaAudioProcessor::macroFeedback(int index) const`
  Returns the macro feedback value.
- `readOutputScope` at line 303 through 306: `void TypePAlphaAudioProcessor::readOutputScope(float* destination, int count)`
  Reads the output scope.
- `readTapScope` at line 308 through 315: `void TypePAlphaAudioProcessor::readTapScope(int channel, float* destination, int count)`
  Reads the tap scope.
- `TypePAlphaAudioProcessor` at line 317 through 370: `TypePAlphaAudioProcessor::TypePAlphaAudioProcessor() : AudioProcessor(BusesProperties().withOutput( "Output", juce::AudioChannelSet::stereo(), true)), midi_(engine_), apvts_(*this, nullptr, P1_STATE_TREE_ID, createLayout())`
  Constructs the processor, the engine interface, the MIDI bridge, the APVTS and the library.
- `createLayout` at line 372 through 444: `juce::AudioProcessorValueTreeState::ParameterLayout TypePAlphaAudioProcessor::createLayout()`
  Creates the layout.
- `prepareToPlay` at line 446 through 476: `void TypePAlphaAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)`
  Resets the engine, sizes the rate bridge and buffers, and syncs the parameters.
- `releaseResources` at line 478 through 481: `void TypePAlphaAudioProcessor::releaseResources()`
  Releases every engine voice.
- `isBusesLayoutSupported` at line 483 through 490: `bool TypePAlphaAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const`
  Accepts a disabled input and a mono or stereo output.
- `syncParams` at line 492 through 511: `void TypePAlphaAudioProcessor::syncParams()`
  Writes the changed APVTS values to the engine.
- `syncMacros` at line 513 through 617: `void TypePAlphaAudioProcessor::syncMacros()`
  Resolves the six macros to destination offsets and writes the effective values.
- `feedMessage` at line 619 through 624: `void TypePAlphaAudioProcessor::feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message)`
  Feeds the message.
- `renderCore` at line 626 through 629: `void TypePAlphaAudioProcessor::renderCore(int32_t* out, int frames)`
  Renders one core chunk through the engine interface.
- `pushTapScopes` at line 631 through 644: `void TypePAlphaAudioProcessor::pushTapScopes(p1::Engine& engine, int frames)`
  Copies the ten engine tap buffers into the scope rings.
- `publishMorph` at line 646 through 652: `void TypePAlphaAudioProcessor::publishMorph(p1::Engine& engine)`
  Publishes the eight live spectral morphs.
- `publishPhaseMaps` at line 654 through 670: `void TypePAlphaAudioProcessor::publishPhaseMaps(p1::Engine& engine)`
  Publishes the eight phase-map snapshots through the three-buffer handoff.
- `feedScheduled` at line 672 through 695: `void TypePAlphaAudioProcessor::feedScheduled(int64_t limit)`
  Feeds the scheduled MIDI events whose core frame is before the limit.
- `flushDeferred` at line 697 through 702: `void TypePAlphaAudioProcessor::flushDeferred()`
  Feeds the note-offs that waited for the chunk to render.
- `coreChunk` at line 704 through 715: `void TypePAlphaAudioProcessor::coreChunk(void* context, p1::Engine& engine, int frames)`
  Runs the per-core-chunk MIDI, tap and snapshot work.
- `processBlock` at line 717 through 882: `void TypePAlphaAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi)`
  Runs the host block through the 48 kHz core path or the rate bridge.
- `phaseMap` at line 884 through 897: `bool TypePAlphaAudioProcessor::phaseMap(int op, int part, p1::PhaseMapSnapshot& out) const`
  Reads the current phase-map snapshot for the editor.
- `specMorph` at line 899 through 907: `double TypePAlphaAudioProcessor::specMorph(int op, int part) const`
  Reads the live spectral morph for the editor.
- `libraryFile` at line 909 through 918: `juce::File TypePAlphaAudioProcessor::libraryFile() const`
  Returns the shared library path.
- `resetPhaseMaps` at line 920 through 923: `void TypePAlphaAudioProcessor::resetPhaseMaps()`
  Clears the phase-map, LFO and envelope pools.
- `ensureLibraryLoaded` at line 925 through 945: `void TypePAlphaAudioProcessor::ensureLibraryLoaded()`
  Loads the shared library once.
- `saveLibrary` at line 947 through 1022: `LibrarySaveResult TypePAlphaAudioProcessor::saveLibrary()`
  Runs the locked read-merge-write of the shared library.
- `~LockRelease` at line 969 through 969: `~LockRelease()`
  Destroys the object.
- `createEditor` at line 1024 through 1028: `juce::AudioProcessorEditor* TypePAlphaAudioProcessor::createEditor()`
  Creates the plugin editor.
- `getStateInformation` at line 1030 through 1061: `void TypePAlphaAudioProcessor::getStateInformation(juce::MemoryBlock& destData)`
  Serialises the APVTS, names, UI state, routing and library snapshot.
- `setStateInformation` at line 1063 through 1155: `void TypePAlphaAudioProcessor::setStateInformation(const void* data, int sizeInBytes)`
  Restores the project state and migrates older versions.
- `createPluginFilter` at line 1157 through 1160: `juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()`
  Creates the plugin filter.

### `plugin/juce/PluginProcessor.h`

- `push` at line 25 through 25: `void push(const float* data, int count)`
  Runs the push step for the surrounding code.
- `read` at line 26 through 26: `void read(float* destination, int count)`
  Runs the read step for the surrounding code.
- `~TypePAlphaAudioProcessor` at line 52 through 52: `~TypePAlphaAudioProcessor() override = default`
  Destroys the object.
- `prepareToPlay` at line 54 through 54: `void prepareToPlay(double sampleRate, int samplesPerBlock) override`
  Resets the engine, sizes the rate bridge and buffers, and syncs the parameters.
- `releaseResources` at line 55 through 55: `void releaseResources() override`
  Releases every engine voice.
- `isBusesLayoutSupported` at line 56 through 56: `bool isBusesLayoutSupported(const BusesLayout& layouts) const override`
  Accepts a disabled input and a mono or stereo output.
- `processBlock` at line 58 through 58: `void processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midi) override`
  Runs the host block through the 48 kHz core path or the rate bridge.
- `createEditor` at line 60 through 60: `juce::AudioProcessorEditor* createEditor() override`
  Creates the plugin editor.
- `hasEditor` at line 61 through 61: `bool hasEditor() const override`
  Tests whether the the editor.
- `getName` at line 63 through 63: `const juce::String getName() const override`
  Reads the name.
- `acceptsMidi` at line 64 through 64: `bool acceptsMidi() const override`
  Tests the accepts MIDI condition.
- `producesMidi` at line 65 through 65: `bool producesMidi() const override`
  Tests the produces MIDI condition.
- `isMidiEffect` at line 66 through 66: `bool isMidiEffect() const override`
  Tests whether the the MIDI effect.
- `getTailLengthSeconds` at line 67 through 67: `double getTailLengthSeconds() const override`
  Reads the tail length seconds.
- `getNumPrograms` at line 69 through 69: `int getNumPrograms() override`
  Reads the num programs.
- `getCurrentProgram` at line 70 through 70: `int getCurrentProgram() override`
  Reads the current program.
- `setCurrentProgram` at line 71 through 71: `void setCurrentProgram(int) override`
  Writes the current program.
- `getProgramName` at line 72 through 72: `const juce::String getProgramName(int) override`
  Reads the program name.
- `changeProgramName` at line 73 through 73: `void changeProgramName(int, const juce::String&) override`
  Runs the change program name step for the surrounding code.
- `getStateInformation` at line 75 through 75: `void getStateInformation(juce::MemoryBlock& destData) override`
  Serialises the APVTS, names, UI state, routing and library snapshot.
- `setStateInformation` at line 76 through 76: `void setStateInformation(const void* data, int sizeInBytes) override`
  Restores the project state and migrates older versions.
- `ensureLibraryLoaded` at line 78 through 78: `void ensureLibraryLoaded()`
  Loads the shared library once.
- `resetPhaseMaps` at line 79 through 79: `void resetPhaseMaps()`
  Clears the phase-map, LFO and envelope pools.
- `saveLibrary` at line 80 through 80: `LibrarySaveResult saveLibrary()`
  Runs the locked read-merge-write of the shared library.
- `libraryFile` at line 81 through 81: `juce::File libraryFile() const`
  Returns the shared library path.
- `setLibraryFileForTesting` at line 82 through 85: `void setLibraryFileForTesting(const juce::File& file)`
  Writes the library file for testing.
- `parameters` at line 87 through 87: `juce::AudioProcessorValueTreeState& parameters()`
  Returns the parameters value.
- `presets` at line 88 through 88: `p1::PresetStore& presets()`
  Returns the presets value.
- `presets` at line 89 through 89: `const p1::PresetStore& presets() const`
  Returns the presets value.
- `readOutputScope` at line 91 through 91: `void readOutputScope(float* destination, int count)`
  Reads the output scope.
- `readTapScope` at line 92 through 92: `void readTapScope(int channel, float* destination, int count)`
  Reads the tap scope.
- `specMorph` at line 95 through 95: `double specMorph(int op, int part) const`
  The live morph for an operator and part, 0 to 1, or -1 when the operator has not read the bank since the reset.
- `phaseMap` at line 96 through 96: `bool phaseMap(int op, int part, p1::PhaseMapSnapshot& out) const`
  Reads the current phase-map snapshot for the editor.
- `getPatchName` at line 98 through 98: `juce::String getPatchName() const`
  Reads the patch name.
- `setPatchName` at line 99 through 99: `void setPatchName(const juce::String& name)`
  Writes the patch name.
- `getPartName` at line 100 through 100: `juce::String getPartName(int part) const`
  Reads the part name.
- `setPartName` at line 101 through 101: `void setPartName(int part, const juce::String& name)`
  Writes the part name.
- `getMacroName` at line 102 through 102: `juce::String getMacroName(int index) const`
  Reads the macro name.
- `setMacroName` at line 103 through 103: `void setMacroName(int index, const juce::String& name)`
  Writes the macro name.
- `macroEffectiveValue` at line 104 through 104: `double macroEffectiveValue(int index) const`
  Returns the macro effective value value.
- `macroFeedback` at line 113 through 113: `MacroFeedback macroFeedback(int index) const`
  Returns the macro feedback value.
- `getUiActivePart` at line 114 through 114: `int getUiActivePart() const`
  Reads the UI active part.
- `setUiActivePart` at line 115 through 115: `void setUiActivePart(int part)`
  Writes the UI active part.
- `getUiPage` at line 116 through 116: `int getUiPage() const`
  Reads the UI page.
- `setUiPage` at line 117 through 120: `void setUiPage(int page)`
  Writes the UI page.
- `createLayout` at line 123 through 123: `static juce::AudioProcessorValueTreeState::ParameterLayout createLayout()`
  Creates the layout.
- `feedMessage` at line 124 through 124: `static void feedMessage(p1::MidiBridge& midi, const juce::MidiMessage& message)`
  Feeds the message.
- `syncParams` at line 125 through 125: `void syncParams()`
  Writes the changed APVTS values to the engine.
- `syncMacros` at line 126 through 126: `void syncMacros()`
  Resolves the six macros to destination offsets and writes the effective values.
- `renderCore` at line 127 through 127: `void renderCore(int32_t* out, int frames)`
  Renders one core chunk through the engine interface.
- `pushTapScopes` at line 128 through 128: `void pushTapScopes(p1::Engine& engine, int frames)`
  Copies the ten engine tap buffers into the scope rings.
- `publishMorph` at line 129 through 129: `void publishMorph(p1::Engine& engine)`
  Publishes the eight live spectral morphs.
- `publishPhaseMaps` at line 130 through 130: `void publishPhaseMaps(p1::Engine& engine)`
  Publishes the eight phase-map snapshots through the three-buffer handoff.
- `feedScheduled` at line 131 through 131: `void feedScheduled(int64_t limit)`
  Feeds the scheduled MIDI events whose core frame is before the limit.
- `flushDeferred` at line 132 through 132: `void flushDeferred()`
  Feeds the note-offs that waited for the chunk to render.
- `coreChunk` at line 133 through 133: `static void coreChunk(void* context, p1::Engine& engine, int frames)`
  Runs the per-core-chunk MIDI, tap and snapshot work.

### `plugin/juce/VolumeLaw.h`

- `volumeDbFromPosition` at line 23 through 36: `inline double volumeDbFromPosition(int position)`
  Returns the volume dB from position value.
- `volumeGainFromPosition` at line 38 through 44: `inline float volumeGainFromPosition(int position)`
  Returns the volume gain from position value.
- `volumePositionFromDb` at line 46 through 58: `inline int volumePositionFromDb(double db)`
  Returns the volume position from dB value.

### `plugin/preset/CutoffLaw.h`

- `cutoffHzFromPosition` at line 20 through 25: `inline double cutoffHzFromPosition(int32_t position)`
  Returns the cutoff Hz from position value.
- `cutoffPositionFromHz` at line 27 through 35: `inline int32_t cutoffPositionFromHz(double hz)`
  Returns the cutoff position from Hz value.
- `cutoffOldToNew` at line 40 through 53: `inline int32_t cutoffOldToNew(int32_t oldValue)`
  The old cutoff was a coefficient in 0..55296, scaled by 60211/65536 to keep the 44.1 kHz cutoff in hertz. Map the old coefficient to the frequency it produced and then to the new log position, so a saved patch keeps its cutoff.

### `plugin/preset/PresetStore.cpp`

- `remapPanBlock` at line 49 through 68: `void remapPanBlock(std::vector<int32_t>& params, int base)`
  The version 11 pan envelope block: on, r1, r2, r3, r4, l1, l2, l3, l4, depth. The version 12 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode. The release rate and level come from the old r4 and l4, and the depth becomes the mode: a non-zero depth was a live envelope, which maps to sustain on.
- `remapOffsetBlock` at line 74 through 93: `void remapOffsetBlock(const std::vector<int32_t>& in, int oldBase, std::vector<int32_t>& out, int newBase)`
  The version 12 offset envelope block: on, r1, r2, r3, r4, l1, l2, l3, l4. The version 13 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode. The old r4 and l4 are the release rate and level. The old envelope always sustained at l3, so the mode is sustain on.
- `remapAbsSpecBlock` at line 99 through 118: `void remapAbsSpecBlock(const std::vector<int32_t>& in, int oldBase, int depthIndex, std::vector<int32_t>& out, int newBase)`
  The version 13 spectral envelope block: r1, r2, r3, r4, l1, l2, l3 at the block, and `depth` far before it. The version 14 block: on, r1, r2, r3, l1, l2, l3, rel rate, rel level, mode. A non-zero depth meant a live envelope, so the mode is sustain on, r4 becomes the release rate and r4 is dropped.
- `remapAbsFilterBlock` at line 122 through 141: `void remapAbsFilterBlock(const std::vector<int32_t>& in, int oldBase, std::vector<int32_t>& out, int newBase)`
  The version 13 filter envelope block: depth, r1, r2, r3, r4, l1, l2, l3. The depth sits before the block, so the new block starts where it was.
- `remapWaveform19` at line 145 through 148: `int32_t remapWaveform19(int32_t value)`
  Version 19 inserts the half square at waveform 11, so the old square, saw and spectrum move up one. Values below 11 keep their meaning.
- `remapAlgorithm19` at line 152 through 161: `int32_t remapAlgorithm19(int32_t value)`
  Version 19 inserts one algorithm after 4 and another after 6, so the old algorithms 5, 6 and 7 become 6, 7 and 9.
- `remapInsertionValues` at line 166 through 181: `void remapInsertionValues(std::vector<int32_t>& params, uint8_t version, int algorithmIndex, const std::array<int, 4>& waveIndices)`
  Shift the algorithm and waveform values of a version 18 (or earlier) record. The record length is unchanged, so this runs on both the grow path and the same-size load path.
- `PresetStore` at line 185 through 200: `PresetStore::PresetStore(int partParamCount, int partCount, std::vector<int32_t> defaults, int cutoffIndex) : cutoffIndex_(cutoffIndex >= 0 && cutoffIndex < partParamCount ? cutoffIndex : -1), partParamCount_(std::max(0, partParamCount)), partCount_(std::max(0, partCount)), defaults_(std::move(defaults)), parts_(static_cast<std::size_t>(kPartSlots)), multis_(static_cast<std::size_t>(kMultiSlots)), partSource_(static_cast<std::size_t>(partCount_), -1)`
  Returns the preset store value.
- `partRecordSize` at line 202 through 205: `std::size_t PresetStore::partRecordSize() const`
  Returns the part record size value.
- `multiRecordSize` at line 207 through 210: `std::size_t PresetStore::multiRecordSize() const`
  Returns the multi record size value.
- `savePart` at line 212 through 227: `bool PresetStore::savePart(int slot, const std::string& name, const std::vector<int32_t>& params)`
  Stores one part record in the in-memory bank.
- `loadPart` at line 229 through 238: `bool PresetStore::loadPart(int slot, std::string& name, std::vector<int32_t>& params) const`
  Reads one part record from the in-memory bank.
- `partUsed` at line 240 through 244: `bool PresetStore::partUsed(int slot) const`
  Tests the part used condition.
- `partName` at line 246 through 251: `std::string PresetStore::partName(int slot) const`
  Returns the part name value.
- `partParams` at line 253 through 259: `const std::vector<int32_t>& PresetStore::partParams(int slot) const`
  Returns the part params value.
- `saveMulti` at line 261 through 293: `bool PresetStore::saveMulti(int slot, const std::string& name, const std::vector<uint8_t>& partSlot, const std::vector<int32_t>& level, const std::vector<int32_t>& delay, const std::vector<int32_t>& channel, const std::vector<int32_t>& low, const std::vector<int32_t>& high, const std::vector<int32_t>& macro, const std::array<std::string, kMacroCount>& macroNames)`
  Saves the multi.
- `loadMulti` at line 295 through 301: `bool PresetStore::loadMulti(int slot, Multi& out) const`
  Loads the multi.
- `multiUsed` at line 303 through 307: `bool PresetStore::multiUsed(int slot) const`
  Tests the multi used condition.
- `multiName` at line 309 through 314: `std::string PresetStore::multiName(int slot) const`
  Returns the multi name value.
- `packPartBank` at line 316 through 329: `std::vector<uint8_t> PresetStore::packPartBank() const`
  Packs all 128 part records.
- `loadPartBank` at line 331 through 383: `bool PresetStore::loadPartBank(const uint8_t* data, std::size_t size)`
  Loads and migrates a part bank.
- `packMultiBank` at line 385 through 408: `std::vector<uint8_t> PresetStore::packMultiBank() const`
  Packs all 64 multi records.
- `loadMultiBank` at line 410 through 468: `bool PresetStore::loadMultiBank(const uint8_t* data, std::size_t size)`
  Loads a multi bank, including the old macro-less size.
- `packLibrary` at line 470 through 490: `std::vector<uint8_t> PresetStore::packLibrary() const`
  Packs the library header, part bank and multi bank.
- `loadLibrary` at line 492 through 588: `bool PresetStore::loadLibrary(const uint8_t* data, std::size_t size)`
  Loads a library file and migrates its part records.
- `toVersion14Index` at line 590 through 597: `int PresetStore::toVersion14Index(int targetIndex) const`
  The part-block index in the version 14 layout, before the 34 LFO fade slots were inserted.
- `toVersion13Index` at line 599 through 622: `int PresetStore::toVersion13Index(int targetIndex) const`
  Version 13 to 14 removed each spectral depth (134, 325, 516, 707), grew * each spectral block by three slots at `on`, and then grew the filter * block by two at its own base.
- `toVersion13aIndex` at line 624 through 641: `int PresetStore::toVersion13aIndex(int targetIndex) const`
  The version 14 layout before the filter block became absolute.
- `toVersion12Index` at line 643 through 661: `int PresetStore::toVersion12Index(int targetIndex) const`
  The part-block index in the version 12 layout, before the five offset slots and the five absolute blocks were inserted.
- `migratePartBank` at line 663 through 1023: `bool PresetStore::migratePartBank(const uint8_t* data, uint32_t oldCount, std::vector<uint8_t>& out) const`
  Expands a part bank through the version 10 to 19 chain.
- `hasChanges` at line 1025 through 1031: `bool PresetStore::hasChanges() const`
  Reports whether any local slot is dirty.
- `mergeInto` at line 1033 through 1078: `std::vector<PresetStore::Conflict> PresetStore::mergeInto(PresetStore& disk)`
  Merges the local change set into a disk store and reports conflicts.
- `adoptLibrary` at line 1080 through 1089: `void PresetStore::adoptLibrary(const PresetStore& other)`
  Takes the merged banks and keeps the routing state.
- `markSaved` at line 1091 through 1095: `void PresetStore::markSaved()`
  Clears the local change set.
- `findFreePartSlot` at line 1097 through 1103: `int PresetStore::findFreePartSlot() const`
  Finds an unused part slot.
- `findFreeMultiSlot` at line 1105 through 1111: `int PresetStore::findFreeMultiSlot() const`
  Finds an unused multi slot.
- `setCurrentMulti` at line 1113 through 1116: `void PresetStore::setCurrentMulti(int slot)`
  Sets the current multi-patch slot.
- `partSource` at line 1118 through 1123: `int PresetStore::partSource(int part) const`
  Returns the part source value.
- `setPartSource` at line 1125 through 1131: `void PresetStore::setPartSource(int part, int slot)`
  Sets the library part source for a part.
- `shortName` at line 1133 through 1136: `std::string PresetStore::shortName(const std::string& name)`
  Returns the short name value.

### `plugin/preset/PresetStore.hpp`

- `partParamCount` at line 47 through 47: `int partParamCount() const`
  Returns the part param count value.
- `partCount` at line 48 through 48: `int partCount() const`
  Returns the part count value.
- `partRecordSize` at line 49 through 49: `std::size_t partRecordSize() const`
  Returns the part record size value.
- `multiRecordSize` at line 50 through 50: `std::size_t multiRecordSize() const`
  Returns the multi record size value.
- `savePart` at line 52 through 53: `bool savePart(int slot, const std::string& name, const std::vector<int32_t>& params)`
  Stores one part record in the in-memory bank.
- `loadPart` at line 54 through 54: `bool loadPart(int slot, std::string& name, std::vector<int32_t>& params) const`
  Reads one part record from the in-memory bank.
- `partUsed` at line 55 through 55: `bool partUsed(int slot) const`
  Tests the part used condition.
- `partName` at line 56 through 56: `std::string partName(int slot) const`
  Returns the part name value.
- `partParams` at line 57 through 57: `const std::vector<int32_t>& partParams(int slot) const`
  Returns the part params value.
- `saveMulti` at line 59 through 67: `bool saveMulti(int slot, const std::string& name, const std::vector<uint8_t>& partSlot, const std::vector<int32_t>& level, const std::vector<int32_t>& delay, const std::vector<int32_t>& channel, const std::vector<int32_t>& low, const std::vector<int32_t>& high, const std::vector<int32_t>& macro = {}, const std::array<std::string, kMacroCount>& macroNames = {})`
  Saves the multi.
- `loadMulti` at line 68 through 68: `bool loadMulti(int slot, Multi& out) const`
  Loads the multi.
- `multiUsed` at line 69 through 69: `bool multiUsed(int slot) const`
  Tests the multi used condition.
- `multiName` at line 70 through 70: `std::string multiName(int slot) const`
  Returns the multi name value.
- `packPartBank` at line 72 through 72: `std::vector<uint8_t> packPartBank() const`
  Packs all 128 part records.
- `loadPartBank` at line 73 through 73: `bool loadPartBank(const uint8_t* data, std::size_t size)`
  Loads and migrates a part bank.
- `packMultiBank` at line 74 through 74: `std::vector<uint8_t> packMultiBank() const`
  Packs all 64 multi records.
- `loadMultiBank` at line 75 through 75: `bool loadMultiBank(const uint8_t* data, std::size_t size)`
  Loads a multi bank, including the old macro-less size.
- `packLibrary` at line 77 through 77: `std::vector<uint8_t> packLibrary() const`
  Packs the library header, part bank and multi bank.
- `loadLibrary` at line 78 through 78: `bool loadLibrary(const uint8_t* data, std::size_t size)`
  Loads a library file and migrates its part records.
- `revision` at line 82 through 82: `uint32_t revision() const`
  The library revision written into the file header. A v2 file has no revision field, so it loads as revision 0.
- `setRevision` at line 83 through 83: `void setRevision(uint32_t revision)`
  Writes the revision.
- `mergeInto` at line 98 through 98: `std::vector<Conflict> mergeInto(PresetStore& disk)`
  Copy the slots this instance changed into `disk`, which must be a fresh store loaded from the current file. A slot the other side changed comes back as a conflict and is left out, so the disk version stays. The local store keeps its value and its queued change is cleared, so the edit can be saved to another slot.
- `adoptLibrary` at line 101 through 101: `void adoptLibrary(const PresetStore& other)`
  Take the merged banks back, keeping the routing state the user chose.
- `setOperatorEnvelopeIndices` at line 107 through 110: `void setOperatorEnvelopeIndices(const std::array<int, 4>& indices)`
  The part-block indices of the four operator warm-up rates, used to remap a version 10 record where the operator envelope grew by one parameter. -1 in any slot means the caller has no layout, so the older append-only migration runs instead.
- `setPanEnvelopeIndex` at line 115 through 115: `void setPanEnvelopeIndex(int index)`
  The part-block index of `pan_env_on`, used to remap a version 11 record where the pan envelope dropped the depth and gained a release level and a mode. -1 means no layout was supplied.
- `setOffsetEnvelopeIndices` at line 121 through 126: `void setOffsetEnvelopeIndices(const std::array<int, 4>& feedback, int pitch)`
  The part-block indices of each operator feedback envelope's `on` param and of `penv_on`, used to remap a version 12 record where the pitch and feedback envelopes gained a mode and swapped the fourth level for a release level. -1 in any slot means no layout was supplied.
- `setAbsEnvelopeIndices` at line 132 through 136: `void setAbsEnvelopeIndices(int filter, const std::array<int, 4>& spectral)`
  The part-block index of `fenv_on` and of each operator's `spec_env_on`, used to remap a version 13 record where the filter and spectral envelopes became absolute and gained on, release and mode controls. -1 in any slot means no layout was supplied.
- `setLfoFadeIndices` at line 141 through 145: `void setLfoFadeIndices(const std::array<int, 34>& indices)`
  The part-block indices of the 34 LFO fade parameters, used to remap a version 14 record to the version 15 layout. -1 in any slot means the caller has no layout, so the older append-only migration runs instead.
- `setInsertionIndices` at line 151 through 155: `void setInsertionIndices(int algorithm, const std::array<int, 4>& waves)`
  The part-block index of `algorithm` and of each operator's `waveform`, used to remap a version 18 record: version 19 inserts the half square after pulse and two algorithms after 4 and 6. -1 means the caller has no layout, so the values stay as they are.
- `markSaved` at line 158 through 158: `void markSaved()`
  Drop the queued changes after the file holds them.
- `hasChanges` at line 160 through 160: `bool hasChanges() const`
  Reports whether any local slot is dirty.
- `findFreePartSlot` at line 162 through 162: `int findFreePartSlot() const`
  Finds an unused part slot.
- `findFreeMultiSlot` at line 163 through 163: `int findFreeMultiSlot() const`
  Finds an unused multi slot.
- `currentMulti` at line 165 through 165: `int currentMulti() const`
  Returns the current multi value.
- `setCurrentMulti` at line 166 through 166: `void setCurrentMulti(int slot)`
  Sets the current multi-patch slot.
- `partSource` at line 167 through 167: `int partSource(int part) const`
  Returns the part source value.
- `setPartSource` at line 168 through 168: `void setPartSource(int part, int slot)`
  Sets the library part source for a part.
- `shortName` at line 171 through 171: `static std::string shortName(const std::string& name)`
  Returns the short name value.
- `toVersion12Index` at line 175 through 175: `int toVersion12Index(int targetIndex) const`
  The part-block index in the version 12 layout, before the five offset slots and the five absolute blocks were inserted.
- `toVersion14Index` at line 179 through 179: `int toVersion14Index(int targetIndex) const`
  The part-block index in the version 14 layout, before the 34 LFO fade slots were inserted.
- `toVersion13Index` at line 183 through 183: `int toVersion13Index(int targetIndex) const`
  The part-block index in the version 13 layout, before the ten absolute envelope slots were inserted.
- `toVersion13aIndex` at line 187 through 187: `int toVersion13aIndex(int targetIndex) const`
  The part-block index in the intermediate version 14 layout, after the spectral envelope blocks changed but before the filter block did.
- `migratePartBank` at line 193 through 194: `bool migratePartBank(const uint8_t* data, uint32_t oldCount, std::vector<uint8_t>& out) const`
  Expand a part bank saved with `oldCount` params per record to the current count. The chain runs version 10 operator blocks, the version 11 pan reorder, the version 12 offset envelope insert, and the version 13 filter and spectral insert.

### `plugin/preset/part_defaults.h`

- `partParamDefaults` at line 12 through 19: `inline std::vector<int32_t> partParamDefaults()`
  Returns the part param defaults value.

### `plugin/preset/presets_host.c`

- `slot_save` at line 8 through 14: `int slot_save(uint32_t i, const void *p, uint32_t n)`
  Writes a bank or record through the host store.
- `slot_load` at line 16 through 22: `int slot_load(uint32_t i, void *p, uint32_t n)`
  Reads a bank, record or library file and migrates it.
- `settings_save` at line 24 through 29: `int settings_save(const void *p, uint32_t n)`
  Writes a bank or record through the host store.
- `settings_load` at line 31 through 36: `int settings_load(void *p, uint32_t n)`
  Reads a bank, record or library file and migrates it.

### `plugin/tests/additive_bench.cpp`

- `secondsSince` at line 31 through 35: `double secondsSince(std::chrono::steady_clock::time_point start)`
  Returns the seconds since value.
- `clampQ15` at line 37 through 40: `int16_t clampQ15(int32_t value)`
  Clamps the q 15.
- `sineTable` at line 42 through 51: `std::vector<int16_t> sineTable(int size)`
  Returns the sine table value.
- `recipe` at line 53 through 73: `std::vector<int16_t> recipe(int harmonics, double tilt, int formant, double peak)`
  Returns the recipe value.
- `generateTable` at line 75 through 93: `void generateTable(std::vector<int16_t>& table, int harmonics, int size, const std::vector<int16_t>& amps, const std::vector<int16_t>& sine, int updates, uint64_t& checksum)`
  Runs the generate table step for the surrounding code.
- `baselineSeconds` at line 95 through 113: `double baselineSeconds()`
  Returns the baseline seconds value.
- `readModelSeconds` at line 115 through 152: `double readModelSeconds(int tableSize)`
  Reads the model seconds capture.
- `readTable` at line 154 through 161: `double readTable(const std::vector<int16_t>& table, double phase)`
  Reads the table capture.
- `writeWav` at line 163 through 198: `void writeWav(const std::string& path, const std::vector<int16_t>& left, const std::vector<int16_t>& right)`
  Writes the WAV capture.
- `audition` at line 200 through 243: `void audition(const std::string& path)`
  Runs the audition step for the surrounding code.
- `mipLevels` at line 251 through 257: `int mipLevels(int harmonics)`
  Returns the mip levels value.
- `mipFor` at line 259 through 265: `int mipFor(double fundamental, int harmonics, int levels)`
  Returns the mip for value.
- `buildWaveTable` at line 267 through 284: `WaveTable buildWaveTable(int harmonics, int size, const std::vector<int16_t>& amps)`
  Builds the wave table.
- `readWave` at line 286 through 298: `double readWave(const WaveTable& table, int mip, double phase, bool linear)`
  Reads the wave capture.
- `magnitude` at line 300 through 314: `double magnitude(const std::vector<double>& samples, double frequency)`
  Returns the magnitude value.
- `aliasFloor` at line 316 through 345: `double aliasFloor(const WaveTable& table, int mip, bool linear, double fundamental, int firstAlias, int harmonics)`
  Returns the alias floor value.
- `aliasQuality` at line 347 through 372: `void aliasQuality()`
  Runs the alias quality step for the surrounding code.
- `mipForBright` at line 374 through 380: `int mipForBright(double fundamental, int harmonics, int levels)`
  Returns the mip for bright value.
- `harmonicEnergy` at line 382 through 406: `double harmonicEnergy(const WaveTable& table, int mip, bool linear, double fundamental, int first, int last)`
  Returns the harmonic energy value.
- `tableError` at line 408 through 441: `double tableError(const WaveTable& table, int mip, bool linear, double fundamental, const std::vector<int16_t>& amps, int limit)`
  Returns the table error value.
- `qualityExtras` at line 443 through 477: `void qualityExtras()`
  Runs the quality extras step for the surrounding code.
- `oscillatorSeconds` at line 479 through 532: `double oscillatorSeconds(bool additive)`
  Returns the oscillator seconds value.
- `auditionOscillator` at line 534 through 591: `void auditionOscillator(const std::string& path)`
  Runs the audition oscillator step for the surrounding code.
- `main` at line 595 through 667: `int main(int argc, char** argv)`
  Runs the test or tool entry point.

### `plugin/tests/additive_engine_bench.cpp`

- `secondsSince` at line 28 through 32: `double secondsSince(std::chrono::steady_clock::time_point start)`
  Returns the seconds since value.
- `setEngineParam` at line 34 through 43: `bool setEngineParam(p1::Engine& engine, const char* id, int32_t value)`
  Writes the engine param.
- `writeWav` at line 45 through 85: `void writeWav(const std::string& path, const std::vector<int32_t>& samples)`
  Writes the WAV capture.
- `main` at line 89 through 155: `int main(int argc, char** argv)`
  Runs the test or tool entry point.

### `plugin/tests/editor_snapshot.cpp`

- `saveSnapshot` at line 16 through 28: `bool saveSnapshot(juce::AudioProcessorEditor& editor, const juce::File& file)`
  Saves the snapshot.
- `main` at line 32 through 933: `int main(int argc, char** argv)`
  Runs the test or tool entry point.

### `plugin/tests/engine_parity.cpp`

- `midi_get` at line 18 through 21: `int midi_get(uint8_t*, uint8_t*, uint8_t*)`
  Returns the MIDI get value.
- `check` at line 27 through 32: `void check(bool ok, const char* what)`
  Records a test assertion.
- `same` at line 34 through 37: `bool same(const int32_t* a, const int32_t* b, std::size_t n)`
  Compares a rendered result with its reference.
- `param_id` at line 39 through 46: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `test_engine_render` at line 48 through 63: `void test_engine_render()`
  Runs the `test_engine_render` case.
- `test_state_isolation` at line 65 through 91: `void test_state_isolation()`
  Runs the `test_state_isolation` case.
- `test_filter_is_per_part` at line 93 through 154: `void test_filter_is_per_part()`
  Runs the `test_filter_is_per_part` case.
- `filter_rms` at line 156 through 197: `double filter_rms(int32_t resonance, bool carriers)`
  Returns the filter RMS value.
- `test_filter_output_gate` at line 199 through 212: `void test_filter_output_gate()`
  Runs the `test_filter_output_gate` case.
- `test_sustain_isolation` at line 214 through 235: `void test_sustain_isolation()`
  Runs the `test_sustain_isolation` case.
- `test_channel_separation` at line 237 through 258: `void test_channel_separation()`
  Runs the `test_channel_separation` case.
- `test_sustain_per_channel` at line 260 through 281: `void test_sustain_per_channel()`
  Runs the `test_sustain_per_channel` case.
- `test_all_notes_off_per_channel` at line 283 through 307: `void test_all_notes_off_per_channel()`
  Runs the `test_all_notes_off_per_channel` case.
- `test_bend_per_part` at line 309 through 334: `void test_bend_per_part()`
  Runs the `test_bend_per_part` case.
- `test_pan_per_part` at line 336 through 357: `void test_pan_per_part()`
  Runs the `test_pan_per_part` case.
- `test_pan_envelope` at line 359 through 385: `void test_pan_envelope()`
  Runs the `test_pan_envelope` case.
- `main` at line 389 through 491: `int main(int argc, char** argv)`
  Runs the test or tool entry point.

### `plugin/tests/feedback_bipolar.cpp`

- `check` at line 26 through 31: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 33 through 40: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 42 through 51: `bool set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `setup` at line 53 through 83: `bool setup(p1::Engine& engine, int32_t feedback, int32_t envLevel)`
  Prepares or releases the shared test fixtures.
- `magnitudeAt` at line 92 through 106: `double magnitudeAt(const std::vector<int32_t>& samples, double frequency, const std::vector<double>& window, double windowSum)`
  Returns the magnitude at value.
- `analyse` at line 108 through 143: `Spectrum analyse(const std::vector<int32_t>& samples)`
  Returns the analyse value.
- `render` at line 145 through 155: `bool render(int32_t feedback, int32_t envLevel, std::vector<int32_t>& samples)`
  Tests the render condition.
- `ratio` at line 157 through 163: `double ratio(const Spectrum& s, int numerator, int denominator)`
  Returns the ratio value.
- `main` at line 167 through 230: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/feedback_envelope.cpp`

- `check` at line 26 through 31: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 33 through 40: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 42 through 51: `bool set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `setup` at line 53 through 89: `bool setup(p1::Engine& engine, int32_t feedback, int32_t envL1, int32_t envR1, int32_t on)`
  Prepares or releases the shared test fixtures.
- `analyseWindow` at line 96 through 128: `Window analyseWindow(const std::vector<int32_t>& samples, int start)`
  Analyses the window.
- `peakOf` at line 130 through 141: `double peakOf(const std::vector<int32_t>& samples)`
  Returns the peak of value.
- `render` at line 143 through 156: `bool render(int32_t feedback, int32_t envL1, int32_t envR1, int32_t on, std::vector<int32_t>& samples, bool* negative = nullptr)`
  Tests the render condition.
- `main` at line 160 through 256: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/filter_envelope.cpp`

- `check` at line 20 through 25: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 27 through 34: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 36 through 44: `void set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `setup` at line 46 through 71: `void setup(p1::Engine& engine, int32_t r1)`
  Prepares or releases the shared test fixtures.
- `run` at line 73 through 83: `int32_t run(int32_t r1, int blocks)`
  Runs one measured case and returns its result.
- `main` at line 86 through 110: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/filter_law.cpp`

- `check` at line 14 through 19: `void check(bool ok, const char* what)`
  Records a test assertion.
- `res_k` at line 21 through 25: `int32_t res_k(int32_t res)`
  Returns the res k value.
- `main` at line 29 through 86: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/lfo_fades.cpp`

- `check` at line 31 through 36: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 38 through 45: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 47 through 56: `bool set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `base_setup` at line 58 through 86: `bool base_setup(p1::Engine& engine)`
  Tests the base setup condition.
- `trem_setup` at line 88 through 97: `bool trem_setup(p1::Engine& engine, int32_t depth, int32_t rateFade, int32_t depthFade)`
  Tests the trem setup condition.
- `voice_of` at line 99 through 107: `int voice_of(p1::Engine& engine, uint32_t note)`
  Returns the voice of value.
- `tick` at line 109 through 113: `void tick(p1::Engine& engine)`
  Refreshes the live display state.
- `render` at line 115 through 119: `void render(p1::Engine& engine, int frames, std::vector<int32_t>& pcm)`
  Runs the render step for the surrounding code.
- `run_ramp` at line 128 through 153: `bool run_ramp(int32_t rateFade, int32_t depthFade, Ramp& ramp)`
  Tests the run ramp condition.
- `near` at line 155 through 158: `bool near(int value, int expected, int slack)`
  Tests the near condition.
- `rms` at line 160 through 168: `double rms(const std::vector<int32_t>& pcm, int startFrame, int frames)`
  Returns the RMS value.
- `spread` at line 170 through 190: `double spread(const std::vector<int32_t>& pcm, int startFrame, int endFrame)`
  Returns the spread value.
- `same` at line 192 through 196: `bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)`
  Compares a rendered result with its reference.
- `all_family_levels` at line 198 through 219: `bool all_family_levels(p1::Engine& engine, uint32_t& rate, uint32_t& depth)`
  Tests the all family levels condition.
- `main` at line 223 through 451: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/operator_lfos.cpp`

- `check` at line 17 through 22: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 24 through 31: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 33 through 42: `bool set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `setup` at line 44 through 80: `bool setup(p1::Engine& engine, int32_t feedbackLfoDepth, int32_t detuneLfoDepth, int32_t feedbackLfoRate, int32_t detuneLfoRate)`
  Prepares or releases the shared test fixtures.
- `render` at line 82 through 95: `bool render(int32_t feedbackLfoDepth, int32_t detuneLfoDepth, int32_t feedbackLfoRate, int32_t detuneLfoRate, std::vector<int32_t>& samples)`
  Tests the render condition.
- `same` at line 97 through 101: `bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)`
  Compares a rendered result with its reference.
- `main` at line 105 through 139: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/phase_map.cpp`

- `check` at line 52 through 57: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 59 through 66: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 68 through 77: `bool set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `base_setup` at line 79 through 105: `bool base_setup(p1::Engine& engine)`
  Tests the base setup condition.
- `pm` at line 107 through 118: `uint32_t* pm(p1::Engine& engine, int op, int part)`
  Returns the pm value.
- `lfo_pool` at line 120 through 131: `uint32_t* lfo_pool(p1::Engine& engine, int op, int part)`
  Returns the LFO pool value.
- `env_pool` at line 133 through 144: `uint32_t* env_pool(p1::Engine& engine, int op, int part)`
  Returns the env pool value.
- `map_on` at line 146 through 156: `bool map_on(p1::Engine& engine)`
  Maps the on.
- `tick` at line 158 through 162: `void tick(p1::Engine& engine)`
  Refreshes the live display state.
- `render` at line 164 through 168: `void render(p1::Engine& engine, int frames, std::vector<int32_t>& pcm)`
  Runs the render step for the surrounding code.
- `same` at line 170 through 174: `bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)`
  Compares a rendered result with its reference.
- `sorted_x` at line 176 through 183: `bool sorted_x(const uint32_t* p, uint32_t active)`
  Sorts the ed x.
- `permutes` at line 185 through 197: `bool permutes(const uint32_t* p, uint32_t active)`
  Tests the permutes condition.
- `mask_nonempty` at line 199 through 205: `bool mask_nonempty(const uint32_t* p)`
  Tests the mask nonempty condition.
- `locked_flip_free` at line 207 through 218: `bool locked_flip_free(const uint32_t* p)`
  Tests the locked flip free condition.
- `wait_event` at line 220 through 232: `void wait_event(p1::Engine& engine, uint32_t* p, uint32_t accIndex, int maxTicks, p1::PhaseMapSnapshot* snap = nullptr)`
  Runs the wait event step for the surrounding code.
- `rms` at line 234 through 242: `double rms(const std::vector<int32_t>& pcm)`
  Returns the RMS value.
- `main` at line 246 through 946: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/phase_map_perceptual.cpp`

- `midiHz` at line 81 through 84: `double midiHz(int note)`
  Returns the MIDI Hz value.
- `rateQ32` at line 86 through 94: `uint32_t rateQ32(double hz)`
  Returns the rate q 32 value.
- `paramId` at line 96 through 103: `uint32_t paramId(const char* name)`
  Returns the param id value.
- `setParam` at line 105 through 114: `bool setParam(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `setPart` at line 116 through 121: `bool setPart(p1::Engine& engine, int part, const char* stem, int32_t value)`
  Writes the part.
- `configurePart` at line 123 through 200: `bool configurePart(p1::Engine& engine, int part, const Scenario& s)`
  Tests the configure part condition.
- `pool` at line 202 through 206: `uint32_t* pool(p1::Engine& engine, int part)`
  Returns the pool value.
- `mixWord` at line 208 through 215: `uint64_t mixWord(uint64_t h, uint32_t w)`
  Mixes the word.
- `geometryHash` at line 219 through 228: `uint64_t geometryHash(const uint32_t* pm)`
  The geometry hash excludes the table (which differs by design) and the per-block clocks, but keeps the seed, so a stochastic event shows up.
- `exactY` at line 230 through 249: `uint32_t exactY(const uint32_t* pm, uint32_t x)`
  Returns the exact y value.
- `tableY` at line 251 through 291: `uint32_t tableY(const uint32_t* pm, uint32_t x, int mode)`
  Returns the table y value.
- `transferError` at line 299 through 315: `Transfer transferError(const uint32_t* pm, int mode)`
  Returns the transfer error value.
- `exactYq` at line 319 through 337: `uint32_t exactYq(const uint32_t* pm, uint32_t q)`
  The post-F map y(q) is piecewise linear. A table in q does not inherit the F compression at extreme breakpoints. This is the audit for that variant.
- `qTransferError` at line 339 through 368: `Transfer qTransferError(const uint32_t* pm, uint32_t n, uint32_t step, bool linear)`
  Returns the q transfer error value.
- `writeQAudit` at line 370 through 388: `void writeQAudit(std::FILE* f, const char* id, int block, int part, const uint32_t* pm)`
  Writes the q audit capture.
- `actualQError` at line 392 through 427: `Transfer actualQError(const uint32_t* pm, int mode)`
  Read the compiled table back and compare it with the exact q map. This catches a rebuild that was skipped, which a model-only audit cannot see.
- `buildScenarios` at line 429 through 656: `std::vector<Scenario> buildScenarios()`
  Builds the scenarios.
- `main` at line 660 through 853: `int main(int argc, char** argv)`
  Runs the test or tool entry point.

### `plugin/tests/phase_map_perceptual_compare.cpp`

- `split` at line 100 through 114: `std::vector<std::string> split(const std::string& line)`
  Returns the split value.
- `readManifest` at line 116 through 184: `bool readManifest(const std::string& dir, Manifest& m)`
  Reads the manifest capture.
- `readPcm` at line 186 through 204: `bool readPcm(const std::string& dir, const std::string& id, std::vector<int32_t>& out, int frames, int channels)`
  Reads the PCM capture.
- `compareLogs` at line 206 through 240: `bool compareLogs(const std::string& refDir, const std::string& candDir, const char* name)`
  Tests the compare logs condition.
- `Fft` at line 244 through 258: `explicit Fft(int n) : n_(n), rev_(static_cast<std::size_t>(n)), tw_(static_cast<std::size_t>(n))`
  Constructs the object.
- `run` at line 260 through 283: `void run(std::vector<std::complex<double>>& a) const`
  Runs one measured case and returns its result.
- `percentile` at line 291 through 299: `double percentile(std::vector<double> v, double q)`
  Returns the percentile value.
- `toDb` at line 301 through 304: `double toDb(double power, double floorPower)`
  Returns the to dB value.
- `bandCenters` at line 324 through 330: `std::vector<double> bandCenters()`
  One-third-octave centers from 25 Hz to 20 kHz.
- `analyze` at line 332 through 468: `Analysis analyze(const std::vector<int32_t>& pcm, int frames, int channels, int sr, double f0, bool harmonics)`
  Returns the analyze value.
- `rmsOf` at line 470 through 476: `double rmsOf(const std::vector<int32_t>& pcm)`
  Returns the RMS of value.
- `peakOf` at line 478 through 484: `double peakOf(const std::vector<int32_t>& pcm)`
  Returns the peak of value.
- `crossing` at line 486 through 513: `bool crossing(const std::vector<double>& env, int from, int to, double level, bool rising, double blockMs, double& outMs)`
  Tests the crossing condition.
- `compareScenario` at line 515 through 729: `Metrics compareScenario(const Scenario& s, const std::vector<int32_t>& ref, const std::vector<int32_t>& cand, int sr, int channels)`
  Returns the compare scenario value.
- `metricFails` at line 731 through 770: `bool metricFails(const Metrics& m, const Scenario& s, const char*& what, double& value, double& limit)`
  Tests the metric fails condition.
- `gateScore` at line 772 through 789: `double gateScore(const Metrics& m)`
  Returns the gate score value.
- `printScenario` at line 791 through 801: `void printScenario(const char* tag, const Scenario& s, const Metrics& m)`
  Runs the print scenario step for the surrounding code.
- `main` at line 805 through 910: `int main(int argc, char** argv)`
  Runs the test or tool entry point.

### `plugin/tests/pitch_envelope.cpp`

- `check` at line 20 through 25: `void check(bool ok, const char* what)`
  Records a test assertion.
- `param_id` at line 27 through 34: `uint32_t param_id(const char* name)`
  Returns the param id value.
- `set_param` at line 36 through 45: `bool set_param(p1::Engine& engine, const char* name, int32_t value)`
  Writes the param.
- `setup` at line 47 through 77: `bool setup(p1::Engine& engine, int32_t l1, int32_t l2, int32_t l3, int32_t relLevel, int32_t mode, int32_t r1 = 99)`
  Prepares or releases the shared test fixtures.
- `render` at line 79 through 87: `void render(p1::Engine& engine, int blocks, int32_t* level)`
  Runs the render step for the surrounding code.
- `main` at line 91 through 178: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/plugin_smoke.cpp`

- `check` at line 32 through 37: `void check(bool ok, const char* what)`
  Records a test assertion.
- `bitEqual` at line 39 through 42: `bool bitEqual(float a, float b)`
  Tests the bit equal condition.
- `renderReference` at line 44 through 56: `void renderReference(int32_t* out, int frames, int noteOffset, int32_t algorithm)`
  Renders the reference.
- `matches` at line 58 through 71: `bool matches(const juce::AudioBuffer<float>& buffer, const int32_t* reference, int frames)`
  Compares a rendered result with its reference.
- `main` at line 75 through 1104: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/plugin_stability.cpp`

- `check` at line 20 through 25: `void check(bool ok, const char* what)`
  Records a test assertion.
- `render` at line 32 through 49: `Peak render(TypePAlphaAudioProcessor& processor, juce::MidiBuffer& midi, int blocks)`
  Returns the render value.
- `setAll` at line 51 through 61: `void setAll(TypePAlphaAudioProcessor& processor, bool maximum)`
  Writes the all.
- `main` at line 65 through 256: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/preset_smoke.cpp`

- `check` at line 25 through 30: `void check(bool ok, const char* what)`
  Records a test assertion.
- `pattern` at line 32 through 38: `std::vector<int32_t> pattern(int seed)`
  Returns the pattern value.
- `setRecordVersion` at line 42 through 50: `void setRecordVersion(uint8_t* record, uint32_t count, uint8_t version)`
  `preset_part_pack` seals a current-version record. An older record needs the version byte and then a fresh CRC.
- `lfoFadeIndices` at line 52 through 66: `std::array<int, 34> lfoFadeIndices()`
  Returns the LFO fade indices value.
- `main` at line 70 through 1008: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/rate_bridge_test.cpp`

- `amplitude` at line 24 through 40: `double amplitude(const std::vector<float>& samples, int start, int count, double frequency, double sampleRate)`
  Returns the amplitude value.
- `rms` at line 42 through 50: `double rms(const std::vector<float>& samples, int start, int count)`
  Returns the RMS value.
- `run` at line 52 through 91: `Measurement run(double hostRate, double frequency, int hostFrames, int blockSize)`
  Runs one measured case and returns its result.
- `run` at line 93 through 96: `Measurement run(double hostRate, double frequency, int hostFrames)`
  Runs one measured case and returns its result.
- `imageFrequencies` at line 98 through 118: `std::vector<double> imageFrequencies(double hostRate, double frequency)`
  Returns the image frequencies value.
- `check` at line 120 through 124: `bool check(bool condition, const char* what)`
  Records a test assertion.
- `main` at line 128 through 221: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/spectral.cpp`

- `setParam` at line 24 through 34: `bool setParam(p1::Engine& engine, const char* id, int32_t value)`
  Writes the param.
- `setAll` at line 36 through 43: `bool setAll(p1::Engine& engine, const std::vector<std::pair<const char*, int32_t>>& values)`
  Writes the all.
- `magnitude` at line 45 through 64: `double magnitude(const std::vector<int32_t>& samples, int start, int count, double frequency)`
  Returns the magnitude value.
- `setupCarrier` at line 66 through 81: `bool setupCarrier(p1::Engine& engine)`
  Writes the up carrier.
- `render` at line 83 through 88: `std::vector<int32_t> render(p1::Engine& engine, int frames = kFrames)`
  Returns the render value.
- `checkMorph` at line 90 through 123: `bool checkMorph()`
  Checks the morph result.
- `checkMorphEnvelope` at line 125 through 204: `bool checkMorphEnvelope()`
  Checks the morph envelope result.
- `checkMorphLfo` at line 206 through 236: `bool checkMorphLfo()`
  Checks the morph LFO result.
- `checkColdCommit` at line 238 through 268: `bool checkColdCommit()`
  Checks the cold commit result.
- `checkCrossfadeDirection` at line 270 through 308: `bool checkCrossfadeDirection()`
  Checks the crossfade direction result.
- `checkTable` at line 312 through 334: `bool checkTable()`
  Checks the table result.
- `checkMip` at line 336 through 347: `bool checkMip()`
  Checks the mip result.
- `checkRange` at line 349 through 373: `bool checkRange()`
  Checks the range result.
- `main` at line 375 through 384: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/spectral_bench.cpp`

- `secondsSince` at line 24 through 28: `double secondsSince(std::chrono::steady_clock::time_point start)`
  Returns the seconds since value.
- `runTickSeries` at line 36 through 73: `TickResult runTickSeries(int ticks, bool reads)`
  Returns the run tick series value.
- `setParam` at line 75 through 84: `bool setParam(p1::Engine& engine, const char* id, int32_t value)`
  Writes the param.
- `setAll` at line 86 through 91: `void setAll(p1::Engine& engine, const std::vector<std::pair<const char*, int32_t>>& values)`
  Writes the all.
- `setupSpectrumEngine` at line 94 through 110: `void setupSpectrumEngine(p1::Engine& engine)`
  Four operators, both parts, spectrum mode, one clean carrier per part.
- `runEngineChanges` at line 119 through 168: `BlockResult runEngineChanges(int blocks, int parts)`
  parts is the number of parts with notes: 0, 1 or 2.
- `generateWide` at line 173 through 197: `void generateWide(int16_t *out, const int32_t *levels, int first, int size, int shift)`
  A 64-harmonic spectrum with seven mips (64, 32, 16, 8, 4, 2, 1). This is the production layout. The bench keeps a local copy so the 512-sample comparison does not depend on the engine build.
- `main` at line 201 through 265: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/ui_frame_bench.cpp`

- `makeSignal` at line 24 through 36: `std::array<float, 512> makeSignal()`
  Creates the signal.
- `measure` at line 39 through 50: `double measure(Fn&& fn)`
  Warm the caches and the FFT plan.
- `measureEditor` at line 52 through 71: `double measureEditor(int page)`
  Measures the editor path.
- `measureComponent` at line 73 through 87: `double measureComponent(juce::Component& component, int frames)`
  Measures the component path.
- `reportTree` at line 89 through 106: `void reportTree(juce::Component& root, int depth)`
  Runs the report tree step for the surrounding code.
- `reportEditorTree` at line 108 through 119: `void reportEditorTree(int page)`
  Runs the report editor tree step for the surrounding code.
- `main` at line 123 through 186: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/voice_alloc.cpp`

- `check` at line 16 through 21: `void check(bool ok, const char* what)`
  Records a test assertion.
- `chord` at line 23 through 44: `bool chord(p1::Engine& engine, uint32_t channel, int& gated, bool* notes)`
  Tests the chord condition.
- `main` at line 48 through 155: `int main()`
  Runs the test or tool entry point.

### `plugin/tests/wavetable.cpp`

- `fnv` at line 17 through 25: `uint64_t fnv(const int16_t* data, size_t count)`
  Returns the fnv value.
- `checkBase` at line 27 through 39: `bool checkBase()`
  Checks the base result.
- `checkHarmonics` at line 41 through 72: `bool checkHarmonics()`
  Checks the harmonics result.
- `checkSamples` at line 74 through 119: `bool checkSamples()`
  Checks the samples result.
- `main` at line 123 through 129: `int main()`
  Runs the test or tool entry point.

### `plugin/tools/additive_prototype.py`

- `_recipe` at line 25 through 36: `def _recipe(harmonics: int, tilt: float, formant: int, peak: float)`
  Implements the recipe helper.
- `_table` at line 39 through 48: `def _table(amps: list[float], level: int)`
  Implements the table helper.
- `_emit` at line 51 through 57: `def _emit(name: str, table: list[int])`
  Implements the emit helper.
- `_header` at line 60 through 110: `def _header()`
  Implements the header helper.
- `_patch` at line 113 through 154: `def _patch(source: str)`
  Implements the patch helper.
- `replace` at line 120 through 128: `def replace(match: re.Match[str])`
  Implements the replace helper.
- `main` at line 157 through 170: `def main()`
  Runs the test or tool entry point.

### `plugin/tools/feedback_figures.py`

- `_read_wav` at line 41 through 47: `def _read_wav(path: Path)`
  Implements the read WAV helper.
- `_chunk` at line 50 through 56: `def _chunk(tag: bytes, data: bytes)`
  Implements the chunk helper.
- `_write_png` at line 59 through 68: `def _write_png(path: Path, width: int, height: int, rows: list[bytearray])`
  Implements the write png helper.
- `_pixel` at line 71 through 76: `def _pixel(row: bytearray, x: int, y: int, colour: tuple[int, int, int])`
  Implements the pixel helper.
- `_panel` at line 79 through 102: `def _panel(samples: list[int])`
  Implements the panel helper.
- `main` at line 105 through 120: `def main()`
  Runs the test or tool entry point.

### `plugin/tools/gen_macro_targets.py`

- `op_env_rates` at line 20 through 56: `def op_env_rates(op: int)`
  The amp, feedback, morph and phase rate names for one operator.
- `c_string` at line 92 through 93: `def c_string(value: str)`
  Implements the c string helper.
- `is_ring_param` at line 96 through 99: `def is_ring_param(name: str)`
  Tests whether the the ring param.
- `reorder_ring` at line 102 through 131: `def reorder_ring(params: list[dict[str, Any]])`
  Move the ringmod part rows to the end of each part group.
- `move` at line 123 through 125: `def move(group: list[dict[str, Any]])`
  Implements the move helper.
- `load_carriers` at line 134 through 147: `def load_carriers(tables_path: Path)`
  Read the carrier masks from the generated routing table.
- `union_members` at line 150 through 166: `def union_members(entry: dict[str, Any], part_suffix: str)`
  Every member any algorithm can select, with its operator index.
- `main` at line 169 through 427: `def main()`
  Runs the test or tool entry point.

### `plugin/tools/gen_params.py`

- `display_name` at line 43 through 49: `def display_name(name: str)`
  Implements the display name helper.
- `is_ring_param` at line 52 through 55: `def is_ring_param(name: str)`
  Tests whether the the ring param.
- `reorder_ring` at line 58 through 89: `def reorder_ring(params: list[ParamDoc])`
  Move the ringmod part rows to the end of each part group.
- `move` at line 81 through 83: `def move(group: list[ParamDoc])`
  Implements the move helper.
- `reorder_key` at line 92 through 127: `def reorder_key(params: list[ParamDoc])`
  Move the key-follow part rows to the end of each part group.
- `is_key` at line 101 through 102: `def is_key(name: str)`
  Tests whether the the key.
- `move` at line 119 through 121: `def move(group: list[ParamDoc])`
  Implements the move helper.
- `c_string` at line 130 through 131: `def c_string(value: str)`
  Implements the c string helper.
- `main` at line 134 through 246: `def main()`
  Runs the test or tool entry point.

### `plugin/tools/gen_tokens.py`

- `_argb` at line 24 through 28: `def _argb(hex_colour: str)`
  Implements the argb helper.
- `main` at line 31 through 90: `def main()`
  Runs the test or tool entry point.

### `plugin/tools/ratio.py`

- `_display` at line 26 through 30: `def _display(value: int)`
  The 2-decimal readout and the ideal it was rounded from.
- `ratio_detents` at line 33 through 50: `def ratio_detents(lo: int, hi: int)`
  The sorted Q16 ratio detents in [lo, hi]. Two series that meet inside the

### `plugin/tools/snapshot_check.py`

- `_sha256` at line 29 through 34: `def _sha256(path: Path)`
  Implements the sha 256 helper.
- `_png_size` at line 37 through 44: `def _png_size(path: Path)`
  Read the IHDR width and height from a PNG without decoding it.
- `_images` at line 47 through 50: `def _images(directory: Path)`
  Implements the images helper.
- `_load_manifest` at line 53 through 61: `def _load_manifest(golden: Path)`
  Implements the load manifest helper.
- `_write_manifest` at line 64 through 76: `def _write_manifest(golden: Path, images: dict[str, Path])`
  Implements the write manifest helper.
- `_diff_images` at line 79 through 114: `def _diff_images(actual: Path, golden: Path, out: Path, names: list[str])`
  Write review images for changed snapshots when Pillow is installed.
- `main` at line 117 through 203: `def main()`
  Runs the test or tool entry point.

### `plugin/ui/ParamIDs.h`

- `partSuffix` at line 10 through 13: `inline juce::String partSuffix(int part)`
  Returns the `.pN` suffix for a part index.
- `withPart` at line 15 through 18: `inline juce::String withPart(const juce::String& base, int part)`
  Appends the part suffix to a base parameter id.
- `algorithm` at line 20 through 20: `inline juce::String algorithm(int part)`
  Returns the `algorithm` parameter id with the part suffix.
- `channel` at line 21 through 21: `inline juce::String channel(int part)`
  Returns the `channel` parameter id with the part suffix.
- `pan` at line 22 through 22: `inline juce::String pan(int part)`
  Returns the `pan` parameter id with the part suffix.
- `panEnvOn` at line 23 through 23: `inline juce::String panEnvOn(int part)`
  Returns the `pan_env_on` parameter id with the part suffix.
- `panEnvRelRate` at line 24 through 27: `inline juce::String panEnvRelRate(int part)`
  Returns the `pan_env_relr` parameter id with the part suffix.
- `panEnvRelLevel` at line 28 through 31: `inline juce::String panEnvRelLevel(int part)`
  Returns the `pan_env_rell` parameter id with the part suffix.
- `panEnvMode` at line 32 through 32: `inline juce::String panEnvMode(int part)`
  Returns the `pan_env_mode` parameter id with the part suffix.
- `panEnvRate` at line 33 through 36: `inline juce::String panEnvRate(int index, int part)`
  Returns the `pan_env_r` parameter id with the part suffix.
- `panEnvLevel` at line 37 through 40: `inline juce::String panEnvLevel(int index, int part)`
  Returns the `pan_env_l` parameter id with the part suffix.
- `bendRange` at line 41 through 41: `inline juce::String bendRange(int part)`
  Returns the `bend_range` parameter id with the part suffix.
- `keyLow` at line 42 through 42: `inline juce::String keyLow(int part)`
  Returns the `low` parameter id with the part suffix.
- `keyHigh` at line 43 through 43: `inline juce::String keyHigh(int part)`
  Returns the `high` parameter id with the part suffix.
- `level` at line 44 through 44: `inline juce::String level(int part)`
  Returns the `level` parameter id with the part suffix.
- `delay` at line 45 through 45: `inline juce::String delay(int part)`
  Returns the `delay` parameter id with the part suffix.
- `detune` at line 46 through 46: `inline juce::String detune(int part)`
  Returns the `detune` parameter id with the part suffix.
- `transpose` at line 47 through 47: `inline juce::String transpose(int part)`
  Returns the `transpose` parameter id with the part suffix.
- `mode` at line 48 through 48: `inline juce::String mode(int part)`
  Returns the `mode` parameter id with the part suffix.
- `legato` at line 49 through 49: `inline juce::String legato(int part)`
  Returns the `legato` parameter id with the part suffix.
- `unisonCount` at line 50 through 50: `inline juce::String unisonCount(int part)`
  Returns the `unison_count` parameter id with the part suffix.
- `unisonDetune` at line 51 through 51: `inline juce::String unisonDetune(int part)`
  Returns the `unison_detune` parameter id with the part suffix.
- `portamento` at line 52 through 52: `inline juce::String portamento(int part)`
  Returns the `portamento` parameter id with the part suffix.
- `notePriority` at line 53 through 53: `inline juce::String notePriority(int part)`
  Returns the `note_priority` parameter id with the part suffix.
- `opBase` at line 55 through 58: `inline juce::String opBase(int op, const char* field)`
  Builds an operator parameter id from the operator index and field.
- `opRatio` at line 60 through 63: `inline juce::String opRatio(int op, int part)`
  Builds the operator parameter id for `_ratio`.
- `opDetune` at line 64 through 67: `inline juce::String opDetune(int op, int part)`
  Builds the operator parameter id for `_detune`.
- `opWaveform` at line 68 through 71: `inline juce::String opWaveform(int op, int part)`
  Builds the operator parameter id for `_waveform`.
- `opLevel` at line 72 through 75: `inline juce::String opLevel(int op, int part)`
  Builds the operator parameter id for `_level`.
- `opFeedback` at line 76 through 79: `inline juce::String opFeedback(int op, int part)`
  Builds the operator parameter id for `_feedback`.
- `opFeedbackOn` at line 80 through 83: `inline juce::String opFeedbackOn(int op, int part)`
  Builds the operator parameter id for `_fbenv_on`.
- `opFeedbackLfoRate` at line 84 through 87: `inline juce::String opFeedbackLfoRate(int op, int part)`
  Builds the operator parameter id for `_fblfo_rate`.
- `opFeedbackLfoDepth` at line 88 through 91: `inline juce::String opFeedbackLfoDepth(int op, int part)`
  Builds the operator parameter id for `_fblfo_depth`.
- `opFeedbackLfoWave` at line 92 through 95: `inline juce::String opFeedbackLfoWave(int op, int part)`
  Builds the operator parameter id for `_fblfo_wave`.
- `opFeedbackLfoMode` at line 96 through 99: `inline juce::String opFeedbackLfoMode(int op, int part)`
  Builds the operator parameter id for `_fblfo_mode`.
- `opFeedbackLfoRateFade` at line 100 through 103: `inline juce::String opFeedbackLfoRateFade(int op, int part)`
  Builds the operator parameter id for `_fblfo_rate_fade`.
- `opFeedbackLfoDepthFade` at line 104 through 107: `inline juce::String opFeedbackLfoDepthFade(int op, int part)`
  Builds the operator parameter id for `_fblfo_depth_fade`.
- `opDetuneLfoRate` at line 108 through 111: `inline juce::String opDetuneLfoRate(int op, int part)`
  Builds the operator parameter id for `_dlfo_rate`.
- `opDetuneLfoDepth` at line 112 through 115: `inline juce::String opDetuneLfoDepth(int op, int part)`
  Builds the operator parameter id for `_dlfo_depth`.
- `opDetuneLfoWave` at line 116 through 119: `inline juce::String opDetuneLfoWave(int op, int part)`
  Builds the operator parameter id for `_dlfo_wave`.
- `opDetuneLfoMode` at line 120 through 123: `inline juce::String opDetuneLfoMode(int op, int part)`
  Builds the operator parameter id for `_dlfo_mode`.
- `opDetuneLfoRateFade` at line 124 through 127: `inline juce::String opDetuneLfoRateFade(int op, int part)`
  Builds the operator parameter id for `_dlfo_rate_fade`.
- `opDetuneLfoDepthFade` at line 128 through 131: `inline juce::String opDetuneLfoDepthFade(int op, int part)`
  Builds the operator parameter id for `_dlfo_depth_fade`.
- `opTremRate` at line 132 through 135: `inline juce::String opTremRate(int op, int part)`
  Builds the operator parameter id for `_trem_rate`.
- `opTremDepth` at line 136 through 139: `inline juce::String opTremDepth(int op, int part)`
  Builds the operator parameter id for `_trem_depth`.
- `opTremWave` at line 140 through 143: `inline juce::String opTremWave(int op, int part)`
  Builds the operator parameter id for `_trem_wave`.
- `opTremMode` at line 144 through 147: `inline juce::String opTremMode(int op, int part)`
  Builds the operator parameter id for `_trem_mode`.
- `opTremRateFade` at line 148 through 151: `inline juce::String opTremRateFade(int op, int part)`
  Builds the operator parameter id for `_trem_rate_fade`.
- `opTremDepthFade` at line 152 through 155: `inline juce::String opTremDepthFade(int op, int part)`
  Builds the operator parameter id for `_trem_depth_fade`.
- `opMute` at line 156 through 159: `inline juce::String opMute(int op, int part)`
  Builds the operator parameter id for `_mute`.
- `opPhase` at line 161 through 164: `inline juce::String opPhase(int op, const char* field, int part)`
  Builds the operator parameter id for `_phase_`.
- `opPhaseLfo` at line 165 through 168: `inline juce::String opPhaseLfo(int op, const char* field, int part)`
  Builds the operator parameter id for `_phase_lfo_`.
- `opPhaseEnv` at line 169 through 172: `inline juce::String opPhaseEnv(int op, const char* field, int part)`
  Builds the operator parameter id for `_phase_env_`.
- `opSpecA` at line 174 through 177: `inline juce::String opSpecA(int op, int harmonic, int part)`
  Builds the operator parameter id for `_spec_a`.
- `opSpecB` at line 178 through 181: `inline juce::String opSpecB(int op, int harmonic, int part)`
  Builds the operator parameter id for `_spec_b`.
- `opSpecMorph` at line 182 through 185: `inline juce::String opSpecMorph(int op, int part)`
  Builds the operator parameter id for `_spec_morph`.
- `opSpecRangeA` at line 186 through 189: `inline juce::String opSpecRangeA(int op, int part)`
  Builds the operator parameter id for `_spec_range_a`.
- `opSpecRangeB` at line 190 through 193: `inline juce::String opSpecRangeB(int op, int part)`
  Builds the operator parameter id for `_spec_range_b`.
- `opSpecEnvRate` at line 194 through 197: `inline juce::String opSpecEnvRate(int op, int index, int part)`
  Builds the operator parameter id for `_spec_env_r`.
- `opSpecEnvLevel` at line 198 through 201: `inline juce::String opSpecEnvLevel(int op, int index, int part)`
  Builds the operator parameter id for `_spec_env_l`.
- `opSpecEnvOn` at line 202 through 205: `inline juce::String opSpecEnvOn(int op, int part)`
  Builds the operator parameter id for `_spec_env_on`.
- `opSpecEnvRelRate` at line 206 through 209: `inline juce::String opSpecEnvRelRate(int op, int part)`
  Builds the operator parameter id for `_spec_env_relr`.
- `opSpecEnvRelLevel` at line 210 through 213: `inline juce::String opSpecEnvRelLevel(int op, int part)`
  Builds the operator parameter id for `_spec_env_rell`.
- `opSpecEnvMode` at line 214 through 217: `inline juce::String opSpecEnvMode(int op, int part)`
  Builds the operator parameter id for `_spec_env_mode`.
- `opSpecLfoRate` at line 218 through 221: `inline juce::String opSpecLfoRate(int op, int part)`
  Builds the operator parameter id for `_spec_lfo_rate`.
- `opSpecLfoDepth` at line 222 through 225: `inline juce::String opSpecLfoDepth(int op, int part)`
  Builds the operator parameter id for `_spec_lfo_depth`.
- `opSpecLfoWave` at line 226 through 229: `inline juce::String opSpecLfoWave(int op, int part)`
  Builds the operator parameter id for `_spec_lfo_wave`.
- `opSpecLfoMode` at line 230 through 233: `inline juce::String opSpecLfoMode(int op, int part)`
  Builds the operator parameter id for `_spec_lfo_mode`.
- `opSpecLfoRateFade` at line 234 through 237: `inline juce::String opSpecLfoRateFade(int op, int part)`
  Builds the operator parameter id for `_spec_lfo_rate_fade`.
- `opSpecLfoDepthFade` at line 238 through 241: `inline juce::String opSpecLfoDepthFade(int op, int part)`
  Builds the operator parameter id for `_spec_lfo_depth_fade`.
- `opEnv` at line 243 through 250: `inline juce::String opEnv(int op, const char* field, int part)`
  Returns the `attack` parameter id with the part suffix.
- `opFeedbackRate` at line 252 through 256: `inline juce::String opFeedbackRate(int op, int index, int part)`
  Returns the op feedback rate value.
- `opFeedbackLevel` at line 257 through 261: `inline juce::String opFeedbackLevel(int op, int index, int part)`
  Returns the op feedback level value.
- `opFeedbackRelRate` at line 262 through 265: `inline juce::String opFeedbackRelRate(int op, int part)`
  Returns the op feedback rel rate value.
- `opFeedbackRelLevel` at line 266 through 269: `inline juce::String opFeedbackRelLevel(int op, int part)`
  Returns the op feedback rel level value.
- `opFeedbackMode` at line 270 through 273: `inline juce::String opFeedbackMode(int op, int part)`
  Returns the op feedback mode value.
- `pitchRate` at line 275 through 275: `inline juce::String pitchRate(int part)`
  Returns the `pitch_rate` parameter id with the part suffix.
- `pitchDepth` at line 276 through 276: `inline juce::String pitchDepth(int part)`
  Returns the `pitch_depth` parameter id with the part suffix.
- `pitchWave` at line 277 through 277: `inline juce::String pitchWave(int part)`
  Returns the `pitch_wave` parameter id with the part suffix.
- `pitchMode` at line 278 through 278: `inline juce::String pitchMode(int part)`
  Returns the `pitch_mode` parameter id with the part suffix.
- `pitchRateFade` at line 279 through 282: `inline juce::String pitchRateFade(int part)`
  Returns the `pitch_rate_fade` parameter id with the part suffix.
- `pitchDepthFade` at line 283 through 286: `inline juce::String pitchDepthFade(int part)`
  Returns the `pitch_depth_fade` parameter id with the part suffix.
- `pitchEnvOn` at line 287 through 287: `inline juce::String pitchEnvOn(int part)`
  Returns the `penv_on` parameter id with the part suffix.
- `pitchEnvRate` at line 288 through 291: `inline juce::String pitchEnvRate(int index, int part)`
  Returns the `penv_r` parameter id with the part suffix.
- `pitchEnvLevel` at line 292 through 295: `inline juce::String pitchEnvLevel(int index, int part)`
  Returns the `penv_l` parameter id with the part suffix.
- `pitchEnvRelRate` at line 296 through 299: `inline juce::String pitchEnvRelRate(int part)`
  Returns the `penv_relr` parameter id with the part suffix.
- `pitchEnvRelLevel` at line 300 through 303: `inline juce::String pitchEnvRelLevel(int part)`
  Returns the `penv_rell` parameter id with the part suffix.
- `pitchEnvMode` at line 304 through 304: `inline juce::String pitchEnvMode(int part)`
  Returns the `penv_mode` parameter id with the part suffix.
- `filterCutoff` at line 306 through 306: `inline juce::String filterCutoff(int part)`
  Returns the `cutoff` parameter id with the part suffix.
- `filterRes` at line 307 through 307: `inline juce::String filterRes(int part)`
  Returns the `res` parameter id with the part suffix.
- `filterType` at line 308 through 308: `inline juce::String filterType(int part)`
  Returns the `filter_type` parameter id with the part suffix.
- `filterOn` at line 309 through 309: `inline juce::String filterOn(int part)`
  Returns the `filter_on` parameter id with the part suffix.
- `ringOn` at line 311 through 314: `inline juce::String ringOn(int pair, int part)`
  Returns the `ring` parameter id with the part suffix.
- `ringLevel` at line 316 through 319: `inline juce::String ringLevel(int pair, int part)`
  Returns the `ring` parameter id with the part suffix.
- `carrierOut` at line 321 through 324: `inline juce::String carrierOut(int op, int part)`
  Returns the `car` parameter id with the part suffix.
- `filterEnvOn` at line 325 through 325: `inline juce::String filterEnvOn(int part)`
  Returns the `fenv_on` parameter id with the part suffix.
- `filterEnvRelRate` at line 326 through 329: `inline juce::String filterEnvRelRate(int part)`
  Returns the `fenv_relr` parameter id with the part suffix.
- `filterEnvRelLevel` at line 330 through 333: `inline juce::String filterEnvRelLevel(int part)`
  Returns the `fenv_rell` parameter id with the part suffix.
- `filterEnvMode` at line 334 through 334: `inline juce::String filterEnvMode(int part)`
  Returns the `fenv_mode` parameter id with the part suffix.
- `filterRate` at line 335 through 338: `inline juce::String filterRate(int index, int part)`
  Returns the `fenv_r` parameter id with the part suffix.
- `filterLevel` at line 339 through 342: `inline juce::String filterLevel(int index, int part)`
  Returns the `fenv_l` parameter id with the part suffix.
- `followTargets` at line 351 through 393: `inline std::array<FollowTarget, 19> followTargets(int part)`
  Returns the `op1_level_ks_lo` parameter id with the part suffix.
- `macroValue` at line 395 through 398: `inline juce::String macroValue(int index)`
  Returns the macro value value.
- `macroMidi` at line 400 through 403: `inline juce::String macroMidi(int index)`
  Returns the macro MIDI value.
- `macroMidiRange` at line 405 through 408: `inline juce::String macroMidiRange(int index)`
  Returns the macro MIDI range value.
- `macroTarget` at line 410 through 413: `inline juce::String macroTarget(int index, int destination)`
  Returns the macro target value.
- `macroRange` at line 415 through 418: `inline juce::String macroRange(int index, int destination)`
  Returns the macro range value.
- `waveNames` at line 420 through 425: `inline juce::StringArray waveNames()`
  Returns the wave names value.
- `lfoWaveNames` at line 427 through 430: `inline juce::StringArray lfoWaveNames()`
  Returns the LFO wave names value.

### `plugin/ui/SpectrumAnalysis.cpp`

- `analyseWaveTable` at line 16 through 42: `std::array<double, 64> analyseWaveTable(int wave, int firstHarmonic)`
  Sixty-four harmonic magnitudes of one mip-0 wave table, normalised to 0..99. `firstHarmonic` is the 1-based harmonic of the first bar. Wave 14 has no table and returns zeros.

### `plugin/ui/SpectrumAnalysis.h`

- `analyseWaveTable` at line 11 through 11: `std::array<double, 64> analyseWaveTable(int wave, int firstHarmonic = 1)`
  Sixty-four harmonic magnitudes of one mip-0 wave table, normalised to 0..99. `firstHarmonic` is the 1-based harmonic of the first bar. Wave 14 has no table and returns zeros.

### `plugin/ui/Theme.cpp`

- `makeTypeface` at line 11 through 14: `juce::Typeface::Ptr makeTypeface(const void* data, int size)`
  Creates the typeface.
- `colour` at line 18 through 21: `juce::Colour colour(std::uint32_t argb)`
  Returns the colour value.
- `uiTypeface` at line 23 through 28: `juce::Typeface::Ptr uiTypeface()`
  Returns the UI typeface value.
- `displayTypeface` at line 30 through 35: `juce::Typeface::Ptr displayTypeface()`
  Returns the display typeface value.
- `brandTypeface` at line 37 through 44: `juce::Typeface::Ptr brandTypeface()`
  The provisional wordmark's alpha takes the bold cut, so it has real * weight rather than a second draw at an offset.
- `uiFont` at line 46 through 49: `juce::Font uiFont(float height)`
  Returns the UI font value.
- `displayFont` at line 51 through 54: `juce::Font displayFont(float height)`
  Returns the display font value.
- `brandFont` at line 56 through 59: `juce::Font brandFont(float height)`
  Returns the brand font value.
- `drawBoldText` at line 61 through 75: `void drawBoldText(juce::Graphics& g, const juce::Font& font, const juce::String& text, juce::Rectangle<float> area, float extraWeight)`
  Zen Dots has one weight, so the R boxes widen the outline to read bold.
- `textWidth` at line 77 through 84: `float textWidth(const juce::Font& font, const juce::String& text)`
  Returns the text width value.
- `drawSectionTitle` at line 86 through 95: `void drawSectionTitle(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& title)`
  Draws the section title.
- `P1LookAndFeel` at line 97 through 114: `P1LookAndFeel::P1LookAndFeel()`
  Returns the p 1 look and feel value.
- `drawComboBox` at line 116 through 130: `void P1LookAndFeel::drawComboBox(juce::Graphics& g, int width, int height, bool, int, int, int, int, juce::ComboBox&)`
  Draws the combo box.
- `positionComboBoxText` at line 132 through 138: `void P1LookAndFeel::positionComboBoxText(juce::ComboBox& box, juce::Label& label)`
  Returns the position combo box text value.
- `drawPopupMenuBackground` at line 140 through 146: `void P1LookAndFeel::drawPopupMenuBackground(juce::Graphics& g, int width, int height)`
  Draws the popup menu background.
- `drawPopupMenuItem` at line 148 through 172: `void P1LookAndFeel::drawPopupMenuItem( juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool, const juce::String& text, const juce::String&, const juce::Drawable*, const juce::Colour*)`
  Draws the popup menu item.
- `drawScrollbar` at line 174 through 185: `void P1LookAndFeel::drawScrollbar(juce::Graphics& g, juce::ScrollBar&, int, int, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool, bool)`
  Draws the scrollbar.
- `drawRotarySlider` at line 187 through 246: `void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)`
  Draws the rotary slider.

### `plugin/ui/Theme.h`

- `colour` at line 12 through 12: `juce::Colour colour(std::uint32_t argb)`
  Returns the colour value.
- `uiTypeface` at line 14 through 14: `juce::Typeface::Ptr uiTypeface()`
  Returns the UI typeface value.
- `displayTypeface` at line 15 through 15: `juce::Typeface::Ptr displayTypeface()`
  Returns the display typeface value.
- `brandTypeface` at line 16 through 16: `juce::Typeface::Ptr brandTypeface()`
  The provisional wordmark's alpha takes the bold cut, so it has real * weight rather than a second draw at an offset.
- `uiFont` at line 18 through 18: `juce::Font uiFont(float height)`
  Returns the UI font value.
- `displayFont` at line 19 through 19: `juce::Font displayFont(float height)`
  Returns the display font value.
- `brandFont` at line 20 through 20: `juce::Font brandFont(float height)`
  Returns the brand font value.
- `textWidth` at line 21 through 21: `float textWidth(const juce::Font& font, const juce::String& text)`
  Returns the text width value.
- `drawBoldText` at line 24 through 26: `void drawBoldText(juce::Graphics& g, const juce::Font& font, const juce::String& text, juce::Rectangle<float> area, float extraWeight)`
  Zen Dots has one weight, so the R boxes widen the outline to read bold.
- `drawSectionTitle` at line 28 through 29: `void drawSectionTitle(juce::Graphics& g, juce::Rectangle<int> area, const juce::String& title)`
  Draws the section title.
- `drawComboBox` at line 35 through 37: `void drawComboBox(juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override`
  Draws the combo box.
- `positionComboBoxText` at line 38 through 38: `void positionComboBoxText(juce::ComboBox& box, juce::Label& label) override`
  Runs the position combo box text step for the surrounding code.
- `drawPopupMenuBackground` at line 39 through 39: `void drawPopupMenuBackground(juce::Graphics& g, int width, int height) override`
  Draws the popup menu background.
- `drawPopupMenuItem` at line 40 through 46: `void drawPopupMenuItem(juce::Graphics& g, const juce::Rectangle<int>& area, bool isSeparator, bool isActive, bool isHighlighted, bool isTicked, bool hasSubMenu, const juce::String& text, const juce::String& shortcutKeyText, const juce::Drawable* icon, const juce::Colour* textColour) override`
  Draws the popup menu item.
- `drawScrollbar` at line 47 through 50: `void drawScrollbar(juce::Graphics& g, juce::ScrollBar& bar, int x, int y, int width, int height, bool isScrollbarVertical, int thumbStartPosition, int thumbSize, bool isMouseOver, bool isMouseDown) override`
  Draws the scrollbar.
- `drawRotarySlider` at line 55 through 57: `void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override`
  Draws the rotary slider.

### `plugin/ui/Widgets.cpp`

- `knobLookAndFeel` at line 17 through 21: `KnobLookAndFeel& knobLookAndFeel()`
  Returns the knob look and feel value.
- `signedNumber` at line 28 through 31: `juce::String signedNumber(int value)`
  Returns the signed number value.
- `snapRatio` at line 33 through 46: `int32_t snapRatio(int32_t value)`
  Returns the snap ratio value.
- `stepRatioDetent` at line 51 through 65: `int32_t stepRatioDetent(int32_t value, int dir)`
  The detent one step above (dir > 0) or one step below the current value. An off-detent value moves to the neighbour on that side, so a typed 1.37 followed by an up-arrow lands on 1.41 rather than 1.50.
- `notePair` at line 67 through 72: `juce::String notePair(int note)`
  Returns the note pair value.
- `percentage` at line 74 through 84: `juce::String percentage(const P1ParamSpec& spec, double value, bool inverse)`
  Returns the percentage value.
- `findSpec` at line 88 through 95: `const P1ParamSpec* findSpec(const juce::String& id)`
  Finds the spec.
- `volumeText` at line 97 through 104: `juce::String volumeText(int position)`
  The master volume fader position as dB text, and back. The law is in VolumeLaw.h, which the processor and the header slider share.
- `volumePositionFromText` at line 106 through 119: `int volumePositionFromText(const juce::String& text)`
  Returns the volume position from text value.
- `formatParamValue` at line 121 through 220: `juce::String formatParamValue(const P1ParamSpec& spec, double value)`
  Formats the param value.
- `formatParamValuePlain` at line 222 through 235: `juce::String formatParamValuePlain(const P1ParamSpec& spec, double value)`
  The same value without its unit, for the effective value beside a stored one.
- `parseParamValue` at line 237 through 336: `double parseParamValue(const P1ParamSpec& spec, const juce::String& text)`
  Parses the param value.
- `ParamValueLabel` at line 338 through 341: `ParamValueLabel::ParamValueLabel() : storedColour_(juce::Colours::white), effectiveColour_(juce::Colours::white)`
  Returns the param value label value.
- `setStoredColour` at line 343 through 350: `void ParamValueLabel::setStoredColour(juce::Colour colour)`
  Writes the stored colour.
- `setEffectiveText` at line 352 through 360: `void ParamValueLabel::setEffectiveText(const juce::String& text, juce::Colour colour)`
  Writes the effective text.
- `paint` at line 362 through 400: `void ParamValueLabel::paint(juce::Graphics& g)`
  Draws the component.
- `ParamKnob` at line 402 through 460: `ParamKnob::ParamKnob(APVTS& state, juce::String paramID, juce::String label, float dialSize, float labelSize) : state_(state), paramID_(std::move(paramID)), dialSize_(dialSize), labelSize_(labelSize)`
  Returns the param knob value.
- `~ParamKnob` at line 462 through 465: `ParamKnob::~ParamKnob()`
  Destroys the object.
- `mouseWheelMove` at line 467 through 499: `void ParamKnob::KnobSlider::mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel)`
  Handles a mouse interaction.
- `setParamID` at line 501 through 517: `void ParamKnob::setParamID(const juce::String& paramID)`
  Writes the param id.
- `setMacroFeedback` at line 519 through 529: `void ParamKnob::setMacroFeedback(bool targeted, bool hasEffective, double effective)`
  Writes the macro feedback.
- `setModArc` at line 531 through 540: `void ParamKnob::setModArc(double fraction)`
  Writes the mod arc.
- `resized` at line 542 through 553: `void ParamKnob::resized()`
  Lays out the child components.
- `paint` at line 555 through 555: `void ParamKnob::paint(juce::Graphics&)`
  Draws the component.
- `updateValueText` at line 557 through 563: `void ParamKnob::updateValueText()`
  Updates the value text.
- `refreshValueDisplay` at line 565 through 576: `void ParamKnob::refreshValueDisplay()`
  Refreshes the value display display.
- `isRatio` at line 578 through 581: `bool ParamKnob::isRatio() const`
  Tests whether the the ratio.
- `snapValue` at line 583 through 593: `void ParamKnob::snapValue()`
  Returns the snap value value.
- `stepRatio` at line 595 through 611: `bool ParamKnob::stepRatio(const juce::KeyPress& key)`
  Tests the step ratio condition.
- `commitValue` at line 613 through 623: `void ParamKnob::commitValue()`
  Returns the commit value value.
- `P1Button` at line 625 through 631: `P1Button::P1Button(const juce::String& label, bool mini) : juce::Button(label)`
  Returns the p 1 button value.
- `paintButton` at line 633 through 647: `void P1Button::paintButton(juce::Graphics& g, bool highlighted, bool down)`
  Draws the button in its normal, highlighted and down states.
- `P1MuteTile` at line 649 through 662: `P1MuteTile::P1MuteTile(const juce::String& label, bool muted, std::function<void(bool)> onToggle) : juce::Button(label)`
  Returns the p 1 mute tile value.
- `paintButton` at line 664 through 678: `void P1MuteTile::paintButton(juce::Graphics& g, bool highlighted, bool)`
  Draws the button in its normal, highlighted and down states.
- `P1Toggle` at line 680 through 688: `P1Toggle::P1Toggle(APVTS& state, const juce::String& paramID, const juce::String& label) : juce::Button(label), label_(label)`
  Returns the p 1 toggle value.
- `paintButton` at line 690 through 711: `void P1Toggle::paintButton(juce::Graphics& g, bool highlighted, bool)`
  Draws the button in its normal, highlighted and down states.
- `ParamChoice` at line 713 through 731: `ParamChoice::ParamChoice(APVTS& state, const juce::String& paramID, juce::StringArray items)`
  Returns the param choice value.
- `resized` at line 733 through 736: `void ParamChoice::resized()`
  Lays out the child components.
- `updateFromNormalized` at line 738 through 745: `void ParamChoice::updateFromNormalized(float normalized)`
  Updates the from normalized.
- `Tab` at line 747 through 754: `P1TabBar::Tab::Tab(const juce::String& label, int index, P1TabBar& owner) : juce::Button(label), index_(index), owner_(owner)`
  Returns the tab value.
- `paintButton` at line 756 through 782: `void P1TabBar::Tab::paintButton(juce::Graphics& g, bool highlighted, bool)`
  Draws the button in its normal, highlighted and down states.
- `P1TabBar` at line 784 through 784: `P1TabBar::P1TabBar() = default`
  Returns the p 1 tab bar value.
- `setLabels` at line 786 through 800: `void P1TabBar::setLabels(const juce::StringArray& labels)`
  Writes the labels.
- `setCompact` at line 802 through 809: `void P1TabBar::setCompact(bool compact)`
  Sets the compact flag.
- `setActive` at line 811 through 822: `void P1TabBar::setActive(int index)`
  Sets the active flag.
- `resized` at line 824 through 868: `void P1TabBar::resized()`
  Lays out the child components.
- `paint` at line 870 through 875: `void P1TabBar::paint(juce::Graphics& g)`
  Draws the component.
- `P1Scope` at line 877 through 880: `P1Scope::P1Scope()`
  Returns the p 1 scope value.
- `setSamples` at line 882 through 890: `void P1Scope::setSamples(const float* samples, int count)`
  Writes the samples.
- `paint` at line 892 through 925: `void P1Scope::paint(juce::Graphics& g)`
  Draws the component.
- `P1Spectro` at line 927 through 940: `P1Spectro::P1Spectro()`
  Returns the p 1 spectro value.
- `setSamples` at line 942 through 950: `void P1Spectro::setSamples(const float* samples, int count)`
  Writes the samples.
- `paint` at line 952 through 980: `void P1Spectro::paint(juce::Graphics& g)`
  Draws the component.
- `P1SpectralBars` at line 982 through 987: `P1SpectralBars::P1SpectralBars(APVTS& state, juce::StringArray parameterIDs, juce::Colour barColour) : state_(state), parameterIDs_(std::move(parameterIDs)), colour_(barColour)`
  Returns the p 1 spectral bars value.
- `levelAt` at line 989 through 996: `double P1SpectralBars::levelAt(int index) const`
  Returns the level at value.
- `levels` at line 998 through 1005: `std::vector<double> P1SpectralBars::levels() const`
  Returns the levels value.
- `setFirstHarmonic` at line 1007 through 1013: `void P1SpectralBars::setFirstHarmonic(int first)`
  Writes the first harmonic.
- `setEditWindow` at line 1015 through 1020: `void P1SpectralBars::setEditWindow(int first, int last)`
  The selection window in absolute harmonics, and the live/dead test that composes with it. Freehand editing only writes a bar both allow.
- `setEditTest` at line 1022 through 1025: `void P1SpectralBars::setEditTest(std::function<bool(int, double)> test)`
  Writes the edit test.
- `inWindow` at line 1027 through 1030: `bool P1SpectralBars::inWindow(int harmonic) const`
  Tests the in window condition.
- `setGroupTest` at line 1032 through 1036: `void P1SpectralBars::setGroupTest(std::function<bool(int)> test)`
  Writes the group test.
- `setLevel` at line 1038 through 1045: `void P1SpectralBars::setLevel(int index, double value)`
  Writes the level.
- `setLevels` at line 1047 through 1054: `void P1SpectralBars::setLevels(const std::vector<double>& levels)`
  Writes the levels.
- `setFromMouse` at line 1056 through 1080: `void P1SpectralBars::setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `mouseDown` at line 1082 through 1087: `void P1SpectralBars::mouseDown(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `mouseDrag` at line 1089 through 1094: `void P1SpectralBars::mouseDrag(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `paint` at line 1096 through 1164: `void P1SpectralBars::paint(juce::Graphics& g)`
  Draws the component.
- `P1SpectrumDisplay` at line 1166 through 1183: `P1SpectrumDisplay::P1SpectrumDisplay(APVTS& state, juce::StringArray aIDs, juce::StringArray bIDs, juce::String morphID, juce::String rangeAID, juce::String rangeBID, juce::Colour aColour, juce::Colour bColour) : state_(state), aIDs_(std::move(aIDs)), bIDs_(std::move(bIDs)), morphID_(std::move(morphID)), rangeAID_(std::move(rangeAID)), rangeBID_(std::move(rangeBID)), aColour_(aColour), bColour_(bColour)`
  Returns the p 1 spectrum display value.
- `tick` at line 1185 through 1216: `void P1SpectrumDisplay::tick()`
  Refreshes the live display state.
- `levelAt` at line 1218 through 1225: `double P1SpectrumDisplay::levelAt(const juce::StringArray& ids, int index) const`
  Returns the level at value.
- `paint` at line 1227 through 1398: `void P1SpectrumDisplay::paint(juce::Graphics& g)`
  Draws the component.
- `P1Envelope` at line 1400 through 1418: `P1Envelope::P1Envelope(APVTS& state, juce::StringArray rateIDs, juce::StringArray levelIDs, juce::Colour curveColour, double levelMin, double levelMax, bool zeroLine, EnvelopeShape shape, juce::String modeID, juce::String startID, double startScale) : state_(state), rateIDs_(std::move(rateIDs)), levelIDs_(std::move(levelIDs)), colour_(curveColour), levelMin_(levelMin), levelMax_(levelMax), zeroLine_(zeroLine), shape_(shape), modeID_(std::move(modeID)), startID_(std::move(startID)), startScale_(startScale)`
  Returns the p 1 envelope value.
- `tick` at line 1420 through 1454: `void P1Envelope::tick()`
  Refreshes the live display state.
- `paint` at line 1456 through 1687: `void P1Envelope::paint(juce::Graphics& g)`
  Draws the component.
- `P1Wave` at line 1689 through 1693: `P1Wave::P1Wave(APVTS& state, juce::String waveID, juce::Colour curveColour) : state_(state), waveID_(std::move(waveID)), colour_(curveColour)`
  Returns the p 1 wave value.
- `tick` at line 1695 through 1710: `void P1Wave::tick()`
  Refreshes the live display state.
- `paint` at line 1712 through 1747: `void P1Wave::paint(juce::Graphics& g)`
  Draws the component.
- `P1LevelSlider` at line 1749 through 1769: `P1LevelSlider::P1LevelSlider(APVTS& state, const juce::String& paramID, juce::String caption, bool bipolar) : state_(state), paramID_(paramID), caption_(std::move(caption)), bipolar_(bipolar)`
  Returns the p 1 level slider value.
- `setMacroFeedback` at line 1771 through 1781: `void P1LevelSlider::setMacroFeedback(bool targeted, bool hasEffective, double effective)`
  Writes the macro feedback.
- `resized` at line 1783 through 1786: `void P1LevelSlider::resized()`
  Lays out the child components.
- `paint` at line 1788 through 1853: `void P1LevelSlider::paint(juce::Graphics& g)`
  Draws the component.
- `mouseDown` at line 1855 through 1858: `void P1LevelSlider::mouseDown(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `mouseDrag` at line 1860 through 1863: `void P1LevelSlider::mouseDrag(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `setFromMouse` at line 1865 through 1876: `void P1LevelSlider::setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `P1VolumeSlider` at line 1878 through 1892: `P1VolumeSlider::P1VolumeSlider(APVTS& state, const juce::String& paramID) : state_(state), paramID_(paramID)`
  Returns the p 1 volume slider value.
- `resized` at line 1894 through 1897: `void P1VolumeSlider::resized()`
  Lays out the child components.
- `paint` at line 1899 through 1928: `void P1VolumeSlider::paint(juce::Graphics& g)`
  Draws the component.
- `mouseDown` at line 1930 through 1933: `void P1VolumeSlider::mouseDown(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `mouseDrag` at line 1935 through 1938: `void P1VolumeSlider::mouseDrag(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `setFromMouse` at line 1940 through 1950: `void P1VolumeSlider::setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `P1Card` at line 1952 through 1964: `P1Card::P1Card(juce::String title, std::function<void()> onCopy, std::function<void()> onPaste) : title_(std::move(title))`
  Returns the p 1 card value.
- `paint` at line 1966 through 1970: `void P1Card::paint(juce::Graphics& g)`
  Draws the component.
- `resized` at line 1972 through 1982: `void P1Card::resized()`
  Lays out the child components.
- `setHeaderTrailingWidth` at line 1984 through 1988: `void P1Card::setHeaderTrailingWidth(int width)`
  Writes the header trailing width.
- `headerTrailingBounds` at line 1990 through 1994: `juce::Rectangle<int> P1Card::headerTrailingBounds() const`
  Returns the header trailing bounds value.
- `contentArea` at line 1996 through 1999: `juce::Rectangle<int> P1Card::contentArea() const`
  Returns the content area value.
- `P1AlgorithmBlock` at line 2001 through 2004: `P1AlgorithmBlock::P1AlgorithmBlock()`
  Returns the p 1 algorithm block value.
- `setAlgorithm` at line 2006 through 2010: `void P1AlgorithmBlock::setAlgorithm(int algorithm)`
  Writes the algorithm.
- `setRingMask` at line 2012 through 2018: `void P1AlgorithmBlock::setRingMask(uint8_t mask)`
  Writes the ring mask.
- `setDirectMask` at line 2020 through 2026: `void P1AlgorithmBlock::setDirectMask(uint8_t mask)`
  Writes the direct mask.
- `paint` at line 2028 through 2047: `void P1AlgorithmBlock::paint(juce::Graphics& g)`
  Draws the component.
- `mouseUp` at line 2049 through 2053: `void P1AlgorithmBlock::mouseUp(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `mouseEnter` at line 2055 through 2059: `void P1AlgorithmBlock::mouseEnter(const juce::MouseEvent&)`
  Handles a mouse interaction.
- `mouseExit` at line 2061 through 2065: `void P1AlgorithmBlock::mouseExit(const juce::MouseEvent&)`
  Handles a mouse interaction.
- `mouseWheelMove` at line 2067 through 2072: `void P1AlgorithmBlock::mouseWheelMove(const juce::MouseEvent&, const juce::MouseWheelDetails& wheel)`
  Handles a mouse interaction.
- `P1Dialog` at line 2074 through 2105: `P1Dialog::P1Dialog(juce::String title, std::unique_ptr<juce::Component> body, int width, int height, juce::StringArray buttons, std::function<void(int)> onButton, std::function<void()> onClose) : title_(std::move(title)), body_(std::move(body)), buttons_(std::move(buttons)), onButton_(std::move(onButton)), onClose_(std::move(onClose))`
  Returns the p 1 dialog value.
- `paint` at line 2107 through 2124: `void P1Dialog::paint(juce::Graphics& g)`
  Draws the component.
- `resized` at line 2126 through 2168: `void P1Dialog::resized()`
  Lays out the child components.
- `mouseUp` at line 2170 through 2180: `void P1Dialog::mouseUp(const juce::MouseEvent& event)`
  Handles a mouse interaction.
- `drawAlgorithmGraph` at line 2182 through 2348: `void drawAlgorithmGraph(juce::Graphics& g, juce::Rectangle<float> area, int algorithm, uint8_t ringMask, uint8_t directMask)`
  Draws the algorithm graph.

### `plugin/ui/Widgets.h`

- `findSpec` at line 21 through 21: `const P1ParamSpec* findSpec(const juce::String& id)`
  Finds the spec.
- `formatParamValue` at line 22 through 22: `juce::String formatParamValue(const P1ParamSpec& spec, double value)`
  Formats the param value.
- `formatParamValuePlain` at line 24 through 24: `juce::String formatParamValuePlain(const P1ParamSpec& spec, double value)`
  The same value without its unit, for the effective value beside a stored one.
- `parseParamValue` at line 25 through 25: `double parseParamValue(const P1ParamSpec& spec, const juce::String& text)`
  Parses the param value.
- `volumeText` at line 29 through 29: `juce::String volumeText(int position)`
  The master volume fader position as dB text, and back. The law is in VolumeLaw.h, which the processor and the header slider share.
- `volumePositionFromText` at line 30 through 30: `int volumePositionFromText(const juce::String& text)`
  Returns the volume position from text value.
- `~MacroFeedbackTarget` at line 37 through 37: `virtual ~MacroFeedbackTarget() = default`
  Destroys the object.
- `macroParamIndex` at line 38 through 38: `virtual int macroParamIndex() const = 0`
  Returns the manifest index used for macro feedback.
- `setMacroFeedback` at line 39 through 40: `virtual void setMacroFeedback(bool targeted, bool hasEffective, double effective) = 0`
  Writes the macro feedback.
- `setStoredColour` at line 49 through 49: `void setStoredColour(juce::Colour colour)`
  Writes the stored colour.
- `setEffectiveText` at line 50 through 50: `void setEffectiveText(const juce::String& text, juce::Colour colour)`
  Writes the effective text.
- `paint` at line 51 through 51: `void paint(juce::Graphics& g) override`
  Draws the component.
- `setParamID` at line 65 through 65: `void setParamID(const juce::String& paramID)`
  Writes the param id.
- `setModArc` at line 66 through 66: `void setModArc(double fraction)`
  Writes the mod arc.
- `macroParamIndex` at line 67 through 67: `int macroParamIndex() const override`
  Returns the manifest index used for macro feedback.
- `setMacroFeedback` at line 68 through 69: `void setMacroFeedback(bool targeted, bool hasEffective, double effective) override`
  Writes the macro feedback.
- `resized` at line 70 through 70: `void resized() override`
  Lays out the child components.
- `paint` at line 71 through 71: `void paint(juce::Graphics& g) override`
  Draws the component.
- `keyPressed` at line 85 through 90: `bool keyPressed(const juce::KeyPress& key) override`
  Handles a key press.
- `mouseWheelMove` at line 92 through 93: `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override`
  Handles a mouse interaction.
- `updateValueText` at line 100 through 100: `void updateValueText()`
  Updates the value text.
- `refreshValueDisplay` at line 101 through 101: `void refreshValueDisplay()`
  Refreshes the value display display.
- `commitValue` at line 102 through 102: `void commitValue()`
  Runs the commit value step for the surrounding code.
- `snapValue` at line 103 through 103: `void snapValue()`
  Runs the snap value step for the surrounding code.
- `stepRatio` at line 104 through 104: `bool stepRatio(const juce::KeyPress& key)`
  Tests the step ratio condition.
- `isRatio` at line 105 through 105: `bool isRatio() const`
  Tests whether the the ratio.
- `paintButton` at line 127 through 128: `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override`
  Draws the button in its normal, highlighted and down states.
- `paintButton` at line 135 through 136: `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override`
  Draws the button in its normal, highlighted and down states.
- `paintButton` at line 143 through 144: `void paintButton(juce::Graphics& g, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override`
  Draws the button in its normal, highlighted and down states.
- `P1Choice` at line 153 through 153: `P1Choice() = default`
  Constructs the object.
- `resized` at line 160 through 160: `void resized() override`
  Lays out the child components.
- `updateFromNormalized` at line 163 through 163: `void updateFromNormalized(float normalized)`
  Updates the from normalized.
- `setLabels` at line 174 through 174: `void setLabels(const juce::StringArray& labels)`
  Writes the labels.
- `setActive` at line 175 through 175: `void setActive(int index)`
  Sets the active flag.
- `setCompact` at line 176 through 176: `void setCompact(bool compact)`
  Sets the compact flag.
- `resized` at line 177 through 177: `void resized() override`
  Lays out the child components.
- `paint` at line 178 through 178: `void paint(juce::Graphics& g) override`
  Draws the component.
- `paintButton` at line 185 through 185: `void paintButton(juce::Graphics& g, bool highlighted, bool down) override`
  Draws the button in its normal, highlighted and down states.
- `setSamples` at line 201 through 201: `void setSamples(const float* samples, int count)`
  Writes the samples.
- `paint` at line 202 through 202: `void paint(juce::Graphics& g) override`
  Draws the component.
- `setSamples` at line 211 through 211: `void setSamples(const float* samples, int count)`
  Writes the samples.
- `paint` at line 212 through 212: `void paint(juce::Graphics& g) override`
  Draws the component.
- `paint` at line 228 through 228: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseDown` at line 229 through 229: `void mouseDown(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseDrag` at line 230 through 230: `void mouseDrag(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `levels` at line 232 through 232: `std::vector<double> levels() const`
  Returns the levels value.
- `setLevels` at line 233 through 233: `void setLevels(const std::vector<double>& levels)`
  Writes the levels.
- `setFirstHarmonic` at line 234 through 234: `void setFirstHarmonic(int first)`
  Writes the first harmonic.
- `setGroupTest` at line 235 through 235: `void setGroupTest(std::function<bool(int)> test)`
  Writes the group test.
- `setEditWindow` at line 238 through 238: `void setEditWindow(int first, int last)`
  The selection window in absolute harmonics, and the live/dead test that composes with it. Freehand editing only writes a bar both allow.
- `setEditTest` at line 239 through 239: `void setEditTest(std::function<bool(int, double)> test)`
  Writes the edit test.
- `setFromMouse` at line 243 through 243: `void setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `levelAt` at line 244 through 244: `double levelAt(int index) const`
  Returns the level at value.
- `setLevel` at line 245 through 245: `void setLevel(int index, double value)`
  Writes the level.
- `inWindow` at line 246 through 246: `bool inWindow(int harmonic) const`
  Tests the in window condition.
- `paint` at line 264 through 264: `void paint(juce::Graphics& g) override`
  Draws the component.
- `setLiveMorph` at line 266 through 266: `void setLiveMorph(double value)`
  The morph the engine last read, 0 to 1, or -1 for the base parameter.
- `tick` at line 267 through 267: `void tick()`
  Repaint only when the drawn parameter state changed.
- `levelAt` at line 270 through 270: `double levelAt(const juce::StringArray& ids, int index) const`
  Returns the level at value.
- `paint` at line 299 through 299: `void paint(juce::Graphics& g) override`
  Draws the component.
- `refresh` at line 300 through 300: `void refresh()`
  Refreshes the display.
- `tick` at line 302 through 302: `void tick()`
  Repaint only when the drawn parameter state changed.
- `paint` at line 323 through 323: `void paint(juce::Graphics& g) override`
  Draws the component.
- `tick` at line 324 through 324: `void tick()`
  Repaint only when the drawn parameter state changed.
- `resized` at line 338 through 338: `void resized() override`
  Lays out the child components.
- `paint` at line 339 through 339: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseDown` at line 340 through 340: `void mouseDown(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseDrag` at line 341 through 341: `void mouseDrag(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `macroParamIndex` at line 342 through 342: `int macroParamIndex() const override`
  Returns the manifest index used for macro feedback.
- `setMacroFeedback` at line 343 through 344: `void setMacroFeedback(bool targeted, bool hasEffective, double effective) override`
  Writes the macro feedback.
- `setFromMouse` at line 347 through 347: `void setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `resized` at line 369 through 369: `void resized() override`
  Lays out the child components.
- `paint` at line 370 through 370: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseDown` at line 371 through 371: `void mouseDown(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseDrag` at line 372 through 372: `void mouseDrag(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `setFromMouse` at line 375 through 375: `void setFromMouse(const juce::MouseEvent& event)`
  Writes the from mouse.
- `paint` at line 388 through 388: `void paint(juce::Graphics& g) override`
  Draws the component.
- `resized` at line 389 through 389: `void resized() override`
  Lays out the child components.
- `setHeaderTrailingWidth` at line 391 through 391: `void setHeaderTrailingWidth(int width)`
  Writes the header trailing width.
- `headerTrailingBounds` at line 392 through 392: `juce::Rectangle<int> headerTrailingBounds() const`
  Returns the header trailing bounds value.
- `contentArea` at line 393 through 393: `juce::Rectangle<int> contentArea() const`
  Returns the content area value.
- `setAlgorithm` at line 406 through 406: `void setAlgorithm(int algorithm)`
  Writes the algorithm.
- `setRingMask` at line 407 through 407: `void setRingMask(uint8_t mask)`
  Writes the ring mask.
- `setDirectMask` at line 408 through 408: `void setDirectMask(uint8_t mask)`
  Writes the direct mask.
- `paint` at line 409 through 409: `void paint(juce::Graphics& g) override`
  Draws the component.
- `mouseUp` at line 410 through 410: `void mouseUp(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseEnter` at line 411 through 411: `void mouseEnter(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseExit` at line 412 through 412: `void mouseExit(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `mouseWheelMove` at line 413 through 414: `void mouseWheelMove(const juce::MouseEvent& event, const juce::MouseWheelDetails& wheel) override`
  Handles a mouse interaction.
- `paint` at line 432 through 432: `void paint(juce::Graphics& g) override`
  Draws the component.
- `resized` at line 433 through 433: `void resized() override`
  Lays out the child components.
- `mouseUp` at line 434 through 434: `void mouseUp(const juce::MouseEvent& event) override`
  Handles a mouse interaction.
- `drawAlgorithmGraph` at line 447 through 449: `void drawAlgorithmGraph(juce::Graphics& g, juce::Rectangle<float> area, int algorithm, uint8_t ringMask = 0, uint8_t directMask = 0)`
  Draws the algorithm graph.

### `tools/audit_modules.py`

- `load` at line 23 through 24: `def load(path: Path)`
  Implements the load helper.
- `module_spec` at line 27 through 32: `def module_spec(kind: str)`
  Implements the module spec helper.
- `target_node` at line 35 through 38: `def target_node(value: object)`
  Implements the target node helper.
- `main` at line 41 through 131: `def main()`
  Runs the test or tool entry point.

### `tools/gen_code_docs.py`

- `text` at line 86 through 87: `def text(node, src: bytes)`
  Implements the text helper.
- `name_of` at line 90 through 116: `def name_of(node, src: bytes)`
  The last declarator name, resolving qualified names and pointers.
- `clean_comment` at line 119 through 147: `def clean_comment(lines: list[str], start_line: int)`
  The comment block immediately above `start_line`, collapsed to prose.
- `clean_signature` at line 150 through 153: `def clean_signature(value: str)`
  Implements the clean signature helper.
- `parser_for` at line 156 through 161: `def parser_for(path: Path)`
  Parses the r for.
- `is_global_scope` at line 164 through 191: `def is_global_scope(node)`
  Tests whether the the global scope.
- `class_scope` at line 194 through 200: `def class_scope(node, src: bytes)`
  Implements the class scope helper.
- `collect_c_family` at line 203 through 414: `def collect_c_family(path: Path)`
  Implements the collect c family helper.
- `add_function` at line 221 through 258: `def add_function(node)`
  Adds the function.
- `add_type` at line 260 through 305: `def add_type(node)`
  Adds the type.
- `walk` at line 307 through 411: `def walk(node)`
  Implements the walk helper.
- `collect_python` at line 417 through 500: `def collect_python(path: Path)`
  Implements the collect python helper.
- `docstring` at line 427 through 437: `def docstring(node)`
  Implements the docstring helper.
- `walk` at line 439 through 497: `def walk(node)`
  Implements the walk helper.
- `source_files` at line 503 through 523: `def source_files(include_dev: bool)`
  The tracked source and CMake files, from the git index.
- `md_escape` at line 526 through 527: `def md_escape(value: str)`
  Implements the md escape helper.
- `file_summary` at line 530 through 588: `def file_summary(path: Path)`
  The first prose sentence of the file's leading comment or heading.
- `write_files` at line 642 through 704: `def write_files(paths: list[Path], out: Path)`
  Writes the files.
- `write_types` at line 707 through 765: `def write_types(types: list[Type], out: Path)`
  Writes the types.
- `split_words` at line 919 through 924: `def split_words(name: str)`
  Splits a camelCase or snake_case identifier into lower-case words.
- `inner_comment` at line 927 through 962: `def inner_comment(f: Function, lines: list[str])`
  The prose comment at the top of the function body, as one sentence.
- `describe_function` at line 965 through 1236: `def describe_function(f: Function, lines: list[str])`
  Implements the describe function helper.
- `write_functions` at line 1239 through 1296: `def write_functions(functions: list[Function], out: Path)`
  Writes the functions.
- `write_symbols` at line 1299 through 1321: `def write_symbols(symbols: list[Symbol], out: Path)`
  Writes the symbols.
- `state_fields` at line 1324 through 1336: `def state_fields()`
  Implements the state fields helper.
- `write_state` at line 1339 through 1351: `def write_state(fields: list[tuple[str, str]], out: Path)`
  Writes the state.
- `write_parameters` at line 1354 through 1379: `def write_parameters(out: Path)`
  Writes the parameters.
- `render_walk` at line 1382 through 1462: `def render_walk()`
  Renders the walk.
- `write_nodes` at line 1465 through 1495: `def write_nodes(out: Path)`
  Writes the nodes.
- `write_tests` at line 1498 through 1538: `def write_tests(out: Path)`
  Writes the tests.
- `main` at line 1541 through 1574: `def main()`
  Runs the test or tool entry point.

### `tools/gen_math_tables.py`

- `q15` at line 40 through 42: `def q15(x: float)`
  Implements the q 15 helper.
- `q16` at line 45 through 47: `def q16(x: float)`
  Implements the q 16 helper.
- `c_array` at line 50 through 61: `def c_array(ctype: str, name: str, values: list[int], width: int)`
  Implements the c array helper.
- `rate_base` at line 64 through 68: `def rate_base()`
  Implements the rate base helper.
- `rate_r3_base` at line 71 through 75: `def rate_r3_base()`
  Implements the rate r 3 base helper.
- `keep_time` at line 78 through 82: `def keep_time(coefficient: int, old_rate: float, new_rate: float)`
  Transform a Q16 per-block coefficient to the new block period.
- `filter_g_q16` at line 85 through 93: `def filter_g_q16(rate: float)`
  The per-stage VCF coefficient at each log cutoff position.
- `build` at line 96 through 123: `def build(rate: float)`
  Implements the build helper.
- `render` at line 126 through 158: `def render(rate: float)`
  Implements the render helper.
- `main` at line 161 through 168: `def main()`
  Runs the test or tool entry point.

### `tools/gen_wavetable.py`

- `q15` at line 38 through 40: `def q15(x: float)`
  Implements the q 15 helper.
- `clamp15` at line 43 through 46: `def clamp15(x: float)`
  Round an already-Q15 value and clamp it into [-32768, 32767].
- `sin_q15` at line 53 through 57: `def sin_q15(phase: int)`
  Implements the sin q 15 helper.
- `q15_mul` at line 60 through 61: `def q15_mul(a: int, b: int)`
  Implements the q 15 mul helper.
- `sin_wave` at line 64 through 104: `def sin_wave(phase: int, ws: int)`
  The naive shape as a pure function of the top 10 phase bits.
- `band_limited` at line 107 through 131: `def band_limited(ws: int, harm: int)`
  The band-limited Q15 table for waveform `ws`, harmonics 1..harm.
- `c_array` at line 134 through 145: `def c_array(name: str, ctype: str, values: list[int])`
  Implements the c array helper.
- `build` at line 148 through 159: `def build()`
  Implements the build helper.
- `harmonics` at line 162 through 178: `def harmonics(wave: list[int], base: list[int])`
  Implements the harmonics helper.
- `render_harmonics` at line 181 through 197: `def render_harmonics(rows: list[list[int]])`
  Renders the harmonics.
- `render` at line 200 through 220: `def render(wave: list[int], base: list[int])`
  Implements the render helper.
- `main` at line 223 through 237: `def main()`
  Runs the test or tool entry point.
