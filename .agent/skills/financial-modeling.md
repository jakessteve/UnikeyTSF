---
description: Financial Modeling - unit economics, burn rate, revenue projections, and scenario analysis for business planning
---

# SKILL: Financial Modeling

**Trigger:** When @ba creates financial projections for investor materials, business planning, or go/no-go feature decisions.

---

## When to Use
- Preparing investor pitch materials (skill `investor-pitch-writer`).
- Evaluating revenue potential of a new feature (go/no-go decision).
- Monitoring business health (burn rate, runway).
- Planning pricing strategy (freemium vs. premium).
- Quarterly business review.

---

## Core Metrics

| Metric | Formula | Target | Why It Matters |
|---|---|---|---|
| **CAC** (Customer Acquisition Cost) | Marketing spend ÷ New customers | Lower is better | How expensive is each user? |
| **LTV** (Lifetime Value) | ARPU × Avg lifespan (months) | LTV > 3× CAC | Is a user worth more than they cost? |
| **LTV:CAC Ratio** | LTV ÷ CAC | > 3:1 | Healthy unit economics indicator |
| **ARPU** (Avg Revenue Per User) | Total revenue ÷ Active users | — | Revenue density |
| **Churn Rate** | Lost customers ÷ Total per period | < 5% monthly | User retention health |
| **MRR** (Monthly Recurring Revenue) | Paying customers × Price | — | Revenue predictability |
| **ARR** (Annual Recurring Revenue) | MRR × 12 | — | Annualized revenue |
| **Burn Rate** | Monthly expenses - Monthly revenue | — | Cash consumption speed |
| **Runway** | Cash reserves ÷ Monthly burn | > 12 months | Time until money runs out |
| **Payback Period** | CAC ÷ Monthly ARPU | < 12 months | Time to recoup acquisition cost |

---

## The 3-Scenario Model
Always model three scenarios:

| Scenario | Growth Assumption | Conversion | Churn | Use |
|---|---|---|---|---|
| **Pessimistic** | 50% of base | 50% of base | 150% of base | Worst case / crisis planning |
| **Base** | Based on data/comparables | Industry standard | Industry standard | Primary planning model |
| **Optimistic** | 150% of base | 150% of base | 75% of base | Stretch goals / best case |

---

## Revenue Projection Template

```markdown
# Financial Projections — [Product/Feature]
**Date:** YYYY-MM-DD | **Author:** @ba
**Confidence:** [0-100] (Rule `decision-routing.md`)

## Unit Economics
| Metric | Current | Target |
|---|---|---|
| CAC | $X | $Y |
| LTV | $X | $Y |
| LTV:CAC | X:1 | >3:1 |
| Churn | X% | <5% |
| Payback | X months | <12 months |

## Revenue Projections (3-Year, Base Scenario)
| Metric | Year 1 | Year 2 | Year 3 |
|---|---|---|---|
| Total users | X | Y | Z |
| Conversion rate | A% | B% | C% |
| Paying users | X×A | Y×B | Z×C |
| ARPU (monthly) | $N | $N | $N |
| MRR (end of year) | $M | $M | $M |
| ARR | $M×12 | $M×12 | $M×12 |

## Cost Structure
| Category | Monthly | Annual | Notes |
|---|---|---|---|
| Hosting/infra | $X | $Y | [Provider, scaling assumptions] |
| Marketing | $X | $Y | [Channels, CAC target] |
| Tools/services | $X | $Y | [List key tools] |
| **Total costs** | **$X** | **$Y** | |

## Runway Analysis
| Scenario | Monthly Burn | Cash | Runway |
|---|---|---|---|
| Pessimistic | $X | $Y | Z months |
| Base | $X | $Y | Z months |
| Optimistic | $X | $Y | Z months |

## Assumptions
| # | Assumption | Source | Confidence |
|---|---|---|---|
| 1 | [Growth rate] | [Industry data / comparable] | High/Med/Low |
| 2 | [Conversion rate] | [Benchmark] | High/Med/Low |

## Sources
1. [Citation with URL]
```

## Rules
- **Always document assumptions** — every number needs a stated source or basis.
- **Use industry benchmarks** as sanity checks (SaaS, mobile app, freemium comparables).
- **Model 3 scenarios** — optimistic alone is investor fiction; pessimistic alone is fear.
- **Update quarterly** with actual data — projections without actuals drift into fantasy.
- **Include confidence scores** (Rule `decision-routing.md`) on projections.
