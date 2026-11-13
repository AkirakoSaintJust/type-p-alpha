# WebAssembly engine

The first web stage is the engine, not the panel. It compiles the same generated
C engine to a standalone WASM module and gates it against the native 48 kHz
capture before any audio or interface work.

## Build and parity

The Emscripten image pins the same compiler version as the parity check.

```sh
docker build -f docker/Dockerfile.wasm -t type-p-alpha-wasm .
docker run --rm -v "$PWD:/work" -w /work type-p-alpha-wasm sh -c "./wasm/build.sh"
```

The script writes `build/web/p1.wasm` and `build/web/reference.json`, then runs
`wasm/parity.mjs`. The test initialises the engine, plays note 60 on the omni
channel, renders 256 frames, releases the note and renders another 256 frames. It
compares all 1,024 interleaved samples with the native capture.

It also copies the app pages, the AudioWorklet and the manifest into `build/web`.
Serve that directory to try the audio path:

```sh
python3 -m http.server 8080 --directory build/web
```

The root `index.html` redirects to `panel.html`. The bare page is at
`http://localhost:8080/bare.html`; press `start audio`, then hold `note 60`. The
level slider writes `op1_level.p0` through the same message bridge the full panel
uses.

The engine always runs at 48 kHz. When the context reports another rate, the
worklet resamples at the output boundary. At 48 kHz the path bypasses the
resampler and stays exact. The status line reports the context rate. Open
`panel.html?rate=44100` to force the fallback and see the resampled path. The
browser smoke uses that query to check the 44.1 kHz output.

The browser smoke test uses Playwright. On Windows, run it against the installed
Edge:

```sh
npm install playwright
PLAYWRIGHT_CHANNEL=msedge node web/smoke.mjs
```

The bare page is at `bare.html`; the panel smoke is at `panel.html`. On Linux,
install Playwright’s Chromium and run both without a channel.

## Panel

`panel.html` is the full parameter panel. It loads `params.json`, builds the six
tabs from the manifest, and sends every knob through the worklet. The page uses
the mockup’s CSS, extracted into `panel.css`. The header carries the algorithm
graph, the output scope and spectro toggle, and the patch name. The toolbar
carries the operator and part mutes, the part switch and the part library action.

The patch settings page has the two part cards with routing, tune, transpose,
delay, level, rename, copy and paste. The FX page splits into the six mockup cards:
filter, filter envelope, pitch LFO, pitch envelope, key follow and velocity
follow. The follow cards list every target in a selector and wipe all key or
velocity amounts at once.

The patch and part library keeps 64 multi-patches and 128 part slots. A
multi-patch stores two part slots and the routing, so loading one restores both
parts and their MIDI routing. The library lives in `localStorage` and exports to
or imports from a JSON wrapper with the mockup's `type-p-alpha-library` name and
slot arrays. The operator pages carry a
copy and paste pair in each column header, and the operator scopes come from the
engine tap buffers for the active part.

The panel also carries the envelope graphs, the sixteen-bar spectrum editors, the
`from wave` preview and the `op spectra mod` A/B display with the waveform overlay
and morph marker. The first edit of a spectrum card or the first morph knob move
commits the preview and switches the waveform to spectrum.

Open `http://localhost:8080/panel.html`. The root redirects there. The bare page
remains at `http://localhost:8080/bare.html` for the smallest audio and parameter
smoke test.

The module is about 164 KB with no imports. It exports `memory` and the wrapper
functions. Memory is fixed at 32 MB, so the worklet does not need to refresh typed
arrays during playback.

## Seam

`wasm/p1_wasm.c` owns one static engine state and a fixed interleaved output
buffer. The worklet calls:

```text
p1_create, p1_reset
p1_set_param, p1_get_param
p1_note_on, p1_note_off, p1_all_notes_off, p1_set_bend
p1_render, p1_output, p1_tap
p1_param_count
p1_analyze, p1_analysis
```

`p1_output` returns `frames * 2` int32 samples. `p1_tap` returns one of the ten
256-frame tap buffers for the scopes. `p1_analyze` turns one mip-0 wave table into
sixteen normalized harmonic magnitudes for the `from wave` preview. The parameter
ids are the engine ids in `engine/gen/params.json`.

## Next stage

The AudioWorklet owns the module and the engine state. The main thread compiles
the module, posts it to the worklet, and sends parameter and note messages. The
open work is the cosmetic pass against `design/mockup.html` and the operator and
spectrum copy and paste pairs. The panel is served from the same build tree, so a
change to `web/` needs a rebuild to reach `build/web`.
