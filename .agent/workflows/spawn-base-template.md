---
description: Shared 6-phase structure for all /spawn-* swarm workflows. Individual spawn workflows inherit and customize.
---

# Spawn Swarm — Base Template

> All `/spawn-*` workflows follow this 6-phase pipeline. Each workflow customizes the **Track Table**, **Validation Checklist**, **Cross-Review Questions**, **Synthesis Weights**, and **Report Template** sections.
> This file is **not a workflow itself** — it documents the shared execution protocol.

---

## Phase 1 — DECOMPOSE (Track Planning)

**Lead:** @pm

1. Parse the user's request. Identify focus area.
2. Decompose into **2-5 parallel tracks**, each covering a distinct dimension.
   - Use `task-router` skill to identify relevant domains.
   - Each track must be **independently investigable** (no cross-dependencies during investigation).
3. For each track, define:
   - **Investigation question** (specific, answerable)
   - **Scope** (which files/areas/components to investigate)
   - **Agent role** (which persona perspective is most valuable)
   - **Output format** (Structured JSON schema is MANDATORY for `completion_artifact` to reduce parsing overhead and minimize hallucination)
     ```json
     { "findings": [], "critique": "...", "confidence": 0-100 }
     ```

**Output:** Track plan in `.hc/<domain>/YYYY-MM-DD-[topic]-plan.md`

---

## Phase 2 — SPAWN (Parallel Workers)

**Lead:** @pm (orchestrator)

1. **Assess spawning strategy** using §3.1.8 Mandatory Spawn Gate:

| Track Count | Strategy |
|---|---|
| 2 tracks   | CLI workers OK (parallel, 2 max simultaneous) |
| 3 tracks   | CLI 2-worker waves: spawn 2, wait for completion, 30s cool-down, spawn 1 |
| 4-5 tracks | CLI 2-worker waves with 30s inter-wave cool-down |
| 5+ tracks  | CLI 2-worker waves, batch related tracks, 30s cool-down |

2. **Compose prompts** using appropriate template in `.agent/spawn_agent_tasks/templates/`.
3. **Spawn workers** (or persona-switch) per the chosen strategy.
   - CLI: Use `spawn-agent` skill. **You MUST pass the `-Async` flag** via `spawn-agent.ps1` if spawning 2 workers concurrently to prevent CLI I/O lockups. Pass `-Args "--max-tokens 8192"` (or relevant max token flag) to avoid silent output truncation.
   - Persona-switch: Assume persona → investigate → output strictly in JSON → switch back.
4. **Max parallel workers:** 2 (per `execution-protocol.md` §6). Use wave batching for >2 tracks.
5. **Inter-wave cool-down:** Wait **30 seconds** after all workers in a wave complete before starting the next wave.

**Safety checks (from `anti-patterns-swarm.md`):**
- [ ] Each worker has a clearly scoped investigation question
- [ ] No two workers are investigating the same exact question
- [ ] Each worker prompt includes structured output requirement
- [ ] CRITICAL: Agents MUST NOT use `multi_replace_file_content` concurrently on the same files. Warn workers of strict whitespace sensitivity if background formatters are active.

**Output:** N reports (one per track)

---

## Phase 3 — CRITIQUE & VALIDATE

**Lead:** @pm

1. Gather all reports from spawned workers / persona-switch outputs.
2. **Workers Critique:** Each spawned worker (or persona) must explicitly critique the current state and the initial assumptions within their isolated scope.
3. **Validate completeness** per workflow-specific checklist, ensuring the critique yields actionable insights.
4. Normalize reports into a consistent format for cross-review.

**Output:** Validated reports (with critique) in `.hc/<domain>/YYYY-MM-DD-[topic]-track-N.md`

---

## Phase 4 — CROSS-REVIEW (Round Table)

**Lead:** @pm (Facilitation Mode — `facilitation` skill)

> **Dynamic Skip Rule (ENFORCED):** If the JSON output scopes of the worker tracks are structurally independent and modify/read disjoint file sets, you **MUST SKIP Phase 4** entirely and proceed directly to Phase 5. Do not waste tokens on round-table discussion for independent tracks.

1. **Present each track's findings** to all other domain perspectives.
2. For each track, @pm persona-switches to relevant cross-domain roles and asks workflow-specific cross-review questions.
3. **Discussion protocol** (adapted from `/party-mode`):
   - Max **3 discussion rounds** per conflict/bottleneck.
   - Use interaction markers: ✅ Agree, ❌ Disagree, 🔨 Build, ❓ Question.
   - @pm moderates and redirects off-topic responses.
4. **Surface conflicts/correlations** in a structured log, paying close attention to the earlier critiques.
5. **Unresolved items** carry forward to Phase 5 for PM arbitration.

**Output:** Cross-review summary

---

## Phase 5 — SYNTHESIZE (Critical Thinking Arbitration)

**Lead:** @pm

1. **Apply `critical-thinking-models` skill** (7-model checklist):

| Model | Question |
|---|---|
| First Principles | What fundamental constraints make this recommendation valid? |
| Second-Order Effects | If we adopt this, and then what? |
| Inversion | If this recommendation fails, why? |
| Cynefin | Is this a Clear/Complicated/Complex change? |
| Opportunity Cost | What gets delayed if we do this? |
| Circle of Competence | Is this within our project scope? |
| Cognitive Bias Scan | Am I favoring this because of anchoring/confirmation bias? |

2. **Resolve unresolved items** from Phase 4 using evidence weight.
3. **Rank recommendations** using workflow-specific weighted scoring.
4. **Final prioritization:** Stack-rank into P0 (critical), P1 (significant), P2 (polish), P3 (reject with rationale).

**Output:** Ranked recommendation matrix

---

## Phase 6 — REPORT (Final Summary)

**Lead:** @pm + @ba

1. Write the final report using workflow-specific report template.
2. Save to `.hc/<domain>/YYYY-MM-DD-[topic]-report.md`.
3. Present to user for review.

---

## Output Files (Generic Pattern)
| File | Location |
|------|----------|
| Track plan | `.hc/<domain>/YYYY-MM-DD-[topic]-plan.md` |
| Individual track reports | `.hc/<domain>/YYYY-MM-DD-[topic]-track-N.md` |
| Cross-review summary | `.hc/<domain>/YYYY-MM-DD-[topic]-cross-review.md` |
| Final report | `.hc/<domain>/YYYY-MM-DD-[topic]-report.md` |

---

## Flow Diagram

```
Phase 1: DECOMPOSE (Check current state) ──→ Phase 2: SPAWN (parallel investigation)
                                              │ │ │ │
                                         Track A  B  C  D
                                              │ │ │ │
                                         Phase 3: CRITIQUE & VALIDATE
                                              │
                                         Phase 4: CROSS-REVIEW (Discuss among relevant agents)
                                              │
                                         Phase 5: SYNTHESIZE (PM uses critical thinking to derive best solutions)
                                              │
                                         Phase 6: REPORT (Propose and report in MD)
```
