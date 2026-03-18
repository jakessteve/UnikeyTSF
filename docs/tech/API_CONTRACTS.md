# API Contracts — UniKey TSF Reborn

## IPC Contract: Shared Memory

All inter-process communication between `UniKeyTSF.exe` and the TSF DLLs uses a single packed struct in shared memory.

### Named Mapping
- **Name:** `Local\UniKeyTSF_SharedConfig`
- **Mutex:** `Local\UniKeyTSF_SharedMutex`
- **Size:** `sizeof(UniKeyConfig)` = 532 bytes
- **Access:** EXE creates and writes; DLLs open and read-only

### Data Structure (`shared_config.h`)
```cpp
#pragma pack(push, 1)

enum InputMethod : uint8_t { IM_TELEX = 0, IM_VNI = 1, IM_VIQR = 2 };
enum Charset      : uint8_t { CS_UNICODE = 0, CS_TCVN3 = 1, CS_VNI_WIN = 2 };
enum ToneType     : uint8_t { TONE_MODERN = 0, TONE_CLASSIC = 1 };
enum ToggleKey    : uint8_t { TK_CTRL_SHIFT = 0, TK_ALT_Z = 1 };

struct UniKeyConfig {
    uint32_t version;              // Struct version (currently 2)
    uint8_t  inputEnabled;         // 1 = Vietnamese, 0 = English
    uint8_t  inputMethod;          // InputMethod enum
    uint8_t  charset;              // Charset enum
    uint8_t  toneType;             // ToneType enum
    uint8_t  spellCheck;           // 1 = enabled
    uint8_t  macroEnabled;         // 1 = enabled
    uint8_t  freeToneMarking;      // 1 = enabled
    uint8_t  toggleKey;            // ToggleKey enum
    wchar_t  macroFilePath[260];   // Path to .ukm macro file
};

#pragma pack(pop)
// sizeof(UniKeyConfig) == 532 bytes
// UNIKEY_CONFIG_VERSION == 2
```

### Synchronization
- **Write:** Only `UniKeyTSF.exe` writes to this struct (single writer).
- **Read:** DLLs read atomically via aligned field access. Mutex used for multi-field updates.
- **Version field:** Used for forward compatibility. DLLs ignore configs with unknown versions.

## Engine API (`VnEngine`)

```cpp
class VnEngine {
    bool ProcessKey(wchar_t ch, InputMethod method);
    std::wstring GetCompositionString() const;
    std::wstring GetRawString() const;
    bool DidTransform() const;
    void Clear();
    bool IsInWord() const;
    void RemoveLastChar();
    void SetToneType(ToneType type);
    void SetFreeToneMarking(bool enabled);
    void SetSpellCheck(bool enabled);
};
```

## Process Blacklist

- **File:** `blacklist.txt` (next to the EXE)
- **Format:** One process name per line, `#` for comments
- **Functions:** `InitBlacklist()`, `LoadBlacklist()`, `SaveBlacklist()`, `ReloadBlacklist()`, `IsProcessBlacklisted()`
- **Defaults:** `cmd.exe`, `conhost.exe`, `WindowsTerminal.exe`, `mstsc.exe`, `ubuntu.exe`, `wsl.exe`

## COM Registration Contract

### Registry Keys (HKCU)
```
HKCU\Software\Classes\CLSID\{<CLSID>}
HKCU\Software\Microsoft\CTF\TIP\{<CLSID>}
```

### Required ACL
All registry keys MUST grant read access to:
- `ALL APPLICATION PACKAGES` (SID: `S-1-15-2-1`)

This ensures the IME works in UWP/AppContainer sandboxed processes.
