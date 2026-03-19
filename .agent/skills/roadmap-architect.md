---
description: Roadmap Architect � decompose a project into Phases, Epics, and Sprints with timelines
---

# SKILL: ROADMAP ARCHITECT

**Trigger:** When @pm needs to plan a project roadmap, break a large scope into manageable phases, or create a sprint plan.

---

## When to Use
- Starting a new project or major initiative.
- Translating a PRD into an actionable delivery plan.
- Resizing scope after a pivot or priority change.
- Planning quarterly or monthly roadmaps.

---

## The 3-Level Decomposition

Break every project into exactly 3 levels:

### Level 1: Phases
A Phase is a **major milestone** that delivers independently deployable value.

```markdown
## Phase 1: Core MVP (Weeks 1�4)
**Goal:** [What the user gets at the end of this phase]
**Success Metric:** [How we know it's done]

## Phase 2: Enhanced Experience (Weeks 5�8)
**Goal:** [What improves]
**Success Metric:** [Measurable outcome]
```

**Rules:**
- Each phase MUST be independently deployable and testable.
- Max 3�4 phases for any project. If more ? the project is too big, split it.
- Phase 1 is always the MVP (Rule `engineering-mindset.md`).

### Level 2: Epics
An Epic is a **feature area** within a phase. Each epic contains 2�5 related stories.

```markdown
### Epic 1.1: Core Engine Module
- S-001: Solar-to-Lunar date conversion
- S-002: Solar term calculation
- S-003: Good/bad day assessment

### Epic 1.2: Calendar UI
- S-004: Month grid view
- S-005: Day detail panel
- S-006: Date picker
```

**Rules:**
- An epic with > 5 stories MUST be split.
- Each epic has a clear owner agent (@dev, @designer, etc.).
- Epics within a phase can be parallelized if they don't share dependencies.

### Level 3: Stories (Sprints)
Stories are individual **implementable tasks** sized for 1�3 day completion. Follow Rule `agile-user-stories.md` for format.

Group stories into sprints (1�2 week cycles):

```markdown
#### Sprint 1 (Week 1�2)
- [S-001] Solar-to-Lunar conversion � @dev � Size: M
- [S-004] Month grid view � @designer + @dev � Size: M

#### Sprint 2 (Week 3�4)
- [S-002] Solar terms � @dev � Size: S
- [S-003] Good/bad day � @dev � Size: M
- [S-005] Day detail panel � @designer + @dev � Size: M
```

---

## Roadmap Template

Save to `.hc/roadmap.md`:

```markdown
# Project Roadmap: [Name]
**Version:** 1.0 | **Date:** YYYY-MM-DD | **Owner:** @pm
**Status:** Planning | Active | Completed

## Timeline Overview
| Phase | Timeline | Goal | Status |
|-------|---------|------|--------|
| Phase 1: MVP | Wk 1�4 | [Goal] | ?? In Progress |
| Phase 2: Enhanced | Wk 5�8 | [Goal] | ? Not Started |

## Phase 1: [Name]
### Epic 1.1: [Name]
| Story | Owner | Size | Sprint | Status |
|-------|-------|------|--------|--------|
| S-001 | @dev | M | Sprint 1 | ? Done |
| S-002 | @dev | S | Sprint 2 | ?? In Progress |

[Repeat for each epic and phase]
```

---

## Estimation Guidelines

| Size | Story Points | Time Estimate | Files Changed |
|------|-------------|---------------|---------------|
| **S (Small)** | 1�2 | = 1 day | = 3 files |
| **M (Medium)** | 3�5 | 2�3 days | 4�8 files |
| **L (Large)** | 8+ | 4�5 days | 9+ files ? **MUST split** |

**Buffer rule:** Add 20% buffer to total estimates for unknowns and bugs.

---

## Quality Checklist
- [ ] Every phase has a clear, independently deliverable goal
- [ ] No epic has more than 5 stories
- [ ] Every story has a size estimate and owner
- [ ] Dependencies between epics are documented
- [ ] Phase 1 is the smallest useful MVP
- [ ] Total timeline includes 20% buffer
