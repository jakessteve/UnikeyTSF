#pragma once
// =============================================================================
// UniKey TSF Reborn — Vietnamese Engine (State Machine)
// =============================================================================
// Core logic for Telex/VNI.
// =============================================================================

#include <string>
#include <vector>
#include "../shared_config.h"

class VnEngine {
public:
    VnEngine();
    
    // Process a key. Returns true if the key was "eaten" (handled by engine).
    bool ProcessKey(wchar_t ch, InputMethod method);

    // Get the current composition string.
    std::wstring GetCompositionString() const;

    // Get the raw keystroke buffer.
    std::wstring GetRawString() const;

    // Returns true if the last ProcessKey call produced a composition
    // that differs from the raw typed text (i.e. Vietnamese transformation occurred).
    bool DidTransform() const;

    // Clear the current word buffer.
    void Clear();

    // Check if we are currently in a word.
    bool IsInWord() const;

    // Check if a character is valid for continuing a word in the given method
    bool IsWordChar(wchar_t ch, InputMethod method) const;

    // Check if a character could be a Vietnamese modifier/tone key regardless of engine state.
    // Used to trigger reconversion when the engine is empty but a modifier key is pressed.
    bool IsPotentialModifier(wchar_t ch, InputMethod method) const;

    // Remove the last typed character and its mark.
    void RemoveLastChar();

    // Configuration setters
    void SetToneType(ToneType type) { _toneType = type; }
    void SetFreeToneMarking(bool enabled) { _freeToneMarking = enabled; }
    void SetSpellCheck(bool enabled) { _spellCheck = enabled; }

private:
    // Core composition builders
    std::wstring ApplyTelexRules(const std::wstring& raw);
    std::wstring ApplyVniRules(const std::wstring& raw);
    std::wstring ApplyViqrRules(const std::wstring& raw);

    std::wstring _raw;         // The exact sequence of keys typed (e.g., "tieengs")
    std::wstring _composition; // The resulting Vietnamese string (e.g., "tiếng")

    bool _isUppercase;
    bool _didTransform;        // True when composition != raw
    InputMethod _currentMethod;
    ToneType _toneType;        // Modern or classic tone placement
    bool _freeToneMarking;     // Allow free tone marking (bypass rules)
    bool _spellCheck;          // Vietnamese spell check enabled
};
