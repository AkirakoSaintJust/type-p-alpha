# Feature contract: library revision

## Identity

- **Feature:** Give the shared preset library a revision, merge the slots each
  instance changed, refuse a slot both sides changed, and stop a project from
  writing its bank copy over the library.
- **Brief section:** README research item 7 and the persistence section of
  [`../docs/codebase-audit.md`](../docs/codebase-audit.md).
- **Owner / date:** P1Onken, 2026-11-19.
- **Disposition:** complete. The in-process two-store tests and the plugin smoke
  pass in the Linux image, and the CI run is still to follow.

## Player outcome

- **New player action:** none. Two open instances keep each other's saved slots
  instead of the second save replacing the file.
- **Sound and behaviour:** saving a slot first reloads the file, then writes the
  slots this instance changed. A slot the other side changed keeps the disk
  version, and the editor names the slot and the patch. Opening an old project no
  longer rewrites the library with the project's copy.
- **Failure mode:** a silent overwrite of another instance's slot, a project load
  replacing every slot with an older bank, or a corrupt file written over.

## Dependencies and interactions

- **Other features:** the preset store, the plugin processor, the editor's save
  paths, the project state and the library browser.
- **Platform/API/HAL changes:** none. `PresetStore` gains revision and change
  tracking, and `saveLibrary` returns a result instead of nothing.
- **Persistence/version impact:** the library header becomes `P1LIB3` with a
  4-byte revision. A `P1LIB2` file loads as revision 0, so an old library keeps
  working. The preset record version is unchanged by this feature.
- **MIDI/clock impact:** none.

## Parameter and event semantics

- The revision is written at the start of the file, after the 7-byte magic. A save
  writes `max(local, disk) + 1`.
- Each instance records the revision it loaded and the value every changed slot had
  before its first change.
- Before a save, the file is loaded into a fresh store. A dirty slot whose disk
  value still equals its base is copied across. A dirty slot whose disk value has
  moved is a conflict: the disk value stays and the slot is reported.
- A conflict does not block the other slots. The non-conflicting changes are still
  written, and the editor reports the first conflict by bank, slot and name.
- A project still carries a copy of the part and multi banks. `setStateInformation`
  no longer loads them into the store, so a project cannot write the library.
- The write stays atomic through `juce::File::replaceWithData`, which writes a
  temporary file and replaces the target.

## Evidence

- `preset_smoke` passes `a slot only A changed merges without a conflict`, `both
  instances keep their slot after the merge`, the multi-slot merge, `the right
  instance reports the slot both changed`, `the conflicting slot keeps the disk
  version`, the v2 load as revision 0, and the old-record-size expansion.
- `plugin_smoke` passes `a project load does not overwrite the shared library` and
  `the reader's own slot still reaches the library`, with a second processor over
  the same temporary file.
- The Linux image builds the plugin and runs 14 of 14 tests.

## Limits

- **The conflict is resolved by hand.** The local edit stays in the editor and the
  user is told to save it under another slot. There is no in-place merge of two
  versions of one patch.
- **The file is read on save, not watched.** Another instance's new slots appear in
  the browser after this instance saves or reloads, not the moment they land.
- **A corrupt library is refused.** The save reports that the file could not be read
  and writes nothing, rather than replacing a file it cannot parse.
- **Two processes are not tested.** The merge is covered by two stores in one
  process over one file. A manual two-instance check belongs with the release.

## Risks and fallback

- The merge copies whole records, so a slot's size is the record size. The part bank
  is the large one, and the save reads it once per save.
- The fallback is to stop merging and write the local bank whole, which is the old
  behaviour. That loses the other instance's slots, so the conflict report is the
  part to keep if the merge has to be reduced.

## Safe experiment and stop criterion

Open two instances on one library, save a different slot in each, then save a slot
both changed. Stop if the first instance's slot disappears, if the conflicting slot
is overwritten, or if the file fails to load after either save.
