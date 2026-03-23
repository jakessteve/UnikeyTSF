#pragma once
// =============================================================================
// UniKey TSF Reborn â€” GUID Definitions
// =============================================================================
// CLSIDs and Profile GUIDs for the TSF Text Input Processor.
// These must be unique and consistent across DLL/EXE.
// =============================================================================

#include <guiddef.h>

// CLSID for the UniKey TSF Text Service
// {7B3E4F1A-2C8D-4A5B-9E6F-0D1C2B3A4E5F}
DEFINE_GUID(CLSID_UniKeyTSF,
    0x7b3e4f1a, 0x2c8d, 0x4a5b,
    0x9e, 0x6f, 0x0d, 0x1c, 0x2b, 0x3a, 0x4e, 0x5f);

// Profile GUID for the Vietnamese language profile
// {8C4F5A2B-3D9E-4B6C-AF70-1E2D3C4B5A6E}
DEFINE_GUID(GUID_UniKeyTSF_Profile,
    0x8c4f5a2b, 0x3d9e, 0x4b6c,
    0xaf, 0x70, 0x1e, 0x2d, 0x3c, 0x4b, 0x5a, 0x6e);

// Vietnamese language ID
constexpr LANGID UNIKEY_LANGID = MAKELANGID(LANG_VIETNAMESE, SUBLANG_VIETNAMESE_VIETNAM);
// Vietnamese keyboard language ID (0x042A)
constexpr WORD UNIKEY_LANG_PROFILE = 0x042A;
