---
description: Always On — verification gates, iteration budgets, commit discipline, agent coordination, and CLI worker governance
---

# RULE: EXECUTION PROTOCOL

All agents MUST follow these execution protocols. This file combines verification gates, iteration budgets, coordination rules, and commit discipline.

---

## 1. SOT-Driven Development

All work references **Source of Truth** documents. **Before coding:** read SOT. **After coding:** update SOT if deviated. **If conflict:** SOT wins.

**SOT Documents:** `docs/biz/PRD.md`, `docs/tech/ARCHITECTURE.md`, `docs/tech/API_CONTRACTS.md`, `WIREFRAMES.md` (created on-demand).

---

## 2. SPARC Compliance

Follow SPARC phases: Specification → Pseudocode → Architecture → Refinement → Completion.

- Do NOT skip phases (use `/hc-sdlc`). Fast-path (≤3 files, single concern) may skip to Refinement.
- For **Medium+ tasks**, use `amateur-proof-plans` skill during Specification.

### Project-Type Adaptation
Detect project type from config files (`package.json`, `tsconfig.json`, `CMakeLists.txt`, etc.) and adapt SPARC gates accordingly:
- **Frontend SPA:** wireframe/story sufficient for S→P; TypeScript interfaces serve as implicit contracts for A→R
- **Backend API:** API contracts required for A→R; security gate mandatory for auth/data flows
- **Full-stack:** both frontend and backend gates apply
- Security Gate: required only for user data, external APIs, or auth flows

### Upstream Gates (P1+ features only)
- **Market Validation:** `.hc/business/research/` must exist with GO/CONDITIONAL GO.
- **Discovery:** `.hc/discovery/` must exist with composite score ≥ 4.0.
- **Exception:** Bug fixes, tech debt, P2 minor features skip upstream gates.

---

## 3. Verification Gate (Tiered)

Match verification tier to task complexity (see `decision-routing.md`):

| Tier | Scope | Steps | QC Switch? |
|---|---|---|---|
| **Trivial** (≤1 file, ≤10 lines) | Typo/config | @dev-fe/@dev-be self-verify: tests + tsc (Bypass SOT update) | No |
| **Small** (≤3 files) | Bug fixes | @dev-fe/@dev-be + @qc spot-check (Bypass SOT update unless arch changed) | Minimal |
| **Standard** (4-6 files) | Pattern features | @dev-fe/@dev-be + @sa arch review (if needed) + @qc | Minimal |
| **Medium** (7-10 files) | Novel features | Full: @dev-fe/@dev-be → @sa arch review → @qc → @pm synthesis | Full |
| **Large** (>10 files) | Epics | Full + `@sa arch review` + `/implementation-review` | Full |

### Standard Steps (Medium+)
1. Tests pass (`npm test`), lint passes (`npm run lint`), tsc clean (`npx tsc --noEmit`)
2. Visual verification for UI changes (browser screenshot)
3. SOT documents updated. Use `verification-before-completion` skill.

### Cross-Verification Gates
- **Security (Large tasks):** @devops fixes → @whitehat-hacker independently re-tests.
- **UX (ANY UI change):** @dev-fe implements → @user-tester evaluates via `/user-test-session`.
- **Bug Fix:** Bug record in `.hc/bugs/` required → @dev-fe/@dev-be fixes → @qc re-verifies → bug record updated.
- **UAT Done-Gate:** Critical findings block Done. Medium/Minor queued as stories.

---

## 4. Iteration Budget

Every task has a max tool-call budget. This prevents runaway sessions.

| Task Type | Max Calls | Checkpoint At |
|---|---|---|
| Quick fix (≤3 files) | 20 | 15 |
| Standard (4-6 files) | 30 | 22 |
| Feature (7-10 files) | 40 | 30 |
| Epic (>10 files) | 60 | 45 |
| Orchestration / Swarm | 80 | 60 |

**At checkpoint:** Summarize progress, assess remaining work vs budget. If >50% work remains with <25% budget → trigger Context Reset (`anti-patterns-core.md` §4).

**Exceeding max:** STOP and escalate to @pm (or User if @pm is stuck).

---

## 5. Agent Communication

Inter-agent communication uses structured format:
```
**FROM:** @agent | **TO:** @agent | **RE:** [Subject]
**CONTENT:** [Message] | **ACTION NEEDED:** [Yes/No — what]
```

Team-mode (party/swarm): Use `🎯 **@pm:**` emoji-prefixed format. Max 3 discussion rounds before synthesis.

---

## 6. Parallel Resource Limits

- Max **4 parallel agent threads** (hard limit).
- Max **5 agents per swarm wave** (hard cap).
- Queue additional work if capacity reached.

---

## 7. Auto-Fallback & Recovery

When an agent is stuck: **Detect** (3x expected time or anti-loop trigger) → **Kill** → **Reset** (re-read task from SOT) → **Replace** (fundamentally different approach) → escalate to User if still failing.

---

## 8. Sub-Agent Spawn Limits

- **Max spawn depth:** 2 levels (orchestrator → agent → sub-task).
- **Max total active agents per task:** 5.
- **Max sequential delegations without execution:** 3 → HALT.

### CLI Worker Rules (Gemini CLI via spawn-agent)
- CLI workers count toward all limits above. Each process = one agent.
- Workers MUST NOT spawn sub-agents. They are terminal nodes.
- **Context injection:** Use role-scoped LITE files: `AGENTS-LITE-dev.md`, `AGENTS-LITE-qc.md`, `AGENTS-LITE-pm.md`, `AGENTS-LITE-designer.md`, `AGENTS-LITE-sa.md`, `AGENTS-LITE-devops.md`, or generic `AGENTS-LITE.md`. **Never** full `AGENTS.md`.
- **Skill lazy-loading mandatory:** Read MANIFEST → view_file only needed skills.
- **Output review mandatory:** Read output logs AND run `git diff`. Never assume success.
- **One task per spawn.** Review output between spawns.
- **Failed worker = orchestrator takes over.** Fix prompt or complete inline.

---

## 9. Coordination Model

### Persona-Switching (Trivial/Small — ≤3 files)
Zero handoff cost, full context sharing, sequential only.

### Lightweight Delegation (Standard — 4-6 files)
Inline prompt via `-Prompt`, quick validation only, orchestrator reviews `git diff` directly.

### CLI Worker Delegation (Medium+ — DEFAULT)
True process isolation, true parallelism, workers start cold. Default for 7+ files.

**Decision rule:** Default to CLI workers for Medium+. Lightweight for Standard. Persona-switching ONLY for Trivial/Small.

> **Analysis/review tasks** touching zero files but requiring 3+ perspectives are effectively Large+. Route through Mandatory Spawn Gate.

---

## 10. Commit Discipline

- **Atomic commits:** One logical change per commit.
- **Conventional format:** `type(scope): description`.
- **Types:** `feat`, `fix`, `refactor`, `test`, `docs`, `chore`, `style`, `perf`.
- **Never commit:** Broken tests, lint errors, `console.log` debugging, secrets.
