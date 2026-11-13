/* SPDX-License-Identifier: GPL-3.0-only
 *
 * Measures the additive table regeneration and the control tick. The table
 * generator is timed on its own, then the tick is timed across a run that
 * changes the levels, and the engine block is timed with and without voices to
 * separate the cold path from the active crossfade.
 */
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>

#include "Engine.hpp"
#include "Params.h"
#include "p1_math.h"

namespace {

constexpr int kRate = 48000;
constexpr int kControlBlock = 32;

double secondsSince(std::chrono::steady_clock::time_point start)
{
    return std::chrono::duration<double>(std::chrono::steady_clock::now() - start)
        .count();
}

struct TickResult {
    double mean = 0.0;
    double worst = 0.0;
    int over50us = 0;
};

TickResult runTickSeries(int ticks, bool reads)
{
    int16_t bank[P1_SPEC_BANK] = {0};
    int32_t waveform[2] = {13, 13};
    int32_t rangeA[2] = {0, 0};
    int32_t rangeB[2] = {0, 0};
    int32_t a[16][2] = {};
    int32_t b[16][2] = {};
    for (int i = 0; i < 16; ++i) {
        a[i][0] = a[i][1] = (i * 7 + 3) % 100;
        b[i][0] = b[i][1] = (i * 11 + 5) % 100;
    }

    TickResult result;
    double total = 0.0;
    for (int tick = 0; tick < ticks; ++tick) {
        if (tick % 16 == 0) {
            a[(tick / 16) % 16][0] = (tick * 13) % 100;
            a[(tick * 5) % 16][1] = (tick * 17) % 100;
        }
        if (reads) {
            p1_spectral_read(bank, 0, 0x20000000u, 0, 0);
            p1_spectral_read(bank, 1, 0x20000000u, 0, 0);
        }
        const auto start = std::chrono::steady_clock::now();
        p1_spectral_tick(
            bank, waveform, rangeA, rangeB,
            a[0], a[1], a[2], a[3], a[4], a[5], a[6], a[7],
            a[8], a[9], a[10], a[11], a[12], a[13], a[14], a[15],
            b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7],
            b[8], b[9], b[10], b[11], b[12], b[13], b[14], b[15]);
        const double elapsed = secondsSince(start);
        total += elapsed;
        result.worst = std::max(result.worst, elapsed);
        if (elapsed > 50e-6)
            ++result.over50us;
    }
    result.mean = total / ticks;
    return result;
}

bool setParam(p1::Engine& engine, const char* id, int32_t value)
{
    for (const auto& spec : kP1ParamSpecs) {
        if (std::strcmp(spec.id, id) == 0) {
            engine.setParam(static_cast<uint32_t>(spec.engineId), value);
            return true;
        }
    }
    return false;
}

void setAll(p1::Engine& engine,
            const std::vector<std::pair<const char*, int32_t>>& values)
{
    for (const auto& [id, value] : values)
        setParam(engine, id, value);
}

/* Four operators, both parts, spectrum mode, one clean carrier per part. */
void setupSpectrumEngine(p1::Engine& engine)
{
    engine.reset();
    setAll(engine, {
        {"algorithm.p0", 7}, {"algorithm.p1", 7},
        {"filter_on.p0", 0}, {"filter_on.p1", 0},
        {"op1_level.p0", 99}, {"op1_level.p1", 99},
        {"op1_feedback.p0", 0}, {"op1_feedback.p1", 0},
        {"op1_waveform.p0", 13}, {"op1_waveform.p1", 13},
        {"op2_waveform.p0", 13}, {"op2_waveform.p1", 13},
        {"op3_waveform.p0", 13}, {"op3_waveform.p1", 13},
        {"op4_waveform.p0", 13}, {"op4_waveform.p1", 13},
        {"op2_level.p0", 0}, {"op2_level.p1", 0},
        {"op3_level.p0", 0}, {"op3_level.p1", 0},
        {"op4_level.p0", 0}, {"op4_level.p1", 0},
    });
}

struct BlockResult {
    double changeMean = 0.0;
    double changeWorst = 0.0;
    double steadyMean = 0.0;
};

/* parts is the number of parts with notes: 0, 1 or 2. */
BlockResult runEngineChanges(int blocks, int parts)
{
    p1::Engine engine;
    setupSpectrumEngine(engine);
    for (int part = 0; part < parts; ++part) {
        for (int i = 0; i < 4; ++i)
            engine.noteOn(static_cast<uint32_t>(48 + i * 5), 100,
                          static_cast<uint32_t>(part));
    }
    std::vector<int32_t> output(static_cast<std::size_t>(kRate) * 2u);
    engine.render(output.data(), kRate); /* settle */
    output.assign(static_cast<std::size_t>(kControlBlock) * 2u, 0);

    BlockResult result;
    double changeTotal = 0.0;
    double steadyTotal = 0.0;
    int changes = 0;
    int steady = 0;
    for (int block = 0; block < blocks; ++block) {
        const bool change = block % 16 == 0;
        if (change) {
            /* Change every operator and part so all eight spectra are dirty. */
            for (int op = 1; op <= 4; ++op) {
                for (int part = 0; part < 2; ++part) {
                    char id[32];
                    std::snprintf(id, sizeof(id), "op%d_spec_a1.p%d", op, part);
                    setParam(engine, id,
                             (block * 13 + op * 7 + part) % 100);
                    std::snprintf(id, sizeof(id), "op%d_spec_b1.p%d", op, part);
                    setParam(engine, id,
                             (block * 17 + op * 5 + part) % 100);
                }
            }
        }
        const auto start = std::chrono::steady_clock::now();
        engine.render(output.data(), kControlBlock);
        const double elapsed = secondsSince(start);
        if (change) {
            changeTotal += elapsed;
            result.changeWorst = std::max(result.changeWorst, elapsed);
            ++changes;
        } else {
            steadyTotal += elapsed;
            ++steady;
        }
    }
    result.changeMean = changeTotal / changes;
    result.steadyMean = steadyTotal / steady;
    return result;
}

} // namespace

int main()
{
    std::printf("additive schedule bench\n");
    std::printf("rate %d, control block %d samples\n", kRate, kControlBlock);

    int16_t table[P1_SPEC_TABLE] = {0};
    int32_t levels[16];
    for (int i = 0; i < 16; ++i)
        levels[i] = (i * 37 + 11) % 100;
    constexpr int kTables = 2000;
    uint64_t checksum = 0;
    double best = 1e9;
    for (int run = 0; run < 3; ++run) {
        const auto start = std::chrono::steady_clock::now();
        for (int n = 0; n < kTables; ++n) {
            p1_spectral_table(table, levels, 1 + (n & 7) * 16);
            checksum += static_cast<uint16_t>(table[n & (P1_SPEC_SIZE - 1)]);
        }
        best = std::min(best, secondsSince(start));
    }
    std::printf("table generation: %.1f us per spectrum (5 mips)\n",
                best / kTables * 1e6);

    for (const bool reads : {false, true}) {
        const TickResult tick = runTickSeries(20000, reads);
        std::printf("tick series, %s: mean %.1f us, worst %.1f us, "
                    "over 50 us %d\n",
                    reads ? "active read" : "no read", tick.mean * 1e6,
                    tick.worst * 1e6, tick.over50us);
    }

    for (const int parts : {1, 2}) {
        const BlockResult block = runEngineChanges(4096, parts);
        std::printf("engine block, %s: change mean %.1f us, worst %.1f us, "
                    "steady %.1f us\n",
                    parts == 1 ? "one part" : "both parts",
                    block.changeMean * 1e6, block.changeWorst * 1e6,
                    block.steadyMean * 1e6);
    }

    std::printf("checksum %llu\n", static_cast<unsigned long long>(checksum));
    return 0;
}
