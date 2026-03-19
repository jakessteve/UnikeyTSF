# Bug Fix Task
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


> For targeted bug fixes with known or suspected location.

```markdown
# Task: Fix [BUG SUMMARY]

## Bug Description
[What's broken, expected vs actual behavior]

## Architecture Context
- **Project type**: Read `package.json`, `tsconfig.json`, and build config to detect stack
- **Styling**: Read project config to determine CSS framework and conventions
- **State**: Zustand
- **Conventions**: UI text follows project locale. English code. No `any` types.

## Suspected Location
| File | Why suspected |
|------|-------------|
| `src/path/to/file.ts` | [Evidence/reasoning] |

## Scope
### Files to MODIFY
| File | Description |
|------|-------------|
| `src/path/to/file.ts` | [Fix description] |

### Files to READ (context only)
| File | Why |
|------|-----|
| `src/path/to/related.ts` | [Understand dependencies] |

### Off-limits
- All files not listed above
- `package.json`
- `src/index.css`

## Fix Approach
1. [Investigate: read suspected files]
2. [Identify: root cause]
3. [Fix: specific change]
4. [Verify: run checks]

## Acceptance Criteria
1. [ ] Bug is fixed — [expected behavior restored]
2. [ ] No regressions — existing functionality unchanged
3. [ ] `npm run lint` passes
4. [ ] `npm run build` passes

## Verification Commands
```bash
npm run lint
npm run build
```

## Constraints
- Fix the ROOT CAUSE, not symptoms
- Minimal changes — don't refactor unrelated code
- If the fix requires changes outside scope → note it, don't do it

## Report Format
### Root Cause
[What caused the bug]

### Fix Applied
| File | Change |
|------|--------|

### Verification Results
- Lint: /
- Build: /

### Side Effects
- [Any potential regressions to watch]
```
