#pragma once

#include <string>
#include <vector>

#include "../shared_config.h"

enum class SettingsHostMessageType {
    Invalid = 0,
    UpdateConfig,
    UpdateBlacklist,
    RequestConfig,
    UiReady,
    SetAutostart,
    RequestBlacklist,
};

struct SettingsHostMessage {
    SettingsHostMessageType type = SettingsHostMessageType::Invalid;
    UniKeyConfig config{};
    std::vector<std::wstring> blacklist;
    bool enabled = false;
};

bool ParseSettingsHostMessage(const std::wstring& message, SettingsHostMessage* parsed);
std::wstring BuildSettingsConfigMessage(const UniKeyConfig& config);
std::wstring BuildAutostartStateMessage(bool enabled);
std::wstring BuildBlacklistDataMessage(const std::vector<std::wstring>& list);
