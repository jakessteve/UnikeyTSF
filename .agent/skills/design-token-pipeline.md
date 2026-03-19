---
description: Design Token Pipeline — 3-layer token architecture, component specs, Tailwind v4 integration, and automated token management
---

# SKILL: Design Token Pipeline

**Trigger:** When @designer establishes or updates design tokens (colors, typography, spacing, shadows) or when ensuring consistency across light/dark themes.

---

## When to Use
- Setting up a new design system from scratch.
- Adding a new color, font, or spacing value to the system.
- Auditing UI consistency (are all components using tokens?).
- Implementing dark mode / theme switching.
- Syncing tokens between design tools and code.
- Creating component state definitions (default/hover/active/disabled).
- Configuring Tailwind v4 `@theme` directive with design tokens.
- Design-to-code handoff (translating Figma tokens to CSS variables).

---

## The Token Hierarchy

### Layer 1: Primitive Tokens (Raw Values)
The lowest level — raw values with no semantic meaning:
```css
:root {
  /* Color primitives */
  --hsl-blue-500: 220, 90%, 56%;
  --hsl-blue-700: 220, 90%, 36%;
  --hsl-amber-500: 38, 92%, 50%;

  /* Size primitives */
  --size-1: 0.25rem;  /* 4px */
  --size-2: 0.5rem;   /* 8px */
  --size-4: 1rem;     /* 16px */
  --size-8: 2rem;     /* 32px */
}
```

### Layer 2: Semantic Tokens (Contextual Meaning)
Map primitives to purpose — **components reference this layer:**
```css
:root {
  /* Semantic colors */
  --color-primary: hsl(var(--hsl-blue-500));
  --color-primary-hover: hsl(var(--hsl-blue-700));
  --color-accent: hsl(var(--hsl-amber-500));
  --color-bg: hsl(0, 0%, 100%);
  --color-bg-elevated: hsl(0, 0%, 97%);
  --color-text: hsl(0, 0%, 10%);
  --color-text-muted: hsl(0, 0%, 45%);

  /* Semantic spacing */
  --space-xs: var(--size-1);
  --space-sm: var(--size-2);
  --space-md: var(--size-4);
  --space-lg: var(--size-8);
}

.dark {
  --color-bg: hsl(220, 20%, 10%);
  --color-bg-elevated: hsl(220, 20%, 15%);
  --color-text: hsl(0, 0%, 90%);
  --color-text-muted: hsl(0, 0%, 55%);
}
```

### Layer 3: Component Tokens (Per-Component Customization)
Bind semantic tokens to specific component properties — enables per-component override:
```css
:root {
  /* Button tokens */
  --button-bg: var(--color-primary);
  --button-bg-hover: var(--color-primary-hover);
  --button-text: hsl(0, 0%, 100%);
  --button-radius: var(--radius-md);
  --button-shadow: var(--shadow-sm);

  /* Card tokens */
  --card-bg: var(--color-bg-elevated);
  --card-border: var(--color-border);
  --card-radius: var(--radius-lg);
  --card-shadow: var(--shadow-md);

  /* Input tokens */
  --input-bg: var(--color-bg);
  --input-border: var(--color-border);
  --input-ring: var(--color-primary);
  --input-radius: var(--radius-md);
}
```

**Flow:**
```
Primitive (raw values)
       ↓
Semantic (purpose aliases)
       ↓
Component (component-specific)
```

---

## Component Spec Pattern

Define state variations for each component using tokens:

| Property | Default | Hover | Active | Disabled |
|----------|---------|-------|--------|----------|
| Background | `--color-primary` | `--color-primary-hover` | `--color-primary-active` | `--color-muted` |
| Text | `white` | `white` | `white` | `--color-muted-fg` |
| Border | `none` | `none` | `none` | `--color-muted-border` |
| Shadow | `--shadow-sm` | `--shadow-md` | `none` | `none` |
| Opacity | `1` | `1` | `1` | `0.5` |
| Cursor | `pointer` | `pointer` | `pointer` | `not-allowed` |

**Example component using spec:**
```css
.btn-primary {
  background: var(--button-bg);
  color: var(--button-text);
  border-radius: var(--button-radius);
  box-shadow: var(--button-shadow);
  transition: all 0.2s ease;
  cursor: pointer;
}
.btn-primary:hover {
  background: var(--button-bg-hover);
  box-shadow: var(--shadow-md);
}
.btn-primary:active {
  box-shadow: none;
}
.btn-primary:disabled {
  background: var(--color-muted);
  color: var(--color-muted-fg);
  cursor: not-allowed;
  opacity: 0.5;
}
```

---

## Token Categories

| Category | Token Pattern | Format | Examples |
|---|---|---|---|
| Color | `--color-{purpose}` | HSL preferred | `--color-primary`, `--color-bg`, `--color-error` |
| Typography | `--font-{property}-{variant}` | rem/px | `--font-size-sm`, `--font-weight-bold` |
| Spacing | `--space-{size}` | rem (4px base grid) | `--space-xs`, `--space-md`, `--space-xl` |
| Shadows | `--shadow-{size}` | CSS shadow | `--shadow-sm`, `--shadow-lg` |
| Borders | `--radius-{size}` | px | `--radius-sm`, `--radius-full` |
| Breakpoints | `--bp-{device}` | px | `--bp-mobile: 640px`, `--bp-tablet: 1024px` |
| Transitions | `--duration-{speed}` | ms + easing | `--duration-fast: 150ms` |
| Z-index | `--z-{layer}` | integer | `--z-dropdown: 100`, `--z-modal: 1000` |

---

## Tailwind v4 Integration

Map token layers to Tailwind v4's `@theme` directive (CSS-first configuration):

```css
/* src/index.css */
@import "tailwindcss";

@theme {
  /* Primitive → Tailwind utilities */
  --color-primary: hsl(220, 90%, 56%);
  --color-primary-hover: hsl(220, 90%, 36%);
  --color-accent: hsl(38, 92%, 50%);
  --color-bg: hsl(0, 0%, 100%);
  --color-bg-elevated: hsl(0, 0%, 97%);
  --color-text: hsl(0, 0%, 10%);
  --color-text-muted: hsl(0, 0%, 45%);

  /* Spacing scale */
  --spacing-xs: 0.25rem;
  --spacing-sm: 0.5rem;
  --spacing-md: 1rem;
  --spacing-lg: 2rem;

  /* Radius scale */
  --radius-sm: 0.375rem;
  --radius-md: 0.5rem;
  --radius-lg: 0.75rem;
  --radius-xl: 1rem;
  --radius-full: 9999px;

  /* Shadow scale */
  --shadow-sm: 0 1px 2px rgba(0, 0, 0, 0.05);
  --shadow-md: 0 4px 6px rgba(0, 0, 0, 0.1);
  --shadow-lg: 0 10px 15px rgba(0, 0, 0, 0.1);
}
```

This auto-generates utilities: `bg-primary`, `text-text-muted`, `p-md`, `rounded-lg`, `shadow-md`, etc.

### Theme Switching (Light/Dark)

Dark mode = swapping semantic CSS variables, NOT adding new classes:

```css
/* Light mode (default) */
:root {
  --color-bg: hsl(0, 0%, 100%);
  --color-text: hsl(0, 0%, 10%);
  --color-border: hsl(0, 0%, 88%);
}

/* Dark mode — swap values only */
.dark {
  --color-bg: hsl(220, 20%, 10%);
  --color-text: hsl(0, 0%, 90%);
  --color-border: hsl(220, 15%, 25%);
}
```

Components don't change — they reference `var(--color-bg)` which resolves differently per theme.

---

## Token Audit Checklist

When reviewing a component for token compliance:

```markdown
## Token Audit — [Component Name]
- [ ] All colors use `--color-*` tokens (no hex/rgb/hsl literals)
- [ ] All spacing uses `--space-*` tokens (no magic numbers)
- [ ] All font sizes use `--font-size-*` tokens
- [ ] All shadows use `--shadow-*` tokens
- [ ] All border radii use `--radius-*` tokens
- [ ] All z-indices use `--z-*` tokens
- [ ] Component renders correctly in dark mode
- [ ] No hardcoded breakpoints (use `--bp-*` tokens)
- [ ] Component has defined states (default/hover/active/disabled)
- [ ] States use component-level tokens, not ad-hoc values
```

## Rules

1. **Components MUST use semantic tokens.** Never reference primitives directly.
2. **Dark mode = swapping semantic values**, not adding new classes.
3. **New values require justification.** Before adding a token, check if an existing one works.
4. **Document all tokens** in `src/index.css` `@theme {}` block or the design system file.
5. **Token naming is a contract** — changing a token name is a breaking change.
6. **Never use raw hex in components** — always reference semantic or component tokens.
7. **Semantic layer enables theme switching** — light/dark without touching component code.
8. **Component tokens enable per-component customization** — override without breaking the system.
9. **Use HSL format for opacity control** — `hsl(var(--hsl-primary) / 0.5)` for transparency.
10. **Document every token's purpose** — undocumented tokens become tech debt.
