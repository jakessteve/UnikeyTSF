---
description: HC Software Development Life Cycle - sequential pipeline aligned with SDLC, Waterfall, and SPARC
---

# WORKFLOW: /hc-sdlc (HC Software Development Life Cycle)

Sequential pipeline aligned with **SDLC**, **Waterfall** (gates), and **SPARC** (Specification → Pseudocode → Architecture → Refinement → Completion).

Triggered by @pm upon receiving a new Epic/Phase request.

> **Single-LLM Execution Model:** In this workflow, "Call @agent" means **assume that agent's persona** (read `.agent/roles/@agent.md`), then **execute their duties using available tools** (see Tool Binding Table in `@pm.md` §3.2). You are a single LLM wearing different hats — not spawning separate processes.

> **Fast-Path:** For simple tasks (≤3 files, single concern), skip this pipeline. Use `@pm.md` §3.3 Fast-Path instead.

Execute sequentially, DO NOT skip steps:

---

## SPARC: Specification (SDLC: Planning + Analysis) — `GEMINI-H/Plan`

1. **[RESEARCH]:** Assume **@ba persona** → read `.agent/roles/@ba.md` → investigate the domain using `search_web`, `context7`, `view_file` → write `docs/biz/PRODUCT_BRIEF.md`. Use research-analysis skill.
2. **[REQUIREMENTS]:** Switch to **@pm persona** → read the brief → write `docs/biz/PRD.md`, define acceptance criteria, break scope into stories in `.hc/stories/`.

### ⇄ Model Handoff Gate: S → P (GEMINI → OPUS)
> If the current session is on **Gemini** (ideal for Specification research), the **Pseudocode** phase benefits from **Opus-level reasoning**. Run `model-selector` Step 5. If a handoff is warranted, execute `/handoff` workflow and instruct the user to switch to Opus for the next phase. If already on Opus, skip this gate.

## SPARC: Pseudocode (SDLC: Design) — `OPUS/Plan`

3. **[PSEUDOCODE]:** Assume **@sa persona** → read `.agent/roles/@sa.md` → design high-level algorithm flows, data structures, and logic plans using `sequential-thinking` and `view_file`. Save to `.hc/pseudocode/`. NO architecture decisions yet — only logic flow.

## SPARC: Architecture (SDLC: Design) — `OPUS/Plan`

4. **[ARCHITECTURE]:** Assume **@sa persona** → design ERD in `docs/tech/ARCHITECTURE.md` and define ALL JSON payloads in `docs/tech/API_CONTRACTS.md`. Architecture MUST align with pseudocode.
5. **[DESIGN]:** Assume **@designer persona** → read `.agent/roles/@designer.md` → draft visual layouts and Component Trees in `WIREFRAMES.md`. Use `browser_subagent` for visual references if needed.

## Security Gate (Waterfall Gate #1) — `SONNET/Fast`

6. **[SECURITY REVIEW]:** Assume **@devops persona** → read `.agent/roles/@devops.md` → review architecture, API contracts, and pseudocode for security concerns. Run security-audit skill.
7. **[APPROVAL]:** Switch to **@pm persona** → Pause. Ask User: "SOT is prepared (Brief → PRD → Pseudocode → Architecture → Wireframes → Security). Type 'Approve' to start coding."

### ⇄ Model Handoff Gate: A → R (Planning → Coding)
> After user approval, run `model-selector` Step 5 to evaluate whether the **Refinement** phase should run on a different model (e.g., switch from `OPUS/Plan` to `GEMINI-H/Plan` for large implementations). Consult `model-routing.md` Handoff Boundaries. If >5 files are involved and current model is Opus, consider handoff to Gemini for speed. Execute `/handoff` if warranted.

## SPARC: Refinement — TDD Loop (SDLC: Implementation + Testing) — `SONNET/Fast`

8. **[TEST SCAFFOLDING]:** Assume **@qc persona** → read `.agent/roles/@qc.md` → write test scaffolds from acceptance criteria BEFORE implementation using `write_to_file`. Tests must initially FAIL (TDD red phase). Save in `tests/phase-[name]/`.
9. **[IMPLEMENTATION]:** Assume **@dev persona** → read `.agent/roles/@dev.md` → read test scaffolds first → implement code to make tests pass using `view_file`, `grep_search`, `replace_file_content`, `run_command`, `context7`. Verify against `.hc/pseudocode/`. Use code-review-excellence skill. **After each passing implementation:** commit per Rule `execution-protocol.md`.
10. **[TESTING & REFINEMENT]:** Assume **@qc persona** → run full test suite with `run_command`. If failures exist, switch to **@dev persona** to fix → switch to **@qc** to retest → repeat until all pass. Refactor for quality (TDD refactor phase). Use `verification-before-completion` skill before reporting done.

## Waterfall: Verification Gate #2 — `SONNET/Fast`

11. **[VERIFICATION]:** As **@qc** → produce a formal **Verification Report** (pass/fail counts, coverage %, risk areas). As **@devops** → run final security scan. As **@pm** → review. NO deployment without approved Verification Report.

### ⇄ Model Handoff Gate: R → C (Coding → Deployment)
> After verification passes, the **Completion** phase (docs, release notes, SOT updates) benefits from **Gemini's large context**. Run `model-selector` Step 5. If currently on Opus and the completion phase involves substantial documentation, execute `/handoff` to switch to Gemini.

## SPARC: Completion (SDLC: Deployment) — `GEMINI-H/Plan`

12. **[COMPLETION & DEPLOY]:** As **@devops** → configure CI/CD and execute deployment (cicd-pipeline skill). As **@ba** → update all SOT documents to reflect delivered state, write release notes. As **@devops** → set up monitoring and alerts.

## SDLC + Waterfall: Maintenance

13. **[MAINTENANCE PLAN]:** As **@devops** → create a maintenance runbook in `.hc/maintenance/`. As **@qc** → document known issues and regression test plan. As **@pm** → trigger `/close-phase` to clean up workspace.
