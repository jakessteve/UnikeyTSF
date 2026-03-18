// =============================================================================
// UniKey TSF Reborn â€” DLL Main & COM Registration
// =============================================================================
// DLL entry points for the TSF COM server:
//   - DllGetClassObject: Creates the class factory
//   - DllCanUnloadNow: COM garbage collection check
//   - DllRegisterServer: Registers CLSID + TIP profile in HKCU with ACLs
//   - DllUnregisterServer: Removes registration
// =============================================================================

#include <windows.h>
#include <msctf.h>
#include <olectl.h>
#include <sddl.h>
#include <aclapi.h>
#include <strsafe.h>
#include <wrl/client.h>
#include <new>

#include <initguid.h>  // MUST come before guids.h to make DEFINE_GUID allocate storage
#include "guids.h"
#include "class_factory.h"

using Microsoft::WRL::ComPtr;

// --- Globals ---
HMODULE g_hModule  = nullptr;
LONG    g_cDllRef  = 0;

// --- Display name ---
static const wchar_t* TSF_DISPLAY_NAME = L"UniKey TSF Reborn";

// --- Helper: Convert GUID to string ---
static bool GuidToString(const GUID& guid, wchar_t* buffer, size_t bufferLen)
{
    return StringCchPrintfW(buffer, bufferLen,
        L"{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}",
        guid.Data1, guid.Data2, guid.Data3,
        guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
        guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]) == S_OK;
}

// =============================================================================
// AppContainer ACL â€” CRITICAL for Windows 11 UWP apps
// =============================================================================
// Sets security descriptor on a registry key to grant read access to
// ALL APPLICATION PACKAGES (SID: S-1-15-2-1). Without this, the IME
// will fail to load in sandboxed UWP apps (Notepad, Calculator, etc.)
// =============================================================================

static BOOL SetAppContainerACL(HKEY hKey)
{
    // SDDL: Grant this key + subkeys read access to ALL APPLICATION PACKAGES
    // D: = DACL
    // (A;OICI;KR;;;S-1-15-2-1) = Allow, Object Inherit + Container Inherit, Key Read, ALL APPLICATION PACKAGES
    const wchar_t* sddl = L"D:(A;OICI;KR;;;S-1-15-2-1)";

    PSECURITY_DESCRIPTOR pSD = nullptr;
    if (!ConvertStringSecurityDescriptorToSecurityDescriptorW(
        sddl, SDDL_REVISION_1, &pSD, nullptr))
    {
        return FALSE;
    }

    BOOL bResult = TRUE;
    PACL pDacl = nullptr;
    BOOL bDaclPresent = FALSE;
    BOOL bDaclDefaulted = FALSE;

    if (!GetSecurityDescriptorDacl(pSD, &bDaclPresent, &pDacl, &bDaclDefaulted) || !bDaclPresent)
    {
        LocalFree(pSD);
        return FALSE;
    }

    DWORD err = SetSecurityInfo(
        hKey, SE_REGISTRY_KEY,
        DACL_SECURITY_INFORMATION | UNPROTECTED_DACL_SECURITY_INFORMATION,
        nullptr, nullptr, pDacl, nullptr);

    if (err != ERROR_SUCCESS) {
        bResult = FALSE;
    }

    LocalFree(pSD);
    return bResult;
}

// =============================================================================
// Registry Helpers
// =============================================================================

static HRESULT RegisterCLSID(const wchar_t* dllPath)
{
    wchar_t clsidStr[64];
    if (!GuidToString(CLSID_UniKeyTSF, clsidStr, ARRAYSIZE(clsidStr)))
        return E_FAIL;

    // Key: HKCU\Software\Classes\CLSID\{...}
    wchar_t keyPath[256];
    StringCchPrintfW(keyPath, ARRAYSIZE(keyPath),
        L"Software\\Classes\\CLSID\\%s", clsidStr);

    HKEY hKey = nullptr;
    LONG res = RegCreateKeyExW(HKEY_CURRENT_USER, keyPath, 0, nullptr,
        REG_OPTION_NON_VOLATILE, KEY_WRITE | WRITE_DAC, nullptr, &hKey, nullptr);
    if (res != ERROR_SUCCESS) return SELFREG_E_CLASS;

    RegSetValueExW(hKey, nullptr, 0, REG_SZ,
        reinterpret_cast<const BYTE*>(TSF_DISPLAY_NAME),
        static_cast<DWORD>((wcslen(TSF_DISPLAY_NAME) + 1) * sizeof(wchar_t)));

    SetAppContainerACL(hKey);
    RegCloseKey(hKey);

    // InprocServer32 subkey
    wchar_t inprocPath[256];
    StringCchPrintfW(inprocPath, ARRAYSIZE(inprocPath),
        L"%s\\InprocServer32", keyPath);

    res = RegCreateKeyExW(HKEY_CURRENT_USER, inprocPath, 0, nullptr,
        REG_OPTION_NON_VOLATILE, KEY_WRITE | WRITE_DAC, nullptr, &hKey, nullptr);
    if (res != ERROR_SUCCESS) return SELFREG_E_CLASS;

    RegSetValueExW(hKey, nullptr, 0, REG_SZ,
        reinterpret_cast<const BYTE*>(dllPath),
        static_cast<DWORD>((wcslen(dllPath) + 1) * sizeof(wchar_t)));

    const wchar_t* threadModel = L"Apartment";
    RegSetValueExW(hKey, L"ThreadingModel", 0, REG_SZ,
        reinterpret_cast<const BYTE*>(threadModel),
        static_cast<DWORD>((wcslen(threadModel) + 1) * sizeof(wchar_t)));

    SetAppContainerACL(hKey);
    RegCloseKey(hKey);

    return S_OK;
}

static HRESULT RegisterTIPProfile()
{
    ComPtr<ITfInputProcessorProfileMgr> pProfileMgr;
    HRESULT hr = CoCreateInstance(
        CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pProfileMgr));
    if (FAILED(hr)) return hr;

    hr = pProfileMgr->RegisterProfile(
        CLSID_UniKeyTSF,
        UNIKEY_LANGID,
        GUID_UniKeyTSF_Profile,
        TSF_DISPLAY_NAME,
        static_cast<ULONG>(wcslen(TSF_DISPLAY_NAME)),
        nullptr,          // pchIconFile (no custom icon file)
        0,                // cchFile
        0,                // uIconIndex
        static_cast<HKL>(nullptr),  // hklSubstitute
        0,                // dwPreferredLayout
        TRUE,             // bEnabledByDefault
        0                 // dwFlags
    );

    if (FAILED(hr)) return hr;

    // Register in the CTF category
    ComPtr<ITfCategoryMgr> pCatMgr;
    hr = CoCreateInstance(
        CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pCatMgr));
    if (FAILED(hr)) return hr;

    hr = pCatMgr->RegisterCategory(
        CLSID_UniKeyTSF,
        GUID_TFCAT_TIP_KEYBOARD,
        CLSID_UniKeyTSF);

    return hr;
}

static HRESULT UnregisterCLSID()
{
    wchar_t clsidStr[64];
    if (!GuidToString(CLSID_UniKeyTSF, clsidStr, ARRAYSIZE(clsidStr)))
        return E_FAIL;

    wchar_t keyPath[256];
    StringCchPrintfW(keyPath, ARRAYSIZE(keyPath),
        L"Software\\Classes\\CLSID\\%s", clsidStr);

    // Delete InprocServer32 subkey first, then the CLSID key
    wchar_t inprocPath[256];
    StringCchPrintfW(inprocPath, ARRAYSIZE(inprocPath),
        L"%s\\InprocServer32", keyPath);

    RegDeleteKeyW(HKEY_CURRENT_USER, inprocPath);
    RegDeleteKeyW(HKEY_CURRENT_USER, keyPath);

    return S_OK;
}

static HRESULT UnregisterTIPProfile()
{
    ComPtr<ITfInputProcessorProfileMgr> pProfileMgr;
    HRESULT hr = CoCreateInstance(
        CLSID_TF_InputProcessorProfiles, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pProfileMgr));
    if (FAILED(hr)) return hr;

    hr = pProfileMgr->UnregisterProfile(
        CLSID_UniKeyTSF,
        UNIKEY_LANGID,
        GUID_UniKeyTSF_Profile,
        0);

    // Unregister category
    ComPtr<ITfCategoryMgr> pCatMgr;
    HRESULT hrCat = CoCreateInstance(
        CLSID_TF_CategoryMgr, nullptr, CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&pCatMgr));
    if (SUCCEEDED(hrCat)) {
        pCatMgr->UnregisterCategory(
            CLSID_UniKeyTSF,
            GUID_TFCAT_TIP_KEYBOARD,
            CLSID_UniKeyTSF);
    }

    return hr;
}

// =============================================================================
// DLL Entry Points
// =============================================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID /*lpReserved*/)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH:
        g_hModule = hModule;
        DisableThreadLibraryCalls(hModule);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppvObj)
{
    if (!ppvObj) return E_INVALIDARG;
    *ppvObj = nullptr;

    if (!IsEqualCLSID(rclsid, CLSID_UniKeyTSF))
        return CLASS_E_CLASSNOTAVAILABLE;

    CClassFactory* pFactory = new (std::nothrow) CClassFactory();
    if (!pFactory) return E_OUTOFMEMORY;

    HRESULT hr = pFactory->QueryInterface(riid, ppvObj);
    pFactory->Release();
    return hr;
}

STDAPI DllCanUnloadNow()
{
    return (g_cDllRef <= 0) ? S_OK : S_FALSE;
}

STDAPI DllRegisterServer()
{
    wchar_t dllPath[MAX_PATH];
    GetModuleFileNameW(g_hModule, dllPath, MAX_PATH);

    HRESULT hr = RegisterCLSID(dllPath);
    if (FAILED(hr)) return hr;

    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    hr = RegisterTIPProfile();
    CoUninitialize();

    return hr;
}

STDAPI DllUnregisterServer()
{
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    UnregisterTIPProfile();
    CoUninitialize();

    return UnregisterCLSID();
}
