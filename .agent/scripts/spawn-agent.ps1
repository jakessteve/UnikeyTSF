<#
.SYNOPSIS
    Spawn a worker agent in headless mode using any CLI agent.

.DESCRIPTION
    PowerShell port of spawn-agent.sh for Windows systems.
    Spawns a headless CLI worker agent to execute a scoped task.
    Supports any CLI agent (gemini, codex, claude, aider, etc.).

.EXAMPLE
    # Gemini — research (yolo, read-only)
    .\spawn-agent.ps1 -Agent gemini -ApprovalMode Yolo -Timeout 120 -File .agent\spawn_agent_tasks\research.md

.EXAMPLE
    # Claude — implementation (auto-approve edits, no timeout)
    .\spawn-agent.ps1 -Agent claude -ExtraArgs "--dangerously-skip-permissions" -File .agent\spawn_agent_tasks\task.md

.EXAMPLE
    # Quick inline task
    .\spawn-agent.ps1 -Agent gemini -ApprovalMode AutoEdit -Timeout 60 -Prompt "Fix typo 'recieve' -> 'receive' in auth.service.ts"
#>

[CmdletBinding()]
param(
    [string]$Agent = "gemini",

    [string]$ApprovalMode = "AutoEdit",

    [ValidateSet("Mechanical", "Integration", "Architecture")]
    [string]$ModelTier,

    [int]$Timeout = 0,

    [string]$Prompt,

    [string]$File,

    [string]$Output,

    [string]$ExtraArgs,

    [string]$FreezePath,

    [switch]$Async,

    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

if ($ModelTier) {
    switch ($ModelTier) {
        "Mechanical" {
            $ApprovalMode = "Yolo"
            $Timeout = 60
            Write-Host "[TIER] Mechanical: forcing -ApprovalMode Yolo -Timeout 60"
        }
        "Integration" {
            $ApprovalMode = "AutoEdit"
            $Timeout = 300
            Write-Host "[TIER] Integration: forcing -ApprovalMode AutoEdit -Timeout 300"
        }
        "Architecture" {
            $ApprovalMode = "AutoEdit"
            $Timeout = 600
            Write-Host "[TIER] Architecture: forcing -ApprovalMode AutoEdit -Timeout 600"
        }
    }
}

$projectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$TasksDir = Join-Path $projectRoot ".agent\spawn_agent_tasks"
$Timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$tempId = [guid]::NewGuid().ToString().Substring(0, 8)
$OutputFile = if ($Output) { $Output } else { Join-Path $TasksDir "output-$Timestamp-$tempId.log" }

if ($File) {
    if (-not (Test-Path -LiteralPath $File)) {
        Write-Error "[ERROR] Prompt file not found: $File"
        exit 1
    }
    $Prompt = Get-Content -LiteralPath $File -Raw
}

if (-not $Prompt) {
    if (-not [Console]::IsInputRedirected) {
        Write-Error "❌ No prompt provided. Use -Prompt, -File, or pipe input."
        exit 1
    }
    $Prompt = [Console]::In.ReadToEnd()
}

if ([string]::IsNullOrWhiteSpace($Prompt)) {
    Write-Error "❌ Empty prompt."
    exit 1
}

if ($FreezePath) {
    Write-Host "[FREEZE] Injecting Directory Freezing constraint for: $FreezePath" -ForegroundColor Cyan
    $freezeInstruction = "`n`n[CRITICAL CONTENTION AVOIDANCE: You are computationally FROZEN to the directory '$FreezePath'. You MUST NOT read, edit, or modify any source files outside this path unless explicitly authorized by @pm. This is a strict framework rule (anti-patterns-core.md Section 9).]"
    $Prompt += $freezeInstruction
}

# ─── Sanitize global GEMINI.md context to prevent CLI crashes ─────────
$globalGeminiFile = Join-Path $env:USERPROFILE ".gemini\GEMINI.md"
if (Test-Path -LiteralPath $globalGeminiFile) {
    try {
        $geminiContent = Get-Content -LiteralPath $globalGeminiFile -Raw
        if ($geminiContent -match '(?<![a-zA-Z0-9.+_])@([a-zA-Z0-9\-]+)') {
            Write-Host "Patching the user's global GEMINI.md context file to remove rogue @ tags that cause CLI crashes" -ForegroundColor Yellow
            $geminiContent = $geminiContent -replace '(?<![a-zA-Z0-9.+_])@([a-zA-Z0-9\-]+)', '$1'
            Set-Content -LiteralPath $globalGeminiFile -Value $geminiContent -NoNewline
        }
    } catch {
        Write-Host "[WARN] Failed to patch global GEMINI.md: $($_.Exception.Message)" -ForegroundColor Yellow
    }
}

$agentCmd = Get-Command $Agent -ErrorAction SilentlyContinue
if (-not $agentCmd) {
    Write-Error "❌ '$Agent' CLI not found. Ensure it is installed and on your PATH."
    exit 1
}

function Get-ApprovalFlags([string]$agent, [string]$mode) {
    $agentLower = $agent.ToLower()
    switch ($agentLower) {
        "gemini" {
            $modeValue = switch ($mode) {
                "Yolo"     { "yolo" }
                "AutoEdit" { "auto_edit" }
                "Safe"     { "default" }
                default    { "auto_edit" }
            }
            return @("--approval-mode", $modeValue)
        }
        "codex" {
            $modeValue = switch ($mode) {
                "Yolo"     { "full-auto" }
                "AutoEdit" { "auto-edit" }
                "Safe"     { "suggest" }
                default    { "auto-edit" }
            }
            return @("exec", "-c", """approval_mode=`"$modeValue`"""")
        }
        "claude" {
            return @()
        }
        default {
            return @()
        }
    }
}

$approvalFlags = Get-ApprovalFlags $Agent $ApprovalMode
$modeDisplay = if ($approvalFlags.Count -gt 0) { $approvalFlags -join " " } else { "(custom via -ExtraArgs)" }

$cmdArgs = @()
$cmdArgs += $approvalFlags

if ($ExtraArgs) {
    $cmdArgs += ($ExtraArgs -split '\s+(?=(?:[^"]*"[^"]*")*[^"]*$)')
}

$agentLower = $Agent.ToLower()
$useStdin = $false

if ($agentLower -eq "gemini" -or $agentLower -eq "claude") {
    $useStdin = $true
    if ($agentLower -eq "gemini") {
        $cmdArgs += @("-p", " ")
    }
} elseif ($agentLower -eq "codex") {
    $cmdArgs += $Prompt
} else {
    $cmdArgs += @("-p", $Prompt)
}

if (-not (Test-Path -LiteralPath $TasksDir)) {
    New-Item -ItemType Directory -Path $TasksDir -Force | Out-Null
}

$pmCmdSource = Join-Path $PSScriptRoot "pm.cmd"
$pmPs1Source = Join-Path $PSScriptRoot "pm.ps1"
$pmShSource  = Join-Path $PSScriptRoot "pm.sh"

$pmCmdTarget = Join-Path $projectRoot "pm.cmd"
$pmPs1Target = Join-Path $projectRoot "pm.ps1"
$pmShTarget  = Join-Path $projectRoot "pm.sh"

function Update-KickstartScript([string]$source, [string]$target) {
    if (Test-Path -LiteralPath $source) {
        if (-not (Test-Path -LiteralPath $target) -or ((Get-Item -LiteralPath $source).LastWriteTime -gt (Get-Item -LiteralPath $target).LastWriteTime)) {
            Copy-Item -LiteralPath $source -Destination $target -Force
        }
    }
}

Update-KickstartScript $pmCmdSource $pmCmdTarget
Update-KickstartScript $pmPs1Source $pmPs1Target
Update-KickstartScript $pmShSource $pmShTarget

$AgentUpper = $Agent.ToUpper()
$dryLabel = if ($DryRun) { " [DRY RUN]" } else { "" }
$timeoutDisplay = if ($Timeout -gt 0) { "${Timeout}s" } else { "None (wait forever)" }
$banner = "+======================================================+`n"
$banner += "|  🚀 Spawning $AgentUpper agent$dryLabel`n"
$banner += "+------------------------------------------------------+`n"
$banner += "|  Agent:   $Agent`n"
$banner += "|  Mode:    $modeDisplay`n"
$banner += "|  Timeout: $timeoutDisplay`n"
$banner += "|  Output:  $OutputFile`n"
$banner += "+======================================================+"
Write-Host $banner

if ($DryRun) {
    Write-Host ""
    Write-Host "[DRY] DRY RUN -- Command that would be executed:"
    Write-Host "  cmd.exe /c $Agent $($cmdArgs -join ' ')"
    exit 0
}

$header = "=== Spawn Agent: $AgentUpper ===`n"
$header += "Timestamp: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
$header += "Mode: $modeDisplay`n"
$header += "Timeout: $timeoutDisplay`n"
$header += "Prompt preview: $($Prompt.Substring(0, [Math]::Min(200, $Prompt.Length)))...`n"
$header += "================================`n`n"
$header | Tee-Object -FilePath $OutputFile

$ExitCode = 1

try {
    $tempId = [guid]::NewGuid().ToString().Substring(0, 8)
    $argsXml = "$TasksDir\temp_args_$tempId.xml"
    $runnerPs1 = "$TasksDir\temp_runner_$tempId.ps1"
    $promptFile = "$TasksDir\temp_prompt_$tempId.txt"
    $tmpOut = "$TasksDir\temp_out_$tempId.log"
    $tmpErr = "$TasksDir\temp_err_$tempId.log"
    
    $cmdArgs | Export-Clixml -Path $argsXml
    $agentCmd = Get-Command $Agent
    $agentPath = if ($agentCmd.Source) { $agentCmd.Source } elseif ($agentCmd.Path) { $agentCmd.Path } else { $Agent }
    
    $argsXmlEscaped = $argsXml -replace "'", "''"
    $agentPathEscaped = $agentPath -replace "'", "''"
    $promptFileEscaped = $promptFile -replace "'", "''"
    
    $runnerScript = "`$ErrorActionPreference = 'Stop'`n"
    $runnerScript += "`$OutputEncoding = [Console]::InputEncoding = [Console]::OutputEncoding = New-Object System.Text.UTF8Encoding `$false`n"
    $runnerScript += "`$argsArray = Import-Clixml -LiteralPath '$argsXmlEscaped'`n"
    if ($useStdin) {
        $jsonPayload = @{ prompt = $Prompt } | ConvertTo-Json -Depth 10 -Compress
        [System.IO.File]::WriteAllText($promptFile, $jsonPayload, (New-Object System.Text.UTF8Encoding $false))
        $runnerScript += "`n`$promptString = [System.IO.File]::ReadAllText('$promptFileEscaped', (New-Object System.Text.UTF8Encoding `$false))"
        $runnerScript += "`n`$promptString | & '$agentPathEscaped' @argsArray"
    } else {
        $runnerScript += "`n& '$agentPathEscaped' @argsArray"
    }

    if ($Async) {
        $outFileEscaped = $OutputFile -replace "'", "''"
        $runnerScript += " *>> '$outFileEscaped'"
        $runnerScript += "`nRemove-Item -LiteralPath '$argsXmlEscaped' -Force -ErrorAction SilentlyContinue"
        $runnerScript += "`nRemove-Item -LiteralPath '$promptFileEscaped' -Force -ErrorAction SilentlyContinue"
        $runnerScript += "`nRemove-Item -LiteralPath '$($runnerPs1 -replace "'", "''")' -Force -ErrorAction SilentlyContinue"
    }

    Set-Content -LiteralPath $runnerPs1 -Value $runnerScript

    $pwshCmd = Get-Command pwsh -ErrorAction SilentlyContinue
    $psExe = if ($pwshCmd) { "pwsh" } else { "powershell.exe" }

    $procArgs = @("-NoProfile", "-ExecutionPolicy", "Bypass", "-File", $runnerPs1)

    $maxAttempts = 5
    $attempt = 1
    $success = $false

    if ($Async) {
        Start-Sleep -Milliseconds (Get-Random -Minimum 2000 -Maximum 10000)
        Start-Process -FilePath $psExe -ArgumentList $procArgs -WindowStyle Hidden
        
        Write-Host ""
        Write-Host "+======================================================+" -ForegroundColor Cyan
        Write-Host "|  [ASYNC] $AgentUpper agent started in background." -ForegroundColor Cyan
        Write-Host "|  Logs will stream to: $OutputFile" -ForegroundColor Cyan
        Write-Host "+======================================================+" -ForegroundColor Cyan
        exit 0
    }

    while ($attempt -le $maxAttempts -and -not $success) {
        if ($attempt -gt 1) {
            Write-Host "`n[WARN] Retry attempt $attempt/$maxAttempts triggered for $Agent..." -ForegroundColor Yellow
        }

        Start-Sleep -Milliseconds (Get-Random -Minimum 2000 -Maximum 5000)
        
        if (Test-Path $tmpOut) { Remove-Item $tmpOut -Force -ErrorAction SilentlyContinue }
        if (Test-Path $tmpErr) { Remove-Item $tmpErr -Force -ErrorAction SilentlyContinue }

        $proc = Start-Process -FilePath $psExe -ArgumentList $procArgs -RedirectStandardOutput $tmpOut -RedirectStandardError $tmpErr -WindowStyle Hidden -PassThru
        
        if ($Timeout -gt 0) {
            $timeoutMs = $Timeout * 1000
            $elapsed = 0
            while (-not $proc.HasExited -and $elapsed -lt $timeoutMs) {
                Start-Sleep -Milliseconds 100
                $elapsed += 100
            }
            if (-not $proc.HasExited) {
                $proc.Kill()
                $ExitCode = 124
                Write-Host "`n⏰ $AgentUpper agent timed out after ${Timeout}s"
            } else {
                $ExitCode = $proc.ExitCode
            }
        } else {
            $proc.WaitForExit()
            $ExitCode = $proc.ExitCode
        }

        $agentOutput = if (Test-Path $tmpOut) { Get-Content -Raw $tmpOut } else { "" }
        if ([string]::IsNullOrWhiteSpace($agentOutput)) { $agentOutput = "" }
        $agentOutput | Add-Content -Path $OutputFile
        Write-Host $agentOutput

        $errOutput = if (Test-Path $tmpErr) { Get-Content -Raw $tmpErr } else { "" }
        if ([string]::IsNullOrWhiteSpace($errOutput)) { $errOutput = "" }
        $errOutput | Add-Content -Path $OutputFile
        if ($errOutput) { Write-Host $errOutput -ForegroundColor Yellow }

        if ($agentOutput -match '(?m)exhausted your capacity' -or $errOutput -match '(?m)exhausted your capacity' -or $agentOutput -match '429' -or $errOutput -match '429') {
            Write-Host "`n[ERROR] Gemini API Capacity Exhausted detected! (Code 429)" -ForegroundColor Red
            $ExitCode = 1
            if ($attempt -lt $maxAttempts) {
                $sleepSeconds = 15 * [math]::Pow(2, $attempt - 1)
                Write-Host "Sleeping $sleepSeconds seconds before retry (Exponential Backoff)..." -ForegroundColor Yellow
                Start-Sleep -Seconds $sleepSeconds
                $attempt++
                continue
            }
        } elseif (($ExitCode -eq 0 -or $null -eq $ExitCode) -and ($agentOutput -match '(?m)^\[ERROR\]' -or $errOutput -match '(?m)^\[ERROR\]')) {
            Write-Host "`n[WARN] ExitCode 0 but log contains [ERROR]. Overriding ExitCode to 1." -ForegroundColor Yellow
            $ExitCode = 1
        }

        $success = $true
    }

} catch {
    $ExitCode = 1
    $_.Exception.Message | Add-Content -Path $OutputFile
} finally {
    if (-not $Async) {
        if (Test-Path -LiteralPath $argsXml) { Remove-Item -LiteralPath $argsXml -Force -ErrorAction SilentlyContinue }
        # if (Test-Path -LiteralPath $runnerPs1) { Remove-Item -LiteralPath $runnerPs1 -Force -ErrorAction SilentlyContinue }
        if (Test-Path -LiteralPath $promptFile) { Remove-Item -LiteralPath $promptFile -Force -ErrorAction SilentlyContinue }
        if (Test-Path -LiteralPath $tmpOut) { Remove-Item -LiteralPath $tmpOut -Force -ErrorAction SilentlyContinue }
        if (Test-Path -LiteralPath $tmpErr) { Remove-Item -LiteralPath $tmpErr -Force -ErrorAction SilentlyContinue }
    }
}

$footer = "`n================================`nExit code: $ExitCode`nCompleted: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')`n"
$footer | Tee-Object -FilePath $OutputFile -Append

$BenchmarkFile = Join-Path $projectRoot ".agent\benchmarks\spawn-agent-benchmark.md"
if (Test-Path -LiteralPath $BenchmarkFile) {
    try {
        $StartTime = [datetime]::ParseExact($Timestamp, "yyyyMMdd-HHmmss", $null)
        $Duration = [math]::Round(((Get-Date) - $StartTime).TotalSeconds, 1)
        $PromptLen = $Prompt.Length
        $StatusEmoji = switch ($ExitCode) { 0 { "✅" } 124 { "⏰" } default { "❌" } }
        $TaskName = if ($File) { [System.IO.Path]::GetFileNameWithoutExtension($File) } else { "inline" }
        $BenchLine = "| $(Get-Date -Format 'yyyy-MM-dd') | $TaskName | $AgentUpper | $modeDisplay | ${PromptLen} chars | ${Duration}s | $timeoutDisplay | $ExitCode | $StatusEmoji |"
        Add-Content -Path $BenchmarkFile -Value $BenchLine
        Write-Host "[BENCH] Benchmark logged to $BenchmarkFile"
    } catch {
    }
}

Write-Host ""
if ($ExitCode -eq 0) {
    Write-Host "[OK] $AgentUpper agent completed successfully"
    Write-Host "[OK] Full output: $OutputFile"
} elseif ($ExitCode -eq 124) {
    Write-Host "[TIMEOUT] $AgentUpper agent timed out after ${Timeout}s"
    Write-Host "[LOG] Partial output: $OutputFile"
} else {
    Write-Host "[FAIL] $AgentUpper agent failed (exit code: $ExitCode)"
    Write-Host "[LOG] Output with errors: $OutputFile"
}

if ($MyInvocation.InvocationName -ne '.') {
    [Environment]::Exit($ExitCode)
} else {
    exit $ExitCode
}
