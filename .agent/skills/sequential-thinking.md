---
description: Sequential Thinking - decompose complex logic into verifiable thought chains before implementation
---

# SKILL: Sequential Thinking

**Trigger:** Before implementing complex calculations, debugging non-obvious logic, or designing multi-step algorithms — especially for computations involving multiple interacting variables, order-dependent operations, or domain-specific rule engines.

---

## When to Use

| Scenario | Use Sequential Thinking? | Alternative |
|---|---|---|
| Algorithmic complexity (within one function) | Yes | — |
| Multi-variable calculations (>3 interacting variables) | Yes | — |
| Debugging non-obvious logic failures | Yes | After `systematic-debugging` Phase 1-2 |
| Task-level complexity (multiple files/systems) | No | Use `task-decomposition` skill |
| Both algorithmic AND system complexity | Decompose first, then think | `task-decomposition` → then sequential thinking per sub-task |
| Simple, well-understood operations | No | Just code it |

---

## The 5-Step Process

### Step 1: Frame the Problem
Before calling the tool, write a clear problem statement:
```markdown
## Problem
**What:** [What computation/logic needs to be implemented?]
**Inputs:** [What data goes in?]
**Expected output:** [What should come out?]
**Known rules:** [What constraints or domain rules apply?]
**Verification:** [How will I know the solution is correct?]
```

### Step 2: Call Sequential Thinking Tool
```
Tool: mcp_sequential-thinking_sequentialthinking
thought: [Your first analytical step]
nextThoughtNeeded: true
thoughtNumber: 1
totalThoughts: [estimated — can adjust]
```

**Tips for effective thought chains:**
- Start with the simplest sub-problem and build up.
- Each thought should produce a **testable intermediate result**.
- When stuck, use `isRevision: true` to reconsider a previous step.
- When a thought branches, use `branchFromThought` and `branchId` to explore alternatives.

### Step 3: Generate Hypothesis
After enough thought steps, form a concrete hypothesis:
```
thought: "Based on steps 1-N, my hypothesis is: [specific algorithm/approach]"
```

### Step 4: Verify Against Known Rules
Before writing code:
1. Check `docs/` reference files for domain rules.
2. Check `src/data/` lookup tables for correctness.
3. Cross-reference with existing engine implementations.
4. If the hypothesis contradicts a trusted source, **the source wins** (Rule `anti-patterns-core.md`).

### Step 5: Implement Only After Verification
- Write code ONLY after the thinking chain confirms a validated approach.
- Include the thought chain summary as a code comment for future maintainers.

---

## Domain-Specific Triggers
These types of computations REQUIRE sequential thinking before implementation:

| Category | Example Computation | Why |
|---|---|---|
| Multi-variable algorithms | Calculations with 3+ interacting variables | Complex state interactions |
| Order-dependent operations | Pipelines where step order changes output | Subtle sequencing bugs |
| Lookup-with-exceptions | Mapping tables with edge case overrides | Exception paths hide bugs |
| Precision-critical math | Floating-point, calendar, or financial calculations | Rounding/boundary errors |
| Layered system construction | Multi-layer data structures (e.g., grids, trees) | Inter-layer dependencies |
| Conditional rule engines | Business rules with nested conditions and edge cases | Combinatorial explosion |
| Pattern-based derivation | Input → derived output via historical or mathematical patterns | Pattern-based with variants |

## Integration with Confidence Routing
After completing sequential thinking:
- Include a **confidence score** (Rule `decision-routing.md`) in the final thought.
- If confidence < 60%, re-enter thinking or escalate to @pm.
- Document the verified approach before proceeding to implementation.

## Rules
- **Never skip verification** — thinking without checking sources is just confident guessing.
- **Adjust thought count** — if you reach the estimated total but aren't confident, add more thoughts.
- **Branch, don't force** — if a thought chain hits a wall, branch from an earlier point rather than pushing forward.
- **Keep thoughts focused** — each thought should advance the solution, not restate the problem.
