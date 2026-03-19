---
description: Performance & Scale Swarm - spawn parallel agents to audit bundle size, runtime, Core Web Vitals, database, caching, and scale readiness
---

# WORKFLOW: /spawn-performance (Multi-Agent Performance & Scale Swarm)

Triggered when @pm receives a request to deeply audit the application's performance, identify bottlenecks, and assess scalability.

> **Inherits:** [`spawn-base-template.md`](spawn-base-template.md) — read the base template first for the full 6-phase pipeline.
> **Related workflows:** `/spawn-debug` (bug-focused)

Execute sequentially, respecting gate decisions:

---

## Customizations (over base template)

### Track Assignment Guide (Phase 1)

| Track | Agent | Investigation Focus |
|---|---|---|
| Frontend Performance | @dev | Bundle size analysis, tree-shaking, lazy loading, code splitting, render performance, re-render cycles, virtual DOM efficiency |
| Core Web Vitals & UX Perf | @qc | LCP, FID/INP, CLS measurements, Lighthouse scoring, perceived performance, loading states, animation jank |
| Infrastructure & Runtime | @devops | Server response times, CDN configuration, caching headers, compression (gzip/brotli), memory usage, connection pooling, resource hints |
| Architecture & Scalability | @sa | Algorithm complexity, data structure efficiency, API design bottlenecks, N+1 queries, state management overhead, horizontal scaling readiness |

### Spawn Details (Phase 2)

- Use `.agent/spawn_agent_tasks/templates/perf-track-task.md` for prompts.
- Every prompt MUST include **quantitative metrics requirement** (not subjective).
- Every prompt MUST include **before/after measurement plan**.
- Every prompt MUST specify performance budget baselines from `performance-budget` rule.
- @qc tracks SHOULD use `browser_subagent` for Lighthouse runs.
- @dev tracks SHOULD use `run_command` for bundle analysis.

**Additional safety checks:**
- [ ] Each worker prompt includes quantitative measurement requirement
- [ ] Workers have baseline metrics to compare against

### Validation Checklist (Phase 3)

Each report MUST contain:
- [ ] Quantitative findings with specific measurements (ms, KB, scores)
- [ ] Bottleneck identification with evidence (profiles, flame charts, bundle maps)
- [ ] Impact classification per finding (Critical / Significant / Moderate / Minor)
- [ ] Estimated improvement potential (e.g., "LCP: 3.2s → ~1.8s")
- [ ] Optimization approach with effort estimate

**Performance Impact Classification:**

| Impact | Definition | Threshold Example |
|---|---|---|
| Critical | Fails performance budget | LCP > 4s, Bundle > 500KB, CLS > 0.25 |
| Significant | Noticeably slow | LCP 2.5-4s, API > 500ms, FPS < 30 |
| Moderate | Room for improvement | LCP 1.5-2.5s, API 200-500ms |
| Minor | Already acceptable, but optimizable | LCP < 1.5s, micro-optimization |

### Cross-Review Questions (Phase 4)

- Ask @sa: "Does this runtime bottleneck stem from the data model design?"
- Ask @devops: "Can caching solve this API latency issue found by @dev?"
- Ask @qc: "Does the bundle size issue from @dev show up in your Lighthouse LCP?"
- Ask @dev: "Can the architecture change @sa suggests be implemented without regression?"

Use markers: ⚡ Quick win, 🏗️ Architecture change, 📦 Bundle fix, 🗄️ Infra fix, 📊 Metric.

**Bottleneck Map format:**
```markdown
| Bottleneck | Metric Impact | Found By | Root Cause | Fix Category |
|---|---|---|---|---|
| [bottleneck] | LCP: +1.2s | @dev, @qc | Unoptimized images | Quick win |
```

### Scoring Weights (Phase 5)

| Factor | Weight |
|---|---|
| User-perceived improvement (LCP, INP, CLS) | 35% |
| Metric delta (measured improvement) | 25% |
| Implementation effort + regression risk | 20% |
| Scale impact (benefit at 10x load) | 10% |
| Quick win potential (< 1 day to ship) | 10% |

### Report Template (Phase 6)

```markdown
# Performance Audit — [App/Feature Name]
**Date:** YYYY-MM-DD | **Requested by:** User
**Audit tracks:** N | **Agents involved:** [list]

## Executive Summary
## Performance Dashboard
| Metric | Current | Target | Status | Top Bottleneck |
|---|---|---|---|---|
| LCP | [N]s | < 2.5s | 🟢/🟡/🔴 | [one-liner] |
| INP | [N]ms | < 200ms | 🟢/🟡/🔴 | [one-liner] |
| CLS | [N] | < 0.1 | 🟢/🟡/🔴 | [one-liner] |
| Bundle Size | [N]KB | < [budget]KB | 🟢/🟡/🔴 | [one-liner] |
| Lighthouse | [N]/100 | > 90 | 🟢/🟡/🔴 | [one-liner] |

## Ranked Optimizations
### P0 — Critical / ### P1 — Significant / ### P2 — Polish / ### P3 — Rejected
| # | Optimization | Metric Impact | Effort | Risk | Track Source |
|---|---|---|---|---|---|

## Bottleneck Map
## Performance Budget Recommendations
## Methodology
## Limitations
## Appendix: Full Track Reports
```

**Output directory:** `.hc/perf-audit/`
