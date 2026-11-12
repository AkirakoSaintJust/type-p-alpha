#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Render the feedback-shaper WAV sweep into oscilloscope PNGs.

The `feedback_shaper` test writes 2-second WAVs for the positive reference and the
square and abs shapers, with the average and the one-pole filter. This script reads
the last 2,048 samples of each file and draws four panels per algorithm. It uses
only the standard library, so it runs wherever the WAVs exist.

Usage:
    python plugin/tools/feedback_figures.py docs/research/feedback-shaper
"""

from __future__ import annotations

import argparse
import struct
import wave
import zlib
from pathlib import Path

_SAMPLE_RATE = 44100
_PANEL_WIDTH = 900
_PANEL_HEIGHT = 160
_WINDOW = 2048
_BG = (0, 0, 0)
_GRID = (0x1a, 0x1a, 0x1a)
_ZERO = (0x3a, 0x3a, 0x3a)
_LINE = (0x3d, 0xff, 0x9e)

_GROUPS = (
    ("positive", "positive"),
    ("square-avg", "square-avg"),
    ("square-lp", "square-lp"),
    ("abs-avg", "abs-avg"),
    ("abs-lp", "abs-lp"),
)
_LEVELS = ("25", "50", "75", "100")


def _read_wav(path: Path) -> list[int]:
    with wave.open(str(path), "rb") as handle:
        if handle.getnchannels() != 1 or handle.getsampwidth() != 2:
            raise ValueError(f"{path} is not mono 16-bit")
        frames = handle.getnframes()
        data = handle.readframes(frames)
    return list(struct.unpack(f"<{frames}h", data))


def _chunk(tag: bytes, data: bytes) -> bytes:
    return (
        struct.pack(">I", len(data))
        + tag
        + data
        + struct.pack(">I", zlib.crc32(tag + data) & 0xFFFFFFFF)
    )


def _write_png(path: Path, width: int, height: int, rows: list[bytearray]) -> None:
    raw = b"".join(b"\x00" + bytes(row) for row in rows)
    header = struct.pack(">IIBBBBB", width, height, 8, 2, 0, 0, 0)
    data = (
        b"\x89PNG\r\n\x1a\n"
        + _chunk(b"IHDR", header)
        + _chunk(b"IDAT", zlib.compress(raw, 9))
        + _chunk(b"IEND", b"")
    )
    path.write_bytes(data)


def _pixel(row: bytearray, x: int, y: int, colour: tuple[int, int, int]) -> None:
    if 0 <= x < _PANEL_WIDTH and 0 <= y < _PANEL_HEIGHT:
        offset = x * 3
        row[offset] = colour[0]
        row[offset + 1] = colour[1]
        row[offset + 2] = colour[2]


def _panel(samples: list[int]) -> list[bytearray]:
    rows = [bytearray(_BG * _PANEL_WIDTH) for _ in range(_PANEL_HEIGHT)]
    for x in range(0, _PANEL_WIDTH, 60):
        for y in range(_PANEL_HEIGHT):
            _pixel(rows[y], x, y, _GRID)
    for y in range(0, _PANEL_HEIGHT, 32):
        for x in range(_PANEL_WIDTH):
            _pixel(rows[y], x, y, _GRID)
    centre = _PANEL_HEIGHT // 2
    for x in range(_PANEL_WIDTH):
        _pixel(rows[centre], x, centre, _ZERO)

    window = samples[-_WINDOW:] if len(samples) > _WINDOW else samples
    previous = centre
    for x in range(_PANEL_WIDTH):
        index = int(x * (len(window) - 1) / max(1, _PANEL_WIDTH - 1))
        value = window[index] / 32768.0
        y = int(centre - value * (centre - 8))
        y = max(0, min(_PANEL_HEIGHT - 1, y))
        start, end = (previous, y) if previous <= y else (y, previous)
        for row in range(start, end + 1):
            _pixel(rows[row], x, row, _LINE)
        previous = y
    return rows


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("directory", type=Path, help="the WAV directory")
    args = parser.parse_args()

    for group, prefix in _GROUPS:
        rows: list[bytearray] = []
        for level in _LEVELS:
            path = args.directory / f"{prefix}-{level}.wav"
            if not path.is_file():
                raise SystemExit(f"missing {path}")
            rows.extend(_panel(_read_wav(path)))
        out = args.directory / f"scope-{group}.png"
        _write_png(out, _PANEL_WIDTH, _PANEL_HEIGHT * len(_LEVELS), rows)
        print(f"wrote {out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
