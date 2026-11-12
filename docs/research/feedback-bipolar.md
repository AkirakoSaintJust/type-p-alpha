# Bipolar feedback

This is the shipped feedback control after the envelope and shaper experiments. The
contract is
[`../../plugin/feature-contract-feedback-bipolar.md`](../../plugin/feature-contract-feedback-bipolar.md).

## The control

Each operator has one feedback knob from -100 to +100 percent. The envelope depth
stays per operator on the feedback page.

- **Positive half:** the amount is the knob value, the shaper is off and the filter
  is the two-sample average. This is the path before the experiments, bit-exact.
- **Negative half:** the amount is the knob magnitude, the feedback sample takes
  the absolute value and is negated, and the same two-sample average is used. This
  is the Reface-style pulse family.
- **Zero:** the amount is 0, so the switch between the two paths is silent.
- **Envelope:** the depth offsets the magnitude. The knob sign selects the path, so
  the envelope can close the feedback but cannot flip it from square to saw.

The negative half uses the absolute value and the average, not the one-pole. The
scope images in [`feedback-shaper/`](feedback-shaper/) show why: the abs is unruly
from the low end and stays close to the positive path's character, which is what the
instrument already sounds like. The square is the cleaner research alternative, and
the one-pole cleans the top end. Players can shape the unruly top with the per-part
filter after the operators, so the negative half keeps its own character.

## Parameters and record

The feedback parameter becomes `opN_feedback`, -32767..32767, default 0, with the
`pct_bip` display. The engine carries 32,767 steps, and the host automation has the
full step; the panel display is the signed percent. The per-operator depth
`opN_fb_depth` and the shared `fbenv` stay. The shape and filter parameters from the
shaper experiment are removed.

The part block returns from 179 to 171 and the manifest from 372 to 356. The 179
experiment layout migrates by dropping the shape and filter values, and the 146,
160 and 171 layouts stay. The current 171 layout is the envelope layout with the
wider feedback range.

## Measurement

The shaper experiment measured the two halves. At 50 percent the positive path is
the reference. The negative abs moves the third-to-fundamental ratio to 0.391 and
the high-frequency energy to 667.8, close to the positive 858.0. The WAVs and scope
images at 25, 50, 75 and 100 percent are in
[`feedback-shaper/`](feedback-shaper/), so the negative character can be judged by
ear as well as by the table.

| Feedback | A1 | A3/A1 | HF energy | Peak |
| --- | --- | --- | --- | --- |
| +25% | 3,410 | 0.253 | 188.5 | 11,769 |
| -25% | 8,155 | 0.235 | 163.0 | 8,607 |
| +50% | 3,896 | 0.132 | 858.0 | 9,143 |
| -50% | 6,061 | 0.391 | 667.8 | 8,964 |
| +75% | 63 | 4.611 | 815.8 | 9,179 |
| -75% | 3,195 | 0.589 | 721.0 | 8,930 |
| +100% | 1,229 | 0.368 | 1,008.6 | 8,859 |
| -100% | 1,874 | 0.304 | 743.8 | 8,707 |

The positive top is the current chaotic region, and the table shows the near-zero
fundamental at 75 percent. The negative abs stays unruly across the range, with the
high-frequency energy at 163.0 at 25 percent and 743.8 at 100 percent, close to the
positive half. The test checks the positive 50 percent reference, the abs family,
the negative top noise, the abs third harmonic, the envelope depth and the peak
bound. `engine_parity`, `preset_smoke` and `feedback_envelope` also pass on the
bipolar layout.

## Alternatives

The abs and the average are the shipped pair. The square and the one-pole remain the
research pair: the square is the cleaner pulse, and the one-pole cleans the top end.
Switching the negative half to the square is one line in `fb_tap`, and the shaper
evidence and the WAVs remain the reference for that decision.
