---
description: React Patterns - hooks, render optimization, composition, state management, and anti-patterns
---

# SKILL: React Patterns

**Trigger:** When @dev builds React components or optimizes React performance. Especially for React 19 patterns.

---

## When to Use
- Building new React components or pages.
- Optimizing rendering performance (jank, unnecessary re-renders).
- Choosing state management strategy for a feature.
- Refactoring components for reusability.
- Reviewing code for React anti-patterns.

---

## Pattern Reference

### 1. Custom Hooks (Extract Reusable Logic)
```typescript
// Good custom hook: encapsulates logic, returns stable interface
function useLunarDate(solarDate: Date) {
 const [lunarDate, setLunarDate] = useState<LunarDate | null>(null);
 const [loading, setLoading] = useState(true);

 useEffect(() => {
 setLoading(true);
 const result = convertToLunar(solarDate);
 setLunarDate(result);
 setLoading(false);
 }, [solarDate]);

 return { lunarDate, loading };
}
```

**Custom hook naming:** Always prefix with `use`. One hook = one concern.

### 2. Render Optimization

| Problem | Solution | When to Apply |
|---|---|---|
| Re-renders on every parent render | `React.memo()` wrapper | Component renders same output for same props |
| Expensive calculations re-computing | `useMemo(() => compute(data), [data])` | Computation is measurably slow (>1ms) |
| Callback references changing | `useCallback(fn, [deps])` | Callback is passed to memoized child |
| Large list re-renders | Virtual scrolling (`@tanstack/virtual`) | List has >100 items |
| Context causing tree re-renders | Split context by update frequency | Some consumers update often, others rarely |

> **Warning:** Don't prematurely optimize. Only add `memo`/`useMemo`/`useCallback` when you've measured a performance problem.

### 3. Composition Over Inheritance
```typescript
// Composition: flexible, reusable
function Card({ header, children, footer }: CardProps) {
 return (
 <div className="card">
 <div className="card-header">{header}</div>
 <div className="card-body">{children}</div>
 {footer && <div className="card-footer">{footer}</div>}
 </div>
 );
}

// Usage: compose flexible layouts
<Card header={<h2>Chart</h2>} footer={<ExportButton />}>
 <TuViChart data={chartData} />
</Card>
```

### 4. Error Boundaries
```typescript
// React 19: use() can suspend, but error boundaries still need class components
class ErrorBoundary extends React.Component<{ fallback: ReactNode }> {
 state = { hasError: false, error: null as Error | null };

 static getDerivedStateFromError(error: Error) {
 return { hasError: true, error };
 }

 componentDidCatch(error: Error, info: ErrorInfo) {
 logError(error, info.componentStack);
 }

 render() {
 if (this.state.hasError) return this.props.fallback;
 return this.props.children;
 }
}

// Usage: wrap feature areas, not the entire app
<ErrorBoundary fallback={<ChartError />}>
 <TuViChart />
</ErrorBoundary>
```

### 5. State Management Decision Tree
```
Is state used by only one component?
 └─ Yes → useState

Is state shared between siblings?
 └─ Yes → Lift state up to parent

Is state needed by many distant components?
 └─ Yes → useContext (for low-frequency updates)

Is state complex with many transitions?
 └─ Yes → useReducer

Is state server-cached data?
 └─ Yes → React Query / SWR / use() (React 19)

Is state URL-driven (filters, pagination)?
 └─ Yes → URL search params (react-router)
```

### 6. React 19 Patterns
```typescript
// use() for promises (replaces many useEffect patterns)
function ChartView({ chartPromise }) {
 const data = use(chartPromise); // Suspends until resolved
 return <Chart data={data} />;
}

// Server components (if using Next.js/RSC)
// Prefer server components for data-fetching pages
// Client components for interactivity (forms, charts, animations)
```

---

## Component Structure Standard
```
src/components/FeatureName/
├── FeatureName.tsx # Main component
├── FeatureName.test.tsx # Tests
├── useFeatureName.ts # Custom hook (if needed)
├── FeatureName.types.ts # Types (if complex)
└── index.ts # Public exports
```

## Anti-Patterns

| Anti-Pattern | Why It's Bad | Fix |
|---|---|---|
| Business logic inside components | Hard to test, violates SRP | Extract to hooks or utils |
| `useEffect` for derived state | Unnecessary re-renders, stale state | Use `useMemo` |
| Index as key in dynamic lists | Causes incorrect re-renders, bugs | Use stable unique ID |
| Prop drilling >3 levels | Brittle, hard to maintain | Context or composition |
| Mutating state directly | Silent bugs, React won't re-render | Always return new references |
| `useEffect` as event handler | Runs on every render, not on action | Use plain event handler |
| Giant components (>200 lines) | Hard to read, test, and maintain | Extract sub-components |

## Rules
- **One component per file.** Max ~200 lines per component.
- **Extract logic into hooks.** Components should be primarily JSX.
- **Measure before optimizing.** React DevTools Profiler, not guessing.
- **Use `Context7` skill** to check React 19 API changes before using new patterns.
- **Error boundaries per feature area** — not one global boundary.
