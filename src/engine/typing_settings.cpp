#include "typing_settings.h"

#include <cwchar>

void ApplyTypingSettingsToEngine(const UniKeyConfig& config, VnEngine& engine)
{
    engine.SetToneType(static_cast<ToneType>(config.toneType));
    engine.SetSpellCheck(config.spellCheck != 0);
    engine.SetFreeToneMarking(config.freeToneMarking != 0);
}

MacroConfigDelta EvaluateMacroConfigDelta(const UniKeyConfig* previousConfig, const UniKeyConfig& currentConfig)
{
    MacroConfigDelta delta;

    const bool currentMacroEnabled = currentConfig.macroEnabled != 0;
    const bool currentHasMacroPath = currentConfig.macroFilePath[0] != L'\0';

    const bool previousMacroEnabled = previousConfig && previousConfig->macroEnabled != 0;
    const bool previousHasMacroPath = previousConfig && previousConfig->macroFilePath[0] != L'\0';

    const bool macroEnabledChanged = previousConfig && (previousMacroEnabled != currentMacroEnabled);
    const bool macroPathChanged = previousConfig && std::wcscmp(previousConfig->macroFilePath, currentConfig.macroFilePath) != 0;

    if (!currentMacroEnabled || !currentHasMacroPath) {
        delta.shouldClear = previousMacroEnabled || previousHasMacroPath || macroEnabledChanged || macroPathChanged;
        return delta;
    }

    if (!previousConfig || !previousMacroEnabled || macroPathChanged || macroEnabledChanged) {
        delta.shouldReload = true;
        delta.macroFilePath = currentConfig.macroFilePath;
    }

    return delta;
}
