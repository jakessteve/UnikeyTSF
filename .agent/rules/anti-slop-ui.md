# RULE: Anti-Slop UI

**Scope:** All User Interface design, CSS generation, and component implementation by `@designer` or `@dev-fe`.

## 1. Context
Current AI generation tools default to a very recognizable, generic "SaaS template" aesthetic (often colloquially known as "slop"). This includes uninspired color palettes, cramped default spacing (`p-4`), heavy box shadows, linear animations, and lack of typographic personality. 

We require **high-agency, premium visual design** with explicit taste.

## 2. The Core Bans (Anti-Patterns)
Any UI code generation MUST AVOID the following unless specifically requested to do otherwise:

*   **Banned Shadow:** `shadow-md` or `box-shadow: 0 4px 6px` — instead, use subtle hairline borders (`border border-white/10`) or highly diffused soft shadows.
*   **Banned Spacing Clamp:** Defaulting to `p-4` or `gap-2` for everything. UI must explicitly use large whitespace (`p-8`, `p-12`) to breathe, creating a luxury or editorial feel.
*   **Banned Boring Fonts:** Relying solely on `font-sans` with `text-base` everywhere. You must introduce typographic contrast (e.g., massive serif headings paired with clean sans-serif body, or extreme scale differences like `text-xs uppercase tracking-widest` subheads).
*   **Banned Choppy Motion:** Hard state changes (0ms delay) or linear easing across the board. Hover and transition states must use spring physics curves (e.g., `ease-out`, `duration-300`, `cubic-bezier`).
*   **Banned "Lazy Dev" Placeholders:** DO NOT write `<!-- Content goes here -->` or `// styles...`. Implement the complete structure.

## 3. Enforcement
Before saving any CSS or React component file:
1. Does it look expensive?
2. Is the whitespace large enough?
3. Did we rely on a cheap default drop-shadow instead of a subtle structural border?

If the UI feels like a template, you have failed the Anti-Slop rule. Revise before reporting 'Done'.
