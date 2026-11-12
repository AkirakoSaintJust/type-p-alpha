# Bipolar feedback

This is the shipped feedback control after the envelope and shaper experiments. The
contract is
[`../../plugin/feature-contract-feedback-bipolar.md`](../../plugin/feature-contract-feedback-bipolar.md).

## The control

Each operator has one feedback knob from -100 to +100 percent. The envelope depth
stays per operator on the feedback page.

- **Positive half:** the amount is the knob value, the shaper is off and the filter
  is the two-sample average. This is the path before the experiments, bit-exact.
- **Negative half:** the amount is the knob magnitude, the feedback sample is
  squared and negated, and the same two-sample average is used. This is the
  Reface-style square family.
- **Zero:** the amount is 0, so the switch between the two paths is silent.
- **Envelope:** the depth offsets the magnitude. The knob sign selects the path, so
  the envelope can close the feedback but cannot flip it from square to saw.

The negative half uses the square and the average, not the one-pole. The scope
images in [`feedback-shaper/`](feedback-shaper/) show why: the square with the
average keeps a clean square through the middle and turns noisy at the top, which
is the snare and noise territory. The one-pole cleans that top end. The abs turns
noisy earlier and stays less square.

## Parameters and record

The feedback parameter becomes `opN_feedback`, -32767..32767, default 0, with the
`pct_bip` display. The per-operator depth `opN_fb_depth` and the shared `fbenv`
stay. The shape and filter parameters from the shaper experiment are removed.

The part block returns from 179 to 171 and the manifest from 372 to 356. The 179
experiment layout migrates by dropping the shape and filter values, and the 146,
160 and 171 layouts stay. The current 171 layout is the envelope layout with the
wider feedback range.

## Measurement

The shaper experiment measured the two halves. At 50 percent the positive path is
the reference. The negative square (the `square neg` row) moves the
third-to-fundamental ratio to 0.215 and the high-frequency energy to 6.2. The WAVs
and scope images at 25, 50, 75 and 100 percent are in
[`feedback-shaper/`](feedback-shaper/), so the negative character can be judged by
ear as well as by the table.

| Feedback | A1 | A3/A1 | HF energy | Peak |
| --- | --- | --- | --- | --- |
| +25% | 3,410 | 0.253 | 188.5 | 11,769 |
| -25% | 8,041 | 0.144 | 2.6 | 8,193 |
| +50% | 3,896 | 0.132 | 858.0 | 9,143 |
| -50% | 8,320 | 0.215 | 6.2 | 8,182 |
| +75% | 63 | 4.611 | 815.8 | 9,179 |
| -75% | 8,430 | 0.240 | 83.9 | 8,461 |
| +100% | 1,229 | 0.368 | 1,008.6 | 8,859 |
| -100% | 8,105 | 0.265 | 155.6 | 8,749 |

The positive top is the current chaotic region, and the table shows the near-zero
fundamental at 75 percent. The negative half stays square-like, and its
high-frequency energy grows from 2.6 at 25 percent to 155.6 at 100 percent, which is
the noise the square path keeps. The test checks the positive 50 percent reference,
the square family, the negative top noise, the square third harmonic, the envelope
depth and the peak bound. `engine_parity`, `preset_smoke` and `feedback_envelope`
also pass on the bipolar layout.

## Alternatives

The square and the one-pole are a research pair, the abs and the one-pole are
another. The shipped square and average was chosen for the top-end noise. Switching
the negative half to the abs is one line in `fb_tap`, and the shaper evidence and
the WAVs remain the reference for that decision.
