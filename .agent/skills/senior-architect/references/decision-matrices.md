# Decision Matrices

## Evaluation Criteria Table
| Criteria | Option A | Option B | Option C |
|----------|----------|----------|----------|
| **Simplicity** | ? | ? | ? |
| **Scalability** | ? | ? | ? |
| **Performance** | ? | ? | ? |
| **Team Familiarity** | ? | ? | ? |
| **Ecosystem/Community** | ? | ? | ? |
| **Maintenance Cost** | ? | ? | ? |
| **Migration Effort** | ? | ? | ? |

Rate each: Strong · Acceptable · Weak

---

## SQL vs NoSQL
| Choose SQL When | Choose NoSQL When |
|----------------|-------------------|
| Data has clear relationships (FKs) | Document-shaped or hierarchical data |
| ACID transactions critical | Eventual consistency acceptable |
| Complex queries with JOINs | Simple key-value lookups |
| Schema stable and well-defined | Schema evolves rapidly |
| Reporting/analytics important | High write throughput priority |

## Monolith vs Microservices
| Choose Monolith When | Choose Microservices When |
|---------------------|--------------------------|
| Small team (< 5 devs) | Large team, clear domain boundaries |
| Early-stage product (MVP) | Mature, needs independent scaling |
| Simple deployment priority | Independent deploy cycles needed |
| Shared data model dominant | Services own their data |

## REST vs GraphQL
| Choose REST When | Choose GraphQL When |
|-----------------|---------------------|
| Simple CRUD | Multiple clients, different data needs |
| Caching critical (HTTP cache) | Avoid over/under-fetching |
| Public API for third parties | Single internal API for frontend |

## SSR vs SPA vs SSG
| SSR | SPA | SSG |
|-----|-----|-----|
| SEO critical | Rich interactivity | Mostly static content |
| First paint speed | App-like experience | Build-time data sufficient |
| Dynamic per-request data | Offline capability | Maximum performance |
| E-commerce, blogs | Dashboards, tools | Docs, marketing |
