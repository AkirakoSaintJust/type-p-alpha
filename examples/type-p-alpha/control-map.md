# Control map (UI plan), type-p-fm1

> The UI interview, translated into `panel.json`. The control surface is the real
> M-VAVE FM-1: a smooth `MASTER` volume pot, three dedicated left encoders
> (`SELECT`/`ALGORITHM`/`PRESETS`), four page-slot encoders (`KNOB1–4`), fourteen
> buttons, and a 27-key keybed. The editor uses a **page-slot** model: `KNOB1–4`
> edit the four on-screen parameter rows, `SELECT` pages, and `SEL` selects the
> operator.

## 1. Surface inventory

- **Board / revision:** FM-1 (`--board fm1`).
- **Controls and their kinds:** 27 keys (momentary note, F3–G5). `MASTER` is a
  smooth analogue pot (volume, platform-owned, never a page slot), whilst `SELECT`,
  `ALGORITHM`, `PRESETS` and `KNOB1–4` are detented encoders and `OCT− OCT+ FX SEL
  ENV LFO EDIT GLO HOME SAVE ARP SEQ PLAY REC` are momentary buttons.
- **Knob kinds:** dedicated (`ALGORITHM`, `PRESETS`) and page-slot (`KNOB1–4`),
  and `MASTER` is volume.
- **Silkscreen labels:** `ALGORITHM`, `PRESETS`, `ENV`, `LFO`, `FX`, `GLO`,
  `EDIT`, `HOME`, `SAVE`, `SEL`, `OCT−/OCT+`. The firmware means what they say.
- **Display / fonts / LEDs available:** a 240×240 LCD, with a widget set that
  includes `label`, `value`, `bar`, `param_row`, `param_column`, `list`, `grid`,
  `op_row`, `part_row`, `envelope`, `genv`, `scope`, `spectro`, `algo`, `name`,
  `status`, and `custom`, plus per-key and per-button LEDs.
- **Controls explicitly unmapped:** none. `ARP/SEQ/PLAY/REC` mute operators and, with
  `SEL`, mute parts, whilst `PRESETS` opens the browser, `SAVE` opens the save
  chooser, and `ENV` and `LFO` open their own contexts.
- **Firmware-level controls:** `OCT−`/`OCT+` shift the keybed base note a whole
  octave (`app_oct`, clamped ±2) in `app.c`, whilst `MASTER` is platform volume.

## 2. Player model

- **Performance gesture:** one hand on the keys and one on the `ALGORITHM` encoder,
  with the four page knobs belonging to the page you open.
- **Menu depth tolerance:** six sibling contexts (`home`, `edit`, `env`, `lfo`,
  `fx`, `glo`), so the stack is `[home, mode]`, depth 2.
- **Keys as shortcuts:** `SEL` + key mutes the active operator, and the keybed does
  not double as a menu surface.
- **What must stay playable during any interaction:** held notes, so redraw is
  rate-bounded and never runs in the audio callback.

## 3. Navigation

- **Context buttons:** `HOME`, `EDIT`, `ENV`, `LFO`, `FX` and `GLO`. A button opens
  its context on the page it was left on, and the first visit opens page 0. Pressing
  the same button again advances to the next page with wrapping, and `SELECT` moves
  forward and back through the current context. Each context keeps its own page
  while another context is open.
- **HOME:** pops back to the home context, and pressing `HOME` while already home
  advances to the next home page.
- **Page knobs:** `KNOB1–4` edit the page's parameter rows from top to bottom, so a
  page with fewer than four rows leaves the extra knobs idle, and display pages have
  no assignments.
- **Modifier chords:**

| Gesture | Action |
| --- | --- |
| `SEL` (single tap) | Next operator of the active part, after the double-tap window |
| `SEL` + `SELECT` twist | Move the operator selection through the active part, wrapping |
| `2x SEL` | Switch the active part |
| `SEL` + key | Mute the active operator |
| `SEL` + `EDIT` | Copy the current page |
| `SEL` + `SAVE` | Init patch, or init part in GLO |
| `SEL` + `ARP` or `SEL` + `PLAY` | Mute part 1 |
| `SEL` + `SEQ` or `SEL` + `REC` | Mute part 2 |
| `ARP`, `SEQ`, `PLAY`, `REC` | Mute operators 1, 2, 3, 4 |

## 4. Contexts and pages

### `home`

Display and performance context, nine pages. It opens on `PERFORMANCE`, the live
page with cutoff, resonance, and operator 1 feedback, whilst the other eight pages
are displays, and the four page knobs have no assignments there.
The `ALGORITHM` encoder edits the active part's algorithm and shows the graph popup.

| Page | Contents |
| --- | --- |
| PERFORMANCE | Cutoff, resonance, operator 1 feedback. |
| ALGORITHM | Algorithm graph and routing string. Carriers pink, modulators white. |
| A-ENVS | An envelope graph for each operator of the active part. |
| SCOPES | Four operator scopes for the active part. |
| SPECTROS | Four operator spectrograms for the active part. |
| SCOPES | Two larger part-mix scopes. |
| SPECTROS | Two larger part-mix spectrograms. |
| POST-FX MULTI-PATCH SCOPE | Final mix scope after the filter. |
| POST-FX MULTI-PATCH SPECTRO | Final mix spectrogram. |

### `edit`

Operator editor, four pages, operator-relative except `OP DETUNES`.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| OSCILLATOR | RATIO | LEVEL | FEEDBACK | WAVE | Operator band, scope kind |
| OP DETUNES | OP 1 | OP 2 | OP 3 | OP 4 | Four part-level detunes |
| OP KEY FOLLOW | OP LEVEL KS LO | OP LEVEL KS HI | OP FB KS LO | OP FB KS HI | Selected operator |
| OP VEL SENS | OP LEVEL VS | OP FB VS | | | Selected operator |

The oscillator page and the A-ENV pages share one **operator band**, six rows tall
(96 px) between the last parameter row and the footer legend. A carrier gets one
graph filling the band. A modulator gets its own graph on the top two rows, a label
row naming its direct targets, and one target graph per column on the next two
rows. The last row stays free above the legend. The direct targets come from the
current algorithm, so the band has no controls of its own and the column count
follows the routing: one target at 240 px, two at 120 px, three at 80 px. A hairline
separates the target columns. The selected graph is named by the page title, and
each target column is named by its label.

### `env`

Envelope context, six pages, where A-ENV follows the selected operator and P-ENV is
part-level.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| A-ENV (rates) | ATTACK RATE | DECAY 1 RATE | DECAY 2 RATE | RELEASE RATE | Operator band, envelope kind |
| A-ENV (levels) | LEVEL 1 | LEVEL 2 | SUSTAIN | | Operator band, envelope kind |
| P-ENV (rates) | ON | ATTACK RATE | DECAY 1 RATE | DECAY 2 RATE | P-ENV graph |
| P-ENV (levels) | RELEASE RATE | LEVEL 1 | LEVEL 2 | SUSTAIN | P-ENV graph |
| ENV KEY SCALE | A-ENV RATE KS LO | A-ENV RATE KS HI | P-ENV RATE KS LO | P-ENV RATE KS HI | |
| VEL SENS | A-ENV RATE VS | P-ENV RATE VS | | | |

### `lfo`

Two pages, where `A-LFO` follows the selected operator and `P-LFO` is part-level.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| A-LFO | RATE | DEPTH | WAVE | | Selected operator's amplitude LFO |
| P-LFO | RATE | DEPTH | WAVE | | Part pitch LFO |

### `fx`

Output chain, five pages, where the signal order is filter 1, then the DC
blocker. The filter has the F-ENV.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| FILTER | ON | CUTOFF | RES | TYPE | Filter 1 |
| F-ENV (rates) | ATTACK RATE | DECAY 1 RATE | DECAY 2 RATE | RELEASE RATE | F-ENV graph |
| F-ENV (levels) | LEVEL 1 | LEVEL 2 | SUSTAIN | DEPTH | F-ENV graph |
| F1 KEY FOLLOW | F1 CUTOFF KS LO | F1 CUTOFF KS HI | F-ENV RATE KS LO | F-ENV RATE KS HI | |
| FILTER VEL SENS | F1 CUTOFF VS | F-ENV RATE VS | | | |

### `glo`

Setup and global context. Eleven pages.

| Page | KNOB 1 | KNOB 2 | KNOB 3 | KNOB 4 | Screen |
| --- | --- | --- | --- | --- | --- |
| SINGLE PART LAYERING | PART 1 LEVEL | PART 1 DELAY | PART 2 LEVEL | PART 2 DELAY | Two part levels and delays |
| SINGLE PART TUNING | PART 1 DETUNE | PART 1 TRANSPOSE | PART 2 DETUNE | PART 2 TRANSPOSE | Part detune and transpose |
| KEY SPLIT | P1 LOW | P1 HIGH | P2 LOW | P2 HIGH | Key ranges |
| MIDI CH | P1 CH | P2 CH | | | Part channels |
| KEY SCALING | BREAKPOINT | KEY SENS ON | | | Targets in EDIT/FX/ENV |
| VEL SENS | VEL SENS ON | | | | Targets in EDIT/FX/ENV |
| VOICE MODE | MODE | PRIORITY | LEGATO | PORTA | The part voice mode |
| MONO STACK | VOICES | DETUNE | | | The mono stack and its spread |
| NAMES | PART 1 NAME | PART 2 NAME | MULTI-PATCH NAME | | Opens the name editor |
| BACKUP / LOAD | | | | | Host-side SysEx, model byte 64 |
| VERSION + CREDITS | | | | | Firmware and typeface credit |

## 5. Flows

- **Save:** `SAVE` opens the chooser. `KNOB1` toggles single part or multi-patch,
  `KNOB2` scrolls the slot, `SAVE` commits, `HOME` cancels, a single part then opens
  the part chooser, a multi-patch then opens the mode chooser (`discard edits to
  parts`, `save as new parts`, `mutate parts in place`) for both parts, and the bank
  write waits for silence.
- **Load:** `PRESETS` opens the multi-patch browser outside GLO and the single-part
  browser in GLO. The popup shows the slot and name. After two seconds of idle, a
  clean patch loads, and the already-loaded slot just closes. If the patch has
  unsaved edits, a confirm popup asks `LOAD PATCH? UNSAVED EDITS`, where `SAVE`
  loads and discards the edits and `HOME` keeps the work. A single-part load still
  opens the part chooser first, and that chooser needs `SAVE` to apply.
- **Init:** `SEL+SAVE` outside GLO opens a confirmation popup, where SAVE resets the
  whole patch, mutes part 2, and resets the names, and HOME cancels. In GLO it opens
  the part chooser to initialise one part.
- **Copy:** `SEL+EDIT` on a page with copyable params, where part pages copy to
  another part, operator pages copy to another part and operator, `KNOB1` picks the
  target, `SAVE` applies, and `HOME` cancels.
- **Names:** from `GLO/NAMES`, the matching knob opens the editor, where `K1` changes
  the character, `K2` moves the cursor, `SEL` deletes, `SAVE` accepts, `HOME`
  cancels, and an idle close restores the old name.

## 6. LED / pad feedback

- Sounding keys light.
- `OCT−`/`OCT+` light when the keybed is shifted below or above its base.
- Muted parts and operators show red in the header.
- The selected part and operator show light blue in the header.
- Parameter values and the algorithm number are hot pink. The page description is
  bright green.

## 7. Boot screen

- **Needed?** No.

## 8. Control flow

- **Context transitions:** boot → `home`, where `EDIT`, `ENV`, `LFO`, `FX`, and
  `GLO` are siblings, a switch pops back to home and pushes the new context, and
  pressing the same context button again advances its page.
- **Full context stack, worst case:** `[home, mode]`, depth 2.
- **Selected operator:** a flat 0..3 counter (`op_next`), shared across contexts.
- **Selected part:** a flat 0..1 counter (`part_next`), shared across contexts.
- **Unresolved or ambiguous bindings:** none.

## 9. Open problems

None after the panel fixes, because the live page, the part detune and transpose
placement, the per-operator envelope graphs, and the INIT PATCH confirmation all
match `panel.json` and the app handlers.

## 10. Acceptance

- **Host UI tests:** `test_generate.py` resolves `panel.json` against the patch's
  `param_ids`. The generated `panel.h` compiles against `ui.h`.
- **Device checks:** held notes during redraw. Visible safe-failure screen,
  `blocked` until hardware.
