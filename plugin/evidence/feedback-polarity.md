# Feedback polarity: measured result

This is the evidence for the bounded experiment in
[`../feature-contract-feedback-polarity.md`](../feature-contract-feedback-polarity.md).
The experiment asked whether a sign flip of the current self-feedback tap raises
the odd harmonics of a sine operator.

## Verdict

The stop criterion failed at 25 and 50 percent feedback. The `neg` parameter, the
editor toggle, the 164-parameter part layout and the `.p1lib` migration were not
shipped. The current 160-parameter layout stays current.

## Build and setup

The measurement ran on Windows with clang-cl and lld-link, against product commit
`0d16765` plus the local experiment node, macro, manifest and test. The test binary
was `build/windows-clangcl-lld/plugin/feedback_polarity.exe`. The separate
`engine_parity` test passed against the captured reference, so the positive path
(`neg` at 0) is bit-exact.

The test used one sine operator: note 60, velocity 100, ratio 1.0, level 99,
algorithm 7, the other three operators muted, a flat DX envelope (`r1..r4` and
`l1..l3` at 99), the filter off, and no amplitude LFO. It rendered 8,192 frames at
44.1 kHz, skipped the first 2,048, and applied a Hann-windowed DFT at the exact
harmonic frequencies. Each render used a fresh engine state. The engine output is
stereo interleaved, so each render used a buffer of 2 x 8,192 samples and the
analysis read the left channel.

## Measurement

| Feedback | Sign | A2/A1 | A3/A2 | A5/A2 | Peak |
| --- | --- | --- | --- | --- | --- |
| 0% | pos | n/a | n/a | n/a | 8263 |
| 0% | neg | n/a | n/a | n/a | 8263 |
| 20% | pos | 0.465 | 0.525 | 0.256 | 11153 |
| 20% | neg | 0.462 | 0.524 | 0.256 | 11134 |
| 25% | pos | 0.585 | 0.483 | 0.216 | 11748 |
| 25% | neg | 0.585 | 0.487 | 0.213 | 11747 |
| 35% | pos | 1.219 | 1.382 | 0.489 | 10176 |
| 35% | neg | 1.095 | 0.658 | 0.166 | 10686 |
| 50% | pos | 0.760 | 0.239 | 0.125 | 8937 |
| 50% | neg | 0.788 | 0.184 | 0.076 | 9103 |
| 75% | pos | 7.662 | 0.330 | 0.142 | 9133 |
| 75% | neg | 0.770 | 0.471 | 0.274 | 8641 |
| 100% | pos | 0.566 | 0.898 | 0.345 | 8859 |
| 100% | neg | 0.575 | 0.508 | 0.187 | 8610 |

The 0 percent ratios are not meaningful because the sine has almost no second
harmonic. The two 0 percent renders were bit-identical, which is the real check at
that amount.

## Stop criterion

The contract requires a 25 percent relative rise in both ratios at 25, 50 and 75
percent feedback.

| Amount | A3/A2 change | A5/A2 change | Result |
| --- | --- | --- | --- |
| 25% | 0.483 to 0.487, 1 percent higher | 0.216 to 0.213, 1 percent lower | fail |
| 50% | 0.239 to 0.184, 23 percent lower | 0.125 to 0.076, 39 percent lower | fail |
| 75% | 0.330 to 0.471, 43 percent higher | 0.142 to 0.274, 93 percent higher | pass |

The 25 and 50 percent failures stop the feature. The 75 percent pass does not
rescue it. At 75 percent the positive A2/A1 is 7.662, which means the positive
second harmonic is nearly zero, so the ratio is unstable and the row is a regime
artefact rather than a usable odd-harmonic control.

The 100 percent negative peak is 8,610 against 8,859 positive, so it is 3 percent
lower and inside the 10 percent limit.

## Interpretation

The sign flip is not a reliable odd-harmonic control. At 20 and 25 percent the two
signs are the same within 2 percent. At 35 and 50 percent the negative feedback
lowers the odd ratios. At 75 percent the negative ratios rise, but the positive
second harmonic is nearly zero. At 100 percent the negative ratios fall again. The
sign changes the phase relationship of the sidebands, and at high feedback it moves
the loop into a different regime, but the association between negative feedback and
odd harmonics does not hold across the useful range.

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
