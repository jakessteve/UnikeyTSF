---
description: Domain-Driven Design - strategic DDD for bounded contexts, ubiquitous language, and context mapping
---

# SKILL: Domain-Driven Design (DDD)

**Trigger:** When @sa designs complex domain models, decomposes systems into bounded contexts, or establishes ubiquitous language.

---

## When to Use
- Designing a new system or major subsystem.
- Decomposing a monolith into modules or services.
- Resolving ambiguity in domain terms across teams.
- Establishing clear module boundaries.

---

## Strategic DDD

### 1. Ubiquitous Language
The team MUST use consistent domain terms everywhere — code, docs, conversations.

**How to establish:**
1. List all domain terms used in the project.
2. Define each term precisely (one definition, no synonyms in code).
3. Document in a glossary (`GLOSSARY.md` or `docs/tech/ARCHITECTURE.md`).
4. Enforce: if code uses one term but the domain uses another, rename in code.

**Example:**
| Term | Domain Meaning |
|---|---|
| _[Term 1]_ | _[Precise definition in this project's domain]_ |
| _[Term 2]_ | _[Precise definition in this project's domain]_ |
| _[Term 3]_ | _[Precise definition in this project's domain]_ |

### 2. Bounded Contexts
A bounded context is a boundary within which a domain model is consistent.

**Rules:**
- Each context has its own model (same word can mean different things in different contexts).
- Communication between contexts goes through explicit interfaces (anti-corruption layers).
- One team owns one context.

**Example:**
```
┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
│  Context A       │  │  Context B       │  │  Context C       │
│                 │  │                 │  │                 │
│ • [Concept 1]   │  │ • [Concept 4]   │  │ • [Concept 7]   │
│ • [Concept 2]   │  │ • [Concept 5]   │  │ • [Concept 8]   │
│ • [Concept 3]   │  │ • [Concept 6]   │  │ • [Concept 9]   │
└─────────────────┘  └─────────────────┘  └─────────────────┘
        │                     │                     │
        └─────────────────────┴─────────────────────┘
                    Shared Kernel: [shared utilities]
```

### 3. Context Mapping
Define how bounded contexts relate:

| Relationship | Description | Example |
|---|---|---|
| **Shared Kernel** | Shared code both teams use | Common data types, utility functions |
| **Customer/Supplier** | One context depends on another's output | Service A consumes Service B's data |
| **Anti-Corruption Layer** | Translate between context models | Convert external API types to internal types |
| **Published Language** | Shared format for communication | JSON schemas, TypeScript interfaces, Protobuf |

### 4. Aggregates
An aggregate is a cluster of domain objects treated as a single unit.

**Rules:**
- Each aggregate has one root entity.
- External references only point to the root.
- Changes within an aggregate are transactional.
- Keep aggregates small (prefer smaller, more focused aggregates).

---

## Tactical DDD Patterns

### Value Objects
Immutable objects defined by their attributes, not identity:
```
// Good: Value object — immutable, compared by value
class DateRange {
  constructor(readonly start: Date, readonly end: Date) {}
  equals(other: DateRange) { return this.start === other.start && this.end === other.end; }
}
```

### Entities
Objects with identity that persists across changes:
```
// Entity: identified by ID, mutable state
class Order {
  constructor(readonly id: string, private items: OrderItem[]) {}
}
```

### Domain Events
Record that something happened in the domain:
```
interface OrderPlaced {
  type: 'ORDER_PLACED';
  orderId: string;
  timestamp: Date;
  customerData: CustomerData;
}
```

---

## DDD Review Checklist
- [ ] All domain terms documented in glossary
- [ ] Bounded contexts clearly identified and mapped
- [ ] No model leaks across context boundaries
- [ ] Aggregates are small and focused
- [ ] Value objects are immutable
- [ ] Domain events captured for significant state changes
