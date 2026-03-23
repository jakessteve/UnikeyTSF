#pragma once

#include <windows.h>
#include <msctf.h>

class CUniKeyTextService;

// Interface for handling TSF key events.
// Owned by CUniKeyTextService and connected via ITfKeystrokeMgr.
class CKeyEventSink : public ITfKeyEventSink
{
public:
    CKeyEventSink(CUniKeyTextService* pTextService);
    ~CKeyEventSink();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void **ppvObj);
    STDMETHODIMP_(ULONG) AddRef(void);
    STDMETHODIMP_(ULONG) Release(void);

    // ITfKeyEventSink
    STDMETHODIMP OnSetFocus(BOOL fForeground);
    STDMETHODIMP OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten);
    STDMETHODIMP OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten);

    HRESULT AdviseSink(ITfKeystrokeMgr* pKeystrokeMgr, TfClientId clientId);
    HRESULT UnadviseSink();

private:
    LONG _refCount;
    CUniKeyTextService* _pTextService; // Weak reference
    ITfKeystrokeMgr* _pKeystrokeMgr;   // Strong reference to sink manager
    TfClientId _clientId;
};
