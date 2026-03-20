---
description: UI/UX & Business Flow Swarm - spawn parallel agents to audit visual design, accessibility, responsiveness, design system, and business flow UX
---

# WORKFLOW: /spawn-ui (Multi-Agent UI/UX & Business Flow Swarm)

Triggered when @pm receives a request to exhaustively audit the app's UI/UX quality, design system consistency, and business flow user experience.

> **Inherits:** [`spawn-base-template.md`](spawn-base-template.md) — read the base template first for the full 6-phase pipeline.
> **Related workflows:** `/spawn-biz` (business-focused), `/ux-audit` (single-agent), `/qa-responsive-check` (responsive-only)

Execute sequentially, respecting gate decisions:

---

## Customizations (over base template)

### Track Assignment Guide (Phase 1)

| Track | Agent | Investigation Focus |
|---|---|---|
| Product & Flow Design | @pm | Navigation architecture, information hierarchy, user flow efficiency, feature discoverability, onboarding funnel |
| Requirements & Content | @ba | Content clarity, labeling accuracy, help text quality, error messages, localization readiness, copy consistency |
| Visual Design & System | @designer | Design token consistency, color/typography/spacing adherence, component reuse, visual hierarchy, animation quality, dark mode, responsive breakpoints |
| Business Flow UX | @biz | Conversion funnel optimization, CTA effectiveness, monetization touchpoint UX, trust signals, competitive UI benchmarking |
| Real-User Simulation | @user-tester | End-to-end flow testing, first-use experience, accessibility (WCAG), mobile usability, error recovery, edge case interactions |

### Spawn Details (Phase 2)

- Use `.agent/spawn_agent_tasks/templates/ui-audit-task.md` for prompts.
- Every prompt MUST include **screenshot/recording capture** requirement.
- @designer and @user-tester tracks MUST use `browser_subagent` for visual inspection.
- @designer: Use design system rules from `design-system-uiux` skill.

**Additional safety checks:**
- [ ] Visual tracks include screenshot capture in their prompts
- [ ] @user-tester and @designer have access to running application URL

### Validation Checklist (Phase 3)

Each report MUST contain:
- [ ] Findings with visual evidence (screenshots, recordings, annotated mockups)
- [ ] UX severity per finding (Critical / Major / Minor / Enhancement)
- [ ] Affected screens/components listed explicitly
- [ ] Before/after recommendations where applicable
- [ ] Accessibility impact notes (WCAG level affected, if any)

**UX Severity Classification:**

| Severity | Definition | Example |
|---|---|---|
| Critical | User cannot complete task | Broken form, invisible CTA, nav dead-end |
| Major | User struggles significantly | Confusing labels, poor error recovery, no loading state |
| Minor | User notices but works around | Inconsistent spacing, odd animation, small touch target |
| Enhancement | Opportunity to delight | Micro-animation, empty state illustration, subtle hover effect |

### Cross-Review Questions (Phase 4)

- Ask @designer: "Does this usability issue stem from a design system inconsistency?"
- Ask @biz: "Does this visual issue affect conversion rates?"
- Ask @user-tester: "Did real users actually stumble on the issue @designer found?"
- Ask @ba: "Is this labeling confusion caused by unclear requirements?"

Use markers: 🎨 Design fix, ♿ A11y fix, 📱 Mobile fix, 💰 Conversion fix, ✨ Enhancement.

**UX Issue Map format:**
```markdown
| Issue | Found By | Severity | Screens Affected | Design System Impact | Business Impact |
|---|---|---|---|---|---|
| [issue] | @designer, @user-tester | Major | [screens] | Token violation | Conversion: -5% est. |
```

### Scoring Weights (Phase 5)

| Factor | Weight |
|---|---|
| User impact (task completion, satisfaction) | 35% |
| Business impact (conversion, retention) | 25% |
| Design system alignment | 15% |
| Implementation effort | 15% |
| Accessibility improvement | 10% |

**Identify design system gaps:** Issues that require new tokens, components, or patterns.

### Report Template (Phase 6)

```markdown
# UI/UX & Business Flow Audit — [App/Feature Name]
**Date:** YYYY-MM-DD | **Requested by:** User
**Audit tracks:** N | **Agents involved:** [list]

## Executive Summary
## UX Health Dashboard
| Dimension | Score (1-10) | Issues Found | Top Issue |
|---|---|---|---|
| Visual Consistency | [N] | [N] | [one-liner] |
| Accessibility | [N] | [N] | [one-liner] |
| Responsiveness | [N] | [N] | [one-liner] |
| Flow Efficiency | [N] | [N] | [one-liner] |
| Business Flow UX | [N] | [N] | [one-liner] |

## Issue Dashboard
| Total | Critical | Major | Minor | Enhancement |
|---|---|---|---|---|

## Ranked Improvements
### P0 — Fix Now / ### P1 — Fix Soon / ### P2 — Next Design Sprint / ### P3 — Nice-to-Have
| # | Issue | Screens | UX Impact | Biz Impact | Effort | Track Source |
|---|---|---|---|---|---|---|

## Design System Gaps
## Accessibility Report
## Screenshot Gallery
## Methodology
## Limitations
## Appendix: Full Track Reports
```

**Output directory:** `.hc/ui-audit/`
