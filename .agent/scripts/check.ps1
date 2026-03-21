param([string]$Path = ".agent\scripts")

$files = Get-ChildItem -Path $Path -Filter "*.ps1" -Recurse
$totalErrors = 0

foreach ($file in $files) {
    if ($file.Name -eq "check.ps1") { continue }
    $c = Get-Content $file.FullName -Raw
    $err = $null
    $tokens = $null
    [void][System.Management.Automation.Language.Parser]::ParseInput($c, [ref]$tokens, [ref]$err)
    if ($null -ne $err -and $err.Count -gt 0) {
        Write-Host "Syntax errors in $($file.Name):" -ForegroundColor Red
        $err | ForEach-Object { Write-Host "  $($_.Message) at line $($_.Extent.StartLineNumber) col $($_.Extent.StartColumnNumber)" }
        $totalErrors += $err.Count
    }
}

if ($totalErrors -eq 0) {
    Write-Host "✅ No syntax errors found in $Path" -ForegroundColor Green
} else {
    Write-Host "❌ Found $totalErrors syntax errors!" -ForegroundColor Red
}
