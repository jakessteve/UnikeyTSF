# Test Plan — UniKey TSF Reborn

## Overview
Test strategy for a native C++17 Win32/COM application. Testing focuses on shared memory integrity, tray icon behavior, and (future) TSF keystroke processing.

## Constraints
- **No unit test framework in Phase 1.** Testing is manual + build verification.
- Unit testing will be introduced with GoogleTest in Phase 2 (TSF DLL).
- All tests are Windows-only (no cross-platform).

## Test Levels

### Build Verification (Phase 1)
- **Tool:** CMake + MSVC
- **Pass criteria:** Zero compiler errors, zero warnings at `/W4`
- **Command:** `cmake --build build --config Release`

### Manual Smoke Tests (Phase 1)
| # | Test | Expected Result |
|---|------|-----------------|
| 1 | Run `UniKeyTSF.exe` | Tray icon "V" appears |
| 2 | Left-click tray icon | Icon toggles V ↔ E |
| 3 | Right-click tray icon | Context menu appears |
| 4 | Press `Ctrl+Shift` | Icon toggles V ↔ E |
| 5 | Click "Exit" in menu | Tray icon removed, process exits |
| 6 | Run second instance | Detects existing instance, exits silently |

### Unit Tests (Phase 2+)
- **Tool:** GoogleTest
- **Location:** `tests/` directory
- **Coverage targets:**
  - Typing engine (tone placement, spell check): 90%+
  - Shared memory read/write: 80%+
  - COM lifecycle: 80%+

### Integration Tests (Phase 2+)
- TSF DLL keystroke processing in a mock `ITfContext`
- Shared memory producer-consumer between EXE and DLL

## Test Data
- Vietnamese word lists for typing engine validation
- Known-good tone placement reference data
- `.ukm` macro files for macro expansion testing
