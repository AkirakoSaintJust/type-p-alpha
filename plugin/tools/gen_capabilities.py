"""Generate the plugin-only capability files from the enabled packages.

The base manifest is the firmware's `params.json`. Capability parameters append
after the base parameters, so every firmware id and the device part record stay
unchanged. The generated files are the merged manifest, the output DSP chain,
the page table, the C++ page registry, the migration table and the CMake source
list.

Usage:
    gen_capabilities.py --params params.json --dir capabilities
        --enabled capabilities/enabled.json --out generated
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path
from typing import TypedDict, cast


class ParamDoc(TypedDict):
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


class ParamsDoc(TypedDict, total=False):
    params: list[ParamDoc]
    base_param_count: int
    routing_base: int


class CapabilityDoc(TypedDict, total=False):
    id: str
    name: str
    version: str
    description: str
    design: str
    params: list[dict[str, object]]
    dsp: dict[str, object]
    ui: dict[str, object]
    migrations: list[dict[str, object]]


_ROUTING_PREFIXES = ("channel.", "low.", "high.", "level.", "delay.")
_NAME_RE = re.compile(r"^[A-Za-z_][A-Za-z0-9_]*$")
_PREFIX_RE = re.compile(r"^[a-z][a-z0-9_]*$")


def c_string(value: str) -> str:
    return '"' + value.replace("\\", "\\\\").replace('"', '\\"') + '"'


def identifier(value: str) -> str:
    name = re.sub(r"[^A-Za-z0-9_]", "_", value)
    if not name or name[0].isdigit():
        name = "cap_" + name
    return name


def load_params(path: Path) -> tuple[list[ParamDoc], int, int]:
    raw = cast(object, json.loads(path.read_text(encoding="utf-8")))
    data = cast(ParamsDoc, raw)
    params = data["params"]
    names = [p["name"] for p in params]
    if len(names) != len(set(names)):
        raise SystemExit(f"{path}: duplicate parameter names")
    routing = sum(name.startswith(_ROUTING_PREFIXES) for name in names)
    base_count = len(params)
    routing_base = base_count - routing
    return params, base_count, routing_base


def load_enabled(path: Path) -> list[str]:
    raw = cast(object, json.loads(path.read_text(encoding="utf-8")))
    data = cast(dict[str, object], raw)
    enabled = cast(list[str], data.get("enabled", []))
    if not all(isinstance(item, str) for item in enabled):
        raise SystemExit(f"{path}: 'enabled' must be a list of package ids")
    return enabled


def load_capability(cap_dir: Path, expected_id: str) -> CapabilityDoc:
    path = cap_dir / "capability.json"
    raw = cast(object, json.loads(path.read_text(encoding="utf-8")))
    data = cast(CapabilityDoc, raw)
    if data.get("id") != expected_id:
        raise SystemExit(f"{path}: id must be '{expected_id}'")
    design = data.get("design")
    if design is not None and not (cap_dir / cast(str, design)).is_file():
        raise SystemExit(f"{path}: design note {design} does not exist")
    if not isinstance(data.get("params"), list):
        raise SystemExit(f"{path}: 'params' must be a list")
    for param in data["params"]:
        for key in ("name", "min", "max", "default", "curve", "display"):
            if key not in param:
                raise SystemExit(f"{path}: a parameter is missing '{key}'")
        if not _NAME_RE.match(cast(str, param["name"])):
            raise SystemExit(f"{path}: bad parameter name {param['name']!r}")
    if "dsp" in data:
        dsp = cast(dict[str, object], data["dsp"])
        source = cap_dir / cast(str, dsp.get("source", "dsp.c"))
        if not source.is_file():
            raise SystemExit(f"{path}: DSP source {source} does not exist")
        if not isinstance(dsp.get("params"), list):
            raise SystemExit(f"{path}: 'dsp.params' must be a list")
    ui = data.get("ui")
    if ui is not None:
        if not isinstance(ui, dict) or not isinstance(ui.get("pages"), list):
            raise SystemExit(f"{path}: 'ui.pages' must be a list")
        for page in cast(list[dict[str, object]], ui["pages"]):
            if not isinstance(page.get("title"), str):
                raise SystemExit(f"{path}: every page needs a title")
            if page.get("kind") not in ("schema", "cpp"):
                raise SystemExit(f"{path}: page kind must be schema or cpp")
            if page["kind"] == "cpp" and not _NAME_RE.match(
                cast(str, page.get("factory", ""))
            ):
                raise SystemExit(f"{path}: a C++ page needs a factory name")
            if page["kind"] == "cpp":
                for name in ("ui.h", "ui.cpp"):
                    if not (cap_dir / name).is_file():
                        raise SystemExit(f"{path}: a C++ page needs {name}")
            if page["kind"] == "schema":
                controls = page.get("controls")
                if not isinstance(controls, list):
                    raise SystemExit(f"{path}: a schema page needs controls")
                for control in cast(list[dict[str, object]], controls):
                    if control.get("kind") not in ("knob", "choice"):
                        raise SystemExit(f"{path}: control kind must be knob or choice")
                    if not isinstance(control.get("param"), str):
                        raise SystemExit(f"{path}: a control needs a param")
                    if control["kind"] == "choice" and not isinstance(
                        control.get("choices"), list
                    ):
                        raise SystemExit(f"{path}: a choice needs choices")
    for migration in cast(list[dict[str, object]], data.get("migrations", [])):
        for key in ("param", "from", "to"):
            if key not in migration:
                raise SystemExit(f"{path}: a migration is missing '{key}'")
    return data


def merge_params(
    base: list[ParamDoc], capabilities: list[tuple[str, CapabilityDoc]], start: int
) -> tuple[list[ParamDoc], list[tuple[str, int, int]]]:
    merged = list(base)
    names = {p["name"] for p in merged}
    ranges: list[tuple[str, int, int]] = []
    next_id = start
    for cap_id, cap in capabilities:
        cap_params = cast(list[dict[str, object]], cap["params"])
        first = next_id
        for param in cap_params:
            name = cast(str, param["name"])
            if name in names:
                raise SystemExit(f"{cap_id}: parameter {name!r} already exists")
            names.add(name)
            merged.append(
                {
                    "id": next_id,
                    "name": name,
                    "target": name,
                    "min": int(cast(int, param["min"])),
                    "max": int(cast(int, param["max"])),
                    "default": int(cast(int, param["default"])),
                    "curve": str(param.get("curve", "linear")),
                    "unit": str(param.get("unit", "")),
                    "rate": str(param.get("rate", "block")),
                    "modulatable": bool(param.get("modulatable", False)),
                    "display": str(param.get("display", "raw")),
                }
            )
            next_id += 1
        ranges.append((cap_id, first - start, next_id - first))
    return merged, ranges


def page_header(
    capabilities: list[tuple[str, CapabilityDoc]],
) -> str:
    lines = [
        "/* GENERATED by plugin/tools/gen_capabilities.py - do not edit. */",
        "#pragma once",
        "",
        "struct P1CapabilityControl {",
        "    int kind;",
        "    const char* param;",
        "    const char* label;",
        "    const char* const* choices;",
        "    int n_choices;",
        "};",
        "",
        "struct P1CapabilityPage {",
        "    const char* title;",
        "    const char* note;",
        "    int kind;",
        "    const P1CapabilityControl* controls;",
        "    int n_controls;",
        "};",
        "",
    ]
    pages: list[str] = []
    for cap_id, cap in capabilities:
        ui = cast(dict[str, object] | None, cap.get("ui"))
        if ui is None:
            continue
        for page_index, page in enumerate(cast(list[dict[str, object]], ui["pages"])):
            title = cast(str, page["title"])
            note = c_string(cast(str, page.get("note", "")))
            kind = 0 if page["kind"] == "schema" else 1
            if kind == 1:
                pages.append("    {" + c_string(title) + f", {note}, 1, nullptr, 0}},")
                continue
            controls = cast(list[dict[str, object]], page["controls"])
            page_prefix = f"p1_cap_{identifier(cap_id)}_{page_index}"
            control_name = f"{page_prefix}_controls"
            entries: list[str] = []
            for control_index, control in enumerate(controls):
                ckind = 0 if control["kind"] == "knob" else 1
                param = c_string(cast(str, control["param"]))
                label = c_string(cast(str, control.get("label", control["param"])))
                choices = cast(list[str], control.get("choices", []))
                choices_name = "nullptr"
                n_choices = 0
                if ckind == 1:
                    choices_name = f"{page_prefix}_{control_index}_choices"
                    lines.append(
                        f"inline constexpr const char* {choices_name}[] = {{"
                        + ", ".join(c_string(choice) for choice in choices)
                        + "};"
                    )
                    n_choices = len(choices)
                entries.append(
                    f"    {{{ckind}, {param}, {label}, {choices_name}, {n_choices}}},"
                )
            lines.append(f"inline constexpr P1CapabilityControl {control_name}[] = {{")
            lines.extend(entries)
            lines.append("};")
            pages.append(
                "    {"
                + c_string(title)
                + f", {note}, 0, {control_name}, {len(entries)}}},"
            )
    lines.append("inline constexpr P1CapabilityPage kP1CapabilityPages[] = {")
    lines.extend(pages)
    lines.append("};")
    lines.append(f"inline constexpr int kP1CapabilityPageCount = {len(pages)};")
    lines.append("")
    return "\n".join(lines)


def registry_header() -> str:
    return "\n".join(
        [
            "/* GENERATED by plugin/tools/gen_capabilities.py - do not edit. */",
            "#pragma once",
            "#include <memory>",
            '#include "Pages.h"',
            "",
            "namespace p1::ui {",
            "std::unique_ptr<PageBase> makeCapabilityPage(int index,",
            "                                              PluginEditor& editor);",
            "} // namespace p1::ui",
            "",
        ]
    )


def registry_source(
    capabilities: list[tuple[str, CapabilityDoc]],
) -> str:
    includes: list[str] = []
    cases: list[str] = []
    page_index = 0
    for cap_id, cap in capabilities:
        ui = cast(dict[str, object] | None, cap.get("ui"))
        if ui is None:
            continue
        cpp_pages = [
            page
            for page in cast(list[dict[str, object]], ui["pages"])
            if page["kind"] == "cpp"
        ]
        if cpp_pages:
            includes.append(f'#include "{cap_id}/ui.h"')
        for page in cast(list[dict[str, object]], ui["pages"]):
            if page["kind"] == "cpp":
                cases.append(
                    f"    case {page_index}: return {page['factory']}(editor);"
                )
            page_index += 1
    lines = [
        "/* GENERATED by plugin/tools/gen_capabilities.py - do not edit. */",
        '#include "CapabilityRegistry.h"',
        *includes,
        "",
        "namespace p1::ui {",
        "std::unique_ptr<PageBase> makeCapabilityPage(int index,",
        "                                              PluginEditor& editor)",
        "{",
        "    switch (index) {",
        *cases,
        "    default: return nullptr;",
        "    }",
        "}",
        "} // namespace p1::ui",
        "",
    ]
    return "\n".join(lines)


def dsp_header(base_count: int, total: int) -> str:
    return "\n".join(
        [
            "/* GENERATED by plugin/tools/gen_capabilities.py - do not edit. */",
            "#pragma once",
            "#include <stdint.h>",
            "",
            f"#define P1_BASE_PARAM_COUNT {base_count}",
            f"#define P1_CAPABILITY_PARAM_COUNT {total}",
            "",
            "#ifdef __cplusplus",
            'extern "C" {',
            "#endif",
            "void p1_capabilities_reset(void);",
            "int p1_capabilities_set_param(uint32_t local_id, int32_t value);",
            "void p1_capabilities_process(int32_t* out, uint32_t frames);",
            "#ifdef __cplusplus",
            "}",
            "#endif",
            "",
        ]
    )


def dsp_source(
    capabilities: list[tuple[str, CapabilityDoc]],
    ranges: list[tuple[str, int, int]],
) -> str:
    includes: list[str] = []
    resets: list[str] = []
    dispatch: list[str] = []
    processes: list[str] = []
    for cap_id, cap in capabilities:
        dsp = cast(dict[str, object] | None, cap.get("dsp"))
        if dsp is None:
            continue
        prefix = identifier(cast(str, dsp.get("prefix", cap_id)))
        includes.append(f'#include "{cap_id}/{dsp.get("source", "dsp.c")}"')
        resets.append(f"    {prefix}_init();")
        first, count = next(
            (start, length) for name, start, length in ranges if name == cap_id
        )
        if count:
            dispatch.append(f"    if (local_id < {first + count}u) {{")
            dispatch.append(f"        {prefix}_set_param(local_id - {first}u, value);")
            dispatch.append("        return 1;")
            dispatch.append("    }")
        processes.append(f"    {prefix}_process(out, frames);")
    lines = [
        "/* GENERATED by plugin/tools/gen_capabilities.py - do not edit. */",
        '#include "CapabilityDSP.h"',
        *includes,
        "",
        "void p1_capabilities_reset(void)",
        "{",
        *(resets or ["    /* no capability DSP */"]),
        "}",
        "",
        "int p1_capabilities_set_param(uint32_t local_id, int32_t value)",
        "{",
        "    (void)local_id;",
        "    (void)value;",
        *(dispatch or ["    return 0;"]),
        *(["    return 0;"] if dispatch else []),
        "}",
        "",
        "void p1_capabilities_process(int32_t* out, uint32_t frames)",
        "{",
        "    (void)out;",
        "    (void)frames;",
        *(processes or ["    /* no capability DSP */"]),
        "}",
        "",
    ]
    return "\n".join(lines)


def migration_header(
    capabilities: list[tuple[str, CapabilityDoc]], base: list[ParamDoc]
) -> str:
    by_name = {p["name"]: p["id"] for p in base}
    rows: list[str] = []
    for cap_id, cap in capabilities:
        for migration in cast(list[dict[str, object]], cap.get("migrations", [])):
            param = cast(str, migration["param"])
            if param not in by_name:
                raise SystemExit(
                    f"{cap_id}: migration targets unknown base param {param!r}"
                )
            rows.append(
                "    {"
                + c_string(param)
                + f", {int(cast(int, migration['from']))}, "
                + f"{int(cast(int, migration['to']))}}},"
            )
    count = len(rows)
    if not rows:
        rows = ["    {nullptr, 0, 0},"]
    return "\n".join(
        [
            "/* GENERATED by plugin/tools/gen_capabilities.py - do not edit. */",
            "#pragma once",
            "#include <cstdint>",
            "",
            "struct P1CapabilityMigration {",
            "    const char* param;",
            "    int32_t from;",
            "    int32_t to;",
            "};",
            "",
            "inline constexpr P1CapabilityMigration kP1CapabilityMigrations[] = {",
            *rows,
            "};",
            f"inline constexpr int kP1CapabilityMigrationCount = {count};",
            "",
        ]
    )


def sources_cmake(
    capabilities: list[tuple[str, CapabilityDoc]], cap_root: Path, out: Path
) -> str:
    ui_sources: list[str] = []
    include_dirs = [cap_root.as_posix()]
    for cap_id, cap in capabilities:
        ui = cast(dict[str, object] | None, cap.get("ui"))
        if ui is None:
            continue
        pages = cast(list[dict[str, object]], ui["pages"])
        if any(page["kind"] == "cpp" for page in pages):
            ui_sources.append((cap_root / cap_id / "ui.cpp").as_posix())
    lines = [
        "# GENERATED by plugin/tools/gen_capabilities.py - do not edit.",
        "set(P1_CAPABILITY_UI_SOURCES",
        *[f'    "{source}"' for source in ui_sources],
        ")",
        "set(P1_CAPABILITY_INCLUDE_DIRS",
        *[f'    "{path}"' for path in include_dirs],
        ")",
        f'set(P1_CAPABILITY_DSP_SOURCE "{(out / "CapabilityDSP.c").as_posix()}")',
        "",
    ]
    return "\n".join(lines)


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="gen_capabilities", description=__doc__)
    parser.add_argument("--params", required=True, type=Path)
    parser.add_argument("--dir", required=True, type=Path)
    parser.add_argument("--enabled", required=True, type=Path)
    parser.add_argument("--out", required=True, type=Path)
    args = parser.parse_args(argv)

    base, base_count, routing_base = load_params(args.params)
    enabled = load_enabled(args.enabled)
    capabilities: list[tuple[str, CapabilityDoc]] = []
    for cap_id in enabled:
        cap_dir = args.dir / cap_id
        if not cap_dir.is_dir():
            raise SystemExit(f"{args.enabled}: package {cap_id!r} does not exist")
        capabilities.append((cap_id, load_capability(cap_dir, cap_id)))
    merged, ranges = merge_params(base, capabilities, base_count)

    out = args.out
    out.mkdir(parents=True, exist_ok=True)
    params_doc: ParamsDoc = {
        "params": merged,
        "base_param_count": base_count,
        "routing_base": routing_base,
    }
    (out / "plugin-params.json").write_text(
        json.dumps(params_doc, indent=2) + "\n", encoding="utf-8"
    )
    (out / "CapabilityDSP.h").write_text(
        dsp_header(base_count, len(merged) - base_count), encoding="utf-8"
    )
    (out / "CapabilityDSP.c").write_text(
        dsp_source(capabilities, ranges), encoding="utf-8"
    )
    (out / "CapabilityPages.h").write_text(page_header(capabilities), encoding="utf-8")
    (out / "CapabilityRegistry.h").write_text(registry_header(), encoding="utf-8")
    (out / "CapabilityRegistry.cpp").write_text(
        registry_source(capabilities), encoding="utf-8"
    )
    (out / "CapabilityMigrations.h").write_text(
        migration_header(capabilities, base), encoding="utf-8"
    )
    (out / "CapabilitySources.cmake").write_text(
        sources_cmake(capabilities, args.dir, out), encoding="utf-8"
    )
    print(
        f"gen_capabilities: {len(capabilities)} package(s), "
        f"{len(merged) - base_count} param(s)"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
