---
description: Performance Budget - bundle size limits and Core Web Vitals thresholds
---

# RULE: PERFORMANCE BUDGET

**Mode:** Always On
**Scope:** @dev-fe, @designer

---

## Bundle Size Budgets
| Asset | Budget (gzipped) |
|---|---|
| Total JS | < 200KB |
| Individual route chunk | < 50KB |
| Total CSS | < 50KB |
| Any single image | < 200KB |
| Total initial load | < 500KB |

## Core Web Vitals Targets
| Metric | Budget |
|---|---|
| LCP | < 2.5s |
| FID / INP | < 200ms |
| CLS | < 0.1 |
| Time to Interactive | < 5s (3G) |

## Enforcement
- New dependencies MUST be justified and their bundle impact documented.
- `npm run build` + bundle analysis should be run before merging large PRs.
- Budget violations are Major findings in code review.

## Rules
- Never add a dependency > 50KB (gzipped) without @sa approval.
- Prefer native browser APIs over library solutions.
- Lazy load all routes and heavy components.
- Use tree-shakeable imports (`import { x } from 'lib'`, not `import * from 'lib'`).
