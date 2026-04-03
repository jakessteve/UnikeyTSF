#include "input_routing.h"

#include <windows.h>

namespace {

HANDLE g_routingMap = nullptr;
HANDLE g_routingMutex = nullptr;
InputRoutingState* g_routingState = nullptr;

constexpr DWORD kRoutingMutexTimeoutMs = 100;

RoutingDecision MakeFallbackDecision(InputRoutingOwner owner, bool hookContextActive, bool tsfContextActive, bool blacklisted)
{
    return RoutingDecision{
        ROUTING_HOOK_PRIMARY,
        owner,
        ROUTING_REASON_ROUTING_STATE_UNAVAILABLE,
        hookContextActive,
        tsfContextActive,
        blacklisted,
        0
    };
}

RoutingDecision FromState(const InputRoutingState& state)
{
    return RoutingDecision{
        static_cast<InputRoutingMode>(state.mode),
        static_cast<InputRoutingOwner>(state.owner),
        static_cast<InputRoutingReason>(state.reason),
        state.hookContextActive != 0,
        state.tsfContextActive != 0,
        state.blacklisted != 0,
        state.decisionSequence
    };
}

bool EnsureRoutingHandles(bool createIfMissing)
{
    if (!g_routingMutex) {
        if (createIfMissing) {
            g_routingMutex = CreateMutexW(nullptr, FALSE, UNIKEY_ROUTING_MUTEX_NAME);
        } else {
            g_routingMutex = OpenMutexW(SYNCHRONIZE | MUTEX_MODIFY_STATE, FALSE, UNIKEY_ROUTING_MUTEX_NAME);
        }
    }

    if (!g_routingMap) {
        if (createIfMissing) {
            g_routingMap = CreateFileMappingW(
                INVALID_HANDLE_VALUE,
                nullptr,
                PAGE_READWRITE,
                0,
                sizeof(InputRoutingState),
                UNIKEY_ROUTING_MEMORY_NAME);
        } else {
            g_routingMap = OpenFileMappingW(FILE_MAP_READ | FILE_MAP_WRITE, FALSE, UNIKEY_ROUTING_MEMORY_NAME);
        }
    }

    if (!g_routingState && g_routingMap) {
        g_routingState = static_cast<InputRoutingState*>(
            MapViewOfFile(g_routingMap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, sizeof(InputRoutingState)));
    }

    return g_routingMutex != nullptr && g_routingMap != nullptr && g_routingState != nullptr;
}

bool LockRouting()
{
    if (!g_routingMutex) {
        return false;
    }
    const DWORD wait = WaitForSingleObject(g_routingMutex, kRoutingMutexTimeoutMs);
    return wait == WAIT_OBJECT_0 || wait == WAIT_ABANDONED;
}

void UnlockRouting()
{
    if (g_routingMutex) {
        ReleaseMutex(g_routingMutex);
    }
}

void WriteRoutingStateLocked(const RoutingDecision& decision)
{
    if (!g_routingState) {
        return;
    }

    g_routingState->version = UNIKEY_ROUTING_STATE_VERSION;
    g_routingState->mode = static_cast<uint8_t>(decision.mode);
    g_routingState->owner = static_cast<uint8_t>(decision.owner);
    g_routingState->reason = static_cast<uint8_t>(decision.reason);
    g_routingState->hookContextActive = decision.hookContextActive ? 1 : 0;
    g_routingState->tsfContextActive = decision.tsfContextActive ? 1 : 0;
    g_routingState->blacklisted = decision.blacklisted ? 1 : 0;
    g_routingState->reserved[0] = 0;
    g_routingState->reserved[1] = 0;
    g_routingState->decisionSequence = decision.decisionSequence;
}

RoutingDecision ReadRoutingStateLocked()
{
    if (!g_routingState || g_routingState->version != UNIKEY_ROUTING_STATE_VERSION) {
        return MakeFallbackDecision(ROUTING_OWNER_HOOK, true, false, false);
    }
    return FromState(*g_routingState);
}

RoutingDecision MergeAndResolveDecision(
    const RoutingDecision& current,
    bool updateHookContext,
    bool hookContextActive,
    bool updateTsfContext,
    bool tsfContextActive,
    bool blacklisted)
{
    const bool mergedHookContext = updateHookContext ? hookContextActive : current.hookContextActive;
    const bool mergedTsfContext = updateTsfContext ? tsfContextActive : current.tsfContextActive;
    return ResolveRoutingDecision(current.mode, mergedHookContext, mergedTsfContext, blacklisted);
}

RoutingDecision RefreshDecision(
    bool updateHookContext,
    bool hookContextActive,
    bool updateTsfContext,
    bool tsfContextActive,
    bool blacklisted)
{
    if (!EnsureRoutingHandles(false)) {
        return MakeFallbackDecision(hookContextActive ? ROUTING_OWNER_HOOK : ROUTING_OWNER_NONE,
            hookContextActive,
            tsfContextActive,
            blacklisted);
    }

    if (!LockRouting()) {
        return MakeFallbackDecision(hookContextActive ? ROUTING_OWNER_HOOK : ROUTING_OWNER_NONE,
            hookContextActive,
            tsfContextActive,
            blacklisted);
    }

    RoutingDecision current = ReadRoutingStateLocked();
    RoutingDecision next = MergeAndResolveDecision(
        current,
        updateHookContext,
        hookContextActive,
        updateTsfContext,
        tsfContextActive,
        blacklisted);
    if (next.mode == current.mode &&
        next.owner == current.owner &&
        next.reason == current.reason &&
        next.hookContextActive == current.hookContextActive &&
        next.tsfContextActive == current.tsfContextActive &&
        next.blacklisted == current.blacklisted) {
        next.decisionSequence = current.decisionSequence;
    } else {
        next.decisionSequence = current.decisionSequence + 1;
    }
    WriteRoutingStateLocked(next);
    UnlockRouting();
    return next;
}

} // namespace

RoutingDecision ResolveRoutingDecision(
    InputRoutingMode mode,
    bool hookContextActive,
    bool tsfContextActive,
    bool blacklisted)
{
    if (blacklisted) {
        return RoutingDecision{mode, ROUTING_OWNER_NONE, ROUTING_REASON_BLACKLISTED_CONTEXT,
            hookContextActive, tsfContextActive, true, 0};
    }

    switch (mode) {
    case ROUTING_TSF_PRIMARY:
        if (tsfContextActive) {
            return RoutingDecision{mode, ROUTING_OWNER_TSF, ROUTING_REASON_TSF_PRIMARY_DEFAULT,
                hookContextActive, tsfContextActive, false, 0};
        }
        if (hookContextActive) {
            return RoutingDecision{mode, ROUTING_OWNER_HOOK, ROUTING_REASON_TSF_PRIMARY_HOOK_FALLBACK,
                hookContextActive, tsfContextActive, false, 0};
        }
        break;

    case ROUTING_FALLBACK_SCOPED:
        if (tsfContextActive) {
            return RoutingDecision{mode, ROUTING_OWNER_TSF, ROUTING_REASON_FALLBACK_SCOPED_TSF_CONTEXT,
                hookContextActive, tsfContextActive, false, 0};
        }
        if (hookContextActive) {
            return RoutingDecision{mode, ROUTING_OWNER_HOOK, ROUTING_REASON_FALLBACK_SCOPED_HOOK_CONTEXT,
                hookContextActive, tsfContextActive, false, 0};
        }
        break;

    case ROUTING_HOOK_PRIMARY:
    default:
        if (hookContextActive) {
            return RoutingDecision{ROUTING_HOOK_PRIMARY, ROUTING_OWNER_HOOK, ROUTING_REASON_HOOK_PRIMARY_DEFAULT,
                hookContextActive, tsfContextActive, false, 0};
        }
        if (tsfContextActive) {
            return RoutingDecision{ROUTING_HOOK_PRIMARY, ROUTING_OWNER_TSF, ROUTING_REASON_HOOK_PRIMARY_TSF_ONLY_CONTEXT,
                hookContextActive, tsfContextActive, false, 0};
        }
        break;
    }

    return RoutingDecision{mode, ROUTING_OWNER_NONE, ROUTING_REASON_NO_FOCUSED_CONTEXT,
        hookContextActive, tsfContextActive, false, 0};
}

bool InitializeInputRoutingCoordinator()
{
    if (!EnsureRoutingHandles(true)) {
        return false;
    }

    if (!LockRouting()) {
        return false;
    }

    const RoutingDecision bootDecision{
        ROUTING_HOOK_PRIMARY,
        ROUTING_OWNER_HOOK,
        ROUTING_REASON_MANAGER_STARTUP_DEFAULT,
        true,
        false,
        false,
        1
    };
    WriteRoutingStateLocked(bootDecision);
    UnlockRouting();
    return true;
}

void ShutdownInputRoutingCoordinator()
{
    if (g_routingState) {
        UnmapViewOfFile(g_routingState);
        g_routingState = nullptr;
    }
    if (g_routingMap) {
        CloseHandle(g_routingMap);
        g_routingMap = nullptr;
    }
    if (g_routingMutex) {
        CloseHandle(g_routingMutex);
        g_routingMutex = nullptr;
    }
}

bool SetInputRoutingMode(InputRoutingMode mode, InputRoutingReason reason)
{
    if (!EnsureRoutingHandles(false) || !LockRouting()) {
        return false;
    }

    const RoutingDecision current = ReadRoutingStateLocked();
    RoutingDecision next = ResolveRoutingDecision(
        mode,
        current.hookContextActive,
        current.tsfContextActive,
        current.blacklisted);

    if (next.owner == ROUTING_OWNER_NONE &&
        !current.hookContextActive &&
        !current.tsfContextActive &&
        !current.blacklisted &&
        reason != ROUTING_REASON_UNSPECIFIED) {
        next.reason = reason;
    }

    next.decisionSequence = current.decisionSequence + 1;
    WriteRoutingStateLocked(next);
    UnlockRouting();
    return true;
}

bool TryParseInputRoutingModeString(const wchar_t* text, InputRoutingMode* mode)
{
    if (!text || !mode) {
        return false;
    }

    if (_wcsicmp(text, L"hook_primary") == 0) {
        *mode = ROUTING_HOOK_PRIMARY;
        return true;
    }
    if (_wcsicmp(text, L"tsf_primary") == 0) {
        *mode = ROUTING_TSF_PRIMARY;
        return true;
    }
    if (_wcsicmp(text, L"fallback_scoped") == 0) {
        *mode = ROUTING_FALLBACK_SCOPED;
        return true;
    }

    return false;
}

InputRoutingReason InputRoutingConfiguredReason(InputRoutingMode mode)
{
    switch (mode) {
    case ROUTING_TSF_PRIMARY:
        return ROUTING_REASON_STARTUP_CONFIGURED_TSF_PRIMARY;
    case ROUTING_FALLBACK_SCOPED:
        return ROUTING_REASON_STARTUP_CONFIGURED_FALLBACK_SCOPED;
    case ROUTING_HOOK_PRIMARY:
    default:
        return ROUTING_REASON_STARTUP_CONFIGURED_HOOK_PRIMARY;
    }
}

RoutingDecision RefreshHookRoutingDecision(bool hookContextActive, bool blacklisted)
{
    return RefreshDecision(true, hookContextActive, false, false, blacklisted);
}

RoutingDecision RefreshTsfRoutingDecision(bool tsfContextActive, bool blacklisted)
{
    return RefreshDecision(false, false, true, tsfContextActive, blacklisted);
}

RoutingDecision GetInputRoutingDecisionSnapshot()
{
    if (!EnsureRoutingHandles(false) || !LockRouting()) {
        return MakeFallbackDecision(ROUTING_OWNER_HOOK, true, false, false);
    }

    RoutingDecision snapshot = ReadRoutingStateLocked();
    UnlockRouting();
    return snapshot;
}

const wchar_t* InputRoutingModeToString(InputRoutingMode mode)
{
    switch (mode) {
    case ROUTING_TSF_PRIMARY: return L"tsf_primary";
    case ROUTING_FALLBACK_SCOPED: return L"fallback_scoped";
    case ROUTING_HOOK_PRIMARY:
    default: return L"hook_primary";
    }
}

const wchar_t* InputRoutingOwnerToString(InputRoutingOwner owner)
{
    switch (owner) {
    case ROUTING_OWNER_HOOK: return L"hook";
    case ROUTING_OWNER_TSF: return L"tsf";
    case ROUTING_OWNER_NONE:
    default: return L"none";
    }
}

const wchar_t* InputRoutingReasonToString(InputRoutingReason reason)
{
    switch (reason) {
    case ROUTING_REASON_HOOK_PRIMARY_DEFAULT: return L"hook_primary_default";
    case ROUTING_REASON_TSF_PRIMARY_DEFAULT: return L"tsf_primary_default";
    case ROUTING_REASON_FALLBACK_SCOPED_HOOK_CONTEXT: return L"fallback_scoped_hook_context";
    case ROUTING_REASON_FALLBACK_SCOPED_TSF_CONTEXT: return L"fallback_scoped_tsf_context";
    case ROUTING_REASON_HOOK_PRIMARY_TSF_ONLY_CONTEXT: return L"hook_primary_tsf_only_context";
    case ROUTING_REASON_TSF_PRIMARY_HOOK_FALLBACK: return L"tsf_primary_hook_fallback";
    case ROUTING_REASON_BLACKLISTED_CONTEXT: return L"blacklisted_context";
    case ROUTING_REASON_NO_FOCUSED_CONTEXT: return L"no_focused_context";
    case ROUTING_REASON_ROUTING_STATE_UNAVAILABLE: return L"routing_state_unavailable";
    case ROUTING_REASON_MANAGER_STARTUP_DEFAULT: return L"manager_startup_default";
    case ROUTING_REASON_STARTUP_CONFIGURED_HOOK_PRIMARY: return L"startup_configured_hook_primary";
    case ROUTING_REASON_STARTUP_CONFIGURED_TSF_PRIMARY: return L"startup_configured_tsf_primary";
    case ROUTING_REASON_STARTUP_CONFIGURED_FALLBACK_SCOPED: return L"startup_configured_fallback_scoped";
    case ROUTING_REASON_UNSPECIFIED:
    default: return L"unspecified";
    }
}
