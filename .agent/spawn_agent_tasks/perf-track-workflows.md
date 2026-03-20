# Task: Performance Audit - Workflows (@pm)
## Goal: Perform a static performance and scale audit on all workflow definitions in `.agent/workflows/`.
## Files: `.agent/workflows/*`
## Expected output:
Analyze all workflows for:
1. Linear blocking steps (synchronous bottlenecks).
2. Number of spawned agents required.
3. Execution time estimations and token usage impact.

Follow the exact `.agent/workflows/spawn-performance.md` metrics requirement.
Provide quantitative findings.
Provide a ranked optimizations list.
## When done: Output a bottleneck map and summary per workflow format.
**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
