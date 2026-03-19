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

    [ValidateSet("AutoEdit", "Yolo", "Safe")]
    [string]$ApprovalMode = "AutoEdit",

    [ValidateSet("Mechanical", "Integration", "Architecture")]
    [string]$ModelTier,

    [int]$Timeout = 0,

    [string]$Prompt,

    [string]$File,

    [string]$Output,

    [string]$ExtraArgs,

    [switch]$DryRun
)

$ErrorActionPreference = "Stop"

# ─── ModelTier enforcement ───────────────────────────────────
# ModelTier sets approval mode and timeout only — does NOT force agent choice
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

# ─── Constants ──────────────────────────────────────────────
$TasksDir = ".agent\spawn_agent_tasks"
$Timestamp = Get-Date -Format "yyyyMMdd-HHmmss"
$OutputFile = if ($Output) { $Output } else { "$TasksDir\output-$Timestamp.log" }

# ─── Resolve prompt ─────────────────────────────────────────
if ($File) {
    if (-not (Test-Path $File)) {
        Write-Error "[ERROR] Prompt file not found: $File"
        exit 1
    }
    $Prompt = Get-Content $File -Raw
}

if (-not $Prompt) {
    # Check if there's piped input
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

# ─── Check agent availability ──────────────────────────────
$agentCmd = Get-Command $Agent -ErrorAction SilentlyContinue
if (-not $agentCmd) {
    Write-Error "❌ '$Agent' CLI not found. Ensure it is installed and on your PATH."
    exit 1
}

# Add approval flags for known agents
# For known agents (gemini, codex), map approval modes to their CLI flags.
# For unknown agents, skip approval mode mapping — use -ExtraArgs for custom flags.
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
            # Claude CLI: --dangerously-skip-permissions is handled via -ExtraArgs
            # No built-in approval mode mapping
            return @()
        }
        default {
            # Unknown agent — no approval mode mapping
            return @()
        }
    }
}

$approvalFlags = Get-ApprovalFlags $Agent $ApprovalMode
$modeDisplay = if ($approvalFlags.Count -gt 0) { $approvalFlags -join " " } else { "(custom via -ExtraArgs)" }

# ─── Build full command args ────────────────────────────────
$cmdArgs = @()

# Add approval flags for known agents
$cmdArgs += $approvalFlags

# Add custom args if provided
if ($ExtraArgs) {
    # Split the args string respecting quotes
    $cmdArgs += ($ExtraArgs -split '\s+(?=(?:[^"]*"[^"]*")*[^"]*$)')
}

# Add prompt flag and prompt
$agentLower = $Agent.ToLower()
if ($agentLower -eq "codex") {
    # Codex uses positional prompt after exec flags
    $cmdArgs += $Prompt
} else {
    # Most agents use -p for prompt (gemini, claude, etc.)
    $cmdArgs += @("-p", $Prompt)
}

# ─── Ensure output directory exists ─────────────────────────
if (-not (Test-Path $TasksDir)) {
    New-Item -ItemType Directory -Path $TasksDir -Force | Out-Null
}

# ─── Auto-install Kickstart Scripts ─────────────────────────
$pmCmdSource = Join-Path $PSScriptRoot "pm.cmd"
$pmPs1Source = Join-Path $PSScriptRoot "pm.ps1"
$pmShSource  = Join-Path $PSScriptRoot "pm.sh"

$projectRoot = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$pmCmdTarget = Join-Path $projectRoot "pm.cmd"
$pmPs1Target = Join-Path $projectRoot "pm.ps1"
$pmShTarget  = Join-Path $projectRoot "pm.sh"

function Update-KickstartScript([string]$source, [string]$target) {
    if (Test-Path $source) {
        if (-not (Test-Path $target) -or ((Get-Item $source).LastWriteTime -gt (Get-Item $target).LastWriteTime)) {
            Copy-Item -Path $source -Destination $target -Force
            Write-Host "[INIT] Auto-updated kickstart script: $(Split-Path $target -Leaf)" -ForegroundColor Yellow
        }
    }
}

Update-KickstartScript $pmCmdSource $pmCmdTarget
Update-KickstartScript $pmPs1Source $pmPs1Target
Update-KickstartScript $pmShSource $pmShTarget


# ─── Banner ─────────────────────────────────────────────────
$AgentUpper = $Agent.ToUpper()
$dryLabel = if ($DryRun) { " [DRY RUN]" } else { "" }
$timeoutDisplay = if ($Timeout -gt 0) { "${Timeout}s" } else { "None (wait forever)" }
$banner = @"
╔══════════════════════════════════════════════════════╗
║  🚀 Spawning $AgentUpper agent$dryLabel
╠══════════════════════════════════════════════════════╣
║  Agent:   $Agent
║  Mode:    $modeDisplay
║  Timeout: $timeoutDisplay
║  Output:  $OutputFile
╚══════════════════════════════════════════════════════╝
"@
Write-Host $banner

# ─── Dry Run ────────────────────────────────────────────────
if ($DryRun) {
    Write-Host ""
    Write-Host "[DRY] DRY RUN -- Command that would be executed:"
    Write-Host "  cmd.exe /c $Agent $($cmdArgs -join ' ')"
    Write-Host ""
    $promptLen = $Prompt.Length
    Write-Host "Prompt preview ($promptLen chars):"
    Write-Host ($Prompt.Substring(0, [Math]::Min(500, $Prompt.Length)))
    if ($Prompt.Length -gt 500) { Write-Host "..." }
    Write-Host ""
    Write-Host "Validation: prompt is $promptLen chars, agent '$Agent' available, mode '$modeDisplay'"
    exit 0
}

# ─── Write header to output log ─────────────────────────────
$header = @"
=== Spawn Agent: $AgentUpper ===
Timestamp: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
Mode: $modeDisplay
Timeout: $timeoutDisplay
Prompt preview: $($Prompt.Substring(0, [Math]::Min(200, $Prompt.Length)))...
================================

"@
$header | Tee-Object -FilePath $OutputFile

# ─── Build and execute command ──────────────────────────────
$ExitCode = 0

try {
    # Build the full command string for cmd.exe
    # npm shims are .cmd files that need cmd.exe to execute properly
    $escapedArgs = $cmdArgs | ForEach-Object { 
        if ($_ -match '\s') { "`"$_`"" } else { $_ }
    }
    $argString = $escapedArgs -join " "

    $psi = New-Object System.Diagnostics.ProcessStartInfo
    $psi.FileName = "cmd.exe"
    $psi.Arguments = "/c $Agent $argString"
    $psi.UseShellExecute = $false
    $psi.RedirectStandardOutput = $true
    $psi.RedirectStandardError = $true
    $psi.CreateNoWindow = $true

    $proc = New-Object System.Diagnostics.Process
    $proc.StartInfo = $psi

    # Capture output asynchronously to avoid deadlocks
    $stdoutBuilder = New-Object System.Text.StringBuilder
    $stderrBuilder = New-Object System.Text.StringBuilder

    $stdoutEvent = Register-ObjectEvent -InputObject $proc -EventName OutputDataReceived -Action {
        if ($null -ne $EventArgs.Data) {
            $Event.MessageData.AppendLine($EventArgs.Data) | Out-Null
            Write-Host $EventArgs.Data
        }
    } -MessageData $stdoutBuilder

    $stderrEvent = Register-ObjectEvent -InputObject $proc -EventName ErrorDataReceived -Action {
        if ($null -ne $EventArgs.Data) {
            $Event.MessageData.AppendLine($EventArgs.Data) | Out-Null
        }
    } -MessageData $stderrBuilder

    # Add jitter to prevent API Rate Limits (429 Too Many Requests) during parallel swarms
    Start-Sleep -Milliseconds (Get-Random -Minimum 2000 -Maximum 10000)

    $proc.Start() | Out-Null
    $proc.BeginOutputReadLine()
    $proc.BeginErrorReadLine()

    if ($Timeout -gt 0) {
        $timeoutMs = $Timeout * 1000
        $elapsed = 0
        while (-not $proc.HasExited -and $elapsed -lt $timeoutMs) {
            Start-Sleep -Milliseconds 100
            $elapsed += 100
        }
        if (-not $proc.HasExited) {
            $proc.Kill()
            $ExitCode = 124  # mimic Unix timeout exit code
            Write-Host "`n⏰ $AgentUpper agent timed out after ${Timeout}s"
        } else {
            $proc.WaitForExit() # Flush async buffers
            $ExitCode = $proc.ExitCode
        }
    } else {
        # No timeout — wait indefinitely
        while (-not $proc.HasExited) {
            Start-Sleep -Milliseconds 100
        }
        $proc.WaitForExit() # Flush async buffers
        $ExitCode = $proc.ExitCode
    }

    # Wait a moment for async events to flush
    Start-Sleep -Milliseconds 500

    $stdoutIdentifier = if ($stdoutEvent.Name) { $stdoutEvent.Name } else { $stdoutEvent.SourceIdentifier }
    $stderrIdentifier = if ($stderrEvent.Name) { $stderrEvent.Name } else { $stderrEvent.SourceIdentifier }
    
    Unregister-Event -SourceIdentifier $stdoutIdentifier -ErrorAction SilentlyContinue
    Unregister-Event -SourceIdentifier $stderrIdentifier -ErrorAction SilentlyContinue

    # Append captured output to log
    $agentOutput = $stdoutBuilder.ToString()
    if ($agentOutput) {
        $agentOutput | Add-Content -Path $OutputFile
    }
    $errOutput = $stderrBuilder.ToString()
    if ($errOutput) {
        $errOutput | Add-Content -Path $OutputFile
    }

    # Manually detect Gemini CLI import crashes or rate limits that exit with 0
    if ($ExitCode -eq 0 -and ($agentOutput -match '\[ERROR\]|exhausted your capacity' -or $errOutput -match '\[ERROR\]|exhausted your capacity')) {
        $ExitCode = 1
        Write-Host "`n[WARN] Overriding ExitCode to 1 due to detected error in output." -ForegroundColor Yellow
    }

} catch {
    $ExitCode = 1
    $_.Exception.Message | Tee-Object -FilePath $OutputFile -Append
}

# ─── Write footer ──────────────────────────────────────────
$footer = @"

================================
Exit code: $ExitCode
Completed: $(Get-Date -Format 'yyyy-MM-dd HH:mm:ss')
"@
$footer | Tee-Object -FilePath $OutputFile -Append

# ─── Auto-benchmark logging ──────────────────────────────────
$BenchmarkFile = ".agent\benchmarks\spawn-agent-benchmark.md"
if (Test-Path $BenchmarkFile) {
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
        Write-Host "[WARN] Benchmark logging failed: $($_.Exception.Message)"
    }
}

# ─── Report ─────────────────────────────────────────────────
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

exit $ExitCode
