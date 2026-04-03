#include <gtest/gtest.h>

#include <filesystem>

#include "characterization_harness.h"
#include "engine/reconversion_word.h"
#include "engine/typing_settings.h"

namespace {

UniKeyConfig MakeDefaultConfig() {
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.inputEnabled = 1;
    config.inputMethod = IM_TELEX;
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
    config.macroFilePath[0] = L'\0';
    return config;
}

std::wstring FixtureMacroPath() {
    const std::filesystem::path thisFile = std::filesystem::path(__FILE__).parent_path();
    return (thisFile / L"fixtures" / L"task2_macros.ukm").wstring();
}

} // namespace

TEST(CharacterizationContract, VnOn_And_VnOff_Telex_Baseline) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness vnOn;
    vnOn.SetConfig(config);
    vnOn.TypeText(L"tieesng Vieetj");
    EXPECT_EQ(vnOn.GetVisibleText(), L"tiếng Việt");

    config.inputEnabled = 0;
    CharacterizationHarness vnOff;
    vnOff.SetConfig(config);
    vnOff.TypeText(L"tieesng Vieetj");
    EXPECT_EQ(vnOff.GetVisibleText(), L"tieesng Vieetj");
}

TEST(CharacterizationContract, InputMethodBaselines_Telex_Vni_Viqr) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness telex;
    config.inputMethod = IM_TELEX;
    telex.SetConfig(config);
    telex.TypeText(L"Vieejt");
    EXPECT_EQ(telex.GetVisibleText(), L"Việt");

    CharacterizationHarness vni;
    config.inputMethod = IM_VNI;
    vni.SetConfig(config);
    vni.TypeText(L"Vie65t");
    EXPECT_EQ(vni.GetVisibleText(), L"Việt");

    CharacterizationHarness viqr;
    config.inputMethod = IM_VIQR;
    viqr.SetConfig(config);
    viqr.TypeText(L"Vie^.");
    EXPECT_EQ(viqr.GetVisibleText(), L"Việ");
}

TEST(CharacterizationContract, DelimiterCommit_Baseline) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness harness;
    harness.SetConfig(config);
    harness.TypeText(L"tieengs, Vieejt.");

    EXPECT_EQ(harness.GetVisibleText(), L"tiếng, Việt.");
}

TEST(CharacterizationContract, HyphenCommit_Baseline) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness harness;
    harness.SetConfig(config);
    harness.TypeText(L"tieengs-vieejt");

    EXPECT_EQ(harness.GetVisibleText(), L"tiếng-việt");
}

TEST(CharacterizationContract, ToggleDuringInput_Baseline) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness harness;
    harness.SetConfig(config);

    harness.TypeText(L"Vieej");
    harness.ToggleInputEnabled();
    harness.TypeText(L"t");

    EXPECT_EQ(harness.GetVisibleText(), L"Việt");
}

TEST(CharacterizationContract, MacroExpansion_Baseline) {
    UniKeyConfig config = MakeDefaultConfig();
    config.macroEnabled = 1;

    CharacterizationHarness harness;
    harness.SetConfig(config);

    ASSERT_TRUE(harness.LoadMacroFile(FixtureMacroPath()));

    harness.TypeText(L"btw ");
    EXPECT_EQ(harness.GetVisibleText(), L"bây giờ ");
}

TEST(CharacterizationContract, RestoreReconversion_Baseline) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness harness;
    harness.SetConfig(config);

    harness.TypeText(L"tieeng ");
    harness.TypeText(L"s");

    EXPECT_EQ(harness.GetVisibleText(), L"tiếng");
}

TEST(CharacterizationContract, RestoreReconversion_DoesNotCrossHyphenBoundary) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness harness;
    harness.SetConfig(config);

    harness.TypeText(L"tieeng-");
    harness.TypeText(L"s");

    EXPECT_EQ(harness.GetVisibleText(), L"tiêng-s");
}

TEST(CharacterizationContract, ReconversionHelper_HyphenBoundaryDoesNotReplayPreviousWord) {
    const Reconversion::WordSpan span = Reconversion::ExtractWordSpanAroundCaret(
        L"tieeng-",
        L"",
        IM_TELEX,
        true);

    EXPECT_FALSE(span.HasWord());
    EXPECT_FALSE(span.hasTrailingWhitespaceDelimiter);
    EXPECT_EQ(span.leftDelimiterCount, 0);
}

TEST(CharacterizationContract, ReconversionHelper_TrailingDelimiterWordSpan) {
    const Reconversion::WordSpan span = Reconversion::ExtractWordSpanAroundCaret(
        L"tieeng ",
        L"",
        IM_TELEX,
        true);

    ASSERT_TRUE(span.HasWord());
    EXPECT_EQ(span.word, L"tieeng");
    EXPECT_TRUE(span.hasTrailingWhitespaceDelimiter);
    EXPECT_TRUE(span.usedTrailingLeftWord);
    EXPECT_TRUE(Reconversion::CanReplayCommittedModifier(span, true, true));
}

TEST(CharacterizationContract, PerAppInputStateExpectation_Baseline) {
    {
        UniKeyConfig globalOnly = MakeDefaultConfig();
        globalOnly.perAppInputState = 0;

        CharacterizationHarness globalHarness;
        globalHarness.SetConfig(globalOnly);
        globalHarness.SetCurrentApp(L"notepad.exe");
        globalHarness.ToggleInputEnabled();
        EXPECT_EQ(globalHarness.GetInputEnabled(), 0);

        globalHarness.SetCurrentApp(L"code.exe");
        EXPECT_EQ(globalHarness.GetInputEnabled(), 0);
    }

    UniKeyConfig config = MakeDefaultConfig();
    config.perAppInputState = 1;

    CharacterizationHarness harness;
    harness.SetConfig(config);

    harness.SetCurrentApp(L"notepad.exe");
    harness.ToggleInputEnabled();
    EXPECT_EQ(harness.GetInputEnabled(), 0);

    harness.SetCurrentApp(L"code.exe");
    EXPECT_EQ(harness.GetInputEnabled(), 0);
    harness.ToggleInputEnabled();
    EXPECT_EQ(harness.GetInputEnabled(), 1);

    harness.SetCurrentApp(L"notepad.exe");
    EXPECT_EQ(harness.GetInputEnabled(), 0);
}

TEST(CharacterizationContract, TypingRelevantSettings_Baselines) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness modern;
    config.toneType = TONE_MODERN;
    modern.SetConfig(config);
    modern.TypeText(L"hoas");
    EXPECT_EQ(modern.GetVisibleText(), L"hoá");

    CharacterizationHarness classic;
    config.toneType = TONE_CLASSIC;
    classic.SetConfig(config);
    classic.TypeText(L"hoas");
    EXPECT_EQ(classic.GetVisibleText(), L"hóa");

    CharacterizationHarness spellcheckOn;
    config = MakeDefaultConfig();
    config.spellCheck = 1;
    spellcheckOn.SetConfig(config);
    spellcheckOn.TypeText(L"windows");
    EXPECT_EQ(spellcheckOn.GetVisibleText(), L"windows");

    CharacterizationHarness spellcheckOff;
    config.spellCheck = 0;
    spellcheckOff.SetConfig(config);
    spellcheckOff.TypeText(L"windows");
    EXPECT_EQ(spellcheckOff.GetVisibleText(), L"windớ");

    CharacterizationHarness freeToneOff;
    config = MakeDefaultConfig();
    config.freeToneMarking = 0;
    freeToneOff.SetConfig(config);
    freeToneOff.TypeText(L"rooif");
    EXPECT_EQ(freeToneOff.GetVisibleText(), L"rồi");

    CharacterizationHarness freeToneOn;
    config.freeToneMarking = 1;
    freeToneOn.SetConfig(config);
    freeToneOn.TypeText(L"rooif");
    EXPECT_EQ(freeToneOn.GetVisibleText(), L"rồi");

    CharacterizationHarness restoreDisabled;
    config = MakeDefaultConfig();
    config.restoreKeyEnabled = 0;
    restoreDisabled.SetConfig(config);
    restoreDisabled.TypeText(L"tieeng ");
    restoreDisabled.TypeText(L"s");
    EXPECT_EQ(restoreDisabled.GetVisibleText(), L"tiêng s");
}

TEST(CharacterizationContract, TypingSettings_PropagateLiveWithoutRestart) {
    UniKeyConfig config = MakeDefaultConfig();

    CharacterizationHarness harness;
    harness.SetConfig(config);
    harness.TypeText(L"hoas ");
    EXPECT_EQ(harness.GetVisibleText(), L"hoá ");

    config.toneType = TONE_CLASSIC;
    harness.SetConfig(config);
    harness.TypeText(L"hoas ");
    EXPECT_EQ(harness.GetVisibleText(), L"hoá hóa ");

    config.spellCheck = 0;
    config.freeToneMarking = 0;
    config.restoreKeyEnabled = 0;
    harness.SetConfig(config);
    harness.TypeText(L"windows ");

    const std::wstring visible = harness.GetVisibleText();
    EXPECT_TRUE(visible.rfind(L"hoá hóa ", 0) == 0);
    EXPECT_NE(visible, L"hoá hóa windows ");
    EXPECT_EQ(visible.back(), L' ');
}

TEST(CharacterizationContract, MacroConfigDelta_DetectsReloadAndClear) {
    UniKeyConfig baseline = MakeDefaultConfig();
    UniKeyConfig withMacroA = baseline;
    withMacroA.macroEnabled = 1;
    wcsncpy_s(withMacroA.macroFilePath, _countof(withMacroA.macroFilePath), L"C:\\temp\\macro_a.ukm", _TRUNCATE);

    const MacroConfigDelta initialEnableDelta = EvaluateMacroConfigDelta(&baseline, withMacroA);
    EXPECT_TRUE(initialEnableDelta.shouldReload);
    EXPECT_FALSE(initialEnableDelta.shouldClear);
    EXPECT_EQ(initialEnableDelta.macroFilePath, L"C:\\temp\\macro_a.ukm");

    UniKeyConfig withMacroB = withMacroA;
    wcsncpy_s(withMacroB.macroFilePath, _countof(withMacroB.macroFilePath), L"C:\\temp\\macro_b.ukm", _TRUNCATE);
    const MacroConfigDelta pathChangeDelta = EvaluateMacroConfigDelta(&withMacroA, withMacroB);
    EXPECT_TRUE(pathChangeDelta.shouldReload);
    EXPECT_FALSE(pathChangeDelta.shouldClear);
    EXPECT_EQ(pathChangeDelta.macroFilePath, L"C:\\temp\\macro_b.ukm");

    UniKeyConfig macroDisabled = withMacroB;
    macroDisabled.macroEnabled = 0;
    const MacroConfigDelta disableDelta = EvaluateMacroConfigDelta(&withMacroB, macroDisabled);
    EXPECT_FALSE(disableDelta.shouldReload);
    EXPECT_TRUE(disableDelta.shouldClear);
}
