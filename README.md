# Type P.alpha

Type P.alpha is a four-operator FM instrument for the desktop. It runs as a VST3,
CLAP or standalone build on Linux and Windows. The engine is the Type P.fm1 voice
engine, kept at its 44.1 kHz core with the rate adapted at the output boundary.

The instrument has two independent parts, 20 voices, four operators per voice, the
eight FM algorithms, a per-operator amplitude LFO, a part pitch LFO and pitch
envelope, a filter and filter envelope, and a 64 multi-patch and 128 part library.
The editor is the p1onken panel language in [`design/README.md`](design/README.md),
and the wordmark is `TYPE P.` in Zen Dots with a lowercase `a` in Unbounded.

This repository carries the plugin, the shared engine, the generated DSP, the design
tokens and the patches. Type P.fm1 remains in the umetate repository and receives
bug fixes only. The device firmware lives there, so this tree builds the desktop
instrument and nothing else.

## Build

The build fetches JUCE 8.0.9 and the pinned `clap-juce-extensions` commit at
configure time. It needs CMake 3.22, a C++20 compiler, Ninja, and the platform JUCE
dependencies.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
```

On Debian or Ubuntu, install the build packages first:

```sh
sudo apt install build-essential cmake ninja-build libasound2-dev libfreetype6-dev \
  libx11-dev libxext-dev libxinerama-dev libxrandr-dev libxcursor-dev \
  libxcomposite-dev libxrender-dev
```

The build writes the plugin into `build/plugin/juce/type_p_alpha_artefacts/Release/`:

- `VST3/Type P.alpha.vst3`
- `CLAP/Type P.alpha.clap`
- `Standalone/Type P.alpha`

The tests cover engine parity, the rate bridge, the preset store, the processor
smoke, the stability pass, the editor snapshots, and the Linux DAW matrix. The
`linux_daws` test needs Ardour, Carla, JACK and Xvfb. Use `ctest -E linux_daws` when
those hosts are not installed.

## Editor and sound

The editor follows the shared visual language. Pink is identity and the primary
value, green is live signal and active state, and purple is selection and
modulation. The scope, spectro, envelopes and routing graph are signals rather than
decoration. Every page keeps the same 1240 by 985 canvas and scales as one image.

The engine is the Type P.fm1 graph: four operators, eight algorithms, the part pitch
envelope, the filter, and the two-part multi-timbral layout. The rate bridge adapts
the 44.1 kHz core to the host rate at the output.

## Licensing

The source is GPL-3.0-only. The plugin binary also links JUCE 8 under the
AGPL-3.0-only terms, and the bundled fonts are SIL Open Font License 1.1. Read
[`LICENSING.md`](LICENSING.md) before packaging a binary. The repository is the
corresponding source for every release, and the release archives carry the notices
in `licences/`.

## Status

VST3 and CLAP pass `pluginval` at strictness 5 and the CLAP validator on Linux and
Windows. REAPER, Renoise, Ardour and Carla load the builds, and the Linux DAW
matrix runs in CTest. The macOS build and the AU target remain open.
