---
description: Systematic Debugging - multi-hypothesis methodology for root cause analysis with confidence-ranked fallback chains
---

# SKILL: Systematic Debugging

## When to Use
When @dev or @qc encounters a bug, failing test, or unexpected behavior. **NEVER guess-and-check.** Follow this process.

## The Iron Law
> **Never fix what you don't understand.** Understand first, fix second.

## The Four Phases

### Phase 1: Root Cause Investigation
**Goal:** Understand WHAT is happening before trying to fix it.

1. **Reproduce:** Can you reliably trigger the bug? Document exact steps.
2. **Isolate:** Narrow down WHERE the bug occurs.
 - Which file? Which function? Which line?
 - Use binary search: comment out half the code, does the bug persist?
3. **Read the error:** Read the FULL error message and stack trace. Every word matters.
4. **Check recent changes:** What was the last change before the bug appeared? (`git log`, `git diff`)
5. **Contract Mismatch Check:** Compare actual data against `docs/tech/API_CONTRACTS.md` JSON schemas. Is the bug caused by a mismatch between implementation and contract (wrong field name, wrong type, missing field)? If yes, this is a contract violation — report to @sa.

**Output:** A clear statement: "The bug is in [file:function] because [specific cause]."

### Phase 2: Pattern Analysis
**Goal:** Understand WHY it's happening and if it's a systemic issue.

1. **Is this a known pattern?** Search web for the error message.
2. **Is this a recurring issue?** Check `.hc/bugs/` for similar past bugs.
3. **Is this a symptom of a deeper problem?** (e.g., a null from a broken API, not a missing null check)
4. **Are there other places with the same bug?** Search codebase with grep.

**Output:** Root cause confirmed, scope of impact assessed.

### Phase 3: Multi-Hypothesis Generation & Testing
**Goal:** Generate multiple fix hypotheses, rank by confidence, and test sequentially with a fallback chain.

> **Upgrade from linear debugging:** Instead of forming one hypothesis, generate 3 ranked alternatives. This prevents tunnel vision and provides automatic fallbacks.

1. **Generate 3 hypotheses:**
 - **H1 (Primary):** Most likely fix based on Phase 1-2 evidence. Confidence: [0-100]%
 - **H2 (Alternative):** Different approach to the same root cause. Confidence: [0-100]%
 - **H3 (Lateral):** Fix assuming the root cause in Phase 2 was wrong. Confidence: [0-100]%

2. **Rank by confidence** (Rule `decision-routing.md`):
 - Use evidence from Phase 1-2 to assign honest confidence scores.
 - Do NOT inflate scores to avoid branching.

3. **Test H1 first:**
 - Write a test that currently FAILS because of the bug.
 - Apply H1's fix.
 - If test passes → proceed to Phase 4.
 - If test fails → **revert H1**, document WHY it failed, proceed to H2.

4. **Fallback to H2, then H3:**
 - Same process: apply, test, revert if failed.
 - If H3 also fails → **escalate to @pm** with all 3 hypothesis results.

5. **Predict side effects** for the winning hypothesis:
 - What else might break? Check with full test suite.
 - If side effects are detected, document and mitigate BEFORE proceeding.

**Fallback Chain Visualization:**
```
H1 (70%) ──PASS──→ Phase 4
 │
 FAIL

H2 (20%) ──PASS──→ Phase 4
 │
 FAIL

H3 (10%) ──PASS──→ Phase 4
 │
 FAIL

 ESCALATE to @pm with full hypothesis report
```

**Output:** Winning hypothesis with confidence score, failed hypotheses with failure reasons.

### Phase 4: Implementation & Verification
**Goal:** Apply the fix and confirm it works without regressions.

1. **Make the minimal change:** Fix only what's broken. Do not refactor while debugging.
2. **Run the failing test:** It should now PASS.
3. **Run full test suite:** No regressions.
4. **Visual verify:** If UI-related, check with browser-visual-testing.
5. **Document:** Write bug report in `.hc/bugs/` with root cause and fix.

## Red Flags — STOP and Follow Process
If you catch yourself doing any of these, STOP and restart from Phase 1:
- "Let me just try changing this..."
- Making multiple changes at once without testing between them.
- "I don't know why, but this fix works."
- Spending more than 15 minutes without a clear root cause.
- Adding workarounds instead of fixing the actual bug.

## Bug Report Template
```markdown
# Bug Report: [Title]
**Date:** YYYY-MM-DD | **Reporter:** @dev/@qc
**Severity:** Critical/High/Medium/Low

## Symptoms
[What the user sees / what fails]

## Root Cause
[The actual underlying issue]

## Fix Applied
[What was changed and why]

## Tests Added
[Test file and test name]

## Verification
- [ ] Failing test now passes
- [ ] Full test suite passes
- [ ] Visual verification (if UI)
```
