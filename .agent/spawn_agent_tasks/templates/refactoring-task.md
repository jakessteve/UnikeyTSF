# Refactoring Task
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


> For safe, scoped refactoring: extract, move, rename, restructure. No behavior changes.

```markdown
# Task: Refactor [SHORT DESCRIPTION]

## Goal
[1-2 sentences — what structural improvement should be achieved, and WHY]

Example: "Extract the scoring logic from `lunarUtils.ts` into a dedicated
`activityScoring.ts` module to improve separation of concerns."

## Architecture Context
- **Project type**: Read `package.json`, `tsconfig.json`, and build config to detect stack
- **Styling**: Read project config to determine CSS framework and conventions
- **State**: Zustand
- **Conventions**: UI text follows project locale. English code. No `any` types. DRY principle.
- **File structure**: `src/components/`, `src/services/`, `src/utils/`, `src/pages/`

## Before / After Structure

### Before
```
src/
├── utils/
│ └── lunarUtils.ts ← contains scoring logic + calendar logic (mixed)
```

### After
```
src/
├── utils/
│ ├── lunarUtils.ts ← calendar logic only
│ └── activityScoring.ts ← extracted scoring logic
```

## File Map

### Files to MODIFY
| File | What changes |
|------|-------------|
| `src/path/to/source.ts` | [Extract X, remove Y, rename Z] |

### Files to CREATE
| File | Purpose |
|------|---------|
| `src/path/to/new-module.ts` | [Extracted logic from source] |

### Files to READ (context only — DO NOT modify)
| File | Why read |
|------|---------|
| `src/path/to/consumer.ts` | Understand all import sites |

### Off-limits (MUST NOT touch)
- `src/index.css` — theme tokens managed separately
- `package.json` — no dependency changes
- Any file not listed above

## Refactoring Steps
### Step 1: Extract
[Move specific functions/types from source to new file]

### Step 2: Update Imports
[Update all consumers to import from new location]

### Step 3: Clean Source
[Remove extracted code from original file, clean up unused imports]

### Step 4: Verify
[Run lint + build to confirm no breakage]

## Non-Breaking Checklist
- [ ] All public exports from original module still exist (or re-exported)
- [ ] No API signature changes — same inputs, same outputs
- [ ] No behavioral changes — logic is identical, just relocated
- [ ] All consumers updated to use new import paths

## Acceptance Criteria
1. [ ] [Describe the structural outcome]
2. [ ] All existing functionality works identically
3. [ ] No circular dependencies introduced
4. [ ] `npm run lint` passes
5. [ ] `npm run build` passes

## Verification Commands
```bash
npm run lint
npm run build
```

## Constraints
- **ZERO behavior changes** — this is a structure-only refactor
- DO NOT change function signatures, prop types, or return types
- DO NOT rename exported symbols unless explicitly instructed
- DO NOT introduce new patterns — follow existing conventions
- If a function is imported in 5+ places, verify ALL import sites
- If unclear about scope, keep the original and ADD a new module (don't delete)

## Report Format
### Changes Made
| File | Action | Description |
|------|--------|-------------|

### Import Updates
| Consumer File | Old Import | New Import |
|---------------|-----------|------------|

### Decisions Made
- [Any ambiguous choices]

### Potential Issues
- [Circular deps? Missing consumers? Re-export needed?]
```
