# Architecture Overview — UniKey TSF Reborn

## Project Type
Pure C++17 native Windows application (Win32 API + COM/TSF).

## Directory Structure
```
UnikeyTSF/
├── CMakeLists.txt           # Build system (x64 + Win32)
├── build_all.bat            # Build script for both architectures
├── src/
│   ├── main.cpp             # WinMain — EXE entry, tray, hotkeys, keyboard hook
│   ├── shared_config.h      # Packed IPC struct + constants (v2, 532 bytes)
│   ├── resource.h           # Win32 resource IDs
│   ├── UniKeyTSF.rc         # Resource script (icons, dialogs)
│   ├── engine/
│   │   ├── vn_engine.h/cpp  # Vietnamese typing engine (Telex/VNI/VIQR)
│   │   ├── vn_ortho.h/cpp   # Vietnamese orthography validator (syllable check)
│   │   └── macro.h/cpp      # .ukm macro file loading/expansion
│   ├── config/
│   │   └── blacklist.h      # File-based process blacklist (blacklist.txt)
│   ├── toolkit/
│   │   ├── clipboard.h/cpp  # Clipboard accent stripping + charset conversion
│   │   └── charset_convert.h # Pure charset conversion functions (no Win32 deps)
│   ├── ui/
│   │   └── settings_dialog.h/cpp # OLED dark mode settings dialog
│   └── tsf/
│       ├── text_service.h/cpp    # TSF COM server (dormant — hook is primary)
│       └── dllmain.cpp           # DLL entry point + COM registration
├── tests/
│   ├── engine_test.cpp      # 86 engine tests (Telex/VNI/VIQR/SpellCheck)
│   ├── spellcheck_test.cpp  # Spell check & typo restoration tests
│   └── clipboard_test.cpp   # 48 charset conversion tests
├── res/
│   ├── icon_v.ico           # Tray icon — Vietnamese mode
│   └── icon_e.ico           # Tray icon — English mode
└── docs/
    ├── biz/                 # PRD, Product Brief
    └── tech/                # Architecture, API Contracts
```

## Tech Stack
- **Language:** C++17 (MSVC)
- **UI:** Pure Win32 API (windows.h, .rc resource files, DialogBoxParam)
- **IPC:** Shared Memory via `CreateFileMappingW` / `MapViewOfFile`
- **Input Engine:** Global keyboard hook (`WH_KEYBOARD_LL`) + `SendInput` (KEYEVENTF_UNICODE)
- **COM:** Windows Runtime Library (`wrl/client.h`, `ComPtr<T>`) — dormant TSF path
- **Build:** CMake 3.20+ targeting Visual Studio 2022
- **Testing:** GoogleTest (fetched via CMake FetchContent)

## Component Architecture

```
┌──────────────────────────────────────────────────┐
│             UniKeyTSF.exe (Manager)              │
│  ┌────────────┐ ┌──────────┐ ┌───────────────┐  │
│  │  Tray Icon │ │ Settings │ │  Keyboard Hook│  │
│  │   (V/E)    │ │  Dialog  │ │(WH_KEYBOARD_LL)│ │
│  └─────┬──────┘ └────┬─────┘ └───────┬───────┘  │
│        │             │                │           │
│        │        ┌────▼────┐    ┌──────▼───────┐  │
│        │        │ Shared  │    │   VnEngine   │  │
│        │        │ Memory  │    │ (Telex/VNI/  │  │
│        │        │  (IPC)  │    │  VIQR+Ortho) │  │
│        │        └────┬────┘    └──────┬───────┘  │
│        │             │                │           │
│  ┌─────▼─────────────▼────────────────▼───────┐  │
│  │              Config Layer                   │  │
│  │  blacklist.txt | config.dat | macros.ukm    │  │
│  └─────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────┘
                       │ READS (shared memory)
       ┌───────────────┼───────────────┐
       │                               │
   ┌───▼────────┐              ┌───────▼────┐
   │ uktsf_core │              │ uktsf_core │
   │   64.dll   │              │   32.dll   │
   │  (TSF COM) │              │  (TSF COM) │
   │  [dormant] │              │  [dormant] │
   └────────────┘              └────────────┘
```

## Key Architectural Decisions
1. **Pure Win32 — No Frameworks:** EXE size < 2MB, zero runtime dependencies
2. **Shared Memory IPC:** O(1) config reads, no parsing, no syscalls per keystroke
3. **Keyboard Hook over TSF:** `WH_KEYBOARD_LL` provides reliable cross-app keystroke interception; TSF code exists but is dormant awaiting future activation
4. **Vietnamese Spell Check:** `VnOrtho::IsSyllableValid()` validates syllables against 100+ valid Vietnamese consonant/vowel/final combinations; invalid compositions auto-revert to raw keystrokes
5. **File-based Blacklist:** User-configurable `blacklist.txt` replaces hardcoded list; UI editor in expanded settings panel
6. **Dual-arch DLL builds:** x64 EXE registers both x86 and x64 COM servers
7. **ComPtr everywhere:** Memory-safe COM lifecycle via WRL, no manual ref counting
