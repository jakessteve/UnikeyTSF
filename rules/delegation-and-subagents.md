# Rule: Delegation And Sub-Agents

- Delegation is a tool, not a ritual.
- Use sub-agents only when the user explicitly asks for delegation, parallel work, or sub-agents.
- Keep urgent blocking work local unless a delegated result can arrive in parallel while the main thread makes progress.
- Use `explorer` for read-only questions and `worker` for bounded implementation.
- Assign ownership clearly. Two workers should not write the same file.
- Require a short report with changed files, verification, and unresolved risks.
- Integrate delegated output before asking for more delegation.
