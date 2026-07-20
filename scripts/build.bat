@echo off
setlocal EnableDelayedExpansion

echo.
echo ============================================================
echo  3D Game Engine ^| Build Script
echo ============================================================
echo.

rem ─── Parse arguments ─────────────────────────────────────────────────────────
set "BUILD_TYPE=%~1"
if "!BUILD_TYPE!"=="" set "BUILD_TYPE=Debug"

rem ─── Locate vcvars64.bat (VS 2022 / 2019 / 2017, any edition) ────────────────
set "VCVARS="
for %%V in (2022 2019 2017) do (
    if not defined VCVARS (
        for %%E in (BuildTools Enterprise Professional Community) do (
            if not defined VCVARS (
                for %%R in ("C:\Program Files (x86)" "C:\Program Files") do (
                    if not defined VCVARS (
                        set "_C=%%~R\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat"
                        if exist "!_C!" set "VCVARS=!_C!"
                    )
                )
            )
        )
    )
)

if not defined VCVARS (
    echo [ERROR] Could not find Visual Studio or Build Tools ^(2017-2022^).
    echo         Install Visual Studio 2017+ with "Desktop development with C++".
    exit /b 1
)

echo [build] Compiler env : !VCVARS!
call "!VCVARS!"
if !ERRORLEVEL! neq 0 (
    echo [ERROR] Failed to initialise MSVC environment.
    exit /b !ERRORLEVEL!
)

rem ─── Locate cmake (prefer VS-bundled, fall back to PATH) ─────────────────────
set "CMAKE_EXE="
for %%V in (2022 2019 2017) do (
    if not defined CMAKE_EXE (
        for %%E in (BuildTools Enterprise Professional Community) do (
            if not defined CMAKE_EXE (
                for %%R in ("C:\Program Files (x86)" "C:\Program Files") do (
                    if not defined CMAKE_EXE (
                        set "_CM=%%~R\Microsoft Visual Studio\%%V\%%E\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
                        if exist "!_CM!" set "CMAKE_EXE=!_CM!"
                    )
                )
            )
        )
    )
)

if not defined CMAKE_EXE (
    for /f "tokens=* usebackq" %%i in (`where cmake 2^>nul`) do (
        if not defined CMAKE_EXE set "CMAKE_EXE=%%i"
    )
)

if not defined CMAKE_EXE (
    echo [ERROR] cmake not found. Install Visual Studio C++ tools or add cmake to PATH.
    exit /b 1
)

echo [build] cmake         : !CMAKE_EXE!

rem ─── Locate ninja (prefer VS-bundled, fall back to PATH) ───────────────────
set "NINJA_EXE="
for %%V in (2022 2019 2017) do (
    if not defined NINJA_EXE (
        for %%E in (BuildTools Enterprise Professional Community) do (
            if not defined NINJA_EXE (
                for %%R in ("C:\Program Files (x86)" "C:\Program Files") do (
                    if not defined NINJA_EXE (
                        set "_NJ=%%~R\Microsoft Visual Studio\%%V\%%E\Common7\IDE\CommonExtensions\Microsoft\CMake\Ninja\ninja.exe"
                        if exist "!_NJ!" set "NINJA_EXE=!_NJ!"
                    )
                )
            )
        )
    )
)

if not defined NINJA_EXE (
    for /f "tokens=* usebackq" %%i in (`where ninja 2^>nul`) do (
        if not defined NINJA_EXE set "NINJA_EXE=%%i"
    )
)

if not defined NINJA_EXE (
    echo [ERROR] ninja not found. Ensure Visual Studio C++ tools are installed.
    exit /b 1
)

echo [build] ninja         : !NINJA_EXE!

rem ─── Paths ───────────────────────────────────────────────────────────────────
rem  %~dp0 = <repo>\scripts\  →  %~dp0.. = <repo>
pushd "%~dp0.."
set "SRC_DIR=%CD%"
popd
set "BUILD_DIR=!SRC_DIR!\out\build\windows-!BUILD_TYPE!"

echo [build] Source        : !SRC_DIR!
echo [build] Output        : !BUILD_DIR!
echo [build] Configuration : !BUILD_TYPE!
echo.

rem ─── Configure (only when no cache exists; delete build dir to force reconfigure) ──
if exist "!BUILD_DIR!\CMakeCache.txt" goto :do_build

echo [build] Configuring ...
"!CMAKE_EXE!" -S "!SRC_DIR!" -B "!BUILD_DIR!" ^
    -G Ninja ^
    "-DCMAKE_MAKE_PROGRAM=!NINJA_EXE!" ^
    -DCMAKE_BUILD_TYPE=!BUILD_TYPE! ^
    -DVCPKG_TARGET_TRIPLET=x64-windows
if !ERRORLEVEL! neq 0 (
    echo [ERROR] CMake configuration failed.
    exit /b !ERRORLEVEL!
)
echo.

:do_build
rem ─── Build ───────────────────────────────────────────────────────────────────
echo [build] Building ...
"!CMAKE_EXE!" --build "!BUILD_DIR!" --config !BUILD_TYPE! --parallel
if !ERRORLEVEL! neq 0 (
    echo [ERROR] Build failed.
    exit /b !ERRORLEVEL!
)

echo.
echo [build] SUCCESS ^| !BUILD_DIR!
