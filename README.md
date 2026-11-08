# Embedded Linux & Firmware Development Lab

A hands-on embedded systems learning and development environment focused on Linux kernel internals, device drivers, firmware development, and hardware-software interaction.

This repository documents my practical learning journey, experiments, and implementations in embedded Linux and firmware development.

## Objectives

- Understand Linux kernel architecture and boot process.
- Learn Linux kernel module and device driver development.
- Explore character, platform, I2C, and SPI drivers.
- Understand memory-mapped I/O, interrupts, and hardware interfaces.
- Explore embedded Linux using QEMU without requiring physical development boards.
- Develop practical firmware and RTOS projects.

## Development Environment

| Component | Configuration |
|---|---|
| Host OS | Ubuntu 22.04 |
| Processor | Intel Core i5-1135G7 |
| Virtualization | QEMU |
| Target Architecture | ARM64 (AArch64) |
| Emulated Machine | QEMU virt |
| Emulated CPU | Cortex-A57 |
| Linux Kernel | 6.6 |
| Root Filesystem | BusyBox-based initramfs |
| Kernel Console | ttyAMA0 |
| Memory Allocated to QEMU | 512 MB |

## Repository Structure

```text
embedded-lab/
│
├── drivers/                 # Embedded driver experiments
│
├── linux/
│   ├── kernel-6.6/          # Linux kernel source and build
│   ├── docs/                # Linux kernel documentation
│   └── drivers/             # Linux kernel driver projects
│
├── qemu/                    # QEMU configuration and scripts
│
├── rootfs/                  # BusyBox-based root filesystem
│
├── busybox/                 # BusyBox source and build environment
│
├── freertos/                # RTOS experiments
│
├── projects/                # Independent embedded projects
│
├── scripts/                 # Automation scripts
│
├── docs/                    # General documentation
│
└── README.md
```

## QEMU ARM64 Linux Environment

I configured an ARM64 Linux environment using QEMU to experiment with embedded Linux without requiring a physical ARM development board.

### 1. Install development tools

Install the required packages on Ubuntu:

```bash
sudo apt update

sudo apt install -y \
    build-essential \
    git \
    gcc-aarch64-linux-gnu \
    binutils-aarch64-linux-gnu \
    qemu-system-arm \
    libncurses-dev \
    flex \
    bison \
    bc \
    libssl-dev \
    libelf-dev
```

### 2. Obtain Linux kernel source

Download the Linux kernel source and extract it into the `linux` directory.

The kernel version used in this environment is Linux 6.6.

The kernel source is maintained locally and is excluded from the main Git repository to avoid pushing the entire upstream kernel source.

### 3. Configure the ARM64 kernel

Configure the kernel for the ARM64 architecture:

```bash
cd ~/embedded-lab/linux/kernel-6.6

make ARCH=arm64 CROSS_COMPILE=aarch64-linux-gnu- defconfig
```

The kernel must be built with the appropriate configuration for the QEMU ARM64 environment.

### 4. Build the Linux kernel

```bash
make -j$(nproc) \
    ARCH=arm64 \
    CROSS_COMPILE=aarch64-linux-gnu-
```

The generated kernel image is:

```text
arch/arm64/boot/Image
```

### 5. Create the root filesystem using BusyBox

BusyBox provides essential Linux user-space utilities and a shell.

A minimal root filesystem is prepared under:

```text
~/embedded-lab/rootfs/
```

The root filesystem contains:

- BusyBox executable
- Command symlinks
- `/init` startup script
- Basic directory structure

The root filesystem is packaged as a compressed CPIO archive:

```text
rootfs.cpio.gz
```

This archive is passed to QEMU as the initial RAM filesystem (initramfs).

### 6. Boot Linux using QEMU

Run the following command:

```bash
cd ~/embedded-lab/linux/kernel-6.6/arch/arm64/boot

qemu-system-aarch64 \
    -machine virt \
    -cpu cortex-a57 \
    -m 512M \
    -kernel Image \
    -initrd ~/embedded-lab/rootfs.cpio.gz \
    -append "console=ttyAMA0" \
    -nographic
```

### 7. Verify Linux boot

After successful boot, the Linux kernel starts and mounts the initramfs.

The BusyBox shell provides a minimal user-space environment.

Useful commands inside QEMU:

```bash
uname -a
uname -m
cat /proc/cpuinfo
cat /proc/meminfo
ls /
```

Expected architecture:

```text
aarch64
```

To exit QEMU, use:

```text
Ctrl + A
X
```

## Current Progress

| Area | Status |
|---|---|
| Ubuntu development environment | Completed |
| QEMU ARM64 configuration | Completed |
| Linux 6.6 kernel build | Completed |
| BusyBox root filesystem | Completed |
| ARM64 Linux boot using QEMU | Completed |
| Kernel module development | In progress |
| Character device driver | Planned |
| Platform device driver | Planned |
| I2C driver | Planned |
| SPI driver | Planned |
| Interrupt handling | Planned |
| FreeRTOS experiments | Planned |

## Future Development

The next stages of this project will focus on:

1. Linux kernel module development.
2. Character device driver implementation.
3. Linux device model and platform drivers.
4. Device tree concepts and bindings.
5. I2C and SPI driver development.
6. Interrupt handling and synchronization.
7. Memory-mapped register access.
8. RTOS and bare-metal firmware experiments.

## Learning Approach

The focus of this repository is practical implementation, debugging, documentation, and understanding how software interacts with hardware.

All experiments and implementations will be documented as the project progresses.