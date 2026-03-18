---
description: Always On — MVP-first, mobile-first, ruthless prioritization, and continuous improvement mindsets
---

# RULE: ENGINEERING MINDSET (Combined)

Core engineering philosophies that guide all decision-making.

---

## 1. MVP-First Mindset

Ship the smallest useful increment first. Then iterate.
- Identify the core value proposition → build ONLY that for Phase 1.
- Defer "nice-to-have" features to Phase 2+.
- A working MVP > a perfect but unfinished product.
- Apply ruthless prioritization (§3) to feature lists.

---

## 2. Mobile-First Mindset

Design and implement for mobile (375px) first, then scale up.
- Touch targets: minimum 44×44px.
- Test at 375px, 768px, 1024px breakpoints.
- Avoid hover-only interactions — they don't exist on mobile.
- Performance budget: < 3s first meaningful paint on 3G.

---

## 3. Ruthless Prioritization

When overwhelmed with tasks, apply this filtering:
1. **Impact vs Effort matrix:** High impact + Low effort = do first.
2. **User-facing > Internal:** Features users see > refactoring they don't.
3. **Blockers first:** Unblock other work before starting new work.
4. **Time-box exploration:** Task-aware research limits before making a decision:
 | Task | Max Research |
 |---|---|
 | Bug fix | 10 min |
 | Feature (existing pattern) | 20 min |
 | Architecture decision | 45 min |
 | Research task | 60 min |
5. **Cut scope, not quality:** Reduce feature scope rather than shipping buggy code.

---

## 4. Continuous Improvement (Kaizen)

**After every task** (not just phases), briefly note ONE improvement:

```markdown
## Kaizen Note
**What went well:** [One thing]
**What could improve:** [One thing]
**Action:** [Specific, small improvement]
```

- Keep it brief (2-3 lines). Focus on actionable items, not vague observations.
- If improvement requires a code/skill change, note it for @pm to schedule.
- After each **phase**, run `/retrospective` workflow for deeper analysis.
- Identify recurring patterns → propose rule/skill/workflow changes.
- Track improvements in `.hc/improvements/`.
- Never auto-modify `.agent/` files — always get User approval.
