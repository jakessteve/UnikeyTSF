---
description: Review an entire codebase against framework best practices and generate a prioritized improvement plan
---

# Codebase Review Workflow

> **Token Efficiency**: Use `wc -l` / `awk` counts. Summarize internally. Never repeat skill rules verbatim.

## Step 1 — Project Context

```bash
ls -F
cat package.json 2>/dev/null
find . -maxdepth 2 -not -path '*/.*' -not -path '*/node_modules/*' -not -path '*/build/*' -not -path '*/dist/*'
```

## Step 2 — Framework Detection & Source Dirs

| Manifest | Framework | `$SRC` | `$TEST` |
|---|---|---|---|
| `package.json` + react in deps | React/Vite | `src/` | `test/` or `src/` |
| `package.json` + next in deps | Next.js | `src/` | `__tests__/` |
| `go.mod` | Golang | `.` | `.` |

> [!IMPORTANT]
> Record `$SRC` and `$TEST` now. Every scan MUST use correct directories.

## Step 3 — Read Relevant Skills

Read discovered `SKILL.md` files. Note P0/P1 patterns only.

## Step 4 — Breadth Scan (Run ALL)

**File counts:**
```bash
find $SRC -type f -name "*.ts" -o -name "*.tsx" | grep -v node_modules | grep -v ".d.ts" | wc -l
find $TEST -type f -name "*.test.ts" -o -name "*.test.tsx" -o -name "*.spec.ts" | wc -l
```

**TODO/FIXME count (code comments only):**
```bash
grep -rn "// TODO\|// FIXME\|//TODO\|//FIXME" $SRC --include="*.ts" --include="*.tsx" | wc -l
```

**Fat files (>600 LOC, sorted descending):**
```bash
find $SRC -type f \( -name "*.ts" -o -name "*.tsx" \) ! -name "*.d.ts" ! -name "*.test.*" | xargs wc -l 2>/dev/null | awk '$1 > 600 && $2 != "total" {print $1, $2}' | sort -rn
```

## Step 5 — Security Stress Test

```bash
# Hardcoded secrets
grep -rnE "(password|apiKey|api_key|secret|private_key)\s*=\s*['\"][^'\"]{6,}" $SRC --include="*.ts" --include="*.tsx" | grep -v "//\|label\|hint\|placeholder"

# Sensitive data in logs
grep -rnE "console\.(log|warn|error)\(" $SRC --include="*.ts" --include="*.tsx" | grep -iE "password|token|secret" | wc -l

# Dependency audit
npm audit --audit-level=high 2>/dev/null | head -40
```

| Signal | Threshold | Severity |
|---|---|---|
| Hardcoded secrets | Any match | Critical |
| Secrets in logs | > 0 | Critical |
| High-severity CVEs | > 0 | High |

## Step 6 — Deep Quality Check (3 Files, 3 Layers)

Pick the **largest non-generated file** from fat-file scan for each:
- **Service layer:** SRP, error handling, no UI code
- **Component layer:** No business logic in render, decomposition
- **Utility layer:** Pure functions, type safety, edge cases

## Step 7 — Scored Report

**Scoring:** Critical: -15 | High: -8 | Medium: -3 | Low: -1

### Category Scores
| Category | Score | Key Driver |
|---|---|---|
| Security | /100 | |
| Architecture | /100 | |
| Testing | /100 | |
| Code Quality | /100 | |

### Improvement Plan
**Phase 1 — Quick Wins** | **Phase 2 — Refactoring** | **Phase 3 — Polish**

Each row MUST include: ID, Action, File(s), Why / Benefits.

## Step 8 — Interactive Follow-Up

Ask: Fix [ID] now? Generate task.md for Phase 1? Deep-dive refactoring plan for [component]?
