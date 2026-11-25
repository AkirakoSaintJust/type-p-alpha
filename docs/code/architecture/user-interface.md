# User interface

The UI is a JUCE editor with a fixed 1,240 by 985 design canvas. The editor scales
the whole canvas to the host window, so every page shares one coordinate system.
The audio thread never calls into the UI. The editor timer reads parameter
atomics, scope rings and snapshots, then repaints the visible page.

## The editor object

`PluginEditor` is in `plugin/juce/PluginEditor.h` and `PluginEditor.cpp`. It
owns:

```cpp
p1::ui::P1LookAndFeel lookAndFeel_;
p1::ui::P1Canvas canvas_;
std::unique_ptr<Header> header_;
std::unique_ptr<Toolbar> toolbar_;
p1::ui::P1TabBar tabs_;
std::unique_ptr<p1::ui::PageBase> page_;
std::unique_ptr<p1::ui::P1Dialog> dialog_;
```

The constructor copies the active part and page from the processor, applies the
look and feel, sets the resize limits to 620 by 493 and 2,480 by 1,970, fixes the
aspect ratio to 1,240 by 985, builds the UI and starts a 30 Hz timer. The initial
size is the largest scale of the design canvas that fits the primary display's
work area, clamped to 0.5 to 2.0.

`resized` measures the window and applies one transform to `canvas_`:

```cpp
const float scale = min(width / 1240, height / 985);
const float x = (width - 1240 * scale) * 0.5f;
const float y = (height - 985 * scale) * 0.5f;
canvas_.setBounds(0, 0, 1240, 985);
canvas_.setTransform(AffineTransform::scale(scale).translated(x, y));
```

Every component inside the canvas uses the design coordinates. The scale is the
only place that knows the host window size. The tab bar switches to its compact
form below 0.75 scale.

`fitToHostWindow` measures the window chrome from the peer and the editor bounds
rather than guessing it, then sizes the editor to the display work area. The
timer retries for a few ticks because a host can resize the editor after it
first attaches.

## Building a page

`buildUi` creates the header and the toolbar, sets the tab labels and calls
`buildPage`. The page order is:

| Index | Page | Factory |
| ---: | --- | --- |
| 0 | patch settings | `makePatchPage` |
| 1 | macros | `makeMacrosPage` |
| 2 | part params | `makeFxModPage` |
| 3 | op envs | `makeOperatorsPage` |
| 4 | op lfos | `makeOperatorLfosPage` |
| 5 | op phasemap | `makePhaseMapPage` |
| 6 | op spectra a | `makeOperatorSpectraPage(editor, true)` |
| 7 | op spectra b | `makeOperatorSpectraPage(editor, false)` |
| 8 | op spectra mod | `makeOperatorSpectraModPage` |

`buildPage` destroys the old page and creates the new one, then sets it to
`(17, 255, 1206, 685)`. The header is `(1, 1, 1238, 159)`, the toolbar is
`(1, 160, 1238, 44)` and the tab bar is `(1, 204, 1238, 37)`. The page content is
inside the same canvas.

The page factory returns a `PageBase`:

```cpp
class PageBase : public juce::Component {
public:
    virtual void tick() = 0;
};
```

The editor timer calls `page_->tick()`, then walks the page tree and refreshes
the macro feedback for every `MacroFeedbackTarget`. The headers and toolbars are
`juce::Component` subclasses with their own `tick`. They are not `PageBase`
because they survive a page change.

## The header

`Header` is in `PluginEditor.cpp`. It draws the wordmark, the brand line, the
patch name, the scope or spectrogram, the output volume and the algorithm block.
The algorithm block is a `P1AlgorithmBlock`:

```cpp
alg_.onClick = [this] { editor_.showAlgorithmPicker(); };
alg_.onWheel = [this](int direction) {
    const int current = ...
    editor_.applyAlgorithm((current + direction + 10) % 10);
};
```

The block paints the 30px Zen Dots algorithm number, a caret and the routing
graph. The graph uses the ten `drawAlgorithmGraph` layouts. White operators are
modulators, pink operators are carriers, grey lines are modulation and purple
downward arrows mark carrier output. The ring mask and the direct mask come from
the part's `ringN_on` and `carN_out` parameters, so the header graph reflects the
current ringmod state as well as the algorithm.

The output section is a `scope` and `spectro` segmented control. The two
`P1Scope` and `P1Spectro` components share the same 330 by 50 area and one is
visible at a time. The scope draws the last samples from the output
`ScopeRing`. The spectrogram computes a 512-sample FFT with `juce::dsp::FFT` and
draws 22 bars. The `volume` slider is `P1VolumeSlider`, which shows the fader
position and the dB text from `VolumeLaw.h`.

## The toolbar

`Toolbar` draws the mute legend and the six real mute tiles: `op1` to `op4`,
`p1` and `p2`. An active tile is solid pink with a black label. A muted tile is
black with a pink outline and a pink label. The right side has the active part
name and the single-part library button. The part mute is not a separate gate in
the engine. It stores the last positive level and writes level 0 for the mute,
then restores the stored level on unmute:

```cpp
void PluginEditor::togglePartMute(int part, bool muted) { ... }
```

The same helper is used by the part card header and the toolbar.

## Parameter binding

Most controls take an `APVTS& state` and a parameter id. `ParamKnob` binds a
`juce::Slider` through a `SliderAttachment`. `P1Toggle` binds a `ButtonAttachment`.
`P1LevelSlider` and `P1VolumeSlider` use `ParameterAttachment` and paint their
own tracks. `ParamChoice` uses a `ParameterAttachment` and maps the combo index
to the integer parameter.

`ParamKnob` is the main control. It has a 42px dial by default, a name label and
a value label. The `KnobSlider` subclass handles the ratio detents:

- the mouse wheel steps the value by one host step,
- the arrow keys step a detented ratio knob to the next detent rather than
  nudging the raw value,
- `onUserEdit` fires once at the start of a user drag,
- `onUserEditEnd` fires once after a user edit.

The attachments handle host automation. A host write updates the slider and the
value text without firing the user callbacks. That distinction is what keeps a
host automation pass from marking a library slot as locally edited.

The value text comes from `formatParamValue`. The function is generated from the
manifest display kind:

| Display | Text |
| --- | --- |
| `hz` | a frequency from the Q32 rate |
| `pct` | a percentage of the parameter span |
| `pct_bip` | a signed percentage |
| `semitones` | signed semitones |
| `cent` | signed cents |
| `cutoff` | the cutoff law in hertz |
| `oct` | octaves |
| `frac3` | three fractional digits |
| `wave` | the waveform name |
| `lfo_wave` | the LFO waveform name |
| `onoff` | `off` or `on` |
| `fade` | `off` or the fade value |
| `raw` | the integer |

`parseParamValue` is the inverse. The host text round trip is tested for every
parameter in `plugin_smoke`.

The macro feedback is a second value beside the stored one. A control that
implements `MacroFeedbackTarget` reports its manifest index. The editor calls
`processor_.macroFeedback(index)` once per refresh and passes `targeted`,
`hasEffective` and `effective` to the control. `ParamValueLabel` draws the stored
value with its unit and the effective value in smaller green type without a unit.
The stored value is not overwritten.

## The pages

### Patch settings

`PatchPage` has two `PartCard`s and one `VoiceMode` block. A part card has a
header with rename, copy, paste and mute, seven knobs (MIDI channel, keyzone
start, keyzone end, tune, transpose, delay and bend), the level and pan sliders,
a scope and a spectrogram. The scope and spectrogram read tap 8 and 9, the carrier
sums for each part.

`VoiceMode` has four mode buttons (`poly`, `mono`, `legato`, `unison`) and three
knobs (`unison`, `detune`, `porta`). The legato and unison buttons toggle their
parameters, and the knobs are disabled and dimmed when the part is poly.

### Macros

`MacrosPage` has six `MacroCard`s. A macro card has the macro value, the source
select, the source range, three target selects and three range controls. The
target choice is a `MacroTargetChoice` with a searchable list. The `MacroRange`
control is a bipolar bar with a centre line. The macro card also shows the
effective macro value after the source law.

The macro target table is generated from `synth/macros.json` by
`plugin/tools/gen_macro_targets.py`. The generated `MacroTargets.h` carries the
row names, the member pool, the member offsets, the role and the carrier masks.
The editor uses the same table as the processor, so a target cannot drift between
the UI and the audio path.

### Part params

`FxModPage` has the filter, filter envelope, pitch LFO, pitch envelope, pan
envelope, key follow, velocity follow and ring modulation. Each card is a
`P1Card` with a header copy and paste action. The filter card has cutoff,
resonance, type and on. The filter envelope card has the on switch, the graph,
three rates and levels, the release rate and level, and the mode. The pitch LFO
card has rate, depth, rate fade, depth fade, wave and bipolar. The pitch envelope
card has the same envelope shape. The pan envelope card has the pan graph and
the same envelope controls. The key follow and velocity follow cards have the
target choice, the breakpoint or the amount, and the two or one value knobs.
The ringmod card is described below.

### Operator envs

`OperatorsPage` has four `OpColumn`s. Each column has the oscillator card, the
amplitude envelope card, the feedback envelope card, the amplitude LFO card and
the feedback LFO card. The oscillator card has ratio, detune, level, feedback and
the wave select. The envelope cards draw the `P1Envelope` graph. The LFO cards
draw a wave preview and the rate, depth, fade, wave and mode controls.

### Operator LFOs

`OperatorLfosPage` has four `OpLfoColumn`s. Each column repeats the three LFO
cards: amplitude, feedback and detune. The detune LFO has a `semi_lfo` display
because it moves in semitones. The page is separate from the operator page so
the LFO cards can be 113px tall.

### Phase map

`PhaseMapPage` has four operator columns. A column has the compact oscillator
strip, the phase card, the phase LFO card and the phase envelope card.
`PhaseMapView` draws the composed transfer curve from the per-part snapshot. The
green curve is the forward map and the purple segments are flipped. Locked
segments are drawn at 35% opacity, order jumps are pink dashed verticals, the
fixed y boundaries are faint rules, `F` is a faint polyline, and the breakpoint
is a pink vertical with the phase dot at the newest voice's phase. The view
repaints only when the snapshot signature changes.

### Spectra a and b

`OperatorSpectraPage` has four operator columns. Each column has the
`SpectraOperatorCard` with the oscillator strip and the `SpectrumRow`. A
`SpectrumRow` has 64 `P1SpectralBars`, the range select, the group select, the
mask switch, the selection window, the `norm`, `to b` or `to a` buttons, copy and
paste, and the `load spectrum template...` button. The bars support a freehand
drag, a spring/tilt/formant gesture and a group edit. The analysis uses
`analyseWaveTable` from `plugin/ui/SpectrumAnalysis.cpp`, which reads the mip-0
wavetable and returns 64 harmonic magnitudes.

### Spectra mod

`OperatorSpectraModPage` has four `SpectraModColumn`s. Each column has the
`P1SpectrumDisplay`, the morph knob, the morph envelope card and the morph LFO
card. The display draws the a and b spectra, the live morph cursor from
`processor_.specMorph`, and the range markers.

The spectral pages call `ensureSpectrumMode` when the player edits the morph
envelope switch or a morph LFO depth away from zero. That helper writes the
operator waveform as 14 if it is not already in spectrum mode. A template load
does not change the waveform, so the stored levels wait for the player to choose
`spectrum`.

### Ring modulation

`RingmodCard` follows the current algorithm. It draws one pair per consecutive
carrier group, one level knob per pair, and one direct output toggle per carrier.
The card reflows when the algorithm changes. A one-carrier algorithm shows the
title and `no ringmod` and no controls. A hidden pair keeps its parameter values,
so switching back restores them. The `RingGraph` shows the two carriers, the
pair input, the ring product and the direct output. A muted or hidden row keeps
the parameter but does not draw the product.

## Dialogs

`P1Dialog` is a modal overlay with a black 72% background, a raised panel, a
title, a close cross and a row of buttons. The dialog body is any `juce::Component`. The footer starts at 42px for one row
of buttons and grows by 32px for each extra row. The dialog is centred in the
canvas.

The editor uses dialogs for:

- the algorithm picker (`AlgPickerBody`, ten tiles in a 4 by 3 grid),
- the patch browser and patch save mode (`SlotListBody`),
- the single-part library (`SlotListBody`),
- the multi-patch library (`SlotListBody` and `ModeListBody`),
- rename part, patch and macro (`RenameBody`),
- confirmation (`P1Dialog` with one message and two buttons).

The list bodies are `ListBoxModel` implementations. They show the slot number,
the name and the used state. The patch browser also shows the part names and the
source slot. The save mode body chooses whether a patch save also saves the two
parts.

## Copy and paste

The editor keeps four clipboards:

- `partClipboard_` for a whole part,
- `operatorClipboard_` for a whole operator,
- `operatorSectionClipboard_` for one operator section,
- the part fx/mod section copy uses `operatorSectionClipboard_` with a
  `PartFxSection` marker.

A section copy reads the parameter ids from `sectionParamIDs` and stores their
current values. A paste writes them back through `setParam`. The spectrum paste
also forces the waveform to 14 so the pasted levels are audible. The whole part
copy includes the part parameters and the five routing values.

## Refresh and redraw

The editor timer runs at 30 Hz. `header_->tick()` reads the algorithm, the ring
and direct masks, the patch name and the output scope. `toolbar_->tick()` reads
the mute state. `page_->tick()` refreshes the visible page. A page tick reads
only the controls it owns. The phase map, envelope, spectrum and scope widgets
keep a signature of the values they draw and skip the repaint when nothing
changed.

`p1_ui_frame_bench` measures the frame cost. The phase page is the expensive
one because it draws the composed map, the three graphs and the oscillator
strip. The scope is about 190 to 200 µs, the spectrogram about 17 µs, the full
phase page about 11.5 ms and the spectra page about 4.1 ms on the development
build. The modern AVX2 build does not help there because the cost is raster and
text, not vector DSP.

## Theme and fonts

`plugin/ui/Theme.h` and `Theme.cpp` own the font loading and the look and feel.
`UiTokens.h` is generated from `design/tokens.json` by
`plugin/tools/gen_tokens.py`. The tokens carry the colours, the text sizes, the
grid, the card heights and the control sizes. Every editor value should come
from that header.

The three fonts are:

- Zen Dots for the wordmark, headings and section titles,
- DotGothic16 for labels, buttons, values and dialog body,
- Unbounded for the `a` in the wordmark.

The `P1LookAndFeel` draws the combo boxes, the popup menu, the scrollbar and the
rotary slider. `KnobLookAndFeel` draws the knob body, the pointer and the mod
arc. The paint code is intentionally flat: square corners, no gradients, no
blurred shadows and no glow. The only shadow is the 4px solid offset in the
wordmark.

## Snapshot tests

`plugin/tests/editor_snapshot.cpp` renders 31 images: every page, the dialogs,
the long name, the half-size frame, the ringmod states and the section paste.
The goldens are `plugin/tests/snapshots/linux-x86_64`. The diff gate is pinned to
the Linux container because font rasterisation differs on other hosts.
`plugin/tools/snapshot_check.py` compares or accepts a set. The manifest records
the width, height and SHA-256 of each image.
