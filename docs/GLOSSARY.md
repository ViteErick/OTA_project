# Glossary

| Term | Meaning in this project |
|---|---|
| Active image | Image currently selected as the normal boot candidate |
| Artifact | Immutable firmware binary and associated release metadata |
| Confirmation | Application action that accepts a successfully tested trial image |
| Control plane | Backend services that manage devices, releases, and deployments |
| Deployment | Policy assigning one firmware release to an eligible device cohort |
| Device agent | Firmware component that checks, downloads, and schedules updates |
| DoIP | Diagnostics over Internet Protocol, used to transport UDS over Ethernet |
| Factory recovery | Manual recovery using the STM32 ROM bootloader or ST-Link |
| HIL | Hardware-in-the-loop testing against the physical target |
| Image trailer | MCUboot metadata used to track swap, trial, and confirmation state |
| MCUboot | Bootloader responsible for image validation and update boot policy |
| Primary slot | Flash region from which the normal application image executes |
| Release | Versioned, signed firmware artifact approved for deployment |
| Rollback | Return to a previously bootable image after a failed trial |
| Secondary slot | Flash region holding a candidate or previous image, strategy-dependent |
| Security counter | Monotonic image value used by policy to reject older vulnerable images |
| Trial boot | First boot of an unconfirmed update with automatic revert semantics |
| UDS | Unified Diagnostic Services, ISO 14229 application-layer diagnostics |
| Update authority | Entity whose public key is trusted to authorize firmware images |
| Wave | Controlled subset of a deployment released after defined conditions |

## Terminology rule

Use MCUboot's exact terms when describing its implemented behavior. Use generic
terms such as "A/B" only for architectural discussion because MCUboot upgrade
modes may use swap, overwrite, direct-XIP, or another supported strategy.
