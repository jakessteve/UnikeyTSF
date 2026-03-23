#include "tsf_registration.h"
#include <strsafe.h>
#include <ole2.h>

typedef HRESULT (STDAPICALLTYPE *PFNDLLREGISTERSERVER)();

bool RegisterTsfDll()
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    wchar_t* lastSlash = wcsrchr(exePath, L'\\');
    if (!lastSlash) return false;
    *(lastSlash + 1) = L'\0';

    wchar_t dllPath[MAX_PATH];
    StringCchCopyW(dllPath, MAX_PATH, exePath);
    StringCchCatW(dllPath, MAX_PATH, L"uktsf_core64.dll");

    if (GetFileAttributesW(dllPath) != INVALID_FILE_ATTRIBUTES) {
        HMODULE hDll = LoadLibraryW(dllPath);
        if (hDll) {
            auto pfnRegister = reinterpret_cast<PFNDLLREGISTERSERVER>(
                GetProcAddress(hDll, "DllRegisterServer"));
            if (pfnRegister) {
                CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
                pfnRegister();
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
        StringCchPrintfW(cmd, ARRAYSIZE(cmd), L"regsvr32.exe /s \"%s\"", dllPath32);
        
        STARTUPINFOW si = { sizeof(si) };
        PROCESS_INFORMATION pi = { 0 };
        if (CreateProcessW(nullptr, cmd, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
            WaitForSingleObject(pi.hProcess, INFINITE);
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
        }
    }

    return true;
}

void UnregisterTsfDll()
{
    wchar_t exePath[MAX_PATH];
    GetModuleFileNameW(nullptr, exePath, MAX_PATH);

    wchar_t* lastSlash = wcsrchr(exePath, L'\\');
    if (!lastSlash) return;
    *(lastSlash + 1) = L'\0';

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
