# Deployment Guide — UniKey TSF Reborn

## Prerequisites
- Windows 10/11 (x64)
- Visual Studio 2022 with C++ Desktop workload
- CMake 3.20+

## Build Commands
```powershell
# Configure
cmake -B build -G "Visual Studio 17 2022" -A x64

# Build Release
cmake --build build --config Release

# Output
#   build\Release\UniKeyTSF.exe
```

## Deployment (User Machine)
1. Copy `UniKeyTSF.exe` to a permanent location (e.g., `C:\Program Files\UniKeyTSF\`)
2. (Phase 2) Copy `uktsf_core64.dll` and `uktsf_core32.dll` alongside the EXE
3. Run `UniKeyTSF.exe` — it auto-registers the COM DLLs and creates the tray icon
4. (Optional) Add to `HKCU\...\Run` for auto-start on login

## Uninstall
1. Exit `UniKeyTSF.exe` from the tray menu
2. (Phase 2) Run `UniKeyTSF.exe /unregister` to remove COM entries
3. Delete the installation directory
