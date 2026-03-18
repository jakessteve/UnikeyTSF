---
description: Model Routing — unified reference for AI model selection by task type, SPARC phase, and cost optimization
---

# RULE: MODEL ROUTING (Unified)

> [!IMPORTANT]
> **Enforced Rule.** The orchestrator MUST select the model matching the Task Type → Model Matrix below. The `model-selector` skill output is binding, not advisory. Only 3 models are in use — do NOT route to any other model.

## 1. Model Inventory

| Code | Model | Strength | Cost |
|------|-------|----------|------|
| `OPUS/Plan` | Claude Opus (Thinking) | Deep reasoning, architecture, complex planning, strategy | $$$ |
| `SONNET/Fast` | Claude Sonnet (Thinking) | Strong coding, multi-file implementation, testing, UI | $$ |
| `GEMINI-H/Plan` | Gemini 3.1 Pro (High) | Massive context, research, reports, docs, multilingual | $$ |

### Modes
- **Plan** — deep reasoning, exploration, architecture, strategy, research
- **Fast** — execution tasks, coding, testing, quick fixes

---

## 2. Task Type → Model Matrix

| Task Type | Model Code | Rationale |
|---|---|---|
| Brainstorming & Strategy | `OPUS/Plan` | Deep thinking, creative exploration |
| Research & Analysis | `GEMINI-H/Plan` | Large context window, research synthesis |
| Architecture & System Design | `OPUS/Plan` | Complex structural decisions |
| Data Enrichment & Content | `GEMINI-H/Plan` | Fast multilingual content, large context |
| Coding & Implementation | `SONNET/Fast` | Strong code generation, fast execution |
| DevOps & Infrastructure | `SONNET/Fast` | Infrastructure code + fast execution |
| Testing & QA | `SONNET/Fast` | Precise test logic, code-centric |
| UI/UX Design & Styling | `SONNET/Fast` | Visual code, CSS, component coding |
| Quick Fixes & Triage | `SONNET/Fast` | Fast coding for small changes |
| Reports & Documentation | `GEMINI-H/Plan` | Organized report generation, large output |
| Security Audit / Pentest | `OPUS/Plan` | Deep reasoning + threat analysis |

---

## 3. SPARC Phase → Model Mapping

| Phase | Agent | Model | Rationale |
|-------|-------|-------|-----------|
| **S** — Specification | @ba, @pm | `GEMINI-H/Plan` | Research + content, large context |
| **P** — Pseudocode | @sa | `OPUS/Plan` | Algorithm design, deep reasoning |
| **A** — Architecture | @sa, @designer | `OPUS/Plan` | API contracts, trade-offs |
| **R** — Refinement | @dev, @qc | `SONNET/Fast` | Coding + testing |
| **C** — Completion | @devops, @ba | `GEMINI-H/Plan` | Release notes, SOT updates |

---

## 4. Agent Defaults

| Agent | Default | Primary Task |
|---|---|---|
| @pm | `OPUS/Plan` | Orchestration, planning, strategy |
| @biz | `GEMINI-H/Plan` | Content, marketing, research; `OPUS/Plan` for strategy |
| @ba | `GEMINI-H/Plan` | Research, content; `OPUS/Plan` for deep analysis |
| @sa | `OPUS/Plan` | Architecture, system design |
| @dev | `SONNET/Fast` | Coding, implementation |
| @qc | `SONNET/Fast` | Testing, verification |
| @devops | `SONNET/Fast` | CI/CD, security code, deployment |
| @designer | `SONNET/Fast` | UI/UX, CSS, visual code |

---

## 5. Handoff Boundaries

### Worth Switching (different model category)
| From → To | Why |
|-----------|-----|
| S (GEMINI) → P (OPUS) | Research → deep logic needs stronger reasoning |
| A (OPUS) → R (SONNET) | Architecture done, coding is Sonnet's strength |
| R (SONNET) → C (GEMINI) | Coding done, docs are Gemini's strength |

### Not Worth Switching
| From → To | Why |
|-----------|-----|
| OPUS/Plan task to another OPUS/Plan task | Same model — context preservation > speed gain |
| GEMINI-H/Plan → another GEMINI-H task | Same model — adjust prompting only |
| Any complex task mid-stream | Context loss > model benefit |

---

## 6. Cost Optimization

1. **Use the cheapest model that can do the job well.** Don't use Opus for typo fixes — use Sonnet.
2. **Plan/research on Gemini High, think deeply on Opus, code on Sonnet.**
3. **Batch content tasks for Gemini.** 20 texts in one Gemini session > one-by-one on others.
4. **Don't split for <15 minutes of work.** Handoff overhead isn't worth it.
5. **When in doubt, stay.** Preserved context > marginal model advantage.

---

## 6.1 Enforcement for CLI Workers

| Task Complexity | `-Agent` Flag | `-ApprovalMode` | `-Timeout` | Context File |
|----------------|--------------|-----------------|-----------|---------------|
| **Mechanical** (lint, typo, rename, ≤2 files) | `gemini` | `Yolo` | `60` | `.agent/indexes/AGENTS-LITE.md` |
| **Integration** (multi-file impl, patterns) | `gemini` | `AutoEdit` | `300` | `.agent/indexes/AGENTS-LITE.md` |
| **Architecture/Review** (design, security audit) | `codex` | `AutoEdit` | `600` | `.agent/indexes/AGENTS-LITE.md` |

The orchestrator MUST pass `-ModelTier` (`Mechanical`, `Integration`, `Architecture`) to `spawn-agent.ps1` to enforce these defaults.

---

## 7. Mixed-Mode Tasks
1. Identify the **dominant** activity.
2. Use the dominant model + mode.
3. Note the secondary model in the task handoff.
4. For `/hc-sdlc` and `/swarm-execute`, each phase uses its SPARC mapping above.

## 8. Escalation
- Unsure → default to `OPUS/Plan` (safest all-rounder for thinking tasks) or `SONNET/Fast` (safest for coding tasks).
- User explicit preference → respect it over this rule.
- Task characteristics change mid-session → note shift, don't interrupt.
