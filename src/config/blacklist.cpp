#include "blacklist.h"
#include <fstream>
#include <algorithm>
#include <windows.h>

// Built-in defaults (used when blacklist.txt doesn't exist yet)
static const std::vector<std::wstring> DEFAULT_BLACKLIST = {
    L"cmd.exe",
    L"conhost.exe",
    L"WindowsTerminal.exe",
    L"mstsc.exe",
    L"ubuntu.exe",
    L"wsl.exe",
};

// Single global runtime blacklist — shared across all TUs
static std::vector<std::wstring> g_processBlacklist;
static bool g_blacklistInitialized = false;
static FILETIME g_lastBlacklistWriteTime = {0, 0};

std::wstring GetBlacklistFilePath() {
    wchar_t exePath[MAX_PATH] = {0};
    HMODULE hModule = NULL;
    GetModuleHandleExW(
        GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
        (LPCWSTR)&GetBlacklistFilePath,
        &hModule);
    GetModuleFileNameW(hModule, exePath, MAX_PATH);
    std::wstring path(exePath);
    auto pos = path.find_last_of(L"\\/");
    if (pos != std::wstring::npos)
        path = path.substr(0, pos + 1);
    path += L"blacklist.txt";
    return path;
}

std::vector<std::wstring> LoadBlacklist() {
    std::wstring filePath = GetBlacklistFilePath();
    std::wifstream file(filePath);
    if (!file.is_open()) {
        return DEFAULT_BLACKLIST;
    }

    std::vector<std::wstring> list;
    std::wstring line;
    while (std::getline(file, line)) {
        // Trim whitespace
        while (!line.empty() && (line.back() == L' ' || line.back() == L'\t' || line.back() == L'\r'))
            line.pop_back();
        while (!line.empty() && (line.front() == L' ' || line.front() == L'\t'))
            line.erase(line.begin());
        // Skip empty lines and comments
        if (!line.empty() && line[0] != L'#') {
            list.push_back(line);
        }
    }
    return list;
}

bool SaveBlacklist(const std::vector<std::wstring>& list) {
    std::wstring filePath = GetBlacklistFilePath();
    std::wofstream file(filePath);
    if (!file.is_open()) return false;

    file << L"# UniKey TSF \u2014 Process Blacklist\n";
    file << L"# One process name per line. Lines starting with # are comments.\n";
    for (const auto& name : list) {
        file << name << L"\n";
    }
    return true;
}

void InitBlacklist() {
    g_processBlacklist = LoadBlacklist();
    g_blacklistInitialized = true;
    
    WIN32_FILE_ATTRIBUTE_DATA attr;
    if (GetFileAttributesExW(GetBlacklistFilePath().c_str(), GetFileExInfoStandard, &attr)) {
        g_lastBlacklistWriteTime = attr.ftLastWriteTime;
    }
}

bool IsProcessBlacklisted(const std::wstring& processName) {
    if (!g_blacklistInitialized) {
        InitBlacklist();
    } else {
        WIN32_FILE_ATTRIBUTE_DATA attr;
        if (GetFileAttributesExW(GetBlacklistFilePath().c_str(), GetFileExInfoStandard, &attr)) {
            if (CompareFileTime(&attr.ftLastWriteTime, &g_lastBlacklistWriteTime) != 0) {
                InitBlacklist();
            }
        }
    }
    for (const auto& name : g_processBlacklist) {
        if (_wcsicmp(processName.c_str(), name.c_str()) == 0) {
            return true;
        }
    }
    return false;
}

void ReloadBlacklist() {
    InitBlacklist();
}
