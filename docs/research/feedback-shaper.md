# Feedback shaper: measured result

This is the evidence for
[`../feature-contract-feedback-shaper.md`](../../plugin/feature-contract-feedback-shaper.md).
The shape and filter choices shipped on the feedback page.

## Verdict

The shaper passes. The square and abs shapes change the spectrum, their negative
forms differ, the one-pole lowers the high-frequency energy, and the peaks stay
bounded. The signed square is a phase variant in the magnitude spectrum.

## Build and setup

The measurement ran on Windows with clang-cl and lld-link, against product commit
`cda8d3b` plus the shaper changes. The experiment test wrote the WAVs and the scope
renderer read them; the files are committed in `feedback-shaper/`. The shipped
control is the bipolar feedback knob, and the `feedback_bipolar` test covers it.

The test uses one sine operator: note 60, velocity 100, ratio 1.0, level 99,
algorithm 7, the other three operators muted, a flat operator envelope, the filter
off and no amplitude LFO. It renders 16,384 frames at 44.1 kHz into a 2 x 16,384
buffer and analyses the left channel in the last 8,192 samples with a
Hann-windowed DFT at the harmonics and at 8 to 20 kHz for the high-frequency
energy.

## Measurement

At 50 percent feedback, with the two-sample average:

| Shape | A1 | A3/A1 | A5/A1 | DC | HF energy | Peak |
| --- | --- | --- | --- | --- | --- | --- |
| off | 3,896 | 0.132 | 0.100 | 76.5 | 858.0 | 9,143 |
| square | 6,733 | 0.169 | 0.054 | -14.8 | 5.6 | 8,353 |
| square neg | 8,320 | 0.215 | 0.078 | -51.7 | 6.2 | 8,182 |
| abs | 5,723 | 0.263 | 0.007 | -63.8 | 658.6 | 8,830 |
| abs neg | 6,061 | 0.391 | 0.058 | -37.6 | 667.8 | 8,964 |
| signed | 6,696 | 0.198 | 0.068 | -4.8 | 9.7 | 9,681 |
| signed neg | 6,696 | 0.198 | 0.068 | -60.2 | 10.2 | 9,679 |

At 75 percent feedback, the high-frequency energy:

| Shape | Average | One-pole |
| --- | --- | --- |
| abs | 975.7 | 578.9 |
| square | 113.2 | 11.3 |

The one-pole also lowers the 50 percent energy, for example the abs negative form
from 667.8 to 152.6 and the off shape from 858.0 to 491.2.

## Pass conditions

- The square shape changes the spectrum: A3/A1 moves from 0.132 to 0.169, and the
  high-frequency energy from 858 to 5.6.
- The abs shape changes the spectrum: A3/A1 moves to 0.263 and the high-frequency
  energy to 658.6.
- The negative forms differ: square 0.169 to 0.215 in A3/A1, abs 0.263 to 0.391.
- The signed square has the same magnitude for both signs, which is expected for an
  odd shaper. Its positive form has the least DC, -4.8 against -14.8 and -63.8.
- The one-pole lowers the high-frequency energy for both abs and square at 75
  percent, by 41 and 90 percent.
- The peaks are between 8,182 and 9,681, and the one-pole off case is 12,021. All
  are bounded.

`engine_parity` stays bit-exact with the captured reference, because the off shape
and the average filter are the current path. `preset_smoke` loads the 146, 160 and
171 layouts and round-trips the current 179 layout. `plugin_smoke` exposes all 372
parameters and restores them.

## Interpretation

The square and abs shapers are the Reface-style family. The square is the cleanest:
its high-frequency energy collapses and the fundamental rises. The abs keeps more
high-frequency energy and its negative form is the strongest sign change in A3/A1.
The signed square is a symmetric alternative, but its sign is a phase flip in the
magnitude spectrum, so it is the least useful of the three as a tone control.

The one-pole filter earns its place. It lowers the noisy top of the feedback range
without changing the default path, because the average is still the default. The
filter is per operator, like the shape and the feedback amount.

The experiment page carried the three controls per operator: the depth knob, the
shape choice and the filter choice. The shipped page keeps the envelope and the
four depths, and the bipolar feedback knob lives on the operator card.

## Waveforms and listening files

The scope images show the last 2,048 samples of a two-second note at 25, 50, 75 and
100 percent, from top to bottom. The four traces are the four amounts.

- Positive reference: [scope](feedback-shaper/scope-positive.png), WAVs at
  [25](feedback-shaper/positive-25.wav),
  [50](feedback-shaper/positive-50.wav),
  [75](feedback-shaper/positive-75.wav) and
  [100](feedback-shaper/positive-100.wav).
- Square, two-sample average: [scope](feedback-shaper/scope-square-avg.png), WAVs
  at [25](feedback-shaper/square-avg-25.wav),
  [50](feedback-shaper/square-avg-50.wav),
  [75](feedback-shaper/square-avg-75.wav) and
  [100](feedback-shaper/square-avg-100.wav).
- Square, one-pole: [scope](feedback-shaper/scope-square-lp.png), WAVs at
  [25](feedback-shaper/square-lp-25.wav),
  [50](feedback-shaper/square-lp-50.wav),
  [75](feedback-shaper/square-lp-75.wav) and
  [100](feedback-shaper/square-lp-100.wav).
- Abs, two-sample average: [scope](feedback-shaper/scope-abs-avg.png), WAVs at
  [25](feedback-shaper/abs-avg-25.wav),
  [50](feedback-shaper/abs-avg-50.wav),
  [75](feedback-shaper/abs-avg-75.wav) and
  [100](feedback-shaper/abs-avg-100.wav).
- Abs, one-pole: [scope](feedback-shaper/scope-abs-lp.png), WAVs at
  [25](feedback-shaper/abs-lp-25.wav),
  [50](feedback-shaper/abs-lp-50.wav),
  [75](feedback-shaper/abs-lp-75.wav) and
  [100](feedback-shaper/abs-lp-100.wav).

The traces show the choice. The square with the average keeps a clean square through
the middle and turns noisy at the top. The one-pole cleans that top end. The abs is
unruly from the low end and stays close to the positive path's character. The
shipped bipolar control uses the abs and the average, because the instrument's
positive feedback is already unruly and the per-part filter can shape the top. The
square and the one-pole stay as research alternatives, and this directory keeps
their measurements.

The experiment test wrote the WAVs when it was run with an output directory. The
files are committed here. The figure script rereads them with the standard library
only:

```sh
python plugin/tools/feedback_figures.py docs/research/feedback-shaper
```
