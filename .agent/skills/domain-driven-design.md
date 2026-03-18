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
4. Enforce: if code uses `chart` but domain says `lá số`, rename in code.

**Example for Lịch Việt:**
| Term | Vietnamese | Meaning |
|---|---|---|
| Palace | Cung | One of 12 positions in a Tử Vi chart |
| Star | Sao | Celestial influence placed in a palace |
| Four Transformations | Tứ Hóa | Four modifications applied to stars (Lộc, Quyền, Khoa, Kỵ) |
| Day Master | Nhật Chủ | The Heavenly Stem of the Day Pillar in Bát Tự |

### 2. Bounded Contexts
A bounded context is a boundary within which a domain model is consistent.

**Rules:**
- Each context has its own model (same word can mean different things in different contexts).
- Communication between contexts goes through explicit interfaces (anti-corruption layers).
- One team owns one context.

**Example:**
```
┌─────────────────┐  ┌─────────────────┐  ┌─────────────────┐
│  Lunar Calendar  │  │   Tử Vi Engine  │  │  Numerology     │
│  Context         │  │   Context       │  │  Context        │
│                  │  │                 │  │                 │
│ • Solar terms    │  │ • Palaces       │  │ • Life Path     │
│ • Lunar dates    │  │ • Stars         │  │ • Expression    │
│ • Auspicious     │  │ • Tứ Hóa        │  │ • Soul Urge     │
│   activities     │  │ • Interpretation│  │ • Cycles        │
└─────────────────┘  └─────────────────┘  └─────────────────┘
        │                     │                     │
        └─────────────────────┴─────────────────────┘
                    Shared Kernel: Date utilities, Can/Chi
```

### 3. Context Mapping
Define how bounded contexts relate:

| Relationship | Description | Example |
|---|---|---|
| **Shared Kernel** | Shared code both teams use | Date utilities, Can/Chi types |
| **Customer/Supplier** | One context depends on another's output | Tử Vi uses Lunar Calendar dates |
| **Anti-Corruption Layer** | Translate between context models | Convert iztro types to Vietnamese types |
| **Published Language** | Shared format for communication | JSON schemas, TypeScript interfaces |

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
```typescript
// Good: Value object
class LunarDate {
  constructor(readonly day: number, readonly month: number, readonly year: number, readonly leap: boolean) {}
  equals(other: LunarDate) { return this.day === other.day && this.month === other.month && ...; }
}
```

### Entities
Objects with identity that persists across changes:
```typescript
// Entity: identified by ID, mutable state
class TuViChart {
  constructor(readonly id: string, private palaces: Palace[]) {}
}
```

### Domain Events
Record that something happened in the domain:
```typescript
interface ChartGenerated {
  type: 'CHART_GENERATED';
  chartId: string;
  timestamp: Date;
  birthData: BirthData;
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
