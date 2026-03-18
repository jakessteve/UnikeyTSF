# UniKey TSF Reborn

**UniKey TSF Reborn** is a modern, memory-safe Vietnamese keyboard (Input Method Editor) built purely on the Windows Text Services Framework (TSF). 

It is designed to be a 100% feature-complete clone of classic UniKey, but built from the ground up to completely eliminate the legacy `SetWindowsHookEx` architecture. This resolves long-standing issues such as input lag, typing bugs in Chrome and Windows Terminal, and UAC elevation limitations.

---

## 🚀 Key Features

- **Pure TSF Architecture:** Zero hooks, zero input lag, and proper modern application support.
- **100% UniKey Feature Parity:** Supports all major typing methods (Telex, VNI, VIQR), macros (`.ukm`), and the classic clipboard toolkit.
- **Ultra-lightweight:** Built with Pure C++17 utilizing the Win32 API and Windows COM, resulting in a total footprint of less than 2MB.
- **AppContainer / UWP Native:** Works flawlessly in UWP and Windows Store apps via properly configured ACLs (`S-1-15-2-1`).
- **Memory Safety Guaranteed:** Uses `wrl/client.h` (`ComPtr<T>`) for strict COM object lifecycles—no manual memory management.

## 🏗️ Architecture & Components

The application uses an O(1) shared memory IPC model (`Local\UniKeyTSF_SharedConfig`) to communicate instantly between the Manager GUI and the injected TSF DLLs without JSON parsing or named pipe overhead.

```mermaid
graph TD
    A["UniKeyTSF.exe<br>Manager & Tray"] -->|Writes Config| B[("Shared Memory IPC<br>Zero-Overhead struct")]
    B -->|Reads Config O(1)| C("uktsf_core64.dll<br>64-bit TSF COM")
    B -->|Reads Config O(1)| D("uktsf_core32.dll<br>32-bit TSF COM")
    A --> E["Config Layer<br>blacklist.txt, macros.ukm"]
```

### Tech Stack
- **Language:** C++17 (MSVC)
- **UI:** Pure Win32 API (`windows.h`, `.rc` resource files)
- **IPC:** `CreateFileMappingW` / `MapViewOfFile`
- **COM Lifecycle:** Windows Runtime Library (`wrl/client.h`)
- **Build System:** CMake 3.20+ targeting Visual Studio 2022
- **Testing:** GoogleTest

## 🛠️ Build & Deployment

### Prerequisites
- Windows 10/11 (x64)
- Visual Studio 2022 with **C++ Desktop workload**
- CMake 3.20+

### Building from Source

The build system automatically generates both x86 and x64 targets to ensure full compatibility across 32-bit and 64-bit applications.

```powershell
# 1. Configure the project for Visual Studio 2022
cmake -B build -G "Visual Studio 17 2022" -A x64

# 2. Build the Release configuration
cmake --build build --config Release
```

*Alternatively, you can run the provided `build_all.bat` script to compile both architectures automatically.*

### Deployment
1. Copy the resulting `UniKeyTSF.exe` to a permanent location (e.g., `C:\Program Files\UniKeyTSF\`).
2. Run `UniKeyTSF.exe`. It will automatically register the necessary COM DLLs and spawn the system tray icon.
3. To uninstall, exit the application from the tray menu, run `UniKeyTSF.exe /unregister`, and delete the folder.

## 📚 Documentation

Detailed documentation is available in the `docs/` folder:

- **Business / Product**
  - [Product Requirements Document (PRD)](./docs/biz/PRD.md)
  - [Product Brief](./docs/biz/PRODUCT_BRIEF.md)
- **Engineering**
  - [Architecture Overview](./docs/tech/ARCHITECTURE.md)
  - [API Contracts / IPC](./docs/tech/API_CONTRACTS.md)
  - [Deployment Guide](./docs/tech/DEPLOYMENT.md)
  - [Test Plan](./docs/tech/TEST_PLAN.md)

## ⚖️ License

This project is licensed under the **GPL-3.0 License**, ensuring it remains free and open-source software (FOSS).
