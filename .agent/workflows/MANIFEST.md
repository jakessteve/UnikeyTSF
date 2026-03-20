# Workflow Manifest (`.agent/workflows/`)

> This file acts as a lazy-loaded index for all available workflows to prevent context bloat in the main `AGENTS.md`.

## Workflows — 35 files

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
| `/visual-redesign` | Existing UI upgrade | Aesthetic audit → Design params → High-end refactor → Verify |
| `/idea-to-prd` | Raw idea | Brainstorm → Interview → PRD |
| `/run-e2e-qa` | E2E testing | Browser flows → Failure capture |
| `/qa-responsive-check` | Responsive QA | Mobile → Tablet → Desktop checks |
| `/sprint-review` | End of sprint | Commits → CHANGELOG → Summary |
| `/retrospective` | Phase end | Reflection → Self-improvement |
| `/close-phase` | Phase done | Cleanup → SOT sync |
| `/codebase-review` | Code audit | Scan → Score → Improvement plan |
| `/code-review` | PR review | Quality check → Approve/Request changes |
| `/user-test-session` | Post-feature UX testing | Persona sweep → Journey flows → Scorecard → Feedback report |
| `/pentest-session` | Pre-release security testing | Recon → OSINT → Attack simulation → SE Assessment → Exploit → Report → Remediation |
| `/battle-test` | Stress-test a feature | Edge cases → Boundary conditions → Adversarial inputs |
| `/handoff` | Cross-model delegation (PM→Dev or Dev→PM) | Generate structured handoff artifact for context transfer |
| `/receive-handoff` | Bootstrap new conversation from handoff | Verify freshness → Load context → Begin execution |
| `/framework-lint` | After `.agent/` changes or periodic audit | Validate cross-refs → Check counts → Find orphans → Report |
| `/ux-audit` | Monthly or on-demand UX audit for existing screens | Heuristic Eval → User Test → Prioritize → Improve → Re-test → Close |
| `/spawn-research` | Research + recommend (multi-angle) | Decompose → Spawn parallel researchers → Self-critique → Cross-review → Critical thinking synthesis → Report |
| `/spawn-debug` | Exhaustive multi-angle debugging | Decompose → Spawn @sa/@ba/@dev/@qc/@devops/@whitehat → Collect → Root cause correlation → Fix prioritization → Report |
| `/spawn-biz` | Business viability & usability audit | Decompose → Spawn @pm/@ba/@biz/@user-tester → Collect → Stakeholder round table → Strategic prioritization → Report |
| `/spawn-ui` | UI/UX & business flow audit | Decompose → Spawn @pm/@ba/@designer/@biz/@user-tester → Collect → Design round table → Improvement strategy → Report |
| `/spawn-performance` | Performance & scale deep-dive | Decompose → Spawn @dev/@qc/@devops/@sa → Collect → Bottleneck correlation → Optimization roadmap → Report |
| `/spawn-security` | Security hardening (offense + defense) | Decompose → Spawn @whitehat/@devops/@dev/@sa → Collect → Attack surface correlation → Remediation strategy → Report |
| `/spawn-base-template` | Shared 6-phase structure for all /spawn-* workflows | (Not a standalone workflow — inherited by all spawn-* workflows) |
| `/token-check` | Context audit | Context audit and token-burn mitigation workflow |
| `/new-endpoint` | Add new backend API endpoint | Setup → Edge Cases → API Contract → TDD Red → Implement → Verify → @devops sign-off → @qc hand-off |
| `/refactor-be` | Backend refactoring safely | Baseline run → Coverage audit → One-transform-at-a-time → Full suite → Security re-scan → Hand-off |
| `/test-all-be` | Backend regression sweep | Discover → Run serial → Triage failures → File bug records → Flaky detection → Coverage check → PR pass gate |
