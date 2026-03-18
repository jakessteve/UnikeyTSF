#pragma once
// =============================================================================
// UniKey TSF Reborn â€” Class Factory
// =============================================================================
// Standard COM IClassFactory that creates CUniKeyTextService instances.
// =============================================================================

#include <unknwn.h>

class CClassFactory : public IClassFactory
{
public:
    CClassFactory();
    ~CClassFactory();

    // IUnknown
    STDMETHODIMP QueryInterface(REFIID riid, void** ppvObj) override;
    STDMETHODIMP_(ULONG) AddRef() override;
    STDMETHODIMP_(ULONG) Release() override;

    // IClassFactory
    STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObj) override;
    STDMETHODIMP LockServer(BOOL fLock) override;

private:
    LONG _refCount = 1;
};

// Global DLL lock count (used by DllCanUnloadNow)
extern LONG g_cDllRef;
extern HMODULE g_hModule;
