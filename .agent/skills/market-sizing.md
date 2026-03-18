---
description: Market Sizing - TAM/SAM/SOM calculations with validation methodology for investor materials and strategic decisions
---

# SKILL: Market Sizing

**Trigger:** When @ba needs to estimate market opportunity for investor pitches, strategic decisions, or go/no-go evaluations.

---

## When to Use
- Preparing investor pitch materials (skill `investor-pitch-writer`).
- Evaluating whether to build a new feature (is the market big enough?).
- Comparing market opportunities for prioritization.
- Validating assumptions in a PRD.

---

## The TAM/SAM/SOM Framework

### TAM (Total Addressable Market)
The entire market demand for your product category, globally.

**Calculation methods:**
| Method | Approach | Best When |
|---|---|---|
| **Top-down** | Industry report total × your category % | Established markets with research data |
| **Bottom-up** | Total potential customers × Avg revenue per customer | New/niche markets, more accurate |
| **Value theory** | Value delivered ÷ What % you can capture | Deep tech, platform plays |

**Formula (bottom-up):** `TAM = Total potential customers × Average annual revenue per customer`

### SAM (Serviceable Addressable Market)
The portion of TAM you can realistically target, filtered by constraints.

**Filters to apply:**
- **Geography:** Which countries/regions can you serve?
- **Language:** What languages does your product support?
- **Platform:** Web only? Mobile? Desktop?
- **Demographics:** Age, income, tech-savviness
- **Use case:** Which specific problem are you solving?

**Example:** "Vietnamese-speaking users interested in astrology apps" = subset of "all astrology app users"

### SOM (Serviceable Obtainable Market)
What you can realistically capture in 1-3 years.

**Formula:** `SOM = SAM × Realistic market share %`

**Market share benchmarks:**
| Market Type | Realistic Year 1 | Year 3 |
|---|---|---|
| Crowded market, strong competitors | 0.5-2% | 3-8% |
| Growing market, moderate competition | 2-5% | 5-15% |
| Niche market, limited competition | 5-15% | 15-30% |
| Blue ocean, first mover | 10-25% | 20-40% |

---

## Validation Step
Every market sizing MUST include a sanity check:

```markdown
### Sanity Check
- [ ] TAM > SAM > SOM (if not, logic error)
- [ ] SOM is achievable with current team/resources
- [ ] Multiple sources cross-referenced (≥2 for TAM)
- [ ] Assumptions clearly stated and justified
- [ ] Comparable companies' market share referenced
```

## Deliverable Template
```markdown
# Market Sizing — [Product/Feature]
**Date:** YYYY-MM-DD | **Author:** @ba
**Confidence:** [0-100] (Rule `decision-routing.md`)

## TAM: $X (or N users)
**Method:** [Top-down / Bottom-up / Value theory]
[Calculation methodology and sources]

## SAM: $Y
**Filters applied:** [Geography, language, platform, etc.]
[Rationale for each filter]

## SOM: $Z (Year 1-3 projection)
**Market share assumption:** [X%]
[Growth assumptions and competitive factors]

## Key Assumptions
| # | Assumption | Source | Confidence |
|---|---|---|---|
| 1 | [Assumption] | [Source] | High/Med/Low |

## Sanity Check
[Cross-reference with comparable companies]

## Sources
1. [Citation with URL]
2. [Citation with URL]
```

## Rules
- **Always document assumptions** — a number without an assumption is a guess.
- **Use multiple methods** when possible (top-down + bottom-up should roughly converge).
- **Include confidence scores** (Rule `decision-routing.md`).
- **Source everything** — "I estimated" is not a source.
- **Model 3 scenarios** (optimistic, base, pessimistic) for SOM projections.
