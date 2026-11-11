# JUCE mapping

This maps the p1onken interface language to a JUCE editor. The visual reference is
`design/mockup.html`, the source values are `tokens.json`, and the
product rules are in `README.md`.

Scope: the Type P.alpha plugin host and its generated project. The first targets
are VST3 and CLAP on Linux and Windows, and AU is a later macOS addition.

Status: Milestone 3 implements this mapping. `plugin/tools/gen_tokens.py` emits
`UiTokens.h`, the editor is `plugin/juce/PluginEditor.*` and `plugin/ui/*`, and
`plugin_editor_snapshot` renders the pages and dialogs for review.

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
- At startup, fit the 1240 by 985 canvas to the display work area less a band for
  the host chrome, then keep applying the cap for a moment because a host can set
  its own size after the editor attaches. A constructor-only cap is not enough,
  and a maximised host window is smaller than the display work area.

Every page fills the same height:

- patch settings: two part cards, each with five knobs, a level slider and a
  scope and spectro, which grow to fill the remaining card height, followed by the
  voice mode block.
- operators: four columns with oscillator, envelope and amplitude LFO, where the
  envelope uses one row for R1 to R4 and a second row for L1 to L3.
- fx / mod: a three by two grid. Filter 1, the filter envelope and the pitch LFO
  fill the first row. Pitch envelope, key follow and velocity follow fill the second
  row. The two follow cards are separate, and each has its own on switch and wipe
  button.

## Header

- Draw `TYPE P.` in Zen Dots at the 30px wordmark size, with the dot in pink, then
  a lowercase `a` in Unbounded Regular at the same height. Keep the solid
  `brandShadow` offset behind the whole wordmark.
- The algorithm block is a button. Draw the 30px number, the caret and the 240 by
  128 graph. Centre the graph content in its viewBox so the header and the picker
  tiles share one optical centre.
- Clicking the algorithm block opens the picker, whilst the mouse wheel over the
  block cycles the active part's algorithm and must not open the picker.
- In the picker, reserve the caption line below each graph. Centre the graph in the
  remaining figure area so it cannot clip or touch `alg n`.
- The patch control shows `multi` and the current multi-patch name, with the hint
  `click above for multi-patch library actions`. The library popup carries export,
  import and init.

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

Context sizes:

- default 52 cell, 42 dial
- operator envelope 37 cell, 34 dial
- operator oscillator and amplitude LFO 56 cell, 34 dial
- part settings 62 cell, 38 dial
- pitch envelope 52 cell, 38 dial
- key follow and velocity follow 64 cell, 42 dial

Mute controls are tiles, so active is a solid pink tile with a black label such as
`op1` or `p1`, and muted is a black tile with a pink outline and a pink label, whilst
the 猫 squares in the toolbar legend are illustrative only, and part and operator
mute switches use the inline form `active [track] mute`, with the word for the
current state in pink and the other word faint.

The level slider is a 14px track with a purple fill and a `level` caption.

List dialogs scroll, with patch browser rows at 260px maximum and part slots at
240px maximum. Use a thin scrollbar, and keep names at the preset record limit of
15 characters.

## Visualisers

- Scope: green trace on black, no frame.
- Spectro: pink bars with a purple band every fifth band.
- Amplitude and pitch envelopes: purple.
- Filter envelope and pitch LFO: green.
- Routing graph: white modulators, pink carriers, grey modulation lines, purple
  downward arrows under every carrier.

Use a `juce::Timer` at about 30 Hz for repaint, fed from a lock-free ring buffer
written by the audio thread, with no allocation, locks or file access on the audio
thread, and the visualisers are optional, so they must degrade to a flat line when
the host disables them.

## Parameters

`synth/patch.json` is the only reference. The plugin exposes the same
parameter set and the same ranges, grouped to match the pages rather than the JSON
order.

- patch settings: parts, keyzones, levels, delays, voice mode.
- operators: four sets of oscillator, envelope and amplitude LFO parameters.
- fx / mod: filter, filter envelope, pitch LFO, pitch envelope, key and velocity
  follow.
- global: algorithm and patch name.

Use stable parameter IDs derived from the manifest. Host automation, state save and
restore, and the web lab must all agree.

## Audio

The engine is locked at 44.1 kHz, so the plugin runs the same core and resamples the
output to the host rate. Leave the input and the core rate untouched.

## Licensing

- Plugin code: GPL-3.0-only, the same licence as my personal framework tooling.
- Fonts: DotGothic16 and Zen Dots under the SIL Open Font License. Include the
  licence text with the binaries.
- Brand font: not bundled until the licence is verified.
- JUCE: use the AGPLv3 path or a commercial JUCE licence, and record the JUCE
  version in the build manifest, because JUCE 9 is not GPLv3, and the source
  stays GPL-3.0-only, where GPLv3 section 13 permits the combined binary.

## Open items

- The brand font licence is no longer a blocker. The wordmark is all Zen Dots, and
  Digital Desolation Plus is not used or bundled.
- Decide whether AU joins VST3 and CLAP in the first macOS build.
- The in-plugin 64 multi and 128 part banks are in. SysEx import is not part
  of this product.
