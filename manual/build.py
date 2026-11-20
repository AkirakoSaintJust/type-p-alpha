"""Build the manual: HTML for the screen, HTML for print, and a PDF."""

from __future__ import annotations

import datetime
import json
import shutil
import subprocess
import sys
from pathlib import Path

try:
    import markdown
except ImportError:
    sys.exit("manual: pip install -r manual/requirements.txt")

import figures

ROOT = Path(__file__).resolve().parents[1]
MANUAL = ROOT / "manual"
OUT = ROOT / "build" / "manual"

BROWSERS = (
    Path(r"C:\Program Files (x86)\Microsoft\Edge\Application\msedge.exe"),
    Path(r"C:\Program Files\Microsoft\Edge\Application\msedge.exe"),
    Path(r"C:\Program Files\Google\Chrome\Application\chrome.exe"),
    Path("/usr/bin/google-chrome"),
    Path("/usr/bin/chromium"),
    Path("/usr/bin/chromium-browser"),
    Path("/Applications/Google Chrome.app/Contents/MacOS/Google Chrome"),
)

COVER = """<section class="cover">
<div class="cover-mark">TYPE P.<span class="dot">.</span><span class="alpha">α</span></div>
<p class="cover-kicker">{kind}</p>
<h1 class="cover-title">{title}</h1>
<p class="cover-sub">A phase modulation and additive synthesizer</p>
<p class="cover-meta">draft · {date}</p>
</section>"""

COLOPHON = """<div class="colophon">
<h2>Colophon</h2>
<p>{title}, draft, {date}.</p>
<p>Set in Zen Dots, Libre Franklin and Fira Mono. The fonts are under the SIL
Open Font License 1.1.</p>
</div>"""


def strip_toc(toc: str) -> str:
    """Drop Python-Markdown's outer TOC div, because the template owns the nav."""
    toc = toc.strip()
    if toc.startswith('<div class="toc">'):
        toc = toc[len('<div class="toc">') :]
    if toc.endswith("</div>"):
        toc = toc[: -len("</div>")]
    return toc.strip()


def find_browser() -> Path | None:
    return next((browser for browser in BROWSERS if browser.is_file()), None)


def main() -> int:
    manifest = json.loads((MANUAL / "manifest.json").read_text(encoding="utf-8"))
    OUT.mkdir(parents=True, exist_ok=True)

    md = markdown.Markdown(
        extensions=["toc", "tables", "fenced_code", "attr_list", "md_in_html"]
    )
    parts: list[str] = []
    for chapter in manifest["chapters"]:
        source = MANUAL / "chapters" / chapter["file"]
        parts.append(md.convert(source.read_text(encoding="utf-8")))
    content = "\n".join(parts)
    toc = strip_toc(md.toc)

    date = datetime.date.today().isoformat()
    template = (MANUAL / "theme" / "template.html").read_text(encoding="utf-8")
    common = {
        "{{title}}": manifest["title"],
        "{{doc_kind}}": manifest["doc_kind"],
        "{{date}}": date,
        "{{toc}}": toc,
        "{{content}}": content + COLOPHON.format(
            title=manifest["title"], date=date
        ),
    }

    screen = template
    for key, value in common.items():
        screen = screen.replace(key, value)
    screen = (
        screen.replace("{{css}}", "screen.css")
        .replace("{{body_class}}", "screen")
        .replace("{{cover}}", "")
    )
    (OUT / "index.html").write_text(screen, encoding="utf-8")

    print_page = template
    for key, value in common.items():
        print_page = print_page.replace(key, value)
    print_page = (
        print_page.replace("{{css}}", "print.css")
        .replace("{{body_class}}", "print")
        .replace(
            "{{cover}}",
            COVER.format(
                kind=manifest["doc_kind"], title=manifest["title"], date=date
            ),
        )
    )
    (OUT / "print.html").write_text(print_page, encoding="utf-8")

    for name in ("screen.css", "print.css"):
        shutil.copy2(MANUAL / "theme" / name, OUT / name)
    figures.write(OUT)

    browser = find_browser()
    pdf = OUT / "manual.pdf"
    if browser is None:
        print("manual: no Edge, Chrome or Chromium; open print.html and print to PDF")
    else:
        result = subprocess.run(
            [
                str(browser),
                "--headless=new",
                "--disable-gpu",
                "--no-pdf-header-footer",
                f"--print-to-pdf={pdf}",
                (OUT / "print.html").as_uri(),
            ],
            check=False,
        )
        if result.returncode != 0:
            print(f"manual: {browser.name} returned {result.returncode}")

    print(f"manual: wrote {OUT / 'index.html'}")
    print(f"manual: wrote {OUT / 'print.html'}")
    if pdf.is_file():
        print(f"manual: wrote {pdf}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
