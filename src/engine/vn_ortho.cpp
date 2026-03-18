#include "vn_ortho.h"
#include <cwctype>
#include <vector>
#include <set>
#include <algorithm>

namespace VnOrtho {

wchar_t ToLower(wchar_t ch) {
    if (ch >= L'A' && ch <= L'Z') return ch - L'A' + L'a';
    
    // Explicit Vietnamese mappings
    switch (ch) {
        // Base
        case L'Â': return L'â'; case L'Ă': return L'ă';
        case L'Ê': return L'ê'; case L'Ơ': return L'ơ';
        case L'Ư': return L'ư'; case L'Đ': return L'đ';
        // á, à, ả, ã, ạ
        case L'Á': return L'á'; case L'À': return L'à'; case L'Ả': return L'ả'; case L'Ã': return L'ã'; case L'Ạ': return L'ạ';
        // ắ, ằ, ẳ, ẵ, ặ
        case L'Ắ': return L'ắ'; case L'Ằ': return L'ằ'; case L'Ẳ': return L'ẳ'; case L'Ẵ': return L'ẵ'; case L'Ặ': return L'ặ';
        // ấ, ầ, ẩ, ẫ, ậ
        case L'Ấ': return L'ấ'; case L'Ầ': return L'ầ'; case L'Ẩ': return L'ẩ'; case L'Ẫ': return L'ẫ'; case L'Ậ': return L'ậ';
        // é, è, ẻ, ẽ, ẹ
        case L'É': return L'é'; case L'È': return L'è'; case L'Ẻ': return L'ẻ'; case L'Ẽ': return L'ẽ'; case L'Ẹ': return L'ẹ';
        // ế, ề, ể, ễ, ệ
        case L'Ế': return L'ế'; case L'Ề': return L'ề'; case L'Ể': return L'ể'; case L'Ễ': return L'ễ'; case L'Ệ': return L'ệ';
        // í, ì, ỉ, ĩ, ị
        case L'Í': return L'í'; case L'Ì': return L'ì'; case L'Ỉ': return L'ỉ'; case L'Ĩ': return L'ĩ'; case L'Ị': return L'ị';
        // ó, ò, ỏ, õ, ọ
        case L'Ó': return L'ó'; case L'Ò': return L'ò'; case L'Ỏ': return L'ỏ'; case L'Õ': return L'õ'; case L'Ọ': return L'ọ';
        // ố, ồ, ổ,ỗ, ộ
        case L'Ố': return L'ố'; case L'Ồ': return L'ồ'; case L'Ổ': return L'ổ'; case L'Ỗ': return L'ỗ'; case L'Ộ': return L'ộ';
        // ớ, ờ, ở, ỡ, ợ
        case L'Ớ': return L'ớ'; case L'Ờ': return L'ờ'; case L'Ở': return L'ở'; case L'Ỡ': return L'ỡ'; case L'Ợ': return L'ợ';
        // ú, ù, ủ, ũ, ụ
        case L'Ú': return L'ú'; case L'Ù': return L'ù'; case L'Ủ': return L'ủ'; case L'Ũ': return L'ũ'; case L'Ụ': return L'ụ';
        // ứ, ừ, ử, ữ, ự
        case L'Ứ': return L'ứ'; case L'Ừ': return L'ừ'; case L'Ử': return L'ử'; case L'Ữ': return L'ữ'; case L'Ự': return L'ự';
        // ý, ỳ, ỷ, ỹ, ỵ
        case L'Ý': return L'ý'; case L'Ỳ': return L'ỳ'; case L'Ỷ': return L'ỷ'; case L'Ỹ': return L'ỹ'; case L'Ỵ': return L'ỵ';
        default: return std::towlower(ch);
    }
}

wchar_t ToUpper(wchar_t ch) {
    if (ch >= L'a' && ch <= L'z') return ch - L'a' + L'A';
    
    switch (ch) {
        case L'â': return L'Â'; case L'ă': return L'Ă';
        case L'ê': return L'Ê'; case L'ơ': return L'Ơ';
        case L'ư': return L'Ư'; case L'đ': return L'Đ';
        case L'á': return L'Á'; case L'à': return L'À'; case L'ả': return L'Ả'; case L'ã': return L'Ã'; case L'ạ': return L'Ạ';
        case L'ắ': return L'Ắ'; case L'ằ': return L'Ằ'; case L'ẳ': return L'Ẳ'; case L'ẵ': return L'Ẵ'; case L'ặ': return L'Ặ';
        case L'ấ': return L'Ấ'; case L'ầ': return L'Ầ'; case L'ẩ': return L'Ẩ'; case L'ẫ': return L'Ẫ'; case L'ậ': return L'Ậ';
        case L'é': return L'É'; case L'è': return L'È'; case L'ẻ': return L'Ẻ'; case L'ẽ': return L'Ẽ'; case L'ẹ': return L'Ẹ';
        case L'ế': return L'Ế'; case L'ề': return L'Ề'; case L'ể': return L'Ể'; case L'ễ': return L'Ễ'; case L'ệ': return L'Ệ';
        case L'í': return L'Í'; case L'ì': return L'Ì'; case L'ỉ': return L'Ỉ'; case L'ĩ': return L'Ĩ'; case L'ị': return L'Ị';
        case L'ó': return L'Ó'; case L'ò': return L'Ò'; case L'ỏ': return L'Ỏ'; case L'õ': return L'Õ'; case L'ọ': return L'Ọ';
        case L'ố': return L'Ố'; case L'ồ': return L'Ồ'; case L'ổ': return L'Ổ'; case L'ỗ': return L'Ỗ'; case L'ộ': return L'Ộ';
        case L'ớ': return L'Ớ'; case L'ờ': return L'Ờ'; case L'ở': return L'Ở'; case L'ỡ': return L'Ỡ'; case L'ợ': return L'Ợ';
        case L'ú': return L'Ú'; case L'ù': return L'Ù'; case L'ủ': return L'Ủ'; case L'ũ': return L'Ũ'; case L'ụ': return L'Ụ';
        case L'ứ': return L'Ứ'; case L'ừ': return L'Ừ'; case L'ử': return L'Ử'; case L'ữ': return L'Ữ'; case L'ự': return L'Ự';
        case L'ý': return L'Ý'; case L'ỳ': return L'Ỳ'; case L'ỷ': return L'Ỷ'; case L'ỹ': return L'Ỹ'; case L'ỵ': return L'Ỵ';
        default: return std::towupper(ch);
    }
}

bool IsUpper(wchar_t ch) {
    if (ch >= L'A' && ch <= L'Z') return true;
    switch (ch) {
        case L'Â': case L'Ă': case L'Ê': case L'Ơ': case L'Ư': case L'Đ':
        case L'Á': case L'À': case L'Ả': case L'Ã': case L'Ạ':
        case L'Ắ': case L'Ằ': case L'Ẳ': case L'Ẵ': case L'Ặ':
        case L'Ấ': case L'Ầ': case L'Ẩ': case L'Ẫ': case L'Ậ':
        case L'É': case L'È': case L'Ẻ': case L'Ẽ': case L'Ẹ':
        case L'Ế': case L'Ề': case L'Ể': case L'Ễ': case L'Ệ':
        case L'Í': case L'Ì': case L'Ỉ': case L'Ĩ': case L'Ị':
        case L'Ó': case L'Ò': case L'Ỏ': case L'Õ': case L'Ọ':
        case L'Ố': case L'Ồ': case L'Ổ': case L'Ỗ': case L'Ộ':
        case L'Ớ': case L'Ờ': case L'Ở': case L'Ỡ': case L'Ợ':
        case L'Ú': case L'Ù': case L'Ủ': case L'Ũ': case L'Ụ':
        case L'Ứ': case L'Ừ': case L'Ử': case L'Ữ': case L'Ự':
        case L'Ý': case L'Ỳ': case L'Ỷ': case L'Ỹ': case L'Ỵ':
            return true;
        default: return std::iswupper(ch) != 0;
    }
}

static const std::set<std::wstring> VALID_INITIALS = {
    L"b", L"c", L"ch", L"d", L"đ", L"g", L"gh", L"gi", L"h", L"k", L"kh", L"l", L"m", L"n", L"ng", L"ngh", L"nh", L"p", L"ph", L"qu", L"r", L"s", L"t", L"th", L"tr", L"v", L"x"
};

static const std::set<std::wstring> VALID_FINALS = {
    L"", L"c", L"ch", L"m", L"n", L"ng", L"nh", L"p", L"t"
};

// Simplified set of valid vowel combinations (base characters, without tone)
// Note: 'y' and 'i' are often interchangeable in certain positions.
static const std::set<std::wstring> VALID_VOWEL_CLUSTERS = {
    L"a", L"ă", L"â", L"e", L"ê", L"i", L"o", L"ô", L"ơ", L"u", L"ư", L"y",
    L"ai", L"ao", L"au", L"ay", L"âu", L"ây", L"eo", L"êu", L"iu", L"oa", L"oe", L"oi", L"ôi", L"ơi", L"ua", L"uê", L"ui", L"uy", L"ưa", L"ưi", L"ưu",
    L"ia", L"iê", L"yê", L"ya", L"uô", L"ươ", L"oă", L"oâ", L"uâ", L"uơ", L"uyê", L"uya", L"uyu", L"uyuê",
    L"iêu", L"oai", L"oao", L"oay", L"uây", L"uôi", L"ươi", L"ươu", L"yêu"
};

// Helper: Normalize a Vietnamese vowel and extract tone
struct NormalizedVowel {
    wchar_t base;
    int tone;
};

static NormalizedVowel Normalize(wchar_t ch) {
    const std::wstring a_tones = L"aáàảãạ";
    const std::wstring aw_tones = L"ăắằẳẵặ";
    const std::wstring aa_tones = L"âấầẩẫậ";
    const std::wstring e_tones = L"eéèẻẽẹ";
    const std::wstring ee_tones = L"êếềểễệ";
    const std::wstring i_tones = L"iíìỉĩị";
    const std::wstring o_tones = L"oóòỏõọ";
    const std::wstring oo_tones = L"ôốồổỗộ";
    const std::wstring ow_tones = L"ơớờởỡợ";
    const std::wstring u_tones = L"uúùủũụ";
    const std::wstring uw_tones = L"ưứừửữự";
    const std::wstring y_tones = L"yýỳỷỹỵ";

    auto check = [&](const std::wstring& arr) -> NormalizedVowel {
        for (int i = 0; i < (int)arr.size(); ++i) {
            if (arr[i] == ch) return {arr[0], i};
            if (ToUpper(arr[i]) == ch) return {arr[0], i};
        }
        return {0, -1};
    };

    NormalizedVowel res;
    if ((res = check(a_tones)).tone >= 0) return res;
    if ((res = check(aw_tones)).tone >= 0) return res;
    if ((res = check(aa_tones)).tone >= 0) return res;
    if ((res = check(e_tones)).tone >= 0) return res;
    if ((res = check(ee_tones)).tone >= 0) return res;
    if ((res = check(i_tones)).tone >= 0) return res;
    if ((res = check(o_tones)).tone >= 0) return res;
    if ((res = check(oo_tones)).tone >= 0) return res;
    if ((res = check(ow_tones)).tone >= 0) return res;
    if ((res = check(u_tones)).tone >= 0) return res;
    if ((res = check(uw_tones)).tone >= 0) return res;
    if ((res = check(y_tones)).tone >= 0) return res;

    return { ToLower(ch), 0 };
}

bool IsVowel(wchar_t ch) {
    ch = ToLower(ch);
    return ch == L'a' || ch == L'e' || ch == L'i' || ch == L'o' || ch == L'u' || ch == L'y' ||
           ch == L'â' || ch == L'ê' || ch == L'ô' || ch == L'ơ' || ch == L'ư' || ch == L'ă' ||
           ch == L'á' || ch == L'à' || ch == L'ả' || ch == L'ã' || ch == L'ạ' ||
           ch == L'ắ' || ch == L'ằ' || ch == L'ẳ' || ch == L'ẵ' || ch == L'ặ' ||
           ch == L'ấ' || ch == L'ầ' || ch == L'ẩ' || ch == L'ẫ' || ch == L'ậ' ||
           ch == L'é' || ch == L'è' || ch == L'ẻ' || ch == L'ẽ' || ch == L'ẹ' ||
           ch == L'ế' || ch == L'ề' || ch == L'ể' || ch == L'ễ' || ch == L'ệ' ||
           ch == L'í' || ch == L'ì' || ch == L'ỉ' || ch == L'ĩ' || ch == L'ị' ||
           ch == L'ó' || ch == L'ò' || ch == L'ỏ' || ch == L'õ' || ch == L'ọ' ||
           ch == L'ố' || ch == L'ồ' || ch == L'ổ' || ch == L'ỗ' || ch == L'ộ' ||
           ch == L'ớ' || ch == L'ờ' || ch == L'ở' || ch == L'ỡ' || ch == L'ợ' ||
           ch == L'ú' || ch == L'ù' || ch == L'ủ' || ch == L'ũ' || ch == L'ụ' ||
           ch == L'ứ' || ch == L'ừ' || ch == L'ử' || ch == L'ữ' || ch == L'ự' ||
           ch == L'ý' || ch == L'ỳ' || ch == L'ỷ' || ch == L'ỹ' || ch == L'ỵ';
}

bool IsModifiedVowel(wchar_t ch) {
    ch = ToLower(ch);
    return ch == L'â' || ch == L'ê' || ch == L'ô' || ch == L'ơ' || ch == L'ư' || ch == L'ă' ||
           ch == L'ấ' || ch == L'ầ' || ch == L'ẩ' || ch == L'ẫ' || ch == L'ậ' ||
           ch == L'ế' || ch == L'ề' || ch == L'ể' || ch == L'ễ' || ch == L'ệ' ||
           ch == L'ố' || ch == L'ồ' || ch == L'ổ' || ch == L'ỗ' || ch == L'ộ' ||
           ch == L'ớ' || ch == L'ờ' || ch == L'ở' || ch == L'ỡ' || ch == L'ợ' ||
           ch == L'ứ' || ch == L'ừ' || ch == L'ử' || ch == L'ữ' || ch == L'ự' ||
           ch == L'ắ' || ch == L'ằ' || ch == L'ẳ' || ch == L'ẵ' || ch == L'ặ';
}

SyllableParts Decompose(const std::wstring& word) {
    SyllableParts parts;
    parts.tone = 0;

    int firstVowel = -1;
    int lastVowel = -1;
    std::wstring normalizedBase = L"";

    for (int i = 0; i < (int)word.size(); ++i) {
        if (IsVowel(word[i])) {
            if (firstVowel == -1) firstVowel = i;
            lastVowel = i;
            NormalizedVowel nv = Normalize(word[i]);
            normalizedBase += nv.base;
            if (nv.tone > 0) parts.tone = nv.tone;
        }
    }

    if (firstVowel == -1) {
        parts.initial = word;
        return parts;
    }

    // Handle 'qu' as initial
    if (firstVowel == 1 && ToLower(word[0]) == L'q' && ToLower(normalizedBase[0]) == L'u' && normalizedBase.size() > 1) {
        parts.initial = L"qu";
        parts.vowels = normalizedBase.substr(1);
    } else if (firstVowel == 2 && ToLower(word[0]) == L'g' && ToLower(word[1]) == L'i' && IsVowel(word[1]) && normalizedBase.size() > 1) {
        // Handle 'gi' as initial followed by vowels
        parts.initial = L"gi";
        parts.vowels = normalizedBase.substr(1);
    } else {
        parts.initial = L"";
        for (int i = 0; i < firstVowel; ++i) parts.initial += ToLower(word[i]);
        parts.vowels = normalizedBase;
    }

    parts.final = L"";
    for (int i = lastVowel + 1; i < (int)word.size(); ++i) parts.final += ToLower(word[i]);

    return parts;
}

bool IsSyllableValid(const std::wstring& word) {
    if (word.empty()) return true;

    // A word is considered a syllable only if it has at least one vowel
    // unless it's a very short word like "đ" or "k" (though not really a syllable)
    bool hasVowel = false;
    for (wchar_t ch : word) if (IsVowel(ch)) hasVowel = true;
    if (!hasVowel) {
        // Consonant-only "syllables" are invalid in Vietnamese except for some special cases
        // But for an IME, we might want to allow them as intermediate states.
        // However, spell check usually applies when we have a full word.
        return true; 
    }

    SyllableParts parts = Decompose(word);

    // 1. Check Initial
    if (parts.initial != L"" && VALID_INITIALS.find(parts.initial) == VALID_INITIALS.end()) {
        return false;
    }

    // 2. Check Vowel Cluster
    if (VALID_VOWEL_CLUSTERS.find(parts.vowels) == VALID_VOWEL_CLUSTERS.end()) {
        return false;
    }

    // 3. Check Final
    if (VALID_FINALS.find(parts.final) == VALID_FINALS.end()) {
        return false;
    }

    // 4. Check Tone vs Final
    // c, ch, p, t can only have sac (1) or nang (5)
    if (parts.final == L"c" || parts.final == L"ch" || parts.final == L"p" || parts.final == L"t") {
        if (parts.tone != 0 && parts.tone != 1 && parts.tone != 5) {
            return false;
        }
    }

    // 5. Check Initial vs Vowels
    // k, gh, ngh must be followed by e, ê, i
    if (parts.initial == L"k" || parts.initial == L"gh" || parts.initial == L"ngh") {
        if (parts.vowels.empty() || (parts.vowels[0] != L'e' && parts.vowels[0] != L'ê' && parts.vowels[0] != L'i' && parts.vowels[0] != L'y')) {
            return false;
        }
    }
    // c, g, ng must NOT be followed by e, ê, i
    if (parts.initial == L"c" || parts.initial == L"g" || parts.initial == L"ng") {
        if (!parts.vowels.empty() && (parts.vowels[0] == L'e' || parts.vowels[0] == L'ê' || parts.vowels[0] == L'i')) {
            // Wait, 'gi' is handled as an initial, but 'g' alone cannot be followed by 'i'
            if (parts.initial != L"g" || parts.vowels[0] != L'i') {
                return false;
            }
        }
    }

    return true;
}

} // namespace VnOrtho
