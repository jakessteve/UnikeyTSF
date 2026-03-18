---
description: Mobile UX Patterns - touch targets, gestures, progressive disclosure, and mobile-first design
---

# SKILL: Mobile UX Patterns

**Trigger:** When @designer designs for mobile or @dev implements mobile-responsive components.

---

## When to Use
- Designing new UI components for mobile viewports.
- Adapting desktop layouts for mobile.
- Fixing mobile usability issues.
- Preparing for mobile app ports (React Native, Flutter).

---

## Core Mobile UX Principles

### 1. Touch Targets
All interactive elements MUST meet minimum touch target sizes:

| Standard | Minimum Size | Padding |
|---|---|---|
| Apple HIG | 44 × 44 pt | 8pt between targets |
| Material Design | 48 × 48 dp | 8dp between targets |
| WCAG 2.5.8 (AAA) | 44 × 44 CSS px | — |
| **Our Standard** | **44 × 44 px** | **8px between targets** |

**Common violations:**
- Icon buttons without padding (fix: add `padding: 12px`)
- Close/dismiss buttons in corners (fix: enlarge hit area)
- Dense table rows (fix: increase row height to 48px+)
- Inline links in text blocks (fix: increase line-height)

### 2. Thumb Zone Design
Design for one-handed use (right thumb):

```
┌─────────────────────┐
│   Hard to reach     │  ← Nav bar, secondary actions
│                     │
│   Comfortable       │  ← Primary content, tabs
│                     │
│   Easy to reach     │  ← Primary actions, FAB
│                     │
│   [Tab Bar / Nav]   │  ← Bottom navigation
└─────────────────────┘
```

**Rules:**
- Primary actions → bottom of screen
- Navigation → bottom tabs (not top hamburger for key nav)
- Destructive actions → require confirmation, place in hard-to-reach zone

### 3. Progressive Disclosure
Show less, reveal more on demand:

| Pattern | When | Implementation |
|---|---|---|
| Collapsible sections | Dense content | `<CollapsibleCard>` component |
| Read more / truncation | Long text | Show 3 lines + "Xem thêm" |
| Drill-down navigation | Hierarchical data | Master → detail pattern |
| Bottom sheets | Contextual options | Slide-up panel on action |
| Tabs | Parallel content | Sub-navigation tabs |

### 4. Gesture Support
| Gesture | Use For | Example |
|---|---|---|
| Swipe left/right | Navigate between items | Calendar month navigation |
| Pull to refresh | Data refresh | Reload daily info |
| Long press | Contextual menu | Show options for a date |
| Pinch to zoom | Detailed view | Tử Vi chart zoom |
| Swipe down | Dismiss | Close bottom sheet |

### 5. Form Design for Mobile
- [ ] Use appropriate input types (`type="date"`, `type="tel"`, `inputmode="numeric"`)
- [ ] Labels above inputs (not beside — saves horizontal space)
- [ ] Large submit buttons (full width, 48px+ height)
- [ ] Show validation inline (below the field)
- [ ] Auto-advance between fields where logical
- [ ] Use native pickers for date/time selection

### 6. Performance on Mobile
- [ ] Images optimized (WebP, responsive srcset)
- [ ] Lazy load below-fold content
- [ ] Minimize JavaScript bundle (mobile networks are slower)
- [ ] Use skeleton loaders instead of spinners
- [ ] Avoid layout shifts (set explicit dimensions on images/containers)

---

## Mobile-Specific Checklist
- [ ] Touch targets ≥ 44 × 44 px
- [ ] Primary action in thumb zone (bottom of screen)
- [ ] No horizontal scroll on any viewport < 375px
- [ ] Text readable without zooming (min 14px body text)
- [ ] Forms use correct input types and modes
- [ ] Navigation accessible via bottom tabs or hamburger
- [ ] Dense content progressively disclosed
- [ ] Loading states use skeleton loaders
