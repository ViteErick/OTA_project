# Verification Strategy

## 1. Purpose

Verification provides evidence that requirements are met and that failure behavior
is understood. Coverage percentage supports this goal but is not a substitute for
requirement, boundary, state-transition, and fault testing.

## 2. Test levels

| Level | Environment | Primary targets |
|---|---|---|
| Host unit | Native host process | Policy state machine, parsers, records, domain services |
| Zephyr unit/component | Twister-supported target | Zephyr adapters and configuration-sensitive components |
| Backend integration | FastAPI with ephemeral PostgreSQL/storage | API, persistence, idempotency, RBAC, rollout policy |
| Frontend component/E2E | Browser test environment | Workflows, permissions, error/loading states |
| Fleet simulation | Multiple software device actors | Waves, thresholds, retries, mixed outcomes |
| HIL | NUCLEO-H755ZI-Q, ST-Link, UART, network | Boot, flash, Ethernet, TLS, trial, revert, timing |
| Physical fault injection | HIL plus controlled power fixture | Torn download/state operations and recovery |

## 3. Test architecture

Policy code should depend on ports for transport, image storage, boot control, clock,
and event delivery. Host fakes provide deterministic errors and state exploration.
Hardware tests then validate assumptions that cannot be modeled reliably: flash
geometry, reset causes, MCUboot trailers, caches, Ethernet driver behavior, and timing.

## 4. Critical scenarios

| Test ID | Scenario | Expected result |
|---|---|---|
| TST-BOOT-001 | Boot correctly signed current image | Application starts and reports identity |
| TST-BOOT-002 | Boot candidate with modified byte | MCUboot rejects candidate |
| TST-BOOT-003 | Trial image never confirms | Previous image boots and rollback is reported |
| TST-BOOT-004 | Trial health passes | Image confirms exactly once |
| TST-BOOT-005 | Candidate security counter is lower | Candidate is rejected |
| TST-NET-001 | Valid HTTPS update | Stream completes within bounds |
| TST-NET-002 | TLS CA or hostname invalid | Connection fails closed; active image remains |
| TST-NET-003 | Artifact is oversized/truncated | Candidate is rejected; reason is reported |
| TST-FAULT-001 | Reset at sampled download offsets | Active image remains bootable |
| TST-FAULT-002 | Reset around pending transition | Device reaches documented old/new state, never partial boot |
| TST-FAULT-003 | Power interruption during project record write | Invalid record is detected and ignored |
| TST-PLT-001 | Duplicate device event | One logical event exists |
| TST-PLT-002 | Canary rollback threshold exceeded | Deployment auto-pauses unopened waves |
| TST-PLT-003 | Viewer attempts deployment change | Request is rejected and audited |
| TST-REC-001 | Application made non-booting | Documented factory recovery restores image |

## 5. Fault-injection points

Initial reset injection points are: before first artifact write, during each sampled
erase region, after final image byte, before pending mark, after pending mark, before
health completion, before confirmation, and after confirmation. Actual power-loss
claims require a controllable power fixture; software reset results are labeled
separately.

## 6. HIL fixture responsibilities

- Build and flash known baseline and candidate images.
- Capture UART from reset through confirmation or rollback.
- Control reset and, when hardware is available, target power.
- Trigger deployments through the API rather than internal database changes.
- Observe reported version/events and compare them with boot logs.
- Store firmware identities, timestamps, command versions, and results as artifacts.

## 7. Static and dynamic analysis

- Compiler warnings are elevated and baselined; suppressions require rationale.
- Formatting and selected clang-tidy/cppcheck checks run where compatible with Zephyr.
- Host-testable C code uses sanitizers where supported.
- Backend and frontend dependencies are scanned and an SBOM is generated.
- Secret scanning covers source, fixtures, images, and container build context.
- MISRA C:2012 rules are selected and deviations recorded; no compliance claim is made.

## 8. Resource and timing evidence

Every release build records MCUboot/application flash, static RAM, configured stacks,
and accepted margins. Every successful HIL OTA records download, validation, reboot,
first-health, and confirmation durations. Budget thresholds remain `TBD` until the
spike provides measured baselines and the partition ADR is accepted.

## 9. Entry and exit criteria

### Test entry

- Requirement and acceptance criteria exist.
- Test environment and image identities are recorded.
- Expected boot/update states are specified before execution.

### Phase exit

- All gate-critical scenarios pass repeatedly.
- No open critical/high security defect is accepted without an ADR and mitigation.
- Traceability has no orphan critical requirement.
- Failures preserve logs and produce a reproducible issue or documented limitation.

## 10. Evidence retention

CI retains machine-readable reports, build artifacts, maps, size output, SBOM, and
test logs. HIL runs additionally retain serial logs, fixture version, board identity,
network setup, and observed reset reasons. Secrets are redacted before retention.
