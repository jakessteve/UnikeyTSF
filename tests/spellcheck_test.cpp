#include <gtest/gtest.h>
#include "engine/vn_engine.h"
#include "shared_config.h"

// Helper: feed a string of chars into VnEngine and return composition
static std::wstring TypeWord(VnEngine& eng, const std::wstring& input, InputMethod method) {
    eng.Clear();
    for (wchar_t ch : input) {
        eng.ProcessKey(ch, method);
    }
    return eng.GetCompositionString();
}

// =============================================================================
// Smart Typo Restoration Tests
// =============================================================================
TEST(SmartRestorationTest, Telex_ToneBeforeVowel) {
    VnEngine engine;
    engine.SetFreeToneMarking(true);
    // "hso" -> "hó". 's' is a tone mark typed before the vowel 'o'.
    EXPECT_EQ(TypeWord(engine, L"hso", IM_TELEX), L"h\xF3");
}

TEST(SmartRestorationTest, Vni_ToneBeforeVowel) {
    VnEngine engine;
    engine.SetFreeToneMarking(true);
    // "h1o" -> "hó". '1' is a tone mark typed before the vowel 'o'.
    EXPECT_EQ(TypeWord(engine, L"h1o", IM_VNI), L"h\xF3");
}

TEST(SmartRestorationTest, Viqr_ToneBeforeVowel) {
    VnEngine engine;
    engine.SetFreeToneMarking(true);
    // "h'o" -> "hó". ''' is a tone mark typed before the vowel 'o'.
    EXPECT_EQ(TypeWord(engine, L"h'o", IM_VIQR), L"h\xF3");
}

TEST(SmartRestorationTest, ToneBeforeVowel_RequiresFreeToneMarking) {
    VnEngine telex;
    telex.SetFreeToneMarking(false);
    EXPECT_EQ(TypeWord(telex, L"hso", IM_TELEX), L"hso");

    VnEngine vni;
    vni.SetFreeToneMarking(false);
    EXPECT_EQ(TypeWord(vni, L"h1o", IM_VNI), L"h1o");

    VnEngine viqr;
    viqr.SetFreeToneMarking(false);
    EXPECT_EQ(TypeWord(viqr, L"h'o", IM_VIQR), L"h'o");
}

TEST(SmartRestorationTest, Telex_InitialNotATone) {
    VnEngine engine;
    // "sang" -> "sang" (s at start is initial, not tone)
    EXPECT_EQ(TypeWord(engine, L"sang", IM_TELEX), L"sang");
}

TEST(SmartRestorationTest, Telex_ClusterNotATone) {
    VnEngine engine;
    // "tra" -> "tra" (r in tr is cluster, not tone)
    EXPECT_EQ(TypeWord(engine, L"tra", IM_TELEX), L"tra");
}

// =============================================================================
// Vietnamese Spell Check Tests
// =============================================================================
class SpellCheckTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetSpellCheck(true);
    }
};

TEST_F(SpellCheckTest, ValidWord_Hoc) {
    EXPECT_EQ(TypeWord(engine, L"hocj", IM_TELEX), L"học");
}

TEST_F(SpellCheckTest, InvalidWord_Hho) {
    // "hho" is invalid initial cluster
    EXPECT_EQ(TypeWord(engine, L"hho", IM_TELEX), L"hho");
}

TEST_F(SpellCheckTest, InvalidWord_Tieg) {
    // "tieg" is invalid final
    EXPECT_EQ(TypeWord(engine, L"tieg", IM_TELEX), L"tieg");
}

TEST_F(SpellCheckTest, InvalidTone_HocSac) {
    // "học" with sắc is invalid because 'c' final only allows sắc/nặng, 
    // but wait, 'hóc' IS valid. 
    // Let's try "hòc" (huyền) which is invalid.
    EXPECT_EQ(TypeWord(engine, L"hocf", IM_TELEX), L"hocf");
}

TEST_F(SpellCheckTest, ValidWord_Tieng) {
    EXPECT_EQ(TypeWord(engine, L"tieengs", IM_TELEX), L"tiếng");
}

TEST_F(SpellCheckTest, InvalidWord_Kha) {
    // "ka" should be "ca". "k" only with e, ê, i
    EXPECT_EQ(TypeWord(engine, L"ka", IM_TELEX), L"ka");
}

TEST_F(SpellCheckTest, ValidWord_Ke) {
    EXPECT_EQ(TypeWord(engine, L"ke", IM_TELEX), L"ke");
}

TEST_F(SpellCheckTest, InvalidWord_Ce) {
    // "ce" should be "ke"
    EXPECT_EQ(TypeWord(engine, L"ce", IM_TELEX), L"ce");
}

// Regression tests for previously missing vowel clusters
TEST_F(SpellCheckTest, ValidWord_Cau) {
    // "câu" — tests the 'âu' vowel cluster
    EXPECT_EQ(TypeWord(engine, L"caaus", IM_TELEX), L"c\x1EA5u");
}

TEST_F(SpellCheckTest, ValidWord_Keu) {
    // "kêu" — tests the 'êu' vowel cluster
    EXPECT_EQ(TypeWord(engine, L"keeu", IM_TELEX), L"k\x00EAu");
}

TEST_F(SpellCheckTest, ValidWord_Gui) {
    // "gửi" — tests the 'ưi' vowel cluster
    EXPECT_EQ(TypeWord(engine, L"guwir", IM_TELEX), L"g\x1EEDi");
}
