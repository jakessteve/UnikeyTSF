---
description: Always On — DRY principle, strict TypeScript type safety, strict test coverage, and no-truncation
---

# RULE: CODE STANDARDS (Combined)

All agents writing code MUST follow these standards at all times.

---

## 1. DRY Principle (Don't Repeat Yourself)

**Before writing any new function/component:** Search codebase first (`grep_search`, `find_by_name`).
- **80%+ match:** Reuse directly.
- **60–80%:** Extend with parameters.
- **< 60%:** Create new, document why existing code couldn't be reused.

**2 occurrences =** note potential duplication. **3+ =** extract immediately to shared utility.

**Constants over magic values.** Extract repeated strings/numbers to named constants.

**DRY Exceptions:** Test setup repetition for clarity is acceptable. Semantically different functions may look alike.

---

## 2. Strict Type Safety

**`any` is FORBIDDEN.** Use `unknown` + type guards, or generics.

**End-to-end flow:** Data Source → Type Definition → Service → Hook → Component Props → UI.
- All function params and return types explicit.
- All React props have typed interfaces.
- Run `npx tsc --noEmit` before reporting done.

**Also forbidden:** `as unknown as X` escape hatches, `@ts-ignore` without justification.

---

## 3. Strict Test Coverage

**Every new logic file MUST have a companion `.test.ts`.**

| Source | Exempt |
|---|---|
| `src/services/**`, `src/utils/**`, `src/hooks/**` | Type defs, index files, configs, pure UI presenters |

**Coverage Thresholds:** 80% minimum (lines + branches). 90% for new modules.

**Test Quality:** Descriptive names, behavior-focused, cover happy + error + edge paths. No `expect(true).toBe(true)`.

**Completion Gate:** @dev-fe/@dev-be MUST NOT report done if tests are missing/failing/below threshold.

** QC Enforcement:** Follow the tiered verification model in `execution-protocol.md` §3. For Medium+ tasks, @qc MUST independently verify test coverage and quality. For Small tasks, @qc spot-checks. For Trivial tasks, @dev-fe/@dev-be self-verification is sufficient.

---

## 4. No Truncation

When producing output (code, documentation, reports): NEVER truncate with "..." or "// rest of the code". Always produce complete, runnable output.
