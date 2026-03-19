---
description: Anti-Lazy Output — Stops placeholder comments, half-finished code, and generic cutting corners during UI generation.
---

# SKILL: ANTI-LAZY OUTPUT

## When to Apply
Use this skill whenever generating code, components, CSS, or markup. It is designed to force high-effort completeness for `@designer` and `@dev` outputs.

## Core Rule: NO PLACEHOLDER SLOP

1. **Write the Entire Component**
   - **Do:** Generate every line necessary to render the UI exactly as discussed or designed.
   - **Don't:** Output boilerplate with `<!-- ... user content ... -->`, `/* styles go here */`, or `// rest of the code remains the same`. This is absolutely unacceptable.

2. **Full Implementation of Aesthetic Details**
   - **Do:** Include all interaction states (`hover:`, `focus:`, `active:`), dark mode variants (`dark:`), and structural Tailwind classes. If the design calls for an SVG icon, write the full `svg` tag with its `viewBox` and `path`.
   - **Don't:** Skip details because "the user can add them later." YOU are the designer.

3. **No Abstract Suggestions**
   - **Do:** Provide the exact implementation. "Add padding to the left sidebar" is bad. Code the `<aside className="pl-6 pt-12">` directly.

4. **Complete Data Mocks**
   - **Do:** When creating UI components that need sample data (like tables or cards), write realistic, full placeholder data objects that match the industry/context to demonstrate the visual layout perfectly.
   - **Don't:** Use `[{ id: 1, name: 'Test' }]` as dummy data.

**The Agency Principle:** Act like a senior frontend engineer delivering a final PR, not a rushed tutorial writer skipping over the hard parts.
