---
description: Accessibility Empathy - simulate low-tech user cognition, evaluate cognitive load, and assess inclusive design
---

# SKILL: Accessibility Empathy

**Trigger:** When `@user-tester` evaluates a page from the perspective of non-tech-savvy users (Bác Tư, Chị Lan), or when any agent needs to assess cognitive load and inclusivity.

## 1. Cognitive Load Assessment

Evaluate each page for the three types of cognitive load:

### Intrinsic Load (Complexity of the content itself)
| Rating | Indicators |
|---|---|
| Low | Simple, familiar concepts. Vietnamese language. Familiar cultural references |
| Medium | Some specialized terms (astrology, feng shui) but explained inline |
| High | Dense unexplained jargon, foreign concepts, English-heavy |

### Extraneous Load (Unnecessary complexity from poor design)
| Rating | Indicators |
|---|---|
| Low | Clear layout, logical grouping, consistent patterns, good whitespace |
| Medium | Some visual clutter, inconsistent patterns, minor confusion points |
| High | Overwhelming information density, no visual hierarchy, competing elements |

### Germane Load (Effort to learn the app itself)
| Rating | Indicators |
|---|---|
| Low | Follows common app patterns, self-explanatory, progressive disclosure |
| Medium | Some unique interaction patterns that need learning |
| High | Custom navigation, non-standard gestures, requires tutorial |

## 2. Low-Tech User Simulation Rules

When simulating a non-tech-savvy user:

### Things They DON'T Know
- What a "hamburger menu" () means without a label
- How to swipe to reveal actions
- That they can scroll horizontally
- That tapping a small icon does something
- What technical abbreviations mean (API, PDF, URL)
- That they need to pull-to-refresh

### Things They DO Know
- Tapping big, labeled buttons
- Scrolling vertically
- Looking for Vietnamese text
- Using the back button
- Recognizing common icons (home , settings , search ) **only if large enough**

### Simulation Checklist
For each interaction point, ask:
- [ ] Would Bác Tư find this button/link?
- [ ] Is the text large enough (≥ 16px body, ≥ 14px secondary)?
- [ ] Is the touch target large enough (≥ 44×44px)?
- [ ] Does the label explain itself in Vietnamese?
- [ ] Is there visual feedback when tapped?
- [ ] Can they recover if they tap the wrong thing?

## 3. Information Accessibility Patterns

### Text Readability
| Accessible | Barrier |
|---|---|
| Body text ≥ 16px | Body text < 14px |
| Line height ≥ 1.5 | Cramped line spacing |
| Dark text on light bg (or vice versa) | Low contrast combinations |
| Short paragraphs (3-4 lines) | Wall of text |
| Vietnamese throughout | Unexpected English passages |

### Visual Hierarchy
| Clear | Confusing |
|---|---|
| One primary action per screen | Multiple competing CTAs |
| Section headings visible | Flat, unstructured content |
| Important info at top | Key info buried at bottom |
| Progressive disclosure (show more) | Everything visible at once |

### Navigation Clarity
| Intuitive | Confusing |
|---|---|
| Labeled icons | Icon-only navigation |
| Max 5 nav items visible | 7+ nav items |
| Current page clearly indicated | No active state on nav |
| Breadcrumbs for deep pages | No way to know where you are |

## 4. Cultural Sensitivity Checklist

For Vietnamese metaphysics content specifically:
- [ ] Is the language respectful and culturally appropriate?
- [ ] Are astrological terms in Vietnamese, not Chinese characters or English?
- [ ] Are dates shown in lunar format when contextually appropriate?
- [ ] Do color choices respect cultural associations (red = lucky, white = mourning context)?
- [ ] Is the tone academic and trustworthy, not sensationalist or pop-astrology?

## 5. Empathy Report Template

When this skill is invoked, produce:

```markdown
### Empathy Assessment — [Page/Feature]
**Persona:** [Bác Tư / Chị Lan / etc.]

**Cognitive Load:** Intrinsic: // | Extraneous: // | Germane: //
**Readability:** [assessment]
**Navigation Clarity:** [assessment]
**Cultural Fit:** [assessment]

**Barriers Found:**
1. [Specific barrier and its impact on this persona]

**Recommendations:**
1. [Specific improvement, framed as what the persona needs]
```
