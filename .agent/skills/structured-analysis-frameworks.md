---
description: Structured Analysis Frameworks — MECE decomposition, Issue Trees, Pre-Mortem scenarios, and Weighted Decision Matrix for complex decisions
---

# SKILL: Structured Analysis Frameworks

**Trigger:** When @pm or @sa faces a complex decision with multiple variables, competing options, or need for exhaustive decomposition. Complements `critical-thinking-models` (fast gate) with deeper structured analysis when needed.

---

## When to Use
- **MECE:** Breaking down a complex problem into non-overlapping, exhaustive categories.
- **Issue Tree:** Diagnosing root causes or mapping all solution paths for a problem.
- **Pre-Mortem:** Deep adversarial analysis of a plan BEFORE implementation (deeper than the quick pre-mortem in `critical-thinking-models` §3).
- **Weighted Matrix:** Choosing between 3+ competing options with multiple criteria.

## When to SKIP
- Trivial or Small tasks (see `decision-routing.md`).
- Only 1-2 obvious options → use `critical-thinking-models` quick gate instead.
- Already classified as Cynefin **Clear** → just execute.

---

## Framework 1: MECE Decomposition
> **M**utually **E**xclusive, **C**ollectively **E**xhaustive

Ensures no overlap and no gaps when breaking down a problem.

### Process
1. **Define the top-level question** in one sentence.
2. **Break into 2-5 categories** that are:
 - **Mutually Exclusive:** No item belongs to two categories.
 - **Collectively Exhaustive:** All possibilities are covered.
3. **Test completeness:** Ask "Is there anything that doesn't fit any category?"
4. **Test exclusivity:** Ask "Does any item fit multiple categories?"

### Template
```markdown
### MECE Decomposition — [Topic]

**Top-level question:** [What are we decomposing?]

| Category | Items | Notes |
|---|---|---|
| [Category A] | [items that belong here] | [why this grouping] |
| [Category B] | [items] | [notes] |
| [Category C] | [items] | [notes] |

**Completeness check:** All items accounted for / Gap: [what's missing]
**Exclusivity check:** No overlaps / Overlap: [where items fit multiple categories]
```

### Common MECE Patterns for Software
| Problem Type | MECE Split |
|---|---|
| User journey | Awareness → Acquisition → Activation → Retention → Revenue → Referral (AARRR) |
| Code architecture | Frontend / Backend / Infrastructure / Data / Security |
| Bug diagnosis | Input / Processing / Output / Environment |
| Performance | Network / Rendering / Computation / Memory / I/O |
| Feature scope | Must-have / Should-have / Could-have / Won't-have (MoSCoW) |

---

## Framework 2: Issue Tree
> Systematic decomposition of a problem into sub-problems, then sub-sub-problems.

### Process
1. **Root node:** State the problem as a yes/no question or a "how" question.
2. **Level 1 branches:** Break into 2-4 MECE sub-questions.
3. **Level 2 branches:** Break each sub-question further (max 3 levels deep).
4. **Leaves:** Each leaf should be a testable hypothesis or actionable item.

### Template
```markdown
### Issue Tree — [Problem Statement]

[Root Question]
├── [Branch 1: Sub-question]
│ ├── [Leaf 1a: Hypothesis / Action]
│ └── [Leaf 1b: Hypothesis / Action]
├── [Branch 2: Sub-question]
│ ├── [Leaf 2a]
│ └── [Leaf 2b]
└── [Branch 3: Sub-question]
 ├── [Leaf 3a]
 └── [Leaf 3b]

**Prioritized investigation order:** [Leaf X] → [Leaf Y] → [Leaf Z]
```

### When to Use Issue Trees
| Scenario | Root Question Format |
|---|---|
| Bug diagnosis | "Why is [symptom] happening?" |
| Feature design | "How can we achieve [goal]?" |
| Architecture decision | "Should we use [approach A] or [approach B]?" |
| Performance issue | "What is causing [slowness/latency]?" |

---

## Framework 3: Pre-Mortem (Deep)
> Imagine the project has FAILED. Work backwards to find why.

This is the **deep version** — for significant decisions. For quick checks, use `critical-thinking-models` §3 (Inversion).

### Process
1. **Set the scene:** "It's [6 months from now]. The [feature/decision] has failed. It was a disaster."
2. **Individual brainstorm:** Each agent independently writes 2-3 failure scenarios.
3. **Categorize failures** using MECE: Technical / UX / Market / Organizational / External.
4. **Score** each scenario: Probability (1-5) × Impact (1-5) = Risk Score.
5. **Address top 3:** Create mitigations for the highest-scoring scenarios.

### Template
```markdown
### Pre-Mortem — [Decision/Feature Name]

**Premise:** It's [future date]. This has failed completely.

| # | Failure Scenario | Category | Probability | Impact | Risk Score | Mitigation |
|---|---|---|---|---|---|---|
| 1 | [What went wrong] | [Tech/UX/Market/Org/External] | [1-5] | [1-5] | [P×I] | [What we can do now] |
| 2 | ... | ... | ... | ... | ... | ... |
| 3 | ... | ... | ... | ... | ... | ... |

**Top Risks to Address Before Proceeding:**
1. [Highest risk score] → [Action]
2. [Second highest] → [Action]
3. [Third highest] → [Action]
```

---

## Framework 4: Weighted Decision Matrix
> Systematic comparison of options against weighted criteria.

### Process
1. **List options** (2-5 competing approaches).
2. **Define criteria** (4-7 evaluation dimensions).
3. **Assign weights** (must sum to 100%) — this is where values/priorities are made explicit.
4. **Score** each option against each criterion (1-5 scale).
5. **Calculate** weighted scores and rank.

### Template
```markdown
### Weighted Decision Matrix — [Decision Name]

**Options:** A: [name] | B: [name] | C: [name]

| Criterion | Weight | Option A | Option B | Option C |
|---|---|---|---|---|
| [Criterion 1] | [%] | [1-5] (×W=[X]) | [1-5] (×W=[X]) | [1-5] (×W=[X]) |
| [Criterion 2] | [%] | [score] | [score] | [score] |
| [Criterion 3] | [%] | [score] | [score] | [score] |
| [Criterion 4] | [%] | [score] | [score] | [score] |
| **Total** | **100%** | **[sum]** | **[sum]** | **[sum]** |

**Winner:** [Option X] with [score] — [1-sentence justification]
**Runner-up:** [Option Y] — [why it lost]
**Key differentiator:** [The criterion that swung it]
```

### Common Criteria Sets for Software Decisions
| Decision Type | Recommended Criteria (example weights) |
|---|---|
| Tech stack choice | Performance (25%), DX (20%), Ecosystem (20%), Learning curve (15%), Scalability (20%) |
| Architecture pattern | Maintainability (25%), Performance (20%), Testability (20%), Complexity (15%), Flexibility (20%) |
| Library selection | API quality (25%), Bundle size (20%), Community (20%), TypeScript support (15%), Documentation (20%) |
| Feature prioritization | User impact (30%), Dev effort (25%), Strategic fit (20%), Risk (15%), Dependencies (10%) |

---

## Integration Points
- **Input from:** `critical-thinking-models` (when deeper analysis needed), `requirement-enrichment` (complex requirements), `/party-mode` (brainstorm outputs)
- **Output to:** Auto-Delegation table, `/idea-forge`, `.hc/decisions/`
- **Complements:** `critical-thinking-models` (quick 7-model gate), `idea-validation` (DFV scoring), `red-team-ideas` (adversarial analysis)

---

## Guardrails
- **Match framework to problem:** Don't use a Weighted Matrix for a 2-option choice — that's a quick pros/cons.
- **Time-box:** MECE split ≤ 5 min, Issue Tree ≤ 10 min, Pre-Mortem ≤ 15 min, Matrix ≤ 10 min.
- **Bias check:** After completing any framework, run the Cognitive Bias Scan from `critical-thinking-models` §7.
- **Don't stack frameworks.** Use 1-2 per decision. If you need all 4, the problem is probably too big — decompose first.
