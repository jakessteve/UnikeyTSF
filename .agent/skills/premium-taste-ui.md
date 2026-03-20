---
description: Premium Taste UI - Teaches high-agency visual design to avoid generic templates and create expensive-feeling frontend code.
---

# SKILL: PREMIUM TASTE UI

## When to Apply
Use this skill whenever generating, iterating, or refactoring UI components. This is the core "Anti-Slop" skill for `@designer`.

## Aesthetic Directives

Stop generating generic, boring, SaaS-template slop. Operate with high visual agency. Apply these aesthetic principles by default:

### 1. Massive Whitespace
- **Do:** Use large, breathing margins and padding (`p-8`, `gap-12`, `mb-16`). Let elements breathe. Space is luxury.
- **Don't:** Cram objects together with `p-4` or `gap-2` unless building a high-density dashboard.

### 2. Typographic Contrast
- **Do:** Mix serif and sans-serif fonts for editorial quality (e.g., a serif heading with a clean sans-serif body). Use extreme size contrast between titles (`text-5xl`, `tracking-tight`) and secondary labels (`text-xs`, `tracking-widest`, `uppercase`).
- **Don't:** Rely purely on `text-base` and `font-bold` to distinguish hierarchy.

### 3. Layered Card Depth & Hairline Borders
- **Do:** Create depth with flat bento grids, subtle `border-white/10` or `border-zinc-200` hairline borders (`border`, `border-opacity-10`), and multi-layered surfaces. Use muted pastel accents.
- **Don't:** Rely on generic, heavy drop-shadows (`box-shadow: 0 4px 6px rgba(...)`) or stark black borders.

### 4. Smooth Spring-Based Animations
- **Do:** Apply spring physics easing where possible (e.g., `cubic-bezier(0.22, 1, 0.36, 1)`). Animate `transform` and `opacity` with 150-300ms durations. Add floating navigation.
- **Don't:** Use linear or choppy transitions. Do not animate layout properties (`width`, `height`).

### 5. Warm Monochrome Palettes
- **Do:** Rely heavily on neutral, desaturated, or warm gray scales (`zinc`, `stone`, `neutral`). Add one or two highly muted/pastel accent colors to draw the eye softly.
- **Don't:** Use pure saturated secondary/success colors (bright red, bright green) for backgrounds unless explicitly requested.

## High Agency Check
Before finalizing any CSS:
1. Did I just implement a default Bootstrap or Vercel-like template? If yes, rip it out and make it look expensive.
2. Are the margins large enough? 
3. Is the typography contrasting enough?
