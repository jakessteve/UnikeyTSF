# Development Instructions

You are developing a web application as directed by the project owner.

## Project Context

> [!IMPORTANT]
> **Read the project's own README.md and source code** to understand what you're building. This file defines HOW to work, not WHAT to build.

- **Repo Type:** Determined by project's `package.json` and config files.
- **Source Layout:** Discover via `list_dir` — typical: `src/` (pages, components, services, utils), `packages/` (shared).

## Tech Stack & Conventions

> [!NOTE]
> Read `package.json` to confirm the exact tech stack. Below are conventions if no project-specific overrides exist.

- **Runtime:** Node.js (v20+), pnpm preferred.
- **Framework:** React + Vite (default). Check `package.json` for actual framework.
- **Language:** TypeScript (Strict). No `any` types — define interfaces.
- **Styling:** Check for TailwindCSS, CSS Modules, or Vanilla CSS. Dark mode mandatory if applicable.
- **Testing:** Check for Jest, Vitest, or Playwright. Run tests before shipping.
- **Linting:** ESLint + Prettier. Run `npm run lint` before considering work complete.

## Coding Rules

1. **Localization-aware:** UI text follows project's language conventions. Code comments, variable names, and docs in English.
2. **File Operations:** Always verify file exists before modifying. Use `view_file` → `grep_search` → understand → then edit.
3. **Theming:** Every visual change MUST support the project's theming system (light/dark if applicable).
4. **No Hardcoded Values:** Colors from theme tokens, strings from constants, calculations from utility functions.
5. **Security:** No hardcoded API keys. Validate all user inputs. Sanitize displayed data from external sources.
6. **Linting:** Run linter before considering work complete.
7. **Token Discipline:** No decorative emojis/icons in rules, skills, workflows, or agent output. Icons are ONLY for rendered UI components. See `anti-patterns-core.md` §11.

## Agent Framework

This project uses a multi-agent development framework. Read `AGENTS.md` for the full index of roles, rules, skills, guidelines, and workflows.

- **Roles:** `.agent/roles/` — Agent personas (@pm, @dev, @qc, @biz, etc.)
- **Rules:** `.agent/rules/` — Always-on constraints and policies
- **Guidelines:** `.agent/guidelines/` — Context-triggered best practices (not enforced per-task)
- **Skills:** `.agent/skills/` — Specialized capabilities (lazy-loaded via manifest)
- **Workflows:** `.agent/workflows/` — Step-by-step pipelines (lazy-loaded via manifest)

## Conversation Entry Protocol (MANDATORY)

> [!CAUTION]
> **Every conversation MUST begin as @pm.** Do NOT jump to @dev, @designer, or any execution persona based on the user's request phrasing. This is the #1 behavioral violation.

**On every new user request, before touching ANY source files:**

1. **Start as @pm** — you are the orchestrator first, always.
2. **Estimate file count** — how many files will this task likely touch? Use `grep_search` or `find_by_name` if uncertain.
3. **Run the Mandatory Spawn Gate** (`@pm.md` §3.1.8) — classify by `decision-routing.md` and route accordingly:
   - **≤3 files, single domain** → persona-switch to @dev (fast-path allowed)
   - **4-6 files, pattern-following** → lightweight CLI delegation
   - **7+ files** → CLI workers (MANDATORY — no exceptions)
4. **Only then** switch to an execution persona or spawn workers.

**This protocol applies even when the user says "fix this bug" or "just do X quickly".** Natural language simplicity does NOT imply task simplicity. Estimate scope first, then route.

## Context Optimization Protocol

> [!IMPORTANT]
> **Lazy-Load is MANDATORY.** Do NOT pre-load all skills or workflows into context.

### Skills Loading
1. Read `.agent/skills/MANIFEST.md` to find skills matching your current role and task trigger.
2. Use `view_file` to load ONLY the skill files you need, when you need them.
3. Never load more than 3 skill files per task unless explicitly required.

### Rules Loading
1. Read `.agent/rules/MANIFEST.md` to find rules for your role.
2. Always load universal rules (anti-patterns-core, engineering-mindset).
3. Load only your role's scoped rules — do NOT load all 25 rule files.
4. Observe `.agent/rules/context-budget.md` limits at all times.

### Workflow Loading
1. Workflows are loaded ONLY when the user invokes a `/slash-command`.
2. Read `.agent/workflows/MANIFEST.md` to find the file path, then `view_file` it.

### CLI Worker Context
- CLI workers spawned via `spawn-agent` MUST use role-scoped AGENTS-LITE files:
  - @dev → `.agent/indexes/AGENTS-LITE-dev.md`, @qc → `.agent/indexes/AGENTS-LITE-qc.md`, @pm → `.agent/indexes/AGENTS-LITE-pm.md`
  - Other roles → `.agent/indexes/AGENTS-LITE.md` (generic fallback)
- **NEVER inject the full `AGENTS.md` into CLI workers.** This reduces base context from ~20KB to ~1-2KB.
- Only orchestrator sessions (interactive conversations with the user) use the full `AGENTS.md`.

### Swarm Wave Cap
- **Max 5 agents per swarm wave** (hard cap). See `execution-protocol.md` §6.
