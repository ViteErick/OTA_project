# ADR-001: Prefer Zephyr and MCUboot

- **Status:** Proposed
- **Date:** 2026-08-28

## Context

The project needs networking, TLS, test infrastructure, a maintained secure
bootloader, and portability beyond one vendor. The board is dual-core and its exact
upstream support must be verified against the selected Zephyr revision.

## Decision

Prefer Zephyr for the Cortex-M7 application and MCUboot for signed image validation,
trial boot, confirmation, and revert behavior. Use sysbuild if the verified board
support and upgrade mode permit it.

## Consequences

- The project learns portable RTOS concepts, devicetree, Kconfig, and upstream tools.
- Existing MCUboot image formats and security behavior replace custom boot crypto.
- The initial learning curve is higher than vendor-generated STM32Cube code.
- Board, Ethernet, flash, and MCUboot assumptions must pass a physical spike.

## Alternatives

- STM32Cube plus FreeRTOS and MCUboot standalone: fallback if the spike blocks progress.
- ST secure update examples: more vendor-specific and less portable.
- Custom bootloader: rejected because it adds security risk without project value.

## Acceptance evidence

Change to `Accepted` only after repeatable MCUboot signed boot, Ethernet operation,
trial/revert, and a measured two-image layout are demonstrated on the board.
