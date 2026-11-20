# 01 · Make a sound

<p class="lede">Type P.α starts with four sine operators. This chapter makes one
sound, then shapes it with the controls you will use in every patch.</p>

## The init patch

Load the init patch and hold middle C. You hear four sines together: two at the
note, one an octave and a fifth above, and one an octave above. The four
operators are all carriers, so their outputs add rather than modulate each other.

Each operator card carries the same controls:

- **level** sets how much of the operator reaches the output.
- **ratio** multiplies the note frequency. 1.00 is the note, 2.00 is an octave,
  3.00 is an octave and a fifth.
- **waveform** chooses one of the thirteen single-cycle tables, plus the spectrum
  mode.
- **feedback** feeds the operator back into itself.
- **detune** moves the operator in cents.

<figure>
<img src="figures/editor-operator-envs.png" alt="The operator page, with four operator cards.">
<figcaption>The operator page. The four cards carry the oscillator, the level, the feedback and the amplitude envelope. The init patch uses all four as carriers.</figcaption>
</figure>

<div class="patch-card">
<h3>Init, four sines</h3>
<table>
<thead><tr><th>Operator</th><th>Ratio</th><th>Level</th><th>Waveform</th></tr></thead>
<tbody>
<tr><td>1</td><td>1.00</td><td>75</td><td>sine</td></tr>
<tr><td>2</td><td>1.00</td><td>85</td><td>sine</td></tr>
<tr><td>3</td><td>3.00</td><td>70</td><td>sine</td></tr>
<tr><td>4</td><td>2.00</td><td>85</td><td>sine</td></tr>
</tbody>
</table>
<p>The filter is off and the amplitude envelopes are at their defaults.</p>
</div>

## One operator

Set the level of operators 2, 3 and 4 to 0. The chord becomes a single sine at
the note. This is the cleanest starting point for a new patch, because every
addition after it is audible.

Raise operator 1's level to 99 and listen to the level control do its job. The
level is not a volume knob in the mixer sense; it is the operator's output level
before the carrier sum, so it changes the timbre when the operator is a modulator.

## Add a fifth

Set operator 4's level back to 85 and its ratio to 1.50. The sine at the fifth
joins the note. A ratio of 1.50 is the perfect fifth above the note, and the two
operators together sound like an interval rather than a chord.

Change operator 3's ratio to 2.00 and its level to 60. The octave thickens the
top without adding a new pitch class. On a carrier algorithm, the ratio is a
pitch control. In the next chapter, the same ratio becomes a timbre control when
the operator modulates another one.

<div class="patch-card">
<h3>A fifth</h3>
<table>
<thead><tr><th>Operator</th><th>Ratio</th><th>Level</th><th>Waveform</th></tr></thead>
<tbody>
<tr><td>1</td><td>1.00</td><td>75</td><td>sine</td></tr>
<tr><td>2</td><td>1.00</td><td>0</td><td>sine</td></tr>
<tr><td>3</td><td>2.00</td><td>60</td><td>sine</td></tr>
<tr><td>4</td><td>1.50</td><td>85</td><td>sine</td></tr>
</tbody>
</table>
<p>All four operators are carriers. The filter is off.</p>
</div>

## Change the waveform

Change operator 1's waveform to **triangle**. The sine's round top gains a few
odd harmonics and the tone softens. Try **rect sine** and **half sine** next. The
thirteen tables cover the OPL3 and OPZ families, plus `rect tri`, and the last
entry in the list is the additive spectrum mode, which the spectral chapters
cover.

The waveform control is a block-rate parameter, so it changes at the next control
block rather than at the next sample. That is fast enough to feel immediate and
slow enough that the engine never allocates in the audio callback.

## Shape the envelope

Open the **operator envs** page and find operator 1's amplitude envelope. The card
carries the graph, the warm-up and surge rates, decay 1 and decay 2, the release,
and the mode.

The init patch has the warm-up off, the surge at 80, decay 1 at 60 to a level of
93, decay 2 at 50 to a level of 87, and the mode at sustain on. Set decay 1's
level to 50 and its rate to 40. The note now falls to a lower sustain and the
sound becomes plucked rather than sustained. Set the mode to sustain off and the
same envelope becomes a one-shot. The four modes are sustain off, sustain on,
loop 1 and loop 2; the loop modes repeat the two decay stages.

The graph is not decoration. The warm-up is convex, the surge is concave, and the
decays and release are straight in the display. The shape tells you which stage
you are editing.

<figure>
<img src="figures/amp-envelope.svg" alt="A schematic amplitude envelope with the warm-up off, a surge, two decays, a sustain and a release.">
<figcaption>The amplitude envelope's stages. The warm-up is off in this patch, so the surge is the first rise. The double bars mark the start and end of the sustain.</figcaption>
</figure>

## Let the filter in

Open the **part fx/mod** page and switch the filter on. The cutoff sits at about
909 Hz, the resonance at 38 percent, and the filter is after the four operators.
Lower the cutoff and the top of the sound closes. Raise the resonance and the
corner becomes a peak.

The resonance self-oscillates above about 79 percent. The output gate follows the
operators, so the loop only sounds while they do; with the levels at zero, the
filter is silent. That is the behaviour you want from a filter after an
amplitude envelope, and it is why a held note with a high resonance does not leave
a tone behind.

## What you have

You have made a sound from four carriers, changed its interval, changed a
waveform, shaped an amplitude envelope and moved a filter. The next chapter uses
the same four operators as modulators and explains why the ratio changes from a
pitch to a timbre.
