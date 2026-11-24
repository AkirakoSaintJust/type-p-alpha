/* SPDX-License-Identifier: GPL-3.0-only */
/* The phase_map pool layout shared by the engine snapshot, the tests and the
 * perceptual harness. P1_PHASE_READ selects the direct-read table:
 *   0  the F and segment read (the reference)
 *   1  the exact 4096-entry table
 *   2  the 512-step table
 *   3  the 512-linear table
 *   4  the 1024-linear table
 *   5  the 256-step table
 *   6  the 256-linear table
 *   7  the 256-step post-F table
 *   8  the 256-linear post-F table
 *   9  the 512-linear post-F table
 *  10  the 1024-linear post-F table
 *  11  the 1024-entry exact post-F table (q is 4-phase quantised)
 * Modes 1-6 are indexed by input phase; modes 7-11 are indexed by the post-F
 * q, so they follow a compressed breakpoint instead of undersampling it.
 * The node derives the same values from the same switch. P1_PM_WORDS_EXPECTED
 * is set by CMake so a mismatch fails the build instead of silently reading
 * the next part's pool. */
#pragma once

#include <cstdint>

#ifndef P1_PHASE_READ
#define P1_PHASE_READ 0
#endif

namespace p1::phase_map {

constexpr uint32_t kBaseWords = 2560u; /* the geometry, F, SEG and clocks */
constexpr uint32_t kTable = kBaseWords; /* the direct-read table starts here */

#if P1_PHASE_READ == 1
constexpr uint32_t kTableN = 4096u;
constexpr uint32_t kTableStep = 1u;
constexpr uint32_t kTableExtra = 0u;
#elif P1_PHASE_READ == 2
constexpr uint32_t kTableN = 512u;
constexpr uint32_t kTableStep = 8u;
constexpr uint32_t kTableExtra = 0u;
#elif P1_PHASE_READ == 3
constexpr uint32_t kTableN = 512u;
constexpr uint32_t kTableStep = 8u;
constexpr uint32_t kTableExtra = 1u;
#elif P1_PHASE_READ == 4
constexpr uint32_t kTableN = 1024u;
constexpr uint32_t kTableStep = 4u;
constexpr uint32_t kTableExtra = 1u;
#elif P1_PHASE_READ == 5
constexpr uint32_t kTableN = 256u;
constexpr uint32_t kTableStep = 16u;
constexpr uint32_t kTableExtra = 0u;
#elif P1_PHASE_READ == 6
constexpr uint32_t kTableN = 256u;
constexpr uint32_t kTableStep = 16u;
constexpr uint32_t kTableExtra = 1u;
#elif P1_PHASE_READ == 7
constexpr uint32_t kTableN = 256u;
constexpr uint32_t kTableStep = 16u;
constexpr uint32_t kTableExtra = 0u;
#elif P1_PHASE_READ == 8
constexpr uint32_t kTableN = 256u;
constexpr uint32_t kTableStep = 16u;
constexpr uint32_t kTableExtra = 1u;
#elif P1_PHASE_READ == 9
constexpr uint32_t kTableN = 512u;
constexpr uint32_t kTableStep = 8u;
constexpr uint32_t kTableExtra = 1u;
#elif P1_PHASE_READ == 10
constexpr uint32_t kTableN = 1024u;
constexpr uint32_t kTableStep = 4u;
constexpr uint32_t kTableExtra = 1u;
#elif P1_PHASE_READ == 11
constexpr uint32_t kTableN = 1024u;
constexpr uint32_t kTableStep = 4u;
constexpr uint32_t kTableExtra = 1u;
#else
constexpr uint32_t kTableN = 0u;
constexpr uint32_t kTableStep = 1u;
constexpr uint32_t kTableExtra = 0u;
#endif

constexpr bool kQDomain = P1_PHASE_READ >= 7;

/* 16-bit entries, two per pool word, plus the interpolation tail. */
constexpr uint32_t kTableWords = (kTableN + kTableExtra + 1u) / 2u;
constexpr uint32_t kWords = kTable + kTableWords;

#ifdef P1_PM_WORDS_EXPECTED
static_assert(kWords == static_cast<uint32_t>(P1_PM_WORDS_EXPECTED),
              "the phase-map read table changed the pool size; update CMake");
#endif

} // namespace p1::phase_map
