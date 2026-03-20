$ArgsArr = [string[]]$args
$logFile = $Env:MOCK_GEMINI_LOG
if (-not $logFile) {
    $logFile = Join-Path $PSScriptRoot "invoke.log"
}

$piped = ($input | Out-String).Trim()

# Use pipelined input if provided
if ([string]::IsNullOrWhiteSpace($piped)) {
    if (-not [Console]::IsInputRedirected) {
        $stdin = ""
    } else {
        $stdin = [Console]::In.ReadToEnd()
    }
} else {
    $stdin = $piped
}

$outputObj = @{
    args = $ArgsArr
    stdin = $stdin
}

$jsonObj = $outputObj | ConvertTo-Json -Depth 10 -Compress
Add-Content -Path $logFile -Value $jsonObj

if (Test-Path (Join-Path $PSScriptRoot "mock-delay.txt")) {
    $delay = [int](Get-Content (Join-Path $PSScriptRoot "mock-delay.txt"))
    Start-Sleep -Seconds $delay
}

if (Test-Path (Join-Path $PSScriptRoot "mock-output.txt")) {
    $outText = Get-Content (Join-Path $PSScriptRoot "mock-output.txt") -Raw
    [Console]::Out.WriteLine($outText)
}

if (Test-Path (Join-Path $PSScriptRoot "mock-error.txt")) {
    $errText = Get-Content (Join-Path $PSScriptRoot "mock-error.txt") -Raw
    [Console]::Error.WriteLine($errText)
}

if (Test-Path (Join-Path $PSScriptRoot "mock-exitcode.txt")) {
    $code = Get-Content (Join-Path $PSScriptRoot "mock-exitcode.txt")
    exit [int]$code
}

exit 0
