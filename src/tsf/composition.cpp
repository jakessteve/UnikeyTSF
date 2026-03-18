#include "composition.h"

CCompositionManager::CCompositionManager()
{
}

CCompositionManager::~CCompositionManager()
{
    // Make sure it's ended
}

HRESULT CCompositionManager::StartComposition(TfEditCookie ec, ITfContext* pContext, TfClientId /*tfClientId*/)
{
    if (IsComposing()) return S_OK;

    ComPtr<ITfInsertAtSelection> pIAS;
    HRESULT hr = pContext->QueryInterface(IID_ITfInsertAtSelection, (void**)&pIAS);
    if (FAILED(hr)) return hr;

    ComPtr<ITfRange> pRange;
    hr = pIAS->InsertTextAtSelection(ec, TF_IAS_QUERYONLY, nullptr, 0, &pRange);
    if (FAILED(hr)) return hr;

    ComPtr<ITfContextComposition> pContextComposition;
    hr = pContext->QueryInterface(IID_ITfContextComposition, (void**)&pContextComposition);
    if (FAILED(hr)) return hr;

    hr = pContextComposition->StartComposition(ec, pRange.Get(), nullptr, &_pComposition);
    if (FAILED(hr)) return hr;

    return S_OK; 
}

HRESULT CCompositionManager::UpdateComposition(TfEditCookie ec, ITfContext* pContext, const std::wstring& text)
{
    if (!IsComposing()) {
        StartComposition(ec, pContext, 0);
    }
    
    if (!IsComposing()) return E_FAIL;

    // Update the text in the composition
    ComPtr<ITfRange> pRange;
    HRESULT hr = _pComposition->GetRange(&pRange);
    if (SUCCEEDED(hr)) {
        pRange->SetText(ec, 0, text.c_str(), (LONG)text.length());
        
        // Add visual feedback (underlining)
        ComPtr<ITfProperty> pProp;
        if (SUCCEEDED(pContext->GetProperty(GUID_PROP_COMPOSING, &pProp))) {
            VARIANT var;
            var.vt = VT_I4;
            var.lVal = TRUE;
            pProp->SetValue(ec, pRange.Get(), &var);
        }

        // Move selection to the end of the composition
        ComPtr<ITfRange> pSelectionRange;
        pRange->Clone(&pSelectionRange);
        pSelectionRange->Collapse(ec, TF_ANCHOR_END);
        
        TF_SELECTION tfSelection = {};
        tfSelection.range = pSelectionRange.Get();
        tfSelection.style.ase = TF_AE_NONE;
        tfSelection.style.fInterimChar = FALSE;
        pContext->SetSelection(ec, 1, &tfSelection);
    }

    return S_OK;
}

HRESULT CCompositionManager::EndComposition(TfEditCookie ec)
{
    if (!IsComposing()) return S_OK;

    _pComposition->EndComposition(ec);
    _pComposition.Reset();

    return S_OK;
}
