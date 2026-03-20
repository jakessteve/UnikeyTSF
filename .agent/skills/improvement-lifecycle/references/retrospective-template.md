# Retrospective Template

```markdown
# Retrospective — [Phase/Feature]
**Date:** YYYY-MM-DD | **Facilitator:** @ba
**Phase Duration:** [start → end]

## What Went Well
- [Item with evidence from logs]

## What Didn't Go Well
- [Item with evidence from logs]

## What To Improve
- [Improvement] → Target: [rule/skill/workflow to update]

## Phase Metrics
| Metric | Value |
|---|---|
| Stories completed | X / Y |
| Bugs found | X (Critical: X, High: X) |
| Test coverage | XX% |
| Security issues | X |
| Blockers | X |

## SOT Sync Status
| Document | In Sync? | Notes |
|---|---|---|
| `docs/biz/PRD.md` | Yes/No | [drift?] |
| `docs/tech/ARCHITECTURE.md` | Yes/No | [undocumented changes?] |
| `docs/tech/API_CONTRACTS.md` | Yes/No | [violations?] |

## Recurring Patterns
[Patterns seen across phases — improvement candidates]

## Action Items
- [ ] [Improvement] → Owner: @agent → File: [.agent/...]
```

## Improvement Proposal Template

```markdown
# Improvement Proposal — [Title]
**Date:** YYYY-MM-DD | **Proposed by:** @pm

## Pattern Observed
[Recurring issue with evidence from logs]

## Root Cause
[Why this keeps happening]

## Proposed Change
- **File:** `.agent/[type]/[filename].md` | **Type:** Rule / Skill / Workflow
- **Change:** [What to add/modify]

## Expected Impact / Risk
[How this prevents recurrence] / [Any risk from the change]
```
