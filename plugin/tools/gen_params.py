"""Generate the C++ parameter table from engine/gen/params.json.

The manifest is the only reference. This keeps the JUCE APVTS, the engine ids
and the editor names in one place, because the APVTS key is the stable manifest
name and the engine id is the manifest index.
"""

from __future__ import annotations

import json
import sys
from pathlib import Path
from typing import TypedDict, cast

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))


class ParamDoc(TypedDict):
    """One generated manifest row."""

    id: int
    name: str
    target: str
    min: int
    max: int
    default: int
    curve: str
    unit: str
    rate: str
    modulatable: bool
    display: str
    automatable: bool


class ParamsDoc(TypedDict, total=False):
    """The generated params.json document."""

    params: list[ParamDoc]
    base_param_count: int
    routing_base: int


def display_name(name: str) -> str:
    part = ""
    if name.endswith(".p0") or name.endswith(".p1"):
        part = f" p{int(name[-1]) + 1}"
        name = name[:-3]
    name = name.replace("_", " ").replace(".", " ")
    return name + part


def is_ring_param(name: str) -> bool:
    return name.startswith("ring") or (
        name.startswith("car") and "_out.p" in name
    )


def reorder_ring(params: list[ParamDoc]) -> list[ParamDoc]:
    """Move the ringmod part rows to the end of each part group.

    The engine keeps the ringmod parameters at the route node, but the part
    record appends them after the version 16 block. The spec order is the record
    order, so the ring rows move to the end of the part 1 and part 2 groups and
    the engine id stays on the row.
    """
    p1_start = next(
        (i for i, p in enumerate(params) if str(p["name"]).endswith(".p1")),
        None,
    )
    if p1_start is None:
        return params
    p1_end = next(
        (
            i
            for i in range(p1_start + 1, len(params))
            if not str(params[i]["name"]).endswith(".p1")
        ),
        len(params),
    )

    def move(group: list[ParamDoc]) -> list[ParamDoc]:
        ring = [p for p in group if is_ring_param(str(p["name"]))]
        return [p for p in group if not is_ring_param(str(p["name"]))] + ring

    return (
        move(params[:p1_start])
        + move(params[p1_start:p1_end])
        + params[p1_end:]
    )


def reorder_key(params: list[ParamDoc]) -> list[ParamDoc]:
    """Move the key-follow part rows to the end of each part group.

    The preset record stores part parameters in spec order. The breakpoint and
    the two follow enables are new in version 18 and must append after every
    earlier part row, so every existing part index keeps its position.
    """
    key_names = {"breakpoint", "key_sens_on", "vel_sens_on"}

    def is_key(name: str) -> bool:
        return name.split(".")[0] in key_names

    p1_start = next(
        (i for i, p in enumerate(params) if str(p["name"]).endswith(".p1")),
        None,
    )
    if p1_start is None:
        return params
    p1_end = next(
        (
            i
            for i in range(p1_start + 1, len(params))
            if not str(params[i]["name"]).endswith(".p1")
        ),
        len(params),
    )

    def move(group: list[ParamDoc]) -> list[ParamDoc]:
        key = [p for p in group if is_key(str(p["name"]))]
        return [p for p in group if not is_key(str(p["name"]))] + key

    return (
        move(params[:p1_start])
        + move(params[p1_start:p1_end])
        + params[p1_end:]
    )


def c_string(value: str) -> str:
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"') + '"'


def main() -> int:
    if len(sys.argv) != 3:
        print("usage: gen_params.py params.json Params.h", file=sys.stderr)
        return 2
    source = Path(sys.argv[1])
    target = Path(sys.argv[2])
    raw = cast(object, json.loads(source.read_text(encoding="utf-8")))
    data = cast(ParamsDoc, raw)
    params = reorder_key(reorder_ring(data["params"]))
    from ratio import ratio_detents

    ratio_params = [p for p in params if p["display"] == "ratio"]
    ratio_detents_values: list[int] = []
    if ratio_params:
        ratio_detents_values = ratio_detents(
            min(p["min"] for p in ratio_params),
            max(p["max"] for p in ratio_params),
        )
    names = [str(p["name"]) for p in params]
    routing_prefixes = ("channel.", "low.", "high.", "level.", "delay.")
    part_param_count = sum(
        name.endswith(".p0") and not name.startswith(routing_prefixes) for name in names
    )
    part_count = len({name[-2:] for name in names if name.endswith((".p0", ".p1"))})
    routing_count = sum(name.startswith(routing_prefixes) for name in names)
    routing_base = int(data.get("routing_base", len(params) - routing_count))
    base_count = int(data.get("base_param_count", len(params)))
    part_cutoff_index = next(
        (i for i, p in enumerate(params) if p["name"] == "cutoff.p0"), -1
    )
    if part_cutoff_index >= 0 and part_count > 1:
        neighbour = params[part_param_count + part_cutoff_index]["name"]
        if neighbour != "cutoff.p1":
            raise SystemExit(
                f"part cutoff index {part_cutoff_index} maps to {neighbour!r}"
            )
    target.parent.mkdir(parents=True, exist_ok=True)
    lines = [
        "/* GENERATED by plugin/tools/gen_params.py - do not edit. */",
        "#pragma once",
        "#include <array>",
        "#include <cstdint>",
        "",
        "struct P1ParamSpec {",
        "    int32_t engineId;",
        "    const char* id;",
        "    const char* name;",
        "    const char* unit;",
        "    const char* display;",
        "    int32_t min;",
        "    int32_t max;",
        "    int32_t def;",
        "    bool logarithmic;",
        "    bool modulatable;",
        "    bool automatable;",
        "    bool audioRate;",
        "};",
        "",
        f"inline constexpr std::array<P1ParamSpec, {len(params)}> kP1ParamSpecs = {{{{",
    ]
    for p in params:
        row = (
            f"{p['id']}, "
            + c_string(p["name"])
            + ", "
            + c_string(display_name(p["name"]))
            + ", "
            + c_string(p["unit"])
            + ", "
            + c_string(p["display"])
            + ", "
            + f"{p['min']}, {p['max']}, {p['default']}, "
            + ("true" if p["curve"] == "log" else "false")
            + ", "
            + ("true" if p["modulatable"] else "false")
            + ", "
            + ("true" if p["automatable"] else "false")
            + ", "
            + ("true" if p["rate"] == "sample" else "false")
        )
        lines.append("    {" + row + "},")
    lines += [
        "}};",
        f"inline constexpr int kP1ParamCount = {len(params)};",
        f"inline constexpr int kP1BaseParamCount = {base_count};",
        f"inline constexpr int kP1PartParamCount = {part_param_count};",
        f"inline constexpr int kP1PartCount = {part_count};",
        f"inline constexpr int kP1PartCutoffIndex = {part_cutoff_index};",
        f"inline constexpr int kP1RoutingCount = {routing_count};",
        f"inline constexpr int kP1RoutingBase = {routing_base};",
        "",
    ]
    macro_ids = [p["name"] for p in params if str(p["name"]).startswith("macro")]
    lines.append(f"inline constexpr int kP1MacroParamCount = {len(macro_ids)};")
    lines.append("inline constexpr const char* kP1MacroParamIDs[] = {")
    for name in macro_ids:
        lines.append(f"    {c_string(name)},")
    lines.append("};")
    lines.append("")
    lines += [
        f"inline constexpr std::array<int32_t, {len(ratio_detents_values)}> "
        "kP1RatioDetents = {",
    ]
    for i in range(0, len(ratio_detents_values), 12):
        lines.append(
            "    " + ", ".join(str(v) for v in ratio_detents_values[i : i + 12]) + ","
        )
    lines += [
        "};",
        "",
    ]
    target.write_text("\n".join(lines), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
