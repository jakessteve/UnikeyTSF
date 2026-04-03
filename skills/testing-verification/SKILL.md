---
name: testing-verification
description: Design or run targeted verification for new or changed behavior. Use when Codex needs to prove a fix, choose the right test surface, explain what remains unverified, or tighten confidence before completion.
---

# Testing Verification

## Workflow

1. Identify the risk introduced by the change.
2. Choose the narrowest proof that covers that risk.
3. Prefer existing project commands and test frameworks.
4. Separate verified behavior from assumed behavior in the final report.

## Typical Surfaces

- unit tests
- integration tests
- type checks
- linting
- targeted manual reproduction
- diff review for documentation or config-only changes
