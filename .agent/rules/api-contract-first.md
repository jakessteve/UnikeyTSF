---
description: API Contract First - API contracts must be defined before implementation
---

# RULE: API CONTRACT FIRST

**Mode:** Always On
**Scope:** @sa, @dev

---

## Core Mandate
API contracts (TypeScript interfaces or OpenAPI specs) MUST be defined and reviewed BEFORE implementation begins.

## Process
1. @sa defines the contract in `docs/tech/API_CONTRACTS.md` or TypeScript types.
2. @pm reviews the contract.
3. Frontend and backend can implement in parallel against the contract.
4. Implementation MUST match the contract exactly.
5. Contract changes require a review and version bump.

## Contract Must Include
- [ ] Request shape (method, URL, headers, body)
- [ ] Response shape (status codes, body)
- [ ] Error responses (codes, messages)
- [ ] Authentication requirements
- [ ] Rate limiting specifications
- [ ] Validation rules for each field

## Rules
- Never start implementing an API without a written contract.
- Never change a published contract without versioning.
- Contract conflicts are resolved by @sa, not by the implementer.
