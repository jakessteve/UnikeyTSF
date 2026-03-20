---
description: Error Handling Standards - structured error types, boundaries, and user-facing messages
---

# RULE: ERROR HANDLING STANDARDS

**Mode:** Always On
**Scope:** @dev-fe, @dev-be

---

## Error Categories
| Category | HTTP Code | User Message | Log Level |
|---|---|---|---|
| Validation error | 400 | Specific field error | `warn` |
| Auth error | 401/403 | Generic "access denied" | `warn` |
| Not found | 404 | "Resource not found" | `info` |
| Business logic error | 422 | Explain what went wrong | `warn` |
| Rate limit | 429 | "Please try again later" | `info` |
| Internal error | 500 | Generic "something went wrong" | `error` |

## Error Typing
```typescript
// Define structured error types
interface AppError {
 code: string; // Machine-readable: 'VALIDATION_ERROR'
 message: string; // User-readable: 'Invalid birth date'
 details?: ErrorDetail[];
 cause?: Error; // Original error for debugging
}

interface ErrorDetail {
 field: string; // 'birthDate'
 message: string; // 'Date must be in the past'
}
```

## Rules
- Never expose stack traces to users.
- Never log sensitive data (passwords, tokens) in error logs.
- Never silently swallow errors (`catch (e) {}`).
- Always log errors with context (who, what, where).
- Use error boundaries in React for graceful UI recovery.
- Distinguish between expected errors (validation) and unexpected errors (bugs).
- Include enough context to debug without reproducing.
