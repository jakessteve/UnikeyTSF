---
description: Context Management -- budget tracking, progressive loading, compression checkpoints, skill bundles, task decomposition, and multi-agent context sync.
---

# SKILL: CONTEXT MANAGEMENT

**Trigger:** Context pressure (50%+ usage), large multi-file tasks, parallel agent coordination, or when approaching tool/token budget limits.

> Consolidates: context-optimization, context-juggler, task-decomposition, skill-bundles

---

## 1. Context Budget Tracking

| Usage | Action |
|-------|--------|
| 0-30% | Normal. Load files as needed. |
| 30-50% | Summarize completed sub-tasks. Discard old tool outputs. |
| 50-70% | **Compress actively.** Summarize all file contents. Shed old context. |
| 70%+ | **Emergency.** Flush everything except current task summary. |

### Compression Triggers (act immediately)
- Read more than 5 files in this session
- Terminal output > 50 lines
- Re-reading a file you already read
- About to start a new sub-task

---

## 2. Progressive Loading

```
Layer 1: File structure (grep for exports/classes)
Layer 2: Grep for specific pattern
Layer 3: Read only the relevant 20-50 lines
```

**Never read a 500-line file when you only need 1 function.**

---

## 3. Summarize-and-Discard

After reading a large file:
1. Extract key facts (function names, types, line numbers)
2. Write 3-5 bullet summary
3. Stop referencing full file content

```markdown
## Context Summary: [filename]
- Exports: [list]
- Key types: [names + locations]
- Entry point: [line range]
(Full file content discarded)
```

---

## 4. Mandatory Compression Checkpoints

1. **After reading 5 files** -- summarize all into combined context note
2. **After each persona switch** -- flush previous persona's working context
3. **After each swarm wave** -- summarize wave output before next wave
4. **After tool output > 50 lines** -- summarize immediately
5. **Before QC verification** -- compress to: changes + files + expected behavior

---

## 5. Task Decomposition

For Large/Epic tasks, decompose into independent chunks:
1. Identify **natural boundaries** (by module, by layer, by feature)
2. Ensure each chunk has **clear inputs and outputs**
3. Minimize cross-chunk dependencies
4. Each chunk should be delegable to a single worker

---

## 6. Multi-Agent Context Sync

When coordinating parallel agents:
- Give each agent **only their needed files** (specific paths)
- Have agents **summarize results** back to orchestrator (not full code dumps)
- Track which agent owns which files to prevent conflicts
- After all agents complete, synthesize summaries into unified state

---

## 7. Skill Bundles (Quick Lookup)

Common task patterns and which skills to load:

| Task Pattern | Load These Skills |
|---|---|
| New feature implementation | `test-driven-development`, `react-patterns`, `code-review-excellence` |
| Bug investigation | `systematic-debugging`, `test-fixing` |
| Architecture decision | `senior-architect/`, `architecture-decision-records`, `architecture-patterns` |
| CLI worker delegation | `cli-worker-lifecycle` |
| Security review | `security-audit`, `penetration-testing` |
| UI/UX changes | `design-system-uiux/`, `browser-visual-testing`, `mobile-ux-patterns` |
