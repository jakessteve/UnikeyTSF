# Feature Documentation: Startup Settings UI Control

## Feature ID: FEAT-001

## Status: Already Implemented

## Summary
The feature to control whether the settings UI opens on startup **already exists** in the codebase. This document clarifies how it works.

## How It Works

### Config Field
The `showDialogOnStartup` field in `UniKeyConfig` controls this behavior:
- `true` (1): Settings dialog is shown when the app starts
- `false` (0): App starts minimized to tray without showing settings

### Implementation Locations

#### 1. Config Definition (`src/shared_config.h`)
```cpp
struct UniKeyConfig {
    // ... other fields ...
    uint8_t  showDialogOnStartup;    // 1 = enabled
    // ...
};
```

#### 2. Startup Behavior (`src/main.cpp`, lines 338-340)
```cpp
if (g_pConfig && g_pConfig->showDialogOnStartup) {
    PostMessage(g_hWnd, WM_COMMAND, IDM_SETTINGS, 0);
}
```

#### 3. Settings UI Updates (`src/ui/settings_webview.cpp`, line 284)
The setting is saved when the user changes it in the settings UI:
```cpp
g_pConfig->showDialogOnStartup = parsedMessage.config.showDialogOnStartup;
```

#### 4. Message Contract (`src/ui/settings_message_contract.cpp`)
The `showDialogOnStartup` field is included in the config payload parsing and building.

## User Experience

### When "Show settings on startup" is CHECKED:
1. User launches UniKeyTSF
2. App initializes in the background
3. Settings dialog automatically appears
4. User can configure settings immediately

### When "Show settings on startup" is UNCHECKED:
1. User launches UniKeyTSF
2. App initializes in the background
3. App minimizes to system tray silently
4. User can access settings by double-clicking the tray icon

## Default Value
The default value is set to `1` (enabled) in `src/config/ipc_manager.cpp` (line 138):
```cpp
g_pConfig->showDialogOnStartup = 1;
```

## Notes for Frontend Team
The frontend settings UI should include a toggle for this option. The field is already available in the config message contract:
- Send as part of `UPDATE_CONFIG` message
- Receive as part of `SET_CONFIG` message
- Field name: `showDialogOnStartup` (boolean)
