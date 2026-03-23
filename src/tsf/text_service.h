#pragma once
// =============================================================================
// UniKey TSF Reborn â€” Text Service (ITfTextInputProcessorEx)
// =============================================================================
// Stub TSF Text Input Processor. Activates/deactivates with the thread manager
// but does not process keystrokes yet (that's Phase 3).
// =============================================================================

#include <windows.h>
#include <msctf.h>
#include <wrl/client.h>
#include <string>
#include "../engine/vn_engine.h"
#include "../engine/macro.h"
#include "composition.h"
#include "../shared_config.h"

using Microsoft::WRL::ComPtr;

extern LONG g_cDllRef;

class CKeyEventSink;

class CUniKeyTextService : public ITfTextInputProcessorEx,
                           public ITfThreadMgrEventSink
{
public:
    CUniKeyTextService();
    ~CUniKeyTextService();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // ITfTextInputProcessor
    STDMETHODIMP Activate(ITfThreadMgr* pThreadMgr, TfClientId tfClientId) override;
    STDMETHODIMP Deactivate() override;

    // ITfTextInputProcessorEx
    STDMETHODIMP ActivateEx(ITfThreadMgr* pThreadMgr, TfClientId tfClientId, DWORD dwFlags) override;

    // ITfThreadMgrEventSink
    STDMETHODIMP OnInitDocumentMgr(ITfDocumentMgr* pDocMgr) override;
    STDMETHODIMP OnUninitDocumentMgr(ITfDocumentMgr* pDocMgr) override;
    STDMETHODIMP OnSetFocus(ITfDocumentMgr* pDocMgrFocus, ITfDocumentMgr* pDocMgrPrevFocus) override;
    STDMETHODIMP OnPushContext(ITfContext* pContext) override;
    STDMETHODIMP OnPopContext(ITfContext* pContext) override;

    // Internal methods called by CKeyEventSink
    HRESULT OnSetFocus(BOOL fForeground);
    HRESULT OnTestKeyDown(ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);
    HRESULT OnKeyDown(ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten);

private:
    HRESULT _InitThreadMgrEventSink();
    void    _UninitThreadMgrEventSink();

    HRESULT _InitKeyEventSink();
    void    _UninitKeyEventSink();

    // Helper to read shared config
    void _UpdateConfig();

    LONG                  _refCount;
    ComPtr<ITfThreadMgr>  _pThreadMgr;
    TfClientId            _tfClientId;
    DWORD                 _dwThreadMgrEventSinkCookie;

    ComPtr<CKeyEventSink> _pKeyEventSink;

    VnEngine              _engine;
    MacroEngine           _macroEngine;
    std::wstring          _currentMacroFile;
    CCompositionManager   _composition;

    // Cached IPC Handles
    HANDLE                _hMapFile;
    HANDLE                _hMutex;
    UniKeyConfig          _config;
    bool                  _isBlacklisted;
};
