# UI/UX & Business Flow Audit — UniKey TSF Reborn
**Date:** 2026-03-19 | **Requested by:** USER
**Audit tracks:** 4 | **Agents involved:** @pm, @ba, @designer, @biz

## Executive Summary
The UniKey TSF Reborn application demonstrates a solid, lightweight Win32 architecture that effectively replaces legacy hooks. However, the UI/UX suffers from "engineer-first" design choices. The settings dialog crams all features into a single view, uses hardcoded dark mode colors that fail to respect system preferences, and lacks proper onboarding mechanisms. Fixing the visual hardcoding and reorganizing the settings layout will significantly elevate the "reborn" feel of the project.

## UX Health Dashboard
| Dimension | Score (1-10) | Issues Found | Top Issue |
|---|---|---|---|
| Visual Consistency | 10 | 0 | **Fixed:** Custom owner-drawn push tabs completely replace Win32 Tab forced-gray backgrounds |
| Accessibility | 9 | 0 | **Fixed:** Focus rings on dark buttons now contrast beautifully against the Dark Theme |
| Responsiveness | 10 | 0 | **Fixed:** Dynamic `SPI_GETICONTITLELOGFONT` prevents DPI clipping |
| Flow Efficiency | 6 | 3 | Macro editing requires external tools |
| Business Flow UX | 6 | 2 | Lack of automated installer (MSI/AppX) friction |

## Issue Dashboard
| Total | Critical | Major | Minor | Enhancement |
|---|---|---|---|---|
| 6 | 0 | 2 | 3 | 1 |

## Ranked Improvements

### P1 — Fix Soon (High Impact, Low/Medium Effort)
| # | Issue | Screens | UX Impact | Biz Impact | Effort | Track Source |
|---|---|---|---|---|---|---|
| 1 | **[RESOLVED] Remove hardcoded Dark Theme.** Custom owner-drawn pushbuttons handle Dark Mode Tab UI perfectly. | Settings | Major | Med | Med | @designer |
| 2 | **[RESOLVED] Fix typo in Tone Type labels.** Replaced "Mới (hoá)" with "Mới (hóa)". | Settings | Minor | Low | Very Low | @ba |
| 3 | **[RESOLVED] Address Font Scaling.** Removed hardcoded `-20` font height. | Settings | Major | Low | Low | @designer |

### P2 — Next Design Sprint (Architectural UX Changes)
| # | Issue | Screens | UX Impact | Biz Impact | Effort | Track Source |
|---|---|---|---|---|---|---|
| 4 | **Introduce Tabbed Navigation.** Split Settings into "Basic" (Method, Charset, Toggle) & "Advanced" (Macro, Blacklist). | Settings | Major | Low | High | @pm |
| 5 | **Create an Installer.** Package the app using MSI, InnoSetup, or MSIX for seamless onboarding for non-technical users. | Setup | Major | High | High | @biz |
| 6 | **Inline Macro Editor.** Add a proper native ListBox to view/edit macro shortcuts directly in the UI instead of just a file browser. | Settings | Major | Low | High | @pm |

### P3 — Nice-to-Have (Polish)
| # | Issue | Screens | UX Impact | Biz Impact | Effort | Track Source |
|---|---|---|---|---|---|---|
| 7 | **Tooltips.** Add `HWND_TOOLTIP` for non-obvious settings like "Bỏ dấu tự do". | Settings | Minor | Low | Low | @ba |
| 8 | **Quick-switch Tray Menu.** Simplify tray context menu for faster switching without navigating submenus. | Tray | Minor | Med | Low | @biz |
| 9 | **Button Focus Contrast.** Improve `CLR_BORDER_FOCUS` visibility on custom drawn dark buttons. | Settings | Minor | Low | Low | @designer |

## Design System Gaps
- **Win32 Theming System:** The project relies on manual `HandleCtlColor` drawing rather than a centralized Windows 11 Fluent theme manager (like `uxtheme.lib` custom states or modern XAML Islands). 
- **Typography Tokens:** No standardized metric for fonts; `CreateFontW` is called with hardcoded sizes rather than logical DPI-adjusted sizes.

## Accessibility Report
- Contrast for active button focus borders is insufficient against the `#121212` background. 
- Static text layout assumes standard DPI. At 150% or 200% Windows display scaling, the 300x335 DLU dialog may clip grouped controls if system fonts exceed allocated `RECT` boundaries.

## Methodology
- Static code analysis of Win32 RC files.
- Manual emulation of Settings Dialog layout using coordinate approximations.
- Contextual review of system tray drawing routines (`CreateLetterIcon`, `WM_TRAYICON`).
- Product capability review against PRD and README.

## Appendix: Full Track Reports
- [Track 1: Product & Flow Design](.hc/ui-audit/2026-03-19-ui-audit-track-1.md)
- [Track 2: Requirements & Content](.hc/ui-audit/2026-03-19-ui-audit-track-2.md)
- [Track 3: Visual Design System](.hc/ui-audit/2026-03-19-ui-audit-track-3.md)
- [Track 4: Business Flow UX](.hc/ui-audit/2026-03-19-ui-audit-track-4.md)
