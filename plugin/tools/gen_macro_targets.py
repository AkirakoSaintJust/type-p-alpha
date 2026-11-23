"""Generate MacroTargets.h from synth/macros.json and the parameter manifest.

The target enum is the bridge between the macro parameters and the panel. The spec
lists the 29 base targets in order, and every target exists once per part, so the
enum is 58 rows. The generator resolves each target's members against the manifest
names, which catches a renamed parameter at build time, and emits the display names
and sections for the panel.

usage: gen_macro_targets.py macros.json params.json tables.c MacroTargets.h
"""

from __future__ import annotations

import json
import re
import sys
from pathlib import Path
from typing import Any, cast

def op_env_rates(op: int) -> list[str]:
    """The amp, feedback, morph and phase rate names for one operator.

    Operator 1's surge and release are the legacy `attack` and `release` names,
    which the editor maps for the same knobs. The other operators carry their own
    `eg.surge` and `eg.rel` names.
    """
    if op == 1:
        amp = [
            "op1.eg.warm.p{part}",
            "attack.p{part}",
            "op1.eg.d1r.p{part}",
            "op1.eg.d2r.p{part}",
            "release.p{part}",
        ]
    else:
        amp = [
            f"op{op}.eg.warm.p{{part}}",
            f"op{op}.eg.surge.p{{part}}",
            f"op{op}.eg.d1r.p{{part}}",
            f"op{op}.eg.d2r.p{{part}}",
            f"op{op}.eg.rel.p{{part}}",
        ]
    return amp + [
        f"op{op}_fbenv_r1.p{{part}}",
        f"op{op}_fbenv_r2.p{{part}}",
        f"op{op}_fbenv_r3.p{{part}}",
        f"op{op}_fbenv_relr.p{{part}}",
        f"op{op}_spec_env_r1.p{{part}}",
        f"op{op}_spec_env_r2.p{{part}}",
        f"op{op}_spec_env_r3.p{{part}}",
        f"op{op}_spec_env_relr.p{{part}}",
        f"op{op}_phase_env_r1.p{{part}}",
        f"op{op}_phase_env_r2.p{{part}}",
        f"op{op}_phase_env_r3.p{{part}}",
        f"op{op}_phase_env_relr.p{{part}}",
    ]


FAMILIES: dict[str, list[str]] = {
    "op_env_rates": [],  # built per operator by op_env_rates()
    "op_lfo_rates": [
        "op{op}_trem_rate.p{part}",
        "op{op}_fblfo_rate.p{part}",
        "op{op}_dlfo_rate.p{part}",
        "op{op}_spec_lfo_rate.p{part}",
        "op{op}_phase_lfo_rate.p{part}",
    ],
    "op_lfo_depths": [
        "op{op}_trem_depth.p{part}",
        "op{op}_fblfo_depth.p{part}",
        "op{op}_dlfo_depth.p{part}",
        "op{op}_spec_lfo_depth.p{part}",
        "op{op}_phase_lfo_depth.p{part}",
    ],
    "op_levels": ["op{op}_level.p{part}"],
    "op_feedbacks": ["op{op}_feedback.p{part}"],
    "op_morphs": ["op{op}_spec_morph.p{part}"],
    "op_breakpoints": ["op{op}_phase_bp.p{part}"],
}

VELOCITY_DEPTH = [
    *[f"op{op}_{field}_vs.p{{part}}"
      for op in range(1, 5)
      for field in ("level", "fb", "aenv")],
    "penv_vs.p{part}",
    "f1_cutoff_vs.p{part}",
    "fenv_vs.p{part}",
    *[f"op{op}_phase_vs.p{{part}}" for op in range(1, 5)],
]


def c_string(value: str) -> str:
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"') + '"'


def load_carriers(tables_path: Path) -> list[list[bool]]:
    """Read the 8 carrier masks from the generated routing table."""
    text = tables_path.read_text(encoding="utf-8")
    match = re.search(r"table_routing\[160\]\s*=\s*\{(.*?)\};", text, re.S)
    if match is None:
        raise SystemExit(f"{tables_path}: the routing table is missing")
    values = [int(v) for v in re.findall(r"-?\d+", match.group(1))]
    if len(values) != 160:
        raise SystemExit(f"{tables_path}: expected 160 routing values")
    return [
        [values[algorithm * 20 + 16 + op] != 0 for op in range(4)]
        for algorithm in range(8)
    ]


def union_members(entry: dict[str, Any], part_suffix: str) -> list[tuple[str, int]]:
    """Every member any algorithm can select, with its operator index.

    The role rows include all four operators and the runtime filters them with the
    algorithm's carrier mask, so the table is algorithm independent.
    """
    if "members" in entry:
        return [(name.replace("{part}", part_suffix), 0) for name in entry["members"]]
    family = entry["family"]
    if family == "velocity_depth":
        return [(name.replace("{part}", part_suffix), 0) for name in VELOCITY_DEPTH]
    names: list[tuple[str, int]] = []
    for op in range(1, 5):
        templates = op_env_rates(op) if family == "op_env_rates" else FAMILIES[family]
        names.extend((template.format(op=op, part=part_suffix), op)
                     for template in templates)
    return names


def main() -> int:
    if len(sys.argv) != 6:
        print(
            "usage: gen_macro_targets.py macros.json params.json tables.c "
            "MacroTargets.h macro_target_ids.h",
            file=sys.stderr,
        )
        return 2
    spec = cast(dict[str, Any], json.loads(Path(sys.argv[1]).read_text(encoding="utf-8")))
    manifest = cast(
        dict[str, Any], json.loads(Path(sys.argv[2]).read_text(encoding="utf-8"))
    )
    carriers = load_carriers(Path(sys.argv[3]))
    params = cast(list[dict[str, Any]], manifest["params"])
    known = {str(param["name"]): int(param["id"]) for param in params}
    curves = {str(param["name"]): str(param["curve"]) for param in params}
    target_max = next(
        (int(param["max"]) for param in params if param["name"] == "macro1_target1"),
        -1,
    )
    sections = [str(section) for section in spec["sections"]]
    entries = cast(list[dict[str, Any]], spec["targets"])
    role_ids = {"all": 0, "carrier": 1, "modulator": 2}

    rows: list[tuple[str, str]] = []
    pool: list[tuple[int, int, bool]] = []
    offsets = [0]
    roles: list[int] = []
    parts: list[int] = []
    logs: list[bool] = []
    target_index: dict[tuple[str, int], int] = {}
    for entry in entries:
        for part, suffix in ((1, "0"), (2, "1")):
            names = union_members(entry, suffix)
            unknown = [name for name, _ in names if name not in known]
            if unknown:
                raise SystemExit(
                    f"{entry['name']} part {part}: unknown members {unknown[:4]}"
                )
            rows.append((f"part {part} {entry['name']}", str(entry["section"])))
            target_index[(str(entry["name"]), part)] = len(rows)
            for name, op in names:
                pool.append((known[name], op, curves[name] == "log"))
            offsets.append(len(pool))
            roles.append(role_ids.get(str(entry.get("role", "all")), 0))
            parts.append(part - 1)
            logs.append(any(curves[name] == "log" for name, _ in names))
    if len(rows) != target_max:
        raise SystemExit(
            f"the spec has {len(rows)} rows but the manifest target enum is "
            f"0..{target_max}"
        )
    if len(pool) > 65535:
        raise SystemExit("the member pool exceeds the uint16 offset table")

    # The engine applies the velocity source itself to the operator amplitude,
    # feedback and spectral envelope rates. The phase envelope rates in the same
    # row stay in the part-level layer because that envelope is per part. Operator
    # 1's surge and release are the legacy `attack` and `release` names.
    engine_rate_ids = sorted(
        {
            known[name]
            for entry in entries
            if entry["name"] == "op env rates"
            for _, suffix in ((1, "0"), (2, "1"))
            for name, _ in union_members(entry, suffix)
            if (name.startswith("op")
                and (".eg." in name or "_fbenv_" in name or "_spec_env_" in name))
            or name.startswith("attack.")
            or name.startswith("release.")
        }
    )

    # The operator LFOs are per voice, except the phase LFO, which holds one
    # state per part. The engine owns the per-voice rates; the phase LFO rate
    # stays in the part-level layer.
    lfo_rate_ids = sorted(
        {
            known[name]
            for entry in entries
            if entry["name"] == "op lfo rates"
            for _, suffix in ((1, "0"), (2, "1"))
            for name, _ in union_members(entry, suffix)
            if ("_lfo_rate" in name or "_trem_rate" in name)
            and "_phase_lfo_rate" not in name
        }
    )

    # The LFO depths are per voice for the operator LFOs; the phase LFO depth
    # stays part-level with its rate.
    lfo_depth_ids = sorted(
        {
            known[name]
            for entry in entries
            if entry["name"] == "op lfo depths"
            for _, suffix in ((1, "0"), (2, "1"))
            for name, _ in union_members(entry, suffix)
            if ("_lfo_depth" in name or "_trem_depth" in name)
            and "_phase_lfo_depth" not in name
        }
    )

    lines = [
        "/* GENERATED by plugin/tools/gen_macro_targets.py - do not edit. */",
        "#pragma once",
        "#include <algorithm>",
        "#include <array>",
        "#include <cstdint>",
        "",
        "namespace p1::macros {",
        "",
        "struct TargetInfo {",
        "    const char* name;",
        "    const char* section;",
        "};",
        "",
        f"inline constexpr int kTargetCount = {len(rows)};",
        f"inline constexpr std::array<TargetInfo, kTargetCount> kTargets = {{{{",
    ]
    for name, section in rows:
        lines.append(f"    {{{c_string(name)}, {c_string(section)}}},")
    lines += [
        "}};",
        "",
        f"inline constexpr int kSectionCount = {len(sections)};",
        "inline constexpr std::array<const char*, kSectionCount> kSections = {{",
    ]
    for section in sections:
        lines.append(f"    {c_string(section)},")
    lines += [
        "}};",
        "",
        "struct TargetMember {",
        "    int32_t id;",
        "    uint8_t op;",
        "    uint8_t log;",
        "};",
        "",
        f"inline constexpr std::array<TargetMember, {len(pool)}> kTargetMemberPool = {{{{",
    ]
    for member_id, op, log in pool:
        lines.append(f"    {{{member_id}, {op}, {1 if log else 0}}},")
    lines += [
        "}};",
        "",
        f"inline constexpr std::array<uint16_t, {len(rows) + 1}> "
        "kTargetMemberOffsets = {{",
        "    " + ", ".join(str(offset) for offset in offsets),
        "}};",
        "",
        "inline constexpr std::array<uint8_t, kTargetCount> kTargetRole = {{",
        "    " + ", ".join(str(role) for role in roles),
        "}};",
        "",
        "inline constexpr std::array<uint8_t, kTargetCount> kTargetPart = {{",
        "    " + ", ".join(str(part) for part in parts),
        "}};",
        "",
        "inline constexpr std::array<bool, kTargetCount> kTargetLog = {{",
        "    " + ", ".join("true" if log else "false" for log in logs),
        "}};",
        "",
        f"inline constexpr std::array<int32_t, {len(engine_rate_ids)}> "
        "kEnvRateMembers = {{",
        "    " + ", ".join(str(member) for member in engine_rate_ids),
        "}};",
        "",
        "inline bool isEnvRateMember(int32_t id)",
        "{",
        "    return std::binary_search(kEnvRateMembers.begin(),",
        "                              kEnvRateMembers.end(), id);",
        "}",
        "",
        f"inline constexpr std::array<int32_t, {len(lfo_rate_ids)}> "
        "kLfoRateMembers = {{",
        "    " + ", ".join(str(member) for member in lfo_rate_ids),
        "}};",
        "",
        "inline bool isLfoRateMember(int32_t id)",
        "{",
        "    return std::binary_search(kLfoRateMembers.begin(),",
        "                              kLfoRateMembers.end(), id);",
        "}",
        "",
        f"inline constexpr std::array<int32_t, {len(lfo_depth_ids)}> "
        "kLfoDepthMembers = {{",
        "    " + ", ".join(str(member) for member in lfo_depth_ids),
        "}};",
        "",
        "inline bool isLfoDepthMember(int32_t id)",
        "{",
        "    return std::binary_search(kLfoDepthMembers.begin(),",
        "                              kLfoDepthMembers.end(), id);",
        "}",
        "",
        "inline constexpr std::array<std::array<uint8_t, 4>, 8> kCarrierMasks = {{",
    ]
    for mask in carriers:
        lines.append("    {{" + ", ".join("1" if c else "0" for c in mask) + "}},")
    lines += [
        "}};",
        "",
        "} // namespace p1::macros",
        "",
    ]
    target = Path(sys.argv[4])
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text("\n".join(lines), encoding="utf-8")
    c_lines = [
        "/* GENERATED by plugin/tools/gen_macro_targets.py - do not edit. */",
        "#pragma once",
        f"#define P1_MACRO_TARGET_COUNT {len(rows)}",
        f"#define P1_MACRO_TARGET_P1_OP_LEVELS "
        f"{target_index[('op levels', 1)]}",
        f"#define P1_MACRO_TARGET_P2_OP_LEVELS "
        f"{target_index[('op levels', 2)]}",
        f"#define P1_MACRO_TARGET_P1_OP_FEEDBACKS "
        f"{target_index[('op feedbacks', 1)]}",
        f"#define P1_MACRO_TARGET_P2_OP_FEEDBACKS "
        f"{target_index[('op feedbacks', 2)]}",
        f"#define P1_MACRO_TARGET_P1_OP_MORPHS "
        f"{target_index[('op morphs', 1)]}",
        f"#define P1_MACRO_TARGET_P2_OP_MORPHS "
        f"{target_index[('op morphs', 2)]}",
        f"#define P1_MACRO_TARGET_P1_OP_ENV_RATES "
        f"{target_index[('op env rates', 1)]}",
        f"#define P1_MACRO_TARGET_P2_OP_ENV_RATES "
        f"{target_index[('op env rates', 2)]}",
        f"#define P1_MACRO_TARGET_P1_OP_LFO_RATES "
        f"{target_index[('op lfo rates', 1)]}",
        f"#define P1_MACRO_TARGET_P2_OP_LFO_RATES "
        f"{target_index[('op lfo rates', 2)]}",
        f"#define P1_MACRO_TARGET_P1_OP_LFO_DEPTHS "
        f"{target_index[('op lfo depths', 1)]}",
        f"#define P1_MACRO_TARGET_P2_OP_LFO_DEPTHS "
        f"{target_index[('op lfo depths', 2)]}",
        "",
    ]
    ids = Path(sys.argv[5])
    ids.parent.mkdir(parents=True, exist_ok=True)
    ids.write_text("\n".join(c_lines), encoding="utf-8")
    print(f"wrote {target} and {ids} ({len(rows)} targets)")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
