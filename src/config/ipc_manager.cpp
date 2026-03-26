#include "ipc_manager.h"
#include <shlobj.h>
#include <sddl.h>
#include <fstream>
#include <memory>

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
    // Create security descriptor restricted to current user and AppContainer
    SECURITY_ATTRIBUTES sa = {};
    sa.nLength = sizeof(SECURITY_ATTRIBUTES);
    sa.bInheritHandle = FALSE;

    PSECURITY_DESCRIPTOR rawSD = nullptr;
    // SDDL: Owner=current user. DACL: Full access to Current User (CU) and All Application Packages (AC)
    // D:(A;;GA;;;CU)(A;;GA;;;AC) = Allow, Generic All, Current User + Allow, Generic All, All App Packages
    // AC needs GA (or at least GRGW) to acquire the Mutex (MUTEX_MODIFY_STATE)
    if (ConvertStringSecurityDescriptorToSecurityDescriptorW(
            L"D:(A;;GA;;;CU)(A;;GRGW;;;AC)", SDDL_REVISION_1, &rawSD, nullptr)) {
        sa.lpSecurityDescriptor = rawSD;
    }

    // RAII for SD
    struct ScopedLocalFree {
        HLOCAL ptr;
        ScopedLocalFree(HLOCAL p) : ptr(p) {}
        ~ScopedLocalFree() { if (ptr) LocalFree(ptr); }
    };
    ScopedLocalFree autoFreeSD(rawSD);

    g_hMapFile = CreateFileMappingW(
        INVALID_HANDLE_VALUE,   // Use paging file
        &sa,                    // ACL: current user + AppContainer read
        PAGE_READWRITE,
        0,
        sizeof(UniKeyConfig),
        UNIKEY_SHARED_MEMORY_NAME
    );

    bool bAlreadyExists = (GetLastError() == ERROR_ALREADY_EXISTS);

    if (!g_hMapFile) return false;

    g_pConfig = static_cast<UniKeyConfig*>(
        MapViewOfFile(g_hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(UniKeyConfig))
    );

    if (!g_pConfig) {
        CloseHandle(g_hMapFile);
        g_hMapFile = nullptr;
        return false;
    }

    // Mutex must also have the Security Descriptor so UWP can open/wait it
    g_hSharedMutex = CreateMutexW(&sa, FALSE, UNIKEY_SHARED_MUTEX_NAME);

    // Initial load (shared mutex is already created)
    if (LockConfig()) {
        if (!bAlreadyExists) {
            // Only load from file if we are the first process creating the shared memory!
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
                g_pConfig->restoreKeyEnabled = 1;
                g_pConfig->useClipboardForUnicode = 0;
                g_pConfig->showDialogOnStartup = 1;
                g_pConfig->perAppInputState = 0;
                g_pConfig->macroFilePath[0] = L'\0';
            }
        }
        UnlockConfig();
    }

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

bool LockConfig()
{
    if (g_hSharedMutex) {
        // Use timeout to prevent deadlock if another process crashes while holding lock
        DWORD waitResult = WaitForSingleObject(g_hSharedMutex, 500);
        if (waitResult == WAIT_TIMEOUT || waitResult == WAIT_FAILED) {
            // Lock acquisition failed, do not barge into critical section
            return false;
        }
        return true;
    }
    return false;
}

void UnlockConfig()
{
    if (g_hSharedMutex) {
        ReleaseMutex(g_hSharedMutex);
    }
}
