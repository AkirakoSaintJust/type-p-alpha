# Feature contract: SH-101 filter law, resonance and input knee

## Identity

- **Feature:** An exponential cutoff law over 12 octaves, a linear resonance slider
  that self-oscillates near the top, a carrier-following output gate, the ladder's
  natural bass thinning, and a fixed soft knee on the filter input, all inside the
  existing four-pole `liquid_filter` node.
- **Brief section:** [`../design/filter.md`](../design/filter.md), with the reference
  in the Lemus SH-101 model at tag `ro-mono-101-v0.1.3`.
- **Owner / date:** P1Onken, 2026-11-18.
- **Disposition:** complete for the engine, the native editor and the migration, with the cutoff table, the resonance onset, the input knee and the
  old-patch migration measured, and the Linux editor snapshot goldens carrying the new
  filter card. I've kept the resonance and `comp` non-migration in Limits, because that
  is the part a player hears on an old patch.

## Player outcome

- **New player action:** set `cutoff` by hertz over about 3.5 Hz to 14.3 kHz and set
  `res` as a linear percentage. The filter envelope moves the cutoff through its own 0
  to 99 positions, and the passband makeup is internal, so the player does not manage
  it.
- **Sound and behaviour:** the cutoff knob is exponential, with its midpoint about
  224 Hz, so the musical range is spread over the travel rather than crowded into the
  top. Resonance self-oscillates from about 79 percent, matching the SH-101's VR18 at
  0.79 to 0.82, and the self-oscillation level rises from about 1,493 RMS at 80
  percent to 3,492 RMS at the top in the integer node model.
  The output gate follows the carrier input, so the loop's tone is heard while the
  operators are sounding and fades out when they stop. The self-oscillation stays a
  character of the loop rather than a free-running source, which is the behaviour a
  player expects from a filter after an amplifier envelope.
  The SH-101 puts part of the passband loss back at the filter output, summing
  `TR26`'s non-inverted emitter signal with `OUT4` so the VCA sees
  `(1 + a) * OUT4 / 152`, and at `a = 0.75` the reference's passband is -8.8 dB with
  that sum and -13.7 dB without.
  The engine's internal `comp` is 22 percent of the full `1 + res_k` makeup, which
  gives -8.95 dB at 75 percent and -9.69 dB at 100 percent in the ladder, and the
  input soft knee is transparent below about -6 dBFS and compresses to about -29 dB
  THD at 0 dBFS.
- **Failure mode:** an old patch loads with a different cutoff, the filter
  self-oscillates far from 79 percent, the cutoff law is not exponential, a clean
  patch is dirtied by the input knee, the internal makeup removes too much level, the
  gate lets the loop sound with silent carriers, the gate chatters on a quiet patch, or
  the top of the cutoff range breaks into a parasitic oscillation.

## Dependencies and interactions

- **Other features:** the `liquid_filter` node and its `.dev` source, the
  `f1_cutoff_ks` and `fenv_ks` follow nodes, the `cutoff_pos` control node, the
  `mod_env` filter envelope, the `p1_math.h` helpers, the `math_tables.h` generator,
  the manifest and its display forms, the native filter card, the preset store, the
  plugin state and the editor snapshots.
- **Platform/API/HAL changes:** none, because the engine stays integer C, the core
  stays at 48 kHz, and the output rate bridge is unchanged.
- **Persistence/version impact:** `PRESET_VERSION` moves from 6 to 7, the plugin state
  version from 4 to 5, whilst the records
  keep their byte layout. On load, the old cutoff coefficient is mapped through the
  old one-pole frequency to the new log position, so a patch saved before the change
  keeps its cutoff, and the resonance and `comp` raw values are reinterpreted under
  the new law. Host automation lanes have no migration and their raw values shift.

## Parameter law

| Parameter | Range | Default | Display | Meaning |
| --- | --- | --- | --- | --- |
| `cutoff` | 0 to 55296 | 36958 | Hz | Log position, 3.5 Hz to 14.336 kHz over 12 octaves |
| `res` | 0 to 41942 | 16000 | percent | Linear resonance, self-oscillation at about 79 percent |
| `comp` | -32768 to 32767 | 7209 | raw | Internal, 22 percent of the `1 + res_k` makeup, hidden from the panel |

The filter envelope is the absolute `mod_env` family: `fenv_on`, `fenv_r1` to `r3`,
`fenv_l1` to `l3`, `fenv_relr`, `fenv_rell` and `fenv_mode`. It starts at the
key- and velocity-followed cutoff and replaces it while it is on. The parameters and
their migration are in [`feature-contract-envelopes.md`](feature-contract-envelopes.md).

The cutoff table at selected positions:

| Position | Frequency | Coefficient (Q16) |
| --- | --- | --- |
| 0 | 3.5 Hz | 30 |
| 9216 | 14 Hz | 120 |
| 23040 | 112 Hz | 954 |
| 36864 | 896 Hz | 7253 |
| 46080 | 3.584 kHz | 24541 |
| 50688 | 7.168 kHz | 39892 |
| 55296 | 14.336 kHz | 55296 (clamped from 55502) |

The table error is within 4 cents above 50 Hz, about 10 cents at 20 Hz, and larger
only at the sub-audio bottom.

## Evidence

- **Table and law:** `plugin/tests/filter_law.cpp` checks the endpoints, the monotonic
  coefficient, the migrated 909 Hz default, the 80 percent loop gain and the soft-knee
  symmetry, and it passes on Windows, Linux and the sanitizer build.
- **Engine output gate:** `plugin/tests/engine_parity.cpp` drives the filter at 90
  percent and at 48 percent. With the carriers sounding it measures 6,422 RMS at 90
  percent. It then mutes the operators, waits for the follower release and the gate,
  and measures 1.0 RMS at 90 percent and 0.0 at 48 percent, so the loop is silent once
  the carriers are silent. The same test checks that the filter still changes the part
  render while the carriers sound.
- **Integer node model:** a faithful model of the node before the gate, with a 30,000
  seed, gives 1,493 RMS at 80 percent, 3,066 at 90 percent and 3,492 at 100 percent,
  and the first audible onset is around 78 to 79 percent. Those are the loop's own
  levels, and the gate now decides whether they reach the output.
- **Input knee:** the same model gives -46 dB THD+N at -12 dBFS and -6 dBFS, -41 dB at
  -3 dBFS and -29 dB at 0 dBFS. The -46 dB figure is the integer floor, so a normal
  level stays clean, which is the part of the knee I care about most.
- **Migration:** `plugin/tests/preset_smoke.cpp` builds a version 6 record with the old
  8000 default, loads it through `PresetStore`, and checks that the cutoff lands on
  position 36958, the 909 Hz point, whilst a version 7 record keeps its raw value.
- **UI:** the native filter card reads `909Hz` and `38%` and keeps the `comp` trim
  off the panel, and the Linux `editor-fxmod.png` golden is regenerated. The filter
  envelope card carries its own on switch, rates, levels, release and mode. The host
  text for `cutoff` and `res` round-trips at 21 normalised points per parameter, which
  is the check the CLAP `param-conversions` validator makes.

## Bounds and resources

- The table is 257 `int32_t` values, 1,028 bytes, read once per control block per
  voice, and the interpolation is one division by 216 and one multiply per voice per
  block.
- The input knee adds four multiplies per sample per active voice, and the output gate
  adds one absolute value, two shifts, a compare and four Q15 multiplies per sample per
  active voice.
- The manifest grew from 1,614 to 1,676 parameters and the part block from 800 to 831
  across the later envelope work, and the node state gains `res_k`.
- The preset and state record sizes do not change.

## Limits

- No full IR3109 solver runs here, so the ladder's peak shape, self-oscillation
  spectrum and the compression above 10 kHz remain approximations.
- The gate threshold and release are fixed. The threshold is -60 dBFS and the follower
  releases over 42 ms, so a very quiet carrier can close the gate early.
- The filter is not a free-running oscillator. A patch that wants a sine from the loop
  alone needs a different source.
- The top coefficient is clamped at 55296, so the last 0.2 percent of the cutoff travel
  is flat at about 14.18 kHz.
- Input knee fixed, with no drive parameter to push it.
- Inside the Type P panel the `comp` trim is hidden and fixed at 22 percent, though it
  remains a module parameter and a host automation lane.
- Only `cutoff` and `res` get the human-readable host text, and the other parameters
  keep the host's numeric text, so their automation lanes are unchanged.
- Key follow still uses the proportional push, so 100 percent does not track at exactly
  one octave per octave.
- Resonance and `comp` raw values are not migrated, so an old patch keeps its cutoff
  whilst its resonance and level change.

## Safe experiment and stop criterion

If the self-oscillation onset moves more than a few percent from 79, adjust the `res_k`
numerator and re-run the node model and the engine output-gate check. If the gate
chatters on a quiet patch, lower the threshold or lengthen the release before the
default changes. If the
compensated passband is more than about 1 dB from the reference at 75 or 100 percent,
adjust the `comp` default and re-run the DC gain check. If a clean patch at -12 dBFS
gains audible distortion, reduce the fifth-order term. If the top of the cutoff range
shows a parasitic oscillation, lower the table clamp and regenerate. If an old preset
does not land on its frequency, fix the migration before accepting the next golden set.
