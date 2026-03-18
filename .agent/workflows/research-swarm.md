---
description: Research Swarm - spawn parallel research agents with self-critique, cross-discussion, and critical-thinking synthesis
---

# WORKFLOW: /research-swarm (Multi-Agent Research & Synthesis)

Triggered when @pm receives a research + recommendation request that benefits from parallel investigation across multiple angles.

> **Related workflows:** `/party-mode` (brainstorming), `/swarm-execute` (implementation waves), `/delegate-task` (single worker)
> **Key difference:** This workflow spawns **parallel researchers**, each self-critiques, then PM synthesizes via critical thinking — no existing workflow does all three.

Execute sequentially, respecting gate decisions:

---

## Phase 1 — DECOMPOSE (Research Track Planning)

**Lead:** @pm

1. Parse the user's request. Identify the core research question.
2. Decompose into **2-5 parallel research tracks**, each covering a distinct angle.
   - Use `task-router` skill to identify relevant domains.
   - Each track must be **independently researchable** (no cross-dependencies during research).

**Track Selection Guide:**

| Request Type | Typical Tracks |
|---|---|
| "Suggest upgrades" | Tech stack, Security, Performance, DX, Architecture |
| "Evaluate alternatives" | Feature comparison, Cost analysis, Migration risk, Community health |
| "Improve quality" | Test coverage, Code health, Error handling, Accessibility, Performance |
| "Scale readiness" | Infrastructure, Database, Caching, Monitoring, Cost projection |

3. For each track, define:
   - **Research question** (specific, answerable)
   - **Scope** (which files/areas to investigate)
   - **Agent role** (which persona perspective is most valuable)
   - **Output format** (structured findings + self-critique)

**Output:** Research track plan in `.hc/research/YYYY-MM-DD-[topic]-plan.md`

---

## Phase 2 — SPAWN (Parallel Research Workers)

**Lead:** @pm (orchestrator)

1. **Assess spawning strategy** using §3.1.8 Mandatory Spawn Gate:

| Track Count | Strategy |
|---|---|
| 2-3 tracks | Persona-switching is acceptable (sequential, fast) |
| 4-5 tracks | CLI workers recommended (parallel, context-clean) |
| 5+ tracks | CLI workers MANDATORY, batch related tracks |

2. **Compose prompts** using `.agent/spawn_agent_tasks/templates/research-critique-task.md`.
   - Every prompt MUST include the mandatory self-critique instruction.
   - Every prompt MUST specify the structured output format.

3. **Spawn workers** (or persona-switch) per the chosen strategy.
   - CLI: Use `spawn-agent` skill with `-ApprovalMode Yolo` (research mode).
   - Persona-switch: Assume persona → research → apply `critique` skill → write report → switch back.

4. **Max parallel workers:** 4 (per `execution-protocol.md` §6).

**Safety checks (from `anti-patterns-swarm.md`):**
- [ ] Each worker has a clearly scoped research question
- [ ] No two workers are researching the same exact question
- [ ] Each worker prompt includes self-critique requirement

**Output:** N research reports (one per track)

---

## Phase 3 — COLLECT & VALIDATE

**Lead:** @pm

1. Gather all reports from spawned workers / persona-switch outputs.
2. **Validate completeness:** Each report MUST contain:
   - [ ] Findings section with evidence
   - [ ] Self-critique section (5 Lenses from `critique` skill)
   - [ ] Confidence score (1-10) per finding
   - [ ] Limitations and blind spots acknowledged
3. If any report is missing self-critique → @pm applies `critique` skill to that report.
4. Normalize reports into a consistent format for cross-review.

**Output:** Validated research reports in `.hc/research/YYYY-MM-DD-[topic]-track-N.md`

---

## Phase 4 — CROSS-REVIEW (Round Table Discussion)

**Lead:** @pm (Facilitation Mode — `facilitation` skill)

1. **Present each track's findings** to all other domain perspectives.
2. For each track, @pm persona-switches to relevant opposing roles:
   - Ask: "Does this conflict with Track X's findings?"
   - Ask: "What did this track miss that Track Y found?"
   - Ask: "Are any recommendations mutually exclusive?"

3. **Discussion protocol** (adapted from `/party-mode`):
   - Max **3 discussion rounds** per conflict.
   - Use interaction markers: ✅ Agree, ❌ Disagree, 🔨 Build, ❓ Question.
   - @pm moderates and redirects off-topic responses.

4. **Surface conflicts** explicitly:

```markdown
## Conflict Log
| Track A Finding | Track B Finding | Conflict | Resolution |
|---|---|---|---|
| [finding] | [contradicting finding] | [nature of conflict] | [resolved/unresolved] |
```

5. **Unresolved conflicts** carry forward to Phase 5 for PM arbitration.

**Output:** Cross-review summary with conflict log

---

## Phase 5 — SYNTHESIZE (Critical Thinking Arbitration)

**Lead:** @pm

1. **Apply `critical-thinking-models` skill** (7-model checklist) to the top 5-10 recommendations:

| Model | Question Applied to Each Recommendation |
|---|---|
| First Principles | What constraints make this recommendation valid? |
| Second-Order Effects | If we adopt this, and then what? |
| Inversion | If this recommendation fails, why? |
| Cynefin | Is this a Clear/Complicated/Complex change? |
| Opportunity Cost | What gets delayed if we do this? |
| Circle of Competence | Is this within our project scope? |
| Cognitive Bias Scan | Am I favoring this because of anchoring/confirmation bias? |

2. **Resolve unresolved conflicts** from Phase 4 using evidence weight:
   - Which track had stronger evidence?
   - Which recommendation has lower risk?
   - Which aligns better with project priorities?

3. **Rank recommendations** using weighted scoring:

| Factor | Weight |
|---|---|
| Impact (value delivered) | 40% |
| Effort (cost to implement) | 25% |
| Risk (what could go wrong) | 20% |
| Confidence (evidence quality) | 15% |

4. **Final prioritization:** Stack-rank into P0 (do now), P1 (do soon), P2 (consider later), P3 (reject with rationale).

**Output:** Ranked recommendation matrix

---

## Phase 6 — REPORT (Final Summary)

**Lead:** @pm + @ba

1. Write the final research report:

```markdown
# Research Report — [Topic]
**Date:** YYYY-MM-DD | **Requested by:** User
**Research tracks:** N | **Agents involved:** [list]

## Executive Summary
[3-5 sentences: what was researched, key finding, top recommendation]

## Ranked Recommendations

### P0 — Do Now
| # | Recommendation | Impact | Effort | Risk | Confidence | Track Source |
|---|---|---|---|---|---|---|
| 1 | [title] | High | Low | Low | 9/10 | Track 2 |

### P1 — Do Soon
[same table format]

### P2 — Consider Later
[same table format]

### P3 — Rejected (with rationale)
[same table format]

## Conflicts Resolved
[From Phase 4 conflict log — how each was resolved]

## Methodology
- Tracks researched: [list with 1-line description each]
- Self-critique applied: Yes (5 Lenses)
- Cross-review rounds: [N]
- Critical thinking models applied: [list which of the 7]

## Limitations
[What this research did NOT cover, suggested follow-ups]

## Appendix: Full Track Reports
[Links to individual track reports in .hc/research/]
```

2. Save to `.hc/research/YYYY-MM-DD-[topic]-report.md`.
3. Present to user for review.

---

## Output Files
| File | Location |
|------|----------|
| Research track plan | `.hc/research/YYYY-MM-DD-[topic]-plan.md` |
| Individual track reports | `.hc/research/YYYY-MM-DD-[topic]-track-N.md` |
| Cross-review summary | `.hc/research/YYYY-MM-DD-[topic]-cross-review.md` |
| Final report | `.hc/research/YYYY-MM-DD-[topic]-report.md` |

---

## Flow Diagram

```
Phase 1: DECOMPOSE ──→ Phase 2: SPAWN (parallel)
                            │ │ │
                       Track A  Track B  Track C
                       + critique + critique + critique
                            │ │ │
                       Phase 3: COLLECT & VALIDATE
                            │
                       Phase 4: CROSS-REVIEW (round table)
                            │
                       Phase 5: SYNTHESIZE (critical thinking)
                            │
                       Phase 6: REPORT
```
