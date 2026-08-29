# Software Requirements Specification

## 1. Purpose

This specification defines verifiable behavior for the STM32H755 OTA Platform.
Implementation choices are documented separately in architecture decision records.

## 2. Requirement format

- **Status:** `Proposed`, `Accepted`, `Implemented`, `Verified`, or `Retired`.
- **Verification:** `Inspection`, `Analysis`, `Test`, or `Demonstration`.
- IDs remain stable. A retired ID is never assigned to another requirement.
- `Shall` denotes a mandatory requirement.

## 3. Device update requirements

### OTA-FUN-001: Update discovery

**Status:** Proposed  
**Requirement:** The device shall request update eligibility using its unique
identity, hardware identity, current firmware version, and current security counter.  
**Rationale:** The platform must avoid offering incompatible or unnecessary images.  
**Acceptance criteria:** An eligible device receives one release descriptor; an
ineligible device receives an explicit no-update response.  
**Verification:** Integration test.

### OTA-FUN-002: Bounded artifact download

**Status:** Proposed  
**Requirement:** The device shall download an offered artifact without buffering
the complete image in volatile memory and shall reject data beyond the declared size.  
**Rationale:** Device RAM is constrained and remote sizes are untrusted.  
**Acceptance criteria:** A valid artifact reaches the candidate storage; oversized
and truncated transfers enter a defined error state without changing the active image.  
**Verification:** Unit and HIL tests.

### OTA-FUN-003: Candidate scheduling

**Status:** Proposed  
**Requirement:** The device shall schedule only a completely received and locally
validated candidate image for trial boot.  
**Rationale:** Partial downloads must never become boot candidates.  
**Acceptance criteria:** Reset during download leaves the current image bootable;
completed valid download is marked pending through the bootloader API.  
**Verification:** Fault-injection HIL test.

### OTA-FUN-004: Trial confirmation

**Status:** Proposed  
**Requirement:** A trial image shall be confirmed only after all mandatory local
health checks pass.  
**Rationale:** Network availability is not proof that the application is healthy.  
**Acceptance criteria:** Passing checks confirm once; any failed mandatory check
prevents confirmation and records its result.  
**Verification:** Unit and HIL tests.

### OTA-FUN-005: Rollback

**Status:** Proposed  
**Requirement:** The boot chain shall return to the previous bootable image when a
trial image is not confirmed within the configured attempt policy.  
**Rationale:** A faulty release must not permanently disable normal operation.  
**Acceptance criteria:** An intentionally non-confirming trial reverts and the
device reports the rollback after connectivity returns.  
**Verification:** HIL demonstration and retained boot log.

### OTA-FUN-006: Transport independence

**Status:** Proposed  
**Requirement:** Update policy and image lifecycle logic shall not depend directly
on Ethernet- or Wi-Fi-specific APIs.  
**Rationale:** Wi-Fi is a planned extension and must not fork security policy.  
**Acceptance criteria:** Transport is accessed through a documented interface;
policy state-machine tests run with a fake transport.  
**Verification:** Design inspection and unit test.

## 4. Security requirements

### OTA-SEC-001: Image authenticity

**Status:** Proposed  
**Requirement:** The bootloader shall boot only images authorized by a configured
trusted public key and supported signature algorithm.  
**Rationale:** Transport security alone cannot authorize firmware content.  
**Acceptance criteria:** A correctly signed image boots; modified content and an
image signed by an untrusted key are rejected.  
**Verification:** Negative HIL tests.

### OTA-SEC-002: Downgrade resistance

**Status:** Proposed  
**Requirement:** The boot chain shall reject an image whose security counter is
below the device's accepted policy value.  
**Rationale:** Reinstalling a known-vulnerable valid image is a security failure.  
**Acceptance criteria:** A lower counter is rejected while a higher permitted
counter can enter trial. The exact persistence mechanism is resolved by ADR-003.  
**Verification:** HIL test and configuration inspection.

### OTA-SEC-003: Server authentication

**Status:** Proposed  
**Requirement:** The device shall authenticate OTA service endpoints using TLS and
a provisioned trust anchor.  
**Rationale:** Endpoint authentication protects metadata and device credentials.  
**Acceptance criteria:** Valid endpoint succeeds; unknown CA, hostname mismatch,
expired certificate, and invalid time basis fail closed.  
**Verification:** Integration tests.

### OTA-SEC-004: Signing key isolation

**Status:** Proposed  
**Requirement:** Private release-signing keys shall not be stored in device
firmware, runtime control-plane services, container images, or version control.  
**Rationale:** Publishing access must not imply signing authority.  
**Acceptance criteria:** Repository and built artifacts contain no private key;
signing occurs through a separate offline or CI boundary.  
**Verification:** Inspection and secret scan.

### OTA-SEC-005: Least-privilege operations

**Status:** Proposed  
**Requirement:** Human platform actions shall be authenticated, authorized by role,
and recorded when they alter releases, credentials, or deployments.  
**Rationale:** Fleet changes require attribution and constrained authority.  
**Acceptance criteria:** Viewer, release manager, and administrator permissions are
enforced and rejected privileged actions are recorded.  
**Verification:** API authorization tests.

## 5. Recovery and robustness requirements

### OTA-SAF-001: Interrupted download

**Status:** Proposed  
**Requirement:** Loss of network, reset, or power during download shall preserve a
bootable active image and produce a recoverable state on the next boot.  
**Rationale:** Download is a common and long interruption window.  
**Acceptance criteria:** Injection at defined transfer offsets never selects the
partial image and normal boot remains possible.  
**Verification:** HIL fault injection.

### OTA-SAF-002: Persistent-state integrity

**Status:** Proposed  
**Requirement:** Project-owned persistent update state shall detect incomplete or
corrupt records and choose a conservative recovery action.  
**Rationale:** Torn metadata writes can misrepresent image state.  
**Acceptance criteria:** Corrupted checksum and interrupted write are detected;
neither authorizes a candidate image.  
**Verification:** Host unit test and HIL fault injection.

### OTA-SAF-003: Factory recovery

**Status:** Proposed  
**Requirement:** The project shall document and verify a manual factory recovery
procedure independent of the OTA control plane.  
**Rationale:** Development hardware needs a final recovery path.  
**Acceptance criteria:** A known image is restored with ST-Link or the STM32 ROM
bootloader from a deliberately non-booting application state.  
**Verification:** Demonstration.

## 6. Performance and observability requirements

### OTA-PER-001: Resource budget

**Status:** Proposed  
**Requirement:** Firmware builds shall report flash and RAM consumption and fail CI
when an accepted partition or runtime budget is exceeded.  
**Rationale:** OTA feasibility depends directly on image size and memory headroom.  
**Acceptance criteria:** Build output records sizes; thresholds become numeric after
the feasibility spike and a deliberate oversized fixture fails the check.  
**Verification:** CI test.

### OTA-PER-002: Update timing

**Status:** Proposed  
**Requirement:** The system shall measure download, validation, reboot, first-health,
and confirmation durations for each HIL update.  
**Rationale:** Measured behavior is required to set timeouts and rollout policy.  
**Acceptance criteria:** A successful HIL report includes all five durations.  
**Verification:** HIL report inspection.

### OTA-OBS-001: Device event reporting

**Status:** Proposed  
**Requirement:** The device shall report significant update transitions, failures,
active version, reset reason, and rollback after connectivity is available.  
**Rationale:** Operators need enough evidence to reconstruct an update outcome.  
**Acceptance criteria:** The device timeline orders events idempotently and includes
reason codes for failed and rolled-back updates.  
**Verification:** End-to-end test.

## 7. Platform requirements

### PLT-FUN-001: Release immutability

**Status:** Proposed  
**Requirement:** A published release shall reference immutable artifact content and
identity metadata.  
**Rationale:** A deployment must not change meaning after approval.  
**Acceptance criteria:** Replacement of artifact bytes under an existing release is
rejected; digest mismatch prevents publication or delivery.  
**Verification:** API integration test.

### PLT-FUN-002: Staged deployment

**Status:** Proposed  
**Requirement:** The platform shall support ordered deployment waves with targeting,
pause, resume, cancel, and automatic pause based on configured failure thresholds.  
**Rationale:** Fleet exposure must be limited while evidence accumulates.  
**Acceptance criteria:** A simulated fleet demonstrates canary progression and an
automatic pause after crossing a rollback threshold.  
**Verification:** End-to-end simulator test.

### PLT-FUN-003: Idempotent device reporting

**Status:** Proposed  
**Requirement:** Repeated delivery of the same device event shall not create
conflicting platform state or duplicate logical timeline entries.  
**Rationale:** Embedded clients retry requests after ambiguous network failures.  
**Acceptance criteria:** Replaying an event with the same device event ID preserves
one logical event and returns a successful response.  
**Verification:** API integration test.

### PLT-FUN-004: Audit history

**Status:** Proposed  
**Requirement:** Privileged release, credential, role, and deployment operations
shall append an immutable audit event with actor, action, target, time, and result.  
**Rationale:** Operators and reviewers need accountability.  
**Acceptance criteria:** Every tested privileged operation has a corresponding
audit record that normal roles cannot alter or delete.  
**Verification:** API authorization and persistence test.
