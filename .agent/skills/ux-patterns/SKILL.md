---
description: UX Patterns — accessibility (a11y), frontend performance, layout responsiveness, and structural animation rules
---

# SKILL: UX PATTERNS

## When to Apply
Use this skill when establishing page layouts, responsive grids, performance optimizations at the UI layer, and overarching accessibility or structural animation rules.

## Quick Reference

### §1 Accessibility (CRITICAL)
- **color-contrast** — Minimum 4.5:1 for normal text
- **focus-states** — Visible focus rings (2–4px) on interactive elements
- **alt-text** — Descriptive alt text for meaningful images
- **aria-labels** — `aria-label` for icon-only buttons
- **keyboard-nav** — Tab order matches visual order; full keyboard support
- **form-labels** — Use `<label>` with `for` attribute
- **skip-links** — Skip-to-main-content for keyboard users
- **heading-hierarchy** — Sequential h1→h6, no level skip
- **color-not-only** — Don't convey info by color alone (add icon/text)
- **reduced-motion** — Respect `prefers-reduced-motion`

### §2 Performance (HIGH)
- **image-optimization** — Use WebP/AVIF, `srcset`/`sizes`, lazy load non-critical
- **image-dimension** — Declare `width`/`height` or `aspect-ratio` to prevent CLS
- **font-loading** — Use `font-display: swap` to avoid FOIT
- **critical-css** — Prioritize above-the-fold CSS
- **lazy-loading** — Lazy load non-hero components via dynamic import
- **content-jumping** — Reserve space for async content (skeletons, aspect-ratio)
- **virtualize-lists** — Virtualize lists with 50+ items
- **progressive-loading** — Use skeleton screens for >1s operations

### §3 Layout & Responsive (HIGH)
- **viewport-meta** — `width=device-width initial-scale=1` (never disable zoom)
- **mobile-first** — Design mobile-first, then scale up
- **breakpoint-consistency** — Systematic breakpoints: 375 / 768 / 1024 / 1440
- **readable-font-size** — Minimum 16px body text on mobile
- **line-length-control** — Mobile 35–60 chars; desktop 60–75 chars
- **horizontal-scroll** — No horizontal scroll on mobile
- **spacing-scale** — Use 4px/8px incremental spacing
- **container-width** — Consistent max-width (`max-w-6xl` / `max-w-7xl`)
- **z-index-management** — Define layered z-index scale (0/10/20/40/100/1000)
- **viewport-units** — Prefer `min-h-dvh` over `100vh` on mobile

### §4 Animation (MEDIUM)
- **duration-timing** — 150–300ms micro-interactions; complex transitions ≤400ms
- **transform-performance** — Only animate `transform`/`opacity`
- **loading-states** — Show skeleton or progress when loading exceeds 300ms
- **easing** — `ease-out` entering, `ease-in` exiting; avoid linear for UI
- **motion-meaning** — Every animation must express cause-effect, not decoration
- **state-transition** — State changes should animate smoothly, not snap
- **spring-physics** — Prefer spring/physics-based curves for natural feel
- **exit-faster-than-enter** — Exit animations ~60–70% of enter duration
- **stagger-sequence** — Stagger list/grid entrance by 30–50ms per item
- **interruptible** — Animations must be interruptible; user gesture cancels immediately
- **layout-shift-avoid** — Animations must not cause reflow or CLS
