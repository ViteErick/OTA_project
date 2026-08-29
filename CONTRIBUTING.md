# Contributing

This repository is both a working system and an engineering learning record.
Changes should preserve the reasoning that makes the implementation defensible.

## Workflow

1. Identify the requirement or architecture decision affected by the change.
2. Update or add a requirement before changing externally visible behavior.
3. Add the smallest test that can falsify the intended behavior.
4. Implement the change within the owning component.
5. Update traceability and retain relevant measurement or test evidence.
6. Record a new ADR when changing a cross-component or difficult-to-reverse decision.

## Documentation rules

- Write repository documentation, code, tests, and user-facing text in English.
- Use stable requirement IDs; never reuse a retired ID.
- Use relative links and Mermaid diagrams so documentation remains portable.
- Mark an unknown as `TBD` and state how it will be resolved.
- Separate observed facts from proposals and assumptions.
- Do not claim certification or standards compliance without the required process and evidence.

## Firmware rules

- Prefer fixed-width integer types at hardware and protocol boundaries.
- Avoid dynamic allocation in boot and update-critical paths unless justified by measurement.
- Do not implement cryptographic primitives.
- Treat all network, manifest, image, and persistent-state data as untrusted.
- Check lengths, integer conversions, state transitions, and every flash operation result.
- Follow Zephyr and MCUboot conventions before introducing local abstractions.
- Record MISRA C:2012 deviations when a rule is selected for project enforcement.

## Security rules

- Never commit private keys, access tokens, passwords, or production credentials.
- Development credentials must be clearly labeled and replaceable.
- Signing and publishing are separate actions with separate authorization.
- Security-sensitive changes require negative tests, not only a happy path.

## Definition of done

A change is done when its acceptance criteria pass, relevant documentation and
traceability are current, no unexplained budget regression exists, and evidence is
reproducible by another contributor.
