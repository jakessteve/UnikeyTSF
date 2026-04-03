$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $PSScriptRoot
$errors = New-Object System.Collections.Generic.List[string]
$warnings = New-Object System.Collections.Generic.List[string]

function Add-Error {
    param([string]$Message)
    $errors.Add($Message)
}

function Add-Warning {
    param([string]$Message)
    $warnings.Add($Message)
}

function Get-FileText {
    param([string]$RelativePath)
    $path = Join-Path $root $RelativePath
    if (-not (Test-Path -LiteralPath $path)) {
        Add-Error "Missing file: $RelativePath"
        return $null
    }
    return Get-Content -LiteralPath $path -Raw
}

function Assert-Contains {
    param(
        [string]$RelativePath,
        [string]$Needle,
        [string]$Description
    )

    $text = Get-FileText $RelativePath
    if ($null -eq $text) {
        return
    }
    if ($text -notmatch [regex]::Escape($Needle)) {
        Add-Error "$RelativePath missing: $Description"
    }
}

function Assert-PathReferencesExist {
    param([string]$RelativePath)

    $text = Get-FileText $RelativePath
    if ($null -eq $text) {
        return
    }

    $matches = [regex]::Matches($text, '(?<path>(rules|roles|skills|workflows|templates|references)/[A-Za-z0-9._\-/\*]+)')
    foreach ($match in $matches) {
        $rawPath = $match.Groups['path'].Value
        $candidate = $rawPath.Replace('/', '\')
        $fullPath = Join-Path $root $candidate
        if ($rawPath.Contains('*')) {
            $globMatches = Get-ChildItem -Path $fullPath -ErrorAction SilentlyContinue
            if (-not $globMatches) {
                Add-Error "$RelativePath references missing path: $rawPath"
            }
        }
        elseif (-not (Test-Path -LiteralPath $fullPath)) {
            Add-Error "$RelativePath references missing path: $rawPath"
        }
    }
}

Assert-Contains 'AGENTS.md' 'explicitly requested by the user' 'delegation policy wording aligned with explicit user request'
Assert-Contains 'workflows/delegate-implementation.md' 'templates/task-brief.md' 'task brief template reference'
Assert-Contains 'workflows/delegate-research.md' 'templates/task-brief.md' 'task brief template reference'
Assert-Contains 'workflows/delegate-implementation.md' 'off-limits files' 'full worker brief requirements'
Assert-Contains 'workflows/delegate-research.md' 'off-limits files' 'full explorer brief requirements'
Assert-Contains 'workflows/delegate-research.md' 'workflows/subagent-integration-checklist.md' 'delegate research integration checklist reference'
Assert-Contains 'templates/implementation-plan.md' '## Scope' 'scope section'
Assert-Contains 'templates/implementation-plan.md' '## Risks' 'risks section'
Assert-Contains 'templates/implementation-plan.md' '## Verification' 'verification section'
Assert-Contains 'templates/code-review-report.md' '## Residual Risk' 'residual risk section'
Assert-Contains 'templates/code-review-report.md' '## Verification Status' 'verification status section'
Assert-Contains 'AGENTS.md' 'references/task-routing.md' 'task routing reference'
Assert-Contains 'references/migration-map.md' 'references/task-routing.md' 'task routing migration mapping'
Assert-Contains 'AGENTS.md' 'references/migration-map.md' 'migration map reference'
Assert-Contains 'references/task-routing.md' 'skills/project-docs-system/SKILL.md' 'project docs system route'
Assert-Contains 'references/task-routing.md' 'workflows/project-docs.md' 'project docs workflow route'
Assert-Contains 'references/task-routing.md' 'templates/project-overview.md' 'project overview template route'
Assert-Contains 'references/task-routing.md' 'templates/system-map.md' 'system map template route'
Assert-Contains 'references/task-routing.md' 'templates/update-guide.md' 'update guide template route'
Assert-Contains 'references/task-routing.md' 'Primary skill: `skills/debugging-forensics/SKILL.md`' 'quick fix and debugging primary skill guidance'
Assert-Contains 'references/task-routing.md' 'Secondary skill: `skills/testing-verification/SKILL.md`' 'debugging secondary verification guidance'
Assert-Contains 'references/task-routing.md' 'Primary skill: `skills/testing-verification/SKILL.md`' 'integration check primary skill guidance'
Assert-Contains 'references/task-routing.md' 'Secondary skill: `skills/code-review/SKILL.md`' 'integration check secondary review guidance'
Assert-Contains 'skills/project-docs-system/SKILL.md' 'workflows/project-docs.md' 'project docs workflow reference'
Assert-Contains 'skills/project-docs-system/SKILL.md' 'templates/project-overview.md' 'project overview template reference'
Assert-Contains 'skills/project-docs-system/SKILL.md' 'templates/system-map.md' 'system map template reference'
Assert-Contains 'skills/project-docs-system/SKILL.md' 'templates/update-guide.md' 'update guide template reference'
Assert-Contains 'skills/architecture-design/SKILL.md' '## Example Uses' 'architecture examples section'
Assert-Contains 'skills/debugging-forensics/SKILL.md' '## Example Uses' 'debugging examples section'
Assert-Contains 'skills/project-docs-system/SKILL.md' '## Example Uses' 'project docs examples section'
Assert-Contains 'references/task-routing.md' 'roles/architect.md' 'architect role routing'
Assert-Contains 'references/task-routing.md' 'roles/backend.md' 'backend role routing'
Assert-Contains 'references/task-routing.md' 'roles/biz.md' 'business role routing'
Assert-Contains 'references/task-routing.md' 'roles/designer.md' 'designer role routing'
Assert-Contains 'references/task-routing.md' 'roles/devops.md' 'devops role routing'
Assert-Contains 'references/task-routing.md' 'roles/frontend.md' 'frontend role routing'
Assert-Contains 'references/task-routing.md' 'roles/pm.md' 'pm role routing'
Assert-Contains 'references/task-routing.md' 'roles/qc.md' 'qc role routing'
Assert-Contains 'references/task-routing.md' 'Follow-up verification: `workflows/subagent-integration-checklist.md`' 'delegation follow-up verification routing'

$referenceFiles = @(
    'AGENTS.md',
    'references/task-routing.md',
    'references/migration-map.md',
    'workflows/plan-feature.md',
    'workflows/code-review.md',
    'workflows/delegate-implementation.md',
    'workflows/delegate-research.md',
    'workflows/debug-issue.md',
    'workflows/implementation-review.md',
    'workflows/subagent-integration-checklist.md',
    'workflows/project-docs.md',
    'workflows/product-docs.md',
    'skills/project-docs-system/SKILL.md'
)

foreach ($file in $referenceFiles) {
    Assert-PathReferencesExist $file
}

$markdownFiles = Get-ChildItem -Path $root -Recurse -File -Filter *.md
$referencePatterns = New-Object System.Collections.Generic.List[string]
foreach ($file in $markdownFiles) {
    $text = Get-Content -LiteralPath $file.FullName -Raw
    $matches = [regex]::Matches($text, '(?<path>(rules|roles|skills|workflows|templates|references)/[A-Za-z0-9._\-/\*]+)')
    foreach ($match in $matches) {
        $referencePatterns.Add($match.Groups['path'].Value)
    }
}

foreach ($template in Get-ChildItem -Path (Join-Path $root 'templates') -File -Filter *.md) {
    $relative = $template.FullName.Substring($root.Length + 1).Replace('\', '/')
    $isReferenced = $false
    foreach ($pattern in $referencePatterns) {
        if ($pattern.Contains('*')) {
            if ($relative -like $pattern) {
                $isReferenced = $true
                break
            }
        }
        elseif ($relative -eq $pattern) {
            $isReferenced = $true
            break
        }
    }
    if (-not $isReferenced) {
        Add-Error "Unreferenced template: $relative"
    }
}

foreach ($workflow in Get-ChildItem -Path (Join-Path $root 'workflows') -File -Filter *.md) {
    $relative = $workflow.FullName.Substring($root.Length + 1).Replace('\', '/')
    $isReferenced = $false
    foreach ($pattern in $referencePatterns) {
        if ($pattern.Contains('*')) {
            if ($relative -like $pattern) {
                $isReferenced = $true
                break
            }
        }
        elseif ($relative -eq $pattern) {
            $isReferenced = $true
            break
        }
    }
    if (-not $isReferenced) {
        Add-Warning "Unreferenced workflow: $relative"
    }
}

if ($errors.Count -gt 0) {
    Write-Host 'Pack validation failed:' -ForegroundColor Red
    foreach ($error in $errors) {
        Write-Host " - $error" -ForegroundColor Red
    }
    exit 1
}

Write-Host 'Pack validation passed.' -ForegroundColor Green
if ($warnings.Count -gt 0) {
    Write-Host 'Pack validation warnings:' -ForegroundColor Yellow
    foreach ($warning in $warnings) {
        Write-Host " - $warning" -ForegroundColor Yellow
    }
}
