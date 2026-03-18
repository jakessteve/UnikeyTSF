
// =============================================================================
// UniKey TSF Reborn — Settings UI Acceptance Tests
// =============================================================================
// Comprehensive E2E tests for the Settings UI covering:
//   - Initial state validation based on config
//   - Input method and charset selection
//   - Toggle key and tone type radio buttons
//   - Checkbox options (spell check, free tone, macro, auto-start)
//   - Blacklist management (add/remove)
//   - Dialog closing and config saving
// =============================================================================

#include <gtest/gtest.h>
#include <windows.h>
#include <commctrl.h>
#include <thread>
#include <chrono>
#include <vector>
#include <string>

#include "ui/settings_dialog.h"
#include "shared_config.h"
#include "resource.h"

// Define a helper to wait for the dialog to appear
HWND WaitForDialog(const wchar_t* title, int timeout_ms = 5000) {
    auto start = std::chrono::steady_clock::now();
    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < timeout_ms) {
        HWND hwnd = FindWindowW(NULL, title);
        if (hwnd) return hwnd;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return NULL;
}

class SettingsUITest : public ::testing::Test {
protected:
    HWND hDlg = NULL;
    UniKeyConfig config;
    std::thread uiThread;
    bool dialogRunning = false;

    void SetUp() override {
        // Initialize default config
        memset(&config, 0, sizeof(config));
        config.version = UNIKEY_CONFIG_VERSION;
        config.inputEnabled = 1;
        config.inputMethod = IM_TELEX;
        config.charset = CS_UNICODE;
        config.toneType = TONE_MODERN;
        config.toggleKey = TK_CTRL_SHIFT;
        config.spellCheck = 0;
        config.macroEnabled = 0;
        config.freeToneMarking = 1;

        // Launch dialog in a separate thread
        dialogRunning = true;
        uiThread = std::thread([this]() {
            HINSTANCE hInstance = GetModuleHandle(NULL);
            ShowSettingsDialog(NULL, hInstance, &this->config);
            this->dialogRunning = false;
        });

        // Wait for the window to appear
        hDlg = WaitForDialog(L"UniKey TSF Reborn");
        ASSERT_NE(hDlg, nullptr) << "Failed to find Settings Dialog";
    }

    void TearDown() override {
        if (hDlg && IsWindow(hDlg)) {
            // Close the dialog using IDOK (Đóng) to trigger save logic
            SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), (LPARAM)GetDlgItem(hDlg, IDOK));
        }
        
        if (uiThread.joinable()) {
            uiThread.join();
        }
    }

    // Helper to simulate a button click
    void ClickButton(int id) {
        HWND hwnd = GetDlgItem(hDlg, id);
        ASSERT_NE(hwnd, nullptr) << "Button ID not found: " << id;
        SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(id, BN_CLICKED), (LPARAM)hwnd);
    }

    // Helper to toggle a checkbox
    void ToggleCheckbox(int id, bool check) {
        HWND hwnd = GetDlgItem(hDlg, id);
        ASSERT_NE(hwnd, nullptr) << "Checkbox ID not found: " << id;
        SendMessage(hwnd, BM_SETCHECK, check ? BST_CHECKED : BST_UNCHECKED, 0);
        // Notify the dialog procedure
        SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(id, BN_CLICKED), (LPARAM)hwnd);
    }

    // Helper to check if a checkbox/radio is checked
    bool IsChecked(int id) {
        HWND hwnd = GetDlgItem(hDlg, id);
        if (!hwnd) return false;
        return SendMessage(hwnd, BM_GETCHECK, 0, 0) == BST_CHECKED;
    }

    // Helper to select an item in a combo box
    void SelectComboItem(int id, int index) {
        HWND hwnd = GetDlgItem(hDlg, id);
        ASSERT_NE(hwnd, nullptr) << "Combobox ID not found: " << id;
        SendMessage(hwnd, CB_SETCURSEL, index, 0);
        SendMessage(hDlg, WM_COMMAND, MAKEWPARAM(id, CBN_SELCHANGE), (LPARAM)hwnd);
    }

    // Helper to get combo box selection
    int GetComboSelection(int id) {
        HWND hwnd = GetDlgItem(hDlg, id);
        if (!hwnd) return -1;
        return (int)SendMessage(hwnd, CB_GETCURSEL, 0, 0);
    }
};

TEST_F(SettingsUITest, VerifyInitialStateFromConfig) {
    EXPECT_TRUE(IsChecked(IDC_CHK_VIETNAMESE));
    EXPECT_EQ(GetComboSelection(IDC_CBO_INPUT_METHOD), IM_TELEX);
    EXPECT_EQ(GetComboSelection(IDC_CBO_CHARSET), CS_UNICODE);
    EXPECT_TRUE(IsChecked(IDC_RAD_CTRL_SHIFT));
    EXPECT_FALSE(IsChecked(IDC_RAD_ALT_Z));
    EXPECT_TRUE(IsChecked(IDC_RAD_MODERN_TONE));
    EXPECT_FALSE(IsChecked(IDC_RAD_CLASSIC_TONE));
    EXPECT_FALSE(IsChecked(IDC_CHK_SPELL_CHECK));
    EXPECT_FALSE(IsChecked(IDC_CHK_MACRO));
    EXPECT_TRUE(IsChecked(IDC_CHK_FREE_TONE));
}

TEST_F(SettingsUITest, ChangeSettingsAndSave) {
    // Modify settings in UI
    ToggleCheckbox(IDC_CHK_VIETNAMESE, false);
    SelectComboItem(IDC_CBO_INPUT_METHOD, IM_VNI);
    SelectComboItem(IDC_CBO_CHARSET, CS_TCVN3);
    ToggleCheckbox(IDC_RAD_ALT_Z, true);
    ToggleCheckbox(IDC_RAD_CLASSIC_TONE, true);
    ToggleCheckbox(IDC_CHK_SPELL_CHECK, true);
    ToggleCheckbox(IDC_CHK_MACRO, true);
    ToggleCheckbox(IDC_CHK_FREE_TONE, false);

    // Verify UI state after modification
    EXPECT_FALSE(IsChecked(IDC_CHK_VIETNAMESE));
    EXPECT_EQ(GetComboSelection(IDC_CBO_INPUT_METHOD), IM_VNI);
    EXPECT_EQ(GetComboSelection(IDC_CBO_CHARSET), CS_TCVN3);
    EXPECT_TRUE(IsChecked(IDC_RAD_ALT_Z));
    EXPECT_TRUE(IsChecked(IDC_RAD_CLASSIC_TONE));
    EXPECT_TRUE(IsChecked(IDC_CHK_SPELL_CHECK));
    EXPECT_TRUE(IsChecked(IDC_CHK_MACRO));
    EXPECT_FALSE(IsChecked(IDC_CHK_FREE_TONE));

    // Close and save
    ClickButton(IDOK);
    
    // Wait for UI thread to finish (saving happens before EndDialog)
    if (uiThread.joinable()) {
        uiThread.join();
    }
    hDlg = NULL; // Prevent TearDown from trying to close it again

    // Verify config struct was updated
    EXPECT_EQ(config.inputEnabled, 0);
    EXPECT_EQ(config.inputMethod, IM_VNI);
    EXPECT_EQ(config.charset, CS_TCVN3);
    EXPECT_EQ(config.toggleKey, TK_ALT_Z);
    EXPECT_EQ(config.toneType, TONE_CLASSIC);
    EXPECT_EQ(config.spellCheck, 1);
    EXPECT_EQ(config.macroEnabled, 1);
    EXPECT_EQ(config.freeToneMarking, 0);
}

TEST_F(SettingsUITest, MacroUIEnableDisable) {
    // Initially macro is disabled (from SetUp)
    HWND hEdit = GetDlgItem(hDlg, IDC_TXT_MACRO_FILE);
    HWND hBtn = GetDlgItem(hDlg, IDC_BTN_MACRO_FILE);
    EXPECT_FALSE(IsWindowEnabled(hEdit));
    EXPECT_FALSE(IsWindowEnabled(hBtn));

    // Enable macro
    ToggleCheckbox(IDC_CHK_MACRO, true);
    EXPECT_TRUE(IsWindowEnabled(hEdit));
    EXPECT_TRUE(IsWindowEnabled(hBtn));

    // Disable macro again
    ToggleCheckbox(IDC_CHK_MACRO, false);
    EXPECT_FALSE(IsWindowEnabled(hEdit));
    EXPECT_FALSE(IsWindowEnabled(hBtn));
}

TEST_F(SettingsUITest, BlacklistAddRemove) {
    // Add a process to blacklist
    HWND hEdit = GetDlgItem(hDlg, IDC_TXT_BLACKLIST_ADD);
    ASSERT_NE(hEdit, nullptr);
    SetWindowTextW(hEdit, L"test_process.exe");
    
    ClickButton(IDC_BTN_BLACKLIST_ADD);

    // Verify it's in the list
    HWND hList = GetDlgItem(hDlg, IDC_LST_BLACKLIST);
    ASSERT_NE(hList, nullptr);
    int count = (int)SendMessage(hList, LB_GETCOUNT, 0, 0);
    bool found = false;
    for (int i = 0; i < count; i++) {
        wchar_t buf[MAX_PATH];
        SendMessage(hList, LB_GETTEXT, i, (LPARAM)buf);
        if (wcscmp(buf, L"test_process.exe") == 0) {
            found = true;
            // Select it for deletion test
            SendMessage(hList, LB_SETCURSEL, i, 0);
            break;
        }
    }
    EXPECT_TRUE(found);

    // Remove it
    ClickButton(IDC_BTN_BLACKLIST_DEL);
    
    // Verify it's gone
    count = (int)SendMessage(hList, LB_GETCOUNT, 0, 0);
    found = false;
    for (int i = 0; i < count; i++) {
        wchar_t buf[MAX_PATH];
        SendMessage(hList, LB_GETTEXT, i, (LPARAM)buf);
        if (wcscmp(buf, L"test_process.exe") == 0) {
            found = true;
            break;
        }
    }
    EXPECT_FALSE(found);
}
