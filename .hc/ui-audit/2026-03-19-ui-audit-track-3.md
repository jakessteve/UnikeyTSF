# Track 3 Report: Visual Design System
**Agent:** @designer

## Findings
1. **Hardcoded Dark Theme:** `settings_dialog.cpp` uses a purely hardcoded dark theme (e.g., `RGB(18, 18, 18)`). It does not respect the system's light/dark mode preferences (`WM_SETTINGCHANGE`).
2. **Font Sizing Issue:** The `CreateDarkFont` function uses height `-20` for "Segoe UI" which translates to ~15pt. This is unusually large for standard Windows dialogs and overrides the `.rc` base font of 9pt, potentially causing text clipping on 100% scaling if the dialog isn't sized perfectly.
3. **Custom Button Drawing:** Owner-drawn buttons (`DrawDarkButton`) use fixed rounding (`6, 6`). This looks modern, but the focus ring is a simple 1px solid border (`CLR_BORDER_FOCUS`), which might not meet accessibility contrast ratios on a dark background.

## UX Severity
- Issue 1: Major (Forces dark mode on users who might prefer light mode)
- Issue 2: Major (Risk of UI clipping or disproportionate text on different DPIs)
- Issue 3: Minor (Custom drawing can look inconsistent with native Windows 11 controls)
