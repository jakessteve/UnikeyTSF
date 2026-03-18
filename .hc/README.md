# .hc/ — Project Management SOT Directory

This directory contains project management artifacts used by the HC Software Factory agent framework.

## Structure

| Directory | Purpose | Created By |
|-----------|---------|------------|
| `checkpoints/` | Context Reset Protocol snapshots | Agents (via `anti-patterns.md §4`) |
| `bugs/` | Bug records for tracked defects | `@qc` (via `process-gates.md §3.3`) |
| `decisions/` | Architecture and design decisions | `@pm`, `@sa` |
| `stories/` | User stories and backlog items | `@pm`, `@ba` |
| `feedback/` | User testing feedback reports | `@user-tester` |
| `handoffs/` | Cross-model handoff artifacts | `@pm` (via `/handoff` workflow) |
| `improvements/` | Kaizen notes and improvement proposals | All agents |
| `phases/` | Amateur-proof plan phase files | `@pm` (via `amateur-proof-plans` skill) |

## Convention
- Directories are created on-demand when an agent first needs them
- Files follow naming: `YYYY-MM-DD-[slug].md`
- This directory is gitignored (project management artifacts, not source code)
