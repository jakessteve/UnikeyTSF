# UniKey TSF Reborn — Roadmap

This document outlines the strategic phases for developing and stabilizing **UniKey TSF Reborn**. The project aims for 100% feature parity with classic UniKey while utilizing the modern Windows Text Services Framework (TSF).

## Phase 1: Core TSF Infrastructure & IPC 
- [x] Set up MSBuild/CMake environment for dual architecture (x86 & x64).
- [x] Implement Pure C++17 TSF COM servers (`uktsf_core64.dll`, `uktsf_core32.dll`).
- [x] Establish zero-overhead Shared Memory IPC (`Local\UniKeyTSF_SharedConfig`).
- [x] Implement system tray manager UI (`UniKeyTSF.exe`) using Win32 API.

## Phase 2: Typing Engine Implementation 
- [ ] Implement Telex, VNI, and VIQR algorithms.
- [ ] Integrate TCVN3 and VNI Windows character sets.
- [ ] Support free tone marking and modern/classic tone placement (e.g., oà vs. òa).
- [ ] Add smart typo restoration and spell checking.
- [ ] Test `ITfComposition` inline rendering fidelity.

## Phase 3: Legacy Features & Advanced Support 
- [ ] Implement `.ukm` macro file reading and dynamic expansion.
- [ ] Develop the Clipboard Toolkit (charset conversion, accent stripping) via global hotkeys (`Ctrl+Shift+F6/F9`).
- [ ] Apply AppContainer ACLs (`S-1-15-2-1`) for UWP compatibility.
- [ ] Refine `ITfThreadMgrEventSink` for process blacklisting (gaming mode).

## Phase 4: Quality Assurance & Optimization 
- [ ] Comprehensive unit and integration testing.
- [ ] Validate memory safety boundaries (`ComPtr<T>` enforcement).
- [ ] Resolve cross-process registry/ACL synchronization issues.
- [ ] Verify latency / input lag improvements over legacy hooks.

## Phase 5: Production Release 
- [ ] Finalize documentation.
- [ ] Prepare deployment scripts / installer framework.
- [ ] Launch v1.0.0.
