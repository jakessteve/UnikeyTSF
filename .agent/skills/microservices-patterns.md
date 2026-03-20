---
description: Microservices Patterns - service decomposition, communication, saga, circuit breaker, and anti-patterns
---

# SKILL: Microservices Patterns

**Trigger:** When @sa designs distributed systems, decomposes monoliths, or evaluates service boundaries.

---

## When to Use
- Designing a new system with multiple independent concerns.
- Decomposing a growing monolith into services.
- Choosing communication patterns between services.
- Implementing distributed transactions or resilience.
- Evaluating whether microservices are even the right choice.

---

## Decision: Monolith vs. Microservices

| Factor | Monolith | Microservices |
|---|---|---|
| Team size | 1-5 developers | 5+ per service team |
| Domain complexity | Simple, well-understood | Complex, multiple bounded contexts |
| Deployment frequency | Same cadence for all features | Different features ship independently |
| Scaling needs | Uniform load across features | Specific services need different scaling |
| Development maturity | Early stage, rapid iteration | Mature, stable domain model |

> **For a solo developer / small team:** Start with a well-structured monolith. Extract to services ONLY when specific scaling or deployment pressures arise (Strangler Fig pattern).

---

## Service Decomposition Strategies

| Strategy | When | Example | Risk |
|---|---|---|---|
| **By business capability** | Clear domain boundaries | Cart ↔ Checkout ↔ Inventory | Over-decomposition |
| **By subdomain (DDD)** | Complex domain model | Core ↔ Supporting ↔ Generic | Requires DDD expertise |
| **Strangler fig** | Migrating from monolith | Replace one module at a time | Long migration period |
| **Event-driven extraction** | Loosely coupled concerns | Analytics ↔ Notification ↔ Core | Debugging complexity |

---

## Communication Patterns

| Pattern | Use For | Pros | Cons |
|---|---|---|---|
| **Sync REST** | Simple CRUD, real-time queries | Easy to understand, tooling mature | Tight coupling, cascading failures |
| **Sync gRPC** | Internal service-to-service, performance | Fast, type-safe, streaming | Learning curve, browser support |
| **Async (Message Queue)** | Event-driven, eventual consistency | Loose coupling, resilience | Debugging difficulty, eventual consistency |
| **Event Sourcing** | Audit trail, replay, temporal queries | Full history, replayable | Storage cost, query complexity |

**Selection guide:**
- User-facing, needs instant response → Synchronous (REST/gRPC)
- Background processing, can tolerate delay → Asynchronous (Queue)
- Need audit trail → Event Sourcing (see `event-sourcing-cqrs` skill)

---

## Resilience Patterns

### Circuit Breaker
```
CLOSED → [failures > threshold] → OPEN → [timeout expires] → HALF-OPEN
 ↑ ↓
 └── [failure] ←────────────────┘
 [success] → CLOSED
```
- **Closed:** Normal operation, requests pass through.
- **Open:** Requests fail immediately (return fallback), no calls to downstream.
- **Half-Open:** One test request allowed; if succeeds → close, if fails → re-open.

### Other Resilience Patterns
| Pattern | Implementation | When |
|---|---|---|
| **Retry with backoff** | Exponential delay + jitter | Transient failures (network blips) |
| **Bulkhead** | Separate thread pools per service | Prevent cascading failures |
| **Timeout** | Explicit timeout on all external calls | No indefinite waits (max 5s) |
| **Fallback** | Return cached/default data on failure | Graceful degradation |

---

## Saga Pattern (Distributed Transactions)

| Approach | How It Works | Pros | Cons |
|---|---|---|---|
| **Choreography** | Each service publishes events; next reacts | Simple, no coordinator | Hard to track, debug |
| **Orchestration** | Central coordinator directs the flow | Clear flow, easy to monitor | Single point of coordination |

**Key rule:** Every saga step MUST have a **compensating action** for rollback.

```
CreateOrder → ReserveInventory → ChargePayment → ConfirmOrder
 ↓ FAIL
 UndoReserve ← RefundPayment ← CancelOrder
```

---

## Anti-Patterns to Avoid

| Anti-Pattern | Symptom | Fix |
|---|---|---|
| Distributed monolith | Services must deploy together | Re-evaluate boundaries, reduce coupling |
| Shared database | Multiple services write to same DB | Database-per-service, API contracts |
| Sync chain >3 deep | A calls B calls C calls D | Introduce async processing, CQRS |
| No monitoring | Can't tell which service is failing | Distributed tracing (OpenTelemetry) |
| God service | One service handles everything | Decompose using bounded contexts |

## Rules
- **Start with a monolith** unless you have a strong reason not to.
- **One database per service** — shared databases defeat the purpose.
- **Design for failure** — every network call can fail; plan for it.
- **Distributed tracing is mandatory** for any multi-service system.
- **Document service boundaries** using `c4-architecture` skill.
