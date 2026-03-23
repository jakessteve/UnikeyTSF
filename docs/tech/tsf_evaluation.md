# TSF Integration — Technical Evaluation

## Current Architecture

UniKey TSF Reborn uses a **hybrid** architecture:

| Layer | Mechanism | Purpose |
|---|---|---|
| **Primary** | `WH_KEYBOARD_LL` hook | Intercepts keystrokes globally, processes through VnEngine, injects composed text via `SendInput` |
| **Secondary** | TSF COM DLL (`uktsf_core64.dll`) | Registered as an Input Method Editor for applications that query the TSF framework |

### Why This Hybrid?

TSF was the original design target, but several issues made it unreliable as the sole input mechanism:

1. **Manual user activation required** — Users must go to Settings → Language → Input Methods and manually select "UniKey TSF Reborn" before it works. This is a poor first-run experience.
2. **DLL loading failures** — `explorer.exe` sometimes locks the DLL or fails to load it, creating zombie processes.
3. **Limited COM lifetime control** — The DLL's lifetime is managed by the OS and COM, not by the application. Cleanup is unreliable.
4. **Chromium incompatibility** — Chrome and Edge have known issues with TSF composition, requiring clipboard-based workarounds regardless.

The keyboard hook (`WH_KEYBOARD_LL`) provides:
- **Immediate activation** — works as soon as `UniKeyTSF.exe` starts
- **Universal coverage** — works in all applications including Win32, UWP, and Electron
- **Full control** — hook lifetime is managed by the EXE process
- **Simpler debugging** — single process, single codebase

## TSF Advantages (When It Works)

| Feature | Hook | TSF |
|---|---|---|
| UWP/Store apps | ✅ (via hook) | ✅ (native) |
| Composition candidate window | ❌ | ✅ |
| Per-application input state | ❌ (global) | ✅ (per-thread) |
| System language bar integration | ❌ | ✅ |
| Touch keyboard integration | ❌ | ✅ |
| Windows Sandbox/AppContainer | ⚠️ (needs elevation) | ✅ (if ACLs set) |

## Recommendation

**Keep the hybrid architecture.** The keyboard hook should remain the primary input mechanism for reliability and user experience. The TSF DLL should be maintained as an optional component for:

1. **Future UWP deep integration** — when/if the Windows input model stabilizes
2. **System language bar presence** — showing UniKey in the language bar for discoverability
3. **Composition window** — for applications that benefit from a candidate list UI

### Roadmap

| Phase | Action | Priority |
|---|---|---|
| **Now** | Maintain TSF DLL compiled + registered, but hook does the work | ✅ Done |
| **Next** | Add per-app mode memory (remember E/V state per application) | Medium |
| **Future** | Explore `ITfFnReconversion` for richer TSF integration | Low |
| **Long-term** | Evaluate `InputPane` API for touch/tablet input | Low |

## Conclusion

The keyboard hook is the correct primary mechanism for a Vietnamese IME on Windows. TSF adds value as a supplementary channel but should **not** be the sole input path due to the activation UX barrier and COM reliability issues. The current hybrid design gives the best of both worlds: instant activation via hook, plus TSF registration for system integration.
