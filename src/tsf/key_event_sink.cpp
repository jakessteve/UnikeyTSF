#include "key_event_sink.h"
#include "text_service.h"
#include "../config/blacklist.h"

CKeyEventSink::CKeyEventSink(CUniKeyTextService* pTextService)
    : _refCount(1), _pTextService(pTextService), _pKeystrokeMgr(nullptr), _clientId(TF_CLIENTID_NULL)
{
}

CKeyEventSink::~CKeyEventSink()
{
    UnadviseSink();
}

STDMETHODIMP CKeyEventSink::QueryInterface(REFIID riid, void **ppvObj)
{
    if (ppvObj == nullptr) return E_INVALIDARG;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfKeyEventSink))
    {
        *ppvObj = static_cast<ITfKeyEventSink*>(this);
    }
    else
    {
        *ppvObj = nullptr;
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) CKeyEventSink::AddRef(void)
{
    return InterlockedIncrement(&_refCount);
}

STDMETHODIMP_(ULONG) CKeyEventSink::Release(void)
{
    LONG count = InterlockedDecrement(&_refCount);
    if (count == 0) delete this;
    return count;
}

HRESULT CKeyEventSink::AdviseSink(ITfKeystrokeMgr* pKeystrokeMgr, TfClientId clientId)
{
    if (!pKeystrokeMgr) return E_INVALIDARG;

    _pKeystrokeMgr = pKeystrokeMgr;
    _pKeystrokeMgr->AddRef();
    _clientId = clientId;

    return _pKeystrokeMgr->AdviseKeyEventSink(clientId, this, TRUE);
}

HRESULT CKeyEventSink::UnadviseSink()
{
    if (_pKeystrokeMgr)
    {
        _pKeystrokeMgr->UnadviseKeyEventSink(_clientId);
        _pKeystrokeMgr->Release();
        _pKeystrokeMgr = nullptr;
    }
    return S_OK;
}

STDMETHODIMP CKeyEventSink::OnSetFocus(BOOL fForeground)
{
    if (_pTextService) {
        return _pTextService->OnSetFocus(fForeground);
    }
    return S_OK;
}

STDMETHODIMP CKeyEventSink::OnTestKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    if (!pfEaten) return E_INVALIDARG;
    if (_pTextService) {
        return _pTextService->OnTestKeyDown(pContext, wParam, lParam, pfEaten);
    }
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CKeyEventSink::OnKeyDown(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    if (!pfEaten) return E_INVALIDARG;
    if (_pTextService) {
        return _pTextService->OnKeyDown(pContext, wParam, lParam, pfEaten);
    }
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CKeyEventSink::OnTestKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    if (!pfEaten) return E_INVALIDARG;
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CKeyEventSink::OnKeyUp(ITfContext *pContext, WPARAM wParam, LPARAM lParam, BOOL *pfEaten)
{
    if (!pfEaten) return E_INVALIDARG;
    *pfEaten = FALSE;
    return S_OK;
}

STDMETHODIMP CKeyEventSink::OnPreservedKey(ITfContext *pContext, REFGUID rguid, BOOL *pfEaten)
{
    if (!pfEaten) return E_INVALIDARG;
    *pfEaten = FALSE;
    return S_OK;
}
