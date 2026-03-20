---
description: Full user testing session — simulate end-users testing all features and generate structured feedback report
---

# Workflow: /user-test-session

> Simulates real end-users interacting with the app. Evaluates speed, usability, info richness, usefulness, and UI/UX polish. Produces `user-feedback.md` for other agents to act on.

## Prerequisites
- App running on `localhost` (typically via `npm run dev`)
- Browser Subagent available
- @user-tester role activated (by @pm)

## Step 1 — Session Setup

1. Read `@user-tester.md` to load persona definitions.
2. Determine testing scope:
 - **Full session:** All pages × all personas
 - **Feature session:** Specific feature × relevant personas (see Persona Selection Guide)
3. Check `.hc/feedback/` for previous reports to enable trend comparison.
4. Set up browser viewport for the first persona's device.

## Step 2 — First Impression Sweep

For each **major page**, per persona:
1. Open the page fresh.
2. Spend exactly 5 seconds — record gut reaction (use `user-experience-testing` skill §3).
3. Screenshot the page. Save to `.hc/feedback/screenshots/[date]-[page]-[persona].png`.
4. Note: What stands out? What's confusing? What's delightful?

Pages to sweep:
- Homepage / Landing
- Calendar (Âm Lịch)
- Feature A (primary user flow)
- Feature B (detailed view)
- Feature C (analysis view)
- Feature D (input/output flow)
- Settings
- Auth (Login / Register)

## Step 3 — User Journey Flows

Test these core flows as each persona. Use `user-experience-testing` skill §4 for measurement.

| Flow | Description | Primary Persona |
|------|-------------|----------------|
| **Morning Routine** | Open app → check main screen → see key info → close | Primary User |
| **Fortune Reading** | Navigate to detail view → enter data → read results → understand | Senior User |
| **Decision Making** | Go to decision tool → pick option → check result → decide | Professional |
| **Deep Exploration** | Browse analysis view → read output → explore details → learn | Young User |
| **Quick Divination** | Open input tool → enter question → read result → understand answer | Primary User |

For each flow, record:
- Task success (yes/no/partial)
- Steps taken vs. expected
- Time to complete
- Friction points

## Step 4 — Page-by-Page Deep Dive

For each page, complete the **UX Scorecard** (5 dimensions × 1–5 score) per persona.

For each page, document:
- **First Impression:** 5-second gut reaction
- **Positives:** What works well (celebrate wins!)
- **Issues:** Problems found with severity ( Critical / Medium / Minor)
- **Suggestions:** Improvement ideas, phrased in user language
- **Empathy Assessment:** Use `accessibility-empathy` skill for Senior User and Primary User personas

## Step 5 — Mobile + Dark Mode Pass

1. Set viewport to 375px (Primary / Senior User perspective).
2. Repeat Step 2–4 key checks on mobile.
3. Toggle dark mode.
4. Repeat key checks in dark mode.
5. Note any mobile-specific or dark-mode-specific issues.

## Step 6 — Trend Comparison (if previous reports exist)

1. Read the most recent `user-feedback-*.md` from `.hc/feedback/`.
2. Compare scores dimension-by-dimension per page.
3. Mark: Improved / Regressed / Unchanged.
4. Identify patterns across sessions.

## Step 7 — Generate Feedback Report

Compile all findings into a report following Rule `user-feedback-format.md`.
Save to `.hc/feedback/user-feedback-YYYY-MM-DD.md`.

Include:
- Executive summary (2-3 sentences)
- UX Scorecard table (all pages × all personas)
- Detailed findings per page
- Task completion results
- Trend comparison (if applicable)
- Priority recommendations (ranked by user impact)

## Step 8 — Triage & Route

Automatically classify and route findings:

| Severity | Destination | Action |
|---|---|---|
| Critical | `.hc/bugs/` with `user-experience` label | Blocks next deployment |
| Medium | `.hc/stories/` as backlog items | Queued for next sprint by @pm |
| Minor | Included in report only | Design polish backlog |

Report back to @pm with summary of findings.

## Step 9 — Done-Gate (MANDATORY)

After reviewing the feedback report, @pm MUST enforce (Rule `execution-protocol.md` §3.5):

1. **If ANY Critical finding exists** → task stays **"In Progress"**:
 - Route criticals through Bug Record Gate (`@qc.md` §7.6 → `execution-protocol.md` §3.3).
 - @dev fixes → @qc re-verifies (`execution-protocol.md` §3.4) → @user-tester re-tests.
 - Repeat until ALL Criticals pass re-test.
2. **If only Medium and Minor** → task may be marked **"Done"**.
 - Mediums queued as stories for next sprint.
 - Minors noted in report only.

> [!CAUTION]
> This is the FINAL gate in the lifecycle. No task with unresolved Critical UAT findings can be reported as complete to the User.

---

## Auto-Trigger Points

This workflow is automatically triggered by @pm when:
- A feature implementation + @qc verification is complete
- UI/UX design changes have been deployed
- A responsive or performance sprint ends
- A new phase begins (baseline measurement)

## Chaining

| Preceding Workflow | Following Workflow |
|---|---|
| `/scaffold-feature` → @qc verification | → `/user-test-session` |
| `/design-to-code` → visual verification | → `/user-test-session` |
| `/qa-responsive-check` | → `/user-test-session` (mobile focus) |
| `/user-test-session` → critical bugs found | → @dev fix → @qc verify → `/user-test-session` (re-test) |
