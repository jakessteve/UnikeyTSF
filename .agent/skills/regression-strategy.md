---
description: Regression Strategy - risk-based test selection, smart regression planning, and suite maintenance
---

# SKILL: Regression Strategy

**Trigger:** When @qc plans regression test suites or selects tests for a specific change.

---

## When to Use
- Selecting which tests to run for a PR or code change.
- Preparing a full regression suite for a release.
- After a bug fix, deciding what else to re-test.
- Reviewing test suite health during sprint reviews.

---

## The 3-Step Regression Process

### Step 1: Change Impact Analysis
Before selecting tests, understand what changed:

1. **Read the diff:** Identify all changed files and their types.
2. **Map dependencies:** Which other files import/use the changed code?
3. **Classify the change scope:**

| Change Scope | Risk Level | Test Strategy |
|---|---|---|
| Utility function change | Medium | Unit tests for that function + all callers |
| Component change (UI) | Medium | Component tests + visual regression + E2E for affected flows |
| Shared dependency change | High | Full regression suite |
| CSS/style change | Medium | Visual regression across breakpoints (via `browser-visual-testing` skill) |
| Data file / engine change | High | Data validation + all affected calculation engines |
| Build/config change | High | Full smoke test suite |
| Test file change only | Low | Run only the modified test file |
| Documentation only | Low | No regression needed |

### Step 2: Test Selection (Risk-Based Prioritization)
Select tests from highest to lowest priority:

| Priority | Criteria | Run When | Examples |
|---|---|---|---|
| **P1 — Critical** | Core business logic, data integrity, engine calculations | Every change | Core engines, critical calculations, data processing |
| **P2 — High** | Key user flows, primary features, navigation | Every PR | Chart generation, PDF export, tab switching |
| **P3 — Medium** | Edge cases, secondary features, error handling | Pre-release | Leap month handling, timezone edge cases |
| **P4 — Low** | Cosmetic, preferences, settings | Major releases only | Theme switching, font size preferences |

**Quick Selection Guide:**
```
Single utility change → P1 tests for that function + P2 tests for callers
Component change → P1-P2 for the feature area + visual regression
Shared code change → P1-P3 full regression
Release candidate → P1-P4 full suite
```

### Step 3: Execute and Report
```markdown
## Regression Report — [PR/Release]
**Date:** YYYY-MM-DD | **QC:** @qc

### Change Impact
- Files changed: [N]
- Risk level: [Low/Medium/High]

### Tests Selected
| Priority | Tests Run | Passed | Failed | Skipped |
|---|---|---|---|---|
| P1 | N | N | N | N |
| P2 | N | N | N | N |
| P3 | N | N | N | N |

### Findings
- [Any failures or concerns]

### Verdict
[PASS / PASS WITH NOTES / FAIL — details]
```

---

## Regression Suite Maintenance
Perform quarterly regression suite health checks:

| Check | Action |
|---|---|
| Tests for deleted features | Remove them |
| Tests with changed requirements | Update assertions |
| Flaky tests (fail intermittently) | Investigate root cause (never just skip) — use `test-fixing` skill |
| Coverage gaps (untested critical paths) | Write new tests |
| Slow tests (>30s) | Optimize or move to nightly suite |

## Rules
- **Never skip a failing test.** Fix it or investigate it (skill `test-fixing.md`).
- **P1 tests must always pass.** A failing P1 test blocks the release.
- **Visual regression is mandatory for CSS changes.** Use `browser-visual-testing` skill.
- **Document regression decisions.** "Why didn't we test X?" should always have an answer.
