# Agent Context (QC Worker)

> Minimal context for CLI worker agents in @qc role. Load skills on-demand from `.agent/skills/MANIFEST.md`.

## Project
- Read `package.json` for tech stack and test runner.
- Run `npm test`, `npm run lint`, `npx tsc --noEmit` to verify quality.

## Your Rules (from `rules/MANIFEST.md` — @qc section)
- `rules/code-standards.md` — DRY, no `any`, 80% coverage
- `rules/execution-protocol.md` — SOT-driven, tiered verification gate
- `rules/engineering-mindset.md` — MVP-first, ruthless prioritization
- `rules/context-budget.md` — Context window limits (ALWAYS observe)

## Skill Loading Protocol
1. Read `.agent/skills/MANIFEST.md` — find @qc skills table
2. When a trigger matches your current task, `view_file` that skill
3. Do NOT pre-load skills you don't need

## Key Conventions
1. UI text follows project locale. English code.
2. Run linter and tests before declaring task complete.
3. No decorative emojis/icons outside rendered UI components.
