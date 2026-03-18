# Agent Skills Index

> Auto-generated index for AI agent skill discovery. Read `instructions.md` first.

## ⚠️ Conversation Entry Rule

**Always begin as @pm.** Run the Pre-Delegation Pipeline (`@pm.md` §3.1) and Mandatory Spawn Gate (`@pm.md` §3.1.8) before touching any source files — even for seemingly simple requests like "fix this bug". Estimate file count first, then route. See `instructions.md` §Conversation Entry Protocol.

## Roles (`.agent/roles/`)

| Role | Trigger Context | Key Skills |
|---|---|---|
| `@pm` | Planning, prioritization, delegation, orchestration, facilitation, idea validation | `task-router`, `roadmap-architect`, `model-selector`, `context-juggler`, `conflict-resolver`, `facilitation`, `red-team-ideas`, `idea-validation`, `implementation-debate`, `requirement-enrichment`, `critical-thinking-models`, `opportunity-cost-analysis`, `amateur-proof-plans`, `orchestrator-delegation-guide`, `worker-output-parsing`, `multi-worker-coordination` |
| `@pm-extended` | Advanced orchestration, facilitation, dialectical development, sprint management | Extends `@pm` — product management details, facilitation mode, conflict resolution, model handoffs, SOT/SPARC gates, sprint reviews |
| `@biz` | Business strategy, marketing, GTM, market research, monetization, partnerships, brand, growth | `competitive-landscape`, `market-sizing`, `financial-modeling`, `content-marketing`, `seo-copywriting`, `content-strategy`, `launch-strategy`, `growth-metrics`, `investor-pitch-writer`, `telemetry-analysis`, `user-feedback-loop`, `analytics-tracking`, `business-writing`, `monetization-strategy`, `partnership-development`, `brand-strategy`, `customer-acquisition` |
| `@ba` | Research, requirements, PRD, technical writing, deep analysis | `prd-architect`, `requirement-interviewer`, `research-analysis`, `technical-writing` |
| `@sa` | Architecture, API design, data models, DDD | `senior-architect`, `c4-architecture`, `architecture-patterns`, `domain-driven-design`, `architecture-decision-records`, `microservices-patterns`, `api-design-principles` |
| `@dev` | Implementation, coding, debugging, optimization | `systematic-debugging`, `code-review-excellence`, `context7-integration`, `test-driven-development`, `refactoring-patterns`, `typescript-expert`, `react-patterns`, `performance-optimization` |
| `@qc` | Testing, verification, quality, test design | `playwright-testing`, `verification-before-completion`, `test-case-design`, `test-fixing`, `regression-strategy`, `performance-testing`, `mutation-testing` |
| `@devops` | Security, CI/CD, deployment, observability, incident response | `security-audit`, `cicd-pipeline`, `dependency-upgrade`, `observability-engineer`, `incident-responder`, `postmortem-writing`, `docker-containerization`, `infrastructure-as-code`, `slo-implementation` |
| `@designer` | UI/UX, styling, visual design, motion, tokens | `design-system-uiux`, `browser-visual-testing`, `mobile-ux-patterns`, `animation-choreography`, `design-token-pipeline` |
| `@user-tester` | User experience testing, UX feedback | `user-experience-testing`, `accessibility-empathy`, `browser-visual-testing` |
| `@whitehat-hacker` | Offensive security, penetration testing, API security, social engineering | `penetration-testing`, `attack-simulation`, `security-audit`, `api-security-testing`, `social-engineering-testing` |

## Rules (`.agent/rules/`) -- 23 files

| Rule | Scope | Key Constraints |
|---|---|---|
| `anti-patterns-core` | All (universal) | No hallucination, 3-strike loop break, context overflow, circuit breaker, plan-then-code bypass, source trust, icon/emoji token discipline |
| `anti-patterns-swarm` | Swarm/parallel only | Swarm clash prevention, cognitive bias detection, inline execution bias, asset burn guard |
| `execution-protocol` | All | SOT-driven, SPARC, **tiered verification gate**, iteration budgets, agent coordination, CLI worker governance, commit discipline |
| `code-standards` | Code | DRY, no `any`, 80% coverage, tiered QC enforcement |
| `security-standards` | All | Zero hardcoded secrets, shift-left, FOSS-first |
| `engineering-mindset` | All | MVP-first, mobile-first, ruthless prioritization, **kaizen**, task-aware research time-boxing |
| `model-routing` | All | Unified model inventory, task→model matrix, SPARC phase mapping, handoff boundaries |
| `decision-routing` | All | Task classification (Trivial-Epic), confidence scoring (75+ auto-proceed), progress tracking, checkpoint rule |
| `dialectical-development` | All | **Novelty-based** gate, adversarial scrutiny before/after implementation |
| `no-code-boundary` | All | Single-LLM code boundary: plan as @pm, code as @dev |
| `a11y-standards` | UI | WCAG compliance |
| `agile-user-stories` | PM/BA | User story format |
| `autonomous-tooling` | All | Tool usage patterns, domain-specific search triggers |
| `scalable-folder-structure` | SA | File organization |
| `ui-design-system` | UI | Design token usage |
| `user-feedback-format` | @user-tester | Standardized feedback report structure |
| `git-hygiene` | @dev, @devops | Conventional commits, atomic changes, branch naming |
| `error-handling-standards` | @dev | Structured error types, boundaries, user-facing messages |
| `api-contract-first` | @sa, @dev | Contracts defined before implementation |
| `performance-budget` | @dev, @designer | Bundle size limits, Core Web Vitals thresholds |
| `documentation-standards` | All | JSDoc, README, architecture diagram requirements |
| `spawn-governance` | All | Pre-spawn validation + post-spawn timeout escalation |
| `context-budget` | All | Per-session context caps, lazy-load accounting, bloat detection |

## Guidelines (`.agent/guidelines/`) — 4 files (retired from rules)

| Guideline | Scope | Note |
|---|---|---|
| `investor-metrics` | PM | KPI tracking — use when needed, not enforced per-task |
| `gtm-readiness` | PM/BA | GTM checklist — use for launches, not routine changes |
| `observability-standards` | @devops, @dev | Structured logging — apply on infrastructure tasks |
| `dependency-policy` | @devops, @dev | Update cadence — apply during dependency upgrades |

## Skills (`.agent/skills/`) -- 69 entries

> Run `list_dir .agent/skills` to discover available skills. Use `view_file .agent/skills/{skill-name}/SKILL.md` to load a skill on-demand.

## Workflows (`.agent/workflows/`) -- 25 active

> Active workflows: Run `view_file .agent/workflows/MANIFEST.md` for the complete catalog.
