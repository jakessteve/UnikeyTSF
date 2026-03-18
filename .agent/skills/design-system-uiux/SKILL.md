---
description: Design System UI/UX — professional design intelligence with 99 UX rules, 10 priority categories, and searchable quick-reference for building consistent, accessible, and polished UI
---

# SKILL: DESIGN SYSTEM UI/UX

Comprehensive design guide for web and mobile applications. Contains 99 UX rules across 10 priority categories, professional UI checklists, and interaction patterns. Adapted for our stack: React 19, Tailwind v4, Vite 7.

## When to Apply

This Skill should be used when the task involves **UI structure, visual design decisions, interaction patterns, or user experience quality control**.

### Must Use

- Designing new pages or views (Landing Page, Dashboard, Settings, Reports)
- Creating or refactoring UI components (buttons, modals, forms, tables, charts)
- Choosing color schemes, typography systems, spacing standards, or layout systems
- Reviewing UI code for user experience, accessibility, or visual consistency
- Implementing navigation structures, animations, or responsive behavior
- Improving perceived quality, clarity, or usability of interfaces

### Recommended

- UI looks "not professional enough" but the reason is unclear
- Receiving feedback on usability or experience
- Pre-launch UI quality optimization
- Building design systems or reusable component libraries

### Skip

- Pure backend logic development
- Only involving API or database design
- Performance optimization unrelated to the interface
- Infrastructure or DevOps work

**Decision criteria**: If the task will change how a feature **looks, feels, moves, or is interacted with**, this Skill should be used.

---

## Phase 1 — AUDIT (Before Coding)

1. **Read the design system** in `src/index.css` to understand existing tokens, components, and patterns.
2. **Identify reusable primitives** — Colors: `@theme` semantic tokens. Surfaces: `.card-surface`, `.card-subtle`. Typography: `.label-standard`, `.section-title`. Shadows: `.shadow-apple`. Animations: `.animate-fade-in-up`.
3. **Check dark mode** parity: every light style MUST have a `.dark` counterpart.
4. **Review affected component** in browser (`browser-visual-testing` skill).

## Phase 2 — DESIGN (Decide Approach)

5. **Reuse over reinvent:** Compose from existing primitives before creating new classes.
6. **Consistency:** Font sizes: `10–15px` scale. Spacing: TailwindCSS utilities. Radius: `rounded-lg/xl/2xl`. Transitions: `all 0.2s ease`.
7. **Responsive:** Mobile-first (375px, 768px, 1024px). Tử Vi grid min-width: 700px with scroll.
8. **Accessibility:** Contrast ≥ 4.5:1, 44×44px touch targets, focus-visible outlines.

## Phase 3–6 — IMPLEMENT & VERIFY

See `references/professional-ui-rules.md` for detailed Do/Don't tables.
See `references/ux-reasoning-rules.md` for navigation, animation, forms, performance, charts, and z-index rules.
See `references/implementation-checklist.md` for CSS placement rules, templates, and pre-delivery checklist.

---

## Rule Categories by Priority

Follow priority 1→10 to decide which rule category to focus on first.

| Priority | Category | Impact | Key Checks (Must Have) | Anti-Patterns (Avoid) |
|----------|----------|--------|------------------------|------------------------|
| 1 | Accessibility | CRITICAL | Contrast 4.5:1, Alt text, Keyboard nav, Aria-labels | Removing focus rings, Icon-only buttons without labels |
| 2 | Touch & Interaction | CRITICAL | Min 44×44px, 8px+ spacing, Loading feedback | Reliance on hover only, Instant state changes (0ms) |
| 3 | Performance | HIGH | WebP/AVIF, Lazy loading, Reserve space (CLS < 0.1) | Layout thrashing, CLS, no skeleton screens |
| 4 | Style Selection | HIGH | Match product type, Consistency, SVG icons (no emoji) | Mixing flat & skeuomorphic randomly, Emoji as icons |
| 5 | Layout & Responsive | HIGH | Mobile-first breakpoints, Viewport meta, No h-scroll | Horizontal scroll, Fixed px widths, Disable zoom |
| 6 | Typography & Color | MEDIUM | Base 16px, Line-height 1.5, Semantic color tokens | Text < 12px body, Gray-on-gray, Raw hex in components |
| 7 | Animation | MEDIUM | Duration 150–300ms, Motion conveys meaning | Decorative-only animation, No reduced-motion support |
| 8 | Forms & Feedback | MEDIUM | Visible labels, Error near field, Progressive disclosure | Placeholder-only label, Errors only at top |
| 9 | Navigation | HIGH | Predictable back, Bottom nav ≤5, Deep linking | Overloaded nav, Broken back behavior |
| 10 | Charts & Data | LOW | Legends, Tooltips, Accessible colors | Relying on color alone to convey meaning |

---

## Quick Reference

### §1 Accessibility (CRITICAL)

- **color-contrast** — Minimum 4.5:1 for normal text, 3:1 for large text
- **focus-states** — Visible focus rings (2–4px) on interactive elements
- **alt-text** — Descriptive alt text for meaningful images
- **aria-labels** — `aria-label` for icon-only buttons
- **keyboard-nav** — Tab order matches visual order; full keyboard support
- **form-labels** — Use `<label>` with `for` attribute
- **skip-links** — Skip-to-main-content for keyboard users
- **heading-hierarchy** — Sequential h1→h6, no level skip
- **color-not-only** — Don't convey info by color alone (add icon/text)
- **reduced-motion** — Respect `prefers-reduced-motion`; reduce/disable animations
- **voiceover-sr** — Meaningful `aria-label`/`aria-describedby`; logical reading order
- **escape-routes** — Provide cancel/back in modals and multi-step flows
- **keyboard-shortcuts** — Preserve system a11y shortcuts; offer keyboard alternatives for drag-and-drop

### §2 Touch & Interaction (CRITICAL)

- **touch-target-size** — Min 44×44px interactive area
- **touch-spacing** — Minimum 8px gap between touch targets
- **hover-vs-tap** — Use click/tap for primary interactions; don't rely on hover alone
- **loading-buttons** — Disable button during async operations; show spinner
- **error-feedback** — Clear error messages near problem
- **cursor-pointer** — Add `cursor-pointer` to clickable elements
- **tap-delay** — Use `touch-action: manipulation` to reduce 300ms delay
- **press-feedback** — Visual feedback on press (ripple/opacity/elevation) within 80–150ms
- **gesture-alternative** — Don't rely on gesture-only interactions; always provide visible controls
- **safe-area-awareness** — Keep primary elements away from notch, gesture bar, screen edges

### §3 Performance (HIGH)

- **image-optimization** — Use WebP/AVIF, `srcset`/`sizes`, lazy load non-critical assets
- **image-dimension** — Declare `width`/`height` or `aspect-ratio` to prevent CLS
- **font-loading** — Use `font-display: swap` to avoid FOIT
- **critical-css** — Prioritize above-the-fold CSS
- **lazy-loading** — Lazy load non-hero components via dynamic import
- **bundle-splitting** — Split code by route/feature to reduce initial load
- **content-jumping** — Reserve space for async content (skeletons, aspect-ratio)
- **virtualize-lists** — Virtualize lists with 50+ items
- **main-thread-budget** — Keep per-frame work under ~16ms for 60fps
- **progressive-loading** — Use skeleton screens instead of long blocking spinners for >1s operations
- **debounce-throttle** — Use debounce/throttle for high-frequency events (scroll, resize, input)

### §4 Style Selection (HIGH)

- **no-emoji-icons** — Use SVG icons (Heroicons, Lucide), not emojis
- **consistency** — Use same style across all pages
- **color-palette** — Choose palette from product/industry context
- **effects-match-style** — Shadows, blur, radius aligned with chosen style
- **state-clarity** — Make hover/pressed/disabled states visually distinct
- **elevation-consistent** — Consistent elevation/shadow scale for cards, sheets, modals
- **dark-mode-pairing** — Design light/dark variants together
- **icon-style-consistent** — Use one icon set/visual language across the product
- **primary-action** — Each screen should have only one primary CTA

### §5 Layout & Responsive (HIGH)

- **viewport-meta** — `width=device-width initial-scale=1` (never disable zoom)
- **mobile-first** — Design mobile-first, then scale up
- **breakpoint-consistency** — Systematic breakpoints: 375 / 768 / 1024 / 1440
- **readable-font-size** — Minimum 16px body text on mobile
- **line-length-control** — Mobile 35–60 chars; desktop 60–75 chars
- **horizontal-scroll** — No horizontal scroll on mobile
- **spacing-scale** — Use 4px/8px incremental spacing
- **container-width** — Consistent max-width (`max-w-6xl` / `max-w-7xl`)
- **z-index-management** — Define layered z-index scale (0/10/20/40/100/1000)
- **fixed-element-offset** — Fixed navbar/bottom bar must reserve safe padding
- **viewport-units** — Prefer `min-h-dvh` over `100vh` on mobile
- **visual-hierarchy** — Establish hierarchy via size, spacing, contrast — not color alone

### §6 Typography & Color (MEDIUM)

- **line-height** — Use 1.5–1.75 for body text
- **font-pairing** — Match heading/body font personalities
- **font-scale** — Consistent type scale (e.g. 12 14 16 18 24 32)
- **contrast-readability** — Darker text on light backgrounds (e.g. slate-900 on white)
- **weight-hierarchy** — Bold headings (600–700), Regular body (400), Medium labels (500)
- **color-semantic** — Define semantic color tokens (primary, secondary, error, surface)
- **color-dark-mode** — Dark mode uses desaturated/lighter tonal variants, not inverted colors
- **color-accessible-pairs** — Foreground/background pairs must meet 4.5:1 (AA)
- **truncation-strategy** — Prefer wrapping over truncation; when truncating use ellipsis + tooltip
- **number-tabular** — Use tabular/monospaced figures for data columns, prices, timers
- **whitespace-balance** — Use whitespace intentionally to group related items

### §7 Animation (MEDIUM)

- **duration-timing** — 150–300ms micro-interactions; complex transitions ≤400ms
- **transform-performance** — Only animate `transform`/`opacity`; avoid `width`/`height`/`top`/`left`
- **loading-states** — Show skeleton or progress when loading exceeds 300ms
- **easing** — `ease-out` entering, `ease-in` exiting; avoid linear for UI transitions
- **motion-meaning** — Every animation must express cause-effect, not decoration
- **state-transition** — State changes should animate smoothly, not snap
- **spring-physics** — Prefer spring/physics-based curves for natural feel
- **exit-faster-than-enter** — Exit animations ~60–70% of enter duration
- **stagger-sequence** — Stagger list/grid entrance by 30–50ms per item
- **interruptible** — Animations must be interruptible; user gesture cancels immediately
- **no-blocking-animation** — Never block user input during animation
- **shared-element-transition** — Use shared element transitions for visual continuity
- **navigation-direction** — Forward = left/up; backward = right/down
- **layout-shift-avoid** — Animations must not cause reflow or CLS

### §8 Forms & Feedback (MEDIUM)

- **input-labels** — Visible label per input (not placeholder-only)
- **error-placement** — Show error below the related field
- **submit-feedback** — Loading → success/error state on submit
- **required-indicators** — Mark required fields (asterisk)
- **empty-states** — Helpful message and action when no content
- **toast-dismiss** — Auto-dismiss toasts in 3–5s
- **confirmation-dialogs** — Confirm before destructive actions
- **progressive-disclosure** — Reveal complex options progressively; don't overwhelm upfront
- **inline-validation** — Validate on blur, not keystroke
- **input-type-keyboard** — Use semantic input types (`email`, `tel`, `number`)
- **autofill-support** — Use `autocomplete` attributes for system autofill
- **undo-support** — Allow undo for destructive/bulk actions ("Undo delete" toast)
- **error-recovery** — Error messages must include recovery path (retry, edit, help link)
- **multi-step-progress** — Multi-step flows show step indicator; allow back navigation
- **focus-management** — After submit error, auto-focus the first invalid field
- **destructive-emphasis** — Destructive actions use danger color and are visually separated

### §9 Navigation (HIGH)

- **bottom-nav-limit** — Max 5 items; use labels with icons
- **drawer-usage** — Use drawer/sidebar for secondary navigation, not primary actions
- **back-behavior** — Back navigation must be predictable; preserve scroll/state
- **deep-linking** — All key screens must be reachable via deep link/URL
- **nav-label-icon** — Navigation items must have both icon and text
- **nav-state-active** — Current location must be visually highlighted
- **nav-hierarchy** — Primary nav vs secondary nav must be clearly separated
- **modal-escape** — Modals must have clear close/dismiss affordance
- **search-accessible** — Search must be easily reachable
- **state-preservation** — Navigating back must restore scroll, filters, input
- **gesture-nav-support** — Support system gesture navigation without conflict
- **adaptive-navigation** — Large screens prefer sidebar; small screens use bottom/top nav
- **back-stack-integrity** — Never silently reset nav stack or jump to home
- **navigation-consistency** — Navigation placement must stay the same across all pages
- **persistent-nav** — Core navigation must remain reachable from deep pages

### §10 Charts & Data (LOW)

- **chart-type** — Match chart type to data type (trend → line, comparison → bar, proportion → pie/donut)
- **color-guidance** — Accessible color palettes; avoid red/green only pairs
- **data-table** — Provide table alternative for accessibility
- **pattern-texture** — Supplement color with patterns/shapes
- **legend-visible** — Always show legend near the chart
- **tooltip-on-interact** — Tooltips on hover/tap showing exact values
- **axis-labels** — Label axes with units and readable scale
- **responsive-chart** — Charts must reflow on small screens
- **empty-data-state** — Show meaningful empty state, not blank chart
- **loading-chart** — Use skeleton while chart data loads
- **large-dataset** — For 1000+ data points, aggregate; provide drill-down
- **number-formatting** — Locale-aware formatting for numbers, dates, currencies
- **no-pie-overuse** — Avoid pie/donut for >5 categories; switch to bar
- **legend-interactive** — Legends clickable to toggle series visibility
