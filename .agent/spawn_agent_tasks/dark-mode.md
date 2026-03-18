# Task: Pragmatic Dark Mode for Settings UI

## Goal
Implement a lightweight "Dark Mode" for the `UniKeyTSF` settings dialog using raw Win32 APIs, ensuring we do not break standard Windows UI Automation hooks (needed for E2E testing).

## Architecture Context
- **Project type**: Windows TSF Input Method (C++ Win32).
- **Goal**: Maintain the Win32 `HWND` properties so that screen readers and accessibility tools can still parse the checkboxes, but paint the backgrounds dark.

## File Map

### Files to MODIFY
| File | What to change |
|------|---------------|
| `src/ui/settings_dialog.cpp` | Intercept `WM_CTLCOLORDLG`, `WM_CTLCOLORSTATIC`, `WM_CTLCOLORBTN` in `SettingsDlgProc`. Return a dark brush (`HBRUSH`) and set the text color to white using `SetTextColor(hdc, RGB(255,255,255))` and `SetBkMode(hdc, TRANSPARENT)`. |
| `src/main.cpp` or `src/ui/settings_dialog.cpp` | Use `DwmSetWindowAttribute` (with `DWMWA_USE_IMMERSIVE_DARK_MODE` value 20 or 19 depending on Windows SDK) to force a dark title bar for the Settings Window (`hWnd`). |

## Step-by-Step Implementation
### Step 1: Create Global Brushes
In `settings_dialog.cpp`, on `WM_INITDIALOG`, create a static dark brush (e.g., `CreateSolidBrush(RGB(30,30,30))`).

### Step 2: Handle Paint Messages
Handle `WM_CTLCOLORDLG` and `WM_CTLCOLORSTATIC`. For static controls and checkboxes, change the DC text color to white and the background mode to `TRANSPARENT`, and return the dark brush. Ensure you don't break the return values for buttons if using themes, or just handle static controls.

### Step 3: Dark Title Bar
Include `<dwmapi.h>` and link `dwmapi.lib`. Apply `DwmSetWindowAttribute(hWnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &darkMode, sizeof(darkMode))` in `WM_INITDIALOG`.

## Constraints
- Do NOT owner-draw everything (`BS_OWNERDRAW`). Checkboxes must remain standard checkboxes where possible to ensure PyWinAuto testing works without writing custom UIA providers.
- Avoid introducing huge external UI libraries. Only use `<dwmapi.h>` and GDI.

**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
