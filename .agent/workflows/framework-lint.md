---
description: Validate .agent/ framework integrity — reference checks, manifest accuracy, and structural consistency
---

# Workflow: Framework Lint

> **Trigger:** After any `.agent/` file modification, or on-demand via `/framework-lint`.
> **Purpose:** Prevent reference drift, stale manifests, and broken cross-links.

## Pipeline

### 1. Baseline Health Tests (P0)
```powershell
# turbo
# Verify core framework health (manifest sync, bad markdown links)
& .\.agent\tests\framework.health.test.ps1
```
If any checks fail → stop and fix the output before proceeding.

### 2. Deep Reference Integrity Check
```powershell
# turbo
# Find any references to frequently-deleted/legacy files
$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
cd $projectRoot\.agent
Select-String -Path "**/*.md" -Pattern 'anti-patterns\.md[^-]' -SimpleMatch | ForEach-Object { "STALE: $($_.Filename):$($_.LineNumber)" }
```

### 3. AGENTS-LITE Sync Check
Verify AGENTS-LITE index files reference the correct rule names.
```powershell
# turbo
$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$liteFiles = Get-ChildItem (Join-Path $projectRoot ".agent\indexes\AGENTS-LITE*.md")
foreach ($f in $liteFiles) {
    Select-String -Path $f.FullName -Pattern 'anti-patterns[^-]' | ForEach-Object {
        "STALE LITE: $($f.Name):$($_.LineNumber) - should be anti-patterns-core"
    }
}
```

### 3.5 LITE Content Parity Check
Verify AGENTS-LITE role instructions visually match the core sections of full role files:
```powershell
# turbo
$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$roles = Get-ChildItem (Join-Path $projectRoot ".agent\roles\@*.md") | Where-Object { $_.Name -notmatch "extended" }
foreach ($r in $roles) {
    "WARN: Ensure .agent/indexes/AGENTS-LITE-$($r.Name -replace '@','') is synchronized with $($r.Name)"
}
```

### 4. Size Budget Check
```powershell
# turbo
$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
Write-Host "=== Role files ==="
Get-ChildItem (Join-Path $projectRoot ".agent\roles\*.md") | ForEach-Object { "$([math]::Round($_.Length/1024,1)) KB  $($_.Name)" } | Sort-Object -Descending
Write-Host ""
Write-Host "=== Universal rules ==="
@("anti-patterns-core.md","engineering-mindset.md","execution-protocol.md") | ForEach-Object {
    $p = Join-Path ".agent/rules" $_
    if (Test-Path $p) { "$([math]::Round((Get-Item $p).Length/1024,1)) KB  $_" }
}
```
Flag any role file > 8 KB or universal rule > 6 KB.

### 5. Dead Reference Scan
```powershell
# turbo
# Check for references to commonly-renamed/deleted files
$projectRoot = Resolve-Path (Join-Path $PSScriptRoot "..\..")
$deadRefs = @("AGENTS_OLD_TEMP", "anti-patterns\.md[^-]", "process-gates\.md", "agent-behavior\.md")
foreach ($pattern in $deadRefs) {
    $hits = Select-String -Path "$projectRoot\.agent\**\*.md" -Pattern $pattern
    if ($hits) { foreach ($h in $hits) { "DEAD REF: $($h.Filename):$($h.LineNumber) matches '$pattern'" } }
}
```

### 6. Report
If all checks pass: `✅ Framework lint: PASS`
If any fail: list issues and stop. Fix before merging/shipping.
