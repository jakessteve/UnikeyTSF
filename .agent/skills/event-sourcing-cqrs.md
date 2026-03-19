---
description: Event Sourcing & CQRS - event-driven state management, audit trails, and command/query separation patterns
---

# SKILL: Event Sourcing & CQRS

**Trigger:** When @sa designs systems needing full audit trails, temporal queries, or complex state management.

---

## When to Use

| Scenario | Use Event Sourcing? | Use CQRS? |
|---|---|---|
| Full audit trail required | Yes | Optional |
| Need "state at time X" queries | Yes | Yes (temporal projections) |
| Complex business rules with many transitions | Yes | Yes |
| Undo/redo functionality | Yes | Optional |
| Read/write workloads differ significantly | Optional | Yes |
| Simple CRUD, no audit needs | No | No |
| Team unfamiliar with eventual consistency | No | With caution |

---

## Event Sourcing

### Core Concepts
- **Event:** An immutable record of something that happened. Named in past tense (`OrderPlaced`, `ChartGenerated`).
- **Event Store:** Append-only log of events. Never delete or modify.
- **Aggregate:** A consistency boundary that produces events.
- **Projection:** A read model built by replaying events from the store.
- **Snapshot:** Periodic state capture to avoid replaying the entire event history.

### Event Design Checklist
```typescript
// Good event: self-contained, past tense, includes all relevant data
interface ChartGenerated {
 type: 'ChartGenerated';
 timestamp: Date;
 userId: string;
 chartType: 'typeA' | 'typeB' | 'typeC';
 inputData: BirthData;
 resultSummary: string;
 engineVersion: string; // For reproducibility
}
```

| Principle | Rule |
|---|---|
| **Immutability** | Never modify or delete events |
| **Self-contained** | Include all relevant data in the event (avoid enrichment from external sources) |
| **Past tense naming** | `OrderPlaced`, not `PlaceOrder` |
| **Versioning** | Add `schemaVersion` field for schema evolution |
| **Causality** | Include `correlationId` and `causationId` for event chains |

### Event Store Implementation Pattern
```typescript
interface EventStore {
 append(streamId: string, events: DomainEvent[], expectedVersion: number): Promise<void>;
 read(streamId: string, fromVersion?: number): Promise<DomainEvent[]>;
 readAll(fromPosition?: number): Promise<DomainEvent[]>; // For projections
}
```

### Snapshotting Strategy
| Aggregate Size | Snapshot Frequency |
|---|---|
| < 100 events | No snapshot needed |
| 100-1000 events | Every 100 events |
| > 1000 events | Every 50 events + consider archiving |

---

## CQRS (Command Query Responsibility Segregation)

### Architecture
```
Commands → [Command Handler] → [Aggregate] → [Event Store]
 ↓
 [Event Bus]
 ↓
Queries ← [Read Model/Projection] ← [Projection Handler]
```

### Command Side
- Validates business rules.
- Produces domain events.
- Writes to the event store.
- Returns success/failure only (not data).

### Query Side
- Reads from optimized projections (denormalized read models).
- Can have multiple projections for different query needs.
- Eventually consistent with the command side.

### Projection Patterns
| Pattern | When to Use |
|---|---|
| **Synchronous** | Low throughput, strong consistency needed |
| **Async via queue** | High throughput, eventual consistency acceptable |
| **Catch-up subscription** | Rebuilding projections from event history |

---

## Decision Record Template
When choosing to adopt Event Sourcing / CQRS, document the decision using the `architecture-decision-records` skill:

```markdown
# ADR: Event Sourcing for [System Component]
**Justification:** [Why ES/CQRS over simple CRUD?]
**Consistency model:** [Strong / Eventual — and why acceptable]
**Snapshot strategy:** [When and how to snapshot]
**Migration plan:** [How to migrate existing state to events]
```

## Rules
- Events are **immutable** — corrections are new compensating events, not edits.
- Design events for the **business domain**, not the data model.
- Projections are **disposable** — you should be able to rebuild any projection from events.
- Consider **event versioning** early — schema evolution is hard to retrofit.
- **Start simple** — CQRS without Event Sourcing is valid; don't add complexity you don't need.
