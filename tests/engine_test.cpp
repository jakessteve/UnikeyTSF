// =============================================================================
// UniKey TSF Reborn — Engine Unit Tests
// =============================================================================
// Comprehensive tests for VnEngine covering:
//   - Telex input (basic, modifiers, tones, double-key escape, mixed case)
//   - VNI input (tones, modifiers, edge cases)
//   - VIQR input (tones, circumflex, breve, horn, dd)
//   - Tone placement (modern vs classic, free tone marking)
//   - Backspace handling and word boundary detection
//   - Macro expansion
// =============================================================================

#include <gtest/gtest.h>
#include "engine/vn_engine.h"
#include "engine/macro.h"
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
// Telex Basic Input
// =============================================================================
class TelexTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetToneType(TONE_MODERN);
        engine.SetFreeToneMarking(false);
    }
};

TEST_F(TelexTest, SimpleWord_Viet) {
    EXPECT_EQ(TypeWord(engine, L"Vieejt", IM_TELEX), L"Việt");
}

TEST_F(TelexTest, SimpleWord_Nam) {
    EXPECT_EQ(TypeWord(engine, L"Nam", IM_TELEX), L"Nam");
}

TEST_F(TelexTest, CircumflexA) {
    EXPECT_EQ(TypeWord(engine, L"aas", IM_TELEX), L"ấ");
}

TEST_F(TelexTest, CircumflexE) {
    EXPECT_EQ(TypeWord(engine, L"ees", IM_TELEX), L"ế");
}

TEST_F(TelexTest, CircumflexO) {
    EXPECT_EQ(TypeWord(engine, L"oos", IM_TELEX), L"ố");
}

TEST_F(TelexTest, BreveA) {
    EXPECT_EQ(TypeWord(engine, L"aw", IM_TELEX), L"ă");
}

TEST_F(TelexTest, HornO) {
    EXPECT_EQ(TypeWord(engine, L"ow", IM_TELEX), L"ơ");
}

TEST_F(TelexTest, HornU) {
    EXPECT_EQ(TypeWord(engine, L"uw", IM_TELEX), L"ư");
}

TEST_F(TelexTest, StrokeD) {
    EXPECT_EQ(TypeWord(engine, L"dd", IM_TELEX), L"đ");
}

TEST_F(TelexTest, StrokeD_Uppercase) {
    EXPECT_EQ(TypeWord(engine, L"DD", IM_TELEX), L"Đ");
}

TEST_F(TelexTest, HornUO) {
    // "uow" -> ươ
    EXPECT_EQ(TypeWord(engine, L"uow", IM_TELEX), L"ươ");
}

// --- Tone Tests ---
TEST_F(TelexTest, ToneSac) {
    // 's' applies sắc (acute)
    EXPECT_EQ(TypeWord(engine, L"as", IM_TELEX), L"á");
}

TEST_F(TelexTest, ToneHuyen) {
    EXPECT_EQ(TypeWord(engine, L"af", IM_TELEX), L"à");
}

TEST_F(TelexTest, ToneHoi) {
    EXPECT_EQ(TypeWord(engine, L"ar", IM_TELEX), L"ả");
}

TEST_F(TelexTest, ToneNga) {
    EXPECT_EQ(TypeWord(engine, L"ax", IM_TELEX), L"ã");
}

TEST_F(TelexTest, ToneNang) {
    EXPECT_EQ(TypeWord(engine, L"aj", IM_TELEX), L"ạ");
}

TEST_F(TelexTest, ToneRemoval_Z) {
    // 'z' removes tone (index 0, no tone)
    EXPECT_EQ(TypeWord(engine, L"asz", IM_TELEX), L"a");
}

// --- Double-key Escape ---
TEST_F(TelexTest, DoubleKeyEscape_S) {
    // First 's' applies acute tone, second 's' reverts. Result: raw 'a' + 's' + 's'
    std::wstring result = TypeWord(engine, L"ass", IM_TELEX);
    // After escape, tone is removed and 's' is appended
    // The exact behavior: active_tone set to -1, result += 's', so composition = "as" + tone_removal + "s"
    // Just verify no tone is present
    EXPECT_NE(result.find(L'á'), 0u); // Should NOT start with á
}

TEST_F(TelexTest, DoubleKeyEscape_F) {
    std::wstring result = TypeWord(engine, L"aff", IM_TELEX);
    EXPECT_NE(result.find(L'à'), 0u); // Should NOT have huyền tone
}

// --- Complex Words ---
TEST_F(TelexTest, Word_Tieeng) {
    EXPECT_EQ(TypeWord(engine, L"tieengs", IM_TELEX), L"tiếng");
}

TEST_F(TelexTest, Word_Tuowng) {
    EXPECT_EQ(TypeWord(engine, L"tuowngf", IM_TELEX), L"tường");
}

TEST_F(TelexTest, Word_Nguowif) {
    EXPECT_EQ(TypeWord(engine, L"nguowif", IM_TELEX), L"người");
}

TEST_F(TelexTest, Word_Ddaats) {
    EXPECT_EQ(TypeWord(engine, L"ddaats", IM_TELEX), L"đất");
}

TEST_F(TelexTest, Word_Hoas) {
    // Modern: hoá (tone on 'a', not 'o')
    EXPECT_EQ(TypeWord(engine, L"hoas", IM_TELEX), L"hoá");
}

TEST_F(TelexTest, Word_Xueer) {
    EXPECT_EQ(TypeWord(engine, L"xueer", IM_TELEX), L"xuể");
}

// =============================================================================
// VNI Input
// =============================================================================
class VniTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetToneType(TONE_MODERN);
    }
};

TEST_F(VniTest, ToneSac) {
    EXPECT_EQ(TypeWord(engine, L"a1", IM_VNI), L"á");
}

TEST_F(VniTest, ToneHuyen) {
    EXPECT_EQ(TypeWord(engine, L"a2", IM_VNI), L"à");
}

TEST_F(VniTest, ToneHoi) {
    EXPECT_EQ(TypeWord(engine, L"a3", IM_VNI), L"ả");
}

TEST_F(VniTest, ToneNga) {
    EXPECT_EQ(TypeWord(engine, L"a4", IM_VNI), L"ã");
}

TEST_F(VniTest, ToneNang) {
    EXPECT_EQ(TypeWord(engine, L"a5", IM_VNI), L"ạ");
}

TEST_F(VniTest, ToneRemoval_0) {
    // VNI tone 0 should remove the tone
    EXPECT_EQ(TypeWord(engine, L"a10", IM_VNI), L"a");
}

TEST_F(VniTest, Circumflex_6) {
    EXPECT_EQ(TypeWord(engine, L"a6", IM_VNI), L"â");
}

TEST_F(VniTest, Horn_7) {
    EXPECT_EQ(TypeWord(engine, L"o7", IM_VNI), L"ơ");
}

TEST_F(VniTest, Breve_8) {
    EXPECT_EQ(TypeWord(engine, L"a8", IM_VNI), L"ă");
}

TEST_F(VniTest, StrokeD_9) {
    EXPECT_EQ(TypeWord(engine, L"d9", IM_VNI), L"đ");
}

TEST_F(VniTest, Word_Viet) {
    EXPECT_EQ(TypeWord(engine, L"Vie65t", IM_VNI), L"Việt");
}

TEST_F(VniTest, Word_Dat) {
    EXPECT_EQ(TypeWord(engine, L"d9a61t", IM_VNI), L"đất");
}

// =============================================================================
// VIQR Input
// =============================================================================
class ViqrTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetToneType(TONE_MODERN);
    }
};

TEST_F(ViqrTest, ToneSac) {
    EXPECT_EQ(TypeWord(engine, L"a'", IM_VIQR), L"á");
}

TEST_F(ViqrTest, ToneHuyen) {
    EXPECT_EQ(TypeWord(engine, L"a`", IM_VIQR), L"à");
}

TEST_F(ViqrTest, ToneHoi) {
    EXPECT_EQ(TypeWord(engine, L"a?", IM_VIQR), L"ả");
}

TEST_F(ViqrTest, ToneNga) {
    EXPECT_EQ(TypeWord(engine, L"a~", IM_VIQR), L"ã");
}

TEST_F(ViqrTest, ToneNang) {
    EXPECT_EQ(TypeWord(engine, L"a.", IM_VIQR), L"ạ");
}

TEST_F(ViqrTest, Circumflex) {
    EXPECT_EQ(TypeWord(engine, L"a^", IM_VIQR), L"â");
}

TEST_F(ViqrTest, Breve) {
    EXPECT_EQ(TypeWord(engine, L"a(", IM_VIQR), L"ă");
}

TEST_F(ViqrTest, Horn) {
    EXPECT_EQ(TypeWord(engine, L"o+", IM_VIQR), L"ơ");
}

TEST_F(ViqrTest, StrokeD) {
    EXPECT_EQ(TypeWord(engine, L"dd", IM_VIQR), L"đ");
}

TEST_F(ViqrTest, Word_Viet) {
    EXPECT_EQ(TypeWord(engine, L"Vie^.", IM_VIQR), L"Việ");
}

// =============================================================================
// Modern Tone Placement
// =============================================================================
class ModernTonePlacementTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetToneType(TONE_MODERN);
        engine.SetFreeToneMarking(false);
    }
};

TEST_F(ModernTonePlacementTest, OpenSyllable_Oa_ToneOnA) {
    // "hoá" — tone on 'a' (second vowel of oa diphthong)
    EXPECT_EQ(TypeWord(engine, L"hoas", IM_TELEX), L"hoá");
}

TEST_F(ModernTonePlacementTest, OpenSyllable_Uy_ToneOnY) {
    // "quý" — tone on 'y'
    EXPECT_EQ(TypeWord(engine, L"quys", IM_TELEX), L"quý");
}

TEST_F(ModernTonePlacementTest, ClosedSyllable_ToneOnLastVowel) {
    // "oán" — tone on 'a' (last vowel before consonant)
    EXPECT_EQ(TypeWord(engine, L"oans", IM_TELEX), L"oán");
}

TEST_F(ModernTonePlacementTest, ModifiedVowel_ToneOnModified) {
    // "ước" — tone on 'ư' (modified vowel)
    EXPECT_EQ(TypeWord(engine, L"uwowcs", IM_TELEX), L"ước");
}

TEST_F(ModernTonePlacementTest, SingleVowel_ToneOnIt) {
    EXPECT_EQ(TypeWord(engine, L"tas", IM_TELEX), L"tá");
}

// =============================================================================
// Classic Tone Placement
// =============================================================================
class ClassicTonePlacementTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetToneType(TONE_CLASSIC);
        engine.SetFreeToneMarking(false);
    }
};

TEST_F(ClassicTonePlacementTest, OA_ToneOnFirstVowel) {
    // Classic: "hóa" not "hoá" — tone on first vowel of cluster
    EXPECT_EQ(TypeWord(engine, L"hoas", IM_TELEX), L"hóa");
}

TEST_F(ClassicTonePlacementTest, SingleVowel) {
    EXPECT_EQ(TypeWord(engine, L"tas", IM_TELEX), L"tá");
}

// =============================================================================
// Free Tone Marking
// =============================================================================
class FreeToneTest : public ::testing::Test {
protected:
    VnEngine engine;
    void SetUp() override {
        engine.SetFreeToneMarking(true);
    }
};

TEST_F(FreeToneTest, ToneFollowsOrthography) {
    // Free mode should still respect proper tone placement rules
    std::wstring result = TypeWord(engine, L"rooif", IM_TELEX);
    // Should safely output "rồi" and not "rôì"
    EXPECT_EQ(result, L"r\x1ED3i"); // "rồi"
}

// =============================================================================
// Backspace and Word Boundary
// =============================================================================
class BackspaceTest : public ::testing::Test {
protected:
    VnEngine engine;
};

TEST_F(BackspaceTest, RemoveLastChar_Basic) {
    engine.Clear();
    engine.ProcessKey(L'a', IM_TELEX);
    engine.ProcessKey(L'n', IM_TELEX);
    engine.RemoveLastChar();
    EXPECT_EQ(engine.GetCompositionString(), L"a");
}

TEST_F(BackspaceTest, RemoveLastChar_AfterModifier) {
    engine.Clear();
    engine.ProcessKey(L'a', IM_TELEX);
    engine.ProcessKey(L'a', IM_TELEX);  // â
    engine.RemoveLastChar();
    EXPECT_EQ(engine.GetCompositionString(), L"a");
}

TEST_F(BackspaceTest, RemoveAll) {
    engine.Clear();
    engine.ProcessKey(L'a', IM_TELEX);
    engine.RemoveLastChar();
    EXPECT_EQ(engine.GetCompositionString(), L"");
    EXPECT_FALSE(engine.IsInWord());
}

TEST_F(BackspaceTest, WordTermination_Space) {
    engine.Clear();
    engine.ProcessKey(L'a', IM_TELEX);
    bool eaten = engine.ProcessKey(L' ', IM_TELEX);
    EXPECT_FALSE(eaten);
    EXPECT_FALSE(engine.IsInWord());
}

TEST_F(BackspaceTest, WordTermination_Punctuation) {
    engine.Clear();
    engine.ProcessKey(L'a', IM_TELEX);
    bool eaten = engine.ProcessKey(L',', IM_TELEX);
    EXPECT_FALSE(eaten);
    EXPECT_FALSE(engine.IsInWord());
}

// =============================================================================
// Mixed Case
// =============================================================================
TEST(MixedCaseTest, Uppercase_Telex) {
    VnEngine engine;
    // Test uppercase composition — may have font-dependent rendering
    std::wstring result = TypeWord(engine, L"VIEEJT", IM_TELEX);
    // Should contain an uppercase circumflex E and nặng tone
    EXPECT_FALSE(result.empty());
    EXPECT_EQ(result.length(), 4u); // V-I-Ệ-T
}

TEST(MixedCaseTest, TitleCase_Telex) {
    VnEngine engine;
    EXPECT_EQ(TypeWord(engine, L"Vieejt", IM_TELEX), L"Việt");
}

// =============================================================================
// Edge Cases
// =============================================================================
TEST(EdgeCaseTest, EmptyInput) {
    VnEngine engine;
    EXPECT_EQ(TypeWord(engine, L"", IM_TELEX), L"");
}

TEST(EdgeCaseTest, PureConsonants) {
    VnEngine engine;
    std::wstring result = TypeWord(engine, L"bcd", IM_TELEX);
    EXPECT_EQ(result, L"bcd");
}

TEST(EdgeCaseTest, DidTransform_True) {
    VnEngine engine;
    engine.Clear();
    engine.ProcessKey(L'a', IM_TELEX);
    engine.ProcessKey(L's', IM_TELEX); // á
    EXPECT_TRUE(engine.DidTransform());
}

TEST(EdgeCaseTest, DidTransform_False) {
    VnEngine engine;
    engine.Clear();
    engine.ProcessKey(L'b', IM_TELEX);
    EXPECT_FALSE(engine.DidTransform());
}

TEST(EdgeCaseTest, IsWordChar_VNI_DigitsInWord) {
    VnEngine engine;
    engine.Clear();
    engine.ProcessKey(L'a', IM_VNI);
    bool eaten = engine.ProcessKey(L'1', IM_VNI);
    EXPECT_TRUE(eaten);
}

TEST(EdgeCaseTest, IsWordChar_VIQR_Punctuation) {
    VnEngine engine;
    engine.Clear();
    engine.ProcessKey(L'a', IM_VIQR);
    bool eaten = engine.ProcessKey(L'^', IM_VIQR);
    EXPECT_TRUE(eaten);
}

// =============================================================================
// Macro Engine Tests
// =============================================================================
TEST(MacroTest, EmptyEngine) {
    MacroEngine macros;
    EXPECT_EQ(macros.Expand(L"hello"), L"");
}

// =============================================================================
// Main
// =============================================================================
// GoogleTest main is provided by GTest::gtest_main

// =============================================================================
// Spell Check & Typo Restoration Tests
// =============================================================================
TEST_F(TelexTest, SpellCheck_Off_AllowsGibberish) {
    engine.SetSpellCheck(false);
    TypeWord(engine, L"windows", IM_TELEX);
    // "w" applies to "o" to make "ơ". "s" is sắc tone.
    EXPECT_NE(engine.GetCompositionString(), L"windows");
}

TEST_F(TelexTest, SpellCheck_On_RestoresRawForGibberish) {
    engine.SetSpellCheck(true);
    TypeWord(engine, L"windows", IM_TELEX);
    // Because "windớs" is an invalid Vietnamese syllable, spell check rejects it.
    // The engine automatically falls back to raw keystrokes -> "windows".
    EXPECT_EQ(engine.GetCompositionString(), L"windows");
}

TEST_F(TelexTest, SpellCheck_On_AllowsValidVietnamese) {
    engine.SetSpellCheck(true);
    
    TypeWord(engine, L"nguyeexn", IM_TELEX);
    EXPECT_EQ(engine.GetCompositionString(), L"nguy\x1ec5n"); // nguyễn
    
    engine.Clear();
    TypeWord(engine, L"vieejt", IM_TELEX);
    EXPECT_EQ(engine.GetCompositionString(), L"vi\x1ec7t"); // việt
}

TEST_F(TelexTest, SpellCheck_RejectInvalidConsonantAppend) {
    engine.SetSpellCheck(true);
    
    TypeWord(engine, L"ngoos", IM_TELEX);
    EXPECT_EQ(engine.GetCompositionString(), L"ng\x1ed1"); 

    // Now append 'q' which is a valid word char but makes the syllable invalid "ngốq"
    engine.ProcessKey(L'q', IM_TELEX);
    // Since 'ngốq' is invalid, the engine should revert to raw "ngoosq"
    EXPECT_EQ(engine.GetCompositionString(), L"ngoosq"); 
}

TEST_F(VniTest, SpellCheck_On_Uppercase_VNI) {
    engine.SetSpellCheck(true);
    // User types 'D' '9' 'u' 'c' '7' '1' -> 'Đức'
    EXPECT_EQ(TypeWord(engine, L"D9uc71", IM_VNI), L"Đ\x1ee9" L"c"); // Đức
    // Also test raw 'Đ' if pasted or native keyboard
    EXPECT_EQ(TypeWord(engine, L"Đuc71", IM_VNI), L"Đ\x1ee9" L"c"); // Đức
}

// =============================================================================
// IsPotentialModifier Tests
// =============================================================================
TEST(PotentialModifierTest, VNI_Digits) {
    VnEngine engine;
    for (wchar_t d = L'0'; d <= L'9'; d++) {
        EXPECT_TRUE(engine.IsPotentialModifier(d, IM_VNI))
            << "VNI digit " << (char)d << " should be a potential modifier";
    }
}

TEST(PotentialModifierTest, Telex_ToneKeys) {
    VnEngine engine;
    EXPECT_TRUE(engine.IsPotentialModifier(L's', IM_TELEX));
    EXPECT_TRUE(engine.IsPotentialModifier(L'f', IM_TELEX));
    EXPECT_TRUE(engine.IsPotentialModifier(L'r', IM_TELEX));
    EXPECT_TRUE(engine.IsPotentialModifier(L'x', IM_TELEX));
    EXPECT_TRUE(engine.IsPotentialModifier(L'j', IM_TELEX));
    EXPECT_TRUE(engine.IsPotentialModifier(L'z', IM_TELEX));
    EXPECT_TRUE(engine.IsPotentialModifier(L'w', IM_TELEX));
}

TEST(PotentialModifierTest, VIQR_Marks) {
    VnEngine engine;
    EXPECT_TRUE(engine.IsPotentialModifier(L'\'', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'`', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'?', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'~', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'.', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'^', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'(', IM_VIQR));
    EXPECT_TRUE(engine.IsPotentialModifier(L'+', IM_VIQR));
}

TEST(PotentialModifierTest, Alpha_ReturnsFalse) {
    VnEngine engine;
    // Regular alpha chars should NOT be potential modifiers in VNI
    EXPECT_FALSE(engine.IsPotentialModifier(L'a', IM_VNI));
    EXPECT_FALSE(engine.IsPotentialModifier(L'b', IM_VNI));
    // Regular alpha non-modifier chars should NOT be potential modifiers in Telex
    EXPECT_FALSE(engine.IsPotentialModifier(L'a', IM_TELEX));
    EXPECT_FALSE(engine.IsPotentialModifier(L'b', IM_TELEX));
    EXPECT_FALSE(engine.IsPotentialModifier(L'n', IM_TELEX));
}
