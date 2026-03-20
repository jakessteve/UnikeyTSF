---
description: Red Team Ideas — structured adversarial analysis to stress-test brainstorm outputs before implementation
---

# SKILL: RED TEAM IDEAS

**Trigger:** After a brainstorm round produces a consensus direction (e.g., during `/party-mode` or `/idea-forge`). Used to pressure-test ideas before committing resources to implementation.

---

## When to Use
- After `/party-mode` reaches preliminary consensus.
- Before routing brainstorm decisions to `/hc-sdlc` for implementation.
- When a proposed feature, architecture, or approach feels "too easy" or unchallenged.
- During `/idea-forge` Phase 2 (Red Team).

---

## Process

### Step 1: Assign the Devil's Advocate
@pm selects **one agent** to serve as the Red Team lead for this session. Rotate the assignment to avoid bias:

| Session Context | Recommended Red Team Lead | Rationale |
|---|---|---|
| Feature/product idea | @qc | Tests edge cases, failure modes |
| Architecture/tech choice | @dev | Tests implementation feasibility |
| UI/UX direction | @user-tester | Tests real-user friction |
| Security-sensitive idea | @whitehat-hacker | Tests attack surface |
| Business/market idea | @ba | Tests market viability |

### Step 2: Pre-Mortem Analysis
The Red Team lead executes a **pre-mortem** — imagine the idea has been implemented and *failed*. Work backwards:

```markdown
## Pre-Mortem: [Idea Name]

**Assumption:** This idea was implemented and it FAILED. Here's why:

### Failure Scenario 1: [Title]
- **What went wrong:** [Describe the failure]
- **Root cause:** [Why it was always going to fail]
- **Warning signs we ignored:** [What signals exist today]
- **Probability:** High / Medium / Low

### Failure Scenario 2: [Title]
...

### Failure Scenario 3: [Title]
...
```

### Step 3: Assumption Mapping
List every unstated assumption behind the idea and challenge each:

```markdown
## Assumption Map

| # | Assumption | Evidence For | Evidence Against | Risk if Wrong |
|---|---|---|---|---|
| 1 | Users want X | [data/research] | [counter-evidence] | High / Medium / Low |
| 2 | We can build X in Y time | [estimate basis] | [complexity signals] | High / Medium / Low |
| 3 | X won't break existing Y | [analysis] | [dependency risks] | High / Medium / Low |
```

### Step 4: Edge-Case Ideation
Brainstorm the worst-case scenarios specific to the idea:

- **Scale:** What happens at 10x the expected load/data?
- **Abuse:** How could a malicious user exploit this?
- **Regression:** What existing features could this break?
- **Maintenance:** Who maintains this in 6 months? Is it clear?
- **Reversal:** Can we undo this if it's wrong? What's the rollback cost?

### Step 5: Red Team Verdict

```markdown
## Red Team Verdict

**Overall Risk Level:** Low / Medium / High / Critical

### Proceed with Modifications
- [Modification 1 to address Failure Scenario X]
- [Modification 2 to mitigate Assumption Y]

### Must-Address Before Implementation
- [ ] [Blocker 1 — cannot proceed until resolved]
- [ ] [Blocker 2]

### Accepted Risks
- [Risk we acknowledge but choose to accept, with rationale]

**Recommendation:** PROCEED / PROCEED WITH CHANGES / RETHINK / REJECT
```

---

## Integration Points
- **Input from:** `/party-mode` consensus, brainstorm artifacts from `.hc/brainstorms/`
- **Output to:** `/idea-forge` Phase 3 (Iterate Design), or back to `/party-mode` if rejected
- **Artifacts:** Save to `.hc/red-team/YYYY-MM-DD-[topic-slug].md`

---

## Guardrails
- The Red Team lead MUST argue in good faith — real risks, not nitpicking
- Other agents may defend, but must acknowledge valid criticisms
- Max **2 Red Team rounds** — if issues persist, escalate to User
- Red Team findings are NOT vetoes — @pm makes the final call based on evidence
