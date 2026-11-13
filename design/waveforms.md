# Waveforms

The oscillator carries fourteen selectable entries: thirteen single-cycle tables
and the spectrum mode. This note covers the list, the `rect tri` table, the
spectrum mode and the harmonic analysis behind the spectral pages.

## The list

`rect tri` is inserted after `rect sine`, and `spectrum` sits at the end. The
stored value is the visible position:

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
| 11 | square |
| 12 | saw |
| 13 | spectrum |

The insertion shifts pulse, square and saw from 9, 10 and 11 to 10, 11 and 12.
There is no release yet, so `PRESET_VERSION` moves to 3, the old test banks are
regenerated, and no migration path is added. The new values are the only values.

Value 13 is the spectrum mode, not a table. The engine reads the additive bank for
13 and the wave table for 0 to 12.

## Rect tri

`rect tri` is the full-wave rectified linear triangle:
`abs(1 - 4|((p + 0.25) mod 1) - 0.5|)`. It starts at zero, peaks at the quarter
point and folds positive. The table is Q15, 1024 samples, peak normalised, with the
same phase convention as `rect sine`. It is a single table at value 9. The shape
carries h2 and the higher even harmonics, so it sounds an octave above the phase
rate with a soft edge. The pulse, square and saw at 10 to 12 keep the nine-level
band-limited mip sets.

## The generator

The old generator is `firmware/tools/gen_wavetable.py` in the umetate history before
the FM-1 scrub. The new one is `tools/gen_wavetable.py` in this repository. It keeps
the quarter-sine table and the exact `sin_wave` formulas for the twelve current
waves, adds the rectified triangle at 9, shifts the three band-limited waves, and
writes `engine/gen/wavetable.h`.

The new table is 37,888 samples: ten single tables (0 to 9) and three nine-level
mip sets (10, 11 and 12), which places the band-limited pulse, square and saw after
the rectified triangle while the first ten tables keep their original order, so the
current sine, triangle, half and rectified shapes render with the same samples and
the new triangle is the only added table. The base array is 13 entries. The current
twelve tables keep their samples, apart from the base offsets, and a test compares
the old and new arrays before the new header is accepted.

It is a host tool.

The generator carries no firmware, M-VAVE or FM-1 device code. It is a host-side
table tool for the plugin.

## The parameter

The waveform parameter runs from 0 to 13. The `wave` display name list grows to
fourteen. The LFO wave parameter keeps its own six-name list and 0 to 11 range, so
the shared `wave` display reads the stored value. The LFO list is separate.

## Harmonic analysis

The spectral pages analyse a wave from 0 to 12 into sixteen harmonic magnitudes. The
analysis reads the mip-0 table, drops DC, and scales the largest harmonic to 99.
The result is a preview while the waveform is not `spectrum`, and the first edit
commits it. The `spectrum` entry has no analysis. It uses the stored a and b
levels.

The analysis table is generated with the wavetable bank, thirteen waves by sixteen
harmonics, and a test compares it with a DFT of the checked-in table.

The analysis has the limits in `spectral.md`: a sixteen-harmonic truncation, no
phase, and no DC. A 2x wave analyses to h2 and its multiples, so h1 reads zero and
the bar scale still starts at h1. A saw or square starting point is darker than its
full-band table at low notes.
