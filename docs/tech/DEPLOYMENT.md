# Deployment Guide

## Environments

| Environment | URL | Purpose | Deploy Method |
|-------------|-----|---------|---------------|
| Development | _[local dev URL]_ | Local dev | _[dev command]_ |
| Staging | _[staging URL]_ | Pre-release QA | Manual / CI |
| Production | _[production URL]_ | Public release | CI/CD pipeline |

## Build Process

### Prerequisites
- _[Runtime/toolchain and version]_
- _[Package manager and version, if applicable]_

### Build Commands
```
[install command]       # Install dependencies
[build command]         # Production build
[preview command]       # Preview production build locally (if applicable)
```

### Build Output
- Output directory: _[build output path]_
- _[Any optimization notes]_

## Deployment Checklist

### Pre-Deploy
- [ ] All tests pass (`[test command]`)
- [ ] Build/compile clean (`[build/type-check command]`)
- [ ] Production build succeeds
- [ ] Binary/bundle size within performance budget
- [ ] Security scan clean

### Deploy Steps
1. Merge to main branch
2. CI/CD triggers build
3. Run smoke tests against staging
4. Promote to production
5. Monitor error rates for 30 minutes

### Post-Deploy
- [ ] Verify critical user flows
- [ ] Check error monitoring dashboard
- [ ] Update CHANGELOG.md in `docs/log/`

## Rollback Procedure
1. Revert to previous deployment
2. Verify rollback succeeded
3. Create incident report in `docs/log/INCIDENT_LOG.md`
