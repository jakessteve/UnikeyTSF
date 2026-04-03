---
name: debugging-forensics
description: Investigate failures, regressions, and unclear runtime behavior through evidence-first debugging. Use when Codex needs to reproduce an issue, isolate a root cause, compare hypotheses, or plan a safe fix for a bug or incident.
---

# Debugging Forensics

## Workflow

1. Reconstruct the failing path from evidence.
2. State the leading hypotheses.
3. Eliminate hypotheses with the cheapest reliable checks first.
4. Isolate the root cause before editing.
5. Verify the fix against the original failure mode and at least one nearby edge case.

## Avoid

- editing before the failing path is understood
- treating symptoms as proof of root cause
- repeating the same failed check without new information

## Example Uses

- isolating a failing API path from logs and tests
- comparing two likely root causes for a regression
- proving whether a bug is in business logic, configuration, or integration wiring
