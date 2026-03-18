# Task: Phase 3 - Vietnamese Typing Engine (TSF Keystroke Sink & Composition)

## Goal
Implement the core Vietnamese typing capability in the pure C++17 TSF COM DLL. The DLL must intercept keystrokes via `ITfKeyEventSink`, perform Vietnamese composition (Telex/VNI) using a state machine engine, and render text inline via `ITfComposition`.

## Architecture Context
- **Project Type**: Native Windows C++17 COM Server (Text Services Framework)
- **Engine**: Pure Win32 API, NO heavy frameworks (Qt/MFC), NO standard library exceptions.
- **IPC**: Uses existing `shared_config.h` (memory-mapped) to read active Input Method (Telex/VNI) and Charset.
- **Memory Safety**: `Microsoft::WRL::ComPtr` MUST be used for all COM lifetime management.
- **Build System**: CMake with MSVC (`/utf-8` flag enabled).

## File Map

### Files to MODIFY
| File | What to change |
|------|---------------|
| `src/tsf/text_service.h` | Add `ITfKeyEventSink` interface inheritance and member variables for composition context, thread manager, clientId. |
| `src/tsf/text_service.cpp` | Implement `OnSetFocus` (to get context), `OnKeyDown`, `OnKeyUp`, `OnTestKeyDown`. Advise/Unadvise the key event sink. Handle basic toggling. |
| `CMakeLists.txt` | Add the new `.cpp` files to the `uktsf_core64` SHARED library target. |

### Files to CREATE
| File | Purpose |
|------|---------|
| `src/tsf/key_event_sink.h` / `.cpp` | Logic for handling intercepted keys, deciding whether to eat them, and delegating to the `vn_engine`. |
| `src/engine/vn_engine.h` / `.cpp` | Core Vietnamese typing state machine (Telex: aa->â, ow->ơ; VNI: a1->á, a6->â). Handles appending, backspacing, and finalizing words. |
| `src/tsf/composition.h` / `.cpp` | Wraps `ITfComposition` lifecycle (StartComposition, UpdateInlineText, EndComposition) and handles standard TSF Edit Sessions (`ITfEditSession`). |
| `src/config/blacklist.h` | Hardcoded array of process names (e.g., `cmd.exe`, `conhost.exe`, `WindowsTerminal.exe`, `mstsc.exe`) to bypass IME processing automatically. |

### Files to READ (context only — DO NOT modify)
| File | Why read |
|------|---------|
| `src/shared_config.h` | To read `g_pConfig->inputMethod` (TELEX/VNI) and `g_pConfig->inputEnabled`. |

## Step-by-Step Implementation

### Step 1: Keystroke Interception
Implement `ITfKeyEventSink` in `CUniKeyTextService`. Advise it during `Activate` and Unadvise during `Deactivate`. Log/Debug messages optional, but ensure keys (A-Z, 0-9, Backspace) can be trapped and "eaten" when Vietnamese mode is active.

### Step 2: Composition Edit Session
Implement a synchronous Edit Session (`ITfEditSession`) to mutate the document. TSF requires an `ITfEditSession` to insert or replace text. Create a helper class `CEditSession` that takes a lambda/callback to perform the actual DOM mutation.

### Step 3: Vietnamese Typing Engine State Machine
Create a robust `VnEngine` class. It should keep a buffer of the current "word being typed" (composition string). Apply Telex/VNI rules (e.g., if buffer is `a`, and input is `a`, buffer becomes `â`). 
Support basic logic:
- Vowels + Vowels -> Marked vowels (Telex)
- Vowels + Numbers -> Marked vowels (VNI)
- Vowels + Tone marks (s, f, r, x, j / 1, 2, 3, 4, 5) -> apply tones.

### Step 4: Connecting the Engine to Composition
When a key is typed:
1. Feed it to `VnEngine`.
2. If `VnEngine` absorbs it, start or update `ITfComposition`.
3. If it outputs a finalized word (e.g. typing space), end composition.
4. If it's a backspace, update composition or fallback to standard backspace.

### Step 5: Process Blacklisting
In `OnSetFocus`, get the executable name of the current process. If it's in `blacklist.h`, temporarily disable composition.

## Code Conventions
- **COM**: Use `ComPtr`. NEVER raw `AddRef`/`Release`.
- **Strings**: Use `wchar_t` and `std::wstring` for text processing. TSF uses `WCHAR` strings.
- **Includes**: Maintain standard Win32 include order.

## Acceptance Criteria
1. [ ] Builds successfully with `cmake --build build --config Release`.
2. [ ] Zero MSVC warnings (`/W4 /WX-`).
3. [ ] DLL intercepts keystrokes and inputs basic Vietnamese characters (e.g. typing "aa" outputs "â") in Notepad.

## Verification Commands
```bash
cmake -B build -G "Visual Studio 18 2026"
cmake --build build --config Release
```

## Report Format
### Changes Made
| File | Action | Description |
|------|--------|-------------|
### Decisions Made
- [Any ambiguous choices]
### Potential Issues
- [Anything needing orchestrator review]

**Progress Reporting:** After completing each major step, output a progress marker:
`[PROGRESS] Step X/Y: <what you just completed>`
When finished, output: `[PROGRESS] Status: DONE` or `[PROGRESS] Status: DONE_WITH_CONCERNS`
