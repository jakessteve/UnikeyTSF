---
description: User Experience Testing - UX scorecard methodology, persona simulation, and experience evaluation techniques
---

# SKILL: User Experience Testing

**Trigger:** When `@user-tester` runs a testing session, when evaluating UX quality of any page or feature, or when @pm delegates a user-perspective review.

## 1. UX Scorecard Methodology

### Scoring Scale (Per Dimension, Per Persona)

Each dimension is scored 1–5 using behavioral anchors:

| Score | Behavioral Anchor |
|---|---|
| **1 — Broken** | User cannot complete the task, leaves in frustration |
| **2 — Painful** | User completes task but with significant difficulty or confusion |
| **3 — Adequate** | User completes task with minor friction, "it works" |
| **4 — Good** | User completes task smoothly, notices quality |
| **5 — Delightful** | User completes task effortlessly, praises the experience |

### Dimension-Specific Criteria

#### Speed (Perceived Performance)
| Score | Criteria |
|---|---|
| 1 | Visible blank screen > 3s, janky animations, layout shifts |
| 2 | Noticeable delay (1-3s), some visual feedback but sluggish |
| 3 | Loads within ~1s, transitions exist but not smooth |
| 4 | Sub-second loads, smooth transitions, meaningful loading states |
| 5 | Instant feel, fluid animations, content pre-loaded or cached |

#### Usability (Navigation & Interaction)
| Score | Criteria |
|---|---|
| 1 | User cannot find the feature, no clear navigation path |
| 2 | Feature findable but requires 4+ taps, confusing labels |
| 3 | Feature reachable in 2-3 taps, some labels unclear |
| 4 | Feature reachable in 1-2 taps, clear labels, intuitive flow |
| 5 | Feature immediately obvious, zero-confusion navigation, excellent affordances |

#### Information Richness (Content Quality)
| Score | Criteria |
|---|---|
| 1 | Missing critical information, blank sections, English-only |
| 2 | Sparse content, key details missing, mixed languages |
| 3 | Core information present, could use more detail |
| 4 | Comprehensive content, well-organized, all in Vietnamese |
| 5 | Rich, layered information with both quick-scan and deep-dive options |

#### Usefulness (Practical Value)
| Score | Criteria |
|---|---|
| 1 | Feature serves no clear user need |
| 2 | Some utility but unclear how to act on the information |
| 3 | Useful information presented, user knows what to do next |
| 4 | Highly actionable, directly helps user make daily decisions |
| 5 | Indispensable — user would miss this feature if removed |

#### UI/UX Polish (Visual & Interaction Quality)
| Score | Criteria |
|---|---|
| 1 | Broken layout, inconsistent styling, amateur appearance |
| 2 | Functional but visually dated, inconsistent spacing/colors |
| 3 | Clean and consistent, meets baseline design expectations |
| 4 | Modern, polished, attention to detail, pleasing animations |
| 5 | Premium feel, delightful micro-interactions, professional-grade design |

## 2. Persona Simulation Technique

When testing as a specific persona:

1. **Enter character.** Read the persona card in `@user-tester.md §2`. Adopt their:
 - Device (set browser viewport accordingly)
 - Tech skill level (avoid actions they wouldn't know)
 - Goals (focus on what they care about)
 - Frustration triggers (be sensitive to these)
2. **Think out loud.** Narrate your experience as the persona would. Use their voice patterns.
3. **Follow their journey.** Don't navigate like a developer — navigate like the persona would. Start from the homepage. Look for obvious entry points.
4. **Note confusion.** Any moment of *"huh?"* is a finding. Record it.
5. **Time yourself.** How long does each task take? Would the persona wait that long?

## 3. First Impression Protocol (5-Second Test)

For each page, execute this rapid assessment:

1. Open the page fresh (clear any prior navigation context)
2. Look at the page for exactly 5 seconds
3. Look away and answer:
 - What is this page about?
 - What is the most prominent element?
 - What would you do first?
 - How does it make you feel?
4. Record as `First Impression` in the finding

## 4. Emotional Context Tags

Real users don't open the app in a vacuum — they have **moods and motivations** that change what "good UX" means. Before each task test, assign an emotional context tag:

| Tag | Emotion | Persona Fit | UX Implications |
|---|---|---|---|
| **Anxious** | Worried, seeking reassurance | Users facing high-stakes decisions | Needs calming language, clear yes/no answers, no ambiguity |
| **Curious** | Exploring for fun, no urgency | New users, browsing users | Tolerates complexity, enjoys depth, wants "wow" moments |
| **Urgent** | Needs an answer NOW | Users under time pressure | Zero patience for loading, needs answer above the fold, minimal steps |
| **Distracted** | Half-paying attention | Users multitasking or on the go | Needs scannable layout, large tap targets, no precision required |
| **Skeptical** | Doesn't trust the app yet | First-time visitors, cautious users | Needs credibility signals, transparency, value before asking for data |

**How to apply:** Before running a task test, state the emotional context:
> *"Testing [Task ID] ([Task description]) as [Persona] in [Emotional tag] mode — [scenario context]."*

This changes the scoring emphasis: in Anxious mode, a Score 3 in Usefulness becomes unacceptable (they need strong, clear guidance), even though the same score is fine in Curious mode.

## 5. Task Completion Testing

Define tasks based on the persona's goals, then measure:

| Metric | How to Measure |
|---|---|
| **Task Success** | Did the persona complete the task? (Yes/No/Partial) |
| **Steps Taken** | Count actual taps/clicks from start to finish |
| **Steps Expected** | Ideal minimum taps/clicks |
| **Time Taken** | Seconds from first action to task completion |
| **Error Rate** | Number of wrong taps/dead-ends encountered |
| **Friction Points** | Specific moments of confusion or frustration |
| **Emotional Fit** | Did the UX match the persona's emotional context? |

### Standard Task Library
Define project-specific tasks based on the PRD and user stories:

| Task ID | Task Description | Expected Steps |
|---|---|---|
| T1 | _[Core task 1 — primary user flow]_ | _[N steps]_ |
| T2 | _[Core task 2 — secondary flow]_ | _[N steps]_ |
| T3 | _[Settings / configuration task]_ | _[N steps]_ |
| T4 | _[Data entry / input task]_ | _[N steps]_ |
| T5 | _[Search / discovery task]_ | _[N steps]_ |
| T6 | _[Account / profile task]_ | _[N steps]_ |

## 6. Trend Tracking

When previous feedback reports exist:

```markdown
## Trend Comparison (vs. YYYY-MM-DD)

| Page | Dimension | Previous | Current | Δ | Trend |
|------|-----------|----------|---------|---|-------|
| [Page 1] | Speed | 3 | 4 | +1 | Improved |
| [Page 2] | Usability | 2 | 2 | 0 | Unchanged |
| [Page 3] | UI/UX | 4 | 3 | -1 | Regressed |

### Notable Changes
- [What improved and why it matters to users]
- [What got worse and impact on user experience]
- [Persistent issues needing attention]
```

## 7. Issue Severity Classification

| Severity | Icon | Criteria | Action |
|---|---|---|---|
| **Critical** | | User cannot complete core task, feature broken | File to `.hc/bugs/`, block deployment |
| **Medium** | | User can complete task but with significant friction | Queue in `.hc/stories/` for next sprint |
| **Minor** | | Polish issue, slight annoyance, cosmetic | Add to design polish backlog |
