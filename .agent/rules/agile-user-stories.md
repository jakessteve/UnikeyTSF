---
description: Always On — mandate standard user story format with acceptance criteria on every feature
---

# RULE: AGILE USER STORIES

**Mode:** Always On
**Scope:** All agents creating or consuming feature requirements

---

## Core Mandate

Every feature, enhancement, or change MUST be expressed as a **user story** with **acceptance criteria** before any design or implementation begins. User stories are the contract between @ba and @dev-fe/@dev-be.

---

## Binding Constraints

### 1. User Story Format
Every story MUST follow this format:

```
As a [type of user],
I want to [perform an action],
so that [I get a benefit/value].
```

**Examples:**
- "As a Vietnamese user, I want to view the lunar date for any solar date, so that I can plan traditional events."
- "As a premium user, I want to see my dashboard with all key categories, so that I can understand the full analysis."
- "Add lunar date conversion feature." (No user, no benefit)
- "Build a chart component." (Technical task, not a user story)

### 2. Acceptance Criteria
Every story MUST have **testable** acceptance criteria in checkbox format:

```markdown
### Acceptance Criteria
- [ ] Given [precondition], when [action], then [expected result]
- [ ] Happy path: [description of success case]
- [ ] Error path: [description of failure handling]
- [ ] Edge case: [at least one edge case]
```

**Non-negotiable:** Each story must cover at minimum:
- **1 happy path** — the ideal flow
- **1 error path** — what happens when something goes wrong
- **1 edge case** — boundary condition or unusual input

### 3. Story Sizing
Stories must be sized to be completable in a single phase:

| Size | File Changes | Guideline |
|------|-------------|-----------|
| **Small (S)** | ≤ 3 files | Single component or minor change |
| **Medium (M)** | 4–8 files | Feature with UI + logic |
| **Large (L)** | 9+ files | **MUST be split** into smaller stories |

If a story is **Large**, @ba MUST break it down before @dev-fe/@dev-be can start work.

### 4. Story File Format
Save stories in `.hc/stories/` with this structure:

```markdown
# Story: [Short Title]
**ID:** S-[number] | **Size:** S/M/L | **Phase:** [Phase name]
**Priority:** Must-have / Should-have / Nice-to-have

## User Story
As a [user], I want to [action], so that [benefit].

## Acceptance Criteria
- [ ] [Criterion 1]
- [ ] [Criterion 2]
- [ ] Error: [Error handling criterion]
- [ ] Edge: [Edge case criterion]

## Technical Notes (Optional)
[Any technical context @dev-fe/@dev-be needs — affected files, APIs, dependencies]

## Dependencies
- [Blocked by: S-X] or [None]
```

### 5. Definition of Done
A story is "Done" only when:
- [ ] All acceptance criteria checkboxes are checked
- [ ] Code reviewed by @dev-fe/@dev-be (self-review at minimum)
- [ ] Tests written and passing
- [ ] No regressions in existing functionality
- [ ] Design verified (if it has UI changes)

---

## Enforcement
Any feature request passed to @dev-fe/@dev-be without a properly formatted user story should be flagged as ` STORY FORMAT VIOLATION` and returned to @ba for correction.
