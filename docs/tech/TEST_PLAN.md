# Test Plan

## Current Test Strategy

UniKeyTSF currently relies on a mix of GoogleTest-based executable suites and targeted UI/runtime checks. The project has good coverage around the composition engine and core helper layers, but it still lacks the full automation and app-matrix evidence required for a TSF-primary default signoff.

## Current Executable Suites

From `build64/Release`, the repo currently expects these suites to exist:

- `UniKeyTests.exe`
- `IPCTests.exe`
- `ClipboardTests.exe`
- `SettingsUITests.exe`
- `SettingsMessageContractTests.exe`
- `CharacterizationTests.exe`
- `RoutingTests.exe`
- `TsfDiagnosticsTests.exe`

## Coverage by Suite

| Suite / File | Current focus |
|---|---|
| `tests/engine_test.cpp` | `VnEngine` behavior across Telex, VNI, VIQR, tone placement, backspace handling, and macros |
| `tests/spellcheck_test.cpp` | spelling and restoration-oriented engine behavior |
| `tests/ipc_test.cpp` | shared-memory creation, mutex locking behavior, and cross-process config integrity |
| `tests/clipboard_test.cpp` | Unicode/VNI conversion and accent stripping helpers |
| `tests/e2e/settings_ui_test.cpp` | settings window open/close/reopen behavior |
| `tests/settings_message_contract_test.cpp` | typed WebView/native message envelope parsing, version enforcement, and outbound payload generation |
| `tests/characterization_test.cpp` + `tests/characterization_harness.*` | Task 2 frozen baseline contract for current observable typing semantics |
| `tests/routing_test.cpp` | routing ownership, fallback-scoped behavior, and decision-state regression coverage |
| `tests/tsf_diagnostics_test.cpp` | Task 5 TSF diagnostics contract: status mapping, machine-readable output fields, and actionable next-step values |

## Build-and-Test Baseline

For modernization work, the current baseline is:

1. Build the Release solution.
2. Run the existing regression executables from `build64/Release`.
3. Add new behavior-gating harnesses before changing routing defaults.

For guarded routing validation, the manager can be launched with:

- `UniKeyTSF.exe /routing-hook-primary`
- `UniKeyTSF.exe /routing-tsf-primary`
- `UniKeyTSF.exe /routing-fallback-scoped`

or by setting `UNIKEY_ROUTING_MODE` to `hook_primary`, `tsf_primary`, or `fallback_scoped` before launch.

The modernization plan’s definition of done adds more gates later, but the current suites should still be treated as real release-blocking regressions for the current hybrid runtime.

## Task 2 Characterization Harness (Frozen Contract)

Purpose (Task 2 only): freeze current observable behavior before any routing-seam or TSF-primary work.

From `build64/Release`:

- `./CharacterizationTests.exe --gtest_brief=1`
- `./CharacterizationTests.exe --gtest_output=xml:../../.sisyphus/evidence/task-2-characterization.xml`

Frozen coverage in this harness:

- Vietnamese ON/OFF baseline
- Telex, VNI, and VIQR typing baselines
- delimiter commit behavior
- toggle during active input baseline
- macro expansion baseline
- restore/reconversion baseline (including reconversion helper contract)
- per-app input state expectation baseline
- typing-relevant setting baselines (`inputMethod`, `toneType`, `spellCheck`, `freeToneMarking`, `restoreKeyEnabled`)

## Required Modernization Additions

The active modernization plan requires these layers before a TSF-primary default flip:

- characterization harnesses that freeze current observable behavior
- routing-authority regression checks that prevent dual processing
- config/state propagation checks across manager, hook, and TSF paths
- TSF activation/onboarding diagnostics
- app-matrix smoke coverage for the blocking validation set

## Task 5 Activation Diagnostics Evidence

From `build64/Release` (or equivalent config output directory):

- `./TsfDiagnosticsTests.exe --gtest_brief=1`
- `./UniKeyTSF.exe /tsf-diagnostics`

Optional failure/success path capture for evidence files:

- `./UniKeyTSF.exe /unregister`
- `./UniKeyTSF.exe /tsf-diagnostics`  (expected non-ready status)
- `./UniKeyTSF.exe /register`
- `./UniKeyTSF.exe /tsf-diagnostics`  (expected ready, profile_missing, or profile_inactive with actionable next_step)

Task 5 evidence files:

- `.sisyphus/evidence/task-5-tsf-diagnostics.txt`
- `.sisyphus/evidence/task-5-tsf-diagnostics-failure.txt`

## Blocking Validation Matrix

The plan’s default blocking matrix is:

- Notepad
- Edge textarea
- VS Code editor
- Windows Terminal
- elevated Notepad
- WordPad or another RichEdit-style editor

These scenarios are required evidence for modernization readiness even though they are not all automated today.

## Acceptance Policy for Current Repo State

At the current stage of the repo, the truthful acceptance bar is:

- the Release solution builds successfully
- existing regression executables continue to pass
- docs do not overclaim pure-TSF behavior
- future routing or TSF-primary work must add new automated evidence rather than relying on these existing suites alone

## Non-Goals of the Current Test Surface

The existing test set does **not** currently prove:

- TSF as the sole authoritative input path
- complete parity between hook and TSF behavior
- bounded fallback behavior under explicit routing control
- full compatibility across the blocking app matrix

Those are explicit modernization tasks, not facts about the current repo.
