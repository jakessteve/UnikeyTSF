# Execution Backlog

## Priority Legend

- **P0**: must complete before later architectural work is trustworthy
- **P1**: required to safely progress the modernization program
- **P2**: follow-on hardening after core risks are reduced

## Backlog

### P0 — Sprint 1

1. **Align repo truth with current hybrid runtime**
   - Update repo-facing docs so they consistently describe the app as hook-primary hybrid.
   - Acceptance: no touched doc claims pure TSF as the current runtime.
   - Risk mitigated: architecture credibility drift.

2. **Fix stale roadmap framing**
   - Replace legacy phase wording in `ROADMAP.md` with current-state-aware milestones.
   - Acceptance: roadmap reflects completed foundations and remaining modernization work.
   - Risk mitigated: execution drift from outdated milestones.

3. **Correct runtime-facing About text**
   - Remove the current “Pure TSF (x86 + x64)” claim from the settings UI.
   - Acceptance: About tab reflects the hybrid runtime truth.
   - Risk mitigated: user-facing product misstatement.

4. **Replace stale CI test discovery**
   - Stop searching for `UniKeyTSFTests.exe`; run the actual configured test graph.
   - Acceptance: CI executes the real suites declared by CMake.
   - Risk mitigated: false-green builds.

### P1 — Sprint 2 / 3

5. **Unify config versioning and field semantics**
   - Acceptance: one canonical `UniKeyConfig` version contract.
   - Risk mitigated: schema drift between native and frontend.

6. **Harden WebView/native message parsing**
   - Acceptance: typed message envelope with validation and negative-path handling.
   - Risk mitigated: silent config corruption.

### P1 — Sprint 4 / 5

7. **Enforce one routing owner per context**
   - Acceptance: no dual-processing across hook and TSF paths.
   - Risk mitigated: duplicate or dropped input.

8. **Make validation matrix blocking**
   - Acceptance: app-matrix evidence is required for TSF-default promotion.
   - Risk mitigated: rollout on incomplete evidence.

### P2 — Sprint 6

9. **Extract shared engine semantics**
   - Acceptance: hook and TSF paths follow the same reset/context/rewrite contract.
   - Risk mitigated: path-specific behavior drift.

10. **Enable guarded TSF-primary rollout**
    - Acceptance: TSF-primary can be enabled and rolled back safely.
    - Risk mitigated: irreversible rollout instability.

## Current Execution Status

- Active sprint: **Sprint 1**
- Started work: repo truth alignment, backlog creation, CI correction
- Deferred intentionally: config schema changes, routing changes, TSF-default behavior changes
