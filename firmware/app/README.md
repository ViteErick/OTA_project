# Device Firmware

This directory contains the project-owned Zephyr application. The first increment
is intentionally limited to board bring-up: startup identity, logging, and a green
LED heartbeat on Cortex-M7.

Build from the repository root after entering the project environment:

```powershell
. .\scripts\Enter-ZephyrEnv.ps1
.\.venv\Scripts\west.exe build -p always `
  -b "nucleo_h755zi_q/stm32h755xx/m7" `
  .\firmware\app `
  -d .\build\app-m7
```

No OTA, networking, security, or production-health behavior is claimed by this
bring-up increment.
