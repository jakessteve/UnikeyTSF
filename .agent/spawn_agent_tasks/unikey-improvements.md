# Task: Implement UniKeyTSF Improvements (TSF Composition & Blacklist UI)

## Context
You are `@dev`. An implementation plan has been approved to improve `UniKeyTSF` based on research into common bugs (like Chrome/Excel autocomplete conflict and Game input latency). 

1. **Robust TSF Composition:** In `src/tsf/composition.cpp`, the `StartComposition` method is mostly a stub returning `S_OK`. It needs to be properly implemented so that the composition is formally declared (e.g. tracking the character ranges of the syllable being built). This prevents desyncs in advanced editors.
2. **Blacklist UI Enhancement:** In `src/ui/settings_dialog.cpp` and `src/resource.h`, the current Blacklist UI only allows manual typing of the process name (`IDC_TXT_BLACKLIST_ADD`). You need to add a "Find Active Window / Browse" type functionality or add a "Pick Running Process" button, so users don't have to manually type `League of Legends.exe`.

## Scope & Files
You are authorized to modify:
- `src/tsf/composition.cpp`
- `src/ui/settings_dialog.cpp`
- `src/resource.h`
- `src/tsf/text_service.cpp` (if needed for the TSF changes)

## Constraints
- **DO NOT** fundamentally change the application to use a global `WH_KEYBOARD_LL` hook. The application must remain a modern TSF input method.
- Follow existing codebase patterns (C++, Win32 UI, TSF COM interfaces).
- **Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE`

## Verification
- Ensure the project continues to build without errors (using standard CMake or the `build_project.bat` script if available).
- Summarize exactly what changes were made when you consider the implementation complete.
