---
description: Research Swarm - spawn parallel research agents with self-critique, cross-discussion, and critical-thinking synthesis
---

# WORKFLOW: /spawn-research (Multi-Agent Research & Synthesis)

Triggered when @pm receives a research + recommendation request that benefits from parallel investigation across multiple angles.

> **Inherits:** [`spawn-base-template.md`](spawn-base-template.md) — read the base template first for the full 6-phase pipeline.
> **Related workflows:** `/party-mode` (brainstorming), `/swarm-execute` (implementation waves), `/delegate-task` (single worker)

Execute sequentially, respecting gate decisions:

---

## Customizations (over base template)

### Track Assignment Guide (Phase 1)

| Request Type | Typical Tracks |
|---|---|
| "Suggest upgrades" | Tech stack, Security, Performance, DX, Architecture |
| "Evaluate alternatives" | Feature comparison, Cost analysis, Migration risk, Community health |
| "Improve quality" | Test coverage, Code health, Error handling, Accessibility, Performance |
| "Scale readiness" | Infrastructure, Database, Caching, Monitoring, Cost projection |

### Spawn Details (Phase 2)

- Use `.agent/spawn_agent_tasks/templates/research-critique-task.md` for prompts.
- Every prompt MUST include the mandatory **self-critique instruction** (5 Lenses from `critique` skill).
- CLI: Use `spawn-agent` skill with `-ApprovalMode Yolo` (research mode).
- Persona-switch: Assume persona → research → apply `critique` skill → write report → switch back.

### Validation Checklist (Phase 3)

Each report MUST contain:
- [ ] Findings section with evidence
- [ ] Self-critique section (5 Lenses from `critique` skill)
- [ ] Confidence score (1-10) per finding
- [ ] Limitations and blind spots acknowledged

If any report is missing self-critique → @pm applies `critique` skill to that report.

### Cross-Review Questions (Phase 4)

- "Does this conflict with Track X's findings?"
- "What did this track miss that Track Y found?"
- "Are any recommendations mutually exclusive?"

**Conflict Log format:**
```markdown
| Track A Finding | Track B Finding | Conflict | Resolution |
|---|---|---|---|
| [finding] | [contradicting finding] | [nature] | [resolved/unresolved] |
```

### Scoring Weights (Phase 5)

| Factor | Weight |
|---|---|
| Impact (value delivered) | 40% |
| Effort (cost to implement) | 25% |
| Risk (what could go wrong) | 20% |
| Confidence (evidence quality) | 15% |

### Report Template (Phase 6)

```markdown
# Research Report — [Topic]
**Date:** YYYY-MM-DD | **Requested by:** User
**Research tracks:** N | **Agents involved:** [list]

## Executive Summary
[3-5 sentences: what was researched, key finding, top recommendation]

## Ranked Recommendations
### P0 — Do Now
| # | Recommendation | Impact | Effort | Risk | Confidence | Track Source |
|---|---|---|---|---|---|---|

### P1 — Do Soon / ### P2 — Consider Later / ### P3 — Rejected
[same table format]

## Conflicts Resolved
## Methodology
## Limitations
## Appendix: Full Track Reports
```

**Output directory:** `.hc/research/`
