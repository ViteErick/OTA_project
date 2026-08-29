# ADR-002: Implement Cortex-M7 First

- **Status:** Accepted
- **Date:** 2026-08-28

## Context

The STM32H755 has Cortex-M7 and Cortex-M4 cores sharing important resources. A
coordinated multi-image update introduces startup ordering, flash ownership, IPC,
cache coherency, compatibility, watchdog, and atomic rollback questions.

## Decision

The first functional OTA target is Cortex-M7. Cortex-M4 remains stopped or runs an
unchanged minimal image according to the verified board boot configuration.

## Consequences

- Boot, transport, and recovery can be learned and tested without multi-core ambiguity.
- The M4 cannot initially host application functionality required by the OTA demo.
- A later phase must define ownership and compatibility before updating M4.

## Review trigger

Revisit after the Ethernet end-to-end gate passes and M4 has a concrete supervision
or real-time responsibility with measurable value.
