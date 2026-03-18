# Task: Decompose main.cpp (IPC & System Tray)

## Goal
Extract the Shared Memory IPC and System Tray notification logic from `src/main.cpp` into two dedicated Win32 modules (`src/core/ipc_manager` and `src/core/tray_service`).

## Architecture Context
- **Project type**: Windows TSF Input Method (C++ Win32).
- **Conventions**: No standard library bloated wrappers unless necessary; raw Win32 APIs (e.g., `CreateFileMapping`, `Shell_NotifyIconW`) are used heavily.
- **Constraints**: This is purely a refactoring task. DO NOT change existing program logic or user-facing behavior.

## File Map

### Files to CREATE
| File | Purpose |
|------|---------|
| `src/core/ipc_manager.h` | Class definition for managing Shared Memory mappings and synchronization mutexes. |
| `src/core/ipc_manager.cpp` | Implementation. Extract logic currently in `InitSharedMemory`, `CleanupSharedMemory`, and `GetConfigFilePath` from `main.cpp`. |
| `src/core/tray_service.h` | Class definition for managing the `NOTIFYICONDATA`, `Shell_NotifyIconW`, and `ShowContextMenu`. |
| `src/core/tray_service.cpp` | Implementation. Extract `InitTrayIcon`, `UpdateTrayIcon`, `RemoveTrayIcon`, `ShowContextMenu`, and `CreateLetterIcon` from `main.cpp`. |

### Files to MODIFY
| File | What to change |
|------|---------------|
| `src/main.cpp` | Remove the IPC and Tray logic and replace them with calls to instances of the new `IpcManager` and `TrayService` classes. Update header includes. |
| `CMakeLists.txt` | Add `src/core/ipc_manager.cpp` and `src/core/tray_service.cpp` to the executable build target. |

## Step-by-Step Implementation
### Step 1: Create Headers and cpp files
Create the `src/core` directory. Write the headers and move the function definitions from `main.cpp` into the new CPP files.

### Step 2: Refactor main.cpp
Replace all direct calls to `InitSharedMemory` etc. with method calls on your new objects. Make sure `main.cpp` continues to handle the `WM_TRAYICON` window messages, routing menu commands as required.

### Step 3: CMake
Update `CMakeLists.txt` to compile the new source files.

## Constraints
- DO NOT break the Global Keyboard Hook structure in `main.cpp`. It must remain intact.
- The IPC map must retain the exact same struct alignment (`UniKeyConfig`) because the TSF DLL (`tsf/text_service.cpp`) will still be reading from it using raw Win32 API.
- Do not add external dependencies.

**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
