# Feature contract: the rate bridge

## Identity

- **Feature:** Milestone 5, the output-only polyphase rate bridge.
- **Brief section:** `plugin/README.md` section 7 and `docs/roadmap.md` 13.22.
- **Owner / date:** P1Onken, 2026-11-04.
- **Disposition:** accepted.

## Player outcome

- **New player action:** run the 44.1 kHz engine in a host at 48, 96 or 192 kHz
  without dropouts or a changed core sound.
- **Sound and behaviour:** at 44.1 kHz the bridge is bypassed and the output
  is bit-exact with the firmware render. At other rates a 32-tap
  Kaiser-windowed sinc resampler converts the output and reports its group delay
  as host latency.
- **Failure mode:** a missing or silent core block produces silence, not noise,
  and the core rate never changes.

## Dependencies and interactions

- **Other features:** the generated engine, the processor output path, the host
  latency report and the editor timer. The editor stays on the 44.1 kHz timeline.
- **Platform/API/HAL changes:** `RateBridge` gains `pushCore`, `pull` and
  `latencySamples`. The processor calls `setLatencySamples` in
  `prepareToPlay`.
- **Persistence/version impact:** none.
- **MIDI/clock impact:** none.

## Parameter and event semantics

- **Parameters, ranges, defaults, curves, units:** unchanged.
- **Note/event ordering and ownership:** unchanged. The engine still receives
  MIDI at the 32-sample control block.
- **Voice allocation and stealing:** unchanged.
- **Sustain/panic behaviour:** unchanged.

## Bounds and resources

- **Maximum voices/events/effects:** unchanged, 20 voices.
- **Per-sample and per-block operations:** 32 multiply-adds per output sample
  per channel. The kernel is 32 taps and 1024 phases, about 128 KiB of floats, and
  the core history ring is sized for the host block and the filter history.
- **RAM, flash, stack, pool:** one bridge per plugin instance. The kernel and
  ring are allocated in `prepareToPlay`, never on the audio thread.
- **Overflow/overload policy:** a host block larger than the prepared maximum
  would exceed the ring, so the host contract is the prepared block size.
- **Division, shift, and range analysis:** the index ring uses integer modulo.
  The phase is a double accumulator.

## Files and interfaces

- **Files:** `plugin/engine/RateBridge.hpp`, `plugin/engine/RateBridge.cpp`,
  `plugin/juce/PluginProcessor.cpp`, `plugin/tests/rate_bridge_test.cpp`,
  `plugin/tests/plugin_smoke.cpp`, `plugin/CMakeLists.txt`.
- **Public contract/header:** `RateBridge::prepare`, `reset`, `process`,
  `pushCore`, `pull`, `latencySamples` and `ratio`.
- **State owner and publication rule:** the processor owns one bridge, which is
  used only by the audio thread after `prepareToPlay`.
- **Forbidden dependencies:** no allocation, locks or file access on the audio
  thread, and no input resampling or core rate change.

## Verification

- **Host vector(s):** `rate_bridge_test` drives a synthetic core sine through
  `pushCore` and `pull` at 48, 96 and 192 kHz, and it measures the 1 kHz, 10 kHz
  and 20 kHz gains, the 20 kHz image rejection, THD, the reported latency and
  blocks of 32, 64, 128, 256, 512 and 1024 frames.
- **Browser vector:** unchanged, the web parity remains green.
- **Target package check:** unchanged, `deep-check --all` still builds the
  target package.
- **Physical timing/device check:** blocked, no hardware.
- **Tolerance/metric:** 44.1 kHz bypass is bit-exact, whilst at other rates the
  passband is flat within 0.5 dB at 1 kHz and 1.5 dB at 10 kHz, and the 20 kHz
  image is below the fundamental.
- **Evidence artefact:** the `rate_bridge_test` output and the plugin CTest
  output.

## Result (2026-11-04)

- The kernel is 32 taps, Kaiser beta 8.6, 1024 phases, with cutoff at the lower
  of the core and host Nyquist, and the group delay is 15.5 core samples.
- The reported host latency is 17 samples at 48 kHz, 34 at 96 kHz and 67 at
  192 kHz, whilst at 44.1 kHz the bypass reports zero.
- Measured at 48, 96 and 192 kHz:
  - 1 kHz gain within 0.01 dB
  - 10 kHz gain within 0.01 dB
  - 20 kHz gain -0.45 dB
  - 20 kHz image rejection -110 dB at 48 kHz and -25.4 dB at 96 and 192 kHz
  - THD about -25 dB for the 20 kHz tone
- Every block size from 32 to 1024 frames renders finite, non-silent audio.
- `plugin_smoke` keeps the 44.1 kHz render bit-exact, reports zero latency there
  and 17 samples at 48 kHz.
- The near-Nyquist image rejection at 96 and 192 kHz is the expected limit of a
  32-tap kernel, so a longer kernel is the next quality step if a stricter budget
  is set.
- Physical device acceptance remains blocked.

## Risks and fallback

- **Assumptions/open research:** the host obeys the prepared maximum block size,
  and the reported latency rounds the 15.5 core sample group delay to host
  samples.
- **Safe experiment and stop criterion:** bypass at 44.1 first and compare the
  host WAV byte for byte, and stop if any sample differs.
- **Smaller alternative:** keep the linear bridge behind a build flag. The
  current build uses the 32-tap bridge.
- **Rollback:** restore the linear `RateBridge` and remove the latency call. The
  core and the firmware are unchanged.
