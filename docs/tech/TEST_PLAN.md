# Test Plan

## Overview
Test strategy and coverage targets for the project. Update this document per-project with the actual test framework, tools, and module breakdown.

## Test Tools
- **Unit Test Framework:** _[e.g., Google Test, Catch2, Vitest, Jest, pytest]_
- **Integration Test Framework:** _[e.g., same as unit, or separate tool]_
- **E2E Test Tool:** _[e.g., Playwright, Selenium, AutoHotKey, manual scripts]_
- **Coverage Tool:** _[e.g., gcov, lcov, Istanbul, c8]_

## Test Levels

### Unit Tests
- **Coverage target:** 80% line coverage (90% for critical modules)
- **Location:** _[e.g., co-located `*.test.*` files, or `tests/unit/`]_

### Integration Tests
- **Scope:** Cross-module interactions, API contracts, data flow
- **Location:** _[e.g., `tests/integration/`]_

### E2E Tests
- **Scope:** Critical user flows, smoke tests
- **Location:** _[e.g., `tests/e2e/`]_

## Test Categories by Module

| Module | Unit | Integration | E2E | Notes |
|--------|------|-------------|-----|-------|
| _[Module 1]_ | Required | Required | Required | _[Priority / risk notes]_ |
| _[Module 2]_ | Required | Required | — | _[Priority / risk notes]_ |
| _[Module 3]_ | Required | — | — | _[Priority / risk notes]_ |

## Acceptance Criteria
- All tests pass before merge
- No regression in existing coverage
- Critical modules require 90%+ coverage
- UI components require visual / screenshot tests (if applicable)

## Test Data Management
- Static test fixtures in _[fixtures location]_
- Known-good reference data for calculation / logic engines
- Mock / stub data for external service dependencies
