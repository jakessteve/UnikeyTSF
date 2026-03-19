# Track 4 Report: Business Flow UX
**Agent:** @biz

## Findings
1. **Installation Friction:** The current deployment is purely manual (ZIP copy). To achieve mainstream adoption and trust, a proper MSI/AppX framework or setup executable is needed.
2. **Tray Icon Clarity:** The tray icon custom rendering logic (`CreateLetterIcon`) draws 'V' or 'E'. This perfectly matches user mental models of classic UniKey, maintaining low switching costs.
3. **Tray Menu Hierarchy:** The tray menu uses submenus for "Kiểu gõ" (Input Method) and "Bảng mã" (Charset). While functional, these require precise mouse movements. A single-click fast switch (or relying entirely on hotkeys) could improve speed.

## UX Severity
- Issue 1: Major (Barrier to entry for non-technical users)
- Issue 2: Enhancement (Already well-implemented, strong positive signal)
- Issue 3: Minor (Standard menu behavior, but could be refined)
