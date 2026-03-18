---
description: Critical Thinking Models — structured executive decision gate for @pm before delegating features or architecture
---

# SKILL: Critical Thinking Models

**Trigger:** Before any feature or architecture delegation (after `requirement-enrichment`, before Auto-Delegation). Skipped for bug fixes, documentation, and trivial changes (≤3 files).

---

## When to Use
- User requests a new feature, module, or architectural change.
- A brainstorm session concludes and routes to implementation.
- `requirement-enrichment` has clarified the task and it's a feature/architecture scope.

## When to SKIP
- Bug fixes, hotfixes, production incidents.
- Documentation, content, or copy changes.
- Trivial changes (≤3 files, single concern).
- Tasks already classified as **Clear** by Cynefin (§4 below).

---

## The Seven-Model Checklist

Run through each model in order. Each takes ~1-2 sentences. Total gate: < 30 seconds of reasoning.

### 1. First Principles
> "What are the 2-3 non-negotiable constraints of this feature?"

Strip away assumptions, industry norms, and "how others do it." What MUST be physically, logically, or technically true for this to work?

**Output:** 2-3 bullet points of fundamental constraints.

### 2. Second-Order Effects
> "If we build this, and then what?"

Look beyond the immediate result. Identify downstream consequences:
- **1 negative downstream effect** on codebase, performance, or maintenance.
- **1 negative downstream effect** on UX or user behavior.
- **1 positive compounding effect** (if any — don't force it).

**Output:** 3 bullet points (2 risks, 1 opportunity).

### 3. Inversion (Quick Pre-Mortem)
> "If this feature failed in 3 months, what are the top 2 reasons?"

Assume the feature was built and flopped. Work backward to find the cause. This is NOT a full Pre-Mortem (use `red-team-ideas` for that on major features) — it's a 2-sentence gut check.

**Output:** 2 failure reasons, each with a 1-line mitigation.

### 4. Cynefin Classification
> "What complexity domain does this problem live in?"

| Domain | Signal | Response Strategy |
|---|---|---|
| **Clear** | Best practice exists, no debate needed | Fast-path → skip remaining models |
| **Complicated** | Needs expert analysis, multiple valid approaches | Route to @sa for architecture review |
| **Complex** | No clear cause-effect, emergent patterns | Probe first: build a spike/experiment, measure, then decide |
| **Chaotic** | Crisis, production down, urgent | Act immediately → analyze later (skip this gate entirely) |

**Output:** Domain classification + response strategy.

### 5. Opportunity Cost
> "What existing priority will be delayed if we do this?"

Check the current roadmap/backlog. Name the specific story, epic, or sprint goal that gets pushed back.
- If nothing gets pushed → capacity is free → proceed.
- If a P1 gets delayed → flag to User with trade-off.

**Output:** Trade-off statement or "No competing priority identified."

### 6. Circle of Competence
> "Is this within our project's core scope?"

| Assessment | Action |
|---|---|
| Core scope (directly serves users) | Proceed |
| Adjacent (related but not core) | Flag to User: "This is adjacent to our scope. Recommend deferring to Phase X." |
| Out of scope (unrelated to product vision) | Recommend Icebox with rationale |

**Output:** Scope assessment + recommendation.

### 7. Cognitive Bias Scan
> "Am I falling into a known thinking trap?"

Quickly scan for 6 common biases that derail agent decisions:

| Bias | Trigger Question | Debiasing Action |
|---|---|---|
| **Confirmation** | Am I only looking at evidence that supports my preferred approach? | Force yourself to list **1 strong argument AGAINST** your choice. |
| **Anchoring** | Am I fixated on the first solution I thought of? | Generate **2 alternative approaches** before proceeding. |
| **Sunk Cost** | Am I continuing because of past effort, not future value? | Ask: "If I started fresh today, would I still choose this?" |
| **Status Quo** | Am I avoiding change because current code "works fine"? | Ask: "Would I design it this way if building from scratch?" |
| **Survivorship** | Am I only considering successful examples? | Ask: "What projects tried this approach and failed?" |
| **Dunning-Kruger** | Am I overconfident about an unfamiliar domain? | Check: Have I consulted docs/context7 for this area? |

**Output:** Flag any bias detected. If >1 bias flagged → reduce confidence score by 10 points and note the specific bias in your reasoning.

---

## Output Format

```markdown
### Critical Thinking Gate — [Feature Name]

**First Principles:** [2-3 constraints]
**Second-Order:** [risk 1] | [risk 2] | [opportunity]
**Pre-Mortem:** Fail reason 1: [x] → Mitigation: [y]. Fail reason 2: [x] → Mitigation: [y].
**Cynefin:** [Domain] → [Response strategy]
**Opportunity Cost:** [Trade-off or "None"]
**Scope:** [Core / Adjacent / Out of scope] → [Action]
**Bias Check:** [None detected / Bias: [name] → [debiasing action taken]]

**Gate Verdict:** PROCEED / PROCEED WITH CHANGES / PROBE FIRST / DEFER
```

---

## Integration Points
- **Input:** Enriched requirement from `requirement-enrichment` skill.
- **Output:** Feeds into Auto-Delegation Decision Table (§3.2 of `@pm.md`) or `/idea-forge` if Complex domain.
- **Placement:** After @pm Step 0 (Input Enrichment), before Step 3.2 (Auto-Delegation).
- **Complements:** `red-team-ideas` (deeper adversarial analysis), `idea-validation` (idea comparison), `structured-analysis-frameworks` (MECE, Issue Trees, Weighted Matrix for complex decisions).

---

## Guardrails
- This gate is a **mental checklist**, not a workflow. It should take < 30 seconds.
- Do NOT block on this gate for simple tasks. If Cynefin says **Clear** → skip.
- Do NOT use this gate to justify inaction. The default is **PROCEED**.
- If all seven models agree → there's no reason to delay.
- The Bias Scan is a **check-yourself** mechanism, not a reason to halt. If a bias is detected, address it and move on.

