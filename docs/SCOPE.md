# Scope and Boundaries

## Initial product scope

The initial product is a locally deployable OTA platform for one physical
NUCLEO-H755ZI-Q plus simulated fleet members. It includes:

- Cortex-M7 firmware using Zephyr and MCUboot, subject to the feasibility gate.
- Signed full-image updates delivered over wired Ethernet and HTTPS.
- Trial boot, application health checks, confirmation, and rollback.
- Security counter policy for downgrade resistance.
- FastAPI control plane, PostgreSQL persistence, and abstract artifact storage.
- Device enrollment, releases, deployments, waves, telemetry, and audit events.
- React/TypeScript operator dashboard with role-based access.
- Offline development signing tool and non-production development credentials.
- Host, integration, hardware-in-the-loop, and fault-injection testing.

## Planned extensions

These are intentionally sequenced after the Ethernet end-to-end gate:

1. Wi-Fi through an external module and a transport adapter.
2. Cortex-M4 health supervision and explicit inter-core contracts.
3. Coordinated multi-image updates if hardware and MCUboot evidence supports them.
4. UDS over DoIP, reusing the same update policy and image service.
5. Optional UDS over CAN-FD after adding an external transceiver.
6. Uptane/TUF-inspired role separation and metadata after the base security model is proven.

## Out of scope for the first release

- Formal AUTOSAR Classic conformance or commercial AUTOSAR tooling.
- ISO 26262, IEC 61508, or MISRA compliance claims.
- Delta updates, firmware-at-rest encryption, and peer-to-peer distribution.
- Production certificate authority, hardware security module, or secure element provisioning.
- High availability, multi-region cloud deployment, and commercial-scale load targets.
- A third recovery image without verified external nonvolatile storage.
- Atomic M7/M4 bundle activation before a supported mechanism is demonstrated.

## Constraints

- Hardware starts with one NUCLEO-H755ZI-Q and its onboard Ethernet interface.
- Flash geometry, partition sizes, board identifiers, and exact Zephyr support are
  `TBD` until the Phase 0 spike records authoritative evidence.
- Private keys and production-like secrets must not be committed.
- The operator UI is not in the device's critical update path.
- A backend outage must not cause an endless trial-boot loop.

## Feasibility gate

Zephyr and MCUboot remain the preferred stack only if the spike demonstrates:

1. A supported board/CPU target and repeatable flash/debug workflow.
2. Working Ethernet on Cortex-M7.
3. MCUboot launching a signed application on physical hardware.
4. A partition layout that fits two measured images with explicit margin.
5. Trial and revert behavior compatible with the selected flash strategy.

Failure of a gate triggers an ADR comparing external staging storage with the
fallback stack: STM32Cube, FreeRTOS, and MCUboot standalone.
