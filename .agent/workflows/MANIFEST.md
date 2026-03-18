# Workflow Manifest (`.agent/workflows/`)

> This file acts as a lazy-loaded index for all available workflows to prevent context bloat in the main `AGENTS.md`.

## Workflows — 42 files

| Workflow | Trigger | Pipeline |
|---|---|---|
| `/hc-sdlc` | New epic/phase | SPARC: Spec → Pseudo → Arch → Refine → Complete |
| `/party-mode` | Brainstorm / multi-agent (@pm facilitation mode) | Opening → Rounds → Red Team → Synthesis → Deploy |
| `/idea-forge` | Significant idea needing full validation | Brainstorm → Red Team → Refine → Implement → Battle Test → Debate → Log |
| `/swarm-execute` | Complex multi-agent parallel execution | Decompose → Pre-Wave Safety → Waves (Req/Arch → Impl → Test → Review) → Handoff |
| `/delegate-task` | Delegate scoped work to CLI worker agents via spawn-agent | Assess → Compose → git stash → Spawn → git diff review → Report |
| `/implementation-review` | Post-build approach evaluation | Roundtable → Intent check → Tech debt → SHIP/RETHINK verdict |
| `/scaffold-feature` | New feature | Types → Logic → UI with review |
| `/design-to-code` | Mockup to code | Design → Components → Visual verify |
| `/idea-to-prd` | Raw idea | Brainstorm → Interview → PRD |
| `/audit-and-deploy` | Release prep | Security → Tests → Build → Deploy |
| `/run-e2e-qa` | E2E testing | Browser flows → Failure capture |
| `/qa-responsive-check` | Responsive QA | Mobile → Tablet → Desktop checks |
| `/sprint-review` | End of sprint | Commits → CHANGELOG → Summary |
| `/retrospective` | Phase end | Reflection → Self-improvement |
| `/close-phase` | Phase done | Cleanup → SOT sync |
| `/dependency-upgrade` | Dep updates | Staged rollout with testing |
| `/prep-pitch-deck` | Pitch prep | Docs → One-pager |
| `/codebase-review` | Code audit | Scan → Score → Improvement plan |
| `/code-review` | PR review | Quality check → Approve/Request changes |
| `/user-test-session` | Post-feature UX testing | Persona sweep → Journey flows → Scorecard → Feedback report |
| `/pentest-session` | Pre-release security testing | Recon → OSINT → Attack simulation → SE Assessment → Exploit → Report → Remediation |
| `/incident-response` | Production issue detected | Detect → Investigate → Mitigate → Postmortem |
| `/api-design` | New API endpoint | Contract → Mock → Implement → Test |
| `/performance-audit` | Performance concerns | Lighthouse → Bundle → Profile → Optimize |
| `/mobile-readiness` | Mobile port or responsive QA | Viewport → Touch → Nav → PWA check |
| `/release-checklist` | Production deployment | Changelog → Security → Build → Smoke → Deploy → Monitor |
| `/onboard-developer` | New contributor | Setup → Tour → First PR → Standards review |
| `/ddd-discovery` | Domain modeling | Event storm → Bounded contexts → Aggregates → Map |
| `/go-to-market` | Feature ready for launch | Content → Copy → SEO → Landing page → Launch checklist → Monitor |
| `/customer-feedback-loop` | Telemetry data or user feedback available | Analyze → Prioritize (ICE) → Stories → Backlog → Verify fix |
| `/battle-test` | Stress-test a feature | Edge cases → Boundary conditions → Adversarial inputs |
| `/handoff` | Cross-model delegation (PM→Dev or Dev→PM) | Generate structured handoff artifact for context transfer |
| `/receive-handoff` | Bootstrap new conversation from handoff | Verify freshness → Load context → Begin execution |
| `/framework-lint` | After `.agent/` changes or periodic audit | Validate cross-refs → Check counts → Find orphans → Report |
| `/business-review` | Quarterly business review or on-demand | Market update → Financial health → Growth metrics → Brand audit → Strategic recommendations |
| `/market-research` | Before P1+ feature commitment or new market entry | Define → Desk Research → Competitive Scan → Sizing → Synthesis → @pm Decision Gate |
| `/product-discovery` | New feature idea needing validation | Problem → Hypothesis → Signals → Solutions → Feasibility → Value×Effort → GO/NO-GO Gate |
| `/ux-audit` | Monthly or on-demand UX audit for existing screens | Heuristic Eval → User Test → Prioritize → Improve → Re-test → Close |
| `/feature-review` | Quarterly feature health evaluation | Inventory → Usage Data → Value Assessment → Keep/Improve/Deprecate/Kill → Action Plans |
| `/scale-readiness` | Before major scaling events | Tech Audit → Perf Audit → Cost → Market → Product → Gap Analysis → Scale Decision Gate |
| `/pricing-experiment` | Testing pricing models or revenue streams | Hypothesis → Benchmark → A/B Design → Implement → Monitor → Analyze → Decision Gate |
| `/competitive-response` | Competitor makes significant move | Alert (4h SLA) → Deep Analysis (24h) → Response Options → @pm Decision → Execute → Review |
