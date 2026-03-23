#include "tray_icon.h"
#include <shellapi.h>
#include <strsafe.h>
#include "../resource.h"
#include "settings_dialog.h"
#include "../config/ipc_manager.h"

// --- Constants ---
static const COLORREF MENU_BG        = RGB(30, 30, 30);
static const COLORREF MENU_BG_HOVER  = RGB(0, 100, 200);
static const COLORREF MENU_TEXT      = RGB(230, 230, 230);
static const COLORREF MENU_SEPARATOR = RGB(60, 60, 60);
static const int MENU_ITEM_HEIGHT    = 32;
static const int MENU_ITEM_PADDING   = 28;
static const int MENU_SEP_HEIGHT     = 9;

// --- Globals ---
static HICON          g_hIconV       = nullptr;
static HICON          g_hIconE       = nullptr;
static NOTIFYICONDATAW g_nid          = {};
static HFONT          s_hMenuFont    = nullptr;

struct MenuItemData {
    wchar_t text[64];
    UINT    id;
    bool    isChecked;
    bool    isSeparator;
    bool    isSubmenu;
};

static MenuItemData s_menuItems[20];
static int s_menuItemCount = 0;

// =============================================================================
// Icon Creation — EVKey-style solid white letter on transparent background
// =============================================================================

static HICON CreateLetterIcon(wchar_t letter, int fontWidth = 0, int fontWeight = FW_EXTRABOLD)
{
    const int size = 16;
    HDC hdcScreen = GetDC(nullptr);
    HDC hdcMem = CreateCompatibleDC(hdcScreen);

    BITMAPINFO bmi = {};
    bmi.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth       = size;
    bmi.bmiHeader.biHeight      = -size;
    bmi.bmiHeader.biPlanes      = 1;
    bmi.bmiHeader.biBitCount    = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    DWORD* pBits = nullptr;
    HBITMAP hbmColor = CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS,
        reinterpret_cast<void**>(&pBits), nullptr, 0);
    HBITMAP hbmOld = static_cast<HBITMAP>(SelectObject(hdcMem, hbmColor));

    memset(pBits, 0, size * size * 4);
    SetBkMode(hdcMem, TRANSPARENT);
    SetTextColor(hdcMem, RGB(255, 255, 255));

    HFONT hFont = CreateFontW(18, fontWidth, 0, 0, fontWeight, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Arial");
    HFONT hOldFont = static_cast<HFONT>(SelectObject(hdcMem, hFont));

    RECT rc = {0, -1, size, size};
    wchar_t text[2] = { letter, L'\0' };
    DrawTextW(hdcMem, text, 1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

    SelectObject(hdcMem, hOldFont);
    DeleteObject(hFont);
    SelectObject(hdcMem, hbmOld);

    for (int i = 0; i < size * size; i++) {
        BYTE b = (BYTE)(pBits[i] & 0xFF);
        BYTE g = (BYTE)((pBits[i] >> 8) & 0xFF);
        BYTE r = (BYTE)((pBits[i] >> 16) & 0xFF);
        BYTE alpha = r;
        if (g > alpha) alpha = g;
        if (b > alpha) alpha = b;
        if (alpha > 0) {
            pBits[i] = ((DWORD)alpha << 24) | ((DWORD)alpha << 16) |
                        ((DWORD)alpha << 8)  | (DWORD)alpha;
        }
    }

    HBITMAP hbmMask = CreateBitmap(size, size, 1, 1, nullptr);
    HDC hdcMask = CreateCompatibleDC(hdcScreen);
    HBITMAP hbmMaskOld = static_cast<HBITMAP>(SelectObject(hdcMask, hbmMask));
    RECT rcMask = {0, 0, size, size};
    FillRect(hdcMask, &rcMask, (HBRUSH)GetStockObject(BLACK_BRUSH));
    SelectObject(hdcMask, hbmMaskOld);
    DeleteDC(hdcMask);

    DeleteDC(hdcMem);
    ReleaseDC(nullptr, hdcScreen);

    ICONINFO ii = {};
    ii.fIcon    = TRUE;
    ii.hbmMask  = hbmMask;
    ii.hbmColor = hbmColor;
    HICON hIcon = CreateIconIndirect(&ii);

    DeleteObject(hbmColor);
    DeleteObject(hbmMask);
    return hIcon;
}

// =============================================================================
// Tray Management
// =============================================================================

bool InitTrayIcon(HWND hWnd)
{
    if (!g_hIconV) g_hIconV = CreateLetterIcon(L'V', 8, FW_HEAVY);
    if (!g_hIconE) g_hIconE = CreateLetterIcon(L'E');

    ZeroMemory(&g_nid, sizeof(g_nid));
    g_nid.cbSize           = sizeof(NOTIFYICONDATAW);
    g_nid.hWnd             = hWnd;
    g_nid.uID              = IDC_TRAY;
    g_nid.uFlags           = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    g_nid.uCallbackMessage = WM_TRAYICON;
    g_nid.hIcon            = g_hIconV;

    StringCchCopyW(g_nid.szTip, ARRAYSIZE(g_nid.szTip), L"UniKey TSF - Ti\x1ebfng Vi\x1ec7t (V)");

    bool success = Shell_NotifyIconW(NIM_ADD, &g_nid) != FALSE;
    if (success) UpdateTrayIcon();
    return success;
}

void UpdateTrayIcon()
{
    if (!g_pConfig) return;

    bool isVietnamese = (g_pConfig->inputEnabled != 0);
    g_nid.hIcon = isVietnamese ? g_hIconV : g_hIconE;

    const wchar_t* methodName = L"Telex";
    if (g_pConfig->inputMethod == IM_VNI) methodName = L"VNI";
    else if (g_pConfig->inputMethod == IM_VIQR) methodName = L"VIQR";

    if (isVietnamese) {
        StringCchPrintfW(g_nid.szTip, ARRAYSIZE(g_nid.szTip),
            L"UniKey - Ti\x1ebfng Vi\x1ec7t - %s", methodName);
    } else {
        StringCchCopyW(g_nid.szTip, ARRAYSIZE(g_nid.szTip), L"UniKey - English (E)");
    }

    Shell_NotifyIconW(NIM_MODIFY, &g_nid);
}

void RemoveTrayIcon()
{
    Shell_NotifyIconW(NIM_DELETE, &g_nid);
}

void CleanupTrayResources()
{
    if (g_hIconV) DestroyIcon(g_hIconV);
    if (g_hIconE) DestroyIcon(g_hIconE);
    if (s_hMenuFont) DeleteObject(s_hMenuFont);
    g_hIconV = g_hIconE = nullptr;
    s_hMenuFont = nullptr;
}

void ToggleInputState()
{
    if (LockConfig()) {
        g_pConfig->inputEnabled = g_pConfig->inputEnabled ? 0 : 1;
        UnlockConfig();
    }
    UpdateTrayIcon();
}

// =============================================================================
// Context Menu
// =============================================================================

static MenuItemData* AllocMenuItem(const wchar_t* text, UINT id, bool checked = false, bool isSep = false, bool isSub = false) {
    if (s_menuItemCount >= 20) return &s_menuItems[0];
    MenuItemData* item = &s_menuItems[s_menuItemCount++];
    StringCchCopyW(item->text, 64, text ? text : L"");
    item->id = id;
    item->isChecked = checked;
    item->isSeparator = isSep;
    item->isSubmenu = isSub;
    return item;
}

void ShowContextMenu(HWND hWnd)
{
    if (!g_pConfig) return;

    s_menuItemCount = 0;

    if (!s_hMenuFont) {
        s_hMenuFont = CreateFontW(-18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS, L"Segoe UI");
    }

    HMENU hMenu = CreatePopupMenu();
    if (!hMenu) return;

    bool isViet = (g_pConfig->inputEnabled != 0);

    auto addItem = [&](HMENU hM, const wchar_t* text, UINT id, bool checked = false) {
        MenuItemData* item = AllocMenuItem(text, id, checked);
        AppendMenuW(hM, MF_OWNERDRAW, id, (LPCWSTR)item);
    };
    auto addSep = [&](HMENU hM) {
        MenuItemData* item = AllocMenuItem(L"", 0, false, true);
        AppendMenuW(hM, MF_OWNERDRAW | MF_SEPARATOR, 0, (LPCWSTR)item);
    };

    addItem(hMenu, L"  Ti\x1ebfng Vi\x1ec7t", IDM_TOGGLE_VE, isViet);
    addSep(hMenu);

    addItem(hMenu, L"  G\x00f5: Telex", IDM_INPUT_TELEX, g_pConfig->inputMethod == IM_TELEX);
    addItem(hMenu, L"  G\x00f5: VNI", IDM_INPUT_VNI, g_pConfig->inputMethod == IM_VNI);
    addItem(hMenu, L"  G\x00f5: VIQR", IDM_INPUT_VIQR, g_pConfig->inputMethod == IM_VIQR);
    addSep(hMenu);

    addItem(hMenu, L"  M\x00e3: Unicode", IDM_CHARSET_UNICODE, g_pConfig->charset == CS_UNICODE);
    addItem(hMenu, L"  M\x00e3: TCVN3 (ABC)", IDM_CHARSET_TCVN3, g_pConfig->charset == CS_TCVN3);
    addItem(hMenu, L"  M\x00e3: VNI Windows", IDM_CHARSET_VNI, g_pConfig->charset == CS_VNI_WIN);

    addSep(hMenu);
    addItem(hMenu, L"  B\x1ea3ng \x0111i\x1ec3u khi\x1ec3n...", IDM_SETTINGS);
    addSep(hMenu);
    addItem(hMenu, L"  Tho\x00e1t", IDM_EXIT);

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hWnd);

    MENUINFO mi = { sizeof(mi) };
    mi.fMask = MIM_BACKGROUND | MIM_APPLYTOSUBMENUS;
    mi.hbrBack = CreateSolidBrush(MENU_BG);
    SetMenuInfo(hMenu, &mi);

    TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hWnd, nullptr);
    PostMessage(hWnd, WM_NULL, 0, 0);

    DeleteObject(mi.hbrBack);
    DestroyMenu(hMenu);
}

void OnMeasureItem(HWND hWnd, MEASUREITEMSTRUCT* mis)
{
    if (mis->CtlType == ODT_MENU) {
        MenuItemData* item = (MenuItemData*)mis->itemData;
        if (item && item->isSeparator) {
            mis->itemHeight = MENU_SEP_HEIGHT;
            mis->itemWidth = 180;
        } else {
            mis->itemHeight = MENU_ITEM_HEIGHT;
            if (item && s_hMenuFont) {
                HDC hdc = GetDC(hWnd);
                HFONT hOld = (HFONT)SelectObject(hdc, s_hMenuFont);
                SIZE sz;
                GetTextExtentPoint32W(hdc, item->text, (int)wcslen(item->text), &sz);
                mis->itemWidth = sz.cx + MENU_ITEM_PADDING;
                SelectObject(hdc, hOld);
                ReleaseDC(hWnd, hdc);
            } else {
                mis->itemWidth = 200;
            }
        }
    }
}

void OnDrawItem(HWND hWnd, DRAWITEMSTRUCT* dis)
{
    if (dis->CtlType == ODT_MENU) {
        MenuItemData* item = (MenuItemData*)dis->itemData;
        if (!item) return;

        HDC hdc = dis->hDC;
        RECT rc = dis->rcItem;
        bool isSelected = (dis->itemState & ODS_SELECTED) != 0;

        HBRUSH hBr = CreateSolidBrush(isSelected ? MENU_BG_HOVER : MENU_BG);
        FillRect(hdc, &rc, hBr);
        DeleteObject(hBr);

        if (item->isSeparator) {
            int y = (rc.top + rc.bottom) / 2;
            HPEN hPen = CreatePen(PS_SOLID, 1, MENU_SEPARATOR);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
            MoveToEx(hdc, rc.left + 8, y, nullptr);
            LineTo(hdc, rc.right - 8, y);
            SelectObject(hdc, hOldPen);
            DeleteObject(hPen);
        } else {
            SetBkMode(hdc, TRANSPARENT);
            HFONT hOldFont = s_hMenuFont ? (HFONT)SelectObject(hdc, s_hMenuFont) : nullptr;

            SetTextColor(hdc, isSelected ? RGB(255, 255, 255) : MENU_TEXT);
            RECT rcText = rc;
            rcText.left += 6;
            DrawTextW(hdc, item->text, -1, &rcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

            if (item->isSubmenu) {
                SetTextColor(hdc, RGB(255, 255, 255));
                RECT rcArrow = rc;
                rcArrow.right -= 8;
                DrawTextW(hdc, L"\x25B8", 1, &rcArrow, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
            }

            if (item->isChecked) {
                SetTextColor(hdc, RGB(255, 255, 255));
                RECT rcCheck = rc;
                rcCheck.right -= 8;
                DrawTextW(hdc, L"\x2713", 1, &rcCheck, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);
            }

            if (hOldFont) SelectObject(hdc, hOldFont);
        }
    }
}
