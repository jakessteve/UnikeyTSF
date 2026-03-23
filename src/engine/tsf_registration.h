#pragma once
// =============================================================================
// UniKey TSF Reborn — TSF DLL Registration
// =============================================================================
// Handles (un)registration of the TSF COM server DLLs.
// =============================================================================

#include <windows.h>

bool RegisterTsfDll();
void UnregisterTsfDll();
