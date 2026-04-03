#pragma once
// =============================================================================
// UniKey TSF Reborn â€” Shared Memory Configuration
// =============================================================================
// This packed struct is shared between UniKeyTSF.exe and the TSF DLLs via
// a named file mapping (Local\UniKeyTSF_SharedConfig). The EXE is the sole
// writer; DLLs read-only.
// =============================================================================

#include <cstdint>
#include <windows.h>

// --- Shared Memory Name ---
constexpr const wchar_t* UNIKEY_SHARED_MEMORY_NAME = L"Local\\UniKeyTSF_SharedConfig";
constexpr const wchar_t* UNIKEY_SHARED_MUTEX_NAME  = L"Local\\UniKeyTSF_SharedMutex";

// --- Enums ---
enum InputMethod : uint8_t {
    IM_TELEX = 0,
    IM_VNI   = 1,
    IM_VIQR  = 2,
};

enum Charset : uint8_t {
    CS_UNICODE = 0,
    CS_TCVN3   = 1,
    CS_VNI_WIN = 2,
};

enum ToneType : uint8_t {
    TONE_MODERN  = 0,  // oÃ  (new-style)
    TONE_CLASSIC = 1,  // Ã²a (old-style)
};

enum ToggleKey : uint8_t {
    TK_CTRL_SHIFT = 0,  // Ctrl+Shift (default)
    TK_ALT_Z      = 1,  // Alt+Z
};

enum InputRoutingMode : uint8_t {
    ROUTING_HOOK_PRIMARY   = 0,
    ROUTING_TSF_PRIMARY    = 1,
    ROUTING_FALLBACK_SCOPED = 2,
};

enum InputRoutingOwner : uint8_t {
    ROUTING_OWNER_NONE = 0,
    ROUTING_OWNER_HOOK = 1,
    ROUTING_OWNER_TSF  = 2,
};

enum InputRoutingReason : uint8_t {
    ROUTING_REASON_UNSPECIFIED = 0,
    ROUTING_REASON_HOOK_PRIMARY_DEFAULT = 1,
    ROUTING_REASON_TSF_PRIMARY_DEFAULT = 2,
    ROUTING_REASON_FALLBACK_SCOPED_HOOK_CONTEXT = 3,
    ROUTING_REASON_FALLBACK_SCOPED_TSF_CONTEXT = 4,
    ROUTING_REASON_HOOK_PRIMARY_TSF_ONLY_CONTEXT = 5,
    ROUTING_REASON_TSF_PRIMARY_HOOK_FALLBACK = 6,
    ROUTING_REASON_BLACKLISTED_CONTEXT = 7,
    ROUTING_REASON_NO_FOCUSED_CONTEXT = 8,
    ROUTING_REASON_ROUTING_STATE_UNAVAILABLE = 9,
    ROUTING_REASON_MANAGER_STARTUP_DEFAULT = 10,
    ROUTING_REASON_STARTUP_CONFIGURED_HOOK_PRIMARY = 11,
    ROUTING_REASON_STARTUP_CONFIGURED_TSF_PRIMARY = 12,
    ROUTING_REASON_STARTUP_CONFIGURED_FALLBACK_SCOPED = 13,
};

constexpr const wchar_t* UNIKEY_ROUTING_MEMORY_NAME = L"Local\\UniKeyTSF_InputRouting";
constexpr const wchar_t* UNIKEY_ROUTING_MUTEX_NAME  = L"Local\\UniKeyTSF_InputRoutingMutex";
constexpr uint32_t UNIKEY_ROUTING_STATE_VERSION = 1;

// --- Packed Config Struct ---
#pragma pack(push, 1)
struct UniKeyConfig {
    uint32_t version;          // Struct version (currently 2)
    uint8_t  inputEnabled;     // 1 = Vietnamese, 0 = English
    uint8_t  inputMethod;      // InputMethod enum
    uint8_t  charset;          // Charset enum
    uint8_t  toneType;         // ToneType enum
    uint8_t  spellCheck;       // 1 = enabled
    uint8_t  macroEnabled;     // 1 = enabled
    uint8_t  freeToneMarking;  // 1 = enabled
    uint8_t  toggleKey;        // ToggleKey enum (0=Ctrl+Shift, 1=Alt+Z)
    uint8_t  restoreKeyEnabled;      // 1 = enabled
    uint8_t  useClipboardForUnicode; // 1 = enabled
    uint8_t  showDialogOnStartup;    // 1 = enabled
    uint8_t  perAppInputState;       // 1 = enabled
    wchar_t  macroFilePath[260]; // Path to .ukm file
};

struct InputRoutingState {
    uint32_t version;
    uint8_t  mode;
    uint8_t  owner;
    uint8_t  reason;
    uint8_t  hookContextActive;
    uint8_t  tsfContextActive;
    uint8_t  blacklisted;
    uint8_t  reserved[2];
    uint32_t decisionSequence;
};
#pragma pack(pop)

static_assert(sizeof(UniKeyConfig) == 16 + 520, "UniKeyConfig must be exactly 536 bytes");

constexpr uint32_t UNIKEY_CONFIG_VERSION = 2;
static_assert(sizeof(InputRoutingState) == 16, "InputRoutingState must be exactly 16 bytes");
