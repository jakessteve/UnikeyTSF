---
description: Implementation Review - post-build roundtable to evaluate approach quality and surface tech debt before shipping
---

# WORKFLOW: /implementation-review (Post-Build Review Gate)

Triggered after @qc verification passes. Evaluates whether the *approach* (not just the code) was the right one.

> **When to use:** After any feature implementation that came from a brainstorm, architecture decision, or design sprint. Skip for trivial bug fixes.

Execute sequentially:

---

## Step 1 — Gather Context

1. @pm collects:
 - Original brainstorm/decision artifact (from `.hc/brainstorms/` or `.hc/decisions/`)
 - Battle test report (if `/battle-test` was run)
 - Verification report from @qc
 - List of files created/modified

---

## Step 2 — Roundtable

1. @pm initiates the roundtable using `implementation-debate` skill.
2. Required participants: **@sa**, **@dev**, **@qc**
3. Optional participants: **@designer** (if UI), **@devops** (if infra)
4. Each participant answers:
 - Does the implementation match the original intent?
 - What tech debt was introduced?
 - If we could do it again, would we do it differently?

---

## Step 3 — Verdict

Using the `implementation-debate` skill verdict template:

| Verdict | Action |
|---|---|
| **SHIP** | Route to deployment pipeline |
| **SHIP WITH DEBT** | Ship + create debt stories in `.hc/stories/` with severity labels |
| **REFACTOR** | @dev refactors targeted areas → re-run @qc verification → re-review |
| **RETHINK** | Route back to `/party-mode` or `/idea-forge` Phase 1 with findings attached |

---

## Step 4 — Log

1. Save review to `.hc/reviews/YYYY-MM-DD-[feature-slug].md`
2. If debt stories created, link them from the review artifact.
3. @pm updates the roadmap if priorities shifted.

---

## Output Files
| File | Location |
|---|---|
| Implementation review | `.hc/reviews/YYYY-MM-DD-[feature].md` |
| Debt stories (if any) | `.hc/stories/` |

Report: "Implementation review complete. Verdict: [VERDICT]. [X] debt items logged."
