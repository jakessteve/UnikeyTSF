---
description: Structured Playwright E2E testing — auto-detect server, write scripts, capture evidence
---

# SKILL: QC PLAYWRIGHT TESTING

**Trigger:** When @qc needs to write or run end-to-end browser tests, or when `/run-e2e-qa` workflow is triggered.

---

## When to Use
- Testing user flows (click, type, navigate, verify).
- Verifying responsive layout at different breakpoints.
- Checking that UI matches design specifications.
- Capturing screenshots for visual regression evidence.
- Testing interactive elements (dropdowns, modals, calendars).

---

## Setup

### Auto-Detect Dev Server
Before running tests, verify the dev server is running:
```bash
# Check if dev server is already running
curl -s http://localhost:5173 > /dev/null && echo "Server running" || echo "Start server first"
```

If not running:
```bash
npm run dev &
# Wait for server to be ready
sleep 3
```

---

## Test Script Structure

### Basic Flow Template
```typescript
// Test: [Flow Name]
// Use browser_subagent for execution

const flow = {
 name: '[Flow Name]',
 steps: [
 { action: 'navigate', url: 'http://localhost:5173' },
 { action: 'wait', selector: '[data-testid="main-content"]' },
 { action: 'click', selector: '[data-testid="tab-tuvi"]' },
 { action: 'verify', selector: '.tuvi-grid', exists: true },
 { action: 'screenshot', name: 'tuvi-grid-loaded' },
 ]
};
```

### User Interaction Patterns

| Pattern | How to Test |
|---------|------------|
| **Navigation** | Click tabs/links → verify URL + content changes |
| **Form input** | Type into fields → submit → verify result/response |
| **Dropdown/Select** | Click dropdown → select option → verify selection |
| **Modal** | Trigger modal → verify visible → close → verify closed |
| **Calendar** | Click date → verify selection → check detail panel |
| **Responsive** | Resize viewport → verify layout changes |

### Responsive Breakpoints
Test at these standard widths:
- **Mobile:** 375px (iPhone SE)
- **Tablet:** 768px (iPad)
- **Desktop:** 1280px (Laptop)
- **Wide:** 1920px (Full HD)

---

## Evidence Collection

### Screenshot Rules
- Capture **before and after** for state changes.
- Name screenshots descriptively: `[flow]-[state]-[viewport].png`
- Save to `.hc/qa/screenshots/`

### Test Report Format
```markdown
# E2E Test Report — [Date]

## Flows Tested
| # | Flow | Steps | Result | Screenshot |
|---|------|-------|--------|-----------|
| 1 | [Name] | X | / | [link] |

## Failures
### [Flow Name] — Step X
- **Expected:** [what should happen]
- **Actual:** [what happened]
- **Screenshot:** [link]
- **Severity:** Critical / Major / Minor

## Coverage
- Flows tested: X/Y
- Breakpoints checked: [list]
- Edge cases: [list]
```

---

## Rules
- **Always capture screenshots** for visual verification — Rule `execution-protocol.md`.
- **Test the happy path first**, then edge cases.
- **Test at mobile width** for mobile-first apps.
- **Report failures with screenshots** — every bug report needs visual evidence.
