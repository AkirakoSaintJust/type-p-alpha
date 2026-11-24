$ErrorActionPreference = "Stop"

function Import-VsEnvironment {
    if ($env:VSCMD_VER) { return }
    $vswhere = Join-Path ${env:ProgramFiles(x86)} "Microsoft Visual Studio\Installer\vswhere.exe"
    if (-not (Test-Path $vswhere)) {
        throw "vswhere not found. Install Visual Studio Build Tools with the C++ workload."
    }
    $vs = & $vswhere -latest -products * `
        -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
        -property installationPath
    if (-not $vs) { throw "No Visual Studio C++ toolset found." }
    $vcvars = Join-Path $vs "VC\Auxiliary\Build\vcvars64.bat"
    if (-not (Test-Path $vcvars)) { throw "vcvars64.bat not found under $vs." }
    cmd /c "`"$vcvars`" >nul && set" | ForEach-Object {
        if ($_ -match '^([^=]+)=(.*)$') {
            [Environment]::SetEnvironmentVariable($matches[1], $matches[2], 'Process')
        }
    }
}

function Find-ClangCl {
    $candidates = @(
        (Join-Path $env:VCToolsInstallDir "..\..\Llvm\x64\bin\clang-cl.exe"),
        (Join-Path ${env:ProgramFiles} "LLVM\bin\clang-cl.exe")
    )
    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) { return (Resolve-Path $candidate).Path }
    }
    throw "clang-cl not found. Install LLVM."
}

Import-VsEnvironment
$clang = Find-ClangCl
$clangDir = Split-Path -Parent $clang
$env:PATH = "$clangDir;$env:PATH"
$linker = (Get-Command lld-link.exe -ErrorAction SilentlyContinue).Source
if (-not $linker) { $linker = Join-Path $clangDir "lld-link.exe" }
if (-not (Test-Path $linker)) { throw "lld-link.exe not found." }

$root = Split-Path -Parent $PSScriptRoot
Set-Location $root
$baseline = Join-Path $root "build\windows-clangcl-lld\plugin"
$juce = (Resolve-Path "build\windows-clangcl-lld\_deps\juce-src").Path
$clap = (Resolve-Path "build\windows-clangcl-lld\_deps\clap-juce-extensions-src").Path

function Run-Bench([string] $label, [string] $dir) {
    Write-Output "==== $label"
    $phase = & "$dir\phase_map.exe" 2>&1
    $phaseExit = $LASTEXITCODE
    $phase | Select-String -Pattern "CPU baseline|all checks passed" |
        ForEach-Object { $_.Line }
    if ($phaseExit -ne 0) { throw "$label phase_map failed" }

    $spectral = & "$dir\spectral_bench.exe" 2>&1
    $spectralExit = $LASTEXITCODE
    $spectral | Select-String -Pattern "64 harmonics, 512|engine block, both parts|checksum" |
        ForEach-Object { $_.Line }
    if ($spectralExit -ne 0) { throw "$label spectral_bench failed" }
}

function Build-Bench([string] $name, [string] $flags) {
    $dir = "build\windows-$name"
    cmake -S . -B $dir -G Ninja `
        -DCMAKE_BUILD_TYPE=Release `
        -DCMAKE_C_COMPILER="$clang" `
        -DCMAKE_CXX_COMPILER="$clang" `
        -DCMAKE_LINKER="$linker" `
        -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld `
        -DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=lld `
        "-DCMAKE_C_FLAGS_RELEASE=$flags" `
        "-DCMAKE_CXX_FLAGS_RELEASE=$flags" `
        "-DFETCHCONTENT_SOURCE_DIR_JUCE=$juce" `
        "-DFETCHCONTENT_SOURCE_DIR_CLAP-JUCE-EXTENSIONS=$clap" `
        -DFETCHCONTENT_FULLY_DISCONNECTED=ON
    if ($LASTEXITCODE -ne 0) { throw "$name configure failed" }
    cmake --build $dir --target phase_map spectral_bench engine_parity -j 8
    if ($LASTEXITCODE -ne 0) { throw "$name build failed" }
    & "$dir\plugin\engine_parity.exe" | Out-Null
    if ($LASTEXITCODE -ne 0) { throw "$name engine_parity failed" }
    Run-Bench $name "$dir\plugin"
}

Run-Bench "baseline O2" $baseline
Build-Bench "avx2-o2" "/O2 /Ob2 /DNDEBUG /arch:AVX2"
Build-Bench "avx2-o3" "/O3 /Ob2 /DNDEBUG /arch:AVX2"
