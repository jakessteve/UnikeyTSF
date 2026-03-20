---
description: Documentation Standards - SDLC output structure, code docs, and architecture diagram requirements
---

# RULE: DOCUMENTATION STANDARDS

**Mode:** Always On
**Scope:** All agents

---

## 1. SDLC Document Output (Binding)

All project documents MUST be written to the `/docs/` directory, organized by category:

```
docs/
  tech/             # Technical docs (SPARC phases P, A, R)
    ARCHITECTURE.md       # System architecture, component diagrams
    API_CONTRACTS.md      # API endpoints, request/response schemas
    TEST_PLAN.md          # Test strategy, coverage targets
    DEPLOYMENT.md         # Deploy process, environments, rollback
    ADR/                  # Architecture Decision Records
      ADR-001-*.md
  biz/              # Business docs (SPARC phase S)
    PRD.md                # Product Requirements Document
    PRODUCT_BRIEF.md      # One-pager product summary
    MARKET_RESEARCH.md    # Market sizing, competitive analysis
    GTM_PLAN.md           # Go-to-market strategy
  log/              # Execution logs (SPARC phase C)
    CHANGELOG.md          # Release changelog (Keep a Changelog)
    SPRINT_LOG.md         # Sprint review summaries
    INCIDENT_LOG.md       # Post-incident reports
```

### SDLC Phase Mapping

| SPARC Phase | Output Type | Target Folder |
|-------------|-------------|---------------|
| S - Specification | PRD, market research, user stories | `docs/biz/` |
| P - Pseudocode | Algorithm docs, ADRs | `docs/tech/ADR/` |
| A - Architecture | Architecture, API contracts | `docs/tech/` |
| R - Refinement | Test plans, deployment guides | `docs/tech/` |
| C - Completion | Changelog, sprint logs, incident reports | `docs/log/` |

### Binding Rules
- NEVER write project documents to the project root. Root is for README.md and framework context files only.
- NEVER write documents to `.temp/` — that is for scratch data only.
- ADRs follow the naming convention: `ADR-NNN-short-title.md`
- Changelog follows [Keep a Changelog](https://keepachangelog.com/) format.
- Sprint logs are appended, not overwritten.

---

## 2. Code Documentation

- **Public functions/APIs:** Must have JSDoc with `@param`, `@returns`, `@throws`.
- **Complex algorithms:** Must have explanatory comments explaining *why*, not *what*.
- **Magic numbers:** Must be extracted to named constants with a comment.
- **Type definitions:** Complex types must have `/** */` descriptions.

## 3. Module Documentation

- Every major module/feature directory should have a brief README or header comment explaining:
  - What it does
  - How it fits in the architecture
  - Key dependencies

## 4. Architecture Documentation

- New components/modules must be reflected in `docs/tech/ARCHITECTURE.md`.
- New API endpoints must be reflected in `docs/tech/API_CONTRACTS.md`.
- Significant design decisions must have an ADR in `docs/tech/ADR/`.

## 5. Rules

- Never write `// TODO` without a linked issue or ticket.
- Never write `// fix this later` -- either fix it now or create a tracking item.
- Update docs when changing the interface they describe.
- Inline comments explain intent, not mechanics.
