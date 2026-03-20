param(
    [int]$DaysOld = 7,
    [switch]$WhatIf
)

$targetDirs = @(
    ".agent\spawn_agent_tasks",
    ".hc\debug",
    ".hc\benchmarks"
)

$cutoffDate = (Get-Date).AddDays(-$DaysOld)
$deletedCount = 0
$deletedSize = 0

foreach ($dir in $targetDirs) {
    if (Test-Path -LiteralPath $dir) {
        $files = Get-ChildItem -Path $dir -File | Where-Object { $_.LastWriteTime -lt $cutoffDate }
        foreach ($file in $files) {
            $deletedSize += $file.Length
            if ($WhatIf) {
                Write-Host "WhatIf: Removing $($file.FullName)" -ForegroundColor Cyan
            } else {
                Remove-Item -LiteralPath $file.FullName -Force
                Write-Host "Removed $($file.FullName)" -ForegroundColor DarkGray
            }
            $deletedCount++
        }
    }
}

$sizeMB = [math]::Round($deletedSize / 1MB, 2)
Write-Host ""
Write-Host "Cleanup completed." -ForegroundColor Green
Write-Host "$deletedCount files removed ($sizeMB MB freed)" -ForegroundColor Green
