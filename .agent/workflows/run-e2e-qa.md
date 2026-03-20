---
description: Automated E2E QA — Browser Subagent simulates real user flows, captures failures
---

# Workflow: /run-e2e-qa

End-to-end testing via Browser Subagent — simulate real user interactions, test edge cases, and capture failures.

---

## Prerequisites
- App running on `localhost` (typically via `npm run dev`)
- Browser Subagent available
- User flows defined in stories (`.hc/stories/`) or test plans (`.hc/test-plans/`)

## Steps

### Step 1 — Launch Application
Use Browser Subagent to open the app at `http://localhost:5173` (or active dev server port).
Wait for the app to fully load (check for main content render).

### Step 2 — Execute User Flows
For each user flow defined in the test plan:

1. **Navigate** to the target page/section.
2. **Interact** — perform the user actions:
 - Click buttons, links, tabs
 - Fill form inputs with valid data
 - Submit forms
 - Toggle switches/dropdowns
 - Scroll through content
3. **Verify** — check expected outcomes:
 - Correct content displayed
 - Navigation works (URL changes, page renders)
 - Loading states appear and resolve
 - Success/error feedback shown
4. **Capture** — **MANDATORY:** screenshot the result state. Every flow MUST have at least one screenshot saved to `.hc/qa/screenshots/`. Use descriptive names: `[flow]-[state]-[viewport].png`. Bug reports WITHOUT screenshots will be rejected.

### Step 3 — Test Edge Cases
For each flow, also test:

| Edge Case | Action | Expected |
|-----------|--------|----------|
| Empty state | Load page with no data | Friendly empty state message |
| Invalid input | Submit form with wrong data | Error message near field |
| Rapid clicks | Click submit button 5x fast | Only one submission, button disabled |
| Browser back | Navigate forward, then press back | Previous state restored |
| Long content | Enter very long text | No overflow, proper truncation |
| Mobile viewport | Resize to 375px | No broken layout |

### Step 4 — Test Error States
1. Simulate error conditions where possible:
 - Invalid form data → check error messages appear
 - Missing required fields → check validation messages
2. Verify error messages are:
 - Visible and near the problem
 - Clear and actionable
 - Not exposing technical details

### Step 5 — Generate E2E Report
Create an Artifact report:

```markdown
# E2E QA Report — [Feature/Page]
**Date:** YYYY-MM-DD | **Tester:** @qc

## Summary: PASS / FAIL

## User Flows Tested
| # | Flow | Steps | Result | Screenshot |
|---|------|-------|--------|------------|
| 1 | [Flow name] | [X steps] | PASS / FAIL | [link] |

## Edge Cases Tested
| # | Case | Result | Notes |
|---|------|--------|-------|
| 1 | Empty state | / | [details] |

## Bugs Found
| # | Description | Severity | Screenshot | Steps to Reproduce |
|---|-------------|----------|------------|-------------------|
| 1 | [Bug] | HIGH/MED/LOW | [link] | [steps] |

## Recommendations
- [Action items]
```

### Step 6 — Report & Escalate
- If all flows pass → include in Verification Report, mark APPROVED.
- If bugs found → file in `.hc/bugs/`, assign severity, escalate to @dev via @pm.
- Critical bugs BLOCK deployment.

---

## Chaining
- Can be triggered after `/scaffold-feature` for new feature validation.
- Results feed into @qc's Verification Report.
