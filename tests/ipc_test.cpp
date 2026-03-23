#include <gtest/gtest.h>
#include <windows.h>
#include <thread>
#include <chrono>

#include "config/ipc_manager.h"
#include "shared_config.h"

class IPCTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure clean state
        CleanupSharedMemory();
    }

    void TearDown() override {
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
