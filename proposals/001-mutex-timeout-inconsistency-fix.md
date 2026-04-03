# Fix Proposal: Mutex Timeout Inconsistency

## Issue ID: BUG-001

## Severity: Medium

## Location
- `src/engine/input_routing.cpp` (line 11)
- `src/config/ipc_manager.cpp` (line 175)

## Problem Description
The input routing mutex has a 50ms timeout while the shared config mutex has a 500ms timeout. This inconsistency could lead to:
- Priority inversion scenarios
- Missed input events when config is being updated
- Inconsistent config reads during high-frequency keystroke processing

### Current Code
```cpp
// input_routing.cpp
constexpr DWORD kRoutingMutexTimeoutMs = 50;

// ipc_manager.cpp
DWORD waitResult = WaitForSingleObject(g_hSharedMutex, 500);
```

## Proposed Fix
Standardize on a consistent timeout value of 100ms for both mutexes, which balances responsiveness with reliability.

### Changes Required

#### 1. src/engine/input_routing.cpp
```cpp
// Change from:
constexpr DWORD kRoutingMutexTimeoutMs = 50;

// To:
constexpr DWORD kRoutingMutexTimeoutMs = 100;
```

#### 2. src/config/ipc_manager.cpp
```cpp
// Change from:
DWORD waitResult = WaitForSingleObject(g_hSharedMutex, 500);

// To:
DWORD waitResult = WaitForSingleObject(g_hSharedMutex, 100);
```

## Testing Strategy
1. Add stress test with rapid config updates while processing keystrokes
2. Verify no input events are dropped under load
3. Test with TSF and hook modes simultaneously

## Risk Assessment
- **Low Risk**: Simple timeout value change
- **Backward Compatible**: No API changes
- **Performance Impact**: Minimal, may actually improve responsiveness
