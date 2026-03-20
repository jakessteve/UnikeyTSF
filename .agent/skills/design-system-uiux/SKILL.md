---
description: Design System UI/UX Index — Lightweight meta-skill routing to ui-components, ux-patterns, and visual-design sub-skills.
---

# SKILL: DESIGN SYSTEM UI/UX

> **[Performance Notice]** This monolithic skill has been split to prevent context bloat. You should NOT use this skill directly. Instead, lazy-load the specific sub-skill needed for your task:

## 1. UI Components & Interaction (`ui-components`)
Use for: Touch targets, interaction states, forms, validation, navigation, modals, chart data representation.
Load: `view_file .agent/skills/ui-components/SKILL.md`

## 2. UX Patterns (`ux-patterns`)
Use for: Accessibility (a11y), frontend performance (lazy loading, CLS), layout (mobile-first, grids), and animation choreography.
Load: `view_file .agent/skills/ux-patterns/SKILL.md`

## 3. Visual Design (`visual-design`)
Use for: Aesthetics, typography, color palettes, dark mode styling, shadowing, and imagery/iconography.
Load: `view_file .agent/skills/visual-design/SKILL.md`

## Audit Phase Protocol
1. **Read the design system** in `src/index.css` to understand existing tokens.
2. **Determine Domain:** Identify which of the 3 domains above your task primarily falls under.
3. **Lazy-Load:** Load ONLY the corresponding sub-skill file.
