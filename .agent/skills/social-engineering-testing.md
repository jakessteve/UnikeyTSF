---
description: Social Engineering Testing — OSINT reconnaissance, phishing resistance assessment, pretext scenario simulation, and psychological exploitation analysis
---

# SKILL: Social Engineering Testing

## When to Use
When @whitehat-hacker needs to assess an application's resilience against human-factor attacks — phishing, pretexting, information leakage, and trust manipulation. Use during `/pentest-session` (Steps 2.5 + 7.6) or standalone SE assessments.

> **Scope boundary:** This skill focuses on what an AI agent CAN directly test (DOM analysis, OSINT exposure, auth flow susceptibility, UI trust indicators). Items that require human execution (phone vishing, physical tailgating) are documented as **advisory findings** for human-led SE testing.

---

## 1. Psychological Exploitation Framework

All social engineering exploits one or more of **Cialdini's 7 Principles**. Use this as a lens when designing pretext scenarios and evaluating application defenses:

| # | Principle | Attack Pattern | What to Test |
|---|---|---|---|
| 1 | **Reciprocity** | Offer help → request credentials | Does the app allow impersonation of support staff? |
| 2 | **Scarcity** | Create fake urgency | Can notification text/emails be crafted with urgency language? |
| 3 | **Authority** | Impersonate someone powerful | Does the app display role/title that could be spoofed? |
| 4 | **Commitment** | Small "yes" → escalating requests | Can multi-step auth flows be social-engineered step-by-step? |
| 5 | **Liking** | Build rapport via profile info | How much personal info does the app expose to other users? |
| 6 | **Social Proof** | "Everyone has done this" | Can bulk user actions be faked or displayed misleadingly? |
| 7 | **Unity** | Invoke shared group identity | Does the app expose team/org membership that aids pretexting? |

**Usage:** When scoring SE resilience (§6), map each finding to the Cialdini principle it exploits.

---

## 2. OSINT Reconnaissance Protocol

Assess what intelligence an attacker can gather from the application's public-facing surfaces.

### 2.1 Information Exposure Audit
Test what the application reveals without authentication:

```markdown
| Information Type | Source | Found? | Severity |
|---|---|---|---|
| User email addresses | Registration errors, public profiles | ? | High |
| User real names | Public profiles, comments, shared content | ? | Medium |
| Organization structure | Team pages, about pages, public directories | ? | High |
| Technology stack | HTTP headers, error pages, source maps | ? | Medium |
| API documentation | Swagger/OpenAPI exposed publicly | ? | High |
| Employee count / roles | Public team listings | ? | Medium |
| Internal naming conventions | URL patterns, error messages, JS bundles | ? | Low |
```

### 2.2 User Enumeration Assessment
- **Registration endpoint:** Does the response differ for existing vs. non-existing emails?
- **Password reset endpoint:** Does it confirm whether an email exists?
- **Login endpoint:** Does it distinguish "wrong password" from "user not found"?
- **Public profiles:** Can user IDs be enumerated (sequential? predictable?)?

### 2.3 Metadata & Error Message Intelligence
- Do error messages reveal internal paths, function names, or stack traces?
- Do API responses include internal IDs, timestamps, or server metadata?
- Do HTTP headers disclose versions (`X-Powered-By`, `Server`)?
- Do source maps exist in production builds?

---

## 3. Phishing Resistance Assessment

### 3.1 Login Page Clone Difficulty Score
Evaluate how easy it is for an attacker to replicate the login page:

| Factor | Score 1 (Easy) | Score 3 (Hard) |
|---|---|---|
| **Visual complexity** | Simple form, no branding | Complex UI, unique animations |
| **Anti-clone indicators** | No domain verification shown | Domain prominently displayed in UI |
| **CSP protection** | No CSP or weak CSP | Strict CSP prevents resource loading from other origins |
| **Unique identifiers** | Generic design, easily replicated | Custom fonts/graphics requiring specific assets |
| **MFA integration** | No MFA, password-only | Integrated hardware key / biometric |

**Score = Average(all factors).** ≤1.5 = Easily cloned; 1.6–2.0 = Moderate; ≥2.1 = Resistant.

### 3.2 Email/Notification Spoofability
- Does the app send emails? Are they DKIM/SPF/DMARC protected?
- Do notification emails contain links? Can link destinations be predicted/forged?
- Do emails display user-controllable content (names, messages) that could inject phishing text?
- Are email templates branded enough that a clone would be obvious?

### 3.3 Anti-Phishing UI Indicators
Check if the application helps users detect phishing:
- [ ] Does the app show the domain name prominently on sensitive pages?
- [ ] Are external links clearly marked or warned about?
- [ ] Does the app use custom security indicators (e.g., personalized images/phrases)?
- [ ] Are password inputs protected against autofill on cloned domains?
- [ ] Does the app warn about open redirects in outbound links?

### 3.4 Auth Flow Susceptibility to Phishing Proxies
Test whether the auth flow can be intercepted by a real-time phishing proxy (Evilginx-style):
- Can the login flow be transparently proxied (no pinning, no origin checks)?
- Does the session token survive being issued through a proxy?
- Does the app detect suspicious login locations/devices after proxy-based auth?

---

## 4. Pretext Scenario Library

Pre-built scenarios to test the application's susceptibility. For each scenario, assess: *"Does the application provide enough information or functionality to make this pretext credible?"*

### 4.1 IT Support Impersonation
- **Pretext:** "I'm from IT support, I need to verify your account."
- **Test:** Does the app expose support channels, ticket systems, or staff identities that make this credible?
- **Test:** Can support/help features be abused to impersonate staff?

### 4.2 Vendor/Partner Impersonation
- **Pretext:** "I'm from [vendor], we need to update your integration."
- **Test:** Does the app expose third-party integration names or partner logos?
- **Test:** Are partner/vendor communications distinguishable from phishing?

### 4.3 Executive Authority Exploitation
- **Pretext:** "The CEO needs this data urgently."
- **Test:** Does the app expose org hierarchy, executive names, or reporting structures?
- **Test:** Can admin actions be triggered via social channels outside the app?

### 4.4 New Employee Onboarding Exploit
- **Pretext:** "I'm new, can you help me access the system?"
- **Test:** Does the invite/onboarding flow expose enough process detail to fake it?
- **Test:** Can invitation links be predicted or reused?

---

## 5. Application-Level SE Vectors

### 5.1 Password Reset Social Engineering
- Can an attacker reset another user's password using only OSINT-gathered info?
- Are security questions guessable from public profile data?
- Does the reset flow leak information (token in URL, timing-based enumeration)?

### 5.2 Support/Help Desk Abuse
- Can an attacker submit a support request posing as another user?
- Does the support flow verify identity before taking action?
- Can support tickets inject content visible to other users or staff?

### 5.3 Trust Indicator Manipulation
- Can an attacker manipulate displayed names, avatars, or badges?
- Can verified/trusted status be spoofed via profile manipulation?
- Are system messages distinguishable from user-generated content?

### 5.4 Notification Channel Hijacking
- Can an attacker trigger notifications to other users with controlled content?
- Can push notifications, emails, or SMS be used as phishing vectors through the app?
- Does the app rate-limit or sanitize user-triggered notifications?

---

## 6. SE Resilience Scoring Matrix

After completing the assessment, score each dimension:

| Dimension | Score | Criteria |
|---|---|---|
| **OSINT Exposure** | | How much intelligence can be passively harvested? |
| **Phishing Resistance** | | How hard is it to clone and phish? |
| **Pretext Credibility** | | How much does the app aid pretexting? |
| **Trust Indicator Integrity** | | Can trust signals be spoofed? |
| **Auth Flow SE Resilience** | | Is the auth flow resistant to SE-based attacks? |

**Scores:**
- **Resilient** (4/4) — Minimal exposure, strong anti-SE controls
- **Moderate** (3/4) — Some exposure, basic protections in place
- **Vulnerable** (2/4) — Significant exposure, few SE-specific protections
- **Critical** (1/4) — Extensive exposure, no SE-specific protections

---

## 7. SE Findings Report Template

```markdown
## Social Engineering Assessment
**Date:** YYYY-MM-DD | **Tester:** @whitehat-hacker

### OSINT Exposure Summary
| Item | Exposed? | Source | Risk |
|---|---|---|---|
| User emails | Yes/No | [Where found] | High/Med/Low |

### Phishing Resistance Score
| Factor | Score | Notes |
|---|---|---|
| Login clone difficulty | X/3 | [Details] |
| Email spoofability | X/3 | [Details] |
| Anti-phishing UI | X/3 | [Details] |

### Pretext Vulnerability Assessment
| Scenario | Credibility | App-Aided? | Risk |
|---|---|---|---|
| IT Support | High/Med/Low | Yes/No | High/Med/Low |

### SE Resilience Matrix
[From §6 above]

### Recommendations
1. [Priority-ordered SE-specific hardening actions]

### Advisory: Human-Led Testing Needed
- [ ] Vishing (voice phishing) — test employee phone response
- [ ] Physical tailgating — test office access controls 
- [ ] In-person pretexting — test front-desk/reception protocols
- [ ] USB baiting — test response to found media devices
```

---

## 8. Rules
- **Ethics first:** All SE assessments are simulated — no real manipulation of real people.
- **Scope boundary:** Only test what the application exposes. Do not conduct SE against individuals.
- **Document everything:** Every finding must include the Cialdini principle exploited and the app-level vector.
- **Advisory items:** Clearly mark findings that require human-led SE testing for full validation.
