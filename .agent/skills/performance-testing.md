---
description: Performance Testing - load, stress, and benchmark patterns with structured result reporting
---

# SKILL: Performance Testing

**Trigger:** When @qc needs to validate performance under load, establish baselines, or detect regressions.

---

## When to Use
- Before optimization — establish a baseline to prove improvement.
- After major changes — verify no performance regression.
- Before releases — validate under expected load.
- When users report slowness — quantify the problem.
- During `/performance-audit` workflow — structured benchmark collection.

---

## Types of Performance Tests

| Type | Purpose | When | Duration |
|---|---|---|---|
| **Benchmarking** | Establish baseline metrics | Before optimization, after major changes | Minutes |
| **Load testing** | Verify under expected traffic | Before every release | 15-30 min |
| **Stress testing** | Find the breaking point | Capacity planning | 30-60 min |
| **Soak testing** | Detect memory leaks over time | Before major releases | 1-4 hours |
| **Spike testing** | Handle sudden traffic surges | Before marketing campaigns | 10-15 min |

---

## The 4-Step Process

### Step 1: Define What to Measure

| Metric | Target | Tool | Why |
|---|---|---|---|
| Page load time (LCP) | < 2.5s | Lighthouse, Web Vitals | Core Web Vital |
| First Input Delay (FID) | < 100ms | Lighthouse, Web Vitals | User responsiveness |
| Cumulative Layout Shift (CLS) | < 0.1 | Lighthouse, Web Vitals | Visual stability |
| API response time (p95) | < 500ms | Custom timing | Backend performance |
| Engine calculation speed | < 50ms per chart | `console.time` / Vitest bench | Domain-specific |
| Memory usage growth | < 10MB over 1hr | Chrome DevTools | Leak detection |
| Bundle size (gzipped) | < 200KB | vite-bundle-analyzer | Load time impact |
| Time to Interactive (TTI) | < 3.5s (3G) | Lighthouse | Mobile UX |

### Step 2: Choose Method and Tool

**Frontend benchmarks:**
```typescript
// Vitest benchmark (preferred for engines)
import { bench, describe } from 'vitest';

describe('Tử Vi Engine Benchmark', () => {
 bench('generate chart', () => {
 generateTuViChart(sampleBirthData);
 }, { iterations: 1000, time: 5000 });
});
```

**Quick console benchmark:**
```typescript
console.time('chartGeneration');
for (let i = 0; i < 1000; i++) {
 generateTuViChart(testBirthData);
}
console.timeEnd('chartGeneration');
// Expected: < 1000ms for 1000 iterations
```

**Lighthouse CI:**
```bash
npx lhci autorun --collect.url=http://localhost:5173
```

### Step 3: Run and Record
- Run benchmarks **on a consistent environment** (same machine, same conditions).
- Run each benchmark **at least 3 times** and take the median.
- Record results with timestamps for trend tracking.

### Step 4: Report and Compare

```markdown
## Performance Report — [Feature/Module]
**Date:** YYYY-MM-DD | **QC:** @qc
**Environment:** [Machine, browser, network condition]

### Results
| Metric | Previous | Current | Δ | Target | Status |
|---|---|---|---|---|---|
| LCP | 2.1s | 1.8s | -14% | < 2.5s | |
| Bundle size | 180KB | 195KB | +8% | < 200KB | |
| Engine speed | 45ms | 42ms | -7% | < 50ms | |

### Regression Analysis
[Any metrics that got worse — root cause and action]

### Recommendations
1. [Optimization suggestion with expected impact]
```

---

## Performance Budget (Rule `performance-budget.md`)

| Metric | Budget | Action on Violation |
|---|---|---|
| Bundle size increase | +10KB gzip per PR | Block merge, optimize |
| LCP regression | +500ms | Investigate before merge |
| Engine speed regression | +20% | Optimize or justify |

## Rules
- **Always benchmark BEFORE and AFTER** optimization — prove the improvement.
- **Test on representative devices** — not just your fast laptop. Simulate 3G throttling.
- **Median, not average** — outliers skew averages. Report p50 and p95.
- **Document results** in `.hc/benchmarks/` with timestamps for trend tracking.
- **Performance tests are not optional for engine changes** — Tử Vi, Bát Tự, QMDJ engines MUST be benchmarked.
