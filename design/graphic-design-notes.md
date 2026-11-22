# Graphic design notes

These are my working notes on the Type P.alpha visual material. The interface
language itself is in [`README.md`](README.md), and this page collects the mark,
the type, the palette and the references that the rest of the material grows
from. The shipped editor is the reference for the panel. The manual, the posters
and the web pages follow the same decisions, not a second style.

## The mark

The shipped mark is `TYPE P.` in Zen Dots with a pink dot and an Unbounded
lowercase `a`. Both families are OFL, so they can travel in the binary and in the
mockup. The compromise is visible: the `a` is not a Greek alpha and does not read
as one, because I could not find an OFL family with the Greek alphabet in the
shape I wanted. Zen Dots also has caps that read odd rather than futuristic or
techno to me.

The next mark is an experiment, not a decided design. The spirit is the Foley
Room-era Amon Tobin mark, and a cut-up and layered Rexlia is one candidate.
Rexlia is the contemporary industrial display in the reference list, and that is
the quality I want the custom mark to keep. The alpha can be built from existing
font components, because most free families have no Greek glyphs. The mark will
probably work better as vector art than as an embedded font: the alpha is easier
to draw, and the licensing is simpler. The vector route also lets the alpha keep
the same stroke weight as the caps at every size, which a font substitution
cannot promise once the mark is set at 30px in the header and at poster scale on
the same day.

Eurostile, Stop, Corporate and VDL's Line and Giga stay references for the
character, not models to trace. I do not want a pastiche of a typeface I admire.
The mark should carry the same squared, technical quality without pretending to
be a lost 1970s artifact.

Scale and construction:

- The panel mark is 30px with a solid 4px shadow in `brandShadow`.
- The brand line sits 12px below it.
- The shadow and the caps make the mark read larger than 30px. That is the
  optical size I want in the header.
- The wordmark is the first material to draw, and the manual, the posters and the
  web graphics follow it.

## Type

Zen Dots does the shouting: the wordmark, section titles, tabs, operator and part
names, dialog titles, toolbar labels and status. DotGothic16 does the reading:
values, labels, buttons, notes and dialog body. Unbounded appears only in the
mark's `a`, because a third family would give me one more licence and one more way
to make a label look important when it is not.

| Family | Use | Licence |
| --- | --- | --- |
| Zen Dots | display, titles, tabs, names, status | SIL Open Font License |
| DotGothic16 | interface text, values, labels, notes | SIL Open Font License |
| Unbounded | the wordmark `a` only | SIL Open Font License |
| Digital Desolation Plus | unused | not bundled, not needed |

The browser mockup loads the OFL families from Google Fonts after first paint.
The JUCE build bundles the OFL files as binary resources. The type is deliberately
cheap to render. Keep it lowercase and pixel-sharp, with tight tracking between
0.01em and 0.06em. The JUCE projection adds 4px to every token size because the
same nominal height renders smaller there. Do not edit the token sizes to
compensate, because that pulls the token scale away from the mockup.

## Colour

The palette is small on purpose. Pink carries the identity and the value I want
the player to read first. Green carries anything live or enabled. Purple carries
the selected or modulating thing. When a new state needs a colour, reuse one of
those three rather than adding a fourth, because the moment a colour means two
things the panel starts to look like a dashboard.

That is the whole palette. The reason it stays small is that a fourth hue would
have to mean something the first three do not, and every new state that reaches
for one is a state I have not described well enough in the layout.

| Token | Hex | Role |
| --- | --- | --- |
| bg | `#000000` | window and page ground |
| panel | `#070707` | section and operator fill |
| popup | `#1b1b1b` | dialogs and popovers |
| line | `#1c1c1c` | hairlines inside dark surfaces |
| lineStrong | `#2e2e2e` | structural dividers and control borders |
| text | `#d2d2d2` | body text |
| dim | `#7d7d7d` | labels and secondary text |
| faint | `#4a4a4a` | hints, captions, inactive edges |
| white | `#ffffff` | dialog text, modulator blocks, high-contrast marks |
| pink | `#ff2e7e` | brand dot, carriers, primary values, active tab |
| pinkBright | `#ff4d94` | knob values, hover on pink |
| purple | `#c780ff` | selection, modulation, pitch envelope, level fill |
| green | `#3dff9e` | live signal, enabled state, active part, output label |
| brandInk | `#d6d6d6` | wordmark |
| brandShadow | `#c2ad00` | solid wordmark shadow |

Rules:

- Pink is identity and primary emphasis. It is not a success colour.
- Green means signal or an active path. It is not decoration.
- Purple marks the selected or the modulating thing. The filter envelope and the
  pitch LFO curves are green, whilst the amplitude and pitch envelope curves are
  purple.
- Retire `panel2`, `green2`, `select` and `yellow` from new code. They were
  exploratory values and are not part of v1.

## Layout

The window is 1240 by 985 logical pixels, fixed across pages. The operator page
sets the height and the other pages fit inside it. A grid that reflows is a grid
I have to test twice, so the answer to a page that needs more room is a tighter
card, not a taller window. The fixed window is a decision about testing as much
as about composition, because a page that reflows at every host size is a page I
have to check twice for every control I add, and the second check is the one I
forget.

The visual rules are short:

- Black environment. Surfaces do not need outlines.
- Controls are outlined and content surfaces are not. If it cannot be clicked or
  typed into, it probably should not have a border.
- Square corners, flat fills, no gradients, no blur and no glow. The only shadow
  is the solid offset behind the wordmark.
- One meaning per colour, one home for every token, and every label earns its
  place. There is no filler text such as voice counts or operator counts.
- Lowercase labels, with a `//` prefix in `faint` on section titles.
- The active tab is a cut polygon, not a rectangle. Paint it unclipped or the tail
  is cut at the component edge.

## References

These shape the language. Some are direct decisions and some are typefaces I
admire for the spirit they carry:

- A Novarese's Eurostile and Stop for the squared, technical letterforms.
- Openmind's work for Amon Tobin's Foley Room for the cut-up, layered surfaces.
- R Mitchell's Corporate for the restrained grid.
- TDR's Wipeout and Autechre covers for the high-contrast, print-like marks.
- N2-Antireal's Terminal for the terminal density.
- Maniackers, Nakamura Y and Hashimoto K's Gona for the compact, coded labels.
- VDL's Line and Giga for the thin rules and modular blocks.
- R Larabie's Rexlia for the wide, industrial display type.

The front door carries the short list. This page carries what each reference
contributes, so the next material can reuse the reasoning instead of guessing at
it.

## Next material

The wordmark is first. The manual, the posters and the web graphics follow it, and
the panel is where each one appears first. The manual should feel like a printed
object rather than a web page, with the same black field, the same three colour
roles and the same lowercase discipline. The PDF is the print version, so it can
use a light ground and keep the pink and purple as accents. The web version stays
dark.

The manual has to work as a printed object and as a dark web page from the same
set of decisions, which means the page order, the caption style and the way a
screenshot sits in the margin all need to be drawn before the text is written,
rather than pasted in afterwards. That is the next drawing.

The research lab is a different surface. It is a study page, not the editor, and
it should use the palette and the type without pretending to be the instrument.
Its copy explains the experiment and its limits. It does not sell the feature.

## Open sketches

- A cut-up and layered Rexlia mark, with the alpha drawn as vector shapes rather
  than set from a font.
- An alpha built from existing font components, since most free families have no
  Greek glyphs.
- Keep the `TYPE P.` construction, or break it apart. I do not know yet. The
  first sketch will decide.
- The mark at 30px in the header, in the manual, and as a standalone poster
  element. The same drawing has to work at all three sizes.
- A light-ground print sheet that keeps the palette's roles without turning the
  black panel into a white page with pink stickers on it.
