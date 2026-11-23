# Macros in the Prophecy, Vital and Pigments

The README asks what kind of macro controls would make the synth friendlier, more
performable and more modulatable without turning the panel into an overwhelming mod
matrix, and the three manuals I read for this note give three shipped answers to that
question. The Prophecy is the main case here, because its macro is the only one of the
three that works with no routing layer underneath, which is the shape this engine can
adopt without building a matrix first.

The manuals are the KORG Collection Prophecy owner's manual, the Pigments 7 manual and
the Vital manual, stored at `docs/research/macros/` for the study. They are third-party
manuals, so they stay out of the repository and the sources below cite them by title.
The Prophecy software manual leaves out the hardware's four Performance Editor sets, so
I read the 1995 Basic Guide and Parameter Guide for that part as well. I have not run
the Prophecy plugin, so everything below about its behaviour is the manual's
description rather than a measurement, and the places where the two guides disagree are
marked as questions.

Status: the design interview of 23 November 2026 chose a three-destination offset layer
rather than the Prophecy's single-destination editor, because the destinations must
keep their stored values, the sources have to sit around an automatable centre, and
velocity has to reach the voices. The agreed design is
[`../../design/macros.md`](../../design/macros.md), and the parameters and evidence are
in [`../../plugin/feature-contract-macros.md`](../../plugin/feature-contract-macros.md).
The recommendation at the end of this note predates that interview and is the smallest
shape, not the chosen one.

## The Prophecy performance editor

The Prophecy puts five knobs in its header row, under the name PERFORMANCE EDIT, and
they stay visible whatever page is open, because the 1995 panel carries a small display
and the same five knobs double as the edit controls in Define and Edit mode. Each knob
owns one parameter, chosen from 256 entries in the assign list, and the assignment is
made on the ARP page's CONTROLLER section and stored with the program.

The part worth copying is the window. `LEFT` and `RIGHT` set the two ends of the
knob's travel as percentages of the destination parameter's own range, so the manual's
example of a parameter that runs -99 to +99 takes a `LEFT` of 50% and makes the
parameter 0 when the knob is fully left. A `LEFT` of 0% makes it -99, and `RIGHT` works
the same way at the top of the travel. A third setting, `CURVE`, picks linear, log or
exponential response, so a single knob maps its travel onto a slice of the destination
and can invert the direction by putting `LEFT` above `RIGHT`.

A concrete setting is cutoff on knob 1, left 30%, right 90%, log curve, where the knob
sweeps the upper part of the cutoff range and the low third of the range is never
reached, which is the useful part of the range for a lead patch. The log curve spends
most of the travel where the ear notices the change.

The knob edits the program's parameters in the edit buffer and the Write operation
saves the result, which makes the macro an editor rather than a modulation source, and
the destination's value and the macro's position are the same number. That is why five
knobs need five slots and no matrix, and why the panel never has to draw a modulation
ring over a base value.

The hardware went further than the software manual describes, since the 1995 guide
gives four Performance Editor sets, PE1 to PE4, each set assigning the same five knobs.
One knob can drive up to four parameters, one per set, with the sets switched on and
off from the front panel. Four layered destinations per knob cost four assignment
pages, and the plugin manual documents one assignment per knob and no set selector.
The Global page's MIDI CC list still names PERFORMANCE EDIT SW, so the switch may be
in the plugin without a page here. That is a question for the plugin rather than a
finding.

The CC side is global. The Global page's MIDI CC settings assign a control change to
WHEEL1 to 3, RIBBON X and Z, and PERFORMANCE EDIT SW or KNOB 1 to 5, with MIDI Learn
and a CC number per controller. The knob assignment travels with the program whilst
the CC number does not, so the same physical control changes meaning when the program
changes, which is the usual trade for a global controller map.

## The shape that needs no matrix

The Prophecy's modulation is one source per destination, because a parameter such as
resonance carries a `Mod. Source` selector and a `Mod. Intensity` amount, and the
sources are the four EGs, the pitch and amp EGs, the four LFOs, portamento, note
number, velocity, pitch bend, aftertouch and CC 0 to 95. A fixed set of destinations
has its own LFO and EG amounts, such as the filter's `Mod. LFO lntensity`, so there is
no route list and no destination table anywhere in the program.

A macro layer on that engine only needs a destination slot per knob, whereas five
knobs on a synth with a matrix would be five rows that each hold a source and a list
of destinations, which is where the panel cost comes from.

## Vital macros

Vital has four macros at the left of the window, each a knob with a modulation square
beneath it, and they behave as ordinary modulation sources apart from the one
difference the manual names: the macro knob has to be turned to produce a value. The
knob can be moved by hand, by another source such as an LFO, by DAW automation or by a
MIDI mapping, so a macro can itself be modulated.

The destination bound lives in the matrix rather than in the destination. A row carries
the source, a `bipolar` switch, a `stereo` switch, a `morph` curve, an amount and a
destination, and a click on the row opens a remap curve for the route. Clicking the row
number replaces it with an X and bypasses the route, so a macro can reach any parameter,
the amount decides how far it moves, and the two curves decide the feel.

## Pigments macros

Pigments also ships four macros, described as unipolar controls that can carry any
number of routes, including sidechains. The macro tab stays lit in the modulation
overview whichever source group is selected, so the four knobs are always visible, and
a double-click on the label renames one for the patch.

Assignment has three routes: drag the source name onto the destination, open the Mod
source view for one destination and move one of the 24 sliders, or open the Mod target
view for one source and add destinations to it. Each route has an amount from -1.00 to
1.00 in steps of 0.01 and can carry a sidechain, and MPE slide is wired to Macro 1, so
a controller without MPE can still reach the first macro through polyphonic
aftertouch.

## The direct macro and the route macro

| Synth | Macros | Destinations per macro | Bound by | Writes |
| --- | --- | --- | --- | --- |
| Prophecy hardware | 5 | 1, or 4 with the PE sets | Destination window and curve | The parameter value |
| Prophecy plugin | 5 | 1 | Destination window and curve | The parameter value |
| Vital | 4 | Any | Route amount and route curves | A modulation offset |
| Pigments | 4 | Any | Route amount | A modulation offset |

The two styles answer different questions. A macro that writes the value is a
performance hand on a parameter, and it shows the player exactly what it did when the
destination knob moves, whilst a macro that writes an offset is a modulation source
that needs a ring, a route table and a base value that stays put. The Prophecy gets a
range window and a curve per destination because it only has one destination to spend
them on, and Vital and Pigments spend the equivalent effort on the route list.

## The Type P.alpha parameter set

The current manifest is 1,976 host parameters: 981 per part, 10 routing parameters for
level, delay, channel, keyzone low and high, and 4 globals. Every row is marked
non-modulatable, and the engine's LFOs and envelopes are wired to their destinations
in the patch rather than through a route table, so a macro layer here has no matrix to
plug into and would have to be the kind that writes values.

`syncParams` reads each parameter per block and calls `engine_.setParam` when the
integer changes. A macro that writes a host value therefore reaches the engine through
the same path as a knob, with the same block boundary, so a fast macro sweep gets the
same treatment as a fast knob move.

The part params page already carries a curated target list. Key follow and velocity
share 19 targets, each with a below amount, an above amount and a velocity amount, and
the `wipe all` button clears a whole card. That list is the precedent for a macro
destination, because a named index is readable in the panel and survives a parameter
insert, whilst a raw index into the 981-value part record would need a map at every
format change. The record has already been through six migrations to reach version 16,
with a layout that mirrors the device's `presets.c`.

The patch settings page has an empty band under the two part cards, wide enough for a
row of five knobs. The part params grid keeps its last cell empty, so a macro row has
somewhere to sit without a ninth tab.

## Costs

A direct macro needs five parameters, which are the value, the destination, `left`,
`right` and the curve, whilst a matrix-style route needs two per route, destination
and signed amount, plus one value per macro. The table below puts the totals against
the current manifest.

| Shape | Parameters | Manifest total |
| --- | --- | --- |
| 5 direct macros, per part | 25 per part | 2,026 |
| 5 direct macros, patch scope | 25 global | 2,001 |
| 4 macros with 4 routes each, patch scope | 36 global | 2,012 |
| 5 direct macros with 4 hardware-style sets | 85 per part | 2,146 |
| 4 macros with 8 routes each, patch scope | 68 global | 2,044 |

The current part record is 981 values, so 25 more is a small change to the record and a
version 17 migration, whilst the 85-value layered form is a different proposition and
would be the largest single addition to the part block since the spectra.

## Recommendation

I'd take the Prophecy shape and keep the layered sets out until a performance patch
demands them.

1. Five macros, patch scope, one curated destination each, with a left and right
   percentage of that destination's range and a linear, log or exponential curve,
   which is 25 parameters and a manifest of 2,001.
2. Store the destination as an index into a curated target list, seeded from the 19
   follow targets and extended where a macro needs a target the follows do not
   cover, because a raw parameter index would have to be remapped at every format
   change.
3. Put the row in the empty band of the patch settings page, so the five knobs are
   one click from the part cards and stay visible on the page where the patch is
   assembled.
4. Let the destination knob move with the macro, as the Prophecy does, which keeps
   the panel honest and costs no ring, no base value and no route display.
5. Add a second and third destination per knob only after a patch runs out of
   macros, in the way the hardware added PE1 to PE4, because three more destinations
   per knob is 60 more parameters across five macros and it changes the macro from an
   editor into a small matrix.

The one edit I'd make to the Prophecy's version is the target list. 256 assignable
parameters is more than this panel can name, and the panel already has a readable
follow list, so a macro should offer the targets it can label.

## Open questions

1. Whether one destination per macro is enough. The test is a performance patch with
   a filter sweep, a spectral morph and a feedback rise that wants all three at once.
2. Patch scope or part scope. The Prophecy is per program, Vital and Pigments are
   per patch, and this synth has two parts with their own records and `to 1` and
   `to 2` copy actions, so part scope fits the format whilst patch scope fits the
   playing.
3. Whether a block-rate macro sweep zippers on the ratio or the cutoff. The
   parameter path is the same one the follow cards use, so a render comparison would
   settle it.
4. Whether a macro needs a name. The part record holds integers, so a name means a
   metadata field or a fixed label such as `macro 1`.
5. Whether the Prophecy plugin kept the four PE sets. The manual does not say, and
   the answer decides whether the software can layer destinations on one knob.

## Sources

- KORG Collection Prophecy owner's manual, E1, 2021: PERFORMANCE EDITOR on page 58,
  the modulation source list on page 62, the Global MIDI CC settings on page 61.
- Vital user manual: macros in section 3.5, the matrix in chapter 4.
- Arturia Pigments 7.0.1 manual: macro controls in section 3.6, the modulation
  routings in chapter 19, macros in section 20.7.
- Prophecy Basic Guide and Parameter Guide, 1995, for the four Performance Editor
  sets and the 256-entry assign list.
