# Presets and library

The preset layer has a byte-exact record format, a versioned migration chain and
a read-merge-write library. The format is shared with the device history, so the
record functions in `engine/runtime/presets.c` are the reference for the
layout. The plugin store is `plugin/preset/PresetStore.cpp`.

## The library file

A library is:

```text
7 bytes   magic "P1LIB3\n"
4 bytes   revision, little-endian
4 bytes   part-bank size
N bytes   part bank, 128 records
4 bytes   multi-bank size
M bytes   multi bank, 64 records
```

A version 2 file has no revision field, so it loads as revision 0. The reader
checks the magic, the sizes and the exact tail. A short file is rejected. A file
that exists but cannot be parsed is left untouched by a save and reported as
unreadable.

The part bank is 128 records of

```c
PP_RECORD(N) = 1 + 33 + N * 4 + 4
```

bytes. `N` is 994 for the current part block, so a record is 4,014 bytes. The
record is one version byte, a 33-byte name field, the parameters as little-endian
`int32_t`, and a CRC-32. `0xFF` in the version byte means an empty slot.

The multi bank is 64 records of

```c
MP_RECORD(P) = 1 + 33 + P + P * 5 * 4 + PRESET_MACRO_BYTES + 4
```

bytes. `P` is 2, so a record is 494 bytes. The record is the version, the name,
the two part-slot bytes, five arrays of two `int32_t` values (level, delay,
channel, low, high), the 54-int macro block and six 33-byte macro names, then
the CRC. The old macro-less record is 80 bytes and still loads.

`preset_crc32` is the usual reflected CRC-32 with polynomial `0xEDB88320` over
the version byte and the body. A record is self-validating. The unpack functions
return 0 on an empty slot or a CRC mismatch and leave the outputs untouched.

## The part block

The part block is 994 values. It contains the four operators, the envelopes,
the LFOs, the filter, the spectral levels, the phase-map controls, the ringmod
block and the key-follow block. The block is mirrored for `.p0` and `.p1`. Five
per-part routing parameters (channel, low, high, level and delay) live outside
the block and are stored in the multi record. The manifest has 2,053 parameters:
999 `.p0` values, the same 999 `.p1` values and 55 global values. The part block
is 994 because the five routing values are not part of it.

The block is stored in manifest order. The parameter index is the part-block
index for a `.p0` value, and the migration code uses manifest indices to find
the blocks it must remap.

## The version chain

`PRESET_VERSION` is 19. The record version byte is the first byte of every part
record. The migration chain in `PresetStore::migratePartBank` reads the version
and the record size and applies the steps in order.

| Version | Change |
| ---: | --- |
| 6 | the earlier cutoff law and the pre-operator-block layout |
| 7 | the cutoff law conversion |
| 10 | the operator envelope grew from 7 to 8 values |
| 11 | the pan envelope reorder |
| 12 | the offset envelope blocks |
| 13 | the filter and spectral envelopes |
| 14 | the absolute envelope blocks |
| 15 | the 34 LFO fade slots |
| 16 | the 116 phase-map parameters |
| 17 | the 10 ringmod parameters |
| 18 | the three key-follow parameters |
| 19 | the half-square waveform and the two inserted algorithms |

The current part record is 994 values. The earlier counts are derived by
subtracting the appended block sizes from the target, so a record without the
key-follow block is 991, without ringmod 981, without phase 865, without fades
831, and so on. The code pins the current count in `preset_smoke` so a future
parameter insertion cannot silently shift the chain.

### The value remaps

The version 19 step does not change the record length. It changes the meaning of
two stored values:

- algorithm `5→6`, `6→7`, `7→9` because two algorithms were inserted,
- waveform `11→12`, `12→13`, `13→14` because `half square` was inserted at 11.

`PresetStore::setInsertionIndices` receives the part-block index of `algorithm`
and the four `opN_waveform` values from the plugin. The remap runs on the
same-size load path in `loadPartBank`, on the grow path in `migratePartBank`,
and in the staging store used by `loadLibrary`.

The earlier migrations are index remaps:

- version 10: each operator envelope grows from seven to eight values, so each
  later block shifts by one.
- version 11: the pan block changes from `on, r1..r4, l1..l4, depth` to
  `on, r1..r3, l1..l3, rel rate, rel level, mode`.
- version 12: the pitch and feedback envelopes gain a mode and swap the fourth
  level for a release level.
- version 13: the filter and spectral envelopes become absolute and gain `on`,
  release and mode, and the spectral depth moves into the block.
- version 14: the filter block grows by two.
- version 15: 34 LFO fade values are inserted.
- version 16: 116 phase-map values are appended.
- version 17: 10 ringmod values are appended.
- version 18: 3 key-follow values are appended.

The append steps copy the prefix and fill the tail with defaults. The insert
steps use the indices supplied by the plugin. A missing index means the caller
has no layout, so the append-only path runs rather than corrupting the record.

## The store

`PresetStore` owns:

```cpp
std::vector<Part> parts_;   // 128
std::vector<Multi> multis_; // 64
std::vector<int32_t> defaults_;
```

A `Part` has `used`, `name` and `params`. A `Multi` has `used`, `name`, the two
part slots and the five per-part routing arrays, plus the macro block and names.

`savePart` truncates the name to 32 characters and stores the parameter vector.
`loadPart` returns the stored vector. `partRecordSize` and `multiRecordSize`
return the byte sizes for the current part count.

`packPartBank` writes all 128 records with `preset_part_pack` and leaves unused
slots at `0xFF`. `loadPartBank` accepts the current size or an older size. An
older size is migrated by `migratePartBank`. The current size is unpacked
directly, then the insertion remap and the cutoff conversion run.

`packMultiBank` and `loadMultiBank` do the same for the 64 multi records. The
multi loader accepts the old macro-less size and leaves `macro` empty, so the
editor applies the macro defaults.

## The change set and merge

The store keeps a change set:

```cpp
std::array<bool, 128> dirtyParts_;
std::array<bool, 64> dirtyMultis_;
std::array<Part, 128> baseParts_;
std::array<Multi, 64> baseMultis_;
```

`baseParts_` and `baseMultis_` hold the value each slot had when the store
loaded. A slot is dirty when the local value differs from the base. That is how a
merge can tell a local edit from a change that arrived on disk.

`mergeInto(disk)` walks the dirty slots. If the disk slot still equals the local
base, the local edit is copied to the disk store. If the disk slot changed, the
slot is a conflict and is left out. The local store keeps its value and clears
the dirty flag, so the player can save the edit to another slot. The result is a
list of conflicts with the bank, the slot and the local name.

`adoptLibrary(other)` replaces the banks with the merged disk banks but keeps
the current multi-patch and the two part sources. That is the routing state the
player chose, not library content.

## Saving

`TypePAlphaAudioProcessor::saveLibrary` holds a process-wide
`juce::CriticalSection` and a named `juce::InterProcessLock`. The process lock
covers two plugin instances in the same host. The interprocess lock covers the
standalone and the DAW. The lock is held across the read, the merge and the
write.

The first save writes the local bank as revision 1. A later save loads the disk
library into a temporary store with the same migration indices, calls
`presets_.mergeInto(disk)`, sets the revision to `max(local, disk) + 1`, writes
the merged bytes and calls `markSaved`. A conflict leaves the disk version in
place and reports the first conflict to the editor.

The library path is

```text
<user application data>/P1Onken/Type P.alpha/library.p1lib
```

`setLibraryFileForTesting` overrides it for the tests.

## Project state

`getStateInformation` embeds the packed part and multi banks as base64 in the
project state. The banks are a portable snapshot, not the shared library. A
project load restores the APVTS, the names, the UI page and the part sources, but
it does not write the embedded banks to disk. That prevents an old project from
replacing a newer shared library.

`setStateInformation` migrates the older state versions before restoring the
APVTS:

- version 5 converts the cutoff law,
- version 15 remaps the algorithm and waveform values,
- the `uiPageLayout` property remaps the page index for layouts 2, 3, 4 and 5.

The state version is 15. The `kStateVersion` and `kInsertionStateVersion`
constants are in `PluginProcessor.cpp`.

## The tests

`preset_smoke` covers:

- the current 994-value part record,
- the 32-character name,
- the `P1LIB3` library and the v2 file without a revision,
- the version 6, 10, 12, 13, 14, 16 and 17 records,
- the version 18 to 19 value remap,
- the pre-macro multi record,
- the merge conflicts and the revision,
- the truncated and unreadable files.

`plugin_smoke` covers the project state round trip, the embedded banks not
overwriting the shared library, the UI page migrations and the old cutoff
migration. The full list is in [`reference/tests.md`](../reference/tests.md).
