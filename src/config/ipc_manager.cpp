#include "ipc_manager.h"
#include <shlobj.h>
#include <sddl.h>
#include <fstream>

// --- Globals (defined in ipc_manager.h) ---
UniKeyConfig* g_pConfig      = nullptr;
HANDLE        g_hSharedMutex = nullptr;

// --- Internal Globals ---
static HANDLE g_hMapFile = nullptr;

// =============================================================================
// Config Persistence — %APPDATA%\UniKeyTSF\config.dat
// =============================================================================

std::wstring GetConfigFilePath()
{
    wchar_t appData[MAX_PATH];
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_APPDATA, nullptr, 0, appData))) {
        std::wstring dir = std::wstring(appData) + L"\\UniKeyTSF";
        CreateDirectoryW(dir.c_str(), nullptr);
        return dir + L"\\config.dat";
    }
    return L"";
}

bool LoadConfigFromFile(UniKeyConfig* pConfig)
{
    if (!pConfig) return false;
    std::wstring path = GetConfigFilePath();
    if (path.empty()) return false;

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return false;

    UniKeyConfig loaded = {};
    file.read(reinterpret_cast<char*>(&loaded), sizeof(UniKeyConfig));
    if (!file || loaded.version != UNIKEY_CONFIG_VERSION) return false;

    memcpy(pConfig, &loaded, sizeof(UniKeyConfig));
    return true;
}

bool SaveConfigToFile(const UniKeyConfig* pConfig)
{
    if (!pConfig) return false;
    std::wstring path = GetConfigFilePath();
    if (path.empty()) return false;

    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) return false;

    file.write(reinterpret_cast<const char*>(pConfig), sizeof(UniKeyConfig));
    return file.good();
}

// =============================================================================
// Shared Memory with Security Descriptor
// =============================================================================

bool InitSharedMemory()
{
    // Create security descriptor restricted to current user
    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;
    sa.lpSecurityDescriptor = nullptr;

    PSECURITY_DESCRIPTOR pSD = nullptr;
    // SDDL: Owner=current user, DACL grants full access to current user only
    // D:(A;;GA;;;CU) = Allow, Generic All, Current User
    if (ConvertStringSecurityDescriptorToSecurityDescriptorW(
            L"D:(A;;GA;;;CU)", SDDL_REVISION_1, &pSD, nullptr)) {
        sa.lpSecurityDescriptor = pSD;
    }

    g_hMapFile = CreateFileMappingW(
        INVALID_HANDLE_VALUE,   // Use paging file
        &sa,                    // ACL: current user only
        PAGE_READWRITE,
        0,
        sizeof(UniKeyConfig),
        UNIKEY_SHARED_MEMORY_NAME
    );

    if (pSD) LocalFree(pSD);

    if (!g_hMapFile) return false;

    g_pConfig = static_cast<UniKeyConfig*>(
        MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(UniKeyConfig))
    );

    if (!g_pConfig) {
        CloseHandle(g_hMapFile);
        g_hMapFile = nullptr;
        return false;
    }

    g_hSharedMutex = CreateMutexW(nullptr, FALSE, UNIKEY_SHARED_MUTEX_NAME);

    // Initial load (shared mutex is already created)
    LockConfig();
    if (!LoadConfigFromFile(g_pConfig)) {
        g_pConfig->version         = UNIKEY_CONFIG_VERSION;
        g_pConfig->inputEnabled    = 1;   // Vietnamese ON by default
        g_pConfig->inputMethod     = IM_VNI;   // VNI by user preference
        g_pConfig->charset         = CS_UNICODE;
        g_pConfig->toneType        = TONE_MODERN;
        g_pConfig->spellCheck      = 1;
        g_pConfig->macroEnabled    = 0;
        g_pConfig->freeToneMarking = 1;
        g_pConfig->toggleKey       = TK_CTRL_SHIFT;
        g_pConfig->macroFilePath[0] = L'\0';
    }
    UnlockConfig();

    return true;
}

void CleanupSharedMemory()
{
    if (g_pConfig) {
        SaveConfigToFile(g_pConfig);
        UnmapViewOfFile(g_pConfig);
        g_pConfig = nullptr;
    }
    if (g_hMapFile) {
        CloseHandle(g_hMapFile);
        g_hMapFile = nullptr;
    }
    if (g_hSharedMutex) {
        CloseHandle(g_hSharedMutex);
        g_hSharedMutex = nullptr;
    }
}

// =============================================================================
// Access Synchronization
// =============================================================================

void LockConfig()
{
    if (g_hSharedMutex) {
        WaitForSingleObject(g_hSharedMutex, INFINITE);
    }
}

void UnlockConfig()
{
    if (g_hSharedMutex) {
        ReleaseMutex(g_hSharedMutex);
    }
}
