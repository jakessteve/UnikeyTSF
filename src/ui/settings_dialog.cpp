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
#include <windowsx.h>
#include <commctrl.h>
#include <uxtheme.h>

#pragma comment(lib, "uxtheme.lib")
#pragma comment(lib, "comctl32.lib")

// =============================================================================
// Pure Black Color Palette
// =============================================================================

static const COLORREF CLR_BG           = RGB(0, 0, 0);       // Pure black
static const COLORREF CLR_BG_CONTROL   = RGB(28, 28, 28);    // Input fields
static const COLORREF CLR_BG_HOVER     = RGB(45, 45, 45);    // Hover
static const COLORREF CLR_BORDER       = RGB(60, 60, 60);    // Borders
static const COLORREF CLR_BORDER_FOCUS = RGB(0, 120, 215);   // Focus ring
static const COLORREF CLR_TEXT         = RGB(235, 235, 235);  // White text
static const COLORREF CLR_TEXT_DIM     = RGB(150, 150, 150);  // Secondary
static const COLORREF CLR_ACCENT       = RGB(0, 120, 215);   // Blue accent
static const COLORREF CLR_GROUP_BORDER = RGB(50, 50, 50);    // Group borders
static const COLORREF CLR_BTN_BG      = RGB(45, 45, 45);     // Button
static const COLORREF CLR_BTN_PRIMARY = RGB(0, 100, 200);    // Primary button
static const COLORREF CLR_TAB_SEL     = RGB(0, 0, 0);        // Selected tab (same as bg)
static const COLORREF CLR_TAB_UNSEL   = RGB(20, 20, 20);     // Unselected tab

// Brushes
static HBRUSH s_hBrushBg      = nullptr;
static HBRUSH s_hBrushControl = nullptr;
static HBRUSH s_hBrushBtnBg   = nullptr;

static UniKeyConfig* s_pConfig = nullptr;
static HFONT s_hDarkFont = nullptr;
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

static void InitDarkBrushes() {
    if (!s_hBrushBg) {
        s_hBrushBg      = CreateSolidBrush(CLR_BG);
        s_hBrushControl = CreateSolidBrush(CLR_BG_CONTROL);
        s_hBrushBtnBg   = CreateSolidBrush(CLR_BTN_BG);
    }
}

static void SetDarkTitleBar(HWND hWnd) {
    BOOL useDarkMode = TRUE;
    HMODULE hDwm = LoadLibraryW(L"dwmapi.dll");
    if (hDwm) {
        typedef HRESULT(WINAPI* PFN_DwmSetWindowAttribute)(HWND, DWORD, LPCVOID, DWORD);
        auto pfn = (PFN_DwmSetWindowAttribute)GetProcAddress(hDwm, "DwmSetWindowAttribute");
        if (pfn) {
            pfn(hWnd, 20, &useDarkMode, sizeof(useDarkMode));
        }
        FreeLibrary(hDwm);
    }
}

static HFONT CreateDarkFont() {
    return CreateFontW(
        -20, 0, 0, 0, FW_NORMAL,
        FALSE, FALSE, FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_SWISS,
        L"Segoe UI"
    );
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
    RoundRect(hdc, borderRc.left, borderRc.top, borderRc.right, borderRc.bottom, 6, 6);

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

    COLORREF bgColor;
    if (isDefault || pDIS->CtlID == (UINT)IDOK)
        bgColor = isPressed ? CLR_ACCENT : CLR_BTN_PRIMARY;
    else
        bgColor = isPressed ? CLR_BG_HOVER : CLR_BTN_BG;

    HBRUSH hBrush = CreateSolidBrush(bgColor);
    HPEN hPen = CreatePen(PS_SOLID, 1, isFocused ? CLR_BORDER_FOCUS : CLR_BORDER);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    RoundRect(hdc, rc.left, rc.top, rc.right, rc.bottom, 6, 6);
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

// =============================================================================
// Dialog Procedure
// =============================================================================

static INT_PTR CALLBACK SettingsDlgProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        s_pConfig = (UniKeyConfig*)lParam;
        InitDarkBrushes();
        SetDarkTitleBar(hWnd);

        s_hDarkFont = CreateDarkFont();
        EnumChildWindows(hWnd, [](HWND hChild, LPARAM lp) -> BOOL {
            SendMessageW(hChild, WM_SETFONT, (WPARAM)lp, TRUE);
            return TRUE;
        }, (LPARAM)s_hDarkFont);
        SendMessageW(hWnd, WM_SETFONT, (WPARAM)s_hDarkFont, TRUE);

        // --- Owner-draw buttons ---
        HWND hBtnClose = GetDlgItem(hWnd, IDOK);
        HWND hBtnMacro = GetDlgItem(hWnd, IDC_BTN_MACRO_FILE);
        HWND hBtnAdd = GetDlgItem(hWnd, IDC_BTN_BLACKLIST_ADD);
        HWND hBtnDel = GetDlgItem(hWnd, IDC_BTN_BLACKLIST_DEL);
        HWND hBtnBlacklistBrowse = GetDlgItem(hWnd, IDC_BTN_BLACKLIST_BROWSE);
        
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

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        HFONT hOldFont = s_hDarkFont ? (HFONT)SelectObject(hdc, s_hDarkFont) : nullptr;

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

        if (wId == IDOK || wId == IDCANCEL) {
            if (wId == IDOK) SaveUIToConfig(hWnd);
            if (s_hDarkFont) { DeleteObject(s_hDarkFont); s_hDarkFont = nullptr; }
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
