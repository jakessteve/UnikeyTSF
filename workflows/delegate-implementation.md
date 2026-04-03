# Workflow: Delegate Implementation

Use only when the user explicitly asks for delegation or parallel work.

Use `templates/task-brief.md` to structure each worker brief.
Pair with `skills/delegation-subagents/SKILL.md`, `rules/delegation-and-subagents.md`, and `workflows/subagent-integration-checklist.md`.

1. Split the implementation into disjoint write scopes.
2. Assign each slice to a `worker` agent with clear ownership.
3. For every worker brief, specify:
   - goal
   - owned files or owned module
   - off-limits files
   - verification command or proof
   - expected report format
4. Avoid waiting unless the main thread is blocked.
5. Review, integrate, and verify each returned slice before closing the task.
