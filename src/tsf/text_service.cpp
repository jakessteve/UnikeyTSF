// =============================================================================
// UniKey TSF Reborn â€” Text Service Implementation
// =============================================================================

#include "text_service.h"
#include "guids.h"
#include "edit_session.h"
#include "key_event_sink.h"
#include "../config/blacklist.h"
#include <windows.h>
#include <msctf.h>
#include <olectl.h>
#include <vector>
#include <strsafe.h>

// Helper to safely keep the TextService alive across async edit sessions
struct SafeSvcRef {
    CUniKeyTextService* p;
    SafeSvcRef(CUniKeyTextService* p) : p(p) { if(p) p->AddRef(); }
    SafeSvcRef(const SafeSvcRef& other) : p(other.p) { if(p) p->AddRef(); }
    SafeSvcRef(SafeSvcRef&& other) noexcept : p(other.p) { other.p = nullptr; }
    ~SafeSvcRef() { if(p) p->Release(); }
};

// =============================================================================
// Constructor / Destructor
// =============================================================================

CUniKeyTextService::CUniKeyTextService()
    : _refCount(1)
    , _tfClientId(TF_CLIENTID_NULL)
    , _dwThreadMgrEventSinkCookie(TF_INVALID_COOKIE)
    , _isBlacklisted(false)
    , _hMapFile(NULL)
    , _hMutex(NULL)
{
    InterlockedIncrement(&g_cDllRef); // Keep DLL from unloading while active

    memset(&_config, 0, sizeof(_config));
    _config.inputEnabled = 1; // Default to enabled for now
    _config.inputMethod = IM_VNI;
}

CUniKeyTextService::~CUniKeyTextService()
{
    InterlockedDecrement(&g_cDllRef);
}

// =============================================================================
// IUnknown
// =============================================================================

STDMETHODIMP CUniKeyTextService::QueryInterface(REFIID riid, void** ppvObj)
{
    if (!ppvObj) return E_INVALIDARG;

    *ppvObj = nullptr;

    if (IsEqualIID(riid, IID_IUnknown) ||
        IsEqualIID(riid, IID_ITfTextInputProcessor) ||
        IsEqualIID(riid, IID_ITfTextInputProcessorEx))
    {
        *ppvObj = static_cast<ITfTextInputProcessorEx*>(this);
    }
    else if (IsEqualIID(riid, IID_ITfThreadMgrEventSink))
    {
        *ppvObj = static_cast<ITfThreadMgrEventSink*>(this);
    }
    else
    {
        return E_NOINTERFACE;
    }

    AddRef();
    return S_OK;
}

STDMETHODIMP_(ULONG) CUniKeyTextService::AddRef()
{
    return InterlockedIncrement(&_refCount);
}

STDMETHODIMP_(ULONG) CUniKeyTextService::Release()
{
    LONG count = InterlockedDecrement(&_refCount);
    if (count == 0) {
        delete this;
    }
    return count;
}

// =============================================================================
// ITfTextInputProcessor
// =============================================================================

STDMETHODIMP CUniKeyTextService::Activate(ITfThreadMgr* pThreadMgr, TfClientId tfClientId)
{
    return ActivateEx(pThreadMgr, tfClientId, 0);
}

STDMETHODIMP CUniKeyTextService::Deactivate()
{
    _UninitKeyEventSink();
    _UninitThreadMgrEventSink();

    _pThreadMgr.Reset();
    _tfClientId = TF_CLIENTID_NULL;

    if (_hMapFile) {
        CloseHandle(_hMapFile);
        _hMapFile = NULL;
    }
    if (_hMutex) {
        CloseHandle(_hMutex);
        _hMutex = NULL;
    }

    return S_OK;
}

// =============================================================================
// ITfTextInputProcessorEx
// =============================================================================

STDMETHODIMP CUniKeyTextService::ActivateEx(
    ITfThreadMgr* pThreadMgr, TfClientId tfClientId, DWORD /*dwFlags*/)
{
    _pThreadMgr = pThreadMgr;
    _tfClientId = tfClientId;

    HRESULT hr = _InitThreadMgrEventSink();
    if (FAILED(hr)) {
        Deactivate();
        return hr;
    }

    hr = _InitKeyEventSink();
    if (FAILED(hr)) {
        Deactivate();
        return hr;
    }

    // Open shared config handles (cached for efficiency)
    _hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    _hMutex = OpenMutexW(SYNCHRONIZE, FALSE, UNIKEY_SHARED_MUTEX_NAME);

    _UpdateConfig();


    return S_OK;
}

// =============================================================================
// ITfThreadMgrEventSink
// =============================================================================

STDMETHODIMP CUniKeyTextService::OnInitDocumentMgr(ITfDocumentMgr* /*pDocMgr*/)
{
    return S_OK;
}

STDMETHODIMP CUniKeyTextService::OnUninitDocumentMgr(ITfDocumentMgr* /*pDocMgr*/)
{
    return S_OK;
}

STDMETHODIMP CUniKeyTextService::OnSetFocus(
    ITfDocumentMgr* /*pDocMgrFocus*/, ITfDocumentMgr* /*pDocMgrPrevFocus*/)
{
    // Get the executable name of the current process
    wchar_t szPath[MAX_PATH];
    if (GetModuleFileNameW(NULL, szPath, MAX_PATH)) {
        wchar_t* szName = wcsrchr(szPath, L'\\');
        if (szName) {
            _isBlacklisted = IsProcessBlacklisted(szName + 1);
        } else {
            _isBlacklisted = IsProcessBlacklisted(szPath);
        }
    }
    return S_OK;
}

STDMETHODIMP CUniKeyTextService::OnPushContext(ITfContext* /*pContext*/)
{
    return S_OK;
}

STDMETHODIMP CUniKeyTextService::OnPopContext(ITfContext* /*pContext*/)
{
    return S_OK;
}

// =============================================================================
// Internal methods called by CKeyEventSink
// =============================================================================

HRESULT CUniKeyTextService::OnSetFocus(BOOL /*fForeground*/)
{
    return S_OK;
}

HRESULT CUniKeyTextService::OnTestKeyDown(
    ITfContext* /*pContext*/, WPARAM wParam, LPARAM /*lParam*/, BOOL* pfEaten)
{
    if (pfEaten) *pfEaten = FALSE;

    _UpdateConfig();
    if (!_config.inputEnabled || _isBlacklisted) return S_OK;

    // If we're in an active composition, eat ALL keys so OnKeyDown gets called
    // and we can properly end the composition on non-Vietnamese characters.
    if (_engine.IsInWord()) {
        if (pfEaten) *pfEaten = TRUE;
        return S_OK;
    }

    // Otherwise, eat A-Z and 0-9 for Telex/VNI
    if ((wParam >= 'A' && wParam <= 'Z') || (wParam >= '0' && wParam <= '9') || wParam == VK_BACK) {
        if (pfEaten) *pfEaten = TRUE;
    }

    return S_OK;
}

HRESULT CUniKeyTextService::OnKeyDown(
    ITfContext* pContext, WPARAM wParam, LPARAM lParam, BOOL* pfEaten)
{
    if (pfEaten) *pfEaten = FALSE;

    _UpdateConfig();
    if (!_config.inputEnabled || _isBlacklisted) return S_OK;

    // Handle backspace
    if (wParam == VK_BACK) {
        if (_engine.IsInWord()) {
            _engine.RemoveLastChar();
            
            ComPtr<CEditSession> pEditSession;
            if (_engine.IsInWord()) {
                pEditSession.Attach(new CEditSession([this, pContext, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                    return _composition.UpdateComposition(ec, pContext, _engine.GetCompositionString());
                }));
            } else {
                pEditSession.Attach(new CEditSession([this, pContext, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                    _composition.UpdateComposition(ec, pContext, L"");
                    return _composition.EndComposition(ec);
                }));
            }
            
            HRESULT hrSession = pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_SYNC | TF_ES_READWRITE, nullptr);
            if (hrSession == TF_E_SYNCHRONOUS) {
                pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, nullptr);
            }
            if (pfEaten) *pfEaten = TRUE;
            return S_OK;
        }
        return S_OK;
    }

    // Handle Escape
    if (wParam == VK_ESCAPE) {
        if (_composition.IsComposing()) {
            ComPtr<CEditSession> pEditSession;
            pEditSession.Attach(new CEditSession([this, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                return _composition.EndComposition(ec);
            }));
            
            HRESULT hrSession = pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_SYNC | TF_ES_READWRITE, nullptr);
            if (hrSession == TF_E_SYNCHRONOUS) {
                pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, nullptr);
            }
            _engine.Clear();
            if (pfEaten) *pfEaten = TRUE;
            return S_OK;
        }
    }

    // Convert key to character without consuming dead key state (using flag 4 in Windows 10+)
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);
    wchar_t ch;
    if (ToUnicodeEx((UINT)wParam, (UINT)(lParam >> 16) & 0xFF, keyboardState, &ch, 1, 4, GetKeyboardLayout(0)) > 0) {
        if (_engine.ProcessKey(ch, (InputMethod)_config.inputMethod)) {
            if (pfEaten) *pfEaten = TRUE;

            // Trigger Edit Session to update composition
            ComPtr<CEditSession> pEditSession;
            pEditSession.Attach(new CEditSession([this, pContext, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                return _composition.UpdateComposition(ec, pContext, _engine.GetCompositionString());
            }));

            HRESULT hrSession = pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_SYNC | TF_ES_READWRITE, nullptr);
            if (hrSession == TF_E_SYNCHRONOUS) {
                pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, nullptr);
            }
        } else {
            // Engine didn't eat it (e.g. space, punctuation), end composition
            if (_composition.IsComposing()) {
                std::wstring currentWord = _engine.GetCompositionString();
                std::wstring expanded = _config.macroEnabled ? _macroEngine.Expand(currentWord) : L"";

                ComPtr<CEditSession> pEditSession;
                if (!expanded.empty()) {
                    pEditSession.Attach(new CEditSession([this, pContext, expanded, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                        _composition.UpdateComposition(ec, pContext, expanded);
                        return _composition.EndComposition(ec);
                    }));
                } else {
                    pEditSession.Attach(new CEditSession([this, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                        return _composition.EndComposition(ec);
                    }));
                }
                
                HRESULT hrSession = pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_SYNC | TF_ES_READWRITE, nullptr);
                if (hrSession == TF_E_SYNCHRONOUS) {
                    pContext->RequestEditSession(_tfClientId, pEditSession.Get(), TF_ES_ASYNCDONTCARE | TF_ES_READWRITE, nullptr);
                }
            }
            _engine.Clear();
            // DON'T eat the key — let the app handle the space/punctuation naturally
            if (pfEaten) *pfEaten = FALSE;
        }
    }

    return S_OK;
}

// =============================================================================
// Thread Manager Event Sink (advise/unadvise)
// =============================================================================

HRESULT CUniKeyTextService::_InitThreadMgrEventSink()
{
    ComPtr<ITfSource> pSource;
    HRESULT hr = _pThreadMgr.As(&pSource);
    if (FAILED(hr)) return hr;

    hr = pSource->AdviseSink(
        IID_ITfThreadMgrEventSink,
        static_cast<ITfThreadMgrEventSink*>(this),
        &_dwThreadMgrEventSinkCookie
    );

    return hr;
}

void CUniKeyTextService::_UninitThreadMgrEventSink()
{
    if (_dwThreadMgrEventSinkCookie == TF_INVALID_COOKIE) return;
    if (!_pThreadMgr) return;

    ComPtr<ITfSource> pSource;
    if (SUCCEEDED(_pThreadMgr.As(&pSource))) {
        pSource->UnadviseSink(_dwThreadMgrEventSinkCookie);
    }

    _dwThreadMgrEventSinkCookie = TF_INVALID_COOKIE;
}

// =============================================================================
// Key Event Sink (advise/unadvise)
// =============================================================================

HRESULT CUniKeyTextService::_InitKeyEventSink()
{
    _pKeyEventSink.Attach(new CKeyEventSink(this));
    if (!_pKeyEventSink) return E_OUTOFMEMORY;

    ComPtr<ITfKeystrokeMgr> pKeystrokeMgr;
    HRESULT hr = _pThreadMgr.As(&pKeystrokeMgr);
    if (FAILED(hr)) return hr;

    return _pKeyEventSink->AdviseSink(pKeystrokeMgr.Get(), _tfClientId);
}

void CUniKeyTextService::_UninitKeyEventSink()
{
    if (_pKeyEventSink) {
        _pKeyEventSink->UnadviseSink();
        _pKeyEventSink.Reset();
    }
}

// =============================================================================
// Helper to read shared config
// =============================================================================

void CUniKeyTextService::_UpdateConfig()
{
    // Lazy-open: retry opening handles if they're NULL
    // This handles the case where the DLL was activated before the EXE created shared memory
    if (!_hMapFile) {
        _hMapFile = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    }
    if (!_hMutex) {
        _hMutex = OpenMutexW(SYNCHRONIZE, FALSE, UNIKEY_SHARED_MUTEX_NAME);
    }
    if (!_hMapFile) return; // Still couldn't open — shared memory not created yet

    // If we have the map file but no mutex, read without locking (best effort)
    bool locked = false;
    if (_hMutex) {
        locked = (WaitForSingleObject(_hMutex, 10) == WAIT_OBJECT_0);
    }

    void* pBuf = MapViewOfFile(_hMapFile, FILE_MAP_READ, 0, 0, sizeof(UniKeyConfig));
    if (pBuf != NULL) {
        UniKeyConfig* pRemoteConfig = (UniKeyConfig*)pBuf;
        if (pRemoteConfig->version == UNIKEY_CONFIG_VERSION) {
            bool wasMacroEnabled = _config.macroEnabled;
            std::wstring oldMacroFile = _currentMacroFile;

            memcpy(&_config, pRemoteConfig, sizeof(UniKeyConfig));

            if (_config.macroEnabled) {
                std::wstring newMacroFile(_config.macroFilePath);
                if (!wasMacroEnabled || oldMacroFile != newMacroFile) {
                    _currentMacroFile = newMacroFile;
                    _macroEngine.Load(_currentMacroFile);
                }
            }
        }
        UnmapViewOfFile(pBuf);
    }

    if (locked) {
        ReleaseMutex(_hMutex);
    }
}

