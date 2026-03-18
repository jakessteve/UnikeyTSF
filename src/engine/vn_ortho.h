#pragma once
#include <string>

// Vietnamese Orthography helper functions
namespace VnOrtho {
    // Check if a syllable is valid according to Vietnamese rules.
    bool IsSyllableValid(const std::wstring& syllable);

    // Character classification and handling
    bool IsVowel(wchar_t ch);
    bool IsModifiedVowel(wchar_t ch);
    
    // Locale-independent Vietnamese casing
    wchar_t ToLower(wchar_t ch);
    wchar_t ToUpper(wchar_t ch);
    bool IsUpper(wchar_t ch);

    // Decompose a syllable into its components.
    struct SyllableParts {
        std::wstring initial;
        std::wstring vowels;
        std::wstring final;
        int tone; // 0-5
    };
    SyllableParts Decompose(const std::wstring& syllable);
}
