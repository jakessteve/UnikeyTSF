---
description: PRD Architect — structure a complete Product Requirements Document from scratch
---

# SKILL: PRD ARCHITECT

**Trigger:** When @ba needs to create a new PRD, restructure an existing one, or convert raw ideas into formal requirements.

## When to Use
- Converting brainstorm/chat/bullet-points into structured PRD.
- Starting a new project phase needing formal requirements.
- Running the `/idea-to-prd` workflow.

## Process
1. Gather raw input (idea, brief, user request).
2. Apply the PRD structure template (see `references/prd-template.md`).
3. Validate with Quality Checklist before submitting for review.

## Quality Checklist
- [ ] Every feature has `As a... I want... so that...` user story
- [ ] Every feature has testable Acceptance Criteria
- [ ] Features split into MVP (Phase 1) vs Nice-to-have (Phase 2+)
- [ ] No vague requirements — each item specific and measurable
- [ ] Error/edge cases addressed (use `requirement-interviewer` skill)
- [ ] Timeline realistic with clear owners
- [ ] Risk register has ≥ 3 risks
- [ ] Open questions listed with assigned owners

## Phase Decomposition
- **> 5 Core Features** → split into sub-phases (1a, 1b)
- **Single feature > 8 files** → break into smaller stories
- Always ship smallest useful increment first
- Each phase independently deployable and testable
