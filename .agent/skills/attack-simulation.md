---
description: Attack Simulation - adversarial scenario design, threat personas, and multi-stage attack chain simulation
---

# SKILL: Attack Simulation

## When to Use
When @whitehat-hacker needs to design and execute realistic attack scenarios — simulating real-world threat actors with specific motivations, capabilities, and attack chains. Use this to think beyond individual vulnerabilities and test the application's resilience against coordinated attacks.

## 1. Threat Persona System
Think like the attacker. Select the appropriate threat persona based on the target:

### Threat Persona Registry

#### Script Kiddie — "Hacker Nhí"
| Attribute | Value |
|---|---|
| **Skill Level** | Low — uses pre-made tools and public exploits |
| **Motivation** | Ego, bragging rights, disruption for fun |
| **Tools** | Browser dev tools, automated scanners, copy-pasted payloads |
| **Typical Attacks** | XSS, default credential testing, basic SQL injection, simple DoS |
| **Persistence** | Low — gives up after first few failed attempts |
| **Threat Level** | Medium — still dangerous if defenses are weak |

#### Opportunistic Criminal — "Kẻ Trộm Dữ Liệu"
| Attribute | Value |
|---|---|
| **Skill Level** | Medium — understands web security, uses specialized tools |
| **Motivation** | Financial gain — sell data, ransom, crypto mining |
| **Tools** | Burp Suite, SQLmap, custom scripts, credential databases |
| **Typical Attacks** | Credential stuffing, data exfiltration, payment fraud, account takeover |
| **Persistence** | Medium — will try multiple vectors before moving on |
| **Threat Level** | High — targeted and motivated |

#### Competitor / Corporate Spy — "Đối Thủ"
| Attribute | Value |
|---|---|
| **Skill Level** | Medium-High — has resources, may hire specialists |
| **Motivation** | Business intelligence, user data theft, service disruption |
| **Tools** | Professional pentest tools, social engineering, insider access |
| **Typical Attacks** | API scraping, user database extraction, service cloning, DDoS to disrupt |
| **Persistence** | High — sustained campaign over days/weeks |
| **Threat Level** | Critical — strategic and well-resourced |

#### Disgruntled Insider — "Người Trong Nội Bộ"
| Attribute | Value |
|---|---|
| **Skill Level** | Varies — but has legitimate access and insider knowledge |
| **Motivation** | Revenge, sabotage, data theft for leverage |
| **Tools** | Legitimate credentials, direct DB access, knowledge of architecture |
| **Typical Attacks** | Data deletion, backdoor planting, privilege abuse, data exfiltration |
| **Persistence** | Very High — patient, knows the system intimately |
| **Threat Level** | Critical — hardest to defend against |

### Persona Selection Guide
| Scenario | Persona(s) |
|---|---|
| General security assessment | Script Kiddie + Opportunistic Criminal |
| Pre-launch security review | All 4 personas |
| Auth system hardening | Opportunistic Criminal + Insider |
| Data protection audit | Corporate Spy + Insider |
| DoS resilience testing | Script Kiddie + Corporate Spy |
| Supply chain review | Corporate Spy |

## 2. Attack Chain Design
Real attacks aren't single-step — they're chains. Design multi-stage attack scenarios:

### Attack Chain Template
```markdown
## Attack Chain: [Name]
**Threat Persona:** [Which persona]
**Target:** [Feature/Component]
**Objective:** [What the attacker wants to achieve]

### Stage 1 — Reconnaissance
- Action: [What the attacker does first]
- Expected Result: [What they learn]
- Detection: [Would this be noticed?]

### Stage 2 — Initial Access
- Action: [How they get a foothold]
- Expected Result: [First access achieved]
- Detection: [Would this be noticed?]

### Stage 3 — Escalation
- Action: [How they expand access]
- Expected Result: [Higher privileges or more data]
- Detection: [Would this be noticed?]

### Stage 4 — Objective
- Action: [How they achieve their goal]
- Expected Result: [Data stolen / service disrupted / etc.]
- Detection: [Would this be noticed?]

### Impact Assessment
- Confidentiality: [Impact score]
- Integrity: [Impact score]
- Availability: [Impact score]
- Overall Severity: [Critical/High/Medium/Low]
```

### Pre-Built Attack Chains

#### Chain 1: Account Takeover via Password Reset
```
Recon → Find password reset endpoint
 → Test for email enumeration (different responses for existing vs non-existing)
 → Discover predictable reset token pattern
 → Generate valid reset token for target account
 → Reset password → Full account takeover
```

#### Chain 2: Data Exfiltration via IDOR
```
Recon → Find user data API endpoint
 → Create two test accounts
 → Access Account A's data with Account B's token
 → Enumerate all user IDs (sequential? UUID?)
 → Automate data extraction for all users
 → Exfiltrate user database
```

#### Chain 3: Privilege Escalation via Mass Assignment
```
Recon → Study API request/response bodies
 → Identify user role field in responses
 → Add role=admin to profile update request
 → Check if role was actually modified
 → Access admin panel with elevated privileges
 → Modify application settings, user data
```

#### Chain 4: Service Disruption via Resource Exhaustion
```
Recon → Identify computationally expensive endpoints
 → Map rate limiting configuration
 → Find rate limit bypass (header manipulation)
 → Send 10,000 expensive requests
 → Server CPU/memory spikes
 → Service degraded or unavailable for all users
```

#### Chain 5: Supply Chain Compromise
```
Recon → Analyze package.json for dependency names
 → Check if any package name is available on public npm
 → Create malicious package with same name + higher version
 → Wait for next `npm install` (dependency confusion)
 → Malicious code executes during install
 → Backdoor in build pipeline
```

#### Chain 6: Phishing → Credential Harvest → Account Takeover
```
OSINT → Harvest user emails from app's public surfaces
 → Clone login page (assess difficulty per social-engineering-testing §3.1)
 → Craft phishing email mimicking app notifications
 → Victim enters credentials on cloned page
 → Attacker uses credentials on real app
 → Full account takeover + lateral movement
```
*Cialdini levers: Authority (official-looking email), Scarcity (urgent action required)*

#### Chain 7: Pretext → Info Extraction → Privilege Escalation
```
OSINT → Discover org hierarchy and support processes
 → Impersonate IT support using exposed staff identities
 → Contact target via app's messaging/support channel
 → Extract current password or MFA bypass info
 → Login as target → escalate to admin via mass assignment
 → Full admin access achieved
```
*Cialdini levers: Authority (IT support persona), Commitment (small asks escalating), Reciprocity (offering to "fix" an issue)*

## 3. Simulation Execution Protocol
When executing an attack simulation:

1. **Select persona** — Pick the most relevant threat persona from §1.
2. **Design chain** — Map out the multi-stage attack from §2 or create a custom one.
3. **Execute stages** — Run each stage sequentially, documenting results.
4. **Record evidence** — Screenshot every successful step, save payloads.
5. **Assess detection** — For each stage, note: *"Would the current monitoring catch this?"*
6. **Score resilience** — Rate the application's resistance per attack chain:

| Rating | Meaning |
|---|---|
| ⬛ **Impervious** | Attack chain fails at Stage 1 — no foothold possible |
| **Resilient** | Attack chain fails at Stage 2 — initial access blocked |
| **Moderate** | Attack chain succeeds partially — limited impact |
| **Vulnerable** | Attack chain succeeds — significant impact achievable |
| **Critical** | Full chain completes — maximum impact achieved |

## 4. Red Team Report Template
```markdown
# Red Team Simulation Report
**Date:** YYYY-MM-DD | **Operator:** @whitehat-hacker
**Threat Persona:** [Selected persona]
**Target Scope:** [Features tested]

## Simulation Summary
| Chain | Target | Result | Detection | Severity |
|---|---|---|---|---|
| Account Takeover | Auth system | Moderate | Partial | Medium |
| Data Exfiltration | User API | Critical | None | Critical |

## Detailed Attack Chains
[Each chain from §2 with actual results]

## Detection Gaps
| Stage | Attack | Detected? | Recommended Detection |
|---|---|---|---|
| Recon | Endpoint enumeration | No | Add request rate monitoring |
| Access | IDOR attempt | No | Add ownership validation logging |

## Resilience Score
| Category | Score | Notes |
|---|---|---|
| Authentication | 4/5 | Strong, but 2FA bypass possible |
| Authorization | 2/5 | IDOR found in 3 endpoints |
| Input Validation | 3/5 | XSS blocked, but SQLi possible |
| Rate Limiting | 2/5 | Bypassed via X-Forwarded-For |
| Data Protection | 3/5 | Tokens in localStorage |

## Recommendations (Priority Order)
1. [P0] Fix IDOR in user data endpoints
2. [P1] Move tokens from localStorage to HttpOnly cookies
3. [P2] Implement proper rate limiting
```

## 5. Continuous Threat Intelligence
After each simulation, update the vulnerability knowledge base:
- Track **emerging attack patterns** relevant to the tech stack (React, Vite, Node.js).
- Monitor **CVE databases** for dependencies.
- Document **new bypass techniques** discovered during testing.
- Update attack playbooks with lessons learned.
- Share threat intelligence with @devops for proactive defense.
