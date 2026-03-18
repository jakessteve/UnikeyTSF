# Research Track Plan: UniKeyTSF Improvements

**Topic:** Comprehensive Suggestions for UniKeyTSF Improvements
**Date:** 2026-03-18

## Track 1: Architecture & Modularity
- **Research Question:** How can the monolithic `main.cpp` be safely decoupled, and how can the shared memory IPC be modernized without breaking the TSF integration?
- **Scope:** `src/main.cpp`, `src/tsf/text_service.cpp`, IPC mechanisms.
- **Agent Role:** `@sa` (Software Architect)
- **Output:** Structured findings + self-critique

## Track 2: Quality & Testing
- **Research Question:** What is the most effective way to introduce automated testing for the TSF COM interfaces and Win32 UI components, given the current C++ test suite?
- **Scope:** `tests/`, `src/tsf/`, `src/ui/`.
- **Agent Role:** `@qc` (Quality Control)
- **Output:** Structured findings + self-critique

## Track 3: UI/UX & Modernization
- **Research Question:** How can the settings dialog be modernized (e.g., dark mode) and made more accessible (ARIA), strictly adhering to Win32/C++ paradigms?
- **Scope:** `src/ui/settings_dialog.cpp`, `src/resource.h`, `src/UniKeyTSF.rc`.
- **Agent Role:** `@designer` (UI/UX Designer)
- **Output:** Structured findings + self-critique
