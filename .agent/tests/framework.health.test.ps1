$ErrorActionPreference = "Stop"
$projectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path

Write-Host "=== Framework Health Checks ===" -ForegroundColor Cyan

# 1. Setup & Dependencies
$spawnAgentPath = Join-Path $projectRoot ".agent\scripts\spawn-agent.ps1"
if (-not (Test-Path $spawnAgentPath)) {
    throw "FAIL: Core script spawn-agent.ps1 is missing!"
}
Write-Host "[OK] Dependencies" -ForegroundColor Green

# 2. Workflow Manifest Sync
$workflowsDir = Join-Path $projectRoot ".agent\workflows"
$physicalCount = @(Get-ChildItem $workflowsDir -Filter "*.md" | Where-Object { $_.Name -ne "MANIFEST.md" }).Count
$manifestLines = (Get-Content (Join-Path $workflowsDir "MANIFEST.md") | Select-String '\|\s+`\/').Count

if ($physicalCount -ne $manifestLines) {
    throw "FAIL: Workflow MANIFEST.md has $manifestLines entries, but there are $physicalCount files!"
}
Write-Host "[OK] Workflow Manifest SOT" -ForegroundColor Green

# 3. AGENTS.md Integrity
$agentsMd = Join-Path $projectRoot "AGENTS.md"
$content = Get-Content $agentsMd -Raw
$foundLinks = [regex]::Matches($content, '\.agent\/[\w\-\/]+\.md')
foreach ($m in $foundLinks) {
    $link = $m.Value
    if (-not (Test-Path (Join-Path $projectRoot $link))) {
        throw "FAIL: Broken internal link in AGENTS.md -> $link"
    }
}
Write-Host "[OK] AGENTS.md Broken Links" -ForegroundColor Green

Write-Host "ALL TESTS PASSED" -ForegroundColor Green
