#pragma once
// =============================================================================
// UniKey TSF Reborn — TSF DLL Registration
// =============================================================================
// Handles (un)registration of the TSF COM server DLLs.
// =============================================================================

#include <windows.h>

#include <string>

enum class TsfDiagnosticsStatus {
    kReady = 0,
    kDllNotRegistered,
    kProfileMissing,
    kProfileInactive,
    kDllCheckFailed,
    kProfileCheckFailed,
};

struct TsfDiagnosticsObservation {
    bool dllRegistered = false;
    bool profilePresent = false;
    bool profileActive = false;
    HRESULT dllHr = S_OK;
    HRESULT profileHr = S_OK;
};

struct TsfDiagnosticsReport {
    TsfDiagnosticsStatus status = TsfDiagnosticsStatus::kDllNotRegistered;
    bool dllRegistered = false;
    bool profilePresent = false;
    bool profileActive = false;
    HRESULT dllHr = S_OK;
    HRESULT profileHr = S_OK;
    const wchar_t* nextStep = L"";
};

const wchar_t* TsfDiagnosticsStatusToString(TsfDiagnosticsStatus status);
TsfDiagnosticsReport BuildTsfDiagnosticsReport(const TsfDiagnosticsObservation& observation);
TsfDiagnosticsReport GetTsfDiagnosticsReport();
std::wstring FormatTsfDiagnosticsKeyValue(const TsfDiagnosticsReport& report);

bool RegisterTsfDll();
bool EnsureTsfDllRegistered();
void UnregisterTsfDll();
