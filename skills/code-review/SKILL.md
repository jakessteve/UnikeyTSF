---
name: code-review
description: Perform findings-first code review focused on bugs, regressions, correctness risks, security issues, performance cliffs, and missing tests. Use when the user asks for a review, PR review, audit, or second pass on a change.
---

# Code Review

## Review Order

1. correctness and regressions
2. security and data handling
3. performance or reliability risk
4. missing or weak tests
5. maintainability issues that can cause future defects

## Output Standard

- Findings first, ordered by severity
- File references for each finding
- Brief open questions or assumptions
- Short summary only after findings

## If No Findings

State that directly and note any residual risk or missing verification.
