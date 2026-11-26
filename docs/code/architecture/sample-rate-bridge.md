# Sample-rate bridge

The engine runs at 48 kHz. The host may run at 44.1, 48, 88.2, 96, 176.4 or
192 kHz. The processor bypasses the resampler at 48 kHz and uses a 128-tap
Kaiser-windowed sinc polyphase resampler at every other rate. The bridge is in
`plugin/engine/RateBridge.hpp` and `RateBridge.cpp`.

## Why the core stays at 48 kHz

Every table, coefficient and rate in the engine is built for 48 kHz. The
envelope rate table, the filter coefficient table, the phase-map clock and the
wavetable mip thresholds all assume a 48 kHz sample rate. Keeping the core fixed
means a patch sounds the same at every host rate and the tiny rate-dependent
rounding does not move between hosts. The rate change is an output-only problem.

## The kernel

`RateBridge::prepare` builds a 128-tap kernel at 1,024 phase positions. The
kernel is a windowed sinc:

```text
x = cutoff * (distance - centre)
sinc = sin(pi * x) / (pi * x)
window = I0(beta * sqrt(1 - t^2)) / I0(beta)
value = cutoff * sinc * window
```

with `beta = 8.6`, `centre = taps / 2` and `cutoff = min(1, hostRate / 48000)`.
The cutoff is 1 at 48 kHz and below 1 when the host rate is lower, which removes
the content above the host Nyquist before it can fold. The kernel is normalised
so each phase sums to unity. That removes the DC gain error from the window and
the fractional phase.

`besselI0` is a 32-term series with a relative stop at `1e-12`. It runs only in
`prepare`, not in the audio path.

The kernel is stored as `float[kTaps * kPhases]`, 128 by 1,024, 512 KiB.

## The ring

`prepare` sizes the ring from the maximum host block:

```cpp
const int needed = ceil(maxBlock * ratio) + 128;
ringSize_ = max(64, needed);
ring_.assign(ringSize_ * 2, 0);
```

Each ring slot holds a stereo `int32_t` pair. The `+128` is the kernel history.
The ring size is not rounded to a power of two, so the code uses `% ringSize_`
rather than a mask. That is fine because the ring is large and the modulo is not
the hot inner loop's only work.

`coreGenerated_` counts the core frames written into the ring. `outputIndex_`
counts the host frames pulled out. The ratio is `48000 / hostRate`. At 44.1 kHz
the ratio is about 1.0884, so the bridge renders slightly more core frames than
host frames. At 96 kHz the ratio is 0.5, so one host frame consumes half a core
frame and the bridge can render a core chunk every other host block.

## Pushing core frames

`pushCore` writes interleaved `int32_t` frames:

```cpp
for (int i = 0; i < frames; ++i) {
    const int index = coreGenerated_ % ringSize_;
    ring_[index * 2] = interleaved[i * 2];
    ring_[index * 2 + 1] = interleaved[i * 2 + 1];
    ++coreGenerated_;
}
```

The ring keeps the last `ringSize_` frames. The pull reads a window that starts
`kTaps - 1` samples before the current core position and ends at the current
position. The history is always available because the ring is at least 128
frames longer than a block.

## Pulling host frames

For each host frame the bridge computes the core position:

```cpp
position = outputIndex_ * ratio;
n = floor(position);
fraction = position - n;
phase = round(fraction * 1024);
```

It selects the 128-tap kernel for that phase and accumulates:

```cpp
for (tap = 0; tap < 128; ++tap) {
    sample = n - 127 + tap;
    if (sample < 0 || sample >= coreGenerated_) continue;
    index = sample % ringSize_;
    l += ring_[index * 2] * weights[tap];
    r += ring_[index * 2 + 1] * weights[tap];
}
left[i] = l * (1 / 32768);
right[i] = r * (1 / 32768);
```

The `sample` bounds check skips the zero history before the first core frame.
The ring is already zeroed, so the result is the same as reading zero. The
check also prevents a read before the ring exists.

The kernel phase is rounded to 1,024 positions. That is a phase quantisation of
1/1,024 of a core sample, which is far below the audio noise floor.

## The process loop

`RateBridge::process` does not know how many core frames it needs in advance. It
computes the last core frame required by the host block:

```cpp
last = floor((outputIndex_ + frames - 1) * ratio);
while (coreGenerated_ <= last)
    renderChunk(engine, 32, onCore, context);
pull(left, right, frames);
```

`renderChunk` renders at most 32 core frames, pushes them, and calls the
`onCore` callback. The callback is the processor's `coreChunk`, which flushes
deferred note-offs, advances the core frame counter, resets the chunk note set,
feeds scheduled MIDI for the next control block, and publishes the tap and
snapshot data. The callback runs **after** the chunk is in the ring and **before**
the next chunk overwrites the tap state. That ordering is the reason the scope
and the phase-map display see every core chunk even when the host block is
smaller than a control block.

`hasCore(hostFrames)` tells the processor whether a `process` call will render at
least one chunk. At a high host rate a very small host block can need no new
core frame, so the processor uses that check to decide whether to feed the first
scheduled events before the bridge runs.

## Latency

`latencySamples` is

```cpp
round((kTaps - 1) * 0.5 * hostRate / 48000);
```

The 128-tap kernel has a group delay of 63.5 core samples. The formula would
give 64 host samples at 48 kHz, about 58 at 44.1 kHz, 127 at 96 kHz and 254 at
192 kHz. The processor reports the value through `setLatencySamples`. At 48 kHz
it reports 0 because the bridge is bypassed and the core output is bit-exact.

## The 48 kHz bypass

`prepareToPlay` checks

```cpp
if (std::abs(sampleRate - kCoreRate) > 0.5) {
    rateBridge_.prepare(sampleRate, samplesPerBlock);
    setLatencySamples(rateBridge_.latencySamples());
} else {
    setLatencySamples(0);
}
```

At 48 kHz `processBlock` never calls `rateBridge_.process`. It renders 32-sample
core blocks directly into `directBuffer_`, converts to float and writes the host
buffer. The 48 kHz path is the reference path, and the rate-bridge test checks
that the bridge's 48 kHz output is the same signal with the expected latency and
no extra colour.

## Mono hosts

`RateBridge::pull` accepts a null right channel and folds the pair itself:

```cpp
const float lv = l * scale;
const float rv = r * scale;
if (right != nullptr) {
    left[i] = lv;
    right[i] = rv;
} else {
    left[i] = (lv + rv) * 0.5f;
}
```

A hard-panned part is 6 dB down in mono, and a centred part keeps its level. The
fold happens per frame during the pull, so a mono host needs no scratch buffer
and no allocation on the audio thread. The direct 48 kHz path applies the same
fold while it converts the core buffer.

## Tests and evidence

`plugin/tests/rate_bridge_test.cpp` covers the ratio, the latency, the 48 kHz
bypass, the 44.1, 96 and 192 kHz paths and the image rejection. The 48 kHz
contract records the measured image at -95.9 dB at 44.1 kHz, -108.0 dB at 96 kHz
and -104.3 dB at 192 kHz. The mono fold and the high-rate placeholder path are
covered by `plugin_smoke`.
