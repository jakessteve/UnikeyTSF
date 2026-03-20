---
description: Continuous Improvement - logging templates, retrospective methodology, pattern extraction, and self-improvement analysis
---

# SKILL: Continuous Improvement

**Trigger:** After every phase/epic completion, agents use logging and retrospective templates to record work and identify improvements.

---

## When to Use
- After completing a phase/epic — run retrospective.
- During development — log decisions, bugs, and security findings.
- Pattern extraction — analyze recurring issues across phases.
- When proposing changes to `.agent/` rules, skills, or workflows.

---

## The 5-Step Process

### Step 1: Log (During Development)
Each agent fills their role-specific log template during the phase, not after:

| Role | Log Type | What to Capture |
|---|---|---|
| @dev | Dev Log | Decisions, blockers, tech debt, workarounds |
| @qc | Bug Log | Bugs found, patterns, coverage delta |
| @devops | Security Log | Vulnerabilities, dependency audit results |
| @pm | Decision Log | Key decisions, scope changes, outcomes |

→ Templates: `references/log-templates.md`

### Step 2: Retrospective (After Phase Completion)
@ba facilitates a structured review using three questions:

1. ** What went well?** — Identify practices to continue. Back each item with evidence from logs.
2. ** What didn't go well?** — Identify pain points. Be specific — "builds were slow" → "builds averaged 4min due to unoptimized asset pipeline."
3. ** What to improve?** — Each improvement maps to a target: rule, skill, or workflow to update.

→ Template: `references/retrospective-template.md`

### Step 3: Extract Patterns
After the retrospective, analyze logs across phases:

1. **Frequency check:** Has this issue appeared 2+ times across different phases?
2. **Category check:** Is it a rules gap (missing constraint), skills gap (missing knowledge), or workflow gap (missing step)?
3. **Impact check:** Does it cause bugs, delays, rework, or security issues?

If all three checks pass, it's a **confirmed pattern** that warrants an improvement proposal.

### Step 4: Propose Improvements
Write a structured improvement proposal:

```markdown
# Improvement Proposal — [Title]
**Date:** YYYY-MM-DD | **Proposed by:** @pm

## Pattern Observed
[Recurring issue with evidence from ≥2 phase logs]

## Root Cause
[Why this keeps happening — systemic, not incident-specific]

## Proposed Change
- **File:** `.agent/[type]/[filename].md` | **Type:** Rule / Skill / Workflow
- **Change:** [What to add/modify — be specific]

## Expected Impact / Risk
[How this prevents recurrence] / [Any risk from the change]
```

** NEVER auto-modify `.agent/` files.** Always get User approval first.

### Step 5: Track
- Log all improvements in `.hc/improvements/` history.
- Link each improvement back to the retrospective that identified it.
- Review improvement effectiveness in the next retrospective (closed loop).

---

## Quality Checklist
Before closing a retrospective:

| Check | Why |
|---|---|
| All log templates filled by respective agents | Evidence base |
| Each "What went well" item has supporting evidence | Not just feelings |
| Each "What didn't go well" has a proposed improvement | Actionable output |
| Recurring patterns checked against previous retros | Trend detection |
| SOT sync status verified (PRD, Architecture, API contracts) | Drift prevention |
| Action items have owners and due dates | Accountability |

## File Management
- Retrospectives → `.hc/retrospectives/`
- Phase logs → `.hc/logs/`
- Improvement proposals → `.hc/improvements/`
- Improvement history → `.hc/improvements/history.md`
