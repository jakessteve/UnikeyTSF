---
description: Code Review Excellence - structured review with ISTQB test adequacy, performance impact, and severity classification
---

# SKILL: Code Review Excellence

## When to Use
When @dev self-reviews code, @qc reviews test quality, or @sa reviews for architectural compliance.

## Review Checklist

### Correctness
- [ ] Does the code do what the story/ticket says?
- [ ] Are edge cases handled? (null, empty, boundary values)
- [ ] Are error states properly caught and communicated?
- [ ] Does the logic match `docs/tech/API_CONTRACTS.md` schemas?

### Performance
- [ ] Are there unnecessary re-renders (React)?
- [ ] Are expensive operations memoized or cached?
- [ ] Are loops efficient? No O(n²) when O(n) is possible?
- [ ] Are large data sets paginated or virtualized?
- [ ] Bundle size impact assessed for new dependencies?
- [ ] No memory leaks (event listeners cleaned up, subscriptions unsubscribed)?

### Security
- [ ] Are user inputs validated and sanitized?
- [ ] Are secrets kept out of source code?
- [ ] Are dependencies from trusted sources?
- [ ] Does the code follow Rule `security-standards.md`?

### Readability
- [ ] Are variable/function names descriptive and consistent?
- [ ] Is complex logic commented or extracted into named functions?
- [ ] Does the code follow project conventions (context7)?
- [ ] Is TypeScript strict mode satisfied? No `any` types?
- [ ] Magic numbers extracted into named constants?

### Architecture
- [ ] Does the change follow patterns in `docs/tech/ARCHITECTURE.md`?
- [ ] Is business logic separated from UI components?
- [ ] Are dependencies injected, not hardcoded?
- [ ] Is the change in the right file/module?

### Test Adequacy (ISTQB-Aligned)
- [ ] Are there tests for the new/changed code?
- [ ] **Equivalence Partitioning:** Are representative values from each class tested?
- [ ] **Boundary Value Analysis:** Are edge boundaries tested (0, 1, max, max+1)?
- [ ] **Error paths:** Are error/exception paths tested, not just happy paths?
- [ ] **Decision coverage:** Are both true/false branches of conditions tested?
- [ ] Test names clearly describe what they verify?

### Documentation
- [ ] Public functions/APIs have JSDoc comments?
- [ ] Complex algorithms have explanatory comments?
- [ ] README updated if interface changed?
- [ ] Inline comments explain *why*, not *what*?

## Severity Classification

| Level | Label | Meaning | Action Required |
|---|---|---|---|
| | **Critical** | Bug, security flaw, data loss risk | Must fix before merge |
| | **Major** | Logic error, missing edge case, test gap | Should fix before merge |
| | **Minor** | Code smell, style issue, optimization | Fix when possible |
| | **Nit** | Preference, suggestion, cosmetic | Optional, no block |

## Feedback Format
```
[SEVERITY] [FILE:LINE] Description

Example:
 [CRITICAL] src/utils/calc.ts:42 — Division by zero possible when `count` is 0.
 [MAJOR] src/utils/calc.ts:50 — No test for boundary value when count = MAX_INT.
 [MINOR] src/components/Card.tsx:15 — Extract magic number `32` to a named constant.
 [NIT] src/App.tsx:8 — Consider alphabetizing imports.
```

## Self-Review Protocol
Before reporting "Done", @dev MUST:
1. Re-read every changed line.
2. Run the review checklist above.
3. Fix all Critical and Major items.
4. Document any intentional deviations from checklist.
