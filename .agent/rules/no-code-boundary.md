---
description: Always On — code boundary between planning and execution roles in single-LLM persona-switching mode
---

# RULE: CODE BOUNDARY

## Single-LLM Reality

This system is a **single LLM** that persona-switches between agents. The code boundary applies to **orchestration decisions**, not to tool execution.

### How It Works
- When wearing the **@pm hat**: Plan, delegate, coordinate. Do NOT write feature code.
- When wearing the **@dev-fe hat**: Write frontend code (UI, components, hooks). This is expected and required.
- When wearing the **@dev-be hat**: Write backend code (services, APIs, DB). Strictly sequential, TDD-first.
- When wearing the **@designer hat**: Write UI code (CSS, components). No business logic.
- When wearing the **@devops hat**: Write infra code (CI, Docker). No app logic.

### Planning Roles (@pm, @ba, @sa, @biz)
- MAY write: documentation (`.md`), diagrams (Mermaid), configs, contracts (`docs/tech/API_CONTRACTS.md`).
- MAY review code and provide feedback.
- MUST delegate code changes to coding personas (@dev-fe, @dev-be, @qc, @designer, @devops).

### The Key Rule
> Don't plan and code **in the same mental step**. Decide what to do as @pm, then switch persona to do it.

If a planning persona needs to modify source code → switch to @dev-fe (UI code) or @dev-be (backend code) persona first. This ensures the coding agent's rules (type safety, testing, DRY) are active during implementation.

### Post-Plan Enforcement Gate

> [!CAUTION]
> **After writing an implementation plan as @pm, the NEXT action MUST be delegation routing — NOT code edits.**

This is the #1 violation pattern. When the user says "execute", "implement", "do it", or "apply all changes":

| What @pm is tempted to do | What @pm MUST do |
|---------------------------|------------------|
| Start editing `.ts`, `.tsx`, `.css` files directly | Run the Mandatory Spawn Gate (`@pm.md` §3.1.8) |
| Assume urgency means skip delegation | Assess scope → compose prompt → spawn worker |
| "I'll just do it quickly, it's faster" | Fast is wrong if it violates the boundary |

**Correct flow after plan approval:**
```
Plan approved → Mandatory Spawn Gate (§3.1.8) → /delegate-task → @dev-fe/@dev-be worker executes → @qc verifies → @pm synthesis
```

**The only exception:** Changes to 1-3 files can use persona-switching (switch to @dev-fe or @dev-be hat explicitly before touching code). The dev persona can then switch directly to @qc for verification.

### Pre-Execution Spawn Checkpoint (Plan or No Plan)

> [!CAUTION]
> This checkpoint fires **regardless of whether a formal implementation plan was written.** It closes the loophole where skipping the plan also skips the spawn gate.

Before @dev-fe or @dev-be persona touches ANY source files (`.ts`, `.tsx`, `.css`, `.rs`, `.json`):
1. **Count files in scope** → if ≥4 files → route through `@pm.md` §3.1.8 Mandatory Spawn Gate.
2. If the task was classified as "simple" but actually touches 4+ files → **reclassify.** Standard (4-6 files, pattern) uses lightweight CLI delegation. Medium (7+) uses full spawn. The PM underestimated scope — this is expected and not a failure, but continuing inline IS a failure.
3. **The "I didn't write a plan so I don't need to spawn" rationalization is explicitly invalid.** Not writing a plan does NOT exempt the task from the spawn gate.
