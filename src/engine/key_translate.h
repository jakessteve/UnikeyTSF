#pragma once

#include <windows.h>

wchar_t TranslateVkToWchar(UINT vk, UINT scanCode, const BYTE keyboardState[256], HKL layout);
