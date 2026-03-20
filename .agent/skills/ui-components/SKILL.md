---
description: UI Components & Interaction — interaction patterns, form validation, navigation structures, and data visualization
---

# SKILL: UI COMPONENTS & INTERACTION

## When to Apply
Use this skill when implementing or reviewing the structural and interactive parts of UI components (buttons, forms, modals, navigation menus, tables, and charts).

## Quick Reference

### §1 Touch & Interaction (CRITICAL)
- **touch-target-size** — Min 44×44px interactive area
- **touch-spacing** — Minimum 8px gap between touch targets
- **hover-vs-tap** — Use click/tap for primary interactions; don't rely on hover alone
- **loading-buttons** — Disable button during async operations; show spinner
- **error-feedback** — Clear error messages near problem
- **cursor-pointer** — Add `cursor-pointer` to clickable elements
- **tap-delay** — Use `touch-action: manipulation` to reduce 300ms delay
- **press-feedback** — Visual feedback on press (ripple/opacity/elevation) within 80–150ms
- **gesture-alternative** — Always provide visible controls, not just gestures
- **safe-area-awareness** — Keep primary elements away from notch, gesture bar, screen edges

### §2 Forms & Feedback (MEDIUM)
- **input-labels** — Visible label per input (not placeholder-only)
- **error-placement** — Show error below the related field
- **submit-feedback** — Loading → success/error state on submit
- **required-indicators** — Mark required fields (asterisk)
- **empty-states** — Helpful message and action when no content
- **toast-dismiss** — Auto-dismiss toasts in 3–5s
- **confirmation-dialogs** — Confirm before destructive actions
- **progressive-disclosure** — Reveal complex options progressively
- **inline-validation** — Validate on blur, not keystroke
- **input-type-keyboard** — Use semantic input types (`email`, `tel`, `number`)
- **autofill-support** — Use `autocomplete` attributes for system autofill
- **undo-support** — Allow undo for destructive/bulk actions ("Undo delete" toast)
- **error-recovery** — Error messages must include recovery path (retry, edit, help)
- **multi-step-progress** — Multi-step flows show step indicator; allow back
- **focus-management** — After submit error, auto-focus the first invalid field
- **destructive-emphasis** — Destructive actions use danger color and separation

### §3 Navigation (HIGH)
- **bottom-nav-limit** — Max 5 items; use labels with icons
- **drawer-usage** — Use drawer/sidebar for secondary navigation, not primary actions
- **back-behavior** — Back navigation must be predictable; preserve scroll/state
- **deep-linking** — All key screens must be reachable via deep link/URL
- **nav-label-icon** — Navigation items must have both icon and text
- **nav-state-active** — Current location must be visually highlighted
- **nav-hierarchy** — Primary nav vs secondary nav must be clearly separated
- **modal-escape** — Modals must have clear close/dismiss affordance
- **state-preservation** — Navigating back must restore scroll, filters, input
- **back-stack-integrity** — Never silently reset nav stack or jump to home

### §4 Charts & Data (LOW)
- **chart-type** — Match chart type to data type (trend → line, comparison → bar)
- **data-table** — Provide table alternative for accessibility
- **legend-visible** — Always show legend near the chart
- **tooltip-on-interact** — Tooltips on hover/tap showing exact values
- **responsive-chart** — Charts must reflow on small screens
- **empty-data-state** — Show meaningful empty state, not blank chart
- **loading-chart** — Use skeleton while chart data loads
- **number-formatting** — Locale-aware formatting for numbers, dates, currencies
