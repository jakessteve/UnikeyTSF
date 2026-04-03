# Fix Proposal: Missing Error Handling in TSF ActivateEx()

## Issue ID: BUG-003

## Severity: Low

## Location
- `src/tsf/text_service.cpp` (lines 229-236)

## Problem Description
The `_UpdateConfig()` call in `ActivateEx()` doesn't check if shared memory handles were successfully opened. If the manager EXE hasn't started yet, the TSF service continues with default config values silently, which could lead to:
- User confusion when settings don't take effect
- Silent failure mode that's hard to debug
- Inconsistent behavior between first activation and subsequent activations

### Current Code
```cpp
// Open shared config handles (cached for efficiency)
_hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
_hMutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, UNIKEY_SHARED_MUTEX_NAME);

_UpdateConfig();  // No error check!
```

## Proposed Fix
Add logging for when shared memory handles fail to open, and consider retry logic.

### Changes Required

#### src/tsf/text_service.cpp - ActivateEx() method
```cpp
STDMETHODIMP CUniKeyTextService::ActivateEx(
    ITfThreadMgr* pThreadMgr, TfClientId tfClientId, DWORD /*dwFlags*/)
{
    _pThreadMgr = pThreadMgr;
    _tfClientId = tfClientId;

    HRESULT hr = _InitThreadMgrEventSink();
    if (FAILED(hr)) {
        Deactivate();
        return hr;
    }

    hr = _InitKeyEventSink();
    if (FAILED(hr)) {
        Deactivate();
        return hr;
    }

    // Open shared config handles (cached for efficiency)
    _hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    _hMutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, UNIKEY_SHARED_MUTEX_NAME);

    if (!_hMapFile || !_hMutex) {
        // Log for debugging - manager EXE may not have started yet
        OutputDebugStringW(L"[UniKeyTSF][TSF] Warning: Shared config handles not available. "
                          L"Using defaults until manager EXE creates shared memory.\n");
    }

    _UpdateConfig();

    return S_OK;
}
```

## Testing Strategy
1. Test TSF activation before manager EXE starts
2. Verify graceful fallback to default config
3. Verify handles are acquired when manager starts (via lazy-open in _UpdateConfig)

## Risk Assessment
- **Very Low Risk**: Only adds debug logging
- **Backward Compatible**: No API changes
- **Performance Impact**: None
