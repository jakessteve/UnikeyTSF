#include "hook_manager.h"
#include "input_routing.h"
#include "per_app_input_state.h"
#include <vector>
#include <string>
#include <array>
#include "vn_engine.h"
#include "macro.h"
#include "delimiter_policy.h"
#include "key_translate.h"
#include "typing_settings.h"
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
static ULONGLONG    g_lastCommittedTick = 0;
static wchar_t      g_lastCommitDelimiter = 0;
static bool         g_bBlacklisted = false;
static HWND         g_hLastForeground = nullptr;
static std::wstring g_currentAppId;
static bool         g_suppressedKeys[256] = { false };
static InputRoutingOwner g_lastRoutingOwner = ROUTING_OWNER_HOOK;
static UniKeyConfig g_lastAppliedConfig = {};
static bool g_hasLastAppliedConfig = false;

// --- Toggle hotkey detection ---
static bool g_bCtrlDown  = false;
static bool g_bShiftDown = false;
static bool g_bAltDown   = false;
static bool g_bOtherKeyPressed = false;

// --- Window handle of the main app (for PostMessage) ---
static HWND g_hMainWnd = nullptr;
static constexpr ULONGLONG kRestoreReplayTimeoutMs = 2000;

static void TraceRoutingDecision(const RoutingDecision& decision)
{
    wchar_t buffer[192] = {};
    wsprintfW(buffer,
        L"[UniKeyTSF][Routing][Hook] mode=%s owner=%s reason=%s seq=%lu\n",
        InputRoutingModeToString(decision.mode),
        InputRoutingOwnerToString(decision.owner),
        InputRoutingReasonToString(decision.reason),
        static_cast<unsigned long>(decision.decisionSequence));
    OutputDebugStringW(buffer);
}

static void ClearHookEngineState()
{
    g_engine.Clear();
    g_rawCharCount = 0;
    g_lastComposition.clear();
    g_lastCommittedWord.clear();
    g_lastCommittedTick = 0;
    g_lastCommitDelimiter = 0;
}

static void RefreshHookTypingSettingsFromConfig()
{
    if (!g_pConfig) {
        return;
    }

    ApplyTypingSettingsToEngine(*g_pConfig, g_engine);

    const MacroConfigDelta macroDelta = EvaluateMacroConfigDelta(
        g_hasLastAppliedConfig ? &g_lastAppliedConfig : nullptr,
        *g_pConfig);
    if (macroDelta.shouldClear) {
        g_macroEngine.Clear();
    }
    if (macroDelta.shouldReload) {
        g_macroEngine.Load(macroDelta.macroFilePath);
    }

    g_lastAppliedConfig = *g_pConfig;
    g_hasLastAppliedConfig = true;
}

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

    if (g_pConfig && g_pConfig->useClipboardForUnicode) {
        size_t backspaceEvents = charsToReplace * 2;
        std::vector<INPUT> inputs(backspaceEvents);
        size_t idx = 0;
        for (size_t i = 0; i < charsToReplace; i++) {
            inputs[idx] = {};
            inputs[idx].type = INPUT_KEYBOARD;
            inputs[idx].ki.wVk = VK_BACK;
            idx++;
            inputs[idx] = {};
            inputs[idx].type = INPUT_KEYBOARD;
            inputs[idx].ki.wVk = VK_BACK;
            inputs[idx].ki.dwFlags = KEYEVENTF_KEYUP;
            idx++;
        }
        if (backspaceEvents > 0) {
            g_bInjecting = true;
            SendInput((UINT)inputs.size(), inputs.data(), sizeof(INPUT));
            g_bInjecting = false;
        }

        if (!toType.empty()) {
            Toolkit::ModifyClipboardText(toType);
            
            INPUT vInputs[4] = {};
            vInputs[0].type = INPUT_KEYBOARD; vInputs[0].ki.wVk = VK_CONTROL;
            vInputs[1].type = INPUT_KEYBOARD; vInputs[1].ki.wVk = 'V';
            vInputs[2].type = INPUT_KEYBOARD; vInputs[2].ki.wVk = 'V'; vInputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
            vInputs[3].type = INPUT_KEYBOARD; vInputs[3].ki.wVk = VK_CONTROL; vInputs[3].ki.dwFlags = KEYEVENTF_KEYUP;
            
            g_bInjecting = true;
            SendInput(4, vInputs, sizeof(INPUT));
            g_bInjecting = false;
        }
    } else {
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
}

static wchar_t VkToChar(const KBDLLHOOKSTRUCT& keyInfo)
{
    BYTE keyboardState[256] = {};
    if (!GetKeyboardState(keyboardState)) {
        return 0;
    }

    const auto applyPressedState = [&](int vkCode) {
        if ((GetAsyncKeyState(vkCode) & 0x8000) != 0) {
            keyboardState[vkCode] |= 0x80;
        } else {
            keyboardState[vkCode] &= static_cast<BYTE>(~0x80);
        }
    };

    applyPressedState(VK_SHIFT);
    applyPressedState(VK_LSHIFT);
    applyPressedState(VK_RSHIFT);
    applyPressedState(VK_CONTROL);
    applyPressedState(VK_LCONTROL);
    applyPressedState(VK_RCONTROL);
    applyPressedState(VK_MENU);
    applyPressedState(VK_LMENU);
    applyPressedState(VK_RMENU);

    if (keyInfo.vkCode < 256) {
        keyboardState[keyInfo.vkCode] |= 0x80;
    }

    HKL layout = GetKeyboardLayout(0);
    if (g_hLastForeground) {
        DWORD threadId = GetWindowThreadProcessId(g_hLastForeground, nullptr);
        if (threadId != 0) {
            layout = GetKeyboardLayout(threadId);
        }
    }

    const wchar_t translated = TranslateVkToWchar(keyInfo.vkCode, keyInfo.scanCode, keyboardState, layout);
    if (translated != 0) {
        return translated;
    }

    if (keyInfo.vkCode == VK_SPACE) return L' ';
    if (keyInfo.vkCode == VK_RETURN) return L'\r';
    if (keyInfo.vkCode == VK_TAB) return L'\t';
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
                // Only fire toggle if ALL control keys are released
                if (!(GetAsyncKeyState(VK_LCONTROL) & 0x8000) &&
                    !(GetAsyncKeyState(VK_RCONTROL) & 0x8000)) {
                    if (g_bCtrlDown && g_bShiftDown && !g_bOtherKeyPressed) {
                        if (g_hMainWnd) PostMessage(g_hMainWnd, WM_COMMAND, IDM_TOGGLE_VE, 0);
                    }
                    g_bCtrlDown = false;
                    g_bOtherKeyPressed = false;
                }
            }
            return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
        }
        if (vk == VK_LSHIFT || vk == VK_RSHIFT || vk == VK_SHIFT) {
            if (isKeyDown) { g_bShiftDown = true; }
            if (isKeyUp) {
                // Only fire toggle if ALL shift keys are released
                if (!(GetAsyncKeyState(VK_LSHIFT) & 0x8000) &&
                    !(GetAsyncKeyState(VK_RSHIFT) & 0x8000)) {
                    if (g_bCtrlDown && g_bShiftDown && !g_bOtherKeyPressed) {
                        if (g_hMainWnd) PostMessage(g_hMainWnd, WM_COMMAND, IDM_TOGGLE_VE, 0);
                    }
                    g_bShiftDown = false;
                    g_bOtherKeyPressed = false;
                }
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
            g_currentAppId = NormalizeAppId(ResolveForegroundAppId());
            g_engine.Clear();
            g_rawCharCount = 0;
            g_lastComposition.clear();
            g_lastCommittedWord.clear();  // Window changed, invalidate reconversion cache
            g_lastCommittedTick = 0;
            g_lastCommitDelimiter = 0;
            
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
                            g_bBlacklisted = IsProcessBlacklisted(NormalizeAppId(exePath));
                        }
                        CloseHandle(hProc);
                    }
                }
            }
        }
    }
    if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) || (GetAsyncKeyState(VK_MENU) & 0x8000)) {
        ClearHookEngineState();
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    RoutingDecision routingDecision = RefreshHookRoutingDecision(g_hLastForeground != nullptr, g_bBlacklisted);
    if (routingDecision.owner != g_lastRoutingOwner || routingDecision.reason != ROUTING_REASON_HOOK_PRIMARY_DEFAULT) {
        TraceRoutingDecision(routingDecision);
    }
    g_lastRoutingOwner = routingDecision.owner;

    if (routingDecision.owner != ROUTING_OWNER_HOOK) {
        ClearHookEngineState();
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    RefreshHookTypingSettingsFromConfig();

    if (g_pConfig && ResolveEffectiveInputEnabled(*g_pConfig, g_currentAppId) == 0) {
        ClearHookEngineState();
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    if (vk == VK_BACK) {
        if (g_engine.IsInWord()) {
            const std::wstring oldComposition = g_lastComposition;
            g_engine.RemoveLastChar();
            const std::wstring newComposition = g_engine.GetCompositionString();
            ReplaceText(oldComposition, newComposition);
            g_rawCharCount = static_cast<int>(newComposition.length());
            g_lastComposition = newComposition;
            g_lastCommittedWord.clear();
            g_lastCommittedTick = 0;
            g_lastCommitDelimiter = 0;
            g_suppressedKeys[vk] = true;
            return 1;
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
            g_lastCommittedTick = 0;
            g_lastCommitDelimiter = 0;
        }
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
    }

    wchar_t ch = VkToChar(*pKb);
    if (ch == 0)
        return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

    const InputMethod inputMethod = g_pConfig
        ? static_cast<InputMethod>(g_pConfig->inputMethod)
        : IM_VNI;

    int oldRawCount = g_rawCharCount;

    if (!g_lastCommittedWord.empty() &&
        (g_lastCommittedTick == 0 || (GetTickCount64() - g_lastCommittedTick) > kRestoreReplayTimeoutMs)) {
        g_lastCommittedWord.clear();
        g_lastCommittedTick = 0;
        g_lastCommitDelimiter = 0;
    }

    // Reconversion: if engine is empty and key is a recent post-space modifier, re-feed the last committed word
    if (!g_engine.IsInWord() && !g_lastCommittedWord.empty() &&
        (!g_pConfig || g_pConfig->restoreKeyEnabled) &&
        InputDelimiters::IsReplayEligibleDelimiter(g_lastCommitDelimiter) &&
        g_engine.IsPotentialModifier(ch, inputMethod)) {
        g_engine.FeedContext(g_lastCommittedWord, inputMethod);
        g_rawCharCount = (int)g_engine.GetCompositionString().length();
        g_lastComposition = g_engine.GetCompositionString();
    }

    if (g_engine.ProcessKey(ch, inputMethod)) {
        std::wstring newComposition = g_engine.GetCompositionString();
        g_lastCommittedWord.clear();
        g_lastCommittedTick = 0;
        g_lastCommitDelimiter = 0;
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
                    g_lastCommittedTick = 0;
                    g_lastCommitDelimiter = 0;
                    return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
                }
            }
        }
        // Save the committed word for reconversion before clearing
        if (!g_lastComposition.empty() && InputDelimiters::IsReplayEligibleDelimiter(ch)) {
            g_lastCommittedWord = g_engine.GetRawString();
            g_lastCommittedTick = GetTickCount64();
            g_lastCommitDelimiter = ch;
        } else {
            g_lastCommittedWord.clear();
            g_lastCommittedTick = 0;
            g_lastCommitDelimiter = 0;
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
            g_lastCommittedTick = 0;
            g_lastCommitDelimiter = 0;
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

    g_hasLastAppliedConfig = false;
    if (g_pConfig) {
        RefreshHookTypingSettingsFromConfig();
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

    g_hasLastAppliedConfig = false;
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
    ClearHookEngineState();
}
