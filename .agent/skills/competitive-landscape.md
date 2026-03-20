---
description: Competitive Landscape - structured competitor analysis with feature matrix, SWOT, and positioning strategy
---

# SKILL: Competitive Landscape Analysis

**Trigger:** When @ba needs to analyze competitors, evaluate market positioning, or inform PRD/pitch decisions.

---

## When to Use
- Before writing a PRD for a new product/feature.
- When preparing investor pitch materials (skill `investor-pitch-writer`).
- When evaluating build-vs-buy decisions.
- During market research for strategic planning.
- When market conditions change (new competitor, competitor pivot, market shift).
- Before `/go-to-market` workflow — positioning drives messaging.

---

## The 5-Step Analysis Framework

### Step 1: Identify Competitors
Categorize competitors into tiers:

| Tier | Description | How to Find | Example |
|---|---|---|---|
| **Direct** | Same product, same market | Search app stores, Google | Rival apps in same category |
| **Indirect** | Different product, same need | Think: "otherwise they'd use..." | Traditional alternatives, manual processes |
| **Potential** | Could enter your market | Monitor adjacent players | Adjacent apps entering your market |
| **Substitutes** | Different solution, same job-to-be-done | Ask: "what do users do today?" | Web searches, YouTube videos |

**Research sources:**
- App Store / Google Play search for category keywords
- Google search for target keywords (pages 1-3)
- Product Hunt, AlternativeTo, G2
- Social media communities (Reddit, Facebook groups)
- Context7 for technical comparisons

### Step 2: Feature Matrix
Create a detailed comparison:

```markdown
## Feature Matrix — [Product Category] — YYYY-MM-DD
| Feature | Our Product | Competitor A | Competitor B | Competitor C |
|---|---|---|---|---|
| [Core Feature 1] | Full | Full | Partial | None |
| [Core Feature 2] | Partial | Full | Full | Partial |
| [Differentiator] | Full | None | None | None |
| **Pricing** | Free + Premium | $9/mo | $19/mo | Free |
| **Platform** | Web + PWA | Web + Mobile | Web only | Mobile only |
| **Language** | Vietnamese + English | English only | Multi-lang | Vietnamese |
| **Last Updated** | Active | Active | Stale (6mo+) | Active |
```

**Rating key:** = Full / Best-in-class | = Partial / Basic | = Missing | = Planned

### Step 3: SWOT Per Competitor
For each major (direct) competitor:

```markdown
### Competitor A: [Name]
| Category | Analysis |
|---|---|
| **Strengths** | [What they do well — be specific] |
| **Weaknesses** | [Where they fall short — verify, don't assume] |
| **Opportunities** | [Gaps we can exploit — connect to our features] |
| **Threats** | [What they could do to hurt us — be realistic] |
```

### Step 4: Positioning Map
Plot competitors on 2 key dimensions relevant to your market:
```
 Premium Quality
 |
 [Comp B] | [Comp A]
 |
Simple --------+---------- Feature-Rich
 |
 [Us] | [Comp C]
 |
 Basic Quality
```

**Choose axes that matter to your users**, not to you. Common axes:
- Simple ↔ Feature-Rich
- Budget ↔ Premium
- Local/Niche ↔ Global/General
- Traditional ↔ Modern

### Step 5: Strategic Summary

```markdown
## Competitive Analysis Summary — YYYY-MM-DD
**Confidence:** [0-100] (Rule `decision-routing.md`)

### Our Competitive Advantages
1. **[Advantage 1]:** [Why this matters to users — quantify if possible]
2. **[Advantage 2]:** [Why this is hard to replicate]

### Gaps to Close (Priority Order)
| Gap | Competitor Has | Our Plan | Priority | Timeline |
|---|---|---|---|---|
| [Gap 1] | Comp A, Comp B | [How to address] | P1 | Q2 2025 |
| [Gap 2] | Comp A | [How to address] | P2 | Q3 2025 |

### Defensible Moats
1. **[Moat]:** [Why this is hard for competitors to replicate]

### Recommended Actions
1. [Strategic recommendation based on analysis]
```

---

## Deliverable
Output a `.hc/research/competitive-landscape-YYYY-MM-DD.md` report following the framework above.

## Rules
- **Use public information only** — no reverse engineering, no ToS violations.
- **Verify claims from at least 2 sources** — don't trust a single review.
- **Update quarterly** or before major strategy shifts.
- **Include confidence scores** (Rule `decision-routing.md`) on key assessments.
- **Be honest about weaknesses** — a competitive analysis that says "we're best at everything" is worthless.
