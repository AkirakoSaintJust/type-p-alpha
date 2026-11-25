# Phase map

The phase map is a per-part stochastic transfer curve between the operator phase
accumulator and the wavetable read. It has two stages:

```text
acc -> F -> G -> wavetable
```

`F` is a one-breakpoint polyline. `G` is a segmented map with 1 to 32 linear
segments, each with its own slope, direction and position in the output order.
The map is shared by every voice on a part, so a new note can reshape a held one.

The generated node is in `.dev/modules/nodes/phase_map/node.c` and the generated
copy is in `engine/gen/type_p_alpha_dsp.c`. The display snapshot is in
`plugin/engine/Engine.cpp`. The read-mode constants are in
`plugin/engine/PhaseMapLayout.h`.

## The pool

Each operator has one `int16_t[18432]` pool, 36,864 bytes. The pool is divided
into two parts. Each part has a fixed 2,560-word base and an optional direct-read
table. The base holds:

| Word range | Contents |
| --- | --- |
| 0 to 64 | 33 input boundaries, 0 to 4,096 |
| 33 to 64 | 32 natural slopes, Q16 |
| 65 to 96 | 32 natural flip flags |
| 97 to 128 | 32 one-based order entries |
| 129 to 160 | 32 composed output y values |
| 161 to 192 | 32 composed slopes, Q16 |
| 193 to 1,216 | 1,024 words, four packed segment indices per word |
| 1,217 to 2,240 | 1,024 words, F in 10-bit units |
| 2,241 | last breakpoint |
| 2,242 | PRNG seed |
| 2,243 | seeded flag |
| 2,244 | breakpoint |
| 2,245 | last segment count |
| 2,246 to 2,248 | drift, flip and reorder accumulators |
| 2,249 to 2,281 | 33 snapped y boundaries |
| 2,282 to 2,409 | 128 words, 4,096 protected-point bits |
| 2,410 | mask id |
| 2,411 | lock bits |
| 2,412 | active segment count |
| 2,413 | last waveform |
| 2,414 | last on |

The direct table starts at word 2,560. The table is 16-bit and interleaves with
the base in the same pool.

## The F stage

`F` is a one-breakpoint polyline over the 12-bit phase grid. The breakpoint is
the `breakpoint` parameter from 1 to 999, stored as `bp`. The left side maps
`0..bp` onto `0..bp*4096/999`, and the right side maps the rest onto the upper
range. The slope changes at the breakpoint, so the map can compress one side and
stretch the other. The 1,024-entry `F` table stores the result in 10-bit units.

The breakpoint is modulated by the phase LFO and the signed phase envelope.
Both are per part and are added before the table lookup. The key and velocity
follow can also move the breakpoint. The map therefore changes on the control
tick, not per sample.

## The G stage

`G` cuts the warped phase into `segments` spans. Each span has:

- a start boundary in the input phase,
- a natural slope from 0.5 to 2.66 in Q16,
- a flip bit that reverses the output direction,
- an order position.

The composed output is built by walking the spans in order. The composed y at
each input boundary is stored in `PY0`, and the composed slope is stored in
`PSLOPE`. The y boundaries snap away from the waveform's protected points. A
span whose range contains a protected point is locked. A locked span never flips
and never moves in a reorder.

The lock rule is what keeps the waveform's discontinuity from being folded back
on itself. The protected points come from the waveform table. For the fixed 14
waves the node reads the mip-0 table and marks the large steps. For wave 14 it
builds the a and b masks from the regenerated additive bank and combines them
with the base morph. The map therefore follows the spectral morph without
reading a voice's live morph.

## The stochastic events

The map has three clocks:

- **Drift** bends the segment slopes around their natural values.
- **Flip** reverses a span's output direction.
- **Reorder** swaps a span with a later span in the order.

Each clock has a rate from 0 to 20,000,000 raw, which is about 0.01 to 223 Hz. The
event amount is a percentage. The clocks use a per-part deterministic PRNG seeded
with `0x9E3779B9 + part*4`, so a fixed seed and a fixed event sequence give the
same geometry on every build.

A flip is equivalent to changing the slope sign and rotating the output around
the span. A reorder is a local reflected swap: the code swaps `order[i]` and
`order[j]` and reflects the intervening order. A jump of 1 is an adjacent swap.
A jump past the last active position reflects back, so the wrap from last to
first is gone. A larger jump widens the swap without returning to a full
permutation.

## The read modes

`P1_PHASE_READ` selects the read path:

| Mode | Read |
| ---: | --- |
| 0 | the F and segment tables, the reference |
| 1 | the exact 4,096-entry input-phase table |
| 2 | the 512-step input table |
| 3 | the 512-linear input table |
| 4 | the 1,024-linear input table |
| 5 | the 256-step input table |
| 6 | the 256-linear input table |
| 7 | the 256-step post-F table |
| 8 | the 256-linear post-F table |
| 9 | the 512-linear post-F table |
| 10 | the 1,024-linear post-F table |
| 11 | the 1,024-entry exact post-F table |

Modes 1 to 6 are indexed by the input phase, so they undersample a compressed
breakpoint. Modes 7 to 11 are indexed by the post-F `q`, so they follow the
compressed breakpoint. Mode 11 is the shipped read. It is exact because `q` is
always a multiple of four and the table is indexed by `q >> 2`. The 4,095 clamp
uses one tail entry.

The table is rebuilt when the segment layout, the mask, the drift, the flip or
the reorder changes. It is not rebuilt on a breakpoint sweep, because the
breakpoint moves the `F` stage and the table is in `q`. That is why the
breakpoint LFO and envelope are cheap.

The table is 1,025 `int16_t` per part. The phase-map state field is sized on its
own: 18,432 `int16_t` for the two parts and the 4,096 reference.

## The display snapshot

The editor does not read the pool. `Engine::phaseMapSnapshot` copies the
geometry and the newest voice's mapped phase into a `PhaseMapSnapshot`:

```cpp
struct PhaseMapSnapshot {
    bool valid;
    int bp;
    int segments;
    int active;
    int voices;
    uint16_t phaseQ;
    uint16_t phaseY;
    std::array<uint16_t, 33> x;
    std::array<uint16_t, 32> y0;
    std::array<int32_t, 32> slope;
    std::array<uint8_t, 32> flip;
    std::array<uint8_t, 32> order;
    std::array<uint8_t, 32> locked;
};
```

The snapshot is published through the three-buffer handoff in
`publishPhaseMaps`. The reader picks the buffer with the new-data bit and copies
it. The writer never writes the buffer the reader holds.

The newest voice is the one with the highest `voice_age` among the voices that
are gated or still releasing for the part. A chord therefore shows the note that
was played last, not whichever voice ran last.

## The UI

`PhaseMapView` draws the composed curve, the fixed y boundaries, the `F` polyline,
the breakpoint and the newest voice's dot. The forward segments are green and the
flipped segments are purple. Locked spans are 35% opacity. Order jumps are pink
dashed verticals. The view repaints only when the snapshot signature changes.

The phase page also has the four operator columns with the compact oscillator,
the phase controls, the phase LFO and the phase envelope. The phase controls are
`bp`, `segments`, `flip`, `flip rate`, `reorder`, `reorder rate`, `jump`, `drift`,
`drift rate`, `on` and the mode. The LFO and envelope are the same `lfo` and
`mod_env` blocks with per-part pools.

## CPU and limits

The 2x map/table read was designed but not built. The graph's one-rate model
cannot express the 2x path, so the shipped read is 1x. The measured worst case is
about 1.5x the phase-off baseline at 24 voices, four operators, 32 segments, the
maximum event rates, both modulation sources and 1,000 blocks. The 1.6x budget
was for the 2x design. The 1x path leaves little headroom, so the 2x path is not
shipped.

The aliasing limit is the 20.5 to 24 kHz fold-back from the joins. The joins are
step discontinuities, so the map does not promise a clean spectrum. The exact
post-F table is sample-identical to the F/segment reference on the 140-scenario
perceptual battery and on the full engine test set.

The tests are in `plugin/tests/phase_map.cpp`,
`plugin/tests/phase_map_perceptual.cpp` and
`plugin/tests/phase_map_perceptual_compare.cpp`. The contract is
`plugin/feature-contract-stochastic-phaseshaping.md`.
