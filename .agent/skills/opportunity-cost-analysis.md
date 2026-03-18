---
description: Opportunity Cost Analysis — lightweight trade-off calculation for @pm before accepting new work
---

# SKILL: Opportunity Cost Analysis

**Trigger:** When @pm receives a new feature request or scope addition while existing work is in progress. Called as part of the Critical Thinking Gate (§5) or standalone when evaluating backlog changes.

---

## When to Use
- User requests a new feature while an existing sprint/phase is in progress.
- @pm's Cynefin classification is **Complicated** or **Complex** (not Clear/Chaotic).
- During backlog grooming when re-prioritizing stories.
- When multiple competing ideas exist and only one can proceed.

## When to SKIP
- No active sprint work (green-field project start).
- Bug fixes and hotfixes (these take priority by default).
- The task is explicitly marked as P0 by the User.

---

## Process

### Step 1: Estimate New Work
Classify the proposed task's effort:

| Size | Definition | Typical Tool Calls |
|---|---|---|
| **S** (Small) | ≤3 files, single concern | ~10-15 |
| **M** (Medium) | 4-10 files, single domain | ~20-35 |
| **L** (Large) | 10-20 files, multi-domain | ~40-60 |
| **XL** (Extra Large) | >20 files or new module | ~60-80 |

### Step 2: Identify Current Priorities
List the top 3 active priorities from the roadmap, backlog, or current sprint:

| # | Priority | Status | Effort Remaining |
|---|---|---|---|
| 1 | [Name] | [In Progress / Queued / Blocked] | [S/M/L/XL] |
| 2 | [Name] | [Status] | [Remaining] |
| 3 | [Name] | [Status] | [Remaining] |

### Step 3: Assess Impact
For each current priority, determine if the new task would delay it:

| Current Priority | Delay if New Task Added? | Severity |
|---|---|---|
| [Priority 1] | [None / +1 day / +1 sprint / Blocked] | [ / / ] |
| [Priority 2] | ... | ... |
| [Priority 3] | ... | ... |

### Step 4: Present Recommendation

```markdown
### Opportunity Cost — [New Task Name]

**New task effort:** [S/M/L/XL]

| Current Priority | Impact | Delay |
|---|---|---|
| [P1] | [impact] | [delay] |
| [P2] | [impact] | [delay] |
| [P3] | [impact] | [delay] |

**Recommendation:** [One of the following]
- **Proceed** — No competing priority is impacted.
- **Proceed with awareness** — [Priority X] will be delayed by [Y]. User should confirm.
- **Replace** — Drop [Priority X] and take on new task instead.
- **Defer to Icebox** — Current priorities are higher impact. Revisit in Phase [X].
```

### Step 5: Route Decision
- If **Proceed** → continue to delegation.
- If **Proceed with awareness** → flag to User with the trade-off before delegating.
- If **Replace** → get User confirmation, then update roadmap/backlog.
- If **Defer** → add to `.hc/icebox.md` with rationale and re-evaluation date.

---

## Anti-Patterns
| Don't | Do |
|---|---|
| Silently accept all new work | Calculate what gets displaced |
| Block every new request with analysis paralysis | Quick 5-step assessment, not a research project |
| Assume User knows the trade-off | Explicitly state what gets delayed |
| Treat all work as equal priority | Use the impact/effort classification |

---

## Rules
- **Speed over precision.** This is a 2-minute assessment, not a project plan.
- **Default to action.** If no priority is impacted, just proceed.
- **Be honest.** If the new task is lower priority, say so directly.
- **Never hide trade-offs.** The User deserves to know what they're giving up.
