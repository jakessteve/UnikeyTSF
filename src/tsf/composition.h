#pragma once
// =============================================================================
// UniKey TSF Reborn â€” Composition Manager
// =============================================================================
// Wraps ITfComposition lifecycle.
// =============================================================================

#include <windows.h>
#include <msctf.h>
#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;

class CCompositionManager
{
public:
    CCompositionManager();
    ~CCompositionManager();

    bool IsComposing() const { return _pComposition != nullptr; }

    HRESULT StartComposition(TfEditCookie ec, ITfContext* pContext, TfClientId tfClientId);
    HRESULT UpdateComposition(TfEditCookie ec, ITfContext* pContext, const std::wstring& text);
    HRESULT EndComposition(TfEditCookie ec);

private:
    ComPtr<ITfComposition> _pComposition;
};
