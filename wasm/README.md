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

It also copies the bare page, the AudioWorklet and the manifest into `build/web`.
Serve that directory to try the audio path:

```sh
python3 -m http.server 8080 --directory build/web
```

Open `http://localhost:8080`, press `start audio`, then hold `note 60`. The level
slider writes `op1_level.p0` through the same message bridge the full panel will
use. The status line reports the context rate, so a context that refuses 48 kHz is
visible rather than silent.

The browser smoke test uses Playwright. On Windows, run it against the installed
Edge:

```sh
npm install playwright
PLAYWRIGHT_CHANNEL=msedge node web/smoke.mjs
```

On Linux, install Playwright’s Chromium and run it without a channel.

## Panel

`panel.html` is the full parameter panel. It loads `params.json`, builds the six
tabs from the manifest, and sends every knob through the worklet. The page uses
the mockup’s CSS, extracted into `panel.css`, and the generic cards group the
parameters by page. The operator pages carry a per-operator tap scope, taken from
the engine tap buffers for the active part.

Open `http://localhost:8080/panel.html`. The bare page remains at
`http://localhost:8080/` for the smallest audio and parameter smoke test.

The bespoke envelope graphs, the spectrum bar editor and the patch library are the
next visual layer. The panel currently exposes every parameter and proves the full
message bridge and the tap path.

The module is about 157 KB with no imports. It exports `memory` and the wrapper
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
```

`p1_output` returns `frames * 2` int32 samples. `p1_tap` returns one of the ten
256-frame tap buffers for the scopes. The parameter ids are the engine ids in
`engine/gen/params.json`.

## Next stage

The AudioWorklet owns the module and the engine state. The main thread compiles the
module, posts it to the worklet, and sends parameter and note messages. The panel
then reuses the mockup components, driven by `engine/gen/params.json` and the
control map, rather than the mockup’s hardcoded state.
