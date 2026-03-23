// =============================================================================
// UniKey TSF Reborn — Settings Dialog (WebView2 + React Frontend)
// =============================================================================
// Creates a PERSISTENT window hosting the WebView2 control. The window is
// created once and then hidden/shown for instant subsequent opens.
// =============================================================================

#include "settings_dialog.h"
#include "settings_webview.h"
#include "../resource.h"
#include "../shared_config.h"
#include "../config/ipc_manager.h"
#include <windows.h>
#include <winreg.h>

// --- Persistent State ---
static SettingsWebView* s_pWebView = nullptr;
static HWND s_hSettingsWnd = nullptr;       // Persistent window handle
static bool s_classRegistered = false;
static HINSTANCE s_hInstance = nullptr;
static const wchar_t* SETTINGS_WND_CLASS = L"UniKeyTSF_SettingsWnd";

// =============================================================================
// Dark Title Bar
// =============================================================================

static void ApplyDarkTitleBar(HWND hWnd) {
    BOOL useDarkMode = TRUE;
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value = 0, size = sizeof(value);
        if (RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr,
            (LPBYTE)&value, &size) == ERROR_SUCCESS) {
            useDarkMode = (value == 0) ? TRUE : FALSE;
        }
        RegCloseKey(hKey);
    }

    HMODULE hDwm = LoadLibraryW(L"dwmapi.dll");
    if (hDwm) {
        typedef HRESULT(WINAPI* PFN)(HWND, DWORD, LPCVOID, DWORD);
        auto pfn = (PFN)GetProcAddress(hDwm, "DwmSetWindowAttribute");
        if (pfn) {
            pfn(hWnd, 20, &useDarkMode, sizeof(useDarkMode));
            DWORD corner = 2;
            pfn(hWnd, 33, &corner, sizeof(corner));
        }
        FreeLibrary(hDwm);
    }
}

// =============================================================================
// Window Procedure
// =============================================================================

#define WM_WEBVIEW_READY (WM_APP + 1)
static bool s_webviewReady = false;

static LRESULT CALLBACK SettingsWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        s_webviewReady = false;
        s_pWebView = new SettingsWebView();
        HRESULT hr = s_pWebView->Initialize(hWnd);
        if (FAILED(hr)) {
            MessageBoxW(hWnd,
                L"Không thể khởi tạo WebView2.\n\n"
                L"Vui lòng cài đặt Microsoft Edge WebView2 Runtime.",
                L"UniKey TSF Reborn", MB_ICONERROR | MB_OK);
            DestroyWindow(hWnd);
        }
        return 0;
    }

    case WM_PAINT: {
        if (!s_webviewReady) {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            RECT rc;
            GetClientRect(hWnd, &rc);

            HBRUSH hBrush = CreateSolidBrush(RGB(32, 32, 32));
            FillRect(hdc, &rc, hBrush);
            DeleteObject(hBrush);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(160, 160, 160));
            HFONT hFont = CreateFontW(
                14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
            HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
            DrawTextW(hdc, L"Đang tải...", -1, &rc,
                DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            SelectObject(hdc, hOldFont);
            DeleteObject(hFont);

            EndPaint(hWnd, &ps);
            return 0;
        }
        break;
    }

    case WM_WEBVIEW_READY:
        s_webviewReady = true;
        InvalidateRect(hWnd, nullptr, TRUE);
        return 0;

    case WM_SIZE: {
        if (s_pWebView) {
            RECT bounds;
            GetClientRect(hWnd, &bounds);
            s_pWebView->Resize(bounds);
        }
        return 0;
    }

    case WM_GETMINMAXINFO: {
        MINMAXINFO* mmi = (MINMAXINFO*)lParam;
        mmi->ptMinTrackSize.x = 600;
        mmi->ptMinTrackSize.y = 400;
        return 0;
    }

    case WM_CLOSE:
        // HIDE instead of destroy — keeps WebView2 alive for instant re-open
        ShowWindow(hWnd, SW_HIDE);
        return 0;

    case WM_DESTROY:
        if (s_pWebView) {
            delete s_pWebView;
            s_pWebView = nullptr;
        }
        s_hSettingsWnd = nullptr;
        s_webviewReady = false;
        return 0;

    default:
        return DefWindowProcW(hWnd, msg, wParam, lParam);
    }
    return DefWindowProcW(hWnd, msg, wParam, lParam);
}

// =============================================================================
// Public API
// =============================================================================

void ShowSettingsDialog(HWND hWndParent, HINSTANCE hInstance, UniKeyConfig* pConfig) {
    s_hInstance = hInstance;

    // If window already exists, just show it (INSTANT!)
    if (s_hSettingsWnd && IsWindow(s_hSettingsWnd)) {
        ShowWindow(s_hSettingsWnd, SW_SHOW);
        SetForegroundWindow(s_hSettingsWnd);

        // Refresh config data when re-showing
        if (s_pWebView && g_pConfig) {
            s_pWebView->PostConfigMessage(*g_pConfig);
        }
        return;
    }

    // First time only: create the window + WebView2
    CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);

    if (!s_classRegistered) {
        WNDCLASSEXW wc = {};
        wc.cbSize = sizeof(WNDCLASSEXW);
        wc.lpfnWndProc = SettingsWndProc;
        wc.hInstance = hInstance;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
        wc.lpszClassName = SETTINGS_WND_CLASS;
        wc.hIcon = LoadIconW(hInstance, MAKEINTRESOURCEW(IDI_APP_ICON));
        wc.hIconSm = wc.hIcon;
        if (!RegisterClassExW(&wc)) {
            return;
        }
        s_classRegistered = true;
    }

    // DPI-aware window sizing
    int dpi = GetDpiForSystem();
    float scale = (float)dpi / 96.0f;
    int w = (int)(920 * scale);
    int h = (int)(720 * scale);

    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);
    if (w > screenW - 40) w = screenW - 40;
    if (h > screenH - 80) h = screenH - 80;
    int x = (screenW - w) / 2;
    int y = (screenH - h) / 2;

    s_hSettingsWnd = CreateWindowExW(
        0,
        SETTINGS_WND_CLASS,
        L"UniKey TSF Reborn",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        x, y, w, h,
        nullptr,
        nullptr,
        hInstance,
        nullptr);

    if (!s_hSettingsWnd) {
        return;
    }

    ApplyDarkTitleBar(s_hSettingsWnd);
    ShowWindow(s_hSettingsWnd, SW_SHOW);
    UpdateWindow(s_hSettingsWnd);
}
