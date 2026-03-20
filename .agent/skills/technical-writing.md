---
description: Technical Writing - professional documentation standards, templates, and quality review process
---

# SKILL: Technical Writing

**Trigger:** When @ba or any agent creates documentation — PRDs, briefs, research reports, meeting notes, glossaries, or any SOT document.

---

## When to Use
- Creating any document that will be read by others (human or agent).
- Writing PRDs, research reports, or architecture documentation.
- Drafting user-facing content (help docs, feature guides).
- Reviewing existing documentation for quality and clarity.
- Creating SOT (Source of Truth) documents.

---

## Writing Principles

### 1. Clarity First
- **Short, direct sentences.** Avoid jargon unless defined in `GLOSSARY.md`.
- **One idea per paragraph.** One purpose per document.
- **Active voice:** "The system validates input" not "Input is validated by the system."
- **Specificity:** "Reduces load time by 40%" not "Significantly improves performance."

### 2. Structure for Scanning
People scan documents, they don't read them linearly:
- Start with an **Executive Summary** — the reader should get 80% of the value from the first paragraph.
- Use **headers** (H2, H3) to create a navigable table of contents.
- Use **bullet points** for lists of 3+ items.
- Use **tables** for comparisons and structured data.
- Use **bold** for key terms on first use.
- Use **code blocks** for any technical values, commands, or file paths.

### 3. Consistency
- Use consistent terminology — define terms in `GLOSSARY.md`.
- Use consistent date format: `YYYY-MM-DD`.
- Use consistent status labels: `Draft → Review → Approved → Final`.
- Use consistent file naming: `kebab-case-document-name.md`.

---

## Document Templates

### Product Brief
```markdown
# Product Brief: [Name]
**Version:** 1.0 | **Date:** YYYY-MM-DD | **Author:** @ba

## Problem Statement
[What problem are we solving? Who is affected? What's the impact?]

## Proposed Solution
[High-level description of the solution approach]

## Success Metrics
| Metric | Current | Target | Measurement |
|---|---|---|---|
| [Metric 1] | [Baseline] | [Goal] | [How to measure] |

## Scope
### In Scope
- [Feature/capability 1]

### Out of Scope
- [Explicitly excluded item with rationale]

## Timeline
| Milestone | Target Date | Owner |
|---|---|---|
| Research complete | YYYY-MM-DD | @ba |
| Implementation start | YYYY-MM-DD | @dev |
| Release | YYYY-MM-DD | @devops |
```

### Meeting Notes
```markdown
# Meeting Notes: [Topic] — YYYY-MM-DD
**Attendees:** @agent1, @agent2
**Duration:** [X minutes]

## Decisions Made
1. [Decision with rationale]

## Action Items
| # | Action | Owner | Due | Status |
|---|---|---|---|---|
| 1 | [Task] | @agent | YYYY-MM-DD | Open |

## Open Questions
- [Question requiring follow-up — who owns the answer?]
```

### Research Report
```markdown
# Research: [Topic]
**Date:** YYYY-MM-DD | **Author:** @ba
**Confidence:** [0-100] (Rule `decision-routing.md`)

## Executive Summary
[Key findings in 2-3 sentences]

## Methodology
[How was this research conducted?]

## Findings
### Finding 1: [Title]
[Evidence and analysis]

### Finding 2: [Title]
[Evidence and analysis]

## Recommendations
1. [Recommended action with rationale]

## Sources
1. [Citation with URL]
```

### Architecture Decision Record (ADR)
```markdown
# ADR-[NNN]: [Decision Title]
**Date:** YYYY-MM-DD | **Author:** @sa | **Status:** Proposed | Accepted | Deprecated

## Context
[What is the issue? What forces are at play? What constraints exist?]

## Decision
[What is the change that we're proposing and/or doing?]

## Alternatives Considered
| Option | Pros | Cons |
|---|---|---|
| [Option A] | [Benefits] | [Drawbacks] |
| [Option B] | [Benefits] | [Drawbacks] |

## Consequences
- **Positive:** [What becomes easier or better?]
- **Negative:** [What becomes harder or worse?]
- **Risks:** [What could go wrong?]

## References
- [Link to relevant PRD, research, or discussion]
```

### CHANGELOG Entry
```markdown
## [Version] — YYYY-MM-DD

### Added
- [New feature or capability — user-benefit framing]

### Changed
- [Modification to existing behavior]

### Fixed
- [Bug fix — reference issue ID if applicable]

### Removed
- [Deprecated feature or capability]

### Security
- [Security-related change]
```
> Follow [Keep a Changelog](https://keepachangelog.com/) conventions. Group by type, newest first.

### Help Article / Feature Guide
```markdown
# [Feature Name] — How to [Action]

## Quick Start (TL;DR)
[1-3 sentences: what this feature does and the fastest way to use it.]

## Step-by-Step Guide
1. [Step with screenshot or description]
2. [Step]
3. [Step]

## Tips
- [Power-user tip or shortcut]

## Troubleshooting
| Symptom | Cause | Fix |
|---|---|---|
| [What user sees] | [Why it happens] | [How to fix it] |

## Related Features
- [Link to related feature guide]
```

---

## Quality Checklist
Before finalizing any document:

| Check | Why |
|---|---|
| Clear title, date, and author | Traceability |
| Understandable without prior context | Accessibility |
| All acronyms defined on first use | Clarity |
| Formatting consistent (headers, lists, code) | Professionalism |
| Spell-checked and grammar-checked | Quality |
| Links verified (not broken) | Usability |
| Action items have owners and due dates | Accountability |
| Version number if iterative doc | Change tracking |

## Rules
- **Start with `documentation-style-guide`** for brand voice, terminology, and bilingual standards.
- **Every document needs a purpose.** If you can't state it in one sentence, the doc is unfocused.
- **Date everything.** Undated documents are unreliable.
- **Prefer tables over prose** for structured data and comparisons.
- **Link, don't duplicate.** Reference existing docs instead of copying content.
- **Review before publishing.** Use the quality checklist above.
