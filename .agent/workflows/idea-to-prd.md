---
description: Raw idea to structured PRD — from brainstorm to formal requirements document
---

# Workflow: /idea-to-prd

Transform a rough idea, brainstorm, or bullet-point sketch into a structured PRD with user stories.

---

## Prerequisites
- A raw idea from the User (chat message, bullet points, sketch, or voice note summary)
- @ba agent role active

## Steps

### Step 1 — Capture the Raw Idea
Receive the User's input in any form:
- Chat message with rough requirements
- Bullet-point list of features
- A competitor link with "build something like this"
- A problem description without a solution

Save the raw input to `.temp/raw-idea-[name].md` for reference.

### Step 2 — Requirement Interviewing
Activate the `requirement-interviewer` skill:
1. Read the raw idea carefully.
2. Identify gaps — what's missing, vague, or ambiguous?
3. Ask the User **max 5 probing questions** at a time, covering:
   - Happy path clarity
   - Error handling expectations
   - Edge cases relevant to the domain
   - Target audience and primary device
   - MVP scope (what MUST be in Phase 1?)
4. Document answers in the gap analysis.
5. Repeat if needed (max 2 rounds of questions).

Save gap analysis to `.hc/research/gap-analysis-[name].md`.

### Step 3 — Structure the PRD
Activate the `prd-architect` skill:
1. Using the raw idea + interview answers, create a full PRD following the template.
2. Apply Rule `engineering-mindset.md` — classify every feature as Core vs. Nice-to-have.
3. Include all mandatory sections: Problem, Personas, Core Features, AC, Timeline, Risks.
4. Include a Phase 2+ backlog for deferred features.

Save PRD to `docs/biz/PRD.md` (or `PRD-[feature-name].md` if there are multiple).

### Step 4 — Generate User Stories
From the PRD's Core Features section:
1. Write one user story per feature following Rule `agile-user-stories.md`.
2. Each story has: user story format, acceptance criteria (happy + error + edge), sizing.
3. Group into an epic if there are 3+ related stories.

Save stories to `.hc/stories/S-[number]-[name].md`.
Save epics to `.hc/epics/E-[number]-[name].md` (if applicable).

### Step 5 — Present for Review
Create an Artifact presenting:
- PRD summary (executive summary section)
- Feature priority table (Core vs. Nice-to-have)
- User story count and sizing breakdown
- Unresolved questions (if any)

Present to User via `notify_user` for approval. User reviews and may request changes.

### Step 6 — Iterate (if needed)
If User requests changes:
1. Update the PRD in place (increment version).
2. Update affected user stories.
3. Re-present for review.

---

## Output Files
| File | Location |
|------|----------|
| Raw idea backup | `.temp/raw-idea-[name].md` |
| Gap analysis | `.hc/research/gap-analysis-[name].md` |
| PRD | `docs/biz/PRD.md` |
| User stories | `.hc/stories/S-[number]-[name].md` |
| Epics | `.hc/epics/E-[number]-[name].md` |
