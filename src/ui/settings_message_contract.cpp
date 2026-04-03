#include "settings_message_contract.h"

#include <cwchar>
#include <sstream>

namespace {

bool TryFindValueStart(const std::wstring& json, const wchar_t* key, size_t* valueStart)
{
    if (!valueStart) {
        return false;
    }

    const std::wstring needle = std::wstring(L"\"") + key + L"\":";
    size_t pos = json.find(needle);
    if (pos == std::wstring::npos) {
        return false;
    }

    pos += needle.length();
    while (pos < json.length() && iswspace(json[pos])) {
        ++pos;
    }

    *valueStart = pos;
    return true;
}

bool TryExtractBool(const std::wstring& json, const wchar_t* key, bool* value)
{
    if (!value) {
        return false;
    }

    size_t pos = 0;
    if (!TryFindValueStart(json, key, &pos)) {
        return false;
    }

    if (json.compare(pos, 4, L"true") == 0) {
        *value = true;
        return true;
    }

    if (json.compare(pos, 5, L"false") == 0) {
        *value = false;
        return true;
    }

    return false;
}

bool TryExtractInt(const std::wstring& json, const wchar_t* key, int* value)
{
    if (!value) {
        return false;
    }

    size_t pos = 0;
    if (!TryFindValueStart(json, key, &pos)) {
        return false;
    }

    wchar_t* endPtr = nullptr;
    const long parsed = wcstol(json.c_str() + pos, &endPtr, 10);
    if (endPtr == json.c_str() + pos) {
        return false;
    }

    *value = static_cast<int>(parsed);
    return true;
}

bool TryExtractString(const std::wstring& json, const wchar_t* key, std::wstring* value)
{
    if (!value) {
        return false;
    }

    size_t pos = 0;
    if (!TryFindValueStart(json, key, &pos) || pos >= json.length() || json[pos] != L'"') {
        return false;
    }

    ++pos;
    std::wstring result;
    while (pos < json.length()) {
        const wchar_t ch = json[pos];
        if (ch == L'\\') {
            if (pos + 1 >= json.length()) {
                return false;
            }
            result += json[pos + 1];
            pos += 2;
            continue;
        }
        if (ch == L'"') {
            *value = result;
            return true;
        }
        result += ch;
        ++pos;
    }

    return false;
}

bool TryExtractObject(const std::wstring& json, const wchar_t* key, std::wstring* value)
{
    if (!value) {
        return false;
    }

    size_t pos = 0;
    if (!TryFindValueStart(json, key, &pos) || pos >= json.length() || json[pos] != L'{') {
        return false;
    }

    int depth = 0;
    bool inString = false;
    bool escaped = false;
    const size_t start = pos;
    for (; pos < json.length(); ++pos) {
        const wchar_t ch = json[pos];
        if (inString) {
            if (escaped) {
                escaped = false;
            } else if (ch == L'\\') {
                escaped = true;
            } else if (ch == L'"') {
                inString = false;
            }
            continue;
        }

        if (ch == L'"') {
            inString = true;
        } else if (ch == L'{') {
            ++depth;
        } else if (ch == L'}') {
            --depth;
            if (depth == 0) {
                *value = json.substr(start, pos - start + 1);
                return true;
            }
        }
    }

    return false;
}

bool TryExtractStringArray(const std::wstring& json, const wchar_t* key, std::vector<std::wstring>* values)
{
    if (!values) {
        return false;
    }

    size_t pos = 0;
    if (!TryFindValueStart(json, key, &pos) || pos >= json.length() || json[pos] != L'[') {
        return false;
    }

    ++pos;
    values->clear();
    while (pos < json.length()) {
        while (pos < json.length() && iswspace(json[pos])) {
            ++pos;
        }
        if (pos >= json.length()) {
            return false;
        }
        if (json[pos] == L']') {
            return true;
        }
        if (json[pos] != L'"') {
            return false;
        }

        ++pos;
        std::wstring entry;
        while (pos < json.length()) {
            const wchar_t ch = json[pos];
            if (ch == L'\\') {
                if (pos + 1 >= json.length()) {
                    return false;
                }
                entry += json[pos + 1];
                pos += 2;
                continue;
            }
            if (ch == L'"') {
                ++pos;
                break;
            }
            entry += ch;
            ++pos;
        }

        values->push_back(entry);

        while (pos < json.length() && iswspace(json[pos])) {
            ++pos;
        }
        if (pos < json.length() && json[pos] == L',') {
            ++pos;
        }
    }

    return false;
}

bool IsValidInputMethod(int value)
{
    return value >= IM_TELEX && value <= IM_VIQR;
}

bool IsValidCharset(int value)
{
    return value >= CS_UNICODE && value <= CS_VNI_WIN;
}

bool IsValidToneType(int value)
{
    return value >= TONE_MODERN && value <= TONE_CLASSIC;
}

bool IsValidToggleKey(int value)
{
    return value >= TK_CTRL_SHIFT && value <= TK_ALT_Z;
}

std::wstring EscapeJsonString(const std::wstring& value)
{
    std::wstringstream ss;
    for (wchar_t ch : value) {
        switch (ch) {
        case L'\\': ss << L"\\\\"; break;
        case L'"': ss << L"\\\""; break;
        case L'\n': ss << L"\\n"; break;
        case L'\r': ss << L"\\r"; break;
        case L'\t': ss << L"\\t"; break;
        default: ss << ch; break;
        }
    }
    return ss.str();
}

bool ParseConfigPayload(const std::wstring& payload, UniKeyConfig* config)
{
    if (!config) {
        return false;
    }

    int version = 0;
    int inputMethod = 0;
    int charset = 0;
    int toneType = 0;
    int toggleKey = 0;
    bool inputEnabled = false;
    bool spellCheck = false;
    bool macroEnabled = false;
    bool freeToneMarking = false;
    bool restoreKeyEnabled = false;
    bool useClipboardForUnicode = false;
    bool showDialogOnStartup = false;
    bool perAppInputState = false;
    std::wstring macroFilePath;

    if (!TryExtractInt(payload, L"version", &version) || version != static_cast<int>(UNIKEY_CONFIG_VERSION)) {
        return false;
    }
    if (!TryExtractBool(payload, L"inputEnabled", &inputEnabled) ||
        !TryExtractInt(payload, L"inputMethod", &inputMethod) || !IsValidInputMethod(inputMethod) ||
        !TryExtractInt(payload, L"charset", &charset) || !IsValidCharset(charset) ||
        !TryExtractInt(payload, L"toneType", &toneType) || !IsValidToneType(toneType) ||
        !TryExtractBool(payload, L"spellCheck", &spellCheck) ||
        !TryExtractBool(payload, L"macroEnabled", &macroEnabled) ||
        !TryExtractBool(payload, L"freeToneMarking", &freeToneMarking) ||
        !TryExtractInt(payload, L"toggleKey", &toggleKey) || !IsValidToggleKey(toggleKey) ||
        !TryExtractBool(payload, L"restoreKeyEnabled", &restoreKeyEnabled) ||
        !TryExtractBool(payload, L"useClipboardForUnicode", &useClipboardForUnicode) ||
        !TryExtractBool(payload, L"showDialogOnStartup", &showDialogOnStartup) ||
        !TryExtractBool(payload, L"perAppInputState", &perAppInputState) ||
        !TryExtractString(payload, L"macroFilePath", &macroFilePath)) {
        return false;
    }

    UniKeyConfig parsed = {};
    parsed.version = UNIKEY_CONFIG_VERSION;
    parsed.inputEnabled = inputEnabled ? 1 : 0;
    parsed.inputMethod = static_cast<uint8_t>(inputMethod);
    parsed.charset = static_cast<uint8_t>(charset);
    parsed.toneType = static_cast<uint8_t>(toneType);
    parsed.spellCheck = spellCheck ? 1 : 0;
    parsed.macroEnabled = macroEnabled ? 1 : 0;
    parsed.freeToneMarking = freeToneMarking ? 1 : 0;
    parsed.toggleKey = static_cast<uint8_t>(toggleKey);
    parsed.restoreKeyEnabled = restoreKeyEnabled ? 1 : 0;
    parsed.useClipboardForUnicode = useClipboardForUnicode ? 1 : 0;
    parsed.showDialogOnStartup = showDialogOnStartup ? 1 : 0;
    parsed.perAppInputState = perAppInputState ? 1 : 0;
    wcsncpy_s(parsed.macroFilePath, macroFilePath.c_str(), _TRUNCATE);
    *config = parsed;
    return true;
}

std::wstring BuildConfigPayloadJson(const UniKeyConfig& config)
{
    std::wstringstream ss;
    ss << L"{"
       << L"\"version\":" << config.version << L","
       << L"\"inputEnabled\":" << (config.inputEnabled ? L"true" : L"false") << L","
       << L"\"inputMethod\":" << static_cast<int>(config.inputMethod) << L","
       << L"\"charset\":" << static_cast<int>(config.charset) << L","
       << L"\"toneType\":" << static_cast<int>(config.toneType) << L","
       << L"\"spellCheck\":" << (config.spellCheck ? L"true" : L"false") << L","
       << L"\"macroEnabled\":" << (config.macroEnabled ? L"true" : L"false") << L","
       << L"\"freeToneMarking\":" << (config.freeToneMarking ? L"true" : L"false") << L","
       << L"\"toggleKey\":" << static_cast<int>(config.toggleKey) << L","
       << L"\"restoreKeyEnabled\":" << (config.restoreKeyEnabled ? L"true" : L"false") << L","
       << L"\"useClipboardForUnicode\":" << (config.useClipboardForUnicode ? L"true" : L"false") << L","
       << L"\"showDialogOnStartup\":" << (config.showDialogOnStartup ? L"true" : L"false") << L","
       << L"\"perAppInputState\":" << (config.perAppInputState ? L"true" : L"false") << L","
       << L"\"macroFilePath\":\"" << EscapeJsonString(config.macroFilePath) << L"\""
       << L"}";
    return ss.str();
}

} // namespace

bool ParseSettingsHostMessage(const std::wstring& message, SettingsHostMessage* parsed)
{
    if (!parsed) {
        return false;
    }

    SettingsHostMessage result;
    std::wstring type;
    if (!TryExtractString(message, L"type", &type)) {
        return false;
    }

    if (type == L"UPDATE_CONFIG") {
        std::wstring payload;
        if (!TryExtractObject(message, L"payload", &payload) || !ParseConfigPayload(payload, &result.config)) {
            return false;
        }
        result.type = SettingsHostMessageType::UpdateConfig;
    } else if (type == L"UPDATE_BLACKLIST") {
        if (!TryExtractStringArray(message, L"value", &result.blacklist)) {
            return false;
        }
        result.type = SettingsHostMessageType::UpdateBlacklist;
    } else if (type == L"REQ_CONFIG") {
        result.type = SettingsHostMessageType::RequestConfig;
    } else if (type == L"UI_READY") {
        result.type = SettingsHostMessageType::UiReady;
    } else if (type == L"SET_AUTOSTART") {
        if (!TryExtractBool(message, L"enabled", &result.enabled)) {
            return false;
        }
        result.type = SettingsHostMessageType::SetAutostart;
    } else if (type == L"REQ_BLACKLIST") {
        result.type = SettingsHostMessageType::RequestBlacklist;
    } else {
        return false;
    }

    *parsed = result;
    return true;
}

std::wstring BuildSettingsConfigMessage(const UniKeyConfig& config)
{
    return L"{\"type\":\"SET_CONFIG\",\"payload\":" + BuildConfigPayloadJson(config) + L"}";
}

std::wstring BuildAutostartStateMessage(bool enabled)
{
    return std::wstring(L"{\"type\":\"AUTOSTART_STATE\",\"enabled\":") + (enabled ? L"true}" : L"false}");
}

std::wstring BuildBlacklistDataMessage(const std::vector<std::wstring>& list)
{
    std::wstringstream ss;
    ss << L"{\"type\":\"BLACKLIST_DATA\",\"value\":[";
    for (size_t i = 0; i < list.size(); ++i) {
        if (i > 0) {
            ss << L',';
        }
        ss << L'\"' << EscapeJsonString(list[i]) << L'\"';
    }
    ss << L"]}";
    return ss.str();
}
