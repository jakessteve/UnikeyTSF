# Architecture Overview

## Runtime Summary

UniKeyTSF currently runs as a **hybrid Windows IME**:

- `UniKeyTSF.exe` is the long-lived manager process.
- A global low-level keyboard hook in `src/engine/hook_manager.cpp` is the effective primary typing path today.
- A TSF text service in `src/tsf/` is built and registered, but it is not yet the sole authoritative input path.
- Shared memory defined in `src/shared_config.h` and managed by `src/config/ipc_manager.cpp` carries live configuration between the manager and TSF components.

This means the repo should be described as **hook-primary hybrid**, not pure TSF.

## Modernization Direction

The modernization plan in `.sisyphus/plans/vietnamese-ime-modernization.md` targets a **TSF-primary** future state with explicit routing control and bounded fallback. That target state is not the current runtime truth.

## Main Components

### 1. Manager EXE

`src/main.cpp` owns process startup and lifecycle:

- enforces single-instance execution
- ensures the TSF DLL is registered
- initializes shared-memory configuration
- creates the hidden window and tray icon
- pre-initializes the WebView2 settings environment
- installs the keyboard hooks used by the current primary input path

### 2. Hook-Based Input Path

`src/engine/hook_manager.cpp` is the current production typing authority for most scenarios. It:

- intercepts keystrokes with `WH_KEYBOARD_LL`
- tracks toggle state and per-app state memory
- runs Vietnamese composition through `VnEngine`
- injects transformed output with `SendInput` or clipboard-based paste
- handles macro expansion and reconversion-style restore behavior

### 3. TSF Text Service

`src/tsf/text_service.cpp`, `src/tsf/key_event_sink.cpp`, and related files implement a Windows Text Services Framework text service. The TSF path:

- participates in TSF activation and focus events
- reads shared configuration from the manager-owned mapping
- maintains composition state inside TSF contexts
- provides the basis for the planned TSF-primary migration

Today, this path is substantial but not yet documented as the sole runtime authority.

### 4. Shared Configuration and Persistence

`src/shared_config.h` defines the packed `UniKeyConfig` struct shared across processes. `src/config/ipc_manager.cpp`:

- creates the shared file mapping and mutex
- loads and saves `%APPDATA%\UniKeyTSF\config.dat`
- exposes synchronized config access to the manager process
- grants the required security access for the current user and AppContainer packages

### 5. UI Surface

The current UI is native Win32 plus WebView-backed settings integration:

- tray and menu behavior in `src/ui/tray_icon.*`
- settings window/dialog plumbing in `src/ui/settings_dialog.*`
- WebView-backed settings bridge in `src/ui/settings_webview.cpp`

## Directory Map

```text
src/
├── config/        Shared-memory IPC, persistence, blacklist
├── engine/        Hook path, typing engine, macros, TSF registration helpers
├── tsf/           TSF COM text service implementation
├── toolkit/       Clipboard and conversion helpers
├── ui/            Tray icon, settings dialog, WebView bridge
└── main.cpp       Manager process entry point

tests/
├── engine_test.cpp
├── ipc_test.cpp
├── clipboard_test.cpp
├── spellcheck_test.cpp
├── settings_ui_test.cpp
└── e2e/settings_ui_test.cpp
```

## Supported/Blocking Validation Matrix

The modernization plan uses this app matrix as the default blocking set for behavior validation:

- Notepad
- Edge textarea
- VS Code editor
- Windows Terminal
- elevated Notepad
- WordPad or another RichEdit-style editor

This is the matrix the project should use when deciding whether TSF-primary behavior is ready to become the default.

## Architectural Constraints

- Do not claim pure TSF while the manager still installs hooks at startup.
- Do not treat TSF registration as equivalent to TSF-primary routing.
- Maintain one authoritative input path per focused context during modernization work.
- Keep manager, shared config, UI bridge, and TSF components aligned when describing behavior or support policy.
