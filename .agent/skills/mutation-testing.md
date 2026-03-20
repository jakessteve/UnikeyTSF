---
description: Mutation Testing - verify test suite effectiveness through systematic mutation analysis
---

# SKILL: Mutation Testing

**Trigger:** When @qc wants to assess how good the test suite actually is at catching bugs.

---

## When to Use
- After writing tests, to verify their quality (coverage ≠ effectiveness).
- On critical calculation engines (core business logic and critical calculation).
- Before claiming "high test coverage" — mutation score reveals true effectiveness.
- When suspicious that tests pass but don't actually verify correct behavior.
- During `/battle-test` workflow to stress-test test suite quality.

---

## The 4-Step Process

### Step 1: Identify Mutation Targets
Focus mutation testing on code that matters most:

| Target Priority | Examples | Rationale |
|---|---|---|
| Critical | Calculation engines, date logic, financial formulas | Errors have highest user impact |
| Important | Data transformations, validation logic, business rules | Errors cause incorrect behavior |
| Standard | UI state management, navigation logic | Errors are visible but less damaging |
| Skip | Config files, type definitions, pure UI layout | Mutation testing adds no value |

### Step 2: Apply Mutations
Common mutation operators:

| Mutation Type | Example | Tests Should Catch Via |
|---|---|---|
| **Boundary** | `>` → `>=`, `<` → `<=` | Boundary Value Analysis (BVA) tests |
| **Negation** | `if (x)` → `if (!x)` | Branch coverage tests |
| **Arithmetic** | `+` → `-`, `*` → `/` | Calculation assertion tests |
| **Return value** | `return true` → `return false` | Explicit assertion on return |
| **Remove statement** | Delete a line of logic | Behavioral assertion tests |
| **Constant** | `0` → `1`, `""` → `"x"` | Edge case tests |
| **Conditional** | `&&` → `||`, `===` → `!==` | Logic path tests |

### Step 3: Run and Analyze
```bash
# Using Stryker Mutator (JavaScript/TypeScript)
npx stryker run

# Or manual approach for targeted areas:
# 1. Change one operator/value in source
# 2. Run relevant tests
# 3. Verify at least one test fails
# 4. Revert the change
# 5. Repeat for next mutation
```

**Interpreting results:**
| Result | Meaning | Action |
|---|---|---|
| **Killed** | Test caught the mutation | Test suite is effective for this code path |
| **Survived** | Tests passed despite mutation | **Test gap!** Add a test that catches this |
| **Timed out** | Mutation caused infinite loop | Test may be missing termination check |
| **No coverage** | No test runs this code at all | Write a test first (coverage gap) |

### Step 4: Fix Survived Mutants
For each survived mutant:
1. Understand WHY the test didn't catch it.
2. Write a new test case that specifically targets the mutated behavior.
3. Re-run the mutation to confirm it's now killed.

```markdown
## Mutation Report — [Module/File]
**Date:** YYYY-MM-DD | **QC:** @qc

| Metric | Value |
|---|---|
| Total mutants | N |
| Killed | N (X%) |
| Survived | N (Y%) |
| Mutation score | Z% |

### Survived Mutants (Action Required)
| # | Mutation | Line | Why Survived | Fix |
|---|---|---|---|---|
| 1 | `>` → `>=` | L42 | No BVA test for boundary | Add test for exact boundary |
```

---

## Target Scores
| Code Category | Target Mutation Score |
|---|---|
| Critical engine code | ≥ 85% |
| Business logic | ≥ 80% |
| Utility functions | ≥ 75% |
| UI logic | ≥ 60% |

## Rules
- **Focus on business logic**, not UI or config files.
- **Fix survived mutants** by adding targeted test cases, not by weakening the mutation.
- **Mutation testing is expensive** — run on critical code paths, not the entire codebase.
- **Combine with code coverage** — high coverage + high mutation score = confident test suite.
- **Document results** in `.hc/quality/mutation-reports/`.
