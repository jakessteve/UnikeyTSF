---
description: Validate .agent/ framework integrity — reference checks, manifest accuracy, and structural consistency
---

# Workflow: Framework Lint

> **Trigger:** After any `.agent/` file modification, or on-demand via `/framework-lint`.  
> **Purpose:** Prevent reference drift, stale manifests, and broken cross-links.

## Pipeline

### 1. Reference Integrity Check
```powershell
# turbo
# Find any references to deleted files
cd .agent
Select-String -Path "**/*.md" -Pattern 'anti-patterns\.md[^-]' -SimpleMatch | ForEach-Object { "STALE: $($_.Filename):$($_.LineNumber)" }
```
If any results → fix before proceeding.

### 2. File Existence Check
```powershell
# turbo
# Verify all files referenced in MANIFEST files exist
$manifests = @("rules/MANIFEST.md", "skills/MANIFEST.md", "workflows/MANIFEST.md")
foreach ($m in $manifests) {
    $path = Join-Path ".agent" $m
    if (Test-Path $path) {
        Select-String -Path $path -Pattern '\| .+?\| (rules|skills|workflows|roles)/.+?\.md' | ForEach-Object {
            if ($_.Line -match '((?:rules|skills|workflows|roles)/[\w\-/]+\.md)') {
                $ref = $Matches[1]
                $full = Join-Path ".agent" $ref
                if (-not (Test-Path $full)) { "MISSING: $ref (referenced in $m)" }
            }
        }
    }
}
```

### 3. AGENTS-LITE Sync Check
Verify AGENTS-LITE index files reference the correct rule names:
```powershell
# turbo
$liteFiles = Get-ChildItem ".agent/indexes/AGENTS-LITE*.md"
foreach ($f in $liteFiles) {
    Select-String -Path $f.FullName -Pattern 'anti-patterns[^-]' | ForEach-Object {
        "STALE LITE: $($f.Name):$($_.LineNumber) - should be anti-patterns-core"
    }
}
```

### 4. Size Budget Check
```powershell
# turbo
Write-Host "=== Role files ==="
Get-ChildItem ".agent/roles/*.md" | ForEach-Object { "$([math]::Round($_.Length/1024,1)) KB  $($_.Name)" } | Sort-Object -Descending
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
$deadRefs = @("AGENTS_OLD_TEMP", "anti-patterns\.md[^-]", "process-gates\.md", "agent-behavior\.md")
foreach ($pattern in $deadRefs) {
    $hits = Select-String -Path ".agent/**/*.md" -Pattern $pattern
    if ($hits) { foreach ($h in $hits) { "DEAD REF: $($h.Filename):$($h.LineNumber) matches '$pattern'" } }
}
```

### 6. Report
If all checks pass: `✅ Framework lint: PASS`  
If any fail: list issues and stop. Fix before merging/shipping.
