---
description: Always On — enforce organized, scalable folder structure from project inception
---

# RULE: SCALABLE FOLDER STRUCTURE

**Mode:** Always On
**Scope:** All agents creating files or directories

---

## Core Mandate

Every file created MUST be placed in its correct location according to the project's canonical folder structure defined in `docs/tech/ARCHITECTURE.md`. Dumping files in the root or in flat directories is FORBIDDEN.

---

## Binding Constraints

### 1. Read Before Placing
Before creating any new file, you MUST:
- Check `docs/tech/ARCHITECTURE.md` for the defined folder structure.
- If no structure is defined, propose one to @sa before creating files.
- Place the file in the correct directory according to its type and domain.

### 2. Feature-Based Organization
Group files by **feature/domain**, not by file type:

```
 CORRECT (Feature-based):
src/
├── services/
│ ├── tuvi/ # Tử Vi feature
│ │ ├── tuviEngine.ts
│ │ ├── tuviTypes.ts
│ │ └── tuviPatterns.ts
│ └── calendar/ # Calendar feature
│ ├── lunarCalc.ts
│ └── solarTerms.ts
├── components/
│ ├── TuVi/
│ │ ├── TuViGrid.tsx
│ │ └── TuViModule.tsx
│ └── Calendar/
│ ├── MonthCalendar.tsx
│ └── DayDetail.tsx

 WRONG (Type-based flat dump):
src/
├── engines/
│ ├── tuviEngine.ts
│ ├── lunarCalc.ts
│ └── gieoQueEngine.ts
├── types/
│ ├── tuviTypes.ts
│ ├── calendarTypes.ts
│ └── gieoQueTypes.ts
```

### 3. Separation of Concerns
Each directory level has a clear responsibility:

| Directory | Contains | Responsibility |
|-----------|----------|---------------|
| `src/components/` | React/UI components | Rendering and user interaction |
| `src/services/` | Business logic engines | Pure computation, no UI |
| `src/hooks/` | Custom React hooks | Shared stateful logic |
| `src/types/` | Shared type definitions | Cross-cutting TypeScript types |
| `src/utils/` | Pure utility functions | Generic helpers (date math, formatters) |
| `src/data/` | Static data files | JSON/CSV datasets |
| `docs/tech/` | Technical documents | Architecture, API contracts, ADRs, test plans, deployment |
| `docs/biz/` | Business documents | PRD, product brief, market research, GTM plans |
| `docs/log/` | Execution logs | Changelog, sprint logs, incident reports |
| `tests/` | Test files | Mirrors `src/` structure |

### 4. File Naming Convention
- **Components:** PascalCase (`TuViGrid.tsx`, `MonthCalendar.tsx`)
- **Services/Utils:** camelCase (`tuviEngine.ts`, `dateUtils.ts`)
- **Types:** camelCase with `Types` suffix (`tuviTypes.ts`)
- **Tests:** Same name + `.test` suffix (`tuviEngine.test.ts`)
- **Constants/Data:** camelCase (`actionWeight.json`)

### 5. Co-location Principle
Keep related files close together:
- Component + its styles + its tests → same feature directory
- Service + its types → same service directory
- Never force a developer to jump across 5 directories to understand one feature

### 6. Growth Rules
When a directory grows beyond **10 files**:
- Split into sub-directories by sub-feature or concern
- Create an `index.ts` barrel file for clean imports
- Document the new structure in `docs/tech/ARCHITECTURE.md`

### 7. Cleanup & Anti-Memory-Leak (merged from strict-folder-cleanup)
Stale files, orphaned artifacts, and temp data are memory leaks. Keep the workspace pristine:
- **`.temp/` purge:** Delete ALL files in `.temp/` during `/close-phase`. No exceptions.
- **Orphaned files:** Files in root or `src/` that don't belong to any feature/phase must be moved or deleted.
- **Outdated SOT drafts:** Old versions of `docs/biz/PRD.md`, `docs/tech/ARCHITECTURE.md` in `.temp/` must be purged.
- **Context artifact limits:** Never store more than 3 draft files in `.temp/` simultaneously.
- **Log archival:** Log files older than the current phase should be archived to `.hc/logs/` or deleted.
- **Build artifacts:** Compiled output and cache files must be in `.gitignore` and periodically cleaned.

**During `/close-phase`, @pm verifies:**
- [ ] `.temp/` is empty
- [ ] No orphaned files in project root
- [ ] All source files are in correct feature directories
- [ ] No duplicate or conflicting SOT documents exist

---

## Enforcement
Any file placed in an incorrect directory should be flagged as ` FOLDER STRUCTURE VIOLATION`. @sa owns the canonical structure and resolves disputes.
