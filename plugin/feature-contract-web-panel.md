# Feature contract: web panel on the WASM engine

## Identity

- **Feature:** A browser panel that drives the same generated C engine through a
  narrow WebAssembly seam, with the mockup's CSS and six-tab layout, the two-part
  patch page, the six-card FX page, the spectral editors, a patch and part library,
  and an output resampler for context rates other than 48 kHz.
- **Brief section:** [`../design/README.md`](../design/README.md),
  [`../design/mockup.html`](../design/mockup.html),
  [`../design/juce.md`](../design/juce.md) and
  [`../plugin/feature-contract-spectral.md`](feature-contract-spectral.md).
- **Owner / date:** P1Onken, 2026-11-13.
- **Disposition:** complete for the web target. The panel exposes every manifest
  parameter across the six tabs. The browser smokes pass on the local build, and the
  GitHub `wasm` job passed on commit `4ecbcb5` with both browser smokes.

## Player outcome

- **New player action:** open the app, press `start audio`, and play the hold note or
  a host that feeds the page. Choose a part, edit its patch, FX, operator, LFO,
  spectrum and morph settings, watch the real output and tap scopes, then save the
  patch to a multi slot or the part to a part slot. Export the library as JSON and
  import it in another browser.
- **Sound and behaviour:** the engine renders at 48 kHz. At a context rate of 48 kHz
  the worklet copies the engine output with no resampling. At another rate the
  worklet advances through the engine stream at `48000 / rate` and linearly
  interpolates, so pitch and envelope time stay correct at 44.1, 96 and 192 kHz.
  Pasting a wave analysis commits the sixteen a levels and then waveform 13. The
  first bar, macro, norm or copy edit of the A card commits the preview, and the
  first move of the morph knob or a spectrum spring control commits it as well.
- **Failure mode:** a context rate other than 48 kHz plays flat or slow, the panel
  drops a manifest parameter, a stored part or multi loses its routing, the library
  fails to survive a reload, the preview writes the patch on page open, or the
  48 kHz path starts to resample.

## Dependencies and interactions

- **Other features:** the 48 kHz core, the 24-voice allocator, the harmonic ranges,
  the wavetable mip policy, the preset store shape and the editor page grouping.
- **Platform/API/HAL changes:** none in the engine. The new surface is the WASM seam
  in `wasm/p1_wasm.c` and the AudioWorklet in `web/p1-worklet.js`. The module imports
  nothing and uses 32 MB of fixed memory.
- **Persistence/version impact:** none in the engine. The web library is JSON in
  `localStorage` under `type-p-alpha-web-library`, with the mockup's
  `type-p-alpha-library` shape, 64 multi slots and 128 part slots. A slot stores the
  part parameters by engine id, so it follows the manifest and includes the sixteen
  spectrum levels of both spectra.
- **MIDI/clock impact:** none. The page sends note, all-notes-off and bend messages
  through the worklet.

## Parameter and event semantics

The panel reads `engine/gen/params.json` at load. Every control writes the raw value
of one manifest parameter through the worklet, and the worklet returns the engine's
stored value. The page groups the 846 parameters as:

| Page | Controls |
| --- | --- |
| patch settings | 16 |
| part fx/mod | 26 |
| operator envs | 132 |
| operator lfos | 68 |
| operator spectra | 32 |
| op spectra mod | 68 |

The FX page keeps the six mockup cards. Filter cutoff, resonance, envelope amount,
compensation and the DC blocker sit in the filter card with the type select and the
on switch. The filter envelope, pitch LFO and pitch envelope keep their graphs. The
key follow and velocity follow cards select one target at a time from fifteen
targets and wipe every amount at once. The breakpoint, key sensitivity and velocity
sensitivity are global parameters and appear once.

The patch page carries the two part cards with channel, key zone start and end,
tune, transpose, delay and level, plus rename, copy and paste. The voice mode bar
carries the poly and mono buttons, legato, unison count, unison detune, portamento
and note priority.

Each operator column carries its oscillator card on the operator envs, operator
lfos, operator spectra and op spectra mod pages. The operator header carries the
active and mute switch, and it writes the same operator mute as the toolbar box.
Every operator sub-card has a copy and paste pair in its header, so a card copies
across operators and parts. The spectrum a and b cards copy their sixteen levels
for another operator, which is the way to move a spectrum across the four
operators. The range card copies both ranges and the morph.

Each spectrum card carries the group selector, the live, dead and both mask, and
the spring-loaded tilt, level and formant controls. The group sets which harmonic
series a tilt or level gesture writes: all, odd, even, the octave series, or the
series on the third, fifth, seventh, fifteenth, eleventh, thirteenth, ninth,
twenty-first, nineteenth or seventeenth harmonic. The mask narrows that to the
sounding members, the silent members, or both. The bar labels show every harmonic
in the current range, and the members of the select group are green. The legend
names the group and mask and lists the members inside the range. A spring control
writes the spectrum as it moves and returns to centre on release. Tilt up boosts
the higher harmonics, level up raises the selected harmonics, and formant moves
the Gaussian around the selected harmonic. The formant ignores the group and mask.
The group and mask are editor state for those gestures, so they add no manifest
parameters. Every gesture writes the existing `opN_spec_a1` to `a16` and `b1` to
`b16` parameters in the `osc` node. A patch can still be rebuilt from the node and
macro library alone.

The library snapshots a part as `{ name, params }`, where `params` is a list of
`[id, value]` pairs for every parameter with that part suffix, so the spectrum
levels travel with the part. A multi snapshots `{ used, name, partSlots, routing }`.
A multi save materialises every part that does not have a slot, so the two
references always point at saved parts. Loading a multi applies the two part slots
and then the stored routing, so the multi wins over the part's own routing. An
import refuses a used multi whose part references are missing, out of range, or
empty, so a patch bank cannot point at parts that are not in the same file.

The worklet messages are `module`, `params`, `param`, `part`, `note`, `allNotesOff`,
`bend` and `analyze`. It answers with `ready`, `param`, `scope`, `taps`, `analysis`
and `error`.

## Bounds and resources

- **Maximum voices/events/effects:** 24 voices, two parts, ten tap buffers.
- **Per-sample and per-block operations:** the 48 kHz path does one engine render
  and one copy. The resampled path does one engine render, one linear interpolation
  per output sample and one array compaction per block. Taps are copied at the engine
  block size and posted every eight ring fills.
- **Instance memory, CPU, preset size:** the module is 164,298 bytes with no imports
  and 32 MB of fixed memory. The part snapshot is one number per part parameter,
  about 400 pairs. The library bound is 64 multis and 128 part slots.
- **Overflow/overload policy:** the resampler needs `ceil(frames * 48000 / rate) + 1`
  engine samples per block, which stays under the 4,096-frame seam buffer at 128- or
  256-frame blocks. The tap ring caps each copy at 512 samples. A full ring starts a
  new fill and counts a block.
- **Division, shift, and range analysis:** one division per block for the rate ratio
  and one multiply-add per output sample. No engine arithmetic changes.

## Files and interfaces

- **Seam:** `wasm/p1_wasm.c`, `wasm/build.sh`, `wasm/parity.mjs` and
  `wasm/README.md`.
- **Web:** `web/panel.html`, `web/panel.js`, `web/panel.css`, `web/p1-worklet.js`,
  `web/bare.html`, `web/main.js`, `web/smoke.mjs` and `web/panel-smoke.mjs`.
- **Manifest and tokens:** `engine/gen/params.json` and `design/tokens.css`.
- **Design:** `design/mockup.html`, `design/README.md` and `design/juce.md`.
- **CI:** `.github/workflows/ci.yml`, job `wasm`.
- **Public contract:** the WASM exports `p1_create`, `p1_reset`, `p1_set_param`,
  `p1_get_param`, `p1_note_on`, `p1_note_off`, `p1_all_notes_off`, `p1_set_bend`,
  `p1_render`, `p1_output`, `p1_tap`, `p1_param_count`, `p1_analyze` and
  `p1_analysis`.

## Verification

- **Host vector(s):** the WASM parity gate renders note-on and release at 48 kHz and
  compares all 1,024 interleaved samples with the native capture. It also checks the
  sine, saw and high-range analyses. The module reports 846 parameters.
- **Browser vector:** `web/smoke.mjs` drives the bare page and checks a level round
  trip. `web/panel-smoke.mjs` drives the root redirect and checks the six tabs, the
  saw preview, the bar commit, the morph commit, the select group legend, the mask,
  the level, formant and tilt springs and their return to centre, the output and tap
  peaks, the algorithm round trip, the operator copy and paste, the operator mute
  switch and its toolbar box, the four oscillator cards on each operator page, the
  card copy and paste, the spectrum copy across operators, the part switch, the six
  FX cards, the part rename and paste, the part and multi library save, the multi
  save materialising both parts, the spectrum level after the reload, the patch
  rename, the reload persistence and the 44.1 kHz resampled path.
- **Target package check:** `wasm/build.sh` writes `build/web` with the module, the
  manifest, the tokens and the pages. The app is served with
  `python3 -m http.server 8080 --directory build/web`.
- **Host timing and platform check:** the local Windows smokes use the installed
  Edge. The Linux Chromium run passed in the `wasm` CI job on commit `4ecbcb5`.
- **Tolerance/metric:** the 48 kHz output is sample exact against the native capture.
  The browser checks are thresholds rather than exact audio: a scope peak, tap peak
  and route peak above 0.0001, a preview whose second harmonic is above 10, a
  spectrum bar value above 0, waveform 13 after the first edit, and the six page
  control counts.
- **Evidence artefact:** the parity log, the two smoke logs, the measured page
  counts and the four screenshots in `build/web` from the local run.

### Measured evidence

1. **WASM parity:** 846 parameters, note-on and release match the 48 kHz capture,
   the sine analysis passes with the first harmonic at least 90 and the second at
   most 5, the saw analysis passes with the first harmonic at least 80 and the
   second at least 20, and the high range passes at 50 or more. Module size 164,298
   bytes, no imports.
2. **Bare smoke:** 48 kHz, scope peak 0.089, `op1_level.p0` round trip 42.
3. **Panel smoke:** pages `patch:16 fx:26 envs:132 lfos:68 spectra:32 mod:68`,
   preview `[99, 49, 33]`, bar value 74, morph 15, a card copy that copied the amp
   LFO rate to the second operator, a spectrum copy from op1 to op2, the `third P5`
   legend with members 3, 6 and 12, a dead-mask level spring that set those three to
   66 and left h4 at 0, a formant spring that set h5 to 66, a tilt spring that took
   h3 from 93 to 85 and h12 from 66 to 34, every spring back at 0%, output peak
   0.079, operator tap peak 0.078, route peak 0.078, algorithm 7, operator copy 75
   to 75, operator mute 1 with the toolbar box marked muted, part two rebuilt, six FX
   cards with two follow cards, four oscillator cards on each operator page, the
   multi save materialising both parts (2 part slots), the spectrum h3 at 85 after
   the reload, patch name `smoke patch`, persisted name and slots after reload,
   44.1 kHz context with the resampler and a peak of 0.0861.
4. **Release evidence:** the GitHub `wasm` job passed with the bare and panel
   smokes on Chromium. The release package and its binaries remain.

## Risks and fallback

- **Range and aliasing:** the resampler is linear, so a 44.1 kHz context adds a small
  high-frequency roll-off and some aliasing above 20 kHz. The intended quality is the
  48 kHz path. A windowed-sinc resampler is the refinement if a listening test asks
  for it.
- **Top range edge:** the panel shows the same 113-128 limit as the native editor.
- **Assumptions/open research:** the browser library is JSON, not the plugin's
  `.p1lib` bank. The export/import shape is the mockup's JSON, so the browser and the
  plugin do not share files yet.
- **Safe experiment and stop criterion:** the panel reads the manifest at load and
  reports a missing parameter rather than hard-coding a list. If the WASM parity gate
  fails, the web build stops and the last module stays.
- **Smaller alternative:** ship the bare page and the tap scopes without the library
  if the JSON persistence cannot be made durable, which keeps the audio path and the
  parameter bridge and leaves the library out of the release.
- **Rollback:** restore `web/` and `wasm/` from the previous commit, remove the
  `wasm` CI job and rebuild the image. The engine and plugin targets are unaffected.
