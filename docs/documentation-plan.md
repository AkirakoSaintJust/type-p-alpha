# Documentation plan

Status: draft, 2026-11-20. This is the design note for the player manual and the
developer document. The contracts in `plugin/` stay as the reference and the
implementation evidence; the two documents stand on their own.

## Registers

The project has three visual scopes. They share the colour roles and the grid
logic, and each has its own sizes.

- **Panel.** The editor, from `design/tokens.json`. Two families, three colour
  roles, flat surfaces, the fixed 1240 by 985 canvas.
- **Screen document.** The manual and the developer document as HTML. Dark,
  because the project's ground is black. Long-form sizes, a raised reading
  surface, and the same signal colours as accents.
- **Print document.** The same documents as A4 PDF. Light paper, dark covers and
  chapter openers, ink-aware versions of the signal colours, and a print type
  scale.

A poster or a cover can add texture, a display face and a larger scale. The panel
cannot. That is the point of the split: the print register can be expressive
without forcing the editor to change.

## The two documents

### Player manual

The reader is a synthesist who knows subtractive synthesis and finds complex
digital synths overwhelming. The manual teaches the instrument through sound
rather than through a feature list.

1. **Make a sound.** One patch, the minimum controls, the result.
2. **The model.** Operators, phase, feedback, the two parts, explained through
   the patch already in the reader's hands.
3. **The panel.** Follow the workflow: make a sound, shape it, add a second part,
   split the keyboard.
4. **Recipes.** Six to eight patches, each with a goal, the settings and one
   variation.
5. **Reference.** Every parameter, range, default, display and page.

The manual is thorough when the reference is complete and the recipes are honest.
It is attractive when the panel shots, the envelope and spectrum diagrams, and
the patch cards are designed as figures rather than dropped in as screenshots.

### Developer document

The reader needs the exact workings and has to be able to join the project. The
document is a complete onboarding path, not a contract digest. The contracts are
linked as evidence at the end of a section; the section stands alone.

1. **Front matter.** The version line: commit, manifest count, `PRESET_VERSION`,
   state version. Who the document is for.
2. **Start here.** The two repositories, the toolchain, the build on Linux,
   Windows and macOS, the standalone launch, one note, and the test command.
3. **The repository map.** Product against framework, generated against
   hand-written, `.dev` and why it is ignored, `build/` and why nothing there is
   committed.
4. **The mental model.** The graph, the host, the data flow, and the invariants:
   48 kHz, the 32-sample block, integer formats, per-instance state, no
   allocation, the output bridge.
5. **The workflow.** Change a parameter or a node, regenerate, rebuild, run the
   test, update the contract, accept the snapshot, commit.
6. **The DSP families.** Operators, envelopes, LFOs, feedback, filter, spectral,
   pan and pitch. Signal path, math, state, timing, edge cases, and the test that
   proves each.
7. **The formats.** `P1LIB2`, the state blob, the version chain, the CC map.
8. **The test ladder and CI.** What each test proves, how to run it locally,
   which layer covers it, and which layers are blocked.
9. **The design language and the UI gate.** The tokens, the fixed canvas, the
   snapshots, the control map.
10. **The release and licence.** The workflow, the packages, the notices and the
    source obligation.
11. **The decisions and the archive.** Why 48 kHz, why the filter law, why the
    envelope family, why the browser build stopped, what the FM-1 tag holds.
12. **The glossary, the code map, the troubleshooting and the contribution
    path.**

## Visual system

- **Display.** Zen Dots, and the vector wordmark once it exists. The manual cover
  can use the wordmark at a large size; the panel header keeps the 30px mark.
- **Body.** Libre Franklin as the pilot choice. A low-contrast grotesque with an
  industrial and editorial lineage, a real text cut and a readable italic. It
  works at 17px on screen and 10.5pt on paper. Overpass is the screen alternative
  if the pilot reads too much like a newspaper. Spectral is the print-only serif
  alternative if the light PDF needs more reading comfort.
- **Mono.** Fira Mono. No ligatures, so commands and parameter IDs show the exact
  glyphs. Regular and bold are enough.
- **Screen document.** 17px body, 1.6 leading, a 65 to 75 character measure, a
  raised surface above the black ground, and the signal colours for accents.
- **Links.** Body links are pink, hover pinkBright with an underline, and carry a
  visible pink outline on focus. The contents links are dim and hover to pink.
  Green and purple keep their live and selected meanings.
- **Print document.** 10.5pt body, 15pt leading, A4, 20mm margins, a 12-column
  grid. A light body, a dark cover and dark chapter openers. The print pink,
  green and purple are darker than the screen values so text and thin marks keep
  their contrast on paper.
- **Figures.** The panel shots come from the snapshot goldens. The envelope and
  spectrum diagrams are generated as SVG from the same data the engine uses. The
  signal-flow diagrams come from the graph. Hand-drawn figures are the first
  thing to go stale.

## Pipeline

- Chapters live in `manual/chapters/` as Markdown. `manual/manifest.json` carries
  the order and the titles.
- `manual/build.py` renders the chapters to HTML with the screen theme, and to a
  print HTML with the print theme. It calls a headless browser for the PDF.
- The outputs go to `build/manual/`: `index.html`, `print.html`, `manual.pdf` and
  the figures. `build/` is git-ignored. The PDF is attached to the release.
- The screen HTML is part of the project site. The PDF is a download.
- The pilot uses Google Fonts so the pipeline is easy to test. The production
  build self-hosts the OFL files, and the colophon and the site carry the
  notices.
- The print PDF from Chromium does not support `@page` margin boxes, so the pilot
  has no running page numbers. The final pipeline can use WeasyPrint or a
  paged-media step for margin boxes, or accept a fixed footer.

## Generated tables

The exhaustive part of the developer document is generated, not typed:

- the parameter reference from `engine/gen/params.json`;
- the test list from the CTest registration;
- the file map from the repository;
- the version line from the preset, state and manifest headers.

A CI check compares the generated sections with the source and fails when the
document is stale. The prose is hand-written.

## Content rules

Every claim carries a number, a patch or a link. No catchphrases, no stock
photography, no gradients, no testimonials, no email gate and no fake urgency. The
limitations sit next to the claims. The voice is the README's: first person,
concrete, British, with the hesitations kept.

## Pilot

`manual/chapters/01-make-a-sound.md` as Markdown, HTML and PDF. It carries one
panel figure, one generated envelope diagram and two patch cards. The pilot tests
the pipeline, the screen and print themes, the figure generation and the voice
before the rest of the manual is written.

## Sequence

1. This plan, the pilot chapter and the build pipeline.
2. The one-page site with the manual and the developer links.
3. The player manual, chapter by chapter.
4. The developer document, chapter by chapter.
5. The panel refinements, additive first and the tab map last.

## Open items

- The body face: test Libre Franklin in the pilot, compare with Overpass, and add
  Spectral only if the print needs a serif.
- Self-host the OFL fonts and add the notices to `third-party-licenses`.
- The static site host.
- The developer document version line and its generated sections.
