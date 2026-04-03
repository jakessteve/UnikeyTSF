// =============================================================================
// UniKey TSF Reborn — Clipboard Toolkit
// =============================================================================
// Provides clipboard-based text transformation utilities:
//   - Accent stripping (Ctrl+Shift+F9)
//   - Charset conversion (Ctrl+Shift+F6)
// =============================================================================

#include "clipboard.h"
#include <vector>
#include <algorithm>
#include <unordered_map>

namespace Toolkit {

// =============================================================================
// Vietnamese character mapping table
// =============================================================================

static const wchar_t* vn_chars = 
    L"àáảãạăằắẳẵặâầấẩẫậ"
    L"èéẻẽẹêềếểễệ"
    L"ìíỉĩị"
    L"òóỏõọôồốổỗộơờớởỡợ"
    L"ùúủũụưừứửữự"
    L"ỳýỷỹỵ"
    L"đ"
    L"ÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬ"
    L"ÈÉẺẼẸÊỀẾỂỄỆ"
    L"ÌÍỈĨỊ"
    L"ÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢ"
    L"ÙÚỦŨỤƯỪỨỬỮỰ"
    L"ỲÝỶỸỴ"
    L"Đ";

static const wchar_t* ascii_chars =
    L"aaaaaaaaaaaaaaaaa"
    L"eeeeeeeeeee"
    L"iiiii"
    L"ooooooooooooooooo"
    L"uuuuuuuuuuu"
    L"yyyyy"
    L"d"
    L"AAAAAAAAAAAAAAAAA"
    L"EEEEEEEEEEE"
    L"IIIII"
    L"OOOOOOOOOOOOOOOOO"
    L"UUUUUUUUUUU"
    L"YYYYY"
    L"D";

// =============================================================================
// TCVN3 Mapping Table (subset — Unicode <-> TCVN3 code page)
// =============================================================================
// TCVN3 (ABC) uses a single-byte encoding. The mapping covers all Vietnamese
// characters. Each entry maps Unicode codepoint -> TCVN3 byte value.

struct CharsetMapping {
    wchar_t unicode;
    unsigned char tcvn3;
};

// Lowercase Vietnamese chars → TCVN3 byte values
static const CharsetMapping g_UnicodeTcvn3[] = {
    // a variants
    {L'à', 0xB5}, {L'á', 0xB8}, {L'ả', 0xB6}, {L'ã', 0xB7}, {L'ạ', 0xB9},
    {L'ă', 0xA8}, {L'ằ', 0xBB}, {L'ắ', 0xBE}, {L'ẳ', 0xBC}, {L'ẵ', 0xBD}, {L'ặ', 0xC6},
    {L'â', 0xA9}, {L'ầ', 0xC7}, {L'ấ', 0xCA}, {L'ẩ', 0xC8}, {L'ẫ', 0xC9}, {L'ậ', 0xCB},
    // e variants
    {L'è', 0xCC}, {L'é', 0xCE}, {L'ẻ', 0xCD}, {L'ẽ', 0xD0}, {L'ẹ', 0xCF},
    {L'ê', 0xAA}, {L'ề', 0xD1}, {L'ế', 0xD4}, {L'ể', 0xD2}, {L'ễ', 0xD3}, {L'ệ', 0xD5},
    // i variants
    {L'ì', 0xD6}, {L'í', 0xD8}, {L'ỉ', 0xD7}, {L'ĩ', 0xD9}, {L'ị', 0xDA},
    // o variants
    {L'ò', 0xDB}, {L'ó', 0xDD}, {L'ỏ', 0xDC}, {L'õ', 0xDE}, {L'ọ', 0xDF},
    {L'ô', 0xAB}, {L'ồ', 0xE0}, {L'ố', 0xE3}, {L'ổ', 0xE1}, {L'ỗ', 0xE2}, {L'ộ', 0xE4},
    {L'ơ', 0xAC}, {L'ờ', 0xE5}, {L'ớ', 0xE8}, {L'ở', 0xE6}, {L'ỡ', 0xE7}, {L'ợ', 0xE9},
    // u variants
    {L'ù', 0xEA}, {L'ú', 0xEC}, {L'ủ', 0xEB}, {L'ũ', 0xED}, {L'ụ', 0xEE},
    {L'ư', 0xAF}, {L'ừ', 0xEF}, {L'ứ', 0xF2}, {L'ử', 0xF0}, {L'ữ', 0xF1}, {L'ự', 0xF3},
    // y variants
    {L'ỳ', 0xF4}, {L'ý', 0xF6}, {L'ỷ', 0xF5}, {L'ỹ', 0xF8}, {L'ỵ', 0xF7},
    // d
    {L'đ', 0xAE},
    // Uppercase — derived by subtracting 0x20 from lowercase TCVN3 values (VPS convention)
    {L'À', 0x95}, {L'Á', 0x98}, {L'Ả', 0x96}, {L'Ã', 0x97}, {L'Ạ', 0x99},
    {L'Ă', 0x88}, {L'Ằ', 0x9B}, {L'Ắ', 0x9E}, {L'Ẳ', 0x9C}, {L'Ẵ', 0x9D}, {L'Ặ', 0xA6},
    {L'Â', 0x89}, {L'Ầ', 0xA7}, {L'Ấ', 0xAA}, {L'Ẩ', 0xA8}, {L'Ẫ', 0xA9}, {L'Ậ', 0xAB},
    {L'È', 0xAC}, {L'É', 0xAE}, {L'Ẻ', 0xAD}, {L'Ẽ', 0xB0}, {L'Ẹ', 0xAF},
    {L'Ê', 0x8A}, {L'Ề', 0xB1}, {L'Ế', 0xB4}, {L'Ể', 0xB2}, {L'Ễ', 0xB3}, {L'Ệ', 0xB5},
    {L'Ì', 0xB6}, {L'Í', 0xB8}, {L'Ỉ', 0xB7}, {L'Ĩ', 0xB9}, {L'Ị', 0xBA},
    {L'Ò', 0xBB}, {L'Ó', 0xBD}, {L'Ỏ', 0xBC}, {L'Õ', 0xBE}, {L'Ọ', 0xBF},
    {L'Ô', 0x8B}, {L'Ồ', 0xC0}, {L'Ố', 0xC3}, {L'Ổ', 0xC1}, {L'Ỗ', 0xC2}, {L'Ộ', 0xC4},
    {L'Ơ', 0x8C}, {L'Ờ', 0xC5}, {L'Ớ', 0xC8}, {L'Ở', 0xC6}, {L'Ỡ', 0xC7}, {L'Ợ', 0xC9},
    {L'Ù', 0xCA}, {L'Ú', 0xCC}, {L'Ủ', 0xCB}, {L'Ũ', 0xCD}, {L'Ụ', 0xCE},
    {L'Ư', 0x8F}, {L'Ừ', 0xCF}, {L'Ứ', 0xD2}, {L'Ử', 0xD0}, {L'Ữ', 0xD1}, {L'Ự', 0xD3},
    {L'Ỳ', 0xD4}, {L'Ý', 0xD6}, {L'Ỷ', 0xD5}, {L'Ỹ', 0xD8}, {L'Ỵ', 0xD7},
    {L'Đ', 0x8E},
};

static const size_t g_NumMappings = sizeof(g_UnicodeTcvn3) / sizeof(g_UnicodeTcvn3[0]);

// =============================================================================
// VNI Windows Mapping Table (Unicode <-> 2-byte VNI encoding)
// =============================================================================
// VNI Windows encodes Vietnamese characters as 2 bytes:
//   Byte 1 = Base ASCII letter (a, e, i, o, u, y, d)
//   Byte 2 = Diacritical mark modifier
// For non-Vietnamese ASCII chars, only 1 byte is used (same as ASCII).

struct VniMapping {
    wchar_t unicode;    // Unicode codepoint
    wchar_t vniBase;    // VNI base character (ASCII letter)
    wchar_t vniMark;    // VNI diacritical byte (0 if single-byte)
};

// Complete VNI Windows mapping — lowercase
static const VniMapping g_UnicodeVniLower[] = {
    // a + tone
    {L'à', L'a', 0xF8}, {L'á', L'a', 0xF9}, {L'ả', L'a', 0xF6}, {L'ã', L'a', 0xF5}, {L'ạ', L'a', 0xFA},
    // ă (a breve) + tone
    {L'ă', L'a', 0xE5}, {L'ằ', L'a', 0xEB}, {L'ắ', L'a', 0xE9}, {L'ẳ', L'a', 0xEC}, {L'ẵ', L'a', 0xEA}, {L'ặ', L'a', 0xED},
    // â (a circumflex) + tone
    {L'â', L'a', 0xE2}, {L'ầ', L'a', 0xFB}, {L'ấ', L'a', 0xFE}, {L'ẩ', L'a', 0xFC}, {L'ẫ', L'a', 0xFD}, {L'ậ', L'a', 0xFF},
    // e + tone
    {L'è', L'e', 0xF8}, {L'é', L'e', 0xF9}, {L'ẻ', L'e', 0xF6}, {L'ẽ', L'e', 0xF5}, {L'ẹ', L'e', 0xFA},
    // ê (e circumflex) + tone
    {L'ê', L'e', 0xE2}, {L'ề', L'e', 0xFB}, {L'ế', L'e', 0xFE}, {L'ể', L'e', 0xFC}, {L'ễ', L'e', 0xFD}, {L'ệ', L'e', 0xFF},
    // i + tone
    {L'ì', L'i', 0xF8}, {L'í', L'i', 0xF9}, {L'ỉ', L'i', 0xF6}, {L'ĩ', L'i', 0xF5}, {L'ị', L'i', 0xFA},
    // o + tone
    {L'ò', L'o', 0xF8}, {L'ó', L'o', 0xF9}, {L'ỏ', L'o', 0xF6}, {L'õ', L'o', 0xF5}, {L'ọ', L'o', 0xFA},
    // ô (o circumflex) + tone
    {L'ô', L'o', 0xE2}, {L'ồ', L'o', 0xFB}, {L'ố', L'o', 0xFE}, {L'ổ', L'o', 0xFC}, {L'ỗ', L'o', 0xFD}, {L'ộ', L'o', 0xFF},
    // ơ (o horn) + tone
    {L'ơ', L'o', 0xE1}, {L'ờ', L'o', 0xEE}, {L'ớ', L'o', 0xF4}, {L'ở', L'o', 0xF1}, {L'ỡ', L'o', 0xEF}, {L'ợ', L'o', 0xF7},
    // u + tone
    {L'ù', L'u', 0xF8}, {L'ú', L'u', 0xF9}, {L'ủ', L'u', 0xF6}, {L'ũ', L'u', 0xF5}, {L'ụ', L'u', 0xFA},
    // ư (u horn) + tone
    {L'ư', L'u', 0xE1}, {L'ừ', L'u', 0xEE}, {L'ứ', L'u', 0xF4}, {L'ử', L'u', 0xF1}, {L'ữ', L'u', 0xEF}, {L'ự', L'u', 0xF7},
    // y + tone
    {L'ỳ', L'y', 0xF8}, {L'ý', L'y', 0xF9}, {L'ỷ', L'y', 0xF6}, {L'ỹ', L'y', 0xF5}, {L'ỵ', L'y', 0xFA},
    // đ
    {L'đ', L'd', 0xF0},
};
static const size_t g_NumVniLower = sizeof(g_UnicodeVniLower) / sizeof(g_UnicodeVniLower[0]);

// Complete VNI Windows mapping — uppercase
static const VniMapping g_UnicodeVniUpper[] = {
    // A + tone
    {L'À', L'A', 0xD8}, {L'Á', L'A', 0xD9}, {L'Ả', L'A', 0xD6}, {L'Ã', L'A', 0xD5}, {L'Ạ', L'A', 0xDA},
    // Ă (A breve) + tone
    {L'Ă', L'A', 0xC5}, {L'Ằ', L'A', 0xCB}, {L'Ắ', L'A', 0xC9}, {L'Ẳ', L'A', 0xCC}, {L'Ẵ', L'A', 0xCA}, {L'Ặ', L'A', 0xCD},
    // Â (A circumflex) + tone
    {L'Â', L'A', 0xC2}, {L'Ầ', L'A', 0xDB}, {L'Ấ', L'A', 0xDE}, {L'Ẩ', L'A', 0xDC}, {L'Ẫ', L'A', 0xDD}, {L'Ậ', L'A', 0xDF},
    // E + tone
    {L'È', L'E', 0xD8}, {L'É', L'E', 0xD9}, {L'Ẻ', L'E', 0xD6}, {L'Ẽ', L'E', 0xD5}, {L'Ẹ', L'E', 0xDA},
    // Ê (E circumflex) + tone
    {L'Ê', L'E', 0xC2}, {L'Ề', L'E', 0xDB}, {L'Ế', L'E', 0xDE}, {L'Ể', L'E', 0xDC}, {L'Ễ', L'E', 0xDD}, {L'Ệ', L'E', 0xDF},
    // I + tone
    {L'Ì', L'I', 0xD8}, {L'Í', L'I', 0xD9}, {L'Ỉ', L'I', 0xD6}, {L'Ĩ', L'I', 0xD5}, {L'Ị', L'I', 0xDA},
    // O + tone
    {L'Ò', L'O', 0xD8}, {L'Ó', L'O', 0xD9}, {L'Ỏ', L'O', 0xD6}, {L'Õ', L'O', 0xD5}, {L'Ọ', L'O', 0xDA},
    // Ô (O circumflex) + tone
    {L'Ô', L'O', 0xC2}, {L'Ồ', L'O', 0xDB}, {L'Ố', L'O', 0xDE}, {L'Ổ', L'O', 0xDC}, {L'Ỗ', L'O', 0xDD}, {L'Ộ', L'O', 0xDF},
    // Ơ (O horn) + tone
    {L'Ơ', L'O', 0xC1}, {L'Ờ', L'O', 0xCE}, {L'Ớ', L'O', 0xD4}, {L'Ở', L'O', 0xD1}, {L'Ỡ', L'O', 0xCF}, {L'Ợ', L'O', 0xD7},
    // U + tone
    {L'Ù', L'U', 0xD8}, {L'Ú', L'U', 0xD9}, {L'Ủ', L'U', 0xD6}, {L'Ũ', L'U', 0xD5}, {L'Ụ', L'U', 0xDA},
    // Ư (U horn) + tone
    {L'Ư', L'U', 0xC1}, {L'Ừ', L'U', 0xCE}, {L'Ứ', L'U', 0xD4}, {L'Ử', L'U', 0xD1}, {L'Ữ', L'U', 0xCF}, {L'Ự', L'U', 0xD7},
    // Y + tone
    {L'Ỳ', L'Y', 0xD8}, {L'Ý', L'Y', 0xD9}, {L'Ỷ', L'Y', 0xD6}, {L'Ỹ', L'Y', 0xD5}, {L'Ỵ', L'Y', 0xDA},
    // Đ
    {L'Đ', L'D', 0xD0},
};
static const size_t g_NumVniUpper = sizeof(g_UnicodeVniUpper) / sizeof(g_UnicodeVniUpper[0]);

// Build Unicode -> VNI lookup (lazy init)
static std::unordered_map<wchar_t, std::pair<wchar_t, wchar_t>>& GetUnicodeToVniMap() {
    static std::unordered_map<wchar_t, std::pair<wchar_t, wchar_t>> map;
    if (map.empty()) {
        for (size_t i = 0; i < g_NumVniLower; i++) {
            map[g_UnicodeVniLower[i].unicode] = {g_UnicodeVniLower[i].vniBase, g_UnicodeVniLower[i].vniMark};
        }
        for (size_t i = 0; i < g_NumVniUpper; i++) {
            map[g_UnicodeVniUpper[i].unicode] = {g_UnicodeVniUpper[i].vniBase, g_UnicodeVniUpper[i].vniMark};
        }
    }
    return map;
}

// Build VNI (base+mark pair) -> Unicode lookup (lazy init)
static std::unordered_map<uint32_t, wchar_t>& GetVniToUnicodeMap() {
    static std::unordered_map<uint32_t, wchar_t> map;
    if (map.empty()) {
        auto addEntries = [&](const VniMapping* arr, size_t count) {
            for (size_t i = 0; i < count; i++) {
                uint32_t key = ((uint32_t)arr[i].vniBase << 16) | (uint32_t)arr[i].vniMark;
                map[key] = arr[i].unicode;
            }
        };
        addEntries(g_UnicodeVniLower, g_NumVniLower);
        addEntries(g_UnicodeVniUpper, g_NumVniUpper);
    }
    return map;
}

// Build lookup maps lazily
static std::unordered_map<wchar_t, wchar_t>& GetAccentStripMap() {
    static std::unordered_map<wchar_t, wchar_t> map;
    if (map.empty()) {
        for (size_t i = 0; vn_chars[i] != L'\0'; ++i) {
            map[vn_chars[i]] = ascii_chars[i];
        }
    }
    return map;
}

static std::wstring StripAccentsString(const std::wstring& text) {
    auto& map = GetAccentStripMap();
    std::wstring result = text;
    for (auto& c : result) {
        auto it = map.find(c);
        if (it != map.end()) {
            c = it->second;
        }
    }
    return result;
}

// Convert Unicode string to VNI Windows encoding (stored as wchar_t stream)
static std::wstring UnicodeToVni(const std::wstring& text) {
    auto& u2v = GetUnicodeToVniMap();
    std::wstring result;
    result.reserve(text.size() * 2); // VNI can be up to 2x longer
    for (wchar_t c : text) {
        auto it = u2v.find(c);
        if (it != u2v.end()) {
            result += it->second.first;   // Base ASCII letter
            result += it->second.second;  // Diacritical mark byte
        } else {
            result += c; // Non-Vietnamese char passes through
        }
    }
    return result;
}

// Convert VNI Windows encoding to Unicode string
static std::wstring VniToUnicode(const std::wstring& text) {
    auto& v2u = GetVniToUnicodeMap();
    std::wstring result;
    result.reserve(text.size());
    for (size_t i = 0; i < text.size(); i++) {
        // Try 2-byte VNI sequence (base + mark)
        if (i + 1 < text.size()) {
            uint32_t key = ((uint32_t)text[i] << 16) | (uint32_t)text[i + 1];
            auto it = v2u.find(key);
            if (it != v2u.end()) {
                result += it->second;
                i++; // Skip diacritical byte
                continue;
            }
        }
        result += text[i]; // Single-byte passthrough
    }
    return result;
}

bool ModifyClipboardText(const std::wstring& newText) {
    if (!OpenClipboard(nullptr)) return false;
    EmptyClipboard();

    size_t cbStr = (newText.length() + 1) * sizeof(wchar_t);
    HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cbStr);
    if (!hGlobal) {
        CloseClipboard();
        return false;
    }

    void* pGlobal = GlobalLock(hGlobal);
    if (!pGlobal) {
        GlobalFree(hGlobal);
        CloseClipboard();
        return false;
    }
    memcpy(pGlobal, newText.c_str(), cbStr);
    GlobalUnlock(hGlobal);
    
    // SetClipboardData takes ownership of hGlobal on success
    // If it fails, we need to free the memory
    if (!SetClipboardData(CF_UNICODETEXT, hGlobal)) {
        GlobalFree(hGlobal);
        CloseClipboard();
        return false;
    }
    
    CloseClipboard();
    return true;
}

static std::wstring GetClipboardTextW() {
    if (!IsClipboardFormatAvailable(CF_UNICODETEXT)) return L"";
    if (!OpenClipboard(nullptr)) return L"";

    std::wstring result;
    HANDLE hData = GetClipboardData(CF_UNICODETEXT);
    if (hData) {
        wchar_t* pText = static_cast<wchar_t*>(GlobalLock(hData));
        if (pText) {
            result = pText;
            GlobalUnlock(hData);
        }
    }
    CloseClipboard();
    return result;
}

// =============================================================================
// Charset Conversion between any pair: Unicode, TCVN3, VNI Windows
// =============================================================================
// Strategy: normalize to Unicode first, then convert to destination.
// This allows O(N) conversion for any pair using 2 lookup maps.

static std::wstring ToUnicode(const std::wstring& text, uint8_t srcCharset) {
    if (srcCharset == CS_UNICODE) return text;

    if (srcCharset == CS_TCVN3) {
        std::unordered_map<wchar_t, wchar_t> t2u;
        for (size_t i = 0; i < g_NumMappings; i++) {
            t2u[(wchar_t)g_UnicodeTcvn3[i].tcvn3] = g_UnicodeTcvn3[i].unicode;
        }
        std::wstring result;
        result.reserve(text.size());
        for (wchar_t c : text) {
            auto it = t2u.find(c);
            result += (it != t2u.end()) ? it->second : c;
        }
        return result;
    }

    if (srcCharset == CS_VNI_WIN) {
        return VniToUnicode(text);
    }

    return text;
}

static std::wstring FromUnicode(const std::wstring& unicode, uint8_t destCharset) {
    if (destCharset == CS_UNICODE) return unicode;

    if (destCharset == CS_TCVN3) {
        std::unordered_map<wchar_t, wchar_t> u2t;
        for (size_t i = 0; i < g_NumMappings; i++) {
            u2t[g_UnicodeTcvn3[i].unicode] = (wchar_t)g_UnicodeTcvn3[i].tcvn3;
        }
        std::wstring result;
        result.reserve(unicode.size());
        for (wchar_t c : unicode) {
            auto it = u2t.find(c);
            result += (it != u2t.end()) ? it->second : c;
        }
        return result;
    }

    if (destCharset == CS_VNI_WIN) {
        return UnicodeToVni(unicode);
    }

    return unicode;
}

bool ConvertClipboardCharset(uint8_t srcCharset, uint8_t destCharset) {
    if (srcCharset == destCharset) return true;

    std::wstring text = GetClipboardTextW();
    if (text.empty()) return false;

    // Normalize to Unicode, then convert to destination
    std::wstring unicode = ToUnicode(text, srcCharset);
    std::wstring converted = FromUnicode(unicode, destCharset);

    return ModifyClipboardText(converted);
}

bool StripClipboardAccents() {
    std::wstring text = GetClipboardTextW();
    if (text.empty()) return false;
    
    std::wstring stripped = StripAccentsString(text);
    return ModifyClipboardText(stripped);
}

// =============================================================================
// Selected Text Operations — Copy → Transform → Paste
// =============================================================================
// Simulates Ctrl+C to copy selected text, transforms it, then simulates
// Ctrl+V to replace the selection with the transformed text.

static void SimulateCtrlKey(WORD vk) {
    INPUT inputs[4] = {};
    // Ctrl down
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_CONTROL;
    // Key down
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = vk;
    // Key up
    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = vk;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;
    // Ctrl up
    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_CONTROL;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(4, inputs, sizeof(INPUT));
}

bool StripSelectedTextAccents() {
    // Step 1: Copy selection to clipboard
    SimulateCtrlKey('C');
    Sleep(100); // Allow clipboard to sync

    // Step 2: Strip accents from clipboard text
    if (!StripClipboardAccents()) return false;

    // Step 3: Paste back (replaces the selection)
    SimulateCtrlKey('V');
    return true;
}

bool ConvertSelectedTextCharset(uint8_t srcCharset, uint8_t destCharset) {
    if (srcCharset == destCharset) return true;

    // Step 1: Copy selection to clipboard
    SimulateCtrlKey('C');
    Sleep(100);

    // Step 2: Convert charset
    if (!ConvertClipboardCharset(srcCharset, destCharset)) return false;

    // Step 3: Paste back
    SimulateCtrlKey('V');
    return true;
}

} // namespace Toolkit
