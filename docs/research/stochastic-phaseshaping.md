# Stochastic phase shaping

Status: written 2026-11-21. The Phase Transfer Lab was a browser instrument I
built to answer the first line of research in the [README](../../README.md):
where the breakpoint of a phase-distortion transfer function should sit, and
whether phase distortion and phase modulation can share one phase map without
extra machinery. It answered the first half in my own listening and left the
second alone. It never became a measurement: no test, no CPU figure, no
recording, and one listener, me.

The lab is not in this repository, and nothing from it has shipped into Type
P.α. Phase distortion is still an open line in the README, so read this as the
record of an experiment rather than a description of the instrument.

## The lab

A phasor runs at 20 to 880 Hz and wraps at 1. Its phase goes through two
piecewise-linear maps before it reaches a table:

```
φ -> F(φ) -> G -> wavetable
```

`F` is a single breakpoint that warps the phase. `G` cuts the warped phase into
segments whose slopes wander, whose direction can flip, and whose order can
change while the note sounds. The table is one of 13 single-cycle waves, so the
oscillator never changes shape. Only the way the phase walks through it does.

## The pre-map

```
F(φ) = 0.5 · φ / x_b                        for 0 ≤ φ ≤ x_b
F(φ) = 0.5 + 0.5 · (φ - x_b) / (1 - x_b)    for x_b ≤ φ ≤ 1
```

The breakpoint sits at y = 0.5 and moves along x between 0.001 and 0.999, with
0.5 as the default. The limits are there to keep the two slopes finite, which is
a numerical reason rather than a musical one.

A sine LFO at 0 to 30 Hz with an amount of 0 to 100 percent moves `x_b`, and so
does an ADSR envelope with an attack, decay and release of 0 to 5 s, a sustain
of 0 to 100 percent and an amount of plus or minus 100 percent. The envelope
triggered on start and stop. Its amount defaulted to 0 and I never fixed an LFO
default, so in practice a lab patch began with `F` standing still.

`F` is continuous at the breakpoint, so nothing in it clicks. All of the
discontinuity comes later.

## The wave bank

Single-cycle tables, normalised to -1 to +1:

| Wave | Contents |
| --- | --- |
| sine | one full cycle |
| triangle | one full cycle |
| square, band-limited | 8 harmonics, one full cycle |
| saw, band-limited | 8 harmonics, one full cycle |
| quarter-sine pulse | sine from 0 to π/2, then silence |
| half-sine pulse | sine from 0 to π, then silence |
| half-triangle pulse | triangle from 0 to π, then silence |
| two half-sines | two pulses inside the first π, then silence |
| two half-triangles | two pulses inside the first π, then silence |
| full sine, first half | a full sine on 0 to π, then silence |
| full triangle, first half | a full triangle on 0 to π, then silence |
| rectified sine | the positive lobe for the whole cycle |
| rectified triangle | the positive lobe for the whole cycle |

Sine was the default. The two "first half" tables were ambiguous in my own spec
about what happened to the negative half, and the reading above is the one the
lab used. I never went back and checked that reading against the generator, so
an implementation that mirrors instead of repeating would fit the words just as
well.

## The segment map

`G` divides the warped phase into `M` segments, 1 to 256, with `M + 1`
breakpoints running from (0, 0) to (1, 1). The points start evenly spaced in x
and y, so every slope starts at 1. Each segment is linear, with slope

```
s_i = (y_i+1 - y_i) / (x_i+1 - x_i)
```

held between 0.5 and 2.66. That's the same as holding the inverse between 0.375
and 2, which is the form the original note used. I fixed both bounds before
listening rather than deriving them, and never revisited the choice.

Each control tick adds a Gaussian step to the slope, reflects it if it has
crossed a bound, moves the segment's right-hand x to match, and then
renormalises the points so the last one is still 1. The phasor wrap follows that
end point, so the last sample of the table still gets read. The ticks ran at up
to 30 Hz, and the drift rate defaulted to 1.2 Hz.

## Zero crossings

Before anything drifts, the boundaries are adjusted so that none of them falls
inside a zero crossing or a silent flat. A sign change and a run of zero samples
both count as a zero, and a whole silent run collapses into one segment, because
several boundaries inside a flat have nothing to divide. A segment that touches
a zero is then locked: it can't flip and it can't be reordered.

The lock is there to avoid ambiguity, and I should be plain that I never tested
the alternative. Across a zero there is no slope to speak of, and the table has
nothing to read in the flat, so the constraint keeps a question from coming up
rather than answering it.

## Flips

Each eligible segment can be flipped, so it is walked from its right-hand point
back to its left-hand one and the map takes a negative slope. At each flip tick
every eligible segment re-rolls against a flip probability from 0 to 100
percent. The ticks ran at 0 to 30 Hz with 2.0 Hz as the default, and at that
rate I heard each flip as an event rather than as a texture.

## Reordering

The other layer changes the order in which the segments are walked. Normally the
phasor goes 0, 1, 2 and so on. With reordering on, a new order is drawn at 0 to
30 Hz with a probability from 0 to 100 percent, and the jump range sets how far
forward a segment may leap, as a percentage of `M - 1`. One order per cycle
means every segment is still visited exactly once. What changes is the join: `G`
is now disconnected where the order jumps, and those steps in the map are where
the inharmonicity comes from.

The design allowed two ways to draw the order, a global permutation of all `M`
segments or a local jump with wraparound. At 100 percent probability, a 100
percent jump range and a fast rate, either one ends up as close to a new
permutation every tick as the control rate allows.

## The lookup

The composed map is `T(φ) = G(F(φ))`, and the sample is the table at
`floor(T · (N - 1))`, read with linear interpolation. Only the phase moves.
Radna's DSWS gives each segment an amplitude offset and interpolates it across
the join; there are no offsets here, so that part of the method doesn't apply.

## The sound

With small steps, no flips and the natural order, it's a phase-distortion
shaper with a breakpoint you can sweep, which is the part I expected. Flips add
negative slopes and knees, and the timbre takes on the harder edge I associate
with odd harmonics, though I never measured the spectrum to check that. In the
middle of the range the peaks move around in a way that reminded me of a talkbox
made of geometry. At 100 percent reorder probability, a 100 percent jump range
and 30 Hz, the traversal is close to random and the result is noise. At the slow
end, with drift under 2 Hz, the timbre glides and pulses rather than jumping.

That's a listening note, not a result. No spectrum, no recording and no second
listener.

## Open questions

- **The sharing question.** One map for phase distortion and phase modulation is
  still open. The lab only did the distortion half.
- **Aliasing.** At 48 kHz the high-frequency content grew at very high drift
  rates. The joins are step discontinuities: the table is band-limited but the
  map is not, and I added no anti-alias filter. Kleimola and Radna both flag the
  risk. The fix would be minBLEP or oversampling, and neither was built.
- **Defaults.** The envelope amount started at 0, the LFO amount and the
  envelope times were never fixed, and the bank defaulted to sine. A patch
  therefore began from a static map, and I can't say what the intended defaults
  were.
- **Cost.** No CPU measurement, no voice count and no buffer size. The lab ran
  in Chrome and Firefox through Web Audio.
- **Evidence.** No test, no recording and no second listener.
- **Date.** The lab itself has no date in this repository: it was never
  committed, so the status line is when I wrote this up (2026-11-21), not when
  the experiment ran.

## Next experiments

- **Drift:** an Ornstein–Uhlenbeck process pulled back to 1 instead of a free
  walk, or noise with a 1/f tilt. The chaotic map is the one I expect least from
  musically.
- **Flips:** a Markov switch so flips cluster into runs, or a group of segments
  flipped together.
- **Order:** local skips instead of a full permutation, Lévy-distributed jumps
  for rare large leaps, and a fixed non-sequential pattern as a control.
- **The sharing question:** the one the lab was meant to answer and didn't.
- **A band-limited version:** so the extreme settings can be judged without the
  aliasing sitting on top of them.

## Sources

1. Casio CZ phase distortion, the patent and the encyclopaedia entries.
2. J. O. Smith III, the CCRMA DSP book, for waveshaping basics.
3. J. Kleimola, V. Lazzarini, J. Timoney and V. Välimäki, "Vector Phase
   Shaping Synthesis", DAFx-2011.
4. P. Hoffmann, the Xenakis stochastic synthesis pages.
5. Xenakis's DSS, the Tonalux write-up, 2026.
6. R. Radna, "Dynamic Stochastic Wavetable Synthesis", DAFx-2023.
7. R. Radna, Xenos, 2024.
8. Brownian synthesis notes, the Praat script.

Radna's DSWS and Xenakis's DSS are the two this leans on. Part of the original
text came from a draft released under the GNU Free Documentation Licence.

