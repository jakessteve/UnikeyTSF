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

**Keep the current hybrid architecture as the present-day runtime truth, while modernizing toward a TSF-primary target state only behind evidence-backed gates.** The keyboard hook remains the current primary mechanism today for reliability and startup simplicity, while the TSF DLL remains an active integration path that should continue to mature.

In the current repo state, TSF should be maintained as a first-class migration target rather than treated as a permanently secondary feature. That means:

1. **Current truth:** hook-primary hybrid runtime
2. **Near-term objective:** harden contracts, validation, and routing ownership
3. **Long-term objective:** allow TSF-primary rollout only after app-matrix and regression evidence is complete

### Roadmap

| Phase | Action | Priority |
|---|---|---|
| **Now** | Maintain TSF DLL compiled + registered, but hook does the work | ✅ Done |
| **Next** | Unify config contracts, harden the WebView bridge, and make routing ownership explicit | High |
| **Future** | Reach evidence-backed hook/TSF parity across the blocking app matrix | High |
| **Long-term** | Flip to guarded TSF-primary only when fallback and diagnostics are proven | High |

## Conclusion

The current hybrid design is the truthful description of the repo **today**. It should not be treated as proof that hybrid is the final architecture forever. The correct next step is to keep current reliability while reducing contract drift, improving routing control, and earning any future TSF-primary claim with explicit validation evidence.
