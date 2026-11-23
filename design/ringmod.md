# Ring modulation

Status: design, not built. The decisions below are settled; the card and the
engine change are not.

## The idea

A ring modulation stage inside the part's carrier mix, after the operators and
before the part filter. It multiplies pairs of carriers and adds the products to
the carriers that are still heard directly. The direct outputs are gated once per
carrier, so a carrier that appears in two pairs is still heard once.

The pairs follow the current algorithm. A carrier-count of one has no ringmod, two
carriers have one pair, three have two and four have three. The pairs are the
consecutive carriers in operator order: c1 with c2, c2 with c3, and c3 with c4. A
middle carrier therefore feeds two pairs, and its direct output is still a single
term.

## Signal path and law

The `route` node already sums the carriers into `car` for the part filter. The
ringmod lives there, so the filter and its envelope shape the products as well as
the direct carriers. Because both the direct terms and the products meet at the
same `car` output, the part filter, its filter envelope and the amplitude envelope
treat a ringmod product as another carrier, and the stage needs no second filter
path. The pair product is the Q15 product of the two carrier signals, scaled by the
pair's level gain. The level is 0 to 99 on the project's level law, so 99 is unity
and 0 is silence. The pair's on/off gates only the product; the direct carrier gates
are independent and default on.

The direct sum is `sum(out_i ? carrier_i : 0)`. The ringmod sum is
`sum(on_p ? q15(carrier_a, carrier_b) * level_gain_p : 0)`. A carrier's direct
term is added once no matter how many pairs name it. The one-carrier algorithms
keep their single direct term and have no pairs.

At the defaults the ring pairs are off, the levels are 99 and every direct gate is
on, so the carrier sum is exactly the current one. The first render after the
change must be bit-exact with the reference.

## Parameters

Ten per part, twenty for the two parts. The names use the part suffix `.p0` and
`.p1` like the rest of the part block.

| name | range | default | display | target |
| --- | --- | --- | --- | --- |
| `ring1_on` | 0 to 1 | 0 | on/off | `route.ring1_on` |
| `ring1_level` | 0 to 99 | 99 | raw | `route.ring1_level` |
| `ring2_on` | 0 to 1 | 0 | on/off | `route.ring2_on` |
| `ring2_level` | 0 to 99 | 99 | raw | `route.ring2_level` |
| `ring3_on` | 0 to 1 | 0 | on/off | `route.ring3_on` |
| `ring3_level` | 0 to 99 | 99 | raw | `route.ring3_level` |
| `car1_out` | 0 to 1 | 1 | on/off | `route.car1_out` |
| `car2_out` | 0 to 1 | 1 | on/off | `route.car2_out` |
| `car3_out` | 0 to 1 | 1 | on/off | `route.car3_out` |
| `car4_out` | 0 to 1 | 1 | on/off | `route.car4_out` |

The parameters sit at the end of the part block and before the macro block. The
part record grows from 981 to 1001 values, so the preset version takes a new step
and old part records gain the defaults at the end. A version 16 part record ends at
the phase block, which means the loader can append the ten ringmod defaults after
it without touching any existing value, whilst a record from before the phase block
gains both blocks in order. The macro block, the multi record and the project state
keep their sizes; a missing new parameter in a project state takes its default.

## The card

The card fills the empty bottom-right cell of the part params grid, beside
velocity. The card is one cell. The title is `// ringmod`. The body has up to three
pair rows and an original-output row.

A pair row holds a small graph, an `on` toggle and a level knob. The graph is two
pink operator boxes with their operator numbers, arrows into a non-solid box
labelled `R`, and a purple arrow out of the R. The operator numbers are the
current carriers, so the graph changes with the algorithm. The toggle is green
when the pair is on. The level knob is a 42 px dial with the raw 0 to 99 value,
dimmed to 0.35 while the pair is off.

The original-output row is an `original` caption and one toggle for each current
carrier, labelled `op1` to `op4`. The toggle shows the label and a track, and the
track carries the on or off state, so four toggles fit the card width. It is a
single control per carrier, never one per pair, so a carrier that feeds two pairs
still has one direct gate. A middle carrier appears in two graphs because the chain
is consecutive, but its direct toggle appears once in the original row, which is the
rule that keeps the dry signal from being counted twice. The direct toggles stay
active while a pair is off, because they decide whether the direct signal is heard.
The dry term is added once.

The algorithm decides the rows and the carrier toggles. A one-carrier algorithm
draws the title and `no ringmod` and no controls. Two carriers draw one pair and
two direct toggles; three draw two pairs and three toggles; four draw all three
pairs and four toggles. A hidden row keeps its values. Switching the algorithm back
restores them.

## Defaults and bypass

The ring pairs are off, so the card opens in the bypass state and the patch sounds
as it did before the feature. The direct toggles are on. Turning a pair on adds its
product; turning a carrier's direct output off removes only that direct term and
leaves its ring products in place. A pair at level 0 is silent even while it is
on.

## Evidence the milestone needs

- The default patch renders bit-exactly against the engine reference, with the
  ring pairs off and the direct gates on.
- The pair counts follow the algorithm: none at one carrier, one at two, two at
  three and three at four.
- Each product is the two carriers multiplied; the level law is silent at 0 and
  unity at 99.
- A carrier in two pairs contributes its direct term once.
- The direct gate removes a carrier from the direct sum and leaves its ring
  products.
- An old part record expands with the ring defaults; a new record round-trips.
- The card snapshots cover one, two, three and four carriers, a pair off, and the
  dimmed level.
- pluginval and the snapshot diff stay green.
