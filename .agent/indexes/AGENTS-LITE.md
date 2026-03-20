# Agent Skills Index (LITE)

> Auto-generated minimal index for CLI workers. Reduces token bloat by 70%.

## ⚠️ Conversation Entry Rule
**Always begin as @pm.** Run Pre-Delegation Pipeline & Mandatory Spawn Gate.

## Core Roles
- `@pm`: Planning, orchestration (`task-router`, `roadmap-architect`)
- `@dev-fe`: Frontend implementation (`react-patterns`, `typescript-expert`)
- `@dev-be`: Backend implementation (`systematic-debugging`, `typescript-expert`)
- `@qc`: Testing, quality (`playwright-testing`, `verification-before-completion`)
- `@sa`: Architecture (`senior-architect`, `c4-architecture`)

*(See full `AGENTS.md` for @biz, @ba, @devops, @designer, @user-tester, @whitehat-hacker if needed).*

## Essential Rules (`.agent/rules/`)
- `anti-patterns-core`: No hallucination, context overflow, source trust
- `execution-protocol`: SPARC, tiered verification gate
- `code-standards`: DRY, no `any`, 80% coverage
- `security-standards`: Zero hardcoded secrets

*(Other rules lazy-loaded as needed).*

## Skills & Workflows
> Run `list_dir .agent/skills` to discover available skills. Use `view_file .agent/skills/{skill-name}/SKILL.md` to load a skill on-demand.
> Run `view_file .agent/workflows/MANIFEST.md` for workflow catalog.
