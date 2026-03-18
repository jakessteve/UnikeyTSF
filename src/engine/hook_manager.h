#pragma once
// =============================================================================
// UniKey TSF Reborn — Hook Manager
// =============================================================================
// Manages low-level keyboard and mouse hooks to intercept keystrokes
// and process them through the Vietnamese engine.
// =============================================================================

#include <windows.h>

// --- Hook Management ---
bool InstallHooks(HINSTANCE hInstance, HWND hWnd);
void UninstallHooks();

// --- Message Handling ---
// Call this from WndProc to handle hotkey-triggered actions
void HandleHotkey(WPARAM wParam);

// --- State Management ---
// Reset engine state (e.g., on focus change)
void ResetEngine();
