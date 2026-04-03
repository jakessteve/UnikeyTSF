# Codex Agent Pack

This pack adapts the Antigravity operating model to Codex.

## Core Operating Model

- Start with discovery. Read the relevant code, config, and docs before proposing changes.
- Plan explicitly for non-trivial work. Small tasks can be executed directly once the scope is clear.
- Prefer one strong main agent over artificial persona theater.
- Use role playbooks as optional lenses, not mandatory identity switches.
- Keep context lean. Load only the rules, workflows, and skills that help the current task.
- Verify before claiming completion. Reviews, tests, and diffs matter more than polished narration.

## What Changed From Antigravity

- No mandatory `@pm` entry step.
- No hard "planning roles cannot code" boundary.
- No Gemini/Claude worker shell, no external spawn wrapper, no CLI-managed swarms.
- Delegation uses Codex-native sub-agents when the user explicitly asks for delegation or parallel work.
- Model guidance is mapped to Codex models in `references/codex-model-routing.md`.

## Pack Layout

- `rules/` contains always-on policies and quality bars.
- `roles/` contains optional operating lenses for different kinds of work.
- `skills/` contains Codex-native skills with `SKILL.md`.
- `workflows/` contains repeatable playbooks for planning, review, debugging, and delegation.
- `templates/` contains reusable artifacts.
- `references/` contains mapping notes and background material.
- Start with `references/task-routing.md` when you need a quick task-to-skill/workflow/template map.
- Use `references/migration-map.md` for historical mapping context when adapting old Antigravity habits to this pack.

## Default Task Flow

1. Ground in the codebase or artifact.
2. Classify the work: quick fix, implementation, review, debugging, design, research, or planning.
3. Load the smallest useful set of rules and one matching workflow or skill. Use `references/task-routing.md` when helpful.
4. Execute directly unless delegation is explicitly requested by the user and allowed by pack policy.
5. Verify with the narrowest command set that proves the change.
6. Report findings or outcomes concisely.

## Delegation Policy

- Codex sub-agents are optional, not mandatory.
- Use delegation only when the user explicitly asks for sub-agents, delegation, or parallel work.
- Prefer `explorer` for read-only investigation and `worker` for bounded implementation.
- Give each worker a disjoint file set and a concrete report format.
- Do not wait on sub-agents unless the main task is blocked on their result.

## Review Policy

- Findings first, ordered by severity.
- Quote the smallest relevant code region and always reference the file.
- Call out missing tests, regressions, and risk before summarizing strengths.

## Frontend Policy

- Preserve existing product language when editing an established UI.
- For new UI, avoid generic layouts and default typography.
- Use a clear visual direction, responsive behavior, and intentional motion.

## Safety Policy

- Never fabricate files, APIs, tools, or model capabilities.
- Never rely on non-existent tool names such as `list_dir`, `view_file`, or `grep_search`.
- Never recommend destructive git recovery unless explicitly requested.
- Treat old Antigravity shell scripts as historical references only.
