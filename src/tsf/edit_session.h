#pragma once
// =============================================================================
// UniKey TSF Reborn â€” Edit Session Helper
// =============================================================================
// TSF requires all document mutations to happen within an ITfEditSession.
// This class wraps a functional callback to avoid boilerplate.
// =============================================================================

#include <windows.h>
#include <msctf.h>
#include <wrl/client.h>
#include <functional>

using Microsoft::WRL::ComPtr;

class CEditSession : public ITfEditSession
{
public:
    using EditSessionCallback = std::function<HRESULT(TfEditCookie)>;

    CEditSession(EditSessionCallback callback)
        : _refCount(1) // COM objects instantiations must start with 1, not 0
        , _callback(callback)
    {
    }

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj) override
    {
        if (!ppvObj) return E_INVALIDARG;
        *ppvObj = nullptr;

        if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_ITfEditSession)) {
            *ppvObj = static_cast<ITfEditSession*>(this);
        } else {
            return E_NOINTERFACE;
        }

        AddRef();
        return S_OK;
    }

    STDMETHODIMP_(ULONG) AddRef() override
    {
        return InterlockedIncrement(&_refCount);
    }

    STDMETHODIMP_(ULONG) Release() override
    {
        LONG count = InterlockedDecrement(&_refCount);
        if (count == 0) {
            delete this;
        }
        return count;
    }

    // ITfEditSession
    STDMETHODIMP DoEditSession(TfEditCookie ec) override
    {
        if (_callback) {
            return _callback(ec);
        }
        return S_OK;
    }

private:
    LONG                _refCount;
    EditSessionCallback _callback;
};
