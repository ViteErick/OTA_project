# MCUboot Sysbuild Spike Explained

This notebook explains what we built, why each file exists, what failed along the
way, and what the successful build actually proves. It is educational rather than
normative: architecture decisions remain in the ADRs and verified behavior belongs
in retained test evidence.

## 1. What happened in one sentence

We configured Zephyr sysbuild to compile two cooperating programs for the STM32H755
Cortex-M7: MCUboot at the beginning of flash and a signed Zephyr application in the
primary image slot.

The final build proved that the source code, partition description, linker layout,
and image-signing pipeline agree. It did **not** yet prove boot, update, rollback, or
power-loss behavior on the physical board.

## 2. The mental model

An OTA-capable device needs more than an application that can download a file.
There are two separate executable images:

1. **MCUboot** executes first after reset. It decides which application image may run.
2. **The application** performs the product behavior and will eventually download
   updates, request an upgrade, and confirm a successful trial boot.

The trust flow is:

```text
Reset
  |
  v
MCUboot starts from 0x08000000
  |
  +-- Read image metadata
  +-- Validate image structure
  +-- Verify signature with embedded public key
  +-- Apply pending/trial/revert policy
  |
  v
Signed application starts from slot 0 at 0x08020000
```

The application does not decide whether its own signature is acceptable. That
decision belongs to MCUboot, which is a smaller and more controlled trust boundary.

## 3. Why there are two builds

Zephyr normally builds one application. Sysbuild is a Zephyr mechanism that
coordinates multiple images in one top-level build.

In this project:

- `sysbuild.conf` asks sysbuild to include MCUboot.
- Sysbuild creates an `app` child build.
- Sysbuild creates an `mcuboot` child build.
- The app is linked for slot 0 and signed after linking.
- MCUboot is linked for the boot partition and contains the verification public key.

The enabling configuration is intentionally small:

```ini
SB_CONFIG_BOOTLOADER_MCUBOOT=y
```

`SB_CONFIG_` means this option belongs to the **sysbuild** configuration domain. It
does not directly enable a feature inside the application.

## 4. Which tool owns which decision

Several tools participate, but they solve different problems:

| Tool or format | Responsibility |
|---|---|
| `west` | Workspace and build command orchestration |
| sysbuild | Coordinates the MCUboot and application child images |
| CMake | Creates the build graph and invokes compilers/tools |
| Kconfig | Selects software features and calculated configuration values |
| Devicetree | Describes hardware and flash partitions |
| Linker | Places executable sections at final memory addresses |
| `imgtool` | Adds MCUboot metadata and signs the application image |
| Ninja | Executes the generated build steps |
| ARM GCC | Compiles and links code for Cortex-M7 |

A useful debugging rule is to ask: **which tool owns the incorrect fact?**

- Wrong feature enabled: inspect Kconfig.
- Wrong flash address: inspect Devicetree and linker output.
- Missing signature: inspect the post-build `imgtool` step.
- Wrong child image: inspect sysbuild configuration.

## 5. The flash layout

The STM32H755 has two 1 MiB flash banks in this board description. The experimental
layout uses the complete 2 MiB address range:

| Region | Offset | Absolute start | Size | Purpose |
|---|---:|---:|---:|---|
| MCUboot | `0x000000` | `0x08000000` | 128 KiB | Bootloader and trust policy |
| Slot 0 | `0x020000` | `0x08020000` | 896 KiB | Active/primary application |
| Slot 1 | `0x100000` | `0x08100000` | 896 KiB | Candidate/secondary image |
| Storage | `0x1E0000` | `0x081E0000` | 128 KiB | Persistent state experiment |

The arithmetic is:

```text
128 KiB + 896 KiB + 896 KiB + 128 KiB = 2048 KiB = 2 MiB
```

The offsets are relative to flash base `0x08000000`. For example:

```text
slot 0 absolute address = 0x08000000 + 0x00020000
                        = 0x08020000
```

### Why 128 KiB boundaries matter

The STM32H755 flash erase unit used by this target is 128 KiB. Flash cannot be
treated like RAM: changing a few bytes may require erasing an entire sector first.
Partition boundaries therefore align to erase sectors so one partition operation
does not destroy neighboring data.

### The important dual-core warning

Slot 1 begins at `0x08100000`, the bank associated with the Cortex-M4 image in the
upstream board layout. A 2 MiB address range existing on the chip does not prove
that M7 can safely use it as an OTA slot while M4 boot behavior remains enabled.

Therefore this map is currently an **experimental build-time layout**. We must
resolve M4 boot ownership and option-byte behavior before writing slot 1.

## 6. Why the partition description was split

Both child images need the same partition table, so the table lives once in:

```text
firmware/app/dts/h755_ota_partitions.dtsi
```

However, each child image must execute from a different partition:

```dts
/* Application overlay */
/ {
    chosen {
        zephyr,code-partition = &slot0_partition;
    };
};
```

```dts
/* MCUboot overlay */
/ {
    chosen {
        zephyr,code-partition = &boot_partition;
    };
};
```

The partition table answers **what regions exist**. The `chosen` property answers
**which region contains this particular executable**.

This distinction fixed a real defect in the first successful build. Initially both
images consumed a common overlay that selected slot 0. The build completed, but the
MCUboot memory report showed an 896 KiB region. A successful compiler exit was not
enough; the memory report disproved the intended architecture.

After separating the image-specific overlays, generated evidence showed:

```text
Application code partition: slot0_partition
Application FLASH origin:   0x08020000

MCUboot code partition:     boot_partition
MCUboot FLASH origin:       0x08000000
MCUboot FLASH length:       0x00020000 (128 KiB)
```

## 7. How sysbuild passes the MCUboot overlay

The application board overlay is discovered by Zephyr through its board-specific
filename. MCUboot is a separate child build, so it needs its own overlay path.

`sysbuild.cmake` supplies that path only when MCUboot is enabled:

```cmake
if(SB_CONFIG_BOOTLOADER_MCUBOOT)
    set(
        mcuboot_EXTRA_DTC_OVERLAY_FILE
        "${CMAKE_CURRENT_LIST_DIR}/sysbuild/mcuboot.overlay"
        CACHE INTERNAL "MCUboot partition overlay"
        FORCE
    )
endif()
```

The `mcuboot_` prefix targets the child image named `mcuboot`. Without this boundary,
it is easy to configure the application correctly while leaving the bootloader on
the board's default memory region.

## 8. What the build command does

The clean validation command was:

```powershell
.\.venv\Scripts\west.exe build `
  -p always `
  --sysbuild `
  -b 'nucleo_h755zi_q/stm32h755xx/m7' `
  .\firmware\app `
  -d .\build\app-m7-mcuboot
```

Meaning of each argument:

| Argument | Meaning |
|---|---|
| `build` | Ask west to run the Zephyr build command |
| `-p always` | Make the build directory pristine before configuration |
| `--sysbuild` | Build the multi-image system, not only the app |
| `-b .../m7` | Select the exact board, SoC, and Cortex-M7 qualifier |
| `.\firmware\app` | Application source directory |
| `-d ...` | Dedicated generated-output directory |

Using `-p always` was valuable after changing overlays because cached Devicetree or
CMake state could otherwise make the result ambiguous.

## 9. Why Python dependencies blocked a C firmware build

The firmware is written in C, but the build system uses Python tools. `imgtool` is a
Python program and imports the `cryptography` package to create digital signatures.

The first relevant failure was:

```text
ModuleNotFoundError: No module named 'cryptography'
```

This did not mean that MCUboot C code was broken. It meant the host-side signing
tool could not run. We installed the dependencies declared by the checked-out
MCUboot revision:

```powershell
.\.venv\Scripts\python.exe -m pip install `
  -r .\bootloader\mcuboot\scripts\requirements.txt
```

Keeping `.venv\Scripts` first in `PATH` also mattered. Without it, a child build
found the global Python interpreter, which did not contain the workspace packages.

The lesson is that an embedded build has two environments:

1. The **target environment**: ARM machine code that runs on STM32H755.
2. The **host environment**: Python, CMake, Ninja, west, and signing tools on Windows.

A host dependency failure can prevent valid target code from becoming a deployable
artifact.

## 10. Understanding the final memory reports

The final MCUboot report was:

```text
FLASH: 36040 B / 128 KiB (27.50%)
RAM:   21568 B / 512 KiB (4.11%)
```

This is the expected region size for the boot partition.

The application report was:

```text
FLASH: 31264 B / 655024 B (4.77%)
RAM:    7872 B / 512 KiB (1.50%)
```

Why does the application show 655,024 B instead of the full 896 KiB slot?

The generated configuration reserved `0x40150` bytes at the end of the slot for the
selected MCUboot move-swap strategy and its metadata/trailer needs:

```text
Slot size                   = 0xE0000 = 917,504 B
Generated end reservation  = 0x40150 = 262,480 B
Usable linker region       =            655,024 B
```

This is a major embedded-systems lesson: **partition size is not always maximum
payload size**. Headers, trailers, status metadata, alignment, and swap strategy
consume space too.

## 11. What signing produced

The post-build signing step generated at least these application artifacts:

```text
build/app-m7-mcuboot/app/zephyr/zephyr.signed.bin
build/app-m7-mcuboot/app/zephyr/zephyr.signed.hex
```

The observed signed binary size was 31,600 bytes. The unsigned ELF remains useful
for symbols and debugging, while the signed binary or HEX is the bootable artifact
under MCUboot policy.

The current build uses MCUboot's default RSA-2048 development key. The build warns
that this key is insecure. This is acceptable for a bring-up spike only because the
private key is public knowledge and cannot establish product authenticity.

For a real product:

- Generate a project-specific private signing key.
- Keep the private key outside the firmware repository and build agents that do not
  need signing authority.
- Embed only the public verification key in MCUboot.
- Define rotation, revocation, backup, and audit procedures.

## 12. What we proved

The build provides evidence for these statements:

- Zephyr 4.2.2 recognizes the qualified M7 board target.
- Sysbuild can coordinate the application and MCUboot.
- Both child images consume the same fixed-partition table.
- MCUboot links into the 128 KiB boot partition.
- The application links into slot 0 starting at `0x08020000`.
- The checked-out `imgtool` can sign the application in the local Python environment.
- The application and bootloader fit their current linker regions.

## 13. What we did not prove

Do not convert build evidence into runtime claims. We have not yet demonstrated:

- MCUboot starts on the physical board.
- MCUboot accepts this signed application and transfers control to it.
- A modified image is rejected.
- Slot 1 can be erased or written without interfering with Cortex-M4 startup.
- A pending image enters trial mode.
- Application confirmation prevents revert.
- An unconfirmed or crashing image reverts correctly.
- Reset or power loss at each flash transition remains recoverable.
- Downgrade prevention works.
- The final production key-management design is secure.

This distinction is the difference between **configuration confidence** and
**behavioral evidence**.

## 14. Generated files worth inspecting

Generated files are ignored by Git, but they are excellent learning evidence:

| File | Question it answers |
|---|---|
| `app/zephyr/zephyr.dts` | Which hardware and partition choices reached the app? |
| `mcuboot/zephyr/zephyr.dts` | Which partition choices reached MCUboot? |
| `app/zephyr/.config` | Which Kconfig values were finally selected for the app? |
| `mcuboot/zephyr/.config` | Which boot strategy and limits were selected? |
| `*/zephyr/linker.cmd` | What address and capacity did the linker actually use? |
| `*/zephyr/zephyr.map` | Where did every linked section and symbol go? |
| `app/zephyr/zephyr.signed.bin` | What binary will MCUboot authenticate? |

Never treat an input overlay as proof by itself. Inspect the generated DTS and
linker command, because they show the merged result the tools actually consumed.

## 15. Recommended next experiment

The next safe experiment is a **primary-slot boot test**, not a secondary-slot OTA
test:

1. Preserve the current option-byte values and board state as evidence.
2. Determine the STM32H755 M4 boot behavior from authoritative ST documentation.
3. Build and inspect a combined flash artifact containing MCUboot and the signed app.
4. Flash only MCUboot plus slot 0; do not write slot 1 yet.
5. Capture the MCUboot serial log and application heartbeat.
6. Corrupt a copy of the application and verify rejection.
7. Only after resolving M4 ownership, test slot 1 and swap behavior.

The cheapest falsifying observation for the first test is simple: if MCUboot logs an
invalid image or never transfers control to the heartbeat application, the build
layout is not yet sufficient for runtime boot.

## 16. Questions to answer without looking back

1. Why is sysbuild needed when the application already has a `CMakeLists.txt`?
2. What is the difference between defining a partition and choosing a code partition?
3. Why did an 896 KiB MCUboot memory report indicate a defect even though compilation
   succeeded?
4. Why can a missing Python package stop a C firmware build?
5. Why is slot 1 unsafe to flash before resolving Cortex-M4 behavior?
6. Why is HTTPS not a replacement for an MCUboot image signature?
7. Why is the default MCUboot key unsuitable for production?
8. What evidence would prove rollback rather than merely prove compilation?

If these can be explained using the generated DTS, linker command, signed artifact,
and board log, the architecture is becoming understood rather than memorized.

## 17. Source files for this lesson

- [Application configuration](../../firmware/app/prj.conf)
- [Application entry point](../../firmware/app/src/main.c)
- [Sysbuild configuration](../../firmware/app/sysbuild.conf)
- [Sysbuild child-image wiring](../../firmware/app/sysbuild.cmake)
- [Shared partition table](../../firmware/app/dts/h755_ota_partitions.dtsi)
- [Application board overlay](../../firmware/app/boards/nucleo_h755zi_q_stm32h755xx_m7.overlay)
- [MCUboot child overlay](../../firmware/app/sysbuild/mcuboot.overlay)
- [Bootloader and RTOS decision](../architecture/adr/ADR-001-zephyr-mcuboot.md)
- [M7-first decision](../architecture/adr/ADR-002-m7-first.md)
- [Update layout decision](../architecture/adr/ADR-003-update-layout.md)