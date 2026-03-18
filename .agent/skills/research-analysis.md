---
description: Research & Analysis - structured multi-step methodology for investigation, deep research, and data synthesis
---

# SKILL: Research & Analysis

**Trigger:** When @ba or any agent needs to investigate a domain, technology, market, competitor, or problem space — from quick scans to deep dives.

---

## When to Use
- Investigating a new feature domain (e.g., lunar calendar algorithms).
- Evaluating technology choices (e.g., which charting library?).
- Market research for pitch decks or PRDs.
- Understanding a complex problem space before writing requirements.
- Comparing multiple approaches or solutions.
- Competitive landscape analysis.

---

## The 6-Step Research Process

### Step 1: Define the Research Question
Write a clear, bounded question:
```markdown
## Research Question
**Topic:** [What are you investigating?]
**Scope:** [What's in scope? What's out?]
**Depth:** [Surface scan | Standard | Deep dive]
**Deliverable:** [What output will this produce?]
**Time budget:** [Max N searches/reads to avoid rabbit holes]
```

### Step 2: Source Gathering (3+ sources)
Use a minimum of 3 different source types, in this priority order:

| Source Type | Tool | When |
|---|---|---|
| Internal SOT | `view_file` | `docs/tech/ARCHITECTURE.md`, `docs/biz/PRD.md`, `.hc/research/` first |
| Official docs | `context7` | Library APIs, framework usage |
| Web articles | `search_web` | Best practices, market data, comparisons |
| Project code | `grep_search` / `view_file` | Current implementation |
| Academic/standards | `search_web` | Algorithms, specifications, certifications |

### Step 3: Cross-Reference
For every claim or fact:
- Verify against at least **2 sources**.
- If sources disagree, note the discrepancy and assess credibility (official > blog post).
- If only 1 source exists, flag as ` Single-source — verify before relying on this.`

### Step 4: Synthesize
Compile findings into a structured brief:

```markdown
# Research Report — [Topic]
**Date:** YYYY-MM-DD | **Author:** @ba
**Status:** Draft | Review | Final

## Executive Summary
[2-3 sentence overview of findings]

## Key Findings
| # | Finding | Sources | Confidence |
|---|---------|---------|-----------|
| 1 | [Fact] | [Source 1, Source 2] | High/Medium/Low |
| 2 | [Fact] | [Source] | Single-source |

## Comparison (if evaluating options)
| Criteria | Option A | Option B | Option C |
|----------|---------|---------|---------|
| [Criterion 1] | [Assessment] | [Assessment] | [Assessment] |

## Recommendations
1. [Primary recommendation with justification]
2. [Alternative if primary doesn't work]

## Open Questions
- [ ] [What couldn't you find out?]
- [ ] [What needs human expertise?]

## Sources
1. [Full citation with URL]
2. [Full citation with URL]
```

### Step 5: Identify Gaps
Explicitly call out:
- What you couldn't find reliable information on.
- What requires subject matter expert input.
- What assumptions you're making.

### Step 6: Review
Before handoff, self-check:
- [ ] Are all claims supported by sources?
- [ ] Is the recommendation actionable?
- [ ] Are there any assumptions that need validation?
- [ ] Is the document clear to someone without context?

---

## Domain-Specific Research Triggers
- Vietnamese astrology terms (Tử Vi, Tứ Hóa, Cục, Nạp Âm, Sao chủ)
- Lunar calendar algorithms (solar term calculation, leap month rules)
- Phong thủy (Feng Shui) concepts and directional rules
- Mai Hoa Dịch Số (Plum Blossom Numerology) references

## Preferred Sources
- Official library documentation (React, Vite, TypeScript)
- MDN Web Docs for web standards
- Vietnamese astrology reference sites for domain knowledge
- GitHub issues/discussions for library-specific bugs

---

## Serendipity Protocol (Adaptive Research Expansion)

> **Purpose:** Traditional linear research misses insights that emerge *during* investigation. This protocol allows agents to follow unexpected but relevant tangents — mimicking how expert researchers organically discover new angles.

### When to Trigger
During Steps 2-3 (Source Gathering / Cross-Reference), if the agent encounters:
- An unexpected competitor, tool, or approach not in the original scope
- A contradictory finding that challenges the research premise
- A tangential domain that could significantly impact the recommendation
- A surprising data point that warrants deeper investigation

### Expansion Rules
1. **Max 2 tangent levels deep.** Tangent of a tangent is the limit. No rabbit holes.
2. **Max 3 tangent sub-tasks per research session.** Budget tangent time.
3. **Time-box each tangent to 2 search queries.** Quick scan, not deep dive.
4. **Always return to the main thread** after exploring a tangent.
5. **Log every tangent** in the Serendipity Log, even if it turns out to be irrelevant.

### Serendipity Log Template
Add to the research report (Step 4):

```markdown
## Serendipity Log
| # | Tangent | Discovered During | Relevance | Pursued? | Finding |
|---|---------|-------------------|-----------|----------|---------|
| 1 | [What was discovered] | [Which step/source] | High/Med/Low | Yes/No | [Brief outcome or "Not pursued — logged for future"] |
```

### Confidence Integration (Rule `decision-routing.md`)
- If a tangent discovery **changes the primary recommendation**, flag the finding with a confidence score.
- If the tangent **contradicts** the current best finding, the overall research confidence score drops until the contradiction is resolved.

---

## Rules
- **Never fabricate sources.** Rule `anti-patterns-core.md` applies.
- **Cite everything.** Every fact needs a source.
- **Flag uncertainty.** "I don't know" > "I think..."
- **Time-box research.** Max 5-7 search queries per topic (excluding serendipity tangents) to avoid rabbit holes.
- **Deliver actionable output.** Research without recommendations is incomplete.
- **Log tangents.** Even unused tangent discoveries go in the Serendipity Log (Rule `decision-routing.md`).
