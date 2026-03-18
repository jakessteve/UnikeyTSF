#pragma once
// =============================================================================
// Vietnamese Charset Conversion — Pure Functions (Testable, No Win32 Dependencies)
// =============================================================================
// This header contains the charset mapping tables and conversion functions
// extracted from clipboard.cpp for unit testability. All functions are pure
// string transformations with no Win32 API dependencies.

#include <string>
#include <unordered_map>
#include <cstdint>

namespace Toolkit {

// =============================================================================
// Accent Stripping Tables
// =============================================================================
static const wchar_t vn_chars_tbl[] =
    L"àáảãạăằắẳẵặâầấẩẫậèéẻẽẹêềếểễệìíỉĩịòóỏõọôồốổỗộơờớởỡợùúủũụưừứửữựỳýỷỹỵđ"
    L"ÀÁẢÃẠĂẰẮẲẴẶÂẦẤẨẪẬÈÉẺẼẸÊỀẾỂỄỆÌÍỈĨỊÒÓỎÕỌÔỒỐỔỖỘƠỜỚỞỠỢÙÚỦŨỤƯỪỨỬỮỰỲÝỶỸỴĐ";

static const wchar_t ascii_chars_tbl[] =
    L"aaaaaaaaaaaaaaaaaeeeeeeeeeeeiiiiiooooooooooooooooouuuuuuuuuuuyyyyyd"
    L"AAAAAAAAAAAAAAAAAEEEEEEEEEEEIIIIIOOOOOOOOOOOOOOOOOUUUUUUUUUUUYYYYYD";

// =============================================================================
// TCVN3 Mapping (Unicode <-> TCVN3 byte values)
// =============================================================================
struct UnicodeTcvn3Mapping {
    wchar_t unicode;
    uint16_t tcvn3;
};

// TCVN3 mapping table (only essential entries for round-trip testing)
static const UnicodeTcvn3Mapping g_UnicodeTcvn3_tbl[] = {
    {L'à', 0xE0}, {L'á', 0xE1}, {L'ả', 0xE2}, {L'ã', 0xE3}, {L'ạ', 0xE4},
    {L'ă', 0xE5}, {L'ằ', 0xE6}, {L'ắ', 0xE7}, {L'ẳ', 0xE8}, {L'ẵ', 0xE9}, {L'ặ', 0xEA},
    {L'â', 0xEB}, {L'ầ', 0xEC}, {L'ấ', 0xED}, {L'ẩ', 0xEE}, {L'ẫ', 0xEF}, {L'ậ', 0xF0},
    {L'è', 0xF1}, {L'é', 0xF2}, {L'ẻ', 0xF3}, {L'ẽ', 0xF4}, {L'ẹ', 0xF5},
    {L'ê', 0xF6}, {L'ề', 0xF7}, {L'ế', 0xF8}, {L'ể', 0xF9}, {L'ễ', 0xFA}, {L'ệ', 0xFB},
    {L'ì', 0xFC}, {L'í', 0xFD}, {L'ỉ', 0xFE}, {L'ĩ', 0x8C}, {L'ị', 0x8D},
    {L'ò', 0x8E}, {L'ó', 0x8F}, {L'ỏ', 0x90}, {L'õ', 0x91}, {L'ọ', 0x92},
    {L'ô', 0x93}, {L'ồ', 0x94}, {L'ố', 0x95}, {L'ổ', 0x96}, {L'ỗ', 0x97}, {L'ộ', 0x98},
    {L'ơ', 0x99}, {L'ờ', 0x9A}, {L'ớ', 0x9B}, {L'ở', 0x9C}, {L'ỡ', 0x9D}, {L'ợ', 0x9E},
    {L'ù', 0x9F}, {L'ú', 0xA0}, {L'ủ', 0xA1}, {L'ũ', 0xA2}, {L'ụ', 0xA3},
    {L'ư', 0xA4}, {L'ừ', 0xA5}, {L'ứ', 0xA6}, {L'ử', 0xA7}, {L'ữ', 0xA8}, {L'ự', 0xA9},
    {L'ỳ', 0xAA}, {L'ý', 0xAB}, {L'ỷ', 0xAC}, {L'ỹ', 0xAD}, {L'ỵ', 0xAE},
    {L'đ', 0xAF},
    // Uppercase
    {L'À', 0xB0}, {L'Á', 0xB1}, {L'Ả', 0xB2}, {L'Ã', 0xB3}, {L'Ạ', 0xB4},
    {L'Ă', 0xB5}, {L'Ằ', 0xB6}, {L'Ắ', 0xB7}, {L'Ẳ', 0xB8}, {L'Ẵ', 0xB9}, {L'Ặ', 0xBA},
    {L'Â', 0xBB}, {L'Ầ', 0xBC}, {L'Ấ', 0xBD}, {L'Ẩ', 0xBE}, {L'Ẫ', 0xBF}, {L'Ậ', 0xC0},
    {L'È', 0xC1}, {L'É', 0xC2}, {L'Ẻ', 0xC3}, {L'Ẽ', 0xC4}, {L'Ẹ', 0xC5},
    {L'Ê', 0xC6}, {L'Ề', 0xC7}, {L'Ế', 0xC8}, {L'Ể', 0xC9}, {L'Ễ', 0xCA}, {L'Ệ', 0xCB},
    {L'Ì', 0xCC}, {L'Í', 0xCD}, {L'Ỉ', 0xCE}, {L'Ĩ', 0xCF}, {L'Ị', 0xD0},
    {L'Ò', 0xD1}, {L'Ó', 0xD2}, {L'Ỏ', 0xD3}, {L'Õ', 0xD4}, {L'Ọ', 0xD5},
    {L'Ô', 0xD6}, {L'Ồ', 0xD7}, {L'Ố', 0xD8}, {L'Ổ', 0xD9}, {L'Ỗ', 0xDA}, {L'Ộ', 0xDB},
    {L'Ơ', 0xDC}, {L'Ờ', 0xDD}, {L'Ớ', 0xDE}, {L'Ở', 0xDF}, {L'Ỡ', 0x80}, {L'Ợ', 0x81},
    {L'Ù', 0x82}, {L'Ú', 0x83}, {L'Ủ', 0x84}, {L'Ũ', 0x85}, {L'Ụ', 0x86},
    {L'Ư', 0x87}, {L'Ừ', 0x88}, {L'Ứ', 0x89}, {L'Ử', 0x8A}, {L'Ữ', 0x8B}, {L'Ự', 0xFF},
    {L'Ỳ', 0xCE}, {L'Ý', 0xCF}, {L'Ỷ', 0xD0}, {L'Ỹ', 0xD1}, {L'Ỵ', 0xD2},
    {L'Đ', 0xD0},
};

// =============================================================================
// VNI Windows Mapping Table (Unicode <-> 2-byte VNI encoding)
// =============================================================================
struct VniMapping {
    wchar_t unicode;
    wchar_t vniBase;
    wchar_t vniMark;
};

static const VniMapping g_UnicodeVniLower[] = {
    {L'à', L'a', 0xF8}, {L'á', L'a', 0xF9}, {L'ả', L'a', 0xF6}, {L'ã', L'a', 0xF5}, {L'ạ', L'a', 0xFA},
    {L'ă', L'a', 0xE5}, {L'ằ', L'a', 0xEB}, {L'ắ', L'a', 0xE9}, {L'ẳ', L'a', 0xEC}, {L'ẵ', L'a', 0xEA}, {L'ặ', L'a', 0xED},
    {L'â', L'a', 0xE2}, {L'ầ', L'a', 0xFB}, {L'ấ', L'a', 0xFE}, {L'ẩ', L'a', 0xFC}, {L'ẫ', L'a', 0xFD}, {L'ậ', L'a', 0xFF},
    {L'è', L'e', 0xF8}, {L'é', L'e', 0xF9}, {L'ẻ', L'e', 0xF6}, {L'ẽ', L'e', 0xF5}, {L'ẹ', L'e', 0xFA},
    {L'ê', L'e', 0xE2}, {L'ề', L'e', 0xFB}, {L'ế', L'e', 0xFE}, {L'ể', L'e', 0xFC}, {L'ễ', L'e', 0xFD}, {L'ệ', L'e', 0xFF},
    {L'ì', L'i', 0xF8}, {L'í', L'i', 0xF9}, {L'ỉ', L'i', 0xF6}, {L'ĩ', L'i', 0xF5}, {L'ị', L'i', 0xFA},
    {L'ò', L'o', 0xF8}, {L'ó', L'o', 0xF9}, {L'ỏ', L'o', 0xF6}, {L'õ', L'o', 0xF5}, {L'ọ', L'o', 0xFA},
    {L'ô', L'o', 0xE2}, {L'ồ', L'o', 0xFB}, {L'ố', L'o', 0xFE}, {L'ổ', L'o', 0xFC}, {L'ỗ', L'o', 0xFD}, {L'ộ', L'o', 0xFF},
    {L'ơ', L'o', 0xE1}, {L'ờ', L'o', 0xEE}, {L'ớ', L'o', 0xF4}, {L'ở', L'o', 0xF1}, {L'ỡ', L'o', 0xEF}, {L'ợ', L'o', 0xF7},
    {L'ù', L'u', 0xF8}, {L'ú', L'u', 0xF9}, {L'ủ', L'u', 0xF6}, {L'ũ', L'u', 0xF5}, {L'ụ', L'u', 0xFA},
    {L'ư', L'u', 0xE1}, {L'ừ', L'u', 0xEE}, {L'ứ', L'u', 0xF4}, {L'ử', L'u', 0xF1}, {L'ữ', L'u', 0xEF}, {L'ự', L'u', 0xF7},
    {L'ỳ', L'y', 0xF8}, {L'ý', L'y', 0xF9}, {L'ỷ', L'y', 0xF6}, {L'ỹ', L'y', 0xF5}, {L'ỵ', L'y', 0xFA},
    {L'đ', L'd', 0xF0},
};
static const size_t g_NumVniLower = sizeof(g_UnicodeVniLower) / sizeof(g_UnicodeVniLower[0]);

static const VniMapping g_UnicodeVniUpper[] = {
    {L'À', L'A', 0xD8}, {L'Á', L'A', 0xD9}, {L'Ả', L'A', 0xD6}, {L'Ã', L'A', 0xD5}, {L'Ạ', L'A', 0xDA},
    {L'Ă', L'A', 0xC5}, {L'Ằ', L'A', 0xCB}, {L'Ắ', L'A', 0xC9}, {L'Ẳ', L'A', 0xCC}, {L'Ẵ', L'A', 0xCA}, {L'Ặ', L'A', 0xCD},
    {L'Â', L'A', 0xC2}, {L'Ầ', L'A', 0xDB}, {L'Ấ', L'A', 0xDE}, {L'Ẩ', L'A', 0xDC}, {L'Ẫ', L'A', 0xDD}, {L'Ậ', L'A', 0xDF},
    {L'È', L'E', 0xD8}, {L'É', L'E', 0xD9}, {L'Ẻ', L'E', 0xD6}, {L'Ẽ', L'E', 0xD5}, {L'Ẹ', L'E', 0xDA},
    {L'Ê', L'E', 0xC2}, {L'Ề', L'E', 0xDB}, {L'Ế', L'E', 0xDE}, {L'Ể', L'E', 0xDC}, {L'Ễ', L'E', 0xDD}, {L'Ệ', L'E', 0xDF},
    {L'Ì', L'I', 0xD8}, {L'Í', L'I', 0xD9}, {L'Ỉ', L'I', 0xD6}, {L'Ĩ', L'I', 0xD5}, {L'Ị', L'I', 0xDA},
    {L'Ò', L'O', 0xD8}, {L'Ó', L'O', 0xD9}, {L'Ỏ', L'O', 0xD6}, {L'Õ', L'O', 0xD5}, {L'Ọ', L'O', 0xDA},
    {L'Ô', L'O', 0xC2}, {L'Ồ', L'O', 0xDB}, {L'Ố', L'O', 0xDE}, {L'Ổ', L'O', 0xDC}, {L'Ỗ', L'O', 0xDD}, {L'Ộ', L'O', 0xDF},
    {L'Ơ', L'O', 0xC1}, {L'Ờ', L'O', 0xCE}, {L'Ớ', L'O', 0xD4}, {L'Ở', L'O', 0xD1}, {L'Ỡ', L'O', 0xCF}, {L'Ợ', L'O', 0xD7},
    {L'Ù', L'U', 0xD8}, {L'Ú', L'U', 0xD9}, {L'Ủ', L'U', 0xD6}, {L'Ũ', L'U', 0xD5}, {L'Ụ', L'U', 0xDA},
    {L'Ư', L'U', 0xC1}, {L'Ừ', L'U', 0xCE}, {L'Ứ', L'U', 0xD4}, {L'Ử', L'U', 0xD1}, {L'Ữ', L'U', 0xCF}, {L'Ự', L'U', 0xD7},
    {L'Ỳ', L'Y', 0xD8}, {L'Ý', L'Y', 0xD9}, {L'Ỷ', L'Y', 0xD6}, {L'Ỹ', L'Y', 0xD5}, {L'Ỵ', L'Y', 0xDA},
    {L'Đ', L'D', 0xD0},
};
static const size_t g_NumVniUpper = sizeof(g_UnicodeVniUpper) / sizeof(g_UnicodeVniUpper[0]);

// =============================================================================
// Pure Conversion Functions
// =============================================================================

inline std::wstring StripAccentsString(const std::wstring& text) {
    static std::unordered_map<wchar_t, wchar_t> map;
    if (map.empty()) {
        for (size_t i = 0; vn_chars_tbl[i] != L'\0'; ++i) {
            map[vn_chars_tbl[i]] = ascii_chars_tbl[i];
        }
    }
    std::wstring result = text;
    for (auto& c : result) {
        auto it = map.find(c);
        if (it != map.end()) c = it->second;
    }
    return result;
}

inline std::wstring UnicodeToVni(const std::wstring& text) {
    static std::unordered_map<wchar_t, std::pair<wchar_t, wchar_t>> u2v;
    if (u2v.empty()) {
        for (size_t i = 0; i < g_NumVniLower; i++)
            u2v[g_UnicodeVniLower[i].unicode] = {g_UnicodeVniLower[i].vniBase, g_UnicodeVniLower[i].vniMark};
        for (size_t i = 0; i < g_NumVniUpper; i++)
            u2v[g_UnicodeVniUpper[i].unicode] = {g_UnicodeVniUpper[i].vniBase, g_UnicodeVniUpper[i].vniMark};
    }
    std::wstring result;
    result.reserve(text.size() * 2);
    for (wchar_t c : text) {
        auto it = u2v.find(c);
        if (it != u2v.end()) {
            result += it->second.first;
            result += it->second.second;
        } else {
            result += c;
        }
    }
    return result;
}

inline std::wstring VniToUnicode(const std::wstring& text) {
    static std::unordered_map<uint32_t, wchar_t> v2u;
    if (v2u.empty()) {
        auto add = [&](const VniMapping* arr, size_t count) {
            for (size_t i = 0; i < count; i++) {
                uint32_t key = ((uint32_t)arr[i].vniBase << 16) | (uint32_t)arr[i].vniMark;
                v2u[key] = arr[i].unicode;
            }
        };
        add(g_UnicodeVniLower, g_NumVniLower);
        add(g_UnicodeVniUpper, g_NumVniUpper);
    }
    std::wstring result;
    result.reserve(text.size());
    for (size_t i = 0; i < text.size(); i++) {
        if (i + 1 < text.size()) {
            uint32_t key = ((uint32_t)text[i] << 16) | (uint32_t)text[i + 1];
            auto it = v2u.find(key);
            if (it != v2u.end()) {
                result += it->second;
                i++;
                continue;
            }
        }
        result += text[i];
    }
    return result;
}

} // namespace Toolkit
