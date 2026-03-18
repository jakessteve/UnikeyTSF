# Research Report — UniKeyTSF Improvements
**Date:** 2026-03-18 | **Requested by:** User
**Research tracks:** 3 | **Agents involved:** `@sa`, `@qc`, `@designer`

## Executive Summary
This research swarm investigated architectural decoupling, quality assurance strategies, and UI modernization for UniKeyTSF. The primary finding is that the monolithic `main.cpp` and fragile IPC mechanisms are bottlenecks for both automated testing and future UI overhauls. The top recommendation is to immediately decouple the IPC and System Tray logic to enable reliable E2E integration tests.

## Ranked Recommendations

### P0 — Do Now
| # | Recommendation | Impact | Effort | Risk | Confidence | Track Source |
|---|---|---|---|---|---|---|
| 1 | **Decompose `main.cpp` (IPC & Tray)**: Split the 1300+ LOC monolith into `ipc_manager.cpp`, `tray_service.cpp`, and `hook_engine.cpp`. | High | Medium | Low | 9/10 | Track 1 (@sa) |

### P1 — Do Soon
| # | Recommendation | Impact | Effort | Risk | Confidence | Track Source |
|---|---|---|---|---|---|---|
| 2 | **E2E Acceptance Tests**: Build a Blackbox UI test suite (e.g., PyWinAuto) targeting Notepad to verify actual typing and IPC state, instead of mocking TSF COM interfaces. | High | High | Medium | 8/10 | Track 2 (@qc) |

### P2 — Consider Later
| # | Recommendation | Impact | Effort | Risk | Confidence | Track Source |
|---|---|---|---|---|---|---|
| 3 | **Pragmatic Dark Mode**: Implement basic Dark Mode using `DwmSetWindowAttribute` (title bar) and `WM_CTLCOLORDLG` for the Settings Dialog, strictly maintaining standard Win32 classes. | Medium | Low | Low | 7/10 | Track 3 (@designer) |

### P3 — Rejected (with rationale)
| # | Recommendation | Rationale | Track Source |
|---|---|---|---|
| 4 | **Owner-Drawn / Subclassed UI** | Fully owner-drawing the UI breaks standard Win32 UI Automation identifiers, preventing Track 2's E2E tests from working. | Track 3 (@designer / PM) |
| 5 | **COM Mocking for TSF Tests** | Mocking `ITfContext` and `ITfTextInputProcessorEx` in C++ is a massive time sink and extremely fragile. E2E tests provide better ROI. | Track 2 (@qc / PM) |

## Conflicts Resolved
- **UI Customization vs Automated Testing**: `@designer` proposed deep UI customization for Dark Mode. `@qc` noted this breaks standard UI Automation hooks necessary for tests. **Resolved**: Enforced pragmatic, lightweight Dark Mode (changing colors only, not Window Classes) to maintain testability.

## Methodology
- **Tracks researched**: 
  - Track 1 (@sa): Architecture & Modularity of IPC/Hooks.
  - Track 2 (@qc): Automated Testing for TSF COM and Win32 UI.
  - Track 3 (@designer): UI/UX Modenization in Win32.
- **Self-critique applied**: Yes (5 Lenses applied independently to each track).
- **Cross-review rounds**: 1 (Focusing on UI Automation vs Rendering conflicts).
- **Critical thinking models applied**: First Principles, Cynefin, Second-Order Effects, Opportunity Cost.

## Limitations
- Did not investigate migrating to modern IPC mechanisms (e.g., Named Pipes) as Shared Memory is currently functional and performs well for keyloggers/IMEs.
- Did not explore utilizing WebView2 for the UI, as it would significantly increase the installer footprint.

## Appendix: Full Track Reports
- [Track 1: Architecture](file:///c:/Users/HeoCop/Downloads/Projects/UnikeyTSF/.hc/research/2026-03-18-unikey-improvements-track-1.md)
- [Track 2: Quality & Testing](file:///c:/Users/HeoCop/Downloads/Projects/UnikeyTSF/.hc/research/2026-03-18-unikey-improvements-track-2.md)
- [Track 3: UI/UX & Modernization](file:///c:/Users/HeoCop/Downloads/Projects/UnikeyTSF/.hc/research/2026-03-18-unikey-improvements-track-3.md)
- [Cross-Review Discussion](file:///c:/Users/HeoCop/Downloads/Projects/UnikeyTSF/.hc/research/2026-03-18-unikey-improvements-cross-review.md)
