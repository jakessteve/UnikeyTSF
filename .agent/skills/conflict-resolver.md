---
description: Conflict Resolver — automated mediation and fix loops between disagreeing agents
---

# SKILL: CONFLICT RESOLVER

**Trigger:** When @qc reports a bug in @dev's code, when agents disagree on approach, or when parallel outputs are incompatible.

---

## When to Use
- @qc finds a failing test or bug in @dev's implementation.
- @designer's UI calls an API that @dev named differently.
- @sa and @dev disagree on architectural approach.
- Two agents produce conflicting outputs during parallel execution.

---

## Conflict Types & Resolution Strategies

### Type 1: Bug Conflict (@qc → @dev)
The most common conflict: @qc finds a bug, @dev needs to fix it.

**Automated Fix Loop:**
```
1. REPRODUCE — @qc provides exact test case and failure output
2. DIAGNOSE — @dev reads the failing test, identifies root cause
3. FIX — @dev patches the code
4. VERIFY — @qc re-runs the test
5. REPORT — Only report to @pm when GREEN 
```

**Rules:**
- The fix loop runs in the background — don't interrupt other agents.
- Max **3 fix attempts**. If still failing → escalate to @sa for architectural review.
- @dev MUST NOT change the test to make it pass. Fix the code, not the test.

### Type 2: Naming Conflict (@designer ↔ @dev)
Parallel work produced mismatched names (API endpoints, props, types).

**Resolution:**
1. Compare the two names side-by-side.
2. The **producer** (whoever creates the API/function) wins the naming.
3. The **consumer** (whoever calls it) updates their code to match.
4. Broadcast the canonical name via `context-juggler` skill.

### Type 3: Approach Conflict (@sa ↔ @dev)
Disagreement on how to implement something.

**Resolution via Decision Matrix:**
```markdown
| Criteria | Option A (@sa) | Option B (@dev) |
|----------|---------------|----------------|
| Simplicity | ? | ? |
| Performance | ? | ? |
| Testability | ? | ? |
| Maintainability | ? | ? |
| Alignment with ARCHITECTURE.md | ? | ? |
| **Score** | X/5 | X/5 |
```

- If scores are close (within 1 point) → @pm picks the option that aligns with `docs/tech/ARCHITECTURE.md`.
- If scores are tied → escalate to User.

### Type 4: Merge Conflict (Parallel Outputs)
Two agents modified related code or docs simultaneously.

**Resolution:**
1. Identify the conflicting sections.
2. Determine which agent's output is the "source of truth" for that section.
3. Apply the SOT agent's version.
4. Have the other agent re-do their work against the updated context.

---

## Escalation Protocol

| Situation | Action |
|-----------|--------|
| Fix loop succeeds in ≤ 3 attempts | Auto-resolved, log and continue |
| Fix loop fails after 3 attempts | Escalate to @sa for review |
| Approach conflict resolved by matrix | Log decision in `.hc/decisions/` |
| Approach conflict tied | Escalate to User |
| Naming conflict | Producer wins, broadcast update |
| Merge conflict | SOT agent wins, other re-does |

---

## Conflict Log
After every resolution, log the conflict:
```markdown
## Conflict: [ID]
- **Type:** Bug / Naming / Approach / Merge
- **Agents:** @agentA vs @agentB
- **Resolution:** [What was decided]
- **Attempts:** [Number of fix loops]
- **Root Cause:** [Why it happened]
```
Save to `.hc/logs/conflicts/[date]-[slug].md`.

---

## Additional Conflict Types

### Type 5: Role Overlap Conflict (Redundant Work)
Two agents produced solutions for the same problem because their scopes overlapped.

**Prevention (Pre-Wave):**
- @pm uses the File Ownership Map (`anti-patterns-swarm.md` §7.1) to assign clear boundaries.
- If two agents' skills overlap for a sub-task → @pm picks ONE owner, the other is "Reviewer" (read-only + feedback).

**Resolution (Post-Collision):**
1. Compare both outputs for quality.
2. Pick the better one based on: correctness → test coverage → simplicity.
3. Discard the other. Don't merge two solutions — pick one.
4. Log in conflict log with root cause: "Role overlap between @X and @Y on [file]."

