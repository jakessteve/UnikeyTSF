# Product Requirements Document — UniKey TSF Reborn

## Vision
100% feature-complete clone of classic UniKey, built on modern, memory-safe **Pure C++17 TSF** (Text Services Framework) architecture. Eliminates the legacy `SetWindowsHookEx` to resolve input lag, Chrome/Terminal bugs, and UAC issues.

## Architecture
- **Language:** Pure C++17 (Win32 API + Windows COM)
- **License:** GPL-3.0 (FOSS)

## System Components

| Binary | Role |
|--------|------|
| `uktsf_core64.dll` | 64-bit TSF COM server, injected by OS into host processes |
| `uktsf_core32.dll` | 32-bit TSF COM server for legacy 32-bit apps |
| `UniKeyTSF.exe` | Win32 manager GUI + bootstrapper + system tray |

## IPC: Shared Memory (Zero-Overhead)
- Named mapping: `Local\UniKeyTSF_SharedConfig`
- `#pragma pack(push, 1)` C-struct with current state
- EXE writes, DLLs read at O(1) per keystroke

## Core Functionalities

### Typing Engine
- **Methods:** Telex, VNI, VIQR
- **Charsets:** Unicode, TCVN3, VNI Windows
- **Behaviors:** Free tone marking, modern/classic tone placement (oà vs. òa), smart typo restoration, spell checking
- **TSF:** `ITfComposition` inline rendering, `ITfThreadMgrEventSink` for process blacklisting

### Win32 UI
- Classic dialog box (`DialogBoxParam`) — Main, Extensions, About tabs
- System tray icon (V/E), left-click toggle, right-click context menu
- Global hotkey (`Ctrl+Shift`) for E/V toggle

### Legacy Support
- `.ukm` macro file reading/expansion (case-sensitive)
- Clipboard toolkit: charset conversion, accent stripping (`Ctrl+Shift+F6/F9`)

## Strict Guardrails
1. **COM Memory Safety:** `ComPtr<T>` (`wrl/client.h`) for ALL COM objects. Manual `AddRef`/`Release` FORBIDDEN.
2. **AppContainer ACLs:** Registry entries MUST set ACLs with `ALL APPLICATION PACKAGES` (SID: `S-1-15-2-1`).
3. **UI Framework:** Pure Win32 API only. No MFC, Qt, Electron. EXE size < 2MB.
4. **Dual Architecture:** CMake/MSBuild MUST output both x86 and x64 DLLs.
5. **No JSON / No Named Pipes:** IPC via `CreateFileMappingW` with packed struct only.
