---
description: Always On — context window budget tracking, warning thresholds, and progressive load management
---

# RULE: CONTEXT BUDGET

Prevent runaway token consumption by tracking estimated context size and enforcing load limits.

---

## 1. Per-Session Context Budget

| Session Type | Max Base Context | Warning At |
|---|---|---|
| Orchestrator (interactive) | 100 KB | 75 KB |
| CLI Worker (spawned) | 30 KB | 20 KB |

**Base context** = AGENTS file + rules + skills loaded + workflow + instructions.

---

## 2. Lazy-Load Accounting

Every `view_file` on a `.agent/` file adds to the running context total. Track estimated load:

```
Context = .agent/indexes/AGENTS-LITE (1-2 KB)
        + Rules loaded (from rules/MANIFEST.md)
        + Skills loaded (from skills/MANIFEST.md)
        + Source files viewed
```

**Before loading a new skill or rule file**, estimate whether it will push past the warning threshold. If yes:
1. Check if a currently-loaded skill is no longer needed and can be mentally deprioritized.
2. If at the warning threshold, log: `[CONTEXT] Warning: ~{X} KB loaded, approaching {MAX} KB budget.`
3. If at the hard cap, STOP loading new .agent/ files and work with what you have.

---

## 3. Worker Context Injection Checklist

When composing a CLI worker spawn prompt, verify context stays under 30 KB:

- [ ] Role-scoped `.agent/indexes/AGENTS-LITE` file (1-2 KB)
- [ ] Rules from rules/MANIFEST.md for this role (5-20 KB)
- [ ] Skills manifest reference only — not full skill files (0 KB — worker loads on-demand)
- [ ] Task-specific files (remainder of budget)

**If the total prompt exceeds 30 KB**, split the task or reduce context.

---

## 4. Context Bloat Detection

Signs that context is bloated (trigger self-check):
- Loading more than 5 skill files in a single session
- Loading more than 8 rule files as a CLI worker
- Loading any workflow file without a matching `/slash-command` invocation
- Re-reading files already loaded earlier in the session

**Recovery:** Stop, summarize what you've learned so far, and continue without loading more files.

---

## 5. Governance Metrics

At the end of each swarm wave, @pm should estimate:
- Total agents spawned
- Estimated tokens per agent (base context x turns)
- Total wave cost

Compare against the previous wave recorded in `.hc/logs/` and flag if cost increased >20%.
