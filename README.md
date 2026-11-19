# Type P.α

Hey, it's Akirako Saint-Just from the パンク派第１音響合成研究班, or Punk Faction, Sound Synthesis Research Unit Number 1 (P1Onken for short.) Through my commitment to punk, post-punk, techno and industrial politics and aesthetics, I've come to be a fanatic of DX-style phase modulation synths, so over the years I've been accumulating ideas about things I wish I had in a PM synth, which features from which DX-style synths are best, and how to balance ease of use with synthesis power. My inspirations are what I've learned from playing the Yamaha synths and emulations I've used, and in general Yamaha's OPN, OPM, OPP, OPL3 and OPZ chips plus the Reface DX for phase modulation. I've also been curious about how other techniques compose with PM, whether they offer anything of interest, and how to implement them without overcomplicating patch design. On that front, my references are Casio's CZ series for the character of phase distortion, Kleimola-Lazzarini-Timoney-Välimäki's paper on vector phaseshaping for the details, Xenakis' granular and stochastic techniques, Nonlinearcircuits' chaos modules, and the Kawai K5 for my implementation of spectral synthesis.

_Type P_, where _P_ stands for _phase_, is the name for the general project of developing this kind of expanded DX-style++ synth. I've tried several experimental implementations, like _Type P.a_, which was implemented in allocation-free C# and had a complex modular architecture, _Type P.b_, which was radically simplified following lessons taken from _Type P.a_; and _Type P.fm1_, which was constrained by the CPU, memory and UI limits of the M-VAVE FM1. **Type P.α** (_α_ as in _alpha_, distinct from the _a_ in _Type P.a_) is the current iteration, presented as a VST3 plugin, a CLAP plugin and a standalone app, made with JUCE and using my personal framework tooling.

The reasons for abandoning Type P.fm1 are that I've decided to disengage from the custom firmware community for highly personal and slightly political reasons, and that being free from the FM1's limited screen, controls and CPU power allows for a kind of research into synthesis techniques that I really want to try.

## The instrument

**Type P.α** has 24 voices split across two four-operator parts. Each operator carries feedback, a set of 13 waveforms, a spectrum oscillator mode more or less modelled after the OPL3 and the OPZ/TX81z/DX11/V50, and an amplitude LFO. Feedback also has its own envelope and LFO per operator, and each operator has a pitch LFO, so the harmonic spectrum can move without touching the amplitude. The pitch envelope, the pitch LFO and the multi-mode filter have an envelope of their own, all per voice and per part, plus key scaling and velocity sensitivity for a lot of parameters. Parts can be stacked, assigned to different MIDI channels or split into keyboard zones, and each has a delay stage so they can be sequential in time.

The spectral side is the part I'm most proud of. Each operator has two editable 64-harmonic spectra, with an envelope and an LFO to morph between them. The K5 is the starting point, but where the K5 has two overlapping spectra across two parts and four envelopes per part assignable to harmonics, Type P.α has two spectra across four operators and two parts, and each operator has its own morph envelope and LFO on top of its amplitude envelope and LFO.

I believe two 4-operator parts is the sweet spot for usability and power. Compared to 6-operator synths, the number of algorithms is smaller, and I feel like dividing patches into two 4-op blocks makes programming way less overwhelming than presenting users with one 8-op part. I also believe offering feedback on every operator, along with additional waveforms, most of the time makes up for the kind of 6-op algorithms Type P.α lacks. Most importantly, it's an architecture that lends itself well to assembling patches out of stacked or sequential partials, which to me is the ideal patch design approach for phase modulation.

I've also tried to make sound design easier by restraining components to a small maximum parameter count, making sure the state of every operator can be compared and edited from a single page, ensuring mutes are accessible from any page, and providing visual guidance through envelope graphs, spectrograms and oscilloscopes.

Phase modulation indices are calibrated to be similar to Yamaha's OPN, OPM, OPL and OPZ families, rather than the softer sound of synths like the DX7, DX9 and Reface DX. Feedback is bipolar. The positive half follows Yamaha's traditional behaviour and scales with the operator's amplitude. The negative half is rectified into the Reface-style absolute-value pulse, which adds odd harmonics to a sine instead of flipping the waveform. The feedback envelope and LFO move the magnitude, so the shipped feedback is the one I want to use. The contracts are [bipolar feedback](plugin/feature-contract-feedback-bipolar.md) and [operator LFOs](plugin/feature-contract-operator-lfos.md), and the research note is [the feedback study](docs/research/README.md).

Results so far seem to be fit for intent. I've been able to easily make rudimentary additive patches that sound a bit like the PPG sounds Tangerine Dream used circa Exit, additive brass, woodwinds and strings; and pseudo-PWM pads and leads by pairing saw operators with sine operators at 20-35% feedback, which gives them a ramp waveform. Conversion of OPM patches to Type P ranges produces spectrally similar results to the originals, and they are easy to edit.

## Lines of research

1. Phase distortion and vector phaseshaping. Where should the transfer function's
   breakpoint sit, and can phase distortion and phase modulation share one phase
   transfer function without extra machinery? The 2D vector version feels like a
   lot of work for little reward, so it stays a question rather than a commitment.
2. Phase granulation as a layer on top of PM and PD, with chaotic and stochastic
   modulators. What should it do, what does it cost, and which parameters keep it
   playable?
3. Envelope design. Curves, stages and loops.
4. LFO envelopes. What should an envelope on an LFO rate or depth add to the
   operator LFOs?
5. Control macros. Which few macro controls earn a place without turning the panel
   into a second modulation matrix?
6. Stereo. Where does the width come from, and how much of it belongs to the
   operators rather than the parts?
7. Synchronising the sound banks between open instances. The library files are the
   obvious boundary. The open question is what happens when two instances edit the
   same slot.
8. The filter. The cutoff and resonance law is now the SH-101's, but I still want a
   listening pass before I call it finished.
9. Documentation and the graphic design. The panel is close. The manual, the
   posters and the wordmark are not.
10. Extra waves. Is there another waveform that earns its place?
11. Ring modulation algorithms. Does ring mod add something the PM algorithms
    cannot?

## Decided steps

1. More precise spectrograms, with clear differentiation of odd and even harmonics, and maybe some numbers for easier reading. Being able to reliably see the spectra is essential for additive-style synthesis.
2. Exploiting parallelism if possible!
3. A web app made from the same engine and a panel from the editor design, with the build and browser evidence in the [web panel contract](plugin/feature-contract-web-panel.md).
4. A 64-harmonic spectral oscillator per operator, with two editable spectra, a morph envelope and a morph LFO. The K5 group editing is the starting point, with more selection masks. The survey is in the [additive synthesis note](docs/research/additive-synthesis.md), and the current contract is [the spectral contract](plugin/feature-contract-spectral.md).
5. A 48 kHz core with a 128-tap output bridge, so a 48 kHz host takes the core output untouched and the other rates are resampled at the boundary. The measurements are in the [48 kHz contract](plugin/feature-contract-48k-core.md).
6. The SH-101 filter law: exponential cutoff, linear resonance with self-oscillation near 80 percent, the output-side makeup, and the input soft knee. The contract is [the filter law](plugin/feature-contract-filter-law.md).

## Experiments I'd like to try

1. Replicating DX7 patches to find the limitations of Type P.α's architecture in comparison to 6-op architectures.
2. Having other people try the synth and provide feedback on UX and sound.
3. Trying wild PM techniques from chiptune composers like blobnote.
4. Using the spectral oscillator on more material and finding which K5-style editing tools are worth adding.
5. Documenting the pseudo-virtual analogue experience.

## Compiling

The build fetches JUCE 9.0.5 and the pinned `clap-juce-extensions` commit at
configure time, and it needs CMake 3.22, Ninja, clang, and the platform JUCE
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
lld-link and macOS uses AppleClang. [`docker/README.md`](docker/README.md) carries
the platform matrix, the sanitizer build and the clang tooling.

The plugin compiles into `build/plugin/juce/type_p_alpha_artefacts/Release/`:

- `VST3/Type P.alpha.vst3`
- `CLAP/Type P.alpha.clap`
- `Standalone/Type P.alpha`

The tests cover engine parity, the rate bridge, preset storage, processor
smoke, stability pass, editor snapshots, and the Linux DAW matrix. The
`linux_daws` test needs Ardour, Carla, JACK and Xvfb. Use `ctest -E linux_daws` when
those hosts are not installed.

## Code map

The hand-written types and functions, the generated files and the cleanup
findings are in [`docs/codebase-audit.md`](docs/codebase-audit.md). The visual
language, its references and the panel rules are in
[`design/README.md`](design/README.md).

## Installation

The release archives hold the VST3, the CLAP, the standalone app, and the licences. Copy
the VST3 or CLAP into your plugin folder, or just run the standalone app.

## Licensing

The source is GPL-3.0-only. The plugin binary also links JUCE 9 under the
AGPL-3.0-only terms, and the bundled fonts are SIL Open Font License 1.1. Read
[`LICENSING.md`](LICENSING.md) before packaging a binary. The repository is the
corresponding source for every release, and the release archives carry the notices
in `licences/`.

## Status

The Type P.α engine and editor grew out of the abandoned Type P.fm1 build. Patch
and part names carry 32 characters, and the library uses the `P1LIB2` record
layout. Each operator carries its own amplitude envelope, signed feedback envelope,
and amplitude, feedback and detune LFOs, with the three LFO families on the
`operator lfos` tab. The seven-tab editor has the two 64-harmonic spectra pages and
the spectra mod page. The filter follows the SH-101's cutoff and resonance law,
with the old cutoff values migrated on load.

The plugin passes pluginval at strictness 5 on Linux x86_64, with the editor open
and the audio, state and automation checks included. The CLAP validator reports no
failures and one warning, because its random-state test loads garbage without a
crash. The Linux suite runs in CTest, and the Windows clang-cl and macOS universal
VST3, CLAP and standalone builds pass in CI. The AU target and the macOS DAW
validation are still open, and the Linux DAW matrix runs when Ardour, Carla and
JACK are installed.
