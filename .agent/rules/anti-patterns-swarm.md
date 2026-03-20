---
description: Swarm-Only — multi-agent coordination safety, cognitive bias detection, and asset burn guards. Loaded during /swarm-execute and parallel multi-agent workflows only.
---

# RULE: ANTI-PATTERNS (Swarm & Advanced)

> **Load condition:** Only during `/swarm-execute`, parallel multi-agent execution, or when @pm detects cognitive bias signals. NOT loaded for single-agent tasks.

---

## 7. Swarm Clash Prevention (Multi-Agent Coordination Safety)

These rules are MANDATORY during `/swarm-execute` and any parallel multi-agent execution.

### 7.1 File Ownership Protocol (Anti-Dual-Write)
During parallel waves, each file MUST have a single owner. @pm MUST produce a File Ownership Map in Phase 1 (Decomposition) before any waves begin, to eliminate lock-sync waits.

**Rules:**
- Only the owner may WRITE to a file. Others may READ only.
- If an agent needs to modify a file they don't own → request via @pm.
- Shared files (`package.json`, `tsconfig.json`) → @pm batches changes after the wave.

### 7.2 Circular Handoff Detection
If a task is delegated in a cycle (A→B→C→A) without any tool execution between handoffs → HALT immediately. @pm identifies the missing information causing the loop.

### 7.3 Dependency Deadlock Detection
Before each wave, identify inter-agent dependencies as a directed graph. If the graph contains a cycle → break it: @pm decides which agent goes first with a stub/mock.

**Timeout:** If any agent waits > 10 consecutive tool calls without expected input → trigger deadlock check.

### 7.4 Hallucination Firewall (Cross-Agent Validation)
When receiving a claim from another agent, the receiving agent MUST verify it exists before using it. If it doesn't → flag with ` CROSS-AGENT HALLUCINATION` and bounce back.

### 7.5 Priority-Aware Scheduling

| Priority | Examples | Rule |
|---|---|---|
| P0 (Critical) | Production bug, security fix | Gets first slot, can preempt P2 |
| P1 (High) | Feature in current sprint | Gets next available slot |
| P2 (Low) | Refactoring, cleanup, docs | Runs only when P0/P1 free |

A P2 task MUST NOT consume the last available parallel slot.

### 7.6 Agent Veto Protocol (Anti-Coup)
- Veto MUST include specific evidence + remediation path.
- **Max 2 veto rounds** per task → then escalate to User.
- A security agent MUST NOT unilaterally delete or revert another agent's code.

### 7.7 Anti-Swarm-Extravagance (Token Burn Guard)
- **Max 4 CLI workers per wave.** Batch smallest/related tasks into single prompts.
- All workers use `.agent/AGENTS-LITE.md` as their context rulebook (not full `AGENTS.md`).
- Mechanical workers: `-ApprovalMode Yolo -Timeout 60`.
- If decomposition yields >6 tasks → HALT → re-batch → reduce to ≤4 workers.

---

## 8. Cognitive Bias Detection (Runtime Safety Net)

| Bias | Runtime Signal | Auto-Response |
|---|---|---|
| **Sunk Cost** | Spent >50% tool budget on failing approach; resists switching | HALT. "If starting fresh, would I pick this?" If no → switch. |
| **Confirmation** | Only reading files supporting current approach; ignoring failing tests | Force-read failing output. List 1 reason approach might be wrong. |
| **Anchoring** | First result adopted without comparing alternatives | Must find ≥2 approaches (Medium+ tasks). |
| **Status Quo** | Avoiding rewrite despite known tech debt flagged by @qc | "Would I design it this way from scratch?" |
| **Recency** | Preferring latest pattern over proven project conventions | Check project's existing patterns first. |
| **Bandwagon** | Adopting popular approach without verifying fit | Check `instructions.md` and `engineering-mindset.md`. |

**When to Check:** At 75% tool budget checkpoint, on approach switches, on ≥15-point confidence drops.

---

---

## 11. Inline Execution Bias — "The Convenience Trap"

| Signal | Severity |
|---|---|
| @pm edits 4+ source files without spawning CLI workers | **Process violation** |
| @pm says "it's faster inline" for Medium+ task | **Status Quo Bias** |
| @pm classifies multi-domain task as "simple" | **Scope underestimation** |
| @pm skips writing plan AND skips spawning for 7+ files | **Double bypass** |

**Response:** HALT → Route through `@pm.md` §3.1.8.

---

## 13. Asset Generation Burn Guard

- **Max 5 image generations per conversation.** Batch related assets.
- **Max 2 MB total generated artifact size per conversation.**
- Multiple related assets → generate in single batched prompt.

---

## 14. Guardrail Effectiveness Tracking

When ANY guardrail fires, log to `.agent/benchmarks/guardrail-hits.md`.
Every 20 conversations, review: 0 hits → simplify. 5+ hits → strengthen. >50% false positive → calibrate.
