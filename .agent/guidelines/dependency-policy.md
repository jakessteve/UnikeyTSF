---
description: Dependency Policy - update cadence, auto-merge rules, and major upgrade reviews
---

# RULE: DEPENDENCY POLICY

**Mode:** Always On
**Scope:** @devops, @dev

---

## Update Cadence
| Type | Policy | Review Required |
|---|---|---|
| Patch (x.y.Z) | Auto-merge if tests pass | No |
| Minor (x.Y.0) | Merge after test run | Light review |
| Major (X.0.0) | Manual review required | @sa + @dev review |
| Security fix | Immediate (any level) | Post-merge review OK |

## Maximum Age
- No dependency should be more than **2 major versions** behind latest.
- Security patches must be applied within **48 hours** of disclosure.

## Adding New Dependencies
Before adding a new dependency, evaluate:
- [ ] Is there a native/simpler alternative?
- [ ] Bundle size impact (< 50KB gzipped)?
- [ ] Maintenance health (last commit < 6 months)?
- [ ] Download count (> 1000/week)?
- [ ] License compatibility (MIT, Apache-2.0, BSD preferred)?
- [ ] Security audit (no known vulnerabilities)?

## Rules
- Never add a dependency without documenting the justification.
- Never pin to a specific patch version (allow patch updates).
- Use `package-lock.json` for reproducible builds.
- Run `npm audit` as part of CI pipeline.
