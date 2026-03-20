---
description: Launch Strategy - pre-launch checklist, channel strategy, and launch day timeline for feature releases
---

# SKILL: Launch Strategy

**Trigger:** When @pm is preparing to release a new feature, version, or product to users. Triggered by @pm directly or as part of the `/go-to-market` workflow.

---

## When to Use
- New feature launch (any feature significant enough to announce).
- Version release (major or minor with user-facing changes).
- Product launch (first public release / Product Hunt launch).
- Re-launch or pivot announcement.

---

## The Launch Framework: 3 Phases

### Phase 1: Pre-Launch (T-7 to T-1 Days)

#### 1.1 Launch Readiness Checklist
```markdown
## Launch Readiness Checklist — [Feature/Version]
**Target Date:** [YYYY-MM-DD]
**Launch Type:** Soft / Public / Product Hunt

### Product
- [ ] Feature complete and QC verified (`verification-before-completion`)
- [ ] Battle-tested (`/battle-test` score ≥ B)
- [ ] User-tested (`/user-test-session` score acceptable)
- [ ] Production build passes (`npm run build`)
- [ ] Deployment pipeline ready (@devops verified)
- [ ] Rollback plan documented

### Content (via `content-marketing` skill)
- [ ] Blog post draft written
- [ ] Social thread drafted
- [ ] Changelog entry written
- [ ] Release notes authored
- [ ] Screenshots/GIFs captured for social

### SEO (via `seo-copywriting` skill)
- [ ] Landing page copy optimized
- [ ] Meta tags set
- [ ] OG tags configured (title, description, image)

### Distribution
- [ ] Email draft ready (if applicable)
- [ ] Product Hunt ship page created (if applicable)
- [ ] Community posts drafted (Dev.to, Reddit, forums)
```

#### 1.2 Channel Strategy Matrix

| Channel | Audience | Content Type | Timing | Priority |
|---|---|---|---|---|
| **Blog** | Existing users, SEO | Deep-dive post | Launch day | Must |
| **X/Twitter** | Developer community | Thread + GIF | Launch day | Must |
| **Product Hunt** | Tech early adopters | Ship page | Launch day (specific time) | If major |
| **Email** | Existing user base | Announcement | Launch day | Must |
| **LinkedIn** | Professional network | Brief post | Launch day +1 | Optional |
| **Dev.to / Medium** | Developer community | Technical tutorial | Launch day +2-3 | Optional |
| **Reddit** | Niche communities | Soft mention | Launch day +1 | If relevant |
| **Vietnamese forums** | Target users | Product announcement | Launch day | Must |

### Phase 2: Launch Day (T-0)

#### 2.1 Launch Day Timeline
```markdown
## Launch Day Timeline
**Date:** [YYYY-MM-DD]

| Time | Action | Owner | Status |
|---|---|---|---|
| 08:00 | Deploy to production | @devops | [ ] |
| 08:30 | Smoke test production | @qc | [ ] |
| 09:00 | Publish blog post | @ba | [ ] |
| 09:15 | Post social media thread | @ba | [ ] |
| 09:30 | Send email announcement | @ba | [ ] |
| 10:00 | Submit to Product Hunt (if applicable) | @pm | [ ] |
| 12:00 | Monitor — check for errors, user feedback | @devops | [ ] |
| 18:00 | Post to secondary channels | @ba | [ ] |
| 23:00 | Day-1 metrics snapshot | @pm | [ ] |
```

### Phase 3: Post-Launch (T+1 to T+7)

#### 3.1 Post-Launch Actions
1. **Monitor metrics** (via `growth-metrics` skill):
 - Page views, sign-ups, feature adoption, bounce rate
 - Error rates, performance metrics
2. **Respond to feedback:**
 - User comments, tweets, Product Hunt questions
 - Bug reports → fast-track via @dev
3. **Iterate content:**
 - Update blog post based on user questions
 - Create FAQ if patterns emerge
4. **Capture learnings:**
 - What worked? What didn't? Document in `.hc/retrospectives/`
 - Feed insights back to the next launch

#### 3.2 Post-Launch Metrics Template
```markdown
## Post-Launch Report — [Feature/Version]
**Launch Date:** [YYYY-MM-DD]

### Reach
| Channel | Impressions | Clicks | Engagement Rate |
|---|---|---|---|
| Blog | | | |
| X/Twitter | | | |
| Email | | | |

### Conversion
| Metric | Target | Actual |
|---|---|---|
| New users (day 1) | | |
| Feature adoption | | |
| Bounce rate | | |

### Key Learnings
1. [What worked]
2. [What didn't]
3. [What to do differently next time]
```

---

## Rules
- **Never launch without QC verification.** Feature must pass `verification-before-completion`.
- **Always have a rollback plan.** @devops documents how to revert if launch goes wrong.
- **Launch ≠ Done.** Post-launch monitoring for 7 days is mandatory.
- **Coordinate timing.** Don't launch on Fridays or during holidays (no one is around if things break).

## File Management
- Launch plans → `.hc/launches/`
- Post-launch reports → `.hc/launches/reports/`
