param(
    [string]$Platform = "linux/amd64",
    [string]$Sanitizers = ""
)

$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
Push-Location $root
try {
    $name = $Platform -replace "/", "-"
    $image = "type-p-alpha-build:$name"
    $buildDir = "build/$name-clang"

    $cmakeArgs = @(
        "-DCMAKE_C_COMPILER=clang",
        "-DCMAKE_CXX_COMPILER=clang++",
        "-DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld",
        "-DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld",
        "-DCMAKE_MODULE_LINKER_FLAGS=-fuse-ld=lld"
    )
    if ($Sanitizers) {
        $buildDir += "-sanitize"
        $cmakeArgs += "-DP1_SANITIZERS=$Sanitizers"
    }
    $cmakeText = $cmakeArgs -join " "

    docker buildx build --platform $Platform --load -t $image `
        -f docker/Dockerfile .
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

    docker run --rm --platform $Platform -v "${PWD}:/work" -w /work $image `
        sh -c "cmake -S . -B '$buildDir' -G Ninja -DCMAKE_BUILD_TYPE=Release $cmakeText && cmake --build '$buildDir' && ASAN_OPTIONS=detect_leaks=0 UBSAN_OPTIONS=print_stacktrace=1 ctest --test-dir '$buildDir' -E 'linux_daws|plugin_editor_snapshot|plugin_editor_snapshot_diff' --output-on-failure"
    exit $LASTEXITCODE
} finally {
    Pop-Location
}
