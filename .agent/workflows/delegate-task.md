---
description: Delegate Task - streamlined pipeline for delegating scoped work to CLI worker agents via spawn-agent
---

# WORKFLOW: /delegate-task

Triggered when the orchestrator decides to delegate a scoped task to a Gemini CLI worker agent.

## Prerequisites
- Gemini CLI installed globally (`gemini --version`)
- spawn-agent skill available at `~/.gemini/antigravity/skills/spawn-agent/`
- Task passes the Decision Gate in `worker-delegate` skill

## Steps

### Step 0 — Verify Plan Exists (Post-Plan Gate)
Before delegating ANY non-trivial task (>3 files):
1. **Check:** Does an approved implementation plan exist for this work?
 - If YES → proceed to Step 1.
 - If NO → **STOP.** Write the plan first, get user approval, then return here.
2. **Exception:** Quick inline tasks via `-p` flag (1-2 files, trivial fixes) can skip this step.

> [!IMPORTANT]
> This gate exists to prevent the #1 process violation: @pm writing a plan then jumping straight to code editing instead of delegating. Ref: Rule `no-code-boundary.md` §Post-Plan Enforcement Gate.

### Step 1 — Assess Scope
@pm evaluates the task against the Decision Gate (`worker-delegate` skill):
1. Is this task better handled by persona-switching or CLI worker?
2. If CLI worker → proceed. If persona-switching → exit this workflow.
3. Select template: `implementation-task.md`, `research-task.md`, or `bugfix-task.md`.

### Step 2 — Compose Prompt
@pm (or delegating agent) writes the worker prompt:
1. Create `.agent/spawn_agent_tasks/` directory if it doesn't exist.
2. Copy the selected template from `.agent/spawn_agent_tasks/templates/`.
3. Fill ALL mandatory sections (Architecture Context, Reference File, File Scope, Verification, Report Format).
4. Save as `.agent/spawn_agent_tasks/<task-name>.md`.

> [!IMPORTANT]
> Prompt quality = output quality. Workers start with ZERO context and cannot ask questions.

### Step 3 — Safety Net
Before spawning:
```bash
git stash push -m "pre-spawn-backup"
```
> Works identically in PowerShell and Bash (git is cross-platform).

### Step 4 — Spawn Worker

Use the appropriate spawn command from `orchestrator-delegation-guide.md` §Spawn Commands.

Choose the matching mode:
- **Implementation tasks** → `-ApprovalMode AutoEdit` / `--auto-edit`, timeout 300s
- **Research tasks** → `-ApprovalMode Yolo` / `--yolo`, timeout 120s

### Step 5 — Review Output
Follow the Post-Spawn Review Checklist in `orchestrator-delegation-guide.md`:
1. Read output log → `git diff` → scope compliance check → `npm run lint && npm run build`

### Step 6 — Report
- **Success:** Summarize changes, commit with descriptive message
- **Partial:** List what was completed vs what still needs work
- **Failed:** Root cause analysis, restore via `git stash pop`, try alternative approach

If failed, do NOT re-spawn with the same prompt. Fix the prompt or do the work via persona-switching.

---

## Output Files
| File | Location |
|------|----------|
| Task prompts | `.agent/spawn_agent_tasks/<name>.md` |
| Worker output logs | `.agent/spawn_agent_tasks/output-*.log` |

---

## Constraints
- Max 2 concurrent workers (even with Ultra tier, be conservative initially)
- CLI workers count toward `execution-protocol.md §8` limits
- Never chain spawns: Review output A → decide → then spawn B if needed
- Add `.agent/spawn_agent_tasks/` to `.gitignore`
