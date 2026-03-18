# Task: Phase 2.1 - Vietnamese Spell Check & Smart Typo Restoration

## Architecture Context
The project is a C++ TSF / Global Keyboard Hook engine for Vietnamese typing (UniKeyTSF Reborn).
The core typing engine is in `src/engine/vn_engine.cpp`. Currently, the `spellCheck` config flag is checked, but there is no actual Vietnamese syllable validation logic implemented.

## Goal
Implement a Vietnamese syllable validator and smart typo restoration logic.
1. Create `src/engine/vn_dictionary.h` with a fast lookup or algorithmic validation for standard Vietnamese syllables (valid initial + vowel core + final combinations). 
2. Modify `src/engine/vn_engine.cpp` so that when `spellCheck = true`, the engine validates the `m_composition` string.
3. If the resulting word after a keystroke (tone/modifier) is NOT a valid Vietnamese word, it should REJECT the transformation and restore/output the raw keystroke ("smart typo restoration"). For example, typing "windows" with Telex should output "windows", not "ưindows", because "ưindows" is invalid.

## Files
- `src/engine/vn_engine.cpp`
- `src/engine/vn_engine.h`
- `src/engine/vn_dictionary.h` (NEW file)
- `tests/engine_test.cpp` (Add unit tests verifying spell check & typo restoration)

## Constraints
- Do NOT modify the core TSF or Win32 hook code in `main.cpp`.
- Keep the `vn_dictionary.h` dependency-free (use standard C++ containers like `std::unordered_set` or lightweight algorithmic rules). C++17 is allowed.
- Ensure all 69 existing unit tests in `tests/engine_test.cpp` still pass. Add new tests for spell checking.
- Do NOT use any external libraries for the dictionary.

## Verification
1. `npm run lint` / `CMake /WX` should pass.
2. `build_all.bat` should succeed and ALL unit tests must pass.

## Report Format
When done, output:
`[PROGRESS] Status: DONE`
Summarize the files changed and the approach used for the spell check validator.
