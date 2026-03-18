---
description: CI/CD Pipeline - design patterns for build, test, and deployment automation with rollback and environment management
---

# SKILL: CI/CD Pipeline

**Trigger:** When @devops designs, configures, or maintains build and deployment pipelines.

---

## When to Use
- Setting up CI/CD for a new project.
- Adding pipeline stages (security scan, performance test, deployment).
- Optimizing pipeline speed (caching, parallelization).
- Configuring deployment strategies (blue-green, canary, rolling).
- Debugging pipeline failures.

---

## Pipeline Architecture

### Standard Pipeline Stages
```
Code Push → Lint → Test → Security Scan → Build → Deploy (Staging) → Approval → Deploy (Prod)
 │ │ │ │ │ │ │
 │ │ │ │ │ └─ Smoke test ──────────────┘
 │ │ │ │ └─ Bundle size check
 │ │ │ └─ npm audit + Snyk/Trivy
 │ │ └─ Unit + Integration + Coverage threshold
 │ └─ ESLint + Prettier
 └─ Triggered on push to main/develop or PR
```

### GitHub Actions Template
```yaml
name: CI/CD Pipeline
on:
 push:
 branches: [main, develop]
 pull_request:
 branches: [main]

jobs:
 lint:
 runs-on: ubuntu-latest
 steps:
 - uses: actions/checkout@v4
 - uses: actions/setup-node@v4
 with:
 node-version: '20'
 cache: 'npm'
 - run: npm ci
 - run: npm run lint

 test:
 needs: lint
 runs-on: ubuntu-latest
 steps:
 - uses: actions/checkout@v4
 - uses: actions/setup-node@v4
 with:
 node-version: '20'
 cache: 'npm'
 - run: npm ci
 - run: npm test -- --coverage
 - name: Check coverage threshold
 run: |
 COVERAGE=$(npx vitest run --coverage --reporter=json | jq '.total.lines.pct')
 if (( $(echo "$COVERAGE < 80" | bc -l) )); then
 echo "Coverage $COVERAGE% is below 80% threshold"
 exit 1
 fi

 security:
 needs: lint
 runs-on: ubuntu-latest
 steps:
 - uses: actions/checkout@v4
 - run: npm audit --audit-level=high

 build:
 needs: [test, security]
 runs-on: ubuntu-latest
 steps:
 - uses: actions/checkout@v4
 - uses: actions/setup-node@v4
 with:
 node-version: '20'
 cache: 'npm'
 - run: npm ci
 - run: npm run build
 - name: Check bundle size
 run: |
 SIZE=$(du -sb dist/ | cut -f1)
 echo "Bundle size: $((SIZE / 1024))KB"
 - uses: actions/upload-artifact@v4
 with:
 name: build
 path: dist/

 deploy-staging:
 needs: build
 if: github.ref == 'refs/heads/develop'
 runs-on: ubuntu-latest
 environment: staging
 steps:
 - uses: actions/download-artifact@v4
 with:
 name: build
 - run: echo "Deploy to staging"

 deploy-production:
 needs: build
 if: github.ref == 'refs/heads/main'
 runs-on: ubuntu-latest
 environment: production
 steps:
 - uses: actions/download-artifact@v4
 with:
 name: build
 - run: echo "Deploy to production"
```

---

## Deployment Strategies

| Strategy | How | Rollback Speed | Risk | Best For |
|---|---|---|---|---|
| **Blue-Green** | Two environments, swap traffic | Instant | Low | Production services |
| **Canary** | Route 5-10% traffic to new version | Fast | Low | Large user bases |
| **Rolling** | Replace instances one at a time | Slow | Medium | Stateless services |
| **Recreate** | Stop old, start new | Downtime | High | Dev/staging only |

### Selection Guide
- **Blue-Green:** Best for apps where instant rollback is critical.
- **Canary:** Best when you want to validate with real traffic before full rollout.
- **Rolling:** Best for services where some downtime during rollout is acceptable.

---

## Environment Management

```
.env.example → Committed (template only, no real values)
.env.local → NOT committed (local dev overrides)
.env.staging → Server-side only (CI secrets)
.env.production → Server-side only (CI secrets)
```

- **NEVER** commit real secrets to git (Rule `security-standards.md`).
- Use CI/CD secrets management (GitHub Secrets, Vault, AWS Secrets Manager).
- Rotate secrets on a regular schedule (quarterly minimum).

## Rollback Procedure
```markdown
## Rollback Checklist
1. [ ] Identify the issue (monitoring alerts, user reports, error spike)
2. [ ] Confirm rollback is the right action (not a fix-forward)
3. [ ] Execute rollback to last known good deployment
4. [ ] Verify service restored (health checks, smoke tests)
5. [ ] Notify @pm and @dev of rollback with incident details
6. [ ] Create bug report in `.hc/bugs/`
7. [ ] Schedule postmortem (skill `postmortem-writing`)
```

## Pipeline Optimization

| Technique | Impact | How |
|---|---|---|
| Dependency caching | -60% install time | `actions/cache@v4` or `cache: 'npm'` |
| Parallel jobs | -40% total time | Run lint, test, security in parallel |
| Build artifact reuse | -50% build time | Upload once, download in deploy jobs |
| Selective testing | -30% test time | Only run tests for changed files |

## Rules
- **Pipeline must block merge on failure.** No green-squiggly deploys.
- **Security scan is mandatory.** No exceptions, even for "quick fixes."
- **Coverage threshold is enforced.** Pipeline fails if below target (80%).
- **Secrets NEVER in pipeline files.** Use CI/CD secret management only.
- **Pipeline changes need review.** Treat `.github/workflows/` like production code.
