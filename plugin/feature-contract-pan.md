# Feature contract: per-part panning and pan envelope

## Identity

- **Feature:** Place each part in the stereo field with a constant-power pan and a
  three-stage absolute pan envelope, both per part.
- **Brief section:** the part card in [`../design/README.md`](../design/README.md)
  and the part fx/mod page.
- **Owner / date:** P1Onken, 2026-11-20.
- **Disposition:** complete. The Linux suite passes 15 of 15 with the slider, the
  card and the version 12 pan remap, and the full CI matrix is green.

## Player outcome

- **New player action:** each part card gains a `pan` slider under the level
  slider, from -100 to 100 with centre at 0. The part fx/mod page gains a pan
  envelope card with an on switch, three rates, three levels, a release rate and
  level, a mode and the envelope graph.
- **Sound and behaviour:** the parts are summed into left and right buses rather
  than one mono bus, with constant-power gains. Centre is unity on both channels,
  so a patch that never touches pan renders exactly as before. The envelope starts
  at the part's pan knob, moves through absolute levels, holds or loops by mode,
  and releases to the release level. It retriggers when a note starts on the part
  and releases when its last voice releases.
- **Failure mode:** a pan change that moves the existing centre render, a hard-panned
  part leaking into the other channel, a pan envelope that never releases, a release
  level that is ignored, or a mode that holds when it should release.

## Dependencies and interactions

- **Other features:** the voice sum, the global DC blocker, the part card, the part
  fx/mod page, the parameter manifest, the part record and the editor snapshots.
- **Platform/API/HAL changes:** `render_voices_block` takes a second bus, and
  `render_post_block` takes left and right sources. The engine stays integer C.
- **Persistence/version impact:** the pan slice added the ten envelope parameters at
  `PRESET_VERSION` 10, moving the part block from 802 to 812. The envelope family
  later changed the fourth level and depth into a release level and a mode, which is
  the version 12 pan remap. The current manifest is 1,676 parameters with an 831 part
  block, `PRESET_VERSION` 14 and state version 12. Old libraries are remapped, not
  rejected.
- **MIDI/clock impact:** none. Pan is a parameter and a control-rate envelope.

## Parameter and event semantics

- `pan.p0` and `pan.p1` are -100 to 100, default 0.
- The envelope parameters are `pan_env_on` (0 or 1, default 1), `pan_env_r1` to
  `pan_env_r3` (0 to 99, defaults 80, 60, 50), `pan_env_l1` to `pan_env_l3` (-99 to
  99, default 0), `pan_env_relr` (0 to 99, default 80), `pan_env_rell` (-99 to 99,
  default 0) and `pan_env_mode` (0 to 3, default 0). All are per part and stored in
  the part record.
- The envelope starts at the pan knob at note-on. The levels are absolute positions,
  so there is no depth control. Mode 0 releases after the third level, mode 1 holds
  it, and modes 2 and 3 loop between the second and third levels. The release runs
  to `pan_env_rell`.
- The pan law is constant power with unity at centre: the gain is
  `cos(theta) * sqrt(2)` on the left and `sin(theta) * sqrt(2)` on the right, with
  `theta` from 0 to a quarter turn as pan goes from -100 to 100. The gains come from
  a 201-entry Q16 table, because `p1_sin`'s 8-bit quarter table puts 45 degrees at
  0.994 instead of 1.0. Centre is exactly 65536 on both channels.
- The envelope runs once per part per control block. `pan_env_trig[p]` is set by any
  note-on that reaches the part, a part with no gated or delayed voices enters
  release, and the on switch leaves the pan at the knob while it is off.
- The voice sum applies the pan gains with a 64-bit multiply and a 16-bit shift, so
  the centre gain of 65536 is an exact pass-through.

## Evidence

- `engine_parity` passes `hard left puts part 1 on the left`, `hard right puts part 2
  on the right`, `centre pan is exactly unity on both channels`, `the pan envelope
  swings to the right` and `the release returns the pan to centre`, and the captured
  note-on and release references, which are unchanged.
- `preset_smoke` loads a version 10 record and a version 12 record. The version 10
  record maps the old r4 and l4 to the release rate and level and the depth to the
  mode; the version 12 record keeps the pan block through the later inserts.
- The editor snapshots `editor-fxmod.png` and `editor-fxmod-loops.png` show the pan
  card and its loop boundaries.

## Limits

- **The global chain is mono.** The DC blocker in the output stage runs on the left
  channel through the graph and is duplicated for the right with its own state. A
  second global audio effect would need a stereo version before it could sit there.
- **No key or velocity follow on the pan envelope.** The three stages, release and
  mode are the whole control set.
- **Hard pan can raise one channel by 3 dB** relative to centre because the law keeps
  power constant with a unity centre. The output saturates at the sum.

## Risks and fallback

- The pan table is generated with the C file, so it cannot drift from the code. A
  change to the law means regenerating and re-accepting the render reference if the
  centre moves, which it must not.
- The fallback is to switch the envelope off, which leaves the pan at the knob and
  renders exactly like the mono build. The record layout keeps the parameters, so no
  second migration is needed.

## Safe experiment and stop criterion

Pan one part hard left, play a note, and listen for it in the left channel only.
Then switch the envelope on with levels 99, -99 and 99, mode 2 and a slow rate, and
listen for the pan to loop across the field. Stop if the centre render changes by
one sample, if the other part moves with it, or if the release does not end at the
release level.
