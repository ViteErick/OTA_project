# Threat Model

## 1. Scope and method

This model covers firmware creation, signing, publication, deployment, delivery,
boot validation, trial confirmation, telemetry, and operator control. STRIDE is used
as a prompt for analysis, not as proof that all threats have been eliminated.

## 2. Security objectives

1. Only authorized firmware executes.
2. Known-vulnerable older firmware cannot be reintroduced by normal OTA paths.
3. A network or platform compromise alone cannot forge an accepted image.
4. Failed or interrupted updates preserve a documented recovery path.
5. Privileged fleet actions are attributable and constrained.
6. Secrets have explicit owners, locations, rotation, and revocation procedures.

Availability is important, but local physical attackers with invasive hardware
capabilities are outside the initial assurance target.

## 3. Assets

| Asset | Security property |
|---|---|
| Release-signing private key | Confidentiality, integrity, restricted use |
| Trusted public key/configuration | Integrity and controlled replacement |
| Bootloader and boot policy | Integrity and availability |
| Firmware image and release metadata | Authenticity, integrity, compatibility |
| Device credentials | Confidentiality and revocability |
| Active bootable image | Integrity and availability |
| Deployment policy | Integrity and authorized control |
| Device/audit events | Integrity, ordering, attribution |

## 4. Adversaries

- A remote network attacker who can observe, block, replay, or alter traffic.
- An attacker with control-plane user credentials but insufficient role privileges.
- An attacker who compromises artifact storage or a runtime API service.
- A malicious or mistaken release operator.
- A person with non-invasive physical access to the development board.
- Accidental failures: power interruption, corruption, configuration error, and bugs.

## 5. Trust boundaries

See the [system context](../architecture/CONTEXT.md). The highest-risk transitions are:

- Unsigned build output crossing into the isolated signing environment.
- Signed artifacts crossing into storage and publication.
- Untrusted network data crossing into constrained device memory and flash.
- Candidate image state crossing from the application into MCUboot metadata.
- Human commands crossing into release and deployment state.

## 6. Threat register

| ID | STRIDE | Threat | Impact | Primary mitigation | Requirement | Residual work |
|---|---|---|---|---|---|---|
| THR-001 | Spoofing | Device connects to attacker endpoint | Credential/image metadata exposure | TLS hostname and CA validation | OTA-SEC-003 | Define time bootstrap |
| THR-002 | Spoofing | Attacker impersonates a device | False status or artifact access | Individual device credential and rotation | OTA-SEC-005 | Select credential format |
| THR-003 | Tampering | Artifact changed in storage or transit | Malicious/corrupt code | Signed MCUboot image and digest checks | OTA-SEC-001, PLT-FUN-001 | Prove negative HIL tests |
| THR-004 | Tampering | Candidate metadata torn by power loss | Wrong boot selection | MCUboot-owned trailer plus checked local records | OTA-SAF-002 | Verify flash behavior |
| THR-005 | Repudiation | Operator denies rollout action | Unaccountable fleet change | Immutable audit event with actor/result | PLT-FUN-004 | Define retention |
| THR-006 | Information disclosure | Private signing key enters runtime/repo | Fleet-wide forgery capability | Offline/CI isolation and secret scanning | OTA-SEC-004 | Define key ceremony |
| THR-007 | Information disclosure | Device credential extracted from board | Device impersonation | Per-device scope and revocation | OTA-SEC-005 | Assess hardware protection |
| THR-008 | Denial of service | Oversized or endless stream exhausts device | Update agent unavailable | Declared-size bound, timeout, streaming | OTA-FUN-002 | Fuzz parsers/streams |
| THR-009 | Denial of service | Repeated bad release creates boot loop | Device unavailable | Trial attempt policy and revert | OTA-FUN-005 | Measure reset behavior |
| THR-010 | Denial of service | Backend unavailable during trial | Endless trial/reboot | Local confirmation policy and bounded attempts | OTA-FUN-004, OTA-FUN-005 | Resolve exact policy |
| THR-011 | Elevation of privilege | Viewer starts or changes deployment | Unauthorized update exposure | Server-side RBAC | OTA-SEC-005 | Authorization matrix tests |
| THR-012 | Elevation of privilege | Valid old signed image is installed | Restored vulnerability | Security counter policy | OTA-SEC-002 | Prove persistence semantics |
| THR-013 | Tampering | Wrong-hardware image is offered | Device malfunction | Signed target identity and compatibility checks | OTA-FUN-001, OTA-FUN-003 | Define target identifier |
| THR-014 | Repudiation | Device retries duplicate event | Misleading metrics | Stable event ID and idempotent ingestion | PLT-FUN-003 | Define ordering rules |
| THR-015 | Tampering | Compromised API substitutes artifact | Unauthorized candidate offered | Offline image signature remains authoritative | OTA-SEC-001, OTA-SEC-004 | Exercise compromised-storage case |

## 7. Key lifecycle

### Development stage

- Generate replaceable development signing keys outside the repository.
- Commit only a public verification key when implementation requires it.
- Keep TLS development CA and device credentials outside source control.
- Label all development trust material as unsuitable for production.

### Production-oriented evolution

- Separate offline root/recovery authority from online release signing.
- Define signer authentication, approval, audit, backup, and disaster recovery.
- Include key IDs and support at least one tested verification-key rotation path.
- Revoke device credentials independently instead of sharing a fleet secret.
- Introduce TUF/Uptane roles only with expiry, rotation, and conformance tests.

## 8. Security decisions still open

| Decision | Closure evidence |
|---|---|
| Public-key storage/protection on STM32H755 | Hardware manual review and debug-protection experiment |
| Security-counter persistence | MCUboot mode/configuration and downgrade HIL result |
| Device authentication format | RAM/flash measurement and rotation prototype |
| TLS trusted-time bootstrap | Failure tests with invalid RTC and certificate dates |
| Verification-key rotation | Two-key migration demonstration |

## 9. Security verification minimum

The Ethernet end-to-end gate requires tests for modified image bytes, untrusted
signer, downgrade, wrong hardware target, unknown TLS CA, hostname mismatch,
truncated/oversized transfer, duplicate events, unauthorized operator, and trial
failure. Test evidence must include expected and observed device/platform states.
