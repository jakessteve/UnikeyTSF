#include <gtest/gtest.h>
#include <windows.h>
#include <thread>
#include <chrono>

#include "config/ipc_manager.h"
#include "engine/per_app_input_state.h"
#include "shared_config.h"

class IPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure clean state
        CleanupSharedMemory();
        ResetPerAppInputStateStoreForTests();
    }

    void TearDown() override {
        ShutdownPerAppInputStateStore();
        CleanupSharedMemory();
    }
};

TEST_F(IPCTest, InitSharedMemory_CreatesHandles) {
    ASSERT_TRUE(InitSharedMemory());
    ASSERT_NE(g_pConfig, nullptr);
    
    // Check if mapping and mutex handles actually got created
    HANDLE hMutex = OpenMutexW(MUTEX_ALL_ACCESS, FALSE, UNIKEY_SHARED_MUTEX_NAME);
    EXPECT_NE(hMutex, nullptr);
    if (hMutex) {
        CloseHandle(hMutex);
    }

    HANDLE hMap = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    EXPECT_NE(hMap, nullptr);
    if (hMap) {
        CloseHandle(hMap);
    }
}

TEST_F(IPCTest, LockConfig_TimeoutMitigation) {
    ASSERT_TRUE(InitSharedMemory());

    // Create a secondary thread that holds the lock indefinitely
    bool threadStarted = false;
    std::thread locker([&threadStarted]() {
        LockConfig();
        threadStarted = true;
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        UnlockConfig();
    });

    // Wait for thread to acquire lock
    while (!threadStarted) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    auto start = std::chrono::steady_clock::now();
    
    // This should timeout after ~500ms
    LockConfig();
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // The timeout is 500ms, allow a buffer
    EXPECT_GE(duration, 400);
    EXPECT_LT(duration, 1500);

    UnlockConfig(); // Try to unlock, shouldn't crash
    locker.join();
}

TEST_F(IPCTest, DataIntegrity_AcrossUpdates) {
    ASSERT_TRUE(InitSharedMemory());

    // Write a change
    LockConfig();
    g_pConfig->inputMethod = IM_TELEX;
    UnlockConfig();

    // Emulate another process accessing it
    HANDLE hMap = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    ASSERT_NE(hMap, nullptr);

    auto* pReadConfig = static_cast<const UniKeyConfig*>(
        MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, sizeof(UniKeyConfig))
    );
    ASSERT_NE(pReadConfig, nullptr);

    EXPECT_EQ(pReadConfig->inputMethod, IM_TELEX);

    UnmapViewOfFile(pReadConfig);
    CloseHandle(hMap);
}

TEST_F(IPCTest, DataIntegrity_TypingRelevantLiveFields) {
    ASSERT_TRUE(InitSharedMemory());

    ASSERT_TRUE(LockConfig());
    g_pConfig->inputEnabled = 0;
    g_pConfig->inputMethod = IM_TELEX;
    g_pConfig->toneType = TONE_CLASSIC;
    g_pConfig->spellCheck = 0;
    g_pConfig->freeToneMarking = 0;
    g_pConfig->macroEnabled = 1;
    g_pConfig->restoreKeyEnabled = 0;
    g_pConfig->toggleKey = TK_ALT_Z;
    g_pConfig->useClipboardForUnicode = 1;
    wcsncpy_s(g_pConfig->macroFilePath, L"C:\\temp\\live_macros.ukm", _TRUNCATE);
    UnlockConfig();

    HANDLE hMap = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_SHARED_MEMORY_NAME);
    ASSERT_NE(hMap, nullptr);

    auto* pReadConfig = static_cast<const UniKeyConfig*>(
        MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, sizeof(UniKeyConfig))
    );
    ASSERT_NE(pReadConfig, nullptr);

    EXPECT_EQ(pReadConfig->inputEnabled, 0);
    EXPECT_EQ(pReadConfig->inputMethod, IM_TELEX);
    EXPECT_EQ(pReadConfig->toneType, TONE_CLASSIC);
    EXPECT_EQ(pReadConfig->spellCheck, 0);
    EXPECT_EQ(pReadConfig->freeToneMarking, 0);
    EXPECT_EQ(pReadConfig->macroEnabled, 1);
    EXPECT_EQ(pReadConfig->restoreKeyEnabled, 0);
    EXPECT_EQ(pReadConfig->toggleKey, TK_ALT_Z);
    EXPECT_EQ(pReadConfig->useClipboardForUnicode, 1);
    EXPECT_STREQ(pReadConfig->macroFilePath, L"C:\\temp\\live_macros.ukm");

    UnmapViewOfFile(pReadConfig);
    CloseHandle(hMap);
}

TEST_F(IPCTest, PerAppStateStore_SharedMappingContainsToggledEntry) {
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.inputEnabled = 1;
    config.perAppInputState = 1;

    ASSERT_TRUE(EnsurePerAppInputStateStore());
    EXPECT_EQ(ToggleInputEnabledForApp(&config, L"notepad.exe"), 0);

    HANDLE hMap = OpenFileMappingW(FILE_MAP_READ, FALSE, UNIKEY_PER_APP_STATE_MEMORY_NAME);
    ASSERT_NE(hMap, nullptr);

    const auto* shared = static_cast<const PerAppInputStateShared*>(
        MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, sizeof(PerAppInputStateShared)));
    ASSERT_NE(shared, nullptr);
    ASSERT_EQ(shared->version, UNIKEY_PER_APP_STATE_VERSION);

    bool found = false;
    for (const auto& entry : shared->entries) {
        if (!entry.used) {
            continue;
        }

        if (_wcsicmp(entry.appId, L"notepad.exe") == 0) {
            found = true;
            EXPECT_EQ(entry.inputEnabled, 0);
            break;
        }
    }

    EXPECT_TRUE(found);

    UnmapViewOfFile(shared);
    CloseHandle(hMap);
}

TEST_F(IPCTest, PerAppStateResolver_GlobalModeUsesInputEnabled) {
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.perAppInputState = 0;
    config.inputEnabled = 1;

    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"notepad.exe"), 1);

    config.inputEnabled = 0;
    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"notepad.exe"), 0);
}

TEST_F(IPCTest, PerAppStateResolver_PerAppModeUnseenInheritsSeedAndToggleScopesByAppId) {
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.perAppInputState = 1;
    config.inputEnabled = 1;

    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"notepad.exe"), 1);
    EXPECT_EQ(ToggleInputEnabledForApp(&config, L"C:\\Windows\\System32\\notepad.exe"), 0);
    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"notepad.exe"), 0);
    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"code.exe"), 0);

    EXPECT_EQ(ToggleInputEnabledForApp(&config, L"code.exe"), 1);
    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"code.exe"), 1);
    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"notepad.exe"), 0);
}

TEST_F(IPCTest, PerAppToggleDoesNotChangePersistedSeedSnapshot) {
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.perAppInputState = 1;
    config.inputEnabled = 1;

    ASSERT_TRUE(EnsurePerAppInputStateStore());
    SetPerAppInputStateSeed(1);

    EXPECT_EQ(ToggleInputEnabledForApp(&config, L"notepad.exe"), 0);
    EXPECT_EQ(config.inputEnabled, 0);

    const UniKeyConfig persisted = BuildPersistedConfigSnapshot(config);
    EXPECT_EQ(persisted.inputEnabled, 1);
    EXPECT_EQ(GetPerAppInputStateSeed(0), 1);
}

TEST_F(IPCTest, SetInputEnabledForAppUpdatesEffectiveStateWithoutChangingSeed) {
    UniKeyConfig config{};
    config.version = UNIKEY_CONFIG_VERSION;
    config.perAppInputState = 1;
    config.inputEnabled = 1;

    ASSERT_TRUE(EnsurePerAppInputStateStore());
    SetPerAppInputStateSeed(1);

    SetInputEnabledForApp(&config, L"C:\\Windows\\System32\\notepad.exe", 0);

    EXPECT_EQ(config.inputEnabled, 0);
    EXPECT_EQ(ResolveEffectiveInputEnabled(config, L"notepad.exe"), 0);
    EXPECT_EQ(GetPerAppInputStateSeed(0), 1);
}
