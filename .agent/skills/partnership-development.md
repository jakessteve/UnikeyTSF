---
description: Partnership Development - partner identification, evaluation, co-marketing, API partnerships, and integration strategy
---

# SKILL: Partnership Development

**Trigger:** When @biz needs to identify, evaluate, or pursue strategic partnerships — co-marketing, API integrations, distribution deals, or content partnerships.

---

## When to Use
- Identifying potential partners who serve the same audience.
- Evaluating whether a partnership opportunity is worth pursuing.
- Drafting partnership outreach and proposals (use `business-writing` skill for the actual documents).
- Designing API or integration partnerships.
- Planning co-marketing campaigns with partners.

---

## The Partnership Development Framework

### Step 1: Partner Identification
Identify potential partners by category:

| Partnership Type | What They Provide | What We Provide | Example |
|---|---|---|---|
| **Distribution** | Access to their user base | Content/tool for their platform | App stores, portals |
| **Co-marketing** | Shared promotion, cross-audience | Shared promotion, cross-audience | Blog swaps, joint webinars |
| **API / Integration** | Technical integration, data | Technical integration, features | Calendar app + our data |
| **Content** | Guest posts, shared authority | Guest posts, shared authority | Domain expert blogs |
| **Influencer/KOL** | Audience reach, social proof | Product access, affiliate revenue | Vietnamese content creators |
| **White-label** | Distribution under their brand | Product/engine they rebrand | Enterprise clients |

### Step 2: Partner Evaluation Matrix
Score potential partners before pursuing:

```markdown
## Partner Evaluation — [Partner Name]
**Date:** YYYY-MM-DD | **Evaluator:** @biz
**Confidence:** [0-100] (Rule `decision-routing.md`)

| Criterion | Score (1-10) | Notes |
|---|---|---|
| **Audience overlap** | | [How much their users = our users?] |
| **Brand alignment** | | [Does their brand complement ours?] |
| **Reach / scale** | | [How big is their audience?] |
| **Technical feasibility** | | [How easy is integration?] |
| **Revenue potential** | | [Can this generate revenue?] |
| **Effort required** | | [10 = low effort, 1 = massive effort] |
| **Strategic value** | | [Beyond revenue — brand, positioning, data?] |
| **TOTAL** | [sum/70] | |

**Threshold:** Score ≥ 50/70 → Pursue. 35-49 → Investigate further. < 35 → Pass.
```

### Step 3: Partnership Proposal
Use the `business-writing` skill to draft the proposal:
1. **Research the partner** — understand their product, audience, and strategy.
2. **Lead with their benefit** — what do THEY gain from this partnership?
3. **Propose a specific structure** — not vague "let's collaborate."
4. **Include success metrics** — how both parties will measure ROI.

### Step 4: Partnership Management
After a partnership is established:

```markdown
## Active Partnership — [Partner Name]
**Start Date:** YYYY-MM-DD | **Type:** [Co-marketing / API / Distribution]
**Owner:** @biz | **Status:** Active / On Hold / Ended

### Deliverables
| # | Deliverable | Owner | Due | Status |
|---|---|---|---|---|
| 1 | [Item] | [Us/Them] | [Date] | [ ] |

### Metrics
| Metric | Target | Actual | Status |
|---|---|---|---|
| [Referral traffic] | [X/month] | | |
| [Conversion from partner] | [X%] | | |

### Review Cadence
- Monthly metrics check
- Quarterly partnership health review
```

---

## API Partnership Considerations
When evaluating technical integrations:

| Factor | Question | Red Flag |
|---|---|---|
| **API maturity** | Is their API stable and well-documented? | Undocumented, no versioning |
| **Data privacy** | Does data sharing comply with GDPR/privacy? | PII exposure, no DPA |
| **Maintenance burden** | Who maintains the integration? | No SLA, no support |
| **User value** | Does this integration actually help users? | Nice-to-have, not core |

---

## Rules
- **Lead with partner value, not ours.** The pitch is about them, not us.
- **Document everything.** Every partnership gets a tracking file.
- **Quarterly review.** Active partnerships are reviewed every quarter.
- **Legal review.** Any agreement involving data sharing or revenue → legal review (flag to @pm).
- **Never overcommit.** Promise what the team can deliver. Check with @pm before committing engineering resources.

## File Management
- Partner evaluations → `.hc/business/partnerships/evaluations/`
- Active partnerships → `.hc/business/partnerships/active/`
- Partnership proposals → `.hc/business/partnerships/proposals/`
- Partner pipeline → `.hc/business/partnerships/pipeline.md`
