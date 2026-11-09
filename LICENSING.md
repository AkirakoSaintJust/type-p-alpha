# Licensing

Type P.alpha is **GPL-3.0-only** for the plugin source, the generated engine, the
design tokens and the documentation in this repository.

The VST3, CLAP and standalone binaries also link **JUCE 8.0.9**, which is
dual-licensed under **AGPL-3.0-only** or a commercial JUCE licence. A distributed
plugin binary is therefore conveyed under the AGPL-3.0-only terms for the JUCE
part. The Type P.alpha source in the same binary keeps its GPL-3.0-only terms, and
GPLv3 section 13 permits the combination. Anyone who cannot accept the AGPL terms
needs a commercial JUCE licence before shipping the plugin.

## Dependencies

| Material | Version | Licence |
| --- | --- | --- |
| JUCE | 8.0.9 | AGPL-3.0-only or commercial |
| VST3 SDK, bundled by JUCE | as shipped in JUCE 8.0.9 | GPL-3.0-only or Steinberg VST3 licence |
| CLAP | 1.2.7 | MIT |
| clap-juce-extensions | `7adee3a1bd4684d4caa5601100e364abccff4b4d` | MIT |
| DotGothic16 | Google Fonts | SIL OFL 1.1 |
| Zen Dots | Google Fonts | SIL OFL 1.1 |
| Unbounded | Google Fonts | SIL OFL 1.1 |
| CMake | 3.22 or later | BSD-3-Clause |
| Ninja | any | Apache-2.0 |

The licence texts are in `third-party-licenses/`. The JUCE AGPL text is
`third-party-licenses/AGPL-3.0.txt`, and the CLAP and font texts sit beside it.

## Source and binaries

The source is this repository, and every release tag is the corresponding source
for the binaries built from it. The plugin builds fetch JUCE 8.0.9 and the pinned
`clap-juce-extensions` commit with CMake `FetchContent`, so the release workflow
records the upstream commits in the release notes.

The release archives carry a `licences/` folder with the project licence, the AGPL
text for JUCE, the MIT texts for CLAP and the wrapper, and the OFL texts for the
three embedded fonts. Keep that folder with the binary when you redistribute it.

## Scope

This repository contains the desktop plugin, the shared voice engine, the generated
DSP, the design tokens and the patches. It does not contain the FM-1 firmware, the
loader, the emulator, the web build, the M-VAVE assets, or the JieLi toolchain. The
umetate framework remains GPL-3.0-only in its own repository, and Type P.fm1 stays
there for bug fixes.

The wordmark uses Zen Dots for `TYPE P.` and Unbounded Regular for the lowercase
`a`. Digital Desolation Plus is not bundled or used.
