# Macros

Status: the parameter layer, the multi record, the target table, the linear and log
destination offsets, the relative CC, wheel and velocity sources, the per-voice
velocity source on the operator levels, the all-operator feedback and morph groups, the
operator amplitude, feedback and spectral envelope rates, and the operator LFO rates
and depths, the macros page and the target-parameter feedback are built. The rest of
the per-voice layer is outstanding. The research is in
[`../docs/research/macros.md`](../docs/research/macros.md), and the parameter list and
the evidence the milestone needs are in
[`../plugin/feature-contract-macros.md`](../plugin/feature-contract-macros.md).

## The idea

Six macros sit at the patch level, above the two parts. Each one is a bipolar offset,
from -100 to +100, with up to three destinations. A destination keeps its stored
value, and the macro adds a percentage of that parameter's span, clamped at the ends.
A macro at 0 therefore leaves the patch exactly as stored.

The offset is recomputed from the base every block. A destination that hits an end
stays there until the macro returns past the point where it hit, and nothing
accumulates. The stored value never moves. Two macros on one parameter sum their
offsets and clamp once.

The six macros belong to the patch rather than to a part. A multi-patch carries its
macros, and a part saved to the library does not.

## Sources

The knob is the centre. Host automation moves the centre, because the knob is the
automated parameter. A MIDI source adds up to its range around that centre, so a
controller at rest leaves the macro at the knob and a rising source adds.

- `cc 72` to `cc 77`, one fixed CC per macro, appended to the generated CC map.
- `mod wheel`, which is CC 1.
- `velocity`, per note.

The range is bipolar, from -100 to +100, and the sign sets the direction. The
effective macro is clamped to -100 and +100 after the source is added. The source is
relative rather than absolute, so the CC adds to the automation value instead of
fighting it. The six values left the generated CC map: the processor captures CC72 to
CC77, CC1 and the note-on velocity, so the sources are live values rather than patch
state.

Velocity is per note, so the design follows each voice and the display follows the
newest voice. The operator level, all-operator feedback and all-operator morph groups
are already per voice: the engine helpers read each voice's velocity and add the
offset at the operator's total level, feedback level or morph position. The operator
amplitude, feedback and spectral envelope rates also follow each voice, because those
envelopes are per voice; the node takes the offset once per control block and clamps
the rate to 0..99. The operator LFO rates follow each voice as a frequency ratio in the
panel's logarithmic domain, so an offset moves the rate musically and an offset of zero
keeps the stored rate exactly. The operator LFO depths follow each voice with the span
of their own parameter. A tremolo and a detune depth then move by the same fraction
despite their different ranges. The phase envelope and the phase LFO hold one state per
part, so their rates and depths stay in the part-level layer, as do the filter and
pitch envelopes and the other targets until their per-voice paths land. The carrier and
modulator groups also wait, because their role filter needs the algorithm and the
operator index inside the node.

The menu is `off`, `cc 72`, `mod wheel`, `velocity`. The range slider is active for
the three sources and greyed for `off`. The fixed CC is written on the option, so the
MIDI implementation is visible without a settings page. The range defaults to 0, so
a fresh macro is inert until the player sets one.

## Destinations

Each destination is a target and a bipolar range. The range is a percentage of the
target's own span, so 100 percent at macro 100 moves the whole range, and 50 percent
moves half. The sign sets the direction, and the result is clamped.

A group target moves every member by the same percentage of each member's own span,
and each member clamps on its own, so a group can be empty when the algorithm has no
carrier or no modulator, and an empty group does nothing.

The carrier and modulator roles come from the algorithm's routing table at runtime,
so a stored target is a role rather than a frozen operator list, and it follows an
algorithm change.

A parameter with a log curve, such as an LFO rate, takes the offset in the knob's
normalised domain rather than its raw value, because the raw LFO rate spans 0 to
20,000,000, where 4 Hz is 357,914, so a raw 10 percent offset would be about 22 Hz
instead of 4.4 Hz. The LFO rate parameters carry a skewed normalised range now:
0 stays the off stop, 0.01 Hz to 223 Hz is logarithmic above it, and the knob, the
host display and the macro offset all read that range.

The `velocity depth` group skips a member whose stored amount is 0, so a macro does
not give a follow target a velocity sensitivity the player never set. A member that
a macro drives to 0 returns to its stored value when the macro returns.

## Targets

Every row exists once per part, so the menu holds 58 target rows plus `none`, in
eight sections. The member count for a role row follows the algorithm: some
algorithms have one carrier, some four, and the modulators are the rest.

| Section | Target | Members |
| --- | --- | --- |
| part params | filter cutoff | 1 |
| part params | resonance | 1 |
| part params | pan | 1 |
| part params | detune | 1 |
| part params | velocity depth | the part's follow amounts |
| part params | pitch lfo rate | 1 |
| part params | pitch lfo depth | 1 |
| env rates | filter and pitch env rates | 8 |
| env rates | op env rates | 68 |
| env rates | carrier env rates | 17 per carrier |
| env rates | modulator env rates | 17 per modulator |
| op lfo rates | op lfo rates | 20 |
| op lfo rates | carrier lfo rates | 5 per carrier |
| op lfo rates | modulator lfo rates | 5 per modulator |
| op lfo depths | op lfo depths | 20 |
| op lfo depths | carrier lfo depths | 5 per carrier |
| op lfo depths | modulator lfo depths | 5 per modulator |
| op levels | op levels | 4 |
| op levels | carrier levels | carriers |
| op levels | modulator levels | modulators |
| op feedbacks | op feedbacks | 4 |
| op feedbacks | carrier feedbacks | carriers |
| op feedbacks | modulator feedbacks | modulators |
| op morphs | op morphs | 4 |
| op morphs | carrier morphs | carriers |
| op morphs | modulator morphs | modulators |
| op breakpoint xs | op breakpoint xs | 4 |
| op breakpoint xs | carrier breakpoint xs | carriers |
| op breakpoint xs | modulator breakpoint xs | modulators |

The op env rate count is 17 per operator: five amp env rates, four feedback, four
morph and four phase. The op lfo count is five families times four operators, which
is 20 rates and 20 depths.

There is no filter env amount target. The filter envelope levels are absolute
positions that start at the cutoff, so there is no depth to move. The old line in
[`README.md`](README.md) that gave the filter card an env amount was wrong and is
fixed in the same commit as this note.

## Panel

A ninth tab, `macros`, sits after `patch settings`, because both pages are patch
level. The page is six cards in a two by three grid, about 599 by 222 each. The knob
sits at the left and the routing at the right, which leaves room for a long target
name.

The card header carries the name and a `rename` that opens the existing dialog. A
macro name is empty until the player renames it, and an empty name draws `macro n`.
The left column holds a 92 px dial, the largest in the panel and a new token, with
the value below, then the `midi in` row and three destination rows on the right. A
row with `none` keeps its slider, greyed, so the card keeps its shape with no
targets.

Colours: purple for the destination range sliders, green for the MIDI-in range, and
0.35 alpha for a disabled range. The MIDI-in and destination sliders are both
bipolar, with the centre fill the pan slider already uses.

The page is built. Three snapshots cover an empty card, a card with three targets and
a live mod wheel contribution. The whole page sits behind the new tab. The target
menu is built from the generated table, so its eight headings follow the same list as
the engine. A component snapshot cannot capture an open popup. The table test checks
the menu rather than an image.

## Feedback on the destination

There are no rings. A targeted parameter shows its stored value in the usual pink,
and the effective value beside it in smaller green type. The effective value drops
the unit and keeps the multiplier prefix, so `900Hz 2.0k` reads as a move from 900 Hz
to 2.0 kHz and the pair fits the value line. While a macro has a non-zero range on
the parameter but sits at 0, the stored value is purple and the second value is
hidden. The pan slider's value text follows the same rule, and a group target marks
every member.

The macro knob keeps the minimum-origin pink arc, and its value text reads the knob
value. A dotted green arc runs from the knob value to the effective value, which
shows the source contribution.

## Follow cards

The key follow and velocity cards keep their 19 existing targets and gain the new
part and group targets, reorganised under headings:

- part: pitch env, filter cutoff, filter env, resonance, detune, pitch lfo rate,
  pitch lfo depth, filter and pitch env rates
- operator levels: op1 to op4 level, op levels, carrier levels, modulator levels
- operator feedbacks: op1 to op4 feedback, the three feedback groups
- amp envelope: op1 to op4 amp env, op env rates, carrier env rates, modulator env
  rates
- op lfo rates: the three role groups
- op lfo depths: the three role groups
- op morphs: the three role groups
- op breakpoints: the three role groups
- op phase: op1 to op4 phase

Pan stays out, because there is one pan per part and a key or velocity follow on it
needs a note policy. A group target shares one set of three amounts, key below, key
above and velocity, and fans out to every member. That is 45 targets and 135
parameters per part, so the part record grows by 78 values.

## Engine shape

The offset is computed per voice for the level, feedback, envelope, rate, LFO, morph,
breakpoint and phase targets, and per part for pan. The generator injects the offset
where each target is read, and the six macro values and their config are global
parameters in the state. The follow groups reuse the same path.

The largest item is the per-voice rate and LFO layer. The envelope nodes read their
rates from parameters, not signal inputs, so a per-voice rate offset needs a new
read for every rate in the amp, feedback, morph, phase, filter and pitch envelopes.

A macro at 0 with no follow amounts must render the current engine exactly, which is
the parity test.
