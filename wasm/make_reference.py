#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Turn the native 48 kHz capture header into a JSON reference for the WASM test."""
from __future__ import annotations

import json
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
HEADER = ROOT / "plugin" / "tests" / "engine_render_reference.h"


def array(text: str, name: str) -> list[int]:
    match = re.search(rf"{name}\[512\]\s*=\s*\{{(.*?)\}};", text, re.S)
    if match is None:
        raise SystemExit(f"wasm: {name} not found in {HEADER}")
    return [int(value) for value in re.findall(r"-?\d+", match.group(1))]


def main() -> int:
    text = HEADER.read_text(encoding="utf-8")
    json.dump({"noteOn": array(text, "kEngineNoteOn"),
               "release": array(text, "kEngineRelease")}, sys.stdout)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
