#pragma once
// =============================================================================
// UniKey TSF Reborn — IPC & Config Manager
// =============================================================================
// Manages shared memory (Local\UniKeyTSF_SharedConfig) used to communicate
// configuration between the EXE and TSF DLLs.
// Also handles loading and saving configuration to %APPDATA%.
// =============================================================================

#include <windows.h>
#include <string>
#include "../shared_config.h"

// --- Global access to shared configuration ---
// In the EXE, this points to mapped writeable shared memory.
// DLLs use it read-only.
extern UniKeyConfig* g_pConfig;
extern HANDLE       g_hSharedMutex;

// --- IPC Initialization & Cleanup ---
// Call at startup/exit of the EXE.
bool InitSharedMemory();
void CleanupSharedMemory();

// --- Config Persistence ---
// Loads/Saves g_pConfig to %APPDATA%\UniKeyTSF\config.dat.
bool LoadConfigFromFile(UniKeyConfig* pConfig);
bool SaveConfigToFile(const UniKeyConfig* pConfig);

// Helper to get the path to config.dat
std::wstring GetConfigFilePath();

// --- Access Synchronization ---
// Locks the shared mutex for thread-safe access to g_pConfig.
bool LockConfig();
void UnlockConfig();
