---
description: CLI Worker Lifecycle -- delegation gate, prompt validation, output parsing, timeout handling, multi-worker coordination, and session audit. One-stop reference for all CLI worker operations.
---

# SKILL: CLI WORKER LIFECYCLE

**Trigger:** Any CLI worker delegation (Medium+ tasks, post-plan execution, multi-worker coordination).

> Consolidates: worker-delegate, orchestrator-delegation-guide, worker-output-parsing, worker-session-audit, multi-worker-coordination, agent-cli-compatibility

---

## 1. Delegation Gate

### When CLI Workers Are MANDATORY
- Task involves **10+ files** (Large/Epic)
- Post-plan execution: @pm has user-approved implementation plan (any file count)
- Post-implementation review: spec + quality review workers

### When Workers Are Default (Medium: 4-9 files)
Workers are the default. Valid exemptions:
1. All sub-tasks have complex inter-file dependencies requiring shared context
2. Task requires interactive user discussion mid-execution
3. Worker infrastructure unavailable

> [!CAUTION]
> **Status Quo Bias Check:** "Am I avoiding spawning because it's genuinely wrong, or because inline is more convenient?" If convenience -- spawn.

### When Persona-Switching Is Fine
- Task touches <=3 files (Trivial/Small)
- CLI workers optional but allowed if context pressure is high

---

## 2. Pre-Spawn Checklist

| Question | Required |
|----------|----------|
| Can a dev with zero project knowledge execute this prompt? | Yes |
| Is file scope explicitly listed (modify/read-only/off-limits)? | Yes |
| Is tech stack context included? | Yes |
| Is there a reference file to follow? | Yes |
| Is expected output format defined? | Yes |

If ANY answer is No -- fix prompt first, then spawn.

### Spawn Command Quick Reference

**Windows (PowerShell):**
```powershell
# Using template file
.\.agent\scripts\spawn-agent.ps1 -TaskFile ".agent\spawn_agent_tasks\task-name.md" -Agent gemini -Mode auto_edit

# Inline prompt (Standard tier, 4-6 files)
.\.agent\scripts\spawn-agent.ps1 -Prompt "..." -Agent gemini -Mode auto_edit

# Mechanical (lint, typo, rename)
.\.agent\scripts\spawn-agent.ps1 -Prompt "..." -Agent gemini -ApprovalMode Yolo -Timeout 60
```

### Template Selection
| Task | Template | Timeout |
|------|----------|---------|
| Bug fix | `templates/bugfix-task.md` | 120s |
| New feature | `templates/implementation-task.md` | 300s |
| Refactoring | `templates/refactoring-task.md` | 300s |
| Research (read-only) | `templates/research-task.md` | 120s |
| Migration | `templates/migration-task.md` | 300s |

---

## 3. Output Parsing

### Locate and Parse
```powershell
Get-ChildItem .agent\spawn_agent_tasks\output-*.log | Sort-Object LastWriteTime -Descending | Select-Object -First 1
```

### Status Signals
| Status | Action |
|--------|--------|
| `DONE` | Standard parse -- extract changes, verification |
| `DONE_WITH_CONCERNS` | Parse `### Concerns` -- flag for attention |
| `NEEDS_CONTEXT` | Parse `### Context Needed` -- provide and re-delegate |
| `BLOCKED` | Escalation ladder: more context -> stronger model -> smaller scope -> user |

### Hidden Failure Indicators (check even on exit code 0)
`error TS`, `FAIL`, `Cannot find module`, `Permission denied`, `SyntaxError`

### Summary Template
```markdown
## Worker Session Summary
| Field | Value |
|-------|-------|
| Agent | Gemini / Codex |
| Duration | Xs |
| Exit Code | 0/1/124 |
| Status | DONE/CONCERNS/BLOCKED |
| Outcome | Success/Partial/Failed |
### Action Required
- [ ] Review `git diff`
- [ ] Run `npm run lint && npm run build`
```

---

## 4. Multi-Worker Coordination

### Wave Execution
- **Max 5 agents per wave** (hard cap)
- Declare file ownership before spawning (no two workers touch same file)
- After each wave, summarize output before starting next wave

### Conflict Detection
After parallel workers complete:
1. Check for file conflicts (same file modified by multiple workers)
2. Check dependency conflicts (one worker's changes break another's assumptions)
3. Manual merge if needed, then re-run verification

---

## 5. Agent Selection

| Agent | Best For | Avoid For |
|-------|----------|-----------|
| **Gemini CLI** | Complex reasoning, architecture, multi-file | Simple mechanical |
| **Codex CLI** | Fast iterations, test-write, mechanical | Novel architecture |

---

## 6. Integration
- Spawn limits: `execution-protocol.md` Section 6 (max depth=2, max agents=5)
- File ownership: `anti-patterns-swarm.md` Section 7.1
- Safety: Commit your work (`git commit`) before spawn, review via `git diff` after
- Validation: `spawn-governance.md` checks before spawning
