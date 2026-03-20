$c = Get-Content ".agent\scripts\spawn-agent.ps1" -Raw
$err = $null
$tokens = $null
$ast = [System.Management.Automation.Language.Parser]::ParseInput($c, [ref]$tokens, [ref]$err)
if ($null -eq $err -or $err.Count -eq 0) {
    Write-Host "No Syntax Errors"
} else {
    $err | ForEach-Object { Write-Host "$($_.Message) at line $($_.Extent.StartLineNumber) col $($_.Extent.StartColumnNumber)" }
}
