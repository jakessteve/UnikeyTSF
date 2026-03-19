# UI/UX Audit Task: [Track Name]
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


## Audit Question
> [Specific UI/UX dimension this track must evaluate — one sentence]

## Scope
- **Screens/components to inspect:** [list specific pages, components, or flows]
- **Viewports to test:** [mobile (375px), tablet (768px), desktop (1440px)]
- **Boundaries:** DO NOT evaluate [out-of-scope areas]
- **Time budget:** [estimated effort in minutes]

## Context
- **Project type:** [web app / native app / library / API / etc.]
- **Tech stack:** [from docs/tech/ARCHITECTURE.md]
- **Design system:** [tokens, component library — if exists]
- **Application URL:** [URL for browser testing]
- **This track is part of:** `/spawn-ui` workflow with [N] parallel tracks

## Audit Instructions
1. Evaluate the audit question from your domain expertise.
2. Use `browser_subagent` to visually inspect screens (MANDATORY for visual tracks).
3. Capture **screenshots** as evidence for every visual finding.
4. Classify each finding by UX severity (Critical / Major / Minor / Enhancement).
5. Provide before/after improvement suggestions where applicable.
6. **Apply self-assessment** (MANDATORY — do not skip).

## Visual Inspection Checklist

### Design Consistency
- [ ] Color tokens used consistently (no ad-hoc hex values)
- [ ] Typography hierarchy clear and consistent
- [ ] Spacing/padding follows design system grid
- [ ] Component styles reused (not one-off styles)

### Accessibility (WCAG 2.1 AA)
- [ ] Color contrast ≥ 4.5:1 for text, ≥ 3:1 for large text
- [ ] Interactive elements have visible focus indicators
- [ ] Touch targets ≥ 44×44px on mobile
- [ ] Screen reader landmarks and ARIA labels present
- [ ] Alt text on images

### Responsiveness
- [ ] Layout adapts at mobile (375px), tablet (768px), desktop (1440px)
- [ ] No horizontal overflow at any breakpoint
- [ ] Font sizes readable at all breakpoints
- [ ] Touch-friendly on mobile (no hover-only interactions)

### Interaction Design
- [ ] Loading states present for async operations
- [ ] Error states clear and actionable
- [ ] Empty states informative (not blank)
- [ ] Micro-animations smooth (no jank)
- [ ] Transitions feel natural (200-300ms)

## Self-Assessment (MANDATORY)

### Assessment 1: Coverage
- How many screens/components did you actually inspect? (vs. total)
- What did you skip and why?

### Assessment 2: Objectivity
- Are your findings based on design principles or personal preference?
- Which of your "Enhancement" findings are actually important?

### Assessment 3: User Impact
- Which issues would real users actually notice?
- Which are only visible to designers?

### Assessment 4: Design System Impact
- Do any findings require new design tokens or components?
- Are there systemic issues (affects many screens) vs. local issues?

## Output Format (MUST follow this structure)

```markdown
# UI/UX Audit Track Report: [Track Name]

## Issue Findings
| # | Issue | Screen/Component | Severity | Category | Screenshot |
|---|-------|-----------------|----------|----------|------------|
| 1 | [issue] | [screen] | Critical/Major/Minor/Enhancement | Design/A11y/Responsive/Interaction | [path] |

## Improvement Recommendations
| # | Recommendation | Before | After | Effort | Design System Change? |
|---|----------------|--------|-------|--------|-----------------------|
| 1 | [what to improve] | [current state] | [desired state] | H/M/L | Yes/No |

## Accessibility Summary
| WCAG Criterion | Status | Issues |
|---|---|---|
| 1.4.3 Contrast | ✅/⚠️/❌ | [details] |
| 2.4.7 Focus Visible | ✅/⚠️/❌ | [details] |
| 2.5.5 Target Size | ✅/⚠️/❌ | [details] |

## Responsive Summary
| Breakpoint | Status | Issues |
|---|---|---|
| Mobile (375px) | ✅/⚠️/❌ | [details] |
| Tablet (768px) | ✅/⚠️/❌ | [details] |
| Desktop (1440px) | ✅/⚠️/❌ | [details] |

## Self-Assessment
### Coverage: [N]% of screens inspected
### Systemic issues: [N] / Local issues: [N]
### Design system changes needed: [Yes/No — list]

## Limitations
[What was NOT inspected, suggested follow-ups]
```

## Progress Reporting
After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
