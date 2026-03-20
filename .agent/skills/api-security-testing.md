---
description: API Security Testing - structured vulnerability testing for auth bypass, BOLA/IDOR, injection, and rate limiting
---

# SKILL: API Security Testing

**Trigger:** When @whitehat-hacker tests API endpoints for security vulnerabilities. Used during `/pentest-session` workflow or standalone security reviews.

---

## When to Use
- Before releasing any new API endpoint.
- After changes to authentication or authorization logic.
- During scheduled penetration testing sessions.
- When security audit identifies API-related risks.
- After dependency upgrades that affect auth/crypto libraries.

---

## The 5-Category Test Protocol

### Category 1: Authentication Testing
**Goal:** Verify that only authenticated users can access protected endpoints.

| Test | Method | Expected Result |
|---|---|---|
| No auth token | Send request without `Authorization` header | 401 Unauthorized |
| Expired token | Use a token past its `exp` claim | 401 Unauthorized |
| Tampered JWT | Modify payload without re-signing | 401 Unauthorized |
| Wrong algorithm | Change JWT `alg` to `none` | 401 Unauthorized |
| Brute force | Send >N failed login attempts | 429 after threshold |
| Session fixation | Reuse old session ID after re-login | Old session invalid |

**Attack commands:**
```bash
# No auth
curl -X GET http://localhost:3000/api/protected

# Tampered JWT (change role to admin)
# Decode base64, modify payload, re-encode without signing
```

### Category 2: Authorization / BOLA / IDOR
**Goal:** Verify users cannot access other users' resources.

| Test | Method | Expected Result |
|---|---|---|
| Horizontal IDOR | User A requests `/api/users/{B_ID}/data` | 403 Forbidden |
| Vertical escalation | Regular user hits `/api/admin/settings` | 403 Forbidden |
| Object reference | Change resource ID in URL or body | Only own resources returned |
| Mass assignment | Send extra fields (`{"role": "admin"}`) | Extra fields ignored |

**Attack commands:**
```bash
# IDOR test
curl -H "Authorization: Bearer USER_A_TOKEN" \
 http://localhost:3000/api/users/USER_B_ID/charts

# Mass assignment
curl -X PUT -H "Authorization: Bearer USER_TOKEN" \
 -d '{"name": "test", "role": "admin", "isVerified": true}' \
 http://localhost:3000/api/users/me
```

### Category 3: Input Injection
**Goal:** Verify all inputs are sanitized.

| Injection Type | Payloads | Where to Test |
|---|---|---|
| **SQL injection** | `' OR 1=1 --`, `'; DROP TABLE users --` | Login forms, search, query params |
| **XSS** | `<script>alert(1)</script>`, `" onmouseover="alert(1)` | All text inputs, URL params |
| **Command injection** | `; ls`, `| cat /etc/passwd` | File paths, system commands |
| **Path traversal** | `../../etc/passwd`, `..%2F..%2F` | File download/upload endpoints |
| **NoSQL injection** | `{"$gt": ""}`, `{"$ne": null}` | MongoDB query parameters |
| **SSRF** | `http://169.254.169.254/` | URL input fields |

### Category 4: Rate Limiting
**Goal:** Verify API has rate limiting to prevent abuse.

| Test | Method | Expected Result |
|---|---|---|
| Burst requests | 100 rapid requests to same endpoint | 429 after limit |
| `Retry-After` header | Trigger rate limit | Header present with backoff time |
| Per-user limiting | Different users hit different limits | Limits scoped to user |
| Per-IP limiting | Same IP, no auth | IP-based limit applies |

```bash
# Rate limit test
for i in $(seq 1 100); do
 curl -s -o /dev/null -w "%{http_code}\n" http://localhost:3000/api/login -d '{}'
done | sort | uniq -c
```

### Category 5: Data Exposure
**Goal:** Verify responses don't leak internal information.

| Test | Check For | Risk |
|---|---|---|
| Error responses | Stack traces, internal paths | Information disclosure |
| API responses | Internal IDs, timestamps, metadata | Data leakage |
| Debug endpoints | `/debug`, `/status`, `/health` with internal data | Exposed in production |
| HTTP headers | `X-Powered-By`, `Server` version | Fingerprinting |

---

## Severity Classification (OWASP Aligned)

| Severity | Criteria | Example | Action |
|---|---|---|---|
| Critical | Auth bypass, data breach | IDOR returns other users' data | **Fix immediately, block release** |
| High | Injection possible, escalation | XSS in user input field | Fix before release |
| Medium | Rate limiting missing, info disclosure | Stack trace in error | Fix in next sprint |
| Low | Hardening gaps, best practice violations | Missing security headers | Backlog |

## Report Format
Each finding follows the `security-audit` skill report template with OWASP category mapping. Include:
- Severity rating
- Proof of concept (exact commands to reproduce)
- OWASP category reference
- Remediation recommendation

## Rules
- **Test in isolated environments** — never run injection tests against production.
- **Document every finding** with reproduction steps.
- **Re-test after fixes** — verify the vulnerability is actually closed.
- **Report immediately** if a Critical or High finding is discovered mid-test.
