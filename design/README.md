# p1onken interface language

The shared visual language for the Type P.alpha editor. The clickable mockup at
`design/mockup.html` carries the first-pass layout, and `tokens.json` is the
machine-readable source. The CSS projection is `tokens.css`, the platform notes are
in `juce.md`, the spectral control map is in `spectral.md`, the filter law is in
`filter.md`, and the waveform list and analysis are in `waveforms.md`. The shipped panel is the JUCE editor, so where the mockup and the editor differ,
the editor and `spectral.md` carry the current layout.

Status: v1.7 draft, 2026-11-19. v1.6 added the spectrum template menus, the
multi-patch header block, the named part buttons, the centred tilt and the
vertical fast envelope stages. v1.7 adds the SH-101 filter law and the single
spectrum card with its header selectors.

I keep this file because the panel is the part of the project I can see, and a
visual language that only exists in my head is a visual language I break the moment
I add a page. The rules below are the ones I keep forgetting, so they're written
down before they become a debate with myself at two in the morning. The mockup is
the sketch, `tokens.json` is the source the code reads, and the shipped panel is the
one that counts when they disagree.

## Principles

1. Black environment. Surfaces do not need outlines.
2. Pink is identity and the primary value, green is live signal and active state,
   and purple is selection and modulation. No other hues.
3. Controls are outlined and content surfaces are not, so if it cannot be clicked
   or typed into, it probably should not have a border.
4. Square corners, flat fills, no gradients, no blur, no glow, and the only shadow
   is the solid offset behind the wordmark.
5. Type is lowercase and pixel-sharp, with Zen Dots for headers and DotGothic16 for
   everything else. The wordmark is `TYPE P.` in Zen Dots with a pink dot, followed
   by a lowercase `a` in Unbounded Regular, and user-entered names keep their case.
6. Every label earns its place, so there is no filler text such as voice counts,
   operator counts, or a `what this is` section. The page shows the product, and the
   words go on the next action, the controls, or a real limit.
7. One algorithm per node, one meaning per colour, one home for every token.

## References

The visual language draws on these. Some shape a decision directly, and some are
typefaces I admire for the spirit they carry:

- A Novarese's Eurostile and Stop for the squared, technical letterforms.
- Openmind's work for Amon Tobin's Foley Room for the cut-up, layered surfaces.
- R Mitchell's Corporate for the restrained grid.
- TDR's Wipeout and Autechre covers for the high-contrast, print-like marks.
- N2-Antireal's Terminal for the terminal density.
- Maniackers, Nakamura Y and Hashimoto K's Gona for the compact, coded labels.
- VDL's Line and Giga for the thin rules and modular blocks.
- R Larabie's Rexlia for the wide, industrial display type.

The panel carries the language today, since `tokens.json` and the snapshots keep it
honest. The front door carries the short list, and this page carries what each
reference contributes.

The wordmark, the manual, the posters and the web graphics are the next material to
design. They draw on the same references, and the panel is where each one appears
first.

## Colour

| Token | Hex | Role |
| --- | --- | --- |
| bg | `#000000` | window and page ground |
| panel | `#070707` | section and operator fill |
| popup | `#1b1b1b` | dialogs, popovers |
| line | `#1c1c1c` | hairlines inside dark surfaces |
| lineStrong | `#2e2e2e` | structural dividers, control borders |
| text | `#d2d2d2` | body text |
| dim | `#7d7d7d` | labels, secondary text |
| faint | `#4a4a4a` | hints, captions, inactive edges |
| white | `#ffffff` | dialog text, modulator blocks, high-contrast marks |
| pink | `#ff2e7e` | brand dot, carriers, primary values, active tab, unmuted tile |
| pinkBright | `#ff4d94` | knob values, hover on pink |
| purple | `#c780ff` | selection, modulation, pitch envelope, level fill, active scope mode |
| green | `#3dff9e` | live signal, enabled state, active part, output label |
| brandInk | `#d6d6d6` | wordmark |
| brandShadow | `#c2ad00` | solid wordmark shadow |

The palette is small on purpose. Pink carries the identity and the value I want you
to read first, green carries anything live or enabled, and purple carries the
selected or modulating thing. When a new state needs a colour, I'd rather reuse one
of those three than add a fourth, because the moment a colour means two things the
panel starts to look like a dashboard.

Rules:

- Pink is identity and primary emphasis. It is not a success colour.
- Green means signal or an active path. It is not decoration.
- Purple marks the selected or the modulating thing, whilst filter envelope and pitch
  LFO curves are green and amplitude and pitch envelope curves are purple.
- Retire `panel2`, `green2`, `select` and `yellow` from new code, since they were
  exploratory values and are not part of v1.

## Type

Families:

| Family | Use | Licence |
| --- | --- | --- |
| Zen Dots | wordmark, section titles, tabs, operator and part names, dialog titles, toolbar labels, status | SIL Open Font License |
| DotGothic16 | all other interface text, values, labels, buttons, notes | SIL Open Font License |
| Digital Desolation Plus | reserved, unused | not bundled, not needed |

The mockup loads both OFL families from Google Fonts after first paint, whilst the
JUCE build bundles the OFL files as binary resources. The wordmark is all Zen Dots, and Digital Desolation
Plus is not used in the interface, so it stays out of the binary.

The type is the other half of the identity, and it's deliberately cheap to render.
Zen Dots does the shouting and DotGothic16 does the reading, whilst Unbounded
appears only in the wordmark. I'd keep it that way, because a third family would
give me one more thing to license and one more way to make a label look important
when it isn't.

### Wordmark

The shipped wordmark is `TYPE P.` in Zen Dots with a pink dot and an Unbounded
lowercase `a`. Both families are OFL, so they can travel in the binary and the
mockup, and they fit the spirit. The compromise is visible: the `a` is not a Greek
alpha and does not read as one, because I could not find an OFL family with the
Greek alphabet in the shape I wanted. Zen Dots also has caps that read odd rather
than futuristic or techno to me.

The panel token is 30px for the mark, with the solid 4px shadow and the 12px brand
line below it. The shadow and the caps make it read larger than the number, which is
the optical size I want in the header.

The next wordmark is an experiment rather than a decided design. The spirit is the
Foley Room-era Amon Tobin mark, and a cut-up and layered Rexlia is one candidate.
Rexlia is the contemporary industrial display in the list, which is the quality I
want the custom mark to keep. The alpha can be built from existing font components,
because most free families have no Greek glyphs. The mark will probably work better
as vector art than as an embedded font: the alpha is easier to draw, and the
licensing is simpler. Eurostile, Stop, Corporate and VDL's Line and Giga stay
references for the character, not models to trace. The wordmark is the first
material to design, and the manual, the posters and the web graphics follow it.

Scale:

| Role | Family | Size | Case |
| --- | --- | --- | --- |
| base | DotGothic16 | 16 | lower |
| knob name | DotGothic16 | 13 | lower |
| knob value | DotGothic16 | 16 | lower |
| inline label | DotGothic16 | 13 | lower |
| button | DotGothic16 | 12 | lower |
| button key hint | DotGothic16 | 11 | lower |
| mini button | DotGothic16 | 12 | lower |
| legend word | DotGothic16 | 11 | lower |
| switch state | DotGothic16 | 11 | lower |
| legend glyph | DotGothic16 | 12 | lower |
| mute tile label | DotGothic16 | 13 | lower |
| note | DotGothic16 | 13 | lower |
| dialog body | DotGothic16 | 14 | lower |
| slot row | DotGothic16 | 15 | lower |
| select text | DotGothic16 | 12 | lower |
| brand line | DotGothic16 | 12 | lower |
| section title | Zen Dots | 11 | lower |
| tab | Zen Dots | 12 | lower |
| operator name | Zen Dots | 14 | lower |
| part name | Zen Dots | 12 | lower |
| dialog title | Zen Dots | 11 | lower |
| toolbar label | DotGothic16 | 9 | lower |
| status | Zen Dots | 11 | lower |
| algorithm label | Zen Dots | 10 | lower |
| algorithm number | Zen Dots | 30 | lower |
| wordmark | Zen Dots + Unbounded | 30 | `TYPE P.` + `a` |

Tracking is tight, between 0.01em and 0.06em, and section titles carry a `//` prefix
in `faint`. The wordmark is `TYPE P.` in Zen Dots at 30px, with a pink dot after the
`P`, followed by a lowercase `a` in Unbounded Regular at the same height. It keeps
the solid shadow. Both faces use their regular weight. The select text at 9px is
the one survivor from the old scale. Review it when the real control is rebuilt.

The JUCE projection adds 4px to every size in the table. DotGothic16 and Zen Dots
render smaller in JUCE than in the browser at the same nominal height, so the token
scale stays the design value and the mapping applies the optical correction in
`plugin/ui/Theme.cpp`. Do not edit the token sizes to compensate, because that
would pull the token scale away from the mockup.

## Metrics

| Item | Value |
| --- | --- |
| window width | 1240 |
| window height | 985 |
| window height note | fixed across pages, set by the operator page |
| page padding | 14 top, 16 sides, 16 bottom |
| grid gap | 10 |
| card padding | 9 |
| operator column padding | 8 |
| operator card padding | 8 |
| header padding | 14 by 16 |
| toolbar padding | 7 by 16 |
| tab padding | 11 by 16 |
| status padding | 7 by 16 |

Knob cells and dials:

| Context | Cell | Dial |
| --- | --- | --- |
| default | 52 | 42 |
| operator envelope | 37 | 34 |
| operator oscillator and LFO | 56 | 28 |
| operator LFO fade pair | 140 | 28 |
| spectral LFO fade pair | 140 | 22 |
| part settings | 62 | 38 |
| pitch envelope | 52 | 38 |
| key follow and velocity follow | 64 | 42 |

The window is fixed because the pages are a grid, and a grid that reflows is a grid
I have to test twice. The operator page sets the height, the other pages fit inside
it, and the editor sizes to the display's user area less the measured window chrome.
If a page needs more room, the answer is a tighter card rather than a taller
window.

Borders:

| Item | Rule |
| --- | --- |
| plugin frame | 1px `lineStrong` |
| structural dividers | 1px `lineStrong` |
| surfaces and cards | none |
| buttons, toggles, selects, inputs, sliders | 1px `lineStrong` |
| active part | 1px `green` |
| selected slot or row | 1px `purple` |
| focus | 1px `green` for text inputs, 1px `pink` for selects |
| accent corners | 11 by 11, 2px `pink`, top left and bottom right of the window |

Shape:

- Radius is always 0.
- The active tab is a cut polygon:
  `polygon(5px 0, calc(100% - 12px) 0, 100% 50%, calc(100% - 7px) 100%, 0 100%, 5px 50%)`.
  The first tab keeps this shape. Every later tab runs its left edge 10px further
  left, so the active strip reaches into the tab before it. Paint the tab
  unclipped, or the tail is cut at the component edge.
- No gradients and no blurred shadows. The wordmark shadow is a solid 4 by 4 offset
  in `brandShadow`.

## Components

### Header

Brand left, algorithm centre-left, output scope right and patch controls far right,
with the wordmark as `TYPE P.` in Zen Dots, the dot in pink, the lowercase `a` in
Unbounded at a slightly heavier weight than the regular face, and the brand line
`p1onken // パンク派第１音響合成研究班` in 12px DotGothic `dim`.

The algorithm block is a button, and it shows a 30px Zen Dots number, a caret, and
the routing graph. The graph is 240 by 128 in the viewBox, scaled to about 244 by
130. The graph content is centred in that viewBox, so the header and the picker
tiles share one optical centre. White operators are modulators, pink operators are
carriers, grey lines are modulation, and purple downward arrows mark carrier
output. Clicking the block opens the algorithm picker, whilst the mouse wheel over
the block cycles the eight algorithms without opening the picker.

The output section is a label plus a `scope` and `spectro` segmented control, with
the active segment in purple. The plot is 50px high, borderless, black.

The multi-patch block sits above its buttons. `loaded multi-patch:` is drawn in
Zen Dots, the current multi-patch name in DotGothic16 below it, then the
`multi-patch library...` control and the separate `rename` button. Clicking the
control opens the multi-patch library.

### Toolbar

Left is the mute legend. A small dark square with a pink outline and pink 猫 means
muted, and a solid pink square with a black 猫 means active. Then the real mute
tiles, `op1` to `op4` and `p1`, `p2`, at 36 by 22. An active tile is solid pink with
a black label, and a muted tile is black with a pink outline and a pink label. The
猫 is only a legend glyph, never a button label.

Right is `now editing part:` with `part 1: name`, `part 2: name` and
`single part library...`, and the selected part button has a green border and green
text. A long part name is truncated with an ellipsis.

### Tabs

`patch settings`, `part fx/mod`, `op envs`, `op lfos`, `op phasemap`,
`op spectra a`, `op spectra b`, `op spectra mod`, with no vertical dividers. The
active tab is a pink cut polygon rather than a rectangle. Inactive tabs are dim text
on black.

### Patch settings

Two part cards, each titled `part n: name` in Zen Dots. The active part has a green
border, and the inactive part has no visible border. Each card holds rename, copy
and paste at the left of the header, a mute switch at the right with a 16px gap,
seven knobs, a wide level slider with the caption `level`, a bipolar `pan` slider
under it, and two borderless visualisers, one scope and one spectro, with no
captions on the visualisers. The
seven knobs are `midi channel`, `keyzone start`, `keyzone end`, `tune`,
`transpose`, `delay` and `bend`, where `tune` is the part detune in cents and
`bend` is the pitch-bend range in semitones. Then a voice mode block with `poly`,
`mono`, `legato`, `unison` and the unison, detune and porta knobs. The three knobs
sit next to the mode buttons with a 32px gap rather than at the card edge.

### Part fx/mod

One page, a four by two grid that fills the window. `filter`, the filter envelope,
the pitch LFO and the pitch envelope fill the first row. Pan envelope, key follow
and velocity follow fill the second row, and the last cell stays empty. The filter
card has cutoff, resonance and env amount, whilst key and velocity tracking sit in
the two follow cards rather than on the filter card. The filter, its envelope and
its two follow targets address the active part, because each part has its own
filter and its own envelope.

The pitch LFO card carries rate, depth, the wave select, the `bipolar` switch,
and the `rate fade` and `depth fade` pair with its shared graph.

The pan envelope card carries an on switch, three rates, three levels, a release
rate and level, a mode dropdown and the envelope graph. Its levels are absolute percentage
points across the field, and the envelope starts at the part's pan. The envelope
retriggers when a note starts on the part and releases when the part's last voice
releases.

Key follow and velocity follow are separate cards. Each has an on switch, a
`wipe all` button and its own target selector. Key follow has a breakpoint and below
and above amounts, whilst velocity follow has one amount. The wipe button sets every
target amount to 0%, so the card can be cleared without visiting each target. The
same target set is used for both cards: operator level, feedback and amp envelope,
pitch envelope, filter cutoff, and filter envelope.

### Operators

Four borderless columns, each with the operator name at the left, copy and paste
after it, and the mute switch at the right with a 16px gap before it, then a scope
and spectro pair, and three cards: `oscillator`, `amp env`, `fb env`. The
oscillator card holds ratio, detune, level, feedback and the wave select. The wave
select lists sine, triangle, half sine, half tri, 2x sine, 2x tri, 2x half sine,
2x half tri, rect sine, rect tri, pulse, square, saw and spectrum. The `amp
env` card holds the `atk warmup`, `atk surge`, decay1 and decay2 rates on the first row, then
the two decay levels and the release rate on the second, with the mode dropdown
below. The `fb env` card holds R1 to R3 and the release rate on the first row, then
L1 to L3 and the release level on the second, with signed levels in feedback units,
a mode dropdown in the card and an `on` switch in the card header. Every envelope's
mode is a dropdown with `sustain off`, `sustain on`, `loop 1` and `loop 2`, and
every envelope graph marks its sustain and loop boundaries with a dotted pink
vertical line, a 2 px dash and a 2 px gap, the same line as the phase map order
jumps. The signed sum of the feedback knob, the envelope and the LFO chooses
the feedback family, so a negative level can cross zero into the negative
family. Every envelope and LFO card carries
`copy` and `paste` in its header, so a shape or an LFO moves between operators
without touching the rest of the operator. The four oscillator knobs spread across
the card. The envelope rates and levels each get their own row.

### Operator LFOs

Four borderless columns, each with the operator name, copy and paste, the mute
switch, a 240 px scope and spectro pair, then three 113 px cards with 12 px gaps:
`amp lfo`, `fb lfo` and `detune lfo`. The oscillator stays on the operators page.
Each LFO card holds rate, depth, `rate fade` and `depth fade` in one row, with the
wave select and the `bipolar` switch below, and carries `copy` and `paste` in its
header.
The amplitude LFO depth is in level steps, the feedback LFO depth is a percentage
of the feedback range, and the detune LFO depth is in semitones. A negative depth
in bipolar mode inverts the LFO phase.

### Operator spectra a and b

A page per spectrum, `op spectra a` and `op spectra b`. Across the top, four
borderless operator strips, each with the operator name at the left, copy and paste
after it, the mute switch at the right, then a scope and spectro pair and an
`oscillator` card. The oscillator card repeats the operator oscillator controls and
adds the `morph` slider under the wave select. The wave select carries the mode:
`spectrum` is the last entry, and selecting it uses the stored spectra.

Below a 6 px empty band under the strip, one 124 px card per operator. The card
header carries `range`, `mask`, `selection`, `group`, the `load spectrum
template...` button, `norm`, `to b` or `to a`, copy and paste. The body holds the
64-bar editor and the group legend on the left and the `tilt`, `level` and
`formant` springs on the right, with the formant picker h1 to h64 beside the
formant lever. The selection offers seven windows that follow the card's range,
from the whole spectrum down to 16-harmonic blocks, and it gates the springs and
the freehand editor. The four bands fit under the strip without a scroll.
The bars preview a spring curve from the levels at the start of the drag, and the
release leaves the result in place.

The template button opens the thirteen wave names and writes that wave's analysis
into the row. It does not touch the waveform, so the stored levels wait for
`spectrum` in the wave select, a spectrum paste, or the morph envelope switch or a
morph LFO depth moved away from zero. A and b each carry their own button, so either spectrum can start from a
wave. The row header carries copy and paste for the whole spectrum, so a spectrum
moves between operators, and `to b` or `to a` copies it to the other spectrum of the
same operator.

### Op spectra mod

Four columns, each with the operator name, copy and paste, the mute switch, 10 px
gaps, the `oscillator` card, a 130 px `spectra` display, then the `morph env` and
106 px `morph lfo` cards. The display draws the stored spectra. Spectrum a is a filled
purple outline and spectrum b is a pink outline, with a marker under them that runs
from `a` to `b` at the current morph value. The marker and the faint waveform follow
the live morph of the newest rendering voice. The display does not take a drag. The `morph
env` card holds the graph, the on switch, R1 to R3, L1 to L3, the release rate and
level, and the mode dropdown. The `morph lfo`
card holds rate, depth, `rate fade` and `depth fade` in one row, with the wave
select and `bipolar` switch below.

### Op phasemap

Four borderless columns, each with the operator name, copy and paste, and the mute
switch, then a 60 px oscillator strip, an 88 px composed map and three cards with
8 px gaps: `phase`, `phase lfo` and `phase env`. The phase LFO card is 106 px and
the phase envelope is 206 px. The oscillator strip carries `ratio`, `detune`,
`level` and `feedback` as 22 px dials with the wave select beside them, and uses
the operator copy and paste. The map draws the composed transfer curve: green
forward segments,
purple flipped ones, locked segments at 35% opacity, a pink dashed vertical at
each order jump, a faint `F` polyline, the pink breakpoint vertical and a white
dot at the newest voice's phase. The caption reads `bp 0.500 · π`, the active and
requested segment counts, and the locked count when it is above zero. The map
does not take a drag.

The `phase` card holds `on` in its header and nine knobs in two rows:
`breakpoint x`, `segments`, `flip` and `flip rate` on the first, then `reorder`,
`reorder rate`, `reorder jump`, `drift` and `drift rate` on the second, with 4 px
between the rows. The second row uses the compact 10 px label. The `phase lfo`
card puts rate, depth, `rate fade` and `depth fade` in one compact row of 22 px
dials, with the wave select and `bipolar` below. The `phase env` card is the signed envelope with a 48 px
purple graph, `r1..r3`, `l1..l3`, `rel rate`, `rl`, the mode dropdown and `on`, with
2 px between the rows. All three cards carry `copy` and `paste` in their
headers, and the operator header copies and pastes the whole operator.

When `on` is off, the plot shows the neutral parameter geometry rather than the
last live map, so the switch and a patch load both reset the display.
The map is per part and shared by the voices, so a new note can reshape a held one;
the caption shows the actual active count when a lock lowers it. The read is 1×,
so the joins' aliasing stays a recorded limit rather than a drawn quality.

### Dialogs

Patch browser and part slot lists are vertical scrollers with a thin scrollbar,
where rows show the slot number, the name, and the state, and selected rows use a
purple border and text. Confirmations use the same dialog frame. The algorithm
picker is a wide dialog with eight routing tiles, and each tile centres its graph in
the figure area and keeps `alg n` on its own line below, with enough tile height
that the graph cannot clip or touch the caption.

The multi-patch library lists the 64 multi patches and carries `export bank`,
`import bank`, `init multi-patch`, `save` and `load`. Export and import use the
operating system file dialogs. Init multi-patch asks `init multi-patch? this wipes
the current patch.` and offers `init` and `cancel`. The save chooser is titled
`save multi-patch` and carries `discard edits to parts`, `save as new parts` and
`mutate parts in place`.

The single parts library is titled `single parts`. It opens on the 128-slot
scroller, with the active part's source slot selected, and its buttons choose both
the part and the action: `export bank`, `import bank`, `init single part 1`, `init
single part 2`, `save single part 1`, `save single part 2`, `load single part 1`,
`load single part 2` and `cancel`. A save writes the chosen part into the selected
slot, and a load applies the selected slot to the chosen part. Dialogs put `cancel`
at the right edge of the last button row.

### Status

`midi` and `sync` with green LEDs, and no editing text on the right.

## State language

| State | Visual |
| --- | --- |
| unmuted | solid pink tile, black label |
| muted | black tile, pink outline, pink label |
| switch active side | word `active` in pink, thumb left and pink |
| switch muted side | word `mute` in pink, thumb right and dark |
| state switch | word `on` or `off` beside the track, green when on and dim when off |
| active part | 1px green border, green part name |
| selected tab | pink cut polygon, black text |
| selected list row | purple border and text, faint purple fill |
| focus | green text input, pink select |
| hover | pink border on buttons, lighter knob name, underlined value, purple row |
| disabled | 30 to 35 percent opacity |

## Signal language

- Scope trace: green.
- Spectro bars: pink with a purple band every fifth band.
- Amplitude and pitch envelopes: purple.
- Filter envelope and pitch LFO: green.
- Level slider fill: purple.
- Enabled switches: green.
- Voice mode active button: green.
- Routing: white modulator, pink carrier, purple carrier output arrow.

## Copy

- Lowercase everything except the wordmark and user-entered names.
- Labels are nouns. Label the control `oscillator` rather than `pitch`.
- Note readouts show both forms, for example `60/c4` and `127/g9`.
- The mute words are `active` and `mute`.
- The patch control and its library call the item `multi` and `multi-patch`.
- A state switch reads `on` or `off`, with the control name when it needs one,
  so the toggle reads `bipolar on` rather than a fixed `on`.
- Part titles are `part n: name`.
- Names use the preset record limit of 32 characters.
- No `what this is` section. The page shows the product. Use the space for the
  next action, the controls, or a real limit.
- No `The... The...` lists. Drop the article, lead with the noun or the verb, or
  write the list as prose.
- No wh-word headings or table labels. Use a noun phrase or an imperative:
  `Get started!`, `Navigation`, `Functionality`.
- No counted-couplet headings, captions, or table descriptions. `One page, one
  job`, `Two sources, one filter` and `One phase, many amplitudes` sell a shape
  instead of naming the section. Use a noun phrase and put the count in the body.
- Use the panel value rather than the storage value. `ratio (0.50..16.00)` replaces
  `65536`, and `feedback (0..100%)` replaces `0..32767`.
- Do not add descriptions of the obvious.

## Platform mapping

- JUCE: see `juce.md`. Generate `UiTokens` from `tokens.json` rather than typing the
  values again, bundle the OFL fonts, and keep GPL-3.0-only.
- HiDPI: draw in logical units and scale the whole editor uniformly, and never scale
  individual strokes or font sizes.

## Working method

The interface is designed before it is written. Read the visual language and the
panel before changing the editor.

I design the control map and the mockup before I touch the editor, because the JUCE
code is expensive to rearrange and the HTML is cheap. The mockup is allowed to be
wrong, but it has to be wrong in a way I can see. Once the tokens and the layout
agree, the editor build is mostly transcription, and the snapshots catch the places
where the transcription drifted.

## Open items

- Digital Desolation Plus is not used. The wordmark is Zen Dots with an Unbounded
  lowercase `a`, so the brand font licence does not block packaging.
- The `part settings` page is intentionally sparse at the top and fills with the
  two part visualisers, so do not add filler widgets to fill space.
- The wordmark, the manual, the posters and the web graphics are still to be
  designed. The interface language is ahead of the surrounding material.
- A vector wordmark would remove Unbounded from the bundle, because it appears only
  in the wordmark. The licence list follows the binary, so update it when the mark
  lands.
