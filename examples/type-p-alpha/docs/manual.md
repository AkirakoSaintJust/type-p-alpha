# Type P.fm1 user manual

Type P.fm1 is a two-part x four-operator phase-modulation synthesizer in the tradition of the Yamaha DX/OPwhatever family. Yamaha called it FM because that's what John Chowning's original research did, and the name stuck for historical reasons, but it's phase modulation really.
In each of the two parts, you've got four oscillators, which come together with a dedicated amplitude envelope and a dedicated amplitude LFO in units called operators, and then set them up in one of eight routings, aka algorithms, so that they phase-modulate each other. The combined output of both parts' algorithms plays through a filter. As for modulation, besides the per-operator tremolo, you've got one per-part pitch envelope, one per-part pitch LFO, and one global filter envelope. You can also route velocity and key scaling to several parameters within the synth.

I'm a hardcore fan of phase modulation synths, so for the longest time I've kept thinking about things I wanted to change on the Yamahas (and Yamaha emulators) I've been using over the years. That's where Type P.fm1 comes from. Originally, I had a far more complex design incorporating dynamic phase distortion and a kind of phase-indexing granular synthesis, which would have been the "full", "ideal", Type P, but the FM1 is relatively limited both in power and in user interface, plus I wanted to test just how well exactly this framework for making FM1 firmwares works, so I kinda wanted to keep the scope limited.

Anyway, some of the things that have stood out the most to me as a PM synthesis fan are:
1. making patches out of layered partials works especially well with PM.
2. you always need more feedback operators.
3. the extra waveforms on the OPL3 and the TX81z/D11/V50 were actually pretty handy as weird as they are.
4. envelope graphs are essential, especially if you've got rate-level envelopes, because they help massively with the sheer amount of params, and provide an easy overview of what the patch is like at a given moment.
5. oscilloscopes and spectrograms do help a lot.
6. velocity and key scaling are cool but they've got to stay in their little corner because there are just so! many! params!
7. being able to copy/paste envelopes or lfos or whole operators or even whole parts makes your life easier.

So that's the concept basically. It's not an emulation of any one Yamaha chip in particular, and the DSP was implemented from scratch, but it **is** a DX-style synth, and it **does** take cues from the way Yamaha emulators are implemented, particularly ymfm and Dexed. There are other things taken specifically from DX synths, like the general character of the envelopes or the fact the knobs have detents at multiples of 1/2, sqrt(2)/2, pi/2 and sqrt(3)/2.

It's two 4-operator parts because that's where I believe the sweet spot lies. There's this massive jump in complexity between editing a DX21 from the panel vs editing a DX7, and I think it's a combination of the DX7's more complex envelopes and key scaling, and the mental overhead of juggling 6 operators. 4-op is more manageable, then you can work on the second part, and the envelope graphs help massively with keeping track of what you're doing. Then you can look into the key scaling and velocity later, although I've got to say they've been greatly simplified here compared to what the DX7 offers.

What I mean is I designed Type P.fm1 as a manageable 10-voice, 8-op synth where you work with partials, but if you want to do key splits or assign each part to a different MIDI channel you can do that as well. I'm more of a sound design gal, I fully sequence my music and I prefer trackers to little boxes for sequences/composition/arrangement, but it should work for those users as well.


## Voice structure

A voice is four operators plus an output path.

- Each operator is an oscillator with its own pitch ratio, waveform, level,
  feedback, and a four-stage rate-level amplitude envelope.
- The algorithm decides which operators are carriers (you hear them) and which
  are modulators (they bend the phase of the next operator).
- The output runs through a multimode filter, which is bypassable.

As I said, the two parts play at once, so you can layer two timbres, or split the keyboard, or send two different sequences over two different MIDI channels.
There's a total of twenty voices, shared between the parts.

## Choosing an algorithm

"Algorithm" just sounds so cool, innit? The `ALGORITHM` knob, 0 to 7, lets you rewire where the output of the four operators goes. Lower numbers are stacked, i.e. one operator modulating the next. Higher numbers are more parallel, so you've got more carriers playing at once. Which on the original DX synths was more of a drawbar organ thing, but since you've got different waveforms and feedback on every operator here, you can do supersaws or epic pads or whatever. Each algorithm has a different character, and the same
four operator settings will sound different on each one.

The screen shows the algorithm number and a small map of the routing.

## Operator settings

Each operator has the following settings. In the operator editor, the `SELECT` knob steps through
the pages and the `SEL` button moves between operators 1 to 4, then wraps around back to 1.

### Ratio

The pitch of the operator relative to the note, as a multiplier. 1.00 is the base, unmodified pitch.
2.00 is an octave up, 4.00 two octaves up, and so on. 0.50 is an octave down. Whole-number ratios sound clear, odd ratios sound metallic/reedy, and non-integer ratios sound clangy and inharmonic, which can be industrial noise or struck-type sounds like marimbas, vibraphones, weird glassy pads, and the Taco Bell bell (I've never been to a Taco Bell but being terminally online has taught me Americans find this really funny.)

The knob steps through a fixed ladder: multiples of 0.50, 1.00, 1.41, 1.57 and 1.73, with 0.71, 0.79 and 0.87 near the bottom. 1.41 is √2, 1.57 is π/2 and 1.73 is √3, so the inharmonic stops line up with the OPM detune table and with the ratios used for bells, metal and gongs. The readout shows two decimals, and typing a value snaps to the nearest stop.

### Level

How loud the operator is inside the voice. The scale is the Yamaha total-level one:
0.75 decibels per step, so 99 is full, 50 is about 37 decibels quieter, and 0 is
silent. For a modulator, the level is the modulation depth, so this is where the
timbre is shaped as well as the volume. A ratio 1 modulator at 75 gives about half
a cycle of phase deviation, which is the saw-ish region. Extreme modulation levels
get noisy, so be careful or take advantage of that.

### Feedback

The operator feeds its own output back into its own phase. Zero is the clean waveform. As you
raise the feedback, the sound roughens, then turns into noise at the top. The depth follows the operator's level, so a quiet operator feeds back less, which keeps a decaying sound from getting harsher as it fades. All operators have feedback, but traditionally it shows up the most on carriers and on the top operator of a stack. That's because of the way Yamaha chips work: the operators were always computed in the same order and the one with feedback was always the first one.

Tip: a sine operator taking no modulation will kinda become a reverse sawtooth at about 25%-35%.
Tip 2: modulators become more intense when they have feedback, so it's kinda like cranking the level up beyond its limit.
Tip 3: A sine modulator at ratio 2.00 and 25%-35% feedback going into a sine operator at ratio 1.00 will give you a square-ish kind of waveform, although you do have actual square waves in Type P.fm1.

### Waveform

Twelve shapes, 0 to 11. The first nine are sine-derived and stay clean up high.
Waves 9 to 11 are band-limited pulse, square, and saw, with the brightness rolled
off automatically as the note rises to stop aliasing. The tables carry up to 256 harmonics at the bottom of the keyboard and taper to one at the top, so a low note keeps its body instead of stopping at a 1 kHz edge. If you find high notes are too harsh, try tweaking the key follow to rein modulator levels in as you go up the keyboard, or use the filters to cut the ghosts out.

### Detune

A fine pitch offset of plus or minus 50 cents. Detuning two operators that would
otherwise be in unison thickens the sound and adds movement. Detuning modulators can add a beating effect, which sometimes comes in handy if you want to keep carriers out of phase.

### Envelope

Four rates and three levels. Rate 1 is the attack, rate 2 and rate 3 are the two
decay stages, rate 4 is the release. Level 1, 2, and 3 set where the envelope
holds after each stage. Higher rates are faster, lower slower: a full-scale stage runs
from 0.7 ms at 99 to about 16 s at 0. Each stage eases into its level rather than running
in a straight line, so a decay covers most of its drop early and settles gently. The envelope is per-operator, so
a modulator can fade in and out on its own schedule.

### Tremolo

A per-operator amplitude LFO. Rate sets the speed, depth sets the amount in
operator-level steps, and the bipolar switch chooses whether the level swings
around the operator's level or starts there and moves to the depth. On the
device, the switch is the fourth knob on the `A-LFO` page, so the three controls
above it keep their familiar places. A slow sine with a depth of ten steps gives a
gentle wobble, whilst a fast square with a depth near the top makes a carrier pump
hard enough to hear the gate.

### Mute

Turns the operator off entirely. Muting a modulator cuts phase modulation to the operator it
drives. Muting a carrier removes that carrier from the mix. That's useful if you're focusing on a particular operator stack or want to check what a certain operator adds to your patch.

## Poly, mono, and unison

- **Mode** is poly or mono. Mono plays one note at a time.
- **Unison count** stacks that many detuned copies of the note in mono.
- **Unison detune** spreads those copies.
- **Legato** holds the envelope across overlapping mono notes.
- **Portamento** slides the pitch between mono notes. The amount is the slide
  time.
- **Note priority** picks which note wins when more than one is held in mono.
- **Part detune and transpose** shift a whole part in cents or semitones. They are
  on `SINGLE PART TUNING` in `GLO`.

## Filter

A filter, then the DC blocker.

- **Cutoff** and **resonance** set the filter. Cutoff is the frequency the filter
  starts to remove. Resonance is the peak at the cutoff, i.e. it makes the part of your patch that's at the cutoff frequency louder. The resonance display reads 0.00 to 100.00 percent, and the knob is curved so most of the range is the strong resonance near the top. At the top the ladder can ring on its own, kinda like guitar squeals or a sine wave with harmonic distortion.
- **Filter type** is low-pass, band-pass, or high-pass.
- **Filter envelope** bends the filter's cutoff with a four-stage envelope,
  so the sound can open and close in brightness as the note goes on. It restarts
  on every note-on. The filter and its envelope are global, so a new note restarts
  the shared shape for both parts.
- **DC blocker** sits at the output and removes the offset that a pulse waveform can carry. It leaves the audio band alone.

## Vibrato and pitch envelope

- **Pitch LFO** is a vibrato. Rate sets the speed, depth sets the amount. The depth
  reads -12.00 to +12.00 st, and the bipolar switch chooses whether the pitch swings
  around the note or starts at the note and moves to the depth. In bipolar mode the
  depth is a peak deviation, with one-cent steps through the first semitone and the
  envelope-style detents above it. A depth of 0.25 st with a 5 Hz sine is a mild
  vibrato, whilst 2.00 st with a slow triangle is a wide, vocal slide that makes a
  single held note sound like a player leaning into it.
- **Pitch envelope** bends the pitch of the whole voice over the course of a
  note, in semitones. It is a four-stage, rate-level, envelope like the operators' amplitude
  one, but it moves pitch instead of volume, and the levels are pitch offsets relative to the base, unmodified, note.

## Key and velocity follow

- **Breakpoint** is the note where the keyboard split sits. Below it, one set of
  amounts applies. Above it, another.
- **Key follow** scales the operator level, feedback, envelope, filter cutoff,
  and/or pitch envelope by the note position, so higher or lower notes can be
  brighter, darker, or quieter.
- **Velocity follow** does the same from how hard you play, and a global on-off
  turns the whole velocity system off.

## The panel

The first part of this manual was about the sound. This part is about finding your
way around the instrument. The M-VAVE FM-1's control panel has 27 keys, a
volume pot, three left encoders, four page encoders, the transport buttons, and a
240 by 240 screen. The keys and the `ALGORITHM` encoder are always available.
Everything else is a page you open with a context button.

### The surface

- **27 keys** play notes, F3 to G5. `OCT−` and `OCT+` move the keybed an octave
  down or up, clamped to two octaves either way.
- **MASTER** is the volume pot. It is always volume.
- **ALGORITHM** is the dedicated algorithm encoder. It always edits the active
  part's algorithm. Turning it shows the algorithm graph for about 600 ms after
  you stop.
- **SELECT** is the page encoder. It moves forward and back through the current
  context's pages.
- **PRESETS** opens the preset browser.
- **KNOB 1 to 4** are the page encoders. On a parameter page they edit the
  parameter rows from top to bottom. If a page has fewer than four rows, the extra
  knobs do nothing. Display pages have no assignments.
- **HOME, EDIT, ENV, LFO, FX, GLO** are the six context buttons.
- **SEL** selects the next operator on a single tap. A double tap within about
  300 ms switches the active part. Hold SEL for the chords below. The single tap
  lands after the double-tap window, so there is a short pause.
- **SAVE** opens the save chooser. With SEL it initialises the patch or a part.
- **ARP, SEQ, PLAY, REC** mute operators 1 to 4. With SEL they mute parts.

### Navigation

Press a context button to open that context at its first page. Press the same
button again to move to the next page. `SELECT` also pages forward and back. The
pages wrap at the ends. Every context's pages are in the order shown in its table,
so pressing the context button again or turning `SELECT` walks them in that order.

`HOME` pops back to the home context. Press `HOME` again to step through the home
pages.

The header stays at the top. The first line shows the parts, the four operators,
the algorithm, and the context and page. The second line shows the current page
title. The active part and operator are light blue, muted parts and operators are
red, parameter values and the algorithm number are hot pink, and the page
description is bright green.

The footer shows the gesture legend:

| Gesture | Action |
| --- | --- |
| `SEL` (single tap) | Next operator |
| `2x SEL` | Switch part |
| `SEL` + key | Mute the active operator |
| `SEL` + `EDIT` | Copy the current page |
| `SEL` + `SAVE` | Init patch, or init part in GLO |
| `SEL` + `ARP` or `SEL` + `PLAY` | Mute part 1 |
| `SEL` + `SEQ` or `SEL` + `REC` | Mute part 2 |
| `ARP`, `SEQ`, `PLAY`, `REC` | Mute operators 1, 2, 3, 4 |

A popup owns the controls while it is open. It closes on its own after about
three seconds of no input without committing. Press `SAVE` to commit.

### HOME

The home context has nine pages. It opens on `PERFORMANCE`, the live page. The
three live knobs there are cutoff, resonance, and operator 1 feedback, and the
fourth has no assignment. The other eight pages are displays, and the knobs have
no assignments there. The `ALGORITHM` encoder and the keys stay live everywhere.

| Page | Contents |
| --- | --- |
| PERFORMANCE | The live knobs: cutoff, resonance, and operator 1 feedback. |
| ALGORITHM | The current algorithm as four operator blocks and a routing string. Carriers are pink, modulators white. Turn `ALGORITHM` to change it. |
| A-ENVS | An amplitude-envelope graph for each operator of the active part. |
| SCOPES | An oscilloscope trace for each operator of the active part. |
| SPECTROS | A spectrogram for each operator of the active part. |
| SCOPES | A larger scope for each part mix. |
| SPECTROS | A larger spectrogram for each part mix. |
| POST-FX MULTI-PATCH SCOPE | The final mixed output after the filter. |
| POST-FX MULTI-PATCH SPECTRO | The final mix as a spectrogram. |

The two pairs of scope pages share a title. The first pair shows four small traces
for the operators. The second pair shows two larger traces for the part mixes. The
post-FX pages say `POST-FX MULTI-PATCH`, so they are easy to tell apart.

The algorithm graph popup appears while you turn the encoder and stays for about
600 ms after you stop.

### EDIT

The `EDIT` context is the operator editor. It has four pages. `SEL` chooses the
operator, 1 to 4, and the header shows which one. The first three pages follow the
selected operator. `OP DETUNES` is part-level and shows all four operators.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| OSCILLATOR | RATIO | LEVEL | FEEDBACK | WAVE | A scope of the selected operator. |
| OP DETUNES | OP 1 | OP 2 | OP 3 | OP 4 | The four per-operator detunes for the active part. |
| OP KEY FOLLOW | OP LEVEL KS LO | OP LEVEL KS HI | OP FB KS LO | OP FB KS HI | The selected operator's level and feedback key follow. |
| OP VEL SENS | OP LEVEL VS | OP FB VS | | | The selected operator's level and feedback velocity sensitivity. |

`OP DETUNES` is the only part-level page here. The key-follow and velocity pages
are operator-relative, so they change the operator named in the header.

### ENV

The `ENV` context is the envelope editor. It has six pages. The A-ENV pages edit
the selected operator's amplitude envelope. The P-ENV pages edit the part's pitch
envelope. The last two pages set the key and velocity follow for the envelope
rates.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| A-ENV (rates) | ATTACK RATE | DECAY 1 RATE | DECAY 2 RATE | RELEASE RATE | The selected operator's A-ENV graph. |
| A-ENV (levels) | LEVEL 1 | LEVEL 2 | SUSTAIN | | The same graph, for the levels. |
| P-ENV (rates) | ON | ATTACK RATE | DECAY 1 RATE | DECAY 2 RATE | The part pitch-envelope graph. |
| P-ENV (levels) | RELEASE RATE | LEVEL 1 | LEVEL 2 | SUSTAIN | The same graph, for the levels. |
| ENV KEY SCALE | A-ENV RATE KS LO | A-ENV RATE KS HI | P-ENV RATE KS LO | P-ENV RATE KS HI | The rate key-follow amounts. |
| VEL SENS | A-ENV RATE VS | P-ENV RATE VS | | | The rate velocity-follow amounts. |

Decay 2 rate 1 is the slowest musical point, and it lines up with the OPM's slowest second decay. The range has 99 steps, so a converted voice keeps more of its slow fade than the OPM's 31 steps would give.

Both A-ENV pages are titled `A-ENV`, and both P-ENV pages are titled `P-ENV`. The
row labels tell you which half you are on. The A-ENV pages follow the selected
operator. The P-ENV pages are part-level.

### LFO

The `LFO` context has two pages. `A-LFO` is the selected operator's amplitude LFO.
`P-LFO` is the part pitch LFO. This is a separate context from `GLO`.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| A-LFO | RATE | DEPTH | WAVE | BIPOLAR | The selected operator's amplitude LFO. |
| P-LFO | RATE | DEPTH | WAVE | BIPOLAR | The part pitch LFO. |

The fourth knob is the unipolar/bipolar switch on both pages, and the three
controls to its left keep their jobs, so a player who knows the old page can find
the switch without relearning the layout.

### FX

The `FX` context is the output chain. It has five pages. The signal order is the
filter, then the DC blocker. The filter has the F-ENV. The key-follow and velocity
pages set how the filter and the F-ENV respond across the keyboard.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| FILTER | ON | CUTOFF | RES | TYPE | Filter 1. |
| F-ENV (rates) | ATTACK RATE | DECAY 1 RATE | DECAY 2 RATE | RELEASE RATE | The filter-envelope graph. |
| F-ENV (levels) | LEVEL 1 | LEVEL 2 | SUSTAIN | DEPTH | The filter-envelope graph and its depth. |
| F1 KEY FOLLOW | F1 CUTOFF KS LO | F1 CUTOFF KS HI | F-ENV RATE KS LO | F-ENV RATE KS HI | Filter and F-ENV key follow. |
| FILTER VEL SENS | F1 CUTOFF VS | F-ENV RATE VS | | | Filter and F-ENV velocity sensitivity. |

Both F-ENV pages are titled `F-ENV`. The rates page has the four stage rates. The
levels page has the three levels and the envelope depth.

### GLO

The `GLO` context is the setup and global page. It has eleven pages. This is where
the two parts are layered, tuned, split, assigned to MIDI channels, and named. It
also holds the voice mode, the mono stack, the backup page, and the credits.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| SINGLE PART LAYERING | PART 1 LEVEL | PART 1 DELAY | PART 2 LEVEL | PART 2 DELAY | The two part levels and delays. |
| SINGLE PART TUNING | PART 1 DETUNE | PART 1 TRANSPOSE | PART 2 DETUNE | PART 2 TRANSPOSE | The part detune and transpose. |
| KEY SPLIT | P1 LOW | P1 HIGH | P2 LOW | P2 HIGH | The key ranges for the two parts. |
| MIDI CH | P1 CH | P2 CH | | | The MIDI channel for each part. |
| KEY SCALING | BREAKPOINT | KEY SENS ON | | | The global key-follow breakpoint and switch. The targets are in EDIT, FX, and ENV. |
| VEL SENS | VEL SENS ON | | | | The global velocity-follow switch. The targets are in EDIT, FX, and ENV. |
| VOICE MODE | MODE | PRIORITY | LEGATO | PORTA | The part voice mode. |
| MONO STACK | VOICES | DETUNE | | | The mono stack count and its spread. |
| NAMES | PART 1 NAME | PART 2 NAME | MULTI-PATCH NAME | | Open the name editor with the matching knob. |
| BACKUP / LOAD | | | | | Host-side SysEx backup, `umetate preset export/import`, model byte 64. No MIDI channel is needed. |
| VERSION + CREDITS | | | | | The firmware version, the product name, and the typeface credit. |

`KEY SCALING` and `VEL SENS` only hold the breakpoint and the on/off switch. The
per-target amounts are in `EDIT` for operator level and feedback, in `FX` for the
filter cutoffs and the F-ENV rate, and in `ENV` for the A-ENV and P-ENV rates.

`VOICE MODE` has four rows. `MONO STACK` has two: VOICES and DETUNE. The other two
knobs on `MONO STACK` do nothing.

`SINGLE PART TUNING` has one detune and one transpose for each part. Detune is in
cents, so it is the fine tune. Transpose is in whole semitones, so it is the coarse
tune. Both parts can be shifted independently, which is how you get an octave
layer or a slight beating between the two parts.

The `NAMES` page opens the name editor from the matching knob. The editor uses
`K1` for the character, `K2` for the cursor, `SEL` to delete, `SAVE` to accept, and
`HOME` to cancel. The editor auto-closes after about three seconds of no input and
restores the old name.

`BACKUP / LOAD` and `VERSION + CREDITS` are read-only pages. The four page knobs do
nothing there.

### Parameter reference

The sound-design chapters above explain the main parameters in depth. This table
is the quick lookup for every page row, including the ones the chapters do not
cover. The ranges are the ones the panel shows.

#### Operator

| Parameter | Range | Functionality |
| --- | --- | --- |
| RATIO | 0.50 to 16.00 | The operator's pitch as a multiple of the note. 1.00 leaves the operator at the note's base frequency, 2.00 an octave up, 0.50 an octave down. The knob stops on multiples of 0.50, √2, π/2 and √3, with 0.71, 0.79 and 0.87 near the bottom. Whole-number ratios sound clear, odd ratios reedy, and non-integer ratios clangy. |
| LEVEL | 0 to 99 | The operator's output level. 99 is full, 50 is about 37 dB down, and 0 is silent. The scale is the Yamaha total-level one, 0.75 dB per step. On a modulator this is the modulation depth, and 75 is about half a cycle at ratio 1. |
| FEEDBACK | 0 to 100% | The operator's output fed back into its own phase. 0 is clean, higher values roughen the wave, and the top turns to noise. A sine at 25 to 35% becomes a reverse saw. |
| WAVE | 0 to 11 | The operator waveform. 0 to 8 are sine-derived and stay clean high up. 9 is pulse, 10 is square, and 11 is saw, all band-limited with a nine-step mip map. |
| OP DETUNE | -50 to +50 cents | A fine offset for one operator. Detuning two operators that would be in unison thickens the sound. Detuning a modulator adds beating. |
| OP LEVEL KS LO/HI | -100 to +100% | The amount the operator level follows the keyboard below and above the breakpoint. Positive pushes the level toward its extreme, and negative pushes the other way. The init patch sets operator 1's high and low key follow to -50, and operator 3's low follow to -50, so a modulator narrows at the top and the bottom. |
| OP FB KS LO/HI | -100 to +100% | The same key follow for feedback. |
| OP LEVEL VS | -100 to +100% | The amount velocity moves the operator level at maximum velocity. The global `VEL SENS ON` switch must be on. |
| OP FB VS | -100 to +100% | The same velocity follow for feedback. |
| A-ENV RATE 1 to 4 | 0 to 99 | The four stage rates. Higher is faster. Rate 1 is attack, 2 and 3 are decay, and 4 is release. |
| A-ENV LEVEL 1 to 3 | 0 to 99 | The levels the envelope holds after each stage. Level 3 is the sustain. |
| A-ENV RATE KS LO/HI | -100 to +100% | Key follow for the A-ENV rate. |
| A-ENV RATE VS | -100 to +100% | Velocity follow for the A-ENV rate. |
| A-LFO RATE | Hz | The tremolo speed. |
| A-LFO DEPTH | -99 to +99 level steps | The tremolo amount, in the same units as `LEVEL`. In bipolar mode it is the peak deviation around the operator level. In unipolar mode the operator level is the starting point and the depth is the value the LFO moves to. |
| A-LFO WAVE | 0 to 11 | The tremolo waveform. Sine is the usual choice. |
| A-LFO BIPOLAR | on/off | Off is unipolar and on is bipolar. |

#### Pitch envelope and pitch LFO

| Parameter | Range | Functionality |
| --- | --- | --- |
| P-ENV ON | on/off | Turns the part pitch envelope on. |
| P-ENV RATE 1 to 4 | 0 to 99 | The four pitch-envelope rates. Higher is faster. |
| P-ENV LEVEL 1 to 3 | semitone offsets | The pitch offset at each stage, relative to the note. |
| P-ENV RATE KS LO/HI | -100 to +100% | Key follow for the P-ENV rate. |
| P-ENV RATE VS | -100 to +100% | Velocity follow for the P-ENV rate. |
| P-LFO RATE | Hz | The vibrato speed. |
| P-LFO DEPTH | -12.00 to +12.00 st | The vibrato amount. In bipolar mode it is the peak deviation around the note. In unipolar mode the note is the starting point and the depth is the value the LFO moves to. The first semitone steps in cents, the rest in envelope-style detents. |
| P-LFO WAVE | 0 to 11 | The vibrato waveform. |
| P-LFO BIPOLAR | on/off | Off is unipolar and on is bipolar. |

The plugin adds a fourth pitch-envelope level, `L4`, which is where the release
segment ends, whilst the device panel keeps three level knobs and uses `L4` at
0.00 st. The ranges above are the ones the panel shows, and the plugin uses the
same ranges and detents, so a patch moves between the two without a conversion
step.

#### Filter

| Parameter | Range | Functionality |
| --- | --- | --- |
| FILTER ON | on/off | Bypasses or enables filter 1. |
| CUTOFF | off, 1 to 100% | The frequency where filter 1 starts to remove. Turn it down to darken the sound and up to open it. |
| RES | 0 to 100% | The peak at the cutoff. The display is curved, so the top of the range is the strongest resonance. At the top the ladder can ring on its own. |
| TYPE | LPF, BPF, HPF | The filter response. |
| F-ENV RATE 1 to 4 | 0 to 99 | The four filter-envelope rates. Higher is faster. |
| F-ENV LEVEL 1 to 3 | 0 to 99 | The levels the filter envelope holds. Level 3 is the sustain. |
| F-ENV DEPTH | 0 to 100% | The cutoff movement from the filter envelope. |
| F1 CUTOFF KS LO/HI | -100 to +100% | Key follow for filter 1 cutoff. |
| F-ENV RATE KS LO/HI | -100 to +100% | Key follow for the F-ENV rate. |
| F1 CUTOFF VS | -100 to +100% | Velocity follow for filter 1 cutoff. |
| F-ENV RATE VS | -100 to +100% | Velocity follow for the F-ENV rate. |

#### Global

| Parameter | Range | Functionality |
| --- | --- | --- |
| PART 1/2 LEVEL | 0 to 99 | The part's output level. 99 is full, 50 is about 37 dB down, and 0 is silent. |
| PART 1/2 DELAY | 0 to 1000 ms | Delays that part against the other. |
| P1/P2 LOW/HIGH | note names | The key range for each part. Notes outside it are ignored by that part. |
| P1/P2 CH | MIDI channel | The channel each part listens to. |
| BREAKPOINT | note name | The note where key follow is neutral. |
| KEY SENS ON | on/off | The global key-follow switch. |
| VEL SENS ON | on/off | The global velocity-follow switch. |
| MODE | poly, mono | Poly plays all notes. Mono plays one note at a time. |
| PRIORITY | last, highest, lowest | The note that wins when several are held in mono. |
| LEGATO | on/off | Holds the envelope across overlapping mono notes. |
| PORTA | 0 to 5000 ms | The one-octave glide time. 0 is off. |
| PART 1/2 DETUNE | -50 to +50 cents | The fine tune for that part. It is on `SINGLE PART TUNING`. |
| PART 1/2 TRANSPOSE | -24 to +24 semitones | The coarse tune for that part, in whole semitones. It is on `SINGLE PART TUNING`. |
| VOICES | 1 to 8 | The mono stack size. |
| DETUNE | 0 to 200 cents | The spread between the stacked copies. |

Part delay applies to each new note in poly. In mono it applies to the first
note after silence, because a later note takes over the sounding voice.

### Saving and loading

`SAVE` opens the save chooser. The chooser has two rows, `single part` and
`multi-patch`, and each row shows the slot number and the stored name. `KNOB 1`
toggles the kind, `KNOB 2` scrolls the slot, `SAVE` commits, and `HOME` cancels.

For a single part, `SAVE` opens the `WHICH SINGLE PART` chooser. `KNOB 1` picks
part 1 or part 2, `SAVE` saves, and `HOME` cancels.

For a multi-patch, `SAVE` opens the save-mode chooser. `KNOB 1` picks the mode,
`SAVE` saves, and `HOME` goes back to the slot chooser. The three modes work on
both parts:

- **discard edits to parts**: save the composition using each part's stored
  single-part slot, so neither part's live edits are written into a part slot.
- **save as new parts**: write both parts' live edits to new empty single-part
  slots, then save the multi-patch referencing them.
- **mutate parts in place**: write both parts' live edits into their existing
  single-part slots, then save the multi-patch. A part with no stored slot gets a
  new one.

The actual bank write waits until the engine is quiet, so a save never clicks.

`PRESETS` opens the preset browser. Outside `GLO` it browses the multi-patches. In
`GLO` it browses the single parts. The popup shows the slot number and the stored
name. Scroll with the encoder, then stop for about two seconds. A clean patch
loads. If you have edited the patch, a `CONFIRM` popup asks `LOAD PATCH? UNSAVED
EDITS`. `SAVE` loads and discards the edits, and `HOME` keeps them. If the selected
slot is the one already loaded, the browser just closes. A single-part load opens
the `WHICH SINGLE PART` chooser first, and that chooser still needs `SAVE` to
apply.

`SEL` + `SAVE` outside `GLO` opens a confirmation popup. `SAVE` resets the whole
patch, `HOME` cancels. After the reset, both parts go to their init values, part 2
is muted, and the names reset to `INIT PART` and `INIT MULTI`. In `GLO`, `SEL` +
`SAVE` opens the part chooser so you can initialise one part.

`SEL` + `EDIT` copies the current page. On a part-level page the chooser lists the
two parts. On an operator page it lists the eight part and operator targets.
`KNOB 1` picks the target, `SAVE` applies, and `HOME` cancels. On a page with
nothing to copy, the chord opens `EDIT` instead.

### The screen and the LEDs

The screen is 240 by 240. The header and footer stay fixed while the page below
them changes. The header shows the active part and operator, the algorithm, the
context and page, and the page title. The footer carries the gesture legend.

Keys light as they sound. The `OCT−` and `OCT+` buttons light when the keybed is
shifted below or above its base. Muted parts and operators show red in the header.
The parameter values and the algorithm number are hot pink, and the page
description is bright green.

Popups are modal. They cover the page, take every control, and close after about
three seconds of no input without committing. `SAVE` commits the save and name
editors.

### Quick reference

| Context | Pages | Functionality |
| --- | --- | --- |
| HOME | 9 | Performance knobs, algorithm graph, envelope graphs, scopes, spectros, post-FX displays. |
| EDIT | 4 | Operator oscillator, detunes, key follow, velocity sensitivity. |
| ENV | 6 | A-ENV, P-ENV, envelope key follow, envelope velocity sensitivity. |
| LFO | 2 | A-LFO and P-LFO. |
| FX | 5 | Filter 1, F-ENV, filter follow. |
| GLO | 11 | Parts, tuning, split, MIDI channels, voice mode, names, backup, credits. |

With the map in hand, the next section is a first patch to try.

## Get started!

Start on algorithm 7, the additive one. Set the four ratios to 1.00, 2.00, 3.00,
and 4.00, all carriers. You get a drawbar-organ stack. Lower operator 2 a little
and you hear how level shapes the timbre. Then switch to algorithm 0 and the same
four operators become a deep FM bass. Try looking at the operator oscilloscopes to check out how the cascading phase modulation progressively warps each waveform!
