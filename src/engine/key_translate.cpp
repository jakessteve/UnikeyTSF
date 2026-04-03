#include "key_translate.h"

wchar_t TranslateVkToWchar(UINT vk, UINT scanCode, const BYTE keyboardState[256], HKL layout)
{
    if (!keyboardState) {
        return 0;
    }

    wchar_t chars[4] = {};
    const HKL effectiveLayout = layout ? layout : GetKeyboardLayout(0);
    const int translated = ToUnicodeEx(
        vk,
        scanCode,
        keyboardState,
        chars,
        4,
        4,
        effectiveLayout);

    if (translated <= 0) {
        return 0;
    }

    return chars[0];
}
