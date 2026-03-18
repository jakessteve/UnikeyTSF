#pragma once
// =============================================================================
// UniKey TSF Reborn — System Tray Manager
// =============================================================================
// Handles the V/E tray icon, owner-drawn context menu, and user interactions.
// =============================================================================

#include <windows.h>
#include "../shared_config.h"

// Tray message ID
#define WM_TRAYICON (WM_USER + 1)

// --- Tray Management ---
bool InitTrayIcon(HWND hWnd);
void UpdateTrayIcon();
void RemoveTrayIcon();
void ShowContextMenu(HWND hWnd);
void ToggleInputState();

// --- Resource Cleanup ---
void CleanupTrayResources();

// --- Owner-Drawn Menu Helpers ---
// Call these from WndProc to handle owner-drawn menu items
void OnMeasureItem(HWND hWnd, MEASUREITEMSTRUCT* lpMeasureItem);
void OnDrawItem(HWND hWnd, DRAWITEMSTRUCT* lpDrawItem);
