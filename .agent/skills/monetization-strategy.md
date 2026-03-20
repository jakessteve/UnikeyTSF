---
description: Monetization Strategy - pricing models, revenue streams, paywall design, and pricing experiments
---

# SKILL: Monetization Strategy

**Trigger:** When @biz needs to design, evaluate, or optimize how the product generates revenue.

---

## When to Use
- Designing the initial pricing model for a new product.
- Evaluating whether to add a premium tier or change pricing.
- Analyzing freemium-to-paid conversion funnels.
- Planning paywall placement for specific features.
- Conducting pricing sensitivity analysis.
- Quarterly revenue strategy review.

---

## The Monetization Framework

### Step 1: Revenue Model Selection
Evaluate which model(s) fit the product:

| Model | Best For | Pros | Cons | Example |
|---|---|---|---|---|
| **Freemium** | Content/tool apps with viral potential | Large user base, viral growth | Low conversion (2-5%) | Spotify, Notion |
| **Subscription** | Recurring-value products | Predictable revenue, high LTV | Churn management needed | Netflix, Adobe CC |
| **One-time Purchase** | Simple utility apps | Simple, no churn | No recurring revenue, harder to grow | Procreate |
| **Ad-supported** | High-traffic, low willingness to pay | Low friction, mass market | Ad fatigue, low ARPU | Weather apps |
| **Tiered Pricing** | Products with varied user needs | Captures different segments | Complexity, feature cannibalization | Notion, Figma |
| **Usage-based** | API products, infrastructure | Fair pricing, scales with value | Unpredictable revenue | AWS, Twilio |
| **Hybrid** | Mature products | Multiple revenue streams | Operational complexity | YouTube (ads + premium) |

### Step 2: Pricing Analysis
```markdown
## Pricing Analysis — [Product/Feature]
**Date:** YYYY-MM-DD | **Author:** @biz
**Confidence:** [0-100] (Rule `decision-routing.md`)

### Competitor Pricing
| Competitor | Model | Free Tier | Paid Price | Key Paid Features |
|---|---|---|---|---|
| [Comp A] | [Model] | [What's free] | [Price] | [What you pay for] |

### Value-Based Pricing
| Feature | User Value | Willingness to Pay | Paywall? |
|---|---|---|---|
| [Feature] | [High/Med/Low] | [High/Med/Low] | [Free / Premium / Pro] |

### Recommended Pricing
| Tier | Price | Features | Target Segment |
|---|---|---|---|
| Free | $0 | [Core features] | [Mass market] |
| Premium | $X/mo | [Premium features] | [Power users] |
| Pro | $Y/mo | [Pro features] | [Business users] |
```

### Step 3: Paywall Strategy
Define what goes behind the paywall:

**The Paywall Principle:** Free tier must deliver enough value that users *want* more, but not so much that they never upgrade.

| Category | Free | Premium | Rationale |
|---|---|---|---|
| **Core utility** | Full | Full | Core value must be free for growth |
| **Advanced features** | Limited | Full | Upsell trigger |
| **Depth/detail** | Basic | Full | "See more" paywall |
| **Customization** | Default | Full | Personalization upsell |
| **Export/share** | Watermarked | Clean | Social proof + upgrade trigger |
| **Ad-free experience** | Ads | No ads | Quality-of-life upgrade |

### Step 4: Conversion Optimization
Track the freemium-to-paid funnel:

```markdown
## Conversion Funnel — [Product]
| Stage | Metric | Current | Target |
|---|---|---|---|
| Free user → Active user | Activation rate | X% | Y% |
| Active user → Trial/paywall hit | Feature discovery | X% | Y% |
| Paywall hit → Trial start | Trial conversion | X% | Y% |
| Trial → Paid | Payment conversion | X% | Y% |
| Paid → Retained (month 2+) | Paid retention | X% | Y% |
```

### Step 5: Pricing Experiments
Never launch pricing blind. Test first:

| Experiment | Method | Duration | Success Metric |
|---|---|---|---|
| Price point testing | A/B test different prices | 2-4 weeks | Conversion rate × ARPU |
| Tier comparison | A/B test features per tier | 2-4 weeks | Upgrade rate |
| Trial length | A/B test 7-day vs 14-day trial | 4 weeks | Trial-to-paid conversion |
| Paywall placement | A/B test where paywall appears | 2 weeks | Revenue per user |

---

## Rules
- **Free tier must deliver real value.** A useless free tier kills growth.
- **Price based on value, not cost.** Users pay for outcomes, not features.
- **Test before committing.** Every pricing change should be A/B tested if possible.
- **Monitor cannibalization.** If the free tier is too generous, paid conversion drops.
- **Include confidence scores** (Rule `decision-routing.md`) on all pricing recommendations.

## File Management
- Pricing analyses → `.hc/business/financials/pricing/`
- Revenue models → `.hc/business/financials/`
- Experiment plans → `.hc/business/experiments/`
