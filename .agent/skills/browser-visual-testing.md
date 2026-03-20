---
description: Browser Visual Testing - responsive verification, dark mode, a11y, and visual regression with screenshot evidence
---

# SKILL: Browser Visual Testing

**Trigger:** After any UI/CSS change, layout bug fix, or when verifying visual output of components.

---

## When to Use
- After CSS/style changes to verify no visual regressions.
- When implementing responsive layouts.
- After adding/modifying components.
- During `/qa-responsive-check` workflow.
- Before merging any UI-related PR.

---

## Execution Protocol

### Step 1: Setup
1. Ensure the dev server is running (`npm run dev`).
2. Use the `browser_subagent` tool to navigate to `http://localhost:5173` (or active Vite port).
3. Prepare the page state (navigate to right section, set theme, input test data).

### Step 2: Responsive Breakpoint Matrix
Test at these breakpoints using browser resize:

| Breakpoint | Width | Height | Priority | Device Simulation |
|---|---|---|---|---|
| **Mobile S** | 320px | 568px | Must test | iPhone SE |
| **Mobile L** | 425px | 812px | Must test | iPhone 12/13 |
| **Tablet** | 768px | 1024px | Must test | iPad |
| **Desktop** | 1024px | 768px | Must test | Small laptop |
| **Desktop L** | 1440px | 900px | Should test | Standard desktop |
| **Ultrawide** | 1920px | 1080px | Nice to have | Large monitor |

**Per-breakpoint checklist:**
- [ ] No horizontal overflow / scrollbar
- [ ] Text is readable (min 14px on mobile)
- [ ] Touch targets ≥ 44x44px on mobile
- [ ] Navigation is accessible (hamburger on mobile, sidebar on desktop)
- [ ] Images/charts scale proportionally
- [ ] No overlapping elements
- [ ] Content doesn't get cut off

### Step 3: Dark Mode Verification
Toggle between light and dark mode and verify:

| Check | Standard | How to Test |
|---|---|---|
| Text readability | Contrast ratio ≥ 4.5:1 (normal text) | Visual inspection, DevTools |
| Large text readability | Contrast ratio ≥ 3:1 (18px+ text) | Visual inspection |
| Borders and dividers | Visible in both modes | Visual inspection |
| Icons | Appropriate contrast, not invisible | Visual inspection |
| No "invisible" text | Text color ≠ background color | Toggle mode, scan all areas |
| Form inputs | Visible borders, readable text | Tab through form elements |
| Active/selected states | Distinguishable from default | Click through interactive elements |
| Charts/graphs | Colors distinguishable | Check all chart types |

### Step 4: Visual Regression Checks
- [ ] Layout matches expected design (if wireframe exists)
- [ ] Spacing consistent with design tokens (skill `design-token-pipeline`)
- [ ] Colors match the design system tokens (no hardcoded values)
- [ ] Typography follows hierarchy (H1 > H2 > H3 > body)
- [ ] Animations run smoothly, no jank (skill `animation-choreography`)
- [ ] Loading states render correctly (skeleton/spinner)

### Step 5: Accessibility Quick Check
- [ ] All interactive elements keyboard-focusable (Tab through page)
- [ ] Focus order follows visual/logical order
- [ ] Focus rings are visible (not hidden by CSS)
- [ ] Images have alt text
- [ ] Color is not the only way to convey information (icons, patterns)
- [ ] Form inputs have associated labels
- [ ] No content hidden by `display:none` that should be accessible

### Step 6: Project-Specific Verification Areas
Define the critical UI areas for your project and verify each:

| Area | What to Verify |
|---|---|
| _[Core feature 1]_ | _[Key visual elements, layout, data display]_ |
| _[Core feature 2]_ | _[Key visual elements, layout, data display]_ |
| _[Input forms]_ | _[Form controls, validation states, error messages]_ |
| _[Data display]_ | _[Tables, charts, lists — correct rendering at all breakpoints]_ |
| _[Export / output]_ | _[Print layout, PDF, file output — matches screen]_ |

---

## Evidence Documentation
Capture and save screenshots for each breakpoint:
```markdown
## Visual Test Report — [Feature/PR] — YYYY-MM-DD
| Breakpoint | Status | Notes |
|---|---|---|
| Mobile S (320px) | Pass | — |
| Mobile L (425px) | Pass | — |
| Tablet (768px) | Issue | Sidebar overlaps content at this width |
| Desktop (1024px) | Pass | — |
| Dark Mode | Pass | — |
```

## Rules
- **Fix visual defects immediately.** Do NOT report "Done" until the UI matches the spec.
- **Capture screenshots** for each breakpoint to document verification.
- **Test real content**, not lorem ipsum — edge cases often hide in real data.
- **Mobile-first** — always test mobile breakpoints before desktop.
