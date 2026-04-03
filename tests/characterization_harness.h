#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "engine/macro.h"
#include "engine/vn_engine.h"
#include "shared_config.h"

class CharacterizationHarness {
public:
    CharacterizationHarness();

    void SetConfig(const UniKeyConfig& config);
    const UniKeyConfig& GetConfig() const;

    void SetCurrentApp(const std::wstring& appId);
    void ToggleInputEnabled();

    bool LoadMacroFile(const std::wstring& macroFilePath);

    void TypeKey(wchar_t ch);
    void TypeText(const std::wstring& text);

    std::wstring GetVisibleText() const;
    uint8_t GetInputEnabled() const;

private:
    void ApplyTypingSettingsToEngine();
    void CommitCompositionIntoVisibleText();
    void ClearCommittedReplay();
    bool IsRestoreReplayDelimiter(wchar_t ch) const;

    UniKeyConfig config_{};
    VnEngine engine_;
    MacroEngine macroEngine_;

    std::wstring visibleText_;

    std::wstring currentApp_;
    std::map<std::wstring, bool> perAppInputStates_;

    std::wstring lastCommittedRawWord_;
    std::wstring lastCommittedVisibleWord_;
    wchar_t lastCommitDelimiter_ = 0;
    bool hasRecentCommittedReplay_ = false;
    bool hasAppliedConfig_ = false;
};
