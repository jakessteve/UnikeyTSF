#pragma once

#include <string>

#include "vn_engine.h"

struct MacroConfigDelta {
    bool shouldReload = false;
    bool shouldClear = false;
    std::wstring macroFilePath;
};

void ApplyTypingSettingsToEngine(const UniKeyConfig& config, VnEngine& engine);
MacroConfigDelta EvaluateMacroConfigDelta(const UniKeyConfig* previousConfig, const UniKeyConfig& currentConfig);
