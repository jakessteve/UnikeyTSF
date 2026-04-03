#include <gtest/gtest.h>
#include <windows.h>
#include <thread>
#include <chrono>

#include "ui/settings_dialog.h"
#include "shared_config.h"

namespace {

constexpr const wchar_t* kSettingsClass = L"UniKeyTSF_SettingsWnd";
constexpr const wchar_t* kSettingsTitle = L"UniKey TSF Reborn";

HWND WaitForDialog(int timeout_ms = 5000)
{
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - start).count() < timeout_ms) {
        HWND hwnd = FindWindowW(kSettingsClass, kSettingsTitle);
        if (hwnd) {
            return hwnd;
        }

        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }
    return nullptr;
}

void PumpMessages(int duration_ms = 200)
{
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(
               std::chrono::steady_clock::now() - start).count() < duration_ms) {
        MSG msg;
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

} // namespace

class SettingsUITest : public ::testing::Test {
protected:
    HWND hDlg = nullptr;
    UniKeyConfig config = {};

    void SetUp() override
    {
        config.version = UNIKEY_CONFIG_VERSION;
        config.inputEnabled = 1;
        config.inputMethod = IM_TELEX;
        config.charset = CS_UNICODE;
        config.toneType = TONE_MODERN;
        config.toggleKey = TK_CTRL_SHIFT;
        config.freeToneMarking = 1;
    }

    void TearDown() override
    {
        if (hDlg && IsWindow(hDlg)) {
            DestroyWindow(hDlg);
            PumpMessages();
        }
        hDlg = nullptr;
    }

    HWND ShowSettingsWindow()
    {
        ShowSettingsDialog(nullptr, GetModuleHandleW(nullptr), &config);
        PumpMessages();
        hDlg = WaitForDialog();
        return hDlg;
    }
};

TEST_F(SettingsUITest, OpensSettingsWindow)
{
    ASSERT_NE(ShowSettingsWindow(), nullptr);
    EXPECT_TRUE(IsWindowVisible(hDlg));
}

TEST_F(SettingsUITest, CloseHidesSettingsWindow)
{
    ASSERT_NE(ShowSettingsWindow(), nullptr);
    SendMessageW(hDlg, WM_CLOSE, 0, 0);
    PumpMessages();

    EXPECT_TRUE(IsWindow(hDlg));
    EXPECT_FALSE(IsWindowVisible(hDlg));
}

TEST_F(SettingsUITest, ReopenReusesSameWindow)
{
    ASSERT_NE(ShowSettingsWindow(), nullptr);
    HWND first = hDlg;

    SendMessageW(first, WM_CLOSE, 0, 0);
    PumpMessages();

    HWND reopened = ShowSettingsWindow();
    ASSERT_NE(reopened, nullptr);
    EXPECT_EQ(reopened, first);
    EXPECT_TRUE(IsWindowVisible(reopened));
}
