#pragma once

#include <cwctype>

namespace InputDelimiters {

inline bool IsReplayEligibleDelimiter(wchar_t ch)
{
    return ch == L' ' || ch == L'\t' || ch == L'\r' || ch == L'\n';
}

inline bool IsExplicitHyphenDelimiter(wchar_t ch)
{
    return ch == L'-' || ch == 0x2010 || ch == 0x2011;
}

inline bool IsExplicitWordBoundary(wchar_t ch)
{
    return IsReplayEligibleDelimiter(ch) || IsExplicitHyphenDelimiter(ch);
}

inline bool IsPunctuationDelimiter(wchar_t ch)
{
    return std::iswpunct(ch) != 0;
}

} // namespace InputDelimiters
