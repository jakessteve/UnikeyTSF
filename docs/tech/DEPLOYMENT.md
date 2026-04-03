# Deployment Guide

UniKeyTSF is currently deployed as a **Windows desktop binary distribution**, not as a web or service deployment. The deployable runtime today is still a **hook-primary hybrid** manager application with TSF components present and registered.

## Supported Deployment Shape

- **Platform:** Windows 10/11
- **Primary executable:** `UniKeyTSF.exe`
- **TSF binaries:** `uktsf_core64.dll`, `uktsf_core32.dll`
- **Frontend assets:** `src/ui/frontend/dist/`
- **Primary output directory:** `build64/Release/` for x64 Release builds

## Prerequisites

- Visual Studio 2022 with the C++ Desktop workload
- CMake 3.20+
- Node.js 20+ for the WebView settings frontend build

## Build Commands

### Frontend

```powershell
npm ci --prefix src/ui/frontend
npm run build --prefix src/ui/frontend
```

### Native build

```powershell
cmake -B build64 -G "Visual Studio 17 2022" -A x64
cmake --build build64 --config Release
```

### Test graph

```powershell
ctest --test-dir build64 -C Release --output-on-failure
```

## Deployment Steps

1. Build the frontend and native binaries.
2. Verify the regression suites pass.
3. Copy `UniKeyTSF.exe`, `uktsf_core64.dll`, `uktsf_core32.dll`, `WebView2Loader.dll`, and frontend assets from `src/ui/frontend/dist/` into the release folder.
4. Place the release files in a permanent install location such as `C:\Program Files\UniKeyTSF\`.
5. Run `UniKeyTSF.exe /register` once as the user who will use the IME.
6. Run `UniKeyTSF.exe /tsf-diagnostics` to verify registration and activation state.
7. Start `UniKeyTSF.exe` normally to launch the tray application and current hook-primary hybrid runtime.

## Pre-Release Checklist

- [ ] Frontend build succeeds
- [ ] Release native build succeeds
- [ ] `ctest --test-dir build64 -C Release --output-on-failure` passes
- [ ] Docs and UI do not overclaim pure TSF behavior
- [ ] Blocking app matrix evidence is recorded for the current milestone

## Rollback Procedure

1. Exit the tray application.
2. Reinstall the previous known-good release files.
3. Run `UniKeyTSF.exe /register` again if the TSF binaries changed.
4. Verify startup, tray behavior, and `/tsf-diagnostics` output.
5. Record the rollback in `docs/log/INCIDENT_LOG.md` if the failed release reached users.
