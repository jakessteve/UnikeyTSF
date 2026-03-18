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
| 2026-03-17 | phase-3-vietnamese-typing-engine | GEMINI | auto_edit | 4746 chars | 401s | 400s | 124 | ⏰ |
| 2026-03-17 | test_ba | GEMINI | yolo | 622 chars | 89.4s | 120s | 0 | ✅ |
| 2026-03-17 | test_dev | GEMINI | yolo | 609 chars | 120.9s | 120s | 124 | ⏰ |
| 2026-03-17 | round2_qc | GEMINI | yolo | 596 chars | 41.2s | 120s | 0 | ✅ |
| 2026-03-17 | round2_ba | GEMINI | yolo | 607 chars | 54.8s | 120s | 0 | ✅ |
| 2026-03-17 | round2_dev | GEMINI | yolo | 603 chars | 74.4s | 120s | 0 | ✅ |
| 2026-03-18 | phase2-1-spellcheck | GEMINI | auto_edit | 1911 chars | 301.3s | 300s | 124 | ⏰ |
| 2026-03-18 | unikey-improvements | GEMINI | --approval-mode auto_edit | 1776 chars | 213.9s | None (wait forever) | 0 | ✅ |
| 2026-03-18 | decompose-main | GEMINI | --approval-mode auto_edit | 2626 chars | 598.5s | None (wait forever) | 0 | ✅ |
