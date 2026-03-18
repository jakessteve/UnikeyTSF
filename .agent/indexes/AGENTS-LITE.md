# Worker Agent Context (Generic Fallback)

> Lightweight context for CLI worker agents. Use role-scoped files when available:
> - `AGENTS-LITE-dev.md` for @dev workers
> - `AGENTS-LITE-qc.md` for @qc workers
> - `AGENTS-LITE-pm.md` for @pm workers
> This file is the generic fallback when no role-scoped LITE file matches.

## Project
- Read `package.json` for tech stack. Read `README.md` for project context.
- Source: `src/` (pages, components, services, utils).

## Skill Loading Protocol
1. Read `.agent/skills/MANIFEST.md` — find your role's skills table
2. When a trigger matches your current task, `view_file` that skill
3. Do NOT pre-load skills you don't need

## Active Rules (load on-demand from `.agent/rules/`)
| Rule | Key Constraints |
|---|---|
| `anti-patterns-core` | No hallucination, 3-strike loop break, circuit breaker |
| `code-standards` | DRY, no `any`, 80% coverage |
| `error-handling-standards` | Structured error types |
| `git-hygiene` | Conventional commits, atomic changes |
| `performance-budget` | Bundle size limits |
| `security-standards` | Zero hardcoded secrets |

## Coding Rules
1. UI text follows project locale. English code.
2. Verify file exists before modifying.
3. No hardcoded values. Run linter before completion.
4. No decorative emojis/icons outside rendered UI components.

## Progress Reporting
After completing each major step, output:
```
[PROGRESS] Step X/Y: <what you just completed>
```
When finished: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
