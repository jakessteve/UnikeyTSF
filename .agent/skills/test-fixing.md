---
description: Test Fixing - systematic diagnosis and repair of flaky, failing, and environment-dependent tests
---

# SKILL: Test Fixing

**Trigger:** When @qc or @dev encounters failing or flaky tests that need systematic diagnosis.

---

## When to Use
- Tests that fail intermittently (flaky tests).
- Tests that started failing after a code change.
- Tests that fail in CI but pass locally (or vice versa).
- Test suite performance degradation (tests taking too long).
- After `regression-strategy` identifies test failures.

---

## The 4-Step Diagnostic Process

### Step 1: Classify the Failure

| Failure Pattern | Likely Root Cause | Investigation Path |
|---|---|---|
| Always fails | Code bug or outdated test assertion | Compare expected vs. actual behavior |
| Fails intermittently | Race condition, timing, shared state | Check async ops, global state |
| Fails only in CI | Environment difference | Check Node version, OS, timezone, env vars |
| Fails only locally | Missing CI-specific setup | Check test fixtures, env vars |
| Fails after refactor | Test coupled to implementation | Update test to match new interface |
| Multiple tests fail together | Shared setup/teardown issue | Check `beforeEach`/`afterEach` side effects |
| Fails on specific data | Data-dependent logic | Check edge cases, locale, timezone |

### Step 2: Isolate the Problem

1. **Run the failing test alone:**
 ```bash
 npx vitest run --testPathPattern="failing-test"
 ```
2. **If passes alone but fails with others** → Shared state problem
3. **Check execution order dependency:**
 ```bash
 npx vitest run --shuffle # Randomize test order
 ```
4. **Check for global state mutations** (singletons, module-level variables, DOM state)

### Step 3: Identify Root Cause Category

#### Category A: Timing Issues
**Symptoms:** Intermittent failures, works locally, fails in CI
```typescript
// Problem: race condition with async operation
await userEvent.click(button);
expect(screen.getByText('Done')).toBeInTheDocument(); // Might not be updated yet

// Fix: use waitFor
await userEvent.click(button);
await waitFor(() => expect(screen.getByText('Done')).toBeInTheDocument());
```

#### Category B: State Leakage
**Symptoms:** Tests pass alone, fail together
```typescript
// Problem: test modifies global state
let cache = {};
test('adds to cache', () => { cache['key'] = 'value'; }); // Leaks!
test('checks empty cache', () => { expect(cache).toEqual({}); }); // Fails!

// Fix: reset in beforeEach
beforeEach(() => { cache = {}; });
```

#### Category C: Brittle Assertions
**Symptoms:** Tests break on every minor change
```typescript
// Problem: testing implementation details
expect(component.innerHTML).toBe('<div class="card"><span>test</span></div>');

// Fix: test behavior, not structure
expect(screen.getByText('test')).toBeInTheDocument();
expect(screen.getByRole('button')).toBeEnabled();
```

#### Category D: Environment Dependencies
**Symptoms:** Works locally, fails in CI (or vice versa)
```typescript
// Problem: depends on system clock/timezone
expect(formatDate(new Date())).toBe('2025-03-10');

// Fix: mock Date.now() or use fixed dates
vi.useFakeTimers();
vi.setSystemTime(new Date('2025-03-10T00:00:00Z'));
```

### Step 4: Fix and Verify
1. **Fix the root cause**, not the symptom.
2. **Run the test 5 times** consecutively to confirm stability:
 ```bash
 for i in $(seq 1 5); do npx vitest run --testPathPattern="fixed-test"; done
 ```
3. **Run the full suite** to confirm no regressions.
4. **Document the fix** in the commit message.

---

## Anti-Patterns (NEVER Do These)

| Anti-Pattern | Why It's Bad | What to Do Instead |
|---|---|---|
| `retry(3)` | Hides flaky tests | Fix the root cause |
| `.skip` indefinitely | Dead test, false coverage | Fix or delete |
| `sleep(5000)` | Slow, unreliable | Use `waitFor` or `vi.useFakeTimers` |
| Commenting out assertions | Defeats the purpose of testing | Fix the assertion |
| Weakening assertion to pass | Hides real bugs | Strengthen assertion, fix code |

## Rules
- **Fix root causes, not symptoms.** A retried flaky test is still flaky.
- **Never skip a test without a tracking issue.** Skipped tests must have a due date to fix.
- **Verify stability with multiple runs** — one pass doesn't prove the fix works.
- **Log the root cause category** in the commit message (Timing/State/Brittle/Environment).
