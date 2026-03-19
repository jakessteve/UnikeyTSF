---
description: Visual Redesign Workflow — Upgrade existing "boring" UI and escalate visual quality without changing business logic or UX functional structure.
---

# WORKFLOW: /visual-redesign (Premium UI Upgrade)

Triggered to improve the aesthetic quality and premium feel ("Taste") of an existing page or component, transforming it from generic to high-end.

> **Single-LLM Execution Model:** "Call @agent" means **assume that agent's persona** and execute using available tools.

## Prerequisites
- Target UI (component or page) exists and functions correctly.
- Project dev server runnable via `npm run dev`.

## Steps

### Step 1 — Aesthetic Scoping — `OPUS/Plan`
**@pm** scopes the redesign constraint:
1. Review the target UI. Identify the existing functional structure (do NOT change the UX logic).
2. Set Aesthetic Configurations:
   - `DESIGN_VARIANCE` (1-10)
   - `MOTION_INTENSITY` (1-10)
   - `VISUAL_DENSITY` (1-10)
3. Determine the desired "Premium Taste" (e.g., editorial minimalist, glassmorphic dark mode).

### Step 2 — Visual Audit — `SONNET/Fast`
**@designer** evaluates the current screen using `premium-taste-ui.md` skill:
1. **Identify Slop:** Look for generic Tailwind templates, heavy drop-shadows, cramped padding (`p-4`), pure white/black harshness, zero typographic contrast.
2. Outline exactly what properties will change (increasing margins to `p-12`, mixing fonts, making borders hairline `border-white/10`, desaturating backgrounds to warm grays/zinc).

### Step 3 — Apply Redesign Implementation — `SONNET/Fast`
**@designer** implements the code refactor:
1. Apply the aesthetic configurations mapped out in Step 2.
2. **CRITICAL:** Follow the `anti-lazy-output.md` skill. Rewrite the entire component tree structure if necessary, do not leave `// ... existing code`.
3. Add smooth micro-interactions (150-300ms) with spring-like physics where appropriate.
4. Add dark mode parity using refined neutral palettes (`dark:bg-zinc-950`).

### Step 4 — Browser Visual Verification
1. Start the dev server (`npm run dev`).
2. Open the page in Browser Subagent.
3. Compare the "Premiumness" before and after. 
4. Verify the whitespace breathes, borders are refined, and typography scales are highly contrasted.

### Step 5 — Regression QC — `SONNET/Fast`
1. Switch to **@qc persona**.
2. Run `npm test` and `npx tsc --noEmit` and `npm run lint`.
3. Verify that the redesign didn't accidentally remove click handlers, `data-testid` properties, or form submission logic.

## Chaining
- After UI is tested functionally, hand back to **@pm** for approval.
