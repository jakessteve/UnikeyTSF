---
description: Automated responsive QA — screenshots and DOM checks at mobile, tablet, and desktop breakpoints
---

# Workflow: /qa-responsive-check

Automated responsive design verification using Browser Subagent.

---

## Prerequisites
- App running on `localhost` (typically via `npm run dev`)
- Browser Subagent available

## Steps

### Step 1 — Open the Application
Use Browser Subagent to open the app at `http://localhost:5173` (or the active dev server port).
Navigate to the target page. If no page is specified, test the current/home page.

### Step 2 — Mobile Check (375px)
1. Resize browser window to **375px × 812px** (iPhone SE).
2. Capture a screenshot.
3. Read the DOM and check for:
   - Horizontal overflow (any element width > viewport width)
   - Text overflow / clipping
   - Touch targets < 44px
   - Content hidden behind fixed headers/footers
   - Font size < 16px on body text
4. Record all issues found.

### Step 3 — Tablet Check (768px)
1. Resize browser window to **768px × 1024px** (iPad portrait).
2. Capture a screenshot.
3. Read the DOM and check for:
   - Layout breakage at medium width
   - Navigation responsiveness
   - Card/grid layout adjustments
   - Image scaling
4. Record all issues found.

### Step 4 — Desktop Check (1440px)
1. Resize browser window to **1440px × 900px** (standard desktop).
2. Capture a screenshot.
3. Read the DOM and check for:
   - Max-width constraints on content
   - Consistent spacing and alignment
   - Hover states functional
   - No stretched/distorted elements
4. Record all issues found.

### Step 5 — Generate Report
Create an artifact report in the conversation with:
- **Screenshots** from all 3 breakpoints (embedded)
- **Issues table:** Breakpoint | Element | Issue | Severity
- **Pass/Fail status** for each breakpoint
- **Recommendations** for any failing items

## Report Template

```markdown
# Responsive QA Report — [Page Name]
## Summary: [PASS/FAIL]

### Mobile (375px)
![Mobile screenshot](path)
- Status: PASS/FAIL
- Issues: [list or "None"]

### Tablet (768px)
![Tablet screenshot](path)
- Status: PASS/FAIL
- Issues: [list or "None"]

### Desktop (1440px)
![Desktop screenshot](path)
- Status: PASS/FAIL
- Issues: [list or "None"]

### Issues Detail
| Breakpoint | Element | Issue | Severity |
|-----------|---------|-------|----------|
| Mobile | .navbar | Overflows viewport | HIGH |
```

## Chaining
This workflow can be called at the end of `/design-to-code` for automatic validation.
