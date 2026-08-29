# Learning and Reading Guide

This guide is educational, not normative. Read each topic before implementing its
phase, then replace assumptions with observations from the board and test evidence.

## 1. Start with the system problem

Read:

1. [Project vision](../PROJECT_VISION.md)
2. [Scope](../SCOPE.md)
3. [System context](../architecture/CONTEXT.md)
4. [Requirements](../requirements/SRS.md)

You should be able to explain why OTA includes boot policy, cryptographic authority,
recovery, deployment control, and observability rather than only networking.

## 2. Learn boot before transport

Read the MCUboot image format and upgrade-mode documentation from the exact source
revision selected by the project. Then study the STM32H755 reset flow, boot modes,
flash controller, erase/program constraints, option bytes, and cache behavior from
ST's authoritative manuals.

Answer with project evidence:

- Which component executes first after reset?
- What bytes are signed, and which key authorizes them?
- What makes an image pending, trial, confirmed, or reverted?
- Which behavior belongs to MCUboot and which belongs to the application?
- What remains bootable if reset occurs at each update transition?

Do not memorize assumed addresses. Produce the final DTS, linker map, signed image
size, and boot log during the spike.

## 3. Understand authenticity versus confidentiality

Read the [threat model](../security/THREAT_MODEL.md) and ADR-001/ADR-003. Be able to
distinguish:

- SHA-256 integrity from public-key authorization.
- TLS server authentication from firmware signature verification.
- Signing from encryption.
- Semantic version from a security counter.
- A public verification key from a private signing key.

Trace a modified artifact from storage to its rejection point and explain why a
compromised API still cannot create an accepted image without signing authority.

## 4. Study constrained network delivery

Learn Zephyr networking, sockets/HTTP client behavior, TLS credentials, DHCP, DNS,
SNTP, timeouts, and watchdog interaction. Focus on bounded streaming rather than
desktop assumptions.

Measure buffer sizes, stack sizes, throughput, reconnect behavior, and flash-write
latency. Explain how backpressure works and why the complete image is not held in RAM.

## 5. Model state and failure explicitly

Use the state machine and failure matrix in the [design](../architecture/SDD.md).
For every transition identify:

- Trigger and guard.
- State owner and persistent representation.
- Idempotency behavior after retry.
- Timeout and watchdog behavior.
- Recovery after reset or power interruption.
- Event reported to the platform.

Implement policy against fakes on the host before trusting HIL happy paths.

## 6. Understand fleet operations

Study release immutability, device identity, idempotency, cohorts, canary waves,
failure thresholds, audit trails, and eventual consistency. Use the fleet simulator
to explain why one successful board update does not prove safe fleet deployment.

## 7. Add automotive concepts deliberately

After HTTPS OTA is stable, study AUTOSAR Classic layering at a conceptual level and
UDS services relevant to programming. Explain how UDS over DoIP becomes another
front end to shared image/security services rather than a second bootloader.

Do not claim AUTOSAR, UDS, ISO 26262, or MISRA compliance solely because terminology
or selected practices appear in the project.

## 8. Recommended implementation study loop

1. Read one requirement and its linked design section.
2. State the expected behavior and cheapest falsifying test.
3. Implement the smallest component behind a replaceable boundary.
4. Run host tests, then target-specific tests.
5. Capture what the hardware disproved.
6. Update the ADR, design, and traceability before moving on.
