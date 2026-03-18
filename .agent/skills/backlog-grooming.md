---
description: Backlog Grooming — scan, clean, re-prioritize, and flag stale tasks in the project backlog
---

# SKILL: BACKLOG GROOMING

**Trigger:** When @pm needs to review and clean up the task backlog, typically at sprint boundaries or when the backlog feels cluttered.

---

## When to Use
- Before starting a new sprint — groom the backlog to decide what goes in.
- When the backlog has > 15 items — clean up to focus.
- After a pivot or re-prioritization — reassess all pending items.
- When the User asks "what's pending?" or "what should we do next?"

---

## The Grooming Process

### Step 1: Inventory Scan
Scan all pending work across:
- `.hc/stories/` — user stories
- `.hc/epics/` — epic definitions
- `.hc/backlog.md` — icebox / deferred items
- `.hc/roadmap.md` — planned but not started items

For each item, capture:
| ID | Title | Priority | Size | Age (days) | Status |
|----|-------|----------|------|------------|--------|
| S-001 | [Title] | Must/Should/Nice | S/M/L | [days since created] | Ready / Blocked / Stale |

### Step 2: Classify & Flag
Apply these rules to each item:

| Condition | Flag | Action |
|-----------|------|--------|
| No progress in > 14 days | ?? **STALE** | Ask User: still relevant? |
| Blocked by unresolved dependency | ?? **BLOCKED** | Identify blocker, assign resolution |
| Missing acceptance criteria | ?? **INCOMPLETE** | Return to @ba for refinement |
| Duplicates another story | ?? **DUPLICATE** | Merge into the original |
| No longer aligned with roadmap | ? **OBSOLETE** | Recommend closing |

### Step 3: Re-Prioritize
Apply Rule `engineering-mindset.md`:
1. **Must-have** for current phase ? top of backlog.
2. **Should-have** ? next sprint if capacity allows.
3. **Nice-to-have** ? Icebox (`.hc/backlog.md`).
4. **Obsolete** ? mark for closure (needs User confirmation).

### Step 4: Output Groomed Backlog
Present a clean, prioritized backlog:

```markdown
# Groomed Backlog — YYYY-MM-DD
**Groomed by:** @pm

## Ready for Sprint (Must-have)
| # | Story | Size | Owner | Dependencies |
|---|-------|------|-------|-------------|
| 1 | S-XXX: [Title] | M | @dev | None |

## Next Up (Should-have)
| # | Story | Size | Rationale |
|---|-------|------|-----------|
| 1 | S-XXX: [Title] | S | [Why it matters] |

## Flagged for Review
| # | Story | Flag | Question for User |
|---|-------|------|-------------------|
| 1 | S-XXX: [Title] | ?? STALE | Still needed? |

## Icebox (Deferred)
| # | Story | Deferred Because |
|---|-------|-----------------|
| 1 | S-XXX: [Title] | [Reason] |

## Recommended Closures
| # | Story | Reason |
|---|-------|--------|
| 1 | S-XXX: [Title] | Obsolete / Duplicate of S-YYY |
```

---

## Rules of Engagement
1. **Never auto-close** — flag for closure, but the User makes the final call.
2. **Groom regularly** — at every sprint boundary, not just when it gets messy.
3. **Keep it lean** — a backlog with > 20 active items is too big. Push excess to Icebox.
4. **Challenge every item** — "Is this still the most important thing we could work on?"
