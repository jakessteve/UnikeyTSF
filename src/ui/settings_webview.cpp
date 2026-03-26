#include "settings_webview.h"
#include "../config/ipc_manager.h"
#include "../config/blacklist.h"
#include <wrl/event.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace Microsoft::WRL;

// Static cached environment
ComPtr<ICoreWebView2Environment> SettingsWebView::s_cachedEnvironment;

SettingsWebView::SettingsWebView()
    : _parentHwnd(nullptr), _webMessageToken() {
}

SettingsWebView::~SettingsWebView() {
    if (_webview) {
        _webview->remove_WebMessageReceived(_webMessageToken);
    }
}

// =============================================================================
// Pre-initialize environment at app startup (async, non-blocking)
// =============================================================================
void SettingsWebView::PreInitEnvironment() {
    if (s_cachedEnvironment) return;  // Already cached
    CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (SUCCEEDED(result) && env) {
                    s_cachedEnvironment = env;
                }
                return S_OK;
            }).Get());
}

HRESULT SettingsWebView::Initialize(HWND parentHwnd) {
    _parentHwnd = parentHwnd;

    // Helper lambda to create controller from an existing environment
    auto createController = [this](ICoreWebView2Environment* env) -> HRESULT {
        return env->CreateCoreWebView2Controller(
                    _parentHwnd,
                    Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
                        [this](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
                            if (FAILED(result)) return result;
                            _controller = controller;

                            // 3. Get the WebView2 object
                            _controller->get_CoreWebView2(&_webview);

                            // 4. Set initial size
                            RECT bounds;
                            GetClientRect(_parentHwnd, &bounds);
                            _controller->put_Bounds(bounds);

                            // 5. Setup message handler
                            _webview->add_WebMessageReceived(
                                Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                                    this, &SettingsWebView::OnWebMessageReceived).Get(),
                                &_webMessageToken);

                            // 5b. Notify parent window when navigation completes
                            EventRegistrationToken navToken;
                            HWND hwndParent = _parentHwnd;
                            _webview->add_NavigationCompleted(
                                Callback<ICoreWebView2NavigationCompletedEventHandler>(
                                    [hwndParent](ICoreWebView2* /*sender*/,
                                        ICoreWebView2NavigationCompletedEventArgs* /*args*/) -> HRESULT {
                                        PostMessageW(hwndParent, WM_APP + 1, 0, 0);
                                        return S_OK;
                                    }).Get(),
                                &navToken);

                            // 6. Map virtual host to the frontend folder
                            wchar_t exePath[MAX_PATH];
                            GetModuleFileNameW(nullptr, exePath, MAX_PATH);
                            std::wstring exeDir(exePath);
                            size_t pos = exeDir.find_last_of(L"\\/");
                            if (pos != std::wstring::npos) {
                                exeDir = exeDir.substr(0, pos);
                            }
                            std::wstring frontendDir = exeDir + L"\\frontend";

                            // Use ICoreWebView2_3 for virtual host mapping
                            Microsoft::WRL::ComPtr<ICoreWebView2_3> webview3;
                            HRESULT hr3 = _webview->QueryInterface(IID_PPV_ARGS(&webview3));
                            if (SUCCEEDED(hr3) && webview3) {
                                webview3->SetVirtualHostNameToFolderMapping(
                                    L"unikey.local",
                                    frontendDir.c_str(),
                                    COREWEBVIEW2_HOST_RESOURCE_ACCESS_KIND_ALLOW);
                                _webview->Navigate(L"https://unikey.local/index.html");
                            } else {
                                // Fallback: file URI with forward slashes
                                std::wstring filePath = frontendDir + L"\\index.html";
                                for (auto& ch : filePath) {
                                    if (ch == L'\\') ch = L'/';
                                }
                                _webview->Navigate((L"file:///" + filePath).c_str());
                            }

                            return S_OK;
                        }).Get());
    };

    // Use cached environment if available (fast path)
    if (s_cachedEnvironment) {
        return createController(s_cachedEnvironment.Get());
    }

    // Slow path: create environment first
    return CreateCoreWebView2EnvironmentWithOptions(
        nullptr, nullptr, nullptr,
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this, createController](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) return result;
                s_cachedEnvironment = env;
                return createController(env);
            }).Get());
}

// =============================================================================
// Auto-start Registry Helpers
// =============================================================================

static const wchar_t* AUTOSTART_REG_KEY = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
static const wchar_t* AUTOSTART_VALUE_NAME = L"UniKeyTSF";

static bool IsAutoStartEnabled() {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, AUTOSTART_REG_KEY, 0, KEY_READ, &hKey) != ERROR_SUCCESS)
        return false;
    DWORD type = 0, size = 0;
    bool exists = (RegQueryValueExW(hKey, AUTOSTART_VALUE_NAME, nullptr, &type, nullptr, &size) == ERROR_SUCCESS);
    RegCloseKey(hKey);
    return exists;
}

static void SetAutoStart(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, AUTOSTART_REG_KEY, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return;
    if (enable) {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        std::wstring quotedPath = L"\"";
        quotedPath += exePath;
        quotedPath += L"\"";
        RegSetValueExW(hKey, AUTOSTART_VALUE_NAME, 0, REG_SZ,
            (const BYTE*)quotedPath.c_str(), (DWORD)((quotedPath.length() + 1) * sizeof(wchar_t)));
    } else {
        RegDeleteValueW(hKey, AUTOSTART_VALUE_NAME);
    }
    RegCloseKey(hKey);
}

void SettingsWebView::Resize(const RECT& bounds) {
    if (_controller) {
        _controller->put_Bounds(bounds);
    }
}

// =============================================================================
// Simple JSON helpers (no external lib)
// =============================================================================

static bool jsonExtractBool(const std::wstring& json, const wchar_t* key, bool def = false) {
    std::wstring needle = std::wstring(L"\"" ) + key + L"\":";
    size_t pos = json.find(needle);
    if (pos == std::wstring::npos) return def;
    pos += needle.length();
    while (pos < json.length() && json[pos] == L' ') pos++;
    if (pos + 4 <= json.length() && json.substr(pos, 4) == L"true") return true;
    return false;
}

static int jsonExtractInt(const std::wstring& json, const wchar_t* key, int def = 0) {
    std::wstring needle = std::wstring(L"\"" ) + key + L"\":";
    size_t pos = json.find(needle);
    if (pos == std::wstring::npos) return def;
    pos += needle.length();
    while (pos < json.length() && json[pos] == L' ') pos++;
    return _wtoi(json.c_str() + pos);
}

static std::wstring jsonExtractString(const std::wstring& json, const wchar_t* key) {
    std::wstring needle = std::wstring(L"\"" ) + key + L"\":\"";
    size_t pos = json.find(needle);
    if (pos == std::wstring::npos) return L"";
    pos += needle.length();
    std::wstring result;
    while (pos < json.length() && json[pos] != L'"') {
        if (json[pos] == L'\\' && pos + 1 < json.length()) {
            result += json[pos + 1];
            pos += 2;
        } else {
            result += json[pos];
            pos++;
        }
    }
    return result;
}

HRESULT SettingsWebView::PostConfigMessage(const UniKeyConfig& config) {
    if (!_webview) return E_FAIL;

    // Wrap in {"type":"SET_CONFIG","payload":{...}} envelope
    std::wstring json = L"{\"type\":\"SET_CONFIG\",\"payload\":";
    json += SerializeConfigToJson(config);
    json += L"}";
    return _webview->PostWebMessageAsJson(json.c_str());
}

HRESULT SettingsWebView::OnWebMessageReceived(
    ICoreWebView2* sender,
    ICoreWebView2WebMessageReceivedEventArgs* args) {
    
    LPWSTR messageRaw = nullptr;
    HRESULT hr = args->TryGetWebMessageAsString(&messageRaw);
    if (FAILED(hr)) return hr;

    std::wstring message(messageRaw);
    CoTaskMemFree(messageRaw);

    if (message.find(L"\"type\":\"UPDATE_CONFIG\"") != std::wstring::npos) {
        HANDLE hMutex = OpenMutexW(SYNCHRONIZE, FALSE, UNIKEY_SHARED_MUTEX_NAME);
        if (hMutex) {
            WaitForSingleObject(hMutex, INFINITE);
            if (g_pConfig) {
                g_pConfig->inputEnabled   = jsonExtractBool(message, L"inputEnabled") ? 1 : 0;
                g_pConfig->inputMethod    = (uint8_t)jsonExtractInt(message, L"inputMethod");
                g_pConfig->charset        = (uint8_t)jsonExtractInt(message, L"charset");
                g_pConfig->toneType       = (uint8_t)jsonExtractInt(message, L"toneType");
                g_pConfig->spellCheck     = jsonExtractBool(message, L"spellCheck") ? 1 : 0;
                g_pConfig->macroEnabled   = jsonExtractBool(message, L"macroEnabled") ? 1 : 0;
                g_pConfig->freeToneMarking= jsonExtractBool(message, L"freeToneMarking") ? 1 : 0;
                g_pConfig->toggleKey      = (uint8_t)jsonExtractInt(message, L"toggleKey");
                g_pConfig->restoreKeyEnabled = jsonExtractBool(message, L"restoreKeyEnabled") ? 1 : 0;
                g_pConfig->useClipboardForUnicode = jsonExtractBool(message, L"useClipboardForUnicode") ? 1 : 0;
                g_pConfig->showDialogOnStartup = jsonExtractBool(message, L"showDialogOnStartup") ? 1 : 0;
                g_pConfig->perAppInputState = jsonExtractBool(message, L"perAppInputState") ? 1 : 0;

                std::wstring macroPath = jsonExtractString(message, L"macroFilePath");
                wcsncpy_s(g_pConfig->macroFilePath, macroPath.c_str(), 259);

                SaveConfigToFile(g_pConfig);
            }
            ReleaseMutex(hMutex);
            CloseHandle(hMutex);
        }
    }
    else if (message.find(L"\"type\":\"UPDATE_BLACKLIST\"") != std::wstring::npos) {
        size_t vpos = message.find(L"\"value\":[");
        if (vpos != std::wstring::npos) {
            std::vector<std::wstring> list;
            size_t start = vpos + 9;
            while (start < message.length()) {
                size_t quote1 = message.find(L"\"", start);
                if (quote1 == std::wstring::npos) break;
                size_t quote2 = message.find(L"\"", quote1 + 1);
                if (quote2 == std::wstring::npos) break;
                
                std::wstring procName = message.substr(quote1 + 1, quote2 - quote1 - 1);
                if (!procName.empty()) {
                    list.push_back(procName);
                }
                
                start = quote2 + 1;
                size_t comma = message.find(L",", start);
                size_t bracket = message.find(L"]", start);
                if (bracket != std::wstring::npos && (comma == std::wstring::npos || bracket < comma)) {
                    break;
                }
            }
            SaveBlacklist(list);
            ReloadBlacklist();
        }
    }
    else if (message.find(L"\"type\":\"REQ_CONFIG\"") != std::wstring::npos ||
             message.find(L"\"type\":\"UI_READY\"") != std::wstring::npos) {
        if (g_pConfig) {
            PostConfigMessage(*g_pConfig);
        }
        // Also send auto-start state
        bool autoStart = IsAutoStartEnabled();
        std::wstring autoJson = L"{\"type\":\"AUTOSTART_STATE\",\"enabled\":";
        autoJson += autoStart ? L"true" : L"false";
        autoJson += L"}";
        _webview->PostWebMessageAsJson(autoJson.c_str());
    }
    else if (message.find(L"\"type\":\"SET_AUTOSTART\"") != std::wstring::npos) {
        bool enable = jsonExtractBool(message, L"enabled");
        SetAutoStart(enable);
    }
    else if (message.find(L"\"type\":\"REQ_BLACKLIST\"") != std::wstring::npos) {
        std::vector<std::wstring> list = LoadBlacklist();
        std::wstringstream ss;
        ss << L"{\"type\":\"BLACKLIST_DATA\",\"value\":[";
        for (size_t i = 0; i < list.size(); ++i) {
            ss << L"\"" << list[i] << L"\"";
            if (i < list.size() - 1) ss << L",";
        }
        ss << L"]}";
        _webview->PostWebMessageAsJson(ss.str().c_str());
    }

    return S_OK;

}

std::wstring SettingsWebView::SerializeConfigToJson(const UniKeyConfig& config) {
    std::wstringstream ss;
    ss << L"{"
       << L"\"version\":" << config.version << L","
       << L"\"inputEnabled\":" << (config.inputEnabled ? L"true" : L"false") << L","
       << L"\"inputMethod\":" << (int)config.inputMethod << L","
       << L"\"charset\":" << (int)config.charset << L","
       << L"\"toneType\":" << (int)config.toneType << L","
       << L"\"spellCheck\":" << (config.spellCheck ? L"true" : L"false") << L","
       << L"\"macroEnabled\":" << (config.macroEnabled ? L"true" : L"false") << L","
       << L"\"freeToneMarking\":" << (config.freeToneMarking ? L"true" : L"false") << L","
       << L"\"toggleKey\":" << (int)config.toggleKey << L","
       << L"\"restoreKeyEnabled\":" << (config.restoreKeyEnabled ? L"true" : L"false") << L","
       << L"\"useClipboardForUnicode\":" << (config.useClipboardForUnicode ? L"true" : L"false") << L","
       << L"\"showDialogOnStartup\":" << (config.showDialogOnStartup ? L"true" : L"false") << L","
       << L"\"perAppInputState\":" << (config.perAppInputState ? L"true" : L"false") << L","
       << L"\"macroFilePath\":\"";

    // Basic escaping for the path (only backslashes for now)
    for (size_t i = 0; i < 260 && config.macroFilePath[i] != L'\0'; ++i) {
        if (config.macroFilePath[i] == L'\\') ss << L"\\\\";
        else ss << config.macroFilePath[i];
    }

    ss << L"\"}";
    return ss.str();
}
