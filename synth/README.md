# Type P.alpha patch and panel

`patch.json` is the voice graph and parameter manifest. The generated engine in
`../engine/gen` is built from it. The plugin in `../plugin` builds VST3, CLAP and
standalone through JUCE, and the web app in `../web` is the internal test surface.
`plugin.json` is the product identity.

`panel.json` is the legacy device panel control map from the original hardware
design. No current target builds it. The plugin editor and the web app read the
parameter manifest instead.


