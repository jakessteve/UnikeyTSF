# Cross-Review Summary

**Date:** 2026-03-18
**Topic:** UniKeyTSF Improvements

## Round Table Discussion (Facilitated by `@pm`)

**@pm to @designer:** "@qc suggested using PyWinAuto/UI Automation for E2E testing of the Settings Dialog. If you implement Dark Mode by heavily subclassing or owner-drawing standard Win32 checkboxes and buttons, will that break UI Automation identifiers?"
**@designer:** "Yes, if we completely replace the standard `BUTTON` classes with custom painted windows, Screen Readers and UI Automation tools won't know they are interactable. We must ensure that any custom drawing happens via `WM_CTLCOLORSTATIC` on the original standard controls rather than replacing them."

**@pm to @sa:** "@designer wants to modernize the UI, and you want to extract the Tray Icon and IPC into separate files. Does the UI modernization impact your refactoring?"
**@sa:** "Only slightly. The IPC refactoring (`ipc_manager.cpp`) needs to be completed *before* `@qc` writes the E2E tests, because the tests rely on verifying that the UI correctly updates the shared memory config."

## Conflict Log

| Track A Finding | Track B Finding | Conflict | Resolution |
|---|---|---|---|
| Track 3: Dark Mode requires custom drawing or subclassing. | Track 2: E2E Tests rely on standard Win32 UI Automation. | Deep UI subclassing breaks accessibility tools and automated tests. | **Resolved**: Enforce lightweight Dark Mode via `WM_CTLCOLORDLG`/`WM_CTLCOLORSTATIC` only. Keep standard Win32 classes to preserve UIA compatibility. |
| Track 1: Refactor `main.cpp` into IPC Service. | Track 2: Write E2E tests for the UI. | Tests cannot reliably verify state if the IPC is tightly coupled to the message loop. | **Resolved**: Prioritize Track 1's IPC refactoring as a prerequisite for Track 2. |
