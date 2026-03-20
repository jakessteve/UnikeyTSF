#pragma once

#include <windows.h>
#include <string>
#include <wrl.h>
#include <WebView2.h>
#include "../shared_config.h"

/**
 * @class SettingsWebView
 * @brief Host class for Microsoft Edge WebView2 control in the Settings Dialog.
 */
class SettingsWebView {
public:
    SettingsWebView();
    ~SettingsWebView();

    // Initialize the WebView2 environment and controller
    HRESULT Initialize(HWND parentHwnd);

    // Resize the WebView2 controller to fit the given bounds
    void Resize(const RECT& bounds);

    // Send the current configuration to the web frontend as JSON
    HRESULT PostConfigMessage(const UniKeyConfig& config);

    // Get the underlying WebView2 controller (if needed for advanced usage)
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> GetController() const { return _controller; }

    // Get the underlying WebView2 webview object
    Microsoft::WRL::ComPtr<ICoreWebView2> GetWebView() const { return _webview; }

private:
    HWND _parentHwnd;
    Microsoft::WRL::ComPtr<ICoreWebView2Environment> _environment;
    Microsoft::WRL::ComPtr<ICoreWebView2Controller> _controller;
    Microsoft::WRL::ComPtr<ICoreWebView2> _webview;

    // Helper to handle messages received from the web frontend
    HRESULT OnWebMessageReceived(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args);

    // Event registration tokens
    EventRegistrationToken _webMessageToken;

    // Serializes UniKeyConfig to a JSON string
    std::wstring SerializeConfigToJson(const UniKeyConfig& config);
};
