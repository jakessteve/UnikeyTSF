#include "settings_webview.h"
#include <wrl/event.h>
#include <string>
#include <sstream>
#include <iomanip>

using namespace Microsoft::WRL;

SettingsWebView::SettingsWebView()
    : _parentHwnd(nullptr), _webMessageToken() {
}

SettingsWebView::~SettingsWebView() {
    if (_webview) {
        _webview->remove_WebMessageReceived(_webMessageToken);
    }
}

HRESULT SettingsWebView::Initialize(HWND parentHwnd) {
    _parentHwnd = parentHwnd;

    // 1. Create the WebView2 environment
    return CreateCoreWebView2EnvironmentWithOptions(
        nullptr, // Use default browser channel
        nullptr, // Use default user data folder
        nullptr, // Default options
        Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
            [this](HRESULT result, ICoreWebView2Environment* env) -> HRESULT {
                if (FAILED(result)) return result;
                _environment = env;

                // 2. Create the WebView2 controller
                return _environment->CreateCoreWebView2Controller(
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

                            // 6. Navigate to initial page
                            wchar_t exePath[MAX_PATH];
                            GetModuleFileNameW(nullptr, exePath, MAX_PATH);
                            std::wstring path(exePath);
                            size_t pos = path.find_last_of(L"\\/");
                            if (pos != std::wstring::npos) {
                                path = path.substr(0, pos) + L"\\frontend\\index.html";
                            }
                            // Convert backslashes to forward slashes for URI or just use a standard file URI format
                            std::wstring uri = L"file:///" + path;

                            _webview->Navigate(uri.c_str());

                            return S_OK;
                        }).Get());
            }).Get());
}

void SettingsWebView::Resize(const RECT& bounds) {
    if (_controller) {
        _controller->put_Bounds(bounds);
    }
}

HRESULT SettingsWebView::PostConfigMessage(const UniKeyConfig& config) {
    if (!_webview) return E_FAIL;

    std::wstring json = SerializeConfigToJson(config);
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

    // TODO: Parse message and update config
    // For now, this is a stub.
    // Example message: {"type": "UPDATE_CONFIG", "key": "inputMethod", "value": 1}

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
       << L"\"macroFilePath\":\"";

    // Basic escaping for the path (only backslashes for now)
    for (size_t i = 0; i < 260 && config.macroFilePath[i] != L'\0'; ++i) {
        if (config.macroFilePath[i] == L'\\') ss << L"\\\\";
        else ss << config.macroFilePath[i];
    }

    ss << L"\"}";
    return ss.str();
}
