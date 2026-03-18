---
description: Every shipped feature must have a GTM checklist — no silent launches
---

# RULE: GTM-Readiness

## Core Principle
A feature is not truly "done" until it has a Go-to-Market checklist completed alongside the technical verification. **No silent launches** — every user-facing change deserves intentional communication.

## When This Applies
- Any feature that changes what users see or experience.
- Any version release with user-facing changes.
- **Does NOT apply to:** internal refactors, test additions, dependency updates, or pure backend changes with zero UI impact.

## Minimum GTM Checklist
Before a feature can be marked as "shipped" by @pm, the following MUST exist:

```markdown
## GTM Readiness — [Feature Name]
- [ ] **CHANGELOG.md** entry written
- [ ] **Release note** drafted in `.hc/releases/`
- [ ] **Social-ready summary** (≤280 chars, tweetable)
- [ ] **Screenshots/GIFs** captured for the feature
- [ ] **Landing page / docs** updated (if applicable)
```

## Scaling by Launch Size

| Launch Size | Required | Optional |
|---|---|---|
| **Minor** (bug fix, polish) | CHANGELOG entry only | — |
| **Medium** (new feature, UI change) | Full checklist above | Blog post, email |
| **Major** (new module, product launch) | Full checklist + `/go-to-market` workflow | Product Hunt, community posts |

## Integration with Existing Gates
- This checklist is checked **after** @qc verification and **before** @pm marks the task as Done.
- For features routed through `/idea-forge`, GTM-readiness is part of Phase 7 (Log Conclusion).
- For features routed through `/hc-sdlc`, GTM-readiness runs during the Completion phase.

## Anti-Pattern: GTM Theater
Do NOT:
- Write a CHANGELOG entry that says "various improvements" — be specific.
- Skip screenshots because "it's a small change" — users don't care about your diff size.
- Create GTM content after launch — pre-launch prep is the whole point.
