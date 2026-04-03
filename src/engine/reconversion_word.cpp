#include "reconversion_word.h"

#include <cwctype>
#include "delimiter_policy.h"
#include "vn_engine.h"

namespace Reconversion {

namespace {

bool IsEditableCommittedChar(wchar_t ch, InputMethod method)
{
    VnEngine engine;
    return engine.IsWordChar(ch, method) || engine.IsPotentialModifier(ch, method);
}

} // namespace

WordSpan ExtractWordSpanAroundCaret(
    const std::wstring& leftText,
    const std::wstring& rightText,
    InputMethod method,
    bool allowTrailingLeftWord)
{
    WordSpan result;

    int leftIndex = static_cast<int>(leftText.length()) - 1;
    while (leftIndex >= 0 && IsEditableCommittedChar(leftText[leftIndex], method)) {
        result.leftEditableCount++;
        leftIndex--;
    }

    int rightIndex = 0;
    while (rightIndex < static_cast<int>(rightText.length()) &&
           IsEditableCommittedChar(rightText[rightIndex], method)) {
        result.rightEditableCount++;
        rightIndex++;
    }

    if (result.leftEditableCount > 0 || result.rightEditableCount > 0) {
        const size_t start = leftText.length() - static_cast<size_t>(result.leftEditableCount);
        result.word = leftText.substr(start, result.leftEditableCount);
        result.word.append(rightText.substr(0, result.rightEditableCount));
        return result;
    }

    if (!allowTrailingLeftWord) {
        return result;
    }

    while (leftIndex >= 0 && !IsEditableCommittedChar(leftText[leftIndex], method)) {
        if (result.leftDelimiterCount == 0 && InputDelimiters::IsReplayEligibleDelimiter(leftText[leftIndex])) {
            result.hasTrailingWhitespaceDelimiter = true;
        }
        result.leftDelimiterCount++;
        leftIndex--;
    }

    if (!result.hasTrailingWhitespaceDelimiter || result.leftDelimiterCount == 0) {
        result.leftDelimiterCount = 0;
        result.hasTrailingWhitespaceDelimiter = false;
        return result;
    }

    const int wordEnd = leftIndex;
    while (leftIndex >= 0 && IsEditableCommittedChar(leftText[leftIndex], method)) {
        result.leftEditableCount++;
        leftIndex--;
    }

    if (result.leftEditableCount == 0) {
        result.leftDelimiterCount = 0;
        result.hasTrailingWhitespaceDelimiter = false;
        return result;
    }

    const int start = wordEnd - result.leftEditableCount + 1;
    result.word = leftText.substr(start, result.leftEditableCount);
    result.usedTrailingLeftWord = true;
    return result;
}

bool CanReplayCommittedModifier(
    const WordSpan& span,
    bool selectionEmpty,
    bool withinRecentCommitWindow)
{
    return selectionEmpty && withinRecentCommitWindow && span.HasWord();
}

} // namespace Reconversion
