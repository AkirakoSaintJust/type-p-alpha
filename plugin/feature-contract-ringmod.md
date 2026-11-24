# Feature contract: ring modulation

## Identity

- **Feature:** A carrier-pair ring modulation inside the part's carrier mix, with
  up to three consecutive pairs, a pair on/off and level, and one direct-output
  gate per carrier.
- **Brief section:** [`../design/ringmod.md`](../design/ringmod.md).
- **Owner / date:** P1Onken, 2026-11-24.
- **Disposition:** the parameters, the route math, the spec and part-record step,
  the card, the tests and the Windows release build are built.

## Player outcome

- **New player action:** a `ringmod` card in the part params tab, in the cell beside
  velocity. Each current carrier pair has a small routing graph, an on/off and a
  level. An `original` row gates each carrier's direct output.
- **Sound and behaviour:** a pair multiplies its two carriers, scales the product by
  the pair level and adds it to the direct carrier sum. The part filter and its
  envelope shape both. A carrier in two pairs is still heard directly once. A pair at
  level 0 adds nothing even while it is on. A one-carrier algorithm has no pair and
  no ringmod controls. The direct gates apply only while an active pair is on; when
  every active pair is off, the part is exactly the pre-feature carrier sum. The
  header algorithm graph adds the R on every active pair, so the ring is visible
  from every tab.
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
keep their sizes. A library saved before the macro block also loads, because the
multi record check accepts the old record and the macro values start empty.

## Laws

- The carriers follow the algorithm's carrier mask. They are taken in operator order
  and paired consecutively: c1 with c2, c2 with c3, c3 with c4. Two carriers have one
  pair, three have two, four have three, and one has none.
- A pair product is `p1_q15_mul(carrierA, carrierB)`, scaled by
  `p1_level_gain[level]` and by a fixed 64 times makeup, and gated by the pair
  on/off. The raw product is the square of the already attenuated carriers, so the
  36 dB makeup puts a full pair at the default operator levels at the direct pair's
  RMS. The level law remains 0.75 dB per step: 0 is silence and 99 is the compensated
  unity.
- The direct sum is `sum(out_i ? carrier_i : 0)` while at least one active pair is
  on. When every active pair is off, the gates are ignored. A carrier's direct term
  appears once, whatever the pair count. The one-carrier algorithms keep their single
  direct term and ignore the direct gate, because the card has no control for it.
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
`R` is set in Zen Dots with a widened outline, because the family ships one weight. The
level knob dims to 0.35 while the
pair is off. The original row is an `original`
caption and one `opN` toggle per current carrier; the track shows the state. The row
is active while any active pair is on, and dims to 0.35 with every active pair off.
The card follows the algorithm on the next tick; a hidden pair keeps its parameter
values and returns when its algorithm comes back. One carrier shows `no ringmod`
and no controls. The header algorithm graph carries the R too: an active pair's two
direct arrows become the R inputs and one purple output, and the header reads the
active part's pair states on its tick, so the indicator follows the tab's part. The
header `R` is in the same bold Zen Dots face, scaled for its 20 by 14 box. The
wordmark's alpha now uses the Unbounded Bold cut, so its weight matches.

## Evidence

- `plugin_smoke` passes `the ring off leaves the render bit-exact`, `a ring pair
  changes the render`, `a ring level of 0 adds nothing`, `the direct gates are
  ignored while the ring is off`, `a direct gate applies while the ring is on`, `one
  carrier has no ring modulation`, `four carriers with the ring off match`, `four
  carriers ring the first pair` and `the ring level 99 is in the ballpark of the
  direct pair`. The last one compares the steady RMS of the ring-only output with the
  direct pair and expects the ring within 0.6 to 1.6 of it.
- `preset_smoke` keeps `the part record has the ringmod block` at 991 and the old
  version 16 record expands with the ring defaults; the part record round-trips. The
  direct part bank import and the library loader both load a version 16 bank and
  gain the ring defaults. A library with a version 16 part bank and a pre-macro
  multi bank loads, keeps both parts, and gains the ring defaults; that shape matches
  the shipped library. The import path reports a file it cannot read, so a rejected
  library no longer does nothing in silence.
- The snapshots add `editor-part-params-ringmod.png` with four carriers, three pairs,
  the first pair on and the header's R on that pair, and
  `editor-part-params-ringmod-empty.png` with one carrier and `no ringmod`. The three
  existing part-params snapshots change because the card is in the grid, the original
  row is dimmed while the ring is off, and the pair graph's R is in Zen Dots. Every
  golden takes the Unbounded Bold alpha in the wordmark.
- The full Linux suite is 18 of 18, and the Windows build passes its 17 tests.
  `pluginval` 1.0.4 at strictness level 5 with the GUI tests skipped reports
  `SUCCESS` on the VST3. The release package
  `Type-P.alpha-windows-x86_64.zip` (19,752,219 bytes, SHA-256
  `80E6ED13BC16B7CE5491B121FA5246641F179B54393A47CEF21B41B951AEAF53`) carries the
  VST3 (15,550,976 bytes,
  `E3F2B5D7C4FBAB80503996DF63F616B5CD2AED3ECA902E7F45142A95FB61FC12`), the CLAP
  (15,530,496 bytes,
  `CCAC75489905BB2287FF9705DCF47B7B6D225E76EAF9FF74AF518C6FF473C6D4`), the
  standalone (15,901,184 bytes,
  `8844CBC20381AD813B8B868F70559FD866996283E76B90C0B6E31412C16B413E`) and the
  licences. The CLAP validator runs in the macOS CI.

## Limits

- The pairing is fixed to consecutive carriers. A later pair selector would need a
  new parameter set and a new migration.
- The ringmod sits in the carrier mix, before the part filter. A post-filter ringmod
  is a different signal path.
- The ten parameters are part controls. They are not macro destinations; adding them
  to the macro table would add rows and part-record references.
- The one-carrier algorithms ignore `carN_out`, because the card has no control for
  the single carrier. The value stays in the record for a later algorithm.
- The level law is the same 0.75 dB per step law as the operator levels, with the 36
  dB makeup. The 99 setting follows the operator levels: the makeup is tuned at the
  default levels, so a much quieter operator pair gives a quieter ring at 99.
