# Macros

A macro is a relative modulation source with three destinations. The six macros
are global to the patch, not per part. Each macro has a knob, a source select, a
source range, three target rows and three destination ranges. The block is 54
parameters: six values, six source selects, six source ranges, eighteen targets
and eighteen destination ranges.

## The source law

`plugin/juce/MacroLaw.h` has the source law:

```cpp
inline double macroEffective(int knob, int sourceValue, int range)
{
    const double source = clamp(sourceValue, 0, 127) / 127.0;
    return clamp(knob + source * range, -100.0, 100.0);
}
```

The knob is the centre. A source at rest leaves the macro at the knob. A rising
source adds up to `range` macro points. The result is clamped to -100 to 100.
The source select chooses:

| Source | Value |
| ---: | --- |
| 0 | none |
| 1 | a MIDI CC (72 to 77 by default) |
| 2 | the mod wheel (CC 1) |
| 3 | note velocity |

The six macro CCs are `macroCc_` atomics. The audio thread writes them from
`processBlock`. The mod wheel and the last note velocity are atomics too. The
editor reads them for the feedback text.

## The destination law

Each target row has a role and a member pool. The role is:

| Role | Meaning |
| ---: | --- |
| 0 | every operator |
| 1 | carriers only |
| 2 | modulators only |

The carrier mask is the generated `kCarrierMasks[algorithm]`, which comes from
the routing table. A carrier row only reaches carriers and a modulator row only
reaches modulators. The all row reaches every operator.

The destination offset is a fraction of the destination span:

```cpp
inline double macroOffset(double macro, int range)
{
    return (macro / 100.0) * (range / 100.0);
}
```

A macro at 100 with a range of 100 moves the whole span. A macro at 50 with a
range of 50 moves a quarter. The destination is clamped in the normalised domain:

```cpp
inline double macroTargetNormalised(double base, double offset)
{
    return clamp(base + offset, 0.0, 1.0);
}
```

For a linear parameter the processor applies the offset in raw units:

```cpp
const double span = spec.max - spec.min;
const double shifted = base + offset * span;
effective = round(clamp(shifted, spec.min, spec.max));
```

For a logarithmic parameter the offset is added in the normalised domain and
converted back through the parameter's own range. That keeps an LFO rate macro
musical rather than a raw Q32 phase delta.

## The processor pass

`syncMacros` runs after `syncParams` on every control block. It computes the six
effective values. If every macro is zero and no macro was applied on the previous
block, it returns early. That keeps the no-macro path bit-exact and cheap.

It then clears the per-member offset vector and walks the 18 target slots. For
each slot it resolves the target row, the part, the algorithm and the mask. It
walks the member pool from `kTargetMemberOffsets[row]` to the next row and adds
the offset to each member's accumulated offset. The velocity source is a special
case. The engine applies the velocity source itself for the per-voice level,
feedback, morph, envelope rate, LFO rate and LFO depth groups. The processor
skips those members and keeps the phase envelope, phase LFO and part-level pitch
and filter envelopes in its own layer.

The second loop writes every member:

```cpp
if (offset != 0.0) {
    if (macroMemberLog_[k] != 0 && macroMemberParams_[k] != nullptr) {
        const double baseNorm = convertTo0to1(base);
        const double effectiveNorm = macroTargetNormalised(baseNorm, offset);
        effective = round(convertFrom0to1(effectiveNorm));
    } else {
        const double shifted = base + offset * span;
        effective = round(clamp(shifted, min, max));
    }
}
engine_.setParam(spec.engineId, effective);
```

The stored parameter is never overwritten. The engine receives the effective
value. That is why the panel can show the stored value and the effective value
side by side.

## The generated target table

`plugin/tools/gen_macro_targets.py` reads `synth/macros.json`, the manifest and
`engine/gen/tables.c`. It writes `MacroTargets.h` and `macro_target_ids.h`.

`MacroTargets.h` contains:

- `kTargets` with the 58 row names and sections,
- `kTargetRole`, `kTargetPart`, `kTargetMemberOffsets`, `kTargetMemberPool`,
- `kCarrierMasks[10][4]`,
- `isEnvRateMember`, `isLfoRateMember`, `isLfoDepthMember`,
- the member enum ids.

The carrier masks come from the routing table, so adding an algorithm updates the
macro role logic automatically. The generator reads the routing table's 200
values and derives the carrier mask for each of the 10 algorithms from row 4.

The member pool is the union of every member any algorithm can select. The
runtime filters it with the current mask. That keeps the table algorithm
independent and lets the editor draw the right members for the current algorithm.

## The engine-side velocity source

`engine/runtime/macro_apply.h` is the per-voice part of the macro layer. The
plugin writes the knob, the CC, the wheel and the non-velocity offsets at part
level. The velocity source needs each voice's own velocity, which only the engine
has. The helper is

```c
p1_macro_velocity_role_offset(state, part, voice, op, span,
                              all, carrier, modulator)
```

It reads the six macro gains, sources, ranges, targets and destination ranges
from the state. It sums the velocity contributions for every target row that
matches the operator's role. The result is scaled by `span` and clamped. The
convenience wrappers cover level, feedback, morph, envelope rate, LFO rate ratio
and LFO depth.

The LFO rate ratio is special. The panel rate is logarithmic above the 0.01 Hz
floor, so the offset is converted to a frequency ratio:

```c
ratio = pow(20000000.0 / 894.0, offset / 10000.0);
```

A zero offset returns 65536 exactly, which keeps a macro at zero bit-exact.

## The UI

The macros page has six `MacroCard`s. A card shows the macro name, the macro
value, the source select, the source range, three target selects and three
ranges. The target select is searchable. The range control is a bipolar bar with
a centre line. The card computes the effective macro value from the same
`macroEffective` law as the processor.

The panel draws a targeted parameter as its stored value in pink and its
effective value in smaller green type. `refreshMacroFeedback` walks the page and
calls `macroFeedback` once per control. The control implements
`MacroFeedbackTarget` and `setMacroFeedback`. A parameter with no macro, or a
macro at rest, has no effective value and draws the stored value only.

## The tests

`plugin_smoke` checks the macro target table, the generated member enum, the
carrier and modulator rows, the source law, the destination clamp, the
at-rest bit-exact render, the CC, wheel and velocity sources, the feedback,
envelope rate and LFO rate targets, the morph target and the phase-map row. The
`engine_parity` test checks the C carrier-match helper. `preset_smoke` checks the
54-value macro block in the multi record.
