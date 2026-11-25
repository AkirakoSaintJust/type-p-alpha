# JUCE mapping

This maps the p1onken interface language to a JUCE editor. The visual reference is
`design/mockup.html`, the source values are `tokens.json`, and the
product rules are in `README.md`.

Scope: the Type P.alpha plugin host and its generated project. The targets are
VST3, CLAP and the standalone app on Linux, Windows and macOS.

Status: Milestone 3 implements this mapping. `plugin/tools/gen_tokens.py` emits
`UiTokens.h`, the editor is `plugin/juce/PluginEditor.*` and `plugin/ui/*`, and
`plugin_editor_snapshot` renders the pages and dialogs for review.

I wrote this mapping because the browser mockup and the JUCE editor are two
different renderers, and the only way I've found to keep them honest is to make the
tokens the single place where a number changes. The rules below are the ones that
keep the editor from drifting into a second design.

## Token source

Retype a colour, a size or a metric in C++ and it will drift. Generate a
`UiTokens.h` from `tokens.json` at configure time, or commit the generated header
and check it in a test. Every value in the editor must come from that header.

Suggested shape:

```cpp
namespace p1::tokens {
inline constexpr auto bg = 0xff000000;
inline constexpr auto pink = 0xffff2e7e;
inline constexpr float baseText = 16.0f;
inline constexpr int windowWidth = 1240;
}
```

The generator is part of the plugin host. It keeps the design tokens and the JUCE
header from drifting.

## Colour

Map each hex token to `juce::Colour` with an opaque alpha, and draw fills with
`fillRect` and `fillPath`. Keep gradients, rounded rectangles, blurs and glow out of
the editor.

- Window ground `bg`.
- Section fills `panel`.
- Dialogs `popup`.
- Structural lines `lineStrong`.
- Hairlines `line`.
- Primary values `pinkBright`.
- Carriers, active tab and unmuted tiles `pink`.
- Selection and modulation `purple`.
- Signal, enabled state and active part `green`.
- Body text `text`, labels `dim`, hints `faint`.

Colour is the easiest place to cheat, so I keep the list short and forbid the
extras. A gradient or a rounded corner looks fine in a mockup and then costs me an
afternoon in JUCE, and the flat black is the identity anyway.

## Fonts

Bundle DotGothic16 and Zen Dots as binary resources, since both are SIL Open Font
License. Load neither of them from the network in the plugin.

- Zen Dots: the wordmark, section titles, tabs, operator and part names, dialog
  titles, toolbar labels, status and the algorithm number.
- DotGothic16: everything else, including values, labels, buttons and notes.
- Digital Desolation Plus: not used in the interface and not bundled. The
  wordmark is all Zen Dots.

JUCE takes a font height in logical pixels, so use the CSS pixel sizes from
`tokens.json` directly at 1x. Keep the whole editor on one uniform scale factor
instead of scaling individual fonts and strokes.

JUCE draws both families about 4px smaller than the browser at the same nominal
height, so `plugin/ui/Theme.cpp` adds a 4px optical correction to every generated
size. Keep `tokens.json` as the design value and apply the correction in the
mapping, because editing the tokens would pull the scale away from the mockup.

Fonts are the other place where the mockup and the plugin disagree, because the
browser and JUCE rasterise the same family at different optical sizes. The 4px
correction is a fudge, but it's a measured fudge, and I'd rather keep it in one
mapping file than scatter it through the pages.

Check the Japanese glyph coverage for the brand line
`p1onken // パンク派第１音響合成研究班` and for the 猫 legend glyph. DotGothic16
covers both.

## Layout

- Design at 1240 logical pixels wide.
- Height is fixed at 985 logical pixels and shared by every page, so a page change
  never resizes the editor.
- Apply one uniform scale to the whole 1240 wide layout when the host gives a
  different editor size, and never reflow the design.
- Use integer rectangles and align 1px strokes to the pixel grid.
- Page padding is 14 top, 16 sides and 16 bottom.
- Grid gap is 10. Card padding is 9. Operator columns add 8 padding and a 6 gap.
- Put card actions at the left of a header and keep a 16px gap before a mute
  switch. Put a mode's knobs next to its buttons with a 32px gap.
- At startup, size the 1240 by 985 canvas to the display's `userArea`, which
  already excludes the taskbar or dock, and subtract the chrome the window actually
  adds. Measure the chrome as the difference between the peer bounds and the editor
  bounds, so the system title bar and any host menu are counted rather than guessed.
  Use the largest scale that fits, up to the 2480 by 1970 resize limit, and keep
  applying the fit for a moment because a host can set its own size after the editor
  attaches.

Every page fills the same height:

- patch settings: two part cards, each with seven knobs, a level slider, a pan
  slider and a scope and spectro, which grow to fill the remaining card height,
  followed by the voice mode block.
- part params: a three by two grid. `filter`, the filter envelope and the pitch LFO
  fill the first row. Pitch envelope, key follow and velocity fill the second
  row. Every card header carries a `to 1` or `to 2` action that copies that card to
  the other part. The two follow cards are separate, and each has its own on switch,
  `wipe all` button and copy action. The pitch LFO carries its `bipolar` switch beside the wave select.
- op envs: four columns with oscillator, a 214 px `amp env` and a 222 px
  `fb env`, separated by 6 px gaps. The envelope cards put 2 px between their
  graph, rate and level rows. The amplitude envelope puts `atk warmup`, `atk surge`, decay1 and decay2 on the
  first row, the two decay levels and the release rate on the second, and the mode
  dropdown below. The feedback envelope uses R1 to R4 and L1 to L4, with signed
  levels, a mode dropdown and an on switch in the card header. The signed sum of
  the feedback knob, the envelope and the LFO picks the feedback family, so a
  negative level can cross zero from any knob setting. The two envelope cards
  carry copy and paste in their headers, and their graphs mark the sustain and loop
  boundaries with a dotted pink vertical line, a 2 px dash and a 2 px gap. The amp
  env and fb env mode selects are 150 px wide and 18 px tall, the same width as the
  wave select and the same height as the spectrum header selects.
- op lfos: four operator columns, a 240 px scope and spectro pair, then the
  113 px `amp lfo`, `fb lfo` and `detune lfo` cards with 12 px gaps. The
  oscillator stays on the op envs page. Each LFO card carries copy and paste
  in its header, puts rate, depth, `rate fade` and `depth fade` in one row, and
  keeps the wave select and `bipolar` switch below.
- op phasemap: four 294 px operator columns, each with the header, a 60 px
  oscillator strip, an 88 px live map, a 155 px `phase` card, a 106 px `phase lfo`
  card and a 206 px `phase env` card, separated by 8 px gaps. The page total is
  669 px. The oscillator strip carries `ratio`, `detune`, `level` and `feedback`
  as 22 px dials with the wave select beside them, and uses the operator copy,
  paste and mute in the header. The phase LFO uses the compact 22 px row. The
  phase card puts 4 px between its two knob rows and the phase envelope puts
  2 px between its rows. The map is a read-only
  `P1PhaseMap` fed by the 30 Hz `PhaseMapSnapshot`. It draws the composed segments,
  the locks, the order jumps, the faint `F`, the breakpoint and the phase dot.
  When `on` is off, the map draws the neutral parameter geometry and the live
  dot disappears.
  `phase` card has nine knobs in two rows: `breakpoint x`, `segments`, `flip` and `flip rate`, then `reorder`, `reorder rate`, `reorder jump`, `drift` and `drift rate`. The second row uses the compact 10 px label. The LFO card puts rate, depth, `rate
  fade` and `depth fade` in one row, and the envelope card is the signed purple
  graph with its rates, levels and mode dropdown.
- op spectra a: four operator and oscillator cards across the top, a 6 px empty
  band, then four spectrum cards of 124 px. Each card header carries `range`,
  `mask`, `selection`, `group`, `load spectrum template...`, `norm`, `to b`, copy
  and paste. The body holds the 64-bar editor and the group legend on the left and
  the `tilt`, `level` and `formant` springs on the right, with the formant picker
  beside the formant lever. The four cards fit under the strip without a scroll.
- op spectra b: the same page for spectrum b, with `to a` in place of `to b`. Each
  card carries its own range, its own 64 levels and its own template button.
- op spectra mod: four operator columns with 10 px gaps under the operator header,
  the oscillator card, a 130 px `spectra` display, the `morph env` card and the
  106 px `morph lfo` card. The display draws the stored a and b. The marker and the faint
  waveform follow the live morph of the newest rendering voice, so they advance
  with the envelope and the LFO while a note sounds. The display takes no drag.
  When a and b use different ranges, the display zooms out to the union of the two
  spans. The envelope card carries the graph, the on switch, R1 to R3, L1 to L3,
  the release rate and level, and the 96 by 18 mode dropdown. The morph LFO card
  puts rate, depth, `rate fade` and `depth fade` in one row of 22 px dials, with the
  wave select and `bipolar` switch below.

The current layout has nine tabs: patch settings at 0, macros at 1, part params at
2, op envs at 3, op lfos at 4, op phasemap at 5, op spectra a at 6, op spectra b at
7 and op spectra mod at 8. The page order changes with each layout addition, so
the processor remaps the stored `uiPage` when an old state restores. A four-tab
state maps 1 to 2, 2 to 3 and 3 to 1. A six-tab state maps the mod page from 5 to
6. A seven-tab state maps 4 to 5, 5 to 6 and 6 to 7. Layout 5 inserts `macros` at
1 and moves every later page up one. The phase-map insertion layout is 4. The
`uiPageLayout` property is now 5.

## Header

- Draw `TYPE P.` in Zen Dots at the 30px wordmark size, with the dot in pink, then
  a lowercase `a` in Unbounded at the same height, with a slightly heavier weight
  than the regular face. Keep the solid `brandShadow` offset behind the whole
  wordmark.
- The algorithm block is a button. Draw the 30px number, the caret and the 240 by
  128 graph. Centre the graph content in its viewBox so the header and the picker
  tiles share one optical centre.
- Clicking the algorithm block opens the picker, whilst the mouse wheel over the
  block cycles the active part's algorithm and must not open the picker.
- In the picker, reserve the caption line below each graph. Centre the graph in the
  remaining figure area so it cannot clip or touch `alg n`.
- The multi-patch block sits above its buttons: `loaded multi-patch:` in Zen Dots,
  the multi-patch name in DotGothic16 below it, then the
  `multi-patch library...` control and the `rename` button. The library popup
  carries export, import and init.
- The output section carries a `volume` slider under the scope. The caption
  `volume` sits left and the value sits right in `pinkBright`, and the 330 by 38
  control fills the same column as the plot. The track is purple from the left to
  the fader position, with a `dim` unity mark at 75 percent. The fader position is
  the parameter, so the fill is linear in position whilst the gain follows the law
  in `VolumeLaw.h`: silent at 0, a logarithmic rise to 0 dB at 75 percent, then a
  quadratic rise to +12 dB at the top. The value reads in dB, and `silent` at the
  floor.

## Shape and borders

- Radius is 0. Use `fillRect` and `drawRect` rather than rounded helpers.
- Surfaces have no border. Only controls, selected states and the structural frame
  carry a 1px stroke.
- The plugin frame is 1px `lineStrong`, with the 11 by 11 pink corner brackets at
  the top left and bottom right in a 2px stroke.
- The active tab is a `juce::Path` built from the token polygon, filled pink with
  the text drawn in black, and the inactive tabs have no dividers.
- The first tab keeps the token polygon. Every later tab shifts its left edge
  10px left so the active tail overlaps the tab before it. Set the tab to paint
  unclipped, or the tail is cut at the component edge.
- Focus rings are 1px. Green for text entry, pink for selects.

## Controls

Knobs are custom painted, with a 270 degree track, a pink arc from the start to the
value, a white pointer, a name below and a value below that, and you click the value
to type a number, where Enter commits, Escape cancels, and the value is clamped and
stepped.

A toggle reads its current state as `on` or `off` beside the track, with the control
name when it needs one (`bipolar on`). The mouse wheel moves 2 percent of the range
per notch, and shift moves 0.5 percent, because the default 15 percent is too coarse
for a level or a rate.

The knob and the toggle are the two controls I use everywhere, so they carry most of
the interaction rules. If a new control needs a third behaviour, I'd rather make it
a button than teach the knob another trick.

Selects are 18 px tall wherever the row allows it, which is the spectrum card header
height, and a select that shares a row with a switch keeps the switch on the same
line. The wave select and the envelope mode selects are 150 px wide.

Context sizes:

- default 52 cell, 42 dial
- operator envelope 34 cell, 28 dial
- operator oscillator and LFO 50 cell, 28 dial
- operator feedback envelope 34 cell, 28 dial
- part settings 62 cell, 38 dial
- pitch envelope 52 cell, 38 dial
- key follow and velocity 64 cell, 42 dial

Mute controls are tiles, so active is a solid pink tile with a black label such as
`op1` or `p1`, and muted is a black tile with a pink outline and a pink label, whilst
the 猫 squares in the toolbar legend are illustrative only, and part and operator
mute switches use the inline form `active [track] mute`, with the word for the
current state in pink and the other word faint.

The level slider is a 14px track with a purple fill and a `level` caption.

A spectrum card draws 64 columns across its left side and writes the level under
the pointer, then follows a horizontal drag. Only a bar inside the selection window
and passing the live or dead mask is written, so the window narrows both the freehand
editor and the springs. The formant ignores the group and the mask, whilst staying
inside the window. Bars outside the window are dimmed. The bar labels show every
eighth harmonic, and the members of the selected group are green. A spring control
is a 10px track: the drag previews one curve from the 64 levels captured at pointer
down, written to the members the group and mask select, and the release leaves the
result in place, so repeated gestures never compound the same curve. Tilt up is the
reverse tilt, with the highs louder and the lows quieter, and tilt down is the
normal tilt, with the lows louder and the highs quieter. The
`load spectrum template...` button opens the fourteen wave names and writes that
analysis into the row without changing the waveform.

The spectrum card is the most complicated control in the panel, and the selection
window is the part that keeps it usable. Without the window, a drag on a 64-bar row
is a recipe for a smeared spectrum, and the springs would fight the freehand editor.
The window is the shared boundary between the two, so I keep it visible even when the
mask hides most of the bars.

The envelope graph weights each stage by the square of its distance from rate 99,
so a rate of 99 has no horizontal run and reads as vertical whatever its target
level, whilst the usual settings still fill the graph.

The wave select lists the fourteen names in `waveforms.md` in stored order.
`rect tri` is value 9, `pulse`, `half square`, `square` and `saw` are 10 to 13,
and `spectrum` is 14. The `wave` display reads the stored value.

List dialogs fill the dialog body with 24px slot rows. Use a thin scrollbar, and
keep names at the preset record limit of 32 characters. The multi-patch library
carries `save` and `load` beside the scroller, and the single parts library carries
a save and a load button for each part, so the slot is chosen before the action.

## Visualisers

- Scope: green trace on black, no frame.
- Spectro: pink bars with a purple band every fifth band.
- Amplitude and pitch envelopes: purple.
- Filter envelope and pitch LFO: green.
- Two-spectrum display: a purple filled outline for spectrum a, a pink outline for
  spectrum b, a green marker for the morph value and a faint green waveform trace.
  The marker and the trace follow the live morph of the newest rendering voice
  rather than the base parameter.
- Routing graph: white modulators, pink carriers, grey modulation lines, purple
  downward arrows under every carrier.
- Phase map: green forward segments, purple flipped segments at full opacity,
  locked segments at 35%, a pink dashed vertical at an order jump, a faint `line`
  polyline for `F`, the pink breakpoint vertical and a white dot at the newest
  voice's phase. The caption is `bp 0.500 · π`, the active and requested segment
  counts, and the locked count. It is a read-only view fed by `PhaseMapSnapshot`.

Use a `juce::Timer` at about 30 Hz for repaint, fed from a lock-free ring buffer
written by the audio thread, with no allocation, locks or file access on the audio
thread, and the visualisers are optional, so they must degrade to a flat line when
the host disables them.

The timer is change-gated. Each phase-map view hashes the snapshot it would draw.
The envelope views hash their rates, levels, mode and start values. The scope and
spectrogram setters compare the incoming buffer. The wave preview and spectrum
display hash their parameters. A static or silent widget skips the repaint, and a
changing phase map still repaints at most at the 30 Hz timer rate, never at the
220 Hz stochastic event rate. `p1_ui_frame_bench` records the current frame cost:
scope ~190-200 µs, spectrogram ~17 µs, full phase page ~11.5 ms and spectra page
~4.1 ms. The full-page cost is knob and text rasterisation, not a vector loop.
The modern AVX2 build is not faster there. A whole-page image cache is not used
because every knob drag or live update would invalidate it.

## Parameters

`synth/patch.json` is the only reference. The plugin exposes the same
parameter set and the same ranges, grouped to match the pages rather than the JSON
order.

- patch settings: parts, keyzones, levels, delays, voice mode.
- part params: filter, filter envelope, pitch LFO, pitch envelope, pan envelope,
  key and velocity, in a four by two grid.
- op envs: four sets of oscillator, amplitude envelope, feedback envelope and
  the three LFO parameters.
- op phasemap: per operator, the phase map's nine knobs and `on`, the phase LFO's
  six values and the signed envelope's ten values. The page is page 5, after
  `op lfos`, and the map view reads the per-part snapshot rather than the
  parameters.
- op spectra a and op spectra b: per operator, the 64 a levels, the 64 b levels,
  the two ranges, the morph value, the morph envelope and the morph LFO. The
  waveform parameter grows to 0 to 14, with `spectrum` at 14.
- global: algorithm and patch name.

Use stable parameter IDs derived from the manifest. Host automation, state save and
restore must all agree.

The parameter list is generated from the manifest, so the editor does not have a
second list to maintain. That is the only reason I can add a control on one page and
trust the host automation to find it on the next build.

## Audio

The engine core runs at 48 kHz, so the plugin runs the same core and resamples the
output to the host rate. At 48 kHz the processor bypasses the bridge, so the core
stays exact there, and the 128-tap kernel covers 44.1, 96 and 192 kHz hosts. Leave
the input and the core rate untouched.

## Licensing

- Plugin code: GPL-3.0-only, the same licence as my personal framework tooling.
- Fonts: DotGothic16 and Zen Dots under the SIL Open Font License. Include the
  licence text with the binaries.
- Brand font: not bundled. The wordmark uses Zen Dots with an Unbounded lowercase
  `a`, and Digital Desolation Plus stays out of the binary.
- JUCE: use the AGPLv3 path or a commercial JUCE licence, and record the JUCE
  version in the build manifest, because JUCE 9 is not GPLv3, and the source
  stays GPL-3.0-only, where GPLv3 section 13 permits the combined binary.

## Open items

- Brand font licence: no longer a blocker. The wordmark is all Zen Dots, and
  Digital Desolation Plus is not used or bundled.
- macOS DAW matrix: needs a Mac with two hosts before it can run.
- In-plugin 64 multi and 128 part banks: in. SysEx import is not part
  of this product.
