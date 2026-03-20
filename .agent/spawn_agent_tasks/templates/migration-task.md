# Migration Task
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


> For dependency upgrades, schema changes, config migrations, and API version bumps.

```markdown
# Task: Migrate [SHORT DESCRIPTION]

## Goal
[1-2 sentences — what migration to perform and why]

Example: "Upgrade [library] from vX to vY — breaking changes in API
API require updating all service consumers."

## Architecture Context
- **Project type**: Read `package.json`, `tsconfig.json`, and build config to detect stack
- **Styling**: Read project config to determine CSS framework and conventions
- **State**: Zustand
- **Conventions**: UI text follows project locale. English code. No `any` types. DRY principle.
- **Package manager**: [project package manager]

## Migration Details

### What's changing
| Aspect | Before | After |
|--------|--------|-------|
| [Dependency/Config/Schema] | [Current version/state] | [Target version/state] |

### Breaking changes
| Change | Impact | Required action |
|--------|--------|----------------|
| [API rename / removed field / new required param] | [Which files affected] | [How to update] |

### Migration guide reference
[Link to upgrade guide, changelog, or migration docs if available]

## File Map

### Files to MODIFY
| File | What changes |
|------|-------------|
| `src/path/to/file.ts` | [Specific migration change] |

### Files to READ (context only — DO NOT modify)
| File | Why read |
|------|---------|
| `src/path/to/consumer.ts` | Understand current usage |

### Off-limits (MUST NOT touch)
- Any file not listed above
- `src/index.css` — theme tokens managed separately

## Migration Steps
### Step 1: Preparation
[Read current usage, verify scope of changes needed]

### Step 2: Apply changes
[Perform the actual migration changes — update imports, APIs, configs]

### Step 3: Handle deprecations
[Replace deprecated APIs with new equivalents]

### Step 4: Verify
[Build, lint, test — confirm migration is complete]

## Rollback Plan
If migration fails or introduces regressions:
1. [Step to revert — e.g., `git checkout -- [affected-directory]/`]
2. [How to restore previous dependency version]
3. [Any data/config that needs reverting]

## Acceptance Criteria
1. [ ] Migration complete — all references updated to new API/version
2. [ ] No deprecated API usage remains
3. [ ] No type errors in migrated files
4. [ ] `npm run lint` passes
5. [ ] `npm run build` passes

## Verification Commands
```bash
npm run lint
npm run build
```

## Constraints
- Follow the official migration guide step-by-step
- DO NOT skip deprecated warnings — fix all of them
- DO NOT change unrelated code during migration
- If a breaking change affects 10+ files, list ALL affected files in the report
- If rollback becomes necessary, note the exact point of failure

## Report Format
### Migration Summary
| Aspect | Status |
|--------|--------|
| Files modified | [count] |
| Breaking changes resolved | [count] / [total] |
| Deprecations fixed | [count] |

### Changes Made
| File | Action | Description |
|------|--------|-------------|

### Breaking Changes Resolved
| Change | How resolved |
|--------|-------------|

### Rollback Notes
- [Any state that would need manual reverting]

### Potential Issues
- [Anything needing orchestrator review]
```
