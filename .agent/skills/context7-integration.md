---
description: Context7 Integration - fetch library docs and conventions before writing code
---

# SKILL: Context7 Integration

**Trigger:** Autonomously triggered by @dev BEFORE starting to write code, especially when using unfamiliar APIs or updating dependencies.

---

## When to Use
- Before implementing code that uses a library/framework API you haven't used recently.
- When a dependency is being upgraded and you need to check for breaking changes.
- When debugging issues that may stem from incorrect API usage.
- When reviewing code that uses library patterns you need to verify.

---

## The 3-Step Process

### Step 1: Resolve the Library ID
```
Call: mcp_context7_resolve-library-id
Query: [specific question about what you need]
Library: [library name]
```

**Selection criteria** when multiple results come back:
| Criteria | Priority |
|---|---|
| Name match (exact) | Highest |
| Source reputation (High/Medium) | Highest |
| Code snippet count (more = better) | Medium |
| Benchmark score (closer to 100) | Medium |
| Description relevance | Baseline |

### Step 2: Query the Documentation
```
Call: mcp_context7_query-docs
Library ID: [resolved ID from Step 1]
Query: [specific, detailed question]
```

**Good queries:** "How to set up authentication middleware in Express.js 5"
**Bad queries:** "express" or "auth"

### Step 3: Apply Conventions
- Use the returned patterns **exactly** as documented (naming, structure, API signatures).
- If the documentation contradicts your current implementation, the documentation wins.
- If the documentation is ambiguous, flag it with ` Context7 ambiguity` and use your best judgment.

---

## Project Library IDs (Pre-Resolved)

| Library | Context7 ID | Version Notes |
|---|---|---|
| React | `/facebook/react` | v19 — check for new APIs |
| Vite | `/vitejs/vite` | v7 — check plugin API changes |
| TailwindCSS v4 | Resolve via `tailwindcss` | v4 has breaking changes from v3 |
| iztro (紫微斗数) | Resolve via `iztro` | Check latest star data |
| react-router-dom | Resolve via `react-router` | v7 — new data router patterns |
| Zod | Resolve via `zod` | — |
| Tauri | Resolve via `tauri` | v2 — new plugin system |

---

## Decision: When to Skip Context7

| Scenario | Skip? | Rationale |
|---|---|---|
| Standard HTML/CSS/JS | Yes | No library-specific patterns |
| React hooks (basic) | Yes | Well-known patterns |
| New library never used before | No | ALWAYS query first |
| Library version upgrade | No | Breaking changes possible |
| Debugging "works locally, fails in CI" | Maybe | Check if CI uses different version |

## Rules
- **Max 3 calls per task** to avoid context overflow.
- Always query for the **SPECIFIC API** you need, not generic "how to use React."
- Prefer official documentation sources (High reputation).
- If Context7 returns no results, fall back to `search_web` (skill `research-analysis.md`).
- Cache resolved library IDs in memory — don't re-resolve the same library twice per session.
