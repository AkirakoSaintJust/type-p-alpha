# Waveforms

The oscillator carries thirteen single-cycle waves. This note covers the list
order, the `rect tri` addition, and the harmonic analysis that the spectral pages
use as a starting point.

## The list

The twelve existing waves keep their stored values from 0 to 11. `rect tri` takes
value 12, and the wave select shows it after `rect sine`:

| Visible position | Wave | Stored value |
| --- | --- | --- |
| 1 | sine | 0 |
| 2 | triangle | 1 |
| 3 | half sine | 2 |
| 4 | half tri | 3 |
| 5 | 2x sine | 4 |
| 6 | 2x tri | 5 |
| 7 | 2x half sine | 6 |
| 8 | 2x half tri | 7 |
| 9 | rect sine | 8 |
| 10 | rect tri | 12 |
| 11 | pulse | 9 |
| 12 | square | 10 |
| 13 | saw | 11 |

The display order is a small map in the wave select. The stored value stays 12, so
every existing patch, part and session keeps its waveform. The value is stable.
Inserting at value 9 instead would need a `PRESET_VERSION` bump and a remap of 9 to
10, 10 to 11 and 11 to 12 in the part bank and the APVTS state.

## Rect tri

`rect tri` is the full-wave rectified triangle: `abs(tri(p))`, with `tri(p)` the
triangle from -1 to 1. It carries a DC term, the fundamental and the even
harmonics, so it sits between the triangle and the saw in brightness.

The table is Q15, 1024 samples per mip, peak normalised, with the same phase
convention as `rect sine`. A single table would alias at high notes, so the wave
needs a band-limited mip set like the pulse, square and saw tables.

The script named in `wavetable.h` is not in this repository. `engine/gen/wavetable.h`
says it comes from `tools/gen_wavetable.py`, and that path is absent. Adding the
thirteenth table means restoring the generator or adding a small one that writes
the new mip set and the extended `p1_wave_base` array. The existing twelve tables
and their base offsets stay byte for byte, so the new mip set appends after the
saw's nine tables and the base array grows to thirteen entries, which keeps every
stored waveform value and every rendered sample of the current waves unchanged
while the thirteenth table sits at the end of the bank.

## The parameter

The waveform parameter runs from 0 to 12. The `wave` display name list grows from
twelve to thirteen. The LFO wave parameter keeps its own list of six and its 0 to
11 range, so the shared `wave` display reads the waveform name list by stored
value, not by the visible position.

The plugin's wave select needs the display order map. The web lab and the manual
use the same visible order. The engine switch reads the stored value.

## Harmonic analysis

The spectral page analyses the selected wave into sixteen harmonic magnitudes. The
analysis reads the mip-0 table, drops DC, and scales the largest harmonic to 99.
The rest is a lookup. The result is a preview while the spectral mode is off, and
the first edit commits it.

The analysis table is generated with the wavetable bank, thirteen waves by sixteen
harmonics, and a test compares it with a DFT of the checked-in table. A runtime DFT
in the editor would work, but a small generated table keeps the editor and the
engine on one source and avoids a cost on every page open.

The analysis has the limits in `spectral.md`: a sixteen-harmonic truncation, no
phase, and no DC. A 2x wave analyses to h2 and its multiples, so h1 reads zero and
the bar scale still starts at h1. A saw or square starting point is darker than its
full-band table at low notes.
