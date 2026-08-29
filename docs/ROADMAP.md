# Roadmap and Delivery Gates

Progress is gate-based rather than date-based. A phase finishes only when its
evidence is retained and its exit criteria pass.

## Phase 0: feasibility and baseline

**Deliverables**

- Requirements, architecture, threat model, test strategy, traceability, and ADRs.
- Reproducible development environment definition.
- Board target, flash geometry, Ethernet, sysbuild, and MCUboot spike evidence.

**Exit gate**

- Signed image boots on hardware through MCUboot.
- Ethernet sample communicates reliably.
- Final devicetree and linker map support a documented update layout.
- ADR-003 changes from `Proposed` to `Accepted` or is superseded by a fallback ADR.

## Phase 1: local secure update

**Exit gate**

- Valid signed image enters trial and can be confirmed.
- Unconfirmed image reverts after reset.
- Invalid signature, wrong target, and disallowed security counter are rejected.
- Power cycling preserves a bootable image.

## Phase 2: Ethernet end-to-end update

**Exit gate**

- Device discovers, streams, schedules, boots, confirms, and reports a release.
- TLS trust failure, truncated artifact, disconnect, reset, and unavailable backend
  have specified and demonstrated recovery behavior.

## Phase 3: fleet control plane

**Exit gate**

- Releases and deployments are auditable and idempotent.
- Canary waves can pause, resume, cancel, and auto-pause on failure thresholds.
- Fleet simulator demonstrates mixed update outcomes.

## Phase 4: operator dashboard

**Exit gate**

- Authorized users can inspect and control releases and deployments.
- Device and deployment timelines explain every significant transition.
- Accessibility, responsive layout, error states, and role boundaries are tested.

## Phase 5: verification hardening

**Exit gate**

- CI covers firmware build/size, host tests, backend, frontend, analysis, and SBOM.
- HIL executes the critical happy path and defined failure injections repeatedly.
- Critical requirements have design and test traceability with retained evidence.

## Phase 6: Wi-Fi transport

**Exit gate**

- The same OTA policy and backend contracts pass over Ethernet and Wi-Fi.
- Provisioning, reconnect, and interruption behavior are tested.

## Phase 7: dual-core and automotive extension

**Exit gate**

- M4 ownership, IPC, cache, startup, watchdog, and reset contracts are measured.
- Any multi-image activation has demonstrated compatibility and atomic rollback.
- UDS over DoIP invokes the shared image/security service rather than duplicating it.

## Evidence retained per gate

- Build identity and dependency versions.
- Commands or automation used to reproduce the result.
- UART/service logs and test reports.
- Firmware flash/RAM sizes and timing measurements.
- Deviations, open risks, and the ADR that accepts each tradeoff.
