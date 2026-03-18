---
description: Always On — enforce WCAG accessibility compliance on every UI component
---

# RULE: ACCESSIBILITY STANDARDS (WCAG Compliance)

**Mode:** Always On
**Scope:** All agents creating or modifying UI components

---

## Core Mandate

Every UI component produced MUST meet WCAG 2.1 AA baseline. Accessibility is not optional — it is a **delivery requirement** equal in importance to visual correctness.

---

## Binding Constraints

### 1. Color Contrast
- Normal text (< 18px / < 14px bold): contrast ratio ≥ **4.5:1**.
- Large text (≥ 18px / ≥ 14px bold): contrast ratio ≥ **3:1**.
- Never use color as the **sole** indicator of state. Always pair with icon, text, or pattern.
 - Red border only for error → Red border + error icon + error message text.

### 2. Interactive Elements
- All interactive elements MUST have visible **focus states**: `focus:ring-2 focus:ring-blue-500` or equivalent.
- **Keyboard navigation**: Tab order MUST match visual order. No keyboard traps.
- All clickable elements MUST have `cursor-pointer`.
- Icon-only buttons MUST have `aria-label` (e.g., `aria-label="Close menu"`).
- Skip-to-content link on pages with heavy navigation.

### 3. Forms
- Every `<input>` MUST have an associated visible `<label>` (use `for` attribute or wrap). Placeholder is NOT a substitute.
- Required fields MUST be marked with `*` or `(required)` text.
- Error messages MUST appear near the related input, not just at the top of the form.
- Error messages MUST use `role="alert"` or `aria-live` for screen reader announcement.

### 4. Images & Media
- All meaningful images MUST have descriptive `alt` text. Decorative images use `alt=""`.
- Video/audio content should have captions or transcripts where applicable.

### 5. Semantic HTML
- Use semantic elements: `<nav>`, `<main>`, `<article>`, `<section>`, `<aside>`, `<header>`, `<footer>`.
- Heading hierarchy MUST be sequential: `h1` → `h2` → `h3`. Never skip levels.
- Avoid `<div>` soup — if an element conveys structure, use the appropriate semantic tag.

### 6. Motion & Animation
- All animations MUST respect `prefers-reduced-motion`:
 ```css
 @media (prefers-reduced-motion: reduce) {
 * { animation-duration: 0.01ms !important; transition-duration: 0.01ms !important; }
 }
 ```
- No auto-playing video/audio without user control.
- Infinite decorative animations are forbidden (loading spinners are acceptable).

### 7. Touch Accessibility
- Minimum touch target: **44×44px**.
- Minimum gap between adjacent touch targets: **8px**.

---

## Enforcement
Non-compliant components must be flagged as ` A11Y VIOLATION` before delivery. Run the Pre-Delivery Checklist (in `@designer.md` §6) to verify.
