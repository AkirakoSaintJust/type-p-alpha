# Feature contract: ring modulation

## Identity

- **Feature:** A carrier-pair ring modulation inside the part's carrier mix, with
  up to three consecutive pairs, a pair on/off and level, and one direct-output
  gate per carrier.
- **Brief section:** [`../design/ringmod.md`](../design/ringmod.md).
- **Owner / date:** P1Onken, 2026-11-24.
- **Disposition:** the parameters, the route math, the spec and part-record step,
  the card and the tests are built. The packaged release build with the feature is
  outstanding.

## Player outcome

- **New player action:** a `ringmod` card in the part params tab, in the cell beside
  velocity. Each current carrier pair has a small routing graph, an on/off and a
  level. An `original` row gates each carrier's direct output.
- **Sound and behaviour:** a pair multiplies its two carriers, scales the product by
  the pair level and adds it to the direct carrier sum. The part filter and its
  envelope shape both. A carrier in two pairs is still heard directly once. A pair at
  level 0 adds nothing even while it is on. A one-carrier algorithm has no pair and
  no ringmod controls.
- **Failure mode:** a ring pair that adds nothing while on, a carrier that is heard
  twice because it feeds two pairs, a card that does not follow the algorithm, or an
  old part record that loses values when the ringmod block is appended.

## Parameters

Ten per part, twenty across the two parts. They sit at the end of each part group in
the plugin spec and at the route node in the engine.

| name | range | default | display |
| --- | --- | --- | --- |
| `ring1_on` | 0 to 1 | 0 | on/off |
| `ring1_level` | 0 to 99 | 99 | raw |
| `ring2_on` | 0 to 1 | 0 | on/off |
| `ring2_level` | 0 to 99 | 99 | raw |
| `ring3_on` | 0 to 1 | 0 | on/off |
| `ring3_level` | 0 to 99 | 99 | raw |
| `car1_out` | 0 to 1 | 1 | on/off |
| `car2_out` | 0 to 1 | 1 | on/off |
| `car3_out` | 0 to 1 | 1 | on/off |
| `car4_out` | 0 to 1 | 1 | on/off |

The part record grows from 981 to 991 values. A version 16 record gains the ten
defaults at the end, and a record from before the phase block gains the phase and
ringmod blocks in order. The macro block, the multi record and the project state
keep their sizes.

## Laws

- The carriers follow the algorithm's carrier mask. They are taken in operator order
  and paired consecutively: c1 with c2, c2 with c3, c3 with c4. Two carriers have one
  pair, three have two, four have three, and one has none.
- A pair product is `p1_q15_mul(carrierA, carrierB)`, scaled by
  `p1_level_gain[level]` and gated by the pair on/off. The level law is the project's
  0.75 dB per step law: 0 is silence and 99 is unity.
- The direct sum is `sum(out_i ? carrier_i : 0)`. A carrier's direct term appears
  once, whatever the pair count. The one-carrier algorithms keep their single direct
  term and ignore the direct gate, because the card has no control for it.
- The route node produces the carrier sum for the part filter, so the ring products
  and the direct carriers share the filter, the filter envelope and the amplitude
  envelope.
- The engine keeps the ten values at the route node. The plugin spec moves those
  rows to the end of each part group, so the part record appends them without
  shifting the existing indices and the macro table's member ids use the spec index.
- A pair at 0 and every direct gate at 1 renders exactly as the patch did before the
  feature.

## The card

The card fills the eighth part-params cell. Each pair row holds a 112 by 56 graph,
an `on` toggle and a 42 px level knob. The graph is two pink operator boxes with
their numbers, arrows into an outlined box labelled `R`, and a purple arrow out. The
level knob dims to 0.35 while the pair is off. The original row is an `original`
caption and one `opN` toggle per current carrier; the track shows the state. The
card follows the algorithm on the next tick; a hidden pair keeps its parameter
values and returns when its algorithm comes back. One carrier shows `no ringmod`
and no controls.

## Evidence

- `plugin_smoke` passes `the ring off leaves the render bit-exact`, `a ring pair
  changes the render`, `a ring level of 0 adds nothing`, `a direct carrier gate
  removes the carrier`, `one carrier has no ring modulation`, `four carriers with the
  ring off match` and `four carriers ring the first pair`.
- `preset_smoke` keeps `the part record has the ringmod block` at 991 and the old
  version 16 record expands with the ring defaults; the part record round-trips.
- The snapshots add `editor-part-params-ringmod.png` with four carriers, three pairs
  and the first pair on, and `editor-part-params-ringmod-empty.png` with one carrier
  and `no ringmod`. The three existing part-params snapshots change because the card
  is in the grid.
- The full Linux suite is 18 of 18. A release build from this head still needs
  pluginval and the packaged binary check.

## Limits

- The pairing is fixed to consecutive carriers. A later pair selector would need a
  new parameter set and a new migration.
- The ringmod sits in the carrier mix, before the part filter. A post-filter ringmod
  is a different signal path.
- The ten parameters are part controls. They are not macro destinations; adding them
  to the macro table would add rows and part-record references.
- The one-carrier algorithms ignore `carN_out`, because the card has no control for
  the single carrier. The value stays in the record for a later algorithm.
- The level law is the same 0.75 dB per step law as the operator levels, so 99 is
  the table's 32767 rather than a mathematically exact 1.0.
