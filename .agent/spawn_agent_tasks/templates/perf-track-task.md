# Performance Task: [Track Name]
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


## Performance Question
> [Specific metric or performance dimension to profile — one sentence]

## Scope
- **Components/pages to profile:** [list specific files, routes, or services]
- **Metrics to measure:** [LCP, INP, CLS, bundle size, API latency, memory, etc.]
- **Boundaries:** DO NOT profile [out-of-scope areas]
- **Time budget:** [estimated effort in minutes]

## Context
- **Project type:** [web app / native app / library / API / etc.]
- **Tech stack:** [from docs/tech/ARCHITECTURE.md]
- **Performance budgets:** [from `performance-budget` rule, if defined]
- **Known issues:** [any reported slowness or metrics — if any]
- **This track is part of:** `/spawn-performance` workflow with [N] parallel tracks

## Profiling Instructions
1. Establish **baseline measurements** first (MANDATORY).
2. Profile the target area using appropriate tools.
3. Identify bottlenecks with **quantitative evidence** (not subjective impressions).
4. Estimate improvement potential for each bottleneck (e.g., "LCP: 3.2s → ~1.8s").
5. Propose optimization approach with effort estimate.
6. **Apply self-assessment** (MANDATORY — do not skip).

## Measurement Tools Guide

| Metric Area | Tools to Use |
|---|---|
| Bundle size | `npm run build` + analyze output, webpack-bundle-analyzer |
| Core Web Vitals | Lighthouse via `browser_subagent`, web-vitals library |
| Runtime performance | Chrome DevTools Performance tab, React Profiler |
| Memory | Chrome DevTools Memory tab, heap snapshots |
| API latency | Network tab, curl timing, `run_command` |
| Dependencies | `npm ls`, `npx depcheck`, import analysis |

## Self-Assessment (MANDATORY)

### Assessment 1: Measurement Validity
- Were measurements taken in a representative environment?
- Could caching, network conditions, or cold/warm start affect results?

### Assessment 2: Optimization ROI
- Is the improvement worth the implementation effort?
- Are you optimizing a hot path or a rarely-used feature?

### Assessment 3: Regression Risk
- Could the optimization break existing functionality?
- What tests would validate the optimization?

### Assessment 4: Premature Optimization Check
- Is this optimization needed NOW or is it speculative?
- What user-facing metric does this improve?

## Output Format (MUST follow this structure)

```markdown
# Performance Track Report: [Track Name]

## Baseline Measurements
| Metric | Value | Budget | Status |
|---|---|---|---|
| [metric] | [value] | [budget] | ✅/⚠️/❌ |

## Bottleneck Findings
| # | Bottleneck | Metric Impact | Location | Confidence (1-10) |
|---|-----------|---------------|----------|-------------------|
| 1 | [bottleneck] | LCP: +1.2s | [file/component] | [score] |

## Optimization Recommendations
| # | Optimization | Expected Improvement | Effort | Risk | Quick Win? |
|---|-------------|---------------------|--------|------|------------|
| 1 | [what to do] | LCP: -1.2s | Low | Low | Yes/No |

## Self-Assessment
### Measurement confidence: [1-10]
### Premature optimization risk: [Low/Med/High]
### Regression risk: [Low/Med/High]

## Limitations
[What was NOT profiled, environmental caveats, suggested follow-ups]
```

## Progress Reporting
After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
