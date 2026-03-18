---
description: Idea Forge - full dialectical development cycle from brainstorm through adversarial validation to implementation and review
---

# WORKFLOW: /idea-forge (Dialectical Development Cycle)

Full 7-phase cycle for developing ideas from brainstorm through adversarial stress-testing, implementation, and post-build review. This workflow embodies the principle that **ideas must survive debate before and after implementation**.

> **When to use:** For significant features, architectural decisions, or any idea that warrants structured scrutiny. For trivial tasks, use @pm fast-path instead.

> **Single-LLM Execution Model:** "Call @agent" means **assume that agent's persona** and execute using available tools, same as `/hc-sdlc`.

Execute sequentially, respecting gate decisions:

---

## Phase 1 — Group Brainstorm

**Lead:** @pm (Facilitation Mode)

1. Trigger `/party-mode` Steps 1-4:
 - Topic intake → Agent selection → Round table → Discussion (3 rounds max)
2. Produce initial consensus or 2-3 candidate ideas.
3. If multiple ideas: apply `idea-validation` skill → rank by DFV score → select top candidate(s).

**Output:** Brainstorm artifact in `.hc/brainstorms/YYYY-MM-DD-[topic].md`

---

## Phase 2 — Red Team (Stress-Test Ideas)

**Lead:** Rotated agent (see `red-team-ideas` skill)

1. @pm assigns a Devil's Advocate using the rotation table in `red-team-ideas` skill.
2. Red Team lead executes the full process:
 - Pre-mortem analysis (3 failure scenarios minimum)
 - Assumption mapping (challenge every unstated assumption)
 - Edge-case ideation (scale, abuse, regression, maintenance, reversal)
3. Red Team delivers verdict: `PROCEED` / `PROCEED WITH CHANGES` / `RETHINK` / `REJECT`

**Gate Decision:**
| Verdict | Action |
|---|---|
| PROCEED | Continue to Phase 3 |
| PROCEED WITH CHANGES | Modify idea per Red Team recommendations → Phase 3 |
| RETHINK | Return to Phase 1 with Red Team findings as new input |
| REJECT | Log rejection rationale → Exit workflow |

**Output:** Red Team report in `.hc/red-team/YYYY-MM-DD-[topic].md`

---

## Phase 3 — Iterate & Refine Design

**Lead:** @sa + @pm

1. Incorporate Red Team modifications into the idea.
2. @sa drafts technical approach (lightweight — pseudocode level, not full ARCHITECTURE.md).
3. @pm validates the refined idea still meets the original user need.
4. If the idea fundamentally changed: run a **mini-brainstorm** (1 round of `/party-mode` only) to confirm team alignment.

**Output:** Refined design brief in `.hc/designs/YYYY-MM-DD-[topic].md`

---

## Phase 4 — Implement

**Lead:** @dev (+ @designer if UI-involved)

1. Route to `/hc-sdlc` for full pipeline, OR use @pm fast-path (§3.4) for ≤3-file changes.
2. Follow standard TDD flow: @qc writes test scaffolds → @dev implements → @qc verifies.
3. @qc runs `verification-before-completion` skill.

**Output:** Working code + passing verification report

---

## Phase 5 — Battle Test

**Lead:** @qc

1. Execute `/battle-test` workflow against the implemented feature.
2. Record edge-case results, resilience score.

**Gate Decision:**
| Result | Action |
|---|---|
| Score A-B (≥75%) | Continue to Phase 6 |
| Score C (50-74%) | @dev fixes failures → re-test → then Phase 6 |
| Score D-F (<50%) | REDESIGN — return to Phase 2 with battle-test findings |

**Output:** Battle test report

---

## Phase 6 — Debate Implementation

**Lead:** @sa + @dev + @qc (roundtable)

1. Execute `implementation-debate` skill:
 - Intent vs. Reality check (does the build match the brainstorm decision?)
 - Tech debt assessment
 - Alternative approach discussion
2. Deliver verdict: `SHIP` / `SHIP WITH DEBT` / `REFACTOR` / `RETHINK`

**Gate Decision:**
| Verdict | Action |
|---|---|
| SHIP | Continue to Phase 7 |
| SHIP WITH DEBT | Continue to Phase 7 + create debt stories |
| REFACTOR | @dev refactors → re-run Phase 5 & 6 |
| RETHINK | Return to Phase 1 with review findings |

**Output:** Implementation review in `.hc/reviews/YYYY-MM-DD-[topic].md`

---

## Phase 7 — Log Conclusion & Next Steps

**Lead:** @pm + @ba

1. @ba writes a conclusion summary capturing:
 - Original idea → how it evolved through debate → what was actually shipped
 - Key decisions and their rationale
 - Red Team findings that shaped the final output
 - Tech debt accepted (if any)
2. @pm updates the roadmap and creates follow-up stories if needed.
3. Trigger `/retrospective` if this was a significant feature.

**Output:** 
- Conclusion in `.hc/conclusions/YYYY-MM-DD-[topic].md`
- Updated roadmap in `.hc/roadmap.md`
- Follow-up stories in `.hc/stories/` (if applicable)

---

## Flow Diagram

```
Phase 1: Brainstorm ──→ Phase 2: Red Team ──→ Phase 3: Refine Design
 │ RETHINK │
 └───────────────────────┘ 
 Phase 4: Implement
 │
 
 Phase 5: Battle Test ──────┘
 │ REDESIGN │
 └───────────────────────┘
 │ PASS
 
 Phase 6: Debate Implementation
 │ RETHINK │
 └───────────────────────┘
 │ SHIP
 
 Phase 7: Log & Close
```

---

## Output Files
| File | Location |
|---|---|
| Brainstorm artifact | `.hc/brainstorms/YYYY-MM-DD-[topic].md` |
| Red Team report | `.hc/red-team/YYYY-MM-DD-[topic].md` |
| Design brief | `.hc/designs/YYYY-MM-DD-[topic].md` |
| Implementation review | `.hc/reviews/YYYY-MM-DD-[topic].md` |
| Conclusion | `.hc/conclusions/YYYY-MM-DD-[topic].md` |
| Follow-up stories | `.hc/stories/` |
