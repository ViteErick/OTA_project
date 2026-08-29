# Traceability Matrix

This matrix is established before implementation. Code and final test paths remain
`TBD` until their modules exist. A requirement becomes `Verified` only when linked
evidence passes in the intended environment.

| Requirement | Design component | Planned verification | Implementation | Status |
|---|---|---|---|---|
| OTA-FUN-001 | Update policy, device API | Integration eligibility tests | TBD | Proposed |
| OTA-FUN-002 | Transport port, image adapter | TST-NET-001, TST-NET-003 | TBD | Proposed |
| OTA-FUN-003 | Image adapter, MCUboot interface | TST-FAULT-001, TST-FAULT-002 | TBD | Proposed |
| OTA-FUN-004 | Health service | TST-BOOT-004 | TBD | Proposed |
| OTA-FUN-005 | MCUboot, health service | TST-BOOT-003 | TBD | Proposed |
| OTA-FUN-006 | Transport port/adapters | Host fake transport test, design inspection | TBD | Proposed |
| OTA-SEC-001 | MCUboot, signing tool | TST-BOOT-001, TST-BOOT-002 | TBD | Proposed |
| OTA-SEC-002 | MCUboot security-counter policy | TST-BOOT-005 | TBD | Proposed |
| OTA-SEC-003 | Ethernet/TLS adapter | TST-NET-001, TST-NET-002 | TBD | Proposed |
| OTA-SEC-004 | Isolated signing boundary | Secret scan and artifact inspection | TBD | Proposed |
| OTA-SEC-005 | API authentication/RBAC/audit | TST-PLT-003 | TBD | Proposed |
| OTA-SAF-001 | Transport/image adapters | TST-FAULT-001 | TBD | Proposed |
| OTA-SAF-002 | Persistent record adapter | TST-FAULT-003 | TBD | Proposed |
| OTA-SAF-003 | Factory recovery procedure | TST-REC-001 | TBD | Proposed |
| OTA-PER-001 | Firmware build pipeline | Oversize budget CI fixture | TBD | Proposed |
| OTA-PER-002 | HIL measurement collector | Successful HIL timing report | TBD | Proposed |
| OTA-OBS-001 | Telemetry adapter, event API | End-to-end device timeline test | TBD | Proposed |
| PLT-FUN-001 | Release/artifact services | Immutable artifact API test | TBD | Proposed |
| PLT-FUN-002 | Deployment/wave services | TST-PLT-002 | TBD | Proposed |
| PLT-FUN-003 | Device event service | TST-PLT-001 | TBD | Proposed |
| PLT-FUN-004 | Audit service | Privileged-operation persistence tests | TBD | Proposed |

## Maintenance rules

1. Every accepted requirement links to at least one design component and test.
2. Implementation links are added in the same change that introduces the behavior.
3. A test ID maps to a stable automated test or documented manual procedure.
4. Failed or skipped gate tests cannot be counted as verification evidence.
5. Requirement status is updated only after reviewing retained evidence.
