# Research Track 2: Quality & Testing

**Agent:** `@qc` (Quality Control)
**Date:** 2026-03-18
**Topic:** Automated Testing for TSF COM and Win32 UI

## Findings
1. **Current Coverage**: Core engine (`vn_engine.cpp`) is tested, but UI (`settings_dialog.cpp`) and TSF logic (`text_service.cpp`) have zero automated coverage.
2. **TSF Testing Pain**: The `text_service.cpp` implements `ITfTextInputProcessorEx` and interacts heavily with `ITfContext`. Mocking these COM interfaces in C++ (e.g., with GoogleMock) requires dozens of stub classes and is notoriously fragile.
3. **UI Testing**: The settings dialog uses standard Win32 dialog templates. Checking state programmaticly requires `GetDlgItem` and `SendMessage` loops.

## Recommendations
1. **Headless Win32 Dialog Tests**: Write a test suite that invokes `CreateDialog` off-screen, sends `BM_CLICK` messages to checkboxes, and verifies that `IpcManager` correctly updates the shared config.
2. **Acceptance Testing over Unit Testing for TSF**: Do not mock COM. Instead, build a tiny automated test client (using Microsoft's UI Automation or Python with `pywinauto`) that spins up Notepad, simulates physical keystrokes (`SendInput`), and asserts the final text output.

## Self-Critique (5 Lenses)
- **Reality Check**: Mocking COM is a known anti-pattern due to high maintenance overhead. The recommendation to avoid it is grounded in reality.
- **Effort vs Value**: Building an E2E PyWinAuto test is **High Effort** initially but yields incredibly **High Value** by testing the exact user flow.
- **Failure Modes**: E2E typing tests are notoriously flaky in CI environments due to window focus stealing. The tests must be engineered to forcefully lock focus.

## Confidence Score
**8/10**. The shift from unit tests to E2E acceptance tests for the OS-level integration is the only pragmatic approach for TSF development.

## Limitations
This track did not explore memory leak detection tools (like Application Verifier), which should also be part of the QC pipeline for a system-level DLL.
