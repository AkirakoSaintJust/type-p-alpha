# Feature contract: per-part panning and pan envelope

## Identity

- **Feature:** Place each part in the stereo field with a constant-power pan and a
  four-stage bipolar pan envelope, both per part.
- **Brief section:** the part card in [`../design/README.md`](../design/README.md)
  and the part fx/mod page.
- **Owner / date:** P1Onken, 2026-11-20.
- **Disposition:** complete. The Linux suite passes 14 of 14 with the new slider and
  card, and the CI run is still to follow.

## Player outcome

- **New player action:** each part card gains a `pan` slider under the level
  slider, from -100 to 100 with centre at 0. The part fx/mod page gains a pan
  envelope card with an on switch, four rates, four signed levels, a depth knob and
  the envelope graph.
- **Sound and behaviour:** the parts are summed into left and right buses rather
  than one mono bus, with constant-power gains. Centre is unity on both channels,
  so a patch that never touches pan renders exactly as before. The envelope swings
  the part around its base pan, retriggering when a note starts on the part and
  releasing when its last voice releases.
- **Failure mode:** a pan change that moves the existing centre render, a hard-panned
  part leaking into the other channel, a pan envelope that never releases, or a
  depth of 0 that still shifts the part.

## Dependencies and interactions

- **Other features:** the voice sum, the global DC blocker, the part card, the part
  fx/mod page, the parameter manifest, the part record and the editor snapshots.
- **Platform/API/HAL changes:** `render_voices_block` takes a second bus, and
  `render_post_block` takes left and right sources. The engine stays integer C.
- **Persistence/version impact:** the manifest moves from 1,616 to 1,638 parameters,
  and the part block from 802 to 812, because the pan envelope's ten parameters are
  the last mode parameters. `PRESET_VERSION` moves from 9 to 10 and the plugin state
  version from 7 to 8. The library migration expands any older record whose
  appended parameters are the new tail.
- **MIDI/clock impact:** none. Pan is a parameter and a control-rate envelope.

## Parameter and event semantics

- `pan.p0` and `pan.p1` are -100 to 100, default 0.
- The envelope parameters are `pan_env_on` (0 or 1, default 1), `pan_env_r1` to
  `pan_env_r4` (0 to 99, defaults 80, 60, 50, 80), `pan_env_l1` to `pan_env_l4`
  (-99 to 99, default 0) and `pan_env_depth` (-100 to 100, default 0). All are per
  part and stored in the part record.
- The pan law is constant power with unity at centre: the gain is
  `cos(theta) * sqrt(2)` on the left and `sin(theta) * sqrt(2)` on the right, with
  `theta` from 0 to a quarter turn as pan goes from -100 to 100. The gains come from
  a 201-entry Q16 table, because `p1_sin`'s 8-bit quarter table puts 45 degrees at
  0.994 instead of 1.0. Centre is exactly 65536 on both channels.
- The envelope runs once per part per control block. `pan_env_trig[p]` is set by any
  note-on that reaches the part, the attack starts from 0, the stage levels are
  signed, and a part with no gated or delayed voices enters release. The output is
  `level * depth / 99`, added to the base pan and clamped to -100..100.
- The voice sum applies the pan gains with a 64-bit multiply and a 16-bit shift, so
  the centre gain of 65536 is an exact pass-through.

## Evidence

- `engine_parity` passes `hard left puts part 1 on the left`, `hard right puts part 2
  on the right`, `centre pan is exactly unity on both channels`, `the pan envelope
  swings to the right` and `the release returns the pan to centre`, and the captured
  note-on and release references, which are unchanged.
- `preset_smoke` passes the old-record expansion with the ten envelope parameters
  defaulting to on, rates 80, 60, 50, 80, levels 0 and depth 0.
- The editor snapshot `editor-fxmod.png` was reviewed and regenerated for the 4 by 2
  grid and the pan envelope card, and `editor-patch.png` for the pan slider.
- The Linux image builds the plugin and runs 14 of 14 tests.

## Limits

- **The global chain is mono.** The DC blocker in the output stage runs on the left
  channel through the graph and is duplicated for the right with its own state. A
  second global audio effect would need a stereo version before it could sit there.
- **The web panel does not show the pan control.** The wasm engine receives the
  parameters and defaults, so its render is unchanged, but the browser part card
  stays as it was.
- **No key or velocity follow on the pan envelope.** The four stages and the depth
  are the whole control set.
- **Hard pan can raise one channel by 3 dB** relative to centre because the law keeps
  power constant with a unity centre. The output saturates at the sum.

## Risks and fallback

- The pan table is generated with the C file, so it cannot drift from the code. A
  change to the law means regenerating and re-accepting the render reference if the
  centre moves, which it must not.
- The fallback is to leave the pan at centre and the depth at 0, which is the
  shipped default and renders exactly like the mono build. The record layout keeps
  the parameters, so no second migration is needed.

## Safe experiment and stop criterion

Pan one part hard left, play a note, and listen for it in the left channel only.
Then set a fast attack to 99, a sustain level of 99 and a depth of 100 on a centred
part, and listen for the pan to move right and return on release. Stop if the centre
render changes by one sample, if the other part moves with it, or if the envelope
never returns to centre.
