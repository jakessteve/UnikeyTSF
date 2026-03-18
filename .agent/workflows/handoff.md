---
description: Generate a structured handoff artifact for cross-model delegation between PM and Dev conversations
---

# WORKFLOW: /handoff (Cross-Model Delegation) — Current session model

Triggered by @pm when `model-selector` skill determines the next phase requires a different model, OR manually via `/handoff`.

> **Purpose:** Produce a self-contained handoff artifact so a new conversation on a different model can immediately continue execution without re-discovering context.

## Prerequisites
- Planning/architecture phase is **complete** in the current session.
- An `implementation_plan.md` or equivalent deliverable exists.
- @pm has identified the target agent and recommended model.
- For **Large/Epic** tasks: consider using `amateur-proof-plans` skill to generate detailed phase files with code contracts and failure scenarios — these become the handoff payload.

## Steps

### Step 1 — Capture Git State
Run `git rev-parse HEAD` to record the current commit hash. This is the **freshness anchor** — the receiving conversation will verify the codebase hasn't drifted.

### Step 2 — Assess Risk Level
Classify the handoff risk based on context-loss sensitivity:

| Signal | Risk | Recommendation |
|--------|------|----------------|
| ≤3 files, single domain, clear acceptance criteria | LOW | Safe to hand off |
| 4-10 files, 2 domains, some implicit decisions | MEDIUM | Hand off with extra Ghost Decisions |
| >10 files, 3+ domains, deep implicit reasoning | HIGH | **Stay on one model** — do NOT hand off |

If risk is HIGH → advise the user to continue on the current model. Skip remaining steps.

### Step 3 — Build Context Manifest
Using `view_file` and `grep_search`, compile:
1. **Files to read first** — ordered by importance for understanding
2. **Files to modify** — with 1-line summary of what changes
3. **Files to NOT touch** — explicit guard rails
4. **Key dependencies** — external packages, APIs, data schemas

### Step 4 — Document Ghost Decisions
Review the planning session and explicitly list:
- Micro-decisions made during analysis that aren't in the formal plan
- Implicit assumptions about the codebase
- Trade-offs considered but not documented
- Edge cases the PM noticed but didn't formally specify

### Step 5 — Generate Handoff Artifact
Write the handoff to `.hc/handoffs/YYYY-MM-DD-[task-slug].md` using this template:

```markdown
# Handoff: [Task Name]

## Meta
| Field | Value |
|-------|-------|
| Source Agent | @pm |
| Source Model | [current model] |
| Target Agent | @dev (or @designer, @ba, etc.) |
| Recommended Model | [recommended model code from model-selector] |
| Git Hash | [from Step 1] |
| Risk Level | LOW / MEDIUM / HIGH |
| Date | [YYYY-MM-DD HH:MM] |

## Strategic Context
### Goal
[What we're building and why — 2-3 sentences]

### Acceptance Criteria
1. [Criterion 1]
2. [Criterion 2]
3. [Criterion 3]

### Design Decisions
- [Decision 1 — rationale]
- [Decision 2 — rationale]

## Ghost Decisions & Assumptions
> These are micro-decisions and implicit knowledge from the planning session
> that aren't captured in the formal plan. READ THESE CAREFULLY.

- [Ghost decision 1]
- [Ghost decision 2]
- [Assumption 1]

## Context Manifest
### Files to Read First (in order)
1. `[path]` — [why this file matters]
2. `[path]` — [why this file matters]

### Files to Modify
1. `[path]` — [what changes needed]
2. `[path]` — [what changes needed]

### Files to NOT Touch
- `[path]` — [why it's off-limits]

### Dependencies
- [Package/API — how it's used]

## Execution Constraints
- **Time-box:** [estimated effort]
- **Out of scope:** [explicit exclusions]
- **Testing requirements:** [what @qc will verify]

## Verification Plan
[How to confirm the work is correct — commands to run, things to check]
```

### Step 6 — Instruct User
Present the handoff artifact and tell the user:

```
 Plan complete. Handoff artifact saved to `.hc/handoffs/[filename]`.

**Next step:** Start a new Antigravity conversation →
Select **[Recommended Model]** → Type:
"Read `.hc/handoffs/[filename]` and execute the handoff as @dev using /receive-handoff"
```

---

## Output Files
| File | Location |
|------|----------|
| Handoff artifact | `.hc/handoffs/YYYY-MM-DD-[task-slug].md` |
