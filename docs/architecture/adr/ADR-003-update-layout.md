# ADR-003: Prefer a Signed Two-Slot Update Layout

- **Status:** Proposed
- **Date:** 2026-08-28

## Context

Rollback requires retaining a bootable image while a candidate is installed. The
device has internal flash, but geometry, erase alignment, MCUboot overhead, selected
upgrade mode, and application size have not yet been measured. External staging
storage is not assumed to exist on the board.

## Decision

Prefer two signed internal image slots with MCUboot trial/confirm/revert semantics,
provided the Phase 0 spike proves sufficient space and a supported update mode.
Addresses, sizes, and swap strategy remain `TBD`.

## Consequences

- Rollback can operate without mandatory external storage if the layout fits.
- Firmware size becomes a hard product budget enforced in CI.
- "A/B" is not used as a claim about physical bank swapping; exact MCUboot behavior
  follows the selected and tested mode.

## Alternatives

- External flash for staging or recovery: acceptable if internal slots lack margin.
- Overwrite-only update: rejected for the first objective because it weakens rollback.
- Third recovery slot: deferred until external storage and need are demonstrated.

## Acceptance evidence

Reference manual geometry, final devicetree, linker maps, signed image sizes, trailer
overhead, power/reset tests, and trial/revert logs are required before acceptance.
