---
description: Refactoring Patterns - catalog of safe code transformation moves with preconditions
---

# SKILL: Refactoring Patterns

**Trigger:** When @dev improves code structure without changing external behavior.

---

## When to Use
- Code smells identified during code review.
- Before adding new features to complex code.
- After a bug fix reveals structural issues.
- When test coverage is sufficient to refactor safely.

---

## Pre-Refactoring Checklist
- [ ] Tests exist and pass for the code being refactored
- [ ] You understand what the code does (read it fully first)
- [ ] You have a clear goal (what should be better after?)
- [ ] Changes are committed before starting (easy rollback)

---

## Safe Refactoring Moves

### Extract Function
**When:** A block of code does one identifiable thing within a larger function.
```typescript
// Before
function processOrder(order: Order) {
  // validate
  if (!order.items.length) throw new Error('Empty order');
  if (!order.customer) throw new Error('No customer');
  // ... 50 more lines
}

// After
function validateOrder(order: Order) {
  if (!order.items.length) throw new Error('Empty order');
  if (!order.customer) throw new Error('No customer');
}
function processOrder(order: Order) {
  validateOrder(order);
  // ... remaining logic
}
```

### Extract Constant
**When:** Magic numbers or strings appear in code.
```typescript
// Before
if (retryCount > 3) { ... }

// After
const MAX_RETRY_ATTEMPTS = 3;
if (retryCount > MAX_RETRY_ATTEMPTS) { ... }
```

### Replace Conditional with Polymorphism
**When:** Switch statements or if-else chains select behavior based on type.
```typescript
// Before
function getArea(shape: Shape) {
  switch (shape.type) {
    case 'circle': return Math.PI * shape.radius ** 2;
    case 'rectangle': return shape.width * shape.height;
  }
}

// After — each shape knows how to calculate its own area
interface Shape { getArea(): number; }
class Circle implements Shape { getArea() { return Math.PI * this.radius ** 2; } }
```

### Consolidate Duplicate Code
**When:** Similar code appears in multiple places.
1. Identify the common pattern.
2. Extract into a shared utility function.
3. Replace all duplicates with the utility call.
4. Verify all callers still work.

### Rename for Clarity
**When:** Names don't clearly communicate purpose.
- Functions: verb phrases (`calculateTotal`, not `calc`)
- Variables: descriptive nouns (`userAge`, not `x`)
- Booleans: question form (`isValid`, `hasPermission`)
- Constants: SCREAMING_SNAKE_CASE

### Simplify Conditionals
**When:** Complex boolean expressions hurt readability.
```typescript
// Before
if (user.age >= 18 && user.isVerified && !user.isBanned && user.subscription !== 'expired') { ... }

// After
const canAccess = user.age >= 18 && user.isVerified && !user.isBanned && user.subscription !== 'expired';
if (canAccess) { ... }
// Or better: extract to a method
if (user.canAccessPremiumContent()) { ... }
```

---

## Post-Refactoring Checklist
- [ ] All existing tests still pass (no behavior change)
- [ ] Code is measurably better (fewer lines, clearer names, less duplication)
- [ ] No new `any` types introduced
- [ ] TypeScript compilation still clean (`tsc --noEmit`)
