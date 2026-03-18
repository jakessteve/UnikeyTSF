---
description: Scaffold a new feature — types/models → business logic → UI components with review artifacts
---

# Workflow: /scaffold-feature

Structured 3-step pipeline for implementing a new feature module end-to-end.

---

## Prerequisites
- User story exists in `.hc/stories/` with acceptance criteria
- Architecture defined by @sa in `docs/tech/ARCHITECTURE.md`
- API contracts defined in `docs/tech/API_CONTRACTS.md` (if applicable)
- Test scaffolds written by @qc in `tests/phase-[name]/`

## Steps

### Step 1 — Define Types & Models
1. Read the user story and API contracts.
2. Define TypeScript interfaces and types for:
 - Data models (input/output shapes)
 - Component props
 - Service function signatures
 - Shared constants and enums
3. Place types following `scalable-folder-structure.md`:
 - Shared types → `src/types/[feature]Types.ts`
 - Co-located types → alongside their service file
// turbo
4. Run `npx tsc --noEmit` to verify types compile.

### Step 2 — Write Business Logic & Services
1. **DRY check (Rule `code-standards.md`):** Search codebase for existing similar logic.
2. Implement pure business logic functions in `src/services/[feature]/`:
 - All functions must be pure (no side effects, no UI imports).
 - All parameters and return types explicitly typed (Rule `code-standards.md`).
 - Complex algorithms extracted into small, testable functions.
// turbo
3. Run @qc's test scaffolds: `npm test -- --testPathPattern=[feature]`.
4. Iterate until all tests pass (TDD Green).
5. Refactor for clarity and performance (TDD Refactor).

### Step 3 — Build UI Components
1. Read `WIREFRAMES.md` for visual specs.
2. **DRY check:** Search existing components for reusable pieces.
3. Build components in `src/components/[Feature]/`:
 - Follow `design-system-uiux` skill for design consistency.
 - Use typed props interfaces from Step 1.
 - Import services from Step 2 — never inline business logic.
 - Add interaction states: loading, error, empty, success.
 - Ensure dark mode parity and accessibility (Rule `a11y-standards.md`).
4. Run the full test suite: `npm test`.
5. Visually verify in browser using `browser-visual-testing` skill.

### Step 4 — Generate Review Artifact
Create an Artifact summarizing the implementation:
```markdown
# Feature Scaffold: [Feature Name]

## Files Created/Modified
| File | Purpose | Lines |
|------|---------|-------|
| `src/types/...` | Type definitions | X |
| `src/services/...` | Business logic | X |
| `src/components/...` | UI components | X |

## Type Safety
- `npx tsc --noEmit`: PASS / FAIL

## Test Results
- Tests passing: X/Y
- Coverage: XX%

## DRY Checks
- Reused: [list of reused functions/components]
- New: [list of genuinely new code with justification]

## Screenshots
[Before/after if applicable]
```

Present to @pm for review.

### Step 5 — QC Verification (MANDATORY)
1. Switch to **@qc persona**.
2. Run `npm test` — verify ALL tests pass (including pre-existing tests).
3. Run `npx tsc --noEmit` — verify zero type errors.
4. Run `npm run lint` — verify no new lint errors.
5. If UI components were created/modified: use `browser-visual-testing` to verify rendering.
6. Produce a mini verification report (pass/fail counts).
7. If ANY check fails: switch back to @dev persona to fix, then re-run QC.
8. Only after ALL checks pass: report Done to @pm.

> **CRITICAL:** This step is NON-OPTIONAL. @dev self-testing (Steps 2–3) is necessary but NOT sufficient. @qc must independently verify before the feature is considered complete.

---

## Chaining
- After Step 3, MUST trigger `/qa-responsive-check` for UI features.
- After Step 5, optionally trigger `/run-e2e-qa` for user-facing features.
- If tests fail in Step 2/3/5, loop back and fix before proceeding.
