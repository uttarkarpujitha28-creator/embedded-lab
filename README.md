# Embedded Linux & Firmware Development Lab

![Linux](https://img.shields.io/badge/Linux-Kernel%206.6.0-black?logo=linux)
![Architecture](https://img.shields.io/badge/Architecture-ARM64-blue)
![Language](https://img.shields.io/badge/Language-C%2FC%2B%2B-orange)
![Platform](https://img.shields.io/badge/Emulation-QEMU-green)
![Status](https://img.shields.io/badge/Status-In%20Progress-yellow)

## Overview

This repository is a hands-on embedded Linux and firmware development environment focused on learning low-level programming, Linux kernel development, and device driver implementation.

The development environment uses an ARM64 Linux kernel running on QEMU, allowing kernel modules and driver concepts to be developed and tested without requiring physical development hardware.

The repository currently includes:

- ARM64 Linux kernel development using QEMU.
- Linux loadable kernel module development.
- Kernel module parameter handling.
- Custom module parameter callbacks.
- Root filesystem and initramfs creation.
- Cross-compilation using the AArch64 GCC toolchain.

## Development Environment

| Component | Configuration |
|---|---|
| Host OS | Ubuntu 22.04 |
| Target Architecture | ARM64 / AArch64 |
| Kernel Version | Linux 6.6.0 |
| Cross Compiler | AArch64 GCC |
| Emulator | QEMU System AArch64 |
| CPU Model | Cortex-A57 |
| Memory | 512 MB |
| Root Filesystem | BusyBox-based initramfs |
| Programming Language | C |

## Repository Structure

```text
embedded-lab/
│
├── docs/
│
├── drivers/
│
├── freertos/
│
├── linux/
│   ├── kernel-6.6/
│   │
│   └── drivers/
│       ├── hello_module/
│       │   ├── hello_module.c
│       │   ├── Makefile
│       │   └── README.md
│       │
│       └── user_args/
│           ├── user_args.c
│           ├── Makefile
│           └── README.md
│
├── projects/
│
├── qemu/
│
├── rootfs/
│   ├── bin/
│   ├── dev/
│   ├── etc/
│   ├── init
│   ├── lib/
│   │   └── modules/
│   │       └── 6.6.0/
│   │           └── extra/
│   │               ├── hello_module.ko
│   │               └── user_args.ko
│   ├── proc/
│   ├── sbin/
│   ├── sys/
│   └── usr/
│
├── scripts/
│   ├── initramfs.sh
│   └── boot_qemu.sh
│
├── rootfs.cpio.gz
│
└── README.md
```

## ARM64 QEMU Linux Environment

The project uses QEMU to emulate an ARM64 virtual machine.

The Linux kernel is booted using:

- ARM64 Linux kernel Image.
- QEMU `virt` machine.
- Cortex-A57 CPU.
- BusyBox-based root filesystem.
- Serial console through `ttyAMA0`.

### Booting the Linux Environment

From the project root directory:

```bash
./scripts/boot_qemu.sh
```

The script starts QEMU and loads:

- ARM64 Linux kernel Image.
- Generated initramfs archive.
- Kernel command-line parameters.

The guest Linux shell becomes available after successful boot.

To exit QEMU:

```text
Ctrl + A, then X
```

## Root Filesystem and Initramfs

A custom BusyBox-based root filesystem is maintained inside the `rootfs/` directory.

The root filesystem contains the utilities, directories, libraries, kernel modules, and initialization script required to boot the emulated Linux environment.

### What is initramfs?

Initramfs is an initial root filesystem loaded into memory during the Linux boot process.

In this project, the contents of the `rootfs/` directory are packaged into a compressed CPIO archive:

```text
rootfs.cpio.gz
```

QEMU passes this archive to the Linux kernel using the `-initrd` option.

### Creating the Initramfs

The project includes a shell script:

```text
scripts/initramfs.sh
```

This script:

1. Identifies the project root directory.
2. Navigates to the `rootfs/` directory.
3. Collects all files and directories.
4. Creates a CPIO archive using the `newc` format.
5. Compresses the archive using gzip.
6. Generates `rootfs.cpio.gz` in the project root directory.

Run the script:

```bash
./scripts/initramfs.sh
```

The generated archive is:

```text
embedded-lab/rootfs.cpio.gz
```

The script uses the project directory as its reference, so it can be executed from different working directories.

### Verifying the Initramfs

To inspect the contents of the generated archive:

```bash
zcat rootfs.cpio.gz | cpio -it
```

To check whether a particular kernel module is included:

```bash
zcat rootfs.cpio.gz | cpio -it | grep user_args
```

Expected output:

```text
./lib/modules/6.6.0/extra/user_args.ko
```

### When Should Initramfs Be Rebuilt?

Rebuild the initramfs whenever you:

- Add a new kernel module to the root filesystem.
- Replace an existing `.ko` file.
- Modify the root filesystem contents.
- Change the initialization script or other files included in the archive.

For example, after copying a newly compiled module:

```bash
cp linux/drivers/user_args/user_args.ko \
rootfs/lib/modules/6.6.0/extra/
```

Rebuild the archive:

```bash
./scripts/initramfs.sh
```

Then boot QEMU:

```bash
./scripts/boot_qemu.sh
```

This ensures QEMU loads the latest root filesystem and kernel modules.

## Linux Kernel Module Development

This project includes practical exercises for developing Linux loadable kernel modules.

### 1. Hello World Kernel Module

Demonstrates the basic structure and lifecycle of a Linux kernel module.

Concepts covered:

- Kernel module initialization using `module_init()`.
- Kernel module cleanup using `module_exit()`.
- Kernel logging using `pr_info()`.
- Building and loading `.ko` modules.
- Module insertion and removal using `insmod` and `rmmod`.

[View Hello Module Documentation](linux/drivers/hello_module/README.md)

### 2. User Arguments Kernel Module

Demonstrates passing parameters to a kernel module and modifying them at runtime.

Concepts covered:

- `module_param()`
- `module_param_array()`
- `module_param_cb()`
- Custom parameter setter callbacks.
- Runtime parameter updates through sysfs.
- Reading module parameters from user space.

[View User Arguments Documentation](linux/drivers/user_args/README.md)

## Complete Development Workflow

The following workflow is used when developing and testing a kernel module.

```text
       Kernel Module Source (.c)
                  |
                  v
          Cross Compilation
                  |
                  v
             Module (.ko)
                  |
                  v
       Copy Module into rootfs/
                  |
                  v
          Run initramfs.sh
                  |
                  v
       Generate rootfs.cpio.gz
                  |
                  v
          Run boot_qemu.sh
                  |
                  v
          ARM64 Linux Boot
                  |
                  v
          Load Module using
               insmod
                  |
                  v
         Test Module using
          lsmod / dmesg
                  |
                  v
          Unload using rmmod
```

### Quick Reference Commands

| Operation | Command |
|---|---|
| Build initramfs | `./scripts/initramfs.sh` |
| Boot QEMU | `./scripts/boot_qemu.sh` |
| List modules | `lsmod` |
| Load module | `insmod module_name.ko` |
| Check kernel logs | `dmesg` |
| Unload module | `rmmod module_name` |
| Inspect sysfs parameters | `ls /sys/module/module_name/parameters/` |

## Current Progress

| Area | Progress |
|---|---|
| ARM64 QEMU environment | Completed |
| Linux 6.6.0 kernel boot | Completed |
| BusyBox root filesystem | Completed |
| Initramfs generation script | Completed |
| Hello World kernel module | Completed |
| Module parameter handling | Completed |
| Custom parameter callbacks | Completed |
| Runtime sysfs parameter updates | Completed |
| Linux character device driver | Planned |

## Future Development

Planned topics and projects:

- Linux character device drivers.
- `file_operations` and user-space/kernel-space communication.
- GPIO and interrupt handling.
- I2C and SPI device drivers.
- Platform drivers and Device Tree.
- Linux kernel synchronization mechanisms.
- DMA and memory management.
- Embedded Linux BSP development.

## Author

**Pujitha Uttharkar**

Embedded Software | Linux Kernel | Firmware Development

---
*This repository is actively maintained as a practical embedded Linux and firmware development learning project.*