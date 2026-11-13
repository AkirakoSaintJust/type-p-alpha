# Spectral oscillators

The engine carries twelve waveforms per operator. This feature adds a second
oscillator mode: sixteen harmonic levels per spectrum, two spectra per operator,
and a morph between them. The mode is optional. It is off by default, so a patch
that does not enable it renders through the existing waveform path without a
sample change.

The design uses two new tabs, `operator spectra` and `op spectra mod`. The first
edits the levels and the macros. The second shows the spectra as a display and
carries the morph envelope and the morph LFO.

## Tabs and page indices

The tab bar keeps six tabs, with `part fx/mod` next to `patch settings`:

| Index | Tab |
| --- | --- |
| 0 | patch settings |
| 1 | part fx/mod |
| 2 | operators |
| 3 | operator lfos |
| 4 | operator spectra |
| 5 | op spectra mod |

The move shifts the stored page index. A session saved with the old order needs a
remap of 1 to 2, 2 to 3 and 3 to 1 on restore, or it opens the wrong page once.
The patch and multi data are unaffected. The restore can key the remap on a new
`uiPageLayout` property.

The current tab bar lays each label out at `textWidth(displayFont(textTab)) + 32`.
The six labels take about 1,030 px of the 1,238 px strip at the 16 px JUCE tab
font, so 208 px remain. The bar does not scroll or clip.

Four clamps change from 3 to 5:

- `PluginEditor.cpp`, the constructor clamp.
- `PluginEditor.cpp`, `showTab`.
- `PluginProcessor.h`, `setUiPage`.
- `PluginProcessor.cpp`, the state restore.

`showTab` renumbers `part fx/mod`, `operators` and `operator lfos` to 1, 2 and 3,
and gains cases 4 and 5. `buildPage` gains the two factories.

The editor window and page area do not move. The page is 1206 by 685 at (17, 255),
and each operator column is 294 px wide with 278 px of inner width, the same as
the operators and operator lfos pages. Four columns and the two new pages use the
existing column rhythm.

## Page geometry

`operator spectra`, per column:

| Part | Height |
| --- | --- |
| operator header | 24 |
| scope and spectro pair | 40 |
| oscillator card | 140 |
| spectrum a card | 220 |
| spectrum b card | 220 |
| four 4 px gaps | 16 |
| total | 660 |

The page holds 685 px. The column adds 8 px of padding at each end, so nine pixels
remain. The spectrum card holds a 16-bar editor, a harmonic scale, two macro rows
and the card header.

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
sixteen-harmonic outlines and the live morph marker.

The spectra page keeps the scope and spectro pair, because it is an operator
editing page and the pair is the operator's audio identity. The mod page drops
the pair, because the two-spectrum visual already shows the operator's shape and
the page needs the height for the envelope and the LFO.

## Oscillator card

The mode is per operator. The two spectral pages use the oscillator card with two
additions: a `spec` switch in the wave row and a `morph` slider under the wave
select. The operators and operator lfos pages keep the current card, so their
layout is unchanged.

| Control | Meaning |
| --- | --- |
| ratio | oscillator ratio, as now |
| detune | oscillator detune in cents, as now |
| level | operator output level, as now |
| feedback | operator feedback, as now |
| wave | waveform used while the spectral mode is off, and the spectral pages analyse it |
| spec | enables the spectral oscillator. The first spectral edit or the first morph move also enables it |
| morph | blend from spectrum a at 0 to spectrum b at 99 |

When `spec` is on, the wave select gives way to the word `spectral`. The classic
pages show the same state without a new control, so a spectral operator never
claims to be a sine on the operators page. The scope and spectro always show the
audio, so the mode is visible there too.

The morph value is the base for the two modulation sources. The engine computes

```
m = morph / 99 + (envDepth / 99) * (env / 99) + (lfoDepth / 99) * lfo
m = clamp(m, 0, 1)
sample = (1 - m) * tableA + m * tableB
```

The envelope and the LFO run per voice. The tables are shared per part, because
the levels are part parameters and do not vary per voice.

## Starting from the waveform

The spectral page opens on the operator's current wave. The editor analyses the
selected waveform's 1024-sample mip-0 table into sixteen harmonic magnitudes, drops
the DC term, scales the largest harmonic to 99, and shows the result as spectrum a.
The analysis is a forward transform. The engine still rebuilds the time table from
the levels.

The analysis is a preview. While `spec` is off, the operator sounds through the
selected waveform, the stored a and b levels are untouched, and the a card carries
a `from wave` badge. Opening the page does not dirty the patch.

The first edit of a spectral control commits the preview. The controls are the
sixteen a bars, the sixteen b bars, the four macros, `norm`, `to a` and `to b`, the
morph value, and an envelope or LFO depth moved above zero. The editor writes the
sixteen a levels, leaves b as stored, then writes `opN_spec_on`, in that order, so
the engine never reads the new mode against the old table. The mode switch
crossfades over 10 ms. A first edit on the b card or the morph also commits the a
preview, because the engine needs the analysed a as soon as the mode is on.

The first edit writes seventeen parameters, and a host undo may not group them.
The switch is the explicit way back. Host automation of a level does not engage the
mode either, because `opN_spec_on` is the gate.

Once `spec` is on, the pages show the stored spectra. Switching `spec` off keeps
the stored levels and returns the a card to the live waveform analysis. Switching
it on restores the stored spectra without re-analysing.

The analysis is a starting point. A sine, triangle or half sine survives the
sixteen-harmonic limit closely. A saw, square, pulse or rectified shape loses its
upper harmonics, so the spectral version is audibly duller at low notes than the
full-band table. The transform also drops phase, so the waveform shape, the peak
factor and the oscilloscope trace change even when the harmonic magnitudes match.
The half-wave and rectified shapes carry a DC term, which the table drops. The
thirty-two harmonic mode later closes part of the gap.

## Spectrum cards

Each operator has two cards, `spectrum a` and `spectrum b`, with the same
controls. The sixteen stored levels per spectrum are the only saved spectrum state.
While `spec` is off, the a card shows the live waveform analysis. Once the mode is
on, both cards show the stored levels.

| Control | Meaning |
| --- | --- |
| 16 bars | harmonic 1 to 16, 0 to 99 per bar |
| `norm` | scale the sixteen bars so the tallest reads 99 |
| `to b` / `to a` | copy this card's sixteen levels to the other spectrum |
| `odd` | drag to attenuate the even harmonics |
| `even` | drag to attenuate the odd harmonics above h1 |
| `tilt` | drag left to darken and right to brighten |
| `formant` | place a peak at a selectable harmonic and drag its amount |
| harmonic select | the formant centre, h1 to h16 |

The bar editor writes the level under the pointer and follows a drag across the
bars. A bar snaps to whole units from 0 to 99. The engine normalises the generated
table, so the bars set the relative amplitudes whilst the operator level sets the
loudness.

Each card carries `copy` and `paste` in its header, like every other operator
card, so a whole spectrum moves between operators. The `to b` and `to a` buttons
copy within one operator.

## The macros

The macros are editing gestures. The sixteen levels are the host parameters. Each
drag starts from a snapshot of the sixteen levels at pointer down, applies one
curve to that snapshot, and writes the result. The control returns to rest on
release. A second drag starts from the committed levels, so repeated gestures
never compound the same curve.

`a` is the drag amount from 0 to 1 for `odd`, `even` and `formant`, and from -1 to
1 for `tilt`. `L[n]` is the level of harmonic n in the snapshot, with n from 1 to
16. `c` is the formant centre and the bell width is 1.5 harmonics.

| Macro | Curve |
| --- | --- |
| odd | `L'[n] = L[n] * (1 - a)` for even n, unchanged for odd n |
| even | `L'[n] = L[n] * (1 - a)` for odd n above h1, unchanged for h1 and even n |
| tilt | `L'[n] = L[n] * clamp(1 + a * (n - 1) / 15, 0, 1)` |
| formant | `L'[n] = clamp(L[n] + a * 99 * exp(-((n - c)^2) / 4.5), 0, 99)` |

`norm` scales every level by `99 / max(L)`, and leaves a silent spectrum alone.

The curves are a first factory setting. The listening test compares them with a
softer `odd` and `even`, and with a narrower formant bell. The interaction stays
the same if a curve changes. The curves are provisional.

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
outline, with h1 to h16 on the same horizontal scale. A marker under the outlines
runs from `a` at the left to `b` at the right and shows the current morph value.
The visual is a display and does not take a drag.

The marker follows the base morph and the two modulation sources, so the move is
visible while a note sounds. The outlines do not move. The engine publishes the
current morph value for each operator and part on the existing tap path, so the
marker reflects the envelope and the LFO.

## Defaults

The spectral mode is off, so the init patch and every existing patch stay exact.

| Control | Default |
| --- | --- |
| spec | off |
| spectrum a | h1 99, h2 to h16 zero |
| spectrum b | 99, 45, 30, 15, 55, 40, 25, 12, 8, 5, 3, 2, 1, 0, 0, 0 |
| morph | 0 |
| envelope rates | 80, 60, 50, 80 |
| envelope levels | 99, 60, 0 |
| envelope depth | 0 |
| lfo rate | 4.0 Hz |
| lfo depth | 0 |
| lfo wave | sine |
| lfo mode | bipolar |

The stored spectrum a is a sine. The page shows the selected wave while the mode is
off, and the first edit commits that analysis. Spectrum b is a formant shape at h5,
which gives the morph an audible direction and shows the formant macro in context.

## Parameters

Every new parameter is per operator and per part. The ids below use `opN` for
operators 1 to 4 and `.pP` for parts 0 and 1. All are block rate.

| Group | Id | Count | Range | Default |
| --- | --- | --- | --- | --- |
| spectral on | `opN_spec_on.pP` | 1 | off, on | off |
| spectrum a | `opN_spec_a1.pP` to `opN_spec_a16.pP` | 16 | 0 to 99 | 99, then zero |
| spectrum b | `opN_spec_b1.pP` to `opN_spec_b16.pP` | 16 | 0 to 99 | as the defaults table |
| morph | `opN_spec_morph.pP` | 1 | 0 to 99 | 0 |
| envelope rates | `opN_spec_env_r1.pP` to `opN_spec_env_r4.pP` | 4 | 0 to 99 | 80, 60, 50, 80 |
| envelope levels | `opN_spec_env_l1.pP` to `opN_spec_env_l3.pP` | 3 | 0 to 99 | 99, 60, 0 |
| envelope depth | `opN_spec_env_depth.pP` | 1 | -99 to +99 | 0 |
| lfo rate | `opN_spec_lfo_rate.pP` | 1 | 0 to 20 Hz | 4.0 Hz |
| lfo depth | `opN_spec_lfo_depth.pP` | 1 | -99 to +99 | 0 |
| lfo wave | `opN_spec_lfo_wave.pP` | 1 | six waves | sine |
| lfo mode | `opN_spec_lfo_mode.pP` | 1 | unipolar, bipolar | bipolar |

That is 46 parameters per operator, 184 per part, and 368 in the manifest. The
part block grows from 228 to 412, and the manifest from 470 to 838.

A thirty-two harmonic mode would add 32 levels per operator. It is not in the
first release, and the sixteen-bar editor, the table generator and the parameter
names all assume sixteen until that mode exists.

## Engine contract

The engine generates a table bank per part: four operators, two spectra, five mip
levels, 256 samples, signed 16-bit samples. The bank is 20,480 bytes per part. A
shadow set per operator adds 20,480 bytes per part for the crossfade, so the two
parts use 81,920 bytes.

The generator sums the sixteen harmonic amplitudes into one table, finds the peak,
and scales the table to full range, which is the normalisation step that the first
survey run lacked, and without it the table clipped at -23 dB and the alias floor
read as far better than the table actually was. The normalisation is mandatory.

When a level changes, the engine marks the operator dirty, regenerates the affected
spectrum on the 344 Hz control tick, writes the new table into the shadow while the
current table keeps playing, and crossfades the read from the old table to the new
one over ten milliseconds, which keeps the update silent without stopping the
voice. The read never stops. The morph blends the two existing tables per sample.

The mip selector is the conservative one measured in the additive survey. At 16
harmonics and 256 samples, the alias floor at C7 is -87.3 dB with the linear read.
The selector costs the fifth to eighth harmonics at C7, which measure -91.6 dB
against +4.2 dB with the bright policy. The first release keeps the conservative
selector and records the number. A listening test then chooses between it and a
one-level-brighter policy.

The measured costs on this machine, with the caveat that the CPU clock moved the
runs by up to 25 percent:

| Work | Share of one core |
| --- | --- |
| 16 tables, 344 Hz, no mips | 1.7% |
| 16 tables, 344 Hz, five mips | 3.4% |
| full graph, 20 voices, linear read | 2.46% |

A patch that enables all four operators on both parts and regenerates every table
therefore sits near 6% of one core before the rest of the graph. The 1,378 Hz
update path is not used. The control rate stays at 344 Hz.

When `opN_spec_on` is off, the oscillator reads the existing table for the chosen
waveform. The additive bank is not generated and the read path does not run.

## Existing patches

The parameters default to off and to a sine a-spectrum. The current twelve
waveforms, the current init patch and the existing part and multi-patch files
render through the waveform path with the same samples. The engine parity test
covers the off state, and a new table test covers the on state.

## Open items

- Confirm the preview and commit rule, and the control list that engages the mode.
- Generate the thirteen by sixteen harmonic analysis table from the wavetable bank
  and test it against a DFT of the checked-in table.
- Confirm the four macro curves and the formant width by ear.
- Choose the mip brightness policy after the C7 listening test.
- Decide whether the thirty-two harmonic mode earns its parameter count after
  the sixteen-harmonic release is measured.
- Regenerate the thirteen editor goldens after the tab bar changes. The tab bar
  appears in every snapshot, so all thirteen images change, and the two new pages
  add two more, for fifteen in all.
