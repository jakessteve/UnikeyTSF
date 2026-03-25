#include "vn_engine.h"
#include "vn_ortho.h"
#include <cwctype>

// Helper to check if character is vowel
static bool IsVowel(wchar_t ch) {
    return VnOrtho::IsVowel(ch);
}

VnEngine::VnEngine() : _isUppercase(false), _didTransform(false),
    _toneType(TONE_MODERN), _freeToneMarking(false), _spellCheck(false) {}

bool VnEngine::IsWordChar(wchar_t ch, InputMethod method) const {
    // Alphabetic characters are always valid word characters
    if (std::iswalpha(ch)) return true;
    
    // In VNI mode, digits 0-9 are modifiers/tones, so they're valid mid-word
    // BUT only if we're already in a word (have vowels to apply them to)
    if (method == IM_VNI && ch >= L'0' && ch <= L'9' && !_raw.empty()) {
        return true;
    }

    // In VIQR mode, certain punctuation marks are modifiers
    if (method == IM_VIQR && !_raw.empty()) {
        // VIQR uses: ' ` ? ~ . ^ ( + d D
        if (ch == L'\'' || ch == L'`' || ch == L'?' || ch == L'~' ||
            ch == L'.' || ch == L'^' || ch == L'(' || ch == L'+') {
            return true;
        }
    }
    
    return false;
}

bool VnEngine::IsPotentialModifier(wchar_t ch, InputMethod method) const {
    if (method == IM_VNI) {
        // VNI: digits 0-9 are tone/modifier keys
        return (ch >= L'0' && ch <= L'9');
    }
    if (method == IM_TELEX) {
        wchar_t lower = std::towlower(ch);
        // Telex tone keys: s(sắc), f(huyền), r(hỏi), x(ngã), j(nặng), z(remove)
        // Telex modifier key: w(horn/breve)
        return (lower == L's' || lower == L'f' || lower == L'r' ||
                lower == L'x' || lower == L'j' || lower == L'z' || lower == L'w');
    }
    if (method == IM_VIQR) {
        // VIQR tone marks: ' ` ? ~ .
        // VIQR modifiers: ^ ( +
        return (ch == L'\'' || ch == L'`' || ch == L'?' || ch == L'~' ||
                ch == L'.' || ch == L'^' || ch == L'(' || ch == L'+');
    }
    return false;
}

bool VnEngine::ProcessKey(wchar_t ch, InputMethod method) {
    // Whitespace always terminates the word
    if (ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\n') {
        Clear();
        return false; // Let the app handle the word terminator
    }

    // Non-word characters terminate the word and pass through
    if (!IsWordChar(ch, method)) {
        Clear();
        return false;
    }

    if (_raw.empty()) {
        _isUppercase = VnOrtho::IsUpper(ch);
    }

    // Always append to raw buffer, then compute composition from scratch
    _raw += ch;
    _currentMethod = method; // Cache method for use in backspace

    std::wstring result;
    if (method == IM_TELEX) {
        result = ApplyTelexRules(_raw);
    } else if (method == IM_VNI) {
        result = ApplyVniRules(_raw);
    } else if (method == IM_VIQR) {
        result = ApplyViqrRules(_raw);
    } else {
        result = _raw;
    }

    // Spell Check: if enabled, only accept transformation if it's a valid syllable
    if (_spellCheck && result != _raw) {
        // Only validate if we have at least one vowel and one modification/tone
        if (!VnOrtho::IsSyllableValid(result)) {
            // Revert transformation for invalid syllables
            _composition = _raw;
            _didTransform = false;
            return true;
        }
    }

    _composition = result;

    // Determine if a Vietnamese transformation actually occurred
    _didTransform = (_composition != _raw);

    return true; // We eat the key to manage composition ourselves
}

std::wstring VnEngine::GetCompositionString() const {
    return _composition;
}

std::wstring VnEngine::GetRawString() const {
    return _raw;
}

bool VnEngine::DidTransform() const {
    return _didTransform;
}

void VnEngine::Clear() {
    _raw.clear();
    _composition.clear();
    _isUppercase = false;
    _didTransform = false;
}

bool VnEngine::IsInWord() const {
    return !_raw.empty();
}

void VnEngine::RemoveLastChar() {
    if (!_raw.empty()) {
        _raw.pop_back();
        
        // Recompute using the active method seamlessly
        if (_currentMethod == IM_TELEX) {
            _composition = ApplyTelexRules(_raw);
        } else if (_currentMethod == IM_VNI) {
            _composition = ApplyVniRules(_raw);
        } else if (_currentMethod == IM_VIQR) {
            _composition = ApplyViqrRules(_raw);
        } else {
            _composition = _raw;
        }
        _didTransform = (_composition != _raw);
    }
}

// =============================================================================
// Robust FSM for Vietnamese Orthography
// =============================================================================

wchar_t ApplyTone(wchar_t base, int tone) {
    if (tone < 0 || tone > 5) return base;
    
    // Arrays for mapping. Index corresponds to tone 0-5.
    const std::wstring a_tones = L"aáàảãạ";
    const std::wstring A_tones = L"AÁÀẢÃẠ";
    const std::wstring aw_tones = L"ăắằẳẵặ";
    const std::wstring AW_tones = L"ĂẮẰẲẴẶ";
    const std::wstring aa_tones = L"âấầẩẫậ";
    const std::wstring AA_tones = L"ÂẤẦẨẪẬ"; // Last char = Ậ (U+1EAC)
    
    const std::wstring e_tones = L"eéèẻẽẹ";
    const std::wstring E_tones = L"EÉÈẺẼẸ"; // Ẹ U+1EB8
    const std::wstring ee_tones = L"êếềểễệ";
    const std::wstring EE_tones = L"ÊẾỀỂỄỆ";
    
    const std::wstring i_tones = L"iíìỉĩị";
    const std::wstring I_tones = L"IÍÌỈĨỊ"; // Ị U+1ECA
    
    const std::wstring o_tones = L"oóòỏõọ";
    const std::wstring O_tones = L"OÓÒỎÕỌ";
    const std::wstring oo_tones = L"ôốồổỗộ";
    const std::wstring OO_tones = L"ÔỐỒỔỖỘ";
    const std::wstring ow_tones = L"ơớờởỡợ";
    const std::wstring OW_tones = L"ƠỚỜỞỠỢ";
    
    const std::wstring u_tones = L"uúùủũụ";
    const std::wstring U_tones = L"UÚÙỦŨỤ";
    const std::wstring uw_tones = L"ưứừửữự";
    const std::wstring UW_tones = L"ƯỨỪỬỮỰ";
    
    const std::wstring y_tones = L"yýỳỷỹỵ";
    const std::wstring Y_tones = L"YÝỲỶỸỴ";

    auto check_and_apply = [&](const std::wstring& arr) -> wchar_t {
         for (size_t i=0; i<arr.size(); ++i) {
             if (arr[i] == base) return arr[tone];
         }
         return 0;
    };

    wchar_t res;
    if ((res = check_and_apply(a_tones))) return res;
    if ((res = check_and_apply(A_tones))) return res;
    if ((res = check_and_apply(aw_tones))) return res;
    if ((res = check_and_apply(AW_tones))) return res;
    if ((res = check_and_apply(aa_tones))) return res;
    if ((res = check_and_apply(AA_tones))) return res;
    if ((res = check_and_apply(e_tones))) return res;
    if ((res = check_and_apply(E_tones))) return res;
    if ((res = check_and_apply(ee_tones))) return res;
    if ((res = check_and_apply(EE_tones))) return res;
    if ((res = check_and_apply(i_tones))) return res;
    if ((res = check_and_apply(I_tones))) return res;
    if ((res = check_and_apply(o_tones))) return res;
    if ((res = check_and_apply(O_tones))) return res;
    if ((res = check_and_apply(oo_tones))) return res;
    if ((res = check_and_apply(OO_tones))) return res;
    if ((res = check_and_apply(ow_tones))) return res;
    if ((res = check_and_apply(OW_tones))) return res;
    if ((res = check_and_apply(u_tones))) return res;
    if ((res = check_and_apply(U_tones))) return res;
    if ((res = check_and_apply(uw_tones))) return res;
    if ((res = check_and_apply(UW_tones))) return res;
    if ((res = check_and_apply(y_tones))) return res;
    if ((res = check_and_apply(Y_tones))) return res;

    return base; // no match
}

// Check if a character is a modified Vietnamese vowel (circumflex, horn, breve)
static bool IsModifiedVowel(wchar_t ch) {
    ch = VnOrtho::ToLower(ch);
    return ch == L'â' || ch == L'ê' || ch == L'ô' || ch == L'ơ' || ch == L'ư' || ch == L'ă';
}

// =============================================================================
// Vietnamese Tone Placement
// =============================================================================
// Modern (new-style) rules:
// 1. If there's a modified vowel (â, ê, ô, ơ, ư, ă), tone goes on it.
// 2. If open syllable (ends with vowel): tone on penultimate vowel of cluster.
// 3. If closed syllable (ends with consonant): tone on last vowel.
// Classic (old-style) rules:
// - Tone always goes on the first vowel of the vowel cluster.
// =============================================================================
static void ApplyToneToResult(std::wstring& result, int tone, ToneType toneType, bool freeToneMarking) {
    if (tone < 0 || result.empty()) return;

    // Find the vowel cluster: first and last vowel indices
    int firstVowel = -1, lastVowel = -1;
    for (int i = 0; i < (int)result.size(); i++) {
        if (IsVowel(result[i])) {
            if (firstVowel == -1) firstVowel = i;
            lastVowel = i;
        }
    }
    if (firstVowel == -1) return; // No vowels

    // Classic tone mode: always place on first vowel of the cluster
    if (toneType == TONE_CLASSIC) {
        // Collect contiguous vowel groups and use the last one
        int groupStart = -1, groupEnd = -1;
        int bestStart = -1;
        for (int i = 0; i < (int)result.size(); i++) {
            if (IsVowel(result[i])) {
                if (groupStart == -1) groupStart = i;
                groupEnd = i;
            } else {
                if (groupStart != -1) {
                    bestStart = groupStart;
                    groupStart = -1;
                }
            }
        }
        if (groupStart != -1) bestStart = groupStart;
        if (bestStart >= 0) {
            result[bestStart] = ApplyTone(result[bestStart], tone);
        }
        return;
    }

    // Modern tone placement follows below

    // 1. Priority: if there's exactly ONE modified vowel, tone goes on it.
    int modifiedCount = 0;
    int singleModifiedIdx = -1;
    for (int i = firstVowel; i <= lastVowel; i++) {
        if (IsVowel(result[i]) && IsModifiedVowel(result[i])) {
            modifiedCount++;
            singleModifiedIdx = i;
        }
    }
    if (modifiedCount == 1) {
        result[singleModifiedIdx] = ApplyTone(result[singleModifiedIdx], tone);
        return;
    }

    // Collect contiguous vowel groups and use the last one
    int groupStart = -1, groupEnd = -1;
    int bestStart = -1, bestEnd = -1;
    for (int i = 0; i < (int)result.size(); i++) {
        if (IsVowel(result[i])) {
            if (groupStart == -1) groupStart = i;
            groupEnd = i;
        } else {
            if (groupStart != -1) {
                bestStart = groupStart;
                bestEnd = groupEnd;
                groupStart = -1;
            }
        }
    }
    if (groupStart != -1) {
        bestStart = groupStart;
        bestEnd = groupEnd;
    }

    if (bestStart == -1) return;

    int vowelCount = bestEnd - bestStart + 1;
    bool isClosed = (bestEnd < (int)result.size() - 1);

    // Adjust for 'qu' and 'gi' which are initials but contain 'u' and 'i'.
    if (bestStart > 0 && VnOrtho::ToLower(result[bestStart-1]) == L'q' && VnOrtho::ToLower(result[bestStart]) == L'u' && vowelCount > 1) {
        bestStart++;
        vowelCount--;
    } else if (bestStart > 0 && VnOrtho::ToLower(result[bestStart-1]) == L'g' && VnOrtho::ToLower(result[bestStart]) == L'i' && vowelCount > 1) {
        bestStart++;
        vowelCount--;
    }

    // Special handling for modern Vietnamese tone placement on specific diphthongs.
    if (vowelCount == 2 && !isClosed) {
        wchar_t v1 = VnOrtho::ToLower(result[bestStart]);
        wchar_t v2 = VnOrtho::ToLower(result[bestEnd]);
        if ((v1 == L'o' && (v2 == L'a' || v2 == L'e')) ||
            (v1 == L'u' && v2 == L'y')) {
            result[bestEnd] = ApplyTone(result[bestEnd], tone);
            return;
        }
    }

    int target_idx;
    if (vowelCount == 1) {
        target_idx = bestStart;
    } else if (vowelCount == 2) {
        if (isClosed) {
            target_idx = bestEnd;
        } else {
            target_idx = bestStart;
        }
    } else {
        target_idx = bestStart + 1;
    }

    result[target_idx] = ApplyTone(result[target_idx], tone);
}

// Map a TELEX tone key to its tone index, or -1 if not a tone key
static int TelexToneIndex(wchar_t lower) {
    switch (lower) {
        case L's': return 1;
        case L'f': return 2;
        case L'r': return 3;
        case L'x': return 4;
        case L'j': return 5;
        case L'z': return 0;
        default: return -1;
    }
}

static bool IsPartOfInitial(const std::wstring& raw, size_t pos) {
    if (pos == 0) return true;
    wchar_t c1 = VnOrtho::ToLower(raw[pos-1]);
    wchar_t c2 = VnOrtho::ToLower(raw[pos]);
    
    if (c1 == L'c' && c2 == L'h') return true;
    if (c1 == L'g' && (c2 == L'h' || c2 == L'i')) return true;
    if (c1 == L'k' && c2 == L'h') return true;
    if (c1 == L'n' && (c2 == L'g' || c2 == L'h')) return true;
    if (c1 == L'p' && c2 == L'h') return true;
    if (c1 == L'q' && c2 == L'u') return true;
    if (c1 == L't' && (c2 == L'h' || c2 == L'r')) return true;
    
    if (pos >= 2) {
        wchar_t c0 = VnOrtho::ToLower(raw[pos-2]);
        if (c0 == L'n' && c1 == L'g' && c2 == L'h') return true;
    }
    
    return false;
}

std::wstring VnEngine::ApplyTelexRules(const std::wstring& raw) {
    if (raw.empty()) return L"";
    std::wstring result = L"";
    int active_tone = -1;
    bool tone_escaped = false;
    
    for (size_t idx = 0; idx < raw.size(); ++idx) {
        wchar_t ch = raw[idx];
        wchar_t lower = VnOrtho::ToLower(ch);
        
        // 1. Check if Tone key
        int tone_idx = TelexToneIndex(lower);
        // Smart Typo Restoration: tone key can be anywhere except at the start of initial cluster
        if (tone_idx >= 0 && !IsPartOfInitial(raw, idx)) {
            // Double-key escape
            if (active_tone == tone_idx && !tone_escaped) {
                active_tone = -1;
                tone_escaped = true;
                result += ch;
                continue;
            }
            if (tone_escaped) {
                result += ch;
                continue;
            }
            active_tone = tone_idx;
            continue; // absorb tone
        }
        
        // 2. Check Modifiers
        bool modified = false;
        if (lower == L'w') {
            bool modified_any = false;
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                wchar_t rc = VnOrtho::ToLower(result[i]);
                if (rc == L'o' || rc == L'u' || rc == L'a') {
                    if (rc == L'a') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ă' : L'ă';
                    else if (rc == L'o') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ơ' : L'ơ';
                    else if (rc == L'u') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ư' : L'ư';
                    modified_any = true;
                    if (rc == L'o' && i > 0 && VnOrtho::ToLower(result[i-1]) == L'u') {
                        result[i-1] = VnOrtho::IsUpper(result[i-1]) ? L'Ư' : L'ư';
                    }
                    break;
                }
            }
            if (modified_any) { modified = true; continue; }
        }
        else if (lower == L'a' || lower == L'e' || lower == L'o' || lower == L'd') {
            if (!result.empty()) {
                wchar_t prev_lower = VnOrtho::ToLower(result.back());
                if (lower == prev_lower) {
                    if (lower == L'a') result.back() = VnOrtho::IsUpper(result.back()) ? L'Â' : L'â';
                    else if (lower == L'e') result.back() = VnOrtho::IsUpper(result.back()) ? L'Ê' : L'ê';
                    else if (lower == L'o') result.back() = VnOrtho::IsUpper(result.back()) ? L'Ô' : L'ô';
                    else if (lower == L'd') result.back() = VnOrtho::IsUpper(result.back()) ? L'Đ' : L'đ';
                    modified = true;
                }
            }
        }
        
        if (!modified) {
            result += ch;
        }
    }
    
    ApplyToneToResult(result, active_tone, _toneType, _freeToneMarking);
    return result;
}

std::wstring VnEngine::ApplyVniRules(const std::wstring& raw) {
    if (raw.empty()) return L"";
    std::wstring result = L"";
    int active_tone = -1;
    bool tone_escaped = false;
    
    for (size_t idx = 0; idx < raw.size(); ++idx) {
        wchar_t ch = raw[idx];
        // 1. Check Tone
        if ((ch >= L'1' && ch <= L'5') || ch == L'0') {
            // Smart Restoration: allow tone digits anywhere except at the start
            if (idx > 0) {
                int tone_idx = ch - L'0'; 
                if (active_tone == tone_idx && !tone_escaped) {
                    active_tone = -1;
                    tone_escaped = true;
                    result += ch;
                    continue;
                }
                if (tone_escaped) {
                    result += ch;
                    continue;
                }
                active_tone = tone_idx;
                continue;
            }
        }
        
        // 2. Check Modifiers
        bool modified = false;
        if (ch == L'6' && idx > 0) {
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                wchar_t rc = VnOrtho::ToLower(result[i]);
                if (rc == L'a' || rc == L'e' || rc == L'o') {
                    if (rc == L'a') result[i] = VnOrtho::IsUpper(result[i]) ? L'Â' : L'â';
                    else if (rc == L'e') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ê' : L'ê';
                    else if (rc == L'o') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ô' : L'ô';
                    modified = true;
                    break;
                }
            }
            if (modified) continue;
        } else if (ch == L'7' && idx > 0) {
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                wchar_t rc = VnOrtho::ToLower(result[i]);
                if (rc == L'o' || rc == L'u') {
                    if (rc == L'o') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ơ' : L'ơ';
                    else if (rc == L'u') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ư' : L'ư';
                    modified = true;
                    if (rc == L'o' && i > 0 && VnOrtho::ToLower(result[i-1]) == L'u') {
                        result[i-1] = VnOrtho::IsUpper(result[i-1]) ? L'Ư' : L'ư';
                    }
                    break;
                }
            }
            if (modified) continue;
        } else if (ch == L'8' && idx > 0) {
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                if (VnOrtho::ToLower(result[i]) == L'a') {
                    result[i] = VnOrtho::IsUpper(result[i]) ? L'Ă' : L'ă';
                    modified = true;
                    break;
                }
            }
            if (modified) continue;
        } else if (ch == L'9' && idx > 0) {
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                if (VnOrtho::ToLower(result[i]) == L'd') {
                    result[i] = VnOrtho::IsUpper(result[i]) ? L'Đ' : L'đ';
                    modified = true;
                    break;
                }
            }
            if (modified) continue;
        }
        
        if (!modified) {
            result += ch;
        }
    }
    
    ApplyToneToResult(result, active_tone, _toneType, _freeToneMarking);
    return result;
}

// =============================================================================
// VIQR Input Method Rules
// =============================================================================
// VIQR (Vietnamese Quoted Readable) uses ASCII characters as diacritical marks:
// Tones: ' (acute/sắc), ` (grave/huyền), ? (hỏi), ~ (ngã), . (nặng)
// Modifiers: ^ (â/ê/ô), ( (ă), + (ơ/ư), dd (đ)
// =============================================================================

std::wstring VnEngine::ApplyViqrRules(const std::wstring& raw) {
    if (raw.empty()) return L"";
    std::wstring result = L"";
    int active_tone = -1;
    bool tone_escaped = false;
    
    for (size_t idx = 0; idx < raw.size(); ++idx) {
        wchar_t ch = raw[idx];
        
        // 1. Check Tone marks
        bool isTone = false;
        if (idx > 0) {
            int toneIdx = -1;
            if (ch == L'\'') toneIdx = 1;      // sắc (acute)
            else if (ch == L'`') toneIdx = 2;   // huyền (grave)
            else if (ch == L'?') toneIdx = 3;   // hỏi
            else if (ch == L'~') toneIdx = 4;   // ngã
            else if (ch == L'.') toneIdx = 5;   // nặng
            
            if (toneIdx >= 0) {
                if (active_tone == toneIdx && !tone_escaped) {
                    active_tone = -1;
                    tone_escaped = true;
                    result += ch;
                    continue;
                }
                if (tone_escaped) {
                    result += ch;
                    continue;
                }
                active_tone = toneIdx;
                isTone = true;
            }
        }
        if (isTone) continue;
        
        // 2. Check Modifiers
        bool modified = false;
        if (ch == L'^' && idx > 0) {
            // Circumflex: a->â, e->ê, o->ô
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                wchar_t rc = VnOrtho::ToLower(result[i]);
                if (rc == L'a' || rc == L'e' || rc == L'o') {
                    if (rc == L'a') result[i] = VnOrtho::IsUpper(result[i]) ? L'Â' : L'â';
                    else if (rc == L'e') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ê' : L'ê';
                    else if (rc == L'o') result[i] = VnOrtho::IsUpper(result[i]) ? L'Ô' : L'ô';
                    modified = true;
                    break;
                }
            }
        } else if (ch == L'(' && idx > 0) {
            // Breve: a->ă
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                if (VnOrtho::ToLower(result[i]) == L'a') {
                    result[i] = VnOrtho::IsUpper(result[i]) ? L'\u0102' : L'\u0103';
                    modified = true;
                    break;
                }
            }
        } else if (ch == L'+' && idx > 0) {
            // Horn: o->ơ, u->ư
            for (int i = (int)result.size() - 1; i >= 0; i--) {
                wchar_t rc = VnOrtho::ToLower(result[i]);
                if (rc == L'o' || rc == L'u') {
                    if (rc == L'o') result[i] = VnOrtho::IsUpper(result[i]) ? L'\u01A0' : L'\u01A1';
                    else if (rc == L'u') result[i] = VnOrtho::IsUpper(result[i]) ? L'\u01AF' : L'\u01B0';
                    modified = true;
                    break;
                }
            }
        } else if (ch == L'd' || ch == L'D') {
            // dd -> đ (only if previous char is also d/D)
            if (!result.empty()) {
                wchar_t prev = result.back();
                if (VnOrtho::ToLower(prev) == L'd') {
                    result.back() = VnOrtho::IsUpper(prev) ? L'\u0110' : L'\u0111';
                    modified = true;
                }
            }
        }
        
        if (!modified) {
            result += ch;
        }
    }
    
    ApplyToneToResult(result, active_tone, _toneType, _freeToneMarking);
    return result;
}
