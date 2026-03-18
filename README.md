# UniKey TSF Reborn

**UniKey TSF Reborn** is a modern, memory-safe Vietnamese keyboard (IME) built on the Windows Text Services Framework (TSF). It provides a 100% feature-complete clone of classic UniKey while completely eliminating the legacy `SetWindowsHookEx` architecture, resolving input lag, Chrome/Windows Terminal bugs, and UAC elevation issues.

## Key Features

- **Pure TSF Architecture:** Zero hooks, zero input lag.
- **100% UniKey Feature Parity:** Supports Telex, VNI, VIQR, macros, and the clipboard toolkit.
- **Ultra-lightweight:** Built with Pure C++17 (Win32 API + Windows COM), with a total footprint of less than 2MB.
- **Modern Windows Compatibility:** Works flawlessly in UWP/AppContainer applications via proper ACLs.

## Quick Links

- [Product Requirements Document (PRD)](./docs/biz/PRD.md)
- [Product Brief](./docs/biz/PRODUCT_BRIEF.md)
- [Architecture Overview](./docs/tech/ARCHITECTURE.md)
- [API Contracts / IPC](./docs/tech/API_CONTRACTS.md)
- [Deployment Guide](./docs/tech/DEPLOYMENT.md)
- [Test Plan](./docs/tech/TEST_PLAN.md)

## System Components

| Binary | Role |
|--------|------|
| `uktsf_core64.dll` | 64-bit TSF COM server |
| `uktsf_core32.dll` | 32-bit TSF COM server for legacy 32-bit apps |
| `UniKeyTSF.exe` | Win32 manager GUI + bootstrapper + system tray |

## Building from Source

This project requires **CMake** and **MSVC** (Visual Studio). The build system generates both x86 and x64 targets for full compatibility.

Run `build_all.bat` or `build_project.bat` to compile both 32-bit and 64-bit DLLs along with the main executable.

## License

This project is licensed under the GPL-3.0 License.
