# Product Brief — UniKey TSF Reborn

## One-Liner
A modern, memory-safe Vietnamese keyboard built on Windows TSF — zero hooks, zero lag, 100% UniKey compatible.

## Target Audience
Vietnamese Windows users who need a fast, reliable, system-wide Vietnamese input method that works flawlessly in Chrome, Windows Terminal, and UAC dialogs.

## Key Differentiators
1. **Pure TSF architecture** — no `SetWindowsHookEx`, no input lag
2. **100% UniKey feature parity** — Telex, VNI, VIQR, macros, clipboard toolkit
3. **Ultra-lightweight** — pure Win32 API, < 2MB total footprint
4. **Modern Windows compatibility** — works in UWP/AppContainer apps via proper ACLs

## Core References
- Full requirements: [`PRD.md`](./PRD.md)
- Architecture: [`ARCHITECTURE.md`](../tech/ARCHITECTURE.md)
- IPC contracts: [`API_CONTRACTS.md`](../tech/API_CONTRACTS.md)
