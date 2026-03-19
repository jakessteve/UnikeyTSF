// =============================================================================
// UniKey TSF Reborn — Settings Dialog (v4: Black + Dark Tabs)
// =============================================================================
// Pure black background, white text, subclassed tab control for dark rendering,
// compact layout with proper spacing.
// =============================================================================

#include "settings_dialog.h"
#include "../resource.h"
#include "../shared_config.h"
#include "../config/blacklist.h"
#include <windows.h>
#include <winreg.h>
#include <windowsx.h>
#include <commctrl.h>
#include <uxtheme.h>

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "comctl32.lib")

// =============================================================================
// Color Palettes
// =============================================================================

// Dark
static const COLORREF DARK_BG           = RGB(0, 0, 0);
static const COLORREF DARK_BG_CONTROL   = RGB(16, 16, 16);
static const COLORREF DARK_BG_HOVER     = RGB(38, 38, 38);
static const COLORREF DARK_BORDER       = RGB(40, 40, 40);
static const COLORREF DARK_BORDER_FOCUS = RGB(0, 150, 255);
static const COLORREF DARK_TEXT         = RGB(224, 224, 224);
static const COLORREF DARK_TEXT_DIM     = RGB(150, 150, 150);
static const COLORREF DARK_BTN_BG       = RGB(24, 24, 24);
static const COLORREF DARK_GROUP_BORDER = RGB(36, 36, 36);

// Light
static const COLORREF LIGHT_BG           = RGB(255, 255, 255);
static const COLORREF LIGHT_BG_CONTROL   = RGB(240, 240, 240);
static const COLORREF LIGHT_BG_HOVER     = RGB(225, 225, 225);
static const COLORREF LIGHT_BORDER       = RGB(200, 200, 200);
static const COLORREF LIGHT_BORDER_FOCUS = RGB(0, 100, 200);
static const COLORREF LIGHT_TEXT         = RGB(10, 10, 10);
static const COLORREF LIGHT_TEXT_DIM     = RGB(80, 80, 80);
static const COLORREF LIGHT_BTN_BG       = RGB(245, 245, 245);
static const COLORREF LIGHT_GROUP_BORDER = RGB(220, 220, 220);

static COLORREF CLR_BG, CLR_BG_CONTROL, CLR_BG_HOVER, CLR_BORDER, CLR_BORDER_FOCUS, CLR_GROUP_BORDER;
static COLORREF CLR_TEXT, CLR_TEXT_DIM, CLR_BTN_BG;
static const COLORREF CLR_ACCENT = RGB(0, 120, 215);
static const COLORREF CLR_BTN_PRIMARY = RGB(0, 100, 200);

// Brushes
static HBRUSH s_hBrushBg      = nullptr;
static HBRUSH s_hBrushControl = nullptr;
static HBRUSH s_hBrushBtnBg   = nullptr;

static UniKeyConfig* s_pConfig = nullptr;
static HFONT s_hUIFont = nullptr;
static int s_CurrentTab = 0; // 0 = Basic, 1 = Advanced
// =============================================================================
// Auto-start registry
// =============================================================================

static const wchar_t* AUTO_START_REG_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const wchar_t* AUTO_START_REG_VAL = L"UniKeyTSF";

static bool IsAutoStartEnabled() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, AUTO_START_REG_KEY, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD type;
        if (RegQueryValueExW(hKey, AUTO_START_REG_VAL, nullptr, &type, nullptr, nullptr) == ERROR_SUCCESS) {
            RegCloseKey(hKey);
            return true;
        }
        RegCloseKey(hKey);
    }
    return false;
}

static void SetAutoStart(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, AUTO_START_REG_KEY, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (enable) {
            wchar_t exePath[MAX_PATH];
            GetModuleFileNameW(nullptr, exePath, MAX_PATH);
            RegSetValueExW(hKey, AUTO_START_REG_VAL, 0, REG_SZ,
                (const BYTE*)exePath, (DWORD)((wcslen(exePath) + 1) * sizeof(wchar_t)));
        } else {
            RegDeleteValueW(hKey, AUTO_START_REG_VAL);
        }
        RegCloseKey(hKey);
    }
}

// =============================================================================
// Dark Mode Helpers
// =============================================================================

static bool ShouldUseDarkMode() {
    HKEY hKey;
    bool isDark = true;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        DWORD value = 0;
        DWORD size = sizeof(value);
        if (RegQueryValueExW(hKey, L"AppsUseLightTheme", nullptr, nullptr, (LPBYTE)&value, &size) == ERROR_SUCCESS) {
            isDark = (value == 0);
        }
        RegCloseKey(hKey);
    }
    return isDark;
}

static void UpdateColorsBasedOnTheme() {
    bool dark = ShouldUseDarkMode();
    CLR_BG           = dark ? DARK_BG           : LIGHT_BG;
    CLR_BG_CONTROL   = dark ? DARK_BG_CONTROL   : LIGHT_BG_CONTROL;
    CLR_BG_HOVER     = dark ? DARK_BG_HOVER     : LIGHT_BG_HOVER;
    CLR_BORDER       = dark ? DARK_BORDER       : LIGHT_BORDER;
    CLR_BORDER_FOCUS = dark ? DARK_BORDER_FOCUS : LIGHT_BORDER_FOCUS;
    CLR_GROUP_BORDER = dark ? DARK_GROUP_BORDER : LIGHT_GROUP_BORDER;
    CLR_TEXT         = dark ? DARK_TEXT         : LIGHT_TEXT;
    CLR_TEXT_DIM     = dark ? DARK_TEXT_DIM     : LIGHT_TEXT_DIM;
    CLR_BTN_BG       = dark ? DARK_BTN_BG       : LIGHT_BTN_BG;
}

static void InitThemeBrushes() {
    UpdateColorsBasedOnTheme();
    if (s_hBrushBg) DeleteObject(s_hBrushBg);
    if (s_hBrushControl) DeleteObject(s_hBrushControl);
    if (s_hBrushBtnBg) DeleteObject(s_hBrushBtnBg);

    s_hBrushBg      = CreateSolidBrush(CLR_BG);
    s_hBrushControl = CreateSolidBrush(CLR_BG_CONTROL);
    s_hBrushBtnBg   = CreateSolidBrush(CLR_BTN_BG);
}

static void SetDarkTitleBar(HWND hWnd) {
    BOOL useDarkMode = ShouldUseDarkMode() ? TRUE : FALSE;
    HMODULE hDwm = LoadLibraryW(L"dwmapi.dll");
    if (hDwm) {
        typedef HRESULT(WINAPI* PFN_DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
        auto pfn = (PFN_DwmSetWindowAttribute)GetProcAddress(hDwm, "DwmSetWindowAttribute");
        if (pfn) {
            // Apply dark mode to title bar (DWMWA_USE_IMMERSIVE_DARK_MODE = 20)
            pfn(hWnd, 20, &useDarkMode, sizeof(useDarkMode));
            
            // Force Windows 11 rounded corners (DWMWA_WINDOW_CORNER_PREFERENCE = 33, DWMWCP_ROUND = 2)
            DWORD cornerPreference = 2;
            pfn(hWnd, 33, &cornerPreference, sizeof(cornerPreference));
        }
        FreeLibrary(hDwm);
    }
}

static HFONT CreateUIFont() {
    LOGFONTW lf = {0};
    SystemParametersInfoW(SPI_GETICONTITLELOGFONT, sizeof(LOGFONTW), &lf, 0);
    return CreateFontIndirectW(&lf);
}

// =============================================================================
// Load / Save Config
// =============================================================================

static void LoadConfigToUI(HWND hWnd) {
    if (!s_pConfig) return;

    CheckDlgButton(hWnd, IDC_CHK_VIETNAMESE,
        s_pConfig->inputEnabled ? BST_CHECKED : BST_UNCHECKED);

    HWND hIM = GetDlgItem(hWnd, IDC_CBO_INPUT_METHOD);
    SendMessageW(hIM, CB_RESETCONTENT, 0, 0);
    SendMessageW(hIM, CB_ADDSTRING, 0, (LPARAM)L"Telex");
    SendMessageW(hIM, CB_ADDSTRING, 0, (LPARAM)L"VNI");
    SendMessageW(hIM, CB_ADDSTRING, 0, (LPARAM)L"VIQR");
    SendMessageW(hIM, CB_SETCURSEL, s_pConfig->inputMethod, 0);

    HWND hCS = GetDlgItem(hWnd, IDC_CBO_CHARSET);
    SendMessageW(hCS, CB_RESETCONTENT, 0, 0);
    SendMessageW(hCS, CB_ADDSTRING, 0, (LPARAM)L"Unicode");
    SendMessageW(hCS, CB_ADDSTRING, 0, (LPARAM)L"TCVN3 (ABC)");
    SendMessageW(hCS, CB_ADDSTRING, 0, (LPARAM)L"VNI Windows");
    SendMessageW(hCS, CB_SETCURSEL, s_pConfig->charset, 0);

    if (s_pConfig->toneType == TONE_MODERN)
        CheckRadioButton(hWnd, IDC_RAD_MODERN_TONE, IDC_RAD_CLASSIC_TONE, IDC_RAD_MODERN_TONE);
    else
        CheckRadioButton(hWnd, IDC_RAD_MODERN_TONE, IDC_RAD_CLASSIC_TONE, IDC_RAD_CLASSIC_TONE);

    if (s_pConfig->toggleKey == TK_ALT_Z)
        CheckRadioButton(hWnd, IDC_RAD_CTRL_SHIFT, IDC_RAD_ALT_Z, IDC_RAD_ALT_Z);
    else
        CheckRadioButton(hWnd, IDC_RAD_CTRL_SHIFT, IDC_RAD_ALT_Z, IDC_RAD_CTRL_SHIFT);

    CheckDlgButton(hWnd, IDC_CHK_SPELL_CHECK,
        s_pConfig->spellCheck ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHK_FREE_TONE,
        s_pConfig->freeToneMarking ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHK_MACRO,
        s_pConfig->macroEnabled ? BST_CHECKED : BST_UNCHECKED);
    CheckDlgButton(hWnd, IDC_CHK_AUTO_START,
        IsAutoStartEnabled() ? BST_CHECKED : BST_UNCHECKED);

    SetDlgItemTextW(hWnd, IDC_TXT_MACRO_FILE, s_pConfig->macroFilePath);
}

static void SaveUIToConfig(HWND hWnd) {
    if (!s_pConfig) return;

    HANDLE hMutex = OpenMutexW(SYNCHRONIZE, FALSE, UNIKEY_SHARED_MUTEX_NAME);
    if (hMutex) WaitForSingleObject(hMutex, INFINITE);

    s_pConfig->inputEnabled = (IsDlgButtonChecked(hWnd, IDC_CHK_VIETNAMESE) == BST_CHECKED) ? 1 : 0;
    s_pConfig->inputMethod = (uint8_t)SendDlgItemMessageW(hWnd, IDC_CBO_INPUT_METHOD, CB_GETCURSEL, 0, 0);
    s_pConfig->charset = (uint8_t)SendDlgItemMessageW(hWnd, IDC_CBO_CHARSET, CB_GETCURSEL, 0, 0);
    s_pConfig->toneType = (IsDlgButtonChecked(hWnd, IDC_RAD_MODERN_TONE) == BST_CHECKED) ? TONE_MODERN : TONE_CLASSIC;
    s_pConfig->spellCheck = (IsDlgButtonChecked(hWnd, IDC_CHK_SPELL_CHECK) == BST_CHECKED) ? 1 : 0;
    s_pConfig->freeToneMarking = (IsDlgButtonChecked(hWnd, IDC_CHK_FREE_TONE) == BST_CHECKED) ? 1 : 0;
    s_pConfig->macroEnabled = (IsDlgButtonChecked(hWnd, IDC_CHK_MACRO) == BST_CHECKED) ? 1 : 0;
    s_pConfig->toggleKey = (IsDlgButtonChecked(hWnd, IDC_RAD_ALT_Z) == BST_CHECKED) ? TK_ALT_Z : TK_CTRL_SHIFT;

    SetAutoStart(IsDlgButtonChecked(hWnd, IDC_CHK_AUTO_START) == BST_CHECKED);
    GetDlgItemTextW(hWnd, IDC_TXT_MACRO_FILE, s_pConfig->macroFilePath, 260);

    if (hMutex) {
        ReleaseMutex(hMutex);
        CloseHandle(hMutex);
    }
}

// =============================================================================
// Settings Helpers
// =============================================================================

static void UpdateMacroUIMode(HWND hWnd) {
    bool on = (IsDlgButtonChecked(hWnd, IDC_CHK_MACRO) == BST_CHECKED);
    EnableWindow(GetDlgItem(hWnd, IDC_TXT_MACRO_FILE), on);
    EnableWindow(GetDlgItem(hWnd, IDC_BTN_MACRO_FILE), on);
}

static void BrowseMacroFile(HWND hWnd) {
    wchar_t szFile[MAX_PATH] = {0};
    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"UniKey Macro (*.ukm)\0*.ukm\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameW(&ofn)) {
        SetDlgItemTextW(hWnd, IDC_TXT_MACRO_FILE, szFile);
    }
}

static void BrowseBlacklistFile(HWND hWnd) {
    wchar_t szFile[MAX_PATH] = {0};
    OPENFILENAMEW ofn = {0};
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hWnd;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrFilter = L"Programs (*.exe)\0*.exe\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
    if (GetOpenFileNameW(&ofn)) {
        wchar_t* pChar = wcsrchr(szFile, L'\\');
        if (pChar) {
            SetDlgItemTextW(hWnd, IDC_TXT_BLACKLIST_ADD, pChar + 1);
        } else {
            SetDlgItemTextW(hWnd, IDC_TXT_BLACKLIST_ADD, szFile);
        }
    }
}

// =============================================================================
// Custom Drawing
// =============================================================================

static void DrawDarkGroupBox(HWND hWnd, HDC hdc, HWND hGroup) {
    RECT rc;
    GetWindowRect(hGroup, &rc);
    MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rc, 2);

    wchar_t szText[128] = {0};
    GetWindowTextW(hGroup, szText, 128);

    HPEN hPen = CreatePen(PS_SOLID, 1, CLR_GROUP_BORDER);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));

    int textHeight = 0;
    if (szText[0] != L'\0') {
        SIZE textSize;
        GetTextExtentPoint32W(hdc, szText, (int)wcslen(szText), &textSize);
        textHeight = textSize.cy / 2;
    }

    RECT borderRc = { rc.left, rc.top + textHeight, rc.right, rc.bottom };
    RoundRect(hdc, borderRc.left, borderRc.top, borderRc.right, borderRc.bottom, 8, 8);

    if (szText[0] != L'\0') {
        SetTextColor(hdc, CLR_TEXT_DIM);
        SetBkColor(hdc, CLR_BG);
        SetBkMode(hdc, OPAQUE);
        TextOutW(hdc, rc.left + 10, rc.top, szText, (int)wcslen(szText));
    }

    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hPen);
}

static void DrawDarkButton(DRAWITEMSTRUCT* pDIS) {
    HDC hdc = pDIS->hDC;
    RECT rc = pDIS->rcItem;
    bool isPressed = (pDIS->itemState & ODS_SELECTED) != 0;
    bool isFocused = (pDIS->itemState & ODS_FOCUS) != 0;
    bool isDefault = (pDIS->itemState & ODS_DEFAULT) != 0;

    bool isTab = (pDIS->CtlID == IDC_TAB_BASIC || pDIS->CtlID == IDC_TAB_ADVANCED);
    bool isSelectedTab = false;
    if (isTab) {
        isSelectedTab = (pDIS->CtlID == IDC_TAB_BASIC && s_CurrentTab == 0) || 
                        (pDIS->CtlID == IDC_TAB_ADVANCED && s_CurrentTab == 1);
        if (isSelectedTab) isPressed = true;
    }

    COLORREF bgColor;
    if (isDefault || pDIS->CtlID == (UINT)IDOK || isSelectedTab)
        bgColor = isPressed ? CLR_ACCENT : CLR_BTN_PRIMARY;
    else
        bgColor = isPressed ? CLR_BG_HOVER : CLR_BTN_BG;

    HBRUSH hBrush = CreateSolidBrush(bgColor);
    HPEN hPen = CreatePen(PS_SOLID, 1, isFocused ? CLR_BORDER_FOCUS : CLR_BORDER);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 8, 8);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
    DeleteObject(hPen);

    wchar_t szText[64] = {0};
    GetWindowTextW(pDIS->hwndItem, szText, 64);
    SetTextColor(hdc, CLR_TEXT);
    SetBkMode(hdc, TRANSPARENT);
    DrawTextW(hdc, szText, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

// =============================================================================
// WM_CTLCOLOR* Handlers
// =============================================================================

static INT_PTR HandleCtlColor(HDC hdc, UINT msg) {
    switch (msg) {
    case WM_CTLCOLORDLG:
        return (INT_PTR)s_hBrushBg;
    case WM_CTLCOLORSTATIC:
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG);
        SetBkMode(hdc, TRANSPARENT);
        return (INT_PTR)s_hBrushBg;
    case WM_CTLCOLOREDIT:
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG_CONTROL);
        return (INT_PTR)s_hBrushControl;
    case WM_CTLCOLORLISTBOX:
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG_CONTROL);
        return (INT_PTR)s_hBrushControl;
    case WM_CTLCOLORBTN:
        SetTextColor(hdc, CLR_TEXT);
        SetBkColor(hdc, CLR_BG);
        return (INT_PTR)s_hBrushBg;
    default:
        return 0;
    }
}

static void UpdateTabContent(HWND hWnd) {
    int sel = s_CurrentTab;

    int basic[] = {
        IDC_CHK_VIETNAMESE, IDC_GRP_BASIC, IDC_LBL_INPUT_METHOD, IDC_CBO_INPUT_METHOD,
        IDC_LBL_CHARSET, IDC_CBO_CHARSET, IDC_GRP_TOGGLE_KEY, IDC_RAD_CTRL_SHIFT,
        IDC_RAD_ALT_Z, IDC_GRP_TONE_TYPE, IDC_RAD_MODERN_TONE, IDC_RAD_CLASSIC_TONE,
        IDC_GRP_OPTIONS, IDC_CHK_SPELL_CHECK, IDC_CHK_FREE_TONE, IDC_CHK_MACRO,
        IDC_CHK_AUTO_START, IDC_LBL_MACRO_FILE, IDC_TXT_MACRO_FILE, IDC_BTN_MACRO_FILE
    };
    
    int advanced[] = {
        IDC_GRP_BLACKLIST, IDC_LST_BLACKLIST, IDC_TXT_BLACKLIST_ADD,
        IDC_BTN_BLACKLIST_BROWSE, IDC_BTN_BLACKLIST_DEL, IDC_BTN_BLACKLIST_ADD
    };

    for (int id : basic) ShowWindow(GetDlgItem(hWnd, id), sel == 0 ? SW_SHOW : SW_HIDE);
    for (int id : advanced) ShowWindow(GetDlgItem(hWnd, id), sel == 1 ? SW_SHOW : SW_HIDE);
}

static HWND s_hToolTip = nullptr;

static void CreateToolTips(HWND hWndDlg) {
    if (!s_hToolTip) {
        s_hToolTip = CreateWindowExW(WS_EX_TOPMOST, TOOLTIPS_CLASSW, NULL,
            WS_POPUP | TTS_ALWAYSTIP | TTS_NOPREFIX,
            CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
            hWndDlg, NULL, GetModuleHandle(NULL), NULL);
        
        SetWindowTheme(s_hToolTip, L"Explorer", NULL);
        
        TOOLINFOW ti = { 0 };
        ti.cbSize = sizeof(TOOLINFOW);
        ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
        ti.hwnd = hWndDlg;
        
        ti.uId = (UINT_PTR)GetDlgItem(hWndDlg, IDC_CHK_FREE_TONE);
        ti.lpszText = (LPWSTR)L"Bỏ dấu ở bất kỳ vị trí nào trong từ và di chuyển dấu tự động";
        SendMessageW(s_hToolTip, TTM_ADDTOOLW, 0, (LPARAM)&ti);

        ti.uId = (UINT_PTR)GetDlgItem(hWndDlg, IDC_CHK_SPELL_CHECK);
        ti.lpszText = (LPWSTR)L"Chỉ cho phép gõ dấu khi các chữ cái tạo thành từ tiếng Việt hợp lệ";
        SendMessageW(s_hToolTip, TTM_ADDTOOLW, 0, (LPARAM)&ti);
    }
}

// =============================================================================
// Dialog Procedure
// =============================================================================

static INT_PTR CALLBACK SettingsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        s_pConfig = (UniKeyConfig*)lParam;
        InitThemeBrushes();
        SetDarkTitleBar(hWnd);

        s_hUIFont = CreateUIFont();
        EnumChildWindows(hWnd, [](HWND hChild, LPARAM lp) -> BOOL {
            SendMessageW(hChild, WM_SETFONT, (WPARAM)lp, TRUE);
            return TRUE;
        }, (LPARAM)s_hUIFont);
        SendMessageW(hWnd, WM_SETFONT, (WPARAM)s_hUIFont, TRUE);

        CreateToolTips(hWnd);

        // Initialize Tabs
        s_CurrentTab = 0;
        UpdateTabContent(hWnd);

        // --- Owner-draw buttons ---
        HWND hTabBasic = GetDlgItem(hWnd, IDC_TAB_BASIC);
        HWND hTabAdvanced = GetDlgItem(hWnd, IDC_TAB_ADVANCED);
        HWND hBtnClose = GetDlgItem(hWnd, IDOK);
        HWND hBtnMacro = GetDlgItem(hWnd, IDC_BTN_MACRO_FILE);
        HWND hBtnAdd = GetDlgItem(hWnd, IDC_BTN_BLACKLIST_ADD);
        HWND hBtnDel = GetDlgItem(hWnd, IDC_BTN_BLACKLIST_DEL);
        HWND hBtnBlacklistBrowse = GetDlgItem(hWnd, IDC_BTN_BLACKLIST_BROWSE);
        
        if (hTabBasic) {
            SetWindowLongW(hTabBasic, GWL_STYLE, GetWindowLongW(hTabBasic, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hTabBasic, L"", L"");
        }
        if (hTabAdvanced) {
            SetWindowLongW(hTabAdvanced, GWL_STYLE, GetWindowLongW(hTabAdvanced, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hTabAdvanced, L"", L"");
        }
        if (hBtnClose) {
            SetWindowLongW(hBtnClose, GWL_STYLE, GetWindowLongW(hBtnClose, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hBtnClose, L"", L"");
        }
        if (hBtnMacro) {
            SetWindowLongW(hBtnMacro, GWL_STYLE, GetWindowLongW(hBtnMacro, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hBtnMacro, L"", L"");
        }
        if (hBtnAdd) {
            SetWindowLongW(hBtnAdd, GWL_STYLE, GetWindowLongW(hBtnAdd, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hBtnAdd, L"", L"");
        }
        if (hBtnDel) {
            SetWindowLongW(hBtnDel, GWL_STYLE, GetWindowLongW(hBtnDel, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hBtnDel, L"", L"");
        }
        if (hBtnBlacklistBrowse) {
            SetWindowLongW(hBtnBlacklistBrowse, GWL_STYLE, GetWindowLongW(hBtnBlacklistBrowse, GWL_STYLE) | BS_OWNERDRAW);
            SetWindowTheme(hBtnBlacklistBrowse, L"", L"");
        }

        // Disable themes on combos
        HWND hCboIM = GetDlgItem(hWnd, IDC_CBO_INPUT_METHOD);
        HWND hCboCS = GetDlgItem(hWnd, IDC_CBO_CHARSET);
        if (hCboIM) SetWindowTheme(hCboIM, L"", L"");
        if (hCboCS) SetWindowTheme(hCboCS, L"", L"");

        // Load Blacklist File
        HWND hList = GetDlgItem(hWnd, IDC_LST_BLACKLIST);
        if (hList) {
            auto list = LoadBlacklist();
            for (const auto& name : list) {
                SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)name.c_str());
            }
        }

        LoadConfigToUI(hWnd);
        UpdateMacroUIMode(hWnd);
        return (INT_PTR)TRUE;
    }

    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
    case WM_CTLCOLORLISTBOX:
    case WM_CTLCOLORBTN: {
        INT_PTR result = HandleCtlColor((HDC)wParam, message);
        if (result) return result;
        break;
    }

    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rc;
        GetClientRect(hWnd, &rc);
        FillRect(hdc, &rc, s_hBrushBg);
        return (INT_PTR)TRUE;
    }

    case WM_SETTINGCHANGE: {
        InitThemeBrushes();
        InvalidateRect(hWnd, nullptr, TRUE);
        return (INT_PTR)TRUE;
    }

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HFONT hOldFont = s_hUIFont ? (HFONT)SelectObject(hdc, s_hUIFont) : nullptr;

        HWND groupboxes[] = {
            GetDlgItem(hWnd, IDC_GRP_TOGGLE_KEY),
            GetDlgItem(hWnd, IDC_GRP_TONE_TYPE),
            GetDlgItem(hWnd, IDC_GRP_OPTIONS),
            GetDlgItem(hWnd, IDC_GRP_BLACKLIST),
        };
        for (HWND hGroup : groupboxes) {
            if (hGroup && IsWindowVisible(hGroup)) DrawDarkGroupBox(hWnd, hdc, hGroup);
        }

        if (hOldFont) SelectObject(hdc, hOldFont);
        EndPaint(hWnd, &ps);
        return (INT_PTR)FALSE;
    }

    case WM_DRAWITEM: {
        DRAWITEMSTRUCT* pDIS = (DRAWITEMSTRUCT*)lParam;
        if (pDIS->CtlType == ODT_BUTTON) {
            DrawDarkButton(pDIS);
            return (INT_PTR)TRUE;
        }
        break;
    }

    case WM_NOTIFY: {
        break;
    }

    case WM_COMMAND: {
        WORD wId = LOWORD(wParam);
        WORD wNotifyCmd = HIWORD(wParam);

        if (wId == IDC_TAB_BASIC || wId == IDC_TAB_ADVANCED) {
            s_CurrentTab = (wId == IDC_TAB_BASIC) ? 0 : 1;
            UpdateTabContent(hWnd);
            InvalidateRect(GetDlgItem(hWnd, IDC_TAB_BASIC), nullptr, TRUE);
            InvalidateRect(GetDlgItem(hWnd, IDC_TAB_ADVANCED), nullptr, TRUE);
            return (INT_PTR)TRUE;
        }

        if (wId == IDOK || wId == IDCANCEL) {
            if (wId == IDOK) SaveUIToConfig(hWnd);
            if (s_hUIFont) { DeleteObject(s_hUIFont); s_hUIFont = nullptr; }
            EndDialog(hWnd, wId);
            return (INT_PTR)TRUE;
        }
        else if (wId == IDC_CHK_MACRO && wNotifyCmd == BN_CLICKED) {
            UpdateMacroUIMode(hWnd);
        }
        else if (wId == IDC_BTN_MACRO_FILE && wNotifyCmd == BN_CLICKED) {
            BrowseMacroFile(hWnd);
        }
        else if (wId == IDC_BTN_BLACKLIST_BROWSE && wNotifyCmd == BN_CLICKED) {
            BrowseBlacklistFile(hWnd);
        }
        else if (wId == IDC_BTN_BLACKLIST_ADD && wNotifyCmd == BN_CLICKED) {
            wchar_t szName[MAX_PATH] = {};
            GetDlgItemTextW(hWnd, IDC_TXT_BLACKLIST_ADD, szName, MAX_PATH);
            
            // Trim whitespace
            std::wstring name(szName);
            name.erase(0, name.find_first_not_of(L" \t\r\n"));
            name.erase(name.find_last_not_of(L" \t\r\n") + 1);

            if (!name.empty()) {
                HWND hList = GetDlgItem(hWnd, IDC_LST_BLACKLIST);
                
                // Check for duplicates
                int count = (int)SendMessageW(hList, LB_GETCOUNT, 0, 0);
                bool duplicate = false;
                for (int i = 0; i < count; i++) {
                    wchar_t buf[MAX_PATH];
                    SendMessageW(hList, LB_GETTEXT, i, (LPARAM)buf);
                    if (_wcsicmp(buf, name.c_str()) == 0) {
                        duplicate = true;
                        break;
                    }
                }

                if (!duplicate) {
                    SendMessageW(hList, LB_ADDSTRING, 0, (LPARAM)name.c_str());
                    SetDlgItemTextW(hWnd, IDC_TXT_BLACKLIST_ADD, L"");
                    
                    std::vector<std::wstring> list;
                    count = (int)SendMessageW(hList, LB_GETCOUNT, 0, 0);
                    for (int i = 0; i < count; i++) {
                        wchar_t buf[MAX_PATH];
                        SendMessageW(hList, LB_GETTEXT, i, (LPARAM)buf);
                        list.push_back(buf);
                    }
                    SaveBlacklist(list);
                    ReloadBlacklist();
                } else {
                    // Flash the list or something? For now just clear the text
                    SetDlgItemTextW(hWnd, IDC_TXT_BLACKLIST_ADD, L"");
                }
            }
        }
        else if (wId == IDC_BTN_BLACKLIST_DEL && wNotifyCmd == BN_CLICKED) {
            HWND hList = GetDlgItem(hWnd, IDC_LST_BLACKLIST);
            int sel = (int)SendMessageW(hList, LB_GETCURSEL, 0, 0);
            if (sel != LB_ERR) {
                SendMessageW(hList, LB_DELETESTRING, sel, 0);
                int count = (int)SendMessageW(hList, LB_GETCOUNT, 0, 0);
                std::vector<std::wstring> list;
                for (int i = 0; i < count; i++) {
                    wchar_t buf[MAX_PATH];
                    SendMessageW(hList, LB_GETTEXT, i, (LPARAM)buf);
                    list.push_back(buf);
                }
                SaveBlacklist(list);
                ReloadBlacklist();
            }
        }
        break;
    }

    case WM_DESTROY:
        if (s_hBrushBg)      { DeleteObject(s_hBrushBg);      s_hBrushBg = nullptr; }
        if (s_hBrushControl) { DeleteObject(s_hBrushControl);  s_hBrushControl = nullptr; }
        if (s_hBrushBtnBg)   { DeleteObject(s_hBrushBtnBg);    s_hBrushBtnBg = nullptr; }
        if (s_hToolTip)      { DestroyWindow(s_hToolTip);      s_hToolTip = nullptr; }
        break;
    }
    return (INT_PTR)FALSE;
}

// =============================================================================
// Public API
// =============================================================================

void ShowSettingsDialog(HWND hWndParent, HINSTANCE hInstance, UniKeyConfig* pConfig) {
    INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_TAB_CLASSES };
    InitCommonControlsEx(&icc);

    DialogBoxParamW(hInstance, MAKEINTRESOURCEW(IDD_SETTINGS_DIALOG),
                    hWndParent, SettingsDlgProc, (LPARAM)pConfig);
}
