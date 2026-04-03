# UniKey TSF Reborn — Roadmap

This roadmap tracks the modernization of **UniKey TSF Reborn** from its current **hook-primary hybrid** runtime toward a **TSF-primary** future state.

The current repo already contains substantial typing, TSF, shared-memory, UI, and test infrastructure. The remaining work is mainly about making runtime ownership, contracts, validation, and support claims coherent enough for a safe TSF-primary rollout.

## Current Runtime Truth

- `UniKeyTSF.exe` is still the long-lived manager process.
- The hook path remains the effective primary typing authority in current builds.
- The TSF service is implemented and registered, but it is not yet the sole routing authority.
- Shared memory and the WebView-backed settings UI are already present and in active use.

## Milestone 1 — Truth Alignment + CI Correctness

- [ ] Align `README.md`, `ROADMAP.md`, and engineering docs to the current hybrid runtime.
- [ ] Remove UI/runtime copy that incorrectly claims pure TSF.
- [ ] Fix CI to run the actual regression graph declared by CMake.

## Milestone 2 — Canonical Config Contract

- [ ] Unify the native and frontend understanding of `UniKeyConfig`.
- [ ] Define clear versioning and compatibility rules for config persistence and UI messaging.
- [ ] Document the config and settings contract as a single source of truth.

## Milestone 3 — WebView/Runtime Bridge Hardening

- [ ] Replace fragile string-based message parsing with explicit typed message handling.
- [ ] Add payload validation and negative-path handling.
- [ ] Add round-trip tests for settings propagation.

## Milestone 4 — Routing Ownership Hardening

- [ ] Enforce one authoritative input path per focused context.
- [ ] Stabilize routing diagnostics and fallback behavior.
- [ ] Prevent hook and TSF dual-processing regressions.

## Milestone 5 — Validation Matrix Consolidation

- [ ] Keep the regression suites green under the modernized runtime.
- [ ] Treat the blocking app matrix as release-gating evidence: Notepad, Edge textarea, VS Code, Windows Terminal, elevated Notepad, and WordPad/RichEdit.
- [ ] Require explicit routing and propagation evidence before any default flip.

## Milestone 6 — Shared Engine Semantics + Guarded TSF Rollout

- [ ] Align hook and TSF paths around shared reset/context/rewrite semantics.
- [ ] Enable TSF-primary behind a guarded rollout path with rollback.
- [ ] Promote TSF-primary only after validation evidence is complete.

## Non-Goals for the Current Program

- No big-bang rewrite.
- No cross-platform expansion.
- No “pure TSF” claim until runtime ownership and fallback policy actually support it.
