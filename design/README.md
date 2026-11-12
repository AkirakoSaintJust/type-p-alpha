# p1onken interface language

The shared visual language for the Type P.alpha editor. The clickable mockup at
`design/mockup.html` is the reference implementation, and
`tokens.json` is the machine-readable source. The CSS projection is `tokens.css`,
and the platform notes are in `juce.md`.

Status: v1.2, 2026-11-09, reviewed against the editor. Revised for the JUCE
optical type correction, the state-reading toggles, the extending tab tail, the
card action spacing, the voice-mode knob group and the multi-patch library.

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
| operator oscillator and amplitude LFO | 56 | 34 |
| part settings | 62 | 38 |
| pitch envelope | 52 | 38 |
| key follow and velocity follow | 64 | 42 |

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
Unbounded, and the brand line `p1onken // パンク派第１音響合成研究班` in 12px
DotGothic `dim`.

The algorithm block is a button, and it shows a 30px Zen Dots number, a caret, and
the routing graph. The graph is 240 by 128 in the viewBox, scaled to about 244 by
130. The graph content is centred in that viewBox, so the header and the picker
tiles share one optical centre. White operators are modulators, pink operators are
carriers, grey lines are modulation, and purple downward arrows mark carrier
output. Clicking the block opens the algorithm picker, whilst the mouse wheel over
the block cycles the eight algorithms without opening the picker.

The output section is a label plus a `scope` and `spectro` segmented control, with
the active segment in purple. The plot is 50px high, borderless, black.

The patch control shows `multi` and the current multi-patch name, with a separate
`rename` button and the hint `click above for multi-patch library actions` below
it. Clicking the control opens the multi-patch library.

### Toolbar

Left is the mute legend. A small dark square with a pink outline and pink 猫 means
muted, and a solid pink square with a black 猫 means active. Then the real mute
tiles, `op1` to `op4` and `p1`, `p2`, at 36 by 22. An active tile is solid pink with
a black label, and a muted tile is black with a pink outline and a pink label. The
猫 is only a legend glyph, never a button label.

Right is `now editing part:` with `part 1`, `part 2` and `part library actions...`,
and the selected part button has a green border and green text.

### Tabs

`patch settings`, `operators`, `fx / mod`, with no vertical dividers. The active tab
is a pink cut polygon rather than a rectangle. Inactive tabs are dim text on black.

### Patch settings

Two part cards, each titled `part n: name` in Zen Dots. The active part has a green
border, and the inactive part has no visible border. Each card holds rename, copy
and paste at the left of the header, a mute switch at the right with a 16px gap,
five knobs, a wide level slider with the caption `level`, and two borderless
visualisers, one scope and one spectro, with no captions on the visualisers. The
five knobs are `midi channel`, `keyzone start`, `keyzone end`, `tune` and `delay`,
where `tune` is the part detune in cents. Then a voice mode block with `poly`,
`mono`, `legato`, `unison` and the unison, detune and porta knobs. The three knobs
sit next to the mode buttons with a 32px gap rather than at the card edge.

### Operators

Four borderless columns, each with the operator name at the left, copy and paste
after it, and the mute switch at the right with a 16px gap before it, then a scope
and spectro pair, and three cards: `oscillator`, `envelope`, `amplitude lfo`. The oscillator card holds ratio, detune, level, feedback and the
wave select. The envelope holds R1 to R4 on the first row and L1 to L3 on the second
row, and the amplitude LFO holds rate, depth, the wave select and the `bipolar`
switch. The four oscillator knobs spread across the card, and the two LFO knobs sit
apart. The envelope rates and levels each get their own row.

### FX / MOD

One page, a three by two grid that fills the window. Filter 1, the filter envelope
and the pitch LFO fill the first row. Pitch envelope, key follow and velocity follow
fill the second row. The filter card has cutoff, resonance and env amount, whilst
key and velocity tracking sit in the two follow cards rather than on the filter
card. The filter, its envelope and its two follow targets address the active part,
because each part has its own filter and its own envelope.

The pitch LFO card carries rate, depth, the wave select and the `bipolar` switch.

Key follow and velocity follow are separate cards. Each has an on switch, a
`wipe all` button and its own target selector. Key follow has a breakpoint and below
and above amounts, whilst velocity follow has one amount. The wipe button sets every
target amount to 0%, so the card can be cleared without visiting each target. The
same target set is used for both cards: operator level, feedback and amp envelope,
pitch envelope, filter cutoff, and filter envelope.

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

The single parts popup is titled `single parts` and lists part 1 and part 2 with
their names and source slots. It carries `export bank`, `import bank`, `init single
part 1`, `init single part 2`, `save single part 1`, `save single part 2`, `load
single part 1`, `load single part 2` and `cancel`, and the buttons wrap onto two
rows. The part slots list is titled `part n slots` and holds 128 sounds, with the
part's source slot selected on open. Dialogs put `cancel` at the right edge of the
last button row.

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

## Open items

- Digital Desolation Plus is not used. The wordmark is Zen Dots with an Unbounded
  lowercase `a`, so the brand font licence does not block packaging.
- The `part settings` page is intentionally sparse at the top and fills with the
  two part visualisers, so do not add filler widgets to fill space.
