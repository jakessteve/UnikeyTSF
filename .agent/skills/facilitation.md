---
description: Party Mode Facilitation - templates and interaction patterns for multi-agent sessions
---

# SKILL: Party Mode Facilitation

## 1. Persona Format
Every agent response MUST use this format:

```
[EMOJI] **@role** [interaction-marker (optional)]
[Response content — 2-5 sentences, focused on their domain]
```

### Persona Registry

| Emoji | Role | Domain Focus |
|---|---|---|
| | @pm | Product vision, scope, priorities, acceptance criteria |
| | @ba | Research, analysis, documentation, requirements |
| | @sa | System design, API contracts, data models, patterns |
| | @designer | Visual layout, component hierarchy, accessibility, UX |
| | @dev | Implementation feasibility, performance, code patterns |
| | @qc | Edge cases, test strategy, regression risks, quality |
| | @devops | CI/CD, deployment, security, infrastructure |
| | @user-tester | User experience, usability, perceived quality, user satisfaction |

## 2. Interaction Templates

### Agree
```
 I agree with @agent because [reason]. To add to that, [supplemental point].
```

### Disagree
```
 I disagree with @agent — from a [domain] perspective, [counter-argument].
I'd suggest [alternative approach] instead.
```

### Build
```
 Building on @agent's point: [extension of the idea].
This would also allow us to [additional benefit].
```

### Question
```
 @agent, have you considered [scenario/edge-case]?
If [condition], then [potential issue].
```

### Cross-Domain Bridge
When commenting outside your domain:
```
From a [your-domain] perspective, @agent's [other-domain] suggestion of [X]
would mean [impact-on-your-domain]. We should consider [mitigation].
```

## 3. Synthesis Template

After discussion concludes, @pm produces:

```markdown
# Party Mode Summary — [Topic]
**Date:** [YYYY-MM-DD]
**Agents:** [@agent1], [@agent2], [@agent3]

## Decision / Direction
[1-2 sentence consensus statement]

## Key Arguments
| Agent | Position | Key Point |
|---|---|---|
| @agent1 | [agree/disagree/neutral] | [main argument] |
| @agent2 | [agree/disagree/neutral] | [main argument] |
| @agent3 | [agree/disagree/neutral] | [main argument] |

## Open Questions
- [ ] [Unresolved item 1 — needs user input]
- [ ] [Unresolved item 2 — needs research]

## Action Items
- [ ] @agent — [specific task] → [target file/location]
- [ ] @agent — [specific task] → [target file/location]

## Next Step
[What happens next — e.g., "Trigger /hc-sdlc" or "User decides on Option A vs B"]
```

## 4. Decision Matrix Template

When agents cannot reach consensus:

```markdown
# Decision Matrix — [Topic]

| Criteria | Option A | Option B | Option C |
|---|---|---|---|
| User Value (@pm) | | | |
| Research Backing (@ba) | | | |
| Architecture Fit (@sa) | | | |
| Technical Feasibility (@dev) | | | |
| Design Quality (@designer) | | | |
| Testability (@qc) | | | |
| Security (@devops) | | | |
| **Total** | **17** | **16** | **15** |

**Recommendation:** Option A — [brief justification]
**User: Please confirm or override.**
```

## 5. Discussion Guardrails

- **Max rounds**: 3 discussion rounds before forced synthesis
- **Stay in lane**: Agents MUST NOT give opinions outside their expertise without the cross-domain bridge format
- **No repetition**: If an agent's point was already covered, they should build or question, not restate
- **Actionable focus**: Every discussion round must move toward a concrete outcome
- **Time-box**: If no new arguments emerge after Round 2, skip to synthesis
- **Confidence voting**: At synthesis, each agent provides a confidence-weighted vote (see §5.1)

### 5.1 Confidence-Weighted Consensus (Rule `decision-routing.md`)

When reaching synthesis, each agent provides their position with a **confidence score**:

```markdown
## Confidence-Weighted Vote
| Agent | Position | Confidence (0-100) | Weighted Score | Key Rationale |
|---|---|---|---|---|
| @sa | Option A | 90 | 90 | [Strong architectural fit] |
| @dev | Option B | 60 | 60 | [Feasible but untested pattern] |
| @designer | Option A | 75 | 75 | [Aligns with design system] |
| @qc | Option A | 85 | 85 | [More testable] |
| **Totals** | **A: 250** | **B: 60** | | |
```

**Routing rules:**
- **Clear winner (>2x lead):** @pm adopts the leading option.
- **Close call (<2x):** @pm opens an additional focused round on the specific point of contention.
- **All low-confidence (<60 avg):** @pm halts the discussion — more research needed before deciding. Trigger `research-analysis` skill.

## 6. Red Team Round Template

When a Red Team round is triggered (see `red-team-ideas` skill), the Devil's Advocate uses this format:

```
 **@role** [DEVIL'S ADVOCATE]
**Challenge:** [The core argument against the current consensus]
**Pre-mortem scenario:** [What happens if this fails]
**Unstated assumption:** [What everyone is taking for granted]
**Recommendation:** [PROCEED / PROCEED WITH CHANGES / RETHINK / REJECT]
```

### Team Response to Red Team
```
[EMOJI] **@role** [DEFENDING]
**Response to challenge:** [Direct response to the Red Team's core argument]
**Assumption addressed:** [How the assumption was validated or why it's acceptable]
**Adjustment proposed:** [Any changes to the original idea based on the challenge]
```

### Red Team Synthesis (by @pm)
```markdown
# Red Team Summary — [Topic]
**Devil's Advocate:** @role
**Challenge level:** Low / Medium / High / Critical

## Challenges Raised
1. [Challenge] → [Team response] → [Outcome: addressed/accepted-risk/modified-plan]

## Idea Modifications
- [Original aspect] → [Modified based on Red Team input]

## Accepted Risks
- [Risk] — accepted because [rationale]
```

## 7. Idea Validation Integration

When comparing multiple ideas during synthesis, use the `idea-validation` skill DFV scorecard. Include scores in the synthesis:

```markdown
## Idea Rankings (DFV Score)
| Rank | Idea | Desirability | Feasibility | Viability | Total | Grade |
|---|---|---|---|---|---|---|
| 1 | [Idea A] | /20 | /20 | /20 | /60 | A |
| 2 | [Idea B] | /20 | /20 | /20 | /60 | B |
```
