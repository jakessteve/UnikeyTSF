#pragma once
#include <windows.h>

struct UniKeyConfig;

// Displays the Settings Dialog.
// hWndParent: the parent window handle
// hInstance: the application instance handle
void ShowSettingsDialog(HWND hWndParent, HINSTANCE hInstance, UniKeyConfig* pConfig);
