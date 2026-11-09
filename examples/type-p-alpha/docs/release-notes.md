# Release notes

Newest first.

## 1.0.8

- LFOs: the amplitude and pitch LFOs each have a unipolar and bipolar switch. Unipolar starts at the target value and moves to the depth. Bipolar swings around the target value. Amplitude LFO depth is now in the same level steps as `LEVEL`, from -99 to +99. Pitch LFO depth runs from -12.00 to +12.00 st.
- LFO rate: below 1 Hz the display keeps two decimals, so 0.50 Hz and 0.75 Hz no longer round to 0.5 and 0.8.
- Pitch envelope: the plugin adds `L4`, the level the release segment ends at. It defaults to 0.00 st, so existing patches keep their release.
- Toggle switches: the text beside a switch now reads `on` when it is on and `off` when it is off.
- Plugin library: the multi-patch browser has `export bank` and `import bank` buttons. Both use the operating system file dialog, and the file carries the part and multi-patch banks. The browser also has `init multi-patch`, with a confirmation before it wipes the current patch.
- Plugin naming: the header button says `multi`, the legend below it points to the multi-patch library actions, the browser is titled `multi-patches`, and the save-mode dialog is titled `save multi-patch`.
- Plugin editor: every type size is 4 px larger, the mouse wheel moves a knob in 2 percent steps instead of 15 percent, and the window is checked against the display area after the host attaches it, so a host scale cannot open it past the screen.
- Plugin brand line: the `p1onken` line is indented two spaces further.

## 1.0.7

- Filter envelope: the global F-ENV now retriggers on every note-on, so a sequencer restarts the filter shape for each note instead of leaving it at sustain. The filter and its envelope are still global, shared by both parts.
- Filters: FILTER 2 and the wavefolder are removed. The signal chain is FILTER 1, then the DC blocker. The CC map is three entries shorter than 1.0.6, so the CCs after the wavefolder move down by three.
- Voice mode: portamento is mono-only. In poly the plugin disables legato, unison, detune and porta, and a stolen voice snaps to pitch.
- Part delay: the delay now works in mono as well as poly. In mono it applies to the first note after silence, because a later note takes over the sounding voice.
- Plugin library: the part and patch library is now a global user library. It loads when the plugin opens and saves after a part save, a patch save or an import, so saved parts and patches survive a restart and are shared across projects. The standalone app and the VST use the same library file. The part menu also gains `export library...` and `import library...`, which write and read a `.p1lib` file with the part and patch banks. Individual parts still use `save part...` and `load part...`, and individual multi-patches use the `patches` browser.
- Plugin display: the envelope graphs now show the attack, decay, sustain, and release like the firmware. The select text and algorithm operator labels are 12px, the algorithm popup drops the redundant load instruction, the brand line is indented, and INIT PART asks before it wipes the part. The initial window size is capped to the screen's free area and scales proportionally.

## 1.0.6

- GLO pages: MONO STACK now holds only VOICES and DETUNE. The part tune moved to a new SINGLE PART TUNING page, placed straight after SINGLE PART LAYERING.
- Tuning: each part has a detune of -50 to +50 cents and a transpose of -24 to +24 semitones. The transpose is a whole-semitone offset, so the two parts can sit an octave apart without touching the operator ratios.
- Patch loading: the PRESETS browser now guards unsaved work. A clean patch still loads after the two second idle, and the already-loaded slot just closes. If you have edited the patch, a CONFIRM popup asks before the load. SAVE loads and discards the edits, and HOME keeps them. The single-part browser still opens its explicit part chooser.
- Save modes: the multi-patch save modes now work on both parts. `discard edits to parts` uses both stored part slots, `save as new parts` writes both parts to new slots, and `mutate parts in place` writes both parts into their existing slots. A part with no stored slot gets a new one in the latter two modes.
- Plugin save: the patch save dialog asks the same three modes and applies them to both parts. The plugin's part records now use the 135 per-part params the device uses, so part 1 saves and loads correctly. Re-save plugin part presets made before 1.0.6.

## 1.0.5

- Aliasing: the band-limited pulse, square and saw tables now run from 256 harmonics at the bottom of the keyboard down to one at the top. A C1 note used to stop at its 32nd harmonic, which left a 1 kHz edge behind the note. The edge now sits at 8.4 kHz at C1 and C2, so the lower octaves lose the faint tone. The table stays const XIP and the oscillator read is still one load.
- FM: the init patch also sets operator 1's and operator 3's low level key follow to -50. In the stacked algorithms they are modulators, so the high sidebands at the low notes drop by 10 to 20 dB.

## 1.0.4

- Filters: FILTER 1 and FILTER 2 are now the 24 dB/oct liquid ladder. It has low-pass, band-pass and high-pass modes, a level-preserving resonance make-up, and a cutoff envelope that uses the whole range.
- Resonance: the display reads 0.00 to 100.00 percent with an exponential taper. The top of the knob is the strongest stable resonance, and the curve gives fine control near it.
- Cutoff: the top of the range stops at the ladder's stable coefficient ceiling, so the filter stays clean at every resonance.
- Envelopes: decay 2 has its own rate curve. Rate 1 is the engine's slowest musical point and lines up with the OPM's slowest second decay, and the 99 steps cover the OPM's 31 with more resolution. A converted sustain that fades over half a minute keeps its length, subject to the one-pole floor near 22 seconds.
- Oscillators: the pulse wave is now the OPL pulse-sine at the note rather than a shape that repeated twice per cycle. A DC blocker at the output removes the offset from the pulse and the saw.
- Aliasing: the band-limited tables now keep their top harmonic an octave below Nyquist, which removes the high whine behind pulse, square and saw notes from C5 upwards.
- FM: the init patch sets operator 1's high level key follow to -50. In the stacked algorithms that operator is a modulator, so the sidebands narrow at the top and the folded sideband energy at C5 and C6 drops by about 40 dB.
- Pitch LFO: depth now reads 0.00 to 12.00 semitones as the peak deviation, with one-cent steps through the first semitone and the envelope-style detents above it.

## 1.0.3

- Level: the operator level is now the Yamaha total-level scale, 0.75 dB per step. Level 75 on a ratio 1 modulator gives half a cycle of phase deviation, and the level-to-index table matches the OPM and the DX21, DX27, and DX100.
- Modulators: the phase-modulation scale reaches the OPM's four-cycle full-scale deviation at level 99. A 1:1 pair follows the VOPMex centroid within about 11 percent from `TL` 0 to `TL` 32.
- Feedback: the self-feedback tap takes the level-gained output, averages two samples, and reaches two cycles at a full-level operator. The depth follows the operator level, so a decaying sound keeps its shape.
- Ratio knobs: the knob steps through multiples of 0.50, 1, √2, π/2, and √3, with √2/2, π/4, and √3/2 near the bottom. The firmware, web app, and plugin share the ladder, and the plugin's arrow keys step it.
- Effects: the filters and the wavefolder start bypassed at power-on and in the plugin. Part 2 stays active.
- Scopes: the plugin and screen scopes are rescaled for the new level curve, with a soft knee so a loud trace stays inside the widget.
- Docs: the OPM mapping covers the level curve, the feedback depth law, and the measured cases. The envelope note gains the stage lengths and curve shape.
- Licensing: the plugin archives carry their notices in `licences/`, the source archive includes `vendor/deps/`, the Terminus OFL notice is added, and VOPMex is credited.

## 1.0.2

- Adds a live PERFORMANCE page to HOME with cutoff, resonance, wavefolder amount, and operator 1 feedback.
- Moves TUNE to MONO STACK, so the part tune is reachable from a page knob.
- Draws one amplitude envelope per operator on the HOME A-ENVS page.
- Adds a confirmation popup before INIT PATCH.
- Manual: the full panel walkthrough, the parameter reference, and the save, load, init, and copy flows.
- Browser app: draws the 240 by 240 screen at 1:1 on a wide desktop and uses the plugin interface language, with self-hosted Zen Dots and DotGothic16.
- Help panel: a single `controls` section for the key map, the knobs, the buttons, and the shortcuts, plus `links`. It drops the `what this is` section, and the links open the manual, release notes, and source on GitHub.
- Wordmark: `type P.fm1` in Zen Dots. `type` is lowercase, `P` is uppercase, `.fm1` is lowercase, and the dot is pink. The browser app and the plugin use the same mark.

## 1.0.1

- This release works on units that ship the UC25HQ80 flash chip (JEDEC id
  `0xB36014`), which is common on recent AliExpress stock. Those units used to
  show a flash error when saving (saves stayed in RAM), and the update loader
  refused to install. The firmware now accepts both the Puya P25Q80 (`0x856014`)
  and the UC25HQ80 (`0xB36014`), in the app and in the update loader.

## 1.0.0

The first release ships the full instrument.

- Four-operator FM with eight algorithms, two multitimbral parts, twenty voices.
- Two multimode filters and a wavefolder.
- Per-operator feedback, detune, twelve waveforms, four-stage amplitude envelope,
  and tremolo.
- Voice modes: poly and mono, unison up to eight, legato, portamento, and note
  priority.
- Pitch LFO and per-note pitch envelope.
- Key and velocity follow across the operators, filters, and envelopes.
- A 240 by 240 panel built on the M-VAVE FM-1 surface, with the operator editor,
  the output chain, the global modulation, and the save and preset flows.
- MIDI in with CC control, sustain, panic, pitch bend, and program change.
- SysEx for the parameters, the part patches, the multi patches, and the settings.
- A browser app that runs the same sound engine, for trying patches without a
  device.
