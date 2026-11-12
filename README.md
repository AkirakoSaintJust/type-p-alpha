# Type P.α

Hey, it's Akirako Saint-Just from the パンク派第１音響合成研究班, or Punk Faction, Sound Synthesis Research Unit Number 1 (P1Onken for short.) Through my commitment to punk, post-punk, techno and industrial politics and aesthetics, I've come to be a fanatic of DX-style phase modulation synths, so over the years I've been accumulating ideas about things I wish I had in a PM synth, which features from which DX-style synths are best, and how to balance ease of use with synthesis power. I've also been curious about how other techniques compose with PM, whether they offer anything of interest, and how to implement them without overcomplicating patch design.

_Type P_, where _P_ stands for _phase_, is the name I've given to the general project of developing this kind of expanded DX-style++ synth. I've tried several experimental implementations, like _Type P.a_, which was implemented in allocation-free C# and had a complex modular architecture, _Type P.b_, which was radically simplified following lessons taken from _Type P.a_; and _Type P.fm1_, which was constrained by the CPU, memory and UI limits of the M-VAVE FM1. **Type P.α** (_α_ as in _alpha_, distinct from the _a_ in _Type P.a_) is the current iteration, presented as a VST3 plugin, a CLAP plugin and a standalone app, made with JUCE and using my personal framework tooling.

The reasons for abandoning Type P.fm1 are that I've decided to disengage from the custom firmware community for highly personal and slightly political reasons, and that being free from the FM1's limited screen, controls and CPU allows for a kind of research into synthesis techniques that I really want to try.

## Starting point

At the start of development, **Type P.α** has 20 voices split amongst two four-operator parts, where all of the operators have feedback, a set of 12 waveforms more or less modelled after the OPL3 and the OPZ/TX81z/DX11/V50, and an independent amplitude LFO. It also has a pitch envelope, a pitch LFO and a multi-mode filter with an envelope of its own, all per voice and per part, plus key scaling and velocity sensitivity for a lot of parameters. Parts can be stacked, assigned to different MIDI channels or split into keyboard zones, and each has a delay stage so they can be sequential in time.

I believe two 4-operator parts is the sweet spot for usability and power. Compared to 6-operator synths, the number of algorithms is smaller, and I feel like dividing patches into two 4-op blocks makes programming way less overwhelming than presenting users with one 8-op part. I also believe offering feedback on every operator, along with additional waveforms, most of the time makes up for the kind of 6-op algorithms Type P.α lacks. Most importantly, it's an architecture that lends itself well to assembling patches out of stacked or sequential partials, which to me is the ideal patch design approach for phase modulation.

I've also tried to make sound design easier by restraining components to a small maximum parameter count, making sure the state of every operator can be compared and edited from a single page, ensuring mutes are accessible from any page, and providing visual guidance through envelope graphs, spectrograms and oscilloscopes.

Phase modulation indices are calibrated to be similar to Yamaha's OPN, OPM, OPL and OPZ families, rather than the softer sound of synths like the DX7, DX9 and Reface DX. The implementation of feedback is, as of the starting point, similar to Yamaha's traditional one as opposed to the one used by the Reface DX or the Seqtrak, meaning there's no negative feedback, and feedback scales with the operator's amplitude.

Results so far seem to be fit for intent. I've been able to easily make rudimentary additive patches that sound a bit like the PPG sounds Tangerine Dream used circa Exit, additive brass, woodwinds and strings; and pseudo-PWM pads and leads by pairing saw operators with sine operators at 20-35% feedback, which gives them a ramp waveform. Conversion of OPM patches to Type P ranges produces spectrally similar results to the originals, and they are easy to edit.

## Lines of research

1. Feedback envelopes. Are they worth it and where to place them to avoid overcomplicating patch design? See the [feedback design note](docs/research/README.md), the [envelope contract](plugin/feature-contract-feedback-envelope.md), and the [engine measurement](docs/research/feedback-envelope.md).
2. Is negative feedback (i.e. feedback producing only, or mostly, odd harmonics from a sine wave) viable? See the [design note](docs/research/README.md), the [polarity contract](plugin/feature-contract-feedback-polarity.md), and the [measured result](docs/research/feedback-polarity.md). The plain sign flip is not a reliable odd-harmonic control, so that toggle is not shipped. The bipolar feedback knob ships the Reface-style absolute-value pulse on its negative half: [bipolar contract](plugin/feature-contract-feedback-bipolar.md), [design](docs/research/feedback-bipolar.md). The shaper WAVs and scopes are in [the shaper experiment](docs/research/feedback-shaper.md).
3. Phase distortion/Vector phaseshaping. Where to place the transfer function's breakpoint? How to implement it without adding extra machinery (i.e. phase distortion and phase modulation implemented using the same phase transfer function instead of complicating the DSP unnecessarily). Does _d_-axis phase distortion offer anything substantial over operator feedback? Timbrally and conceptually, what would one use phase distortion for in this synth?
4. Envelope design. Curves, stages, interface.
5. Phase granulation as a layer added on top of the PM/PD one. What do I want it to do, exactly? How much does it cost? What are its parameters?
6. Additive synthesis. How to allow for finer, but accessible, spectral design? How to enable precise control of each harmonic's amplitude over time without overwhelming the user? Is a 16-band graphic formant filter per operator, maybe at the cost of some polyphony if enabled, viable? What are the alternatives? Would it help to add waveforms with very specific harmonic spectra to the waveform list? How to balance flexibility, ease of use, and CPU/memory consumption?
7. Is the rate bridge adequate?
8. Any extra waves that would be worth it?
9. Per-operator detune LFOs?
10. Ring modulation algorithms?

## Decided steps

1. More precise spectrograms, with clear differentiation of odd and even harmonics, and maybe some numbers for easier reading. Being able to reliably see the spectra is essential for additive-style synthesis.
2. Exploiting parallelism if possible!
3. Extending patch and part names to a maximum of 32 characters instead of 15.
4. A web app made from the same engine and a panel from the editor design.

## Experiments I'd like to try

1. Replicating DX7 patches to find the limitations of Type P.α's architecture in comparison to 6-op architectures.
2. Having other people try the synth and provide feedback on UX and sound.
3. Trying wild PM techniques from chiptune composers like blobnote.
4. Learning about additive synthesis techniques and trying to use them in Type P.
5. Documenting the pseudo-virtual analogue experience.

## Compiling

The build fetches JUCE 9.0.5 and the pinned `clap-juce-extensions` commit at
configure time. It needs CMake 3.22, Ninja, clang, and the platform JUCE
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

The Type P.α engine and editor grew out of the abandoned Type P.fm1 build. The
plugin passes pluginval at strictness 5 on Linux x86_64, with the editor open and
the audio, state and automation checks included. The CLAP validator reports no
failures and one warning, because its random-state test loads garbage without a
crash. The filter runs per voice and per part now, so each part has its own cutoff,
resonance, mode and envelope, and its own key and velocity follow.

The Linux suite runs in CTest. The Windows clang-cl build and the macOS universal
VST3, CLAP and standalone build pass in CI. The AU target and the macOS DAW
validation are still open. The Linux DAW matrix runs when Ardour, Carla and JACK
are installed.
