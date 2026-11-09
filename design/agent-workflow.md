# Interface work for the agent

This is the process that produced the Type P.alpha interface, and I'd use it for any
future web or JUCE interface work in this repository, because the design language
it assumes is the one in `README.md`, and a page that ignores that language is a
page I'll ask you to redraw, so read `README.md` before you open the mockup.

The general method, the plugin mapping and the design reference dossier sit in the
umetate development skill. The ten-item written gate is in that skill's `SKILL.md`
section 2.

## Use cases

Use this workflow when the task is visual or interaction design. That means a new
page, a component, a restyle, a font change, a layout change, or a plugin editor.

Start with a clickable mockup and make it right in a browser first, because the
interface is designed before it is written. Then write the code.

## Design loop

1. **Orient.** Read the design tokens, the mockup, and the parameter manifest for
   the project. `examples/type-p-alpha/patch.json` is the only reference for controls
   and ranges.
2. **Decide one set.** Take the smallest coherent set of changes, so unrelated
   restyles stay in their own commit.
3. **Back up.** Before an experiment, copy the mockup and commit it, since the user
   may ask for the backup to be removed later.
4. **Change.** Edit the mockup, and leave the generated bundle alone.
5. **Render.** Open the page at a fixed viewport, at 1x, and take a screenshot.
6. **Inspect.** Look at the full page and zoom into dense areas, checking labels,
   collisions, clipping, scrollbars and alignment.
7. **Verify interaction.** Open the dialogs, change a value, mute a part, switch a
   tab, because static screenshots do not prove a control works. Click the thing.
8. **Check the height.** All pages must keep one fixed window height, so re-measure
   after any type or content change.
9. **Commit.** One decision set, one commit, and update the tokens and the roadmap
   when the design changes.

## Lesson record

These are the mistakes that cost time while I was making the mockup. Each one is
here so the next pass is shorter.

- **A screenshot can lie about timing.** Read the file only after the browser has
  exited or the capture is complete, because a missing screenshot is often a race
  rather than a rendering bug.
- **Check the page and the state.** Many captures share a filename, so confirm the
  fragment, the selected tab, and the open dialog before judging a change.
- **A class-based rule only works if the class exists.** The operator knobs stayed
  cramped because the section kind class was never applied to the card, so when a
  specific rule does not take effect, inspect the DOM before changing the rule,
  since the rule is usually fine and the element is what is missing.
- **Font loading can block the page.** A stylesheet in the head can hold the
  inline script until the network answers, so preload it and append the stylesheet
  after the first paint, with local fallbacks.
- **A fixed height drifts with type changes.** Measure the operator page after
  every font change, and update the body minimum height and the grid minimum
  heights together.
- **Tag labels need real spacing.** Enlarging type without widening the knob cell
  makes `ratio` and `detune` touch, so check the rendered label edges as well as
  the dial centers.
- **Hidden scrollbars can hide a layout bug.** The screenshot flag can hide the
  scrollbar, so verify scrollers in a normal browser profile.
- **Japanese and kanji need the right font.** DotGothic16 covers the brand line
  and the 猫 glyph, and a Latin-only display font probably will not, so check
  before you assume it does.
- **Do not add filler to fill space.** Fix the layout, or leave the space, because
  the user does not need a description of the product inside the product.
- **Type size is optical.** The same nominal size renders at a different height in
  the browser and in JUCE, so fix the mapping once in the theme and keep the token
  scale as the design value.
- **A switch states its state.** A toggle that always reads `on` teaches the player
  nothing. Show `on` and `off`, and prefix the control name when it needs one.
- **Pointer steps need a scale.** A wheel notch at 15 percent of a knob is unusable
  for a level or a rate. Use about 2 percent, with a modifier for finer control.
- **Tabs are one shape.** When the active tab overlaps its neighbour, paint it
  unclipped and let the tail reach left. A clipped tab reads as a detached label.
- **Proximity separates actions from state.** Keep a visible gap between a utility
  button group and the switch it acts on. Move the group, not the switch that
  already has a home.
- **A control cluster stays with its trigger.** Put a mode's knobs beside its mode
  buttons, not at the far edge of the card.
- **Fit the window at startup.** A host can set the editor size after it attaches,
  so a constructor-only cap is not enough. Reserve room for the host chrome and
  retry for a moment.

## Design review checklist

- Does every page show black, pink, purple and green in a meaningful role?
- Are the fonts the intended families, at the intended sizes, in lowercase?
- Do the labels and values have clear space at 1x?
- Is there exactly one fixed window height across pages?
- Are all surfaces borderless except controls, selection and the frame?
- Does the active tab use the cut polygon?
- Is the mute state legible, with the correct word lit?
- Does every state switch read `on` or `off` rather than a fixed label?
- Do utility actions keep a visible gap from the mute switch?
- Does the editor fit the host window at startup, including the host chrome?
- Do scrollers work and show a scrollbar in a normal browser?
- Do note fields show both the MIDI number and the note name?
- Does the legal state match the parameter manifest, with no invented controls?
- Are the OFL fonts bundled or loaded non-blocking, is the wordmark all Zen Dots,
  and is the brand font out of the binary?
- Does the page read as an instrument rather than a settings form for an app?

## Definition of done

- Mockup and tokens are committed and in sync.
- Every interactive control has been clicked in a browser.
- Screenshots of every page and dialog are current.
- Fixed height is measured rather than assumed.
- Roadmap has one line for the milestone.
- JUCE and web mapping documents still describe the mockup.

## Handoff to implementation

When the mockup is signed off, extract tokens and build the shared component
recipes, then generate `UiTokens.h` from `tokens.json` so the web and JUCE sides
read the same values. Nobody translates pixels by eye in C++.

The first implementation milestone is a reusable host plus a generated
`type-p-alpha` project rather than a one-off plugin. The parameter manifest drives
the controls, the plugin state and the host automation.
