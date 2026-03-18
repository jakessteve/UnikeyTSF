---
description: Docker Containerization - multi-stage builds, image optimization, compose patterns, and security hardening
---

# SKILL: Docker Containerization

**Trigger:** When @devops sets up container builds, optimizes images, or configures multi-service environments.

---

## When to Use
- Building production-ready Docker images.
- Setting up local development environments with multiple services.
- Optimizing image size and build times.
- Configuring CI/CD container builds.

---

## The 3-Step Container Process

### Step 1: Write the Dockerfile (Multi-Stage Pattern)
```dockerfile
# Stage 1: Dependencies
FROM node:20-alpine AS deps
WORKDIR /app
COPY package*.json ./
RUN npm ci

# Stage 2: Build
FROM node:20-alpine AS builder
WORKDIR /app
COPY --from=deps /app/node_modules ./node_modules
COPY . .
RUN npm run build

# Stage 3: Production
FROM node:20-alpine AS production
WORKDIR /app
RUN addgroup -g 1001 appgroup && adduser -u 1001 -G appgroup -s /bin/sh -D appuser
COPY --from=builder --chown=appuser:appgroup /app/dist ./dist
COPY --from=builder --chown=appuser:appgroup /app/package*.json ./
RUN npm ci --production && npm cache clean --force
USER appuser
EXPOSE 3000
HEALTHCHECK --interval=30s --timeout=3s CMD wget -qO- http://localhost:3000/health || exit 1
CMD ["node", "dist/index.js"]
```

### Step 2: Optimize Image Size

| Practice | Impact | How |
|---|---|---|
| Alpine base images | ~5x smaller | Use `node:20-alpine` not `node:20` |
| Multi-stage builds | No dev deps in prod | Separate build and runtime stages |
| `.dockerignore` | Exclude unnecessary files | `node_modules`, `.git`, `*.md`, `tests/` |
| Pin versions | Reproducible builds | `node:20.11-alpine`, not `node:latest` |
| Layer ordering | Better cache hits | Copy `package.json` before source code |
| Clean caches | Removes npm cache | `npm cache clean --force` after install |

### Step 3: Docker Compose (Multi-Service)
```yaml
services:
  app:
    build:
      context: .
      target: production
    ports: ["3000:3000"]
    environment:
      - NODE_ENV=production
      - DATABASE_URL=${DATABASE_URL}
    depends_on:
      db:
        condition: service_healthy
    restart: unless-stopped

  db:
    image: postgres:16-alpine
    volumes:
      - pgdata:/var/lib/postgresql/data
    environment:
      - POSTGRES_DB=${DB_NAME}
      - POSTGRES_PASSWORD=${DB_PASSWORD}
    healthcheck:
      test: ["CMD-SHELL", "pg_isready -U postgres"]
      interval: 10s
      timeout: 5s
      retries: 5

volumes:
  pgdata:
```

---

## Security Checklist
```markdown
## Docker Security Review
- [ ] Non-root user configured (`USER appuser`)
- [ ] Base image pinned to specific version
- [ ] No secrets in the image (use env vars or Docker secrets)
- [ ] Health check endpoint defined
- [ ] Read-only filesystem where possible (`read_only: true`)
- [ ] Resource limits set (memory, CPU)
- [ ] Vulnerability scan passed (`docker scout cves`)
```

## Rules
- **Never run containers as root.** Always create and use a non-root user.
- **Never store secrets in images.** Use environment variables or secrets management.
- **Always have a health check.** Both in Dockerfile and Compose.
- **Pin base image versions.** Never use `latest` in production.
- **Scan for vulnerabilities** before deploying (`docker scout` or `trivy`).
