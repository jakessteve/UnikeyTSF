---
description: Debug Swarm - spawn parallel debug agents across architecture, code, tests, infra, and security to exhaustively diagnose issues
---

# WORKFLOW: /spawn-debug (Multi-Agent Deep Debug Swarm)

Triggered when @pm receives a request to exhaustively debug, diagnose, or audit the application for defects across multiple domains.

> **Inherits:** [`spawn-base-template.md`](spawn-base-template.md) — read the base template first for the full 6-phase pipeline.
> **Related workflows:** `/run-e2e-qa` (E2E tests), `/pentest-session` (security-only)

Execute sequentially, respecting gate decisions:

---

## Customizations (over base template)

### Track Assignment Guide (Phase 1)

| Track | Agent | Investigation Focus |
|---|---|---|
| Architecture & Design | @sa | Structural anti-patterns, coupling issues, dependency cycles, API contract violations, data model inconsistencies |
| Requirements & Logic | @ba | Business rule violations, edge cases in specs, missing validations, data flow gaps, requirement-implementation drift |
| Code & Runtime | @dev | Runtime errors, logic bugs, race conditions, memory leaks, error handling gaps, dead code, type safety issues |
| Test Coverage | @qc | Missing test cases, flaky tests, untested branches, assertion quality, mock correctness, regression gaps |
| Infrastructure & Build | @devops | Build failures, dependency conflicts, env config issues, CI/CD gaps, deployment risks, resource leaks |
| Security Vulnerabilities | @whitehat-hacker | Input validation bypass, injection vectors, auth/authz flaws, secret exposure, dependency CVEs, CSP gaps |

### Spawn Details (Phase 2)

- Use `.agent/spawn_agent_tasks/templates/debug-track-task.md` for prompts.
- Every prompt MUST include mandatory **reproduction steps** requirement.
- Every prompt MUST include **severity classification** (P0-P3).
- @dev tracks should use `run_command` + `grep_search`.
- @whitehat-hacker tracks should use `browser_subagent` for attack simulation.

**Additional safety checks:**
- [ ] Each worker has access to relevant source files and logs

### Validation Checklist (Phase 3)

Each report MUST contain:
- [ ] Bug findings with specific file/line references
- [ ] Severity classification per finding (P0-P3)
- [ ] Reproduction steps or evidence (stack traces, screenshots, test output)
- [ ] Root cause hypothesis for each finding
- [ ] Suggested fix approach

**Deduplication:** Multiple tracks may find the same root cause from different angles — merge these.

**Severity Classification:**

| Severity | Definition | SLA |
|---|---|---|
| P0 — Critical | App crash, data loss, security breach | Fix immediately |
| P1 — High | Major feature broken, no workaround | Fix this sprint |
| P2 — Medium | Feature degraded, workaround exists | Fix next sprint |
| P3 — Low | Minor annoyance, cosmetic | Backlog |

### Cross-Review Questions (Phase 4)

- Ask @dev: "Can you confirm this architectural flaw causes the runtime error found by @sa?"
- Ask @qc: "Is this code bug covered by existing tests?"
- Ask @devops: "Could this infra issue mask this code bug?"
- Ask @whitehat-hacker: "Does this logic gap create a security vector?"

Use markers: 🐛 Confirmed, ❌ Not reproducible, 🔗 Related, 🔨 Fix suggested.
@pm maps **root cause chains** (e.g., bad API contract → missing validation → security hole).

**Root Cause Map format:**
```markdown
| Root Cause | Symptoms Found By | Severity | Affected Components |
|---|---|---|---|
| [root cause] | @dev (Track 3), @qc (Track 4) | P0 | [components] |
```

### Scoring Weights (Phase 5)

| Factor | Weight |
|---|---|
| Severity (user impact) | 35% |
| Blast radius (how much breaks if unfixed) | 25% |
| Fix complexity (effort + risk of regression) | 20% |
| Confidence (evidence quality) | 10% |
| Dependencies (does fix X need fix Y first?) | 10% |

**Identify quick wins:** Fixes that are P1+ severity but low effort → do first.

### Report Template (Phase 6)

```markdown
# Debug Report — [Topic/Area]
**Date:** YYYY-MM-DD | **Requested by:** User
**Debug tracks:** N | **Agents involved:** [list]

## Executive Summary
## Bug Dashboard
| Total | P0 | P1 | P2 | P3 |
|---|---|---|---|---|

## Ranked Fix Plan
### P0 — Fix Immediately
| # | Bug | Root Cause | Fix Approach | Effort | Track Source | Dependencies |
|---|---|---|---|---|---|---|

### P1 — Fix This Sprint / ### P2 — Fix Next Sprint / ### P3 — Backlog
[same table format]

## Root Cause Map
## Methodology
## Limitations
## Appendix: Full Track Reports
```

**Output directory:** `.hc/debug/`
