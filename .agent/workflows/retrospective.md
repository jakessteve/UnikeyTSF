---
description: Retrospective - post-phase reflection and self-improvement analysis
---

# WORKFLOW: /retrospective (Post-Phase Reflection & Self-Improvement)

Triggered by `/close-phase` after each phase/epic completion. DO NOT SKIP.

Execute sequentially:

1. **[COLLECT LOGS]:** @pm verifies all phase logs exist in `.hc/logs/`:
   - `dev/[phase].md` from @dev
   - `bugs/[phase].md` from @qc
   - `security/[phase].md` from @devops
   - `decisions/[phase].md` from @pm
   - `design/[phase].md` from @designer
   - `research/[phase].md` from @ba
   - `architecture/[phase].md` from @sa
   - If any log is missing, halt and request the responsible agent to write it.

2. **[SOT VERIFICATION]:** @sa verifies all SOT documents are in sync with delivered code (Rule `execution-protocol.md`):
   - `docs/tech/ARCHITECTURE.md` matches the implemented system design.
   - `docs/tech/API_CONTRACTS.md` matches the actual API responses and data schemas.
   - `docs/biz/PRD.md` acceptance criteria match what was delivered.
   - If drift is found ? update the SOT document BEFORE proceeding.

3. **[RETROSPECTIVE]:** @ba reads ALL phase logs and synthesizes a retrospective report using the improvement-lifecycle skill template. Save to `.hc/retrospectives/YYYY-MM-DD-[phase].md`. Include: what went well, what didn't, improvements, metrics, SOT sync status, recurring patterns.

4. **[LESSONS TO KNOWLEDGE HARVESTING]:** Based on the synthesized report, @ba must explicitly harvest key learnings and update system-wide Knowledge Items (KIs) located in `.hc/knowledge/` or `docs/knowledge/`. Make sure temporary insights are persisted globally for future phases.

5. **[IMPROVEMENT ANALYSIS]:** @pm reads the retrospective and identifies improvement opportunities:
   - Are there **recurring bug patterns** ? new rule or skill update needed?
   - Are there **repeated blockers** ? workflow change needed?
   - Are there **skill gaps** ? new skill or skill expansion needed?
   - Are there **process inefficiencies** ? workflow optimization needed?
   - Are there **SOT sync failures** ? enforcement gaps in rules or roles?
   - Are there **loop patterns** ? anti-loop or decomposition improvements needed?
   - Categorize each finding as: **UPDATE** (existing file) or **CREATE** (new file).

6. **[BRAINSTORM & RESEARCH]:** For each identified improvement:
   - **Brainstorm** (use `/party-mode` if the improvement is complex or cross-cutting):
     - Invite relevant agent personas to discuss the best approach.
     - Consider trade-offs, side effects, and impact on other rules/skills/workflows.
   - **Research** online best practices:
     - Use **search_web** to find industry standards, proven patterns, and frameworks.
     - Use **context7** to check for library-specific conventions or updates.
     - Reference GitHub repos, dev blogs, and official docs for validated approaches.
   - **Think systematically:** Use **sequential-thinking** to evaluate alternatives.
   - Document findings in the improvement proposal using the improvement-lifecycle skill template.
   - Save proposals to `.hc/improvements/proposals/`.

7. **[DRAFT CHANGES]:** For each approved improvement proposal:
   - **UPDATE existing:** Draft the specific additions/modifications to the target `.agent/` file. Show a clear diff of what will change and why.
   - **CREATE new:** Draft the complete new skill/rule/workflow file following the existing format conventions (YAML frontmatter + structured sections).
   - Validate the draft doesn't conflict with existing rules/skills/workflows.

8. **[APPLY WITH APPROVAL]:** Present ALL drafted changes to the User for approval:
   - **NEVER auto-modify `.agent/` files** — User must review and approve every change.
   - For each draft, show: target file, change type (update/create), rationale, and full content.
   - If approved ? apply changes and log to `.hc/improvements/history.md`.
   - If partially approved ? apply only approved items, document rejected items with reasons.
   - If rejected ? document the reason and close the proposal.

Report: "Retrospective complete. [X] improvements identified, [Y] brainstormed, [Z] approved and applied."
