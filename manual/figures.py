"""The manual figures: the panel shots and the generated diagrams."""

from __future__ import annotations

import shutil
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
SHOTS = ROOT / "plugin" / "tests" / "snapshots" / "linux-x86_64"

ENVELOPE = """<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 720 260" role="img" aria-labelledby="amp-title">
<title id="amp-title">Amplitude envelope stages</title>
<g stroke="#8b8b8b" stroke-width="1" opacity=".35">
<line x1="50" y1="210" x2="680" y2="210"/>
<line x1="50" y1="40" x2="50" y2="210"/>
</g>
<path d="M 50 210 Q 150 210 150 40 L 250 50 L 380 60 L 520 60 C 570 60 600 160 640 210" fill="none" stroke="#ff2e7e" stroke-width="3"/>
<g stroke="#c780ff" stroke-width="1">
<line x1="516" y1="40" x2="516" y2="210"/>
<line x1="524" y1="40" x2="524" y2="210"/>
</g>
<g font-family="Fira Mono, monospace" font-size="12" fill="#8b8b8b">
<text x="44" y="34">99</text>
<text x="44" y="200">0</text>
<text x="76" y="234">surge</text>
<text x="186" y="234">decay 1</text>
<text x="306" y="234">decay 2</text>
<text x="430" y="234">sustain</text>
<text x="570" y="234">release</text>
</g>
</svg>
"""


def write(out: Path) -> None:
    """Write the figures under `out/figures`."""
    figures = out / "figures"
    figures.mkdir(parents=True, exist_ok=True)
    for name in ("editor-operator-envs.png",):
        source = SHOTS / name
        if source.is_file():
            shutil.copy2(source, figures / name)
    (figures / "amp-envelope.svg").write_text(ENVELOPE, encoding="utf-8")
