# Research Track 3: UI/UX & Modernization

**Agent:** `@designer` (UI/UX Designer)
**Date:** 2026-03-18
**Topic:** Modernizing Settings Dialog (Dark Mode, Accessibility) in Win32

## Findings
1. **Dark Mode in Win32**: Native Win32 does not support "Dark Mode" out of the box for standard dialog controls (buttons, checkboxes). To implement it, the app must handle `WM_CTLCOLORDLG` and `WM_CTLCOLORSTATIC` and paint custom backgrounds. For Title Bars, undocumented Windows 10 APIs (`DwmSetWindowAttribute` with `19` or `20`) must be used.
2. **Accessibility (A11y)**: Win32 resource files (`.rc`) do not have ARIA labels. Accessibility relies on the Z-order of `LTEXT` controls preceding `EDIT` controls or exposing `IAccessible` COM interfaces.

## Recommendations
1. **Pragmatic Dark Mode**: Implement basic Dark Mode using `DwmSetWindowAttribute` for the title bar, and intercept `WM_CTLCOLORDLG` to return a dark brush. Override text colors. Do *not* attempt to fully owner-draw complex controls (like Tab Controls or Scrollbars) as the effort far outweighs the benefit.
2. **Z-Order Accessibility**: Ensure the `.rc` file properly orders Static Text labels immediately before their respective Checkboxes/Edit controls, granting default Narrator compatibility without writing COM wrappers.

## Self-Critique (5 Lenses)
- **Reality Check**: Raw Win32 is hostile to modern UX. Any attempt to make it look like a sleek React/Vite app will require rewriting the UI in WinUI 3 or WPF.
- **Scope Creep**: Completely rewriting the UI layer violates the "strictly adhere to Win32/C++" constraint. The pragmatic approach correctly bounds the scope.
- **Second-Order Effects**: Hardcoding dark brushes might break if Windows High Contrast mode is enabled. Must check `SystemParametersInfo` for `SPI_GETHIGHCONTRAST`.

## Confidence Score
**7/10**. Win32 UI modernization is a game of diminishing returns. The recommendations provide 80% of the value for 20% of the effort.

## Limitations
Did not explore embedding a WebView2 control for the settings UI, which is a modern alternative but increases binary size significantly.
