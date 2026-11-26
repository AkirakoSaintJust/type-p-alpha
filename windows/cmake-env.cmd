@echo off
rem Import the Visual Studio C++ environment for CMake Tools. The workspace kit
rem points at this file through `environmentSetupScript`, so `rc.exe`, the
rem Windows SDK paths and the MSVC libraries are present for clang-cl and Ninja.
rem Do not use `setlocal` here: the caller needs the environment that this script
rem leaves behind.
set "VSWHERE=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if not exist "%VSWHERE%" (
    echo cmake-env: vswhere.exe not found 1>&2
    exit /b 1
)
set "VSROOT="
for /f "usebackq delims=" %%i in (`"%VSWHERE%" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath`) do set "VSROOT=%%i"
if not defined VSROOT (
    echo cmake-env: no Visual Studio C++ toolset found 1>&2
    exit /b 1
)
call "%VSROOT%\VC\Auxiliary\Build\vcvars64.bat" >nul
if errorlevel 1 (
    echo cmake-env: vcvars64.bat failed 1>&2
    exit /b 1
)
rem The CMake tools ship Ninja next to the VS installation, but that directory
rem is not on PATH in every shell.
set "VS_NINJA=%VSROOT%\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja"
if exist "%VS_NINJA%\ninja.exe" set "PATH=%VS_NINJA%;%PATH%"
