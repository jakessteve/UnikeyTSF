// =============================================================================
// UniKey TSF Reborn â€” Text Service Implementation
// =============================================================================

#include "text_service.h"
#include "guids.h"
#include "edit_session.h"
#include "key_event_sink.h"
#include "../engine/input_routing.h"
#include "../engine/per_app_input_state.h"
#include "../engine/typing_settings.h"
#include "../config/blacklist.h"
#include "../engine/reconversion_word.h"
#include <windows.h>
#include <msctf.h>
#include <olectl.h>
#include <vector>
#include <strsafe.h>
#include <cwctype>
#include <memory>

// Helper to safely keep the TextService alive across async edit sessions
struct SafeSvcRef {
    CUniKeyTextService* p;
    SafeSvcRef(CUniKeyTextService* p) : p(p) { if(p) p->AddRef(); }
    SafeSvcRef(const SafeSvcRef& other) : p(other.p) { if(p) p->AddRef(); }
    SafeSvcRef(SafeSvcRef&& other) noexcept : p(other.p) { other.p = nullptr; }
    ~SafeSvcRef() { if(p) p->Release(); }
};

static constexpr ULONGLONG kCommittedEditTimeoutMs = 1000;

static HRESULT RequestEditSessionWithFallback(
    ITfContext* pContext,
    TfClientId clientId,
    ITfEditSession* pEditSession,
    DWORD accessFlags)
{
    if (!pContext || !pEditSession) return E_INVALIDARG;

    HRESULT sessionHr = E_FAIL;
    HRESULT hr = pContext->RequestEditSession(
        clientId,
        pEditSession,
        TF_ES_SYNC | accessFlags,
        &sessionHr);
    if (FAILED(hr)) {
        if (hr != TF_E_LOCKED) {
            return hr;
        }
    } else if (sessionHr != TF_E_SYNCHRONOUS) {
        return sessionHr == TF_S_ASYNC ? S_OK : sessionHr;
    }

    sessionHr = E_FAIL;
    hr = pContext->RequestEditSession(
        clientId,
        pEditSession,
        TF_ES_ASYNCDONTCARE | accessFlags,
        &sessionHr);
    if (FAILED(hr)) {
        return hr;
    }

    return sessionHr == TF_S_ASYNC ? S_OK : sessionHr;
}

struct EditSessionFlag {
    bool value = false;
};

static bool IsNonTextCommitKey(WPARAM wParam)
{
    switch (wParam) {
    case VK_LEFT:
    case VK_RIGHT:
    case VK_UP:
    case VK_DOWN:
    case VK_HOME:
    case VK_END:
    case VK_PRIOR:
    case VK_NEXT:
    case VK_DELETE:
    case VK_TAB:
    case VK_RETURN:
        return true;
    default:
        return false;
    }
}

static void TraceRoutingDecision(const RoutingDecision& decision)
{
    wchar_t buffer[192] = {};
    wsprintfW(buffer,
        L"[UniKeyTSF][Routing][TSF] mode=%s owner=%s reason=%s seq=%lu\n",
        InputRoutingModeToString(decision.mode),
        InputRoutingOwnerToString(decision.owner),
        InputRoutingReasonToString(decision.reason),
        static_cast<unsigned long>(decision.decisionSequence));
    OutputDebugStringW(buffer);
}

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
    , _lastCommittedTick(0)
    , _hasLastAppliedConfig(false)
    , _tsfContextActive(false)
    , _lastRoutingOwner(ROUTING_OWNER_NONE)
{
    InterlockedIncrement(&g_cDllRef); // Keep DLL from unloading while active

    memset(&_config, 0, sizeof(_config));
    memset(&_lastAppliedConfig, 0, sizeof(_lastAppliedConfig));
    _config.inputEnabled = 1; // Default to enabled for now
    _config.inputMethod = IM_VNI;
    _currentAppId = ResolveCurrentProcessAppId();
    ApplyTypingSettingsToEngine(_config, _engine);
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
    _hMutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, UNIKEY_SHARED_MUTEX_NAME);

    if (!_hMapFile || !_hMutex) {
        // Log for debugging - manager EXE may not have started yet
        // Handles will be lazily re-opened in _UpdateConfig() when manager starts
        OutputDebugStringW(L"[UniKeyTSF][TSF] Warning: Shared config handles not available at activation. "
                          L"Using defaults until manager EXE creates shared memory.\n");
    }

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
    ITfDocumentMgr* pDocMgrFocus, ITfDocumentMgr* /*pDocMgrPrevFocus*/)
{
    _tsfContextActive = (pDocMgrFocus != nullptr);
    _currentAppId = ResolveCurrentProcessAppId();

    _isBlacklisted = !_currentAppId.empty() && IsProcessBlacklisted(_currentAppId);

    RoutingDecision routingDecision = RefreshTsfRoutingDecision(_tsfContextActive, _isBlacklisted);
    if (routingDecision.owner != _lastRoutingOwner || routingDecision.reason != ROUTING_REASON_HOOK_PRIMARY_DEFAULT) {
        TraceRoutingDecision(routingDecision);
    }
    _lastRoutingOwner = routingDecision.owner;

    if (_lastRoutingOwner != ROUTING_OWNER_TSF) {
        _engine.Clear();
        _lastCommittedTick = 0;
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
    RoutingDecision routingDecision = RefreshTsfRoutingDecision(_tsfContextActive, _isBlacklisted);
    _lastRoutingOwner = routingDecision.owner;
    return S_OK;
}

HRESULT CUniKeyTextService::OnTestKeyDown(
    ITfContext* /*pContext*/, WPARAM wParam, LPARAM /*lParam*/, BOOL* pfEaten)
{
    if (pfEaten) *pfEaten = FALSE;

    _UpdateConfig();
    const uint8_t effectiveInputEnabled = ResolveEffectiveInputEnabled(_config, _currentAppId);
    if (!effectiveInputEnabled) return S_OK;

    RoutingDecision routingDecision = RefreshTsfRoutingDecision(_tsfContextActive, _isBlacklisted);
    if (routingDecision.owner != _lastRoutingOwner || routingDecision.reason != ROUTING_REASON_HOOK_PRIMARY_DEFAULT) {
        TraceRoutingDecision(routingDecision);
    }
    _lastRoutingOwner = routingDecision.owner;
    if (routingDecision.owner != ROUTING_OWNER_TSF) {
        return S_OK;
    }

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
    const uint8_t effectiveInputEnabled = ResolveEffectiveInputEnabled(_config, _currentAppId);
    if (!effectiveInputEnabled) return S_OK;

    RoutingDecision routingDecision = RefreshTsfRoutingDecision(_tsfContextActive, _isBlacklisted);
    if (routingDecision.owner != _lastRoutingOwner || routingDecision.reason != ROUTING_REASON_HOOK_PRIMARY_DEFAULT) {
        TraceRoutingDecision(routingDecision);
    }
    _lastRoutingOwner = routingDecision.owner;
    if (routingDecision.owner != ROUTING_OWNER_TSF) {
        _engine.Clear();
        _lastCommittedTick = 0;
        return S_OK;
    }

    if (_engine.IsInWord()) {
        _lastCommittedTick = 0;
    }

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
            
            RequestEditSessionWithFallback(pContext, _tfClientId, pEditSession.Get(), TF_ES_READWRITE);
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
            
            RequestEditSessionWithFallback(pContext, _tfClientId, pEditSession.Get(), TF_ES_READWRITE);
            _engine.Clear();
            if (pfEaten) *pfEaten = TRUE;
            return S_OK;
        }
    }

    if (_engine.IsInWord() && IsNonTextCommitKey(wParam)) {
        if (_composition.IsComposing()) {
            ComPtr<CEditSession> pEditSession;
            pEditSession.Attach(new CEditSession([this, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                return _composition.EndComposition(ec);
            }));
            RequestEditSessionWithFallback(pContext, _tfClientId, pEditSession.Get(), TF_ES_READWRITE);
        }
        _engine.Clear();
        if (pfEaten) *pfEaten = FALSE;
        return S_OK;
    }

    // Convert key to character without consuming dead key state (using flag 4 in Windows 10+)
    BYTE keyboardState[256];
    GetKeyboardState(keyboardState);
    wchar_t ch;
    if (ToUnicodeEx((UINT)wParam, (UINT)(lParam >> 16) & 0xFF, keyboardState, &ch, 1, 4, GetKeyboardLayout(0)) > 0) {
        const InputMethod method = (InputMethod)_config.inputMethod;
        const bool isModifierKey = _engine.IsPotentialModifier(ch, method);
        const bool withinRecentCommitWindow =
            _lastCommittedTick != 0 &&
            (GetTickCount64() - _lastCommittedTick) <= kCommittedEditTimeoutMs;

        if (!_engine.IsInWord() &&
            _config.restoreKeyEnabled &&
            withinRecentCommitWindow &&
            isModifierKey) {
            std::shared_ptr<EditSessionFlag> updatedCommittedWord = std::make_shared<EditSessionFlag>();
            ComPtr<ITfContext> committedContext = pContext;
            ComPtr<CEditSession> pCommittedEditSession;
            pCommittedEditSession.Attach(new CEditSession(
                [this, committedContext, ch, method, withinRecentCommitWindow, updatedCommittedWord, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                    TF_SELECTION tfSelection = {};
                    ULONG fetched = 0;
                    if (FAILED(committedContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &fetched)) || fetched != 1) {
                        return E_FAIL;
                    }

                    BOOL isEmpty = FALSE;
                    if (FAILED(tfSelection.range->IsEmpty(ec, &isEmpty)) || !isEmpty) {
                        return S_FALSE;
                    }

                    ComPtr<ITfRange> pScanRange;
                    if (FAILED(tfSelection.range->Clone(&pScanRange))) {
                        return E_FAIL;
                    }

                    LONG shifted = 0;
                    pScanRange->ShiftStart(ec, -64, &shifted, nullptr);

                    wchar_t buf[65] = {0};
                    ULONG copied = 0;
                    if (FAILED(pScanRange->GetText(ec, 0, buf, 64, &copied))) {
                        return S_FALSE;
                    }

                    std::wstring leftText(buf, copied);

                    ComPtr<ITfRange> pRightRange;
                    if (FAILED(tfSelection.range->Clone(&pRightRange))) {
                        return E_FAIL;
                    }

                    shifted = 0;
                    pRightRange->ShiftEnd(ec, 64, &shifted, nullptr);

                    wchar_t rightBuf[65] = {0};
                    ULONG rightCopied = 0;
                    if (FAILED(pRightRange->GetText(ec, 0, rightBuf, 64, &rightCopied))) {
                        return S_FALSE;
                    }

                    const Reconversion::WordSpan committed = Reconversion::ExtractWordSpanAroundCaret(
                        leftText,
                        std::wstring(rightBuf, rightCopied),
                        method,
                        true);
                    if (!Reconversion::CanReplayCommittedModifier(
                            committed,
                            isEmpty == TRUE,
                            withinRecentCommitWindow)) {
                        return S_FALSE;
                    }

                    if (!_engine.ReplayContextKey(committed.word, ch, method)) {
                        _engine.ResetContext();
                        return S_FALSE;
                    }

                    const std::wstring updated = _engine.GetCompositionString();
                    _engine.ResetContext();

                    ComPtr<ITfRange> pReplaceRange;
                    if (FAILED(tfSelection.range->Clone(&pReplaceRange))) {
                        return E_FAIL;
                    }

                    const LONG startShift = static_cast<LONG>(committed.ReplacementStartShift());
                    const LONG endShift = static_cast<LONG>(committed.ReplacementEndShift());
                    pReplaceRange->ShiftStart(ec, startShift, &shifted, nullptr);
                    pReplaceRange->ShiftEnd(ec, endShift, &shifted, nullptr);

                    if (FAILED(pReplaceRange->SetText(ec, 0, updated.c_str(), static_cast<LONG>(updated.length())))) {
                        return E_FAIL;
                    }

                    updatedCommittedWord->value = true;
                    return S_OK;
                }));

            HRESULT hrCommittedEdit = RequestEditSessionWithFallback(
                pContext, _tfClientId, pCommittedEditSession.Get(), TF_ES_READWRITE);
            if (SUCCEEDED(hrCommittedEdit) && updatedCommittedWord->value) {
                _lastCommittedTick = 0;
                if (pfEaten) *pfEaten = TRUE;
                return S_OK;
            }
        }

        // Surrounding Text Assessment (Reconversion)
        if (!_engine.IsInWord() &&
            _config.restoreKeyEnabled &&
            (_engine.IsWordChar(ch, method) || isModifierKey)) {
            std::shared_ptr<EditSessionFlag> reopenedWord = std::make_shared<EditSessionFlag>();
            ComPtr<ITfContext> reconvertContext = pContext;
            ComPtr<CEditSession> pReconvertSession;
            pReconvertSession.Attach(new CEditSession([this, reconvertContext, ch, method, isModifierKey, reopenedWord, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                TF_SELECTION tfSelection = {};
                ULONG fetched = 0;
                if (FAILED(reconvertContext->GetSelection(ec, TF_DEFAULT_SELECTION, 1, &tfSelection, &fetched)) || fetched != 1) {
                    return E_FAIL;
                }

                BOOL isEmpty = FALSE;
                if (FAILED(tfSelection.range->IsEmpty(ec, &isEmpty)) || !isEmpty) {
                    return S_FALSE;
                }

                ComPtr<ITfRange> pScanRange;
                if (FAILED(tfSelection.range->Clone(&pScanRange))) {
                    return E_FAIL;
                }

                LONG shifted = 0;
                pScanRange->ShiftStart(ec, -64, &shifted, nullptr);

                wchar_t buf[65] = {0};
                ULONG copied = 0;
                if (FAILED(pScanRange->GetText(ec, 0, buf, 64, &copied))) {
                    return S_FALSE;
                }

                ComPtr<ITfRange> pRightRange;
                if (FAILED(tfSelection.range->Clone(&pRightRange))) {
                    return E_FAIL;
                }

                shifted = 0;
                pRightRange->ShiftEnd(ec, 64, &shifted, nullptr);

                wchar_t rightBuf[65] = {0};
                ULONG rightCopied = 0;
                if (FAILED(pRightRange->GetText(ec, 0, rightBuf, 64, &rightCopied))) {
                    return S_FALSE;
                }

                const Reconversion::WordSpan surrounding = Reconversion::ExtractWordSpanAroundCaret(
                    std::wstring(buf, copied),
                    std::wstring(rightBuf, rightCopied),
                    method,
                    false);
                if (!surrounding.HasWord()) {
                    return S_FALSE;
                }

                const std::wstring seed = isModifierKey
                    ? surrounding.word
                    : surrounding.word.substr(0, surrounding.leftEditableCount);

                if (!_engine.ReplayContextKey(seed, ch, method)) {
                    _engine.ResetContext();
                    return S_FALSE;
                }

                ComPtr<ITfRange> pWordRange;
                if (FAILED(tfSelection.range->Clone(&pWordRange))) {
                    _engine.ResetContext();
                    return E_FAIL;
                }

                const LONG startShift = isModifierKey
                    ? static_cast<LONG>(surrounding.ReplacementStartShift())
                    : -static_cast<LONG>(surrounding.leftEditableCount);
                const LONG endShift = isModifierKey
                    ? static_cast<LONG>(surrounding.ReplacementEndShift())
                    : 0;
                pWordRange->ShiftStart(ec, startShift, &shifted, nullptr);
                pWordRange->ShiftEnd(ec, endShift, &shifted, nullptr);
                pWordRange->SetText(ec, 0, L"", 0);
                pWordRange->Collapse(ec, TF_ANCHOR_END);

                TF_SELECTION sel = {};
                sel.range = pWordRange.Get();
                sel.style.ase = TF_AE_NONE;
                sel.style.fInterimChar = FALSE;
                reconvertContext->SetSelection(ec, 1, &sel);

                reopenedWord->value = true;
                return _composition.UpdateComposition(ec, reconvertContext.Get(), _engine.GetCompositionString());
            }));

            HRESULT hrReconvert = RequestEditSessionWithFallback(
                pContext, _tfClientId, pReconvertSession.Get(), TF_ES_READWRITE);
            if (SUCCEEDED(hrReconvert) && reopenedWord->value) {
                _lastCommittedTick = 0;
                if (pfEaten) *pfEaten = TRUE;
                return S_OK;
            }
        }

        if (_engine.ProcessKey(ch, method)) {
            _lastCommittedTick = 0;
            if (pfEaten) *pfEaten = TRUE;

            // Trigger Edit Session to update composition
            ComPtr<ITfContext> compositionContext = pContext;
            ComPtr<CEditSession> pEditSession;
            pEditSession.Attach(new CEditSession([this, compositionContext, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                return _composition.UpdateComposition(ec, compositionContext.Get(), _engine.GetCompositionString());
            }));

            RequestEditSessionWithFallback(pContext, _tfClientId, pEditSession.Get(), TF_ES_READWRITE);
        } else {
            // Engine didn't eat it (e.g. space, punctuation), end composition
            const bool hadCommittedWord = _engine.IsInWord();
            if (_composition.IsComposing()) {
                std::wstring currentWord = _engine.GetCompositionString();
                std::wstring expanded = _config.macroEnabled ? _macroEngine.Expand(currentWord) : L"";

                ComPtr<ITfContext> compositionContext = pContext;
                ComPtr<CEditSession> pEditSession;
                if (!expanded.empty()) {
                    pEditSession.Attach(new CEditSession([this, compositionContext, expanded, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                        _composition.UpdateComposition(ec, compositionContext.Get(), expanded);
                        return _composition.EndComposition(ec);
                    }));
                } else {
                    pEditSession.Attach(new CEditSession([this, self = SafeSvcRef(this)](TfEditCookie ec) -> HRESULT {
                        return _composition.EndComposition(ec);
                    }));
                }
                
                RequestEditSessionWithFallback(pContext, _tfClientId, pEditSession.Get(), TF_ES_READWRITE);
                _lastCommittedTick = GetTickCount64();
            } else if (hadCommittedWord) {
                _lastCommittedTick = GetTickCount64();
            } else {
                _lastCommittedTick = 0;
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
        _hMutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, UNIKEY_SHARED_MUTEX_NAME);
    }
    if (!_hMapFile || !_hMutex) return; // Still couldn't open synchronized shared config yet

    bool locked = false;
    // Keystroke handling must stay non-blocking; use the last cached config if the mutex is busy.
    DWORD waitResult = WaitForSingleObject(_hMutex, 0);
    locked = (waitResult == WAIT_OBJECT_0 || waitResult == WAIT_ABANDONED);
    if (!locked) {
        return;
    }

    void* pBuf = MapViewOfFile(_hMapFile, FILE_MAP_READ, 0, 0, sizeof(UniKeyConfig));
    if (pBuf != NULL) {
        // ATOMIC READ: Copy entire struct first, then validate
        UniKeyConfig latestConfig = {};
        memcpy(&latestConfig, pBuf, sizeof(UniKeyConfig));
        UnmapViewOfFile(pBuf);

        // Validate version AFTER atomic copy to prevent race window
        if (latestConfig.version == UNIKEY_CONFIG_VERSION) {
            // Validate field ranges to catch corruption
            if (latestConfig.inputMethod <= IM_VIQR &&
                latestConfig.charset <= CS_VNI_WIN &&
                latestConfig.toneType <= TONE_CLASSIC &&
                latestConfig.toggleKey <= TK_ALT_Z) {

                ApplyTypingSettingsToEngine(latestConfig, _engine);

                const MacroConfigDelta macroDelta = EvaluateMacroConfigDelta(
                    _hasLastAppliedConfig ? &_lastAppliedConfig : nullptr,
                    latestConfig);

                if (macroDelta.shouldClear) {
                    _macroEngine.Clear();
                    _currentMacroFile.clear();
                }
                if (macroDelta.shouldReload) {
                    _currentMacroFile = macroDelta.macroFilePath;
                    _macroEngine.Load(_currentMacroFile);
                }

                _config = latestConfig;
                _lastAppliedConfig = latestConfig;
                _hasLastAppliedConfig = true;
            }
            // Invalid ranges - silently ignore this update
        }
    }

    if (locked) {
        ReleaseMutex(_hMutex);
    }
}

