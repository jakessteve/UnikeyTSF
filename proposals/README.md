# UniKeyTSF Fix Proposals

This directory contains fix proposals for identified issues in the UniKeyTSF codebase.

## Proposals

| ID | Title | Severity | Status |
|----|-------|----------|--------|
| [001](001-mutex-timeout-inconsistency-fix.md) | Mutex Timeout Inconsistency | Medium | Proposed |
| [002](002-shared-config-version-race-window.md) | Shared Config Version Check Race Window | Medium | Proposed |
| [003](003-tsf-activate-error-handling.md) | Missing Error Handling in TSF ActivateEx() | Low | Proposed |
| [004](004-startup-settings-clarification.md) | Startup Settings UI Control (Already Implemented) | N/A | Documentation |

## Summary

### Critical/Medium Issues

1. **Mutex Timeout Inconsistency (001)**: Different timeout values (50ms vs 500ms) for similar mutexes could cause priority inversion or missed events. Proposed fix: standardize to 100ms.

2. **Shared Config Version Race Window (002)**: Version check happens before atomic copy in TSF service, potentially allowing partial config reads. Proposed fix: copy first, validate after.

### Low Severity Issues

3. **Missing Error Handling in ActivateEx() (003)**: No logging when shared memory handles fail to open. Proposed fix: add debug logging.

### Documentation

4. **Startup Settings Feature (004)**: The "show settings on startup" feature already exists via `showDialogOnStartup` config field. This document clarifies how it works.

## Implementation Priority

Recommended implementation order:
1. Fix 001 (Mutex Timeout) - Simple, high impact
2. Fix 002 (Race Window) - Defensive coding
3. Fix 003 (Error Handling) - Documentation/logging only

## Testing Strategy

Each fix should include:
- Unit tests for the specific change
- Integration tests for the affected subsystem
- Regression tests to ensure no new issues introduced
