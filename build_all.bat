@echo off
REM =============================================================================
REM UniKey TSF Reborn — Multi-Architecture Build Script
REM =============================================================================
REM Builds:
REM   1. x64: UniKeyTSF.exe + uktsf_core64.dll + UniKeyTests.exe
REM   2. Win32: uktsf_core32.dll
REM   3. Copies all outputs to production/
REM =============================================================================

setlocal enabledelayedexpansion

REM --- Locate CMake from Visual Studio ---
set "CMAKE="
for /f "usebackq tokens=*" %%i in (`"%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe" -latest -property installationPath 2^>nul`) do (
    set "VS_PATH=%%i"
)
if defined VS_PATH (
    set "CMAKE=%VS_PATH%\Common7\IDE\CommonExtensions\Microsoft\CMake\CMake\bin\cmake.exe"
)

if not exist "!CMAKE!" (
    echo ERROR: Cannot find cmake.exe. Please install Visual Studio with C++ CMake tools.
    exit /b 1
)

echo ============================================================
echo  UniKey TSF Reborn — Build All Architectures
echo ============================================================
echo.

REM --- Step 1: Build x64 ---
echo [1/3] Configuring x64...
"!CMAKE!" -B build64 -A x64
if errorlevel 1 (
    echo FAILED: x64 configure
    exit /b 1
)

echo [2/3] Building x64 (Release)...
"!CMAKE!" --build build64 --config Release
if errorlevel 1 (
    echo FAILED: x64 build
    exit /b 1
)

REM --- Step 2: Build Win32 (32-bit DLL only) ---
echo [3/3] Configuring Win32...
"!CMAKE!" -B build32 -A Win32 -DBUILD_32BIT=ON
if errorlevel 1 (
    echo FAILED: Win32 configure
    exit /b 1
)

echo       Building Win32 (Release)...
"!CMAKE!" --build build32 --config Release --target uktsf_core32
if errorlevel 1 (
    echo FAILED: Win32 build
    exit /b 1
)

REM --- Step 2.5: Build Frontend (React + Vite) ---
echo.
echo [Frontend] Building React UI...
pushd src\ui\frontend
call npm install
call npm run build
popd

REM --- Step 3: Copy to production ---
echo.
echo Copying to production/...
if not exist production mkdir production
if not exist production\frontend mkdir production\frontend

copy /Y build64\Release\UniKeyTSF.exe     production\ >nul
copy /Y build64\Release\uktsf_core64.dll  production\ >nul
copy /Y build32\Release\uktsf_core32.dll  production\ >nul
copy /Y build64\_deps\webview2-src\build\native\x64\WebView2Loader.dll production\ >nul
xcopy /E /I /Y src\ui\dist\*               production\frontend\ >nul

echo.
echo ============================================================
echo  BUILD COMPLETE
echo ============================================================
dir /B production\*.exe production\*.dll
echo.

REM --- Run tests ---
echo Running unit tests...
set "TESTS_FAILED=0"

echo [Test 1] Core Engine
build64\Release\UniKeyTests.exe --gtest_brief=1
if errorlevel 1 set "TESTS_FAILED=1"

echo [Test 2] IPC Manager
build64\Release\IPCTests.exe --gtest_brief=1
if errorlevel 1 set "TESTS_FAILED=1"

echo [Test 3] Clipboard Toolkit
build64\Release\ClipboardTests.exe --gtest_brief=1
if errorlevel 1 set "TESTS_FAILED=1"

echo [Test 4] Settings UI (E2E)
build64\Release\SettingsUITests.exe --gtest_brief=1
if errorlevel 1 set "TESTS_FAILED=1"

echo [Test 5] Characterization Contract
build64\Release\CharacterizationTests.exe --gtest_brief=1
if errorlevel 1 set "TESTS_FAILED=1"

if "!TESTS_FAILED!"=="1" (
    echo WARNING: Some tests failed!
) else (
    echo All tests passed.
)

endlocal
