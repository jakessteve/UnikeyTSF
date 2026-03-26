#pragma once
#include <string>
#include <windows.h>
#include "../shared_config.h"

namespace Toolkit {

    // Converts text currently in the clipboard based on the source and destination charsets.
    // If str is provided, converts str directly instead of modifying clipboard.
    // srcCharset and destCharset use the Charset enum (e.g., CS_UNICODE)
    bool ConvertClipboardCharset(uint8_t srcCharset, uint8_t destCharset);

    // Strips Vietnamese accents from text in the clipboard.
    bool StripClipboardAccents();

    // Copies the currently selected text (Ctrl+C), strips Vietnamese accents,
    // and pastes it back (Ctrl+V), replacing the selection.
    bool StripSelectedTextAccents();

    // Copies the currently selected text (Ctrl+C), converts charset,
    // and pastes it back (Ctrl+V), replacing the selection.
    bool ConvertSelectedTextCharset(uint8_t srcCharset, uint8_t destCharset);

    // Overwrites the clipboard with newText. Used for "use clipboard for Unicode" feature.
    bool ModifyClipboardText(const std::wstring& newText);

} // namespace Toolkit
