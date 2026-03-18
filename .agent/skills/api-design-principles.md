---
description: API Design Principles - RESTful design, OpenAPI specs, versioning, and contract-first development
---

# SKILL: API Design Principles

**Trigger:** When @sa or @dev designs new API endpoints, reviews API architecture, or establishes API contracts.

---

## When to Use
- Designing new REST API endpoints.
- Reviewing API contracts for consistency.
- Establishing API standards for the project.
- Creating OpenAPI/Swagger specifications.

---

## RESTful Design Standards

### URL Conventions
```
GET    /api/v1/users          → List users
GET    /api/v1/users/:id      → Get single user
POST   /api/v1/users          → Create user
PUT    /api/v1/users/:id      → Full update
PATCH  /api/v1/users/:id      → Partial update
DELETE /api/v1/users/:id      → Delete user
```

**Rules:**
- Use **nouns** for resources, not verbs (`/users`, not `/getUsers`)
- Use **plural** for collections (`/users`, not `/user`)
- Use **kebab-case** for multi-word names (`/user-profiles`)
- Nest related resources: `/users/:id/orders`
- Max nesting depth: 2 levels

### HTTP Status Codes
| Code | Meaning | When to Use |
|---|---|---|
| 200 | OK | Successful GET, PUT, PATCH |
| 201 | Created | Successful POST that creates a resource |
| 204 | No Content | Successful DELETE |
| 400 | Bad Request | Invalid input (validation failed) |
| 401 | Unauthorized | Missing or invalid auth token |
| 403 | Forbidden | Valid auth but insufficient permissions |
| 404 | Not Found | Resource doesn't exist |
| 409 | Conflict | Duplicate resource or state conflict |
| 422 | Unprocessable | Syntactically valid but semantically wrong |
| 429 | Too Many Requests | Rate limit exceeded |
| 500 | Internal Error | Unexpected server error |

### Response Format
```json
{
  "data": { ... },
  "meta": {
    "page": 1,
    "perPage": 20,
    "total": 100
  },
  "errors": []
}
```

### Error Response Format
```json
{
  "error": {
    "code": "VALIDATION_ERROR",
    "message": "Human-readable description",
    "details": [
      { "field": "email", "message": "Invalid email format" }
    ]
  }
}
```

---

## Versioning Strategy
- Use **URL versioning**: `/api/v1/...`, `/api/v2/...`
- Never break existing versions — add new fields, don't remove old ones.
- Deprecation process: announce → warning header → sunset date → remove.

---

## Contract-First Development
1. **Define the contract** (TypeScript types or OpenAPI spec) BEFORE implementation.
2. **Share the contract** with frontend and backend teams.
3. **Implement against the contract** — both sides can work in parallel.
4. **Validate at runtime** — ensure actual API matches the contract.

### TypeScript Contract Example
```typescript
// types/api.ts — the contract
interface CreateUserRequest {
  name: string;
  email: string;
  role: 'admin' | 'user';
}

interface CreateUserResponse {
  id: string;
  name: string;
  email: string;
  role: string;
  createdAt: string;
}
```

---

## API Review Checklist
- [ ] URLs follow RESTful conventions
- [ ] HTTP methods match the operation semantics
- [ ] Status codes are correct and consistent
- [ ] Error responses include actionable messages
- [ ] Pagination implemented for list endpoints
- [ ] Input validation on all parameters
- [ ] Auth required on all non-public endpoints
- [ ] Rate limiting configured
- [ ] Response shapes documented in `docs/tech/API_CONTRACTS.md`
