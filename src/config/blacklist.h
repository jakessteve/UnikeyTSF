#pragma once
// =============================================================================
// UniKey TSF Reborn — Process Blacklist (File-Based)
// =============================================================================
// Loads/saves a list of process names where UniKey should be temporarily
// disabled. Falls back to built-in defaults if the file doesn't exist.
// File location: same directory as the EXE, named "blacklist.txt".
// Each line = one process name (case-insensitive).
// =============================================================================

#include <string>
#include <vector>

// Get the path to blacklist.txt (next to the EXE)
std::wstring GetBlacklistFilePath();

// Load blacklist from file. Returns defaults if file doesn't exist.
std::vector<std::wstring> LoadBlacklist();

// Save blacklist to file
bool SaveBlacklist(const std::vector<std::wstring>& list);

// Initialize the blacklist (call once at startup)
void InitBlacklist();

// Check if a process name is blacklisted
bool IsProcessBlacklisted(const std::wstring& processName);

// Reload blacklist from file (call after settings save)
void ReloadBlacklist();
