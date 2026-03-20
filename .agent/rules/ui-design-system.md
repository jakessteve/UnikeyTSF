---
description: Always On — enforce design token usage and prohibit invented colors, spacing, or typography
---

# RULE: UI DESIGN SYSTEM — Token Enforcement

**Mode:** Always On
**Scope:** All agents writing or modifying UI code (CSS, component markup, styles)

---

## Core Mandate

Before writing **any** visual code, you MUST read the project's design token source to understand existing tokens, components, and patterns. The primary sources are:

1. `src/index.css` — `@theme {}` block for color, spacing, and typography tokens.
2. `tailwind.config.js` or `tailwind.config.ts` — if present, for extended theme configuration.
3. Project-specific design system files in `src/styles/`.

**If you skip reading tokens first → your output is INVALID.**

---

## Binding Constraints

### 1. No Invented Colors
- FORBIDDEN: Raw hex (`#333`, `#f5a623`), raw `rgb()`, or `hsl()` values.
- REQUIRED: Use semantic tokens only — `var(--color-primary)`, `bg-surface-light`, `text-text-primary-dark`, etc.
- **Exception:** Extending the theme in `@theme {}` — new tokens MUST follow the existing naming convention.

### 2. No Magic Spacing
- FORBIDDEN: Arbitrary pixel values (`padding: 13px`, `margin: 7px`, `gap: 22px`).
- REQUIRED: Use the project's spacing scale — TailwindCSS utilities (`p-2`, `p-4`, `gap-4`) or CSS custom properties.

### 3. No Rogue Typography
- FORBIDDEN: Font families or sizes not defined in the design system.
- REQUIRED: Use established typography tokens — `font-display`, `.label-standard`, `.section-title`, or the project's font scale.

### 4. Reuse Before Reinvent
- Before creating a new CSS class, **search `index.css`** for existing primitives:
 - Surfaces: `.card-surface`, `.card-subtle`, `.card-highlight`
 - Shadows: `.shadow-apple`, `.shadow-apple-hover`
 - Animations: `.animate-fade-in-up`, `.loading-shimmer`
- If a new class IS needed, follow the existing naming convention (e.g., `card-*, feature-*, component-*`).

### 5. Dark Mode Parity
- Every light-mode visual style MUST have a corresponding `.dark` variant.
- Use the existing dark token pairs: `bg-surface-light` ↔ `bg-surface-dark`, etc.

### 6. CSS Placement
- Design tokens → `@theme {}` block
- Base resets → `@layer base {}`
- Reusable components → `@layer components {}`
- One-off overrides → inline or scoped styles (last resort)

---

## Enforcement
Any UI output that violates these constraints should be flagged as ` DESIGN SYSTEM VIOLATION` and corrected before delivery.
