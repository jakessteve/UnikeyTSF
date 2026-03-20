---
description: End-of-sprint review — scan commit history and closed tasks, generate CHANGELOG and progress summary
---

# Workflow: /sprint-review

Generate a CHANGELOG and progress summary by scanning commit history and completed tasks.

---

## Prerequisites
- Active project with git history
- Stories/tasks tracked in `.hc/stories/` and `.hc/roadmap.md`
- @pm role active

## Steps

### Step 1 — Scan Commit History
Read the recent git log for the sprint period:
```bash
git log --oneline --since="[sprint-start-date]" --until="[today]"
```
- Group commits by feature area or story ID.
- Identify: new features, bug fixes, refactors, documentation changes.

### Step 2 — Scan Completed Tasks
Read completed stories from `.hc/stories/`:
- Identify all stories marked as done (`[x]` in acceptance criteria).
- Cross-reference with the roadmap in `.hc/roadmap.md`.
- Note any stories that were planned but NOT completed.

### Step 3 — Collect Metrics
Gather quantifiable metrics (Rule `investor-metrics.md`):

1. **Run test suite:**
 ```bash
 npm test -- --coverage
 ```
2. **Run type check:**
 ```bash
 npx tsc --noEmit
 ```
3. **Check bundle size (if applicable):**
 ```bash
 npm run build
 ```
4. **Count changed files:**
 ```bash
 git diff --stat HEAD~[commit-count]
 ```

Record: tests passing, coverage %, type errors, bundle size, files changed.

### Step 3.5 — Agent Performance Benchmarks
Collect agent framework health metrics for the sprint (see benchmarking framework in audit report):

1. **Auto-Approve Rate (AAR):** Count tasks where agent proceeded autonomously (confidence ≥75) vs tasks that required user intervention. Target: **70-90%**.
 ```
 AAR = (autonomous tasks) / (total tasks) × 100%
 ```
2. **Escalation Rate:** Count times agents halted and escalated to User. Target: **5-15%** (0% = likely hallucinating through problems).
3. **Turn-to-Resolution (TTR):** Average persona switches per task. Target: routine **1.2-1.8**, complex **<5**.
4. **Token Density Estimate:** Estimate total tool calls per LoC merged in the sprint. Lower = more efficient. *(Note: practical TD measurement requires external token-counting tooling; this is the agent's self-assessment.)*
5. **Verified Items Count:** Count the number of features + fixes that were fully verified (tests pass + QC reviewed) and delivered. This is the raw throughput metric.
 ```
 Throughput = verified_items / sprint_duration_days
 ```
6. **SWE-bench Mini (Quarterly):** If this is a quarterly review, pick 5 representative closed bugs and evaluate: Could the agent have solved them autonomously? Score: X/5.

Record in the Sprint Summary under `## Agent Health Metrics`.

### Step 4 — Generate CHANGELOG
Create or update `CHANGELOG.md`:

```markdown
# Changelog

## [Sprint X] — YYYY-MM-DD

### New Features
- [Feature name]: [Brief description] (S-XXX)

### Bug Fixes
- [Bug description]: [What was fixed] (S-XXX)

### Refactors
- [What was refactored and why]

### Documentation
- [What docs were updated]

### Metrics
- Stories completed: X/Y (Z%)
- Test coverage: XX% (Δ +X%)
- Type errors: 0
- Bundle size: XXX KB
```

### Step 5 — Generate Progress Summary
Create a sprint summary artifact:

```markdown
# Sprint Review — Sprint [X]
**Period:** YYYY-MM-DD to YYYY-MM-DD | **Author:** @pm

## Summary
[2-3 sentence overview of what was accomplished]

## Completed (X stories, Y points)
| Story | Title | Size | Owner |
|-------|-------|------|-------|
| S-XXX | [Title] | M | @dev |

## Not Completed (carried over)
| Story | Title | Blocker |
|-------|-------|---------|
| S-XXX | [Title] | [Why it wasn't done] |

## Metrics
| Metric | Start of Sprint | End of Sprint | Δ |
|--------|----------------|---------------|---|
| MVP % | X% | Y% | +Z% |
| Coverage | X% | Y% | +Z% |
| Open Bugs | X | Y | -Z |

## Velocity Trend
- Sprint [X-2]: [Y] points
- Sprint [X-1]: [Y] points
- Sprint [X]: [Y] points
- Trend: Improving / Stable / Declining

## Next Sprint Focus
- [Top priority for next sprint]
```

### Step 6 — Present & Archive
1. Save CHANGELOG to `CHANGELOG.md` (project root).
2. Save sprint summary to `.hc/sprints/sprint-[X]-review.md`.
3. Update `.hc/metrics/progress.md` with latest numbers.
4. Present summary to User via Artifact for review.

---

## Output Files
| File | Location |
|------|----------|
| Changelog | `CHANGELOG.md` |
| Sprint review | `.hc/sprints/sprint-[X]-review.md` |
| Updated metrics | `.hc/metrics/progress.md` |
