---
description: Requirement Enrichment — transforms vague user inputs into specific, actionable requirements before delegation
---

# SKILL: Requirement Enrichment

**Trigger:** When @pm receives a vague or ambiguous user request that could be interpreted multiple ways.

---

## When to Use
- User input is fewer than 10 words.
- Input uses subjective terms: "better", "improve", "fix", "nice", "good".
- Input lacks specificity: no file names, no feature names, no measurable outcome.
- Input could map to 3+ different actions (ambiguous intent).

## When to SKIP
- User provides specific files, functions, or error messages.
- Input clearly maps to a single action in the Auto-Delegation Table.
- User follows up on a previous conversation with clear context.

---

## The Enrichment Process

### Step 1: Detect Vagueness
Check the user input against these patterns:

| Pattern | Example | Problem |
|---|---|---|
| **Subjective quality** | "Make the chart better" | Better how? Layout? Colors? Data? Performance? |
| **Unscoped action** | "Fix the app" | Which part? What's broken? |
| **Feature-level request** | "Add sharing" | Share what? To where? What format? |
| **Comparison** | "Make it like app X" | Which aspects? Visual? Features? UX? |
| **Emotional** | "I don't like the design" | What specifically? Colors? Layout? Typography? |

### Step 2: Auto-Enrich OR Ask
Based on context availability:

**Option A: Auto-Enrich (Context Available)**
If you can infer specifics from the current conversation, recent work, or SOT documents:
1. Interpret the vague request into 2-3 specific, measurable actions.
2. Present to the user: "I'll interpret this as: [specific actions]. Proceeding."
3. Delegate the enriched request.

**Option B: Clarify (Context Insufficient)**
If the request is genuinely ambiguous:
1. Ask exactly ONE clarifying question (not a list of 5).
2. Frame it as a choice: "Do you mean A or B?" not "What did you mean?"
3. Provide your best guess as the default: "I'll go with A unless you say otherwise."

### Step 3: Transform to Actionable Sub-Tasks
Convert the enriched request into the `task-router` format:

```markdown
**Original:** "Make the chart better"
**Enriched:** "Improve the Tử Vi chart grid layout for readability"
**Sub-tasks:**
1. Increase font size in palace cells from 11px → 13px (@designer)
2. Add more padding between star names (@designer)
3. Improve center info panel contrast in dark mode (@designer)
4. Verify changes at mobile breakpoints (@qc via browser-visual-testing)
```

---

## Anti-Patterns
| Don't | Do |
|---|---|
| Ask 5 clarifying questions at once | Ask 1 question with a default answer |
| Over-interpret and do something unasked | Present your interpretation before acting |
| Ignore vague input and guess silently | Explicitly state what you'll do |
| Block on clarification for obvious tasks | Auto-enrich when context is sufficient |

## Rules
- **Enrichment is lightweight.** Don't turn a 30-second task into a 5-minute interview.
- **Default to action.** If 80% confident in interpretation, proceed with a note. Don't block.
- **Confidence threshold:** If score ≥ 70 on interpretation → auto-enrich. If < 70 → ask one question.
- **Never enrich clear requests.** "Fix the bug in getLunarDay()" is already specific — just delegate.
