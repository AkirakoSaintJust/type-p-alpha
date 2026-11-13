# Build platforms

The plugin is native code. The builds use clang everywhere: clang and lld on
Linux, clang-cl with lld-link on Windows, and AppleClang on macOS. Docker builds
the Linux targets and the WASM app. The Windows and macOS targets need their
native SDKs.

| Target | Where it builds | Compiler | Architecture |
| --- | --- | --- | --- |
| Linux x86_64 | `docker/build.sh linux/amd64` | clang + lld | x86_64 |
| Linux aarch64 | `docker/build.sh linux/arm64`, or `ubuntu-24.04-arm` | clang + lld | aarch64 |
| Windows x86_64 | `windows/build.ps1` | clang-cl + lld-link | x86_64 |
| macOS | `macos-14` | AppleClang | universal `arm64` and `x86_64` |
| Web app | `docker/Dockerfile.wasm` | Emscripten | WASM |

## Linux

```sh
docker/build.sh linux/amd64
```

On Windows PowerShell:

```powershell
.\docker\build.ps1 linux/amd64
```

The image is `docker/Dockerfile`. It carries clang, lld, clang-tidy,
clang-format, the JUCE dependencies and Xvfb. The build uses clang and lld with
`-fuse-ld=lld`, and writes to `build/linux-amd64-clang`.

The aarch64 build needs QEMU/binfmt on an x86_64 host, or a native arm64 host. On
Docker Desktop the emulation setting provides it. The release workflow uses the
native `ubuntu-24.04-arm` runner instead, because QEMU is much slower and the
runner is already arm64.

```sh
docker run --privileged --rm tonistiigi/binfmt --install arm64
docker/build.sh linux/arm64
```

The build image has Xvfb, so the editor snapshots can run in it:

```sh
docker run --rm -v "$PWD:/work" -w /work type-p-alpha-build:linux-amd64 \
  sh -c "ctest --test-dir build/linux-amd64-clang -R 'plugin_editor_snapshot' --output-on-failure"
```

The DAW matrix needs Ardour, Carla and JACK, which the build image does not carry.
Use a host with those tools, or the Linux CI runner. The snapshot goldens are
pinned to the Linux x86_64 container.

The default build excludes `linux_daws`, `plugin_editor_snapshot` and
`plugin_editor_snapshot_diff`.

## Validators

The VST3 runs through pluginval 1.0.4 and the CLAP through the CLAP validator
0.4.1, both under Xvfb. Download them into the ignored `build/` tree first.

```sh
xvfb-run -a build/pluginval/linux/pluginval --validate \
  "build/linux-amd64-clang/plugin/juce/type_p_alpha_artefacts/Release/VST3/Type P.alpha.vst3" \
  --strictness-level 5
xvfb-run -a build/clap-validator/linux/clap-validator validate --only-failed \
  "build/linux-amd64-clang/plugin/juce/type_p_alpha_artefacts/Release/CLAP/Type P.alpha.clap"
```

pluginval passes at strictness 5 with the editor open. The CLAP validator reports
no failures and one warning, because its random-state test loads garbage without a
crash.

## Windows

Docker on a Linux host cannot produce Windows binaries. clang-cl uses the MSVC
C++ ABI, so the Windows SDK, the MSVC headers and the MSVC libraries are still
required. The LLVM tools provide clang-cl and lld-link.

```powershell
.\windows\build.ps1
.\windows\build.ps1 -Linker msvc
.\windows\build.ps1 -Sanitizers address
```

The script imports the Visual Studio environment with `vswhere` and `vcvars64`,
then finds clang-cl in the Visual Studio LLVM, the LLVM install, or on PATH.
The default linker is lld-link. Use `-Linker msvc` if lld-link fails, and the
script switches the linker flag and the `CMAKE_LINKER` to `link.exe`.

The build directories are `build/windows-clangcl-lld` or
`build/windows-clangcl-msvc`, inside the repository. The snapshot diff is not
registered on Windows. The snapshot render still writes images, but the gate is
Linux only.

The MSYS2 GCC and the MSYS2 `clang64` toolchain use the Windows GNU ABI. Keep
MSYS2 for the shell and the Unix tools, but build the release with clang-cl and
the MSVC ABI.

## macOS

Docker cannot build macOS binaries. The release workflow uses `macos-14` and
AppleClang, and produces a universal VST3 and CLAP:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_OSX_ARCHITECTURES="arm64;x86_64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0
cmake --build build
```

The `lipo -info` check on the VST3 and CLAP binaries must report both `arm64` and
`x86_64`. The macOS job runs the portable tests and skips the snapshot tests and
the Linux DAW matrix.

## Clang tooling

The repository carries `.clang-format` and `.clang-tidy`. CMake writes
`compile_commands.json`, so clang-tidy can read the real compile flags.

```sh
clang-format -i plugin/juce/PluginEditor.cpp
clang-tidy -p build/linux-amd64-clang plugin/juce/PluginEditor.cpp
```

The Linux image and the Windows LLVM install carry `clang-tidy` and
`clang-format`. The configuration is a starting point rather than a CI gate,
because the tree predates it.

The sanitizers are a CMake option:

```sh
cmake -S . -B build/sanitize -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DP1_SANITIZERS=address,undefined
```

The Docker script passes it as the second argument, and the Windows script as
`-Sanitizers`. Use `address` alone where the platform UBSan runtime is not
available.

The sanitizer build instruments the engine, the editor and the tests, and skips
the VST3 and CLAP plugin formats. A sanitizer-instrumented shared module needs a
runtime that the host does not load, so build the plugin binaries with the normal
configuration.

## WASM

`docker/Dockerfile.wasm` pins Emscripten 6.0.13, the same version as the engine
parity check. The first web stage is in `wasm/`: the generated C engine compiles to
`build/web/p1.wasm`, and `wasm/parity.mjs` compares its 48 kHz render with the
native capture.

```sh
docker build -f docker/Dockerfile.wasm -t type-p-alpha-wasm .
docker run --rm -v "$PWD:/work" -w /work type-p-alpha-wasm sh -c "./wasm/build.sh"
```

The module is about 164 KB, has no imports and exports `memory` plus the wrapper
functions in `wasm/p1_wasm.c`. The AudioWorklet and the browser pages are in
`web/`: `index.html` redirects to `panel.html`, the full parameter panel, and
`bare.html` is the bare audio and parameter smoke test. The panel loads
`engine/gen/params.json`, groups the parameters into the six tabs and sends every
control through the worklet. It carries the algorithm graph, the output scope and
spectro toggle, the two-part patch page, the six-card FX page, the envelope graphs,
the sixteen-bar spectrum editors, the live `from wave` preview, the `op spectra mod`
A/B display and the per-operator tap scopes. A patch and part library keeps 64
multi-patches and 128 part slots in `localStorage` and exports them as JSON. The
engine runs at 48 kHz and the worklet resamples at the output boundary when the
context rate differs. The cosmetic pass against the mockup is the remaining work.
The JUCE editor itself is not a browser target.
