---
description: TypeScript Expert - advanced TS patterns including discriminated unions, type guards, generics, and strict typing
---

# SKILL: TypeScript Expert

**Trigger:** When @dev needs advanced TypeScript patterns, strict typing, or type-safe design.

---

## When to Use
- Designing type-safe interfaces for engines and data transformations.
- Replacing `any` or `as` casts with proper type narrowing.
- Creating shared types between modules.
- Implementing generic utilities or type-safe collections.
- Reviewing code for type safety violations (Rule `code-standards.md`).

---

## Pattern Reference

### 1. Discriminated Unions (for variant types)
Use when a value can be one of several shapes:
```typescript
type EngineResult =
 | { kind: 'success'; data: ChartData; timing_ms: number }
 | { kind: 'error'; error: string; code: number }
 | { kind: 'pending'; progress: number };

function handleResult(result: EngineResult) {
 switch (result.kind) {
 case 'success': return renderChart(result.data);
 case 'error': return showError(result.error);
 case 'pending': return showProgress(result.progress);
 // TypeScript ensures all cases handled
 }
}
```

### 2. Type Guards (runtime narrowing)
```typescript
// User-defined type guard
function isValidDate(date: unknown): date is AppDate {
 return typeof date === 'object' && date !== null
 && 'day' in date && 'month' in date;
}

// Use with discriminated unions
function isDetailedResult(result: ResultData.: chart is DetailedResult {
 return result.type === 'detailed';
}
```

### 3. Generics with Constraints
```typescript
// Constrained generic — K must be a key of T
function getProperty<T, K extends keyof T>(obj: T, key: K): T[K] {
 return obj[key];
}

// Generic with default
function createEngine<TResult = ChartData>(config: EngineConfig): Engine<TResult> {
 // ...
}

// Conditional types
type ApiResponse<T> = T extends string ? TextResponse : JsonResponse<T>;
```

### 4. Utility Types Cheat Sheet

| Type | Purpose | Use Case | Example |
|---|---|---|---|
| `Partial<T>` | All props optional | Patch/update payloads | `updateUser(Partial<User>)` |
| `Required<T>` | All props required | Validated input | `createUser(Required<UserInput>)` |
| `Pick<T, K>` | Subset of props | API response shapes | `Pick<User, 'id' \| 'name'>` |
| `Omit<T, K>` | Exclude props | Create without ID | `Omit<User, 'id'>` |
| `Record<K, V>` | Map type | Lookup tables | `Record<EntityId, Entity>` |
| `Readonly<T>` | Immutable | Config, constants | `Readonly<EngineConfig>` |
| `Extract<T, U>` | Filter union members | Narrow type | `Extract<Event, { kind: 'click' }>` |
| `Exclude<T, U>` | Remove union members | Remove type | `Exclude<Status, 'deleted'>` |
| `NonNullable<T>` | Remove null/undefined | After validation | `NonNullable<string \| null>` |
| `ReturnType<F>` | Get function return type | Infer types | `ReturnType<typeof calc>` |

### 5. Branded Types (Nominal Typing)
Prevent accidentally swapping values of the same underlying type:
```typescript
type UserId = string & { readonly __brand: 'UserId' };
type OrderId = string & { readonly __brand: 'OrderId' };

function createUserId(id: string): UserId { return id as UserId; }
function createOrderId(id: string): OrderId { return id as OrderId; }

// Now these are NOT interchangeable — TypeScript will error
function getUser(id: UserId): User { /* ... */ }
getUser(createOrderId('order-1')); // TS Error!
```

### 6. Exhaustive Checks
Ensure all cases in a union are handled at compile time:
```typescript
function assertNever(x: never): never {
 throw new Error(`Unhandled case: ${x}`);
}

// Usage in switch — if you add a new variant, TypeScript will error here
switch (result.kind) {
 case 'success': /* ... */ break;
 case 'error': /* ... */ break;
 default: assertNever(result); // Compile error if 'pending' not handled
}
```

### 7. Template Literal Types
```typescript
type EventName = `${string}_${'clicked' | 'viewed' | 'generated'}`;
type CSSProperty = `--${string}`;
```

---

## Decision Matrix: Which Pattern to Use

| Problem | Pattern |
|---|---|
| Value can be one of several shapes | Discriminated union |
| Need to narrow `unknown` at runtime | Type guard |
| Same logic, different types | Generics |
| Prevent swapping same-type IDs | Branded types |
| Ensure all cases handled in switch | Exhaustive check (`assertNever`) |
| Dynamic object keys | `Record<K, V>` or index signature |
| Optional fields in update payload | `Partial<T>` |
| Immutable configuration | `Readonly<T>` + `as const` |

## Rules
- **Never use `any`** — use `unknown` and narrow with type guards.
- **Never use `as` casts** unless absolutely necessary — prefer type guards or generics.
- **Enable `strict: true`** in `tsconfig.json` (non-negotiable).
- **Use `readonly`** for data that shouldn't be mutated.
- **Prefer interfaces** for object shapes, **types** for unions/intersections.
- **Export types** from a shared `types.ts` per module — avoid inline type definitions.
