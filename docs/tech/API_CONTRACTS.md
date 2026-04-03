# API Contracts

This project does not expose a network API. Its important contracts are **process, configuration, and registration contracts** inside the Windows IME runtime.

## 1. Shared Configuration Contract

**Source:** `src/shared_config.h`

`UniKeyConfig` is the authoritative cross-process settings payload shared between the manager EXE and TSF components through a named file mapping.

### Shared objects

- File mapping: `Local\UniKeyTSF_SharedConfig`
- Mutex: `Local\UniKeyTSF_SharedMutex`
- Config version constant: `UNIKEY_CONFIG_VERSION`

### Current fields

| Field | Meaning |
|---|---|
| `version` | Struct/schema version used for persisted config compatibility |
| `inputEnabled` | Vietnamese on/off state |
| `inputMethod` | Telex, VNI, or VIQR |
| `charset` | Unicode, TCVN3, or VNI Windows |
| `toneType` | Modern or classic tone placement |
| `spellCheck` | Enables spelling constraints in composition |
| `macroEnabled` | Enables macro expansion |
| `freeToneMarking` | Allows freer tone-key placement behavior |
| `toggleKey` | Current supported global toggle hotkey mode |
| `restoreKeyEnabled` | Enables restore/reconversion-related behavior |
| `useClipboardForUnicode` | Uses clipboard paste instead of pure Unicode injection |
| `showDialogOnStartup` | Opens the settings UI on startup |
| `perAppInputState` | Remembers VN/EN state by application scope in current hook-era logic |
| `macroFilePath` | Path to the active `.ukm` macro file |

### Ownership model

- `UniKeyTSF.exe` creates the shared mapping and is the write-capable owner.
- TSF components open the mapping and consume a read-oriented snapshot via `_UpdateConfig()`.
- Access is synchronized through the named mutex exposed by `LockConfig()` and `UnlockConfig()`.

## 2. Persistence Contract

**Source:** `src/config/ipc_manager.cpp`

Configuration is persisted to:

- `%APPDATA%\UniKeyTSF\config.dat`

Key rules:

- the manager initializes defaults if no valid persisted config is available
- persisted data is accepted only when the stored version matches `UNIKEY_CONFIG_VERSION`
- cleanup writes the current config snapshot back to disk

## 3. Process and Startup Contract

**Source:** `src/main.cpp`

The manager EXE currently defines the top-level runtime contract:

- single instance via `Local\UniKeyTSF_SingleInstance`
- `/register` registers the TSF DLLs
- `/unregister` unregisters the TSF DLLs
- normal startup ensures registration, initializes shared memory, creates tray/UI plumbing, and installs hooks
- startup routing mode can be overridden for guarded rollout and validation via command-line flags (`/routing-hook-primary`, `/routing-tsf-primary`, `/routing-fallback-scoped`) or the `UNIKEY_ROUTING_MODE` environment variable (`hook_primary`, `tsf_primary`, `fallback_scoped`)

This is a key reason the runtime must currently be described as hybrid rather than pure TSF.

## 4. TSF Registration Contract

**Source:** `src/engine/tsf_registration.cpp`, `src/engine/tsf_registration.h`

The registration helper contract is:

- `RegisterTsfDll()` registers the 64-bit in-proc server directly and the 32-bit server through `regsvr32.exe`
- `EnsureTsfDllRegistered()` compares the expected 64-bit DLL path against the registered CLSID path under `HKCU\Software\Classes\CLSID\...\InprocServer32`
- `UnregisterTsfDll()` removes the current TSF registration for both bitnesses

Registration success means the COM server is installed. It does **not** prove that TSF is the active typing path in a given app.

## 5. UI-to-Runtime Settings Contract

**Sources:** `src/ui/settings_dialog.*`, `src/ui/settings_webview.cpp`, `src/ui/settings_message_contract.*`, `src/config/ipc_manager.*`

The settings surface is expected to mutate the same `UniKeyConfig` contract used by both hook and TSF runtime components. The host bridge now enforces a typed JSON envelope with explicit message types for config updates, blacklist updates, startup state requests, and auto-start toggling. The modernization plan treats live propagation across these paths as a dedicated migration risk track.

## 6. Test Contract Surface

**Sources:** `tests/engine_test.cpp`, `tests/ipc_test.cpp`, `tests/clipboard_test.cpp`, `tests/e2e/settings_ui_test.cpp`, `tests/settings_message_contract_test.cpp`, `tests/routing_test.cpp`

The current automated contract surface covers:

- typing engine transforms and tone behavior
- clipboard and charset conversion logic
- shared-memory IPC creation, locking, and persistence behavior
- settings window lifecycle behavior

It does not yet prove full TSF-primary parity or app-matrix readiness.

## External Platform Contracts

The runtime is constrained by Windows platform behavior rather than HTTP/service APIs. The most relevant external contracts are:

- Windows TSF activation, context, and profile behavior
- COM registration and in-proc server loading rules
- low-level keyboard hook behavior and input injection limits

When updating docs or code, validate claims against those platform constraints and the current runtime files above.
