# Editor snapshot goldens

`plugin_editor_snapshot` renders every editor page and dialog to PNG. The
`plugin_editor_snapshot_diff` CTest compares the new images with this golden set.
It fails when an image is missing, extra, or different.

The gate runs only in the Linux container. Font rasterisation differs on other
hosts, so a Windows run is a manual review until a Windows golden set exists.

## Intended UI changes

1. Build and run the snapshot test. The new PNGs are in the build directory
   under `juce/snapshots`.
2. Open the changed images and the diff overlays in
   `juce/snapshot-diff` (red marks the changed pixels, side by side shows old and
   new).
3. If the change is intended, accept it:

   ```sh
   python3 plugin/tools/snapshot_check.py --update \
     --actual <build>/juce/snapshots \
     --golden plugin/tests/snapshots/linux-x86_64
   ```

4. Commit the new golden PNGs, `manifest.json`, and the code change in the same
   commit. The commit message says which pages changed and why.

Do not accept a snapshot change you cannot explain. A red pixel in the diff is a
question rather than an error.

## Unintended UI changes

Run the CTest. If it fails, inspect the diff. A failure means the editor changed
without the design being updated, or a dependency changed the rendering. Fix the
code or record the design change and update the golden set deliberately.

## Adding a page or dialog

Add the render to `plugin/tests/editor_snapshot.cpp`, run the test, review the
new image, and update the golden set. The manifest records the image size and
SHA-256.

## Committed goldens

The gate needs a baseline that travels with the code, because a hash alone would
tell you that something changed without telling you what changed. The PNGs are
small (about 560 KB for the current set), and they give the reviewer a visual
history in git.
