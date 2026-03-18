// =============================================================================
// UniKey TSF Reborn â€” Class Factory Implementation
// =============================================================================

#include "class_factory.h"
#include "text_service.h"

CClassFactory::CClassFactory() { InterlockedIncrement(&g_cDllRef); }
CClassFactory::~CClassFactory() { InterlockedDecrement(&g_cDllRef); }

// =============================================================================
// IUnknown
// =============================================================================

STDMETHODIMP CClassFactory::QueryInterface(REFIID riid, void** ppvObj)
{
    if (!ppvObj) return E_INVALIDARG;

    *ppvObj = nullptr;

    if (IsEqualIID(riid, IID_IUnknown) || IsEqualIID(riid, IID_IClassFactory))
    {
        *ppvObj = static_cast<IClassFactory*>(this);
        AddRef();
        return S_OK;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP_(ULONG) CClassFactory::AddRef()
{
    return InterlockedIncrement(&_refCount);
}

STDMETHODIMP_(ULONG) CClassFactory::Release()
{
    LONG count = InterlockedDecrement(&_refCount);
    if (count == 0) {
        delete this;
    }
    return count;
}

// =============================================================================
// IClassFactory
// =============================================================================

STDMETHODIMP CClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObj)
{
    if (!ppvObj) return E_INVALIDARG;
    *ppvObj = nullptr;

    // No aggregation support
    if (pUnkOuter != nullptr) return CLASS_E_NOAGGREGATION;

    CUniKeyTextService* pService = new (std::nothrow) CUniKeyTextService();
    if (!pService) return E_OUTOFMEMORY;

    HRESULT hr = pService->QueryInterface(riid, ppvObj);
    pService->Release(); // QI adds a ref, balance the initial ref
    return hr;
}

STDMETHODIMP CClassFactory::LockServer(BOOL fLock)
{
    if (fLock) {
        InterlockedIncrement(&g_cDllRef);
    } else {
        InterlockedDecrement(&g_cDllRef);
    }
    return S_OK;
}
