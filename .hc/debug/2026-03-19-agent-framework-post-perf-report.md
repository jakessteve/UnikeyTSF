# Debug Report — `.agent/` Framework (Post-Performance Upgrade)

**Date:** 2026-03-19 | **Requested by:** User
**Debug tracks:** 4 | **Agents involved:** @sa, @ba, @dev, @devops (persona-switch)

## Executive Summary

Ran a full /spawn-debug audit against the `.agent/` framework after the performance upgrade (spawn template extraction, MANIFEST trim, AGENTS-LITE expansion, orphan fix). The framework health test **PASSED** with no critical defects. Found **4 bugs** — all documentation drift from the upgrade, where files that reference AGENTS-LITE variants or rule counts weren't updated to reflect the new 7-variant structure.

## Bug Dashboard
| Total | P0 | P1 | P2 | P3 |
|---|---|---|---|---|
| 4 | 0 | 2 | 2 | 0 |

## Ranked Fix Plan

### P1 — Fix This Sprint

| # | Bug | Root Cause | File | Fix Approach | Effort |
|---|---|---|---|---|---|
| 1 | `execution-protocol.md` §8 lists only 3 LITE variants (dev, qc, pm) instead of 7 | Doc drift after AGENTS-LITE expansion | [execution-protocol.md](file:///c:/Users/HeoCop/Downloads/Projects/Antigravity/.agent/rules/execution-protocol.md#L117) | Update line 117 to list all 7 variants or point to `instructions.md` | Low |
| 2 | `instructions.md` says "do NOT load all 25 rule files" but actual count is 26 | Doc drift after `context-budget` added to MANIFEST | [instructions.md](file:///c:/Users/HeoCop/Downloads/Projects/Antigravity/.agent/instructions.md#L75) | Update "25" → "26" on line 75 | Low |

### P2 — Fix Next Sprint

| # | Bug | Root Cause | File | Fix Approach | Effort |
|---|---|---|---|---|---|
| 3 | `anti-patterns-swarm.md` §7.7 says "All workers use `AGENTS-LITE.md`" but should recommend role-scoped variants | Doc drift — pre-dates role-scoped LITE expansion | [anti-patterns-swarm.md](file:///c:/Users/HeoCop/Downloads/Projects/Antigravity/.agent/rules/anti-patterns-swarm.md#L51) | Update line 51 to say "role-scoped `AGENTS-LITE-{role}.md`" | Low |
| 4 | `model-routing.md` §6.1 context file column always shows generic `AGENTS-LITE.md` | Doc drift — pre-dates role-scoped LITE expansion | [model-routing.md](file:///c:/Users/HeoCop/Downloads/Projects/Antigravity/.agent/rules/model-routing.md#L105-L107) | Update context column to `AGENTS-LITE-{role}.md` | Low |

## Root Cause Map

| Root Cause | Bugs | Severity | Affected Components |
|---|---|---|---|
| Documentation drift after AGENTS-LITE expansion | #1, #3, #4 | P1-P2 | `execution-protocol.md`, `anti-patterns-swarm.md`, `model-routing.md` |
| Documentation drift after context-budget rule addition | #2 | P1 | `instructions.md` |

> **Single root cause:** When the performance audit added 3 new AGENTS-LITE variants and added `context-budget` to the MANIFEST, not all _downstream consumers_ of those references were updated. The files modified by the perf audit (AGENTS.md, instructions.md LITE listing, workflow MANIFESTs) are correct, but 3 rule files that also mention LITE or rule counts were missed.

## Verification Results

| Check | Result |
|---|---|
| Framework health test (`framework.health.test.ps1`) | ✅ PASSED |
| Workflow MANIFEST count vs filesystem | ✅ Match (35) |
| Rules MANIFEST count vs AGENTS.md | ✅ Match (26) |
| Skills MANIFEST count vs AGENTS.md | ✅ Match (78) |
| Spawn-base-template references (6 workflows) | ✅ All 6 reference it |
| Context-budget cross-references | ✅ In MANIFEST + instructions.md |
| Stale old counts (24/69/33) anywhere | ✅ None found |
| Leftover temp files | ✅ None found |
| Spawn task templates (11 files) | ✅ All referenced by workflows |

## Methodology
- Tracks investigated: Architecture (@sa), Requirements (@ba), Code & Config (@dev), Infrastructure (@devops)
- Cross-reference validation via `grep_search` across all `.agent/` files
- Framework health test automated execution
- Stale count sweep for old values (24, 69, 33)

## Limitations
- Did not audit the content accuracy of individual skill `.md` files (78 files — would require dedicated Track)
- Did not audit role file content vs AGENTS.md role table alignment
- PowerShell scripts not linted or unit-tested beyond the health check
