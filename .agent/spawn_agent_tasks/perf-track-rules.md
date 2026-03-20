# Task: Performance Audit - Rules (@qc)
## Goal: Perform a static performance and scale audit on all rule definitions in `.agent/rules/`.
## Files: `.agent/rules/*`
## Expected output:
Analyze rules for:
1. Parsing overhead (how long it takes an LLM to follow them).
2. Rule conflict density (contradictions slowing decision making).
3. Verification overhead (cost of verifying these rules).

Follow the exact `.agent/workflows/spawn-performance.md` metrics requirement.
Provide quantitative findings.
Provide a ranked optimizations list.
## When done: Output a bottleneck map and summary per workflow format.
**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
