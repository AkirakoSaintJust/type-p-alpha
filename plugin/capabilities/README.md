# Plugin capabilities

A capability is a build-time package for the JUCE app. It carries its own design
note, parameters, DSP and UI. The firmware build never reads this folder, so a
capability costs no device RAM, flash or package size.

The first slice is output-stage and global-param only. Per-part capabilities and
capabilities that retarget a firmware parameter are a later extension. The
contract is in `../feature-contract-capabilities.md`.

## Package shape

```text
capabilities/<id>/
  capability.json      the manifest
  design.md            the player outcome, control map, signal path and bounds
  dsp.c                optional DSP, with <prefix>_init/set_param/process
  ui.json              optional schema pages
  ui.h / ui.cpp        optional C++ pages
```

`capabilities/enabled.json` lists the packages the plugin build includes:

```json
{ "enabled": ["tone"] }
```

An empty list builds the plugin with no capability. The generated plugin params
and DSP always exist, so the plugin code has one shape.

## Manifest

```json
{
  "id": "tone",
  "name": "Tone",
  "version": "1.0.0",
  "description": "An optional output tone control.",
  "design": "design.md",
  "params": [
    { "name": "cap_tone_on", "min": 0, "max": 1, "default": 0,
      "curve": "linear", "unit": "", "display": "onoff" },
    { "name": "cap_tone", "min": 0, "max": 100, "default": 100,
      "curve": "linear", "unit": "%", "display": "pct" }
  ],
  "dsp": { "source": "dsp.c", "params": ["on", "tone"] },
  "ui": {
    "pages": [
      { "id": "tone", "title": "tone", "kind": "schema", "controls": [
        { "kind": "knob", "param": "cap_tone_on", "label": "ON" },
        { "kind": "knob", "param": "cap_tone", "label": "TONE" }
      ] },
      { "id": "tone curve", "title": "tone curve", "kind": "cpp",
        "factory": "makeToneCurvePage" }
    ]
  },
  "migrations": []
}
```

Parameter names must be unique against the base manifest. They receive ids after
the 303 base parameters, so every firmware id and the 135-param device part
record stay unchanged. The plugin state carries the values. The `.p1lib` bank
format does not change.

`migrations` is the translation map for a known firmware parameter that changed
meaning in a capability. Each entry names the parameter and the old and new
values. The plugin applies the map after a state load and after a part or patch
load. The firmware does not know the capability, so a migration must only change
values the firmware can still read.

## DSP

The source declares three functions with the capability's short prefix:

```c
void tone_init(void);
void tone_set_param(uint32_t local_id, int32_t value);
void tone_process(int32_t *out, uint32_t frames);
```

`local_id` follows the order in `dsp.params`. The generated chain keeps the
state and resets it with the engine. The default parameter values must leave the
render bit-exact with the base engine. The demo achieves that with an off switch
that returns before the filter.

The DSP is plugin-only, so it may use float. The firmware engine stays integer
and is not rebuilt.

## UI

A schema page is a list of knobs and choices. The generic renderer uses the
existing widget set, so a simple tool needs no C++ page. The supported controls
are `knob` and `choice`. `choice` carries a `choices` array of display names.

A C++ page is a `p1::ui::PageBase` with the factory named in the manifest. Use it
for a visualiser or a layout the schema cannot express. The capability package's
`ui.h` declares the factory, and `ui.cpp` defines it. The generator adds the
source to the editor target.

## Design gate

A capability is accepted when `design.md` carries:

- the player outcome and the failure mode,
- the control map and the page layout,
- the signal path and the parameter semantics,
- the default neutral state and the reset behaviour,
- the CPU, state and memory bounds,
- the tests and the measured evidence.

The page is designed before it is written. Write the control map, then write the
UI.

## Build

The plugin CMake build runs `plugin/tools/gen_capabilities.py`. The script reads the
base `engine/gen/params.json`, the enabled list and the package manifests. It writes
the merged params, the capability DSP chain, the page table, the C++ registry and
the migration table into the plugin build's generated folder.

The firmware build does not run the script. Its package and its host render are
unchanged.
