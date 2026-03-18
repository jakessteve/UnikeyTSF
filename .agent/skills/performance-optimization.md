---
description: Performance Optimization - profiling-first approach to bundle, runtime, and rendering optimization
---

# SKILL: Performance Optimization

**Trigger:** When @dev needs to optimize app speed, bundle size, or runtime performance. Always used during `/performance-audit` workflow.

---

## When to Use
- Users report slowness or lag.
- Lighthouse audit scores below target (Rule `performance-budget.md`).
- Bundle size exceeds budget (>200KB gzip).
- Calculation engines take too long (>50ms per chart).
- Before major releases — ensure no performance regressions.

---

## The Iron Rule
> **Measure BEFORE and AFTER.** Optimization without benchmarks is guessing. Use `performance-testing` skill to establish baselines.

---

## The 5-Step Optimization Process

### Step 1: Profile and Measure
```bash
# Bundle analysis
npx vite-bundle-analyzer

# Lighthouse audit (via CLI)
npx lhci autorun --collect.url=http://localhost:5173

# Chrome DevTools Performance panel
# Record → Analyze → Identify bottlenecks
```

### Step 2: Identify the Bottleneck Category

| Category | Symptoms | Diagnostic Tool |
|---|---|---|
| **Bundle size** | Slow initial load, large JS chunks | `vite-bundle-analyzer` |
| **Rendering** | Jank, dropped frames, layout shifts | Chrome Performance panel |
| **Runtime logic** | Slow calculations, unresponsive UI | `console.time`, Vitest bench |
| **Network** | Slow API calls, blocking resources | Network panel, waterfall chart |
| **Memory** | Growing usage over time, crashes | Memory panel, heap snapshots |

### Step 3: Apply Targeted Fixes

#### Bundle Size Optimization
| Technique | Impact | How |
|---|---|---|
| Route-based code splitting | High | `React.lazy(() => import('./Page'))` |
| Tree-shaking unused exports | High | Named imports, no barrel files |
| Replace heavy libraries | Medium | `date-fns` → lighter alternative, `lodash` → native JS |
| Externalize large deps | Medium | Vite `build.rollupOptions.external` |
| Compress assets | Medium | `vite-plugin-compression` (gzip/brotli) |

#### Runtime Optimization
| Problem | Solution | React Pattern |
|---|---|---|
| Expensive calculations | Memoize results | `useMemo(() => compute(data), [data])` |
| Frequent re-renders | Stabilize references | `React.memo()`, `useCallback()` |
| Large lists (>100 items) | Virtualize rendering | `react-window` or `@tanstack/virtual` |
| Heavy initial computation | Defer to idle time | `requestIdleCallback()` or web worker |
| Layout thrashing | Batch DOM operations | Read-then-write pattern |

#### Rendering Optimization
| Problem | Solution |
|---|---|
| Layout shifts (CLS) | Set explicit `width`/`height` on images/embeds |
| Slow LCP | Preload critical assets, inline critical CSS |
| Unnecessary paints | Use `will-change` sparingly, `transform` for animations |
| Font loading flash | `font-display: swap` + preload font files |

#### Image Optimization
| Technique | Impact |
|---|---|
| WebP/AVIF format | 30-50% smaller than JPEG/PNG |
| Responsive `srcset` | Right size for each viewport |
| Lazy loading | `loading="lazy"` for below-fold images |
| Explicit dimensions | Prevents CLS |

### Step 4: Verify Improvement
Run the same benchmarks from Step 1 and compare:
```markdown
| Metric | Before | After | Δ | Target Met? |
|---|---|---|---|---|
| Bundle size | XKB | YKB | -Z% | / |
| LCP | Xs | Ys | -Zms | / |
| Engine speed | Xms | Yms | -Z% | / |
```

### Step 5: Document
Record in `.hc/benchmarks/` for trend tracking.

---

## Performance Budget (Rule `performance-budget.md`)

| Asset | Budget | Action on Violation |
|---|---|---|
| Total JS bundle (gzipped) | < 200KB | Block merge until optimized |
| Individual route chunk | < 50KB | Code split or optimize |
| CSS (gzipped) | < 50KB | Purge unused styles |
| LCP | < 2.5s on 3G | Optimize critical path |
| TTI | < 5s on 3G | Defer non-critical JS |
| CWV (Core Web Vitals) | All "Good" | Required for release |

## Rules
- **Profile first, optimize second** — never optimize based on assumptions.
- **One optimization at a time** — isolate the impact of each change.
- **Test on representative devices** — not just your fast laptop; simulate 3G throttling.
- **Document improvements** — future-you needs to know what was optimized and why.
- **Don't micro-optimize** — save effort for changes that move the metric meaningfully (>5%).
