---
description: Full design-to-code pipeline — from mockup/brief to coded components with visual verification
---

# Workflow: /design-to-code

End-to-end pipeline from a design mockup or text brief to production-ready coded components.

---

## Prerequisites
- Design input: either a mockup image, Figma reference, or a text description
- Project dev server runnable via `npm run dev`
- Existing design system in `src/index.css` (or equivalent)

## Steps

### Step 1 — Analyze Design Input
1. **If image mockup:** Use vision to analyze the layout, identify:
 - Grid structure and layout hierarchy
 - Color palette used (map to existing theme tokens)
 - Typography (font families, sizes, weights)
 - Component boundaries (cards, buttons, forms, navbars, etc.)
 - Spacing patterns and alignment
2. **If text brief:** Extract requirements:
 - Product type and industry
 - Style keywords (minimal, dark mode, glassmorphism, etc.)
 - Key components and features needed
 - Target devices (mobile, desktop, both)

### Step 2 — Create Component Breakdown
Write a structured plan in `WIREFRAMES.md`:
```markdown
# [Feature/Page Name] — Component Breakdown

## Layout Structure
- [Describe overall grid/flex layout]

## Components
1. **ComponentName** — [description, location in layout]
 - Props: [key props]
 - Reuses: [existing primitives from design system]
 - New classes needed: [if any, following naming convention]

2. **ComponentName** — ...

## Design Tokens
- Colors: [mapped to existing tokens or new tokens needed]
- Typography: [mapped to existing scale]
- Spacing: [using project scale]
```

### Step 3 — Read Design System
**MANDATORY.** Before writing any code:
1. Read `src/index.css` to identify all reusable tokens and primitives.
2. Map the design's colors to existing theme tokens.
3. Identify which existing components can be reused.
4. Follow the `design-system-uiux` skill (Phase 1: AUDIT).

### Step 4 — Implement Components
Code each component following the `design-system-uiux` skill:
1. Use existing primitives and design tokens.
2. Follow CSS placement rules (`@theme`, `@layer base`, `@layer components`).
3. Implement dark mode parity for every visual style.
4. Add interaction states: hover, focus, active, disabled, loading.
5. Ensure accessibility: `aria-label`, keyboard nav, contrast ratios.

### Step 5 — Browser Visual Verification
1. Start the dev server (`npm run dev`).
2. Open the page in Browser Subagent.
3. Compare the implementation against the original design input.
4. Toggle light/dark mode — verify both look polished.
5. Check hover/focus/active states on all interactive elements.
6. Capture before/after screenshots.

### Step 6 — Responsive Validation
// turbo
Run `/qa-responsive-check` to automatically test at mobile, tablet, and desktop breakpoints.

### Step 7 — Functional QC Verification (MANDATORY)
1. Switch to **@qc persona**.
2. Run `npm test` — verify ALL tests pass (design changes must not break existing functionality).
3. Run `npx tsc --noEmit` — verify zero type errors.
4. Run `npm run lint` — verify no new lint errors.
5. If any check fails: switch back to @dev/@designer persona to fix, then re-run QC.

> **CRITICAL:** Visual verification (Steps 5–6) is NOT sufficient. Functional QC ensures design-to-code work didn't break existing logic.

### Step 8 — Deliver
1. Update `WIREFRAMES.md` with final component documentation.
2. Write design-log to `.hc/logs/design/[phase].md`.
3. Report Done to @pm with screenshot proof and QC verification results.

## Chaining
- Step 6 auto-triggers Workflow `/qa-responsive-check`.
- If responsive issues are found, loop back to Step 4 to fix, then re-run Step 6.
- Step 7 is mandatory and cannot be skipped.
