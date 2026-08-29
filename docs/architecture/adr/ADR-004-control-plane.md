# ADR-004: Use FastAPI and PostgreSQL for the Control Plane

- **Status:** Accepted
- **Date:** 2026-08-28

## Context

The control plane must expose clear contracts, model release/deployment state,
support retries and audit history, and remain quick to evolve while embedded work is
the primary learning focus.

## Decision

Use Python with FastAPI for versioned APIs and PostgreSQL for durable relational
state. Keep domain logic separate from HTTP, persistence, and artifact storage.
Generate OpenAPI as the dashboard/device contract.

## Consequences

- Validation, API exploration, and test setup are straightforward.
- Database migrations and concurrency behavior still require explicit engineering.
- Deployment orchestration starts in-process and moves to a worker only with evidence.

## Alternatives

NestJS and Go were viable, but add less value to the initial embedded objective. A
managed IoT platform was rejected because it would hide fleet update mechanics.
