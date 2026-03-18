---
description: Model Selector — analyzes task characteristics to recommend the optimal AI model and mode per Rule model-routing.md
---

# SKILL: MODEL SELECTOR

**Trigger:** When @pm routes a task and needs to recommend which AI model + mode the User should use.

---

## When to Use
- Routing any task via the Auto-Delegation Decision Table (`@pm.md` §3.0).
- Decomposing tasks via `task-router` skill.
- Starting a `/party-mode` or `/swarm-execute` session.
- User asks "which model should I use for X?"

---

## Selection Process

### Step 1: Extract Task Signals
Parse the task description for keyword signals:

| Signal Keywords | Task Type | Model Code |
|---|---|---|
| "brainstorm", "discuss", "debate", "pros/cons", "compare", "party", "retrospective" | Brainstorming | `OPUS/Plan` |
| "research", "analyze", "investigate", "market", "gap analysis", "feasibility" | Research | `GEMINI-H/Plan` |
| "architecture", "design system", "API contract", "pseudocode", "data model", "trade-off" | Architecture | `OPUS/Plan` |
| "write docs", "PRD", "content", "enrich", "interpret", "glossary", "release notes", "data table" | Content | `GEMINI-H/Plan` |
| "implement", "code", "build", "fix bug", "refactor", "component", "function" | Coding | `SONNET/Fast` |
| "deploy", "CI/CD", "Docker", "security scan", "pipeline", "infrastructure" | DevOps | `SONNET/Fast` |
| "test", "QA", "coverage", "E2E", "scaffold test", "verify" | Testing | `SONNET/Fast` |
| "UI", "CSS", "design", "animation", "responsive", "style", "layout", "dark mode" | UI/UX | `SONNET/Fast` |
| "quick fix", "small change", "typo", "rename", simple tasks ≤3 files | Quick Fix | `SONNET/Fast` |
| "report", "sprint review", "changelog", "summary", "release notes" | Reports | `GEMINI-H/Plan` |
| "security audit", "pentest", "threat model" | Security Audit | `OPUS/Plan` |
| "delegate", "spawn worker", "parallel", "isolated research", scoped 10+ files | CLI Worker | `GEMINI-CLI/Worker` |

> [!NOTE]
> **CLI Worker (`GEMINI-CLI/Worker`):** Not a model switch — this delegates the task to a Gemini CLI process via the `worker-delegate` skill and `/delegate-task` workflow. Use when the task qualifies per the Decision Gate (scoped, independent, context-heavy). See `execution-protocol.md §8.1`.

### Step 2: Handle Mixed Signals
If the task matches **multiple** types:
1. Count signal matches per type.
2. Estimate effort distribution (e.g., 70% coding, 30% brainstorming).
3. Select the **dominant** model + mode (highest effort %).
4. Note the **secondary** model as a suggestion for later phases.

### Step 3: Check for Overrides
- **User preference** always overrides the matrix.
- **Current session model** — if the user is already in a conversation with a specific model, note the recommendation but don't demand a session restart for minor mismatches.

### Step 4: Output Recommendation
Include in the task routing output:

```markdown
 **Model Recommendation**
- **Primary:** [Model Name] — [Mode] (`CODE/Mode`) — [reason]
- **Secondary:** [Model Name] — [Mode] (`CODE/Mode`) — for [which sub-task], if applicable
```

### Step 5: Handoff Trigger Check (NEW — Cross-Model Delegation)
After outputting the recommendation, check if a **model handoff** is warranted:

1. Compare the recommended model's **category** to the current session model.
2. Consult Rule `model-routing.md` Handoff Boundaries matrix.
3. If a handoff IS warranted:
 - Output the actionable switch instruction (see format below)
 - Trigger `/handoff` workflow per `@pm.md` §3.10
4. If a handoff is NOT warranted:
 - Continue in the current session — no action needed.

**Actionable Switch Instruction Format:**
```markdown
 **Model Switch Recommended**
- The next phase (**[phase name]**) would benefit from **[Model Name]**.
- **Current model:** [current] → **Recommended:** [new] — [1-line reason]
- **Action:** I will generate a `/handoff` artifact. Then start a new conversation 
 on **[Model Name]** and type: `/receive-handoff`
- **Risk Level:** LOW / MEDIUM — [brief justification]
```

If the switch is NOT significant (same model category), output instead:
```markdown
 **Model Note:** Staying on current model — switching would not provide significant benefit.
```

---

## Examples

**Task:** "Let's brainstorm ideas for the new calendar widget"
→ Recommended: Opus — Plan (`OPUS/Plan`) — deep thinking requires extended reasoning

**Task:** "Implement the hexagram calculation engine with full test coverage"
→ Recommended: Sonnet — Fast (`SONNET/Fast`) — coding + testing is Sonnet's strength

**Task:** "Write all the Vietnamese interpretation texts for the 12 palaces"
→ Recommended: Gemini High — Plan (`GEMINI-H/Plan`) — bulk multilingual research/content

**Task:** "Design the architecture for the new astrology module, then implement it"
→ Primary: Opus — Plan (`OPUS/Plan`) — architecture phase
→ Secondary: Sonnet — Fast (`SONNET/Fast`) — implementation phase
→ Model Switch Recommended: Opus → Sonnet after architecture is approved.

**Task:** "Fix the typo in the button label"
→ Recommended: Sonnet — Fast (`SONNET/Fast`) — trivial coding change

**Task (PM completing plan, next phase is bulk coding):** "Plan complete. Next: implement 8 files."
→ Primary: Sonnet — Fast (`SONNET/Fast`) — large implementation, coding priority
→ Model Switch Recommended: Triggering `/handoff` workflow.

