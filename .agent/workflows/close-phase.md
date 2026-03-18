---
description: Close Phase - cleanup workspace and sync SOT after completing a phase
---

# WORKFLOW: /close-phase (CLEANUP & SOT MAINTENANCE)

Mandatory procedure to keep workspace pristine. Triggered after each phase/epic.

@pm MUST execute this sequence:

1. **[FILE AUDIT]:** Move any stray source files into `src/phase-[x]/` and tests into `tests/phase-[x]/`.
2. **[SOT SYNC]:** Update `docs/tech/ARCHITECTURE.md` and `docs/tech/API_CONTRACTS.md` to match the exact code written. @sa reviews.
3. **[SECURITY SCAN]:** @devops runs a final security-audit on the completed phase. Document in `.hc/logs/security/`.
4. **[WIPE TEMP]:** DELETE ALL files inside the `.temp/` directory.
5. **[CHANGELOG]:** Write summary in `CHANGELOG.md`.
6. **[PHASE LOGGING]:** Verify all mandatory logs exist (Rule `engineering-mindset.md`):
   - @dev ? `.hc/logs/dev/[phase].md`
   - @qc ? `.hc/logs/bugs/[phase].md`
   - @devops ? `.hc/logs/security/[phase].md`
   - @pm ? `.hc/logs/decisions/[phase].md`
   - @designer ? `.hc/logs/design/[phase].md`
   - @ba ? `.hc/logs/research/[phase].md`
   - @sa ? `.hc/logs/architecture/[phase].md`
   - If any log is missing, halt and request the responsible agent to write it NOW.
7. **[RETROSPECTIVE]:** Trigger Workflow `/retrospective` for post-phase reflection and self-improvement analysis.
8. **[CONTEXT FLUSH]:** Force the system to run Rule `anti-patterns-core.md` (Clear Context).

Report: "Phase closed. Workspace clean, SOT synced, security scanned, retrospective complete."
