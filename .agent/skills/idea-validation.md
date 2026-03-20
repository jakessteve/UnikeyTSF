---
description: Idea Validation — lightweight hypothesis testing with desirability, feasibility, and viability scoring
---

# SKILL: IDEA VALIDATION

**Trigger:** When a brainstormed idea needs quick validation before full Red Team analysis, or when multiple competing ideas need ranking.

---

## When to Use
- Multiple ideas from a brainstorm need comparison and ranking.
- Quick sanity check before committing to a full `/idea-forge` cycle.
- User wants a gut-check on an idea's viability.
- During `/party-mode` synthesis to decide which ideas to pursue.

---

## DFV Scorecard (Desirability, Feasibility, Viability)

For each idea, score across three dimensions:

```markdown
# Idea Validation Scorecard

## Idea: [Name / One-line Description]

### Desirability (Do users want this?)
| Criteria | Score (1-5) | Evidence |
|---|---|---|
| Solves a real user pain point | ? | [user research, complaints, requests] |
| Frequency of use (daily/weekly/rare) | ? | [usage pattern estimate] |
| Differentiation from competitors | ? | [competitive analysis] |
| Emotional appeal / delight factor | ? | [UX assessment] |
| **Subtotal** | **/20** | |

### Feasibility (Can we build this?)
| Criteria | Score (1-5) | Evidence |
|---|---|---|
| Technical complexity | ? | [architecture assessment] |
| Fits existing architecture | ? | [dependency analysis] |
| Estimated effort (inverse: 5=easy, 1=massive) | ? | [story points / file count] |
| Team has required expertise | ? | [skill assessment] |
| **Subtotal** | **/20** | |

### Viability (Should we build this?)
| Criteria | Score (1-5) | Evidence |
|---|---|---|
| Aligns with project vision | ? | [PRD / roadmap alignment] |
| Maintenance burden (inverse: 5=low, 1=high) | ? | [complexity assessment] |
| Risk of regression to existing features | ? | [integration risk] |
| Time-to-value (5=quick wins, 1=long payoff) | ? | [delivery timeline] |
| **Subtotal** | **/20** | |

---

## Total Score: __/60

### Rating Scale
| Score | Grade | Action |
|---|---|---|
| 48-60 | A — Strong Yes | Fast-track to implementation |
| 36-47 | B — Proceed | Normal pipeline, monitor risks |
| 24-35 | C — Conditional | Needs modifications or scope reduction |
| 12-23 | D — Weak | Defer to icebox unless compelling reason |
| 0-11 | F — Reject | Do not pursue |
```

---

## Comparison Matrix (Multiple Ideas)

When comparing 2+ ideas:

```markdown
# Idea Comparison Matrix

| Dimension | Idea A | Idea B | Idea C |
|---|---|---|---|
| Desirability | /20 | /20 | /20 |
| Feasibility | /20 | /20 | /20 |
| Viability | /20 | /20 | /20 |
| **Total** | **/60** | **/60** | **/60** |
| Grade | ? | ? | ? |
| Recommendation | ? | ? | ? |
```

---

## Quick Validation (5-Minute Version)

For rapid ideation sessions, use the simplified checklist:

- [ ] **User need:** Can I describe the problem this solves in one sentence?
- [ ] **Existing solution:** Is there already something that does this (internally or via competitors)?
- [ ] **Effort estimate:** Can this be done in ≤ 1 sprint?
- [ ] **Regression risk:** Will this break anything existing?
- [ ] **Reversibility:** Can we undo this if it's wrong?

All → Proceed | Any → Needs deeper analysis with full DFV scorecard.

---

## Integration Points
- **Input from:** `/party-mode` brainstorm output, user feature requests
- **Output to:** `/idea-forge` (if validated), icebox (if deferred), `/party-mode` re-debate (if conditional)
- **Artifacts:** Save to `.hc/validations/YYYY-MM-DD-[idea-slug].md`
