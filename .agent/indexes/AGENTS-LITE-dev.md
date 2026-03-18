# Agent Context (Dev Worker)

> Minimal context for CLI worker agents in @dev role. Load skills on-demand from `.agent/skills/MANIFEST.md`.

## Project
- Read `package.json` for tech stack. Read `README.md` for project context.
- Source: `src/` (pages, components, services, utils). Packages: `packages/`.

## Your Rules (from `rules/MANIFEST.md` — @dev section)
- `rules/code-standards.md` — DRY, no `any`, 80% coverage
- `rules/git-hygiene.md` — Conventional commits, atomic changes
- `rules/error-handling-standards.md` — Structured error types
- `rules/performance-budget.md` — Bundle size limits
- `rules/engineering-mindset.md` — MVP-first, mobile-first
- `rules/context-budget.md` — Context window limits (ALWAYS observe)

## Skill Loading Protocol
1. Read `.agent/skills/MANIFEST.md` — find your role's skills table
2. When a trigger matches your current task, `view_file` that skill
3. Do NOT pre-load skills you don't need

## Coding Rules
1. UI text follows project locale. English code. Verify file exists before modifying.
2. Support theming (light/dark) using semantic tokens.
3. No hardcoded values. Run linter before completion.
4. No decorative emojis/icons outside rendered UI components.
