# SPDX-License-Identifier: GPL-3.0-only
# Copyright (C) 2026 P1Onken (umetate)
"""Coarse ratio detents shared by the firmware panel and the plugin editor."""

from __future__ import annotations

import math

_Q16 = 65536

# A ratio detent is a multiple of one of these bases. The first five are the
# panel's harmonic and inharmonic series, whilst the last three add the half roots
# in the lower range: sqrt(2)/2, pi/4 and sqrt(3)/2.
_RATIO_BASES: tuple[float, ...] = (
    0.5,
    1.0,
    math.sqrt(2.0),
    math.pi / 2.0,
    math.sqrt(3.0),
    math.sqrt(2.0) / 2.0,
    math.pi / 4.0,
    math.sqrt(3.0) / 2.0,
)


def _display(value: int) -> tuple[str, float]:
    """The 2-decimal readout and the ideal it was rounded from."""
    ratio = value / _Q16
    text = f"{ratio:.2f}"
    return text, float(text)


def ratio_detents(lo: int, hi: int) -> list[int]:
    """The sorted Q16 ratio detents in [lo, hi]. Two series that meet inside the
    last displayed digit collapse to one detent, so every click changes the
    readout, and the survivor is the one whose ratio is closest to that readout."""
    groups: dict[str, list[int]] = {}
    for base in _RATIO_BASES:
        k = 1
        while True:
            value = math.ceil(base * k * _Q16)
            if value > hi:
                break
            if value >= lo:
                groups.setdefault(_display(value)[0], []).append(value)
            k += 1
    kept: list[int] = []
    for values in groups.values():
        kept.append(min(values, key=lambda v: abs(v / _Q16 - _display(v)[1])))
    return sorted(kept)
