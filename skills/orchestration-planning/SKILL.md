---
name: orchestration-planning
description: Plan and sequence non-trivial work with explicit goals, success criteria, constraints, scope, risks, and verification. Use when Codex needs to turn a broad request into an implementable plan, break work into phases, or choose between direct execution and staged delivery.
---

# Orchestration Planning

Use this skill to build a clear plan without adding ceremony for its own sake.

## Workflow

1. Ground in the code or artifact first.
2. State the target outcome and success criteria.
3. Identify scope, constraints, and likely file or subsystem touch points.
4. Choose a delivery shape:
   - direct execution for small bounded work
   - phased execution for larger or riskier work
   - delegation only if the user explicitly asks for it
5. Define verification before implementation.
6. Record assumptions explicitly instead of hiding them.

## Output Standard

- Short title
- Summary
- Key changes grouped by behavior
- Verification plan
- Assumptions

## Do Not

- force a PM persona on every task
- invent files or interfaces that were not grounded in discovery
- turn a one-file fix into a multi-phase ritual
