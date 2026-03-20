---
description: Bootstrap a new conversation from a handoff artifact — verify freshness, load context, and begin execution
---

# WORKFLOW: /receive-handoff (Cross-Model Handoff Receiver) — Per handoff recommendation

Triggered by the target agent (usually @dev) at the **start of a new conversation** after the user has switched models.

> **Purpose:** Safely bootstrap execution context from a handoff artifact produced by `/handoff`, verify nothing has drifted, and begin work immediately.

## Prerequisites
- A handoff artifact exists at `.hc/handoffs/[filename].md`
- User has started a new conversation on the recommended model

## Steps

### Step 1 — Locate & Read Handoff
1. If user specifies a handoff file, read it with `view_file`.
2. If not specified, list `.hc/handoffs/` and read the **most recent** handoff artifact.
3. Parse the Meta table: source agent, target agent, recommended model, git hash, risk level.

### Step 2 — Freshness Check
Run `git rev-parse HEAD` and compare to the Git Hash in the handoff:

| Result | Action |
|--------|--------|
| Hash matches | Proceed — codebase hasn't changed since planning |
| Hash differs | Run `git log --oneline [handoff-hash]..HEAD` to review changes. If changes are in unrelated files → proceed with caution. If changes touch files in the Context Manifest → **STOP** and alert user: "Codebase has changed since this handoff was created. The following files were modified: [list]. Please re-run /handoff from the PM conversation or confirm it's safe to proceed." |

### Step 3 — Load Context
Read files from the Context Manifest in the specified order:
1. Read all "Files to Read First" using `view_file`
2. Skim "Files to Modify" to understand current state
3. Note "Files to NOT Touch" as guard rails

### Step 4 — Internalize Constraints
Parse and acknowledge:
- **Ghost Decisions & Assumptions** — treat these as binding constraints
- **Acceptance Criteria** — these define "done"
- **Out of Scope** — do NOT implement anything listed here
- **Testing Requirements** — plan verification accordingly

### Step 5 — Confirm Ready
Before beginning work, output a brief confirmation:

```markdown
## Handoff Received

| Field | Value |
|-------|-------|
| Task | [task name from handoff] |
| Source | @pm on [source model] |
| Freshness | Verified (hash match) / Proceeding with caution |
| Files to modify | [count] files |
| Risk Level | [from handoff] |

**Ghost Decisions loaded:** [count] items
**Acceptance Criteria:** [count] items

Beginning execution as **@dev** on **[current model]**...
```

### Step 6 — Execute
Assume the target agent persona and begin implementation:
1. Follow the implementation plan / acceptance criteria from the handoff
2. Respect all ghost decisions and assumptions
3. Stay within the "Files to Modify" list — do NOT touch guarded files
4. Run verification steps from the handoff's Verification Plan

### Step 7 — Completion Report
After work is complete, generate a completion report:

```markdown
# Completion Report: [Task Name]

## Meta
| Field | Value |
|-------|-------|
| Handoff Source | [handoff filename] |
| Executing Agent | @dev on [current model] |
| Status | Complete / Partial |

## Changes Made
| File | Change Summary |
|------|---------------|
| `[path]` | [what was done] |

## Acceptance Criteria Status
| # | Criterion | Status |
|---|-----------|--------|
| 1 | [criterion] | / |

## Verification Results
- `npm run lint`: [PASS/FAIL]
- `npx tsc --noEmit`: [PASS/FAIL]
- Tests: [X/Y passing]

## Notes for PM
- [Any deviations from the plan and why]
- [Anything the PM should review]
```

Save to `.hc/handoffs/YYYY-MM-DD-[task-slug]-completion.md`.

---

## Output Files
| File | Location |
|------|----------|
| Completion report | `.hc/handoffs/YYYY-MM-DD-[task-slug]-completion.md` |
