---
description: Per-task verification checklist — run before declaring any dev task complete. Includes test running, build checks, and quality gates.
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
Run the project's test suite to capture baseline state:
```
[project test command — see docs/tech/TEST_PLAN.md]
```
- Capture the baseline test state. Note any pre-existing failures.
- After coding, compare results with this baseline.

### 2. Build / Compilation Check
Run the project's build or type-check command:
```
[project build/type-check command — see docs/tech/ARCHITECTURE.md]
```
- Must produce **zero errors**.
- If errors: fix them. Don't report done with build errors.

### 3. Test Suite
Run the project's test suite:
```
[project test command — see docs/tech/TEST_PLAN.md]
```
- All tests must **pass**.
- If you wrote new code, there MUST be new or updated tests covering it.
- If tests fail: fix the code (or the test if the test is wrong), then re-run.
- If writing new logic, check if corresponding tests exist. If not, create them.

### 4. Data Validation (if applicable)
- Run data validation if any data files were modified or new data files added.
- Use the project's data validation command if one exists.

### 5. Lint Check (if configured)
- Run the project's linter if one is configured.
- Fix any new lint errors introduced by your changes.

### 6. Dev / Runtime Smoke Test
- Start the application (dev server, executable, or equivalent).
- Verify it starts without errors.
- If UI changes: use `browser_subagent` or manual inspection to verify the output loads correctly.

### 7. Accessibility Quick Check (for UI changes)
- If UI changed, verify basic a11y: color contrast, keyboard navigation, alt text on images.
- Check for missing accessibility labels on interactive elements.
- Use platform-appropriate accessibility tools if available.

### 8. Performance Budget Check (for significant changes)
- If adding new dependencies: check binary/bundle size impact.
- If adding new components: verify no unnecessary overhead.
- If modifying data processing: verify no algorithmic complexity regressions (e.g., O(n²)).

### 9. Acceptance Criteria Cross-Check
- Re-read the story's acceptance criteria from `.hc/stories/`.
- Confirm each criterion is met by your implementation.
- If any criterion is not met: implement it before reporting done.

### 10. SOT Alignment
- Verify your implementation matches `docs/tech/API_CONTRACTS.md` (if API/contract changes).
- Verify your file placement matches `docs/tech/ARCHITECTURE.md`.
- If you deviated from SOT: update the SOT first (or flag to @sa).

---

## Quick Checklist (copy-paste into completion report)
```markdown
**Verification:**
- [ ] Baseline captured before coding
- [ ] Build/compile — 0 errors
- [ ] Tests — X/Y passing (no regressions from baseline)
- [ ] Lint — clean (if configured)
- [ ] Application runs — no startup errors
- [ ] A11y — basic checks pass (if UI changed)
- [ ] Performance — no bloat, no complexity regressions (if significant change)
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
