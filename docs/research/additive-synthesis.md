# Additive synthesis for the operator spectrum

## The question

The README asks whether each operator can carry a 16 or 32 harmonic waveform and
let the player shape those harmonics without a CPU cost that eats the polyphony.
This note maps the DSP methods, the control surfaces and the costs, and it proposes
the smallest engine change that keeps the 20-voice budget.

The patch is the only reference: `synth/patch.json`, the generated `engine/gen`,
and the current table in `engine/gen/wavetable.h`.

## The current engine

The core runs at 44.1 kHz with 20 voices, two parts and four operators per voice.
That is 80 operator voices at full polyphony. A control block is 32 samples, so the
control rate is 1,378.125 Hz.

Each operator has a phasor and an `osc` node that reads the 12-wave `p1_wave`
table, which holds 1024 samples per wave as Q15 values and is read at the nearest
sample from the top ten bits of the phase, whilst waves 9 to 11 carry nine mip
levels whose harmonic counts fall from 256 to 1 and whose level is chosen by the
phasor's `mip` output, so the top octaves read a table with fewer harmonics and the
oscillator stays band-limited. The table is 36,864 `int16` values, 73,728 bytes.

At full polyphony the oscillator performs 80 table reads per sample, 3.528 million
reads per second. The table is shared by every voice that uses the same waveform,
which is the cost model that makes a larger spectrum affordable.

## Direct methods

The first group generates the harmonics for every voice.

| Method | Per-sample work | Strength | Limit |
| --- | --- | --- | --- |
| Summed sine table reads | H reads and H multiply-adds per operator voice | Arbitrary phase and amplitude per partial | 56.4 million partial evaluations per second at H = 16, 112.9 million at H = 32 |
| Recursive resonators | One rotation or two state updates per partial | No sine table read | Coefficient drift, still per voice and per partial |
| Chebyshev waveshaping | A degree H polynomial per sample | Odd and even families from one coefficient set | Arbitrary spectra are awkward, and alias control needs oversampling or a table |
| BLIT and BLEP | One impulse or step correction per sample | Cheap bright source | The spectrum follows the correction filter, not a free 16 or 32 level vector |

The summed method is the reference. At H = 16 it needs 56.4 million sine
evaluations per second at 20 voices, and each evaluation costs a table read, a
phase index and a multiply-add. The arithmetic lands between 170 and 340 million
simple operations per second, before envelopes and modulation. H = 32 doubles it.
That is more than the rest of the engine at full polyphony, and it moves with every
extra voice.

The recursive and Chebyshev paths lower the constant but keep the per-voice and
per-partial scaling. Chebyshev is useful for a harmonic family with a fixed phase
relation, such as an odd-only or even-only series. It is not a general spectrum
editor.

## Table methods

A table method generates the sum once and lets the voices read the result. The
oscillator keeps its current cost, one read per operator voice per sample, and the
spectrum work moves to the table generator.

### Shared part tables

One table per operator per part, generated from the harmonic amplitudes and read by
every voice of that part. The generator runs for eight tables at full polyphony
(four operators times two parts). The per-sample read count stays at 3.528 million
per second.

| Spectrum | Table | Generation at 1,378 Hz | Generation at 344 Hz |
| --- | --- | --- | --- |
| 16 harmonics | 256 samples | 45.2 million multiply-adds per second | 11.3 million |
| 32 harmonics | 512 samples | 180.6 million | 45.2 million |
| 32 harmonics | 256 samples | 90.3 million | 22.6 million |

The generation figure is eight tables times H times N times the update rate. A
mip map roughly doubles the generation and the memory.

The table itself is small. A 256-sample `int16` cycle is 512 bytes; a 512-sample
cycle is 1024 bytes. Five mip levels for 16 harmonics at 256 samples is 2,560 bytes
per table, so eight tables and a second crossfade buffer are about 41 KB. The
32-harmonic case at 512 samples with six levels is about 98 KB. The current
`p1_wave` table is 73 KB, so the memory is in the same class.

The part table is shared by every voice, so the spectrum is a part setting. The
operator's amplitude and feedback envelopes remain per voice, but they only scale
the result. A per-voice harmonic shape needs the morph described below.

### Per-voice tables

One table per operator voice removes the sharing and lets every note carry its own
spectrum. The memory grows by the voice count, and the generation grows by the same
factor. At 16 harmonics and 256 samples, 80 tables at 1,378 Hz would need 451
million multiply-adds per second, ten times the part table. At 32 harmonics and 512
samples the generation is 1.8 billion. That is not a default path for 20 voices.

A per-voice table can still be useful as a quality mode with a lower voice count,
or for a fixed note range where the table is generated once per note instead of at
the control rate. The first case costs polyphony; the second costs memory and note
setup time.

### Shared tables with a per-voice morph

The middle path keeps the tables at part level and adds a per-voice position
between two of them. The bank holds, for example, a dark and a bright table per
operator. The oscillator reads both tables and blends them with the morph value.
The generation cost stays at the part level and only the read cost doubles, from
3.528 to 7.056 million reads per second.

The morph value can follow the amplitude envelope, a dedicated spectral envelope,
an LFO or key position. This keeps the per-voice movement that the existing
envelopes provide without generating a table per voice. A bank of four or eight
recipes and a two-table crossfade at the current position extends the same idea;
the cost is one or two reads per voice, and the recipes are generated when they are
edited, not while they are static.

### Regeneration and crossfade

Writing a new table while the voices read it produces a step. The safe pattern is
two buffers: write the new table into the inactive one, then ramp a mix value from
0 to full over one control block, 32 samples or 0.73 ms. The oscillator reads both
tables during the ramp. The extra cost is one table read and one multiply-add per
operator voice, which is small next to the generation.

The generator should run only when the harmonic recipe changes by more than a
small threshold, and a knob move at 30 to 60 Hz regenerates at the knob rate whilst
an LFO or envelope that reaches the harmonics needs a chosen update rate, with the
control block at 1,378 Hz available but not always needed because a 344 Hz update
gives a 2.9 ms step and a 0.73 ms crossfade. The table above lists both rates. The
choice is a measurement.

### Mipmaps and interpolation

A table with H harmonics aliases when H times the fundamental exceeds Nyquist. The
current phasor already computes a mip from the phase step. The additive table can
reuse `p1_wave_mip(step)` and generate one table per level with a lower harmonic
limit. The lowest level keeps one or two harmonics; the highest keeps all of them.

A 256-sample table with 16 harmonics gives 16 samples per cycle at the top
harmonic. Linear interpolation between the two nearest samples removes the
quantisation noise of a nearest read; the cost is one more multiply-add per read.
The current `osc` reads nearest because its 1024-sample tables are dense. A custom
table at 256 or 512 samples should use linear interpolation.

## Harmonic control surfaces

The engine can generate any 16 or 32 level vector. The open question is how the
player edits it and how much of it reaches the host.

### Raw harmonic levels

One host parameter per harmonic gives exact control. Sixteen levels per operator is
64 per part and 128 in the manifest; 32 levels is 256. The current part block is
228 parameters and the manifest is 470, so the raw form grows them to 356 and 598
for 16 levels, or 484 and 726 for 32. The part record grows by 256 or 512 bytes.

A 16 or 32 bar editor is a natural page. The cost is host automation lanes, a
larger preset and a larger editor. The table regeneration is triggered by every
level change.

### Spectral macros

A small set of controls generates the harmonics from a compact description: a
tilt, an odd and even balance, a low and high shelf, and one to three formant peaks
with centre, width and gain. Six to eight parameters per operator is 24 to 32 per
part and 48 to 64 in the manifest. The UI can draw the resulting 16 or 32 bars as a
read-only spectrum while the player turns macros.

A concrete recipe: a 16 harmonic table with tilt -1.2, odd and even balance +0.3, a
formant at harmonic 6 with width 2 and +6 dB, and the fundamental at 0 dB. The
table shows a peak near harmonic 6 and a falling tail. The generation is a few
operations per harmonic, which is negligible next to the H times N sum.

The macros are playable and automatable. The player who wants an exact vector needs
the raw editor; the macros cover most musical spectra.

### Harmonic bands

Eight band levels per operator, interpolated to the 16 or 32 harmonics, is a middle
ground: 32 parameters per part and 64 in the manifest. The UI is a familiar graphic
shape. The bands describe a harmonic envelope, so the shape moves with the note.
This is not a fixed formant filter, which is a separate method below.

### Recipe morph

A bank of four or eight spectra per operator, with one morph parameter, keeps the
automation small and the CPU low. The recipes are edited in a spectral designer and
stored with the preset, not exposed as host parameters. The oscillator reads the
two tables around the current position. The per-voice morph can follow an envelope.
The limit is that the spectrum stays inside the bank; a new colour needs a new
recipe.

### Formant filters

A graphic formant filter of 16 bands per operator is a different tool. The peaks
sit at fixed frequencies, so a per-part table cannot track them; the filter has to
run per voice. Sixteen bands at five operations per sample across 80 operator
voices is about 282 million operations per second. Four bands is about 70 million,
which is plausible as a quality mode with a lower voice count. A per-part filter
only works when the part plays in a narrow range.

The additive table and a formant filter answer different questions. The table
shapes the harmonic envelope relative to the fundamental, so it transposes with the
note. The filter shapes absolute frequencies, so a formant stays put. A hybrid of a
16 harmonic table and two to four per-voice formant peaks covers both, at the cost
of the filter state.

## Spectrum ownership

The table method forces a choice between three owners:

| Owner | Movement | Cost | Use |
| --- | --- | --- | --- |
| Part table | Knobs, macros and part LFOs | One read per operator voice | The default |
| Part table with per-voice morph | Envelopes and LFOs move between recipes | Two reads per operator voice | The expressive default |
| Per-voice table | Every voice has its own spectrum | Table memory and generation times the voice count | A quality mode |

The existing amplitude and feedback envelopes are per voice. If the additive
spectrum is part level, an envelope that changes the harmonic shape will not follow
the individual note unless the morph carries it. The per-voice morph is the
smallest change that preserves the current feel. The part table remains the unit
that is generated, which is where the CPU is saved.

## A worked example

A 16 harmonic table at 256 samples for a four-operator part:

- The recipe sets the fundamental and the first 15 harmonics. The table generator
  runs 16 times 256, or 4,096 multiply-adds per table.
- Four operators at 344 Hz is 4,096 times four times 344, or 5.6 million
  multiply-adds per second for that part. The second part doubles it.
- Three held notes read the same table three times per sample. The generation is
  unchanged by the note count.
- A dark and a bright table per operator, with the amplitude envelope as the morph
  value, doubles the reads to six per sample for three notes. The spectrum moves
  under each note while the part owns the two recipes.
- The same notes with per-voice tables would need three generations instead of one
  and three times the memory, for a spectrum that differs only by the envelope
  position.

## Recommendation

1. Start with 16 harmonics, a shared table per operator per part, and a per-voice
   morph between two part tables. This is the smallest design that gives per-voice
   spectral movement without a per-voice table.
2. Expose six to eight macros per operator and a read-only 16 bar spectrum. Add the
   raw 16 level editor only after a UI prototype shows the macros cannot cover the
   useful patches. The raw form adds 128 manifest parameters and a larger preset.
3. Generate the table from the existing sine table at 344 Hz by default, with a
   1,378 Hz option, a change threshold and a one-block crossfade. Generate the mip
   levels from the same amplitudes and reuse `p1_wave_mip`.
4. Keep the table bank in the engine state: eight tables for four operators and two
   parts, a second buffer for the crossfade, 256 samples at 16 harmonics, `int16`,
   with five mip levels. The memory is about 41 KB, close to the current table.
5. Keep the 12 standard waves as they are. The additive table is a new waveform
   mode per operator, so existing patches stay bit-exact when it is off.
6. Measure before expanding to 32 harmonics. The 32 harmonic table is still
   affordable at a 344 Hz update and 512 samples, but its memory and generation are
   four times the 16 harmonic case. The 1,378 Hz path is the one to watch. The
   headless measurement below covers the first pass; the integrated table remains.

## Headless measurement

The standalone harness is `plugin/tests/additive_bench.cpp`. It is a console
target, not a ctest gate. The baseline is the real engine at 20 voices and four
operators. The generator is the proposed shared-part algorithm measured in
isolation. The oscillator prototype measures the single-table nearest read and the
two-table linear and mip read at 80 operator voices, with the same 16/256 tables.
The integrated prototype is `plugin/tools/additive_prototype.py` and
`plugin/tests/additive_engine_bench.cpp`; the script copies the generated engine
into the build tree and replaces the four operator reads with the additive pair.

Platform: Windows 11 build 26300, AMD Ryzen 7 5825U (8 cores, 16 threads), clang
23.1.5, Release. The figures are one representative run and the minimum of three
iterations inside each measurement. The CPU clock moved the run totals by up to 25
percent, so read the shares as a range.

| Work | Time for 60 s | Share of one core |
| --- | --- | --- |
| Baseline, 20 voices, four operators | 931 ms | 1.55% |
| 16/256 table, eight tables, 344 Hz | 515 ms | 0.86% |
| 16/256 table, 1,378 Hz | 2,063 ms | 3.44% |
| 32/256 table, 344 Hz | 1,039 ms | 1.73% |
| 32/512 table, 344 Hz | 2,065 ms | 3.44% |
| 32/512 table, 1,378 Hz | 8,491 ms | 14.15% |
| Isolated single-table nearest read, 80 voices | 476 ms | 0.79% |
| Isolated two-table linear and mip read, 80 voices | 1,519 ms | 2.53% |
| Integrated additive engine, 20 voices | 1,253 ms | 2.09% |

The generator sustains about 1.0 to 1.3 billion multiply-adds per second on this
machine. The eight tables are four operators and two parts.

The integrated engine is 2.09% against the 1.55% baseline, so the additive
oscillator adds about 0.54% in the real voice graph. The isolated read model
overestimated the cost because its double loop differs from the inlined engine.
The table generation adds 0.86% at 16/256 and 344 Hz. A mip set roughly doubles
that, so the 16/256 path at 344 Hz is about 3% of one core without mips and 3.8%
with them. The 32/512 path at 344 Hz is about 5.5% and 9% with mips. The 32/512
path at 1,378 Hz is about 16% and 30% with mips, so the fast update remains the one
to watch. The 32/256 path at 344 Hz is about 3.8% and 5.6% with mips.

The table must be normalised. The first run summed the harmonics without
normalising, the table clipped, and the alias floor read as -23 dB. The normalised
table gives a table error at C3 and C4 of -22.1 dB with a nearest read and -47.9 dB
with linear interpolation. The alias floor at C7 is:

| H/N | Mip on, nearest | Mip on, linear | Mip off, nearest | Mip off, linear |
| --- | --- | --- | --- | --- |
| 16/256 | -61.6 dB | -87.3 dB | -13.9 dB | -13.9 dB |
| 16/512 | -61.6 dB | -90.3 dB | -13.9 dB | -13.9 dB |
| 32/256 | -57.4 dB | -72.3 dB | -11.7 dB | -11.8 dB |
| 32/512 | -57.5 dB | -72.6 dB | -11.6 dB | -11.6 dB |

The mip removes 43 to 76 dB of folded energy, and the linear read adds 13 to 29 dB
at the mip. The linear read is the clear choice. The 256 and 512 sample sizes do
not change the alias floor at C7.

The conservative mip costs brightness. The fifth to eighth harmonics at C7 measure:

| H/N | Conservative mip | Bright mip |
| --- | --- | --- |
| 16/256 | -91.6 dB | +4.2 dB |
| 16/512 | -94.0 dB | +4.2 dB |
| 32/256 | -88.8 dB | +4.2 dB |
| 32/512 | -89.5 dB | +4.2 dB |

The bright mip puts the top harmonic just below Nyquist and recovers the group;
the conservative mip keeps it an octave below and loses it. A production policy can
sit between the two, or the table can carry more mip levels.

The auditions are:

- `build/additive-headless.wav`: the per-sample morph from the standalone table
  oscillator.
- `build/additive-oscillator.wav`: the 344 Hz control-rate morph with the mix
  interpolated across the full block.
- `build/additive-engine.wav`: the integrated engine, four notes, a single clean
  carrier (algorithm 7, one operator, no feedback) and the four-second time ramp.
  The full four-operator graph remains in the 20-voice CPU row.

The first two are table oscillators. The third is the real voice graph with the
additive oscillator. The full init patch adds its modulators and FM sidebands,
which is why the first engine audition sounded brighter; the clean carrier
isolates the table and the morph from that FM.

## Remaining experiments

- **Dynamic table generation:** regenerate the shared table at 344 or 1,378 Hz in
  the engine and crossfade, with the parameter surface and the normalisation.
- **Mip policy:** choose a level between the conservative and the Nyquist edge,
  and measure the alias risk under vibrato and detune.
- **Formant hybrid:** four per-voice peaks on the table output. Check the fixed
  formants across two octaves and the CPU cost.
- **Perceptual:** the three auditions, a formant peak and a 32 harmonic saw, judged
  against the spectral character and the envelope, LFO and modulation timing.

The stop criteria are the current engine's exact render when the additive mode is
off, an alias floor no worse than the band-limited waves, no click at a table
change, and a full-polyphony cost that keeps the plugin within its CPU budget. The
`plugin_stability` test and the 44.1 kHz parity test stay in the gate.

## Sources

1. C. Roads, *The Computer Music Tutorial*, MIT Press, 1996, chapters on additive
   and wavetable synthesis.
2. J. O. Smith III, *Spectral Audio Signal Processing*, online book, CCRMA,
   Stanford, 2011.
3. J. Moorer, "The Synthesis of Complex Audio Spectra by Means of Discrete
   Summation Formulas", *Journal of the Audio Engineering Society* 25(10/11),
   1977.
4. D. Arfib, "Digital Synthesis of Complex Spectra by Means of Multiplication of
   Nonlinear Distorted Sine Waves", *Journal of the Audio Engineering Society*
   27(10), 1979.
5. A. Horner, J. Beauchamp and L. Haken, "Methods for Multiple Wavetable Synthesis
   of Musical Instrument Tones", *Journal of the Audio Engineering Society*
   41(5), 1993.
6. X. Serra, *A System for Sound Analysis, Transformation and Synthesis Based on a
   Deterministic plus Stochastic Decomposition*, PhD thesis, Stanford University,
   1989.
7. V. Välimäki and A. Huovilainen, "Oscillator and Filter Algorithms for Virtual
   Analog Synthesis", *Computer Music Journal* 30(2), 2006.
8. U. Zölzer (ed.), *DAFX: Digital Audio Effects*, second edition, Wiley, 2011.
9. M. Puckette, *The Theory and Technique of Electronic Music*, World Scientific,
   2007.
