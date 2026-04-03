# Fix Proposal: Shared Config Version Check Race Window

## Issue ID: BUG-002

## Severity: Medium

## Location
- `src/tsf/text_service.cpp` (lines 742-768)

## Problem Description
In `CUniKeyTextService::_UpdateConfig()`, the version check happens after mapping the view but before copying the config. If the manager process updates the version during this window, the TSF service could read a partially updated config, leading to:
- Corrupted config values being applied to the engine
- Undefined behavior from partial struct reads
- Potential crashes from invalid enum values

### Current Code Flow
1. Map view of shared memory (line 742)
2. Check version (line 745) - **RACE WINDOW STARTS**
3. Copy config with memcpy (line 747) - **RACE WINDOW ENDS**
4. Apply config to engine

## Proposed Fix
Read the entire struct atomically into a local buffer first, validate it, then apply it.

### Changes Required

#### src/tsf/text_service.cpp - _UpdateConfig() method
```cpp
void CUniKeyTextService::_UpdateConfig()
{
    // Lazy-open: retry opening handles if they're NULL
    if (!_hMapFile) {
        _hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    }
    if (!_hMutex) {
        _hMutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, UNIKEY_SHARED_MUTEX_NAME);
    }
    if (!_hMapFile || !_hMutex) return;

    bool locked = false;
    DWORD waitResult = WaitForSingleObject(_hMutex, 0);
    locked = (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_ABANDONED);
    if (!locked) {
        return;
    }

    void* pBuf = MapViewOfFile(_hMapFile, FILE_MAP_READ, 0, 0, sizeof(UniKeyConfig));
    if (pBuf != NULL) {
        // ATOMIC READ: Copy entire struct first, then validate
        UniKeyConfig latestConfig = {};
        memcpy(&latestConfig, pBuf, sizeof(UniKeyConfig));
        
        // Validate version AFTER atomic copy
        if (latestConfig.version == UNIKEY_CONFIG_VERSION) {
            // Validate field ranges to catch corruption
            if (latestConfig.inputMethod <= IM_VIQR &&
                latestConfig.charset <= CS_VNI_WIN &&
                latestConfig.toneType <= TONE_CLASSIC &&
                latestConfig.toggleKey <= TK_ALT_Z) {
                
                ApplyTypingSettingsToEngine(latestConfig, _engine);

                const MacroConfigDelta macroDelta = EvaluateMacroConfigDelta(
                    _hasLastAppliedConfig ? &_lastAppliedConfig : nullptr,
                    latestConfig);

                if (macroDelta.shouldClear) {
                    _macroEngine.Clear();
                    _currentMacroFile.clear();
                }
                if (macroDelta.shouldReload) {
                    _currentMacroFile = macroDelta.macroFilePath;
                    _macroEngine.Load(_currentMacroFile);
                }

                _config = latestConfig;
                _lastAppliedConfig = latestConfig;
                _hasLastAppliedConfig = true;
            }
            // Invalid ranges - silently ignore this update
        }
        UnmapViewOfFile(pBuf);
    }

    if (locked) {
        ReleaseMutex(_hMutex);
    }
}
```

## Testing Strategy
1. Add unit test that rapidly updates config from manager while TSF reads
2. Verify config values are never corrupted
3. Test with intentionally malformed config data

## Risk Assessment
- **Low Risk**: Defensive coding, only adds validation
- **Backward Compatible**: No API changes
- **Performance Impact**: Negligible (additional memcpy is tiny)
