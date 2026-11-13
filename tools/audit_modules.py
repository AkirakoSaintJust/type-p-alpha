#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Audit the local .dev node and macro library against the Type P.alpha patch.

The .dev folder is local-only, so a checkout without it exits 0 with a notice.
This checks that every type and every node parameter used by the patch resolves
to a module, and that the generated manifest still has the expected shape.
"""
from __future__ import annotations

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
MODULES = ROOT / ".dev" / "modules"
PATCH = ROOT / "synth" / "patch.json"
PARAMS = ROOT / "engine" / "gen" / "params.json"
NODES = MODULES / "nodes"
MACROS = MODULES / "macros"


def load(path: Path) -> dict:
    return json.loads(path.read_text(encoding="utf-8"))


def module_spec(kind: str) -> dict:
    if (NODES / kind / "node.json").exists():
        return load(NODES / kind / "node.json")
    if (MACROS / kind / "macro.json").exists():
        return load(MACROS / kind / "macro.json")
    raise SystemExit(f"missing module for {kind!r}")


def target_node(value: object) -> str:
    if isinstance(value, dict):
        value = value.get("target", "")
    return str(value).split(".")[0]


def main() -> int:
    if not MODULES.exists():
        print("audit: .dev is local-only, skipped")
        return 0

    patch = load(PATCH)
    used: dict[str, list[dict]] = {}
    for zone in ("voice", "global"):
        for node in patch[zone]["nodes"]:
            used.setdefault(node["type"], []).append(node)

    macro = load(MACROS / "fm_op" / "macro.json")
    for node in macro["nodes"]:
        used.setdefault(node["type"], []).append(node)

    missing = [kind for kind in used if not (
        (NODES / kind / "node.json").exists()
        or (MACROS / kind / "macro.json").exists())]

    param_problems: list[str] = []
    tables = set(patch.get("tables", {}))
    for kind, instances in used.items():
        spec = module_spec(kind)
        declared = set(spec.get("params", {}))
        for instance in instances:
            for name, value in instance.get("params", {}).items():
                if isinstance(value, str) and value in tables:
                    continue  # a table selector, not a node parameter
                if name not in declared:
                    param_problems.append(
                        f"{kind}/{instance['id']}.{name}")

    map_problems: list[str] = []
    for name, value in macro["params"].items():
        node = target_node(value)
        if node not in {n["id"] for n in macro["nodes"]}:
            map_problems.append(f"{name} -> {value}")

    params = load(PARAMS)["params"]
    names = [p["name"] for p in params]
    routing = ("channel.", "low.", "high.", "level.", "delay.")
    part_block = sum(
        name.endswith(".p0") and not name.startswith(routing) for name in names)
    waveform_max = max(
        (p["max"] for p in params if p["name"].startswith("op1_waveform")),
        default=-1)
    spec_per_op = sum(
        1 for name in names
        if name.startswith("op1_spec_") and name.endswith(".p0"))
    spec_params = [p for p in params if p["name"].startswith("op1_spec_")
                   and p["name"].endswith(".p0")]
    spec_defaults = {p["name"]: p["default"] for p in spec_params
                     if p["name"] in (
                         "op1_spec_a1.p0", "op1_spec_a2.p0",
                         "op1_spec_b1.p0", "op1_spec_b2.p0",
                         "op1_spec_morph.p0", "op1_spec_env_depth.p0",
                         "op1_spec_range_a.p0", "op1_spec_range_b.p0")}
    expected_defaults = {
        "op1_spec_a1.p0": 99, "op1_spec_a2.p0": 0,
        "op1_spec_b1.p0": 99, "op1_spec_b2.p0": 45,
        "op1_spec_morph.p0": 0, "op1_spec_env_depth.p0": 0,
        "op1_spec_range_a.p0": 0, "op1_spec_range_b.p0": 0,
    }

    nodes = sorted(p.name for p in NODES.iterdir() if p.is_dir())
    macros = sorted(p.name for p in MACROS.iterdir() if p.is_dir())
    macro_params = len(macro.get("params", {}))

    print(f"modules: {len(nodes)} nodes, {len(macros)} macros")
    print(f"used by type-p: {len(used)} types")
    print(f"missing: {', '.join(missing) if missing else 'none'}")
    print(f"node params: {', '.join(param_problems) if param_problems else 'ok'}")
    print(f"macro map: {', '.join(map_problems) if map_problems else 'ok'}"
          f" ({macro_params} params)")
    print(f"manifest: {len(params)} params, part block {part_block}, "
          f"waveform max {waveform_max}")
    print(f"spectral: {spec_per_op} params per operator")
    print(f"spectral defaults: {spec_defaults}")
    print("unused nodes: " + ", ".join(sorted(set(nodes) - set(used))))
    print("unused macros: " + ", ".join(sorted(set(macros) - set(used))))

    if missing or param_problems or map_problems:
        return 1
    if part_block != 416 or waveform_max != 13 or spec_per_op != 47:
        print("audit: generated manifest shape changed")
        return 1
    if spec_defaults != expected_defaults:
        print("audit: spectral defaults changed")
        return 1
    print("audit: passed")
    return 0


if __name__ == "__main__":
    sys.exit(main())
