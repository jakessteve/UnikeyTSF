$ErrorActionPreference = "Stop"

$scriptDir = $PSScriptRoot
$projectRoot = (Resolve-Path (Join-Path $scriptDir "..\..\..")).Path
$spawnScript = Join-Path $projectRoot ".agent\scripts\spawn-agent.ps1"

# Completely sandbox PATH so NO global 'gemini' (cargo/npm/etc) can shadow our mock
$env:PATH = "$scriptDir;C:\Windows\system32;C:\Windows;C:\Windows\System32\WindowsPowerShell\v1.0\"
$env:MOCK_GEMINI_LOG = Join-Path $scriptDir "invoke_$(Get-Random).log"
$spawnLog = Join-Path $scriptDir "spawn_$(Get-Random).log"

function Reset-Mock {
    if (Test-Path $env:MOCK_GEMINI_LOG) { Remove-Item $env:MOCK_GEMINI_LOG -Force }
    if (Test-Path $spawnLog) { Remove-Item $spawnLog -Force }
    
    Remove-Item (Join-Path $scriptDir "mock-delay.txt") -Force -ErrorAction SilentlyContinue
    Remove-Item (Join-Path $scriptDir "mock-output.txt") -Force -ErrorAction SilentlyContinue
    Remove-Item (Join-Path $scriptDir "mock-error.txt") -Force -ErrorAction SilentlyContinue
    Remove-Item (Join-Path $scriptDir "mock-exitcode.txt") -Force -ErrorAction SilentlyContinue
}

function Set-MockState([string]$key, [string]$value) {
    Set-Content -Path (Join-Path $scriptDir "mock-$key.txt") -Value $value
}

function Assert-Equal($expected, $actual, $message) {
    if ($expected -ne $actual) {
        Write-Host "[FAIL] $message" -ForegroundColor Red
        Write-Host "  Expected: '$expected'" -ForegroundColor Red
        Write-Host "  Actual:   '$actual'" -ForegroundColor Red
        if (Test-Path $spawnLog) { Write-Host "--- SPAWN LOG ---"; Get-Content $spawnLog }
        exit 1
    } else {
        Write-Host "[PASS] $message" -ForegroundColor Green
    }
}

function Run-Agent([string]$prompt, [string[]]$extraArgs) {
    $agentCmd = "gemini"
    $tmpPromptFile = Join-Path $scriptDir "prompt_$(Get-Random).txt"
    $prompt | Set-Content -Path $tmpPromptFile -NoNewline -Encoding UTF8

    $baseArgs = @(
        "-NoProfile", "-ExecutionPolicy", "Bypass",
        "-File", $spawnScript, "-Agent", $agentCmd, "-File", $tmpPromptFile
    )
    if ($extraArgs) {
        $baseArgs += $extraArgs
    }
    
    $proc = Start-Process -FilePath "powershell.exe" -ArgumentList $baseArgs -RedirectStandardOutput "$spawnLog" -RedirectStandardError "$spawnLog.err" -PassThru -Wait
    return $proc
}

Write-Host "Starting tests for spawn-agent.ps1..."

# Test 1: Simple Prompt Execution
Reset-Mock
Set-MockState "output" "Mock success output"
$proc = Run-Agent -prompt "Hello World" -extraArgs @("-ApprovalMode", "AutoEdit")
Assert-Equal 0 $proc.ExitCode "Test 1: Normal execution exit code"
$mockData = Get-Content $env:MOCK_GEMINI_LOG | ConvertFrom-Json
$expectedPayload = @{ prompt = "Hello World" } | ConvertTo-Json -Depth 10 -Compress
Assert-Equal $expectedPayload $mockData.stdin "Test 1: Stdin payload matches JSON encoding"

# Test 2: Exit Code Override on [ERROR]
Reset-Mock
Set-MockState "output" "[ERROR] Something went wrong but I exited 0"
$proc = Run-Agent -prompt "Test ERROR override"
Assert-Equal 1 $proc.ExitCode "Test 2: Overrides exit code 0 to 1 when [ERROR] is in stdout"

# Test 3: Timeout Enforcement
Reset-Mock
Set-MockState "delay" "2"
$proc = Run-Agent -prompt "Test Timeout" -extraArgs @("-Timeout", "1")
Assert-Equal 124 $proc.ExitCode "Test 3: Enforces Timeout and returns 124"

# Test 4: Special Characters in Prompt
Reset-Mock
$complexPrompt = "Test `r`n `$var `"`" '' back``tick"
$proc = Run-Agent -prompt $complexPrompt
Assert-Equal 0 $proc.ExitCode "Test 4: Special characters exit code is 0"
$mockData = Get-Content $env:MOCK_GEMINI_LOG | ConvertFrom-Json
$expectedPayload = @{ prompt = $complexPrompt } | ConvertTo-Json -Depth 10 -Compress
Assert-Equal $expectedPayload $mockData.stdin "Test 4: Complex characters passed safely via JSON serialization"

# Test 5: Async Dispatch
Reset-Mock
$proc = Run-Agent -prompt "Test Async" -extraArgs @("-Async")
Assert-Equal 0 $proc.ExitCode "Test 5: Async mode returns exit code 0 immediately"

Write-Host "All tests completed successfully." -ForegroundColor Cyan
