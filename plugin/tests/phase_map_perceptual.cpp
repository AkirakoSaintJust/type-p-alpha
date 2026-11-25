/* SPDX-License-Identifier: GPL-3.0-only */
/* Deterministic phase-map perceptual battery. It renders a fixed set of
 * scenarios with the compiled P1_PHASE_READ mode, writes the PCM and two
 * compact logs:
 *
 *   manifest.csv  one row per scenario (parameters, note, length, f0)
 *   geometry.csv  the map event timeline (hash of the geometry, seed and
 *                 stochastic clocks) plus a per-scenario timeline hash
 *   transfer.csv  the direct-read table error against the exact F/SEG read,
 *                 for every mode 1..6, sampled at map rebuilds
 *
 * The renderer never uses wall-clock time. Every scenario starts from
 * Engine::reset(), so the PRNG seed is the fixed 0x9E3779B9 + part*4 and the
 * event sequence is reproducible across builds. Compare two directories with
 * phase_map_perceptual_compare. */
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <string>
#include <vector>

#include "Engine.hpp"
#include "Params.h"
#include "PhaseMapLayout.h"

namespace {

constexpr uint32_t kRate = 48000u;
constexpr int kTick = 32;
constexpr uint32_t kBase = p1::phase_map::kBaseWords;

/* The phase_map pool offsets, mirrored from the node. */
constexpr uint32_t kNx = 0u;
constexpr uint32_t kNslope = 33u;
constexpr uint32_t kNflip = 65u;
constexpr uint32_t kOrder = 97u;
constexpr uint32_t kPy0 = 129u;
constexpr uint32_t kPslope = 161u;
constexpr uint32_t kSeg = 193u;
constexpr uint32_t kF = 1217u;
constexpr uint32_t kBp = 2244u;
constexpr uint32_t kLastSegs = 2245u;
constexpr uint32_t kDacc = 2246u;
constexpr uint32_t kFacc = 2247u;
constexpr uint32_t kRacc = 2248u;
constexpr uint32_t kSeed = 2242u;
constexpr uint32_t kSeeded = 2243u;
constexpr uint32_t kActive = 2412u;
constexpr uint32_t kLastB = 2241u;

struct Scenario {
    std::string id;
    int note = 60;
    int note1 = -1;      /* part 1 note, or -1 */
    int chord = -1;      /* a second note on part 0 */
    int wave = 12;
    int bp = 500;
    int segments = 8;
    int flip = 0;
    int flipHz = 0;
    int reorder = 0;
    int reorderHz = 0;
    int jump = 1;
    int drift = 0;
    int driftHz = 0;
    int morph = 0;
    int morphSweep = 0;
    int lfoDepth = 0;
    int lfoHz = 0;
    int lfoWave = 0;
    int lfoMode = 0;
    int envOn = 0;
    int envMode = 0;
    int releaseAt = -1;  /* frames, or -1 for no note-off */
    int frames = 36000;
};

double midiHz(int note)
{
    return 440.0 * std::pow(2.0, (static_cast<double>(note) - 69.0) / 12.0);
}

uint32_t rateQ32(double hz)
{
    if (hz <= 0.0)
        return 0u;
    const double q = hz * 4294967296.0 / static_cast<double>(kRate);
    if (q >= 4294967295.0)
        return 4294967295u;
    return static_cast<uint32_t>(std::llround(q));
}

uint32_t paramId(const char* name)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (std::strcmp(spec.id, name) == 0)
            return static_cast<uint32_t>(spec.engineId);
    }
    return ~0u;
}

bool setParam(p1::Engine& engine, const char* name, int32_t value)
{
    const uint32_t id = paramId(name);
    if (id == ~0u) {
        std::printf("perceptual: missing parameter %s\n", name);
        return false;
    }
    engine.setParam(id, value);
    return true;
}

bool setPart(p1::Engine& engine, int part, const char* stem, int32_t value)
{
    char name[96];
    std::snprintf(name, sizeof(name), "%s.p%d", stem, part);
    return setParam(engine, name, value);
}

bool configurePart(p1::Engine& engine, int part, const Scenario& s)
{
    bool ok = true;
    ok &= setPart(engine, part, "algorithm", 9);
    ok &= setPart(engine, part, "level", 99);
    ok &= setPart(engine, part, "filter_on", 0);
    ok &= setPart(engine, part, "attack", 99);
    ok &= setPart(engine, part, "release", 99);
    ok &= setPart(engine, part, "op1_waveform", s.wave);
    ok &= setPart(engine, part, "op1_ratio", 65536);
    ok &= setPart(engine, part, "op1_detune", 0);
    ok &= setPart(engine, part, "op1_level", 99);
    ok &= setPart(engine, part, "op1_feedback", 0);
    ok &= setPart(engine, part, "op1_mute", 0);
    ok &= setPart(engine, part, "op2_level", 0);
    ok &= setPart(engine, part, "op2_mute", 1);
    ok &= setPart(engine, part, "op3_level", 0);
    ok &= setPart(engine, part, "op3_mute", 1);
    ok &= setPart(engine, part, "op4_level", 0);
    ok &= setPart(engine, part, "op4_mute", 1);
    ok &= setPart(engine, part, "op1.eg.d1r", 99);
    ok &= setPart(engine, part, "op1.eg.d2r", 99);
    ok &= setPart(engine, part, "op1.eg.d1l", 99);
    ok &= setPart(engine, part, "op1.eg.d2l", 99);

    ok &= setPart(engine, part, "op1_phase_on", 1);
    ok &= setPart(engine, part, "op1_phase_bp", s.bp);
    ok &= setPart(engine, part, "op1_phase_segments", s.segments);
    ok &= setPart(engine, part, "op1_phase_flip", s.flip);
    ok &= setPart(engine, part, "op1_phase_flip_rate",
                  static_cast<int32_t>(rateQ32(s.flipHz)));
    ok &= setPart(engine, part, "op1_phase_reorder", s.reorder);
    ok &= setPart(engine, part, "op1_phase_reorder_rate",
                  static_cast<int32_t>(rateQ32(s.reorderHz)));
    ok &= setPart(engine, part, "op1_phase_jump", s.jump);
    ok &= setPart(engine, part, "op1_phase_drift", s.drift);
    ok &= setPart(engine, part, "op1_phase_drift_rate",
                  static_cast<int32_t>(rateQ32(s.driftHz)));
    ok &= setPart(engine, part, "op1_phase_ks_lo", 0);
    ok &= setPart(engine, part, "op1_phase_ks_hi", 0);
    ok &= setPart(engine, part, "op1_phase_vs", 0);

    ok &= setPart(engine, part, "op1_phase_lfo_depth", s.lfoDepth);
    ok &= setPart(engine, part, "op1_phase_lfo_rate",
                  static_cast<int32_t>(rateQ32(s.lfoHz)));
    ok &= setPart(engine, part, "op1_phase_lfo_wave", s.lfoWave);
    ok &= setPart(engine, part, "op1_phase_lfo_mode", s.lfoMode);
    ok &= setPart(engine, part, "op1_phase_env_on", s.envOn);
    ok &= setPart(engine, part, "op1_phase_env_mode", s.envMode);
    if (s.envOn) {
        ok &= setPart(engine, part, "op1_phase_env_r1", 40);
        ok &= setPart(engine, part, "op1_phase_env_r2", 70);
        ok &= setPart(engine, part, "op1_phase_env_r3", 40);
        ok &= setPart(engine, part, "op1_phase_env_l1", 99);
        ok &= setPart(engine, part, "op1_phase_env_l2", 0);
        ok &= setPart(engine, part, "op1_phase_env_l3", 99);
    }

    if (s.wave == 14) {
        /* A bright saw-like a and a hollow odd-harmonic b. The phase map's
         * protected mask is built from these, and the morph sweeps exercise
         * the mask rebuild. */
        char stem[96];
        for (int h = 1; h <= 32; ++h) {
            std::snprintf(stem, sizeof(stem), "op1_spec_a%d", h);
            ok &= setPart(engine, part, stem, 99 / h);
            std::snprintf(stem, sizeof(stem), "op1_spec_b%d", h);
            ok &= setPart(engine, part, stem, (h & 1) ? (99 / h) : 0);
        }
        ok &= setPart(engine, part, "op1_spec_range_a", 0);
        ok &= setPart(engine, part, "op1_spec_range_b", 0);
        ok &= setPart(engine, part, "op1_spec_morph", s.morph);
        ok &= setPart(engine, part, "op1_spec_env_on", 0);
        ok &= setPart(engine, part, "op1_spec_lfo_depth", 0);
    }

    return ok;
}

uint32_t* pool(p1::Engine& engine, int part)
{
    return reinterpret_cast<uint32_t*>(engine.state()->op1_phase_map) +
           static_cast<uint32_t>(part) * p1::phase_map::kWords;
}

uint64_t mixWord(uint64_t h, uint32_t w)
{
    for (int b = 0; b < 4; ++b) {
        h ^= (w >> (b * 8)) & 0xFFu;
        h *= 1099511628211ull;
    }
    return h;
}

/* The geometry hash excludes the table (which differs by design) and the
 * per-block clocks, but keeps the seed, so a stochastic event shows up. */
uint64_t geometryHash(const uint32_t* pm)
{
    uint64_t h = 1469598103934665603ull;
    for (uint32_t i = 0; i < kBase; ++i) {
        if (i == kDacc || i == kFacc || i == kRacc || i == kLastB)
            continue;
        h = mixWord(h, pm[i]);
    }
    return h;
}

uint32_t exactY(const uint32_t* pm, uint32_t x)
{
    uint32_t m = pm[kActive];
    if (m < 1u)
        m = 1u;
    if (m > 32u)
        m = 32u;
    uint32_t q = pm[kF + (x >> 2)] << 2;
    if (q > 4095u)
        q = 4095u;
    uint32_t sh = (q & 3u) * 8u;
    uint32_t k = (pm[kSeg + (q >> 2)] >> sh) & 0xFFu;
    if (k >= m)
        k = m - 1u;
    int32_t y = static_cast<int32_t>(pm[kPy0 + k]) +
                ((static_cast<int32_t>(q) - static_cast<int32_t>(pm[kNx + k])) *
                     static_cast<int32_t>(pm[kPslope + k]) >>
                 16);
    return static_cast<uint32_t>(y);
}

uint32_t tableY(const uint32_t* pm, uint32_t x, int mode)
{
    switch (mode) {
    case 1:
        return exactY(pm, x);
    case 2: {
        const uint32_t base = x & ~7u;
        return exactY(pm, base);
    }
    case 3: {
        const uint32_t base = x & ~7u;
        const uint32_t fr = x & 7u;
        const int32_t y0 = static_cast<int32_t>(exactY(pm, base));
        const int32_t y1 = static_cast<int32_t>(
            exactY(pm, (base + 8u <= 4095u) ? base + 8u : 4095u));
        return static_cast<uint32_t>(y0 + (((y1 - y0) * static_cast<int32_t>(fr)) >> 3));
    }
    case 4: {
        const uint32_t base = x & ~3u;
        const uint32_t fr = x & 3u;
        const int32_t y0 = static_cast<int32_t>(exactY(pm, base));
        const int32_t y1 = static_cast<int32_t>(
            exactY(pm, (base + 4u <= 4095u) ? base + 4u : 4095u));
        return static_cast<uint32_t>(y0 + (((y1 - y0) * static_cast<int32_t>(fr)) >> 2));
    }
    case 5: {
        const uint32_t base = x & ~15u;
        return exactY(pm, base);
    }
    case 6: {
        const uint32_t base = x & ~15u;
        const uint32_t fr = x & 15u;
        const int32_t y0 = static_cast<int32_t>(exactY(pm, base));
        const int32_t y1 = static_cast<int32_t>(
            exactY(pm, (base + 16u <= 4095u) ? base + 16u : 4095u));
        return static_cast<uint32_t>(y0 + (((y1 - y0) * static_cast<int32_t>(fr)) >> 4));
    }
    default:
        return exactY(pm, x);
    }
}

struct Transfer {
    uint32_t maxAbs = 0u;
    double rms = 0.0;
    uint32_t worstX = 0u;
};

Transfer transferError(const uint32_t* pm, int mode)
{
    Transfer t;
    double sum = 0.0;
    for (uint32_t x = 0; x < 4096u; ++x) {
        const int32_t e = static_cast<int32_t>(exactY(pm, x)) -
                          static_cast<int32_t>(tableY(pm, x, mode));
        const uint32_t a = static_cast<uint32_t>(e < 0 ? -e : e);
        if (a > t.maxAbs) {
            t.maxAbs = a;
            t.worstX = x;
        }
        sum += static_cast<double>(e) * static_cast<double>(e);
    }
    t.rms = std::sqrt(sum / 4096.0);
    return t;
}

/* The post-F map y(q) is piecewise linear; a table in q does not inherit the
 * F compression at extreme breakpoints. This is the audit for that variant. */
uint32_t exactYq(const uint32_t* pm, uint32_t q)
{
    uint32_t m = pm[kActive];
    if (m < 1u)
        m = 1u;
    if (m > 32u)
        m = 32u;
    if (q > 4095u)
        q = 4095u;
    const uint32_t sh = (q & 3u) * 8u;
    uint32_t k = (pm[kSeg + (q >> 2)] >> sh) & 0xFFu;
    if (k >= m)
        k = m - 1u;
    const int32_t y = static_cast<int32_t>(pm[kPy0 + k]) +
                      ((static_cast<int32_t>(q) - static_cast<int32_t>(pm[kNx + k])) *
                           static_cast<int32_t>(pm[kPslope + k]) >>
                       16);
    return static_cast<uint32_t>(y);
}

Transfer qTransferError(const uint32_t* pm, uint32_t n, uint32_t step, bool linear)
{
    Transfer t;
    double sum = 0.0;
    for (uint32_t q = 0; q < 4096u; ++q) {
        const uint32_t base = (q / step) * step;
        uint32_t v;
        if (!linear) {
            v = exactYq(pm, base);
        } else {
            const uint32_t fr = q - base;
            const uint32_t next = (base + step <= 4095u) ? base + step : 4095u;
            const int32_t y0 = static_cast<int32_t>(exactYq(pm, base));
            const int32_t y1 = static_cast<int32_t>(exactYq(pm, next));
            v = static_cast<uint32_t>(
                y0 + ((y1 - y0) * static_cast<int32_t>(fr)) /
                         static_cast<int32_t>(step));
        }
        const int32_t e = static_cast<int32_t>(exactYq(pm, q)) -
                          static_cast<int32_t>(v);
        const uint32_t a = static_cast<uint32_t>(e < 0 ? -e : e);
        if (a > t.maxAbs) {
            t.maxAbs = a;
            t.worstX = q;
        }
        sum += static_cast<double>(e) * static_cast<double>(e);
    }
    t.rms = std::sqrt(sum / 4096.0);
    return t;
}

void writeQAudit(std::FILE* f, const char* id, int block, int part,
                 const uint32_t* pm)
{
    struct Case {
        uint32_t n;
        uint32_t step;
        bool linear;
    };
    static const Case cases[] = {
        {256u, 16u, false}, {256u, 16u, true}, {512u, 8u, true},
        {1024u, 4u, true},
    };
    for (const Case& c : cases) {
        const Transfer t = qTransferError(pm, c.n, c.step, c.linear);
        std::fprintf(f, "%s,%d,%d,%u,%u,%d,%u,%.6f,%.6f,%u\n", id, block,
                     part, c.n, c.step, c.linear ? 1 : 0, t.maxAbs, t.rms,
                     static_cast<double>(t.maxAbs) * 360.0 / 4096.0, t.worstX);
    }
}

/* Read the compiled table back and compare it with the exact q map. This
 * catches a rebuild that was skipped, which a model-only audit cannot see. */
Transfer actualQError(const uint32_t* pm, int mode)
{
    Transfer t;
    const uint16_t* pt =
        reinterpret_cast<const uint16_t*>(pm + p1::phase_map::kTable);
    double sum = 0.0;
    for (uint32_t q = 0; q < 4096u; ++q) {
        uint32_t v = 0u;
        if (mode == 7) {
            v = pt[q >> 4];
        } else if (mode == 11) {
            v = pt[(q + 3u) >> 2];
        } else if (mode == 8 || mode == 9 || mode == 10) {
            const uint32_t step = (mode == 8) ? 16u : (mode == 9) ? 8u : 4u;
            const uint32_t shift = (mode == 8) ? 4u : (mode == 9) ? 3u : 2u;
            const uint32_t ti = q / step;
            const uint32_t fr = q % step;
            const int32_t y0 = static_cast<int32_t>(pt[ti]);
            const int32_t y1 = static_cast<int32_t>(pt[ti + 1u]);
            v = static_cast<uint32_t>(
                y0 + (((y1 - y0) * static_cast<int32_t>(fr)) >> shift));
        } else {
            return t;
        }
        const int32_t e = static_cast<int32_t>(exactYq(pm, q)) -
                          static_cast<int32_t>(v);
        const uint32_t a = static_cast<uint32_t>(e < 0 ? -e : e);
        if (a > t.maxAbs) {
            t.maxAbs = a;
            t.worstX = q;
        }
        sum += static_cast<double>(e) * static_cast<double>(e);
    }
    t.rms = std::sqrt(sum / 4096.0);
    return t;
}

std::vector<Scenario> buildScenarios()
{
    std::vector<Scenario> v;
    auto add = [&](const Scenario& s) { v.push_back(s); };
    auto named = [](const char* fmt, int a, int b = 0, int c = 0) {
        char buf[128];
        std::snprintf(buf, sizeof(buf), fmt, a, b, c);
        return std::string(buf);
    };

    /* The static base character: a note sweep, a breakpoint sweep and a
     * segment sweep, with no stochastic clocks and no modulation. */
    for (int wave : {0, 11, 12, 13, 14}) {
        for (int note : {24, 36, 48, 60, 72, 84, 96}) {
            Scenario s;
            s.id = named("static-w%d-n%d", wave, note);
            s.wave = wave;
            s.note = note;
            add(s);
        }
    }
    for (int wave : {0, 12}) {
        for (int bp : {1, 125, 250, 375, 500, 625, 750, 875, 999}) {
            Scenario s;
            s.id = named("bp-w%d-bp%d", wave, bp);
            s.wave = wave;
            s.bp = bp;
            add(s);
        }
    }
    for (int wave : {0, 12}) {
        for (int seg : {1, 2, 4, 8, 16, 32}) {
            Scenario s;
            s.id = named("seg-w%d-s%d", wave, seg);
            s.wave = wave;
            s.segments = seg;
            add(s);
        }
    }
    for (int note : {24, 36}) {
        for (int seg : {8, 32}) {
            for (int wave : {11, 12}) {
                Scenario s;
                s.id = named("low-w%d-n%d-s%d", wave, note, seg);
                s.wave = wave;
                s.note = note;
                s.segments = seg;
                add(s);
            }
        }
    }
    for (int note : {84, 96}) {
        for (int bp : {250, 750}) {
            for (int wave : {11, 12}) {
                Scenario s;
                s.id = named("high-w%d-n%d-bp%d", wave, note, bp);
                s.wave = wave;
                s.note = note;
                s.bp = bp;
                add(s);
            }
        }
    }

    /* The stochastic map: drift, flips and the local reorder. */
    for (int seg : {8, 32}) {
        for (int drift : {25, 100}) {
            for (int hz : {1, 10}) {
                Scenario s;
                s.id = named("drift-s%d-d%d-hz%d", seg, drift, hz);
                s.segments = seg;
                s.drift = drift;
                s.driftHz = hz;
                s.frames = 72000;
                add(s);
            }
        }
    }
    for (int seg : {8, 32}) {
        for (int flip : {50, 100}) {
            for (int hz : {2, 20}) {
                Scenario s;
                s.id = named("flip-s%d-f%d-hz%d", seg, flip, hz);
                s.segments = seg;
                s.flip = flip;
                s.flipHz = hz;
                s.frames = 72000;
                add(s);
            }
        }
    }
    for (int seg : {8, 32}) {
        for (int reorder : {50, 100}) {
            for (int hz : {2, 20}) {
                Scenario s;
                s.id = named("order-s%d-r%d-hz%d", seg, reorder, hz);
                s.segments = seg;
                s.reorder = reorder;
                s.reorderHz = hz;
                s.jump = 4;
                s.frames = 72000;
                add(s);
            }
        }
    }
    for (int note : {60, 84}) {
        for (int seg : {8, 32}) {
            Scenario s;
            s.id = named("all-n%d-s%d", note, seg);
            s.note = note;
            s.segments = seg;
            s.flip = 50;
            s.flipHz = 10;
            s.reorder = 50;
            s.reorderHz = 10;
            s.jump = 4;
            s.drift = 50;
            s.driftHz = 10;
            s.frames = 72000;
            add(s);
        }
    }
    {
        Scenario s;
        s.id = "all-part1";
        s.note = -1;
        s.note1 = 67;
        s.flip = 50;
        s.flipHz = 10;
        s.reorder = 50;
        s.reorderHz = 10;
        s.jump = 4;
        s.drift = 50;
        s.driftHz = 10;
        s.frames = 72000;
        add(s);
    }

    /* The modulation paths that move the breakpoint: LFO, envelope and the
     * additive morph. These rebuild F (and the table) every block. */
    for (int rate : {0, 5, 20}) {
        for (int wave : {0, 2, 4}) {
            Scenario s;
            s.id = named("lfo-hz%d-w%d", rate, wave);
            s.lfoDepth = 50;
            s.lfoHz = rate;
            s.lfoWave = wave;
            s.lfoMode = 0;
            add(s);
        }
    }
    for (int mode : {0, 1}) {
        for (int note : {36, 60}) {
            Scenario s;
            s.id = named("env-m%d-n%d", mode, note);
            s.note = note;
            s.envOn = 1;
            s.envMode = mode;
            add(s);
        }
    }
    for (int seg : {8, 32}) {
        for (int morph : {0, 50, 99}) {
            Scenario s;
            s.id = named("morph-s%d-m%d", seg, morph);
            s.wave = 14;
            s.segments = seg;
            s.morph = morph;
            add(s);
        }
    }
    for (int seg : {8, 32}) {
        Scenario s;
        s.id = named("morphsweep-s%d", seg);
        s.wave = 14;
        s.segments = seg;
        s.morphSweep = 1;
        add(s);
    }

    /* Musical transients: note-off release and shared-map chords. */
    for (int note : {24, 60, 96}) {
        for (int stochastic : {0, 1}) {
            Scenario s;
            s.id = named("rel-n%d-st%d", note, stochastic);
            s.note = note;
            s.releaseAt = 24000;
            s.frames = 72000;
            if (stochastic) {
                s.flip = 50;
                s.flipHz = 5;
                s.reorder = 50;
                s.reorderHz = 5;
                s.jump = 4;
            }
            add(s);
        }
    }
    {
        Scenario s;
        s.id = "chord-2";
        s.chord = 64;
        s.frames = 72000;
        add(s);
    }
    {
        Scenario s;
        s.id = "chord-3";
        s.chord = 64;
        s.note1 = 55;
        s.frames = 72000;
        add(s);
    }
    {
        Scenario s;
        s.id = "chord-3-st";
        s.chord = 64;
        s.note1 = 55;
        s.flip = 50;
        s.flipHz = 5;
        s.reorder = 50;
        s.reorderHz = 5;
        s.jump = 4;
        s.frames = 72000;
        add(s);
    }
    return v;
}

} // namespace

int main(int argc, char** argv)
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);
    if (argc < 2) {
        std::printf("usage: %s <output-directory>\n", argv[0]);
        return 2;
    }
    const std::string outdir = argv[1];
    std::error_code ec;
    std::filesystem::create_directories(outdir, ec);
    if (ec) {
        std::printf("perceptual: cannot create %s: %s\n", outdir.c_str(),
                    ec.message().c_str());
        return 2;
    }

    const auto scenarios = buildScenarios();
    std::FILE* manifest = std::fopen((outdir + "/manifest.csv").c_str(), "wb");
    std::FILE* geometry = std::fopen((outdir + "/geometry.csv").c_str(), "wb");
    std::FILE* transfer = std::fopen((outdir + "/transfer.csv").c_str(), "wb");
    std::FILE* qtransfer = std::fopen((outdir + "/qtransfer.csv").c_str(), "wb");
    std::FILE* stale = std::fopen((outdir + "/stale.csv").c_str(), "wb");
    if (!manifest || !geometry || !transfer || !qtransfer || !stale) {
        std::printf("perceptual: cannot write the logs\n");
        return 2;
    }
    std::fprintf(manifest,
                 "# mode=%d sample_rate=%u channels=2 tick=%d table_n=%u "
                 "table_step=%u table_extra=%u\n",
                 P1_PHASE_READ, kRate, kTick, p1::phase_map::kTableN,
                 p1::phase_map::kTableStep, p1::phase_map::kTableExtra);
    std::fprintf(manifest,
                 "id,note,note1,chord,wave,bp,segments,flip,flip_hz,reorder,"
                 "reorder_hz,jump,drift,drift_hz,morph,morph_sweep,lfo_depth,"
                 "lfo_hz,lfo_wave,lfo_mode,env_on,env_mode,release_at,frames,f0\n");
    std::fprintf(geometry,
                 "scenario,kind,block,part,hash,seed,dacc,facc,racc,lastb,"
                 "active,bp\n");
    std::fprintf(transfer,
                 "scenario,block,part,mode,max_lsb,rms_lsb,max_deg,worst_x\n");
    std::fprintf(qtransfer,
                 "scenario,block,part,n,step,linear,max_lsb,rms_lsb,max_deg,"
                 "worst_q\n");
    std::fprintf(stale, "scenario,block,part,mode,max_lsb,rms_lsb,worst_q\n");

    int failures = 0;
    for (const auto& s : scenarios) {
        p1::Engine engine;
        engine.reset();
        const bool ok = configurePart(engine, 0, s) &&
                        (s.note1 < 0 || configurePart(engine, 1, s));
        if (!ok) {
            std::printf("perceptual: setup failed for %s\n", s.id.c_str());
            ++failures;
            continue;
        }

        if (s.note >= 0)
            engine.noteOn(static_cast<uint32_t>(s.note), 100, 0);
        if (s.chord >= 0)
            engine.noteOn(static_cast<uint32_t>(s.chord), 100, 0);
        if (s.note1 >= 0)
            engine.noteOn(static_cast<uint32_t>(s.note1), 100, 1);

        std::vector<int32_t> pcm(static_cast<std::size_t>(s.frames) * 2u, 0);
        uint64_t timeline = 1469598103934665603ull;
        uint64_t lastHash[2] = {0ull, 0ull};
        bool haveHash[2] = {false, false};
        int events[2] = {0, 0};
        int blockNo = 0;
        int releaseAt = s.releaseAt;
        for (int pos = 0; pos < s.frames; pos += kTick) {
            if (releaseAt >= 0 && pos >= releaseAt && releaseAt + kTick > pos) {
                if (s.note >= 0)
                    engine.noteOff(static_cast<uint32_t>(s.note), 0);
                if (s.chord >= 0)
                    engine.noteOff(static_cast<uint32_t>(s.chord), 0);
                if (s.note1 >= 0)
                    engine.noteOff(static_cast<uint32_t>(s.note1), 1);
                releaseAt = -1;
            }
            if (s.morphSweep && (pos % 4800) == 0) {
                const int step = pos / 4800;
                setPart(engine, 0, "op1_spec_morph", step * 99 / 7);
            }
            engine.render(pcm.data() + static_cast<std::size_t>(pos) * 2u, kTick);

            for (int part = 0; part < 2; ++part) {
                uint32_t* pm = pool(engine, part);
                if (!pm[kSeeded])
                    continue;
                const uint64_t h = geometryHash(pm);
                timeline = mixWord(timeline, static_cast<uint32_t>(h));
                timeline = mixWord(timeline, static_cast<uint32_t>(h >> 32));
                for (uint32_t idx : {kSeed, kDacc, kFacc, kRacc, kBp, kActive,
                                     kLastSegs}) {
                    timeline = mixWord(timeline, pm[idx]);
                }
                const bool changed = !haveHash[part] || h != lastHash[part];
                if (changed) {
                    ++events[part];
                    std::fprintf(geometry,
                                 "%s,event,%d,%d,%016llx,%u,%u,%u,%u,%u,%u,%u\n",
                                 s.id.c_str(), blockNo, part,
                                 static_cast<unsigned long long>(h), pm[kSeed],
                                 pm[kDacc], pm[kFacc], pm[kRacc], pm[kLastB],
                                 pm[kActive], pm[kBp]);
                    haveHash[part] = true;
                    lastHash[part] = h;
                }
                /* Audit the transfer error at the first rebuild and then
                 * sparsely through a moving map; the final state is audited
                 * after the render. */
                if (changed && (events[part] == 1 || (events[part] % 64) == 0)) {
                    for (int mode = 1; mode <= 6; ++mode) {
                        const Transfer t = transferError(pm, mode);
                        std::fprintf(
                            transfer, "%s,%d,%d,%d,%u,%.6f,%.6f,%u\n",
                            s.id.c_str(), blockNo, part, mode, t.maxAbs, t.rms,
                            static_cast<double>(t.maxAbs) * 360.0 / 4096.0,
                            t.worstX);
                    }
                    writeQAudit(qtransfer, s.id.c_str(), blockNo, part, pm);
                    if (p1::phase_map::kQDomain) {
                        const Transfer at = actualQError(pm, P1_PHASE_READ);
                        std::fprintf(stale, "%s,%d,%d,%d,%u,%.6f,%u\n",
                                     s.id.c_str(), blockNo, part, P1_PHASE_READ,
                                     at.maxAbs, at.rms, at.worstX);
                    }
                }
            }
            ++blockNo;
        }

        for (int part = 0; part < 2; ++part) {
            uint32_t* pm = pool(engine, part);
            if (!pm[kSeeded])
                continue;
            const uint64_t h = geometryHash(pm);
            std::fprintf(geometry,
                         "%s,final,%d,%d,%016llx,%u,%u,%u,%u,%u,%u,%u\n",
                         s.id.c_str(), blockNo - 1, part,
                         static_cast<unsigned long long>(h), pm[kSeed],
                         pm[kDacc], pm[kFacc], pm[kRacc], pm[kLastB],
                         pm[kActive], pm[kBp]);
            for (int mode = 1; mode <= 6; ++mode) {
                const Transfer t = transferError(pm, mode);
                std::fprintf(transfer, "%s,%d,%d,%d,%u,%.6f,%.6f,%u\n",
                             s.id.c_str(), blockNo - 1, part, mode, t.maxAbs,
                             t.rms,
                             static_cast<double>(t.maxAbs) * 360.0 / 4096.0,
                             t.worstX);
            }
            writeQAudit(qtransfer, s.id.c_str(), blockNo - 1, part, pm);
            if (p1::phase_map::kQDomain) {
                const Transfer at = actualQError(pm, P1_PHASE_READ);
                std::fprintf(stale, "%s,%d,%d,%d,%u,%.6f,%u\n", s.id.c_str(),
                             blockNo - 1, part, P1_PHASE_READ, at.maxAbs,
                             at.rms, at.worstX);
            }
        }
        std::fprintf(geometry, "%s,timeline,%016llx\n", s.id.c_str(),
                     static_cast<unsigned long long>(timeline));

        const std::string path = outdir + "/" + s.id + ".pcm";
        std::FILE* f = std::fopen(path.c_str(), "wb");
        if (!f) {
            std::printf("perceptual: cannot write %s\n", path.c_str());
            ++failures;
            continue;
        }
        std::fwrite(pcm.data(), sizeof(int32_t), pcm.size(), f);
        std::fclose(f);

        std::fprintf(manifest,
                     "%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"
                     "%d,%d,%d,%d,%d,%.6f\n",
                     s.id.c_str(), s.note, s.note1, s.chord, s.wave, s.bp,
                     s.segments, s.flip, s.flipHz, s.reorder, s.reorderHz,
                     s.jump, s.drift, s.driftHz, s.morph, s.morphSweep,
                     s.lfoDepth, s.lfoHz, s.lfoWave, s.lfoMode, s.envOn,
                     s.envMode, s.releaseAt, s.frames,
                     midiHz(s.note >= 0 ? s.note : s.note1));
    }

    std::fclose(manifest);
    std::fclose(geometry);
    std::fclose(transfer);
    std::fclose(qtransfer);
    std::fclose(stale);
    std::printf("perceptual: mode %d, %zu scenarios, %d failure(s)\n",
                P1_PHASE_READ, scenarios.size(), failures);
    return failures == 0 ? 0 : 1;
}
