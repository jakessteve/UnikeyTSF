# Testing UniKeyTSF

## High-Level Strategy
Our E2E testing avoids mocking COM interfaces in C++ (fragile and high-maintenance). Instead, we build the actual `UniKeyTSF.exe` and test against it via UI Automation.

We use **PyWinAuto**, an automated testing wrapper around Windows UIAutomation hooks. By simulating real user clicks and reading text fields directly from the OS composed windows, we get true Blackbox Acceptance Testing. 

## Running E2E UI Tests

1. Install Python 3.8+ for Windows. Ensure `python` or `py` is on your `$PATH`.
2. Install testing dependencies in the project root:
   ```bash
   pip install -r tests/e2e/requirements.txt
   ```
3. Compile the `UniKeyTSF.exe` via `build_all.bat` (Debug configuration is usually placed in `build\Debug\UniKeyTSF.exe`).
4. Run the test suite:
   ```bash
   python tests/e2e/test_settings_ui.py
   ```

*Note: Since standard checkboxes expose standard UIA providers to Windows, tests using `pywinauto` can easily toggle things like `Bật kiểm tra chính tả` without needing deep internal IPC mocks.*
