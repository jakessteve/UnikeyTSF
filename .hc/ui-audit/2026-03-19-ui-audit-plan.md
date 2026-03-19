# UI/UX & Business Flow Swarm - Track Plan (UnikeyTSF Win32 Native App)

**Date:** 2026-03-19
**Project:** UniKey TSF Reborn (Win32 Native App)

## Overview
Because this is a native C++ Win32 application and not a web application, the traditional `/spawn-ui` Web/Browser constraints are adapted for static code analysis of the Win32 GUI declarations, system tray logic, and user documentation.

## Track 1: Product & Interaction Flow
- **Lead:** @pm 
- **Focus:** Analyze `docs/biz/PRD.md`, `src/ui/settings_dialog.cpp`, and `src/ui/tray_icon.cpp`
- **Goal:** Evaluate the configuration hierarchy, tray menu usability, and whether the interaction model effectively handles modern scenarios (UWP apps, fast context switching).

## Track 2: Requirements & Content (Localization / Clarity)
- **Lead:** @ba
- **Focus:** Analyze `src/UniKeyTSF.rc` (String Tables) and `docs/biz/PRD.md`
- **Goal:** Review all user-facing strings, tooltip texts, dialog labels, and messaging for clarity, tone, and accuracy.

## Track 3: Visual Design Layout & System
- **Lead:** @designer
- **Focus:** Analyze `src/UniKeyTSF.rc` (Dialog structures, fonts, control alignment) and `src/ui/settings_dialog.cpp`
- **Goal:** Review dialog layout constraints, spacing, grouping (Group Boxes), and overall visual hierarchy of the Win32 dialogs to ensure it feels native, clean, and uncrowded.

## Track 4: Business Flow & Onboarding UX
- **Lead:** @biz
- **Focus:** Analyze `README.md` (Installation / Onboarding) and `src/ui/tray_icon.cpp`
- **Goal:** Evaluate the user's first-time setup experience (deployment steps), discoverability of the tray icon, and ease of accessing core features quickly without friction.
