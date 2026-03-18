---
description: Implementation Debate — post-build roundtable to evaluate whether implementation matches intent and surface tech debt
---

# SKILL: IMPLEMENTATION DEBATE

**Trigger:** After @qc verification passes and before shipping. Used to debate whether the *approach* (not just the code) was correct.

---

## When to Use
- After `/battle-test` passes — code works, but was the approach right?
- During `/implementation-review` workflow.
- During `/idea-forge` Phase 5 (Debate Implementation).
- When tech debt concerns arise mid-implementation.

---

## Roundtable Format

### Participants
Minimum 3 agents: **@sa** (architecture), **@dev** (implementation), **@qc** (quality). Optional: **@designer** (if UI-involved), **@devops** (if infra-involved).

### Agenda Template

```markdown
# Implementation Review — [Feature/Task Name]
**Date:** YYYY-MM-DD
**Triggered by:** [/idea-forge Phase 5 | /implementation-review | manual]

## 1. Intent vs. Reality Check
**Original brainstorm decision:** [Link to `.hc/brainstorms/` or `.hc/decisions/`]
**What was actually built:** [Brief description of implementation]

### Alignment Assessment
| Aspect | Original Intent | Actual Implementation | Match? |
|---|---|---|---|
| Core functionality | [what was planned] | [what was built] | // |
| Architecture approach | [planned pattern] | [actual pattern] | // |
| User experience | [expected UX] | [actual UX] | // |
| Performance profile | [expected perf] | [actual perf] | // |
| Scope | [planned scope] | [actual scope] | // |

## 2. Tech Debt Assessment
| Item | Severity | Description | Payoff Timeline |
|---|---|---|---|
| [debt item] | // | [what and why] | [when it becomes a problem] |

## 3. Alternative Approach Discussion
Each participating agent answers: **"If we could do it again, would we do it differently?"**

 **@sa:** [architectural perspective — was the pattern right?]
 **@dev:** [implementation perspective — was the approach efficient?]
 **@qc:** [quality perspective — was it testable and maintainable?]
 **@designer:** [UX perspective — did the implementation serve the user?]

## 4. Verdict
```

### Verdict Options

| Verdict | Meaning | Action |
|---|---|---|
| **SHIP** | Implementation matches intent, acceptable debt | Proceed to deployment |
| **SHIP WITH DEBT TICKET** | Works but has notable tech debt | Ship + create stories in `.hc/stories/` for debt items |
| **REFACTOR** | Approach is suboptimal but fixable | @dev refactors specific areas, then re-review |
| **RETHINK** | Implementation fundamentally diverged from intent | Route back to brainstorm / `/party-mode` |

---

## Tech Debt Classification

Use this taxonomy when cataloging debt:

| Type | Example | Urgency |
|---|---|---|
| **Design debt** | Wrong abstraction level, missing interfaces | Fix before next feature in same area |
| **Code debt** | Duplicated logic, magic numbers, missing types | Fix in next sprint |
| **Test debt** | Missing edge-case coverage, fragile tests | Fix before battle-test |
| **Documentation debt** | Outdated comments, missing JSDoc | Fix during polish phase |
| **Architecture debt** | Wrong pattern choice, coupling issues | Escalate to @sa immediately |

---

## Guardrails
- Review is **time-boxed to 1 round** — no extended debates
- Focus on the *approach*, not cosmetic code style (that's `/code-review`'s job)
- @pm has final authority on SHIP vs. RETHINK decisions
- All debt items MUST be logged as stories — never "we'll remember to fix it later"
- If `RETHINK` verdict: attach the review findings to the new brainstorm so the team doesn't repeat mistakes

---

## Integration Points
- **Input from:** `/battle-test` results, `verification-before-completion` report
- **Output to:** Deployment (if SHIP), `.hc/stories/` (if SHIP WITH DEBT), `/party-mode` (if RETHINK)
- **Artifacts:** Save to `.hc/reviews/YYYY-MM-DD-[feature-slug].md`
