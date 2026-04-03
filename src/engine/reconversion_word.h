#pragma once

#include <string>
#include "../shared_config.h"

namespace Reconversion {

struct WordSpan {
    std::wstring word;
    int leftEditableCount = 0;
    int rightEditableCount = 0;
    int leftDelimiterCount = 0;
    bool hasTrailingWhitespaceDelimiter = false;
    bool usedTrailingLeftWord = false;

    bool HasWord() const { return !word.empty(); }
    int ReplacementStartShift() const { return -(leftEditableCount + leftDelimiterCount); }
    int ReplacementEndShift() const { return rightEditableCount - leftDelimiterCount; }
};

WordSpan ExtractWordSpanAroundCaret(
    const std::wstring& leftText,
    const std::wstring& rightText,
    InputMethod method,
    bool allowTrailingLeftWord);

bool CanReplayCommittedModifier(
    const WordSpan& span,
    bool selectionEmpty,
    bool withinRecentCommitWindow);

} // namespace Reconversion
