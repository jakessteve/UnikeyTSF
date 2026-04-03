---
name: project-docs-system
description: Establish or refine a lean source-of-truth documentation system for a project repository. Use when Codex needs to inspect an existing repo, determine whether canonical project docs already exist, and create or update a small set of durable project-truth documents without duplicating existing documentation.
---

# Project Docs System

Use this skill when a repository needs stable project-truth documents that help future work stay grounded and reduce context drift.

## Goals

- inspect the repository before proposing docs
- identify existing canonical docs before creating new ones
- avoid duplicating README, ADR, architecture, product, or operational docs that already serve as project truth
- choose the smallest viable canonical doc set when no coherent system exists
- update existing canonical docs instead of creating parallel truth
- treat templates, notes, and task artifacts as supporting material, not source of truth

## Document Classes

- project truth docs: canonical project overview, system map, architecture map, or update guide documents that future work should trust first
- planning or working docs: PRDs, feature briefs, implementation plans, task notes, and temporary working material
- historical or reference docs: migration notes, archived decisions, external references, and supporting background material

## Default Lean Canonical Set

- project overview
- system map or architecture map
- update guide or doc conventions

## Workflow

1. Inspect the repository's existing docs, entrypoints, and major directories first.
2. Classify what already exists into project truth, planning or working docs, and historical or reference docs.
3. Decide whether the repo already has a usable source-of-truth system.
4. If a canonical home already exists, update it instead of creating duplicate docs.
5. If the system is incomplete, add only the missing canonical docs from the lean set.
6. Record assumptions and unresolved questions inside the canonical docs rather than scattered notes.
7. Keep the document system generic, low-maintenance, and repo-appropriate.

## Pair With

- `workflows/project-docs.md`
- `templates/project-overview.md`
- `templates/system-map.md`
- `templates/update-guide.md`

## Avoid

- creating a second canonical doc set when a workable one already exists
- turning planning documents into project truth by accident
- adding heavy ownership, review cadence, or governance requirements by default

## Example Uses

- setting up a lean source-of-truth doc system in a repo that only has a README
- consolidating scattered architecture and onboarding notes into one canonical set
- updating an existing project overview and system map instead of creating duplicate docs
