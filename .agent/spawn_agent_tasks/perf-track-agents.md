# Task: Performance Audit - Agents (@pm)
## Goal: Perform a static performance and scale audit on all agent definitions in `.agent/roles/`.
## Files: `.agent/roles/*`
## Expected output:
Analyze all agents for:
1. Context token footprint.
2. Cognitive overload (too many responsibilities).
3. Redundancy (overlapping roles).

Follow the exact `.agent/workflows/spawn-performance.md` metrics requirement.
Provide quantitative findings (e.g., token size, responsibility count).
Provide a ranked optimizations list.
## When done: Output a bottleneck map and summary per workflow format.
**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
