---
description: Business & Usability Swarm - spawn parallel agents to audit market fit, user flows, conversion, and real-user simulation
---

# WORKFLOW: /spawn-biz (Multi-Agent Business & Usability Swarm)

Triggered when @pm receives a request to exhaustively evaluate the app's business viability, usability, and user experience from multiple stakeholder perspectives.

> **Inherits:** [`spawn-base-template.md`](spawn-base-template.md) — read the base template first for the full 6-phase pipeline.
> **Related workflows:** `/spawn-ui` (UI/UX-focused), `/user-test-session` (single-tester)

Execute sequentially, respecting gate decisions:

---

## Customizations (over base template)

### Track Assignment Guide (Phase 1)

| Track | Agent | Investigation Focus |
|---|---|---|
| Product Strategy & Market Fit | @pm | Value proposition clarity, competitive positioning, feature-market alignment, pricing viability, growth potential |
| Requirements & User Needs | @ba | User story completeness, persona coverage, pain points addressed, feature gap analysis, requirement traceability |
| Business Model & Growth | @biz | Revenue model validation, customer acquisition cost, retention metrics, monetization opportunities, partnership potential, GTM readiness |
| User Experience Testing | @user-tester | End-to-end user journey simulation, onboarding friction, task completion rates, error recovery, accessibility, first-impression scoring |

### Spawn Details (Phase 2)

- Use `.agent/spawn_agent_tasks/templates/biz-audit-task.md` for prompts.
- Every prompt MUST include **business impact scoring** requirement.
- Every prompt MUST include **user evidence** (not just opinion).
- @user-tester tracks MUST use `browser_subagent` for real simulation.
- @biz tracks should use `search_web` for market data where applicable.

**Additional safety checks:**
- [ ] @user-tester has access to running application URL

### Validation Checklist (Phase 3)

Each report MUST contain:
- [ ] Findings with specific evidence (screenshots, user flow recordings, market data)
- [ ] Business impact score per finding (Revenue / Retention / Acquisition / Brand)
- [ ] User sentiment indicators (Delight / Neutral / Frustration / Abandon)
- [ ] Competitive comparison where relevant
- [ ] Actionable recommendations (not just observations)

**Business Impact Classification:**

| Impact Area | Definition | Priority Weight |
|---|---|---|
| Revenue | Directly affects ability to generate income | Critical |
| Retention | Affects whether users come back | High |
| Acquisition | Affects whether new users convert | High |
| Brand | Affects perception and trust | Medium |
| Operational | Internal efficiency, not user-facing | Low |

### Cross-Review Questions (Phase 4)

- Ask @biz: "Does this usability issue affect our revenue model?"
- Ask @ba: "Does this market gap align with our user personas?"
- Ask @user-tester: "Did real users actually hit the friction point @ba identified?"
- Ask @pm: "Does this feature gap affect our competitive positioning?"

Use markers: 💰 Revenue impact, 👤 User impact, 📈 Growth impact, ⚠️ Risk.

**Business Insight Map format:**
```markdown
| Insight | Evidence From | Business Impact | User Impact | Priority |
|---|---|---|---|---|
| [insight] | @biz, @user-tester | Revenue: High | Frustration | P0 |
```

### Scoring Weights (Phase 5)

| Factor | Weight |
|---|---|
| Revenue impact (direct or indirect) | 30% |
| User satisfaction improvement | 25% |
| Competitive advantage | 20% |
| Implementation effort | 15% |
| Strategic alignment | 10% |

### Report Template (Phase 6)

```markdown
# Business & Usability Audit — [App/Feature Name]
**Date:** YYYY-MM-DD | **Requested by:** User
**Audit tracks:** N | **Agents involved:** [list]

## Executive Summary
## Business Health Dashboard
| Dimension | Score (1-10) | Trend | Key Finding |
|---|---|---|---|
| Market Fit | [N] | 🟢/🟡/🔴 | [one-liner] |
| Usability | [N] | 🟢/🟡/🔴 | [one-liner] |
| Revenue Potential | [N] | 🟢/🟡/🔴 | [one-liner] |
| Competitive Position | [N] | 🟢/🟡/🔴 | [one-liner] |
| User Satisfaction | [N] | 🟢/🟡/🔴 | [one-liner] |

## Ranked Recommendations
### P0 — Do Now / ### P1 — Do Soon / ### P2 — Next Quarter / ### P3 — Rejected
| # | Recommendation | Business Impact | User Impact | Effort | Track Source |
|---|---|---|---|---|---|

## User Journey Insights
## Competitive Landscape
## Methodology
## Limitations
## Appendix: Full Track Reports
```

**Output directory:** `.hc/biz-audit/`
