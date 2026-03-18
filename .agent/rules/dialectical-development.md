---
description: Dialectical Development — ideas must survive adversarial scrutiny before and after implementation
---

# RULE: DIALECTICAL DEVELOPMENT

All significant ideas, features, and architectural decisions MUST pass through a structured debate-and-validate cycle. This rule ensures that confirmation bias does not lead to untested assumptions becoming production code.

---

## Core Principle

> Ideas are hypotheses. Implementation is an experiment. Debate is peer review.

No idea should be implemented without being challenged, and no implementation should ship without being questioned.

---

## When This Rule Applies

| Scope | Applies? | Mechanism |
|---|---|---|
| New pattern or architecture change | Yes | Full `/idea-forge` cycle |
| New module or major UX shift | Yes | Red Team review via `red-team-ideas` skill |
| Extension of existing pattern (new page matching existing) | No | Fast-path, Cynefin "Clear" — skip debate |
| Bug fix | No | Fast-path, skip debate |
| Documentation update | No | Direct execution |
| Performance optimization | Conditional | Red Team if approach changes architecture |
| Ambiguous | Conditional | @pm uses `critical-thinking-models` to classify |

---

## Minimum Requirements

### Before Implementation
- [ ] Idea has been discussed by ≥ 2 agent personas (via `/party-mode` or `/idea-forge`)
- [ ] At least one adversarial challenge has been raised and addressed
- [ ] If multiple approaches exist, a decision matrix has been produced

### After Implementation
- [ ] @qc verification has passed
- [ ] For significant features: `/implementation-review` or `/idea-forge` Phase 6 has been executed
- [ ] Any tech debt has been logged as stories in `.hc/stories/`

---

## Anti-Patterns to Avoid

| Anti-Pattern | Why It's Dangerous | Prevention |
|---|---|---|
| **Echo chamber brainstorm** | All agents agree immediately → untested assumptions | Require Red Team round for significant decisions |
| **Build-first, ask-later** | Implementation locks in a bad approach | Enforce debate *before* coding starts |
| **Ship without review** | Tech debt accumulates silently | Mandatory `/implementation-review` for features |
| **Infinite debate loop** | Analysis paralysis prevents shipping | Max 3 rounds discussion, max 2 Red Team rounds |
| **Ignoring Red Team findings** | Defeats the purpose of adversarial review | All Red Team findings must be addressed in writing |

---

## Escalation

If debate cannot be resolved within the framework:
1. @pm produces a decision matrix with all options scored.
2. Present to User with @pm's recommendation.
3. User makes the final call.
4. Log the decision and rationale in `.hc/decisions/`.
