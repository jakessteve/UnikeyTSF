#pragma once
#include <string>
#include <unordered_map>

class MacroEngine {
public:
    MacroEngine();
    
    // Load macros from a .ukm file
    bool Load(const std::wstring& filePath);
    
    // Check if the exact keyword exists
    bool HasMacro(const std::wstring& keyword) const;

    // Expand the keyword, returns empty if not found
    std::wstring Expand(const std::wstring& keyword) const;
    
    // Clear all macros
    void Clear();

private:
    std::unordered_map<std::wstring, std::wstring> _macros;
};
