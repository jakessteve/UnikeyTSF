---
description: Stress-test a feature with edge cases, boundary conditions, and adversarial inputs
---

# Battle Test Workflow

> Designed to break features before users do. Use after feature implementation is complete.

## Step 1 — Identify Test Surface

1. Read the feature's implementation files.
2. List all user inputs, API boundaries, and data transformations.
3. Identify the most complex logic paths.

## Step 2 — Edge Case Matrix

Generate test cases across these dimensions:

| Dimension | Examples |
|---|---|
| **Empty/null/undefined** | No data, first-time user, empty arrays, null dates |
| **Boundary values** | Min/max dates, 0, -1, MAX_INT, year 1900/2100 |
| **Large scale** | 1000+ items, very long strings, deep nesting |
| **Special characters** | Unicode, Vietnamese diacritics, emojis, HTML injection |
| **Calendar edge cases** | Leap years, leap months (lunar), Tết period, DST transitions |
| **Concurrent/timing** | Rapid clicks, double-submit, stale data, race conditions |
| **Format variations** | Different date formats, timezone offsets, locale differences |

## Step 3 — Execute Tests

For each dimension:
1. Write a test case (use existing test framework).
2. Run it.
3. Record result: Pass / Fail / Unexpected behavior.

```bash
npm test -- --testPathPattern="battle"
```

## Step 4 — Report

```markdown
# Battle Test Report: [Feature Name]
**Date:** YYYY-MM-DD | **Tester:** @qc

## Results
| # | Category | Test Case | Expected | Actual | Status |
|---|---|---|---|---|---|
| 1 | Empty data | Pass empty array to chart renderer | Show "no data" message | [actual] | / |

## Failures Found
[Detail each failure with steps to reproduce]

## Resilience Score
- Tests run: X | Passed: X | Failed: X
- Score: X% | Grade: A/B/C/D/F
```

## Step 5 — Fix Loop

For each failure:
1. @dev fixes the issue.
2. @qc re-runs the specific test.
3. Repeat until all pass.

Add battle test cases to the permanent test suite to prevent regression.

## Step 6 — Decision Gate

Based on the Resilience Score, determine next action:

| Score | Grade | Action |
|---|---|---|
| ≥90% | A | Route to `/implementation-review` (if part of `/idea-forge`) or ship |
| 75-89% | B | Ship with noted edge cases documented |
| 50-74% | C | @dev fixes critical failures → re-test → then proceed |
| <50% | D-F | REDESIGN — route back to `/party-mode` or `/idea-forge` Phase 2 with findings |

> **Integration:** When triggered from `/idea-forge`, this gate feeds directly into Phase 6 (Debate Implementation). The battle-test report becomes input for the `implementation-debate` skill.
