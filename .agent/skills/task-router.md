---
description: Task Router — semantic analysis to decompose prompts into domain-specific sub-tasks and route to agents
---

# SKILL: TASK ROUTER

**Trigger:** When @pm receives a complex prompt that spans multiple agent domains and needs to decompose it into sub-tasks for orchestration.

---

## When to Use
- A user prompt touches multiple domains (UI + backend + testing).
- Running `/swarm-execute` to orchestrate parallel agent work.
- Deciding which agents to involve in a party-mode session.
- Breaking a monolithic request into delegatable sub-tasks.

---

## Semantic Decomposition Process

### Step 1: Parse the Prompt
Read the user's prompt and extract **domain signals**:

| Signal Keywords | Domain | Route To | Model |
|----------------|--------|----------|-------|
| "requirements", "user story", "what if", "edge case" | Requirements | @ba | `GEMINI-H/Plan` |
| "architecture", "data model", "API", "database", "structure" | Architecture | @sa | `OPUS/Plan` |
| "UI", "layout", "design", "color", "responsive", "animation" | Design | @designer | `SONNET/Fast` |
| "implement", "code", "function", "component", "build" | Development | @dev | `SONNET/Fast` |
| "test", "bug", "coverage", "edge case", "QA" | Quality | @qc | `SONNET/Fast` |
| "deploy", "security", "Docker", "CI/CD", "pipeline" | Operations | @devops | `SONNET/Fast` |
| "plan", "prioritize", "scope", "timeline", "sprint" | Management | @pm | `OPUS/Plan` |
| "write", "document", "enrich", "content", "PRD", "glossary" | Content | @ba | `GEMINI-H/Plan` |
| "report", "review", "summary", "changelog" | Reports | @ba | `GEMINI-H/Plan` |

### Step 2: Decompose into Sub-Tasks
Split the prompt into atomic sub-tasks, one per domain:

```markdown
## Task Decomposition: [Original Prompt Summary]

| # | Sub-Task | Domain | Agent | Dependencies | Priority |
|---|---------|--------|-------|-------------|----------|
| 1 | Define requirements and edge cases | Requirements | @ba | None | 1st |
| 2 | Design system architecture | Architecture | @sa | Needs #1 | 2nd |
| 3 | Design UI components | Design | @designer | Can parallel with #2 | 2nd |
| 4 | Implement business logic | Development | @dev | Needs #2 | 3rd |
| 5 | Write tests | Quality | @qc | Can parallel with #4 (TDD) | 3rd |
| 6 | Security review + deploy prep | Operations | @devops | Needs #4, #5 | 4th |
```

### Step 3: Identify Parallelization Opportunities
Mark tasks that can run simultaneously:
- **Sequential:** Task B depends on Task A's output → run A first.
- **Parallel:** Task B and Task C are independent → run together.
- **Sync point:** Both B and C must complete before D starts.

```
@ba (requirements) → ┬─→ @sa (architecture) ──→ @dev (implement) ──→ @devops (deploy)
 └─→ @designer (UI) ──────→ @dev (integrate) ─┘
 @qc (test scaffolds) ─→ @qc (verify) ───┘
```

### Step 4: Route
Prioritize local project skills before falling back to global framework skills:
1. Try looking in `project/skills/` (if it exists) for project-specific overrides.
2. Fall back to global `.agent/skills/` and map tasks to domains.

For each sub-task, prepare a structured handoff:
```markdown
**TO:** @agent
**TASK:** [Specific, actionable description]
**INPUT:** [What SOT docs or previous outputs to read]
**OUTPUT:** [What file/artifact to produce]
**DEADLINE:** [After which task, or "immediate"]
**MODEL:** [Model Name] — [Mode] (`CODE/Mode`) — [reason]
```

---

## Routing Rules
1. **Never route a task to the wrong domain** — @designer doesn't write tests, @qc doesn't write UI.
2. **Respect Rule `execution-protocol.md`** — max 3–4 agents running simultaneously.
3. **Always identify the critical path** — the longest sequential chain determines total time.
4. **Broadcast context at sync points** — use `context-juggler` skill when parallel tasks converge.
