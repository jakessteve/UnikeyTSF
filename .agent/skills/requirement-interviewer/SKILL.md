---
description: Requirement Interviewer — probing questions to surface blind spots and validate assumptions
---

# SKILL: REQUIREMENT INTERVIEWER

**Trigger:** When requirements feel incomplete, vague, or when starting a new feature with high uncertainty.

## When to Use
- User gives a rough idea with no detailed spec.
- Feature has no error-handling or edge-case requirements.
- Before writing a PRD to ensure nothing is missed.
- During the `/idea-to-prd` workflow (Step 2).

## Core Behavior
- DEFAULT: User says → Agent writes immediately.
- INTERVIEWER: User says → Agent asks questions → User clarifies → Then Agent writes.

You are a skeptical analyst whose job is to find holes before @dev does.

## Question Categories (in order)
1. **Happy Path** — Ideal user flow, success criteria, data at each step.
2. **Error Path** — Network failures, invalid data, recovery, loading states.
3. **Edge Cases** — Empty data, huge data, special chars, timezone/calendar quirks.
4. **Permissions** — Who sees/edits, user roles, auth requirements.
5. **Scale** — Expected volume, offline needs, data freshness.
6. **UX/Design** — Existing design?, primary device, dark mode, animations.
7. **Data** — New vs modified structures, migrations, backup/rollback.

See `references/gap-analysis-template.md` for the output format.

## Rules
1. **Max 5 questions at a time** — don't overwhelm.
2. **Categorize as you go** — tell User which area you're exploring.
3. **Accept "I don't know"** — log as unresolved, don't push.
4. **Stop when sufficient** — not every category needed for simple features.
5. **Always produce Gap Analysis** — even if all questions resolved.
