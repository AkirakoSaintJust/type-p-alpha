# Type P.alpha code documentation

This directory is the full code reference for the tracked tree. It has two
layers. The hand-written chapters explain the design, the memory model and the
actual call paths. The reference pages are generated from the source and list
every file, type, function, macro, global and state field with an exact
location, plus every manifest parameter with its id, target, range and display.

Start here:

- [Memory and processing](architecture/memory-and-processing.md) for the state
  layout, the block model and the thread boundary.
- [DSP](architecture/dsp.md) for the operator graph, the envelopes, the LFOs,
  the filter, the phase map and the spectral bank.
- [Macros](architecture/macros.md) for the source and destination laws, the
  generated target table and the per-voice velocity layer.
- [Phase map](architecture/phase-map.md) for the `F`/`G` geometry, the protected
  mask, the stochastic events and the direct-read table.
- [Voice allocation](architecture/voice-allocation.md) for note routing, poly
  and mono behaviour, unison, legato and voice stealing.
- [Sample-rate bridge](architecture/sample-rate-bridge.md) for the 48 kHz core
  and the host-rate resampler.
- [Synchronisation](architecture/synchronisation.md) for the audio/UI handoff,
  the atomics, the snapshots and the library lock.
- [User interface](architecture/user-interface.md) for the JUCE editor, the
  pages, the widgets and the parameter binding.
- [Presets and library](architecture/presets-and-library.md) for the record
  format, the version chain, the merge and the migration.
- [Build and tests](architecture/build-and-tests.md) for CMake, the generators,
  the CTest ladder, CI and packaging.

The generated reference is in [reference/](reference/):

- [File index](reference/files.md)
- [Type, class, struct, union and enum index](reference/types.md)
- [Function and method index](reference/functions.md)
- [Macro, global, alias and constant index](reference/symbols.md)
- [State field index](reference/state-fields.md)
- [Parameter index](reference/parameters.md)
- [Generated render walk](reference/generated-render-walk.md)
- [Generator node catalogue](reference/nodes.md)
- [Test index](reference/tests.md)

## What is covered

The tree is the generated engine in `engine/gen`, the fixed-point HAL in
`engine/hal`, the runtime in `engine/runtime`, the JUCE host in `plugin/juce`,
the panel toolkit in `plugin/ui`, the library in `plugin/preset`, the tests in
`plugin/tests`, the tools in `plugin/tools` and `tools`, the build scripts, the
CMake files and the design and manual sources.

The reference pages describe the tracked files. The local `.dev` generator is
ignored by git, so its node catalogue is an appendix that explains how the
generated code is assembled. A fresh clone does not need `.dev` to build, so the
catalogue is evidence for the generator rather than a build dependency.

The block granularity is the function and the generated node. Every function has
a signature and a purpose, and every distinct block comment in the generated
renderer is listed in the render walk. A statement-level listing would copy the
source and go stale faster than the code. The hand-written chapters follow the
statement-level order for the core paths.

Third-party licence texts and the vendored JUCE tree are not documented here.
Their own documentation and licences stay with those sources.

## How to read the code references

The reference pages use `path:line` locations. The line is the first line of the
declaration or definition. The signature is the declaration text with the body
removed. A description is the comment immediately above the declaration, the
comment at the top of the body, or a short purpose derived from the name, the
scope and the return type. Trivial accessors and JUCE overrides are summarised
by that derived purpose. The hand-written chapters cover the core functions and
the actual call path.

A symbol that appears in a header and in a source file appears twice when the
declaration and the definition are both visible. That is deliberate. The header
entry is the contract and the source entry is the implementation.

## Regenerating the reference

The generator needs Python 3 and the optional parsers:

```sh
python -m pip install tree-sitter tree-sitter-c tree-sitter-cpp tree-sitter-python
python tools/gen_code_docs.py
```

It reads the tracked sources and rewrites `docs/code/reference`. The
hand-written chapters are not touched. Commit the regenerated pages with the
code change that altered the symbols.

## Conventions

- Q15, Q16 and Q31 mean a signed fixed-point fraction with 15, 16 or 31
  fractional bits. A Q15 unity gain is 32768, even though an int16 holds 32767.
- `NVOICE` is 24, `N_PART` is 2 and `CONTROL_BLOCK` is 32 samples.
- The core rate is 48 kHz. The host rate can be 44.1, 48, 88.2, 96, 176.4 or
  192 kHz.
- A part is a complete 4-operator timbre. A patch uses two parts. A multi-patch
  selects one library part for each of the two slots.
- The engine state is one `type_p_alpha_state` allocation. The host wrapper owns
  it through `p1::Engine`.
- The parameter ids are the manifest ids. The generated `type_p_alpha_slots`
  table maps an id to a state offset and a part index.

## Source layout

| Directory | Contents |
| --- | --- |
| `engine/hal/` | Fixed-point arithmetic, wavetable lookup, key and velocity scaling, spectral bank helpers |
| `engine/runtime/` | Preset record format, MIDI parser, host stubs |
| `engine/gen/` | Generated state, parameter table, init table, wavetable, harmonics, render code |
| `plugin/engine/` | C++ engine seam, phase-map snapshot, MIDI bridge and sample-rate bridge |
| `plugin/juce/` | JUCE processor, editor, pages and the shared UI header |
| `plugin/ui/` | Widgets, theme, token bridge and spectrum analysis |
| `plugin/preset/` | Library store, record migrations and cutoff law |
| `plugin/tests/` | Engine, host, preset, UI, perceptual and rate tests |
| `plugin/tools/` | Parameter, macro, token, ratio and snapshot tools |
| `tools/` | Wavetable, maths table and code-doc generators |
| `synth/` | The patch manifest and the graph instance |
| `design/`, `docs/`, `manual/` | Design notes, research and the player manual |
