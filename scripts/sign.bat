@echo off
REM =============================================================================
REM UniKey TSF Reborn — Code Signing Script
REM =============================================================================
REM Usage: scripts\sign.bat <pfx_path> <password>
REM   Example: scripts\sign.bat certs\unikey.pfx MyPassword123
REM
REM Signs all production binaries with SHA-256 + RFC3161 timestamp.
REM =============================================================================

setlocal enabledelayedexpansion

if "%~1"=="" (
    echo Usage: %~nx0 ^<pfx_path^> ^<password^>
    echo.
    echo   pfx_path   Path to the .pfx code-signing certificate
    echo   password   Password for the .pfx file
    echo.
    echo To create a self-signed test certificate:
    echo   powershell -Command "New-SelfSignedCertificate -Type CodeSigningCert -Subject 'CN=UniKey TSF Dev' -CertStoreLocation Cert:\CurrentUser\My"
    echo   Then export from certmgr.msc to .pfx format.
    exit /b 1
)

set "PFX=%~1"
set "PWD=%~2"

if not exist "%PFX%" (
    echo ERROR: Certificate file not found: %PFX%
    exit /b 1
)

REM --- Locate signtool.exe ---
set "SIGNTOOL="
for /f "usebackq tokens=*" %%i in (`where signtool 2^>nul`) do (
    set "SIGNTOOL=%%i"
)

if not defined SIGNTOOL (
    REM Search Windows SDK paths
    for /d %%d in ("%ProgramFiles(x86)%\Windows Kits\10\bin\10.*") do (
        if exist "%%d\x64\signtool.exe" (
            set "SIGNTOOL=%%d\x64\signtool.exe"
        )
    )
)

if not defined SIGNTOOL (
    echo ERROR: Cannot find signtool.exe. Install Windows SDK.
    exit /b 1
)

echo Using signtool: !SIGNTOOL!
echo.

REM --- Timestamp server (DigiCert free RFC3161) ---
set "TSA=http://timestamp.digicert.com"

REM --- Files to sign ---
set "FILES=production\UniKeyTSF.exe production\uktsf_core64.dll production\uktsf_core32.dll"

REM Check for installer
if exist "production\UniKeyTSF_Setup.exe" (
    set "FILES=!FILES! production\UniKeyTSF_Setup.exe"
)

REM --- Sign each file ---
for %%f in (%FILES%) do (
    if exist "%%f" (
        echo Signing %%f ...
        "!SIGNTOOL!" sign /f "%PFX%" /p "%PWD%" /fd sha256 /tr "%TSA%" /td sha256 /v "%%f"
        if errorlevel 1 (
            echo FAILED to sign %%f
            exit /b 1
        )
        echo   OK
    ) else (
        echo SKIP: %%f not found
    )
)

echo.
echo ============================================================
echo  ALL FILES SIGNED SUCCESSFULLY
echo ============================================================
echo.
echo Verify with: signtool verify /pa production\UniKeyTSF.exe
echo.

endlocal
