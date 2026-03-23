---
description: Pre-spawn validation and post-spawn timeout handling for CLI worker delegations
---

# RULE: SPAWN GOVERNANCE

Unified rule for validating delegation prompts before spawning and handling worker timeouts after spawning.

---

## 1. Pre-Spawn Template Validation

**Scope:** Runs inline during the COMPOSE step of delegation protocol, before spawning any CLI worker.

### Quick Validation (Standard Tier -- 4-6 files, inline `-Prompt`)
Only these checks are mandatory:
- **Placeholder check** -- no unfilled `[...]` or `<...>` tokens
- **File path check** -- all referenced files exist

### Full Validation (Medium+ Tier)

| Check | Rule |
|---|---|
| **Placeholders** | No `[...]`, `<...>`, `[TODO]`, `[PLACEHOLDER]` remain |
| **Required sections** | Goal, Architecture Context, File scope, Constraints, Verification commands, Report format |
| **File paths** | All referenced paths verified via `Test-Path` / `test -f` |
| **Scope** | Modify list <=10 files; no overlap with off-limits |
| **Length** | 200-8000 chars (sweet spot: 1000-4000) |

### Validation Output
```
Template Validation:
  No unfilled placeholders
  All required sections present
  File paths verified (X/X exist)
  Scope within limits (Y files)
  Ready to spawn
```

If ANY check fails -- **BLOCKED**. Fix before spawning.

### Exceptions
- Quick inline prompts (`-p` flag): must still have goal, file scope, and constraints.
- Research tasks (read-only): skip file path validation for "where to look" section.

---

## 2. Post-Spawn Timeout Escalation

**Scope:** All spawn-agent delegations that exit with code 124 (timeout).

### Escalation Ladder

**Level 1 -- Retry with Extended Timeout (1 attempt)**
Original timeout 2x (e.g., 300s to 600s). Only if:
- Worker produced partial output showing progress
- No error indicators in partial output
- NOT a Mechanical task (Mechanical tasks use `-Timeout 60` -- if they fail, they are mis-scoped)

**Level 2 -- Split Task**
If extended timeout also fails:
1. Read partial output to identify what completed
2. Split into 2-3 sub-tasks with original timeout
3. Merge results after all complete

**Level 3 -- Complete Manually**
If sub-tasks also struggle:
1. Use partial worker output as context
2. Persona-switch to @dev-fe/@dev-be, complete manually
3. Log the failure in session audit trail

**Level 4 -- Escalate to User**
If manual completion also fails:
1. Present what was attempted and what failed
2. Ask user for guidance on scope or approach

### Timeout Guidelines

**Consult `model-routing.md` Section 6.1** for default timeout values based on Task Complexity.

**Retry Policy:**
- **Mechanical tasks (-Timeout 60):** Do not retry. Fix prompt instead.
- **Integration/Architecture tasks:** Retry once with 2x timeout if partial progress was made, otherwise split task.

---

## 3. Rate-Limit Prevention (429 Avoidance)

**Scope:** All multi-agent spawn workflows using Gemini CLI.

### Concurrency Limits
- **Max simultaneous Gemini CLI workers:** 2
- **Orchestrator counts as 1 active session** — total API sessions = orchestrator + workers = 3 max

### Wave Protocol
When spawning >2 workers total:
1. **Wave 1:** Spawn workers 1-2 with `-Async` flag
2. **Wait:** All Wave 1 workers must complete before proceeding
3. **Cool-down:** Wait **30 seconds** after wave completion
4. **Wave 2:** Spawn workers 3-4 (if needed)
5. Repeat until all tracks are complete

### If 429 Still Occurs
1. Reduce to **1 worker at a time** (sequential spawning)
2. Increase inter-spawn delay to 60 seconds
3. If persistent, switch to persona-based execution (no CLI workers)
