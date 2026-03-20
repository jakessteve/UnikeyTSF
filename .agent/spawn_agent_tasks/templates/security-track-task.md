# Security Task: [Track Name]
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


## Security Question
> [Specific attack surface or defense area to audit — one sentence]

## Scope
- **Endpoints/components to test:** [list specific APIs, pages, or infrastructure]
- **Attack vectors to evaluate:** [injection, auth bypass, XSS, CSRF, etc.]
- **Boundaries:** DO NOT test against production. DO NOT perform destructive actions.
- **Time budget:** [estimated effort in minutes]

## Context
- **Project type:** [web app / native app / library / API / etc.]
- **Tech stack:** [from docs/tech/ARCHITECTURE.md]
- **Auth model:** [JWT / session / OAuth / API key / none]
- **Known security measures:** [CSP, CORS, rate limiting, input sanitization — if documented]
- **This track is part of:** `/spawn-security` workflow with [N] parallel tracks

## Audit Instructions
1. Audit the target area from your security domain expertise.
2. Follow the OWASP Top 10 (2021) checklist where applicable.
3. For each finding, provide **proof-of-concept** or clear reproduction steps.
4. Classify severity using CVSS-aligned scale (Critical / High / Medium / Low / Informational).
5. Map to OWASP category (A01-A10) where applicable.
6. Propose remediation for each finding.
7. **Apply self-assessment** (MANDATORY — do not skip).

## OWASP Top 10 Checklist (2021)

| Code | Category | Check |
|---|---|---|
| A01 | Broken Access Control | Privilege escalation, IDOR, missing authz checks |
| A02 | Cryptographic Failures | Weak encryption, plaintext secrets, insecure protocols |
| A03 | Injection | SQL/NoSQL/OS/LDAP injection, XSS, template injection |
| A04 | Insecure Design | Business logic flaws, missing threat modeling |
| A05 | Security Misconfiguration | Default creds, verbose errors, unnecessary features enabled |
| A06 | Vulnerable Components | Known CVEs in dependencies, outdated packages |
| A07 | Auth Failures | Credential stuffing, weak passwords, session fixation |
| A08 | Data Integrity Failures | Insecure deserialization, unsigned updates, CI/CD tampering |
| A09 | Logging & Monitoring | Missing audit logs, no alerting on suspicious activity |
| A10 | SSRF | Server-side request forgery via user-controlled URLs |

## Responsible Disclosure Rules
- ⛔ NO live exploitation against production
- ⛔ NO data exfiltration (even test data)
- ⛔ NO denial-of-service testing
- ✅ Document proof-of-concept conceptually
- ✅ Test only in dev/staging environment
- ✅ Report all findings regardless of severity

## Self-Assessment (MANDATORY)

### Assessment 1: Coverage
- Which OWASP categories did you actually test? Which were skipped?
- What attack vectors were NOT evaluated?

### Assessment 2: False Positives
- How confident are you that each finding is a real vulnerability? (not theoretical)
- Could any finding be mitigated by a defense you didn't check?

### Assessment 3: Severity Accuracy
- Are your severity scores based on actual exploitability or theoretical worst-case?
- Would a real attacker need special conditions to exploit?

### Assessment 4: Remediation Quality
- Are your proposed fixes complete or just band-aids?
- Could any fix introduce new attack surface?

## Output Format (MUST follow this structure)

```markdown
# Security Track Report: [Track Name]

## Vulnerability Findings
| # | Vulnerability | OWASP | Severity | Location | Exploitable? | Confidence (1-10) |
|---|--------------|-------|----------|----------|--------------|-------------------|
| 1 | [vuln title] | A03 | Critical | [endpoint/file] | Yes/Conditional/No | [score] |

## Proof of Concept
### Vuln #1: [title]
- **Attack vector:** [description]
- **Steps to reproduce:**
  1. [step 1]
  2. [step 2]
- **Expected (secure):** [behavior]
- **Actual (vulnerable):** [behavior]

## Remediation Recommendations
| # | Vulnerability | Remediation | Effort | Regression Risk |
|---|--------------|-------------|--------|-----------------|
| 1 | [vuln] | [fix approach] | H/M/L | H/M/L |

## OWASP Coverage
| Category | Tested? | Findings |
|---|---|---|
| A01-A10 | ✅/⚠️/❌ | [N] findings |

## Self-Assessment
### OWASP coverage: [N]/10 categories tested
### False positive risk: [Low/Med/High]
### Remediation completeness: [complete / partial / band-aid]

## Limitations
[What was NOT tested, environmental constraints, suggested follow-ups]
```

## Progress Reporting
After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
