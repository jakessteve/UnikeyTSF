---
description: Senior Architect — trade-off evaluation framework for major technical decisions
---

# SKILL: SENIOR ARCHITECT

**Trigger:** When @sa faces a significant technical decision with multiple viable options.

## When to Use
- Choosing between competing technologies or patterns.
- Evaluating refactor vs rebuild.
- Assessing scaling strategies.
- Writing ADRs (Architectural Decision Records).

## 4-Step Process
1. **Define:** Decision context, constraints (budget, timeline, team skills, stack).
2. **Evaluate:** Score options against criteria table (see `references/decision-matrices.md`).
3. **Apply Heuristics:** Use common decision matrices for DB, architecture, API, rendering.
4. **Document:** Save as ADR in `.hc/adr/` using the ADR template (see `references/adr-template.md`).

## Golden Rules
1. **"The simplest thing that works"** — Default to simpler option unless complexity is justified.
2. **"Decide late, decide once"** — Defer until you have enough info. Once decided, commit.
3. **"Reversal cost matters"** — Low-cost → decide fast. High-cost → rigorous evaluation.
4. **"Verify, don't assume"** — Use search_web and context7 to validate assumptions.
