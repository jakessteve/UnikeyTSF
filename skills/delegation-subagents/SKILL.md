---
name: delegation-subagents
description: Delegate bounded work to Codex sub-agents using clear ownership, scope, and reporting. Use when the user explicitly asks for sub-agents, delegation, or parallel work and Codex needs a safe, native way to split research, review, or implementation tasks.
---

# Delegation With Sub-Agents

This skill replaces Antigravity's external spawn workflow with Codex-native delegation.

## When To Delegate

- the user explicitly asks for sub-agents or parallel work
- there are independent read-only questions that can be answered in parallel
- there are independent implementation slices with disjoint write sets

## When Not To Delegate

- the next main-thread action is blocked on the answer and local work is faster
- the scope is so coupled that splitting it would create merge risk or coordination waste
- the task is small enough that prompt overhead is larger than the work

## Agent Selection

- `explorer`: targeted codebase questions, read-only analysis
- `worker`: bounded implementation, test repair, or isolated refactor

## Required Brief

Every subtask must specify:

- goal
- owned files or owned module
- off-limits files
- verification command or proof
- expected report format

## Integration Standard

- review returned changes quickly
- check ownership compliance
- run the necessary verification
- integrate before spawning another overlapping subtask
