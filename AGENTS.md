# Working on Type P.alpha

Type P.alpha is the desktop plugin product. The engine is generated into
`firmware/gen`, the plugin is in `plugin/`, and the design language is in
`design/`.

## Rules

- The plugin is the product. Do not add a device firmware, loader, emulator or web
  release path.
- Every manifest parameter must reach the APVTS and the editor. Generate the list
  from the merged manifest, never type it by hand.
- A deliberate editor change needs a reviewed snapshot update. The golden set is
  pinned to the Linux container because font rasterisation differs on other hosts.
- The source is GPL-3.0-only. The plugin binary also carries JUCE 8's AGPL terms.
  Read `LICENSING.md` before packaging.
- Do not bundle Digital Desolation Plus. The wordmark is `TYPE P.` in Zen Dots with
  a lowercase `a` in Unbounded Regular.
- Type P.fm1 lives in the umetate repository and receives bug fixes only.

## Commands

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build --output-on-failure
ctest --test-dir build -E linux_daws --output-on-failure   # without Ardour/Carla
```
