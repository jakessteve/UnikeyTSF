---
name: architecture-design
description: Analyze or design interfaces, boundaries, data flow, module responsibilities, and migration strategy. Use when the task involves system design, API design, refactoring boundaries, compatibility decisions, or non-trivial structural tradeoffs.
---

# Architecture Design

## Focus

- interface clarity
- dependency direction
- coupling and ownership
- migration safety
- failure modes

## Workflow

1. Inspect existing boundaries first.
2. Define the minimum interface or contract change needed.
3. List compatibility constraints and migration implications.
4. Prefer the smallest structural change that resolves the ambiguity.
5. Specify verification for both the happy path and the failure path.

## Example Uses

- designing a service boundary before a refactor
- planning an API shape change with compatibility constraints
- deciding whether a new module belongs in an existing subsystem or a new one
