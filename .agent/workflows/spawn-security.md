---
description: Security Hardening Swarm - spawn parallel agents for offensive + defensive security audit covering pen testing, CVEs, secrets, CSP, and input sanitization
---

# WORKFLOW: /spawn-security (Multi-Agent Security Hardening Swarm)

Triggered when @pm receives a request to comprehensively audit the application's security posture from both offensive and defensive perspectives.

> **Inherits:** [`spawn-base-template.md`](spawn-base-template.md) — read the base template first for the full 6-phase pipeline.
> **Related workflows:** `/pentest-session` (pen-test only), `/spawn-debug` (bug-focused)

Execute sequentially, respecting gate decisions:

---

## Customizations (over base template)

### Track Assignment Guide (Phase 1)

| Track | Agent | Investigation Focus |
|---|---|---|
| Offensive Security (Pen Test) | @whitehat-hacker | Attack simulation, injection vectors, auth bypass, CSRF/XSS, privilege escalation, API abuse, social engineering vectors |
| Infrastructure Security | @devops | Dependency CVEs, secret scanning, environment hardening, CSP headers, CORS policy, HTTPS enforcement, CI/CD supply chain |
| Code-Level Security | @dev | Input validation/sanitization, SQL/NoSQL injection, output encoding, secure coding patterns, cryptographic usage, error info leakage |
| Architecture Security | @sa | Auth/authz model review, data flow analysis, trust boundary mapping, session management, API contract security, zero-trust compliance |

### Spawn Details (Phase 2)

- Use `.agent/spawn_agent_tasks/templates/security-track-task.md` for prompts.
- Every prompt MUST include **CVSS-style severity** scoring requirement.
- Every prompt MUST include **proof-of-concept** or reproduction steps.
- Every prompt MUST follow **responsible disclosure** principles (no live exploitation).
- @whitehat-hacker prompts MUST include **OWASP Top 10** checklist.
- @whitehat-hacker: Use `browser_subagent` + `run_command` for attack simulation.
- @devops: Use `run_command` for dependency scanning, secret detection.
- @dev: Use `grep_search` + `view_file` for code-level analysis.

**Additional safety checks:**
- [ ] All testing is against dev/staging (NEVER production)
- [ ] @whitehat-hacker has responsible disclosure constraints in prompt
- [ ] No destructive actions (data deletion, resource exhaustion)

### Validation Checklist (Phase 3)

Each report MUST contain:
- [ ] Vulnerability findings with specific evidence (file, endpoint, request/response)
- [ ] Severity classification (Critical / High / Medium / Low / Informational)
- [ ] OWASP category mapping (A01-A10) where applicable
- [ ] Proof-of-concept or reproduction steps
- [ ] Remediation recommendation per finding
- [ ] False positive assessment (confidence that this is a real vulnerability)

**Deduplication:** Different tracks may find the same vuln from different angles — merge and escalate severity.

**Security Severity Classification (CVSS-aligned):**

| Severity | CVSS Range | Definition | SLA |
|---|---|---|---|
| Critical | 9.0-10.0 | Active exploitation possible, data breach risk | Fix within 24h |
| High | 7.0-8.9 | Exploitable with moderate effort | Fix within 1 week |
| Medium | 4.0-6.9 | Exploitable under specific conditions | Fix within 1 month |
| Low | 0.1-3.9 | Theoretical risk, defense-in-depth | Backlog |
| Informational | 0.0 | Best practice recommendation | Advisory |

### Cross-Review Questions (Phase 4)

- Ask @whitehat-hacker: "Can you chain this code-level vuln with the auth bypass to escalate impact?"
- Ask @devops: "Does the CSP policy mitigate the XSS vector @whitehat-hacker found?"
- Ask @sa: "Does the trust boundary design prevent the privilege escalation @dev identified?"
- Ask @dev: "Can you validate that this dependency CVE is actually reachable in our code?"

Use markers: 🔴 Exploitable, 🟡 Possible, 🟢 Mitigated, 🔗 Chainable, 🛡️ Defense exists.
@pm maps **attack chains** (e.g., XSS → session hijack → privilege escalation).

**Attack Surface Map format:**
```markdown
| Vulnerability | Found By | Severity | OWASP | Exploitable? | Chained With | Mitigation Exists? |
|---|---|---|---|---|---|---|
| [vuln] | @whitehat-hacker, @dev | Critical | A03 | Yes | Auth bypass | No |
```

### Scoring Weights (Phase 5)

| Factor | Weight |
|---|---|
| Exploitability (how easy to attack) | 30% |
| Impact (data breach, service disruption) | 30% |
| Blast radius (how many users affected) | 15% |
| Fix complexity + regression risk | 15% |
| Regulatory/compliance requirement | 10% |

**Defense-in-depth assessment:** Identify missing defensive layers.

### Report Template (Phase 6)

```markdown
# Security Audit — [App/Feature Name]
**Date:** YYYY-MM-DD | **Requested by:** User
**Audit tracks:** N | **Agents involved:** [list]

## Executive Summary
## Security Posture Dashboard
| Category | Score (1-10) | Vulns Found | Critical | Status |
|---|---|---|---|---|

## Vulnerability Dashboard
| Total | Critical | High | Medium | Low | Informational |
|---|---|---|---|---|---|

## Ranked Remediations
### P0 — Patch Immediately / ### P1 — Fix This Sprint / ### P2 — Next Sprint / ### P3 — Accept Risk
| # | Vulnerability | OWASP | Severity | Attack Vector | Fix Approach | Effort | Track Source |
|---|---|---|---|---|---|---|---|

## Attack Chain Analysis
## Defense-in-Depth Assessment
| Layer | Status | Gaps |
|---|---|---|

## Methodology
## Limitations
## Appendix: Full Track Reports
```

**Output directory:** `.hc/security-audit/`
