---
description: Automated garbage collection for aged telemetry and debug reports
---
This workflow triggers a targeted cleanup of `.agent/spawn_agent_tasks/` and `.hc/debug/` logs older than 7 days to preserve framework token-efficiency and IDE performance.

1. Execute the cleanup script
// turbo
Run `powershell -File .\.agent\scripts\cleanup-state.ps1`
