# Codex Model Routing

This replaces Antigravity's Gemini/Opus/Sonnet matrix with Codex-native guidance.

## Default Guidance

- `gpt-5.4`: deep planning, architecture, difficult reasoning, synthesis
- `gpt-5.4-mini`: balanced implementation and analysis
- `gpt-5.3-codex`: focused coding and code transformation
- `gpt-5.1-codex-mini`: narrow mechanical or repetitive coding tasks

## Delegation Guidance

- Use `explorer` with a stronger model for read-only codebase questions.
- Use `worker` with a coding-oriented model for bounded implementation.
- Prefer keeping work local when the answer is immediately blocking.
