---
description: DevOps Operations -- incident response, postmortem writing, SLO implementation, and observability. Unified reference for production reliability.
---

# SKILL: DEVOPS OPERATIONS

**Trigger:** Production incidents, post-incident analysis, reliability target setting, monitoring and alerting setup.

> Consolidates: incident-responder, postmortem-writing, slo-implementation, observability-engineer

---

## 1. Incident Response

### Severity Levels
| Level | Impact | Response Time |
|---|---|---|
| P1 Critical | Service down, data loss risk | Immediate |
| P2 Major | Feature broken, workaround exists | < 1 hour |
| P3 Minor | Degraded experience | < 4 hours |
| P4 Low | Cosmetic, non-blocking | Next sprint |

### Response Protocol
1. **Detect** -- Alerting fires or user report
2. **Triage** -- Classify severity, assign owner
3. **Mitigate** -- Restore service (rollback, feature flag, hotfix)
4. **Communicate** -- Status page update, stakeholder notification
5. **Resolve** -- Root cause fix deployed
6. **Review** -- Post-incident review within 48 hours

---

## 2. Postmortem Writing

### Template
```markdown
## Incident: [Title]
**Date:** [YYYY-MM-DD] | **Duration:** [X hours] | **Severity:** [P1-P4]

### Timeline
| Time | Event |
|------|-------|
| HH:MM | [What happened] |

### Root Cause
[Technical explanation of the underlying cause]

### Impact
- Users affected: [count/percentage]
- Revenue impact: [if applicable]
- Data impact: [if applicable]

### What Went Well
- [Things that worked in the response]

### What Went Wrong
- [Things that failed or were slow]

### Action Items
| Action | Owner | Due Date | Priority |
|--------|-------|----------|----------|
```

### Rules
- **Blameless.** Focus on systems, not individuals.
- **Specific.** "Deploy pipeline lacked rollback" not "process was bad."
- **Actionable.** Every finding gets an action item with an owner.

---

## 3. SLO Implementation

### Define SLIs (Service Level Indicators)
| Category | SLI Example |
|---|---|
| Availability | % of successful requests (HTTP 2xx/3xx) |
| Latency | p50, p95, p99 response times |
| Correctness | % of responses with correct data |
| Freshness | Age of most recent data |

### Set SLOs
- Start conservative (e.g., 99.5% availability)
- Error budget = 100% - SLO (e.g., 0.5% = ~3.6 hours/month)
- When error budget exhausted: freeze features, focus on reliability

---

## 4. Observability

### Three Pillars
| Pillar | Tool Examples | Use For |
|---|---|---|
| **Logs** | Structured JSON logging | Debugging, audit trail |
| **Metrics** | Counters, histograms, gauges | Dashboards, alerting |
| **Traces** | Distributed tracing (spans) | Request flow, latency |

### Alerting Rules
- Alert on **symptoms** (user-facing), not causes (CPU usage)
- Every alert must have a **runbook** link
- Reduce noise: group related alerts, set appropriate thresholds
