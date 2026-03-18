---
description: Party Mode - multi-agent brainstorming, collaboration, and dialectical idea development
---

# WORKFLOW: /party-mode (Multi-Agent Collaboration) — `OPUS/Plan`

Triggered by @pm when user requests brainstorming, discussion, or collaboration.

> **Related workflows:** `/idea-forge` (full dialectical cycle), `/swarm-execute` (parallel execution pipeline)

Execute sequentially:

1. **[TOPIC INTAKE]:** Read the user's query. Identify the primary domain (product, architecture, design, implementation, quality, security). Summarize the topic in one sentence.

2. **[AGENT SELECTION]:** @pm selects 2-3 agents from the roster based on topic relevance using the `task-router` skill. Available agents: `@pm`, `@ba`, `@sa`, `@designer`, `@dev`, `@qc`, `@devops`, `@user-tester`, `@whitehat-hacker`. Announce selections with reasoning.

3. **[ROUND TABLE]:** Each selected agent gives their initial perspective in-character (emoji prefix + role name). Each response must stay within their domain expertise. Use `facilitation` skill §1 Persona Format.

4. **[DISCUSSION]:** Agents cross-reference and respond to each other. Max **3 discussion rounds**. Use interaction markers ( Agree, Disagree, Build, Question). @pm moderates and redirects off-topic responses.

5. **[RED TEAM]:** *(Optional but recommended for significant decisions)*
 @pm assigns one agent as Devil's Advocate using `red-team-ideas` skill:
 - Execute pre-mortem analysis (what if this idea fails?)
 - Challenge unstated assumptions
 - Identify edge cases and risks
 - Deliver verdict: PROCEED / PROCEED WITH CHANGES / RETHINK / REJECT
 - If RETHINK → return to Step 3 with Red Team findings as new input (max 1 re-entry)

6. **[SYNTHESIS]:** @pm writes a consensus summary using `facilitation` skill §3 template. Save to `.hc/brainstorms/` or `.hc/decisions/` as appropriate. If `idea-validation` skill was used, include DFV scores.

7. **[DEPLOY]:** If actionable outputs exist, route to appropriate workflows:
 - Significant new idea → trigger `/idea-forge` (full dialectical cycle)
 - New feature (no debate needed) → trigger `/hc-sdlc`
 - Complex multi-agent task → trigger `/swarm-execute`
 - Architecture change → @sa updates SOT
 - Bug fix → create story in `.hc/stories/`
 - Security concern → @devops updates `.hc/security/`
 - Skip this step if discussion was purely exploratory.

8. **[EXIT]:** User types "exit"/"done", OR @pm detects natural conclusion. Apply Rule `anti-patterns-core.md` §3 (anti-context-overflow) to clear session context.

---

## Output Files
| File | Location |
|------|----------|
| Brainstorm artifact | `.hc/brainstorms/YYYY-MM-DD-[topic-slug].md` |
| Decision artifact | `.hc/decisions/YYYY-MM-DD-[topic-slug].md` |
| Red Team report (if used) | `.hc/red-team/YYYY-MM-DD-[topic-slug].md` |
