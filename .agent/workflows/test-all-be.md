---
description: Full backend test suite regression sweep — discover tests, run all, file bug records for failures, enforce PR pass gate
---

# WORKFLOW: /test-all-be

> **Persona:** @qc (load `.agent/roles/@qc.md` before starting)
> **Trigger:** Before any backend PR merge · after major backend refactors · on-demand when test health is unclear.
> **Execution mode:** Sequential discovery, then sequential execution. No skipped tests.

---

## Phase 1 — Test Discovery
// turbo
1. Find all backend test files:
   ```powershell
   Get-ChildItem -Path ./tests -Recurse -Filter "*.test.ts" | Select-Object FullName
   Get-ChildItem -Path ./src -Recurse -Filter "*.spec.ts" | Select-Object FullName
   ```
2. Group tests by domain: `api/`, `services/`, `utils/`, `auth/`, `db/`.
3. Document the full list in `.hc/logs/qc/test-inventory-[date].md`.

## Phase 2 — Baseline Run
4. Run the full test suite (e.g., `npx vitest run`, `npx jest --runInBand`).
   - `--runInBand` or equivalent serial flag is MANDATORY for backend tests (no parallel execution).
5. Capture terminal output fully. Record:
   - Total tests: pass / fail / skip counts
   - Execution time
   - Any flaky tests (pass → fail across 2 runs)

## Phase 3 — Failure Triage
For each failing test:
6. Classify the failure type:
   | Type | Description |
   |---|---|
   | `unit-logic` | Pure function returning wrong output |
   | `integration` | Service+DB interaction broken |
   | `contract` | Response shape doesn't match `API_CONTRACTS.md` |
   | `regression` | Previously passing test now failing |
   | `infra` | Test environment / config issue |
7. Create a bug record for each failure → `.hc/bugs/[test-slug]-[date].md`:
   ```markdown
   # Bug: [test name]
   Status: Open
   Type: [classification from above]
   Test file: [path]
   Error message: [paste]
   Root cause hypothesis: [your analysis]
   Affected files: [list]
   Recommended owner: @dev-be / @devops / @sa
   ```

## Phase 4 — Flaky Test Identification
8. Run the test suite a second time. Compare results.
9. Any test that changed result (pass→fail or fail→pass) between runs is **flaky**. Tag it:
   - Add `// FLAKY: [symptoms]` comment in the test file.
   - Create a separate bug record with type `flaky`.

## Phase 5 — Coverage Check
10. Generate coverage report if available:
    ```powershell
    npx vitest run --coverage
    # or: npx jest --coverage --runInBand
    ```
11. Flag any service or utility file with < 70% line coverage. Add to a coverage debt log:
    ```
    .hc/logs/qc/coverage-debt-[date].md
    ```

## Phase 6 — Report & Pass Gate
12. Compile the full sweep report:
    ```markdown
    # Backend Test Sweep — [date]
    Total: [N] tests | Pass: [N] | Fail: [N] | Skip: [N]
    Flaky: [N]
    Bug records filed: [list slugs]
    Coverage debt: [list files below threshold]
    PR Pass Gate: [PASS / FAIL]
    ```
    Save to `.hc/logs/qc/sweep-[date].md`.

13. **PR Pass Gate enforcement:**
    - If `Fail > 0` → report FAIL to @pm. PR cannot merge until all failures are resolved.
    - If `Fail = 0` and `Flaky > 0` → report CONDITIONAL PASS. Flaky tests must be triaged within the current sprint.
    - If `Fail = 0` and `Flaky = 0` → report PASS. PR cleared to merge.

14. Hand the sweep report and gate result to @pm for action routing.
