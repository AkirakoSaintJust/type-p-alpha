# Type P.α

Hey, it's Akirako Saint-Just from the パンク派第１音響合成研究班, or Punk Faction, Sound Synthesis Research Unit Number 1 (P1Onken for short.) Through my commitment to punk, post-punk, techno and industrial politics and aesthetics, I've come to be a fanatic of DX-style phase modulation synths, so over the years I've been accumulating ideas about things I wish I had in a PM synth, which features from which DX-style synths are best, and how to balance ease of use with synthesis power. My inspirations are what I've learned from playing the Yamaha synths and emulations I've used, and in general Yamaha's OPN, OPM, OPP, OPL3 and OPZ chips plus the Reface DX for phase modulation. I've also been curious about how other techniques compose with PM, whether they offer anything of interest, and how to implement them without overcomplicating patch design. On that front, my references are Roland's D50 and D110 and Casio's CZ series for the character phase distortion sounds, Kleimola-Lazzarini-Timoney-Välimäki's paper on vector phaseshaping for details about phase distortion DSP, Xenakis' granular and stochastic techniques, Nonlinearcircuits' chaos modules, and the Kawai K5 for my implementation of spectral synthesis.

_Type P_, where _P_ stands for _phase_, is the name for the general project of developing this kind of expanded DX-style++ synth. I've tried several experimental implementations, like _Type P.a_, which was implemented in allocation-free C# and had a complex modular architecture, _Type P.b_, which was radically simplified following lessons taken from _Type P.a_; and _Type P.fm1_, which was constrained by the CPU, memory and UI limits of the M-VAVE FM1. **Type P.α** (_α_ as in _alpha_, distinct from the _a_ in _Type P.a_) is the current iteration, presented as a VST3 plugin, a CLAP plugin and a standalone app, made with JUCE and using my personal framework tooling.

The reasons for abandoning Type P.fm1 are that I've decided to disengage from the custom firmware community for highly personal and slightly political reasons, and that being free from the FM1's limited screen, controls and CPU power allows for a kind of research into synthesis techniques that I really want to try.

## Current synth architecture

**Type P.α** has 24 voices split across two four-operator parts. Each operator contains:

- The usual output level, frequency ratio and detune controls.
- Bipolar phase modulation feedback, with a dedicated LFO and envelope.
- A set of 14 waveforms: the full OPL3 and OPZ/TX81z/DX11/V50 set, plus `rect tri` and `half square`, with the tables generated from formulas rather than copied from the chips.
- An amplitude LFO over a warm-up, a surge and two decay stages, with four modes.
- A pitch LFO for evolving PM timbres.
- An additive spectral mode with two 64-harmonic tables, an LFO and envelope for morphing between them, and spectral controls inspired by the Kawai K5.
- A rate fade and a depth fade on every LFO, so a tremolo, vibrato or morph can arrive after the note starts.

Parts contain:

- A bipolar pitch envelope.
- A pitch LFO.
- A polyphonic, multimode, resonant filter with a dedicated envelope.
- Key scaling and velocity sensitivity for a lot of params.
- Detune and transposition.
- One stereo panning parameter with a panning envelope.

Parts can be stacked, assigned to different MIDI channels or split into keyboard zones, and each has a delay stage so they can be sequential in time. The pitch bend range is per part, 0 to 24 semitones, and defaults to 2.

The spectral synthesis implementation is the bit I'm the most proud of. My starting points were Yamaha's OPx/DX parallel carrier algorithms and the Kawai K5, but where the K5 has two overlapping spectra and four envelopes per part that can be assigned to individual harmonics, Type P.α has two spectra for each of the four operators, times two parts, and each operator has its own morph envelope and LFO on top of its amplitude envelope and LFO.

I believe two 4-operator parts is the sweet spot for balancing usability and power. Compared to 6-operator synths, the number of algorithms is smaller, and I feel like dividing patches into two 4-op blocks makes programming way less overwhelming than presenting users with one 8-op part. I also believe offering feedback on every operator, along with additional waveforms, most of the time makes up for the kind of 6-op algorithms Type P.α lacks. Most importantly, it's an architecture that lends itself well to assembling patches out of stacked or sequential partials, which to me is the ideal patch design approach for phase modulation.

I've also tried to make sound design easier by restraining components to a relatively small maximum parameter count, making sure the state of every operator can be compared and edited from a single page, ensuring mutes are accessible from any page, and providing visual guidance through envelope graphs, spectrograms and oscilloscopes.

Phase modulation indices are calibrated to be similar to Yamaha's OPN, OPM, OPL and OPZ families, rather than the softer sound of synths like the DX7, DX9 and Reface DX. Feedback scales with the operator's amplitude, as in Yamaha's traditional DX synths and unlike the Reface DX, but like the Reface DX's feedback, it is bipolar and the negative (or more accurately, rectified) half adds odd harmonics when applied to a sine wave. Feedback also has an LFO and envelope per operator, which I think is a powerful addition. I'm well chuffed with how it works but I wonder whether having a feedback envelope would make more sense with Reface DX-style amplitude-independent feedback? Anyway, for more details, see the [operator feedback](plugin/feature-contract-feedback.md) and [operator LFOs](plugin/feature-contract-operator-lfos.md) contracts, and the [the feedback research study](docs/research/README.md).

Results so far seem to be fit for intent. I've been able to easily make rudimentary additive patches that sound a bit like the PPG sounds Tangerine Dream used circa Exit, additive brass, woodwinds and strings; and pseudo-PWM pads and leads by pairing saw operators with sine operators at 20-35% feedback, which gives them a ramp waveform. Conversion of OPM patches to Type P ranges produces spectrally similar results to the originals, and I reckon they are easy to edit. Nobody else has played or heard it yet, and I haven't designed a test.

## Potential things I'm considering

1. A sustain stage on the LFO rate and depth fades?
2. Documentation and graphic design. The panel is mostly ok, but I'd like to work on the wordmark, a nicely designed and human-written manual, and complementary materials such as posters, web graphics, etc. Also refine the panel.

## Done

1. More precise spectrograms, with clear differentiation of odd and even harmonics, and maybe some numbers for easier reading. Being able to reliably see the spectra is essential for additive-style synthesis.
2. A 64-harmonic spectral oscillator per operator, with two editable spectra, a morph envelope and a morph LFO, and K5-style editing controls expanded and modified for the Type P (see the [additive synthesis note](docs/research/additive-synthesis.md) and [the spectral contract](plugin/feature-contract-spectral.md)).
3. A 48 kHz core with a 128-tap output bridge, so a 48 kHz host takes the core output untouched and the other rates are resampled at the boundary. See the [48 kHz contract](plugin/feature-contract-48k-core.md).
4. Giving the filter an SH-101-ish character (see [the filter law](plugin/feature-contract-filter-law.md) contract.)
5. Envelope redesign.
6. Per-part stereo panning.
7. Library synchronisation across running instances.
8. Rate and depth fades on every LFO, with the envelope rate table's time scale and a two-line graph.
9. Per-operator stochastic phaseshaping, with a live composed map, zero-crossing locks, drift, flips, a local reorder, and a dedicated LFO and signed envelope on the `op phasemap` tab (see [the stochastic phaseshaping contract](plugin/feature-contract-stochastic-phaseshaping.md)). The shipped read runs at 1× and uses the exact 1024-entry post-F table (`P1_PHASE_READ=11`), which is bit-exact against the F/segment read. The 2× half-band was designed but not built: the current 1× worst case is about 1.5× the phase-off baseline, and the aliasing would need a fresh CPU budget.
10. Six macros with relative CC, wheel and velocity sources, per-voice operator levels, feedbacks, morphs, envelope rates and LFO rates and depths, and a macros page (see [the macro contract](plugin/feature-contract-macros.md)). The per-voice breakpoints and pan note policy are still to come.
11. Ring modulation between consecutive carriers, with three pairs, a per-pair level and one original-output gate per carrier (see [the ringmod contract](plugin/feature-contract-ringmod.md)).

## Empyrical stuff I'd like to try

1. Replicating DX7 patches to find the limitations of Type P.α's architecture in comparison to 6-op architectures.
2. Having other people try the synth and provide feedback on UX and sound.
3. Trying wild PM techniques from chiptune composers like blobnote.
4. Making more spectral patches and finding which other K5-style editing tools might be worth adding.
5. Documenting the pseudo-virtual analogue experience.

## Graphic design references for future work

I'd like to make a better wordmark and complementary materials such as posters, a web with custom assets, etc. The panel is mostly there but I guess everything can always be refined.

I don't know which direction to take with the wordmark yet, so I've still got to experiment. A cut-up and layered Rexlia is one candidate, or maybe a mix of cut-up typefaces. The alpha can be built from existing font components, since most free fonts have no Greek glyphs. The mark will probably work better as vector art than as an embedded font, which also makes the licensing simpler.

References:

- A Novarese (Eurostile, Stop)
- Openmind (Amon Tobin's Foley Room)
- R Mitchell (Corporate)
- The Designer's Republic (Wipeout and Autechre covers)
- N2-Antireal (Terminal)
- Maniackers
- Nakamura Y and Hashimoto K  (Gona)
- VDL (Line, Giga)
- R Larabie (Rexlia)

## Compiling

The build fetches JUCE 9.0.5 and the pinned `clap-juce-extensions` commit at
configure time. It also needs CMake 3.22, Ninja, clang, and the platform JUCE
dependencies. The current manifest is 2,053 parameters (994 per part) and the
preset record is version 19.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld \
  -DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld \
  -DCMAKE_MODULE_LINKER_FLAGS=-fuse-ld=lld
cmake --build build
ctest --test-dir build --output-on-failure
```

Add `-DP1_MODERN=ON` for the AVX2 / arm-tuned build. The phase-map read is
selected by `P1_PHASE_READ`. The default `11` is the exact 1024-entry post-F
table, and `0` keeps the F/segment reference.

On Debian or Ubuntu, install the toolchain and the build packages first:

```sh
sudo apt install clang lld cmake ninja-build libasound2-dev libfreetype6-dev \
  libx11-dev libxext-dev libxinerama-dev libxrandr-dev libxcursor-dev \
  libxcomposite-dev libxrender-dev
```

The Docker image and the Windows script wrap the same steps:

```sh
docker/build.sh linux/amd64
```

```powershell
.\windows\build.ps1
.\windows\build.ps1 -Modern
```

The compiler is clang on every platform, so the Windows script uses clang-cl with
lld-link and macOS uses AppleClang. The modern Windows package is
`Type-P.alpha-windows-x86_64-avx2.zip`. The baseline package is
`Type-P.alpha-windows-x86_64.zip`, and the release also attaches the modern
universal macOS package `Type-P.alpha-macos-universal-modern.zip`. See
[`docker/README.md`](docker/README.md) for more info on the platform matrix,
sanitizer build and clang tooling.

The plugin compiles into `build/plugin/juce/type_p_alpha_artefacts/Release/`:

- `VST3/Type P.alpha.vst3`
- `CLAP/Type P.alpha.clap`
- `Standalone/Type P.alpha`

The tests cover engine parity, the rate bridge, preset storage, processor
smoke test, stability pass, editor snapshots, the phase-map perceptual battery,
and the Linux DAW matrix. The `linux_daws` test needs Ardour, Carla, JACK and Xvfb.
Use `ctest -E linux_daws` when those hosts are not installed. `p1_ui_frame_bench`
is a manual frame benchmark for the live editor. It is built but not a timing gate.

## Code map

The hand-written types and functions, the generated files and the cleanup
findings are detailed in [`docs/codebase-audit.md`](docs/codebase-audit.md). The visual
language, its references and the panel rules are at
[`design/README.md`](design/README.md).

## Installation

The release packages contain the VST3 and CLAP plugins, the standalone app, and the licences. Copy
the VST3 or CLAP into your plugin folder, or just run the standalone app. The
modern AVX2 Windows package is built with `windows/build.ps1 -Modern`. The release
workflow attaches the baseline and modern Windows packages, and the modern macOS
universal.

## Licensing

The source is GPL-3.0-only. The plugin binary also links JUCE 9 under the
AGPL-3.0-only terms, and the bundled fonts are SIL Open Font License 1.1. Read
[`LICENSING.md`](LICENSING.md) before packaging a binary. The repository is the
corresponding source for every release. Release packages carry the licensing notices
in `licences/`.

## Status

The plugin passes pluginval at strictness 5 on Linux x86_64 and Windows x86_64,
with the editor open and the audio, state and automation checks included. The CLAP
validator reports no failures and one warning, because its random-state test loads
garbage without crashing. The Linux suite runs in CTest, and the Windows clang-cl,
macOS universal, macOS arm and arm64 Linux builds pass in CI. The modern macOS
universal package is built and validated by the macOS-modern job. The Linux DAW
matrix runs when Ardour, Carla and JACK are installed, but the macOS DAW validation
is still untested, pending access to an actual Mac.
