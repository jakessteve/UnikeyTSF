# Debug Task: [Track Name]
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


## Debug Question
> [Specific problem or area this track must investigate — one sentence]

## Scope
- **Files/areas to investigate:** [list specific files, directories, logs, or services]
- **Boundaries:** DO NOT investigate [out-of-scope areas]
- **Time budget:** [estimated effort in minutes]

## Context
- **Project type:** [web app / native app / library / API / etc.]
- **Tech stack:** [from docs/tech/ARCHITECTURE.md]
- **Known symptoms:** [error messages, stack traces, user reports — if any]
- **This track is part of:** `/spawn-debug` workflow with [N] parallel tracks

## Debug Instructions
1. Investigate the debug question from your domain expertise.
2. Search for bugs, issues, and anti-patterns within your scope.
3. For each finding, provide **specific evidence** (file:line, stack trace, reproduction steps).
4. Classify each finding by severity (P0-P3).
5. Propose a fix approach for each finding.
6. **Apply self-assessment** (MANDATORY — do not skip).

## Self-Assessment (MANDATORY)

After completing your investigation, answer these questions:

### Assessment 1: Confidence
- How confident are you that you found ALL issues in your scope? (1-10)
- What areas did you NOT investigate thoroughly?

### Assessment 2: Root Cause Depth
- Did you find root causes or just symptoms?
- Could any of your findings be effects of a deeper issue?

### Assessment 3: Fix Risk
- Could any of your proposed fixes introduce regressions?
- What tests should validate each fix?

### Assessment 4: Cross-Domain Impact
- Could any of your findings affect other tracks' domains?
- Are there dependencies between your findings and other system areas?

## Output Format (MUST follow this structure)

```markdown
# Debug Track Report: [Track Name]

## Bug Findings
| # | Bug | File/Location | Severity | Evidence | Root Cause | Fix Approach |
|---|-----|---------------|----------|----------|------------|--------------|
| 1 | [title] | [file:line] | P0/P1/P2/P3 | [evidence] | [cause] | [approach] |

## Reproduction Steps
### Bug #1: [title]
1. [step 1]
2. [step 2]
3. Expected: [X] / Actual: [Y]

## Self-Assessment
### Confidence: [1-10]
### Root causes found: [N] / Symptoms only: [N]
### High-risk fixes: [list any]
### Cross-domain impacts: [list any]

## Limitations
[What was NOT investigated, suggested follow-ups]
```

## Progress Reporting
After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
