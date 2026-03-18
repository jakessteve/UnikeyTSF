#include "macro.h"
#include <windows.h>
#include <fstream>
#include <sstream>

MacroEngine::MacroEngine() {
}

bool MacroEngine::Load(const std::wstring& filePath) {
    _macros.clear();
    if (filePath.empty()) return false;

    // Read the file entirely into memory
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) return false;

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    if (size == 0) return true;
    file.seekg(0, std::ios::beg);

    std::string buffer(size, '\0');
    if (!file.read(&buffer[0], size)) return false;

    // Skip UTF-8 BOM if present
    const char* p = buffer.data();
    if (size >= 3 && (unsigned char)p[0] == 0xEF && (unsigned char)p[1] == 0xBB && (unsigned char)p[2] == 0xBF) {
        p += 3;
        size -= 3;
    }

    // Convert UTF-8 to UTF-16
    int wlen = MultiByteToWideChar(CP_UTF8, 0, p, (int)size, nullptr, 0);
    if (wlen <= 0) return false;

    std::wstring wbuffer(wlen, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, p, (int)size, &wbuffer[0], wlen);

    // Parse lines
    std::wstringstream wss(wbuffer);
    std::wstring line;
    while (std::getline(wss, line)) {
        if (!line.empty() && line.back() == L'\r') {
            line.pop_back();
        }
        if (line.empty() || line[0] == L';') continue; // Skip comments

        size_t sep = line.find_first_of(L":\t ");
        if (sep != std::wstring::npos) {
            std::wstring key = line.substr(0, sep);
            size_t valStart = line.find_first_not_of(L":\t ", sep);
            if (valStart != std::wstring::npos) {
                std::wstring val = line.substr(valStart);
                _macros[key] = val;
            }
        }
    }

    return true;
}

bool MacroEngine::HasMacro(const std::wstring& keyword) const {
    return _macros.find(keyword) != _macros.end();
}

std::wstring MacroEngine::Expand(const std::wstring& keyword) const {
    auto it = _macros.find(keyword);
    if (it != _macros.end()) {
        return it->second;
    }
    return L"";
}

void MacroEngine::Clear() {
    _macros.clear();
}
