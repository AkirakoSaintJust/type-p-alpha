#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
# Copyright (C) 2026 P1Onken
"""Generate the exhaustive code reference under docs/code/reference.

The hand-written chapters live next to the generated reference and are not
touched. This script reads the tracked C, C++, Python, CMake and JSON sources
and writes the file, type, function, symbol, state-field, parameter and node
indexes. It needs the optional `tree-sitter`, `tree-sitter-c`,
`tree-sitter-cpp` and `tree-sitter-python` packages.

Run from the repository root:

    python tools/gen_code_docs.py
"""

from __future__ import annotations

import argparse
import json
import os
import re
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path
from typing import Iterable

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "docs" / "code" / "reference"
EXCLUDE = re.compile(
    r"\\(build|_deps|\.git|publish|third-party-licenses|__pycache__|\.dev)(\\|$)"
)
SOURCE_EXT = {".c", ".h", ".hpp", ".cpp", ".py", ".ps1", ".sh", ".cmake"}
TEXT_EXT = SOURCE_EXT | {".json", ".md", ".txt", ".yml", ".yaml"}

try:
    from tree_sitter import Language, Parser
    import tree_sitter_c as tsc
    import tree_sitter_cpp as tscpp
    import tree_sitter_python as tspy
except Exception as exc:  # pragma: no cover - the docs are checked in
    raise SystemExit(
        "gen_code_docs: install the optional parsers with\n"
        "  python -m pip install tree-sitter tree-sitter-c tree-sitter-cpp "
        "tree-sitter-python\n"
        f"({exc})"
    )

CPP = Language(tscpp.language())
C = Language(tsc.language())
PY = Language(tspy.language())


@dataclass
class Function:
    path: str
    line: int
    end_line: int
    name: str
    signature: str
    description: str = ""
    parameters: list[str] = field(default_factory=list)
    scope: str = ""
    kind: str = "function"


@dataclass
class Type:
    path: str
    line: int
    end_line: int
    name: str
    kind: str
    description: str = ""
    fields: list[str] = field(default_factory=list)
    methods: list[str] = field(default_factory=list)


@dataclass
class Symbol:
    path: str
    line: int
    name: str
    kind: str
    value: str = ""
    description: str = ""


def text(node, src: bytes) -> str:
    return src[node.start_byte : node.end_byte].decode("utf-8", "replace")


def name_of(node, src: bytes) -> str:
    """The last declarator name, resolving qualified names and pointers."""
    if node is None:
        return ""
    t = node.type
    if t in (
        "identifier",
        "field_identifier",
        "type_identifier",
        "namespace_identifier",
        "destructor_name",
        "operator_name",
    ):
        return text(node, src)
    if t == "qualified_identifier":
        return name_of(node.child_by_field_name("name"), src)
    for field_name in ("declarator", "name"):
        child = node.child_by_field_name(field_name)
        if child is not None:
            value = name_of(child, src)
            if value:
                return value
    for child in reversed(node.children):
        value = name_of(child, src)
        if value:
            return value
    return ""


def clean_comment(lines: list[str], start_line: int) -> str:
    """The comment block immediately above `start_line`, collapsed to prose."""
    i = start_line - 2
    if i < 0:
        return ""
    collected: list[str] = []
    # A block comment ends on the line above the declaration.
    if lines[i].strip().endswith("*/"):
        while i >= 0:
            collected.append(lines[i])
            if "/*" in lines[i]:
                break
            i -= 1
    elif lines[i].strip().startswith("//"):
        while i >= 0 and lines[i].strip().startswith("//"):
            collected.append(lines[i])
            i -= 1
    else:
        return ""
    collected.reverse()
    out: list[str] = []
    for line in collected:
        s = line.strip()
        s = re.sub(r"^/\*+|\*/$", "", s).strip()
        s = re.sub(r"^\*+\s?", "", s).strip()
        s = re.sub(r"^//+\s?", "", s).strip()
        if s:
            out.append(s)
    return " ".join(out)


def clean_signature(value: str) -> str:
    value = re.sub(r"\s+", " ", value).strip()
    value = re.sub(r"\s*\{\s*$", "", value).strip()
    return value


def parser_for(path: Path) -> Parser:
    if path.suffix.lower() == ".py":
        return Parser(PY)
    if path.suffix.lower() in (".cpp", ".hpp", ".h"):
        return Parser(CPP)
    return Parser(C)


def is_global_scope(node) -> bool:
    parent = node.parent
    while parent is not None:
        if parent.type in (
            "translation_unit",
            "namespace_definition",
            "linkage_specification",
        ):
            return True
        if parent.type in (
            "class_specifier",
            "struct_specifier",
            "union_specifier",
            "declaration_list",
            "field_declaration_list",
            "function_definition",
            "compound_statement",
        ):
            return False
        parent = parent.parent
    return False


def class_scope(node, src: bytes) -> str:
    parent = node.parent
    while parent is not None:
        if parent.type in ("class_specifier", "struct_specifier", "union_specifier"):
            return name_of(parent.child_by_field_name("name"), src)
        parent = parent.parent
    return ""


def collect_c_family(path: Path) -> tuple[list[Function], list[Type], list[Symbol]]:
    raw = path.read_text(encoding="utf-8", errors="replace")
    lines = raw.splitlines()
    src = raw.encode("utf-8")
    tree = parser_for(path).parse(src)
    rel = str(path.relative_to(ROOT)).replace("\\", "/")
    functions: list[Function] = []
    types: list[Type] = []
    symbols: list[Symbol] = []

    seen_functions: set[tuple[str, int, str]] = set()
    seen_types: set[tuple[str, int, str]] = set()
    seen_symbols: set[tuple[str, int, str]] = set()
    control_names = {
        "if", "for", "while", "switch", "return", "sizeof", "static_assert",
        "alignas", "decltype", "catch", "throw", "else", "do", "$",
    }

    def add_function(node, kind: str = "function") -> None:
        declarator = node.child_by_field_name("declarator")
        name = name_of(declarator, src)
        if not name or name in control_names or "$" in name:
            return
        body = node.child_by_field_name("body")
        first = node.start_point[0] + 1
        last = node.end_point[0] + 1
        if body is not None:
            signature = clean_signature(text(node, src)[: body.start_byte - node.start_byte])
        else:
            signature = clean_signature(text(node, src).rstrip(";"))
        key = (rel, first, name)
        if key in seen_functions:
            return
        seen_functions.add(key)
        params: list[str] = []
        if declarator is not None:
            for child in declarator.children:
                if child.type == "parameter_list":
                    for param in child.children:
                        if param.type in ("parameter_declaration", "optional_parameter_declaration"):
                            params.append(clean_signature(text(param, src)))
        scope = class_scope(node, src)
        description = clean_comment(lines, first)
        functions.append(
            Function(
                path=rel,
                line=first,
                end_line=last,
                name=name,
                signature=signature,
                description=description,
                parameters=params,
                scope=scope,
                kind=kind,
            )
        )

    def add_type(node) -> None:
        kind = {
            "class_specifier": "class",
            "struct_specifier": "struct",
            "union_specifier": "union",
            "enum_specifier": "enum",
        }[node.type]
        name = name_of(node.child_by_field_name("name"), src)
        if not name:
            return
        first = node.start_point[0] + 1
        key = (rel, first, name)
        if key in seen_types:
            return
        seen_types.add(key)
        body = node.child_by_field_name("body")
        kind_label = kind if body is not None else f"{kind} forward declaration"
        fields: list[str] = []
        methods: list[str] = []
        if body is not None:
            for child in body.children:
                if child.type == "field_declaration":
                    fields.append(clean_signature(text(child, src)))
                elif child.type == "enumerator":
                    fields.append(clean_signature(text(child, src)))
                elif child.type in ("function_definition", "function_declarator"):
                    methods.append(clean_signature(text(child, src)))
        types.append(
            Type(
                path=rel,
                line=first,
                end_line=node.end_point[0] + 1,
                name=name,
                kind=kind_label,
                description=clean_comment(lines, first),
                fields=fields,
                methods=methods,
            )
        )

    def walk(node) -> None:
        t = node.type
        if t == "function_definition":
            add_function(node)
        elif t in ("class_specifier", "struct_specifier", "union_specifier", "enum_specifier"):
            add_type(node)
        elif t == "type_definition":
            alias = name_of(node.child_by_field_name("declarator"), src)
            underlying = node.child_by_field_name("type")
            if alias:
                first = node.start_point[0] + 1
                key = (rel, first, alias)
                if key not in seen_types:
                    seen_types.add(key)
                    types.append(
                        Type(
                            path=rel,
                            line=first,
                            end_line=node.end_point[0] + 1,
                            name=alias,
                            kind="typedef",
                            description=clean_comment(lines, first),
                            fields=[clean_signature(text(underlying, src))]
                            if underlying is not None
                            else [],
                        )
                    )
        elif t in ("preproc_def", "preproc_function_def"):
            name_node = node.child_by_field_name("name")
            name = text(name_node, src) if name_node is not None else ""
            if name:
                first = node.start_point[0] + 1
                key = (rel, first, name)
                if key not in seen_symbols:
                    seen_symbols.add(key)
                    symbols.append(
                        Symbol(
                            path=rel,
                            line=first,
                            name=name,
                            kind="macro",
                            value=clean_signature(text(node, src)),
                            description="",
                        )
                    )
        elif t == "declaration" and is_global_scope(node):
            declarator = node.child_by_field_name("declarator")
            is_function = declarator is not None and (
                declarator.type == "function_declarator"
                or "function_declarator" in {c.type for c in declarator.children}
            )
            if is_function:
                add_function(node, "declaration")
            else:
                for child in node.children:
                    if child.type == "init_declarator":
                        name = name_of(child.child_by_field_name("declarator"), src)
                    elif child.type in ("identifier", "pointer_declarator", "array_declarator"):
                        name = name_of(child, src)
                    else:
                        continue
                    if not name:
                        continue
                    first = node.start_point[0] + 1
                    key = (rel, first, name)
                    if key in seen_symbols:
                        continue
                    seen_symbols.add(key)
                    symbols.append(
                        Symbol(
                            path=rel,
                            line=first,
                            name=name,
                            kind="global",
                            value=clean_signature(text(node, src).rstrip(";")),
                            description=clean_comment(lines, first),
                        )
                    )
        elif t == "field_declaration":
            declarator = node.child_by_field_name("declarator")
            is_function = declarator is not None and (
                declarator.type == "function_declarator"
                or "function_declarator" in {c.type for c in declarator.children}
            )
            if is_function:
                add_function(node, "declaration")
        elif t in ("alias_declaration", "using_declaration"):
            name = name_of(node, src)
            if name:
                first = node.start_point[0] + 1
                key = (rel, first, name)
                if key not in seen_symbols:
                    seen_symbols.add(key)
                    symbols.append(
                        Symbol(
                            path=rel,
                            line=first,
                            name=name,
                            kind="alias",
                            value=clean_signature(text(node, src).rstrip(";")),
                            description=clean_comment(lines, first),
                        )
                    )
        for child in node.children:
            walk(child)

    walk(tree.root_node)
    return functions, types, symbols


def collect_python(path: Path) -> tuple[list[Function], list[Type], list[Symbol]]:
    raw = path.read_text(encoding="utf-8", errors="replace")
    lines = raw.splitlines()
    src = raw.encode("utf-8")
    tree = parser_for(path).parse(src)
    rel = str(path.relative_to(ROOT)).replace("\\", "/")
    functions: list[Function] = []
    types: list[Type] = []
    symbols: list[Symbol] = []

    def docstring(node) -> str:
        body = node.child_by_field_name("body")
        if body is None:
            return ""
        for child in body.children:
            if child.type == "expression_statement" and child.children:
                first = child.children[0]
                if first.type == "string":
                    value = text(first, src).strip("\"'")
                    return value.splitlines()[0].strip()
        return ""

    def walk(node, scope: str = "") -> None:
        if node.type == "function_definition":
            name = name_of(node.child_by_field_name("name"), src)
            if name:
                params: list[str] = []
                parameters = node.child_by_field_name("parameters")
                if parameters is not None:
                    for child in parameters.children:
                        if child.type in ("identifier", "typed_parameter", "default_parameter"):
                            params.append(clean_signature(text(child, src)))
                functions.append(
                    Function(
                        path=rel,
                        line=node.start_point[0] + 1,
                        end_line=node.end_point[0] + 1,
                        name=name,
                        signature=f"def {name}({', '.join(params)})",
                        description=docstring(node) or clean_comment(lines, node.start_point[0] + 1),
                        parameters=params,
                        scope=scope,
                        kind="function",
                    )
                )
        elif node.type == "class_definition":
            name = name_of(node.child_by_field_name("name"), src)
            if name:
                types.append(
                    Type(
                        path=rel,
                        line=node.start_point[0] + 1,
                        end_line=node.end_point[0] + 1,
                        name=name,
                        kind="class",
                        description=docstring(node) or clean_comment(lines, node.start_point[0] + 1),
                    )
                )
                for child in node.children:
                    walk(child, name)
                return
        elif node.type == "expression_statement" and scope == "":
            for child in node.children:
                if child.type == "assignment":
                    left = child.child_by_field_name("left")
                    if left is not None and left.type == "identifier":
                        symbols.append(
                            Symbol(
                                path=rel,
                                line=node.start_point[0] + 1,
                                name=text(left, src),
                                kind="module constant",
                                value=clean_signature(text(node, src).rstrip()),
                                description=clean_comment(lines, node.start_point[0] + 1),
                            )
                        )
        for child in node.children:
            walk(child, scope)

    walk(tree.root_node)
    return functions, types, symbols


def source_files(include_dev: bool) -> list[Path]:
    files: list[Path] = []
    for base, dirs, names in os.walk(ROOT):
        dirs[:] = [d for d in dirs if not EXCLUDE.search(os.path.join(base, d) + "\\")]
        for name in names:
            path = Path(base, name)
            if path.suffix.lower() in SOURCE_EXT or name == "CMakeLists.txt":
                files.append(path)
    return sorted(files)


def md_escape(value: str) -> str:
    return value.replace("|", "\\|").replace("\n", " ")


def file_summary(path: Path) -> str:
    """The first prose sentence of the file's leading comment or heading."""
    try:
        raw = path.read_text(encoding="utf-8", errors="replace")
    except Exception:
        return ""
    lines = raw.splitlines()
    if path.suffix.lower() == ".md":
        for line in lines:
            if line.startswith("# "):
                return line[2:].strip()
        return ""
    chunks: list[str] = []
    inside = False
    saw_code = False
    for line in lines[:60]:
        stripped = line.strip()
        if not stripped:
            continue
        if not inside:
            if stripped.startswith("/*"):
                if saw_code:
                    return ""
                body = re.sub(r"^/\*+", "", stripped)
                if "*/" in body:
                    chunks.append(re.sub(r"\*/.*$", "", body))
                else:
                    chunks.append(body)
                    inside = True
            elif stripped.startswith("//"):
                if saw_code:
                    return ""
                chunks.append(re.sub(r"^//+", "", stripped))
            elif stripped.startswith(("SPDX", "Copyright")):
                continue
            else:
                saw_code = True
        else:
            if "*/" in stripped:
                chunks.append(re.sub(r"^\*+", "", re.sub(r"\*/.*$", "", stripped)))
                inside = False
            else:
                chunks.append(re.sub(r"^\*+", "", stripped))
    if not chunks:
        return ""
    cleaned = [chunk.strip() for chunk in chunks if chunk.strip()]
    if not cleaned:
        return ""
    text = " ".join(cleaned)
    text = re.sub(r"SPDX-License-Identifier:\s*\S+", " ", text)
    text = re.sub(r"Copyright\s*\(C\)[^.]*\.?", " ", text)
    text = re.sub(r"GENERATED by [^,]*,\s*do not edit[^.]*\.", " ", text, flags=re.I)
    text = re.sub(r"GENERATED by [^.]*\.", " ", text, flags=re.I)
    text = re.sub(r"\s+", " ", text).strip()
    for sentence in re.split(r"(?<=\.)\s+", text):
        sentence = sentence.strip().rstrip(".")
        if len(sentence.split()) >= 3 and sentence[:1].isupper():
            return sentence
    return ""


FILE_ROLES = {
    "engine/gen/dsp.h": "generated state struct, parameter table, slot table and engine API",
    "engine/gen/type_p_alpha_dsp.c": "generated parameter table, init table, note allocation and render code",
    "engine/gen/tables.c": "generated algorithm routing weights",
    "engine/gen/tables.h": "generated routing table declaration",
    "engine/gen/math_tables.h": "generated fixed-point tables for the 48 kHz core",
    "engine/gen/wave_harmonics.h": "generated harmonic magnitude table for the 14 waves",
    "engine/gen/wavetable.h": "generated 14-waveform oscillator union, additive bank included",
    "engine/hal/p1_math.h": "fixed-point helpers, level gains, rate tables and wavetable mip selection",
    "engine/hal/spectral.h": "additive spectral bank generator, morph reader and crossfade",
    "engine/runtime/presets.c": "preset record layout, CRC and bank pack functions",
    "engine/runtime/presets.h": "preset record declarations and sizes",
    "engine/runtime/synth_midi.c": "MIDI handler with sustain, channel routing and panic",
    "engine/runtime/synth_midi.h": "MIDI handler interface, context and callback table",
    "engine/runtime/macro_apply.h": "per-voice macro offsets for the velocity source",
    "plugin/engine/Engine.hpp": "C++ seam over the generated state, phase-map snapshot and render call",
    "plugin/engine/Engine.cpp": "engine seam implementation, parameter writes and phase-map snapshot copy",
    "plugin/engine/MidiBridge.hpp": "MIDI bridge interface between the host queue and the engine",
    "plugin/engine/MidiBridge.cpp": "MIDI bridge implementation and the engine callback table",
    "plugin/engine/RateBridge.hpp": "output-only 128-tap polyphase resampler from 48 kHz to the host rate",
    "plugin/engine/RateBridge.cpp": "resampler kernel, ring, pull path and latency",
    "plugin/engine/PhaseMapLayout.h": "phase-map pool layout shared by the engine snapshot and tests",
    "plugin/juce/PluginProcessor.h": "JUCE processor declaration, scope rings and snapshot buffers",
    "plugin/juce/PluginProcessor.cpp": "JUCE processor, block path, MIDI scheduling, parameter sync and library save",
    "plugin/juce/PluginEditor.h": "JUCE editor declaration and page state",
    "plugin/juce/PluginEditor.cpp": "JUCE editor, header, toolbar, dialogs and page switching",
    "plugin/juce/Pages.h": "shared page base, card frame and page factory declarations",
    "plugin/juce/Pages.cpp": "patch settings page, part cards and voice mode",
    "plugin/juce/PagesFxMod.cpp": "part filter, envelopes, follow and ring-modulation cards",
    "plugin/juce/PagesMacros.cpp": "macro cards, target picker and range controls",
    "plugin/juce/PagesOperators.cpp": "operator oscillator, envelope and LFO columns",
    "plugin/juce/PagesPhase.cpp": "phase-map page, transfer-curve view and phase controls",
    "plugin/juce/PagesSpectral.cpp": "spectral a/b editors, spectral modulation page and displays",
    "plugin/juce/ChunkNoteSet.h": "notes started in the current control chunk, keyed by channel and pitch",
    "plugin/juce/MacroLaw.h": "macro source and destination laws and the target table view",
    "plugin/juce/VolumeLaw.h": "master volume fader law and dB text",
    "plugin/ui/ParamIDs.h": "generated parameter id helpers",
    "plugin/ui/Widgets.h": "panel widget declarations: cards, knobs, toggles, bars and scopes",
    "plugin/ui/Widgets.cpp": "panel widget painting, parameter binding and edit gestures",
    "plugin/ui/Theme.h": "font, colour and look-and-feel declarations",
    "plugin/ui/Theme.cpp": "font loading, palette and the panel look and feel",
    "plugin/ui/SpectrumAnalysis.h": "wavetable harmonic analysis interface",
    "plugin/ui/SpectrumAnalysis.cpp": "wavetable harmonic analysis for the spectral editor",
    "plugin/preset/PresetStore.hpp": "in-memory part and multi library with migrations and merge",
    "plugin/preset/PresetStore.cpp": "library store, record migrations, merge and file I/O",
    "plugin/preset/presets_host.c": "host stubs for the preset record layer",
    "plugin/preset/CutoffLaw.h": "cutoff position law and the old-to-new conversion",
    "plugin/preset/part_defaults.h": "factory defaults for one part",
}


def write_files(paths: list[Path], out: Path) -> None:
    lines = [
        "# File index",
        "",
        "Every tracked source and build file, with its line count and role.",
        "",
        "| File | Lines | Language | Role |",
        "| --- | ---: | --- | --- |",
    ]
    roles = [
        ("engine/hal/", "fixed-point DSP, tables and spectral bank"),
        ("engine/runtime/", "preset records and the MIDI handler"),
        ("engine/gen/", "generated engine, parameters and tables"),
        ("plugin/engine/", "C++ seam over the generated engine"),
        ("plugin/juce/", "JUCE host, editor, pages and processor"),
        ("plugin/ui/", "panel widgets, theme and analysis"),
        ("plugin/preset/", "library format, store and migrations"),
        ("plugin/tests/", "engine, host, preset, UI and audio tests"),
        ("plugin/tools/", "parameter, macro, token and snapshot tools"),
        ("tools/", "wavetable and maths table generators"),
        ("windows/", "Windows build and benchmark scripts"),
        ("docker/", "Linux container build"),
        ("manual/", "manual pipeline"),
        ("docs/", "documentation"),
        ("design/", "design notes and tokens"),
        ("synth/", "patch manifest"),
        (".github/", "CI and release workflows"),
    ]
    for path in paths:
        rel = str(path.relative_to(ROOT)).replace("\\", "/")
        try:
            count = len(path.read_text(encoding="utf-8", errors="replace").splitlines())
        except Exception:
            count = 0
        lang = {
            ".c": "C",
            ".h": "C/C++ header",
            ".hpp": "C++ header",
            ".cpp": "C++",
            ".py": "Python",
            ".ps1": "PowerShell",
            ".sh": "shell",
            ".cmake": "CMake",
            ".json": "JSON",
            ".md": "Markdown",
            ".yml": "YAML",
            ".yaml": "YAML",
        }.get(path.suffix.lower(), path.suffix or "text")
        if path.name == "CMakeLists.txt":
            lang = "CMake"
        role = "project source"
        for prefix, value in roles:
            if rel.startswith(prefix):
                role = value
                break
        summary = file_summary(path)
        if rel in FILE_ROLES:
            role = FILE_ROLES[rel]
        elif summary:
            role = summary
        lines.append(f"| `{rel}` | {count} | {lang} | {role} |")
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_types(types: list[Type], out: Path) -> None:
    lines = [
        "# Type, class, struct, union and enum index",
        "",
        "Every type definition found in the tracked C, C++ and Python sources. The",
        "field list is representative for the large generated state, which has its own",
        "field index in `state-fields.md`.",
        "",
        "| Type | Kind | File:Line | Fields | Description |",
        "| --- | --- | --- | ---: | --- |",
    ]
    for t in sorted(types, key=lambda x: (x.path, x.line, x.name)):
        description = t.description or CORE_TYPES.get(t.name, "")
        lines.append(
            f"| `{md_escape(t.name)}` | {t.kind} | `{t.path}:{t.line}` | "
            f"{len(t.fields)} | {md_escape(description)} |"
        )
    lines += ["", "## Details", ""]
    for t in sorted(types, key=lambda x: (x.path, x.line, x.name)):
        description = t.description or CORE_TYPES.get(t.name, "")
        lines.append(f"### `{t.name}`")
        lines.append("")
        lines.append(f"`{t.kind}` at `{t.path}:{t.line}` through line {t.end_line}.")
        if description:
            lines.append("")
            lines.append(description)
        if t.fields:
            lines.append("")
            lines.append("Fields and enumerators:")
            lines.append("")
            for field_text in t.fields:
                lines.append(f"- `{md_escape(field_text)}`")
        if t.methods:
            lines.append("")
            lines.append("Methods:")
            lines.append("")
            for method in t.methods:
                lines.append(f"- `{md_escape(method)}`")
        lines.append("")
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


CORE_FUNCTIONS = {
    "PluginEditor": "Builds the editor, the header, the toolbar, the tabs and the first page, then starts the 30 Hz timer.",
    "buildUi": "Creates the header, toolbar and tab bar and rebuilds the current page.",
    "buildPage": "Destroys the old page and creates the page for the active tab.",
    "buildHeader": "Creates the header component.",
    "buildToolbar": "Creates the toolbar component.",
    "resized": "Scales the fixed design canvas into the host window.",
    "timerCallback": "Runs the startup fit and refreshes the live visualisers.",
    "fitToHostWindow": "Measures the host chrome and sizes the editor to the display work area.",
    "updateVisualizers": "Refreshes the header, toolbar and page once per timer tick.",
    "refreshMacroFeedback": "Walks the component tree and updates every macro feedback target.",
    "showAlgorithmPicker": "Opens the ten-tile algorithm picker.",
    "applyAlgorithm": "Writes the selected algorithm to the active part.",
    "showRenameDialog": "Opens the rename dialog for a part or the patch.",
    "showRenameMacro": "Opens the rename dialog for a macro.",
    "showSinglePartsBrowser": "Opens the single-part library browser.",
    "exportLibrary": "Exports the library to a chosen file.",
    "importLibrary": "Imports a library file.",
    "showPatchBrowser": "Opens the multi-patch browser.",
    "showPatchSaveMode": "Opens the patch save-mode dialog.",
    "copyPart": "Copies the part parameters and routing into the clipboard.",
    "pastePart": "Writes the part clipboard to the current part.",
    "copyOperator": "Copies one operator's parameters.",
    "pasteOperator": "Writes one operator clipboard to the current operator.",
    "copyOperatorSection": "Copies one operator section, such as the oscillator or an envelope.",
    "pasteOperatorSection": "Writes one operator section and, for a spectrum section, selects spectrum mode.",
    "copyPartFxSection": "Copies one part fx/mod card to the other part.",
    "initPart": "Resets a part to the factory init values.",
    "initMultiPatch": "Resets the current multi-patch.",
    "togglePartMute": "Mutes or restores a part through its level parameter.",
    "savePartToSlot": "Saves the current part to a library slot.",
    "loadPartFromSlot": "Loads a library part into the current part.",
    "savePatchToSlot": "Saves the current patch to a multi slot.",
    "loadPatchFromSlot": "Loads a multi slot into both parts.",
    "reportLibrarySave": "Reports a library save result to the player.",
    "showConfirm": "Opens a confirmation dialog.",
    "closeDialog": "Closes the current dialog.",
    "currentPartParams": "Collects the part-block parameters for save or copy.",
    "applyPartParams": "Writes a part-block parameter vector.",
    "currentRouting": "Reads one per-part routing value.",
    "applyRouting": "Writes one per-part routing value.",
    "currentMacroParams": "Collects the 54 macro parameters.",
    "applyMacroParams": "Writes the 54 macro parameters.",
    "TypePAlphaAudioProcessor": "Constructs the processor, the engine seam, the MIDI bridge, the APVTS and the library.",
    "prepareToPlay": "Resets the engine, sizes the rate bridge and buffers, and syncs the parameters.",
    "releaseResources": "Releases every engine voice.",
    "isBusesLayoutSupported": "Accepts a disabled input and a mono or stereo output.",
    "processBlock": "Runs the host block through the 48 kHz core path or the rate bridge.",
    "syncParams": "Writes the changed APVTS values to the engine.",
    "syncMacros": "Resolves the six macros to destination offsets and writes the effective values.",
    "renderCore": "Renders one core chunk through the engine seam.",
    "pushTapScopes": "Copies the ten engine tap buffers into the scope rings.",
    "publishMorph": "Publishes the eight live spectral morphs.",
    "publishPhaseMaps": "Publishes the eight phase-map snapshots through the three-buffer handoff.",
    "feedScheduled": "Feeds the scheduled MIDI events whose core frame is before the limit.",
    "flushDeferred": "Feeds the note-offs that waited for the chunk to render.",
    "coreChunk": "Runs the per-core-chunk MIDI, tap and snapshot work.",
    "phaseMap": "Reads the current phase-map snapshot for the editor.",
    "specMorph": "Reads the live spectral morph for the editor.",
    "libraryFile": "Returns the shared library path.",
    "resetPhaseMaps": "Clears the phase-map, LFO and envelope pools.",
    "ensureLibraryLoaded": "Loads the shared library once.",
    "saveLibrary": "Runs the locked read-merge-write of the shared library.",
    "createEditor": "Creates the plugin editor.",
    "getStateInformation": "Serialises the APVTS, names, UI state, routing and library snapshot.",
    "setStateInformation": "Restores the project state and migrates older versions.",
    "prepare": "Builds the resampler kernel and ring for the host rate.",
    "reset": "Clears the resampler ring and counters.",
    "process": "Renders core chunks and pulls the host-rate output.",
    "pushCore": "Writes interleaved core frames into the ring.",
    "pull": "Reads host frames through the polyphase kernel.",
    "latencySamples": "Returns the bridge group delay in host samples.",
    "hasCore": "Reports whether a host block will render at least one core chunk.",
    "savePart": "Stores one part record in the in-memory bank.",
    "loadPart": "Reads one part record from the in-memory bank.",
    "packPartBank": "Packs all 128 part records.",
    "loadPartBank": "Loads and migrates a part bank.",
    "packMultiBank": "Packs all 64 multi records.",
    "loadMultiBank": "Loads a multi bank, including the old macro-less size.",
    "packLibrary": "Packs the library header, part bank and multi bank.",
    "loadLibrary": "Loads a library file and migrates its part records.",
    "mergeInto": "Merges the local change set into a disk store and reports conflicts.",
    "adoptLibrary": "Takes the merged banks and keeps the routing state.",
    "markSaved": "Clears the local change set.",
    "hasChanges": "Reports whether any local slot is dirty.",
    "findFreePartSlot": "Finds an unused part slot.",
    "findFreeMultiSlot": "Finds an unused multi slot.",
    "setCurrentMulti": "Sets the current multi-patch slot.",
    "setPartSource": "Sets the library part source for a part.",
    "migratePartBank": "Expands a part bank through the version 10 to 19 chain.",
    "setOperatorEnvelopeIndices": "Supplies the operator warm-up indices to the migration.",
    "setPanEnvelopeIndex": "Supplies the pan-envelope index to the migration.",
    "setOffsetEnvelopeIndices": "Supplies the pitch and feedback envelope indices to the migration.",
    "setAbsEnvelopeIndices": "Supplies the filter and spectral envelope indices to the migration.",
    "setLfoFadeIndices": "Supplies the 34 fade indices to the migration.",
    "setInsertionIndices": "Supplies the algorithm and waveform indices to the version 19 remap.",
}

CORE_TYPES = {
    "type_p_alpha_state": "The generated engine state. One instance per plugin instance.",
    "P1ParamSpec": "One manifest parameter description from the generated params header.",
    "P1ParamSpecs": "The generated manifest parameter table.",
    "Engine": "The C++ seam over one generated engine state.",
    "RateBridge": "The 48 kHz to host-rate output resampler.",
    "MidiBridge": "The C callback table that feeds the engine from the MIDI parser.",
    "PhaseMapSnapshot": "A display copy of one per-part phase map.",
    "PluginEditor": "The JUCE editor for the whole panel.",
    "TypePAlphaAudioProcessor": "The JUCE audio processor and the host-facing plugin object.",
    "PresetStore": "The in-memory part and multi library with migrations and merge.",
    "Part": "One part record in the preset store.",
    "Multi": "One multi-patch record in the preset store.",
    "ScopeRing": "The single-producer/single-consumer scope ring.",
    "ParameterLayout": "The APVTS parameter layout built from the manifest.",
    "P1Card": "The shared card frame and header.",
    "ParamKnob": "A parameter-bound rotary knob with a value line and macro feedback.",
    "P1SpectralBars": "The editable 64-bar spectral level display.",
    "P1Envelope": "The envelope graph for the operator, filter, pan and spectral envelopes.",
    "P1PhaseMap": "The phase-map transfer-curve view.",
    "P1Wave": "The small waveform preview.",
    "P1SpectrumDisplay": "The a/b spectral display with the live morph cursor.",
}


ACRONYMS = {
    "pcm": "PCM",
    "midi": "MIDI",
    "ui": "UI",
    "dsp": "DSP",
    "crc": "CRC",
    "id": "id",
    "ids": "ids",
    "cpu": "CPU",
    "rms": "RMS",
    "lfo": "LFO",
    "lfos": "LFOs",
    "fx": "FX",
    "fft": "FFT",
    "json": "JSON",
    "xml": "XML",
    "wav": "WAV",
    "db": "dB",
    "hz": "Hz",
    "q15": "Q15",
    "q16": "Q16",
    "q31": "Q31",
    "q32": "Q32",
    "avx2": "AVX2",
    "tapt": "tap",
}


def split_words(name: str) -> str:
    """Splits a camelCase or snake_case identifier into lower-case words."""
    name = re.sub(r"^[~_]*(?:p1_|type_p_alpha_)?", "", name)
    pieces = re.findall(r"[A-Z]+(?=[A-Z][a-z])|[A-Z]?[a-z]+|[A-Z]+|\d+[a-z]*", name)
    out = [ACRONYMS.get(p.lower(), p.lower()) for p in pieces if p]
    return " ".join(out) if out else name.lower()


def inner_comment(f: Function, lines: list[str]) -> str:
    """The prose comment at the top of the function body, as one sentence."""
    end = min(f.end_line, len(lines))
    i = f.line - 1
    while i < end and "{" not in lines[i]:
        i += 1
    i += 1
    while i < end:
        stripped = lines[i].strip()
        if not stripped:
            i += 1
            continue
        if stripped.startswith("//"):
            block = []
            while i < end and lines[i].strip().startswith("//"):
                block.append(lines[i].strip()[2:].strip())
                i += 1
            text = " ".join(block)
        elif stripped.startswith("/*"):
            block = [stripped]
            while "*/" not in lines[i] and i + 1 < end:
                i += 1
                block.append(lines[i].strip())
            text = " ".join(block)
            text = re.sub(r"/\*+|\*/", " ", text)
            text = re.sub(r"^\*+\s?", "", text.strip())
        else:
            return ""
        text = re.sub(r"\s+", " ", text).strip()
        match = re.match(r"([A-Z][^.]*\.)", text)
        if match:
            sentence = match.group(1).strip()
            if len(sentence.split()) >= 4:
                return sentence
        return ""
    return ""


def describe_function(
    f: Function, lines: list[str], known: dict[tuple[str, str], str] | None = None
) -> str:
    if f.description:
        return f.description
    name = f.name
    if name in CORE_FUNCTIONS:
        return CORE_FUNCTIONS[name]
    scope = f.scope
    if known is not None:
        for key in ((f.path, name), (scope, name), ("", name)):
            if key in known:
                return known[key]
    if name in ("main",):
        return "Runs the test or tool entry point."
    if f.path.endswith("engine/gen/type_p_alpha_dsp.c"):
        roles = {
            "note_to_inc": "Converts a MIDI note to a Q32 phase increment.",
            "type_p_alpha_param_slot": "Maps a manifest id to its state slot.",
            "type_p_alpha_dsp_state_init": "Initialises the state and applies the factory init table.",
            "voice_mono_active": "Selects the active note under the mono priority rule.",
            "voice_highest_note": "Returns the highest held note across the voice pool.",
            "voice_highest_velocity": "Returns the highest held velocity across the voice pool.",
            "type_p_alpha_note_derived": "Derives the four operator increments from the voice pitch and bend.",
            "type_p_alpha_taps_clear": "Clears the ten tap buffers before a render block.",
            "type_p_alpha_render_pre_block": "Runs the control-rate gate, pan, delay and portamento work.",
            "type_p_alpha_render_voices_block": "Runs the per-voice node graph and the audio render.",
            "type_p_alpha_render_post_block": "Applies the DC blocker and writes interleaved output.",
            "type_p_alpha_dsp_render_s": "Drives 32-sample blocks through pre, voices and post.",
            "type_p_alpha_dsp_reset_s": "Clears the voice, delay and mono state.",
            "type_p_alpha_dsp_note_on_ch_s": "Routes a note to the matching parts and allocates poly or mono voices.",
            "type_p_alpha_dsp_note_on_s": "Omni wrapper for the note-on path.",
            "type_p_alpha_dsp_note_off_ch_s": "Releases matching voices and applies the mono fallback.",
            "type_p_alpha_dsp_note_off_s": "Omni wrapper for the note-off path.",
            "type_p_alpha_dsp_all_notes_off_ch_s": "Releases or kills the voices on one channel.",
            "type_p_alpha_dsp_all_notes_off_s": "Global panic wrapper.",
            "type_p_alpha_dsp_set_bend_ch_q16_s": "Converts a 14-bit bend to the part ratio and retunes held voices.",
            "type_p_alpha_dsp_set_bend_q16_s": "Omni wrapper for the bend path.",
            "type_p_alpha_dsp_lit_s": "Returns the 27-note activity mask for the keyboard display.",
            "type_p_alpha_dsp_quiet_s": "Reports whether any voice gate is open.",
            "type_p_alpha_dsp_set_param_s": "Clamps and writes one parameter through the slot table.",
            "type_p_alpha_dsp_get_param_s": "Reads one parameter through the slot table.",
            "type_p_alpha_dsp_param_count": "Returns the manifest parameter count.",
            "type_p_alpha_dsp_tap_s": "Returns one tap buffer by channel.",
        }
        if name in roles:
            return roles[name]
        return "Generated engine function. See `generated-render-walk.md` for its phase."
    if name in ("check", "fail", "ok", "expect"):
        return "Records a test assertion."
    if name.startswith("~"):
        return "Destroys the object."
    if name == scope:
        return "Constructs the object."
    if name == "paint":
        return "Draws the component."
    if name == "resized":
        return "Lays out the child components."
    if name in ("mouseDown", "mouseDrag", "mouseUp", "mouseMove",
                "mouseWheelMove", "mouseEnter", "mouseExit"):
        return "Handles a mouse interaction."
    if name == "keyPressed":
        return "Handles a key press."
    if name in ("tick", "timerCallback"):
        return "Refreshes the live display state."
    if name in ("getWidth", "getHeight", "getLocalBounds"):
        return "Returns a JUCE geometry value."
    if f.path.endswith("ParamIDs.h"):
        body = " ".join(lines[f.line - 1 : f.end_line])
        match = re.search(r"withPart\(\"([^\"]+)\"", body)
        if match:
            return f"Returns the `{match.group(1)}` parameter id with the part suffix."
        match = re.search(r"opBase\(op,\s*\"([^\"]+)\"", body)
        if match:
            return f"Builds the operator parameter id for `{match.group(1)}`."
        if name == "partSuffix":
            return "Returns the `.pN` suffix for a part index."
        if name == "withPart":
            return "Appends the part suffix to a base parameter id."
        if name == "opBase":
            return "Builds an operator parameter id from the operator index and field."
    if name in ("processor",):
        return "Returns the audio processor."
    if name in ("state",):
        if f.scope == "Engine":
            return "Returns the generated state pointer."
        return "Returns the APVTS."
    if name in ("activePart",):
        return "Returns the active part index."
    if name in ("activeTab",):
        return "Returns the active page index."
    if name in ("macroParamIndex",):
        return "Returns the manifest index used for macro feedback."
    words = split_words(name)
    if f.path.startswith("plugin/tests/"):
        if name.startswith("test"):
            return f"Runs the `{name}` case."
        if name.startswith("check"):
            rest = split_words(name[5:])
            return f"Checks the {rest} result." if rest else "Checks one result."
        if name.startswith("measure"):
            rest = split_words(name[7:])
            return f"Measures the {rest} path." if rest else "Measures one path."
        if name in ("run",):
            return "Runs one measured case and returns its result."
        if name in ("matches", "same", "equal", "close"):
            return "Compares a rendered result with its reference."
        if name in ("setup", "teardown"):
            return "Prepares or releases the shared test fixtures."
        if name.startswith("read"):
            rest = split_words(name[4:])
            return f"Reads the {rest} capture." if rest else "Reads one capture."
        if name.startswith("write"):
            rest = split_words(name[5:])
            return f"Writes the {rest} capture." if rest else "Writes one capture."
    if f.path.startswith("engine/runtime/presets") or f.path.startswith("plugin/preset/"):
        if name.endswith("crc32"):
            return "Computes the reflected CRC-32 over a record body."
        if "unpack" in name:
            return "Unpacks and CRC-checks a record."
        if any(token in name for token in ("pack", "serialise")):
            return "Packs a record into its little-endian bytes and appends the CRC."
        if name.endswith(("save", "write")):
            return "Writes a bank or record through the host store."
        if name.endswith(("load", "read")):
            return "Reads a bank, record or library file and migrates it."
        if "size" in name:
            return "Returns a record, bank or file size."
        if "name" in name:
            return "Copies or trims a fixed-width record name field."
    overrides = {
        "paintButton": "Draws the button in its normal, highlighted and down states.",
        "paintListBoxItem": "Draws one list-box row.",
        "paintOverChildren": "Draws content above the child components.",
        "paintRow": "Draws one row of the control.",
        "buttonClicked": "Handles a button click.",
        "buttonStateChanged": "Handles a button state change.",
        "comboBoxChanged": "Handles a combo-box selection.",
        "sliderValueChanged": "Handles a slider value change.",
        "textEditorReturnKeyPressed": "Handles the return key in the editor.",
        "textEditorEscapeKeyPressed": "Handles the escape key in the editor.",
        "textEditorFocusLost": "Handles the editor losing focus.",
        "listBoxItemClicked": "Handles a list-box row click.",
        "listBoxItemDoubleClicked": "Handles a list-box row double click.",
        "visibilityChanged": "Handles a change in component visibility.",
        "parentHierarchyChanged": "Handles a change in the component parent.",
        "mouseWheelMove": "Handles a mouse-wheel step.",
        "mouseDoubleClick": "Handles a mouse double click.",
        "mouseMove": "Handles a mouse move over the component.",
        "focus": "Moves keyboard focus to the control.",
        "focusGained": "Handles keyboard focus arriving.",
        "focusLost": "Handles keyboard focus leaving.",
    }
    if name in overrides:
        return overrides[name]
    if name.startswith("paint") or name.startswith("draw"):
        rest = split_words(name[5:] if name.startswith("paint") else name[4:])
        return f"Draws the {rest}." if rest else "Draws the component."
    if name.startswith("refresh"):
        rest = split_words(name[7:])
        return f"Refreshes the {rest} display." if rest else "Refreshes the display."
    verb_prefixes = (
        ("get", "Reads"),
        ("set", "Writes"),
        ("is", "Tests whether the"),
        ("has", "Tests whether the"),
        ("can", "Tests whether the"),
        ("save", "Saves"),
        ("load", "Loads"),
        ("read", "Reads"),
        ("write", "Writes"),
        ("render", "Renders"),
        ("build", "Builds"),
        ("create", "Creates"),
        ("make", "Creates"),
        ("add", "Adds"),
        ("remove", "Removes"),
        ("apply", "Applies"),
        ("publish", "Publishes"),
        ("feed", "Feeds"),
        ("flush", "Flushes"),
        ("push", "Pushes"),
        ("pull", "Pulls"),
        ("reset", "Resets"),
        ("clear", "Clears"),
        ("init", "Initialises"),
        ("copy", "Copies"),
        ("paste", "Pastes"),
        ("move", "Moves"),
        ("find", "Finds"),
        ("ensure", "Ensures"),
        ("toggle", "Toggles"),
        ("select", "Selects"),
        ("show", "Opens"),
        ("close", "Closes"),
        ("hide", "Hides"),
        ("update", "Updates"),
        ("resolve", "Resolves"),
        ("convert", "Converts"),
        ("clamp", "Clamps"),
        ("scale", "Scales"),
        ("map", "Maps"),
        ("mix", "Mixes"),
        ("parse", "Parses"),
        ("format", "Formats"),
        ("analyse", "Analyses"),
        ("validate", "Validates"),
        ("schedule", "Schedules"),
        ("defer", "Defers"),
        ("route", "Routes"),
        ("advance", "Advances"),
        ("track", "Tracks"),
        ("count", "Counts"),
        ("sum", "Sums"),
        ("fill", "Fills"),
        ("scan", "Scans"),
        ("detect", "Detects"),
        ("merge", "Merges"),
        ("migrate", "Migrates"),
        ("restore", "Restores"),
        ("adopt", "Adopts"),
        ("emit", "Emits"),
        ("seed", "Seeds"),
        ("hash", "Hashes"),
        ("seal", "Seals"),
        ("sort", "Sorts"),
        ("play", "Plays"),
    )
    for prefix, verb in verb_prefixes:
        if name.startswith(prefix) and len(name) > len(prefix) + 1:
            rest = split_words(name[len(prefix):])
            if rest:
                return f"{verb} the {rest}."
    if f.path.endswith("engine/runtime/synth_midi.c"):
        roles = {
            "midi_get": "The host hook that pops one raw MIDI message.",
            "synth_midi_feed": "Applies one MIDI message, including sustain and all-notes-off.",
            "synth_midi_poll": "Drains the host MIDI queue into the handler.",
        }
        if name in roles:
            return roles[name]
    if name == "seal" and "presets" in f.path:
        return "Writes the record CRC into the trailing four bytes."
    comment = inner_comment(f, lines)
    if comment:
        return comment
    head = f.signature.split("(")[0]
    returns = head.replace(name, "").strip()
    if "bool" in returns:
        return f"Tests the {words} condition."
    if returns in ("", "void") or returns.endswith(" void"):
        return f"Runs the {words} step for the surrounding code."
    return f"Returns the {words} value."


def write_functions(functions: list[Function], out: Path) -> None:
    lines = [
        "# Function and method index",
        "",
        "Every function definition and declaration found in the tracked C, C++ and",
        "Python sources. The description is the comment immediately above the",
        "declaration, or a short purpose derived from the declaration when the",
        "function has no leading comment. The hand-written chapters explain the core",
        "paths in depth.",
        "",
        "| Function | Scope | File:Line | Signature | Description |",
        "| --- | --- | --- | --- | --- |",
    ]
    cache: dict[str, list[str]] = {}
    known: dict[tuple[str, str], str] = {}
    ordered = sorted(functions, key=lambda x: (x.path, x.line, x.name))
    for f in ordered:
        source_lines = cache.setdefault(
            f.path,
            (ROOT / f.path).read_text(encoding="utf-8", errors="replace").splitlines(),
        )
        strong = f.description or inner_comment(f, source_lines)
        if strong:
            known.setdefault((f.path, f.name), strong)
            if f.scope:
                known.setdefault((f.scope, f.name), strong)
            if len(f.name) > 6:
                known.setdefault(("", f.name), strong)
    for f in ordered:
        source_lines = cache[f.path]
        description = describe_function(f, source_lines, known)
        if description and not description.endswith((".", ")", "`")):
            description += "."
        signature = f.signature
        if len(signature) > 180:
            signature = signature[:177] + "..."
        lines.append(
            f"| `{md_escape(f.name)}` | {md_escape(f.scope)} | "
            f"`{f.path}:{f.line}` | `{md_escape(signature)}` | "
            f"{md_escape(description)} |"
        )
    lines += ["", "## Details by file", ""]
    current = ""
    for f in sorted(functions, key=lambda x: (x.path, x.line, x.name)):
        if f.path != current:
            current = f.path
            lines += ["", f"### `{current}`", ""]
        lines.append(
            f"- `{f.name}` at line {f.line} through {f.end_line}: "
            f"`{md_escape(f.signature)}`"
        )
        source_lines = cache[f.path]
        description = describe_function(f, source_lines, known)
        if description:
            lines.append(f"  {description}")
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_symbols(symbols: list[Symbol], out: Path) -> None:
    lines = [
        "# Macro, global, alias and constant index",
        "",
        "Every macro, global variable, alias, using declaration and Python module",
        "constant found in the tracked sources. Generated state aliases are omitted",
        "because the state field index covers the same names.",
        "",
        "| Symbol | Kind | File:Line | Value | Description |",
        "| --- | --- | --- | --- | --- |",
    ]
    for s in sorted(symbols, key=lambda x: (x.path, x.line, x.name)):
        if s.kind == "macro" and s.name.startswith(("op1_", "op2_", "op3_", "op4_")) and "S->" in s.value:
            continue
        value = s.value
        if len(value) > 120:
            value = value[:117] + "..."
        lines.append(
            f"| `{md_escape(s.name)}` | {s.kind} | `{s.path}:{s.line}` | "
            f"`{md_escape(value)}` | {md_escape(s.description)} |"
        )
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def state_fields() -> list[tuple[str, str]]:
    path = ROOT / "engine" / "gen" / "dsp.h"
    raw = path.read_text(encoding="utf-8")
    start = raw.find("typedef struct type_p_alpha_state {")
    end = raw.find("} type_p_alpha_state;", start)
    body = raw[start:end]
    out: list[tuple[str, str]] = []
    for line in body.splitlines():
        line = line.strip()
        m = re.match(r"^(.+?)\s+([A-Za-z_]\w*)(\[[^\]]+\])?;$", line)
        if m:
            out.append((m.group(2), line))
    return out


def write_state(fields: list[tuple[str, str]], out: Path) -> None:
    lines = [
        "# `type_p_alpha_state` field index",
        "",
        "Every field of the generated engine state, in declaration order. The",
        "operator blocks repeat the same field pattern for `op1_` to `op4_`.",
        "",
        "| Field | Declaration |",
        "| --- | --- |",
    ]
    for name, declaration in fields:
        lines.append(f"| `{name}` | `{md_escape(declaration)}` |")
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_parameters(out: Path) -> None:
    data = json.loads((ROOT / "engine" / "gen" / "params.json").read_text(encoding="utf-8"))
    params = data["params"]
    lines = [
        "# Parameter index",
        "",
        "Every manifest parameter. The part block has 994 values, mirrored for",
        "`.p0` and `.p1`, plus five per-part routing parameters outside the block and",
        "55 global parameters. The table lists the `.p0` and global entries, and the",
        "`.p1` entry mirrors the `.p0` layout with the engine id offset by 994.",
        "",
        "| ID | Name | Target | Range | Default | Curve | Rate | Display | Auto | Mod |",
        "| ---: | --- | --- | --- | ---: | --- | --- | --- | --- | --- |",
    ]
    for p in params:
        name = str(p["name"])
        if name.endswith(".p1"):
            continue
        automatable = "yes" if p.get("automatable") else "no"
        modulatable = "yes" if p.get("modulatable") else "no"
        lines.append(
            f"| {p['id']} | `{name}` | `{p['target']}` | "
            f"{p['min']}..{p['max']} | {p['default']} | {p['curve']} | "
            f"{p['rate']} | {p['display']} | {automatable} | {modulatable} |"
        )
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def render_walk() -> None:
    path = ROOT / "engine" / "gen" / "type_p_alpha_dsp.c"
    if not path.exists():
        return
    lines = path.read_text(encoding="utf-8").splitlines()
    out = [
        "# Generated render walk",
        "",
        "The generated engine is one translation unit with a parameter table, a",
        "state initialiser, the note and MIDI entry points, and one large render",
        "function. This page records the phase boundaries and every distinct block",
        "comment inside the per-voice renderer, in source order. The four operator",
        "chains repeat the same sequence, so the comment table is the canonical",
        "description of each generated node block.",
        "",
        "## Top-level functions",
        "",
        "| Function | Lines | Role |",
        "| --- | --- | --- |",
    ]
    functions = [
        ("type_p_alpha_dsp_state_init", "initialise the state and apply the init table"),
        ("type_p_alpha_render_pre_block", "per-control-block gate, pan, portamento and voice-delay work"),
        ("type_p_alpha_render_voices_block", "the per-voice node graph and audio render"),
        ("type_p_alpha_render_post_block", "DC blocker and interleaved output"),
        ("type_p_alpha_dsp_render_s", "drive 32-sample blocks through pre, voices and post"),
        ("type_p_alpha_dsp_reset_s", "clear the voice and mono state"),
        ("type_p_alpha_dsp_note_on_ch_s", "part/channel routing, poly and mono allocation"),
        ("type_p_alpha_dsp_note_on_s", "omni note-on wrapper"),
        ("type_p_alpha_dsp_note_off_ch_s", "channel-aware release and mono fallback"),
        ("type_p_alpha_dsp_note_off_s", "omni note-off wrapper"),
        ("type_p_alpha_dsp_all_notes_off_ch_s", "per-channel panic"),
        ("type_p_alpha_dsp_all_notes_off_s", "global panic"),
        ("type_p_alpha_dsp_set_bend_ch_q16_s", "per-channel bend and note-derived increments"),
        ("type_p_alpha_dsp_set_bend_q16_s", "omni bend wrapper"),
        ("type_p_alpha_dsp_lit_s", "27-note voice activity mask"),
        ("type_p_alpha_dsp_quiet_s", "gate test"),
        ("type_p_alpha_dsp_set_param_s", "clamped parameter write through the slot table"),
        ("type_p_alpha_dsp_get_param_s", "parameter read through the slot table"),
        ("type_p_alpha_dsp_param_count", "manifest parameter count"),
        ("type_p_alpha_dsp_tap_s", "per-channel tap buffer"),
    ]
    for name, role in functions:
        for i, line in enumerate(lines):
            if line.startswith("P1_DSP_API") and name + "(" in line:
                out.append(f"| `{name}` | {i + 1} | {role} |")
                break
    out += ["", "## Distinct block comments in the per-voice renderer", ""]
    start = next(i for i, l in enumerate(lines) if "type_p_alpha_render_voices_block" in l)
    end = next(i for i, l in enumerate(lines) if "type_p_alpha_render_post_block" in l)
    comments: list[tuple[int, str]] = []
    i = start
    while i < end:
        line = lines[i]
        if "/*" in line:
            s = line.index("/*")
            j = i
            buf: list[str] = []
            while j < len(lines):
                l = lines[j]
                begin = s + 2 if j == i else 0
                k = l.find("*/", begin)
                if k >= 0:
                    buf.append(l[begin:k])
                    break
                buf.append(l[begin:])
                j += 1
            txt = " ".join(x.strip() for x in buf if x.strip())
            if txt:
                comments.append((i + 1, txt))
            i = j
        i += 1
    seen: set[str] = set()
    out += ["| First line | Repeats | Block comment |", "| ---: | ---: | --- |"]
    counts = Counter(t for _, t in comments)
    for line, txt in comments:
        if txt in seen:
            continue
        seen.add(txt)
        out.append(f"| {line} | {counts[txt]} | {md_escape(txt)} |")
    (OUT / "generated-render-walk.md").write_text("\n".join(out) + "\n", encoding="utf-8")


def write_nodes(out: Path) -> None:
    nodes_dir = ROOT / ".dev" / "modules"
    if not nodes_dir.exists():
        return
    lines = [
        "# Generator node catalogue",
        "",
        "The local `.dev` tree is the generator library and is not part of the public",
        "build. Each node below is a code block that the generator inlines into",
        "`engine/gen/type_p_alpha_dsp.c`. The summary is the node's own description.",
        "",
        "| Node | Kind | Inputs | Outputs | Summary |",
        "| --- | --- | --- | --- | --- |",
    ]
    for folder, kind in (("nodes", "node"), ("macros", "macro")):
        for path in sorted((nodes_dir / folder).glob("*/node.json")):
            data = json.loads(path.read_text(encoding="utf-8"))
            ins = ", ".join(data.get("ins", {}).keys())
            outs = ", ".join(data.get("outs", {}).keys())
            lines.append(
                f"| `{data.get('name', path.parent.name)}` | {kind} | "
                f"{md_escape(ins)} | {md_escape(outs)} | "
                f"{md_escape(data.get('summary', ''))} |"
            )
        for path in sorted((nodes_dir / folder).glob("*/macro.json")):
            data = json.loads(path.read_text(encoding="utf-8"))
            lines.append(
                f"| `{data.get('name', path.parent.name)}` | {kind} |  |  | "
                f"{md_escape(data.get('summary', ''))} |"
            )
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def write_tests(out: Path) -> None:
    exe_sources: dict[str, str] = {}
    cmake = (ROOT / "plugin" / "CMakeLists.txt").read_text(encoding="utf-8")
    for match in re.finditer(r"add_executable\((\w+)\s+([^)]+)\)", cmake, re.S):
        exe, source = match.groups()
        exe_sources[exe] = source.strip().split()[0]
    juce = (ROOT / "plugin" / "juce" / "CMakeLists.txt").read_text(encoding="utf-8")
    for match in re.finditer(r"add_executable\((\w+)\s+([^)]+)\)", juce, re.S):
        exe, source = match.groups()
        exe_sources.setdefault(exe, source.strip().split()[0])
    manual = {
        "linux_daws": "plugin/tests/linux_daws.sh",
        "plugin_smoke": "plugin/tests/plugin_smoke.cpp",
        "plugin_stability": "plugin/tests/plugin_stability.cpp",
        "plugin_editor_snapshot": "plugin/tests/editor_snapshot.cpp",
        "plugin_editor_snapshot_diff": "plugin/tests/editor_snapshot.cpp",
    }
    tests: list[tuple[str, str, str]] = []
    for text in (cmake, juce):
        for match in re.finditer(r"add_test\(NAME\s+(\w+)\s+COMMAND\s+(\w+)", text):
            name, exe = match.groups()
            tests.append((name, exe, exe_sources.get(exe, manual.get(name, "plugin/tests"))))
    tests += [
        ("plugin_editor_snapshot", "p1_editor_snapshot", "plugin/tests/editor_snapshot.cpp"),
        ("plugin_editor_snapshot_diff", "snapshot_check.py", "plugin/tests/editor_snapshot.cpp"),
        ("linux_daws", "sh", "plugin/tests/linux_daws.sh"),
    ]
    lines = [
        "# Test index",
        "",
        "The CTest targets and their source files. The hand-written build chapter",
        "explains what each layer proves and how to run it.",
        "",
        "| CTest name | Executable | Source |",
        "| --- | --- | --- |",
    ]
    for name, exe, source in sorted(set(tests)):
        if source.startswith("tests/"):
            source = "plugin/" + source
        lines.append(f"| `{name}` | `{exe}` | `{source}` |")
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--include-dev", action="store_true")
    args = parser.parse_args()
    OUT.mkdir(parents=True, exist_ok=True)
    paths = source_files(args.include_dev)
    functions: list[Function] = []
    types: list[Type] = []
    symbols: list[Symbol] = []
    for path in paths:
        if path.suffix.lower() == ".py":
            f, t, s = collect_python(path)
        elif path.suffix.lower() in (".c", ".h", ".hpp", ".cpp"):
            f, t, s = collect_c_family(path)
        else:
            continue
        functions.extend(f)
        types.extend(t)
        symbols.extend(s)
    write_files(paths, OUT / "files.md")
    write_types(types, OUT / "types.md")
    write_functions(functions, OUT / "functions.md")
    write_symbols(symbols, OUT / "symbols.md")
    write_state(state_fields(), OUT / "state-fields.md")
    write_parameters(OUT / "parameters.md")
    render_walk()
    if (ROOT / ".dev").exists():
        write_nodes(OUT / "nodes.md")
    write_tests(OUT / "tests.md")
    print(
        f"wrote {OUT}: {len(paths)} files, {len(types)} types, "
        f"{len(functions)} functions, {len(symbols)} symbols"
    )
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
