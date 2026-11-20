# Type P.α

Hey, it's Akirako Saint-Just from the パンク派第１音響合成研究班, or Punk Faction, Sound Synthesis Research Unit Number 1 (P1Onken for short.) Through my commitment to punk, post-punk, techno and industrial politics and aesthetics, I've come to be a fanatic of DX-style phase modulation synths, so over the years I've been accumulating ideas about things I wish I had in a PM synth, which features from which DX-style synths are best, and how to balance ease of use with synthesis power. My inspirations are what I've learned from playing the Yamaha synths and emulations I've used, and in general Yamaha's OPN, OPM, OPP, OPL3 and OPZ chips plus the Reface DX for phase modulation. I've also been curious about how other techniques compose with PM, whether they offer anything of interest, and how to implement them without overcomplicating patch design. On that front, my references are Roland's D50 and D110 and Casio's CZ series for the character phase distortion sounds, Kleimola-Lazzarini-Timoney-Välimäki's paper on vector phaseshaping for details about phase distortion DSP, Xenakis' granular and stochastic techniques, Nonlinearcircuits' chaos modules, and the Kawai K5 for my implementation of spectral synthesis.

_Type P_, where _P_ stands for _phase_, is the name for the general project of developing this kind of expanded DX-style++ synth. I've tried several experimental implementations, like _Type P.a_, which was implemented in allocation-free C# and had a complex modular architecture, _Type P.b_, which was radically simplified following lessons taken from _Type P.a_; and _Type P.fm1_, which was constrained by the CPU, memory and UI limits of the M-VAVE FM1. **Type P.α** (_α_ as in _alpha_, distinct from the _a_ in _Type P.a_) is the current iteration, presented as a VST3 plugin, a CLAP plugin and a standalone app, made with JUCE and using my personal framework tooling.

The reasons for abandoning Type P.fm1 are that I've decided to disengage from the custom firmware community for highly personal and slightly political reasons, and that being free from the FM1's limited screen, controls and CPU power allows for a kind of research into synthesis techniques that I really want to try.

## Current synth architecture

**Type P.α** has 24 voices split across two four-operator parts. Each operator contains:

- The usual output level, frequency ratio and detune controls.
- Bipolar phase modulation feedback, with a dedicated LFO and envelope.
- A set of 13 waveforms more or less modelled after the OPL3 and the OPZ/TX81z/DX11/V50.
- An amplitude LFO on top of the usual amplitude envelope.
- A pitch LFO for evolving PM timbres.
- An additive spectral mode with two 64-harmonic tables, an LFO and envelope for morphing between them, and spectral controls inspired by the Kawai K5.

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

Results so far seem to be fit for intent. I've been able to easily make rudimentary additive patches that sound a bit like the PPG sounds Tangerine Dream used circa Exit, additive brass, woodwinds and strings; and pseudo-PWM pads and leads by pairing saw operators with sine operators at 20-35% feedback, which gives them a ramp waveform. Conversion of OPM patches to Type P ranges produces spectrally similar results to the originals, and I reckon they are easy to edit. Confirmation from external testers is still pending though.

## Lines of research

1. Phase distortion and vector phaseshaping. Where should the transfer function's
   breakpoint be placed? Can phase distortion and phase modulation share one single phase
   transfer function without extra machinery? To be honest I'm not too eager to implement it because I'm unsure it'll add much for the effort, but I guess it's part of the "phase-based synthesizer" research concept.
2. Phase granulation as a layer on top of PM and PD, with chaotic and stochastic
   modulators. What should it do exactly, how much RAM and compute does it cost, and which parameters would be the most user-friendly?
3. LFO envelopes (or fade-in and fade-out, with a possible sustain mode) for rate and depth.
4. Control macros. What kind of macro controls would make the synth friendlier, more performable, and more modulatable, without turning the panel into an overwhelming mod matrix?
5. Documentation and the graphic design. The panel is already ok, but I'd like to work on the wordmark, a nicely designed manual, and complementary materials such as posters, web graphics, etc. Also refine the panel if needed, although I'm not unhappy with it.
6. Would ring mod algorithms add anything worth implementing?

## Done

1. More precise spectrograms, with clear differentiation of odd and even harmonics, and maybe some numbers for easier reading. Being able to reliably see the spectra is essential for additive-style synthesis.
2. A 64-harmonic spectral oscillator per operator, with two editable spectra, a morph envelope and a morph LFO, and K5-style editing controls expanded and modified for the Type P (see the [additive synthesis note](docs/research/additive-synthesis.md) and [the spectral contract](plugin/feature-contract-spectral.md)).
3. A 48 kHz core with a 128-tap output bridge, so a 48 kHz host takes the core output untouched and the other rates are resampled at the boundary. See the [48 kHz contract](plugin/feature-contract-48k-core.md).
4. Giving the filter an SH-101-ish character (see [the filter law](plugin/feature-contract-filter-law.md) contract.)
5. Envelope redesign.
6. Per-part stereo panning.
7. Library synchronisation across running instances.

## Experiments I'd like to try

1. Replicating DX7 patches to find the limitations of Type P.α's architecture in comparison to 6-op architectures.
2. Having other people try the synth and provide feedback on UX and sound.
3. Trying wild PM techniques from chiptune composers like blobnote.
4. Making more spectral patches and finding which other K5-style editing tools might be worth adding.
5. Documenting the pseudo-virtual analogue experience.

## Graphic design references for future work

I'd like to make a better wordmark and complementary materials such as posters, a web with custom assets, etc. The panel is mostly there but I guess everything can always be refined.

References:

- A Novarese (Eurostile, Stop)
- Openmind (Amon Tobin's Foley Room)
- R Mitchell (Corporate)
- The Designer's Republic (Wipeout i portades d'Autechre)
- N2-Antireal (Terminal)
- Maniackers
- Nakamura Y and Hashimoto K  (Gona)
- VDL (Line, Giga)
- R Larabie (Rexlia)

## Compiling

The build fetches JUCE 9.0.5 and the pinned `clap-juce-extensions` commit at
configure time. It also needs CMake 3.22, Ninja, clang, and the platform JUCE
dependencies.

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld \
  -DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld \
  -DCMAKE_MODULE_LINKER_FLAGS=-fuse-ld=lld
cmake --build build
ctest --test-dir build --output-on-failure
```

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
```

The compiler is clang on every platform, so the Windows script uses clang-cl with
lld-link and macOS uses AppleClang. See [`docker/README.md`](docker/README.md) for
more info on the platform matrix, sanitizer build and clang tooling.

The plugin compiles into `build/plugin/juce/type_p_alpha_artefacts/Release/`:

- `VST3/Type P.alpha.vst3`
- `CLAP/Type P.alpha.clap`
- `Standalone/Type P.alpha`

The tests cover engine parity, the rate bridge, preset storage, processor
smoke test, stability pass, editor snapshots, and the Linux DAW matrix. The
`linux_daws` test needs Ardour, Carla, JACK and Xvfb. Use `ctest -E linux_daws` when those hosts are not installed.

## Code map

The hand-written types and functions, the generated files and the cleanup
findings are detailed in [`docs/codebase-audit.md`](docs/codebase-audit.md). The visual
language, its references and the panel rules are at
[`design/README.md`](design/README.md).

## Installation

The release packages contain the VST3 and CLAP plugins, the standalone app, and the licences. Copy
the VST3 or CLAP into your plugin folder, or just run the standalone app.

## Licensing

The source is GPL-3.0-only. The plugin binary also links JUCE 9 under the
AGPL-3.0-only terms, and the bundled fonts are SIL Open Font License 1.1. Read
[`LICENSING.md`](LICENSING.md) before packaging a binary. The repository is the
corresponding source for every release. Release packages carry the licensing notices
in `licences/`.

## Status

The Type P.α engine and editor have grown out of the abandoned Type P.fm1 project. Patch
and part names are 32 characters rather than the older 15, and the library uses the new `P1LIB2` record
layout. Each operator carries its own amplitude envelope, signed feedback envelope,
and amplitude, feedback and detune LFOs, with the three LFO families on the
`operator lfos` tab. The seven-tab editor has the two 64-harmonic spectra pages and
the spectra mod page. The filter follows the SH-101's cutoff and resonance law,
with cutoff values from older versions migrated on load.

The plugin passes pluginval at strictness 5 on Linux x86_64, with the editor open
and the audio, state and automation checks included. The CLAP validator reports no
failures and one warning, because its random-state test loads garbage without crashing. The Linux suite runs in CTest, and the Windows clang-cl and macOS universal
VST3, CLAP and standalone builds pass in CI. The Linux DAW matrix runs when Ardour, Carla and
JACK are installed, but the macOS DAW validation is still untested, pending
access to an actual Mac.
