---
description: Safe backend refactoring workflow — coverage-first, green-stays-green, security re-scan
---

# WORKFLOW: /refactor-be

> **Persona:** @dev-be (load `.agent/roles/@dev-be.md` and Rule `backend-standards.md` before starting)
> **Trigger:** When @pm assigns a backend refactoring task, or when tech debt is surfaced in a dev-log.
> **Execution mode:** Strictly sequential. Never refactor code that doesn't have test coverage first.

---

## Phase 1 — Scope & Baseline
// turbo
1. Read the refactoring task or tech debt record. Understand what is being changed and why.
2. Run the full test suite. Record the baseline pass/fail count.
   - If baseline has **existing failures** → STOP. Escalate to @pm — you cannot refactor broken code. Fix first via `/test-all-be`.

## Phase 2 — Coverage Audit
3. Identify all files in the refactoring scope.
4. For each file, check for test coverage in `tests/`.
5. Document coverage gaps:
   ```
   Coverage Gap Report — [refactor-slug]
   - [file]: no tests → needs scaffold from @qc
   - [service]: partial coverage → specific untested paths listed
   ```
6. If critical coverage gaps exist → STOP. Request @qc to provide test scaffolds before proceeding.

## Phase 3 — Refactoring Execution (Green Stays Green)
7. Apply one refactoring transformation at a time (per `refactoring-patterns` skill). Do NOT batch multiple structural changes.
8. After EACH transformation, immediately run the affected test file. Tests must stay green.
9. If a transformation causes a test failure → revert that specific change, diagnose, and try a safer approach.
10. Repeat until all planned transformations are complete.

## Phase 4 — Full-Suite Verification
11. Run the COMPLETE test suite. Result must be ≥ baseline pass count (no regressions introduced).
12. If regressions appear → bisect to the specific transformation that caused them and fix before proceeding.

## Phase 5 — Post-Refactor Review
13. Auto-trigger `code-review-excellence` skill — verify correctness, DRY improvement, type safety maintained.
14. Run security preflight from `backend-standards.md` §5 on all modified files.
15. Verify `docs/tech/API_CONTRACTS.md` still matches the refactored implementation (external contracts must not change during refactoring).

## Phase 6 — Documentation & Hand-off
16. Update inline comments/JSDoc where the refactoring changed the "why" of the code.
17. Write dev-log → `.hc/logs/dev/be-refactor-[slug].md`. Record: what changed structurally, what tech debt was resolved, what coverage was added.
18. Hand off to @pm → @pm routes to @qc for independent verification before marking Done.

---

### Key Invariants
- **Red tests before refactor → blocked.** Refactoring cannot fix bugs. Fix bugs first.
- **Coverage gaps → blocked.** Untested code cannot be safely refactored.
- **One transformation at a time.** This is the cardinal rule of safe refactoring.
