#include "hook_manager.h"
#include <vector>
#include <string>
#include "vn_engine.h"
#include "macro.h"
#include "../config/blacklist.h"
#include "../config/ipc_manager.h"
#include "../ui/tray_icon.h"
#include "../toolkit/clipboard.h"
#include "../resource.h"

// --- Globals (Internal) ---
static HHOOK        g_hKeyboardHook = nullptr;
static HHOOK        g_hMouseHook    = nullptr;
static VnEngine     g_engine;
static MacroEngine  g_macroEngine;
static bool         g_bInjecting   = false;
static int          g_rawCharCount = 0;
static std::wstring g_lastComposition = L"";
static std::wstring g_lastCommittedWord = L"";  // Reconversion cache: last word before space
static bool         g_bBlacklisted = false;
static HWND         g_hLastForeground = nullptr;
static bool         g_suppressedKeys[256] = { false };

// --- Toggle hotkey detection ---
static bool g_bCtrlDown  = false;
static bool g_bShiftDown = false;
static bool g_bAltDown   = false;
static bool g_bOtherKeyPressed = false;

// --- Window handle of the main app (for PostMessage) ---
static HWND g_hMainWnd = nullptr;

// =============================================================================
// Atomic Text Replacement
// =============================================================================

static void ReplaceText(const std::wstring& oldText, const std::wstring& newText)
{
    if (oldText.empty() && newText.empty()) return;

    size_t commonLen = 0;
    while (commonLen < oldText.length() && commonLen < newText.length() && 
           oldText[commonLen] == newText[commonLen]) {
        commonLen++;
    }

    size_t charsToReplace = oldText.length() - commonLen;
    std::wstring toType = newText.substr(commonLen);

    if (charsToReplace == 0 && toType.empty()) return;

    size_t backspaceEvents = charsToReplace * 2;
    size_t unicodeEvents = toType.size() * 2;
    size_t totalEvents = backspaceEvents + unicodeEvents;

    std::vector<INPUT> inputs(totalEvents);
    size_t idx = 0;

    for (size_t i = 0; i < charsToReplace; i++) {
        inputs[idx] = {};
        inputs[idx].type = INPUT_KEYBOARD;
        inputs[idx].ki.wVk = VK_BACK;
        inputs[idx].ki.dwFlags = 0;
        idx++;
        inputs[idx] = {};
        inputs[idx].type = INPUT_KEYBOARD;
        inputs[idx].ki.wVk = VK_BACK;
        inputs[idx].ki.dwFlags = KEYEVENTF_KEYUP;
        idx++;
    }

    for (size_t i = 0; i < toType.size(); i++) {
        inputs[idx] = {};
        inputs[idx].type = INPUT_KEYBOARD;
        inputs[idx].ki.wScan = toType[i];
        inputs[idx].ki.dwFlags = KEYEVENTF_UNICODE;
        idx++;
        inputs[idx] = {};
        inputs[idx].type = INPUT_KEYBOARD;
        inputs[idx].ki.wScan = toType[i];
        inputs[idx].ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
        idx++;
    }

    g_bInjecting = true;
    SendInput((UINT)inputs.size(), inputs.data(), sizeof(INPUT));
    g_bInjecting = false;
}

static wchar_t VkToChar(UINT vk)
{
    if (vk >= 'A' && vk <= 'Z') {
        bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        bool caps  = (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
        bool upper = shift ^ caps;
        return upper ? (wchar_t)vk : (wchar_t)(vk + 32);
    }
    if (vk >= '0' && vk <= '9') {
        bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
        if (!shift) return (wchar_t)vk;
        return 0;
    }
    if (vk == VK_OEM_PERIOD) return L'.';
    if (vk == VK_OEM_COMMA) return L',';
    if (vk == VK_OEM_1) return L';';
    if (vk == VK_OEM_2) return L'/';
    if (vk == VK_OEM_7) return L'\'';
    if (vk == VK_OEM_4) return L'[';
    if (vk == VK_OEM_6) return L']';
    if (vk == VK_OEM_MINUS) return L'-';
    if (vk == VK_OEM_PLUS) return L'=';
    if (vk == VK_SPACE) return L' ';
    if (vk == VK_RETURN) return L'\r';
    if (vk == VK_TAB) return L'\t';
    return 0;
}

// =============================================================================
// Hook Procedures
// =============================================================================

static LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode != HC_ACTION)
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    KBDLLHOOKSTRUCT* pKb = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    if (g_bInjecting || (pKb->flags & LLKHF_INJECTED))
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    UINT vk = pKb->vkCode;
    if (vk > 255) return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    bool isKeyDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);
    bool isKeyUp   = (wParam == WM_KEYUP   || wParam == WM_SYSKEYUP);

    uint8_t toggleMode = g_pConfig ? g_pConfig->toggleKey : TK_CTRL_SHIFT;

    if (toggleMode == TK_CTRL_SHIFT) {
        if (vk == VK_LCONTROL || vk == VK_RCONTROL || vk == VK_CONTROL) {
            if (isKeyDown) { g_bCtrlDown = true; }
            if (isKeyUp) {
                if (g_bCtrlDown && g_bShiftDown && !g_bOtherKeyPressed) {
                    if (g_hMainWnd) PostMessage(g_hMainWnd, WM_COMMAND, IDM_TOGGLE_VE, 0);
                }
                g_bCtrlDown = false;
                g_bOtherKeyPressed = false;
            }
            return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
        }
        if (vk == VK_LSHIFT || vk == VK_RSHIFT || vk == VK_SHIFT) {
            if (isKeyDown) { g_bShiftDown = true; }
            if (isKeyUp) {
                if (g_bCtrlDown && g_bShiftDown && !g_bOtherKeyPressed) {
                    if (g_hMainWnd) PostMessage(g_hMainWnd, WM_COMMAND, IDM_TOGGLE_VE, 0);
                }
                g_bShiftDown = false;
                g_bOtherKeyPressed = false;
            }
            return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
        }
        if (isKeyDown && (g_bCtrlDown || g_bShiftDown)) {
            g_bOtherKeyPressed = true;
        }
    } else if (toggleMode == TK_ALT_Z) {
        if (vk == VK_LMENU || vk == VK_RMENU || vk == VK_MENU) {
            if (isKeyDown) { g_bAltDown = true; }
            if (isKeyUp)   { g_bAltDown = false; }
            return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
        }
        if (vk == 'Z' && isKeyDown && g_bAltDown) {
            if (g_hMainWnd) PostMessage(g_hMainWnd, WM_COMMAND, IDM_TOGGLE_VE, 0);
            return 1;
        }
    }

    if (isKeyUp) {
        if (g_suppressedKeys[vk]) {
            g_suppressedKeys[vk] = false;
            return 1;
        }
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }
    g_suppressedKeys[vk] = false;

    {
        HWND hFgNow = GetForegroundWindow();
        if (hFgNow != g_hLastForeground) {
            g_hLastForeground = hFgNow;
            g_engine.Clear();
            g_rawCharCount = 0;
            g_lastComposition.clear();
            g_lastCommittedWord.clear();  // Window changed, invalidate reconversion cache
            
            // Query blacklist only when window changes
            g_bBlacklisted = false; // Default to false if we can't determine
            if (hFgNow) {
                DWORD pid = 0;
                GetWindowThreadProcessId(hFgNow, &pid);
                if (pid) {
                    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
                    if (hProc) {
                        wchar_t exePath[MAX_PATH] = {};
                        DWORD sz = MAX_PATH;
                        if (QueryFullProcessImageNameW(hProc, 0, exePath, &sz)) {
                            wchar_t* name = wcsrchr(exePath, L'\\');
                            g_bBlacklisted = IsProcessBlacklisted(name ? name + 1 : exePath);
                        }
                        CloseHandle(hProc);
                    }
                }
            }
        }
    }
    if (g_bBlacklisted)
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(VK_MENU) & 0x8000)) {
        g_engine.Clear();
        g_rawCharCount = 0;
        g_lastComposition.clear();
        g_lastCommittedWord.clear();  // Ctrl/Alt combo, invalidate reconversion cache
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    if (vk == VK_BACK) {
        if (g_engine.IsInWord()) {
            g_engine.RemoveLastChar();
            if (g_rawCharCount > 0) g_rawCharCount--;
            if (!g_lastComposition.empty()) g_lastComposition.pop_back();
        }
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    if (vk == VK_LEFT || vk == VK_RIGHT || vk == VK_UP || vk == VK_DOWN ||
        vk == VK_HOME || vk == VK_END || vk == VK_DELETE ||
        vk == VK_PRIOR || vk == VK_NEXT || vk == VK_ESCAPE ||
        (vk >= VK_F1 && vk <= VK_F24)) {
        g_engine.Clear();
        g_rawCharCount = 0;
        g_lastComposition.clear();
        // Don't clear g_lastCommittedWord on Left/Right — user may be moving back to edit
        if (vk != VK_LEFT && vk != VK_RIGHT) {
            g_lastCommittedWord.clear();
        }
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    wchar_t ch = VkToChar(vk);
    if (ch == 0)
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    int oldRawCount = g_rawCharCount;

    // Reconversion: if engine is empty and key is a potential modifier, re-feed the last committed word
    if (!g_engine.IsInWord() && !g_lastCommittedWord.empty() &&
        g_engine.IsPotentialModifier(ch, (InputMethod)g_pConfig->inputMethod)) {
        std::wstring wordToRestore = g_lastCommittedWord;
        g_engine.Clear();
        for (wchar_t c : wordToRestore) {
            g_engine.ProcessKey(c, (InputMethod)g_pConfig->inputMethod);
        }
        g_rawCharCount = (int)g_engine.GetCompositionString().length();
        g_lastComposition = g_engine.GetCompositionString();
    }

    if (g_engine.ProcessKey(ch, (InputMethod)g_pConfig->inputMethod)) {
        std::wstring newComposition = g_engine.GetCompositionString();
        if (g_engine.DidTransform()) {
            ReplaceText(g_lastComposition, newComposition);
            g_rawCharCount = (int)newComposition.length();
            g_suppressedKeys[vk] = true;
            g_lastComposition = newComposition;
            return 1;
        } else {
            g_rawCharCount++;
            g_lastComposition = newComposition;
            return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
        }
    } else {
        if (g_pConfig && g_pConfig->macroEnabled && oldRawCount > 0) {
            std::wstring lastWord = g_lastComposition;
            if (!lastWord.empty()) {
                std::wstring expanded = g_macroEngine.Expand(lastWord);
                if (!expanded.empty()) {
                    ReplaceText(g_lastComposition, expanded);
                    g_rawCharCount = 0;
                    g_engine.Clear();
                    g_lastComposition.clear();
                    g_lastCommittedWord.clear();
                    return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
                }
            }
        }
        // Save the committed word for reconversion before clearing
        if (!g_lastComposition.empty()) {
            g_lastCommittedWord = g_engine.GetRawString();
        }
        g_rawCharCount = 0;
        g_lastComposition.clear();
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }
}

static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION) {
        if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN || wParam == WM_MBUTTONDOWN) {
            g_engine.Clear();
            g_rawCharCount = 0;
            g_lastComposition.clear();
            g_lastCommittedWord.clear();  // Mouse click, invalidate reconversion cache
        }
    }
    return CallNextHookEx(g_hMouseHook, nCode, wParam, lParam);
}

// =============================================================================
// Public Functions
// =============================================================================

bool InstallHooks(HINSTANCE hInstance, HWND hWnd)
{
    g_hMainWnd = hWnd;
    g_hKeyboardHook = SetWindowsHookExW(WH_KEYBOARD_LL, LowLevelKeyboardProc, hInstance, 0);
    g_hMouseHook = SetWindowsHookExW(WH_MOUSE_LL, LowLevelMouseProc, hInstance, 0);

    // Load macros if enabled
    if (g_pConfig && g_pConfig->macroEnabled && g_pConfig->macroFilePath[0] != L'\0') {
        g_macroEngine.Load(g_pConfig->macroFilePath);
    }

    return g_hKeyboardHook != nullptr;
}

void UninstallHooks()
{
    if (g_hKeyboardHook) {
        UnhookWindowsHookEx(g_hKeyboardHook);
        g_hKeyboardHook = nullptr;
    }
    if (g_hMouseHook) {
        UnhookWindowsHookEx(g_hMouseHook);
        g_hMouseHook = nullptr;
    }
}

void HandleHotkey(WPARAM wParam)
{
    if (wParam == IDH_TOOLKIT_CONVERT) {
        if (g_pConfig) {
            Toolkit::ConvertSelectedTextCharset(CS_UNICODE, (Charset)g_pConfig->charset);
        }
    } else if (wParam == IDH_TOOLKIT_STRIP) {
        Toolkit::StripSelectedTextAccents();
    }
}

void ResetEngine()
{
    g_engine.Clear();
    g_rawCharCount = 0;
    g_lastComposition.clear();
    g_lastCommittedWord.clear();
}
