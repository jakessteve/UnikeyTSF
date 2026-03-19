---
description: Animation Choreography - micro-interaction timing, easing curves, motion design, and a11y-safe animation patterns
---

# SKILL: Animation Choreography

**Trigger:** When @designer plans animations, transitions, or micro-interactions for UI components.

---

## When to Use
- Adding hover/press feedback to interactive elements.
- Designing page or component transitions.
- Creating loading states, skeleton screens, or progress indicators.
- Implementing expand/collapse, modal, or drawer animations.
- Planning staggered list or grid entrance animations.

---

## Animation Principles

| Principle | Rule | Anti-Pattern |
|---|---|---|
| **Purpose** | Every animation MUST serve a functional purpose (feedback, orientation, attention) | Decorative-only animations |
| **Duration** | 150-300ms for micro-interactions, 300-500ms for transitions | >500ms feels sluggish |
| **Easing** | Natural easing (ease-out for entrances, ease-in for exits) | Linear easing (feels robotic) |
| **Restraint** | Less is more — only animate what needs attention | Animating everything |
| **Performance** | Animate `transform` and `opacity` only — never layout properties | Animating `width`, `height`, `margin` |

---

## Standard Timing Reference

| Interaction | Duration | Easing | CSS Token |
|---|---|---|---|
| Button hover/press | 150ms | ease-out | `var(--duration-fast)` |
| Tooltip appear/disappear | 200ms | ease-out | `var(--duration-fast)` |
| Dropdown open | 250ms | ease-out | `var(--duration-normal)` |
| Modal open | 300ms | ease-out | `var(--duration-normal)` |
| Modal close | 200ms | ease-in | `var(--duration-fast)` |
| Page transition | 300ms | ease-in-out | `var(--duration-normal)` |
| Loading skeleton pulse | 1500ms | linear | `var(--duration-slow)` |
| Collapse/expand | 250ms | ease-out | `var(--duration-normal)` |
| Stagger delay (per item) | 50ms | — | Custom |
| Toast notification enter | 300ms | ease-out | `var(--duration-normal)` |
| Toast notification exit | 200ms | ease-in | `var(--duration-fast)` |

---

## CSS Animation Tokens
```css
:root {
  /* Durations */
  --duration-instant: 100ms;
  --duration-fast: 150ms;
  --duration-normal: 250ms;
  --duration-slow: 400ms;
  --duration-glacial: 1000ms;

  /* Easing curves */
  --ease-out: cubic-bezier(0.0, 0.0, 0.2, 1);      /* Entrances */
  --ease-in: cubic-bezier(0.4, 0.0, 1, 1);           /* Exits */
  --ease-in-out: cubic-bezier(0.4, 0.0, 0.2, 1);     /* Transitions */
  --ease-bounce: cubic-bezier(0.34, 1.56, 0.64, 1);   /* Playful feedback */
}
```

## Animation Patterns

### Entrance Pattern (Fade + Slide Up)
```css
.enter {
  animation: fadeSlideUp var(--duration-normal) var(--ease-out) forwards;
}

@keyframes fadeSlideUp {
  from { opacity: 0; transform: translateY(8px); }
  to   { opacity: 1; transform: translateY(0); }
}
```

### Staggered List Entrance
```css
.list-item {
  animation: fadeSlideUp var(--duration-normal) var(--ease-out) both;
}
.list-item:nth-child(1) { animation-delay: 0ms; }
.list-item:nth-child(2) { animation-delay: 50ms; }
.list-item:nth-child(3) { animation-delay: 100ms; }
/* Max 5 items staggered, rest appear together */
```

---

## Accessibility: Reduced Motion
**MANDATORY** — all animations must respect user preferences:

```css
@media (prefers-reduced-motion: reduce) {
  *, *::before, *::after {
    animation-duration: 0.01ms !important;
    animation-iteration-count: 1 !important;
    transition-duration: 0.01ms !important;
  }
}
```

## Rules
- **Respect `prefers-reduced-motion`** — Rule `a11y-standards.md`. This is non-negotiable.
- **Never animate layout properties** (`width`, `height`, `margin`, `padding`) — use `transform` and `opacity`.
- **Test at 0.25x speed** to check for jank (Chrome DevTools → Animations panel).
- **Use design tokens** for all timing values — never hardcode durations (skill `design-token-pipeline`).
- **Max stagger depth: 5 items** — beyond that, batch the rest together.
