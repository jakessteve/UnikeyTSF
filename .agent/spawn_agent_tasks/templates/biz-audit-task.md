# Business Audit Task: [Track Name]
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


## Audit Question
> [Specific business/usability question this track must answer — one sentence]

## Scope
- **Features/flows to evaluate:** [list specific features, user flows, or pages]
- **User personas to consider:** [target audience segments]
- **Boundaries:** DO NOT evaluate [out-of-scope areas]
- **Time budget:** [estimated effort in minutes]

## Context
- **Project type:** [web app / native app / library / API / etc.]
- **Tech stack:** [from docs/tech/ARCHITECTURE.md]
- **Current user base:** [size, demographics, usage patterns — if known]
- **Business model:** [free / freemium / subscription / one-time / ad-supported]
- **This track is part of:** `/spawn-biz` workflow with [N] parallel tracks

## Audit Instructions
1. Evaluate the audit question from your domain expertise.
2. Gather evidence through analysis, market research, or user simulation.
3. For each finding, provide **specific evidence** (screenshots, data, market comparisons).
4. Score business impact per finding (Revenue / Retention / Acquisition / Brand).
5. Provide actionable recommendations (not just observations).
6. **Apply self-assessment** (MANDATORY — do not skip).

## Self-Assessment (MANDATORY)

After completing your audit, answer these questions:

### Assessment 1: Evidence Quality
- Is your evidence data-driven or opinion-based? (rate each finding)
- What data would strengthen your weakest recommendation?

### Assessment 2: User Centricity
- Are your recommendations good for users or just good for metrics?
- Could any recommendation degrade user experience?

### Assessment 3: Market Reality
- Have you compared against actual competitors?
- Is the market context current (not outdated assumptions)?

### Assessment 4: Feasibility
- Can the team realistically implement your recommendations?
- What's the minimum viable version of each recommendation?

## Output Format (MUST follow this structure)

```markdown
# Business Audit Track Report: [Track Name]

## Key Findings
| # | Finding | Evidence | Business Impact | User Impact | Confidence (1-10) |
|---|---------|----------|-----------------|-------------|-------------------|
| 1 | [finding] | [evidence] | Revenue: High | Delight | [score] |

## Recommendations
| # | Recommendation | Impact Area | Expected Outcome | Effort | Risk |
|---|----------------|-------------|------------------|--------|------|
| 1 | [what to do] | Revenue/Retention/Acquisition/Brand | [measurable outcome] | H/M/L | H/M/L |

## User Journey Observations
[Key friction points and delight moments observed]

## Competitive Comparison
| Feature/Aspect | Our App | Competitor A | Competitor B |
|---|---|---|---|
| [aspect] | [status] | [status] | [status] |

## Self-Assessment
### Evidence quality: [data-driven / mixed / opinion-based]
### User impact check: [all positive / some trade-offs / risky]
### Market validation: [verified / assumed / unknown]

## Limitations
[What was NOT evaluated, suggested follow-ups]
```

## Progress Reporting
After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
