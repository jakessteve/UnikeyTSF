# Task: Performance Audit - Skills (@sa)
## Goal: Perform a static performance and scale audit on all skill definitions in `.agent/skills/`.
## Files: `.agent/skills/*/SKILL.md`
## Expected output:
Analyze skills for:
1. Token bloat / context weight.
2. Usability vs complexity (cognitive time to apply).
3. Frequency of use.

Follow the exact `.agent/workflows/spawn-performance.md` metrics requirement.
Provide quantitative findings.
Provide a ranked optimizations list.
## When done: Output a bottleneck map and summary per workflow format.
**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
