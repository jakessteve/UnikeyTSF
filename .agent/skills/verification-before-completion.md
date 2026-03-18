---
description: Per-task verification checklist — run before declaring any dev task complete. Includes test running, a11y audit, and performance checks.
---

# SKILL: DEV VERIFICATION BEFORE COMPLETION

**Trigger:** Before @dev reports ANY task as "Done" — every single time, no exceptions. Also autonomously triggered BEFORE and AFTER every code change.

---

## When to Use
- After implementing a new feature or function.
- After fixing a bug.
- After refactoring code.
- Before responding with "Done" or "Complete."

---

## The Verification Checklist

Run ALL of these before declaring completion:

### 1. Baseline Capture (Before Coding)
```bash
npm test
```
- Capture the baseline test state. Note any pre-existing failures.
- After coding, compare results with this baseline.

### 2. TypeScript Compilation
```bash
npx tsc --noEmit
```
- Must produce **zero errors**.
- If errors: fix them. Don't report done with type errors.

### 3. Test Suite
```bash
npm test
```
- All tests must **pass**.
- If you wrote new code, there MUST be new or updated tests covering it.
- If tests fail: fix the code (or the test if the test is wrong), then re-run.
- If writing new logic in `src/utils/` or `src/services/`, check if corresponding tests exist in `test/`. If not, create them.

### 4. Data Validation (if applicable)
```bash
npm run validate:data
```
- Run if any data files were modified or new data files added.

### 5. Lint Check (if configured)
```bash
npm run lint
```
- Fix any new lint errors introduced by your changes.

### 6. Dev Server Smoke Test
```bash
npm run dev
```
- Verify the dev server starts without console errors.
- If UI changes: use `browser_subagent` to visually verify the page loads correctly.

### 7. Accessibility Quick Check (for UI changes)
- If UI changed, verify basic a11y: color contrast, keyboard navigation, alt text on images.
- Check for missing `aria-label` on interactive elements.
- Run `axe-core` in browser devtools if available.

### 8. Performance Budget Check (for significant changes)
- If adding new dependencies: check bundle size impact.
- If adding new components: verify no unnecessary re-renders.
- If modifying data processing: verify no O(n²) regressions.

### 9. Acceptance Criteria Cross-Check
- Re-read the story's acceptance criteria from `.hc/stories/`.
- Confirm each criterion is met by your implementation.
- If any criterion is not met: implement it before reporting done.

### 10. SOT Alignment
- Verify your implementation matches `docs/tech/API_CONTRACTS.md` (if API changes).
- Verify your file placement matches `docs/tech/ARCHITECTURE.md`.
- If you deviated from SOT: update the SOT first (or flag to @sa).

---

## Quick Checklist (copy-paste into completion report)
```markdown
**Verification:**
- [ ] Baseline captured before coding
- [ ] `tsc --noEmit` — 0 errors
- [ ] `npm test` — X/Y passing (no regressions from baseline)
- [ ] Lint — clean
- [ ] Dev server — runs, no console errors
- [ ] A11y — basic checks pass (if UI changed)
- [ ] Performance — no bundle bloat, no O(n²) (if significant change)
- [ ] Acceptance criteria — all met
- [ ] SOT alignment — matches
```

---

## QC Hand-off Requirement
> **CRITICAL:** This skill is a **@dev self-check** — it is **necessary but NOT sufficient** for task completion.

After @dev completes this checklist:
1. @dev MUST hand the task back to @pm (never report 'Done' directly to User).
2. @pm MUST switch to **@qc persona** and independently re-run this same checklist.
3. Only after @qc independently verifies all steps pass may @pm report Done to the User.
4. If @qc finds failures that @dev missed: route back to @dev to fix, then @qc re-verifies.

**Rationale:** @dev has blind spots for their own code. Independent QC catches regressions, missing edge cases, and assumptions that @dev overlooked.

---

## Rules
- **Never skip any step.** Even "trivial" changes can break things.
- **If ANY step fails:** Fix it before reporting done. Don't report with caveats.
- **Rule `execution-protocol.md` applies:** Paste actual command output, not "I ran it."
- **QC is mandatory:** @dev passing this checklist does NOT mean the task is done. @qc must independently verify.
