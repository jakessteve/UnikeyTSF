#include "characterization_harness.h"

#include "engine/delimiter_policy.h"

#include "engine/reconversion_word.h"
#include "engine/typing_settings.h"

namespace {

InputMethod EffectiveInputMethod(const UniKeyConfig& config) {
    return static_cast<InputMethod>(config.inputMethod);
}

ToneType EffectiveToneType(const UniKeyConfig& config) {
    return static_cast<ToneType>(config.toneType);
}

} // namespace

CharacterizationHarness::CharacterizationHarness() {
    config_.version = UNIKEY_CONFIG_VERSION;
    config_.inputEnabled = 1;
    config_.inputMethod = IM_VNI;
    config_.charset = CS_UNICODE;
    config_.toneType = TONE_MODERN;
    config_.spellCheck = 1;
    config_.macroEnabled = 0;
    config_.freeToneMarking = 1;
    config_.toggleKey = TK_CTRL_SHIFT;
    config_.restoreKeyEnabled = 1;
    config_.useClipboardForUnicode = 0;
    config_.showDialogOnStartup = 1;
    config_.perAppInputState = 0;
    config_.macroFilePath[0] = L'\0';

    ApplyTypingSettingsToEngine();
}

void CharacterizationHarness::SetConfig(const UniKeyConfig& config) {
    const MacroConfigDelta macroDelta = EvaluateMacroConfigDelta(
        hasAppliedConfig_ ? &config_ : nullptr,
        config);

    config_ = config;
    ApplyTypingSettingsToEngine();

    if (macroDelta.shouldClear) {
        macroEngine_.Clear();
    }
    if (macroDelta.shouldReload) {
        macroEngine_.Load(macroDelta.macroFilePath);
    }

    hasAppliedConfig_ = true;
}

const UniKeyConfig& CharacterizationHarness::GetConfig() const {
    return config_;
}

void CharacterizationHarness::SetCurrentApp(const std::wstring& appId) {
    if (appId == currentApp_) {
        return;
    }

    if (config_.perAppInputState && !currentApp_.empty()) {
        perAppInputStates_[currentApp_] = (config_.inputEnabled == 1);
    }

    currentApp_ = appId;

    if (config_.perAppInputState) {
        auto it = perAppInputStates_.find(currentApp_);
        if (it != perAppInputStates_.end()) {
            config_.inputEnabled = it->second ? 1 : 0;
        }
    }

    engine_.Clear();
    ClearCommittedReplay();
}

void CharacterizationHarness::ToggleInputEnabled() {
    CommitCompositionIntoVisibleText();

    config_.inputEnabled = config_.inputEnabled ? 0 : 1;
    if (config_.perAppInputState && !currentApp_.empty()) {
        perAppInputStates_[currentApp_] = (config_.inputEnabled == 1);
    }

    engine_.Clear();
    ClearCommittedReplay();
}

bool CharacterizationHarness::LoadMacroFile(const std::wstring& macroFilePath) {
    if (macroFilePath.empty()) {
        return false;
    }
    return macroEngine_.Load(macroFilePath);
}

void CharacterizationHarness::TypeKey(wchar_t ch) {
    if (!config_.inputEnabled) {
        CommitCompositionIntoVisibleText();
        visibleText_ += ch;
        return;
    }

    const InputMethod method = EffectiveInputMethod(config_);

    if (!engine_.IsInWord() &&
        hasRecentCommittedReplay_ &&
        config_.restoreKeyEnabled &&
        IsRestoreReplayDelimiter(lastCommitDelimiter_) &&
        engine_.IsPotentialModifier(ch, method)) {

        const std::wstring leftText = lastCommittedRawWord_ + std::wstring(1, lastCommitDelimiter_);
        const Reconversion::WordSpan committedSpan = Reconversion::ExtractWordSpanAroundCaret(
            leftText,
            L"",
            method,
            true);

        if (Reconversion::CanReplayCommittedModifier(committedSpan, true, true)) {
            const std::wstring committedSuffix = lastCommittedVisibleWord_ + std::wstring(1, lastCommitDelimiter_);
            if (!committedSuffix.empty() &&
                visibleText_.size() >= committedSuffix.size() &&
                visibleText_.compare(visibleText_.size() - committedSuffix.size(), committedSuffix.size(), committedSuffix) == 0) {
                visibleText_.erase(visibleText_.size() - committedSuffix.size());
            }

            for (wchar_t existing : committedSpan.word) {
                engine_.ProcessKey(existing, method);
            }
        }
    }

    const bool hadWordBeforeKey = engine_.IsInWord();
    const std::wstring compositionBeforeKey = engine_.GetCompositionString();
    const std::wstring rawBeforeKey = engine_.GetRawString();

    const bool eaten = engine_.ProcessKey(ch, method);
    if (eaten) {
        hasRecentCommittedReplay_ = false;
        return;
    }

    if (hadWordBeforeKey) {
        std::wstring committedWord = compositionBeforeKey;
        if (config_.macroEnabled) {
            const std::wstring expanded = macroEngine_.Expand(committedWord);
            if (!expanded.empty()) {
                committedWord = expanded;
            }
        }

        visibleText_ += committedWord;

        if (IsRestoreReplayDelimiter(ch)) {
            lastCommittedRawWord_ = rawBeforeKey;
            lastCommittedVisibleWord_ = committedWord;
            lastCommitDelimiter_ = ch;
            hasRecentCommittedReplay_ = true;
        } else {
            ClearCommittedReplay();
        }
    } else {
        ClearCommittedReplay();
    }

    visibleText_ += ch;
}

void CharacterizationHarness::TypeText(const std::wstring& text) {
    for (wchar_t ch : text) {
        TypeKey(ch);
    }
}

std::wstring CharacterizationHarness::GetVisibleText() const {
    return visibleText_ + engine_.GetCompositionString();
}

uint8_t CharacterizationHarness::GetInputEnabled() const {
    return config_.inputEnabled;
}

void CharacterizationHarness::ApplyTypingSettingsToEngine() {
    ::ApplyTypingSettingsToEngine(config_, engine_);
}

void CharacterizationHarness::CommitCompositionIntoVisibleText() {
    if (engine_.IsInWord()) {
        visibleText_ += engine_.GetCompositionString();
        engine_.Clear();
    }
}

void CharacterizationHarness::ClearCommittedReplay() {
    lastCommittedRawWord_.clear();
    lastCommittedVisibleWord_.clear();
    lastCommitDelimiter_ = 0;
    hasRecentCommittedReplay_ = false;
}

bool CharacterizationHarness::IsRestoreReplayDelimiter(wchar_t ch) const {
    return InputDelimiters::IsReplayEligibleDelimiter(ch);
}
