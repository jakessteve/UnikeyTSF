# Implementation & Verification Checklist

## CSS Placement Rules
- Design tokens → `@theme {}` block
- Base resets → `@layer base {}`
- Reusable components → `@layer components {}`
- One-off overrides → inline or scoped styles

## New Component Template
```css
.component-name {
  @apply bg-surface-light dark:bg-surface-dark;
  @apply text-text-primary-light dark:text-text-primary-dark;
  @apply border border-border-light/50 dark:border-border-dark/50;
  @apply rounded-xl;
  transition: all 0.2s ease;
}
.component-name:hover {
  @apply bg-surface-subtle-light dark:bg-surface-subtle-dark;
}
```

## Pre-Delivery Checklist

### Visual Quality
- [ ] No emojis used as icons (use SVG instead)
- [ ] All icons from consistent icon set (Heroicons/Lucide)
- [ ] Consistent icon sizing via design tokens (16/24/32px)
- [ ] Consistent stroke width within same visual layer
- [ ] Hover states don't cause layout shift
- [ ] All colors from theme tokens (no raw hex/rgb)
- [ ] Official brand assets used with correct proportions

### Interaction
- [ ] All clickable elements have `cursor-pointer`
- [ ] Transitions are smooth (150–300ms)
- [ ] Focus states visible for keyboard navigation (`focus:ring-2`)
- [ ] Buttons disabled + spinner during async
- [ ] Touch targets ≥ 44×44px with ≥ 8px spacing between them
- [ ] Press feedback (ripple/opacity) within 80–150ms
- [ ] No gesture-only interactions without visible controls
- [ ] One primary gesture per region (no overlapping tap/drag conflicts)

### Light/Dark Mode
- [ ] Toggle both modes — both must look polished
- [ ] Light mode text contrast ≥ 4.5:1
- [ ] Dark mode text contrast ≥ 4.5:1 (primary), ≥ 3:1 (secondary)
- [ ] Borders/glass elements visible in both modes
- [ ] Dividers/interaction states distinguishable in both themes
- [ ] Modal scrim opacity strong enough (40–60% black)

### Layout
- [ ] Responsive at 375px, 768px, 1024px, 1440px
- [ ] No content hidden behind fixed elements
- [ ] No horizontal scroll on mobile
- [ ] Safe areas respected for headers, tab bars, bottom CTAs
- [ ] Scroll content not hidden behind sticky bars
- [ ] 4/8px spacing rhythm maintained
- [ ] Long-form text readable on large devices (no edge-to-edge)

### Accessibility
- [ ] All images have `alt` text
- [ ] Form inputs have visible labels
- [ ] Icon-only buttons have `aria-label`
- [ ] `prefers-reduced-motion` respected
- [ ] Heading hierarchy is sequential (h1→h6)
- [ ] Color is not the only indicator (add icon/text)
- [ ] Skip-to-main-content link present

### Performance
- [ ] Images use WebP/AVIF with `srcset`/`sizes`
- [ ] Below-fold images use `loading="lazy"`
- [ ] `width`/`height` or `aspect-ratio` declared on media (prevent CLS)
- [ ] Skeleton screens used for >1s loading operations
- [ ] Lists with 50+ items are virtualized
- [ ] High-frequency events (scroll, resize) debounced/throttled

### Charts & Data (if applicable)
- [ ] Chart type matches data type (trend→line, comparison→bar)
- [ ] Accessible color palette (no red/green only)
- [ ] Legends visible and near chart
- [ ] Tooltips show exact values on hover/tap
- [ ] Empty data state shows meaningful message
- [ ] Charts reflow/simplify on small screens

### Final Steps
- Run `npm test` to ensure no regressions.
- Capture before/after screenshots for the walkthrough.
