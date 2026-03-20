---
description: Penetration Testing - structured pentest methodology, attack playbooks, and vulnerability scoring
---

# SKILL: Penetration Testing

## When to Use
When @whitehat-hacker conducts structured security assessments — during `/pentest-session` workflow, pre-release security reviews, or on-demand penetration testing.

## The Pentest Methodology (PTES-Aligned)

### Phase 0 — Pre-Engagement
Before any testing begins:
1. **Scope confirmation** — Get explicit scope from @pm:
 - Which features/pages/APIs are in scope?
 - Which environments? (dev, staging — NEVER production)
 - Any out-of-bounds areas? (e.g., third-party services)
 - Time window for testing?
2. **Rules of engagement** — Confirm ethical boundaries per `@whitehat-hacker.md` §7.
3. **Tooling preparation** — Set up browser, intercepting proxy, test accounts.

### Phase 1 — Reconnaissance & Enumeration
Map the complete attack surface:

#### 1.1 Passive Recon
- Review source code (client-side) for exposed endpoints, API keys, debug flags.
- Check `robots.txt`, `sitemap.xml`, `.well-known` for info disclosure.
- Analyze HTTP headers for server/framework fingerprinting.
- Review JavaScript bundles for API routes, auth logic, hidden features.
- Check if source maps are exposed in production builds.

#### 1.2 Active Recon
- Enumerate all API endpoints (documented + undocumented).
- Map all user input vectors: forms, URL params, headers, cookies.
- Identify all authentication/authorization checkpoints.
- Discover all file upload endpoints.
- Map all WebSocket/SSE connections.

#### 1.3 Attack Surface Matrix
Build this matrix for each target:

```markdown
| Input Vector | Type | Auth | Validation | Sanitization | Risk |
|---|---|---|---|---|---|
| Login email field | Text input | No | Client-only? | Unknown | High |
| Profile avatar | File upload | Yes | Size only? | Unknown | Medium |
| Search query | URL param | No | None? | Unknown | High |
| API /user/:id | URL path | JWT | Server-side | N/A | Medium |
```

### Phase 2 — Vulnerability Assessment
Systematically test each attack category from `@whitehat-hacker.md` §2:

#### 2.1 Authentication Testing Playbook
```
1. Test login with SQL injection payloads: ' OR '1'='1, admin'--
2. Test JWT: modify payload, change alg to none, use expired tokens
3. Test session: reuse old tokens, check token rotation on password change
4. Test brute force: attempt 100 rapid logins — is rate limiting enforced?
5. Test password reset: is token predictable? Can it be reused?
6. Test OAuth flow: manipulate redirect_uri, test state parameter
7. Test 2FA: attempt bypass via backup codes, race condition on OTP
```

#### 2.2 Authorization Testing Playbook
```
1. IDOR: Change user IDs in API requests — /api/user/123 → /api/user/456
2. Horizontal escalation: Access another user's data with valid but wrong token
3. Vertical escalation: Access admin endpoints with regular user token
4. Force browsing: Navigate directly to /admin, /dashboard, /api/internal
5. Method tampering: Change GET to POST/PUT/DELETE on protected endpoints
6. Parameter tampering: Add admin=true, role=admin to request bodies
```

#### 2.3 Injection Testing Playbook
```
1. XSS: Test all input fields with <script>alert(1)</script>
2. XSS variants: <img onerror=alert(1)>, javascript:alert(1), SVG-based
3. Stored XSS: Submit XSS in profile fields, check if rendered to others
4. SQL injection: ' OR 1=1--, UNION SELECT, time-based blind
5. NoSQL injection: {$gt: ''}, {$ne: null} in JSON payloads
6. Template injection: {{7*7}}, ${7*7}, #{7*7}
7. Prototype pollution: __proto__.isAdmin = true in JSON bodies
```

#### 2.4 DoS Testing Playbook
```
1. Rate limit test: 1000 requests in 10 seconds — does it throttle?
2. Large payload: Submit 10MB+ JSON body — does it reject?
3. ReDoS: Submit known evil regex inputs to search fields
4. Recursive query: Deeply nested JSON/GraphQL — does it limit depth?
5. Slowloris: Hold connections open with partial headers
6. Resource exhaustion: Request computationally expensive operations rapidly
```

#### 2.5 Client-Side Testing Playbook
```
1. Check localStorage/sessionStorage for tokens, PII, secrets
2. Check all cookies: HttpOnly? Secure? SameSite?
3. Check CORS: Can arbitrary origins read responses?
4. Check CSP: Is Content-Security-Policy header present and strict?
5. Check for clickjacking: Can the page be iframed?
6. Check console output: Are errors/debug logs exposing info?
7. Check network tab: Are any requests sending credentials to third parties?
```

### Phase 3 — Exploitation & PoC
For each vulnerability found:
1. **Craft a minimal PoC** that demonstrates the impact.
2. **Document exact reproduction steps** (copy-pasteable).
3. **Assess real-world impact:**
 - Confidentiality: Can data be read?
 - Integrity: Can data be modified?
 - Availability: Can the service be disrupted?
4. **Score severity** using simplified CVSS:

| Factor | Low (1) | Medium (2) | High (3) |
|---|---|---|---|
| Exploitability | Requires deep knowledge | Moderate skill | Script kiddie level |
| Impact | Info disclosure | Data modification | Full compromise |
| Scope | Single user | Multiple users | All users/system |

**Severity = Average(Exploitability + Impact + Scope):**
- 1.0–1.5 → Low
- 1.6–2.0 → Medium
- 2.1–2.5 → High
- 2.6–3.0 → Critical

### Phase 4 — Reporting
Use the report template from `@whitehat-hacker.md` §5. Include:
- Executive summary for @pm (non-technical)
- Technical details for @devops (with PoCs)
- Remediation recommendations (prioritized)
- Risk matrix (severity × exploitability)

### Phase 5 — Re-Test & Verification
After @devops implements fixes:
1. Re-run the exact same exploit against the patched version.
2. Verify the fix doesn't introduce new vulnerabilities (regression).
3. Test for bypass — can the fix be circumvented with a modified attack?
4. Update the report with re-test results.
5. Mark findings as: Fixed | Partially Fixed | Still Vulnerable

## Common Payload Reference

### XSS Payloads
```
<script>alert(document.cookie)</script>
<img src=x onerror=alert(1)>
<svg onload=alert(1)>
javascript:alert(document.domain)
"><script>alert(1)</script>
'-alert(1)-'
```

### SQL Injection Payloads
```
' OR '1'='1
' OR '1'='1' --
' UNION SELECT NULL,NULL,NULL --
1' AND SLEEP(5) --
' OR 1=1#
```

### JWT Bypass
```json
// Algorithm confusion
{"alg": "none", "typ": "JWT"}

// Key confusion (RS256 → HS256)
{"alg": "HS256", "typ": "JWT"}
// Sign with public key as HMAC secret
```

### Header Injection
```
X-Forwarded-For: 127.0.0.1
X-Original-URL: /admin
X-Rewrite-URL: /admin
```
