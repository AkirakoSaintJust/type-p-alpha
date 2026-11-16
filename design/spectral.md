# Spectral oscillators

The engine carries thirteen waveform tables per operator. This feature adds a
second oscillator mode: 64 harmonic levels per spectrum, two spectra per
operator, and a morph between them. The mode is optional. It is off by default, so a patch
that does not enable it renders through the existing waveform path without a
sample change.

The design uses three tabs, `op spectra a`, `op spectra b` and `op spectra mod`.
The first two edit the levels of one spectrum, and the third shows the spectra as a
display and carries the morph envelope and the morph LFO.

## Tabs and page indices

The tab bar keeps seven tabs, with `part fx/mod` next to `patch settings`:

| Index | Tab |
| --- | --- |
| 0 | patch settings |
| 1 | part fx/mod |
| 2 | operator envs |
| 3 | operator lfos |
| 4 | op spectra a |
| 5 | op spectra b |
| 6 | op spectra mod |

The page order moves with each layout change, so restore remaps the stored page. A
four-tab session had operators at 1, operator lfos at 2 and fx/mod at 3. A six-tab
session had fx/mod at 1 and `op spectra mod` at 5. The split inserts `op spectra b`
at 5, so the mod page moves to 6. The remap keys on `uiPageLayout`, now 3, and the
patch and multi data are unaffected.

The tab bar lays each label out at `textWidth(displayFont(textTab)) + 32`. The seven
labels take about 1,183 px of the 1,238 px strip at the 16 px JUCE tab font, so
about 55 px remain. The bar does not scroll or clip.

`showTab` clamps to 0 to 6 and gains cases 4, 5 and 6. `setUiPage` and the state
restore clamp to 6. `buildPage` gains the two spectrum factories and the mod factory
at 6.

The editor window and page area do not move. The page is 1206 by 685 at (17, 255),
the same as the operators and operator lfos pages.

## Page geometry

`op spectra a` and `op spectra b`, per page:

| Part | Width | Height |
| --- | --- | --- |
| operator strip | 1,206 | 176 |
| four row and control bands, 4 px gaps | 1,206 | 508 |

The page is 685 px high, so the 508 px of bands fit under the strip without a
scroll. Each band is 124 px high. The row takes 898 px and its control card 300 px,
with an 8 px gap. The row holds a range select, copy and paste, the 64-bar editor,
the harmonic labels and the group legend. The control card, titled `opN spectral
controls`, holds the group, the mask, the three spring controls, the formant
picker, normalise and to the other spectrum. At 898 px each bar is about 14 px.

The operator strip is the operator column from the operator envs page, reduced to a
30 px scope and spectro pair and the oscillator card. The operator name sits in a
24 px header, so the scope starts below it and nothing clips.

`op spectra mod`, per column:

| Part | Height |
| --- | --- |
| operator header | 24 |
| oscillator card | 140 |
| spectrum display | 120 |
| morph envelope card | 220 |
| morph lfo card | 140 |
| four 4 px gaps | 16 |
| total | 660 |

The same 9 px remain. The visual is 120 px high, enough for two overlaid
64-harmonic outlines and the live morph marker.

The spectrum pages keep the scope and spectro pair, because they are operator
editing page and the pair is the operator's audio identity. The mod page drops
the pair, because the two-spectrum visual already shows the operator's shape and
the page needs the height for the envelope and the LFO.

## Oscillator card

The mode is per operator. The wave select carries the mode, so there is no separate
switch. It lists fourteen entries: the thirteen waves and `spectrum` at the end.
The two spectral pages add a `morph` knob to the oscillator row. The operators
and operator lfos pages keep the current card and show the same wave select, so
`spectrum` is visible and selectable there too.

| Control | Meaning |
| --- | --- |
| ratio | oscillator ratio, as now |
| detune | oscillator detune in cents, as now |
| level | operator output level, as now |
| feedback | operator feedback, as now |
| wave | select a wave 0 to 12, or `spectrum` at 13 for the additive oscillator |
| morph | blend from spectrum a at 0 to spectrum b at 99 |

Selecting `spectrum` uses the stored a and b spectra. Selecting a wave returns the
operator to that wave and keeps the stored spectra for the next `spectrum`
selection.

The morph value is the base for the two modulation sources. The engine computes

```
m = morph / 99 + (envDepth / 99) * (env / 99) + (lfoDepth / 99) * lfo
m = clamp(m, 0, 1)
sample = (1 - m) * tableA + m * tableB
```

The envelope and the LFO run per voice. The tables are shared per part, because
the levels are part parameters and do not vary per voice.

## Starting from the waveform

The spectral page opens on the operator's current wave. While the waveform is not
`spectrum`, the editor analyses the selected waveform's 1024-sample mip-0 table into
64 harmonic magnitudes, drops the DC term, scales the largest harmonic to 99,
and shows the result as spectrum a. The analysis is a forward transform. The engine
still rebuilds the time table from the levels.

The analysis is a preview. The operator sounds through the selected waveform, the
stored a and b levels are untouched, and the a row carries a `from wave` badge.
Opening the page does not dirty the patch.

The first edit of an A-row control commits the preview. The controls are the 64 a
bars, the group and mask selectors, the three spring controls, `norm` and `to b`.
The morph knob does not commit by itself. The editor writes the 64 a levels, leaves
b as stored, then writes `opN_waveform` as 13, in that order, so the engine never
reads the new mode against the old table. The mode switch crossfades over 10 ms. A
first edit on the b row or the morph does not commit the a preview. Select
`spectrum` or edit the a row.

The first edit writes 65 parameters, and a host undo may not group them.
Selecting a wave is the explicit way back. Host automation of a level does not
engage the mode either, because the waveform value is the gate.

Once the waveform is `spectrum`, the pages show the stored spectra. Selecting a wave
keeps the stored levels and returns the a row to the live waveform analysis.
Selecting `spectrum` again restores the stored spectra without re-analysing.

The analysis is a starting point. A sine, triangle or half sine survives the
64-harmonic limit closely. A saw, square, pulse or rectified shape loses the
partials above 64, so the spectral version is duller above 4.2 kHz at C2 and stays
close to the full-band table towards the top of the keyboard, where those partials
sit above Nyquist. The transform also drops phase, so the waveform shape, the peak
factor and the oscilloscope trace change even when the harmonic magnitudes match.
The half-wave and rectified shapes carry a DC term, which the table drops.

## Spectrum rows

Each operator has one row per spectrum, `opN spectrum a` and `opN spectrum b`, with
the same controls. The 64 levels and the range are the saved spectrum state. While
the waveform is not `spectrum`, the a row shows the live waveform analysis. Once the
waveform is `spectrum`, both rows show the stored levels.

| Control | Meaning |
| --- | --- |
| `range` | the harmonic block the 64 bars address: 1-64 or 65-128 |
| 64 bars | the selected range, 0 to 99 per bar |
| `copy` / `paste` | move the whole spectrum to another operator |
| `to b` / `to a` | copy the 64 levels to the other spectrum of the operator |
| group | the harmonic series the spring controls select, from all, odd, even, octave P8, third P5, fifth M3, seventh m7, fifteenth M7, eleventh TT, thirteenth m6, ninth M2, twenty-first P4, nineteenth m3 and seventeenth m2 |
| mask | live, dead or both, so a gesture writes only the sounding, only the silent, or every selected member |
| tilt | drag left to darken and right to brighten the selected members |
| level | drag to add or remove level across the selected members |
| formant | place a peak at the picked harmonic and drag its amount |
| harmonic picker | the formant centre, h1 to h64 |
| `norm` | scale the 64 bars so the tallest reads 99 |

The bar editor writes the level under the pointer and follows a drag across the
bars. A bar snaps to whole units from 0 to 99. The engine normalises the generated
table, so the bars set the relative amplitudes and the operator level sets the
loudness. The range shifts the whole block, so a row set to 65-128 edits harmonics
65 to 128. A and b carry independent ranges, and the mod display zooms to the union
when they differ.

The group and the mask limit which bars a spring control moves. With `third P5`
selected and `both` masked, a level drag writes harmonics 3, 6, 12, 24 and 48 and
leaves h1, h2 and h4 where they were. A `dead` mask writes only the members that
are silent, which is the quick way to build a series from nothing.

Each row carries `copy` and `paste` in its header, like every other operator card,
so a whole spectrum moves between operators. The `to b` and `to a` buttons copy
within one operator.

## The spring controls

The controls are editing gestures. The 64 levels are the host parameters. Each drag
starts from a snapshot of the 64 levels at pointer down, applies one curve to the
selected members of that snapshot, and writes the result. The control returns to
rest on release. A second drag starts from the committed levels, so repeated
gestures never compound the same curve.

`a` is the drag amount from 0 to 1. `L[n]` is the level of harmonic n in the
snapshot, from 1 to 64, and `i` is its bar index from 0 to 63. `c` is the formant
centre and the bell width is 1.5 harmonics.

| Control | Curve |
| --- | --- |
| tilt | `L'[n] = L[n] * (1 + a * i / 63)` |
| level | `L'[n] = L[n] + a * 99` |
| formant | `L'[n] = L[n] + a * 99 * exp(-((n - c)^2) / 4.5)` |

Every result clamps to 0 to 99, and only the selected members change. `norm` scales
every level by `99 / max(L)` and leaves a silent spectrum alone.

The curves are a factory setting. The listening test compares the tilt amount and
the formant width, and the interaction stays the same if a curve changes.

## Morph envelope card

The card holds a graph, a depth knob, four rates and three levels. It uses the
operator envelope shape: the envelope starts at zero, rises to L1 over R1, falls
to L2 over R2, falls to L3 over R3, and returns to zero over R4.

| Control | Meaning |
| --- | --- |
| depth | how far the envelope moves the morph, -99 to +99 |
| R1 to R4 | envelope rates, 0 to 99 |
| L1 to L3 | envelope levels, 0 to 99 |

A depth of zero leaves the base morph alone, so the envelope does not need an on
switch of its own. Zero depth is the bypass. The graph is the same envelope view
as the amp env card.

## Morph LFO card

The card matches the operator LFO cards.

| Control | Meaning |
| --- | --- |
| rate | 0 to 20 Hz |
| depth | how far the LFO moves the morph, -99 to +99 |
| wave | sine, triangle, saw, inverse saw, square, random |
| bipolar | bipolar or unipolar phase |

The LFO runs per voice with its own phase, like the amp, feedback and detune LFOs.
A depth of zero is the bypass.

## Spectrum display

The mod page shows spectrum a as a filled purple outline and spectrum b as a pink
outline, with the union of the two spans on one horizontal scale and 64 levels per
outline. A marker under the outlines
runs from `a` at the left to `b` at the right and shows the current morph value.
The visual is a display and does not take a drag.

The marker follows the base morph and the two modulation sources, so the move is
visible while a note sounds. The outlines do not move. The engine writes the last
morph the oscillator read into the bank header, and the editor polls it on each
tick. The faint waveform is redrawn at that value, so the trace and the cursor
move together. A silent operator reports no read, and the display falls back to
the base morph.

## Defaults

The waveform defaults to sine, so the init patch stays exact. Every patch that
uses waves 0 to 8 stays exact too. The `rect tri` insertion shifts old values 9, 10
and 11 to 10, 11 and 12, and the preset version moved to 3.

| Control | Default |
| --- | --- |
| waveform | sine (0) |
| spectrum a | h1 99, h2 to h64 zero |
| spectrum b | 99, 45, 30, 15, 55, 40, 25, 12, 8, 5, 3, 2, 1, then zero to h64 |
| morph | 0 |
| envelope rates | 80, 60, 50, 80 |
| envelope levels | 99, 60, 0 |
| envelope depth | 0 |
| lfo rate | 4.0 Hz |
| lfo depth | 0 |
| lfo wave | sine |
| lfo mode | bipolar |

The stored spectrum a is a sine. The page shows the selected wave while the
waveform is not `spectrum`, and the first edit commits that analysis. Spectrum b is
a formant shape at h5, which gives the morph an audible direction and shows the
formant spring in context.

## Parameters

Every new parameter is per operator and per part. The ids below use `opN` for
operators 1 to 4 and `.pP` for parts 0 and 1. All are block rate. The existing
waveform parameter grows from 0 to 11 to 0 to 13, where 13 selects `spectrum`.

| Group | Id | Count | Range | Default |
| --- | --- | --- | --- | --- |
| range a | `opN_spec_range_a.pP` | 1 | 0 to 1 | 0 |
| range b | `opN_spec_range_b.pP` | 1 | 0 to 1 | 0 |
| spectrum a | `opN_spec_a1.pP` to `opN_spec_a64.pP` | 64 | 0 to 99 | 99, then zero |
| spectrum b | `opN_spec_b1.pP` to `opN_spec_b64.pP` | 64 | 0 to 99 | as the defaults table |
| morph | `opN_spec_morph.pP` | 1 | 0 to 99 | 0 |
| envelope rates | `opN_spec_env_r1.pP` to `opN_spec_env_r4.pP` | 4 | 0 to 99 | 80, 60, 50, 80 |
| envelope levels | `opN_spec_env_l1.pP` to `opN_spec_env_l3.pP` | 3 | 0 to 99 | 99, 60, 0 |
| envelope depth | `opN_spec_env_depth.pP` | 1 | -99 to +99 | 0 |
| lfo rate | `opN_spec_lfo_rate.pP` | 1 | 0 to 20 Hz | 4.0 Hz |
| lfo depth | `opN_spec_lfo_depth.pP` | 1 | -99 to +99 | 0 |
| lfo wave | `opN_spec_lfo_wave.pP` | 1 | six waves | sine |
| lfo mode | `opN_spec_lfo_mode.pP` | 1 | unipolar, bipolar | bipolar |

That is 143 parameters per operator, 572 per part, and 1,144 in the manifest. The
part block grows from 228 to 800, and the manifest from 470 to 1,614.

The range is the harmonic block the 64 bars address: 0 is 1 to 64 and 1 is
65 to 128. A and b each carry their own range, so the mod display zooms
out to the union of both spans when they differ.

## Engine contract

The engine generates a table bank per operator: two spectra for each of the two
parts, seven mip levels, 256 samples, signed 16-bit samples, and one shadow spectrum
per part for the crossfade. The bank is 10,880 samples per operator, so 21,760
bytes per operator and 87,040 bytes for four operators, both parts included.

The generator sums the 64 harmonic amplitudes into one table, finds the peak,
and scales the table to full range. The first survey run lacked that normalisation,
so the table clipped at -23 dB and the alias floor read as far better than the table
actually was. The normalisation is mandatory.

The bank is 256 samples per mip. The read uses a 24-bit phase fraction, so the
table's harmonic 1 is the note's fundamental. The first prototype used the
1024-sample shift (22 bits) and played the table four times too high. The product
uses 24 bits and the spectral test checks the fundamental.

When a level changes, the engine sees the new hash on the 32-sample control tick,
regenerates the affected spectrum, writes the new table into the shadow while the
current table keeps playing, and crossfades the read from the old table to the new
one over ten milliseconds. The update stays silent and the voice keeps running. The
morph blends the two existing tables per sample.

The mip selector keeps the top harmonic at or below Nyquist/2.25, so at note 60 it
selects mip 2, whose table keeps all 64 harmonics, and a 64-level spectrum reads
the full set at mips 0 to 2 before dropping to 32, 16, 8, 4, 2 and 1 at mips 3 to
8. The read maps `level = clamp(mip - 2, 0, 6)`. Nyquist/2.25 recovers the top octave with less
alias headroom than the bright policy, and the listening test can move it back.

The measured costs on this machine, with the caveat that the CPU clock moved the
runs by up to 25 percent:

| Work | Cost |
| --- | --- |
| one spectrum, 64 harmonics, 256 samples, seven mips | 60 µs |
| eight spectra in one control tick | 482 µs |
| one engine block that changes all eight spectra | about 850 µs |
| one spectrum, 64 harmonics, 512 samples, seven mips | 120 µs |

A part regenerates a spectrum only when its levels change, so the steady cost is
the read path, which reads two tables and blends them once per sample, the same
work the 16-harmonic bank did, and the four operators share their banks across all
24 voices because the levels are part parameters. The worst-case block takes about a third of a 128 frame callback
at 48 kHz, which leaves two thirds of the budget. A 32 frame callback would not hold
that block. The 512-sample table doubles the generation and stays out of the
default bank. The control tick stays at 32 samples, which is 1,500 Hz at 48 kHz.

The full graph read path measured 2.46% of one core at 20 voices, which scales to
about 2.95% at 24.

When `opN_waveform` is 13, the oscillator reads the additive bank. Values 0 to 12
read the wave table, and the additive bank is not generated.

## Existing patches

The waveform defaults to sine, and the spectral levels default to a sine
a-spectrum. Waves 0 to 8 render the same samples. The `rect tri` insertion shifts
old values 9, 10 and 11 to 10, 11 and 12, so the preset version moved to 3. The
64-level change moves it to 6, and the old test banks are regenerated. The engine
parity test covers the wave path, and a new table test covers the spectrum mode.

## Open items

- Confirm the tilt amount, the level amount and the formant width by ear.
- Choose the mip brightness policy after the C7 listening test.
- Decide whether harmonic 128 in the 65-128 range earns a 512-sample table. The
  512-sample bank doubles the generation and needs the work spread across ticks.
