# UX Reasoning Rules

## Navigation

- **Sticky nav:** Add `padding-top` to body equal to nav height.
- **Active state:** Highlight current page/section with color/underline.
- **Back button:** Preserve navigation history. Never break browser back.
- **Breadcrumbs:** Use for sites with 3+ levels of depth.
- **Bottom nav limit:** Max 5 items; use labels with icons (not icon-only).
- **Drawer usage:** Use drawer/sidebar for secondary navigation, not primary actions.
- **Deep linking:** All key screens must be reachable via deep link/URL for sharing.
- **Nav state active:** Current location must be visually highlighted (color, weight, indicator).
- **Nav hierarchy:** Primary nav (tabs/bottom bar) vs secondary nav (drawer/settings) must be clearly separated.
- **Gesture nav support:** Support system gesture navigation (swipe-back) without conflict.
- **Adaptive navigation:** Large screens (≥1024px) prefer sidebar; small screens use bottom/top nav.
- **Back stack integrity:** Never silently reset nav stack or unexpectedly jump to home.
- **Modal escape:** Modals must have clear close/dismiss affordance; swipe-down on mobile.
- **State preservation:** Navigating back must restore scroll position, filter state, and input.
- **Persistent nav:** Core navigation must remain reachable from deep pages.
- **Overflow menu:** When actions exceed available space, use overflow/more menu instead of cramming.

## Animation

- **Duration:** 150–300ms micro-interactions. ≤400ms complex transitions. Never >500ms.
- **Easing:** `ease-out` entering, `ease-in` exiting. `cubic-bezier(0.22, 1, 0.36, 1)` spring.
- **Restraint:** Max 1–2 key animated elements per view.
- **Performance:** Only animate `transform` and `opacity`.
- **Reduced motion:** ALWAYS check `prefers-reduced-motion`. Non-negotiable.
- **Loading:** Show skeleton screens or spinners during async.
- **Spring physics:** Prefer spring/physics-based curves for natural feel.
- **Exit faster than enter:** Exit animations ~60–70% of enter duration.
- **Stagger sequence:** Stagger list/grid item entrance by 30–50ms per item.
- **Interruptible:** Animations must be interruptible; user gesture cancels immediately.
- **No blocking animation:** Never block user input during an animation; UI must stay interactive.
- **Shared element transition:** Use shared element/hero transitions for visual continuity between screens.
- **Navigation direction:** Forward = left/up; backward = right/down — keep logically consistent.
- **Layout shift avoid:** Animations must not cause reflow or CLS; use `transform` for position changes.
- **Modal motion:** Modals should animate from trigger source (scale+fade or slide-in).
- **Motion consistency:** Unify duration/easing tokens globally; all animations share the same rhythm.

## Forms

- **Visible labels:** Always show label above/beside input. Placeholder ≠ label.
- **Error placement:** Below the related input, not at top.
- **Inline validation:** Validate on `blur`, not only on submit.
- **Input types:** `type="email"`, `type="tel"`, `type="url"`, `type="number"`.
- **Mobile keyboards:** `inputmode="numeric"`, `inputmode="email"`.
- **Required indicators:** `*` or `(required)`.
- **Submit feedback:** Loading → success/error. Never submit silently.
- **Progressive disclosure:** Reveal complex options progressively; don't overwhelm upfront.
- **Autofill support:** Use `autocomplete` attributes so the system can autofill.
- **Undo support:** Allow undo for destructive/bulk actions ("Undo delete" toast).
- **Error recovery:** Error messages must include recovery path (retry, edit, help link).
- **Multi-step progress:** Multi-step flows show step indicator/progress bar; allow back navigation.
- **Focus management:** After submit error, auto-focus the first invalid field.
- **Error clarity:** Error messages must state cause + how to fix (not just "Invalid input").
- **Destructive emphasis:** Destructive actions use danger color (red) and are visually separated.
- **Form autosave:** Long forms should auto-save drafts to prevent data loss on accidental dismissal.

## Performance

- **Images:** WebP, `srcset`, `loading="lazy"` for below-fold.
- **Fonts:** `font-display: swap` to prevent invisible text.
- **Content jumping:** Reserve space with `aspect-ratio` or fixed dimensions.
- **Viewport:** Always include viewport meta tag.
- **Virtualize lists:** Virtualize lists with 50+ items for memory efficiency and scroll performance.
- **Main thread budget:** Keep per-frame work under ~16ms for 60fps; move heavy tasks off main thread.
- **Debounce/throttle:** Use debounce/throttle for high-frequency events (scroll, resize, input).
- **Progressive loading:** Use skeleton screens instead of long blocking spinners for >1s operations.
- **Input latency:** Keep input latency under ~100ms for taps/scrolls.
- **Bundle splitting:** Split code by route/feature to reduce initial load and TTI.
- **Third-party scripts:** Load third-party scripts async/defer; audit and remove unnecessary ones.
- **Lazy load below fold:** Use `loading="lazy"` for below-fold images and heavy media.

## Charts & Data

- **Chart type:** Match chart type to data (trend → line, comparison → bar, proportion → pie/donut).
- **Color guidance:** Use accessible palettes; avoid red/green only pairs for colorblind users.
- **Data table:** Provide table alternative for accessibility; charts alone aren't screen-reader friendly.
- **Legend visible:** Always show legend near the chart, not detached below a scroll fold.
- **Tooltip on interact:** Provide tooltips on hover (web) or tap (mobile) showing exact values.
- **Responsive chart:** Charts must reflow or simplify on small screens.
- **Empty data state:** Show meaningful empty state when no data ("No data yet" + guidance).
- **Loading chart:** Use skeleton while chart data loads; don't show empty axis frame.
- **No pie overuse:** Avoid pie/donut for >5 categories; switch to bar chart for clarity.
- **Number formatting:** Use locale-aware formatting for numbers, dates, currencies.
- **Large dataset:** For 1000+ data points, aggregate; provide drill-down for detail.

## Z-Index Scale

- `z-10` — Dropdown menus, tooltips
- `z-20` — Sticky headers, floating buttons
- `z-30` — Modals, overlays
- `z-50` — Toast notifications, critical alerts
- NEVER use arbitrary values like `z-[9999]`.
