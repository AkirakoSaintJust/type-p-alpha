# Build platforms

The plugin is native code, and the builds use clang everywhere: clang and lld on
Linux, clang-cl with lld-link on Windows, and AppleClang on macOS. Docker builds the
Linux targets, whilst the Windows and macOS targets need their native SDKs, which is
why neither one builds in a container.

| Target | Where it builds | Compiler | Architecture |
| --- | --- | --- | --- |
| Linux x86_64 | `docker/build.sh linux/amd64` | clang + lld | x86_64 |
| Linux aarch64 | `docker/build.sh linux/arm64`, or `ubuntu-24.04-arm` | clang + lld | aarch64 |
| Windows x86_64 | `windows/build.ps1` | clang-cl + lld-link | x86_64 |
| macOS | `macos-14` | AppleClang | universal `arm64` and `x86_64` |

## Linux

```sh
docker/build.sh linux/amd64
```

On Windows PowerShell:

```powershell
.\docker\build.ps1 linux/amd64
```

The image is `docker/Dockerfile`, and it carries clang, lld, clang-tidy,
clang-format, the JUCE dependencies and Xvfb, so the build uses clang and lld with
`-fuse-ld=lld` and writes to `build/linux-amd64-clang`.

The aarch64 build needs QEMU/binfmt on an x86_64 host, or a native arm64 host, and
on Docker Desktop the emulation setting provides it. The release workflow uses the
native `ubuntu-24.04-arm` runner instead, because QEMU is much slower and the runner
is already arm64.

```sh
docker run --privileged --rm tonistiigi/binfmt --install arm64
docker/build.sh linux/arm64
```

The build image has Xvfb, so the editor snapshots can run inside it:

```sh
docker run --rm -v "$PWD:/work" -w /work type-p-alpha-build:linux-amd64 \
  sh -c "ctest --test-dir build/linux-amd64-clang -R 'plugin_editor_snapshot' --output-on-failure"
```

The DAW matrix needs Ardour, Carla and JACK, which the build image doesn't carry,
so I'd use a host with those tools, or the Linux CI runner. The snapshot goldens are
pinned to the Linux x86_64 container, and the default build excludes `linux_daws`,
`plugin_editor_snapshot` and `plugin_editor_snapshot_diff`.

## Validators

Download pluginval 1.0.4 and the CLAP validator 0.4.1 into the ignored `build/`
tree first, because the VST3 runs through pluginval and the CLAP through the CLAP
validator, both under Xvfb.

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

Docker on a Linux host can't produce Windows binaries, because clang-cl uses the
MSVC C++ ABI and the Windows SDK, the MSVC headers and the MSVC libraries are still
required. The LLVM tools provide clang-cl and lld-link.

```powershell
.\windows\build.ps1
.\windows\build.ps1 -Linker msvc
.\windows\build.ps1 -Sanitizers address
```

The script imports the Visual Studio environment with `vswhere` and `vcvars64`,
then finds clang-cl in the Visual Studio LLVM, the LLVM install, or on PATH. The
default linker is lld-link, and if that fails, `-Linker msvc` tells the script to
switch the linker flag and the `CMAKE_LINKER` to `link.exe`.

The build directories are `build/windows-clangcl-lld` or
`build/windows-clangcl-msvc`, inside the repository, and the snapshot diff isn't
registered on Windows: the render still writes images, but I'd keep the gate on
Linux only, because that is where the goldens are pinned. The MSYS2 GCC and the
MSYS2 `clang64` toolchain use the Windows GNU ABI, so keep MSYS2 for the shell and
the Unix tools and build the release with clang-cl and the MSVC ABI.

## macOS

Docker can't build macOS binaries, so the release workflow uses `macos-14` and
AppleClang and produces a universal VST3 and CLAP:

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

The repository carries `.clang-format` and `.clang-tidy`, and CMake writes
`compile_commands.json`, so clang-tidy can read the real compile flags.

```sh
clang-format -i plugin/juce/PluginEditor.cpp
clang-tidy -p build/linux-amd64-clang plugin/juce/PluginEditor.cpp
```

The Linux image and the Windows LLVM install carry `clang-tidy` and `clang-format`.
I'd treat the configuration as a starting point rather than a CI gate, because the
tree predates it.

The sanitizers are a CMake option:

```sh
cmake -S . -B build/sanitize -G Ninja -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ \
  -DP1_SANITIZERS=address,undefined
```

The Docker script passes it as the second argument, and the Windows script as
`-Sanitizers`. Use `address` alone where the platform UBSan runtime isn't
available.

The sanitizer build instruments the engine, the editor and the tests, and skips the
VST3 and CLAP plugin formats. A sanitizer-instrumented shared module needs a runtime
that the host doesn't load, so build the plugin binaries with the normal
configuration.

CI runs ThreadSanitizer over `engine_parity`, `preset_smoke` and `rate_bridge`,
then AddressSanitizer and UndefinedBehaviorSanitizer over the full test set. The
editor snapshot runs with LeakSanitizer off because JUCE keeps its global
allocations until process exit; the memory and undefined-behaviour checks still
run for it.
