---
description: Amateur-Proof Plans — generates detailed phase files with data flow, code contracts, and failure scenarios so any model (including weak ones) can execute correctly
---

# SKILL: Amateur-Proof Phase Plans

**Trigger:** During SPARC Specification/Pseudocode phases for **Medium+** tasks (≥4 files), or when preparing work for CLI worker delegation via `worker-delegate` skill.

---

## When to Use
- Planning a feature that will be split across multiple phases or sessions.
- Preparing work for delegation to CLI worker agents (Gemini CLI) — see `worker-delegate` skill.
- Creating `/handoff` artifacts for cross-model delegation — see `/handoff` workflow.
- Any **Large/Epic** task per `decision-routing.md` (>10 files, 2+ domains).
- When context pressure is high and work will span multiple sessions.

## When to SKIP
- Trivial/Small tasks (≤3 files, single concern) — fast-path per `execution-protocol.md` §2.
- Bug fixes with clear root cause.
- Documentation-only changes.

---

## The Amateur-Proof Protocol

The goal: **produce phase files so detailed that even a model with zero project knowledge can execute correctly.** This directly improves:
- CLI worker quality (Rule `execution-protocol.md` §8.1)
- Cross-model handoff success rate (`/handoff` → `/receive-handoff`)
- Multi-session persistence (`anti-patterns-core.md` §4 Context Reset Protocol)

### Step 1 — Phase Decomposition

Break the task into self-contained phases. Each phase MUST be independently executable.
Provide a table containing: Phase Code, Description, Disjoint File Set, Dependencies, and Target Model.

**Rules:**
- Each phase touches a **disjoint file set** — no two phases modify the same file (Rule `anti-patterns-swarm.md` §7.1 File Ownership).
- Phase order follows dependency graph — later phases only depend on earlier phases.
- Each phase has a clear **entry condition** (what must be true before starting) and **exit condition** (what must be true when done).

### Step 2 — Structured Phase Definition
Rather than generating verbose narrative markdown and Mermaid diagrams (which consume heavy tokens), output the definition of each phase in a strict JSON or YAML schema. This schema acts as the direct, machine-readable prompt for CLI workers.

**Required YAML Structure:**
```yaml
phase_id: 1
description: "..."
entry_condition: "..."
exit_condition: "..."
code_contracts: 
  - file: "path"
    signature: "..."
failure_scenarios:
  - trigger: "..."
    recovery: "..."
file_scope:
  read: []
  modify: []
  off_limits: []
verification_commands: []
```

### Step 3 — Phase File Generation
Write each configured YAML/JSON phase definition to a self-contained file. Do not wrap the output in conversational fluff. Save raw configurations for downstream orchestration.

---

## File Management

Phase files are stored based on use case:

| Use Case | Location | Read By |
|---|---|---|
| Multi-session self-persistence | `.hc/phases/[feature]/phase-[N].md` | Same agent in next session |
| CLI worker delegation | `.agent/spawn_agent_tasks/[task-name]-phase-[N].md` | CLI worker via `worker-delegate` |
| Cross-model handoff | `.hc/handoffs/[date]-[task]-phase-[N].md` | Target model via `/receive-handoff` |

---

## Plan Review Loop

After completing each phase file (Step 5), validate it before moving on:

1. Spawn a **review** worker using `plan-reviewer-prompt.md` template (read-only, `--yolo`)
2. Provide: phase file content + original spec/requirements
3. If `ISSUES_FOUND` → fix the issues in the phase file → re-review
4. If `APPROVED` → proceed to next phase (or execution handoff if last phase)

**Loop limits:** Max **3 review iterations** per phase. If still failing → surface to user for guidance.

> [!TIP]
> The review loop is cheap (read-only workers) and catches common issues: missing verification steps, ambiguous instructions, incomplete contracts. Worth the extra 1-2 minutes per phase.

---

## Integration Points

This skill connects to the following framework components:

| Component | Type | Integration |
|---|---|---|
| [`decision-routing.md`](.agent/rules/decision-routing.md) | Rule | Determines when this skill activates (Medium+ tasks) |
| [`model-routing.md`](.agent/rules/model-routing.md) | Rule | Selects per-phase model (cheaper models for simple phases) |
| [`execution-protocol.md`](.agent/rules/execution-protocol.md) | Rule | SPARC Specification phase is where plans are generated |
| [`decision-routing.md`](.agent/rules/decision-routing.md) | Rule | Each phase gets a confidence score at completion |
| [`anti-patterns-core.md`](.agent/rules/anti-patterns-core.md) | Rule | §4 Context Reset → save phase state; §7 File Ownership → phase file disjointness |
| [`execution-protocol.md`](.agent/rules/execution-protocol.md) | Rule | §5 Tool budgets per phase; §6 CLI worker spawn limits |
| [`worker-delegate.md`](.agent/skills/worker-delegate.md) | Skill | Phase files become worker prompts — all mandatory sections satisfied |
| [`critical-thinking-models.md`](.agent/skills/critical-thinking-models.md) | Skill | Run before phase decomposition for Medium+ features |
| [`structured-analysis-frameworks.md`](.agent/skills/structured-analysis-frameworks.md) | Skill | MECE decomposition for phase breakdown |
| [`/handoff`](.agent/workflows/handoff.md) | Workflow | Phase files ARE the handoff payload for cross-model delegation |
| [`/receive-handoff`](.agent/workflows/receive-handoff.md) | Workflow | Receiving agent reads phase files as execution instructions |
| [`/hc-sdlc`](.agent/workflows/hc-sdlc.md) | Workflow | Phase plans generated during SPARC S→P phases |
| [`/delegate-task`](.agent/workflows/delegate-task.md) | Workflow | Phase files used as CLI worker prompts |

---

## Rules
- **Code contracts are binding.** If Phase N+1 finds the contract doesn't match reality, it must fix Phase N's output, NOT silently adapt.
- **Phases must be independently verifiable.** Each phase has its own verification commands (Rule `execution-protocol.md` §3).
- **Failure tables are non-optional.** Every phase MUST list at least 3 failure scenarios. If you can't think of failures, you haven't understood the problem.
- **Confidence scores per phase.** Apply `decision-routing.md` at phase completion — if score < 60, halt before starting next phase.
- **Don't over-phase.** 3-5 phases for Large tasks, 5-8 for Epic. More than 8 phases = you're probably over-engineering the plan.

## Anti-Patterns
- **Phase Explosion:** Breaking into 15 micro-phases — overhead exceeds benefit. Consolidate.
- **Coupled Phases:** Two phases that both need to modify the same file — violates file ownership. Restructure.
- **Contract Drift:** Phase N's output diverges from its contract — downstream phases fail silently. Always verify contracts at phase boundary.
- **Optimism Bias:** Listing zero failure scenarios — see `critical-thinking-models.md` §3 (Inversion/Pre-Mortem).
