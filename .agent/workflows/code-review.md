---
description: PR-level code review — quality check with structured feedback
---

# Code Review Workflow

> Quick, focused review for individual PRs or code changes.

## Step 1 — Understand the Change

1. Read the PR description or task context.
2. List all changed files.
3. Identify the scope: single-file fix, multi-file feature, or refactor.

## Step 2 — Automated Checks

// turbo-all

```bash
npm run lint
npx tsc --noEmit
npm test -- --passWithNoTests
```

If any fail → **flag immediately** before detailed review.

## Step 3 — Manual Review Checklist

### Architecture
- [ ] Changes align with `docs/tech/ARCHITECTURE.md` and `docs/tech/API_CONTRACTS.md`
- [ ] No circular dependencies introduced
- [ ] File placement follows `scalable-folder-structure` rule

### Code Quality
- [ ] No `any` types (Rule: `code-standards`)
- [ ] No duplicated logic (DRY check)
- [ ] Functions < 50 lines, files < 300 lines
- [ ] Meaningful variable/function names (English)
- [ ] Error handling present for async operations

### Security
- [ ] No hardcoded secrets or API keys
- [ ] User input validated/sanitized
- [ ] No `console.log` with sensitive data

### Testing
- [ ] New logic has companion test file
- [ ] Tests cover happy path + error path + edge cases
- [ ] No skipped or disabled tests

### UI (if visual changes)
- [ ] Dark mode parity
- [ ] Responsive at 375px and 1024px
- [ ] Semantic tokens used (no raw hex/rgb)
- [ ] Accessible (aria-labels, alt text, focus states)

## Step 4 — Review Verdict

**Output format:**
```markdown
## Code Review: [Change Description]

### Approve / Request Changes / Reject

**Summary:** [1-2 sentence overall assessment]

### Findings
| # | Severity | File | Line | Issue | Suggestion |
|---|---|---|---|---|---|

### What's Good
- [Positive observations]
```

## Step 5 — Follow-Up

If changes requested: specify exactly what needs to change and why. Re-review after fixes.
