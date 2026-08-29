# Interview Topics and Project Evidence

Use these prompts to practice technical explanations. Strong answers state a threat
or constraint, compare alternatives, describe the chosen mechanism, and point to a
measurement or failure test from this repository.

## Boot and flash

1. Why is a bootloader needed for OTA instead of letting the application replace itself?
2. How do pending, trial, confirmation, and rollback differ?
3. What happens if power fails during download, pending-state update, or confirmation?
4. How do erase units, alignment, headers, trailers, and image size determine the layout?
5. Why does this project avoid assuming that dual-bank flash automatically means A/B OTA?

Evidence target: accepted ADR-003, final DTS/linker maps, boot logs, and
`TST-BOOT-*`/`TST-FAULT-*` results.

## Security

1. Why are HTTPS and a firmware signature both required?
2. How can an old but correctly signed image still be dangerous?
3. Where are private and public keys stored, and how are they rotated or revoked?
4. What can an attacker do after compromising artifact storage or the OTA API?
5. Which physical attacks remain outside the current threat model?

Evidence target: [threat model](../security/THREAT_MODEL.md), signer boundary,
negative image/TLS tests, and secret-scan results.

## RTOS and device architecture

1. Why was Zephyr preferred, and what evidence can force the fallback stack?
2. Which OTA parts are policy, ports, hardware adapters, and bootloader behavior?
3. How are task stacks, buffers, watchdog deadlines, and flash writes budgeted?
4. Why is Cortex-M4 delayed, and what new failure modes appear when it is added?
5. How would Ethernet and Wi-Fi share update logic?

Evidence target: ADR-001, ADR-002, ADR-005, host policy tests, and memory reports.

## Backend and fleet management

1. What is the difference between a firmware artifact, release, deployment, and wave?
2. Why are published releases immutable?
3. How does event idempotency handle ambiguous network failures?
4. What metrics should automatically pause a rollout?
5. Why is the dashboard outside the critical update path?

Evidence target: API contracts, PostgreSQL constraints, fleet simulator scenarios,
RBAC tests, and audit events.

## Testing and diagnosis

1. Which behavior can be proven on the host and which requires physical hardware?
2. How is software reset testing different from real power interruption testing?
3. Why is code coverage insufficient for an OTA system?
4. How do requirement IDs connect to design, code, tests, and retained evidence?
5. Describe a failure discovered by the board that changed an architecture decision.

Evidence target: [test strategy](../verification/TEST_STRATEGY.md),
[traceability matrix](../verification/TRACEABILITY.md), HIL artifacts, and ADR history.

## Automotive extension

1. Why is this project not an AUTOSAR Classic implementation?
2. Which AUTOSAR-inspired boundaries still improve the design?
3. How do UDS RequestDownload and TransferData relate to the existing image service?
4. What must be protected in SecurityAccess, and why is a custom seed/key risky?
5. How would coordinated M7/M4 rollback differ from one-image rollback?

Evidence target: ADR-006, later UDS adapter tests, IPC contract, and multi-image
compatibility experiments.

## One-minute project summary

Practice a concise explanation covering the hardware target, MCUboot trust boundary,
Ethernet flow, trial/revert behavior, FastAPI rollout control, negative HIL tests,
and the deliberate sequence toward Wi-Fi and UDS. Avoid claiming features that have
not passed their roadmap gate.
