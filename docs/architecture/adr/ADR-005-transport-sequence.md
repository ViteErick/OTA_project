# ADR-005: Ethernet First, Wi-Fi Through an Adapter

- **Status:** Accepted
- **Date:** 2026-08-28

## Context

The development board provides wired Ethernet while Wi-Fi needs additional hardware.
Update policy, signed image handling, and rollback should not diverge by transport.

## Decision

Implement and verify HTTPS OTA over Ethernet first. Define a transport port so a
later external Wi-Fi module can implement the same bounded request and stream
contracts without changing boot or update policy.

## Consequences

- The first end-to-end path uses available hardware and has fewer provisioning risks.
- Wi-Fi module selection can use measured RAM, throughput, and driver constraints.
- Network failover and Wi-Fi credential provisioning are deferred explicitly.

## Review trigger

Select Wi-Fi hardware only after the Ethernet update gate and an ADR comparing
Zephyr-native drivers with a networking coprocessor/offload design.
