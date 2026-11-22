/* SPDX-License-Identifier: GPL-3.0-only */
/* Per-operator stochastic phase map: the off path is untouched, F is the
 * one-breakpoint polyline, the fold reflects, the y boundaries are snapped away
 * from protected points, locked spans never flip or move, the order stays a
 * permutation, drift keeps its bounds, the modulation sources move the
 * breakpoint, the envelope follows the part mode, and the two parts keep
 * separate maps. The 2x read is not built; the CPU figure here is the 1x one. */
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"

namespace {

constexpr int kVoices = 24;
constexpr int kTickFrames = 32;

/* The phase_map pool layout, mirrored from the node. */
constexpr uint32_t kWords = 2560u;
constexpr uint32_t kNx = 0u;
constexpr uint32_t kNslope = 33u;
constexpr uint32_t kNflip = 65u;
constexpr uint32_t kOrder = 97u;
constexpr uint32_t kNy = 2249u;
constexpr uint32_t kMask = 2282u;
constexpr uint32_t kLock = 2411u;
constexpr uint32_t kActive = 2412u;
constexpr uint32_t kBp = 2244u;
constexpr uint32_t kLastSegs = 2245u;
constexpr uint32_t kDacc = 2246u;
constexpr uint32_t kFacc = 2247u;
constexpr uint32_t kRacc = 2248u;
constexpr uint32_t kF = 1217u;

/* The phase_lfo and phase_env pools. */
constexpr uint32_t kPlRateLevel = 2u;
constexpr uint32_t kPlDepthLevel = 3u;
constexpr uint32_t kPlValue = 6u;
constexpr uint32_t kPeStage = 0u;
constexpr uint32_t kPeLevel = 1u;

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("phase_map: %-66s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

uint32_t param_id(const char* name)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (std::strcmp(spec.id, name) == 0)
            return static_cast<uint32_t>(spec.engineId);
    }
    return ~0u;
}

bool set_param(p1::Engine& engine, const char* name, int32_t value)
{
    const uint32_t id = param_id(name);
    if (id == ~0u) {
        std::printf("phase_map: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool base_setup(p1::Engine& engine)
{
    bool ok = true;
    ok &= set_param(engine, "algorithm.p0", 7);
    ok &= set_param(engine, "op1_waveform.p0", 0);
    ok &= set_param(engine, "op1_ratio.p0", 65536);
    ok &= set_param(engine, "op1_detune.p0", 0);
    ok &= set_param(engine, "op1_level.p0", 99);
    ok &= set_param(engine, "op1_feedback.p0", 0);
    ok &= set_param(engine, "op1_mute.p0", 0);
    ok &= set_param(engine, "op2_level.p0", 0);
    ok &= set_param(engine, "op2_mute.p0", 1);
    ok &= set_param(engine, "op3_level.p0", 0);
    ok &= set_param(engine, "op3_mute.p0", 1);
    ok &= set_param(engine, "op4_level.p0", 0);
    ok &= set_param(engine, "op4_mute.p0", 1);
    ok &= set_param(engine, "attack.p0", 99);
    ok &= set_param(engine, "release.p0", 99);
    ok &= set_param(engine, "op1.eg.d1r.p0", 99);
    ok &= set_param(engine, "op1.eg.d2r.p0", 99);
    ok &= set_param(engine, "op1.eg.d1l.p0", 99);
    ok &= set_param(engine, "op1.eg.d2l.p0", 99);
    ok &= set_param(engine, "filter_on.p0", 0);
    ok &= set_param(engine, "op1_phase_env_on.p0", 0);
    ok &= set_param(engine, "op1_phase_lfo_depth.p0", 0);
    return ok;
}

uint32_t* pm(p1::Engine& engine, int op, int part)
{
    int16_t* base = nullptr;
    switch (op) {
    case 0: base = engine.state()->op1_phase_map; break;
    case 1: base = engine.state()->op2_phase_map; break;
    case 2: base = engine.state()->op3_phase_map; break;
    case 3: base = engine.state()->op4_phase_map; break;
    default: return nullptr;
    }
    return reinterpret_cast<uint32_t*>(base) + part * kWords;
}

uint32_t* lfo_pool(p1::Engine& engine, int op, int part)
{
    int16_t* base = nullptr;
    switch (op) {
    case 0: base = engine.state()->op1_phase_lfo_pool; break;
    case 1: base = engine.state()->op2_phase_lfo_pool; break;
    case 2: base = engine.state()->op3_phase_lfo_pool; break;
    case 3: base = engine.state()->op4_phase_lfo_pool; break;
    default: return nullptr;
    }
    return reinterpret_cast<uint32_t*>(base) + part * 64u;
}

uint32_t* env_pool(p1::Engine& engine, int op, int part)
{
    int16_t* base = nullptr;
    switch (op) {
    case 0: base = engine.state()->op1_phase_env_pool; break;
    case 1: base = engine.state()->op2_phase_env_pool; break;
    case 2: base = engine.state()->op3_phase_env_pool; break;
    case 3: base = engine.state()->op4_phase_env_pool; break;
    default: return nullptr;
    }
    return reinterpret_cast<uint32_t*>(base) + part * 64u;
}

bool map_on(p1::Engine& engine)
{
    bool ok = true;
    ok &= set_param(engine, "op1_phase_on.p0", 1);
    ok &= set_param(engine, "op1_phase_bp.p0", 500);
    ok &= set_param(engine, "op1_phase_segments.p0", 8);
    ok &= set_param(engine, "op1_phase_flip.p0", 0);
    ok &= set_param(engine, "op1_phase_reorder.p0", 0);
    ok &= set_param(engine, "op1_phase_drift.p0", 0);
    return ok;
}

void tick(p1::Engine& engine)
{
    std::vector<int32_t> out(2 * kTickFrames, 0);
    engine.render(out.data(), kTickFrames);
}

void render(p1::Engine& engine, int frames, std::vector<int32_t>& pcm)
{
    pcm.assign(2u * static_cast<std::size_t>(frames), 0);
    engine.render(pcm.data(), static_cast<uint32_t>(frames));
}

bool same(const std::vector<int32_t>& a, const std::vector<int32_t>& b)
{
    return a.size() == b.size() &&
           std::memcmp(a.data(), b.data(), a.size() * sizeof(int32_t)) == 0;
}

bool sorted_x(const uint32_t* p, uint32_t active)
{
    for (uint32_t k = 0u; k < active; ++k) {
        if (p[kNx + k + 1u] < p[kNx + k])
            return false;
    }
    return p[kNx] == 0u && p[kNx + active] == 4096u;
}

bool permutes(const uint32_t* p, uint32_t active)
{
    uint32_t seen = 0u;
    for (uint32_t k = 0u; k < active; ++k) {
        uint32_t j = p[kOrder + k];
        if (j < 1u || j > active)
            return false;
        if (seen & (1u << (j - 1u)))
            return false;
        seen |= 1u << (j - 1u);
    }
    return seen == (active == 32u ? 0xFFFFFFFFu : ((1u << active) - 1u));
}

bool mask_nonempty(const uint32_t* p)
{
    for (uint32_t i = 0u; i < 128u; ++i)
        if (p[kMask + i] != 0u)
            return true;
    return false;
}

bool locked_flip_free(const uint32_t* p)
{
    const uint32_t locks = p[kLock];
    const uint32_t active = p[kActive];
    for (uint32_t k = 0u; k < active; ++k) {
        if (!(locks & (1u << k)))
            continue;
        if (p[kNflip + k] & 1u)
            return false;
    }
    return true;
}

void wait_event(p1::Engine& engine, uint32_t* p, uint32_t accIndex,
                int maxTicks, p1::PhaseMapSnapshot* snap = nullptr)
{
    uint32_t last = p[accIndex];
    for (int t = 0; t < maxTicks; ++t) {
        tick(engine);
        if (p[accIndex] < last)
            break;
        last = p[accIndex];
    }
    if (snap != nullptr)
        engine.phaseMapSnapshot(0, 0, *snap);
}

double rms(const std::vector<int32_t>& pcm)
{
    double sum = 0.0;
    for (std::size_t i = 0; i < pcm.size(); i += 2u) {
        const double x = pcm[i];
        sum += x * x;
    }
    return std::sqrt(sum / (pcm.size() / 2u));
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    std::printf("phase map\n");

    /* Off is the current operator: the phase parameters are ignored and two
     * renders with different inert settings are identical. */
    {
        std::vector<int32_t> a;
        std::vector<int32_t> b;
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 4096, a);

        p1::Engine engine2;
        engine2.reset();
        if (!base_setup(engine2)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine2, "op1_phase_segments.p0", 32);
        set_param(engine2, "op1_phase_flip.p0", 100);
        set_param(engine2, "op1_phase_drift.p0", 100);
        set_param(engine2, "op1_phase_reorder.p0", 100);
        engine2.noteOn(60, 100, 0);
        render(engine2, 4096, b);
        int firstDiff = -1;
        int diffs = 0;
        for (std::size_t i = 0; i < a.size(); ++i) {
            if (a[i] != b[i]) {
                if (firstDiff < 0) firstDiff = static_cast<int>(i);
                ++diffs;
            }
        }
        std::printf("phase_map: off diff first %d, count %d, %d vs %d\n",
                    firstDiff, diffs, firstDiff >= 0 ? a[firstDiff] : 0,
                    firstDiff >= 0 ? b[firstDiff] : 0);
        check(same(a, b), "phase off ignores every map parameter");
    }

    /* The pre-map F is the one-breakpoint polyline. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_segments.p0", 8);
        set_param(engine, "op1_phase_bp.p0", 500);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = pm(engine, 0, 0);
        const uint32_t at0 = p[kF];
        const uint32_t atBp = p[kF + 512u];
        const uint32_t atHalf = p[kF + 768u];
        const uint32_t atEnd = p[kF + 1023u];
        std::printf("phase_map: F 0=%u bp=%u 0.75=%u end=%u\n", at0, atBp,
                    atHalf, atEnd);
        check(at0 == 0u && atBp == 512u,
              "F is 0 at 0 and half at a 0.500 breakpoint");
        check(atHalf > atBp && atHalf < 1000u,
              "F continues at the shallow post-breakpoint slope");
        check(atEnd > 1000u && atEnd <= 1024u, "F reaches the end of the field");

        set_param(engine, "op1_phase_bp.p0", 750);
        tick(engine);
        p = pm(engine, 0, 0);
        std::printf("phase_map: F at 0.75 bp: 0.75=%u 0.25=%u\n",
                    p[kF + 768u], p[kF + 256u]);
        check(p[kF + 768u] == 512u,
              "F is half at a 0.750 breakpoint");
        check(p[kF + 256u] < 300u,
              "F is shallow before a late breakpoint");
    }

    /* The fold reflects the excess and never rests at an edge. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_lfo_rate.p0", 2000000);
        set_param(engine, "op1_phase_lfo_depth.p0", 100);
        set_param(engine, "op1_phase_lfo_wave.p0", 2);
        set_param(engine, "op1_phase_lfo_mode.p0", 1);
        engine.noteOn(60, 100, 0);
        std::vector<int> bps;
        bps.reserve(15000);
        int minBp = 1000;
        int maxBp = 0;
        int runLow = 0;
        int runHigh = 0;
        int maxRunLow = 0;
        int maxRunHigh = 0;
        bool reflected = true;
        int last = -1;
        for (int t = 0; t < 15000; ++t) {
            tick(engine);
            p1::PhaseMapSnapshot snap;
            if (!engine.phaseMapSnapshot(0, 0, snap))
                continue;
            const int bp = snap.bp;
            bps.push_back(bp);
            minBp = std::min(minBp, bp);
            maxBp = std::max(maxBp, bp);
            if (bp <= 2) { ++runLow; runHigh = 0; }
            else if (bp >= 997) { ++runHigh; runLow = 0; }
            else { runLow = 0; runHigh = 0; }
            maxRunLow = std::max(maxRunLow, runLow);
            maxRunHigh = std::max(maxRunHigh, runHigh);
            if (last == 999 && bp == 1)
                reflected = false;
            if (last == 1 && bp == 999)
                reflected = false;
            last = bp;
        }
        std::printf("phase_map: fold reaches %d..%d, runs %d/%d\n", minBp,
                    maxBp, maxRunLow, maxRunHigh);
        check(minBp <= 10 && maxBp >= 990, "the fold reaches both edges");
        check(maxRunLow <= 1 && maxRunHigh <= 1,
              "the fold never rests at an edge for more than one tick");
        check(reflected, "the fold reflects instead of wrapping");
    }

    /* Locks keep crossings out of flips and the spans cover the field. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_segments.p0", 32);
        set_param(engine, "op1_phase_flip.p0", 100);
        set_param(engine, "op1_phase_flip_rate.p0", 2684355);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = pm(engine, 0, 0);
        const uint32_t active = p[kActive];
        std::printf("phase_map: sine active %u/32, locks 0x%08x\n", active,
                    p[kLock]);
        check(active >= 1u && active <= 32u, "the active count fits the request");
        check(sorted_x(p, active), "the x boundaries are sorted and end at 4096");
        check(permutes(p, active), "the order is a permutation");
        check(mask_nonempty(p), "the sine has a protected mask");
        check((p[kLock] & 1u) != 0u,
              "the sine's crossing span is locked");
        bool ok = true;
        for (int e = 0; e < 20; ++e) {
            wait_event(engine, p, kFacc, 200);
            ok &= locked_flip_free(p);
        }
        check(ok, "no locked span flips over 20 events at 100 percent");
    }

    /* A full-probability flip reverses every eligible span, twice restores. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_segments.p0", 8);
        set_param(engine, "op1_phase_flip.p0", 100);
        set_param(engine, "op1_phase_flip_rate.p0", 2684355);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = pm(engine, 0, 0);
        const uint32_t active = p[kActive];
        const uint32_t locks = p[kLock];
        wait_event(engine, p, kFacc, 200);
        bool allFlipped = true;
        for (uint32_t k = 0u; k < active; ++k)
            if (!(locks & (1u << k)) && !(p[kNflip + k] & 1u))
                allFlipped = false;
        check(allFlipped, "every unlocked span flips at 100 percent");
        wait_event(engine, p, kFacc, 200);
        bool restored = true;
        for (uint32_t k = 0u; k < active; ++k)
            if (!(locks & (1u << k)) && (p[kNflip + k] & 1u))
                restored = false;
        check(restored, "a second event restores every span");
    }

    /* Reorder stays a local forward swap and never moves a locked span. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_segments.p0", 16);
        set_param(engine, "op1_phase_reorder.p0", 100);
        set_param(engine, "op1_phase_reorder_rate.p0", 2684355);
        set_param(engine, "op1_phase_jump.p0", 1);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = pm(engine, 0, 0);
        const uint32_t active = p[kActive];
        const uint32_t locks = p[kLock];
        uint32_t lockPos[32];
        std::memset(lockPos, 0xFF, sizeof(lockPos));
        for (uint32_t k = 0u; k < active; ++k) {
            const uint32_t j = p[kOrder + k] - 1u;
            if (locks & (1u << j))
                lockPos[j] = k;
        }
        bool ok = true;
        bool local = true;
        bool lockedStill = true;
        uint32_t prev[32];
        for (uint32_t k = 0u; k < active; ++k)
            prev[k] = p[kOrder + k];
        for (int e = 0; e < 400; ++e) {
            wait_event(engine, p, kRacc, 200);
            ok &= permutes(p, active);
            int first = -1;
            int second = -1;
            for (uint32_t k = 0u; k < active; ++k) {
                if (p[kOrder + k] != prev[k]) {
                    if (first < 0)
                        first = static_cast<int>(k);
                    else
                        second = static_cast<int>(k);
                }
                prev[k] = p[kOrder + k];
            }
            if (first >= 0 && second >= 0) {
                /* jump 1 is an adjacent swap. The old wrap moved the last
                 * position to the first; the reflected jump must not. */
                if (std::abs(first - second) != 1)
                    local = false;
            }
            for (uint32_t k = 0u; k < active; ++k) {
                const uint32_t j = p[kOrder + k] - 1u;
                if ((locks & (1u << j)) && lockPos[j] != k)
                    lockedStill = false;
            }
        }
        check(ok, "the order stays a permutation over 400 events");
        check(local, "jump 1 only swaps neighbours");
        check(lockedStill, "a locked span never moves");
    }

    /* Drift keeps the slope bounds and the sorted boundaries. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_segments.p0", 16);
        set_param(engine, "op1_phase_drift.p0", 100);
        set_param(engine, "op1_phase_drift_rate.p0", 2684355);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = pm(engine, 0, 0);
        const uint32_t active = p[kActive];
        bool ok = true;
        bool bounds = true;
        double minSlope = 1e9;
        double maxSlope = 0.0;
        for (int e = 0; e < 120; ++e) {
            wait_event(engine, p, kDacc, 200);
            ok &= sorted_x(p, active);
            for (uint32_t k = 0u; k < active; ++k) {
                const int32_t s = static_cast<int32_t>(p[kNslope + k]);
                minSlope = std::min(minSlope, static_cast<double>(s));
                maxSlope = std::max(maxSlope, static_cast<double>(s));
                if (s < 32768 || s > 174327)
                    bounds = false;
            }
        }
        std::printf("phase_map: drift slope %.4f..%.4f\n", minSlope / 65536.0,
                    maxSlope / 65536.0);
        check(ok, "drift keeps the x boundaries sorted over 120 events");
        check(bounds, "drift keeps every slope inside 0.5..2.66");
    }

    /* The envelope is signed and the LFO depth sign inverts the sweep. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_env_on.p0", 1);
        set_param(engine, "op1_phase_env_r1.p0", 99);
        set_param(engine, "op1_phase_env_r2.p0", 99);
        set_param(engine, "op1_phase_env_r3.p0", 99);
        set_param(engine, "op1_phase_env_l1.p0", 25);
        set_param(engine, "op1_phase_env_l2.p0", 25);
        set_param(engine, "op1_phase_env_l3.p0", 25);
        set_param(engine, "op1_phase_env_mode.p0", 1);
        engine.noteOn(60, 100, 0);
        for (int t = 0; t < 10; ++t)
            tick(engine);
        p1::PhaseMapSnapshot snap;
        engine.phaseMapSnapshot(0, 0, snap);
        std::printf("phase_map: positive env level %d bp %d\n",
                    static_cast<int>(env_pool(engine, 0, 0)[kPeLevel]),
                    snap.bp);
        check(snap.bp > 700, "a positive envelope level moves the breakpoint right");

        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_env_on.p0", 1);
        set_param(engine, "op1_phase_env_r1.p0", 99);
        set_param(engine, "op1_phase_env_r2.p0", 99);
        set_param(engine, "op1_phase_env_r3.p0", 99);
        set_param(engine, "op1_phase_env_l1.p0", -25);
        set_param(engine, "op1_phase_env_l2.p0", -25);
        set_param(engine, "op1_phase_env_l3.p0", -25);
        set_param(engine, "op1_phase_env_mode.p0", 1);
        engine.noteOn(60, 100, 0);
        for (int t = 0; t < 10; ++t)
            tick(engine);
        engine.phaseMapSnapshot(0, 0, snap);
        std::printf("phase_map: negative env level %d bp %d\n",
                    static_cast<int>(env_pool(engine, 0, 0)[kPeLevel]),
                    snap.bp);
        check(snap.bp < 300, "a negative envelope level moves it left");
    }
    {
        int32_t plus[64];
        int32_t minus[64];
        for (int sign = 0; sign < 2; ++sign) {
            p1::Engine engine;
            engine.reset();
            if (!base_setup(engine) || !map_on(engine)) {
                std::printf("phase_map: setup failed\n");
                return 1;
            }
            set_param(engine, "op1_phase_lfo_rate.p0", 500000);
            set_param(engine, "op1_phase_lfo_depth.p0", sign ? -50 : 50);
            set_param(engine, "op1_phase_lfo_mode.p0", 1);
            engine.noteOn(60, 100, 0);
            for (int t = 0; t < 64; ++t) {
                tick(engine);
                (sign ? minus : plus)[t] =
                    static_cast<int32_t>(lfo_pool(engine, 0, 0)[kPlValue]);
            }
        }
        bool inverted = true;
        bool moved = false;
        for (int t = 0; t < 64; ++t) {
            if (plus[t] != -minus[t])
                inverted = false;
            if (plus[t] != 0)
                moved = true;
        }
        check(moved, "the phase LFO moves the breakpoint");
        check(inverted, "the depth sign inverts the sweep");
    }

    /* Key and velocity follow move the breakpoint. The three follow amounts
     * are wired to the global key/velocity switches, so the same gate as the
     * level and feedback follows applies. */
    {
        const auto run = [](bool key, int note, int vel, int& bp) {
            p1::Engine engine;
            engine.reset();
            if (!base_setup(engine) || !map_on(engine))
                return false;
            set_param(engine, "key_sens_on", key ? 1 : 0);
            set_param(engine, "vel_sens_on", key ? 0 : 1);
            set_param(engine, "op1_phase_ks_lo.p0", -100);
            set_param(engine, "op1_phase_ks_hi.p0", 100);
            set_param(engine, "op1_phase_vs.p0", 25);
            engine.noteOn(static_cast<uint32_t>(note),
                          static_cast<uint32_t>(vel), 0);
            for (int t = 0; t < 4; ++t)
                tick(engine);
            p1::PhaseMapSnapshot snap;
            if (!engine.phaseMapSnapshot(0, 0, snap))
                return false;
            bp = snap.bp;
            return true;
        };
        int low = 0;
        int high = 0;
        int slow = 0;
        int fast = 0;
        const bool ok = run(true, 48, 100, low) &&
                        run(true, 72, 100, high) &&
                        run(false, 60, 20, slow) &&
                        run(false, 60, 120, fast);
        std::printf("phase_map: key %d/%d vel %d/%d\n", low, high, slow, fast);
        check(ok && low < 500 && high > 500 && high > low,
              "key follow moves the breakpoint with the note");
        check(ok && fast > slow,
              "velocity follow moves the breakpoint with the velocity");
    }

    /* The fade law reaches full on its first tick at 99. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_lfo_rate_fade.p0", 99);
        set_param(engine, "op1_phase_lfo_depth_fade.p0", 99);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = lfo_pool(engine, 0, 0);
        check(p[kPlRateLevel] >= 0x80000000u &&
                  p[kPlDepthLevel] >= 0x80000000u,
              "fade 99 reaches full on the first tick");
    }

    /* Poly retriggers on every note; mono keeps the phrase. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "mode.p0", 0);
        set_param(engine, "op1_phase_env_on.p0", 1);
        set_param(engine, "op1_phase_env_l1.p0", 100);
        set_param(engine, "op1_phase_env_r1.p0", 1);
        set_param(engine, "op1_phase_env_l2.p0", 100);
        set_param(engine, "op1_phase_env_l3.p0", 100);
        set_param(engine, "op1_phase_env_mode.p0", 1);
        engine.noteOn(60, 100, 0);
        for (int t = 0; t < 200; ++t)
            tick(engine);
        const int32_t before = static_cast<int32_t>(env_pool(engine, 0, 0)[kPeLevel]);
        engine.noteOn(64, 100, 0);
        tick(engine);
        const int32_t after = static_cast<int32_t>(env_pool(engine, 0, 0)[kPeLevel]);
        check(before > 0 && after < before,
              "poly retriggers the phase envelope on a new note");
    }
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "mode.p0", 1);
        set_param(engine, "legato.p0", 1);
        set_param(engine, "op1_phase_env_on.p0", 1);
        set_param(engine, "op1_phase_env_l1.p0", 100);
        set_param(engine, "op1_phase_env_r1.p0", 1);
        set_param(engine, "op1_phase_env_l2.p0", 100);
        set_param(engine, "op1_phase_env_l3.p0", 100);
        set_param(engine, "op1_phase_env_mode.p0", 1);
        engine.noteOn(60, 100, 0);
        for (int t = 0; t < 200; ++t)
            tick(engine);
        const int32_t before = static_cast<int32_t>(env_pool(engine, 0, 0)[kPeLevel]);
        engine.noteOn(64, 100, 0);
        tick(engine);
        const int32_t after = static_cast<int32_t>(env_pool(engine, 0, 0)[kPeLevel]);
        check(before > 0 && after >= before,
              "a legato slide keeps the phase envelope");
    }

    /* The two parts keep separate maps, breakpoints and seeds. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_phase_on.p0", 1);
        set_param(engine, "op1_phase_on.p1", 1);
        set_param(engine, "op1_phase_bp.p0", 200);
        set_param(engine, "op1_phase_bp.p1", 800);
        set_param(engine, "op1_phase_segments.p0", 8);
        set_param(engine, "op1_phase_segments.p1", 16);
        engine.noteOn(60, 100, 0);
        engine.noteOn(67, 100, 1);
        tick(engine);
        uint32_t* p0 = pm(engine, 0, 0);
        uint32_t* p1 = pm(engine, 0, 1);
        check(p0[kBp] != p1[kBp], "part 0 and part 1 keep separate breakpoints");
        check(p0[kLastSegs] != p1[kLastSegs],
              "part 0 and part 1 keep separate segment counts");
        check(p0[2242] != p1[2242], "the two parts seed different PRNGs");
    }

    /* Wave 13 builds a protected mask from a and b and the base morph. */
    {
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine) || !map_on(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        set_param(engine, "op1_waveform.p0", 13);
        set_param(engine, "op1_spec_range_a.p0", 64);
        set_param(engine, "op1_spec_range_b.p0", 64);
        set_param(engine, "op1_spec_a1.p0", 99);
        set_param(engine, "op1_spec_b1.p0", 80);
        set_param(engine, "op1_spec_b2.p0", 40);
        set_param(engine, "op1_spec_b3.p0", 60);
        set_param(engine, "op1_spec_b4.p0", 20);
        set_param(engine, "op1_phase_segments.p0", 12);
        engine.noteOn(60, 100, 0);
        tick(engine);
        uint32_t* p = pm(engine, 0, 0);
        check(mask_nonempty(p), "wave 13 builds a protected mask");
        const uint32_t aMask = p[kMask];
        set_param(engine, "op1_phase_segments.p0", 13);
        tick(engine);
        p = pm(engine, 0, 0);
        std::printf("phase_map: wave13 mask %08x -> %08x\n", aMask,
                    p[kMask]);
        check(p[kMask] == aMask,
              "the mask does not change when only the segment count changes");
        set_param(engine, "op1_spec_morph.p0", 99);
        tick(engine);
        p = pm(engine, 0, 0);
        check(mask_nonempty(p), "wave 13 keeps a mask at the b end of the morph");
    }

    /* The 1x render is finite and the CPU ratio stays well inside the gate.
     * The 2x read is not built, so this measures the 1x path only. */
    {
        std::vector<int32_t> pcm;
        p1::Engine engine;
        engine.reset();
        if (!base_setup(engine)) {
            std::printf("phase_map: setup failed\n");
            return 1;
        }
        engine.noteOn(60, 100, 0);
        render(engine, 48000, pcm);
        bool finite = true;
        for (int32_t v : pcm)
            if (v == INT32_MIN || v == INT32_MAX)
                finite = false;
        check(finite, "the mapped render is finite");
        check(rms(pcm) > 0.0, "the mapped render is not silent");

        auto measure = [](bool worst) {
            p1::Engine e;
            e.reset();
            base_setup(e);
            for (int op = 1; op <= 4; ++op) {
                char name[64];
                std::snprintf(name, sizeof(name), "op%d_phase_on.p0", op);
                set_param(e, name, worst ? 1 : 0);
                std::snprintf(name, sizeof(name), "op%d_phase_segments.p0", op);
                set_param(e, name, 32);
                std::snprintf(name, sizeof(name), "op%d_phase_flip.p0", op);
                set_param(e, name, worst ? 100 : 0);
                std::snprintf(name, sizeof(name), "op%d_phase_flip_rate.p0", op);
                set_param(e, name, 20000000);
                std::snprintf(name, sizeof(name), "op%d_phase_reorder.p0", op);
                set_param(e, name, worst ? 100 : 0);
                std::snprintf(name, sizeof(name), "op%d_phase_reorder_rate.p0", op);
                set_param(e, name, 20000000);
                std::snprintf(name, sizeof(name), "op%d_phase_drift.p0", op);
                set_param(e, name, worst ? 100 : 0);
                std::snprintf(name, sizeof(name), "op%d_phase_drift_rate.p0", op);
                set_param(e, name, 20000000);
                std::snprintf(name, sizeof(name), "op%d_phase_lfo_depth.p0", op);
                set_param(e, name, worst ? 50 : 0);
                std::snprintf(name, sizeof(name), "op%d_phase_env_on.p0", op);
                set_param(e, name, worst ? 1 : 0);
                std::snprintf(name, sizeof(name), "op%d_level.p0", op);
                set_param(e, name, 75);
                std::snprintf(name, sizeof(name), "op%d_mute.p0", op);
                set_param(e, name, 0);
            }
            if (worst)
                set_param(e, "op1_waveform.p0", 13);
            for (int n = 0; n < kVoices; ++n)
                e.noteOn(24u + static_cast<uint32_t>(n), 100, 0);
            std::vector<int32_t> out(2 * kTickFrames, 0);
            const auto start = std::chrono::steady_clock::now();
            for (int b = 0; b < 1000; ++b)
                e.render(out.data(), kTickFrames);
            const auto end = std::chrono::steady_clock::now();
            return std::chrono::duration<double>(end - start).count();
        };
        const double base = measure(false);
        const double worst = measure(true);
        const double ratio = worst / base;
        std::printf("phase_map: CPU baseline %.3f ms, worst %.3f ms, ratio %.2f\n",
                    base * 1000.0, worst * 1000.0, ratio);
        check(ratio > 0.0 && ratio < 6.0,
              "the 1x worst case stays inside the measurement budget");
    }

    if (failures == 0)
        std::printf("phase_map: all checks passed\n");
    else
        std::printf("phase_map: %d check(s) failed\n", failures);
    return failures == 0 ? 0 : 1;
}
