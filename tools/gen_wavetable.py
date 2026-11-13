#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
# Copyright (C) 2026 P1Onken
"""Generate engine/gen/wavetable.h, the 13-wave oscillator union.

The layout is one contiguous Q15 table, `p1_wave`:
  - waves 0-9 (the sin-derived OPZ set, the rectified sine and the rectified
    triangle): one 1024-sample table each. They are band-limited by
    construction, so they need no anti-aliasing mip set.
  - waves 10 (pulse-sine), 11 (square) and 12 (saw): MIP=9 band-limited tables
    each, with harmonic counts 2^8..2^0 (256, 128, 64, 32, 16, 8, 4, 2, 1).
    Each is a truncated-Fourier synthesis of the naive shape, so the highest
    harmonics that would otherwise alias above Nyquist are absent, and the
    oscillator picks the level whose top harmonic sits below Nyquist.

`p1_wave_base[13]` gives each waveform's mip-0 offset in 1024-sample units,
and a reader adds `mip << 10` for waves 10..12. Total 37888 samples.

Waveform value 13 selects the spectral oscillator and has no table.

The output is deterministic and is checked in as engine/gen/wavetable.h.

Run from the repository root:  python3 tools/gen_wavetable.py
"""
import argparse
import math
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "engine" / "gen" / "wavetable.h"
HARMONICS_OUT = ROOT / "engine" / "gen" / "wave_harmonics.h"
N = 1024
MIP = 9
WAVES = 13
BL_WAVES = (10, 11, 12)


def q15(x: float) -> int:
    v = int(round(x * 32768.0))
    return max(-32768, min(32767, v))


def clamp15(x: float) -> int:
    """Round an already-Q15 value and clamp it into [-32768, 32767]."""
    v = int(round(x))
    return max(-32768, min(32767, v))


# Quarter sine, the same resolution as the engine's sine table.
SIN = [q15(math.sin(math.pi / 2.0 * i / 256)) for i in range(256)]


def sin_q15(phase: int) -> int:
    q = (phase >> 30) & 3
    i = (phase >> 22) & 255
    v = SIN[255 - i] if (q & 1) else SIN[i]
    return -v if (q & 2) else v


def q15_mul(a: int, b: int) -> int:
    return (a * b + (1 << 14)) >> 15


def sin_wave(phase: int, ws: int) -> int:
    """The naive shape as a pure function of the top 10 phase bits."""
    s = sin_q15(phase)
    a = -s if s < 0 else s
    half = (phase >> 31) & 1
    q = (phase >> 30) & 3
    if ws == 0:
        return s
    if ws == 1:
        return q15_mul(s, a)                      # triangle
    if ws == 2:
        return 0 if half else a                   # half sine
    if ws == 3:
        return 0 if half else q15_mul(s, a)       # half triangle
    if ws == 4:
        return 0 if half else sin_q15((phase << 1) & 0xFFFFFFFF)     # 2x sine
    if ws == 5:
        s2 = sin_q15((phase << 1) & 0xFFFFFFFF)
        return 0 if half else q15_mul(s2, -s2 if s2 < 0 else s2)     # 2x triangle
    if ws == 6:
        s2 = sin_q15((phase << 1) & 0xFFFFFFFF)
        return 0 if half else (-s2 if s2 < 0 else s2)                # 2x half sine
    if ws == 7:
        s2 = sin_q15((phase << 1) & 0xFFFFFFFF)
        return 0 if half else q15_mul(s2, s2)     # 2x half triangle
    if ws == 8:
        return a                                  # rectified sine
    if ws == 9:
        p = phase / 4294967296.0
        q = (p + 0.25) % 1.0
        tri = 1.0 - 4.0 * abs(q - 0.5)
        return clamp15(abs(tri) * 32767.0)        # rectified triangle
    if ws == 10:
        return 0 if q else a                      # pulse sine (first quarter)
    if ws == 11:
        return -32767 if half else 32767          # square
    if ws == 12:
        return (phase >> 16) - 32768              # saw
    return s


def band_limited(ws: int, harm: int) -> list[int]:
    """The band-limited Q15 table for waveform `ws`, harmonics 1..harm.

    The sine and cosine coefficients are projected from the naive shape with a
    DFT, so the pulse-sine is handled exactly like the square and the saw. The
    Gibbs overshoot at a discontinuity is clamped to full scale."""
    a = [0.0] * (harm + 1)
    b = [0.0] * (harm + 1)
    for k in range(harm + 1):
        sa = sb = 0.0
        for i in range(N):
            ang = 2.0 * math.pi * k * i / N
            v = sin_wave(i << 22, ws)
            sa += v * math.cos(ang)
            sb += v * math.sin(ang)
        a[k] = sa * 2.0 / N
        b[k] = sb * 2.0 / N
    out: list[int] = []
    for i in range(N):
        ang = 2.0 * math.pi * i / N
        total = a[0] * 0.5
        for k in range(1, harm + 1):
            total += a[k] * math.cos(k * ang) + b[k] * math.sin(k * ang)
        out.append(clamp15(total))
    return out


def c_array(name: str, ctype: str, values: list[int]) -> list[str]:
    lines = [f"static const {ctype} {name}[{len(values)}] = {{"]
    row: list[str] = []
    for v in values:
        row.append(f"{v:>6d}")
        if len(row) == 16:
            lines.append("    " + ", ".join(row) + ",")
            row = []
    if row:
        lines.append("    " + ", ".join(row) + ",")
    lines.append("};")
    return lines


def build() -> tuple[list[int], list[int]]:
    wave: list[int] = []
    base: list[int] = []
    for ws in range(WAVES):
        base.append(len(wave) // N)
        if ws in BL_WAVES:
            for mip in range(MIP):
                wave.extend(band_limited(ws, 1 << (MIP - 1 - mip)))
        else:
            for p in range(N):
                wave.append(sin_wave(p << 22, ws))
    return wave, base


def harmonics(wave: list[int], base: list[int]) -> list[list[int]]:
    rows: list[list[int]] = []
    for ws in range(WAVES):
        start = base[ws] * N
        table = wave[start : start + N]
        mags: list[float] = []
        for h in range(1, 17):
            re = 0.0
            im = 0.0
            for i, v in enumerate(table):
                ang = 2.0 * math.pi * h * i / N
                re += v * math.cos(ang)
                im -= v * math.sin(ang)
            mags.append(math.hypot(re, im) * 2.0 / N)
        peak = max(mags) or 1.0
        rows.append([max(0, min(99, int(round(m / peak * 99.0)))) for m in mags])
    return rows


def render_harmonics(rows: list[list[int]]) -> str:
    h = [
        "/* SPDX-License-Identifier: GPL-3.0-only",
        " * Copyright (C) 2026 P1Onken",
        " * GENERATED by tools/gen_wavetable.py, do not edit by hand.",
        " * 13 waves by 16 harmonic magnitudes, 0..99, from the mip-0 table.",
        " * DC is dropped and the largest harmonic is scaled to 99.",
        " */",
        "#pragma once",
        "#include <stdint.h>",
        "",
        "static const uint8_t p1_wave_harmonics[13][16] = {",
    ]
    for row in rows:
        h.append("    {" + ", ".join(f"{v:>3d}" for v in row) + "},")
    h.append("};")
    return "\n".join(h) + "\n"


def render(wave: list[int], base: list[int]) -> str:
    h = [
        "/* SPDX-License-Identifier: GPL-3.0-only",
        " * Copyright (C) 2026 P1Onken",
        " * GENERATED by tools/gen_wavetable.py, do not edit by hand.",
        " * The 13-waveform oscillator union, Q15.",
        " * Waves 0-9 are one 1024-sample table each; waves 10 (pulse), 11 (square),",
        " * 12 (saw) are MIP=9 band-limited tables (harmonic counts 256..1) for the",
        " * anti-aliasing. `p1_wave_base[ws]` is ws's mip-0 offset in 1024-sample",
        " * units; the per-sample index is",
        " *   p1_wave[(p1_wave_base[ws] + mip) << 10 | (phase >> 22)].",
        " * Waveform value 13 selects the spectral oscillator and has no table.",
        " */",
        "#pragma once",
        "#include <stdint.h>",
        "",
    ]
    h += c_array("p1_wave", "int16_t", wave)
    h += [""]
    h += c_array("p1_wave_base", "uint8_t", base)
    return "\n".join(h) + "\n"


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("--out", type=Path, default=OUT)
    parser.add_argument("--harmonics-out", type=Path, default=HARMONICS_OUT)
    args = parser.parse_args()
    wave, base = build()
    args.out.parent.mkdir(parents=True, exist_ok=True)
    args.out.write_text(render(wave, base), encoding="utf-8")
    rows = harmonics(wave, base)
    args.harmonics_out.parent.mkdir(parents=True, exist_ok=True)
    args.harmonics_out.write_text(render_harmonics(rows), encoding="utf-8")
    print(
        f"wrote {args.out} ({len(wave)} samples, {len(base)} wave bases) and "
        f"{args.harmonics_out} ({len(rows)}x16 harmonics)"
    )


if __name__ == "__main__":
    main()
