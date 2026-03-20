---
description: Observability Standards - structured logging format, required metrics, and alerting
---

# RULE: OBSERVABILITY STANDARDS

**Mode:** Always On
**Scope:** @devops, @dev

---

## Logging Requirements
- All logs MUST be structured JSON (no plain `console.log` in production).
- Required fields: `timestamp`, `level`, `message`, `component`.
- Sensitive data (passwords, tokens, PII) MUST never appear in logs.
- Error logs MUST include stack trace and request context.

## Metrics Requirements
- Every service MUST expose: request rate, error rate, latency percentiles.
- Business-critical operations MUST have custom metrics (e.g., charts generated per hour).
- Resource metrics (CPU, memory) MUST be collected.

## Alerting Requirements
- SEV-1 conditions MUST have automated alerts.
- Alerts MUST have runbooks linked.
- Alert fatigue MUST be managed — only alert on actionable items.

## Rules
- Never deploy without logging configured.
- Never ignore alerts — either fix or tune them.
- Log at the boundary, not in every function.
- Use correlation IDs across request chains.
