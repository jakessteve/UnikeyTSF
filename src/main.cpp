// =============================================================================
// UniKey TSF Reborn — Manager EXE (Phase 2)
// =============================================================================
// Pure Win32 application:
//   - Bootstraps TSF DLL registration (manual /register flag)
//   - Creates shared memory for IPC with TSF DLLs (IpcManager)
//   - Shows a V/E system tray icon (TrayIcon)
//   - Handles user interactions and hooks (HookManager)
// =============================================================================

#ifndef WINVER
#define WINVER 0x0A00
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <windows.h>
#include "resource.h"
#include "config/ipc_manager.h"
#include "config/blacklist.h"
#include "ui/tray_icon.h"
#include "ui/settings_dialog.h"
#include "ui/settings_webview.h"
#include "engine/hook_manager.h"
#include "engine/tsf_registration.h"

// --- Globals ---
static HINSTANCE g_hInstance = nullptr;
static HWND      g_hWnd      = nullptr;

// --- Mutex for single instance ---
static HANDLE       g_hMutex       = nullptr;
static const wchar_t* MUTEX_NAME   = L"Local\\UniKeyTSF_SingleInstance";

// --- Taskbar restart resilience ---
static UINT         WM_TASKBAR_CREATED = 0;
#define IDT_TRAY_WATCHDOG  5001
#define TRAY_WATCHDOG_MS   30000

// --- Window Class ---
static const wchar_t* WND_CLASS    = L"UniKeyTSF_HiddenWnd";
static const wchar_t* WND_TITLE    = L"UniKey TSF Reborn";

// =============================================================================
// Window Procedure
// =============================================================================

static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_CREATE:
        RegisterHotKey(hWnd, IDH_TOOLKIT_CONVERT, MOD_CONTROL | MOD_SHIFT, VK_F6);
        RegisterHotKey(hWnd, IDH_TOOLKIT_STRIP, MOD_CONTROL | MOD_SHIFT, VK_F9);
        WM_TASKBAR_CREATED = RegisterWindowMessageW(L"TaskbarCreated");
        SetTimer(hWnd, IDT_TRAY_WATCHDOG, TRAY_WATCHDOG_MS, nullptr);
        return 0;

    case WM_MEASUREITEM:
        OnMeasureItem(hWnd, (MEASUREITEMSTRUCT*)lParam);
        return TRUE;

    case WM_DRAWITEM:
        OnDrawItem(hWnd, (DRAWITEMSTRUCT*)lParam);
        return TRUE;

    case WM_HOTKEY:
        HandleHotkey(wParam);
        return 0;

    case WM_TRAYICON:
        switch (LOWORD(lParam)) {
        case WM_LBUTTONUP:    ToggleInputState(); break;
        case WM_LBUTTONDBLCLK: ShowSettingsDialog(hWnd, g_hInstance, g_pConfig); break;
        case WM_RBUTTONUP:    ShowContextMenu(hWnd); break;
        }
        return 0;

    case WM_TIMER:
        if (wParam == IDT_TRAY_WATCHDOG) {
            UpdateTrayIcon(); // This also serves as a heartbeat to ensure icon exists
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_TOGGLE_VE: ToggleInputState(); break;
        case IDM_INPUT_TELEX:
            if (LockConfig()) { g_pConfig->inputMethod = IM_TELEX; UnlockConfig(); }
            UpdateTrayIcon();
            break;
        case IDM_INPUT_VNI:
            if (LockConfig()) { g_pConfig->inputMethod = IM_VNI; UnlockConfig(); }
            UpdateTrayIcon();
            break;
        case IDM_INPUT_VIQR:
            if (LockConfig()) { g_pConfig->inputMethod = IM_VIQR; UnlockConfig(); }
            UpdateTrayIcon();
            break;
        case IDM_CHARSET_UNICODE:
            if (LockConfig()) { g_pConfig->charset = CS_UNICODE; UnlockConfig(); }
            break;
        case IDM_CHARSET_TCVN3:
            if (LockConfig()) { g_pConfig->charset = CS_TCVN3; UnlockConfig(); }
            break;
        case IDM_CHARSET_VNI:
            if (LockConfig()) { g_pConfig->charset = CS_VNI_WIN; UnlockConfig(); }
            break;
        case IDM_SETTINGS:
            ShowSettingsDialog(hWnd, g_hInstance, g_pConfig);
            break;
        case IDM_EXIT:
            DestroyWindow(hWnd);
            break;
        }
        return 0;

    case WM_QUERYENDSESSION:
        RemoveTrayIcon();
        return TRUE;

    case WM_DESTROY:
        UninstallHooks();
        KillTimer(hWnd, IDT_TRAY_WATCHDOG);
        UnregisterHotKey(hWnd, IDH_TOOLKIT_CONVERT);
        UnregisterHotKey(hWnd, IDH_TOOLKIT_STRIP);
        RemoveTrayIcon();
        PostQuitMessage(0);
        return 0;

    default:
        if (WM_TASKBAR_CREATED != 0 && msg == WM_TASKBAR_CREATED) {
            InitTrayIcon(hWnd);
            return 0;
        }
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
}

// =============================================================================
// WinMain
// =============================================================================

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE /*hPrevInstance*/,
    _In_ LPWSTR lpCmdLine,
    _In_ int /*nCmdShow*/)
{
    g_hInstance = hInstance;
    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    if (lpCmdLine && wcsstr(lpCmdLine, L"/unregister")) {
        UnregisterTsfDll();
        return 0;
    }
    if (lpCmdLine && wcsstr(lpCmdLine, L"/register")) {
        RegisterTsfDll();
        return 0;
    }

    g_hMutex = CreateMutexW(nullptr, TRUE, MUTEX_NAME);
    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        if (g_hMutex) CloseHandle(g_hMutex);
        return 0;
    }

    if (!InitSharedMemory()) return 1;

    WNDCLASSEXW wc = {};
    wc.cbSize        = sizeof(WNDCLASSEXW);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance      = hInstance;
    wc.lpszClassName  = WND_CLASS;

    if (!RegisterClassExW(&wc)) {
        CleanupSharedMemory();
        return 1;
    }

    g_hWnd = CreateWindowExW(0, WND_CLASS, WND_TITLE, 0, 0, 0, 0, 0,
                             HWND_MESSAGE, nullptr, hInstance, nullptr);

    if (!g_hWnd) {
        CleanupSharedMemory();
        return 1;
    }

    if (!InitTrayIcon(g_hWnd)) {
        DestroyWindow(g_hWnd);
        CleanupSharedMemory();
        return 1;
    }

    InitBlacklist();
    SettingsWebView::PreInitEnvironment();  // Pre-cache WebView2 env for fast settings open
    InstallHooks(hInstance, g_hWnd);

    PostMessage(g_hWnd, WM_COMMAND, IDM_SETTINGS, 0);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    CleanupTrayResources();
    CleanupSharedMemory();

    if (g_hMutex) {
        ReleaseMutex(g_hMutex);
        CloseHandle(g_hMutex);
    }

    return static_cast<int>(msg.wParam);
}
