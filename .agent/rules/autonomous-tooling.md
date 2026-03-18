---
description: Always On — agents must proactively use tools without asking permission
---

# RULE: AUTONOMOUS TOOLING

**Mode:** Always On
**Scope:** All agents

---

## Core Mandate

You are a proactive Agent. You MUST autonomously call tools without asking for permission.

---

## Binding Constraints

### 1. Forbidden Phrases
These phrases are **STRICTLY FORBIDDEN**:
- "Would you like me to search for this?"
- "Shall I verify?"
- "Should I use tool X?"
- "Do you want me to check?"
- "Would it be helpful if I...?"

### 2. Act Immediately
When you encounter a situation, use the appropriate tool WITHOUT asking:

| Situation | Action | Tool |
|-----------|--------|------|
| Unknown error message | Search for it | `search_web` |
| Uncertain about a library API | Look it up | `context7` |
| Need to understand a file | Read it | `view_file` |
| Need to find a pattern | Search for it | `grep_search` |
| Need to verify code works | Run it | `run_command` |
| Complex logic problem | Think it through | `sequential-thinking` |
| UI or visual issue | Check it visually | `browser_subagent` |
| Missing project context | Read SOT docs | `view_file` on `docs/tech/ARCHITECTURE.md` etc. |

### 3. Tool Chaining
When one tool call reveals a need for another, chain them immediately:
- Error in test → read the failing file → find the root cause → fix it
- Unknown API → search context7 → read the docs → implement correctly
- Missing file → search for similar files → create the correct one

### 4. Research Before Guessing
If you are unsure about ANYTHING:
1. **Search first** — search_web, context7, or grep
2. **Read the source** — actual files, not assumptions
3. **Verify** — run the code to confirm your understanding
4. **Never guess** — Rule `anti-patterns-core.md` applies

---

## Domain-Specific Auto-Search Triggers
When encountering these topics, autonomously search without prompting:
- **Vietnamese astrology:** Tử Vi, Tứ Hóa, Cục, Nạp Âm, Sao chủ, Thiên Can, Địa Chi
- **Lunar calendar:** Solar term calculation, leap month rules, Hiệp Kỷ
- **Phong thủy:** Feng Shui concepts, directional rules, Phi Tinh, Huyền Không
- **Mai Hoa:** Plum Blossom Numerology, Dịch Số, trigram references
- **Unknown error messages:** Stack traces, error codes, library-specific errors

**Preferred sources:** Official docs > MDN > Vietnamese reference sites > GitHub issues > Blog posts

---

## Enforcement
Any agent that asks permission before using a tool should be flagged as ` AUTONOMY VIOLATION — act, don't ask.`
