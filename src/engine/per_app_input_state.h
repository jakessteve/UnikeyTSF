#pragma once

#include <algorithm>
#include <cstdint>
#include <string>
#include <cwctype>
#include <windows.h>

#include "../shared_config.h"

constexpr const wchar_t* UNIKEY_PER_APP_STATE_MEMORY_NAME = L"Local\\UniKeyTSF_PerAppInputState";
constexpr const wchar_t* UNIKEY_PER_APP_STATE_MUTEX_NAME = L"Local\\UniKeyTSF_PerAppInputStateMutex";
constexpr uint32_t UNIKEY_PER_APP_STATE_VERSION = 1;

struct PerAppInputStateEntry {
    uint8_t used;
    uint8_t inputEnabled;
    wchar_t appId[64];
};

struct PerAppInputStateShared {
    uint32_t version;
    uint8_t persistedSeedInputEnabled;
    uint8_t seedInitialized;
    uint8_t reserved[2];
    PerAppInputStateEntry entries[128];
};

inline std::wstring NormalizeAppId(const std::wstring& appIdOrPath) {
    std::wstring normalized = appIdOrPath;
    const size_t slash = normalized.find_last_of(L"\\/");
    if (slash != std::wstring::npos) {
        normalized = normalized.substr(slash + 1);
    }

    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(std::towlower(ch));
    });

    return normalized;
}

inline std::wstring ResolveForegroundAppId() {
    const HWND hWnd = GetForegroundWindow();
    if (!hWnd) {
        return L"";
    }

    DWORD pid = 0;
    GetWindowThreadProcessId(hWnd, &pid);
    if (!pid) {
        return L"";
    }

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!hProcess) {
        return L"";
    }

    wchar_t imagePath[MAX_PATH] = {};
    DWORD pathLen = MAX_PATH;
    if (!QueryFullProcessImageNameW(hProcess, 0, imagePath, &pathLen)) {
        CloseHandle(hProcess);
        return L"";
    }

    CloseHandle(hProcess);
    return NormalizeAppId(imagePath);
}

inline std::wstring ResolveCurrentProcessAppId() {
    wchar_t path[MAX_PATH] = {};
    if (!GetModuleFileNameW(nullptr, path, MAX_PATH)) {
        return L"";
    }

    return NormalizeAppId(path);
}

inline HANDLE& PerAppStoreMapHandle() {
    static HANDLE handle = nullptr;
    return handle;
}

inline HANDLE& PerAppStoreMutexHandle() {
    static HANDLE handle = nullptr;
    return handle;
}

inline bool EnsurePerAppInputStateStore() {
    HANDLE& hMap = PerAppStoreMapHandle();
    HANDLE& hMutex = PerAppStoreMutexHandle();

    if (!hMap) {
        hMap = CreateFileMappingW(
            INVALID_HANDLE_VALUE,
            nullptr,
            PAGE_READWRITE,
            0,
            sizeof(PerAppInputStateShared),
            UNIKEY_PER_APP_STATE_MEMORY_NAME);
    }

    if (!hMutex) {
        hMutex = CreateMutexW(nullptr, FALSE, UNIKEY_PER_APP_STATE_MUTEX_NAME);
    }

    if (!hMap || !hMutex) {
        return false;
    }

    const DWORD waitResult = WaitForSingleObject(hMutex, 50);
    if (waitResult != WAIT_OBJECT_0 && waitResult != WAIT_ABANDONED) {
        return false;
    }

    auto* shared = static_cast<PerAppInputStateShared*>(
        MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, sizeof(PerAppInputStateShared)));
    if (!shared) {
        ReleaseMutex(hMutex);
        return false;
    }

    if (shared->version != UNIKEY_PER_APP_STATE_VERSION) {
        ZeroMemory(shared, sizeof(PerAppInputStateShared));
        shared->version = UNIKEY_PER_APP_STATE_VERSION;
    }

    UnmapViewOfFile(shared);
    ReleaseMutex(hMutex);
    return true;
}

inline void ShutdownPerAppInputStateStore() {
    HANDLE& hMap = PerAppStoreMapHandle();
    HANDLE& hMutex = PerAppStoreMutexHandle();

    if (hMap) {
        CloseHandle(hMap);
        hMap = nullptr;
    }

    if (hMutex) {
        CloseHandle(hMutex);
        hMutex = nullptr;
    }
}

inline bool LockPerAppInputStateStore() {
    HANDLE& hMutex = PerAppStoreMutexHandle();
    if (!hMutex) {
        return false;
    }

    const DWORD waitResult = WaitForSingleObject(hMutex, 50);
    return waitResult == WAIT_OBJECT_0 || waitResult == WAIT_ABANDONED;
}

inline void UnlockPerAppInputStateStore() {
    HANDLE& hMutex = PerAppStoreMutexHandle();
    if (hMutex) {
        ReleaseMutex(hMutex);
    }
}

inline uint8_t ResolveEffectiveInputEnabled(const UniKeyConfig& config, const std::wstring& appId) {
    if (!config.perAppInputState) {
        return config.inputEnabled ? 1 : 0;
    }

    const std::wstring normalizedAppId = NormalizeAppId(appId);
    if (normalizedAppId.empty()) {
        return config.inputEnabled ? 1 : 0;
    }

    if (!EnsurePerAppInputStateStore() || !LockPerAppInputStateStore()) {
        return config.inputEnabled ? 1 : 0;
    }

    uint8_t resolved = config.inputEnabled ? 1 : 0;

    auto* shared = static_cast<const PerAppInputStateShared*>(
        MapViewOfFile(PerAppStoreMapHandle(), FILE_MAP_READ, 0, 0, sizeof(PerAppInputStateShared)));
    if (shared) {
        for (const PerAppInputStateEntry& entry : shared->entries) {
            if (!entry.used) {
                continue;
            }
            if (_wcsicmp(entry.appId, normalizedAppId.c_str()) == 0) {
                resolved = entry.inputEnabled ? 1 : 0;
                break;
            }
        }

        UnmapViewOfFile(shared);
    }

    UnlockPerAppInputStateStore();
    return resolved;
}

inline void SetPerAppInputStateSeed(uint8_t seed) {
    if (!EnsurePerAppInputStateStore() || !LockPerAppInputStateStore()) {
        return;
    }

    auto* shared = static_cast<PerAppInputStateShared*>(
        MapViewOfFile(PerAppStoreMapHandle(), FILE_MAP_WRITE, 0, 0, sizeof(PerAppInputStateShared)));
    if (shared) {
        shared->persistedSeedInputEnabled = seed ? 1 : 0;
        shared->seedInitialized = 1;
        UnmapViewOfFile(shared);
    }

    UnlockPerAppInputStateStore();
}

inline uint8_t GetPerAppInputStateSeed(uint8_t fallback) {
    if (!EnsurePerAppInputStateStore() || !LockPerAppInputStateStore()) {
        return fallback ? 1 : 0;
    }

    uint8_t seed = fallback ? 1 : 0;
    auto* shared = static_cast<const PerAppInputStateShared*>(
        MapViewOfFile(PerAppStoreMapHandle(), FILE_MAP_READ, 0, 0, sizeof(PerAppInputStateShared)));
    if (shared) {
        if (shared->seedInitialized) {
            seed = shared->persistedSeedInputEnabled ? 1 : 0;
        }
        UnmapViewOfFile(shared);
    }

    UnlockPerAppInputStateStore();
    return seed;
}

inline void SetInputEnabledForApp(UniKeyConfig* config, const std::wstring& appId, uint8_t enabled) {
    if (!config) {
        return;
    }

    const uint8_t normalizedEnabled = enabled ? 1 : 0;
    if (!config->perAppInputState) {
        config->inputEnabled = normalizedEnabled;
        SetPerAppInputStateSeed(normalizedEnabled);
        return;
    }

    const std::wstring normalizedAppId = NormalizeAppId(appId);
    if (normalizedAppId.empty()) {
        config->inputEnabled = normalizedEnabled;
        return;
    }

    config->inputEnabled = normalizedEnabled;

    if (!EnsurePerAppInputStateStore() || !LockPerAppInputStateStore()) {
        return;
    }

    auto* shared = static_cast<PerAppInputStateShared*>(
        MapViewOfFile(PerAppStoreMapHandle(), FILE_MAP_WRITE, 0, 0, sizeof(PerAppInputStateShared)));
    if (shared) {
        PerAppInputStateEntry* target = nullptr;
        for (PerAppInputStateEntry& entry : shared->entries) {
            if (!entry.used) {
                continue;
            }
            if (_wcsicmp(entry.appId, normalizedAppId.c_str()) == 0) {
                target = &entry;
                break;
            }
        }

        if (!target) {
            for (PerAppInputStateEntry& entry : shared->entries) {
                if (entry.used) {
                    continue;
                }
                entry.used = 1;
                wcsncpy_s(entry.appId, normalizedAppId.c_str(), _TRUNCATE);
                target = &entry;
                break;
            }
        }

        if (target) {
            target->inputEnabled = normalizedEnabled;
        }

        UnmapViewOfFile(shared);
    }

    UnlockPerAppInputStateStore();
}

inline uint8_t ToggleInputEnabledForApp(UniKeyConfig* config, const std::wstring& appId) {
    if (!config) {
        return 0;
    }

    if (!config->perAppInputState) {
        config->inputEnabled = config->inputEnabled ? 0 : 1;
        SetPerAppInputStateSeed(config->inputEnabled);
        return config->inputEnabled;
    }

    const std::wstring normalizedAppId = NormalizeAppId(appId);
    if (normalizedAppId.empty()) {
        config->inputEnabled = config->inputEnabled ? 0 : 1;
        return config->inputEnabled;
    }

    const uint8_t resolvedCurrent = ResolveEffectiveInputEnabled(*config, normalizedAppId);
    const uint8_t toggled = resolvedCurrent ? 0 : 1;
    SetInputEnabledForApp(config, normalizedAppId, toggled);
    return toggled;
}

inline UniKeyConfig BuildPersistedConfigSnapshot(const UniKeyConfig& config) {
    UniKeyConfig snapshot = config;
    if (config.perAppInputState) {
        snapshot.inputEnabled = GetPerAppInputStateSeed(config.inputEnabled);
    }
    return snapshot;
}

inline void ResetPerAppInputStateStoreForTests() {
    if (!EnsurePerAppInputStateStore() || !LockPerAppInputStateStore()) {
        return;
    }

    auto* shared = static_cast<PerAppInputStateShared*>(
        MapViewOfFile(PerAppStoreMapHandle(), FILE_MAP_WRITE, 0, 0, sizeof(PerAppInputStateShared)));
    if (shared) {
        ZeroMemory(shared, sizeof(PerAppInputStateShared));
        shared->version = UNIKEY_PER_APP_STATE_VERSION;
        UnmapViewOfFile(shared);
    }

    UnlockPerAppInputStateStore();
}
