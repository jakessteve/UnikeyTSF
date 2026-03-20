# Research Task
> **Performance Rule:** Do not load the full AGENTS.md. Load only the relevant .agent/indexes/AGENTS-LITE-<role>.md and lazy-load specific skills as needed.


> For read-only codebase analysis. Worker MUST NOT modify any files.

```markdown
# Task: [RESEARCH TOPIC]

## Goal
[What question(s) need answering. Be specific.]

## Project Context
- **Project type**: Read `package.json` for framework and stack
- **Domain**: Read `README.md` for project domain
- **Source**: `src/` — main source directory
- **Utils**: `src/utils/` — core calculations and utilities

## Where to Look
| Location | What to find |
|----------|-------------|
| `src/services/[engine]/` | [What to analyze] |
| `src/utils/[util].ts` | [What to find] |
| `src/components/[area]/` | [What to check] |

## Questions to Answer
1. [Specific question 1]
2. [Specific question 2]
3. [Specific question 3]

## Constraints
- **READ-ONLY** — DO NOT modify any files
- DO NOT run build or test commands unless asked
- Focus on the directories listed above — don't scan the entire codebase

## Output Format

### Summary
[2-3 sentence summary of findings]

### Detailed Findings
| Question | Answer | Evidence (file:line) |
|----------|--------|---------------------|

### Architecture Observations
- [Patterns found]
- [Dependencies between modules]
- [Potential issues or tech debt]

### Recommendations
- [Actionable next steps based on findings]
```
