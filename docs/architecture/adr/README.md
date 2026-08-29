# Architecture Decision Records

ADRs capture consequential decisions and their tradeoffs. Accepted ADRs are not
silently rewritten when circumstances change; a new ADR supersedes the old one.

| ADR | Decision | Status |
|---|---|---|
| [ADR-001](ADR-001-zephyr-mcuboot.md) | Prefer Zephyr and MCUboot | Proposed |
| [ADR-002](ADR-002-m7-first.md) | Implement Cortex-M7 first | Accepted |
| [ADR-003](ADR-003-update-layout.md) | Prefer signed two-slot update after spike | Proposed |
| [ADR-004](ADR-004-control-plane.md) | Use FastAPI and PostgreSQL | Accepted |
| [ADR-005](ADR-005-transport-sequence.md) | Ethernet first, Wi-Fi through an adapter | Accepted |
| [ADR-006](ADR-006-automotive-extension.md) | Apply AUTOSAR concepts, add UDS later | Accepted |

## Status meanings

- **Proposed:** evidence or review is still required.
- **Accepted:** current implementation direction.
- **Superseded:** replaced by a later ADR.
- **Rejected:** considered but not selected.
