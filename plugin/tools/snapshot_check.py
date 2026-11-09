#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-3.0-only
"""Compare the editor snapshots against the committed golden set.

The C++ `plugin_editor_snapshot` test writes PNGs. This script hashes them,
checks that the golden manifest matches the committed images, and fails when an
image is missing, extra, or different. `--update` is the deliberate accept step:
it copies the current images into the golden directory and rewrites the
manifest. Review the new PNGs before you commit the update.

No third-party dependency is required for the gate. When Pillow is available,
a changed image also gets a red diff overlay and a side-by-side image under
`--diff`, for review.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import shutil
import struct
import sys
from pathlib import Path

_MANIFEST = "manifest.json"


def _sha256(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as handle:
        for chunk in iter(lambda: handle.read(1 << 20), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _png_size(path: Path) -> tuple[int, int]:
    """Read the IHDR width and height from a PNG without decoding it."""
    with path.open("rb") as handle:
        header = handle.read(24)
    if len(header) != 24 or header[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path} is not a PNG")
    width, height = struct.unpack(">II", header[16:24])
    return width, height


def _images(directory: Path) -> dict[str, Path]:
    if not directory.is_dir():
        return {}
    return {path.name: path for path in sorted(directory.glob("*.png"))}


def _load_manifest(golden: Path) -> dict[str, dict[str, object]]:
    path = golden / _MANIFEST
    if not path.is_file():
        return {}
    raw = json.loads(path.read_text(encoding="utf-8"))
    images = raw.get("images", {})
    if not isinstance(images, dict):
        return {}
    return images


def _write_manifest(golden: Path, images: dict[str, Path]) -> None:
    entries: dict[str, dict[str, object]] = {}
    for name, path in images.items():
        width, height = _png_size(path)
        entries[name] = {
            "sha256": _sha256(path),
            "width": width,
            "height": height,
        }
    payload = {"platform": golden.name, "images": entries}
    (golden / _MANIFEST).write_text(
        json.dumps(payload, indent=2, sort_keys=True) + "\n", encoding="utf-8"
    )


def _diff_images(actual: Path, golden: Path, out: Path, names: list[str]) -> list[str]:
    """Write review images for changed snapshots when Pillow is installed."""
    try:
        from PIL import Image, ImageChops
    except ImportError:
        return []
    out.mkdir(parents=True, exist_ok=True)
    written: list[str] = []
    for name in names:
        current_path = actual / name
        golden_path = golden / name
        if not current_path.is_file() or not golden_path.is_file():
            continue
        with (
            Image.open(golden_path) as old_image,
            Image.open(current_path) as new_image,
        ):
            old = old_image.convert("RGB")
            new = new_image.convert("RGB")
            if old.size != new.size:
                new = new.resize(old.size)
            difference = ImageChops.difference(old, new)
            bbox = difference.getbbox()
            if bbox is None:
                continue
            grey = difference.convert("L")
            overlay = Image.new("RGB", old.size, (0, 0, 0))
            overlay.paste((255, 0, 0), mask=grey.point(lambda v: 255 if v else 0))
            blended = Image.blend(old, overlay, 0.55)
            blended.save(out / f"{current_path.stem}-diff.png")
            side = Image.new("RGB", (old.width * 2 + 8, old.height), (24, 24, 24))
            side.paste(old, (0, 0))
            side.paste(new, (old.width + 8, 0))
            side.save(out / f"{current_path.stem}-side.png")
            written.append(name)
    return written


def main(argv: list[str] | None = None) -> int:
    parser = argparse.ArgumentParser(prog="snapshot_check", description=__doc__)
    parser.add_argument("--actual", type=Path, required=True, help="the new snapshots")
    parser.add_argument("--golden", type=Path, required=True, help="the committed set")
    parser.add_argument("--diff", type=Path, help="where to write review images")
    parser.add_argument(
        "--update",
        action="store_true",
        help="copy the actual images into the golden set and rewrite the manifest",
    )
    args = parser.parse_args(argv)

    actual = _images(args.actual)
    if not actual:
        print(f"snapshot: no PNGs in {args.actual}", file=sys.stderr)
        print("snapshot: run the editor snapshot test first", file=sys.stderr)
        return 2

    if args.update:
        args.golden.mkdir(parents=True, exist_ok=True)
        for stale in args.golden.glob("*.png"):
            if stale.name not in actual:
                stale.unlink()
        for name, path in actual.items():
            shutil.copy2(path, args.golden / name)
        _write_manifest(args.golden, actual)
        print(
            f"snapshot: updated {len(actual)} images in {args.golden}. "
            "Review them and commit them with the code."
        )
        return 0

    golden = _images(args.golden)
    manifest = _load_manifest(args.golden)
    if not golden or not manifest:
        print(f"snapshot: no golden set in {args.golden}", file=sys.stderr)
        print(
            "snapshot: create one with `python3 plugin/tools/snapshot_check.py "
            f"--update --actual {args.actual} --golden {args.golden}`",
            file=sys.stderr,
        )
        return 2

    changed: list[str] = []
    problems: list[str] = []
    for name, path in actual.items():
        if name not in golden:
            problems.append(f"new image: {name}")
            continue
        if _sha256(path) != _sha256(golden[name]):
            changed.append(name)
    for name in golden:
        if name not in actual:
            problems.append(f"missing image: {name}")

    for name, entry in manifest.items():
        if name not in golden:
            problems.append(f"manifest names a missing image: {name}")
            continue
        if _sha256(golden[name]) != entry.get("sha256"):
            problems.append(f"manifest is stale for {name}: run --update")
        width, height = _png_size(golden[name])
        if [width, height] != [entry.get("width"), entry.get("height")]:
            problems.append(f"manifest size is stale for {name}: run --update")

    if changed and args.diff:
        written = _diff_images(args.actual, args.golden, args.diff, changed)
        if written:
            print(f"snapshot: review images in {args.diff}")

    if changed or problems:
        print(f"snapshot: {len(changed)} changed, {len(problems)} problem(s)")
        for name in changed:
            print(f"  changed: {name}")
        for problem in problems:
            print(f"  {problem}")
        print()
        print("Review the new images, then accept the change deliberately:")
        print(
            "  python3 plugin/tools/snapshot_check.py --update "
            f"--actual {args.actual} --golden {args.golden}"
        )
        print("Commit the golden images and the code together.")
        return 1

    print(f"snapshot: {len(actual)} images match {args.golden}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
