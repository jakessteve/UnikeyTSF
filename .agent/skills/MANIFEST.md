---
description: Skills Manifest — Lazy-Load Index for role-scoped skill discovery
---

# Skills Manifest -- Lazy-Load Index

> Agents: read this manifest to find relevant skills. Do NOT pre-load skill files.
> When a trigger matches your current task, use `view_file` to load the skill on-demand.
> **Path convention:** All skills live at `skills/<name>.md` or `skills/<name>/SKILL.md` for directory skills.

## Lookup by Role

### @dev-fe
| Skill | Trigger |
|---|---|
| react-patterns | React optimization |
| typescript-expert | Advanced TS patterns |
| performance-optimization | Speed/bundle optimization |
| context7-integration | Library docs needed |
| test-driven-development | New feature implementation |
| code-review-excellence | PR review |
| sequential-thinking | Complex reasoning |

### @dev-be
| Skill | Trigger |
|---|---|
| systematic-debugging | Bug investigation |
| typescript-expert | Advanced TS patterns |
| test-driven-development | New feature implementation |
| refactoring-patterns | Code smell / structural improvement |
| api-design-principles | New API endpoints |
| architecture-patterns | Pattern selection |
| event-sourcing-cqrs | Audit trails, temporal queries |
| context7-integration | Library docs needed |
| code-review-excellence | PR review |
| sequential-thinking | Complex reasoning |

### @dev (shared — available to both @dev-fe and @dev-be)
| Skill | Trigger |
|---|---|
| dependency-upgrade | Dep updates |

### @qc
| Skill | Trigger |
|---|---|
| playwright-testing | E2E testing |
| verification-before-completion | Task completion |
| test-case-design | Test suite creation |
| test-fixing | Flaky/failing tests |
| regression-strategy | Test selection for changes |
| performance-testing | Load/stress validation |
| mutation-testing | Test effectiveness |
| browser-visual-testing | Visual verification |

### @pm
| Skill | Trigger |
|---|---|
| task-router | Complex multi-domain prompt |
| conflict-resolver | Agent disagreements |
| facilitation | Party-mode / brainstorm |
| model-selector | Mixed-mode tasks |
| context-management | Context pressure, parallel coordination, task decomposition |
| cli-worker-lifecycle | CLI worker delegation (Medium+ tasks) |
| requirement-enrichment | Vague/ambiguous user input |
| critical-thinking-models | Feature/architecture delegation gate |
| structured-analysis-frameworks | Complex decisions |
| amateur-proof-plans | Medium+ task phasing |
| roadmap-architect | Planning |
| backlog-grooming | Sprint planning |
| opportunity-cost-analysis | New work vs existing priorities |

### @pm (Dialectical)
| Skill | Trigger |
|---|---|
| red-team-ideas | Post-brainstorm adversarial analysis |
| idea-validation | Idea comparison |
| implementation-debate | Post-build approach review |

### @sa
| Skill | Trigger |
|---|---|
| senior-architect/ | Major tech decisions |
| c4-architecture | System diagramming |
| architecture-patterns | Pattern selection |
| domain-driven-design | Complex domain modeling |
| architecture-decision-records | Significant tech decisions |
| microservices-patterns | Service decomposition |
| event-sourcing-cqrs | Audit trails, temporal queries |
| api-design-principles | New API endpoints |

### @designer
| Skill | Trigger |
|---|---|
| design-system-uiux/ | UI changes |
| browser-visual-testing | Visual verification |
| mobile-ux-patterns | Mobile design/responsive |
| animation-choreography | Micro-interactions |
| design-token-pipeline | Token management |
| premium-taste-ui | High-end visual aesthetics |
| anti-lazy-output | Code completion enforcement |

### @devops
| Skill | Trigger |
|---|---|
| security-audit | Security review |
| cicd-pipeline | CI/CD setup |
| devops-operations | Incident response, postmortem, SLOs, observability |
| docker-containerization | Container builds |
| infrastructure-as-code | Reproducible environments |
| dependency-upgrade | Dep updates |

### @whitehat-hacker
| Skill | Trigger |
|---|---|
| penetration-testing | Pentest session |
| attack-simulation | Adversarial scenario design |
| api-security-testing | API vulnerability testing |
| social-engineering-testing | OSINT recon, phishing |

### @biz
| Skill | Trigger |
|---|---|
| competitive-landscape | Competitor analysis |
| market-sizing | Market opportunity |
| financial-modeling | Business projections |
| content-and-brand | Content planning, SEO, brand voice, landing pages |
| launch-strategy | Feature ready for launch |
| analytics-and-feedback | Analytics, telemetry, user feedback, growth metrics |
| investor-pitch-writer | Pitch creation |
| business-writing | Proposals, pitches |
| monetization-strategy | Pricing, revenue streams |
| partnership-development | B2B outreach |
| customer-acquisition | Channel strategy, funnels |

### @ba
| Skill | Trigger |
|---|---|
| prd-architect/ | New PRD needed |
| requirement-interviewer/ | Vague requirements |
| research-analysis | Research needed |
| technical-writing | Doc creation |
| documentation-style-guide | Before writing any document |

### @user-tester
| Skill | Trigger |
|---|---|
| user-experience-testing | UX evaluation |
| accessibility-empathy | Low-tech user simulation |

### Cross-Role
| Skill | Trigger |
|---|---|
| improvement-lifecycle/ | Phase completion |
| critique | "critique this", "be brutally honest", "@critique" |
