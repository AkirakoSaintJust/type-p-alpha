# Feedback polarity: measured result

This is the evidence for the bounded experiment in
[`../feature-contract-feedback-polarity.md`](../feature-contract-feedback-polarity.md).
The experiment asked whether a sign flip of the current self-feedback tap raises
the odd harmonics of a sine operator.

## Verdict

The stop criterion failed at 25 percent feedback. The `neg` parameter, the editor
toggle, the 164-parameter part layout and the `.p1lib` migration were not shipped.
The current 160-parameter layout stays current.

## Build and setup

The measurement ran on Windows with clang-cl and lld-link, against product commit
`cfb39f8` plus the local experiment node, macro, manifest and test. The test binary
was `build/windows-clangcl-lld/plugin/feedback_polarity.exe`. The separate
`engine_parity` test passed against the captured reference, so the positive path
(`neg` at 0) is bit-exact.

The test used one sine operator: note 60, velocity 100, ratio 1.0, level 99,
algorithm 7, the other three operators muted, a flat DX envelope (`r1..r4` and
`l1..l3` at 99), the filter off, and no amplitude LFO. It rendered 8,192 samples at
44.1 kHz, skipped the first 2,048, and applied a Hann-windowed DFT at the exact
harmonic frequencies. Each render used a fresh engine state. The output buffer
carried two blocks of slack because the capability chain processes whole blocks.

## Measurement

| Feedback | Sign | A2/A1 | A3/A2 | A5/A2 | Peak |
| --- | --- | --- | --- | --- | --- |
| 0% | pos | n/a | n/a | n/a | 8263 |
| 0% | neg | n/a | n/a | n/a | 8263 |
| 20% | pos | 0.701 | 0.240 | 0.036 | 11153 |
| 20% | neg | 0.692 | 0.247 | 0.048 | 11134 |
| 25% | pos | 0.261 | 0.783 | 0.391 | 11707 |
| 25% | neg | 0.264 | 0.749 | 0.422 | 11740 |
| 35% | pos | 0.266 | 1.371 | 0.791 | 10079 |
| 35% | neg | 0.681 | 0.191 | 0.259 | 10557 |
| 50% | pos | 0.537 | 0.430 | 0.091 | 8937 |
| 50% | neg | 0.285 | 1.437 | 0.280 | 9103 |
| 75% | pos | 0.980 | 0.233 | 0.175 | 9133 |
| 75% | neg | 0.522 | 0.564 | 1.215 | 8641 |
| 100% | pos | 0.488 | 1.036 | 0.245 | 8801 |
| 100% | neg | 1.216 | 0.291 | 0.454 | 8610 |

The 0 percent ratios are not meaningful because the sine has almost no second
harmonic. The two 0 percent renders were bit-identical, which is the real check at
that amount.

## Stop criterion

The contract requires a 25 percent relative rise in both ratios at 25, 50 and 75
percent feedback.

| Amount | A3/A2 change | A5/A2 change | Result |
| --- | --- | --- | --- |
| 25% | 0.783 to 0.749, 4 percent lower | 0.391 to 0.422, 8 percent higher | fail |
| 50% | 0.430 to 1.437, 234 percent higher | 0.091 to 0.280, 208 percent higher | pass |
| 75% | 0.233 to 0.564, 142 percent higher | 0.175 to 1.215, 594 percent higher | pass |

The 25 percent failure stops the feature. The 100 percent negative peak is 8,610
against 8,801 positive, so it is 2 percent lower and inside the 10 percent limit.

## Interpretation

The sign flip is not a monotonic odd-harmonic control. At 20 percent the change is
small. At 25 percent the third harmonic falls and the fifth rises by less than the
threshold. At 35 percent both odd ratios fall. At 50 and 75 percent both rise
strongly, and at 100 percent the third harmonic falls again. The sign changes the
phase relationship of the sidebands, and at high feedback it moves the loop into a
different regime, but the association between negative feedback and odd harmonics
does not hold across the useful range.

The result belongs to this tap. The two-sample average and the one-sample delay are
nearly symmetric in time, and a sine carrier's sideband magnitudes are even in the
modulation index. A one-pole feedback tap or an odd-symmetric nonlinearity in the
loop would be a different experiment. The general amplifier meaning of negative
feedback, where a symmetric stage cancels even-order distortion and leaves odd-order
distortion, does not carry over to a sign flip of a phase-modulation index.

The shared feedback envelope with a per-operator depth is the next feedback feature.
It modulates the feedback magnitude, which the spectrum follows.

## Cleanup

The experiment node, macro, manifest entries, regenerated engine and test were
removed. The product tree carries this note and the updated design note and
contract. The 160-parameter layout and the 146-to-160 migration are unchanged.
