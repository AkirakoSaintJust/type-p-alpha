# Tone capability

## Player outcome

Tone is a plugin-only output control. It softens the top end after the shared
engine, so a patch can sit in a mix without changing the patch that runs on the
device. The default state is off, and the plugin render is bit-exact with the
firmware until the player turns it on.

## Controls

- `tone` page, schema: `ON` is an on/off knob, and `TONE` is a percentage knob
  from 0 to 100. Off bypasses the filter. 100 percent is a 20 kHz cutoff.
- `tone curve` page, C++: the same on/off and tone controls beside a log
  frequency response curve.

## Signal path

The capability runs after the shared engine and before the host output.

```text
engine stereo int32 -> tone one-pole (when on) -> host buffer
```

The one-pole is `y[n] = y[n-1] + a(x[n] - y[n-1])`, with
`a = min(1, 2*pi*fc/44100)` and `fc = 200 + (tone/100)^2 * 19800`. The filter has
one state per channel. A reset clears the state and returns to off.

## Parameters

| Name | Range | Default | Meaning |
| --- | --- | --- | --- |
| `cap_tone_on` | 0 to 1 | 0 | Bypass when 0. |
| `cap_tone` | 0 to 100% | 100 | Cutoff, 200 Hz to 20 kHz. |

The ids are appended after the 303 base parameters. The device part record and
the `.p1lib` bank do not change.

## Bounds

The DSP uses two floats of state and a few parameter values. It does one multiply
and one add per channel per sample when on, and returns immediately when off. The
output is clamped to the engine's int32 range. No allocation, flash or display
work happens in the audio thread.

## Tests

- The default off state leaves the base render bit-exact.
- Turning the capability on and lowering the tone reduces high-frequency energy.
- The generated params carry ids after the base manifest.
- The schema page and the C++ page both appear as tabs.
