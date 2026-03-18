---
description: Architecture Decision Records - structured ADR template with traceability, review process, and lifecycle management
---

# SKILL: Architecture Decision Records (ADR)

**Trigger:** When @sa makes a significant technical decision that should be documented for future reference and traceability.

---

## When to Use
- Choosing a framework, library, or technology.
- Deciding on a design pattern or architecture style.
- Making trade-offs between competing approaches.
- Changing or superseding an existing architectural decision.
- When `decision-routing.md` score is < 85 on an architectural choice (document the reasoning).

---

## Decision Significance Matrix
Not every decision needs an ADR. Use this guide:

| Decision Type | ADR Needed? | Example |
|---|---|---|
| Framework / library choice | Yes | "Use Vite instead of Webpack" |
| Data model / schema design | Yes | "Store events as JSONB, not columns" |
| API design pattern | Yes | "REST over GraphQL for public API" |
| Architecture pattern | Yes | "Event sourcing for audit trail" |
| Dependency upgrade (minor) | No | "Upgrade React 19.0.1 → 19.0.2" |
| Refactoring approach | Maybe | Only if it changes public interfaces |
| Bug fix approach | No | Standard debugging, no ADR needed |

---

## The ADR Process

### Step 1: Identify the Decision
A decision exists when there are **two or more viable alternatives** and the choice has **lasting consequences**.

### Step 2: Research Alternatives
Use `research-analysis` skill to evaluate at least 2 alternatives with evidence.

### Step 3: Write the ADR

```markdown
# ADR-[NNN]: [Short, Descriptive Title]

**Date:** YYYY-MM-DD
**Status:** Proposed | Accepted | Deprecated | Superseded by ADR-XXX
**Deciders:** [@sa, @dev, @pm]
**Confidence:** [0-100] (Rule `decision-routing.md`)

## Context
[What is the situation? What problem are we solving? Why do we need to decide now?]

## Decision
[What did we decide? Be specific about the chosen approach and its boundaries.]

## Consequences

### Positive
- [Benefit 1 — be specific, not generic]
- [Benefit 2]

### Negative
- [Trade-off 1 — what we give up]
- [Trade-off 2]

### Risks
- [Risk 1] — Mitigation: [how we'll handle it]
- [Risk 2] — Mitigation: [how we'll handle it]

## Alternatives Considered

### Option A: [Name]
- **Pros:** [specific benefits]
- **Cons:** [specific drawbacks]
- **Rejected because:** [concrete reason]

### Option B: [Name]
- **Pros:** [specific benefits]
- **Cons:** [specific drawbacks]
- **Rejected because:** [concrete reason]

## Validation Plan
[How will we know this decision was correct? What metrics or outcomes will validate it?]

## References
- [Links to research, documentation, or discussions]
```

### Step 4: Review and Accept
1. ADR author: @sa
2. Reviewer: @pm (always), @dev (if implementation-heavy)
3. Status transitions: `Proposed → Accepted` or `Proposed → Rejected`

---

## ADR Lifecycle

```
Proposed → Accepted → [In Use]
 ↓
 Deprecated (no longer relevant)
 or
 Superseded by ADR-XXX (new decision replaces)
```

**Immutability rule:** Once accepted, the ADR content is frozen. To change a decision, create a **new ADR** that references and supersedes the old one.

## Storage & Naming
- Location: `.hc/adrs/`
- Filename: `adr-001-short-title.md`
- Sequential numbering, never reuse numbers.

## Rules
- ADRs are **immutable** once accepted — corrections go in new ADRs.
- All ADRs MUST include **alternatives considered** — a decision without alternatives is not a decision.
- Status changes require **@pm review**.
- Include a **validation plan** — how do you know the decision was right?
- Cross-reference related ADRs — "See also ADR-003."
