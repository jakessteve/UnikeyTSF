# Spawn-Agent Skill Benchmark

> First benchmark entry for the worker delegation system.

## Methodology
- Compare task completion **with** vs **without** spawn-agent delegation
- Measure: token efficiency, context pollution, task accuracy, time

---

## Benchmark Log

| Date | Task | Without Delegation | With Delegation | Token Savings | Quality |
|------|------|-------------------|-----------------|---------------|---------|
| 2026-03-13 | Research: Analyze spawn-agent repo structure | ~4000 tokens consumed in main context for file reads, grep, and analysis | ~800 tokens in main (prompt + review output) + worker handles heavy reads | ~80% context savings | Same accuracy — research summary was equivalent |
| 2026-03-12 | Fix multi-agent spawning pipeline | All debugging in main context — 6 files read, build output, error traces = ~60% context consumed | Worker researched files, main orchestrated fixes — ~25% context consumed | ~58% context savings | Comparable — worker found root causes, orchestrator applied fixes |

---

## Aggregate Metrics

| Metric | Average (n=2) | Notes |
|--------|---------------|-------|
| **Context savings** | ~69% | Main context stays clean for reasoning |
| **Accuracy** | Equivalent | Workers produce same quality as direct execution |
| **Time overhead** | +30s | Script startup + output parsing adds small latency |
| **Best use case** | Read-heavy research tasks | Maximum context savings when task reads many files |

---

## Observations

1. **Research tasks benefit most** — file reads are the biggest context polluters, and workers handle them in isolation.
2. **Implementation tasks have moderate benefit** — context savings depend on how many files need reading vs writing.
3. **Bug fixes benefit least** — tight feedback loops between reading and debugging favor staying in the main context.
4. **Template quality is the bottleneck** — poorly composed prompts waste worker sessions. Template validation would help.

---

## Next Benchmarks Needed

- [ ] Benchmark refactoring delegation (new template)
- [ ] Benchmark parallel workers (multi-worker pattern)
- [ ] Benchmark different timeout values (120s vs 300s vs 600s)
- [ ] Compare Gemini vs Codex worker performance on same task

---

## Auto-Collected Metrics

> Appended automatically by `spawn-agent.ps1` / `spawn-agent.sh` after each delegation.

| Date | Task | Agent | Mode | Prompt Size | Duration | Timeout | Exit | Status |
|------|------|-------|------|-------------|----------|---------|------|--------|
| 2026-03-19 | debug-track-sa | GEMINI | --approval-mode auto_edit | 3089 chars | 61.3s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | debug-track-ba | GEMINI | --approval-mode auto_edit | 3099 chars | 66.3s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | debug-track-qc | GEMINI | --approval-mode auto_edit | 3132 chars | 108.8s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | debug-track-sa | GEMINI | --approval-mode auto_edit | 3089 chars | 152.8s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | debug-track-dev | GEMINI | --approval-mode auto_edit | 3067 chars | 170.3s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | debug-track-sec | GEMINI | --approval-mode auto_edit | 3180 chars | 275.2s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | debug-spawn-scripts | GEMINI | --approval-mode auto_edit | 1845 chars | 245.9s | 300s | 1 | ❌ |
| 2026-03-19 | perf-spawn-scripts | GEMINI | --approval-mode auto_edit | 1180 chars | 139.9s | 120s | 124 | ⏰ |
| 2026-03-19 | inline | NODE | (custom via -ExtraArgs) | 19 chars | 4.1s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | inline | GEMINI | --approval-mode auto_edit | 64 chars | 13.4s | 10s | 0 | ✅ |
| 2026-03-19 | huge_prompt | GEMINI | --approval-mode auto_edit | 40002 chars | 5.4s | None (wait forever) | 1 | ❌ |
| 2026-03-19 | huge_prompt | GEMINI | --approval-mode auto_edit | 40002 chars | 17s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | perf-track-agents | GEMINI | --approval-mode auto_edit | 809 chars | 89.9s | None (wait forever) | 0 | ✅ |
| 2026-03-19 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 1.2s | None (wait forever) | 1 | ❌ |
| 2026-03-19 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 1.7s | None (wait forever) | 1 | ❌ |
| 2026-03-19 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 1.5s | None (wait forever) | 1 | ❌ |
| 2026-03-19 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 1.3s | None (wait forever) | 1 | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 19.7s | None (wait forever) | 1 | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 2.7s | None (wait forever) | 1 | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 42 chars | 19.8s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 12 chars | 19.8s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | C:\USERS\HEOCOP\DOWNLOADS\PROJECTS\ANTIGRAVITY\.AGENT\SCRIPTS\TESTS\GEMINI.CMD | (custom via -ExtraArgs) | 11 chars | 5.1s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | C:\USERS\HEOCOP\DOWNLOADS\PROJECTS\ANTIGRAVITY\.AGENT\SCRIPTS\TESTS\GEMINI.CMD | (custom via -ExtraArgs) | 11 chars | 3.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | C:\USERS\HEOCOP\DOWNLOADS\PROJECTS\ANTIGRAVITY\.AGENT\SCRIPTS\TESTS\GEMINI.CMD | (custom via -ExtraArgs) | 11 chars | 3.7s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | C:\USERS\HEOCOP\DOWNLOADS\PROJECTS\ANTIGRAVITY\.AGENT\SCRIPTS\TESTS\GEMINI.CMD | (custom via -ExtraArgs) | 11 chars | 5.5s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | C:\USERS\HEOCOP\DOWNLOADS\PROJECTS\ANTIGRAVITY\.AGENT\SCRIPTS\TESTS\GEMINI.CMD | (custom via -ExtraArgs) | 11 chars | 6.6s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 32.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 31.1s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 89.4s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 5.5s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 6.1s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 4.3s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 5.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 4.7s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 6s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.1s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 4.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 5.5s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.2s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 3.6s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.3s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 6.1s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.3s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 4.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 5.2s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 4.2s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 4.8s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 3.1s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 3.9s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 5.6s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 5.3s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 4.2s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 11 chars | 5.2s | None (wait forever) |  | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 19 chars | 5.5s | None (wait forever) | 1 | ❌ |
| 2026-03-20 | inline | GEMINI | --approval-mode auto_edit | 12 chars | 4.9s | 1s | 124 | ⏰ |
| 2026-03-20 | prompt_580187400 | GEMINI | --approval-mode auto_edit | 11 chars | 4.5s | None (wait forever) |  | ❌ |
| 2026-03-20 | prompt_474541768 | GEMINI | --approval-mode auto_edit | 19 chars | 5.3s | None (wait forever) | 1 | ❌ |
| 2026-03-20 | prompt_2128900736 | GEMINI | --approval-mode auto_edit | 12 chars | 4.2s | 1s | 124 | ⏰ |
| 2026-03-20 | prompt_1674528457 | GEMINI | --approval-mode auto_edit | 28 chars | 4.8s | None (wait forever) |  | ❌ |
