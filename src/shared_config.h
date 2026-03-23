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
    wchar_t  macroFilePath[260]; // Path to .ukm file
};
#pragma pack(pop)

static_assert(sizeof(UniKeyConfig) == 12 + 520, "UniKeyConfig must be exactly 532 bytes");

constexpr uint32_t UNIKEY_CONFIG_VERSION = 2;
