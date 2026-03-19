# Track 1 Report: Product & Flow Design
**Agent:** @pm

## Findings
1. **Settings Information Architecture:** The settings dialog groups 10+ options into a single 300x335 window without sub-tabs. Advanced options like "Blacklist" and "Macro" are visible immediately, adding cognitive load for basic users who just want to toggle Telex/VNI.
2. **Keyboard Shortcut Discoverability:** The essential toggle shortcuts (Ctrl+Shift+F6, F9) are placed as static text at the absolute bottom of the dialog. Users might miss them.
3. **Macro Configuration UI:** The macro UI only provides a file path selection. There is no inline way to edit the macro dictionary without opening the file manually.

## UX Severity
- Issue 1: Minor (It's a small app, but could be cleaner)
- Issue 2: Minor (Standard UniKey users know this, new users might miss it) 
- Issue 3: Major (Editing macros is a core workflow that requires external tools right now)
