# Phase Log Templates

## Dev Log (@dev)
```markdown
# Dev Log — [Phase/Feature]
**Date:** YYYY-MM-DD | **Author:** @dev

## Decisions Made
| Decision | Rationale | Alternatives Considered |
|---|---|---|

## Blockers Encountered
- [Blocker] → Resolution: [How resolved]

## Technical Debt Introduced
- [ ] [Debt item] — Reason: [why] — Severity: Low/Med/High

## Workarounds Used
- [Workaround] — Proper fix: [What should be done later]

## Code Metrics
- Files changed: X | Lines: +X / -X | New dependencies: [list]
```

## Bug Log (@qc)
```markdown
# Bug Log — [Phase/Feature]
**Date:** YYYY-MM-DD | **Author:** @qc

## Bugs Found
| ID | Severity | Description | Root Cause | Fix | Regression Risk |
|---|---|---|---|---|---|

## Bug Patterns
- [Patterns observed across multiple bugs]

## Test Coverage Delta
- Before: XX% → After: XX% | New tests: X | Edge cases: [list]
```

## Security Log (@devops)
```markdown
# Security Log — [Phase/Feature]
**Date:** YYYY-MM-DD | **Author:** @devops

## Vulnerabilities Found
| ID | Severity | Type | Location | Status |
|---|---|---|---|---|

## Dependency Audit
- New deps: [list] | Audit result: [clean/vulns] | Supply chain risk: Low/Med/High
```

## Decision Log (@pm)
```markdown
# Decision Log — [Phase/Feature]
**Date:** YYYY-MM-DD | **Author:** @pm

## Key Decisions
| Decision | Context | Outcome | Would Repeat? |
|---|---|---|---|

## Scope Changes
- [Change from original PRD and why]
```
