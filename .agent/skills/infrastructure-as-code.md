---
description: Infrastructure as Code - Terraform/Pulumi patterns for reproducible, version-controlled infrastructure
---

# SKILL: Infrastructure as Code (IaC)

**Trigger:** When @devops needs reproducible, version-controlled infrastructure for deployment, staging, or local dev environments.

---

## When to Use
- Setting up cloud infrastructure (hosting, databases, CDN, storage).
- Creating reproducible local development environments.
- Managing multi-environment deployments (dev → staging → production).
- Automating infrastructure provisioning in CI/CD pipelines.

---

## Core Principles

| Principle | Description | Anti-Pattern |
|---|---|---|
| **Declarative** | Define desired state, not procedural steps | Writing bash scripts to provision |
| **Version controlled** | All infra config in git, same review process as code | Manual console changes |
| **Idempotent** | Running twice produces the same result | Scripts that fail on re-run |
| **Modular** | Reusable modules for common patterns | Copy-pasting configs |
| **Immutable** | Replace infrastructure rather than patch it | SSH-ing to fix servers |

---

## Tool Selection Matrix

| Tool | Best For | Language | Cloud Support | Learning Curve |
|---|---|---|---|---|
| **Terraform** | General IaC, multi-cloud | HCL | All clouds | Medium |
| **Pulumi** | Developer-centric teams | TS/Python/Go | All clouds | Medium |
| **AWS CDK** | AWS-only shops, TypeScript teams | TypeScript | AWS only | Low (if know AWS) |
| **Docker Compose** | Local dev, multi-container setups | YAML | Local only | Low |
| **Ansible** | Server configuration management | YAML | All | Medium |

**Decision framework:**
- Single cloud vendor → Cloud-native tool (CDK, Cloud Deployment Manager)
- Multi-cloud or vendor-agnostic → Terraform
- Team of developers, not ops → Pulumi (real programming languages)
- Local dev only → Docker Compose

---

## IaC Project Structure
```
infrastructure/
├── modules/ # Reusable modules
│ ├── vpc/
│ ├── database/
│ └── app-server/
├── environments/ # Environment-specific configs
│ ├── dev/
│ │ └── main.tf
│ ├── staging/
│ │ └── main.tf
│ └── production/
│ └── main.tf
├── shared/ # Cross-environment resources
│ └── dns.tf
└── scripts/ # Helper scripts
 └── setup.sh
```

## Best Practices Checklist
```markdown
## IaC Review Checklist
- [ ] State files stored remotely (S3 + DynamoDB locking for Terraform)
- [ ] Secrets managed via vault/secrets-manager, NEVER in IaC files
- [ ] Modules used for repeatable patterns (VPC, database, app server)
- [ ] All resources tagged for cost tracking
- [ ] Plan before apply (`terraform plan` → human review → `terraform apply`)
- [ ] Environment variables used for environment-specific values
- [ ] Drift detection configured (scheduled `plan` runs)
- [ ] Rollback strategy documented
```

## Rules
- All infrastructure changes MUST go through code review (same as application code).
- **No manual console/portal changes in production** — if it's not in code, it doesn't exist.
- Drift detection runs on a schedule — alert on any manual changes.
- Sensitive values (API keys, passwords) MUST use secret management (Rule `security-standards.md`).
- Tag all resources: `environment`, `project`, `owner`, `cost-center`.
