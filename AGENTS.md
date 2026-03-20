# Agent Skills Index

> Auto-generated index for AI agent skill discovery. Read `instructions.md` first.

## ⚠️ Conversation Entry Rule

**Always begin as @pm.** Run the Pre-Delegation Pipeline (`@pm.md` §3.1) and Mandatory Spawn Gate (`@pm.md` §3.1.8) before touching any source files — even for seemingly simple requests like "fix this bug". Estimate file count first, then route. See `instructions.md` §Conversation Entry Protocol.

## Roles (`.agent/roles/`)

| Role | Trigger Context | Key Skills |
|---|---|---|
| `@pm` | Planning, prioritization, delegation, orchestration, facilitation, idea validation | `task-router`, `roadmap-architect`, `model-selector`, `context-management`, `conflict-resolver`, `facilitation`, `red-team-ideas`, `idea-validation`, `implementation-debate`, `requirement-enrichment`, `critical-thinking-models`, `opportunity-cost-analysis`, `amateur-proof-plans`, `cli-worker-lifecycle` |
| `@pm-extended` | Advanced orchestration, facilitation, dialectical development, sprint management | Extends `@pm` — product management details, facilitation mode, conflict resolution, model handoffs, SOT/SPARC gates, sprint reviews |
| `@biz` | Business strategy, marketing, GTM, market research, monetization, partnerships, brand, growth | `competitive-landscape`, `market-sizing`, `financial-modeling`, `content-and-brand`, `launch-strategy`, `investor-pitch-writer`, `analytics-and-feedback`, `business-writing`, `monetization-strategy`, `partnership-development`, `customer-acquisition` |
| `@ba` | Research, requirements, PRD, technical writing, deep analysis | `prd-architect`, `requirement-interviewer`, `research-analysis`, `technical-writing` |
| `@sa` | Architecture, API design, data models, DDD | `senior-architect`, `c4-architecture`, `architecture-patterns`, `domain-driven-design`, `architecture-decision-records`, `microservices-patterns`, `api-design-principles` |
| `@dev` | **Router stub** — detects `fe:` vs `be:` story prefix and delegates to `@dev-fe` or `@dev-be`. No implementation rules of its own. | — |
| `@dev-fe` | Frontend implementation — creative UI, animations, client-side logic, visual quality gates | `react-patterns`, `typescript-expert`, `animation-choreography`, `mobile-ux-patterns`, `premium-taste-ui`, `anti-lazy-output`, `performance-optimization`, `context7-integration`, `test-driven-development` |
| `@dev-be` | Backend implementation — strict sequential, TDD-first, API/service/DB layer, auth, security | Rule `backend-standards`, `typescript-expert`, `test-driven-development`, `api-design-principles`, `architecture-patterns`, `systematic-debugging`, `refactoring-patterns`, `event-sourcing-cqrs`, `context7-integration` |
| `@qc` | Testing, verification, quality, test design | `playwright-testing`, `verification-before-completion`, `test-case-design`, `test-fixing`, `regression-strategy`, `performance-testing`, `mutation-testing` |
| `@devops` | Security, CI/CD, deployment, observability, incident response | `security-audit`, `cicd-pipeline`, `dependency-upgrade`, `devops-operations`, `docker-containerization`, `infrastructure-as-code` |
| `@designer` | UI/UX, styling, visual design, motion, tokens. Prototyping via **Stitch MCP** mandatory for new screens. Taste-score gate ≥ 8 before ship. | `design-system-uiux`, `browser-visual-testing`, `mobile-ux-patterns`, `animation-choreography`, `design-token-pipeline`, `premium-taste-ui`, `anti-lazy-output` |
| `@user-tester` | User experience testing, UX feedback | `user-experience-testing`, `accessibility-empathy`, `browser-visual-testing` |
| `@whitehat-hacker` | Offensive security, penetration testing, API security, social engineering | `penetration-testing`, `attack-simulation`, `security-audit`, `api-security-testing`, `social-engineering-testing` |

## Rules (`.agent/rules/`) -- 26 files

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
| `no-code-boundary` | All | Single-LLM code boundary: plan as @pm, code as @dev-fe/@dev-be |
| `a11y-standards` | UI | WCAG compliance |
| `agile-user-stories` | PM/BA | User story format |
| `autonomous-tooling` | All | Tool usage patterns, domain-specific search triggers |
| `scalable-folder-structure` | SA | File organization |
| `ui-design-system` | UI | Design token usage |
| `user-feedback-format` | @user-tester | Standardized feedback report structure |
| `git-hygiene` | @dev-fe, @dev-be, @devops | Conventional commits, atomic changes, branch naming |
| `error-handling-standards` | @dev-fe, @dev-be | Structured error types, boundaries, user-facing messages |
| `api-contract-first` | @sa, @dev-be | Contracts defined before implementation |
| `performance-budget` | @dev-fe, @designer | Bundle size limits, Core Web Vitals thresholds |
| `documentation-standards` | All | JSDoc, README, architecture diagram requirements |
| `spawn-governance` | All | Pre-spawn validation + post-spawn timeout escalation |
| `context-budget` | All | Per-session context caps, lazy-load accounting, bloat detection |
| `backend-standards` | `@dev-be`, `@sa`, `@devops` | Sequential mandate, TDD-first gate, edge-case enumeration, security preflight, failure recovery scoping |

## Guidelines (`.agent/guidelines/`) — 4 files (retired from rules)

| Guideline | Scope | Note |
|---|---|---|
| `investor-metrics` | PM | KPI tracking — use when needed, not enforced per-task |
| `gtm-readiness` | PM/BA | GTM checklist — use for launches, not routine changes |
| `observability-standards` | @devops, @dev-fe/@dev-be | Structured logging — apply on infrastructure tasks |
| `dependency-policy` | @devops, @dev-fe/@dev-be | Update cadence — apply during dependency upgrades |

## Skills (`.agent/skills/`) -- 78 entries

> Run `list_dir .agent/skills` to discover available skills. Use `view_file .agent/skills/{skill-name}/SKILL.md` to load a skill on-demand.

## Workflows (`.agent/workflows/`) -- 35 active

> Active workflows: Run `view_file .agent/workflows/MANIFEST.md` for the complete catalog.
