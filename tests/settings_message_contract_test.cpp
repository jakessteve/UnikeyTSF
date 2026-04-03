#include <gtest/gtest.h>

#include "ui/settings_message_contract.h"

namespace {

UniKeyConfig MakeConfig()
{
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.inputEnabled = 1;
    config.inputMethod = IM_VNI;
    config.charset = CS_UNICODE;
    config.toneType = TONE_MODERN;
    config.spellCheck = 1;
    config.macroEnabled = 0;
    config.freeToneMarking = 1;
    config.toggleKey = TK_CTRL_SHIFT;
    config.restoreKeyEnabled = 1;
    config.useClipboardForUnicode = 0;
    config.showDialogOnStartup = 1;
    config.perAppInputState = 0;
    wcsncpy_s(config.macroFilePath, L"C:\\temp\\macros.ukm", _TRUNCATE);
    return config;
}

} // namespace

TEST(SettingsMessageContractTests, ParsesCanonicalUpdateConfigPayload)
{
    const std::wstring message =
        L"{\"type\":\"UPDATE_CONFIG\",\"payload\":{"
        L"\"version\":2,"
        L"\"inputEnabled\":true,"
        L"\"inputMethod\":1,"
        L"\"charset\":0,"
        L"\"toneType\":0,"
        L"\"spellCheck\":true,"
        L"\"macroEnabled\":false,"
        L"\"freeToneMarking\":true,"
        L"\"toggleKey\":0,"
        L"\"restoreKeyEnabled\":true,"
        L"\"useClipboardForUnicode\":false,"
        L"\"showDialogOnStartup\":true,"
        L"\"perAppInputState\":false,"
        L"\"macroFilePath\":\"C:\\\\temp\\\\macros.ukm\"}}";

    SettingsHostMessage parsed;
    ASSERT_TRUE(ParseSettingsHostMessage(message, &parsed));
    EXPECT_EQ(parsed.type, SettingsHostMessageType::UpdateConfig);
    EXPECT_EQ(parsed.config.version, UNIKEY_CONFIG_VERSION);
    EXPECT_EQ(parsed.config.inputMethod, IM_VNI);
    EXPECT_STREQ(parsed.config.macroFilePath, L"C:\\temp\\macros.ukm");
}

TEST(SettingsMessageContractTests, RejectsVersionDriftInUpdateConfigPayload)
{
    const std::wstring message =
        L"{\"type\":\"UPDATE_CONFIG\",\"payload\":{"
        L"\"version\":3,"
        L"\"inputEnabled\":true,"
        L"\"inputMethod\":1,"
        L"\"charset\":0,"
        L"\"toneType\":0,"
        L"\"spellCheck\":true,"
        L"\"macroEnabled\":false,"
        L"\"freeToneMarking\":true,"
        L"\"toggleKey\":0,"
        L"\"restoreKeyEnabled\":true,"
        L"\"useClipboardForUnicode\":false,"
        L"\"showDialogOnStartup\":true,"
        L"\"perAppInputState\":false,"
        L"\"macroFilePath\":\"\"}}";

    SettingsHostMessage parsed;
    EXPECT_FALSE(ParseSettingsHostMessage(message, &parsed));
}

TEST(SettingsMessageContractTests, ParsesBlacklistAndAutostartMessages)
{
    SettingsHostMessage blacklistMessage;
    ASSERT_TRUE(ParseSettingsHostMessage(
        L"{\"type\":\"UPDATE_BLACKLIST\",\"value\":[\"notepad.exe\",\"code.exe\"]}",
        &blacklistMessage));
    EXPECT_EQ(blacklistMessage.type, SettingsHostMessageType::UpdateBlacklist);
    ASSERT_EQ(blacklistMessage.blacklist.size(), 2u);
    EXPECT_EQ(blacklistMessage.blacklist[0], L"notepad.exe");
    EXPECT_EQ(blacklistMessage.blacklist[1], L"code.exe");

    SettingsHostMessage autostartMessage;
    ASSERT_TRUE(ParseSettingsHostMessage(
        L"{\"type\":\"SET_AUTOSTART\",\"enabled\":true}",
        &autostartMessage));
    EXPECT_EQ(autostartMessage.type, SettingsHostMessageType::SetAutostart);
    EXPECT_TRUE(autostartMessage.enabled);
}

TEST(SettingsMessageContractTests, BuildsOutboundMessagesWithExpectedEnvelope)
{
    const UniKeyConfig config = MakeConfig();

    const std::wstring configMessage = BuildSettingsConfigMessage(config);
    EXPECT_NE(configMessage.find(L"\"type\":\"SET_CONFIG\""), std::wstring::npos);
    EXPECT_NE(configMessage.find(L"\"version\":2"), std::wstring::npos);
    EXPECT_NE(configMessage.find(L"\"inputMethod\":1"), std::wstring::npos);

    const std::wstring blacklistMessage = BuildBlacklistDataMessage({L"notepad.exe", L"code.exe"});
    EXPECT_NE(blacklistMessage.find(L"\"type\":\"BLACKLIST_DATA\""), std::wstring::npos);
    EXPECT_NE(blacklistMessage.find(L"notepad.exe"), std::wstring::npos);

    const std::wstring autostartMessage = BuildAutostartStateMessage(true);
    EXPECT_EQ(autostartMessage, L"{\"type\":\"AUTOSTART_STATE\",\"enabled\":true}");
}
