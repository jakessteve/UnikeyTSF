# Rules Manifest -- Role-Scoped Loading Index

> Agents: load ONLY the rules listed for your role. Do NOT pre-load all rule files.
> Orchestrator sessions (@pm interactive) load all rules. CLI workers load only their role's subset.

## Universal Rules (all roles, always loaded)
| Rule | Path | Size |
|---|---|---|
| anti-patterns-core | rules/anti-patterns-core.md | ~7 KB |
| engineering-mindset | rules/engineering-mindset.md | 2.1 KB |
| context-budget | rules/context-budget.md | 2.3 KB |

## Conditional Rules (loaded on trigger only)
| Rule | Path | Trigger | Size |
|---|---|---|---|
| anti-patterns-swarm | rules/anti-patterns-swarm.md | `/swarm-execute`, parallel multi-agent | ~4.5 KB |

## Role-Scoped Rules

### @dev-fe (+ universal)
| Rule | Path | Size |
|---|---|---|
| code-standards | rules/code-standards.md | 2.2 KB |
| git-hygiene | rules/git-hygiene.md | 1.4 KB |
| performance-budget | rules/performance-budget.md | 1.0 KB |
| anti-slop-ui | rules/anti-slop-ui.md | 1.5 KB |

### @dev-be (+ universal)
| Rule | Path | Size |
|---|---|---|
| backend-standards | rules/backend-standards.md | ~4.5 KB |
| code-standards | rules/code-standards.md | 2.2 KB |
| git-hygiene | rules/git-hygiene.md | 1.4 KB |
| error-handling-standards | rules/error-handling-standards.md | 1.4 KB |
| security-standards | rules/security-standards.md | 1.5 KB |
| api-contract-first | rules/api-contract-first.md | 1.0 KB |

### @qc (+ universal)
| Rule | Path | Size |
|---|---|---|
| code-standards | rules/code-standards.md | 2.2 KB |
| execution-protocol | rules/execution-protocol.md | ~6 KB |

### @pm (+ universal)
| Rule | Path | Size |
|---|---|---|
| execution-protocol | rules/execution-protocol.md | ~6 KB |
| decision-routing | rules/decision-routing.md | ~4.5 KB |
| no-code-boundary | rules/no-code-boundary.md | 3.1 KB |

### @sa (+ universal)
| Rule | Path | Size |
|---|---|---|
| code-standards | rules/code-standards.md | 2.2 KB |
| scalable-folder-structure | rules/scalable-folder-structure.md | 4.5 KB |
| api-contract-first | rules/api-contract-first.md | 1.0 KB |
| documentation-standards | rules/documentation-standards.md | 3.1 KB |

### @designer (+ universal)
| Rule | Path | Size |
|---|---|---|
| ui-design-system | rules/ui-design-system.md | 2.5 KB |
| a11y-standards | rules/a11y-standards.md | 2.7 KB |
| performance-budget | rules/performance-budget.md | 1.0 KB |
| anti-slop-ui | rules/anti-slop-ui.md | 1.5 KB |

### @devops (+ universal)
| Rule | Path | Size |
|---|---|---|
| security-standards | rules/security-standards.md | 1.5 KB |
| code-standards | rules/code-standards.md | 2.2 KB |
| git-hygiene | rules/git-hygiene.md | 1.4 KB |
| documentation-standards | rules/documentation-standards.md | 3.1 KB |

### @biz (+ universal)
| Rule | Path | Size |
|---|---|---|
| documentation-standards | rules/documentation-standards.md | 3.1 KB |

### @ba (+ universal)
| Rule | Path | Size |
|---|---|---|
| agile-user-stories | rules/agile-user-stories.md | 3.1 KB |
| documentation-standards | rules/documentation-standards.md | 3.1 KB |

### @whitehat-hacker (+ universal)
| Rule | Path | Size |
|---|---|---|
| security-standards | rules/security-standards.md | 1.5 KB |

### @user-tester (+ universal)
| Rule | Path | Size |
|---|---|---|
| user-feedback-format | rules/user-feedback-format.md | 4.1 KB |

## Orchestrator-Only Rules (interactive @pm sessions, not workers)
| Rule | Path | Size | Note |
|---|---|---|---|
| model-routing | rules/model-routing.md | 5.5 KB | Model selection for delegation |
| dialectical-development | rules/dialectical-development.md | 2.6 KB | Novelty gate |
| autonomous-tooling | rules/autonomous-tooling.md | 2.5 KB | Tool usage patterns |
| spawn-governance | rules/spawn-governance.md | 2.8 KB | Pre-spawn validation + timeout handling |
