---
description: Sequential workflow for adding a new backend endpoint — from contract to tested implementation
---

# WORKFLOW: /new-endpoint

> **Persona:** @dev-be (load `.agent/roles/@dev-be.md` and Rule `backend-standards.md` before starting)
> **Trigger:** When @pm assigns a `be:` story for a new API endpoint.
> **Execution mode:** Strictly sequential. Complete and verify each step before starting the next.

---

## Phase 1 — Setup & Edge Cases
// turbo
1. Read the story file in `.hc/stories/be-[slug].md` completely.
2. Enumerate ALL edge cases to `.hc/pseudocode/[feature]-edges.md` (see `backend-standards.md` §4 for required categories).
3. Run security preflight checklist from `backend-standards.md` §5. Do not proceed if any item fails.

## Phase 2 — API Contract
4. Open `docs/tech/API_CONTRACTS.md`. Verify the endpoint schema exists.
   - If schema is **missing** → STOP. Request @sa to define it before continuing.
   - If schema **exists** → proceed.
5. Read and internalize the request schema, response schema, HTTP method, status codes, and error shapes.

## Phase 3 — Test Scaffold (TDD Red)
6. Check `tests/` for @qc's failing test scaffold for this endpoint.
   - If **no scaffold exists** → STOP. Escalate to @pm to invoke @qc to provide it.
   - If **scaffold exists** → run the tests to confirm they fail (expected at this stage).

## Phase 4 — Implementation (TDD Green)
7. Use `context7-integration` skill to fetch latest docs for any library used.
8. `grep_search` codebase for existing services/utilities that overlap — reuse before creating (Rule `code-standards.md`).
9. Implement the endpoint in this order:
   - Type definitions → `src/types/`
   - Service/business logic → `src/services/`
   - Route handler → `src/api/` or `src/routes/`
   - Input validation middleware (if new schema needed)
10. Run the failing tests from Phase 3. Iterate until all pass.

## Phase 5 — Verification
11. Run the FULL test suite (not just the new tests). Zero regressions allowed.
12. Verify all endpoint outputs match `docs/tech/API_CONTRACTS.md` exactly (field names, types, HTTP codes).
13. Auto-trigger `code-review-excellence` skill — self-review for correctness, performance, security, readability.
14. Anti-hallucination check: verify all imports exist, all API calls match contracts, no library methods used from memory.

## Phase 6 — Refactor & Hand-off
15. Refactor for quality (TDD Refactor phase). Run full suite again — still green.
16. Request @devops security review if the endpoint touches: auth, new DB migrations, or new env variables.
17. Write dev-log → `.hc/logs/dev/be-[slug].md`. Record decisions, edge cases handled, security notes.
18. Hand off to @pm → @pm routes to @qc for independent verification. **Never self-declare Done.**

---

### Failure Recovery
If any step fails more than 3 times on the same scope → log to `.hc/bugs/[slug].md` and escalate to @pm. See `backend-standards.md` §7.
