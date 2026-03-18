---
description: Security Audit - comprehensive vulnerability scanning, OWASP Top 10, dependency analysis, and secret detection
---

# SKILL: Security Audit

## When to Use
When @devops conducts security reviews — before deployments, after adding new dependencies, during code review, or on a scheduled basis.

## The Security Audit Framework

### 0. SOT Contract Review (FIRST — before any scanning)
Before running any scans, review the SOT documents:
- Read `docs/tech/ARCHITECTURE.md` — identify security layers, trust boundaries, and data flows.
- Read `docs/tech/API_CONTRACTS.md` — check auth requirements, input validation specs, error handling for each endpoint.
- Verify security specs are complete. If `docs/tech/API_CONTRACTS.md` is missing auth fields → flag as a finding.
- Use SOT as the baseline: actual implementation MUST match what's specified.

### 1. Dependency Vulnerability Scanning
Run and interpret dependency vulnerability checks:
```bash
# Check for known vulnerabilities
npm audit
npm audit --audit-level=high

# JSON output for automated parsing
npm audit --json

# List outdated packages
npm outdated
```
- Flag any **critical** or **high** severity vulnerabilities.
- For each vulnerability, assess: Is there a fix? Is it exploitable in our context?
- Document findings with CVE IDs and remediation steps.
- Check for supply chain risks: review new dependencies for maintainer reputation, download counts, and last update date.

**Optional (if available):**
```bash
# Snyk scanning
npx snyk test

# Trivy container scanning
trivy fs --severity HIGH,CRITICAL .
```

### 2. Secret Detection
Scan for hardcoded secrets in the codebase:
- **API keys, tokens, passwords** in source files.
- **Private keys** or certificates committed to git.
- **Environment-specific values** not using `.env` pattern.
- Use grep patterns:
```
password|secret|api_key|token|private_key|credential|auth
```
- Verify `.gitignore` includes: `.env`, `.env.local`, `*.pem`, `*.key`.
- Check git history for accidentally committed secrets: `git log --all --full-history -p -- "*.env"`

### 3. OWASP Top 10 (2021) Detailed Checklist

#### A01: Broken Access Control
- [ ] Authorization checks on every endpoint and route
- [ ] Frontend route guards match backend authorization
- [ ] CORS configured restrictively (no wildcard origins in production)
- [ ] Directory listing disabled
- [ ] JWT/session tokens validated on every request
- [ ] Rate limiting implemented on auth endpoints

#### A02: Cryptographic Failures
- [ ] Sensitive data encrypted at rest and in transit
- [ ] No deprecated algorithms (MD5, SHA1 for security)
- [ ] HTTPS enforced (HSTS headers)
- [ ] Passwords hashed with bcrypt/argon2 (not plain text or simple hash)

#### A03: Injection
- [ ] All user inputs sanitized/parameterized
- [ ] No raw SQL queries with string concatenation
- [ ] HTML output properly encoded/escaped (XSS)
- [ ] URLs validated before use in redirects
- [ ] Command injection prevented (no `exec()` with user input)

#### A04: Insecure Design
- [ ] Threat modeling documented for critical flows
- [ ] Rate limiting on sensitive operations
- [ ] Business logic abuse scenarios considered
- [ ] Input validation at both client and server

#### A05: Security Misconfiguration
- [ ] Default credentials changed
- [ ] Error messages don't expose stack traces or internals
- [ ] Security headers set (CSP, X-Frame-Options, X-Content-Type-Options)
- [ ] Unnecessary features/endpoints disabled
- [ ] Debug mode off in production

#### A06: Vulnerable and Outdated Components
- [ ] All dependencies up to date (npm audit clean)
- [ ] No components with known vulnerabilities
- [ ] Dependency versions pinned (lock file committed)
- [ ] Unused dependencies removed

#### A07: Identification and Authentication Failures
- [ ] Strong password policy enforced
- [ ] Multi-factor authentication available for sensitive operations
- [ ] Session management secure (httpOnly, secure, sameSite cookies)
- [ ] Account lockout after failed attempts

#### A08: Software and Data Integrity Failures
- [ ] CI/CD pipeline secured (no unsigned deployments)
- [ ] Subresource integrity (SRI) for CDN assets
- [ ] Serialized data validated before deserialization

#### A09: Security Logging and Monitoring Failures
- [ ] Security events logged (login, logout, failed auth, permission denied)
- [ ] Logs don't contain sensitive data (passwords, tokens)
- [ ] Alerting configured for suspicious patterns
- [ ] Log integrity protected (append-only or immutable)

#### A10: Server-Side Request Forgery (SSRF)
- [ ] URL allowlists for external requests
- [ ] No user-controlled URLs in backend HTTP calls
- [ ] Internal network access restricted

### 4. Security Report Template
```markdown
# Security Audit Report
**Date:** YYYY-MM-DD | **Auditor:** @devops
**Scope:** [What was audited]
**OWASP Coverage:** A01-A10

## Summary
| Severity | Count |
|---|---|
| Critical | X |
| High | X |
| Medium | X |
| Low | X |

## Dependency Health
- npm audit: X vulnerabilities (Y critical, Z high)
- Outdated packages: N
- Supply chain risk: [Assessment]

## OWASP Compliance
| Category | Status | Notes |
|---|---|---|
| A01: Broken Access Control | // | [Details] |
| A02: Cryptographic Failures | // | [Details] |
| A03: Injection | // | [Details] |
| ... | ... | ... |

## Findings
### [FINDING-001] [Title]
- **Severity:** Critical/High/Medium/Low
- **OWASP Category:** A0X
- **Location:** [file:line]
- **Description:** [What the issue is]
- **Impact:** [What could happen if exploited]
- **Remediation:** [How to fix it]
- **Status:** Open | Mitigated | Accepted Risk

## Recommendations
1. [Priority action item]

## Next Audit
Scheduled for: YYYY-MM-DD
```
