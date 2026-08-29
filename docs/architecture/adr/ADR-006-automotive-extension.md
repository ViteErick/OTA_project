# ADR-006: Apply AUTOSAR Concepts and Add UDS Later

- **Status:** Accepted
- **Date:** 2026-08-28

## Context

Automotive relevance is valuable, but a credible AUTOSAR Classic implementation
depends on substantial specifications, generators, integration assets, and often
commercial tooling. Recreating a partial stack would distract from OTA fundamentals
and would not establish conformance.

## Decision

Use automotive engineering concepts now: layered service boundaries, explicit state
machines, diagnostic reason codes, requirements traceability, coding discipline, and
fault-oriented verification. After the HTTPS OTA path is stable, add UDS over DoIP as
an adapter to the same image and security services. CAN-FD/ISO-TP remains optional.

## Consequences

- The project can explain similarities to AUTOSAR without making a false claim.
- UDS exercises an industry-relevant programming path while avoiding duplicate flash logic.
- Full AUTOSAR Classic configuration and conformance are outside project scope.

## Alternatives

- AUTOSAR Classic from the start: rejected due to cost, scope, and weak learning return.
- No automotive code: rejected because UDS/DoIP adds relevant interview evidence.
