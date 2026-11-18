/* SPDX-License-Identifier: GPL-3.0-only */
/* The filter's cutoff law, resonance onset and input soft knee. */
#include <cmath>
#include <cstdint>
#include <cstdio>

#include "CutoffLaw.h"
#include "p1_math.h"

namespace {

int failures = 0;

void check(bool ok, const char* what)
{
    std::printf("filter: %-64s %s\n", what, ok ? "ok" : "FAIL");
    if (!ok)
        ++failures;
}

int32_t res_k(int32_t res)
{
    return static_cast<int32_t>(
        (static_cast<int64_t>(res) * 65536 / 41942) * 25 / 4);
}

} // namespace

int main()
{
    std::setvbuf(stdout, nullptr, _IONBF, 0);

    check(p1_filter_coeff_q16(0) == p1_filter_g_q16[0],
          "position 0 reads the first table entry");
    check(p1_filter_coeff_q16(55296) == p1_filter_g_q16[256],
          "the top position reads the last table entry");
    check(p1_filter_coeff_q16(55296) <= 55296,
          "the top coefficient stays under the feedback ceiling");
    check(p1_filter_coeff_q16(36864) > 7000 &&
              p1_filter_coeff_q16(36864) < 7500,
          "the middle position is about 900 Hz");
    check(p1_filter_coeff_q16(1000) < p1_filter_coeff_q16(2000) &&
              p1_filter_coeff_q16(2000) < p1_filter_coeff_q16(3000),
          "the coefficient rises with the position");

    check(std::abs(p1::cutoffHzFromPosition(0) - 3.5) < 0.01,
          "position 0 reads 3.5 Hz");
    check(std::abs(p1::cutoffHzFromPosition(55296) - 14336.0) < 1.0,
          "the top position reads 14.3 kHz");
    check(p1::cutoffOldToNew(8000) == 36958,
          "the old default migrates to 909 Hz");
    check(std::abs(p1::cutoffHzFromPosition(p1::cutoffOldToNew(8000)) - 908.8) <
              1.0,
          "the migrated default keeps its frequency");
    check(p1::cutoffOldToNew(0) == 0, "a closed old cutoff stays closed");
    check(p1::cutoffOldToNew(55296) > 53000,
          "the old top maps near the new top");

    check(res_k(33554) >= 5 * 65536,
          "80 percent reaches a loop gain of 5");
    check(res_k(30000) < 45 * 65536 / 10,
          "72 percent stays under the onset");
    check(res_k(41942) == 25 * 65536 / 4,
          "the top reaches a loop gain of 6.25");

    check(p1_soft_clip(0) == 0, "the soft knee is silent at zero");
    check(p1_soft_clip(8192) > 8100 && p1_soft_clip(8192) < 8192,
          "the soft knee is gentle at -12 dBFS");
    check(p1_soft_clip(32767) < 30000 && p1_soft_clip(32767) > 28000,
          "the soft knee compresses at full scale");
    check(p1_soft_clip(-8192) == -p1_soft_clip(8192),
          "the soft knee is symmetric");

    const double comp = 7209.0 / 32767.0;
    const double k75 = res_k(31456) / 65536.0;
    const double g75 = (1.0 + k75 * comp) / (1.0 + k75);
    check(std::abs(20.0 * std::log10(g75) + 8.95) < 0.15,
          "the 22 percent makeup matches the SH-101 at 75 percent");
    const double k100 = res_k(41942) / 65536.0;
    const double g100 = (1.0 + k100 * comp) / (1.0 + k100);
    check(std::abs(20.0 * std::log10(g100) + 9.69) < 0.15,
          "the 22 percent makeup matches the SH-101 at 100 percent");

    std::printf("filter: %s\n", failures ? "FAILED" : "passed");
    return failures ? 1 : 0;
}
