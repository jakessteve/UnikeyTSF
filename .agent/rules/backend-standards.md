---
scope: Backend (@dev-be, @sa, @devops when touching backend code)
---

# RULE: BACKEND STANDARDS

This rule consolidates the backend-specific constraints that apply to ALL backend coding tasks. It is the single source of truth for backend quality — @dev-be must load it alongside `test-driven-development` skill before every task.

---

## §1 — Mandatory System Prompt
Every backend task MUST begin with this mental frame:

> *"Follow all backend rules strictly. Think step-by-step. List ALL edge cases first, before writing a single line of code."*

---

## §2 — Sequential Execution Mandate
Backend execution MUST be strictly sequential. No agent may parallelize steps that share mutable state.

**Mutable state** includes:
- Database tables or rows
- Auth tokens, sessions, or cookies
- File system writes
- Environment variables or config files
- Shared service singletons

**Rationale:** Parallel writes to shared state cause non-deterministic bugs that are extremely hard to debug. Sequential execution sacrifices speed for correctness — the correct trade-off for backend.

---

## §3 — TDD-First Enforcement (HARD GATE)
No implementation code may be written before a failing test exists.

| Phase | Action |
|---|---|
| Pre-implementation | @qc provides failing test scaffold |
| Implementation | @dev-be writes code to make tests pass |
| Post-implementation | @dev-be refactors, tests stay green |
| Delivery | Terminal output showing all tests passing is REQUIRED |

**If no test scaffold exists:** @dev-be MUST stop and escalate to @pm to invoke @qc. This is a blocking gate, not a suggestion.

---

## §4 — Edge Case Enumeration (Pre-Coding Requirement)
Before writing tests OR code, the agent MUST enumerate and document edge cases.

Format: `.hc/pseudocode/[feature]-edges.md`

Required categories:
- **Happy path** inputs
- **Empty / null / zero** inputs
- **Max-boundary** inputs (large numbers, long strings)
- **Auth failure** scenarios (unauthenticated, insufficient permissions)
- **Concurrent access** scenarios (if applicable)
- **Network / DB failure** scenarios

Do NOT proceed to test writing until this document exists.

---

## §5 — Security Preflight (Per-Endpoint Checklist)
Run before implementing EVERY endpoint:

```
[ ] No hardcoded secrets, tokens, or passwords
[ ] All user inputs validated (schema validation at entry, not in service)
[ ] Auth check present BEFORE any data access
[ ] Parameterized queries only — no string concatenation into SQL/queries
[ ] Sensitive data NOT logged (passwords, PII, tokens)
[ ] Rate limiting considered for public-facing endpoints
[ ] CORS policy appropriate for the endpoint visibility
```

Failing any item = block implementation. Fix the security issue first.

---

## §6 — API Contract Enforcement
`docs/tech/API_CONTRACTS.md` is the source of truth. Rules:
- If the required schema is missing in API_CONTRACTS.md → throw an error and stop. Request @sa.
- Never infer a schema from example data or existing code.
- After implementation, verify ALL outputs (field names, types, HTTP codes, error shapes) match the contract exactly.

---

## §7 — Failure Recovery Protocol (Token-Efficient Iteration)
When a step fails, do NOT restart the entire task context.

1. Identify the exact function or service that failed.
2. Scope the re-run to ONLY that function — not the full file, not the full service.
3. Log the failure to `.hc/bugs/[slug].md` immediately.
4. After **3 attempts** on the same scope → STOP. Escalate to @pm. Do not attempt a 4th.

**Rationale:** Narrowing the failure scope saves 60–80% of tokens vs full-context restarts and dramatically increases per-iteration accuracy.

---

## §8 — Database Safety Rules
- ORM/query builder exclusively (Prisma, Drizzle, Knex). No raw string SQL construction.
- All schema migrations reviewed by @sa before execution.
- Every query's N+1 risk must be assessed. Use eager loading (`include`/`join`) intentionally.
- No migration runs in a feature branch without @devops approval.

---

## §9 — Type Contract Between @dev-fe and @dev-be
Shared types live in `src/types/`. This is the handshake between frontend and backend.

- @dev-be OWNS type definitions for API response shapes.
- @dev-fe CONSUMES them — never redefines independently.
- If a type must change → @dev-be updates `src/types/`, @dev-fe adapts.
- Type changes are logged in the dev-log of the phase that introduces them.

---

## §10 — Delivery Gate
A backend task is NOT done until:
- [ ] All tests pass (terminal output required as evidence)
- [ ] `docs/tech/API_CONTRACTS.md` matches implementation
- [ ] Edge cases doc exists at `.hc/pseudocode/[feature]-edges.md`
- [ ] Security preflight passed (§5 checklist complete)
- [ ] @devops security sign-off obtained (for auth flows, migrations, new env vars)
- [ ] @qc independently verified (not just @dev-be self-test)
- [ ] Dev-log written to `.hc/logs/dev/be-[phase].md`
