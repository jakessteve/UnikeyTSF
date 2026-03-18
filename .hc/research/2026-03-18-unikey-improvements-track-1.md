# Research Track 1: Architecture & Modularity

**Agent:** `@sa` (Software Architect)
**Date:** 2026-03-18
**Topic:** Decoupling `main.cpp` and modernizing Shared Memory IPC

## Findings
1. **Monolithic `main.cpp`**: Currently 1304 LOC, handling `WH_KEYBOARD_LL`, System Tray (`NOTIFYICONDATA`), IPC Shared Memory mapping, and Vietnamese character injection logic (`ReplaceText`).
2. **Global State Dependency**: `g_rawCharCount`, `g_engine`, and multiple hook handles are accessed globally across the application, making unit testing impossible for the input interceptor.
3. **IPC Mechanism**: Uses raw `CreateFileMappingW` and `MapViewOfFile` with a `CreateMutexW` overlay. The TSF DLL reads this map to sync settings (e.g., Telex vs VNI).

## Recommendations
1. **Decompose into Services**:
   - `tray_service.cpp`: Manages `Shell_NotifyIconW` and Window class registration.
   - `input_interceptor.cpp`: Manages `SetWindowsHookEx` logic and encapsulates `VnEngine`.
   - `ipc_manager.cpp`: Encapsulates the shared memory mapping behind a clean interface (`IpcManager::ReadConfig`, `IpcManager::WriteConfig`).
2. **Modernize IPC**: Create a robust structural overlay (e.g., `SharedState` struct) and implement an event-driven sync (e.g., `CreateEvent`) instead of continuous polling or blocking mutexes.

## Self-Critique (5 Lenses)
- **Reality Check**: Splitting `main.cpp` is a standard and necessary C++ practice. However, `input_interceptor.cpp` still relies on OS-level hooks, so true cross-platform or decoupled testing remains hard without virtualizing the OS inputs.
- **Second-Order Effects**: Moving globals into class singletons or service locators will require updating all dependencies. If done incorrectly, `text_service.cpp` in the DLL will fail to synchronize, breaking the input method entirely.
- **Effort vs Value**: **Medium Effort**, **High Value**. Essential for long-term project survival.
- **Failure Modes**: The biggest risk is an IPC desync during typing. The C++ compiler won't catch logic errors in shared memory mutex deadlocks.

## Confidence Score
**9/10**. The architectural flaws are clear-cut typical Win32 monolithic patterns. The proposed decoupling strictly follows Separation of Concerns.

## Limitations
This track did not investigate migrating to a modern IPC like Named Pipes or COM out-of-proc servers, as shared memory is generally the fastest and most appropriate for low-level input hooks.
