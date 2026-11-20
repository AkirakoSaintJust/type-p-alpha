# Manual

The chapters are Markdown; the theme is the screen and print CSS; `build.py`
writes HTML and a PDF to `build/manual/`.

```sh
pip install -r manual/requirements.txt
python manual/build.py
```

The PDF uses Edge, Chrome or Chromium. The pilot loads the fonts from Google
Fonts; the production build self-hosts the OFL files.

The chapter list is `manual/manifest.json`. The figures are copied from the
snapshot goldens and generated as SVG; `figures.py` owns them.

The rhythm gate measures prose. Remove the figures and patch cards before you run
it, because they are reference blocks and the Markdown meter counts a patch card as
one long sentence. The prose is the target.
