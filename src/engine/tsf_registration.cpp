#include "tsf_registration.h"
#include <strsafe.h>
#include <ole2.h>
#include <initguid.h>
#include <string>
#include "../tsf/guids.h"

typedef HRESULT (STDAPICALLTYPE *PFNDLLREGISTERSERVER)();

namespace {

bool GetExecutableDirectory(wchar_t* buffer, size_t bufferLen)
{
    if (!buffer || bufferLen == 0) return false;

    wchar_t exePath[MAX_PATH] = {};
    DWORD length = GetModuleFileNameW(nullptr, exePath, ARRAYSIZE(exePath));
    if (length == 0 || length >= ARRAYSIZE(exePath)) return false;

    wchar_t* lastSlash = wcsrchr(exePath, L'\\');
    if (!lastSlash) return false;
    *(lastSlash + 1) = L'\0';

    return SUCCEEDED(StringCchCopyW(buffer, bufferLen, exePath));
}

bool GetExpectedDllPath(wchar_t* buffer, size_t bufferLen)
{
    if (!GetExecutableDirectory(buffer, bufferLen)) return false;
    return SUCCEEDED(StringCchCatW(buffer, bufferLen, L"uktsf_core64.dll"));
}

bool IsCurrentDllRegistered(HRESULT* hr)
{
    if (hr) {
        *hr = E_FAIL;
    }

    wchar_t expectedDllPath[MAX_PATH] = {};
    if (!GetExpectedDllPath(expectedDllPath, ARRAYSIZE(expectedDllPath))) {
        if (hr) {
            *hr = E_UNEXPECTED;
        }
        return false;
    }

    wchar_t clsid[64] = {};
    if (StringFromGUID2(CLSID_UniKeyTSF, clsid, ARRAYSIZE(clsid)) == 0) {
        if (hr) {
            *hr = E_UNEXPECTED;
        }
        return false;
    }

    wchar_t regKeyPath[256] = {};
    if (FAILED(StringCchPrintfW(
            regKeyPath,
            ARRAYSIZE(regKeyPath),
            L"Software\\Classes\\CLSID\\%s\\InprocServer32",
            clsid))) {
        if (hr) {
            *hr = E_UNEXPECTED;
        }
        return false;
    }

    HKEY hKey = nullptr;
    LONG openStatus = RegOpenKeyExW(HKEY_CURRENT_USER, regKeyPath, 0, KEY_READ, &hKey);
    if (openStatus == ERROR_FILE_NOT_FOUND || openStatus == ERROR_PATH_NOT_FOUND) {
        if (hr) {
            *hr = S_OK;
        }
        return false;
    }
    if (openStatus != ERROR_SUCCESS) {
        if (hr) {
            *hr = HRESULT_FROM_WIN32(openStatus);
        }
        return false;
    }

    wchar_t registeredDllPath[MAX_PATH] = {};
    DWORD type = 0;
    DWORD size = sizeof(registeredDllPath);
    LONG status = RegQueryValueExW(
        hKey,
        nullptr,
        nullptr,
        &type,
        reinterpret_cast<LPBYTE>(registeredDllPath),
        &size);
    RegCloseKey(hKey);

    if (status == ERROR_FILE_NOT_FOUND) {
        if (hr) {
            *hr = S_OK;
        }
        return false;
    }
    if (status != ERROR_SUCCESS) {
        if (hr) {
            *hr = HRESULT_FROM_WIN32(status);
        }
        return false;
    }
    if (type != REG_SZ) {
        if (hr) {
            *hr = HRESULT_FROM_WIN32(ERROR_DATATYPE_MISMATCH);
        }
        return false;
    }

    if (hr) {
        *hr = S_OK;
    }
    return _wcsicmp(registeredDllPath, expectedDllPath) == 0;
}

bool IsLanguageProfilePresentAndActive(bool* profilePresent, bool* profileActive, HRESULT* hr)
{
    if (profilePresent) {
        *profilePresent = false;
    }
    if (profileActive) {
        *profileActive = false;
    }
    if (hr) {
        *hr = E_FAIL;
    }

    wchar_t clsid[64] = {};
    wchar_t profileGuid[64] = {};
    if (StringFromGUID2(CLSID_UniKeyTSF, clsid, ARRAYSIZE(clsid)) == 0
        || StringFromGUID2(GUID_UniKeyTSF_Profile, profileGuid, ARRAYSIZE(profileGuid)) == 0) {
        if (hr) {
            *hr = E_UNEXPECTED;
        }
        return false;
    }

    wchar_t profileKeyPath[512] = {};
    if (FAILED(StringCchPrintfW(
            profileKeyPath,
            ARRAYSIZE(profileKeyPath),
            L"Software\\Microsoft\\CTF\\TIP\\%s\\LanguageProfile\\0x%08X\\%s",
            clsid,
            static_cast<unsigned int>(UNIKEY_LANGID),
            profileGuid))) {
        if (hr) {
            *hr = E_UNEXPECTED;
        }
        return false;
    }

    HKEY profileKey = nullptr;
    LONG openStatus = RegOpenKeyExW(HKEY_CURRENT_USER, profileKeyPath, 0, KEY_READ, &profileKey);

    if (openStatus == ERROR_FILE_NOT_FOUND || openStatus == ERROR_PATH_NOT_FOUND) {
        if (hr) {
            *hr = S_OK;
        }
        return false;
    }
    if (openStatus != ERROR_SUCCESS) {
        if (hr) {
            *hr = HRESULT_FROM_WIN32(openStatus);
        }
        return false;
    }

    const bool present = true;

    bool active = false;
    if (present) {
        DWORD valueType = 0;
        DWORD enabledValue = 0;
        DWORD valueSize = sizeof(enabledValue);
        const LONG queryStatus = RegQueryValueExW(
            profileKey,
            L"Enable",
            nullptr,
            &valueType,
            reinterpret_cast<LPBYTE>(&enabledValue),
            &valueSize);
        if (queryStatus == ERROR_FILE_NOT_FOUND) {
            active = false;
        } else if (queryStatus != ERROR_SUCCESS) {
            RegCloseKey(profileKey);
            if (hr) {
                *hr = HRESULT_FROM_WIN32(queryStatus);
            }
            return false;
        } else if (valueType != REG_DWORD) {
            RegCloseKey(profileKey);
            if (hr) {
                *hr = HRESULT_FROM_WIN32(ERROR_DATATYPE_MISMATCH);
            }
            return false;
        } else {
            active = enabledValue != 0;
        }
        RegCloseKey(profileKey);
    }

    if (profilePresent) {
        *profilePresent = present;
    }
    if (profileActive) {
        *profileActive = active;
    }
    if (hr) {
        *hr = S_OK;
    }
    return true;
}

const wchar_t* StatusToNextStep(TsfDiagnosticsStatus status)
{
    switch (status) {
    case TsfDiagnosticsStatus::kReady:
        return L"none";
    case TsfDiagnosticsStatus::kDllNotRegistered:
        return L"run_unikeytsf_/register";
    case TsfDiagnosticsStatus::kProfileMissing:
        return L"run_unikeytsf_/register_then_reopen_windows_input_settings";
    case TsfDiagnosticsStatus::kProfileInactive:
        return L"activate_unikey_tsf_profile_in_windows_input_settings";
    case TsfDiagnosticsStatus::kDllCheckFailed:
        return L"retry_tsf_diagnostics_or_check_registry_access";
    case TsfDiagnosticsStatus::kProfileCheckFailed:
        return L"retry_tsf_diagnostics_or_check_text_services_framework";
    default:
        return L"retry_tsf_diagnostics";
    }
}

std::wstring HResultToHex(HRESULT hr)
{
    wchar_t buffer[32] = {};
    if (FAILED(StringCchPrintfW(buffer, ARRAYSIZE(buffer), L"0x%08X", static_cast<unsigned int>(hr)))) {
        return L"0x00000000";
    }
    return std::wstring(buffer);
}

} // namespace

const wchar_t* TsfDiagnosticsStatusToString(TsfDiagnosticsStatus status)
{
    switch (status) {
    case TsfDiagnosticsStatus::kReady:
        return L"ready";
    case TsfDiagnosticsStatus::kDllNotRegistered:
        return L"dll_not_registered";
    case TsfDiagnosticsStatus::kProfileMissing:
        return L"profile_missing";
    case TsfDiagnosticsStatus::kProfileInactive:
        return L"profile_inactive";
    case TsfDiagnosticsStatus::kDllCheckFailed:
        return L"dll_check_failed";
    case TsfDiagnosticsStatus::kProfileCheckFailed:
        return L"profile_check_failed";
    default:
        return L"unknown";
    }
}

TsfDiagnosticsReport BuildTsfDiagnosticsReport(const TsfDiagnosticsObservation& observation)
{
    TsfDiagnosticsReport report = {};
    report.dllRegistered = observation.dllRegistered;
    report.profilePresent = observation.profilePresent;
    report.profileActive = observation.profileActive;
    report.dllHr = observation.dllHr;
    report.profileHr = observation.profileHr;

    if (FAILED(observation.dllHr)) {
        report.status = TsfDiagnosticsStatus::kDllCheckFailed;
    } else if (!observation.dllRegistered) {
        report.status = TsfDiagnosticsStatus::kDllNotRegistered;
    } else if (FAILED(observation.profileHr)) {
        report.status = TsfDiagnosticsStatus::kProfileCheckFailed;
    } else if (!observation.profilePresent) {
        report.status = TsfDiagnosticsStatus::kProfileMissing;
    } else if (!observation.profileActive) {
        report.status = TsfDiagnosticsStatus::kProfileInactive;
    } else {
        report.status = TsfDiagnosticsStatus::kReady;
    }

    report.nextStep = StatusToNextStep(report.status);
    return report;
}

TsfDiagnosticsReport GetTsfDiagnosticsReport()
{
    TsfDiagnosticsObservation observation = {};

    HRESULT dllHr = E_FAIL;
    observation.dllRegistered = IsCurrentDllRegistered(&dllHr);
    observation.dllHr = dllHr;

    bool profilePresent = false;
    bool profileActive = false;
    HRESULT profileHr = E_FAIL;
    if (IsLanguageProfilePresentAndActive(&profilePresent, &profileActive, &profileHr)) {
        observation.profilePresent = profilePresent;
        observation.profileActive = profileActive;
        observation.profileHr = profileHr;
    } else {
        observation.profilePresent = false;
        observation.profileActive = false;
        observation.profileHr = profileHr;
    }

    return BuildTsfDiagnosticsReport(observation);
}

std::wstring FormatTsfDiagnosticsKeyValue(const TsfDiagnosticsReport& report)
{
    std::wstring output;
    output.reserve(256);

    output += L"status=";
    output += TsfDiagnosticsStatusToString(report.status);
    output += L"\n";

    output += L"dll_registered=";
    output += report.dllRegistered ? L"1" : L"0";
    output += L"\n";

    output += L"profile_present=";
    output += report.profilePresent ? L"1" : L"0";
    output += L"\n";

    output += L"profile_active=";
    output += report.profileActive ? L"1" : L"0";
    output += L"\n";

    output += L"dll_hr=";
    output += HResultToHex(report.dllHr);
    output += L"\n";

    output += L"profile_hr=";
    output += HResultToHex(report.profileHr);
    output += L"\n";

    output += L"next_step=";
    output += report.nextStep ? report.nextStep : L"none";
    output += L"\n";

    return output;
}

bool RegisterTsfDll()
{
    wchar_t exePath[MAX_PATH] = {};
    if (!GetExecutableDirectory(exePath, ARRAYSIZE(exePath))) return false;

    HRESULT registerHr64 = S_OK;
    bool attempted64 = false;

    wchar_t dllPath[MAX_PATH];
    StringCchCopyW(dllPath, MAX_PATH, exePath);
    StringCchCatW(dllPath, MAX_PATH, L"uktsf_core64.dll");

    if (GetFileAttributesW(dllPath) != INVALID_FILE_ATTRIBUTES) {
        attempted64 = true;
        HMODULE hDll = LoadLibraryW(dllPath);
        if (hDll) {
            auto pfnRegister = reinterpret_cast<PFNDLLREGISTERSERVER>(
                GetProcAddress(hDll, "DllRegisterServer"));
            if (pfnRegister) {
                const HRESULT initHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
                const bool mustUninitialize = SUCCEEDED(initHr);
                if (FAILED(initHr) && initHr != RPC_E_CHANGED_MODE) {
                    registerHr64 = initHr;
                } else {
                    registerHr64 = pfnRegister();
                }
                if (mustUninitialize) {
                    CoUninitialize();
                }
            } else {
                registerHr64 = HRESULT_FROM_WIN32(ERROR_PROC_NOT_FOUND);
            }
            FreeLibrary(hDll);
        } else {
            registerHr64 = HRESULT_FROM_WIN32(GetLastError());
        }
    }

    wchar_t dllPath32[MAX_PATH];
    StringCchCopyW(dllPath32, MAX_PATH, exePath);
    StringCchCatW(dllPath32, MAX_PATH, L"uktsf_core32.dll");

    if (GetFileAttributesW(dllPath32) != INVALID_FILE_ATTRIBUTES) {
        wchar_t cmd[MAX_PATH * 2];
        StringCchPrintfW(cmd, ARRAYSIZE(cmd), L"regsvr32.exe /s \"%s\"", dllPath32);
        
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        if (CreateProcessW(nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            DWORD exitCode = 0;
            if (GetExitCodeProcess(pi.hProcess, &exitCode) && exitCode != 0 && SUCCEEDED(registerHr64)) {
                registerHr64 = HRESULT_FROM_WIN32(exitCode);
            }
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    TsfDiagnosticsReport report = GetTsfDiagnosticsReport();
    if (attempted64 && FAILED(registerHr64) && SUCCEEDED(report.dllHr)) {
        report.dllHr = registerHr64;
    }
    return report.dllRegistered && report.profilePresent;
}

bool EnsureTsfDllRegistered()
{
    HRESULT hr = E_FAIL;
    if (IsCurrentDllRegistered(&hr)) {
        return true;
    }

    if (!RegisterTsfDll()) {
        return false;
    }

    hr = E_FAIL;
    return IsCurrentDllRegistered(&hr);
}

void UnregisterTsfDll()
{
    wchar_t exePath[MAX_PATH] = {};
    if (!GetExecutableDirectory(exePath, ARRAYSIZE(exePath))) return;

    wchar_t dllPath[MAX_PATH];
    StringCchCopyW(dllPath, MAX_PATH, exePath);
    StringCchCatW(dllPath, MAX_PATH, L"uktsf_core64.dll");

    if (GetFileAttributesW(dllPath) != INVALID_FILE_ATTRIBUTES) {
        HMODULE hDll = LoadLibraryW(dllPath);
        if (hDll) {
            auto pfnUnregister = reinterpret_cast<PFNDLLREGISTERSERVER>(
                GetProcAddress(hDll, "DllUnregisterServer"));
            if (pfnUnregister) {
                CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
                pfnUnregister();
                CoUninitialize();
            }
            FreeLibrary(hDll);
        }
    }

    wchar_t dllPath32[MAX_PATH];
    StringCchCopyW(dllPath32, MAX_PATH, exePath);
    StringCchCatW(dllPath32, MAX_PATH, L"uktsf_core32.dll");

    if (GetFileAttributesW(dllPath32) != INVALID_FILE_ATTRIBUTES) {
        wchar_t cmd[MAX_PATH * 2];
        StringCchPrintfW(cmd, ARRAYSIZE(cmd), L"regsvr32.exe /u /s \"%s\"", dllPath32);
        
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        if (CreateProcessW(nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }
}
