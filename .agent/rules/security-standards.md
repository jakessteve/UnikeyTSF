---
description: Always On — shift-left security, zero-trust secrets, and FOSS pipeline standards
---

# RULE: SECURITY STANDARDS (Combined)

All agents MUST enforce security-first practices at all times.

---

## 1. Shift-Left Security

Security is NOT a post-deployment afterthought. Integrate security checks at every phase:
- **Design:** @sa reviews for OWASP Top 10 in architecture.
- **Implementation:** @dev validates inputs, sanitizes outputs, uses parameterized queries.
- **Testing:** @qc includes security test cases (XSS, injection, auth bypass).
- **Deployment:** @devops runs automated security scanning in CI/CD.

---

## 2. Zero-Trust Secrets

**ABSOLUTE BAN on hardcoded secrets:** API keys, passwords, tokens, connection strings MUST NEVER appear in source code, config files, or logs.

**Required:** Environment variables (`.env`) for ALL sensitive data. `.env` in `.gitignore`.

**Scanning:** `grep -rnE "(password|apiKey|secret|private_key)\s*=\s*['\"][^'\"]{6,}" src/` — any match is a **Critical** violation.

**Logging:** NEVER log passwords, tokens, or secrets. Filter sensitive fields before logging.

---

## 3. FOSS Pipeline

Prefer Free/Open-Source tools where possible. Minimize vendor lock-in.
- Audit dependencies with `npm audit` before adding new packages.
- Prefer well-maintained packages with > 1000 weekly downloads.
- Check license compatibility (MIT, Apache 2.0 preferred).
- No abandoned packages (last publish > 12 months ago = red flag).
