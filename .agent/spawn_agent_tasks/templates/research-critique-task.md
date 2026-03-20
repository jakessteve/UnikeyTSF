# Research Task: [Track Name]
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


## Research Question
> [Specific question this track must answer — one sentence]

## Scope
- **Files/areas to investigate:** [list specific files, directories, or external sources]
- **Boundaries:** DO NOT investigate [out-of-scope areas]
- **Time budget:** [estimated effort in minutes]

## Context
- **Project type:** [web app / native app / library / API / etc.]
- **Tech stack:** [from docs/tech/ARCHITECTURE.md]
- **This track is part of:** `/spawn-research` workflow with [N] parallel tracks

## Research Instructions
1. Investigate the research question thoroughly.
2. Gather evidence (code analysis, documentation, benchmarks, external research).
3. Document findings with **specific evidence** (file names, line numbers, URLs, metrics).
4. Assign a **confidence score (1-10)** to each finding.
5. **Apply self-critique** using the 5 Lenses below (MANDATORY — do not skip).

## Self-Critique (MANDATORY)

After completing your research, apply these 5 lenses to your OWN findings:

### Lens 1: Does It Actually Work?
- Are your recommendations implementable within the project's constraints?
- What edge cases or failure modes could break them?

### Lens 2: Does Anyone Actually Need This?
- Is this solving a real problem or an imagined one?
- What's the evidence of user/developer pain?

### Lens 3: Will It Survive Contact with Reality?
- Will this recommendation still be valid in 6 months?
- What happens at 10x scale?

### Lens 4: Is It Actually Good? (Craftsmanship)
- Is this the simplest solution? Could you recommend something simpler?
- Are you gold-plating?

### Lens 5: Opportunity Cost
- What gets delayed if the team adopts this recommendation?
- Is this the highest-leverage thing to improve?

## Output Format (MUST follow this structure)

```markdown
# Track Report: [Track Name]

## Key Findings
| # | Finding | Evidence | Confidence (1-10) |
|---|---------|----------|--------------------|
| 1 | [finding] | [specific evidence: file, metric, URL] | [score] |
| 2 | ... | ... | ... |

## Recommendations
| # | Recommendation | Impact | Effort | Risk |
|---|----------------|--------|--------|------|
| 1 | [what to do] | High/Med/Low | High/Med/Low | High/Med/Low |
| 2 | ... | ... | ... | ... |

## Self-Critique
### Strengths of This Analysis
[What this research covers well]

### Weaknesses & Blind Spots
[What was NOT investigated, what could be wrong]

### Confidence Assessment
- Overall confidence: [1-10]
- Strongest finding: [#N — why]
- Weakest finding: [#N — why]

## Limitations
[What this track did NOT cover, suggested follow-ups]
```

## Progress Reporting
After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
