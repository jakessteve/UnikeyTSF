#include "settings_webview.h"
#include "settings_message_contract.h"
#include "../config/ipc_manager.h"
#include "../config/blacklist.h"
#include "../engine/per_app_input_state.h"
#include <wrl/event.h>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <filesystem>

using namespace Microsoft::WRL;
namespace fs = std::filesystem;

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

namespace {

std::wstring GetModuleDirectory()
{
    wchar_t exePath[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, exePath, ARRAYSIZE(exePath));
    std::wstring exeDir(exePath);
    size_t pos = exeDir.find_last_of(L"\\/");
    if (pos != std::wstring::npos) {
        exeDir.resize(pos);
    }
    return exeDir;
}

std::wstring ResolveFrontendDirectory()
{
    const std::wstring exeDir = GetModuleDirectory();
    const std::vector<std::wstring> candidates = {
        exeDir + L"\\frontend",
        exeDir + L"\\..\\frontend",
        exeDir + L"\\..\\..\\src\\ui\\dist",
        exeDir + L"\\src\\ui\\dist"
    };

    for (const auto& candidate : candidates) {
        std::error_code ec;
        fs::path dir = fs::weakly_canonical(fs::path(candidate), ec);
        if (ec) {
            dir = fs::path(candidate);
        }
        fs::path indexPath = dir / "index.html";
        if (fs::exists(indexPath, ec) && !ec) {
            return dir.wstring();
        }
    }

    return L"";
}

std::wstring GetMissingFrontendHtml()
{
    return LR"HTML(
<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>UniKey TSF Settings</title>
  <style>
    body { font-family: Segoe UI, sans-serif; background: #1f1f1f; color: #f3f3f3; margin: 0; }
    main { max-width: 640px; margin: 48px auto; padding: 24px 28px; background: #2a2a2a; border-radius: 12px; }
    h1 { margin-top: 0; font-size: 24px; }
    p { line-height: 1.5; color: #d0d0d0; }
    code { background: #161616; padding: 2px 6px; border-radius: 6px; }
  </style>
</head>
<body>
  <main>
    <h1>Settings UI assets were not found</h1>
    <p>The native host started correctly, but the packaged frontend files are missing.</p>
    <p>Build or copy the frontend into <code>frontend\\index.html</code> next to the executable, or generate <code>src\\ui\\dist</code> for local development builds.</p>
  </main>
</body>
</html>
)HTML";
}

} // namespace

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
                            std::wstring frontendDir = ResolveFrontendDirectory();
                            if (frontendDir.empty()) {
                                _webview->NavigateToString(GetMissingFrontendHtml().c_str());
                                return S_OK;
                            }

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

static bool SetAutoStart(bool enable) {
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, AUTOSTART_REG_KEY, 0, KEY_WRITE, &hKey) != ERROR_SUCCESS)
        return false;
    bool success = false;
    if (enable) {
        wchar_t exePath[MAX_PATH];
        GetModuleFileNameW(nullptr, exePath, MAX_PATH);
        std::wstring quotedPath = L"\"";
        quotedPath += exePath;
        quotedPath += L"\"";
        success = RegSetValueExW(hKey, AUTOSTART_VALUE_NAME, 0, REG_SZ,
            (const BYTE*)quotedPath.c_str(), (DWORD)((quotedPath.length() + 1) * sizeof(wchar_t))) == ERROR_SUCCESS;
    } else {
        const LONG deleteResult = RegDeleteValueW(hKey, AUTOSTART_VALUE_NAME);
        success = (deleteResult == ERROR_SUCCESS || deleteResult == ERROR_FILE_NOT_FOUND);
    }
    RegCloseKey(hKey);
    return success;
}

void SettingsWebView::Resize(const RECT& bounds) {
    if (_controller) {
        _controller->put_Bounds(bounds);
    }
}

HRESULT SettingsWebView::PostConfigMessage(const UniKeyConfig& config) {
    if (!_webview) return E_FAIL;
    const std::wstring json = BuildSettingsConfigMessage(config);
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

    SettingsHostMessage parsedMessage;
    if (!ParseSettingsHostMessage(message, &parsedMessage)) {
        return E_INVALIDARG;
    }

    if (parsedMessage.type == SettingsHostMessageType::UpdateConfig) {
        if (!LockConfig()) {
            return HRESULT_FROM_WIN32(ERROR_TIMEOUT);
        }

        HRESULT updateHr = S_OK;
        if (!g_pConfig) {
            updateHr = E_UNEXPECTED;
        } else {
            const uint8_t requestedInputEnabled = parsedMessage.config.inputEnabled;
            const bool previousPerAppInputState = g_pConfig->perAppInputState != 0;
            const bool nextPerAppInputState = parsedMessage.config.perAppInputState != 0;
            g_pConfig->version = parsedMessage.config.version;
            g_pConfig->inputMethod    = parsedMessage.config.inputMethod;
            g_pConfig->charset        = parsedMessage.config.charset;
            g_pConfig->toneType       = parsedMessage.config.toneType;
            g_pConfig->spellCheck     = parsedMessage.config.spellCheck;
            g_pConfig->macroEnabled   = parsedMessage.config.macroEnabled;
            g_pConfig->freeToneMarking= parsedMessage.config.freeToneMarking;
            g_pConfig->toggleKey      = parsedMessage.config.toggleKey;
            g_pConfig->restoreKeyEnabled = parsedMessage.config.restoreKeyEnabled;
            g_pConfig->useClipboardForUnicode = parsedMessage.config.useClipboardForUnicode;
            g_pConfig->showDialogOnStartup = parsedMessage.config.showDialogOnStartup;
            g_pConfig->perAppInputState = nextPerAppInputState ? 1 : 0;
            wcsncpy_s(g_pConfig->macroFilePath, parsedMessage.config.macroFilePath, _TRUNCATE);

            const std::wstring focusedAppId = ResolveForegroundAppId();
            if (!nextPerAppInputState) {
                g_pConfig->inputEnabled = requestedInputEnabled;
                SetPerAppInputStateSeed(requestedInputEnabled);
            } else {
                if (!previousPerAppInputState) {
                    SetPerAppInputStateSeed(g_pConfig->inputEnabled);
                }
                SetInputEnabledForApp(g_pConfig, focusedAppId, requestedInputEnabled);
            }

            if (!SaveConfigToFile(g_pConfig)) {
                updateHr = E_FAIL;
            }
        }
        UnlockConfig();
        return updateHr;
    } else if (parsedMessage.type == SettingsHostMessageType::UpdateBlacklist) {
        SaveBlacklist(parsedMessage.blacklist);
        ReloadBlacklist();
    } else if (parsedMessage.type == SettingsHostMessageType::RequestConfig ||
               parsedMessage.type == SettingsHostMessageType::UiReady) {
        UniKeyConfig configSnapshot = {};
        bool hasConfigSnapshot = false;
        if (LockConfig()) {
            if (g_pConfig) {
                configSnapshot = *g_pConfig;
                hasConfigSnapshot = true;
            }
            UnlockConfig();
        }
        if (hasConfigSnapshot) {
            if (configSnapshot.perAppInputState) {
                configSnapshot.inputEnabled = ResolveEffectiveInputEnabled(configSnapshot, ResolveForegroundAppId());
            }
            PostConfigMessage(configSnapshot);
        }
        // Also send auto-start state
        bool autoStart = IsAutoStartEnabled();
        const std::wstring autoJson = BuildAutostartStateMessage(autoStart);
        return _webview->PostWebMessageAsJson(autoJson.c_str());
    } else if (parsedMessage.type == SettingsHostMessageType::SetAutostart) {
        if (!SetAutoStart(parsedMessage.enabled)) {
            return E_FAIL;
        }
    } else if (parsedMessage.type == SettingsHostMessageType::RequestBlacklist) {
        const std::vector<std::wstring> list = LoadBlacklist();
        const std::wstring blacklistJson = BuildBlacklistDataMessage(list);
        return _webview->PostWebMessageAsJson(blacklistJson.c_str());
    }

    return S_OK;

}
