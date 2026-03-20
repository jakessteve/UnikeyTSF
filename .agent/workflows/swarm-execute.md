---
description: Swarm Execute - coordinated multi-agent wave pipeline for complex tasks requiring parallel execution
---

# WORKFLOW: /swarm-execute (Full Pipeline Orchestration) — Per-wave (use `model-selector` skill)

Triggered when a complex task requires the full agent pipeline to execute in coordinated waves.

## Prerequisites
- Clear task description from User
- Rule `anti-patterns-swarm.md` §3 (anti-context-overflow) activated
- Max 4 parallel threads enforced
- **Max 5 total agents per wave** (hard cap per `execution-protocol.md` §6)

## Steps

### Step 1 — Decompose & Route
@pm activates `task-router` skill:
1. Parse the user's prompt for domain signals.
2. Decompose into sub-tasks with dependencies.
3. Identify parallelization opportunities.
4. Create the execution plan with waves.

### Step 1.5 — Pre-Wave Safety Checks
Before executing any wave, @pm MUST complete these safety steps (Rule `anti-patterns-swarm.md` §7):

1. **File Ownership Map:** Assign every file that will be touched to exactly one agent (`anti-patterns-swarm.md` §7.1). No two agents may write to the same file in parallel. **CRITICAL: Warn agents that `multi_replace_file_content` has strict whitespace/line-number sensitivity. Disable auto-formatters during parallel execution to prevent stale context failures.**
2. **Dependency Graph:** Draw inter-agent dependencies. If cycles exist → break them with stubs/mocks (`anti-patterns-swarm.md` §7.3).
3. **Priority Assignment:** Label each sub-task P0/P1/P2. Ensure P2 work cannot starve P0/P1 of parallel slots (`anti-patterns-swarm.md` §7.5).
4. **Scope Boundaries:** For each agent, list exactly which files they MAY write to and which they MAY read only.
5. **Hallucination Firewall Activation:** Remind all agents that claims received from other agents MUST be verified before use (`anti-patterns-swarm.md` §7.4).
6. **Task Batching Rule (MANDATORY):** Group related mechanical tasks (e.g., lint fixes, console cleanup, a11y patches) into a single agent prompt instead of spawning one agent per trivial fix. Max **5 agents per wave** (hard cap). If decomposition produces >5 tasks, batch the smallest ones together. Violating this cap is a §8 Swarm Extravagance anti-pattern.
7. **Context Injection:** All spawned workers MUST receive role-scoped `.agent/indexes/AGENTS-LITE-{role}.md` as their context rulebook (NOT the full `AGENTS.md`). Workers load skills on-demand from `.agent/skills/MANIFEST.md`. See `instructions.md` and `execution-protocol.md` §8.1.

### Step 2 — Wave 1: Requirements & Architecture — `OPUS/Plan`
Run in parallel (max 2 threads):
- **@ba:** Analyze requirements, identify edge cases, surface blind spots using `requirement-interviewer` skill.
- **@sa:** Begin architecture design once @ba produces initial requirements.

**Sync point:** Wait for both. Use `context-juggler` to broadcast:
- Requirements summary → all agents
- Data models and API names → @dev, @designer, @qc

#### ⇄ Model Handoff Gate: Wave 1 → Wave 2 (Planning → Coding)
> Wave 1 runs on `OPUS/Plan` (reasoning-heavy). Wave 2 runs on `SONNET/Fast` (code execution). Run `model-selector` Step 5. If the current session model is Sonnet and Wave 2 involves substantial coding (>3 files), evaluate whether executing `/handoff` to Opus would improve results. Include Wave 1 outputs (requirements, architecture) in the handoff artifact's Strategic Context.

### Step 3 — Wave 2: Implementation (Parallel) — `SONNET/Fast`
Run in parallel (max 3 threads):
- **@designer:** Design UI components based on requirements and architecture.
- **@dev:** Implement business logic and API based on @sa's architecture.
- **@qc:** Write test scaffolds (TDD red phase) based on requirements.

**Sync point:** Wait for all three. Use `context-juggler` to verify:
- @designer's API calls match @dev's endpoint names
- @qc's test data matches @dev's output format
- Resolve conflicts via `conflict-resolver` skill

### Step 4 — Wave 3: Integration & Testing — `SONNET/Fast`
Run in sequence:
1. **@dev:** Integrate UI components with backend (if both were parallel).
2. **@qc:** Run full test suite — unit, integration, and if applicable, E2E.
3. **@qc → @dev fix loop:** If tests fail, activate `conflict-resolver` bug fix loop (background).

#### ⇄ Model Handoff Gate: Wave 3 → Wave 4 (Coding → Review)
> If Wave 3 was executed on a fast coding model (Gemini), Wave 4's security scan and PM review may benefit from switching to `SONNET/Fast` for deeper analysis. Run `model-selector` Step 5. If handoff warranted, include the Swarm Execution Report so far in the handoff artifact.

### Step 5 — Wave 4: Review & Delivery — `SONNET/Fast`
1. **@devops:** Security scan and build verification.
2. **@pm:** Aggregate ALL outputs into a single delivery Artifact:

```markdown
# Swarm Execution Report — [Task Name]
**Date:** YYYY-MM-DD | **Duration:** [total time]

## Agents Involved
| Agent | Task | Status | Output |
|-------|------|--------|--------|
| @ba | Requirements | | [file path] |
| @sa | Architecture | | [file path] |
| @designer | UI Design | | [file path] |
| @dev | Implementation | | [file path] |
| @qc | Testing | | [file path] |

## Test Results
- Tests: X/Y passing (Z%)
- Type check: PASS
- Security: PASS

## Conflicts Resolved
- [List of conflicts and resolutions]

## Files Created/Modified
[Complete file list]

## User Action Required
 Review and approve, or request changes.
```

### Step 6 — Handoff
Present the Swarm Execution Report to User via Artifact.
On approval → hand off to @pm for standard pipeline continuation.
On changes → route feedback to specific agents and re-execute affected waves only.

---

## Output Files
| File | Location |
|------|----------|
| Swarm execution report | `.hc/swarm/YYYY-MM-DD-[task-slug].md` |
| Conflict logs | `.hc/logs/conflicts/` |
| Fallback logs (if any) | `.hc/logs/fallbacks/` |
