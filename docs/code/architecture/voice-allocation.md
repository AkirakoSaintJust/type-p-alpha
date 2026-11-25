# Voice allocation

The allocator is in the generated
`type_p_alpha_dsp_note_on_ch_s`, `type_p_alpha_dsp_note_off_ch_s`,
`type_p_alpha_dsp_all_notes_off_ch_s` and `type_p_alpha_dsp_set_bend_ch_q16_s`
functions. The plugin feeds it through `p1::MidiBridge`. This chapter follows the
actual branches.

## The pool

The pool has 24 voices. Each voice has a part, a note, a velocity, a gate, a
release counter, an age and a pitch increment. The per-voice arrays are:

```c
uint32_t voice_age[NVOICE];
int32_t voice_velocity[NVOICE];
uint32_t voice_delay[NVOICE];
uint32_t voice_inc[NVOICE];
uint32_t voice_target_inc[NVOICE];
int32_t voice_detune[NVOICE];
uint8_t voice_gate[NVOICE];
uint8_t voice_trig[NVOICE];
uint8_t voice_note[NVOICE];
uint8_t voice_part[NVOICE];
uint8_t voice_rel[NVOICE];
```

The mono tables are:

```c
uint8_t mono_held[N_PART][128];
uint32_t mono_order[N_PART][128];
uint8_t part_mono_voice[N_PART][8];
uint8_t part_mono_count[N_PART];
```

`voice_rel` is a `uint16_t` release counter. A released voice keeps rendering
until the counter reaches zero. The note-off path sets it to 8,916 control
blocks, about 5.9 s at the slowest release, and the voice block decrements it
once per control block. The tail outlasts the longest envelope release, so the
voice is never cut off while the envelope is still audible.

`voice_delay` is the part delay in control blocks. A part delay of one step is
48 samples at 48 kHz, so the note-on path stores `part_delay * 48`. A delayed
voice is not gated yet. The pre-block counts the delay down and opens the gate
when it reaches zero. This is how the part delay avoids starting a note in the
middle of a block.

`voice_age` is a monotonically increasing serial. It is used for stealing and
for the "newest voice" phase-map display. A higher age is newer.

## Channel and key-zone routing

A note-on arrives with a MIDI channel, or with `0xFFFFFFFF` for omni. The
allocator visits both parts:

```c
if (part_channel[p] != -1 && channel != 0xFFFFFFFFu &&
    part_channel[p] != (int32_t)channel) continue;
if (note < part_low[p] || note > part_high[p]) continue;
```

A part channel of -1 is omni. A note below the low key or above the high key
does not reach that part. If the note reaches both parts, each part gets its own
voice. That is how a layered patch works.

The note-off path computes a `part_match` table first and then applies it to both
the mono and poly paths. A note-off on channel 3 therefore releases only the
parts that listen to channel 3. The omni panic `all_notes_off` ignores the
channel.

## Poly allocation

The poly path allocates one voice per matching part. The pick loop is:

```c
uint32_t best = 0xFFFFFFFFu, bp = 0;
for (uint32_t v = 0; v < NVOICE; v++) {
    if (chosen[v]) continue;
    uint32_t score = (voice_gate[v] || voice_delay[v] > 0u)
        ? 0x80000000u + voice_age[v]
        : voice_age[v] + 1u;
    if (score < best) { best = score; bp = v; }
}
chosen[bp] = 1;
```

The score has two ranges. A free voice has score `age + 1`, which is small. An
active or delayed voice has score `0x80000000 + age`, which is always larger. The
allocator therefore prefers any free voice, and when none is free it steals the
oldest active voice. `chosen` is shared across the two parts in the same note-on,
so a layered note cannot pick the same voice twice.

The selected voice gets the part, the note, the velocity, the age and the
detune. In poly the detune is `p1_tune_q16(transpose, detune)` applied to a
unity 65,536 base, so every voice of the part shares it. A mono stack voice
multiplies that base by its own unison offset. The note increment is

```c
voice_target_inc = (note_to_inc(note) * voice_detune) >> 16;
```

Poly always snaps the current increment to the target. The `voice_trig` flag is
set to 1, so the envelopes restart.

A delayed note stores the gate closed and lets the pre-block open it when the
delay expires. That path recomputes the note increment, sets `voice_trig` and
updates the operator increments.

## Mono allocation

Mono mode is selected by `part_mode == 1`. The part keeps a stack of held notes
in `mono_held` and a serial in `mono_order`. `voice_mono_active` chooses the
active note under the part's note priority:

| Priority | Choice |
| ---: | --- |
| 0 | the most recently held note |
| 1 | the highest note |
| 2 | the lowest note |

The mono path also supports unison. `part_mono_count` is the number of stacked
voices. The code grows or shrinks the stack when the unison count changes. The
unison detune is spread from `-span/2` to `+span/2` across the stack, where each
cent is `38` in the Q16 detune scale. The stack voices are chosen from the free
or oldest voices in the same way as poly.

`part_mono_voice[p][0]` is the lead voice for priority and legato. If the active
note changes, every stacked voice is retuned. The `legato` flag decides whether
the envelopes retrigger. `voice_trig` stays 0 when legato is on and a note was
already sounding. The `portamento` flag decides whether the increment snaps or
glides. The pre-block glides toward `voice_target_inc` with

```c
step = (target * 21845 / portamento) >> 16;
```

and clamps at the target. The 21,845 is the scaling that makes the longest
portamento the expected musical time.

A delayed first note after silence is handled by `was`, which is the gate of the
lead voice before the new note. The part delay applies only when `was` is false,
so a legato note does not restart the delay.

## Note-off

The note-off path first removes the note from the mono held table:

```c
if (mono_held[p][note]) mono_held[p][note]--;
active = voice_mono_active(p, priority);
```

If no held note remains, every stacked voice gets `gate = 0` and
`release = 8916`. If another note remains, the stack retunes to the new active
note. Legato decides whether the envelopes retrigger. Portamento decides whether
the increment snaps.

The poly path is:

```c
for (v = 0; v < NVOICE; v++)
    if (voice_note[v] == note && part_match[voice_part[v]]) {
        voice_gate[v] = 0;
        voice_rel[v] = 8916u;
        voice_delay[v] = 0;
    }
```

A voice can be released while it is still delayed, in which case the delay is
cleared and the voice never sounds. The release counter keeps the voice in the
render loop. `type_p_alpha_dsp_quiet_s` reports a voice as active while its gate
is open, so a release tail does not keep the host's "quiet" check busy.

## All notes off and panic

`all_notes_off_ch` releases the voices on the matching parts. It sets gate 0,
release 0 and delay 0, so the voices stop immediately rather than running the
8,916-block tail. It clears the mono held table for the matching parts.

`all_notes_off` is the omni wrapper. `releaseResources` calls it when the host
releases the processor.

The MIDI CC 123 path in `engine/runtime/synth_midi.c` calls `all_off_ch` for the channel and
clears the sustain state and the deferred note-offs for that channel. CC 64
sustain defers note-offs per channel. A fresh note-on cancels a deferred off for
the same note.

## Bend

The bend path receives a 14-bit value with 8,192 as centre. It subtracts 8,192,
multiplies by the part bend range and scales to cents:

```c
b = bend - 8192;
cents = b * part_bend_range * 100 / 8192;
```

It converts the cents to octave, semitone and cent, then calls `p1_tune_q16` to
get the part ratio. The part ratio is stored in `part_bend_q16`. Every gated
voice recomputes its operator increments through `type_p_alpha_note_derived`.

## The plugin chunk note set

`p1::ChunkNoteSet` is a 16 by 128 bit array. It tracks the notes that started in
the current control chunk. At 48 kHz the processor resets it at the start of
each 32-sample block. A note-off for a note that started in the same chunk is
deferred to `deferredOffs_` and applied after the chunk renders. That gives the
note one control block of sound instead of cancelling it before the first sample.

The set is keyed by MIDI channel and note, so two channels can hold the same
pitch without deferring each other's releases.

At a host rate other than 48 kHz the processor resets the set at the start of the
host block, feeds any scheduled events that are still pending, then maps the new
host events to core frames and stores them in `scheduled_`. The `coreChunk`
callback resets the chunk set, feeds the events whose core frame is inside the
next control block, and then publishes the tap and snapshot data.

## The tests

`plugin/tests/voice_alloc.cpp` covers the pool, the two parts, poly stealing,
mono priority and the release tail. `plugin/tests/plugin_smoke.cpp` covers the
host MIDI path, the chunk note set, the channel routing and the sustain deferral.
`plugin/tests/engine_parity.cpp` checks that the note-derived path matches the
reference render. The full test list is in
[`reference/tests.md`](../reference/tests.md).
