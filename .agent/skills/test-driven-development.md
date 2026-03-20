---
description: Test-Driven Development - Red-Green-Refactor cycle enforcement with coverage targets and domain-specific guidance
---

# SKILL: Test-Driven Development (TDD)

**Trigger:** When @dev implements new features, business logic, or utility functions. Especially for code in `src/utils/`, `src/services/`, and `src/engines/`.

---

## When to Use
- Implementing new business logic or calculation functions (ALWAYS).
- Building utility functions with defined input/output.
- Fixing bugs (write a failing test FIRST, then fix).
- Refactoring existing code (ensure tests pass before AND after).
- Implementing engine / business logic (always use TDD for core logic).

## When NOT to Use
- Exploratory prototyping (spike first, then TDD on the solution).
- Pure UI layout (use visual testing instead).
- Configuration-only changes.

---

## The TDD Cycle

### Phase 1: RED — Write a Failing Test
1. **Define the expected behavior** BEFORE writing any implementation.
2. Write a test that describes what the code _should_ do.
3. Run the test — it MUST fail. If it passes, the test is wrong or the feature already exists.

```typescript
// Example: Testing a date conversion utility
describe('convertToBusinessDate', () => {
 it('should return correct business date for a known input', () => {
 const result = convertToBusinessDate(new Date(2025, 0, 29));
 expect(result).toEqual({ day: 29, period: 'Q1', year: 2025, adjusted: false });
 });

 it('should handle period boundary correctly', () => {
 const result = convertToBusinessDate(new Date(2025, 3, 1));
 expect(result.period).toBe('Q2');
 });

 it('should throw for dates before supported range', () => {
 expect(() => convertToBusinessDate(new Date(1800, 0, 1))).toThrow('Date out of range');
 });
});
```

### Phase 2: GREEN — Make It Pass
1. Write the **minimum code** to make the test pass.
2. **Don't optimize, don't refactor, don't gold-plate.**
3. The goal is a passing test, nothing more.
4. It's okay if the code is ugly — you'll clean it up next.

### Phase 3: REFACTOR — Clean It Up
1. **Improve code quality** WITHOUT changing behavior.
2. Extract constants, rename variables, simplify conditionals.
3. Run tests again — they MUST still pass.
4. If any test breaks during refactoring, **undo and try again**.
5. Apply `refactoring-patterns` skill if needed.

---

## Coverage Targets by Code Layer

| Layer | Test Type | Coverage Target | Priority |
|---|---|---|---|
| Engine calculations | Unit test | ≥ 90% | Critical |
| Utility functions | Unit test | ≥ 90% | Critical |
| Data transformations | Unit test | ≥ 85% | Critical |
| Service/business logic | Unit test | ≥ 80% | High |
| React hooks (custom) | Unit test | ≥ 80% | High |
| UI components | Behavior test | Key interactions | High |
| API endpoints / handlers | Integration test | All paths | High |
| CSS / layout | Visual test | Use `browser-visual-testing` | Standard |

## What NOT to Test
- CSS styling (use `browser-visual-testing` skill)
- Third-party library internals
- Simple type definitions or interfaces
- Configuration files
- Constants / enum definitions

---

## Bug Fix Protocol (Test-First)
When fixing a bug:
1. **Write a test that reproduces the bug** — it MUST fail.
2. Fix the code until the test passes.
3. This guarantees the bug can **never recur silently**.

```typescript
// Step 1: Reproduce the bug in a test
it('should not produce duplicate entries in results (bug #42)', () => {
 const chart = processInput(buggyInputData);
 const items = result.items;
 const filteredItems = items.filter(i => i.name === 'TargetItem');
 expect(filteredItems).toHaveLength(1); // Was returning 2 — bug!
});
```

## Test Quality Checklist
Before marking TDD cycle complete:
- [ ] Test names describe **expected behavior**, not implementation
- [ ] Each test tests **ONE thing** (single assertion focus)
- [ ] Tests are **independent** (no shared mutable state, no order dependency)
- [ ] Tests run **fast** (< 100ms per unit test)
- [ ] Tests don't depend on **external services** (mock them)
- [ ] **Edge cases covered:** null, undefined, empty, boundary values, error cases
- [ ] Tests **fail for the right reason** (not infrastructure issues)

## Rules
- **Red-Green-Refactor is sacred.** Never skip a step.
- **Test behavior, not implementation.** Tests should survive refactoring.
- **One test at a time.** Write one failing test, make it pass, repeat.
- **Fast feedback.** If tests take >5 seconds, optimize the test suite.
- **Core business logic MUST use TDD.** No exceptions for critical calculation engines or data processing modules.
