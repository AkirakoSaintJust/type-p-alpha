# Waveforms

The oscillator carries fifteen selectable entries: fourteen single-cycle tables
and the spectrum mode. The tables cover the OPL3 and OPZ/TX81z/DX11/V50 waveform
sets, plus `rect tri` and `half square`. They are generated from the formulas in
`tools/gen_wavetable.py`, not sampled from the chips. This note covers the list,
the `rect tri` table, the spectrum mode and the harmonic analysis behind the
spectral pages.

I keep this note because the waveform list is the one place where a stored number, a
panel label and a table offset have to agree, and they've disagreed before. The list
below is the current order, and the spectrum entry is the only value that isn't a
table.

## The list

`rect tri` is inserted after `rect sine`, `half square` sits after `pulse`, and
`spectrum` sits at the end. The stored value is the visible position:

| Value | Wave |
| --- | --- |
| 0 | sine |
| 1 | triangle |
| 2 | half sine |
| 3 | half tri |
| 4 | 2x sine |
| 5 | 2x tri |
| 6 | 2x half sine |
| 7 | 2x half tri |
| 8 | rect sine |
| 9 | rect tri |
| 10 | pulse |
| 11 | half square |
| 12 | square |
| 13 | saw |
| 14 | spectrum |

The half-square insertion shifted square, saw and spectrum from 11, 12 and 13 to
12, 13 and 14, and `PRESET_VERSION` moved to 19. The earlier insertions moved it
to 3 for `rect tri`, to 6 for the 64-level analysis, and to 7 for the filter law.

Value 14 is the spectrum mode, not a table. The engine reads the additive bank for
14 and the wave table for 0 to 13.

## Half square

`half square` is the first half of the square wave held positive and the second
half held at zero: `1` for the first half cycle and `0` for the second. It joins
pulse, square and saw as a nine-level band-limited mip set, so its harmonics stop
below Nyquist as the note rises. The shape has a DC term by construction, like the
rectified shapes and the pulse, and the analysis drops it. It sounds like a square
with a stronger fundamental, a weaker upper odd series, and a hollow, reedy edge.

## Rect tri

`rect tri` is the full-wave rectified linear triangle:
`abs(1 - 4|((p + 0.25) mod 1) - 0.5|)`. It starts at zero, peaks at the quarter
point and folds positive. The table is Q15, 1024 samples, peak normalised, with the
same phase convention as `rect sine`. It is a single table at value 9. The shape
carries h2 and the higher even harmonics, so it sounds an octave above the phase
rate with a soft edge. The pulse, half square, square and saw at 10 to 13 keep the
nine-level band-limited mip sets.

## The generator

The old generator is `firmware/tools/gen_wavetable.py` in the umetate history before
the FM-1 scrub. The new one is `tools/gen_wavetable.py` in this repository. It keeps
the quarter-sine table and the exact `sin_wave` formulas for the waves that came
before, adds the rectified triangle at 9 and the half square at 11, shifts the
band-limited waves to make room, and writes `engine/gen/wavetable.h`.

The new table is 47,104 samples: ten single tables (0 to 9) and four nine-level
mip sets (10 to 13), which places the band-limited pulse, half square, square and
saw after the rectified triangle while the first ten tables keep their original
order, so the current sine, triangle, half and rectified shapes render with the
same samples and the new shapes are the only added tables. The base array is 14
entries. The existing waves keep their samples, apart from the base offsets, and a
test compares the old and new arrays before the new header is accepted.

It is a host tool, and I'd like to keep it that way. The moment it needs a device
tree or a firmware header, the waveform list stops being portable.

The generator carries no firmware, M-VAVE or FM-1 device code. It is a host-side
table tool for the plugin.

## The parameter

The waveform parameter runs from 0 to 14. The `wave` display name list grows to
fifteen. The LFO wave parameter keeps its own six-name list and 0 to 11 range, so
the shared `wave` display reads the stored value. The LFO list is separate.

## Harmonic analysis

The spectral pages analyse a wave from 0 to 13 into 64 harmonic magnitudes. The
analysis reads the mip-0 table, drops DC, and scales the largest harmonic to 99.
The result is a template. The `load spectrum template...` button writes it into
the chosen spectrum, and the waveform is unchanged until `spectrum` is selected.
The `spectrum` entry has no analysis. It uses the stored a and b levels.

The analysis table is generated with the wavetable bank, fourteen waves by 64
harmonics, and a test compares it with a DFT of the checked-in table.

The analysis has the limits in `spectral.md`: a 64-harmonic truncation, no phase,
and no DC. A 2x wave analyses to h2 and its multiples, so h1 reads zero and
the bar scale still starts at h1. A saw or square starting point is darker than its
full-band table at low notes.

The analysis is a starting point, not a promise. A wave that analyses to 64
harmonics is already a compromise, and the fact that it sounds close is the reason
the feature works at all.
