---
description: Structured methodology for major dependency upgrades — risk assessment, compatibility analysis, staged rollout, and verification
---

# SKILL: Dependency Upgrade

**Trigger:** When @dev or @devops needs to upgrade npm packages, framework versions, or runtime versions.

---

## When to Use
- `npm outdated` shows major version updates available.
- A dependency has a known security vulnerability (`npm audit`).
- A framework (React, Vite, Tailwind) releases a new major version.
- Deprecation warnings appear in build output.
- Scheduled quarterly dependency review (Rule `dependency-policy.md`).

---

## The 6-Step Upgrade Process

### Step 1: Inventory and Categorize
```bash
npm outdated
```

| Update Type | Risk Level | Approach |
|---|---|---|
| **Patch** (1.0.x → 1.0.y) | Low | Batch upgrade, run tests |
| **Minor** (1.x.0 → 1.y.0) | Medium | Check changelog, upgrade in groups |
| **Major** (x.0.0 → y.0.0) | High | Individual upgrade, full process |
| **Security** (any level) | Urgent | Upgrade immediately, even mid-sprint |

### Step 2: Risk Assessment (Major Upgrades Only)
For each major upgrade, complete this assessment:

| Factor | Investigation | Tool / Command |
|---|---|---|
| Breaking changes | Read CHANGELOG / Migration Guide | Package docs, GitHub releases |
| API surface impact | How many files import this? | `grep -r "from 'pkg'" src/ \| wc -l` |
| Test coverage | Are affected paths covered? | Check coverage report |
| Peer dependencies | Does upgrading X require upgrading Y? | `npm ls pkg` |
| Package health | Is it still maintained? | npm stats, GitHub activity |
| Context7 docs | Check for migration patterns | `context7-integration` skill |

### Step 3: Create Branch and Upgrade
```bash
# 1. Create isolated branch
git checkout -b upgrade/package-name-vX

# 2. Upgrade the package
npm install package-name@latest

# 3. Fix type errors first
npx tsc --noEmit

# 4. Run tests
npm test

# 5. Check dev server works
npm run dev
```

### Step 4: Fix Breaking Changes
Follow this priority order:
1. **Import path changes** — update all import statements.
2. **API signature changes** — update function calls with new parameters.
3. **Type changes** — update TypeScript interfaces/types.
4. **Configuration changes** — update config files (vite.config, tsconfig, etc.).
5. **Behavioral changes** — verify logic still produces correct results.

### Step 5: Verify (Full Checklist)
Use `verification-before-completion` skill:
- [ ] TypeScript compiles with no errors (`npx tsc --noEmit`)
- [ ] All tests pass (`npm test`)
- [ ] Dev server runs without errors (`npm run dev`)
- [ ] Build completes (`npm run build`)
- [ ] Bundle size hasn't regressed (check `performance-budget.md`)
- [ ] Visual spot-check on key pages (use `browser-visual-testing` skill)
- [ ] No new deprecation warnings in console

### Step 6: Document
```markdown
## CHANGELOG entry
- chore(deps): upgrade [package] from vX to vY — [brief reason]

## Notes (if breaking changes were significant)
- Changed: [what changed in our code]
- Migration: [key migration steps taken]
- Risk: [any remaining risk or known issues]
```

---

## Upgrade Priority Matrix

| Signal | Priority | Timing |
|---|---|---|
| Critical security vulnerability | P0 | Same day |
| High vulnerability / actively exploited | P1 | Within the week |
| Medium vulnerability | P2 | Next sprint |
| Major framework version (React, Vite) | P2 | Within 2 months of stable release |
| Minor version with useful features | P3 | Quarterly batch |
| Patch updates | P4 | Batch monthly |

## Rules
- **Never upgrade all packages at once.** One-by-one for majors, batched for patches.
- **Never upgrade without tests.** If coverage is low, write tests first.
- **Always read the changelog** before upgrading a major version.
- **Security patches are urgent** — upgrade immediately, even mid-sprint.
- **Use Context7** to check for migration guides and breaking change documentation.
