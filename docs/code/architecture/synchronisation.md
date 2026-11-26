# Synchronisation

The plugin has a message thread, an audio thread and the host's automation
thread. The UI runs on the message thread and the editor timer runs at 30 Hz.
The audio thread runs `processBlock`. The host can call `getStateInformation`
and `setStateInformation` on a non-audio thread. The library save runs on the
message thread under a process lock and an interprocess lock.

This chapter follows every shared object and the rule that makes it safe.

## The engine state

The engine state is owned by `p1::Engine` inside the processor. Only the audio
thread calls the generated render and note functions. The editor never touches
`type_p_alpha_state`. It reads parameter values through the APVTS and reads
`liveMorph_`, `phaseMapSnapshots_`, the scope rings and the tap rings through the
processor's accessors.

The engine is not locked. A lock around a 492 KiB render would block the audio
thread and make the UI the priority. The design instead gives the UI its own
copy of the data it needs and uses atomics for the small scalar values.

## Parameter atomics

`paramValues_` is an array of `std::atomic<float>*`, one per manifest
parameter, obtained from `apvts_.getRawParameterValue`. The APVTS owns the
storage and guarantees that a read and a write of the float are atomic on the
platform. The audio thread reads the floats in `syncParams` and `syncMacros`.
The UI writes them through the APVTS parameter object or through the slider and
button attachments.

`lastParams_` is audio-thread-only. It is not shared with the UI. The
`paramsDirty_` atomic is set when the processor replaces its state, so the next
control block resends every parameter even when the float values compare equal.

The six macro values, the six source selects, the six source ranges, the
eighteen targets and the eighteen destination ranges are all APVTS atomics. The
processor caches their pointers in the constructor. The audio thread reads them
in `syncMacros`.

`modWheel_` and `noteVelocity_` are `std::atomic<int>`. The MIDI thread writes
them in `processBlock`, which is the audio thread, so the same thread reads them
in `syncMacros`. They are atomics because the editor can read them through
`macroEffectiveValue` on the message thread.

`macroCc_` is six `std::atomic<int>`. The audio thread writes the six macro CCs
(72 to 77) and the editor reads them for the macro feedback display.

## The scope ring

`ScopeRing` is a single-producer/single-consumer ring built on
`juce::AbstractFifo`:

```cpp
static constexpr int kCapacity = 2048;
juce::AbstractFifo fifo_{kCapacity};
std::array<float, kCapacity> buffer_{};
std::array<float, kCapacity> scratch_{};
```

The audio thread calls `push`. It prepares a write region, copies the samples,
and calls `finishedWrite`. The editor calls `read`. It reads all ready samples
into `scratch_`, calls `finishedRead`, then copies the last `count` samples to
the destination. The FIFO has one writer and one reader, so the atomic indices
are enough. The ring never blocks.

The output scope and the ten tap scopes are all `ScopeRing` instances. The
output scope receives the final host-rate buffer. The tap scopes receive the
core-rate operator and carrier taps.

## The live morph

`liveMorph_` is eight `std::atomic<int>`, one for each operator and part. The
audio thread writes the last morph the spectral oscillator read, 0 to 99, or -1
when the operator has not read the bank. The editor reads it through
`specMorph` and divides by 99. A plain relaxed store is enough because a stale
value only delays one display frame.

## The phase-map snapshot

The phase map is a large pool of geometry, tables and locks. Copying it under a
lock would stall the audio thread. The processor uses a three-buffer handoff:

```cpp
std::array<std::array<p1::PhaseMapSnapshot, 8>, 3> phaseMapSnapshots_{};
std::atomic<uint32_t> phaseMapMiddle_{1};
uint32_t phaseMapBack_ = 0;
mutable uint32_t phaseMapFront_ = 2;
```

The low two bits of `phaseMapMiddle_` are the buffer index and bit 2 is the
new-data flag.

The writer is `publishPhaseMaps`, called from the audio thread after a core
chunk. It writes the snapshots into `phaseMapBack_`, then exchanges
`phaseMapBack_ | kNewData` into the middle slot:

```cpp
phaseMapBack_ =
    phaseMapMiddle_.exchange(back | kNewData, std::memory_order_acq_rel) &
    kIndexMask;
```

The exchange returns the previous middle. The writer's next back buffer is that
previous index. The reader is `phaseMap`, called from the editor timer. If the
new-data bit is set, it exchanges its front index into the middle and takes the
returned index as the new front:

```cpp
if ((phaseMapMiddle_.load(acquire) & kNewData) != 0)
    phaseMapFront_ =
        phaseMapMiddle_.exchange(phaseMapFront_, acq_rel) & kIndexMask;
out = phaseMapSnapshots_[phaseMapFront_][slot];
```

The three buffers guarantee that the writer's back buffer is never the buffer
the reader is copying. The reader can read a snapshot for a frame and the writer
can fill another buffer at the same time. The `acq_rel` exchange publishes the
snapshot writes before the index becomes visible.

The snapshot is a copy, not a pointer into the pool. It contains the breakpoint,
the segment count, the active count, the y boundaries, the slopes, the flip
flags, the order, the locks and the newest voice's phase and mapped phase. The
editor draws from that copy and never from the engine pool.

## The tap scopes

The tap scopes are pushed from `pushTapScopes`. The function reads the ten tap
buffers from the engine, converts `int32_t` to float, and pushes 32 samples into
each `ScopeRing`. The editor reads the ring at its own rate. A slow editor frame
loses old samples, which is the intended behaviour for a scope.

## The library lock

`TypePAlphaAudioProcessor::saveLibrary` uses two locks:

```cpp
static juce::CriticalSection processSaveLock;
const juce::ScopedLock processLock(processSaveLock);
juce::InterProcessLock libraryLock("P1Onken-TypePAlpha-Library");
if (!libraryLock.enter(-1)) { result.busy = true; return result; }
```

The process lock covers two plugin instances in the same host. The interprocess
lock covers the standalone and the DAW. The lock is held across the read, the
merge and the write, so another process cannot read the same revision and
replace the first writer's slot. The wait is unbounded because the transaction
is short and the OS releases the lock if a holder dies. A finite timeout would
not give the exclusion the merge needs.

The read-merge-write is:

1. If the file does not exist, write the local bank as revision 1.
2. Load the disk library into a temporary `PresetStore` with the same migration
   indices.
3. `presets_.mergeInto(disk)` copies only the slots this instance changed and
   reports conflicts.
4. The revision is the maximum of the local and disk revisions plus one.
5. `disk.packLibrary()` writes the merged bank and the file is replaced.
6. `presets_.markSaved()` clears the local change set.

A conflict leaves the disk version in place and reports the first conflict to
the editor. The local edit is still in memory and the local change set is
cleared, so the player can save it to another slot.

## Project state

`getStateInformation` copies the APVTS state, adds the state version, the patch
and part names, the macro names, the UI page and active part, the current
multi-patch and the two part sources. It also embeds the packed part and multi
banks as base64. The banks are a portable snapshot of the library, not the
shared library itself.

`setStateInformation` restores the APVTS and migrates the older state versions.
It does not write the embedded banks into the shared store. A project made
against an older library would otherwise replace every slot on disk at the next
save. The embedded snapshot is restored only by an explicit action.

The state migration chain is:

- version 5: the cutoff law conversion.
- version 15: the half-square waveform and the two inserted algorithms remap
  `5→6`, `6→7`, `7→9` and `11→12`, `12→13`, `13→14`.
- the UI page layout migrations 2, 3, 4 and 5.

The `uiPageLayout` property is stored with every project so the editor can map a
stored page number to the current page order. The layout 5 order is patch
settings, macros, part params, op envs, op lfos, op phasemap, op spectra a, op
spectra b and op spectra mod.

## Automation and parameter feedback

The host automation writes the same APVTS atomics as the UI. `syncParams` sees
the change on the next control block. The `onUserEdit` callback on a knob fires
only for a user drag, not for host automation, so a host write does not open a
dialog or mark a library slot dirty.

`macroFeedback` walks the macro member index and the 18 target slots to compute
the effective value of one parameter. The editor calls it in
`refreshMacroFeedback` for every visible control that implements
`MacroFeedbackTarget`. The stored value stays in the parameter and the effective
value is drawn beside it. The UI never writes the effective value back.

## Denormals and real-time safety

`processBlock` begins with `juce::ScopedNoDenormals`, so a decaying tail does not
hit denormal arithmetic. The audio path uses no locks, no file access, no
allocation in the steady state and no JUCE component calls. A mono host is folded
inside the resampler pull, so the processor keeps no scratch buffer and never
resizes one on the audio thread.

The editor timer runs at 30 Hz. It reads the scope rings and the phase-map
snapshot, updates the visible page and repaints. A slow frame skips visual data
rather than blocking the audio thread.
