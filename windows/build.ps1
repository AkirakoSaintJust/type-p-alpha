param(
    [ValidateSet("lld", "msvc")][string]$Linker = "lld",
    [string]$Sanitizers = ""
)

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
        (Join-Path ${env:ProgramFiles} "LLVM\bin\clang-cl.exe"),
        ((Get-Command clang-cl -ErrorAction SilentlyContinue).Source)
    )
    return $candidates | Where-Object { $_ -and (Test-Path $_) } | Select-Object -First 1
}

Import-VsEnvironment

$clang = Find-ClangCl
if (-not $clang) {
    throw "clang-cl not found. Install the LLVM tools or the Visual Studio Clang component."
}
$clangDir = Split-Path -Parent $clang
$env:PATH = "$clangDir;$env:PATH"

$linkerName = if ($Linker -eq "lld") { "lld-link.exe" } else { "link.exe" }
$linkerPath = (Get-Command $linkerName -ErrorAction SilentlyContinue).Source
if (-not $linkerPath) { $linkerPath = Join-Path $clangDir $linkerName }
if (-not (Test-Path $linkerPath)) {
    throw "$linkerName not found. Install lld or the MSVC linker."
}
$linkerFlag = if ($Linker -eq "lld") { "lld" } else { "link" }

$root = Split-Path -Parent $PSScriptRoot
if ($root -match '[^\x00-\x7F]') {
    $short = (cmd /c "for %I in (`"$root`") do @echo %~sI" | Out-String).Trim()
    if ($LASTEXITCODE -eq 0 -and $short -and (Test-Path $short)) {
        $root = $short
    } else {
        Write-Warning "JUCE's juceaide needs an ASCII path. Build through a junction, for example C:\Users\Public\p1-alpha."
    }
}
$build = "build/windows-clangcl-$Linker"
if ($Sanitizers) { $build += "-sanitize" }

$cmakeArgs = @(
    "-S", ".",
    "-B", $build,
    "-G", "Ninja",
    "-DCMAKE_BUILD_TYPE=Release",
    "-DCMAKE_C_COMPILER=$clang",
    "-DCMAKE_CXX_COMPILER=$clang",
    "-DCMAKE_LINKER=$linkerPath",
    "-DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=$linkerFlag",
    "-DCMAKE_SHARED_LINKER_FLAGS=-fuse-ld=$linkerFlag"
)
if ($Sanitizers) { $cmakeArgs += "-DP1_SANITIZERS=$Sanitizers" }

Push-Location $root
try {
    cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    cmake --build $build
    if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }
    ctest --test-dir $build --output-on-failure
    exit $LASTEXITCODE
} finally {
    Pop-Location
}
