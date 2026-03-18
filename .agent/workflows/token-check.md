---
description: Context audit and token-burn mitigation workflow. Use when approaching complexity tier token limits.
---

# Workflow: /token-check

**Trigger:** Agent exceeds 80% of current tier's token limit, or user invokes `/token-check`.

**Goal:** Identify bloat in the current conversation, trim unnecessary context, and conditionally prepare a `/handoff` if the conversation needs a hard reset to prevent extreme token burn.

## Pipeline Steps

### 1. Check Usage Estimates
1. Consult `.agent/rules/decision-routing.md` to identify the current task's complexity tier and its associated token limit.
2. Estimate the current prompt length based on recently read files, extensive conversation history, or loaded large artifacts.

### 2. Context Trimming (First Resort)
1. Close out any completed sub-tasks in `task.md`.
2. Summarize key decisions made so far in a single concise chunk and stop referencing the granular chat history or KIs that led to them.
3. Identify files that were read but are no longer relevant to the immediate next steps. Mentally (or explicitly in output) note to stop tracking them.

### 3. Evaluate Need for Handoff
If the current estimated token usage is near the maximum limit for the task tier, AND more than 20% of the work remains:
1. It is time to execute the Context Reset Protocol.
2. Immediately formulate a summary of work completed, current state, and work remaining.
3. Generate a `.hc/handoffs/[date]-[task]-phase-[N].md` artifact capturing the exact state of progress. (See `/handoff` workflow for structure).
4. Inform the user: *"We are approaching the token limit for this complexity tier. I have created a `/handoff` artifact. Please start a new chat session and upload this artifact to reset the context window and continue efficiently."*

### 4. Continuous Execution (If Safe)
If Step 2 successfully trimmed enough mental context and the task is very close to completion, proceed with the task but adopt a strict "no further research" mode unless absolutely critical. Focus entirely on execution output and wrap up quickly.
