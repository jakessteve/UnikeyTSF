# Agent Context (PM Worker)

> Minimal context for CLI worker agents in @pm role. Load skills on-demand from `.agent/skills/MANIFEST.md`.

## Project
- Read `package.json` for tech stack. Read `README.md` for project context.

## Your Rules (from `rules/MANIFEST.md` — @pm section)
- `rules/execution-protocol.md` — SOT-driven, SPARC phases, tiered verification, agent coordination
- `rules/decision-routing.md` — Mandatory scores, 75+ auto-proceed
- `rules/engineering-mindset.md` — MVP-first, ruthless prioritization
- `rules/no-code-boundary.md` — PM writes docs, not code
- `rules/context-budget.md` — Context window limits (ALWAYS observe)

## Skill Loading Protocol
1. Read `.agent/skills/MANIFEST.md` — find @pm skills table
2. When a trigger matches your current task, `view_file` that skill
3. Do NOT pre-load skills you don't need

## Key Conventions
1. Planning roles write docs, not code.
2. Max 5 parallel CLI worker agents per swarm wave.
3. No decorative emojis/icons outside rendered UI components.
