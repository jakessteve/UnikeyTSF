---
description: Automated Documentation Sync — Detects code changes and automatically updates drifting documentation like ARCHITECTURE.md, AGENTS.md, and KIs.
---

# Workflow: /sync-docs

Automatically read recent code changes, cross-reference them with project documentation, and update any files that have drifted.

---

## Prerequisites
- Completed code changes (staged or recently committed).
- @pm or @ba agent role active.

## Steps

### Step 1 — Diff Analysis
Run `git diff` (either `HEAD` for staged changes, or `origin/main` for a feature branch) to capture the exact code modifications.
Analyze the diff to understand:
- New features/components added.
- Architectural patterns modified.
- Configuration or agent framework rules changed.

### Step 2 — Cross-Reference Documentation
Scan the following documentation scopes for drift:
1. **Core Context**: `README.md`, `AGENTS.md`, `.agent/rules/`, and `.agent/roles/`.
2. **Architecture**: `.hc/phases/`, `docs/tech/ARCHITECTURE.md`, and any API contracts.
3. **Knowledge Items**: Any relevant `knowledge/` folders containing `metadata.json` and artifact markdown summaries.

### Step 3 — Apply Updates
1. Identify all documentation files that are now inaccurate or missing information based on the diff.
2. Formulate atomic updates for each file.
3. Ensure the updates follow standard formatting rules (e.g., no emojis, SOT-first grounding).
4. Apply edits directly using `replace_file_content` or `multi_replace_file_content`.

### Step 4 — Verification
Review the modified documentation. Verify that all claims remain accurate and that no hallucinated instructions were added. Report the list of synchronized files back to the user.
