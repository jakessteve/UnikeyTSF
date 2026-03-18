---
description: Analytics and Feedback -- tracking setup, telemetry analysis, user feedback routing, and growth metrics. Unified reference for measurement and optimization.
---

# SKILL: ANALYTICS AND FEEDBACK

**Trigger:** Analytics implementation, user behavior analysis, feedback collection, growth metric tracking, A/B testing.

> Consolidates: analytics-tracking, telemetry-analysis, user-feedback-loop, growth-metrics

---

## 1. Analytics Tracking Setup

### Implementation Checklist
| Step | Action |
|---|---|
| Define events | Map user actions to named events with properties |
| Add tracking code | Implement via SDK or tag manager |
| Verify firing | Test each event in debug/preview mode |
| Document | Maintain event catalog with name, trigger, properties |

### Event Naming Convention
`[object]_[action]` -- e.g., `page_view`, `button_click`, `form_submit`, `feature_used`

### Privacy Requirements
- Consent before tracking (GDPR/CCPA if applicable)
- No PII in event properties
- Anonymize user IDs where possible

---

## 2. Growth Metrics

### Core Metrics Framework
| Metric | Formula | Cadence |
|---|---|---|
| **DAU/MAU** | Daily/Monthly active users | Daily |
| **Retention** | % returning after N days | Weekly |
| **Activation** | % completing key action | Weekly |
| **Churn** | % stopped using in period | Monthly |
| **NPS** | Promoters - Detractors | Quarterly |

### Funnel Analysis
Track conversion at each step: Awareness -> Acquisition -> Activation -> Retention -> Revenue -> Referral

---

## 3. Telemetry Analysis

### When to Analyze
- After feature launch (1-day, 7-day, 30-day)
- When metrics show unexpected change
- Before major product decisions

### Analysis Framework
1. **What happened?** -- Describe the metric change
2. **Why?** -- Identify causal factors (release, campaign, seasonal)
3. **So what?** -- Business impact assessment
4. **Now what?** -- Recommended action

---

## 4. Feedback Routing

### Collection Channels
| Channel | Type | Processing |
|---|---|---|
| In-app surveys | Quantitative + qualitative | Auto-categorize, flag P1 issues |
| App store reviews | Public qualitative | Monitor daily, respond to 1-2 star |
| Support tickets | Problem-focused | Tag by feature area |
| User interviews | Deep qualitative | Synthesize themes |

### Feedback-to-Action Pipeline
Collect -> Categorize -> Prioritize -> Route to owner -> Track resolution -> Close loop with user
