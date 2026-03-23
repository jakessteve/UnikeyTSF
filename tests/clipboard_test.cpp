// =============================================================================
// UniKey TSF Reborn — Clipboard & Charset Conversion Tests
// =============================================================================
// Tests for:
//   - VNI Windows charset mapping (Unicode <-> VNI bidirectional)
//   - Accent stripping
//   - Edge cases (empty strings, pure ASCII, mixed content)
// =============================================================================

#include <gtest/gtest.h>
#include <set>
#include "toolkit/charset_convert.h"

using namespace Toolkit;

// =============================================================================
// VNI Mapping Table Completeness
// =============================================================================
class VniMappingTest : public ::testing::Test {};

TEST_F(VniMappingTest, LowercaseTableHas68Entries) {
    EXPECT_EQ(g_NumVniLower, 67u);
}

TEST_F(VniMappingTest, UppercaseTableSize) {
    EXPECT_EQ(g_NumVniUpper, 67u);
}

TEST_F(VniMappingTest, AllLowercaseEntriesHaveNonZeroMark) {
    for (size_t i = 0; i < g_NumVniLower; i++) {
        EXPECT_NE(g_UnicodeVniLower[i].vniMark, 0)
            << "Entry " << i << " (U+" << std::hex << (int)g_UnicodeVniLower[i].unicode
            << ") has zero mark byte";
    }
}

TEST_F(VniMappingTest, AllUppercaseEntriesHaveNonZeroMark) {
    for (size_t i = 0; i < g_NumVniUpper; i++) {
        EXPECT_NE(g_UnicodeVniUpper[i].vniMark, 0)
            << "Entry " << i << " (U+" << std::hex << (int)g_UnicodeVniUpper[i].unicode
            << ") has zero mark byte";
    }
}

TEST_F(VniMappingTest, NoDuplicateUnicodeKeysInLowercase) {
    std::set<wchar_t> seen;
    for (size_t i = 0; i < g_NumVniLower; i++) {
        auto result = seen.insert(g_UnicodeVniLower[i].unicode);
        EXPECT_TRUE(result.second)
            << "Duplicate Unicode key U+" << std::hex << (int)g_UnicodeVniLower[i].unicode
            << " at index " << i;
    }
}

TEST_F(VniMappingTest, NoDuplicateUnicodeKeysInUppercase) {
    std::set<wchar_t> seen;
    for (size_t i = 0; i < g_NumVniUpper; i++) {
        auto result = seen.insert(g_UnicodeVniUpper[i].unicode);
        EXPECT_TRUE(result.second)
            << "Duplicate Unicode key U+" << std::hex << (int)g_UnicodeVniUpper[i].unicode
            << " at index " << i;
    }
}

TEST_F(VniMappingTest, NoDuplicateVniPairsInLowercase) {
    std::set<uint32_t> seen;
    for (size_t i = 0; i < g_NumVniLower; i++) {
        uint32_t key = ((uint32_t)g_UnicodeVniLower[i].vniBase << 16) | (uint32_t)g_UnicodeVniLower[i].vniMark;
        auto result = seen.insert(key);
        EXPECT_TRUE(result.second)
            << "Duplicate VNI pair ("
            << (char)g_UnicodeVniLower[i].vniBase << ", 0x" << std::hex << (int)g_UnicodeVniLower[i].vniMark
            << ") at index " << i;
    }
}

TEST_F(VniMappingTest, NoDuplicateVniPairsInUppercase) {
    std::set<uint32_t> seen;
    for (size_t i = 0; i < g_NumVniUpper; i++) {
        uint32_t key = ((uint32_t)g_UnicodeVniUpper[i].vniBase << 16) | (uint32_t)g_UnicodeVniUpper[i].vniMark;
        auto result = seen.insert(key);
        EXPECT_TRUE(result.second)
            << "Duplicate VNI pair ("
            << (char)g_UnicodeVniUpper[i].vniBase << ", 0x" << std::hex << (int)g_UnicodeVniUpper[i].vniMark
            << ") at index " << i;
    }
}

TEST_F(VniMappingTest, LowercaseBasesAreAllLowercase) {
    for (size_t i = 0; i < g_NumVniLower; i++) {
        wchar_t base = g_UnicodeVniLower[i].vniBase;
        EXPECT_TRUE(base >= L'a' && base <= L'z')
            << "Entry " << i << " has non-lowercase base '" << (char)base << "'";
    }
}

TEST_F(VniMappingTest, UppercaseBasesAreAllUppercase) {
    for (size_t i = 0; i < g_NumVniUpper; i++) {
        wchar_t base = g_UnicodeVniUpper[i].vniBase;
        EXPECT_TRUE(base >= L'A' && base <= L'Z')
            << "Entry " << i << " has non-uppercase base '" << (char)base << "'";
    }
}

// =============================================================================
// Unicode <-> VNI Conversion
// =============================================================================
class VniConversionTest : public ::testing::Test {};

TEST_F(VniConversionTest, SingleCharForwardReverse_Lowercase) {
    std::wstring input = L"à";
    std::wstring vni = UnicodeToVni(input);
    EXPECT_EQ(vni.size(), 2u);
    EXPECT_EQ(vni[0], L'a');
    EXPECT_EQ(vni[1], (wchar_t)0xF8);
    EXPECT_EQ(VniToUnicode(vni), input);
}

TEST_F(VniConversionTest, SingleCharForwardReverse_Uppercase) {
    std::wstring input = L"Ấ";
    std::wstring vni = UnicodeToVni(input);
    EXPECT_EQ(vni.size(), 2u);
    EXPECT_EQ(vni[0], L'A');
    EXPECT_EQ(vni[1], (wchar_t)0xDE);
    EXPECT_EQ(VniToUnicode(vni), input);
}

TEST_F(VniConversionTest, DStroke_Lowercase) {
    std::wstring input = L"đ";
    std::wstring vni = UnicodeToVni(input);
    EXPECT_EQ(vni.size(), 2u);
    EXPECT_EQ(vni[0], L'd');
    EXPECT_EQ(vni[1], (wchar_t)0xF0);
    EXPECT_EQ(VniToUnicode(vni), input);
}

TEST_F(VniConversionTest, DStroke_Uppercase) {
    std::wstring input = L"Đ";
    std::wstring vni = UnicodeToVni(input);
    EXPECT_EQ(vni.size(), 2u);
    EXPECT_EQ(vni[0], L'D');
    EXPECT_EQ(vni[1], (wchar_t)0xD0);
    EXPECT_EQ(VniToUnicode(vni), input);
}

TEST_F(VniConversionTest, WordRoundTrip_Nguyen) {
    std::wstring input = L"Nguyễn";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, WordRoundTrip_VietNam) {
    std::wstring input = L"Việt Nam";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, WordRoundTrip_XinChao) {
    std::wstring input = L"Xin chào";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, WordRoundTrip_AllTones) {
    std::wstring input = L"áàảãạ";
    std::wstring vni = UnicodeToVni(input);
    EXPECT_EQ(vni.size(), 10u);
    EXPECT_EQ(VniToUnicode(vni), input);
}

TEST_F(VniConversionTest, WordRoundTrip_HornChars) {
    std::wstring input = L"ơớờởỡợưứừửữự";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, WordRoundTrip_BreveChars) {
    std::wstring input = L"ăắằẳẵặ";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, WordRoundTrip_CircumflexChars) {
    std::wstring input = L"âấầẩẫậêếềểễệôốồổỗộ";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, EmptyString) {
    EXPECT_EQ(UnicodeToVni(L""), L"");
    EXPECT_EQ(VniToUnicode(L""), L"");
}

TEST_F(VniConversionTest, PureAscii_PassThrough) {
    std::wstring ascii = L"Hello World 123!";
    EXPECT_EQ(UnicodeToVni(ascii), ascii);
    EXPECT_EQ(VniToUnicode(ascii), ascii);
}

TEST_F(VniConversionTest, MixedContent) {
    std::wstring input = L"Hello Việt Nam 2026!";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(VniConversionTest, AllLowercaseRoundTrip) {
    for (size_t i = 0; i < g_NumVniLower; i++) {
        std::wstring input(1, g_UnicodeVniLower[i].unicode);
        EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input)
            << "Round-trip fail for lowercase entry " << i
            << " U+" << std::hex << (int)g_UnicodeVniLower[i].unicode;
    }
}

TEST_F(VniConversionTest, AllUppercaseRoundTrip) {
    for (size_t i = 0; i < g_NumVniUpper; i++) {
        std::wstring input(1, g_UnicodeVniUpper[i].unicode);
        EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input)
            << "Round-trip fail for uppercase entry " << i
            << " U+" << std::hex << (int)g_UnicodeVniUpper[i].unicode;
    }
}

// --- VNI spec spot checks ---
TEST_F(VniConversionTest, SpecCheck_Acute) {
    auto v = UnicodeToVni(L"á"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xF9);
}
TEST_F(VniConversionTest, SpecCheck_Grave) {
    auto v = UnicodeToVni(L"à"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xF8);
}
TEST_F(VniConversionTest, SpecCheck_Circumflex) {
    auto v = UnicodeToVni(L"â"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xE2);
}
TEST_F(VniConversionTest, SpecCheck_Tilde) {
    auto v = UnicodeToVni(L"ã"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xF5);
}
TEST_F(VniConversionTest, SpecCheck_DotBelow) {
    auto v = UnicodeToVni(L"ạ"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xFA);
}
TEST_F(VniConversionTest, SpecCheck_Horn) {
    auto v = UnicodeToVni(L"ơ"); EXPECT_EQ(v[0], L'o'); EXPECT_EQ(v[1], (wchar_t)0xE1);
    v = UnicodeToVni(L"ư"); EXPECT_EQ(v[0], L'u'); EXPECT_EQ(v[1], (wchar_t)0xE1);
}
TEST_F(VniConversionTest, SpecCheck_UpperHorn) {
    auto v = UnicodeToVni(L"Ơ"); EXPECT_EQ(v[0], L'O'); EXPECT_EQ(v[1], (wchar_t)0xC1);
    v = UnicodeToVni(L"Ư"); EXPECT_EQ(v[0], L'U'); EXPECT_EQ(v[1], (wchar_t)0xC1);
}
TEST_F(VniConversionTest, SpecCheck_CircumflexAcute) {
    auto v = UnicodeToVni(L"ấ"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xFE);
}
TEST_F(VniConversionTest, SpecCheck_CircumflexDotBelow) {
    auto v = UnicodeToVni(L"ậ"); EXPECT_EQ(v[0], L'a'); EXPECT_EQ(v[1], (wchar_t)0xFF);
}

// =============================================================================
// Accent Stripping
// =============================================================================
class AccentStripTest : public ::testing::Test {};

TEST_F(AccentStripTest, BasicStrip) {
    EXPECT_EQ(StripAccentsString(L"Việt Nam"), L"Viet Nam");
}
TEST_F(AccentStripTest, AllTones_a) {
    EXPECT_EQ(StripAccentsString(L"àáảãạ"), L"aaaaa");
}
TEST_F(AccentStripTest, AllModifiedVowels) {
    EXPECT_EQ(StripAccentsString(L"ăâêôơư"), L"aaeoou");
}
TEST_F(AccentStripTest, DStroke) {
    EXPECT_EQ(StripAccentsString(L"đ"), L"d");
    EXPECT_EQ(StripAccentsString(L"Đ"), L"D");
}
TEST_F(AccentStripTest, FullSentence) {
    EXPECT_EQ(StripAccentsString(L"Xin chào Việt Nam!"), L"Xin chao Viet Nam!");
}
TEST_F(AccentStripTest, PureAscii) {
    EXPECT_EQ(StripAccentsString(L"Hello World"), L"Hello World");
}
TEST_F(AccentStripTest, EmptyString) {
    EXPECT_EQ(StripAccentsString(L""), L"");
}
TEST_F(AccentStripTest, MixedCase) {
    EXPECT_EQ(StripAccentsString(L"Nguyễn Văn Ấn"), L"Nguyen Van An");
}

// =============================================================================
// Sentence-Level Integration Tests
// =============================================================================
class IntegrationTest : public ::testing::Test {};

TEST_F(IntegrationTest, LongSentenceRoundTrip_VNI) {
    std::wstring input = L"Hà Nội là thủ đô của nước Cộng hòa Xã hội Chủ nghĩa Việt Nam";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(IntegrationTest, LongSentenceStripAccents) {
    std::wstring input = L"Hà Nội là thủ đô của nước Cộng hòa Xã hội Chủ nghĩa Việt Nam";
    EXPECT_EQ(StripAccentsString(input), L"Ha Noi la thu do cua nuoc Cong hoa Xa hoi Chu nghia Viet Nam");
}

TEST_F(IntegrationTest, NumbersAndPunctuation_Preserved) {
    std::wstring input = L"Năm 2026, Việt Nam có 100 triệu dân!";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}

TEST_F(IntegrationTest, YWithTones_AllFive) {
    std::wstring input = L"ỳýỷỹỵ";
    auto vni = UnicodeToVni(input);
    EXPECT_EQ(vni.size(), 10u);
    EXPECT_EQ(VniToUnicode(vni), input);
}

TEST_F(IntegrationTest, UppercaseWord_HANOI) {
    std::wstring input = L"HÀ NỘI";
    EXPECT_EQ(VniToUnicode(UnicodeToVni(input)), input);
}
