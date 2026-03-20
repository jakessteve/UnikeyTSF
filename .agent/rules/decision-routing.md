---
description: Single reference for task classification, confidence scoring, and decision routing at task boundaries
---

# RULE: DECISION ROUTING

Unified rule for classifying tasks, scoring confidence, and routing decisions. All agents reference this file for consistent complexity classification and probabilistic routing.

---

## 1. Task Complexity Classification

| Level | Files | Domains | Gate | Verification | Tool Budget | Delegation Mode |
|---|---|---|---|---|---|---|
| **Trivial** | <=1 | 1 | None | @dev-fe/@dev-be self-verify | 10 | Persona-switch |
| **Small** | <=3 | 1 | Fast-path | @dev-fe/@dev-be + @qc spot-check | 20 | Persona-switch (default) |
| **Standard** | 4-6 | 1 | Fast-path + validate | @dev-fe/@dev-be + @qc novel parts | 30 | Lightweight delegation |
| **Medium** | 7-10 | 1-2 | SPARC lite | Full @dev-fe/@dev-be, @qc, @pm | 40 | CLI workers preferred |
| **Large** | >10 | 2+ | Full SPARC | Full + `/implementation-review` | 60 | CLI workers MANDATORY |
| **Epic** | >20 | 3+ | SPARC + dialectical | Full + review + debate | 80 | CLI workers + multi-worker |

### How to Classify
1. Count **files** expected to change.
2. Count **domains** (UI, engine, API, infra, testing = separate domains).
3. Use the highest matching level.
4. **Novelty override:** Existing pattern extensions may downgrade one level. Novel patterns never downgrade.

### Lightweight Delegation (Standard Tier)
- Orchestrator composes inline prompt (no template file, use `-Prompt` flag)
- Quick validation only (placeholders + file paths)
- Orchestrator reviews `git diff` directly
- Worker timeout: 120s default

### Analysis/Review Task Override
| Signal | Override Level |
|---|---|
| Multi-agent review (3+ domains) | **Large** minimum |
| User amplifier keywords ("exhaustive", "comprehensive") | **+1 level** |
| "Call every agent" / "all agents" | **Epic** |

> [!TIP]
> **Large and Epic tasks:** Consider `structured-analysis-frameworks` (MECE/Issue Tree) during planning. Use `amateur-proof-plans` for self-contained phase files.

---

## 2. Confidence-Based Routing

### When This Applies
After completing any deliverable, at workflow decision gates, when presenting options, or when debugging.

### Score Format
```markdown
**Confidence:** [0-100] | **Flags:** [uncertain areas] | **Evidence:** [basis]
```

### Auto-Routing Table

| Score | Action |
|---|---|
| **>= 85** | Proceed autonomously |
| **75-84** | Proceed, flag for post-hoc review |
| **60-74** | Flag for @pm review before proceeding |
| **< 60** | HALT -- generate 2-3 alternatives, escalate |

### Calibration
- >=90 = tested and passing all known cases
- 70-89 = works for tested cases, edge cases possible
- 50-69 = reasonable approach, unverified assumptions
- <50 = guessing or incomplete information

### Anti-Pattern: Confidence Theater
Don't always score 85+ to avoid review friction. Don't score low to avoid responsibility. Applies to ALL decision points.

---

## 3. Progress Tracking

### Monotonicity Rule
Track: tests passing (must >= previous), files completed (must >= previous), errors remaining (must <= previous).

Non-monotonic for **2 consecutive iterations** -- revert to last good state, reassess.

### Progressive Confidence Tracking
After every **15 tool calls**, re-assess confidence. Score dropped >=15 points -- investigate. Monotonically decreasing across 3 checkpoints -- Context Reset (`anti-patterns-core.md` Section 4).

### Checkpoint Rule
At **75% of tool budget**, the agent MUST:
1. Summarize progress in `task.md`.
2. Assess remaining work vs. remaining budget.
3. Run `/token-check` if approaching token limits.
4. Run cognitive bias check (`anti-patterns-swarm.md` Section 8) if in swarm mode.
5. If >50% work remains -- trigger Context Reset or `/handoff`.

---

## 4. Cross-References
- **Verification Tiers** -- `execution-protocol.md` Section 3
- **Dialectical Gate** -- `dialectical-development.md`
- **Iteration Budget** -- `execution-protocol.md` Section 4
