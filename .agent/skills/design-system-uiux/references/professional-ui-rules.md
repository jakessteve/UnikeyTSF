# Professional UI Rules

## Icons & Visual Elements

| Rule | Standard | Avoid |
|------|----------|-------|
| **No Emoji as Icons** | Use SVG icons (Heroicons, Lucide, Simple Icons) | Using emojis ( ) for navigation, settings, or system controls |
| **Vector-Only Assets** | Use SVG or vector icons that scale cleanly and support theming | Raster PNG icons that blur or pixelate |
| **Stable Hover States** | Use color, opacity, or elevation transitions without changing layout bounds | Scale transforms that shift layout or cause visual jitter |
| **Correct Brand Logos** | Use official brand assets with correct proportions and clear space | Guessing logo paths, recoloring unofficially |
| **Consistent Icon Sizing** | Define icon sizes as design tokens (e.g., icon-sm=16, icon-md=24, icon-lg=32) | Mixing arbitrary values like 20/24/28px randomly |
| **Stroke Consistency** | Use consistent stroke width within the same visual layer (1.5px or 2px) | Mixing thick and thin stroke styles arbitrarily |
| **Filled vs Outline** | Use one icon style per hierarchy level | Mixing filled and outline icons at the same hierarchy level |
| **Touch Target Minimum** | Min 44×44px interactive area (use hitSlop/padding if icon is smaller) | Small icons without expanded tap area |
| **Icon Alignment** | Align icons to text baseline with consistent padding | Misaligned icons or inconsistent spacing |
| **Icon Contrast** | Follow WCAG: 4.5:1 for small elements, 3:1 for larger UI glyphs | Low-contrast icons that blend into background |

## Interaction & Cursor

| Do | Don't |
|----|-------|
| `cursor-pointer` on all clickable elements | Default cursor on interactive elements |
| Visual hover feedback (color, shadow, border) | No hover indication |
| `transition-colors duration-200` | Instant changes or > 500ms |
| Visible focus rings (`focus:ring-2`) | Hidden/removed focus outlines |
| Disable button + show spinner during async | Button clickable while loading |
| Provide clear pressed feedback within 80–150ms | No visual response on tap |
| Keep micro-interactions 150–300ms with natural easing | Instant transitions or slow animations (>500ms) |
| Ensure focus order matches visual order | Unlabeled controls or confusing focus traversal |
| Use disabled semantics + reduced emphasis | Controls that look tappable but do nothing |
| Keep one primary gesture per region | Overlapping gestures causing accidental actions |

## Light/Dark Mode Contrast

| Do | Don't |
|----|-------|
| Glass card: `bg-white/80+` in light mode | `bg-white/10` (invisible) |
| Body text: `slate-900` (#0F172A) | `slate-400` for body text |
| Muted text: `slate-600` minimum | gray-400 or lighter |
| Borders: `border-gray-200` in light mode | `border-white/10` (invisible) |
| Keep cards/surfaces clearly separated from background | Overly transparent surfaces that blur hierarchy |
| Maintain text contrast ≥4.5:1 in both modes | Dark mode text that blends into background |
| Ensure borders/dividers visible in both themes | Theme-specific borders disappearing |
| Use semantic color tokens per theme | Hardcoded per-screen hex values |
| Modal scrim 40–60% black for legibility | Weak scrim with background competing |

## Layout & Spacing

| Do | Don't |
|----|-------|
| Same `max-w-6xl` or `max-w-7xl` throughout | Mix container widths |
| Account for fixed navbar height in padding | Content hidden behind fixed elements |
| `aspect-ratio` or fixed height for async content | Layout shift on content load |
| `min-h-dvh` for full-screen mobile | `h-screen` (broken on mobile) |
| Respect top/bottom safe areas for fixed elements | Placing UI under notch, status bar, gesture area |
| Use consistent 4/8px spacing rhythm | Random spacing increments with no rhythm |
| Keep long-form text readable (avoid edge-to-edge) | Full-width long text on wide screens |
| Increase horizontal insets on larger widths | Same narrow gutter on all device sizes |
| Add bottom/top insets so lists aren't hidden by fixed bars | Scroll content obscured by sticky headers/footers |

## Anti-Patterns

- Use raw colors like `#333`, `rgb(0,0,0)` — use semantic tokens
- Create one-off utility classes — compose from existing primitives
- Forget dark mode — every visual change needs a dark variant
- Use `!important` — restructure specificity instead
- Skip browser verification — always visually confirm changes
- Add animations without `prefers-reduced-motion` consideration
- Use emojis as icons — use SVG icon libraries
- Leave clickable elements without `cursor-pointer`
- Use `z-[9999]` — follow the z-index scale
- Use placeholder as the only form label
- Animate `width`/`height` — use `transform` and `opacity` only
- Show instant state changes (0ms transitions)
- Mix icon styles (filled + outlined) at the same hierarchy level
