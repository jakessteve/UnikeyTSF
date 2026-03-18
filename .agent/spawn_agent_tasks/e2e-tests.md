# Task: Build E2E Acceptance Tests for Settings UI

## Goal
Implement a Blackbox UI Automation (E2E) test suite using `pywinauto` to launch `UniKeyTSF.exe`, interact with the Settings dialog, and verify that IPC/config changes take effect correctly.

## Architecture Context
- **Project type**: Windows TSF Input Method (C++ Win32). 
- **Testing Goal**: We want to test the actual application build by spinning up the EXE, rather than mocking COM interfaces in C++. 
- **Framework**: Python 3 with `pywinauto` (uia backend for standard Windows dialogs is fine, or win32 if needed). 

## File Map

### Files to CREATE
| File | Purpose |
|------|---------|
| `tests/e2e/test_settings_ui.py` | Python test script utilizing `pywinauto` or `unittest`. |
| `tests/e2e/requirements.txt` | Minimal Python dependency file containing `pywinauto`. |
| `docs/testing.md` | Quick instructions on how developers should run the E2E tests. |

### Files to MODIFY
| File | What to change |
|------|---------------|
| `CMakeLists.txt` | (Optional) Add a custom target `run_e2e_tests` if possible, otherwise leave it standalone. |

## Step-by-Step Implementation
### Step 1: Create Python Test Script
Create `tests/e2e/test_settings_ui.py`. It should:
1. Start `UniKeyTSF.exe` (or verify it is running).
2. Use `pywinauto` to locate the "UniKey TSF Reborn" hidden window or tray icon.
3. Open the "Bảng điều khiển..." (Settings Dialog) either by command line (`UniKeyTSF.exe --settings`) if it supports it, or by simulating right-click in tray. Wait, the main.cpp handles `--settings` ? If not, find the Settings window by class or title after it's launched. Assume sending a simulated hotkey (`Ctrl+Shift+F5`) or right-clicking tray might be hard. You can just execute `UniKeyTSF.exe` normally and simulate finding the Settings Dialog (Title: `UniKey TSF Reborn - Cấu hình`).
4. Interact with checkboxes (e.g., Enable Spell Check).
5. Close/Save the dialog.
6. Verify output or just confirm no crashes.

### Step 2: Documentation
Create `docs/testing.md` explaining that E2E tests are preferred over COM Mock tests. Explain how to set up `pywinauto` and run the script.

## Constraints
- Do NOT rewrite TSF C++ code. This is purely creating testing automation scripts.
- Assume the Python script will run on standard Windows 10/11 machines natively.

**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
