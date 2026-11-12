# Feedback envelope: measured result

This is the engine evidence for
[`../feature-contract-feedback-envelope.md`](../feature-contract-feedback-envelope.md).
The page and its snapshots are the next milestone.

## Verdict

The engine side passes. The depth 0 render is bit-exact, the positive depth
brightens the spectrum, the negative depth darkens it, and the dynamic peaks stay
inside the stability margin. The 146 and 160 libraries still load.

## Build and setup

The measurement ran on Windows with clang-cl and lld-link, against product commit
`c38cc18` plus the local feedback-envelope changes. The test is
`build/windows-clangcl-lld/plugin/feedback_envelope.exe`.

The test uses one sine operator: note 60, velocity 100, ratio 1.0, level 99,
algorithm 7, the other three operators muted, a flat operator envelope, the filter
off and no amplitude LFO. It renders 16,384 frames at 44.1 kHz into a 2 x 16,384
buffer and analyses the left channel. The first window is 2,048..4,096 and the last
is 14,336..16,384, each with a Hann-windowed DFT at the exact harmonic frequencies.

The shared `fbenv` is a `dx_env` with `r1` 50, `r2`..`r4` 99 and `l1`..`l3` 99, so
it attacks slowly and holds. The positive case uses a base feedback of 0 and a
depth of +100 percent. The negative case uses a base of 50 percent and a depth of
-100 percent. The static cases use the same base with depth 0.

## Measurement

| Case | First centroid | Last centroid | First A3/A1 | Last A3/A1 | Peak |
| --- | --- | --- | --- | --- | --- |
| +100% | 2.249 | 2.251 | 0.275 | 0.337 | 11,977 |
| -100% | 2.882 | 1.005 | 0.397 | 0.002 | 11,616 |
| static full | n/a | n/a | n/a | n/a | 8,859 |
| static half | n/a | n/a | n/a | n/a | 9,143 |

The depth 0 render with the fast envelope shape and the depth 0 render with a
silent envelope are bit-identical.

## Pass conditions

- Depth 0 ignores the envelope shape: pass, bit-identical.
- Positive depth: the third-to-fundamental ratio rises from 0.275 to 0.337, 22
  percent higher. The centroid is almost flat because the base is 0 and the
  envelope opens into the rich mid-feedback range, so the harmonic ratio is the
  better measure.
- Negative depth: the centroid falls from 2.882 to 1.005, 65 percent lower, and the
  third-to-fundamental ratio falls to 0.002.
- Positive peak: 11,977 against 8,859 static full, 35 percent higher, inside the
  50 percent dynamic margin.
- Negative peak: 11,616 against 9,143 static half, 27 percent higher, inside the
  same margin.

`engine_parity` stays bit-exact against the captured reference. `preset_smoke`
loads the 146 and 160 layouts and round-trips the current 171 layout. `plugin_smoke`
exposes all 356 parameters and restores them.

## Interpretation

The shared envelope is audible and the per-operator depth is viable. The positive
depth opens the feedback over the note and the negative depth closes it. The
dynamic peak exceeds the endpoint static render by 27 to 35 percent, which is why
the contract uses a 50 percent stability margin rather than a 10 percent one. The
engine is integer and the level is clamped, so the render cannot run away.

The page follows the control map in the contract. It needs the envelope graph, the
seven shape knobs and the four depth knobs, and it needs the Linux snapshot
refresh.
