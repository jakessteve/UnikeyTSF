# Implementation Task

> Fill ALL sections before delegating. Workers start with ZERO context.

```markdown
# Task: [SHORT TASK NAME]

## Goal
[1-2 sentences — what exactly should be achieved]

## Architecture Context
- **Project type**: Vite 7 + React 19 + TypeScript SPA (strict mode)
- **Styling**: TailwindCSS v4 with custom `@theme` tokens in `src/index.css`. Dark mode mandatory.
- **State**: Zustand for global state
- **Key deps**: `iztro` (Tử Vi), `circular-natal-horoscope-js` (Western Astrology), `@dqcai/vn-lunar`
- **Conventions**: Vietnamese UI text, English code/comments. No `any` types. DRY principle.
- **File structure**: `src/components/`, `src/services/`, `src/utils/`, `src/pages/`

## File Map

### Files to MODIFY
| File | What to change |
|------|---------------|
| `src/path/to/file.ts` | [Describe specifically] |

### Files to CREATE
| File | Purpose |
|------|---------|
| `src/path/to/new-file.ts` | [Purpose] |

### Files to READ (context only — DO NOT modify)
| File | Why read |
|------|---------|
| `src/path/to/reference.ts` | Follow same pattern |

### Off-limits (MUST NOT touch)
- `src/index.css` — theme tokens managed separately
- `package.json` — dependency changes require orchestrator approval
- Any file not listed above

## Step-by-Step Implementation
### Step 1: [Name]
[Details]

### Step 2: [Name]
[Details]

## Code Conventions
- **Naming**: camelCase variables, PascalCase components/classes, UPPER_SNAKE constants
- **Imports**: Use relative paths from src/
- **Error handling**: Use typed errors, no bare `catch(e)`
- **Components**: Functional components with hooks, typed props interfaces
- **Dark mode**: Every visual change MUST have light and dark variants via semantic tokens
- **i18n**: All user-facing strings in Vietnamese

## Acceptance Criteria
1. [ ] [Criteria 1]
2. [ ] [Criteria 2]
3. [ ] Code compiles: `npm run build`
4. [ ] No lint errors: `npm run lint`

## Verification Commands
```bash
npm run lint
npm run build
```

## Constraints
- DO NOT modify files outside the File Map scope
- DO NOT install new dependencies
- DO NOT change existing API contracts or component props
- Follow existing code patterns — do NOT introduce new patterns
- If unclear, make the SAFEST choice and note it in the report

## Report Format
### Changes Made
| File | Action | Description |
|------|--------|-------------|

### Decisions Made
- [Any ambiguous choices]

### Potential Issues
- [Anything needing orchestrator review]
```
