---
description: Proactive UX audit — evaluate existing screens, identify improvements, implement, and verify the full cycle
---

# WORKFLOW: /ux-audit (UX Audit & Improvement Cycle)

Triggered monthly (scheduled), when UX scores drop, or on-demand for specific screens/features.

> **Single-LLM Execution Model:** "Call @agent" means **assume that agent's persona** and execute using available tools.

## Prerequisites
- App running on `localhost` (via `npm run dev`)
- Browser Subagent available
- Previous UX feedback reports in `.hc/feedback/` (optional, for trend comparison)

## Steps

### Step 1 — Select Audit Target — `OPUS/Plan`

**@pm** determines audit scope:

1. Choose target: specific page, feature area, or full app sweep.
2. Review previous UX scores from `.hc/feedback/` to identify regression candidates.
3. Check telemetry for pages/features with high bounce rates or low engagement.
4. Prioritize areas with the most user complaints or lowest scores.

**Output:** Audit scope document

### Step 2 — Heuristic Evaluation — `SONNET/Fast`

**@designer** conducts a structured heuristic evaluation using `design-system-uiux` skill:

1. Evaluate against Nielsen's 10 usability heuristics.
2. Check design system compliance: tokens, spacing, typography, color consistency.
3. Check dark mode parity for all audited screens.
4. Check accessibility: contrast ratios, keyboard navigation, screen reader compatibility.
5. Check responsive behavior at mobile (375px), tablet (768px), and desktop (1440px).
6. Screenshot each finding with annotation.

**Output:** Heuristic evaluation report in `.hc/ux-audits/heuristic-[target]-YYYY-MM-DD.md`

### Step 3 — User Test Session — `SONNET/Fast`

**@user-tester** runs a focused `/user-test-session` on the audited screens:

1. Test with relevant personas (at minimum: primary persona + edge-case persona).
2. Score all 5 UX dimensions (Speed, Usability, Info Richness, Usefulness, UI/UX Polish).
3. Compare with previous scores (if available) for trend tracking.
4. Document specific friction points and delight moments.

**Output:** UX scorecard in the feedback report

### Step 4 — Issue Prioritization — `OPUS/Plan`

**@pm** prioritizes findings:

1. Merge findings from heuristic evaluation + user test session.
2. Classify each issue: Critical / Medium / Minor.
3. Score by Impact × Ease of Fix.
4. Select top issues for improvement (capacity-appropriate).
5. Create improvement stories in `.hc/stories/`.

> [!IMPORTANT]
> Critical UX findings MUST go through the Bug Record Gate (`execution-protocol.md` §3.3) before any fix begins.

**Output:** Prioritized issue list + improvement stories

### Step 5 — Design Improvements — `SONNET/Fast`

**@designer** (or @dev for non-design fixes) implements improvements:

1. For each approved improvement story, follow the `/design-to-code` workflow.
2. Apply design system tokens and patterns from `design-system-uiux` skill.
3. Ensure dark mode parity for all changes.
4. Follow `no-code-boundary.md` — if @pm initiated the audit, delegate implementation.

**Output:** Implemented improvements

### Step 6 — Re-test & Verify — `SONNET/Fast`

**@user-tester** re-tests the improved screens:

1. Re-run the same test cases from Step 3 on the modified screens.
2. Compare before/after UX scores — verify improvement.
3. Check for regressions in adjacent functionality.
4. **If scores improved** → mark improvement stories as Done.
5. **If scores unchanged or regressed** → route back to Step 5 with specific feedback.

> [!CAUTION]
> Re-testing is MANDATORY. An improvement that isn't verified is worse than no improvement — it creates false confidence.

**Output:** Before/after comparison report

### Step 7 — Close & Archive — `GEMINI-H/Plan`

**@pm** closes the audit:

1. Document final before/after scores.
2. Archive the audit report in `.hc/ux-audits/`.
3. Update the UX baseline for future trend tracking.
4. Feed unaddressed items into the backlog for future sprints.

---

## Output Files
| File | Location |
|---|---|
| Heuristic evaluation | `.hc/ux-audits/heuristic-[target]-YYYY-MM-DD.md` |
| UX feedback report | `.hc/feedback/user-feedback-YYYY-MM-DD.md` |
| Improvement stories | `.hc/stories/` |
| Before/after comparison | `.hc/ux-audits/improvement-[target]-YYYY-MM-DD.md` |

## Chaining
| Preceding | Following |
|---|---|
| Monthly schedule / @pm decision | → `/ux-audit` |
| `/user-test-session` → low scores | → `/ux-audit` (focused) |
| `/ux-audit` Step 5 | → `/design-to-code` (per improvement) |
| `/ux-audit` Step 6 | → `/user-test-session` (re-test) |
| `/business-review` → UX concerns | → `/ux-audit` |
