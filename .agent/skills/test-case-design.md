---
description: Test Case Design - ISTQB techniques for systematic, structured test case creation with domain-specific examples
---

# SKILL: Test Case Design (ISTQB-Aligned)

**Trigger:** When @qc designs test suites or when @dev needs structured test coverage for critical logic.

---

## When to Use
- Designing test suites for new features.
- Reviewing existing test coverage for gaps.
- Creating regression test sets (see `regression-strategy` skill).
- Testing complex business logic (calculation engines, data transformations).
- When `mutation-testing` reveals survived mutants — design tests to kill them.

---

## ISTQB Test Design Techniques

### 1. Equivalence Partitioning (EP)
Divide input into groups that should behave identically. Test ONE value from each partition.

```
Example: Birth year input (valid: 1900-2100)
Partitions: {<1900: invalid}, {1900-2100: valid}, {>2100: invalid}
Test values: 1899, 2000, 2101
```

**When to use:** Every input parameter, every time. This is the baseline technique.

### 2. Boundary Value Analysis (BVA)
Test at the edges of partitions — bugs cluster at boundaries.

```
Example: Birth year (valid: 1900-2100)
Boundary : 1899 | 1900 | 1901 | ... | 2099 | 2100 | 2101
Expected : | | | | | |
```

**When to use:** After EP, for any numeric/date ranges. Critical for calendar calculations.

**Domain example:**
```typescript
// Date range boundary testing
describe('dateRangeBoundary', () => {
  it('handles last day of month correctly (Jan 31)', () => { /* ... */ });
  it('handles first day of next month (Feb 1)', () => { /* ... */ });
  it('handles leap year boundary (Feb 29)', () => { /* ... */ });
});
```

### 3. Decision Table Testing
For logic with multiple interacting conditions.

```
| Type | Detail Known | Calendar Mode | → Action |
|--------|-------------|----------------|----------|
| Type A | Yes | Standard | Full output |
| Type A | No | Standard | Partial output without detail |
| Type B | Yes | Adjusted | Output with adjustment applied |
| Type B | No | Adjusted | Partial output + warning |
```

**When to use:** Business rules with 2+ conditions. Derive test cases from each row.

### 4. State Transition Testing
For systems with distinct states and valid/invalid transitions.

```
Example: Chart Generation State Machine
╔═══════════╗ generate ╔════════════╗ render ╔══════════╗
║ Idle ║ ──────────────→ ║ Computing ║ ────────────→ ║ Rendered ║
╚═══════════╝ ╚════════════╝ ╚══════════╝
 │ error │ reset
 ↓ ↓
 ╔══════════╗ ╔═══════════╗
 ║ Error ║ ║ Idle ║
 ╚══════════╝ ╚═══════════╝

Invalid transitions to test:
- Idle → Rendered (should be blocked — must compute first)
- Error → Rendered (should be blocked — must retry)
```

### 5. Error Guessing
Based on domain experience, test for common mistakes:

| Category | Test Values |
|---|---|
| Null/undefined | `null`, `undefined`, missing properties |
| Empty values | `""`, `[]`, `{}`, `0` |
| Negative numbers | `-1`, `-999` |
| Very large | `Number.MAX_SAFE_INTEGER`, 10000-char strings |
| Special characters | Unicode, emoji, accented characters, CJK |
| **Date-specific** | Leap years (Feb 29), month boundaries, Dec 31 → Jan 1 |
| **Calendar-specific** | Period boundaries, timezone midnight, DST transitions |
| **Concurrent** | Same input computed twice simultaneously |

---

## Test Case Template

```markdown
## TC-[ID]: [Descriptive Title]
**Priority:** P1/P2/P3
**Technique:** EP / BVA / Decision Table / State Transition / Error Guess
**Preconditions:** [Required state before test]
**Input:**
| Parameter | Value |
|---|---|
| [param1] | [value] |
| [param2] | [value] |
**Steps:**
1. [Action to perform]
2. [Action to perform]
**Expected Result:** [What should happen]
**Actual Result:** [Fill after execution]
**Status:** Pass / Fail / Blocked
```

---

## Coverage Strategy by Risk Level

| Risk Level | Techniques to Apply | Example |
|---|---|---|
| Critical (data integrity, security) | EP + BVA + Decision Table + State + Error | Core engine calculations, auth |
| High (core features) | EP + BVA + Decision Table | Data processing, transformations |
| Medium (secondary features) | EP + Key boundaries | Settings, preferences |
| Low (cosmetic) | Happy path only | UI text, formatting |

## Rules
- **Every test needs a technique.** "I just tested some values" is not systematic.
- **BVA is non-negotiable for ranges.** Calendar, age, and date inputs MUST have boundary tests.
- **Decision tables for business rules.** Any logic with 2+ conditions gets a table.
- **Error guessing supplements, doesn't replace** — always start with EP/BVA, then add error guesses.
- **Cross-reference with `mutation-testing`** — if mutants survive, your test design has gaps.
